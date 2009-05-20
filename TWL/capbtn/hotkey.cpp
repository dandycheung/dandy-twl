#include <windows.h>
#include <windowsx.h>

#include "themedborder.h"

// copied from commctrl.h
#define HOTKEYF_SHIFT           0x01
#define HOTKEYF_CONTROL         0x02
#define HOTKEYF_ALT             0x04
#ifdef _MAC
#define HOTKEYF_EXT             0x80
#else
#define HOTKEYF_EXT             0x08
#endif

#define HKCOMB_NONE             0x0001
#define HKCOMB_S                0x0002
#define HKCOMB_C                0x0004
#define HKCOMB_A                0x0008
#define HKCOMB_SC               0x0010
#define HKCOMB_SA               0x0020
#define HKCOMB_CA               0x0040
#define HKCOMB_SCA              0x0080

#define HKM_SETHOTKEY           (WM_USER+1)
#define HKM_GETHOTKEY           (WM_USER+2)
#define HKM_SETRULES            (WM_USER+3)

#ifndef EVENT_OBJECT_VALUECHANGE
#define EVENT_OBJECT_VALUECHANGE 0x800E
#endif

#ifndef OBJID_CLIENT
#define OBJID_CLIENT ((LONG)0xFFFFFFFC)
#endif

#ifndef WM_GETOBJECT
#define WM_GETOBJECT 0x003D
#endif

#ifndef OBJID_QUERYCLASSNAMEIDX
#define OBJID_QUERYCLASSNAMEIDX ((LONG)0xFFFFFFF4)
#endif

////////////////////////////////////////////////////////////////////////////////

static void InitGlobalStuff();
static BOOL MGetTextExtent(HDC hdc, LPCTSTR pszText, int iCount, int* pcx, int* pcy);
static DWORD SetWindowBits(HWND hWnd, int iWhich, DWORD dwBits, DWORD dwValue);
static void MyNotifyWinEvent(UINT event, HWND hwnd, LONG idContainer, LONG idChild);

static void SetHotKey(HWND hwnd, WORD wVirKey, WORD wMods, BOOL fSendNotify);
static void GetKeyName(UINT uVirKey, LPTSTR pszKeyName, BOOL fExt);
static void PaintHotKey(register HWND hwnd);
static void HKMSetRules(HWND hwnd, WPARAM wParam, LPARAM lParam);
static HFONT HKMSetFont(HWND hwnd, HFONT hfnt);

////////////////////////////////////////////////////////////////////////////////

#define MSAA_CLASSNAMEIDX_BASE       65536L
#define MSAA_CLASSNAMEIDX_HOTKEY     (MSAA_CLASSNAMEIDX_BASE + 16)

static int g_cxBorder;
static int g_cyBorder;

static COLORREF g_clrWindow;
static COLORREF g_clrWindowText;
static COLORREF g_clrGrayText;
static COLORREF g_clrBtnFace;

static HBRUSH g_hbrWindow;
static HBRUSH g_hbrBtnFace;

static HFONT g_hfntSystem;

////////////////////////////////////////////////////////////////////////////////

#define F_EXT               0x01000000L

#define GWU_VIRTKEY         (0)
#define GWU_MODS            (1 * sizeof(ULONG))
#define GWU_INVALID         (2 * sizeof(ULONG))
#define GWU_DEFAULT         (3 * sizeof(ULONG))
#define GWU_HFONT           (4 * sizeof(ULONG))
#define GWU_YFONT           (5 * sizeof(ULONG))
#define NUM_WND_EXTRA       (GWU_YFONT + sizeof(ULONG))

const TCHAR g_szHotkeyClass[] = TEXT("DandyHotKey");

static LRESULT CALLBACK HotKeyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static CEditThemedBorder border;

BOOL InitHotKeyClass(HINSTANCE hInstance)
{
    WNDCLASS wc = { 0 };
    if(!GetClassInfo(hInstance, g_szHotkeyClass, &wc))
    {
        wc.lpfnWndProc   = HotKeyWndProc;
        wc.lpszClassName = g_szHotkeyClass;
        wc.style         = CS_GLOBALCLASS;
        wc.hInstance     = hInstance;
        wc.cbWndExtra    = NUM_WND_EXTRA;

        if(!RegisterClass(&wc))
            return FALSE;
    }

    return TRUE;
}

const UINT s_Combos[8] =
{
    HKCOMB_NONE,
    HKCOMB_S,
    HKCOMB_C,
    HKCOMB_SC,
    HKCOMB_A,
    HKCOMB_SA,
    HKCOMB_CA,
    HKCOMB_SCA
};

void SetHotKey(HWND hwnd, WORD wVirKey, WORD wMods, BOOL fSendNotify)
{
    // don't invalidate if it's the same
    if(wVirKey == GetWindowLong(hwnd, GWU_VIRTKEY) &&
        wMods == GetWindowLong(hwnd, GWU_MODS))
        return;

    SetWindowLong(hwnd, GWU_VIRTKEY, wVirKey);
    SetWindowLong(hwnd, GWU_MODS, wMods);
    InvalidateRect(hwnd, NULL, TRUE);

    if(fSendNotify)
	{
        FORWARD_WM_COMMAND(GetParent(hwnd), GetDlgCtrlID(hwnd), hwnd, EN_CHANGE, SendMessage);
    }

    MyNotifyWinEvent(EVENT_OBJECT_VALUECHANGE, hwnd, OBJID_CLIENT, 0);
}

void GetKeyName(UINT uVirKey, LPTSTR pszKeyName, BOOL fExt)
{
    LONG lScan = (LONG)MapVirtualKey(uVirKey, 0) << 16;
    if(fExt)
        lScan |= F_EXT;

    GetKeyNameText(lScan, pszKeyName, 50);
}

void PaintHotKey(register HWND hwnd)
{
    TCHAR szPlus[10] = TEXT("\0");
    lstrcat(szPlus, TEXT("+"));				// localized character needed?

    int cch = 0;
    TCHAR szText[128] = TEXT("\0");

    UINT wVirKey = (UINT)GetWindowLong(hwnd, GWU_VIRTKEY);
    UINT wMods = (UINT)GetWindowLong(hwnd, GWU_MODS);
    if(!wVirKey && !wMods)
	{
        lstrcat(szPlus, TEXT("(None)"));	// localized string needed?
	}
    else
    {
        if(wMods & HOTKEYF_CONTROL)
        {
            GetKeyName(VK_CONTROL, szText, FALSE);
            lstrcat(szText, szPlus);
        }

        if(wMods & HOTKEYF_SHIFT)
        {
            GetKeyName(VK_SHIFT, szText + lstrlen(szText), FALSE);
            lstrcat(szText, szPlus);
        }
        if(wMods & HOTKEYF_ALT)
        {
            GetKeyName(VK_MENU, szText + lstrlen(szText), FALSE);
            lstrcat(szText, szPlus);
        }

        GetKeyName(wVirKey, szText + lstrlen(szText), wMods & HOTKEYF_EXT);
    }

    cch = lstrlen(szText);

	// prepare the painting

    HideCaret(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);

	// painting begin

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    HGDIOBJ hfntOld = SelectObject(hdc, (HFONT)GetWindowLong(hwnd, GWU_HFONT));

    int x = g_cxBorder;
    int y = g_cyBorder;

    if(IsWindowEnabled(hwnd))
    {
        SetBkColor(hdc, g_clrWindow);
        SetTextColor(hdc, g_clrWindowText);
        TextOut(hdc, x, y, szText, cch);
    }
    else
    {
        // set the background color to Grayed like edit controls
        SetBkColor(hdc, g_clrBtnFace);
        if(g_clrGrayText)
        {
            SetTextColor(hdc, g_clrGrayText);
            TextOut(hdc, x, y, szText, cch);
        }
        else
        {
            GrayString(hdc, NULL, NULL, (LPARAM)(LPTSTR)szText, cch, x, y, 0, 0);
        }
    }

    MGetTextExtent(hdc, szText, cch, &x, NULL);
    if(GetFocus() == hwnd)
         SetCaretPos(x + g_cxBorder, g_cyBorder);

    SelectObject(hdc, hfntOld);
    EndPaint(hwnd, &ps);

    ShowCaret(hwnd);
}

void HKMSetRules(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    SetWindowLong(hwnd, GWU_INVALID, wParam);
    SetWindowLong(hwnd, GWU_DEFAULT, lParam);
}

HFONT HKMSetFont(HWND hwnd, HFONT hfnt)
{
    HFONT hfntOld = (HFONT)GetWindowLong(hwnd, GWU_HFONT);
    SetWindowLong(hwnd, GWU_HFONT, (LONG)hfnt);

    HDC hdc = GetDC(hwnd);
    if(hfnt)
        hfnt = (HFONT)SelectObject(hdc, hfnt);

    INT cy;
    MGetTextExtent(hdc, TEXT("C"), 1, NULL, &cy);
    SetWindowLong(hwnd, GWU_YFONT, cy);

    if(hfnt)
        SelectObject(hdc, hfnt);

    ReleaseDC(hwnd, hdc);
    InvalidateRect(hwnd, NULL, TRUE);

    return hfntOld;
}

LRESULT CALLBACK HotKeyWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    WORD wVirKey;
    WORD wMods;
    RECT rc;
    HDC hdc;

    switch(wMsg)
    {
    case WM_NCCREATE:
        SetWindowBits(hwnd, GWL_EXSTYLE, WS_EX_CLIENTEDGE, WS_EX_CLIENTEDGE);
        InitGlobalStuff();
        return TRUE;

    case WM_CREATE:
        SetHotKey(hwnd, 0, 0, FALSE);
        HKMSetRules(hwnd, 0, 0);
        HKMSetFont(hwnd, g_hfntSystem);
        break;

    case WM_SETFOCUS:
        InvalidateRect(hwnd, NULL, TRUE);
        CreateCaret(hwnd, NULL, 0, (int)GetWindowLong(hwnd, GWU_YFONT));
        ShowCaret(hwnd);
        break;

    case WM_KILLFOCUS:
        if(!GetWindowLong(hwnd, GWU_VIRTKEY))
            SetHotKey(hwnd, 0, 0, TRUE);
        DestroyCaret();
        break;

    case WM_GETDLGCODE:
        return DLGC_WANTCHARS | DLGC_WANTARROWS;

    case HKM_SETHOTKEY:
        SetHotKey(hwnd, LOBYTE(wParam), HIBYTE(wParam), FALSE);
        break;

    case HKM_GETHOTKEY:
        return (256 * (BYTE)GetWindowLong(hwnd, GWU_MODS)) +
                ((BYTE)GetWindowLong(hwnd, GWU_VIRTKEY));
        break;

    case HKM_SETRULES:
        HKMSetRules(hwnd, wParam, LOWORD(lParam));
        break;

    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        break;

    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
        case VK_TAB:
        case VK_SPACE:
        case VK_DELETE:
        case VK_ESCAPE:
        case VK_BACK:
        case VK_LWIN:
        case VK_RWIN:
        case VK_APPS:
            SetHotKey(hwnd, 0, 0, TRUE);
            return DefWindowProc(hwnd, wMsg, wParam, lParam);

        case VK_MENU:
        case VK_SHIFT:
        case VK_CONTROL:
            wVirKey = 0;
            goto SetNewHotKey;

        default:
            wVirKey = (WORD)wParam;
SetNewHotKey:
            wMods = 0;
            if(GetKeyState(VK_CONTROL) < 0)
                wMods |= HOTKEYF_CONTROL;
            if(GetKeyState(VK_SHIFT) < 0)
                wMods |= HOTKEYF_SHIFT;
            if(GetKeyState(VK_MENU) < 0)
                wMods |= HOTKEYF_ALT;

#define IsFUNKEY(vk) ((vk) >= VK_F1 && (vk) <= VK_F24)
#define IsNUMKEY(vk) ((vk) >= VK_NUMPAD0 && (vk) <= VK_DIVIDE)

            //
            //  dont enforce any rules on the Function keys or
            //  on the number pad keys.
            //
            // if this combination is invalid, use the default
            if(!IsFUNKEY(wVirKey) && !IsNUMKEY(wVirKey) &&
                (s_Combos[wMods] & GetWindowLong(hwnd, GWU_INVALID)))
            {
                wMods = (WORD)GetWindowLong(hwnd, GWU_DEFAULT);
            }

            if(lParam & F_EXT)
                wMods |= HOTKEYF_EXT;

            SetHotKey(hwnd, wVirKey, wMods, TRUE);
            break;
        }
        break;

    case WM_SYSKEYUP:
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_KEYUP:
        if(!GetWindowLong(hwnd, GWU_VIRTKEY))
            SetHotKey(hwnd, 0, 0, TRUE);
        break;

    case WM_GETFONT:
        return GetWindowLong(hwnd, GWU_HFONT);

    case WM_SETFONT:
        return (LRESULT)(UINT)HKMSetFont(hwnd, (HFONT)wParam);

    case WM_PAINT:
        PaintHotKey(hwnd);
        break;

    case WM_ERASEBKGND:
        HideCaret(hwnd);
        hdc = GetDC(hwnd);
        GetClientRect(hwnd, &rc);

        if(IsWindowEnabled(hwnd))
            FillRect(hdc, &rc, g_hbrWindow);
		else
            FillRect(hdc, &rc, g_hbrBtnFace);

        ReleaseDC(hwnd, hdc);
        ShowCaret(hwnd);
        return TRUE;

    case WM_GETOBJECT:
        if(lParam == OBJID_QUERYCLASSNAMEIDX)
            return MSAA_CLASSNAMEIDX_HOTKEY;
        goto DoDefault;

    case WM_ENABLE:
        InvalidateRect(hwnd, NULL, TRUE);
        goto DoDefault;
		
	case WM_NCCALCSIZE:
		LRESULT lResult;
		if(border.OnNcCalcSize(hwnd, wMsg, wParam, lParam, lResult))
			return lResult;
		goto DoDefault;

	case WM_NCPAINT:
		if(border.OnNcPaint(hwnd, wMsg, wParam, lParam, lResult))
			return lResult;
		goto DoDefault;

    default:
DoDefault:
        return DefWindowProc(hwnd, wMsg, wParam, lParam);
    }

    return 0L;
}

////////////////////////////////////////////////////////////////////////////////

void InitGlobalStuff()
{
    g_cxBorder = GetSystemMetrics(SM_CXBORDER);
    g_cyBorder = GetSystemMetrics(SM_CYBORDER);

    g_clrWindow = GetSysColor(COLOR_WINDOW);
    g_clrWindowText = GetSysColor(COLOR_WINDOWTEXT);
    g_clrGrayText = GetSysColor(COLOR_GRAYTEXT);
    g_clrBtnFace = GetSysColor(COLOR_BTNFACE);

    g_hbrWindow = GetSysColorBrush(COLOR_WINDOW);
    g_hbrBtnFace = GetSysColorBrush(COLOR_BTNFACE);

    g_hfntSystem = (HFONT)GetStockObject(SYSTEM_FONT);
}

BOOL MGetTextExtent(HDC hdc, LPCTSTR pszText, int iCount, int* pcx, int* pcy)
{
    if(iCount == -1)
        iCount = lstrlen(pszText);

    SIZE size = { 0, 0 };
    BOOL fSuccess = GetTextExtentPoint(hdc, pszText, iCount, &size);

    if(pcx)
        *pcx = size.cx;
    if(pcy)
        *pcy = size.cy;

    return fSuccess;
}

//  Returns previous window bits.
DWORD SetWindowBits(HWND hWnd, int iWhich, DWORD dwBits, DWORD dwValue)
{
    DWORD dwStyle;
    DWORD dwNewStyle;

    dwStyle = GetWindowLong(hWnd, iWhich);
    dwNewStyle = (dwStyle & ~dwBits) | (dwValue & dwBits);
    if(dwStyle != dwNewStyle)
        dwStyle = SetWindowLong(hWnd, iWhich, dwNewStyle);

    return dwStyle;
}

typedef void (CALLBACK* NOTIFYWINEVENTPROC)(UINT, HWND, LONG, LONG);

#define DONOTHING_NOTIFYWINEVENT ((NOTIFYWINEVENTPROC)1)

// --------------------------------------------------------------------------
//
//  MyNotifyWinEvent()
//
//  This tries to get the proc address of NotifyWinEvent().  If it fails, we
//  remember that and do nothing.
//
//  NOTE TO NT FOLKS:
//  Don't freak out about this code.  It will do nothing on NT, nothing yet
//  that is.  Active Accessibility will be ported to NT for Service Pack #1
//  or at worst #2 after NT SUR ships, this code will work magically when
//  that is done/
//
// --------------------------------------------------------------------------
void MyNotifyWinEvent(UINT event, HWND hwnd, LONG idContainer, LONG idChild)
{
    static NOTIFYWINEVENTPROC fnNotifyWinEvent = NULL;

    if(!fnNotifyWinEvent)
    {
        HMODULE hmod;
        if(hmod = GetModuleHandle(TEXT("USER32")))
            fnNotifyWinEvent = (NOTIFYWINEVENTPROC)GetProcAddress(hmod, "NotifyWinEvent");

        if(!fnNotifyWinEvent)
            fnNotifyWinEvent = DONOTHING_NOTIFYWINEVENT;
    }

    if(fnNotifyWinEvent != DONOTHING_NOTIFYWINEVENT)
        fnNotifyWinEvent(event, hwnd, idContainer, idChild);
}
