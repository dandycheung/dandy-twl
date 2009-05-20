//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

//
// message parameters cracker
//

#ifndef __MPX_H__
#define __MPX_H__

#define MPX_WM_NULL(w, l)
// p = pcs
#define MPX_WM_CREATE(w, l, p)					LPCREATESTRUCT p;				\
												p = (LPCREATESTRUCT)l
// 
#define MPX_WM_DESTROY(w, l)
// x = x, y = y
#define MPX_WM_MOVE(w, l, x, y)					int x, y;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uType, x = cx, y = cy
#define MPX_WM_SIZE(w, l, u, x, y)				UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uState, b = bMinimized, h = hwndOther
#define MPX_WM_ACTIVATE(w, l, u, b, h)			UINT u;							\
												BOOL b;							\
												HWND h;							\
												u = (UINT)(short)LOWORD(w);		\
												b = (BOOL)(short)HIWORD(w);		\
												h = (HWND)l
// h = hwndOld
#define MPX_WM_SETFOCUS(w, l, h)				HWND h;							\
												h = (HWND)w
// h = hwndNew
#define MPX_WM_KILLFOCUS(w, l, h)				HWND h;							\
												h = (HWND)w
// b = bEnable
#define MPX_WM_ENABLE(w, l, b)					BOOL b;							\
												b = (BOOL)w
// b = bEnable
#define MPX_WM_SETREDRAW(w, l, b)				BOOL b;							\
												b = (BOOL)w
// p = pszText
#define MPX_WM_SETTEXT(w, l, p)					LPCTSTR p;						\
												p = (LPCTSTR)l
// c = cchSize, p = pszText
#define MPX_WM_GETTEXT(w, l, c, p)				UINT c;							\
												LPTSTR p;						\
												c = (UINT)w;					\
												p = (LPTSTR)l
#define MPX_WM_GETTEXTLENGTH(w, l)
#define MPX_WM_PAINT(w, l)
#define MPX_WM_CLOSE(w, l)
// u = uFlag
#define MPX_WM_QUERYENDSESSION(w, l, u)			UINT u;							\
												u = (UINT)l
// i = iExitCode
#define MPX_WM_QUIT(w, l, i)					int i;							\
												i = (int)w
#define MPX_WM_QUERYOPEN(w, l)
// h = hdc
#define MPX_WM_ERASEBKGND(w, l, h)				HDC h;							\
												h = (HDC)w
#define MPX_WM_SYSCOLORCHANGE(w, l)
// b = bEnding, u = uFlag
#define MPX_WM_ENDSESSION(w, l, b, u)			BOOL b;							\
												UINT u;							\
												b = (BOOL)w;					\
												u = (UINT)l
// b = bShow, u = uState
#define MPX_WM_SHOWWINDOW(w, l, b, u)			BOOL b;							\
												UINT u;							\
												b = (BOOL)w;					\
												u = (UINT)l
// p = pszParameter
#define MPX_WM_WININICHANGE(w, l, p)			LPCTSTR p;						\
												p = (LPCTSTR)l
// p = pszDevice
#define MPX_WM_DEVMODECHANGE(w, l, p)			LPCTSTR p;						\
												p = (LPCTSTR)l
// b = bActive, d = dwThreadId
#define MPX_WM_ACTIVATEAPP(w, l, b, d)			BOOL b;							\
												DWORD d;						\
												b = (BOOL)w;					\
												d = (DWORD)l
#define MPX_WM_FONTCHANGE(w, l)
#define MPX_WM_TIMECHANGE(w, l)
#define MPX_WM_CANCELMODE(w, l)
// h = hwndCursor, i = iHitCode, u = uMsg
#define MPX_WM_SETCURSOR(w, l, h, i, u)			HWND h;							\
												int i;							\
												UINT u;							\
												h = (HWND)w;					\
												i = (int)(short)LOWORD(l);		\
												u = (UINT)(short)HIWORD(l)
// h = hwndTopParent, i = iHitCode, u = uMsg
#define MPX_WM_MOUSEACTIVATE(w, l, h, i, u)		HWND h;							\
												int i;							\
												UINT u;							\
												h = (HWND)w;					\
												i = (int)(short)LOWORD(l);		\
												u = (UINT)(short)HIWORD(l)
#define MPX_WM_CHILDACTIVATE(w, l)
#define MPX_WM_QUEUESYNC(w, l)
// p = pmmi
#define MPX_WM_GETMINMAXINFO(w, l, p)			PMINMAXINFO p;					\
												p = (PMINMAXINFO)l
#define MPX_WM_PAINTICON(w, l)
// h = hdc
#define MPX_WM_ICONERASEBKGND(w, l, h)			HDC h;							\
												h = (HDC)w
// #define MPX_WM_NEXTDLGCTL(w, l)					/// TODO:
// u = uFlags, d = dwJobsNumber
#define MPX_WM_SPOOLERSTATUS(w, l, u, d)		UINT u;							\
												DWORD d;						\
												u = (UINT)w;					\
												d = (DWORD)l
// b = bMenuItem, i = iCtrlId, p = pdis
#define MPX_WM_DRAWITEM(w, l, b, i, p)			BOOL b;							\
												int i;							\
												PDRAWITEMSTRUCT p;				\
												b = !w;							\
												i = (int)w;						\
												p = (PDRAWITEMSTRUCT)l
// b = bMenuItem, i = iCtrlId, p = pmis
#define MPX_WM_MEASUREITEM(w, l, b, i, p)		BOOL b;							\
												int i;							\
												PMEASUREITEMSTRUCT p;			\
												b = !w;							\
												i = (int)w;						\
												p = (PMEASUREITEMSTRUCT)l
// i = iCtrlId, p = pdis
#define MPX_WM_DELETEITEM(w, l, i, p)			int i;							\
												PDELETEITEMSTRUCT p;			\
												i = (int)w;						\
												p = (PDELETEITEMSTRUCT)l
// u = uKey, n = nIndex, h = hwndList
#define MPX_WM_VKEYTOITEM(w, l, u, n, h)		UINT u, n;						\
												HWND h;							\
												u = (UINT)(short)LOWORD(w);		\
												n = (UINT)(short)HIWORD(w);		\
												h = (HWND)l
// u = uChar, n = nIndex, h = hwndList
#define MPX_WM_CHARTOITEM(w, l, u, n, h)		UINT u, n;						\
												HWND h;							\
												u = (UINT)(short)LOWORD(w);		\
												n = (UINT)(short)HIWORD(w);		\
												h = (HWND)l
// h = hFont, b = bRedraw
#define MPX_WM_SETFONT(w, l, h, b)				HFONT h;						\
												BOOL b;							\
												h = (HFONT)w;					\
												b = (BOOL)(short)LOWORD(l)
#define MPX_WM_GETFONT(w, l)
// u = uKey, n = nFlag
#define MPX_WM_SETHOTKEY(w, l, u, n)			UINT u, n;						\
												u = (UINT)(short)LOWORD(w);		\
												n = (UINT)(short)HIWORD(w)
#define MPX_WM_GETHOTKEY(w, l)
#define MPX_WM_QUERYDRAGICON(w, l)
// i = iCtrlId, p = pcis
#define MPX_WM_COMPAREITEM(w, l, i, p)			int i;							\
												PCOMPAREITEMSTRUCT p;			\
												i = (int)w;						\
												p = (PCOMPAREITEMSTRUCT)l
// d = dwFlags, u = dwObjId
#define MPX_WM_GETOBJECT(w, l, d, u)			DWORD d, u;						\
												d = (DWORD)w;					\
												u = (DWORD)l
// i = iCtrlId, p = pnmh
#define MPX_WM_NOTIFY(w, l, i, p)				int i;							\
												LPNMHDR p;						\
												i = (int)w;						\
												p = (LPNMHDR)l
// #define MPX_WM_INPUTLANGCHANGEREQUEST(w, l)		/// TODO:
// #define MPX_WM_INPUTLANGCHANGE(w, l)			/// TODO:
// u = uIdAction, d = dwActionData
#define MPX_WM_TCARD(w, l, u, d)				DWORD u, d;						\
												u = (DWORD)w;					\
												d = (DWORD)l
// p = lphi
#define MPX_WM_HELP(w, l, p)					LPHELPINFO p;					\
												p = (LPHELPINFO)l
#define MPX_WM_USERCHANGED(w, l)
// h = hwndFrom, i = iCmd
#define MPX_WM_NOTIFYFORMAT(w, l, h, i)			HWND h;							\
												int i;							\
												h = (HWND)w;					\
												i = (int)l
// d = dwRatio
#define MPX_WM_COMPACTING(w, l, d)				DWORD d;						\
												d = (DWORD)w
// p = pwp
#define MPX_WM_WINDOWPOSCHANGING(w, l, p)		PWINDOWPOS p;					\
												p = (PWINDOWPOS)l
#define MPX_WM_WINDOWPOSCHANGED(w, l, p)		PWINDOWPOS p;					\
												p = (PWINDOWPOS)l
// i = iPowerEvent
#define MPX_WM_POWER(w, l, i)					int i;							\
												i = (int)w
// h = hwndTo, p = pcds
#define MPX_WM_COPYDATA(w, l, h, p)				HWND h;							\
												PCOPYDATASTRUCT p;				\
												h = (HWND)w;					\
												p = (PCOPYDATASTRUCT)l
#define MPX_WM_CANCELJOURNAL(w, l)
// h = hwndCursor, x = x, y = y
#define MPX_WM_CONTEXTMENU(w, l, h, x, y)		HWND h;							\
												int x, y;						\
												h = (HWND)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// b1 = bStyle, b2 = bExStyle
#define MPX_WM_STYLECHANGING(w, l, b1, b2, p)	BOOL b1, b2;					\
												LPSTYLESTRUCT p;				\
												b1 = (w == GWL_STYLE);			\
												b2 = (w == GWL_EXSTYLE);		\
												p = (LPSTYLESTRUCT)l
// b1 = bStyle, b2 = bExStyle
#define MPX_WM_STYLECHANGED(w, l, b1, b2, p)	BOOL b1, b2;					\
												LPSTYLESTRUCT p;				\
												b1 = (w == GWL_STYLE);			\
												b2 = (w == GWL_EXSTYLE);		\
												p = (LPSTYLESTRUCT)l
// i = iBitDepth, x = cxScreen, y = cyScreen
#define MPX_WM_DISPLAYCHANGE(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uType
#define MPX_WM_GETICON(w, l, u)					UINT u;							\
												u = (UINT)w
// u = uType, h = hIcon
#define MPX_WM_SETICON(w, l, u, h)				UINT u;							\
												HICON h;						\
												u = (UINT)w						\
												h = (HICON)l
// p = pcs
#define MPX_WM_NCCREATE(w, l, p)				LPCREATESTRUCT p;				\
												p = (LPCREATESTRUCT)l
#define MPX_WM_NCDESTROY(w, l)
// b = bCalcValidRects, p = pncsp
#define MPX_WM_NCCALCSIZE(w, l, b, p)			BOOL b;							\
												LPNCCALCSIZE_PARAMS p;			\
												b = (BOOL)w;					\
												p = (LPNCCALCSIZE_PARAMS)l
// x = x, y = y
#define MPX_WM_NCHITTEST(w, l, x, y)			int x, y;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// h = hRgn
#define MPX_WM_NCPAINT(w, l, h)					HRGN h;							\
												h = (HRGN)w
// b = bActive
#define MPX_WM_NCACTIVATE(w, l, b)				BOOL b;							\
												b = (BOOL)w
// p = pMsg
#define MPX_WM_GETDLGCODE(w, l, p)				PMSG p;							\
												p = (PMSG)l
#define MPX_WM_SYNCPAINT(w, l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCMOUSEMOVE(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l);
// i = iHitCode, x = x, y = y
#define MPX_WM_NCLBUTTONDOWN(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l);
// i = iHitCode, x = x, y = y
#define MPX_WM_NCLBUTTONUP(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l);
// i = iHitCode, x = x, y = y
#define MPX_WM_NCLBUTTONDBLCLK(w, l, i, x, y)	int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCRBUTTONDOWN(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCRBUTTONUP(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCRBUTTONDBLCLK(w, l, i, x, y)	int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCMBUTTONDOWN(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCMBUTTONUP(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, x = x, y = y
#define MPX_WM_NCMBUTTONDBLCLK(w, l, i, x, y)	int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, k = iButton, x = x, y = y
#define MPX_WM_NCXBUTTONDOWN(w, l, i, k, x, y)	int i, k, x, y;					\
												i = (int)(short)LOWORD(w);		\
												k = (int)(short)HIWORD(w);		\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, k = iButton, x = x, y = y
#define MPX_WM_NCXBUTTONUP(w, l, i, k, x, y)	int i, k, x, y;					\
												i = (int)(short)LOWORD(w);		\
												k = (int)(short)HIWORD(w);		\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// i = iHitCode, k = iButton, x = x, y = y
#define MPX_WM_NCXBUTTONDBLCLK(w, l, i, k, x, y)	int i, k, x, y;				\
													i = (int)(short)LOWORD(w);	\
													k = (int)(short)HIWORD(w);	\
													x = (int)(short)LOWORD(l);	\
													y = (int)(short)HIWORD(l)
// u = uInputCode, p = pri
#define MPX_WM_INPUT(w, l, u, p)				UINT u;							\
												PRAWINPUT p;					\
												u = (UINT)w;					\
												p = (PRAWINPUT)l
// NOTE: we skip the nRepCnt parameter used in MFC because that is 1 always
// u = uChar, n = nFlags
#define MPX_WM_KEYDOWN(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_KEYUP(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_CHAR(w, l, u, n)					UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_DEADCHAR(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_SYSKEYDOWN(w, l, u, n)			UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_SYSKEYUP(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_SYSCHAR(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_SYSDEADCHAR(w, l, u, n)			UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)HIWORD(l)
// u = uChar, n = nFlags
#define MPX_WM_UNICHAR(w, l, u, n)				UINT u, n;						\
												u = (UINT)w;					\
												n = (UINT)l
#define MPX_WM_IME_STARTCOMPOSITION(w, l)
#define MPX_WM_IME_ENDCOMPOSITION(w, l)
// #define MPX_WM_IME_COMPOSITION(w, l)			/// TODO:
// h = hwndDefCtrl
#define MPX_WM_INITDIALOG(w, l, h)				HWND h;							\
												h = (HWND)w
// i1 = iId, i2 = iCode, h = hwndCtrl
#define MPX_WM_COMMAND(w, l, i1, h, i2)			int i1, i2;						\
												HWND h;							\
												i1 = LOWORD(w);					\
												i2 = HIWORD(w);					\
												h = (HWND)l
// i = iSysCmd, x = x[|key], y = y, b1 = bAccelerator, b2 = bMnemonic
#define MPX_WM_SYSCOMMAND(w, l, i, x, y, b1, b2)	int i, x, y;				\
													BOOL b1, b2;				\
													i = (int)w;					\
													x = (int)(short)LOWORD(l);	\
													y = (int)(short)HIWORD(l);	\
													b1 = (HIWORD(l) == -1);		\
													b2 = (HIWORD(l) == 0)
// u = uTimerId
#define MPX_WM_TIMER(w, l, u)					UINT u;							\
												u = (UINT)w
// u = uSBCode, n = nPos, h = hwndSBCtrl
#define MPX_WM_HSCROLL(w, l, u, n, h)			UINT u, n;						\
												HWND h;							\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												h = (HWND)l
// u = uSBCode, n = nPos, h = hwndSBCtrl
#define MPX_WM_VSCROLL(w, l, u, n, h)			UINT u, n;						\
												HWND h;							\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												h = (HWND)l
// h = hMenu
#define MPX_WM_INITMENU(w, l, h)				HMENU h;						\
												h = (HMENU)w
// h = hmnuPopup, u = uIndex, b = bSysMenu
#define MPX_WM_INITMENUPOPUP(w, l, h, u, b)		HMENU h;						\
												UINT u;							\
												BOOL b;							\
												h = (HMENU)w;					\
												u = (UINT)LOWORD(l);			\
												b = (BOOL)HIWORD(l)
// u = uItemId[|index], n = nFlags, h = hMenu
#define MPX_WM_MENUSELECT(w, l, u, n, h)		UINT u, n;						\
												HMENU h;						\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												h = (HMENU)l
// u = uChar, n = nFlag(MF_POPUP/MF_SYSMENU), h = hMenu
#define MPX_WM_MENUCHAR(w, l, u, n, h)			UINT u, n;						\
												HMENU h;						\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												h = (HMENU)l
// u = uWhy(MSGF_DIALOGBOX/MSGF_MENU), h = hwndWho
#define MPX_WM_ENTERIDLE(w, l, u, h)			UINT u;							\
												HWND h;							\
												u = (UINT)w;					\
												h = (HWND)l
// u = uIndex, h = hMenu
#define MPX_WM_MENURBUTTONUP(w, l, u, h)		UINT u;							\
												HMENU h;						\
												u = (UINT)w;					\
												h = (HMENU)l
// u = uIdxDrag, h = hMenu
#define MPX_WM_MENUDRAG(w, l, u, h)				UINT u;							\
												HMENU h;						\
												u = (UINT)w;					\
												h = (HMENU)l
// p = pmgoi
#define MPX_WM_MENUGETOBJECT(w, l, p)			PMENUGETOBJECTINFO p;			\
												p = (PMENUGETOBJECTINFO)l
// h = hMenu, b = bSysMenu
#define MPX_WM_UNINITMENUPOPUP(w, l, h, b)		HMENU h;						\
												BOOL b;							\
												h = (HMENU)w;					\
												b = (BOOL)(HIWORD(l) == MF_SYSMENU)
// NOTE: b = bWindows9x [IN], u = uIndex, h = hMenu
#define MPX_WM_MENUCOMMAND(w, l, b, u, h)		UINT u;							\
												HMENU h;						\
												u = (UINT)(b ? HIWORD(w) : w);	\
												h = (HMENU)l
// i = iUIAction, u = uFlags
#define MPX_WM_CHANGEUISTATE(w, l, i, u)		int i;							\
												UINT u;							\
												i = (int)(LOWORD)w;				\
												u = (UINT)(HIWORD)w
// i = iUIAction, u = uFlags
#define MPX_WM_UPDATEUISTATE(w, l, i, u)		int i;							\
												UINT u;							\
												i = (int)(LOWORD)w;				\
												u = (UINT)(HIWORD)w
#define MPX_WM_QUERYUISTATE(w, l)
/*
// h1 = hdc, h2 = hwndMsgBox
#define MPX_WM_CTLCOLORMSGBOX(w, l, h1, h2)		HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
*/
// h1 = hdc, h2 = hwndEdit
#define MPX_WM_CTLCOLOREDIT(w, l, h1, h2)		HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// h1 = hdc, h2 = hwndList
#define MPX_WM_CTLCOLORLISTBOX(w, l, h1, h2)	HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// h1 = hdc, h2 = hwndButton
#define MPX_WM_CTLCOLORBTN(w, l, h1, h2)		HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// h1 = hdc, h2 = hwndDlg
#define MPX_WM_CTLCOLORDLG(w, l, h1, h2)		HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// h1 = hdc, h2 = hwndSBCtrl
#define MPX_WM_CTLCOLORSCROLLBAR(w, l, h1, h2)	HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// h1 = hdc, h2 = hwndStatic
#define MPX_WM_CTLCOLORSTATIC(w, l, h1, h2)		HDC h1;							\
												HWND h2;						\
												h1 = (HDC)w;					\
												h2 = (HWND)l
// u = uFlags, x = x, y = y
#define MPX_WM_MOUSEMOVE(w, l, u, x, y)			UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_LBUTTONDOWN(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_LBUTTONUP(w, l, u, x, y)			UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_LBUTTONDBLCLK(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_RBUTTONDOWN(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_RBUTTONUP(w, l, u, x, y)			UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_RBUTTONDBLCLK(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_MBUTTONDOWN(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_MBUTTONUP(w, l, u, x, y)			UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, x = x, y = y
#define MPX_WM_MBUTTONDBLCLK(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// #define MPX_WM_PARENTNOTIFY(w, l)				/// TODO:
// b = bSysMenu
#define MPX_WM_ENTERMENULOOP(w, l, b)			BOOL b;							\
												b = (BOOL)w
// b = bContextMenu
#define MPX_WM_EXITMENULOOP(w, l, b)			BOOL b;							\
												b = (BOOL)w
// u = uFlags, i = iDelta
#define MPX_WM_MOUSEWHEEL(w, l, u, i, x, y)		UINT u;							\
												int i, x, y;					\
												u = (UINT)LOWORD(w);			\
												i = (int)HIWORD(w);				\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, n = iButton, x = x, y = y
#define MPX_WM_XBUTTONDOWN(w, l, u, n, x, y)	UINT u, n;						\
												int x, y;						\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, n = iButton, x = x, y = y
#define MPX_WM_XBUTTONUP(w, l, u, n, x, y)		UINT u, n;						\
												int x, y;						\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uFlags, n = iButton, x = x, y = y
#define MPX_WM_XBUTTONDBLCLK(w, l, u, n, x, y)	UINT u, n;						\
												int x, y;						\
												u = (UINT)LOWORD(w);			\
												n = (UINT)HIWORD(w);			\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uVirKey, p = pmnm
#define MPX_WM_NEXTMENU(w, l, u, p)				UINT u;							\
												PMDINEXTMENU p;					\
												u = (UINT)w;					\
												p = (PMDINEXTMENU)l
// u = uSide, p = prc
#define MPX_WM_SIZING(w, l, u, p)				UINT u;							\
												LPRECT p;						\
												u = (UINT)w;					\
												p = (LPRECT)l
// h = hwndCapture
#define MPX_WM_CAPTURECHANGED(w, l, h)			HWND h;							\
												h = (HWND)l
// p = prc
#define MPX_WM_MOVING(w, l, p)					LPRECT p;						\
												p = (LPRECT)l
// i = iPowerEvent, u = uData
#define MPX_WM_POWERBROADCAST(w, l, i, u)		int i;							\
												DWORD d;						\
												i = (int)w;						\
												u = (UINT)l
// u = uDevEvent
#define MPX_WM_DEVICECHANGE(w, l, u)			UINT u;							\
												u = (UINT)w
// p = pmcs
#define MPX_WM_MDICREATE(w, l, p)				LPMDICREATESTRUCT p;			\
												p = (LPMDICREATESTRUCT)l
// h = hwndMDIChild
#define MPX_WM_MDIDESTROY(w, l, h)				HWND h;							\
												h = (HWND)w
// h = hwndMDIChild
#define MPX_WM_MDIACTIVATE(w, l, h)				HWND h;							\
												h = (HWND)w
// h = hwndMDIChild
#define MPX_WM_MDIRESTORE(w, l, h)				HWND h;							\
												h = (HWND)w
// h = hwndMDIChild, b = bPrevWnd
#define MPX_WM_MDINEXT(w, l, h, b)				HWND h;							\
												BOOL b;							\
												h = (HWND)w;					\
												b = (BOOL)l
// h = hwndMDIChild
#define MPX_WM_MDIMAXIMIZE(w, l, h)				HWND h;							\
												h = (HWND)w
// u = uTileFlags
#define MPX_WM_MDITILE(w, l, u)					UINT u;							\
												u = (UINT)w
// u = uCascadeFlags
#define MPX_WM_MDICASCADE(w, l, u)				UINT u;							\
												u = (UINT)w
#define MPX_WM_MDIICONARRANGE(w, l)
// p = pbMaximized
#define MPX_WM_MDIGETACTIVE(w, l, p)			PBOOL p;						\
												p = (PBOOL)l
// h1 = hmnuFrame, h2 = hmnuWindow
#define MPX_WM_MDISETMENU(w, l, h1, h2)			HMENU h1, h2;					\
												h1 = (HMENU)w;					\
												h2 = (HMENU)l
#define MPX_WM_ENTERSIZEMOVE(w, l)
#define MPX_WM_EXITSIZEMOVE(w, l)
// h = hDrop
#define MPX_WM_DROPFILES(w, l, h)				HDROP h;						\
												h = (HDROP)w
#define MPX_WM_MDIREFRESHMENU(w, l)
// #define MPX_WM_IME_REPORT(w, l)				/// TODO: ???
// b = bActive, u = uDisplayOptions
#define MPX_WM_IME_SETCONTEXT(w, l, b, u)		BOOL b;							\
												UINT u;							\
												b = (BOOL)w;					\
												u = (UINT)l
// n = nIMECmd, u = uData
#define MPX_WM_IME_NOTIFY(w, l, n, u)			UINT n, u;						\
												n = (UINT)w;					\
												u = (UINT)l
// n = nIMECmd, u = uData
#define MPX_WM_IME_CONTROL(w, l, n, u)			UINT n, u;						\
												n = (UINT)w;					\
												u = (UINT)l
#define MPX_WM_IME_COMPOSITIONFULL(w, l)
// #define MPX_WM_IME_SELECT(w, l, b, h)			/// TODO:
// #define MPX_WM_IME_CHAR(w, l)					/// TODO:
// n = nIMECmd, u = uData
#define MPX_WM_IME_REQUEST(w, l, n, u)			UINT n, u;						\
												n = (UINT)w;					\
												u = (UINT)l
// #define MPX_WM_IMEKEYDOWN(w, l)				/// TODO: ???
// #define MPX_WM_IMEKEYUP(w, l)				/// TODO: ???
// i = iHitCode, x = x, y = y
#define MPX_WM_NCMOUSEHOVER(w, l, i, x, y)		int i, x, y;					\
												i = (int)w;						\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
// u = uVirKey, x = x, y = y
#define MPX_WM_MOUSEHOVER(w, l, u, x, y)		UINT u;							\
												int x, y;						\
												u = (UINT)w;					\
												x = (int)(short)LOWORD(l);		\
												y = (int)(short)HIWORD(l)
#define MPX_WM_NCMOUSELEAVE(w, l)
#define MPX_WM_MOUSELEAVE(w, l)
// u = uStatusCode, d = dwSessionId
#define MPX_WM_WTSSESSION_CHANGE(w, l, u, d)	UINT u;							\
												DWORD d;						\
												u = (UINT)w;					\
												d = (DWORD)l
#define MPX_WM_CUT(w, l)
#define MPX_WM_COPY(w, l)
#define MPX_WM_PASTE(w, l)
#define MPX_WM_CLEAR(w, l)
#define MPX_WM_UNDO(w, l)
// i = iClipboardFormat
#define MPX_WM_RENDERFORMAT(w, l, i)			int i;							\
												i = (int)w
#define MPX_WM_RENDERALLFORMATS(w, l)
#define MPX_WM_DESTROYCLIPBOARD(w, l)
#define MPX_WM_DRAWCLIPBOARD(w, l)
// h1 = hwndViewer, h2 = hglbPaintStruct
#define MPX_WM_PAINTCLIPBOARD(w, l, h1, h2)		HWND h1;						\
												HGLOBAL h2;						\
												h1 = (HWND)w;					\
												h2 = (HGLOBAL)l
// h = hwndViewer, u = uSBCode, n = nPos
#define MPX_WM_VSCROLLCLIPBOARD(w, l, h, u, n)	HWND h;							\
												UINT u, n;						\
												h = (HWND)w;					\
												u = (UINT)LOWORD(l);			\
												n = (UINT)HIWORD(l)
// h1 = hwndViewer, h2 = hglbRect
#define MPX_WM_SIZECLIPBOARD(w, l, h1, h2)		HWND h1;						\
												HGLOBAL h2;						\
												h1 = (HWND)w;					\
												h2 = (HGLOBAL)l
// c = cchSize, p = pszFormatName
#define MPX_WM_ASKCBFORMATNAME(w, l, c, p)		UINT c;							\
												LPTSTR p;						\
												c = (UINT)w;					\
												p = (LPTSTR)l
// h1 = hwndRemove, h2 = hwndNext
#define MPX_WM_CHANGECBCHAIN(w, l, h1, h2)		HWND h1, h2;					\
												h1 = (HWND)w;					\
												h2 = (HWND)l
// h = hwndViewer, u = uSBCode, n = nPos
#define MPX_WM_HSCROLLCLIPBOARD(w, l, h, u, n)	HWND h;							\
												UINT u, n;						\
												h = (HWND)w;					\
												u = (UINT)LOWORD(l);			\
												n = (UINT)HIWORD(l)
#define MPX_WM_QUERYNEWPALETTE(w, l)
// h = hwndRealize
#define MPX_WM_PALETTEISCHANGING(w, l, h)		HWND h;							\
												h = (HWND)w
// h = hwndSource
#define MPX_WM_PALETTECHANGED(w, l, h)			HWND h;							\
												h = (HWND)w
// i = iHotKeyId, u = uKey, n = nFlags
#define MPX_WM_HOTKEY(w, l, i, u, n)			int i;							\
												UINT u, n;						\
												i = (int)w;						\
												u = (UINT)LOWORD(l);			\
												n = (UINT)HIWORD(l)
// h = hdc, u = uDrawOptions
#define MPX_WM_PRINT(w, l, h, u)				HDC h;							\
												UINT u;							\
												h = (HDC)w;						\
												u = (UINT)l
// h = hdc, u = uDrawOptions
#define MPX_WM_PRINTCLIENT(w, l, h, u)			HDC h;							\
												UINT u;							\
												h = (HDC)w;						\
												u = (UINT)l
// #define MPX_WM_APPCOMMAND(w, l)					/// TODO:
// #define MPX_WM_THEMECHANGED(w, l)				/// TODO:
// #define MPX_WM_DDE_INITIATE(w, l)				/// TODO:
// #define MPX_WM_DDE_TERMINATE(w, l)				/// TODO:
// #define MPX_WM_DDE_ADVISE(w, l)					/// TODO:
// #define MPX_WM_DDE_UNADVISE(w, l)				/// TODO:
// #define MPX_WM_DDE_ACK(w, l)					/// TODO:
// #define MPX_WM_DDE_DATA(w, l)					/// TODO:
// #define MPX_WM_DDE_REQUEST(w, l)				/// TODO:
// #define MPX_WM_DDE_POKE(w, l)					/// TODO:
// #define MPX_WM_DDE_EXECUTE(w, l)				/// TODO:

// #define MPX_WM_RASDIALEVENT(w, l)

//////////////////////////////////////////////////////////////////////////
#define TMPX_WM_NULL()							MPX_WM_NULL(wParam, lParam)
#define TMPX_WM_CREATE(pCreateStruct)			MPX_WM_CREATE(wParam, lParam, pCreateStruct)
#define TMPX_WM_DESTROY()						MPX_WM_DESTROY(wParam, lParam)
#define TMPX_WM_MOVE(x, y)						MPX_WM_MOVE(wParam, lParam, x, y)
#define TMPX_WM_SIZE(uType, cx, cy)				MPX_WM_SIZE(wParam, lParam, uType, cx, cy)
#define TMPX_WM_ACTIVATE(uState, bMinimized, hwndOther) \
												MPX_WM_ACTIVATE(wParam, lParam, uState, bMinimized, hwndOther)
#define TMPX_WM_SETFOCUS(hwndOld)				MPX_WM_SETFOCUS(wParam, lParam, hwndOld)
#define TMPX_WM_KILLFOCUS(hwndNew)				MPX_WM_KILLFOCUS(wParam, lParam, hwndNew)
#define TMPX_WM_ENABLE(bEnable)					MPX_WM_ENABLE(wParam, lParam, bEnable)
#define TMPX_WM_SETREDRAW(bEnable)				MPX_WM_SETREDRAW(wParam, lParam, bEnable)
#define TMPX_WM_SETTEXT(pszText)				MPX_WM_SETTEXT(wParam, lParam, pszText)
#define TMPX_WM_GETTEXT(cchSize, pszText)		MPX_WM_GETTEXT(wParam, lParam, cchSize, pszText)
#define TMPX_WM_GETTEXTLENGTH()					MPX_WM_GETTEXTLENGTH(wParam, lParam)
#define TMPX_WM_PAINT()							MPX_WM_PAINT(wParam, lParam)
#define TMPX_WM_CLOSE()							MPX_WM_CLOSE(wParam, lParam)
#define TMPX_WM_QUERYENDSESSION(uFlag)			MPX_WM_QUERYENDSESSION(wParam, lParam, uFlag)
#define TMPX_WM_QUIT(iExitCode)					MPX_WM_QUIT(wParam, lParam, iExitCode)
#define TMPX_WM_QUERYOPEN()						MPX_WM_QUERYOPEN(wParam, lParam)
#define TMPX_WM_ERASEBKGND(hdc)					MPX_WM_ERASEBKGND(wParam, lParam, hdc)
#define TMPX_WM_SYSCOLORCHANGE()				MPX_WM_SYSCOLORCHANGE(wParam, lParam)
#define TMPX_WM_ENDSESSION(bEnding, uFlag)		MPX_WM_ENDSESSION(wParam, lParam, bEnding, uFlag)
#define TMPX_WM_SHOWWINDOW(bShow, uState)		MPX_WM_SHOWWINDOW(wParam, lParam, bShow, uState)
#define TMPX_WM_WININICHANGE(pszParameter)		MPX_WM_WININICHANGE(wParam, lParam, pszParameter)
#define TMPX_WM_DEVMODECHANGE(pszDevice)		MPX_WM_DEVMODECHANGE(wParam, lParam, pszDevice)
#define TMPX_WM_ACTIVATEAPP(bActive, dwThreadId) \
												MPX_WM_ACTIVATEAPP(wParam, lParam, bActive, dwThreadId)
#define TMPX_WM_FONTCHANGE()					MPX_WM_FONTCHANGE(wParam, lParam)
#define TMPX_WM_TIMECHANGE()					MPX_WM_TIMECHANGE(wParam, lParam)
#define TMPX_WM_CANCELMODE()					MPX_WM_CANCELMODE(wParam, lParam)
#define TMPX_WM_SETCURSOR(hwndCursor, iHitCode, uMsg) \
												MPX_WM_SETCURSOR(wParam, lParam, hwndCursor, iHitCode, uMsg)
#define TMPX_WM_MOUSEACTIVATE(hwndTopParent, iHitCode, uMsg) \
												MPX_WM_MOUSEACTIVATE(wParam, lParam, hwndTopParent, iHitCode, uMsg)
#define TMPX_WM_CHILDACTIVATE()					MPX_WM_CHILDACTIVATE(wParam, lParam)
#define TMPX_WM_QUEUESYNC()						MPX_WM_QUEUESYNC(wParam, lParam)
#define TMPX_WM_GETMINMAXINFO(pMinMaxInfo)		MPX_WM_GETMINMAXINFO(wParam, lParam, pMinMaxInfo)
#define TMPX_WM_PAINTICON()						MPX_WM_PAINTICON(wParam, lParam)
#define TMPX_WM_ICONERASEBKGND(hdc)				MPX_WM_ICONERASEBKGND(hdc)
#define TMPX_WM_SPOOLERSTATUS(uFlags, dwJobsNumber) \
												MPX_WM_SPOOLERSTATUS(wParam, lParam, uFlags, dwJobsNumber)
#define TMPX_WM_DRAWITEM(bMenuItem, iCtrlId, pdis) \
												MPX_WM_DRAWITEM(wParam, lParam, bMenuItem, iCtrlId, pdis)
#define TMPX_WM_MEASUREITEM(bMenuItem, iCtrlId, pmis) \
												MPX_WM_MEASUREITEM(wParam, lParam, bMenuItem, iCtrlId, pmis)
#define TMPX_WM_DELETEITEM(iCtrlId, pdis)		MPX_WM_DELETEITEM(wParam, lParam, iCtrlId, pdis)
#define TMPX_WM_VKEYTOITEM(uKey, nIndex, hwndList) \
												MPX_WM_VKEYTOITEM(wParam, lParam, uKey, nIndex, hwndList)
#define TMPX_WM_CHARTOITEM(uChar, nIndex, hwndList) \
												MPX_WM_CHARTOITEM(wParam, lParam, uChar, nIndex, hwndList)
#define TMPX_WM_SETFONT(hFont, bRedraw)			MPX_WM_SETFONT(wParam, lParam, hFont, bRedraw)
#define TMPX_WM_GETFONT()						MPX_WM_GETFONT(wParam, lParam)
#define TMPX_WM_SETHOTKEY(uKey, nFlag)			MPX_WM_SETHOTKEY(wParam, lParam, uKey, nFlag)
#define TMPX_WM_GETHOTKEY()						MPX_WM_GETHOTKEY(wParam, lParam)
#define TMPX_WM_QUERYDRAGICON()					MPX_WM_QUERYDRAGICON(wParam, lParam)
#define TMPX_WM_COMPAREITEM(iCtrlId, pcis)		MPX_WM_COMPAREITEM(wParam, lParam, iCtrlId, pcis)
#define TMPX_WM_GETOBJECT(dwFlags, dwObjId)		MPX_WM_GETOBJECT(wParam, lParam, dwFlags, dwObjId)
#define TMPX_WM_NOTIFY(iCtrlId, pnmh)			MPX_WM_NOTIFY(wParam, lParam, iCtrlId, pnmh)
#define TMPX_WM_TCARD(uIdAction, dwActionData)	MPX_WM_TCARD(wParam, lParam, uIdAction, dwActionData)
#define TMPX_WM_HELP(pHelpInfo)					MPX_WM_HELP(wParam, lParam, pHelpInfo)
#define TMPX_WM_USERCHANGED()					MPX_WM_USERCHANGED(wParam, lParam)
#define TMPX_WM_NOTIFYFORMAT(hwndFrom, iCmd)	MPX_WM_NOTIFYFORMAT(wParam, lParam, hwndFrom, iCmd)
#define TMPX_WM_COMPACTING(dwRatio)				MPX_WM_COMPACTING(wParam, lParam, dwRatio)
#define TMPX_WM_WINDOWPOSCHANGING(pWindowPos)	MPX_WM_WINDOWPOSCHANGING(wParam, lParam, pWindowPos)
#define TMPX_WM_WINDOWPOSCHANGED(pWindowPos)	MPX_WM_WINDOWPOSCHANGED(wParam, lParam, pWindowPos)
#define TMPX_WM_POWER(iPowerEvent)				MPX_WM_POWER(wParam, lParam, iPowerEvent)
#define TMPX_WM_COPYDATA(hwndTo, pcds)			MPX_WM_COPYDATA(wParam, lParam, hwndTo, pcds)
#define TMPX_WM_CANCELJOURNAL()					MPX_WM_CANCELJOURNAL(wParam, lParam)
#define TMPX_WM_CONTEXTMENU(hwndCursor, x, y)	MPX_WM_CONTEXTMENU(wParam, lParam, hwndCursor, x, y)
#define TMPX_WM_STYLECHANGING(bStyle, bExStyle, pStyleStruct) \
												MPX_WM_STYLECHANGING(wParam, lParam, bStyle, bExStyle, pStyleStruct)
#define TMPX_WM_STYLECHANGED(bStyle, bExStyle, pStyleStruct) \
												MPX_WM_STYLECHANGED(wParam, lParam, bStyle, bExStyle, pStyleStruct)
#define TMPX_WM_DISPLAYCHANGE(iBitDepth, cxScreen, cyScreen) \
												MPX_WM_DISPLAYCHANGE(wParam, lParam, iBitDepth, cxScreen, cyScreen)
#define TMPX_WM_GETICON(uType)					MPX_WM_GETICON(wParam, lParam, uType)
#define TMPX_WM_SETICON(uType, hIcon)			MPX_WM_SETICON(wParam, lParam, uType, hIcon)
#define TMPX_WM_NCCREATE(pCreateStruct)			MPX_WM_NCCREATE(wParam, lParam, pCreateStruct)
#define TMPX_WM_NCDESTROY()						MPX_WM_NCDESTROY(wParam, lParam)
#define TMPX_WM_NCCALCSIZE(bCalcValidRects, pncsp) \
												MPX_WM_NCCALCSIZE(wParam, lParam, bCalcValidRects, pncsp)
#define TMPX_WM_NCHITTEST(x, y)					MPX_WM_NCHITTEST(wParam, lParam, x, y)
#define TMPX_WM_NCPAINT(hRgn)					MPX_WM_NCPAINT(wParam, lParam, hRgn)
#define TMPX_WM_NCACTIVATE(bActive)				MPX_WM_NCACTIVATE(wParam, lParam, bActive)
#define TMPX_WM_GETDLGCODE(pMsg)				MPX_WM_GETDLGCODE(wParam, lParam, pMsg)
#define TMPX_WM_SYNCPAINT()						MPX_WM_SYNCPAINT(wParam, lParam)
#define TMPX_WM_NCMOUSEMOVE(iHitCode, x, y)		MPX_WM_NCMOUSEMOVE(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCLBUTTONDOWN(iHitCode, x, y)	MPX_WM_NCLBUTTONDOWN(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCLBUTTONUP(iHitCode, x, y)		MPX_WM_NCLBUTTONUP(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCLBUTTONDBLCLK(iHitCode, x, y)	MPX_WM_NCLBUTTONDBLCLK(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCRBUTTONDOWN(iHitCode, x, y)	MPX_WM_NCRBUTTONDOWN(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCRBUTTONUP(iHitCode, x, y)		MPX_WM_NCRBUTTONUP(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCRBUTTONDBLCLK(iHitCode, x, y)	MPX_WM_NCRBUTTONDBLCLK(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCMBUTTONDOWN(iHitCode, x, y)	MPX_WM_NCMBUTTONDOWN(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCMBUTTONUP(iHitCode, x, y)		MPX_WM_NCMBUTTONUP(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCMBUTTONDBLCLK(iHitCode, x, y)	MPX_WM_NCMBUTTONDBLCLK(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_NCXBUTTONDOWN(iHitCode, iButton, x, y) \
												MPX_WM_NCXBUTTONDOWN(wParam, lParam, iHitCode, iButton, x, y)
#define TMPX_WM_NCXBUTTONUP(iHitCode, iButton, x, y) \
												MPX_WM_NCXBUTTONUP(wParam, lParam, iHitCode, iButton, x, y)
#define TMPX_WM_NCXBUTTONDBLCLK(iHitCode, iButton, x, y) \
												MPX_WM_NCXBUTTONDBLCLK(wParam, lParam, iHitCode, iButton, x, y)
#define TMPX_WM_INPUT(uInputCode, pRawInput)	MPX_WM_INPUT(wParam, lParam, uInputCode, pRawInput)
#define TMPX_WM_KEYDOWN(uChar, nFlags)			MPX_WM_KEYDOWN(wParam, lParam, uChar, nFlags)
#define TMPX_WM_KEYUP(uChar, nFlags)			MPX_WM_KEYUP(wParam, lParam, uChar, nFlags)
#define TMPX_WM_CHAR(uChar, nFlags)				MPX_WM_CHAR(wParam, lParam, uChar, nFlags)
#define TMPX_WM_DEADCHAR(uChar, nFlags)			MPX_WM_DEADCHAR(wParam, lParam, uChar, nFlags)
#define TMPX_WM_SYSKEYDOWN(uChar, nFlags)		MPX_WM_SYSKEYDOWN(wParam, lParam, uChar, nFlags)
#define TMPX_WM_SYSKEYUP(uChar, nFlags)			MPX_WM_SYSKEYUP(wParam, lParam, uChar, nFlags)
#define TMPX_WM_SYSCHAR(uChar, nFlags)			MPX_WM_SYSCHAR(wParam, lParam, uChar, nFlags)
#define TMPX_WM_SYSDEADCHAR(uChar, nFlags)		MPX_WM_SYSDEADCHAR(wParam, lParam, uChar, nFlags)
#define TMPX_WM_UNICHAR(uChar, nFlags)			MPX_WM_UNICHAR(wParam, lParam, uChar, nFlags)
#define TMPX_WM_IME_STARTCOMPOSITION()			MPX_WM_IME_STARTCOMPOSITION(wParam, lParam)
#define TMPX_WM_IME_ENDCOMPOSITION()			MPX_WM_IME_ENDCOMPOSITION(wParam, lParam)
#define TMPX_WM_INITDIALOG(hwndDefCtrl)			MPX_WM_INITDIALOG(wParam, lParam, hwndDefCtrl)
#define TMPX_WM_COMMAND(iId, hwndCtrl, iCode)	MPX_WM_COMMAND(wParam, lParam, iId, hwndCtrl, iCode)
#define TMPX_WM_SYSCOMMAND(iSysCmd, x, y, bAccelerator, bMnemonic) \
												MPX_WM_SYSCOMMAND(wParam, lParam, iSysCmd, x, y, bAccelerator, bMnemonic)
#define TMPX_WM_TIMER(uTimerId)					MPX_WM_TIMER(wParam, lParam, uTimerId)
#define TMPX_WM_HSCROLL(uSBCode, nPos, hwndSBCtrl) \
												MPX_WM_HSCROLL(wParam, lParam, uSBCode, nPos, hwndSBCtrl)
#define TMPX_WM_VSCROLL(uSBCode, nPos, hwndSBCtrl) \
												MPX_WM_VSCROLL(wParam, lParam, uSBCode, nPos, hwndSBCtrl)
#define TMPX_WM_INITMENU(hMenu)					MPX_WM_INITMENU(wParam, lParam, hMenu)
#define TMPX_WM_INITMENUPOPUP(hmnuPopup, uIndex, bSysMenu) \
												MPX_WM_INITMENUPOPUP(wParam, lParam, hmnuPopup, uIndex, bSysMenu)
#define TMPX_WM_MENUSELECT(uItemId, nFlags, hMenu) \
												MPX_WM_MENUSELECT(wParam, lParam, uItemId, nFlags, hMenu)
#define TMPX_WM_MENUCHAR(uChar, nFlag, hMenu)	MPX_WM_MENUCHAR(wParam, lParam, uChar, nFlag, hMenu)
#define TMPX_WM_ENTERIDLE(uReason, hwnd)		MPX_WM_ENTERIDLE(wParam, lParam, uReason, hwnd)
#define TMPX_WM_MENURBUTTONUP(uIndex, hMenu)	MPX_WM_MENURBUTTONUP(wParam, lParam, uIndex, hMenu)
#define TMPX_WM_MENUDRAG(uIndex, hMenu)			MPX_WM_MENUDRAG(wParam, lParam, uIndex, hMenu)
#define TMPX_WM_MENUGETOBJECT(pmgoi)			MPX_WM_MENUGETOBJECT(wParam, lParam, pmgoi)
#define TMPX_WM_UNINITMENUPOPUP(hMenu, bSysMenu) \
												MPX_WM_UNINITMENUPOPUP(wParam, lParam, hMenu, bSysMenu)
#define TMPX_WM_MENUCOMMAND(/*IN*/bWin9x, uIndex, hMenu) \
												MPX_WM_MENUCOMMAND(wParam, lParam, bWin9x, uIndex, hMenu)
#define TMPX_WM_CHANGEUISTATE(iUIAction, uFlags) \
												MPX_WM_CHANGEUISTATE(wParam, lParam, iUIAction, uFlags)
#define TMPX_WM_UPDATEUISTATE(iUIAction, uFlags) \
												MPX_WM_UPDATEUISTATE(wParam, lParam, iUIAction, uFlags)
#define TMPX_WM_QUERYUISTATE()					MPX_WM_QUERYUISTATE(wParam, lParam)
#define TMPX_WM_CTLCOLOREDIT(hdc, hwndEdit)		MPX_WM_CTLCOLOREDIT(wParam, lParam, hdc, hwndEdit)
#define TMPX_WM_CTLCOLORLISTBOX(hdc, hwndList)	MPX_WM_CTLCOLORLISTBOX(wParam, lParam, hdc, hwndList)
#define TMPX_WM_CTLCOLORBTN(hdc, hwndButton)	MPX_WM_CTLCOLORBTN(wParam, lParam, hdc, hwndButton)
#define TMPX_WM_CTLCOLORDLG(hdc, hwndDlg)		MPX_WM_CTLCOLORDLG(wParam, lParam, hdc, hwndDlg)
#define TMPX_WM_CTLCOLORSCROLLBAR(hdc, hwndSBCtrl) \
												MPX_WM_CTLCOLORSCROLLBAR(wParam, lParam, hdc, hwndSBCtrl)
#define TMPX_WM_CTLCOLORSTATIC(hdc, hwndStatic) \
												MPX_WM_CTLCOLORSTATIC(wParam, lParam, hdc, hwndStatic)
#define TMPX_WM_MOUSEMOVE(uFlags, x, y)			MPX_WM_MOUSEMOVE(wParam, lParam, uFlags, x, y)
#define TMPX_WM_LBUTTONDOWN(uFlags, x, y)		MPX_WM_LBUTTONDOWN(wParam, lParam, uFlags, x, y)
#define TMPX_WM_LBUTTONUP(uFlags, x, y)			MPX_WM_LBUTTONUP(wParam, lParam, uFlags, x, y)
#define TMPX_WM_LBUTTONDBLCLK(uFlags, x, y)		MPX_WM_LBUTTONDBLCLK(wParam, lParam, uFlags, x, y)
#define TMPX_WM_RBUTTONDOWN(uFlags, x, y)		MPX_WM_RBUTTONDOWN(wParam, lParam, uFlags, x, y)
#define TMPX_WM_RBUTTONUP(uFlags, x, y)			MPX_WM_RBUTTONUP(wParam, lParam, uFlags, x, y)
#define TMPX_WM_RBUTTONDBLCLK(uFlags, x, y)		MPX_WM_RBUTTONDBLCLK(wParam, lParam, uFlags, x, y)
#define TMPX_WM_MBUTTONDOWN(uFlags, x, y)		MPX_WM_MBUTTONDOWN(wParam, lParam, uFlags, x, y)
#define TMPX_WM_MBUTTONUP(uFlags, x, y)			MPX_WM_MBUTTONUP(wParam, lParam, uFlags, x, y)
#define TMPX_WM_MBUTTONDBLCLK(uFlags, x, y)		MPX_WM_MBUTTONDBLCLK(wParam, lParam, uFlags, x, y)
#define TMPX_WM_ENTERMENULOOP(bSysMenu)			MPX_WM_ENTERMENULOOP(wParam, lParam, bSysMenu)
#define TMPX_WM_EXITMENULOOP(bContextMenu)		MPX_WM_EXITMENULOOP(wParam, lParam, bContextMenu)
#define TMPX_WM_MOUSEWHEEL(uFlags, iDelta, x, y) \
												MPX_WM_MOUSEWHEEL(wParam, lParam, uFlags, iDelta, x, y)
#define TMPX_WM_XBUTTONDOWN(uFlags, iButton, x, y) \
												MPX_WM_XBUTTONDOWN(wParam, lParam, uFlags, iButton, x, y)
#define TMPX_WM_XBUTTONUP(uFlags, iButton, x, y) \
												MPX_WM_XBUTTONUP(wParam, lParam, uFlags, iButton, x, y)
#define TMPX_WM_XBUTTONDBLCLK(uFlags, iButton, x, y) \
												MPX_WM_XBUTTONDBLCLK(wParam, lParam, uFlags, iButton, x, y)
#define TMPX_WM_NEXTMENU(uVirtualKey, pMdiNextMenu) \
												MPX_WM_NEXTMENU(wParam, lParam, uVirtualKey, pMdiNextMenu)
#define TMPX_WM_SIZING(uSide, prc)				MPX_WM_SIZING(wParam, lParam, uSide, prc)
#define TMPX_WM_CAPTURECHANGED(hwndCapture)		MPX_WM_CAPTURECHANGED(wParam, lParam, hwndCapture)
#define TMPX_WM_MOVING(prc)						MPX_WM_MOVING(wParam, lParam, prc)
#define TMPX_WM_POWERBROADCAST(iPowerEvent, uData) \
												MPX_WM_POWERBROADCAST(wParam, lParam, iPowerEvent, uData)
#define TMPX_WM_DEVICECHANGE(uDevChange)		MPX_WM_DEVICECHANGE(wParam, lParam, uDevChange)
#define TMPX_WM_MDICREATE(pMdiCreateStruct)		MPX_WM_MDICREATE(wParam, lParam, pMdiCreateStruct)
#define TMPX_WM_MDIDESTROY(hwndMDIChild)		MPX_WM_MDIDESTROY(wParam, lParam, hwndMDIChild)
#define TMPX_WM_MDIACTIVATE(hwndMDIChild)		MPX_WM_MDIACTIVATE(wParam, lParam, hwndMDIChild)
#define TMPX_WM_MDIRESTORE(hwndMDIChild)		MPX_WM_MDIRESTORE(wParam, lParam, hwndMDIChild)
#define TMPX_WM_MDINEXT(hwndMDIChild, bPrevious) \
												MPX_WM_MDINEXT(wParam, lParam, hwndMDIChild, bPrevious)
#define TMPX_WM_MDIMAXIMIZE(hwndMDIChild)		MPX_WM_MDIMAXIMIZE(wParam, lParam, hwndMDIChild)
#define TMPX_WM_MDITILE(uTileFlags)				MPX_WM_MDITILE(wParam, lParam, uTileFlags);
#define TMPX_WM_MDICASCADE(uCascadeFlags)		MPX_WM_MDICASCADE(wParam, lParam, uCascadeFlags)
#define TMPX_WM_MDIICONARRANGE()				MPX_WM_MDIICONARRANGE(wParam, lParam)
#define TMPX_WM_MDIGETACTIVE(pbMaximized)		MPX_WM_MDIGETACTIVE(wParam, lParam, pbMaximized)
#define TMPX_WM_MDISETMENU(hmnuFrame, hmnuWindow) \
												MPX_WM_MDISETMENU(wParam, lParam, hmnuFrame, hmnuWindow)
#define TMPX_WM_ENTERSIZEMOVE()					MPX_WM_ENTERSIZEMOVE(wParam, lParam)
#define TMPX_WM_EXITSIZEMOVE()					MPX_WM_EXITSIZEMOVE(wParam, lParam)
#define TMPX_WM_DROPFILES(hDrop)				MPX_WM_DROPFILES(wParam, lParam, hDrop)
#define TMPX_WM_MDIREFRESHMENU()				MPX_WM_MDIREFRESHMENU(wParam, lParam)
#define TMPX_WM_IME_SETCONTEXT(bActive, uDispalyOptions) \
												MPX_WM_IME_SETCONTEXT(wParam, lParam, bActive, uDispalyOptions)
#define TMPX_WM_IME_NOTIFY(nIMECmd, uData)		MPX_WM_IME_NOTIFY(wParam, lParam, nIMECmd, uData)
#define TMPX_WM_IME_CONTROL(nIMECmd, uData)		MPX_WM_IME_CONTROL(wParam, lParam, nIMECmd, uData)
#define TMPX_WM_IME_COMPOSITIONFULL()			MPX_WM_IME_COMPOSITIONFULL(wParam, lParam)
#define TMPX_WM_IME_REQUEST(nIMECmd, uData)		MPX_WM_IME_REQUEST(wParam, lParam, nIMECmd, uData)
#define TMPX_WM_NCMOUSEHOVER(iHitCode, x, y)	MPX_WM_NCMOUSEHOVER(wParam, lParam, iHitCode, x, y)
#define TMPX_WM_MOUSEHOVER(uVirtualKey, x, y)	MPX_WM_MOUSEHOVER(wParam, lParam, uVirtualKey, x, y)
#define TMPX_WM_NCMOUSELEAVE()					MPX_WM_NCMOUSELEAVE(wParam, lParam)
#define TMPX_WM_MOUSELEAVE()					MPX_WM_MOUSELEAVE(wParam, lParam)
#define TMPX_WM_WTSSESSION_CHANGE(uStatusCode, dwSessionId) \
												MPX_WM_WTSSESSION_CHANGE(wParam, lParam, uStatusCode, dwSessionId)
#define TMPX_WM_CUT()							MPX_WM_CUT(wParam, lParam)
#define TMPX_WM_COPY()							MPX_WM_COPY(wParam, lParam)
#define TMPX_WM_PASTE()							MPX_WM_PASTE(wParam, lParam)
#define TMPX_WM_CLEAR()							MPX_WM_CLEAR(wParam, lParam)
#define TMPX_WM_UNDO()							MPX_WM_UNDO(wParam, lParam)
#define TMPX_WM_RENDERFORMAT(iClipboardFormat)	MPX_WM_RENDERFORMAT(wParam, lParam, iClipboardFormat)
#define TMPX_WM_RENDERALLFORMATS()				MPX_WM_RENDERALLFORMATS(wParam, lParam)
#define TMPX_WM_DESTROYCLIPBOARD()				MPX_WM_DESTROYCLIPBOARD(wParam, lParam)
#define TMPX_WM_DRAWCLIPBOARD()					MPX_WM_DRAWCLIPBOARD(wParam, lParam)
#define TMPX_WM_PAINTCLIPBOARD(hwndViewer, hgblPaintStruct) \
												MPX_WM_PAINTCLIPBOARD(wParam, lParam, hwndViewer, hgblPaintStruct)
#define TMPX_WM_VSCROLLCLIPBOARD(hwndViewer, uSBCode, nPos) \
												MPX_WM_VSCROLLCLIPBOARD(wParam, lParam, hwndViewer, uSBCode, nPos)
#define TMPX_WM_SIZECLIPBOARD(hwndViewer, hglbRect) \
												MPX_WM_SIZECLIPBOARD(wParam, lParam, hwndViewer, hglbRect)
#define TMPX_WM_ASKCBFORMATNAME(cchSize, pszFormatName) \
												MPX_WM_ASKCBFORMATNAME(wParam, lParam, cchSize, pszFormatName)
#define TMPX_WM_CHANGECBCHAIN(hwndRemove, hwndNext) \
												MPX_WM_CHANGECBCHAIN(wParam, lParam, hwndRemove, hwndNext)
#define TMPX_WM_HSCROLLCLIPBOARD(hwndViewer, uSBCode, nPos) \
												MPX_WM_HSCROLLCLIPBOARD(wParam, lParam, hwndViewer, uSBCode, nPos)
#define TMPX_WM_QUERYNEWPALETTE()				MPX_WM_QUERYNEWPALETTE(wParam, lParam)
#define TMPX_WM_PALETTEISCHANGING(hwndRealize)	MPX_WM_PALETTEISCHANGING(wParam, lParam, hwndRealize)
#define TMPX_WM_PALETTECHANGED(hwndSource)		MPX_WM_PALETTECHANGED(wParam, lParam, hwndSource)
#define TMPX_WM_HOTKEY(iHotkeyId, uKey, nFlags)	MPX_WM_HOTKEY(wParam, lParam, iHotkeyId, uKey, nFlags)
#define TMPX_WM_PRINT(hdc, uDrawOptions)		MPX_WM_PRINT(wParam, lParam, hdc, uDrawOptions)
#define TMPX_WM_PRINTCLIENT(hdc, uDrawOptions)	MPX_WM_PRINTCLIENT(wParam, lParam, hdc, uDrawOptions)

#endif // __MPX_H__
