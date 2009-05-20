#include <windows.h>
#include "tme.h"

#define SetSystemTimer	SetTimer
#define KillSystemTimer	KillTimer

#ifndef VK_XBUTTON1
#define VK_XBUTTON1		0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2		0x06    /* NOT contiguous with L & RBUTTON */
#define MK_XBUTTON1		0x0020
#define MK_XBUTTON2		0x0040
#endif // VK_XBUTTON1

#define WARNING			sizeof

typedef struct __TRACKINGLIST
{
    TRACKMOUSEEVENT tme;
    POINT pt; // center of hover rectangle
} _TRACKINGLIST;

// FIXME: move tracking stuff into a per thread data
static _TRACKINGLIST g_trackingInfo;
static UINT_PTR g_trackingTimer;

static WORD GetButtonState()
{
    WORD wRet = 0;
	
    if(GetSystemMetrics(SM_SWAPBUTTON))
    {
        if(GetKeyState(VK_RBUTTON) < 0)
			wRet |= MK_LBUTTON;

        if(GetKeyState(VK_LBUTTON) < 0)
			wRet |= MK_RBUTTON;
    }
    else
    {
        if(GetKeyState(VK_LBUTTON) < 0)
			wRet |= MK_LBUTTON;

        if(GetKeyState(VK_RBUTTON) < 0)
			wRet |= MK_RBUTTON;
    }

    if(GetKeyState(VK_MBUTTON) < 0)
		wRet |= MK_MBUTTON;

    if(GetKeyState(VK_SHIFT) < 0)
		wRet |= MK_SHIFT;

    if(GetKeyState(VK_CONTROL) < 0)
		wRet |= MK_CONTROL;

    if(GetKeyState(VK_XBUTTON1) < 0)
		wRet |= MK_XBUTTON1;

    if(GetKeyState(VK_XBUTTON2) < 0)
		wRet |= MK_XBUTTON2;

    return wRet;
}

static HWND WindowFromPointEx(POINT pt, PINT piHitCode)
{
	HWND hwnd = WindowFromPoint(pt);
	if(!hwnd)
	{
		if(piHitCode)
			*piHitCode = HTNOWHERE;

		return NULL;
	}

	if(piHitCode)
		*piHitCode = SendMessage(hwnd, WM_NCHITTEST, 0, MAKELPARAM((SHORT)pt.x, (SHORT)pt.y));

	return hwnd;
}

static void CheckMouseLeave(HWND hwnd, int hittest)
{
    if(g_trackingInfo.tme.hwndTrack != hwnd)
    {
        if(g_trackingInfo.tme.dwFlags & TME_NONCLIENT)
            PostMessage(g_trackingInfo.tme.hwndTrack, WM_NCMOUSELEAVE, 0, 0);
        else
            PostMessage(g_trackingInfo.tme.hwndTrack, WM_MOUSELEAVE, 0, 0);

        // remove the TME_LEAVE flag
        g_trackingInfo.tme.dwFlags &= ~TME_LEAVE;
    }
    else
    {
        if(hittest == HTCLIENT)
        {
            if(g_trackingInfo.tme.dwFlags & TME_NONCLIENT)
            {
                PostMessage(g_trackingInfo.tme.hwndTrack, WM_NCMOUSELEAVE, 0, 0);

                // remove the TME_LEAVE flag
                g_trackingInfo.tme.dwFlags &= ~TME_LEAVE;
            }
        }
        else
        {
            if(!(g_trackingInfo.tme.dwFlags & TME_NONCLIENT))
            {
                PostMessage(g_trackingInfo.tme.hwndTrack, WM_MOUSELEAVE, 0, 0);

                // remove the TME_LEAVE flag
                g_trackingInfo.tme.dwFlags &= ~TME_LEAVE;
            }
        }
    }
}

static void CALLBACK TrackMouseEventProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT pt;
    INT iHoverWidth = 0, iHoverHeight = 0;
	INT hittest;

    GetCursorPos(&pt);
    hwnd = WindowFromPointEx(pt, &hittest);

    SystemParametersInfo(SPI_GETMOUSEHOVERWIDTH, 0, &iHoverWidth, 0);
    SystemParametersInfo(SPI_GETMOUSEHOVERHEIGHT, 0, &iHoverHeight, 0);

    // see if this tracking event is looking for TME_LEAVE and that the mouse has left the window
    if(g_trackingInfo.tme.dwFlags & TME_LEAVE)
        CheckMouseLeave(hwnd, hittest);

	// mouse is gone, stop tracking mouse hover
    if(g_trackingInfo.tme.hwndTrack != hwnd)
        g_trackingInfo.tme.dwFlags &= ~TME_HOVER;

    // see if we are tracking hovering for this hwnd
    if(g_trackingInfo.tme.dwFlags & TME_HOVER)
    {
        // has the cursor moved outside the rectangle centered around pt?
        if((abs(pt.x - g_trackingInfo.pt.x) > (iHoverWidth / 2)) ||
            (abs(pt.y - g_trackingInfo.pt.y) > (iHoverHeight / 2)))
        {
            // record this new position as the current position
            g_trackingInfo.pt = pt;
        }
        else
        {
            if(hittest == HTCLIENT)
            {
                ScreenToClient(hwnd, &pt);

                PostMessage(g_trackingInfo.tme.hwndTrack, WM_MOUSEHOVER, GetButtonState(), MAKELPARAM(pt.x, pt.y));
            }
            else
            {
                if(g_trackingInfo.tme.dwFlags & TME_NONCLIENT)
                    PostMessage(g_trackingInfo.tme.hwndTrack, WM_NCMOUSEHOVER, hittest, MAKELPARAM(pt.x, pt.y));
            }

            // stop tracking mouse hover
            g_trackingInfo.tme.dwFlags &= ~TME_HOVER;
        }
    }

    // stop the g_trackingTimer if the tracking list is empty
    if(!(g_trackingInfo.tme.dwFlags & (TME_HOVER | TME_LEAVE)))
    {
        KillSystemTimer(g_trackingInfo.tme.hwndTrack, g_trackingTimer);

        g_trackingTimer = 0;
        g_trackingInfo.tme.hwndTrack = 0;
        g_trackingInfo.tme.dwFlags = 0;
        g_trackingInfo.tme.dwHoverTime = 0;
    }
}


/*
 * During mouse tracking WM_MOUSEHOVER or WM_MOUSELEAVE events are posted
 * to the hwnd specified in the ptme structure.  After the event message
 * is posted to the hwnd, the entry in the queue is removed.
 *
 * If the current hwnd isn't ptme->hwndTrack the TME_HOVER flag is completely
 * ignored. The TME_LEAVE flag results in a WM_MOUSELEAVE message being posted
 * immediately and the TME_LEAVE flag being ignored.
 *
 */
BOOL WINAPI TrackMouseEvent(TRACKMOUSEEVENT* ptme)
{
    HWND hwnd;
    POINT pt;
    DWORD dwHoverTime;
    INT hittest;

    if(ptme->cbSize != sizeof(TRACKMOUSEEVENT))
	{
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    // fill the TRACKMOUSEEVENT struct with the current tracking for the given hwnd
    if(ptme->dwFlags & TME_QUERY)
    {
        *ptme = g_trackingInfo.tme;

        // set cbSize in the case it's not initialized yet
        ptme->cbSize = sizeof(TRACKMOUSEEVENT);

        return TRUE; // return here, TME_QUERY is retrieving information
    }

    if(!IsWindow(ptme->hwndTrack))
    {
        SetLastError(ERROR_INVALID_WINDOW_HANDLE);
        return FALSE;
    }

    dwHoverTime = ptme->dwHoverTime;

    // if HOVER_DEFAULT was specified replace this with the systems current value.
    // TME_LEAVE doesn't need to specify hover time so use default
    if(dwHoverTime == HOVER_DEFAULT || dwHoverTime == 0 || !(ptme->dwHoverTime & TME_HOVER))
        SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &dwHoverTime, 0);

    GetCursorPos(&pt);
    hwnd = WindowFromPointEx(pt, &hittest);

    if(ptme->dwFlags & ~(TME_CANCEL | TME_HOVER | TME_LEAVE | TME_NONCLIENT))
        WARNING("Unknown flag(s) %08x\n", ptme->dwFlags & ~(TME_CANCEL | TME_HOVER | TME_LEAVE | TME_NONCLIENT));

    if(ptme->dwFlags & TME_CANCEL)
    {
        if(g_trackingInfo.tme.hwndTrack == ptme->hwndTrack)
        {
            g_trackingInfo.tme.dwFlags &= ~(ptme->dwFlags & ~TME_CANCEL);

            // if we aren't tracking on hover or leave remove this entry
            if(!(g_trackingInfo.tme.dwFlags & (TME_HOVER | TME_LEAVE)))
            {
                KillSystemTimer(g_trackingInfo.tme.hwndTrack, g_trackingTimer);

                g_trackingTimer = 0;
                g_trackingInfo.tme.hwndTrack = 0;
                g_trackingInfo.tme.dwFlags = 0;
                g_trackingInfo.tme.dwHoverTime = 0;
            }
        }
    }
	else
	{
        // In our implementation it's possible that another window will receive a
        // WM_MOUSEMOVE and call TrackMouseEvent before TrackMouseEventProc is
        // called. In such a situation post the WM_MOUSELEAVE now
        if(g_trackingInfo.tme.dwFlags & TME_LEAVE && g_trackingInfo.tme.hwndTrack != NULL)
            CheckMouseLeave(hwnd, hittest);

        if(g_trackingTimer)
        {
            KillSystemTimer(g_trackingInfo.tme.hwndTrack, g_trackingTimer);

            g_trackingTimer = 0;
            g_trackingInfo.tme.hwndTrack = 0;
            g_trackingInfo.tme.dwFlags = 0;
            g_trackingInfo.tme.dwHoverTime = 0;
        }

        if(ptme->hwndTrack == hwnd)
        {
            // Adding new mouse event to the tracking list
            g_trackingInfo.tme = *ptme;
            g_trackingInfo.tme.dwHoverTime = dwHoverTime;

            // Initialize HoverInfo variables even if not hover tracking
            g_trackingInfo.pt = pt;

            g_trackingTimer = SetSystemTimer(g_trackingInfo.tme.hwndTrack, (UINT_PTR)&g_trackingInfo.tme, dwHoverTime, TrackMouseEventProc);
        }
    }

    return TRUE;
}
