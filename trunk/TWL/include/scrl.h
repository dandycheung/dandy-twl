#ifndef __SCRL_H__
#define __SCRL_H__

#pragma once

#include "mpx.h"

#ifndef UINT_MAX
#define UINT_MAX                   0xffffffff
#endif // UINT_MAX
#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES    0x0068
#endif // SPI_GETWHEELSCROLLLINES
#ifndef WHEEL_DELTA
#define WHEEL_DELTA                120
#endif // WHEEL_DELTA
#ifndef WHEEL_PAGESCROLL
#define WHEEL_PAGESCROLL           (UINT_MAX)
#endif // WHEEL_PAGESCROLL

class CScrollImpl
{
protected:
	HWND m_hwnd;
	SIZE m_sizPage;
	SIZE m_sizDisplay;
	SIZE m_sizScrollPos;

public:
	CScrollImpl()
	{
		m_hwnd = NULL;
		m_sizPage.cx = 0;
		m_sizPage.cy = 0;
		m_sizDisplay.cx = 0;
		m_sizDisplay.cy = 0;
		m_sizScrollPos.cx = 0;
		m_sizScrollPos.cy = 0;
	}

	~CScrollImpl()
	{
		Detach();
	}

	// Attach/detach a window.
	void Attach(HWND hwnd)
	{
		m_hwnd = hwnd;
	}

	void Detach()
	{
		m_hwnd = NULL;
	}

	// Set/get the virtual display size. When the dialog or window
	// size is smaller than the display size, then that is when
	// scrollbars will appear. Set either the display width or display
	// height to zero if you don't want to enable the scrollbar in the
	// corresponding direction.
	void SetDisplaySize(int iWidth, int iHeight)
	{
		m_sizDisplay.cx = iWidth;
		m_sizDisplay.cy = iHeight;

		if(m_hwnd != NULL && ::IsWindow(m_hwnd))
			UpdateScrollInfo();
	}

	const SIZE& GetDisplaySize() const
	{
		return m_sizDisplay;
	}

	// Get current scroll position. This is needed if you are scrolling
	// a custom CWnd which implements its own drawing in OnPaint().
	const SIZE& GetScrollPos() const
	{
		return m_sizScrollPos;
	}

	// Get current page size. Useful for debugging purposes.
	const SIZE& GetPageSize() const
	{
		return m_sizPage;
	}

	// Scroll back to top, left, or top-left corner of the window.
	void ScrollToOrigin(bool bScrollLeft, bool bScrollTop)
	{
		if(m_hwnd == NULL)
			return;

		if(bScrollLeft)
		{
			if(m_sizDisplay.cx > 0 && m_sizPage.cx > 0 && m_sizScrollPos.cx > 0)
			{
				int iDeltaPos = -m_sizScrollPos.cx;
				m_sizScrollPos.cx += iDeltaPos;
				SetScrollPos(SB_HORZ, m_sizScrollPos.cx, TRUE);
				ScrollWindow(-iDeltaPos, 0);
			}
		}

		if(bScrollTop)
		{
			if(m_sizDisplay.cy > 0 && m_sizPage.cy > 0 && m_sizScrollPos.cy > 0)
			{
				int iDeltaPos = -m_sizScrollPos.cy;
				m_sizScrollPos.cy += iDeltaPos;
				SetScrollPos(SB_VERT, m_sizScrollPos.cy, TRUE);
				ScrollWindow(0, -iDeltaPos);
			}
		}
	}

	void SetScrollPos(int xPos, int yPos)
	{
		if(m_hwnd == NULL)
			return;

		if(xPos >= 0)
		{
			if(m_sizDisplay.cx > 0 && m_sizPage.cx > 0 && m_sizPage.cx - m_sizDisplay.cx > 0)
			{
				int iDelta = xPos - m_sizScrollPos.cx;
				m_sizScrollPos.cx += iDelta;
				SetScrollPos(SB_HORZ, m_sizScrollPos.cx, TRUE);
				ScrollWindow(-iDelta, 0);
			}
		}

		if(yPos >= 0)
		{
			if(m_sizDisplay.cy > 0 && m_sizPage.cy > 0 && m_sizPage.cy - m_sizDisplay.cy > 0)
			{
				int iDelta = yPos - m_sizScrollPos.cy;
				m_sizScrollPos.cy += iDelta;
				SetScrollPos(SB_VERT, m_sizScrollPos.cy, TRUE);
				ScrollWindow(0, -iDelta);
			}
		}
	}

	// Message handling.
	void OnHScroll(WPARAM wParam, LPARAM lParam)
	{
		MPX_WM_HSCROLL(wParam, lParam, uSBCode, uPos, hwndScrollBar);
		OnHScroll(uSBCode, uPos, hwndScrollBar);
	}

	void OnHScroll(UINT uSBCode, UINT uPos, HWND hwndScrollBar)
	{
		if(m_hwnd == NULL)
			return;

		const int iLineOffset = 60;

		// Compute the desired change or delta in scroll position.
		int iDeltaPos = 0;
		switch(uSBCode)
		{
		case SB_LINELEFT:
			// Left scroll arrow was pressed.
			iDeltaPos = -iLineOffset;
			break;

		case SB_LINERIGHT:
			// Right scroll arrow was pressed.
			iDeltaPos = iLineOffset;
			break;

		case SB_PAGELEFT:
			// User clicked inbetween left arrow and thumb.
			iDeltaPos = -m_sizPage.cx;
			break;

		case SB_PAGERIGHT:
			// User clicked inbetween thumb and right arrow.
			iDeltaPos = m_sizPage.cx;
			break;

		case SB_THUMBTRACK:
			// Scrollbar thumb is being dragged.
			iDeltaPos = Get32BitScrollPos(SB_HORZ, hwndScrollBar) - m_sizScrollPos.cx;
			break;

		case SB_THUMBPOSITION:
			// Scrollbar thumb was released.
			iDeltaPos = Get32BitScrollPos(SB_HORZ, hwndScrollBar) - m_sizScrollPos.cx;
			break;

		default:
			// We don't process other scrollbar messages.
			return;
		}

		// Compute the new scroll position.
		int iScrollPos = m_sizScrollPos.cx + iDeltaPos;

		// If the new scroll position is negative, we adjust
		// deltaPos in order to scroll the window back to origin.
		if(iScrollPos < 0)
			iDeltaPos = -m_sizScrollPos.cx;

		// If the new scroll position is greater than the max scroll position,
		// we adjust deltaPos in order to scroll the window precisely to the
		// maximum position.
		int iMaxPos = m_sizDisplay.cx - m_sizPage.cx;
		if(iScrollPos > iMaxPos)
			iDeltaPos = iMaxPos - m_sizScrollPos.cx;

		// Scroll the window if needed.
		if(iDeltaPos != 0)
		{
			m_sizScrollPos.cx += iDeltaPos;
			SetScrollPos(SB_HORZ, m_sizScrollPos.cx, TRUE);
			ScrollWindow(-iDeltaPos, 0);
		}
	}

	void OnVScroll(WPARAM wParam, LPARAM lParam)
	{
		MPX_WM_VSCROLL(wParam, lParam, uSBCode, uPos, hwndScrollBar);
		OnVScroll(uSBCode, uPos, hwndScrollBar);
	}

	void OnVScroll(UINT uSBCode, UINT uPos, HWND hwndScrollBar)
	{
		if(m_hwnd == NULL)
			return;

		const int iLineOffset = 60;

		// Compute the desired change or delta in scroll position.
		int iDeltaPos = 0;
		switch(uSBCode)
		{
		case SB_LINEUP:
			// Up arrow button on scrollbar was pressed.
			iDeltaPos = -iLineOffset;
			break;

		case SB_LINEDOWN:
			// Down arrow button on scrollbar was pressed.
			iDeltaPos = iLineOffset;
			break;

		case SB_PAGEUP:
			// User clicked inbetween up arrow and thumb.
			iDeltaPos = -m_sizPage.cy;
			break;

		case SB_PAGEDOWN:
			// User clicked inbetween thumb and down arrow.
			iDeltaPos = m_sizPage.cy;
			break;

		case SB_THUMBTRACK:
			// Scrollbar thumb is being dragged.
			iDeltaPos = Get32BitScrollPos(SB_VERT, hwndScrollBar) - m_sizScrollPos.cy;
			break;

		case SB_THUMBPOSITION:
			// Scrollbar thumb was released.
			iDeltaPos = Get32BitScrollPos(SB_VERT, hwndScrollBar) - m_sizScrollPos.cy;
			break;

		default:
			// We don't process other scrollbar messages.
			return;
		}

		// Compute the new scroll position.
		int iScrollPos = m_sizScrollPos.cy + iDeltaPos;

		// If the new scroll position is negative, we adjust
		// deltaPos in order to scroll the window back to origin.
		if(iScrollPos < 0)
			iDeltaPos = -m_sizScrollPos.cy;

		// If the new scroll position is greater than the max scroll position,
		// we adjust deltaPos in order to scroll the window precisely to the
		// maximum position.
		int iMaxPos = m_sizDisplay.cy - m_sizPage.cy;
		if(iScrollPos > iMaxPos)
			iDeltaPos = iMaxPos - m_sizScrollPos.cy;

		// Scroll the window if needed.
		if(iDeltaPos != 0)
		{
			m_sizScrollPos.cy += iDeltaPos;
			SetScrollPos(SB_VERT, m_sizScrollPos.cy, TRUE);
			ScrollWindow(0, -iDeltaPos);
		}
	}

	BOOL OnMouseWheel(WPARAM wParam, LPARAM lParam)
	{
		MPX_WM_MOUSEWHEEL(wParam, lParam, uFlags, zDelta, x, y);
		POINT pt = { x, y };
		return OnMouseWheel(uFlags, zDelta, pt);
	}

	BOOL OnMouseWheel(UINT uFlags, short zDelta, POINT pt)
	{
		if(m_hwnd == NULL)
			return FALSE;

		// MPX_WM_MOUSEWHEEL();

		// Don't do anything if the vertical scrollbar is not enabled.
		int iMin = 0, iMax = 0;
		GetScrollRange(SB_VERT, &iMin, &iMax);

		if(iMin == iMax)
			return FALSE;

		// Compute the number of scrolling increments requested.
		int iScrollIncrements = abs(zDelta) / WHEEL_DELTA;

		// Each scrolling increment corresponds to a certain number of
		// scroll lines (one scroll line is like a SB_LINEUP or SB_LINEDOWN).
		// We need to query the system parameters for this value.
		int iScrollLinesPerIncrement = 3;	// the reasonable default value
		::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &iScrollLinesPerIncrement, 0);

		// Check if a page scroll was requested.
		if(iScrollLinesPerIncrement == WHEEL_PAGESCROLL)
		{
			// Call the vscroll message handler to do the work.
			OnVScroll(zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN, 0, NULL);
			return TRUE;
		}

		// Compute total number of lines to scroll.
		int iScrollLines = iScrollIncrements * iScrollLinesPerIncrement;

		// Adjust numScrollLines to slow down the scrolling a bit more.
		iScrollLines = max(iScrollLines / 3, 1);

		// Do the scrolling.
		for(int i=0; i<iScrollLines; ++i)
		{
			// Call the vscroll message handler to do the work.
			OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, NULL);
		}

		return TRUE;
	}

	void OnSize(WPARAM wParam, LPARAM lParam)
	{
		MPX_WM_SIZE(wParam, lParam, uType, cx, cy);
		OnSize(uType, cx, cy);
	}

	void OnSize(UINT uType, int cx, int cy)
	{
		UpdateScrollInfo();
	}

protected:
	int Get32BitScrollPos(int iBar, HWND hwndScrollBar)
	{
		// Code below is from MSDN Article ID 152252, "How To Get
		// 32-bit Scroll Position During Scroll Messages".

		// First determine if the user scrolled a scroll bar control
		// on the window or scrolled the window itself.

		ASSERT(m_hwnd != NULL);

		HWND hwndScroll;
		if(hwndScrollBar == NULL)
			hwndScroll = m_hwnd;
		else
			hwndScroll = hwndScrollBar;

		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;
		::GetScrollInfo(hwndScroll, iBar, &si);

		return si.nTrackPos;
	}

	void UpdateScrollInfo()
	{
		if(m_hwnd == NULL)
			return;

		// Get the width/height of the attached window that includes the area
		// covered by the scrollbars (if any). The reason we need this is
		// because when scrollbars are present, both cx/cy and GetClientRect()
		// when accessed from OnSize() do not include the scrollbar covered
		// areas. In other words, their values are smaller than what you would
		// expect.
		RECT rc;
		GetClientRectSB(m_hwnd, &rc);

		SIZE sizWindow = { rc.right - rc.left, rc.bottom - rc.top };

		// Update horizontal scrollbar.
		int cxDelta = 0;
		int cyDelta = 0;
		UpdateScrollBar(SB_HORZ, sizWindow.cx, m_sizDisplay.cx,
			m_sizPage.cx, m_sizScrollPos.cx, cxDelta);

		// Update vertical scrollbar.
		UpdateScrollBar(SB_VERT, sizWindow.cy, m_sizDisplay.cy,
			m_sizPage.cy, m_sizScrollPos.cy, cyDelta);

		// See if we need to scroll the window back in place.
		// This is needed to handle the case where the scrollbar is
		// moved all the way to the right for example, and controls
		// at the left side disappear from the view. Then the user
		// resizes the window wider until scrollbars disappear. Without
		// this code below, the controls off the page will be gone forever.
		if(cxDelta != 0 || cyDelta != 0)
			ScrollWindow(cxDelta, cyDelta);
	}

	void UpdateScrollBar(int iBar, int iWindowSize, int iDisplaySize,
						   LONG& pageSize, LONG& scrollPos, int& iDelta)
	{
		int iScrollMax = 0;
		iDelta = 0;
		if(iWindowSize < iDisplaySize)
		{
			iScrollMax = iDisplaySize - 1;
			if(pageSize > 0 && scrollPos > 0)
			{
				// Adjust the scroll position when the window size is changed.
				scrollPos = (LONG)(1.0 * scrollPos * iWindowSize / pageSize);
			}

			pageSize = iWindowSize;
			scrollPos = min(scrollPos, iDisplaySize - pageSize - 1);
			iDelta = GetScrollPos(iBar) - scrollPos;
		}
		else
		{
			// Force the scrollbar to go away.
			pageSize = 0;
			scrollPos = 0;
			iDelta = GetScrollPos(iBar);
		}

		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask  = SIF_ALL;	// SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
		si.nMin   = 0;
		si.nMax   = iScrollMax;
		si.nPage  = pageSize;
		si.nPos   = scrollPos;
		SetScrollInfo(iBar, &si, TRUE);
	}

protected:
	// Helper function to get client rect with possible
	// modification by adding scrollbar width/height.
	static void GetClientRectSB(HWND hwnd, PRECT prc)
	{
		ASSERT(hwnd != NULL);
		ASSERT(prc != NULL);

		if(hwnd == NULL || prc == NULL)
			return;

		RECT rc;
		::GetWindowRect(hwnd, &rc);
		::MapWindowPoints(NULL, hwnd, (LPPOINT)&rc, 2);

		::GetClientRect(hwnd, prc);

		int cxSB = ::GetSystemMetrics(SM_CXVSCROLL);
		int cySB = ::GetSystemMetrics(SM_CYHSCROLL);

		if(rc.right >= (prc->right + cxSB))
			prc->right += cxSB;

		if(rc.bottom >= (prc->bottom + cySB))
			prc->bottom += cySB;
	}

protected:
	// MFC simulate
	virtual HWND GetScrollBarCtrl(int) const
	{
		return NULL;
	}

	int SetScrollPos(int iBar, int iPos, BOOL bRedraw)
	{
		HWND hwndScrollBar;
		if((hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
			return ::SetScrollPos(hwndScrollBar, SB_CTL, iPos, bRedraw);
		else
			return ::SetScrollPos(m_hwnd, iBar, iPos, bRedraw);
	}

	int GetScrollPos(int iBar) const
	{
		HWND hwndScrollBar;
		if((hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
			return ::GetScrollPos(hwndScrollBar, SB_CTL);
		else
			return ::GetScrollPos(m_hwnd, iBar);
	}

	void SetScrollRange(int iBar, int iMinPos, int iMaxPos, BOOL bRedraw)
	{
		HWND hwndScrollBar;
		if((hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
			::SetScrollRange(hwndScrollBar, SB_CTL, iMinPos, iMaxPos, bRedraw);
		else
			::SetScrollRange(m_hwnd, iBar, iMinPos, iMaxPos, bRedraw);
	}

	void GetScrollRange(int iBar, LPINT piMinPos, LPINT piMaxPos) const
	{
		HWND hwndScrollBar;
		if((hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
			::GetScrollRange(hwndScrollBar, SB_CTL, piMinPos, piMaxPos);
		else
			::GetScrollRange(m_hwnd, iBar, piMinPos, piMaxPos);
	}

	BOOL SetScrollInfo(int iBar, LPSCROLLINFO psi, BOOL bRedraw)
	{
		ASSERT(psi != NULL);

		HWND hwnd = m_hwnd;
		HWND hwndScrollBar;
		if(iBar != SB_CTL && (hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
		{
			hwnd = hwndScrollBar;
			iBar = SB_CTL;
		}

		psi->cbSize = sizeof(*psi);
		::SetScrollInfo(hwnd, iBar, psi, bRedraw);

		return TRUE;
	}

	BOOL GetScrollInfo(int iBar, LPSCROLLINFO psi, UINT uMask)
	{
		ASSERT(psi != NULL);

		HWND hWnd = m_hwnd;
		HWND hwndScrollBar;
		if(iBar != SB_CTL && (hwndScrollBar = GetScrollBarCtrl(iBar)) != NULL)
		{
			hWnd = hwndScrollBar;
			iBar = SB_CTL;
		}

		psi->cbSize = sizeof(*psi);
		psi->fMask = uMask;

		return ::GetScrollInfo(hWnd, iBar, psi);
	}

	int GetScrollLimit(int iBar)
	{
		int iMin, iMax;
		GetScrollRange(iBar, &iMin, &iMax);

		SCROLLINFO info;
		if(GetScrollInfo(iBar, &info, SIF_PAGE))
		{
			iMax -= __max(info.nPage - 1, 0);
		}

		return iMax;
	}

	void ScrollWindow(int xAmount, int yAmount, LPCRECT prc = NULL, LPCRECT prcClip = NULL)
	{
		ASSERT(::IsWindow(m_hwnd));

		if(::IsWindowVisible(m_hwnd) || prc != NULL || prcClip != NULL)
		{
			// When visible, let Windows do the scrolling
			::ScrollWindow(m_hwnd, xAmount, yAmount, prc, prcClip);
		}
		else
		{
			// Windows does not perform any scrolling if the window is
			// not visible.  This leaves child windows unscrolled.
			// To account for this oversight, the child windows are moved
			// directly instead.

			HWND hwndChild = ::GetWindow(m_hwnd, GW_CHILD);
			if(hwndChild != NULL)
			{
				for(; hwndChild != NULL; hwndChild = ::GetNextWindow(hwndChild, GW_HWNDNEXT))
				{
					RECT rc;
					::GetWindowRect(hwndChild, &rc);
					::MapWindowPoints(NULL, m_hwnd, (LPPOINT)&rc, 2);
					::SetWindowPos(hwndChild, NULL, rc.left + xAmount, rc.top + yAmount, 0, 0,
						SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
				}
			}
		}
	}
};

#endif // __SCRL_H__
