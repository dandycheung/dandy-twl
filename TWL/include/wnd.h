//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __WND_H__
#define __WND_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <windows.h>
#include <tchar.h>

#include "modutil.h"
#include "trcdbg.h"

namespace TWL
{

class CWindow
{
public:
	static RECT rcDefault;
	HWND m_hWnd;

	CWindow(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CWindow& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	static LPCTSTR GetWndClassName()
	{
		return NULL;
	}

	void Attach(HWND hWndNew)
	{
		ASSERT(::IsWindow(hWndNew));
		m_hWnd = hWndNew;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	HWND Create(LPCTSTR lpstrWndClass, HWND hWndParent, RECT& rcPos, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			UINT nID = 0, LPVOID lpCreateParam = NULL, HINSTANCE hInstance = GetModuleInstance())
	{
		m_hWnd = ::CreateWindowEx(dwExStyle, lpstrWndClass, szWindowName,
			dwStyle, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
			rcPos.bottom - rcPos.top, hWndParent, (HMENU)nID,
			hInstance, lpCreateParam);
		return m_hWnd;
	}

	HWND Create(LPCTSTR lpstrWndClass, HWND hWndParent, LPRECT lpRect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			HMENU hMenu = NULL, LPVOID lpCreateParam = NULL, HINSTANCE hInstance = GetModuleInstance())
	{
		if(lpRect == NULL)
			lpRect = &rcDefault;
		m_hWnd = ::CreateWindowEx(dwExStyle, lpstrWndClass, szWindowName,
			dwStyle, lpRect->left, lpRect->top, lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top, hWndParent, hMenu,
			hInstance, lpCreateParam);
		return m_hWnd;
	}

	BOOL DestroyWindow()
	{
		ASSERT(::IsWindow(m_hWnd));

		if(!::DestroyWindow(m_hWnd))
			return FALSE;

		m_hWnd = NULL;
		return TRUE;
	}

// Attributes

	operator HWND() const { return m_hWnd; }

	DWORD GetStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	DWORD GetExStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	LONG GetWindowLong(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowLong(m_hWnd, nIndex);
	}

	LONG SetWindowLong(int nIndex, LONG dwNewLong)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowLong(m_hWnd, nIndex, dwNewLong);
	}

	WORD GetWindowWord(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowWord(m_hWnd, nIndex);
	}

	WORD SetWindowWord(int nIndex, WORD wNewWord)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowWord(m_hWnd, nIndex, wNewWord);
	}

// Message Functions

	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,message,wParam,lParam);
	}

	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::PostMessage(m_hWnd,message,wParam,lParam);
	}

	BOOL SendNotifyMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SendNotifyMessage(m_hWnd, message, wParam, lParam);
	}

	// support for C style macros
	static LRESULT SendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ASSERT(::IsWindow(hWnd));
		return ::SendMessage(hWnd, message, wParam, lParam);
	}

// Window Text Functions

	BOOL SetWindowText(LPCTSTR lpszString)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowText(m_hWnd, lpszString);
	}

	int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowText(m_hWnd, lpszStringBuf, nMaxCount);
	}

	int GetWindowTextLength() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowTextLength(m_hWnd);
	}

// Font Functions

	void SetFont(HFONT hFont, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}

	HFONT GetFont() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
	}

// Menu Functions (non-child windows only)

	HMENU GetMenu() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetMenu(m_hWnd);
	}

	BOOL SetMenu(HMENU hMenu)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetMenu(m_hWnd, hMenu);
	}

	BOOL DrawMenuBar()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DrawMenuBar(m_hWnd);
	}

	HMENU GetSystemMenu(BOOL bRevert) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetSystemMenu(m_hWnd, bRevert);
	}

	BOOL HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::HiliteMenuItem(m_hWnd, hMenu, uItemHilite, uHilite);
	}

// Window Size and Position Functions

	BOOL IsIconic() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsIconic(m_hWnd);
	}

	BOOL IsZoomed() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsZoomed(m_hWnd);
	}

	BOOL MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
	}

	BOOL MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
	}

	BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, nFlags);
	}

	UINT ArrangeIconicWindows()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ArrangeIconicWindows(m_hWnd);
	}

	BOOL BringWindowToTop()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::BringWindowToTop(m_hWnd);
	}

	BOOL GetWindowRect(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowRect(m_hWnd, lpRect);
	}

	BOOL GetClientRect(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetClientRect(m_hWnd, lpRect);
	}

	BOOL GetWindowPlacement(WINDOWPLACEMENT FAR* lpwndpl) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowPlacement(m_hWnd, lpwndpl);
	}

	BOOL SetWindowPlacement(const WINDOWPLACEMENT FAR* lpwndpl)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPlacement(m_hWnd, lpwndpl);
	}

// Coordinate Mapping Functions

	BOOL ClientToScreen(LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ClientToScreen(m_hWnd, lpPoint);
	}

	BOOL ClientToScreen(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		if(!::ClientToScreen(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);
	}

	BOOL ScreenToClient(LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ScreenToClient(m_hWnd, lpPoint);
	}

	BOOL ScreenToClient(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		if(!::ScreenToClient(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
	}

	int MapWindowPoints(HWND hWndTo, LPPOINT lpPoint, UINT nCount) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, lpPoint, nCount);
	}

	int MapWindowPoints(HWND hWndTo, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)lpRect, 2);
	}

// Update and Painting Functions

	HDC BeginPaint(LPPAINTSTRUCT lpPaint)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::BeginPaint(m_hWnd, lpPaint);
	}

	void EndPaint(LPPAINTSTRUCT lpPaint)
	{
		ASSERT(::IsWindow(m_hWnd));
		::EndPaint(m_hWnd, lpPaint);
	}

	HDC GetDC()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDC(m_hWnd);
	}

	HDC GetWindowDC()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowDC(m_hWnd);
	}

	int ReleaseDC(HDC hDC)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ReleaseDC(m_hWnd, hDC);
	}

	void Print(HDC hDC, DWORD dwFlags) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PRINT, (WPARAM)hDC, dwFlags);
	}

	void PrintClient(HDC hDC, DWORD dwFlags) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)hDC, dwFlags);
	}

	BOOL UpdateWindow()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::UpdateWindow(m_hWnd);
	}

	void SetRedraw(BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
	}

	BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetUpdateRect(m_hWnd, lpRect, bErase);
	}

	int GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetUpdateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL Invalidate(BOOL bErase = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, NULL, bErase);
	}

	BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	BOOL ValidateRect(LPCRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ValidateRect(m_hWnd, lpRect);
	}

	void InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL ValidateRgn(HRGN hRgn)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ValidateRgn(m_hWnd, hRgn);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL IsWindowVisible() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsWindowVisible(m_hWnd);
	}

	BOOL ShowOwnedPopups(BOOL bShow = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ShowOwnedPopups(m_hWnd, bShow);
	}

	HDC GetDCEx(HRGN hRgnClip, DWORD flags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDCEx(m_hWnd, hRgnClip, flags);
	}

	BOOL LockWindowUpdate(BOOL bLock = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::LockWindowUpdate(bLock ? m_hWnd : NULL);
	}

	BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgnUpdate = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgnUpdate, flags);
	}

// Timer Functions

	UINT SetTimer(UINT nIDEvent, UINT nElapse)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, NULL);
	}

	BOOL KillTimer(UINT nIDEvent)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::KillTimer(m_hWnd, nIDEvent);
	}

// Window State Functions

	BOOL IsWindowEnabled() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsWindowEnabled(m_hWnd);
	}

	BOOL EnableWindow(BOOL bEnable = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::EnableWindow(m_hWnd, bEnable);
	}

	HWND SetActiveWindow()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetActiveWindow(m_hWnd);
	}

	HWND SetCapture()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetCapture(m_hWnd);
	}

	HWND SetFocus()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetFocus(m_hWnd);
	}

// Dialog-Box Item Functions

	BOOL CheckDlgButton(int nIDButton, UINT nCheck)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::CheckDlgButton(m_hWnd, nIDButton, nCheck);
	}

	BOOL CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}

	int DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT nFileType)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DlgDirList(m_hWnd, lpPathSpec, nIDListBox, nIDStaticPath, nFileType);
	}

	int DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT nFileType)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DlgDirListComboBox(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);
	}

	BOOL DlgDirSelect(LPTSTR lpString, int nCount, int nIDListBox)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DlgDirSelectEx(m_hWnd, lpString, nCount, nIDListBox);
	}

	BOOL DlgDirSelectComboBox(LPTSTR lpString, int nCount, int nIDComboBox)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DlgDirSelectComboBoxEx(m_hWnd, lpString, nCount, nIDComboBox);
	}

	UINT GetDlgItemInt(int nID, BOOL* lpTrans = NULL, BOOL bSigned = TRUE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDlgItemInt(m_hWnd, nID, lpTrans, bSigned);
	}

	UINT GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDlgItemText(m_hWnd, nID, lpStr, nMaxCount);
	}

	HWND GetNextDlgGroupItem(HWND hWndCtl, BOOL bPrevious = FALSE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetNextDlgGroupItem(m_hWnd, hWndCtl, bPrevious);
	}

	HWND GetNextDlgTabItem(HWND hWndCtl, BOOL bPrevious = FALSE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetNextDlgTabItem(m_hWnd, hWndCtl, bPrevious);
	}

	UINT IsDlgButtonChecked(int nIDButton) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsDlgButtonChecked(m_hWnd, nIDButton);
	}

	LRESULT SendDlgItemMessage(int nID, UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SendDlgItemMessage(m_hWnd, nID, message, wParam, lParam);
	}

	BOOL SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetDlgItemInt(m_hWnd, nID, nValue, bSigned);
	}

	BOOL SetDlgItemText(int nID, LPCTSTR lpszString)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetDlgItemText(m_hWnd, nID, lpszString);
	}

// Scrolling Functions

	int GetScrollPos(int nBar) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetScrollPos(m_hWnd, nBar);
	}

	BOOL GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
	}

	BOOL ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = NULL, LPCRECT lpClipRect = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindow(m_hWnd, xAmount, yAmount, lpRect, lpClipRect);
	}

	int ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate, UINT uFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int ScrollWindowEx(int dx, int dy, UINT uFlags, LPCRECT lpRectScroll = NULL, LPCRECT lpRectClip = NULL, HRGN hRgnUpdate = NULL, LPRECT lpRectUpdate = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	BOOL SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	BOOL ShowScrollBar(UINT nBar, BOOL bShow = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}

	BOOL EnableScrollBar(UINT uSBFlags, UINT uArrowFlags = ESB_ENABLE_BOTH)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::EnableScrollBar(m_hWnd, uSBFlags, uArrowFlags);
	}

// Window Access Functions

	HWND ChildWindowFromPoint(POINT point) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ChildWindowFromPoint(m_hWnd, point);
	}

	HWND ChildWindowFromPointEx(POINT point, UINT uFlags) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ChildWindowFromPointEx(m_hWnd, point, uFlags);
	}

	HWND GetTopWindow() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetTopWindow(m_hWnd);
	}

	HWND GetWindow(UINT nCmd) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindow(m_hWnd, nCmd);
	}

	HWND GetLastActivePopup() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetLastActivePopup(m_hWnd);
	}

	BOOL IsChild(HWND hWnd) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsChild(m_hWnd, hWnd);
	}

	HWND GetParent() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetParent(m_hWnd);
	}

	HWND SetParent(HWND hWndNewParent)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetParent(m_hWnd, hWndNewParent);
	}

// Window Tree Access

	int GetDlgCtrlID() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDlgCtrlID(m_hWnd);
	}

	int SetDlgCtrlID(int nID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SetWindowLong(m_hWnd, GWL_ID, nID);
	}

	HWND GetDlgItem(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetDlgItem(m_hWnd, nID);
	}

// Alert Functions

	BOOL FlashWindow(BOOL bInvert)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::FlashWindow(m_hWnd, bInvert);
	}

	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = _T(""), UINT nType = MB_OK)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
	}

// Clipboard Functions

	BOOL ChangeClipboardChain(HWND hWndNewNext)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ChangeClipboardChain(m_hWnd, hWndNewNext);
	}

	HWND SetClipboardViewer()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetClipboardViewer(m_hWnd);
	}

	BOOL OpenClipboard()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::OpenClipboard(m_hWnd);
	}

// Caret Functions

	BOOL CreateCaret(HBITMAP hBitmap)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, hBitmap, 0, 0);
	}

	BOOL CreateSolidCaret(int nWidth, int nHeight)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, (HBITMAP)0, nWidth, nHeight);
	}

	BOOL CreateGrayCaret(int nWidth, int nHeight)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, (HBITMAP)1, nWidth, nHeight);
	}

	BOOL HideCaret()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::HideCaret(m_hWnd);
	}

	BOOL ShowCaret()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ShowCaret(m_hWnd);
	}

#ifdef _INC_SHELLAPI
// Drag-Drop Functions
	void DragAcceptFiles(BOOL bAccept = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::DragAcceptFiles(m_hWnd, bAccept);
	}
#endif

// Icon Functions

	HICON SetIcon(HICON hIcon, BOOL bBigIcon = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
	}

	HICON GetIcon(BOOL bBigIcon = TRUE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_GETICON, bBigIcon, 0);
	}

// Help Functions

	BOOL WinHelp(LPCTSTR lpszHelp, UINT nCmd = HELP_CONTEXT, DWORD dwData = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::WinHelp(m_hWnd, lpszHelp, nCmd, dwData);
	}

	BOOL SetWindowContextHelpId(DWORD dwContextHelpId)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowContextHelpId(m_hWnd, dwContextHelpId);
	}

	DWORD GetWindowContextHelpId() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowContextHelpId(m_hWnd);
	}

// Hot Key Functions

	int SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, WM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0);
	}

	DWORD GetHotKey() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd, WM_GETHOTKEY, 0, 0);
	}

// Misc. Operations

//N new
	BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetScrollInfo(m_hWnd, nBar, lpScrollInfo);
	}
	BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetScrollInfo(m_hWnd, nBar, lpScrollInfo, bRedraw);
	}
	BOOL IsDialogMessage(LPMSG lpMsg)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsDialogMessage(m_hWnd, lpMsg);
	}

	void NextDlgCtrl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, 0, 0L);
	}
	void PrevDlgCtrl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, 1, 0L);
	}
	void GotoDlgCtrl(HWND hWndCtrl) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)hWndCtrl, 1L);
	}

	BOOL ResizeClient(int nWidth, int nHeight, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));

		RECT rcWnd;
		if(!GetClientRect(&rcWnd))
			return FALSE;

		if(nWidth != -1)
			rcWnd.right = nWidth;
		if(nHeight != -1)
			rcWnd.bottom = nHeight;

		if(!::AdjustWindowRectEx(&rcWnd, GetStyle(), (!(GetStyle() & WS_CHILD) && (GetMenu() != NULL)), GetExStyle()))
			return FALSE;

		UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
		if(!bRedraw)
			uFlags |= SWP_NOREDRAW;

		return SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
	}

	int GetWindowRgn(HRGN hRgn)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowRgn(m_hWnd, hRgn);
	}
	int SetWindowRgn(HRGN hRgn, BOOL bRedraw = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}
	HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}
	DWORD GetWindowThreadID()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetWindowThreadProcessId(m_hWnd, NULL);
	}
	DWORD GetWindowProcessID()
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwProcessID;
		::GetWindowThreadProcessId(m_hWnd, &dwProcessID);
		return dwProcessID;
	}
	BOOL IsWindow()
	{
		return ::IsWindow(m_hWnd);
	}
	BOOL IsWindowUnicode()
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::IsWindowUnicode(m_hWnd);
	}
	BOOL IsParentDialog()
	{
		ASSERT(::IsWindow(m_hWnd));
		TCHAR szBuf[8]; // "#32770" + NUL character
		GetClassName(GetParent(), szBuf, sizeof(szBuf)/sizeof(TCHAR));
		return lstrcmp(szBuf, _T("#32770")) == 0;
	}
	BOOL ShowWindowAsync(int nCmdShow)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::ShowWindowAsync(m_hWnd, nCmdShow);
	}

	HWND GetDescendantWindow(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));

		// GetDlgItem recursive (return first found)
		// breadth-first for 1 level, then depth-first for next level

		// use GetDlgItem since it is a fast USER function
		HWND hWndChild, hWndTmp;
		CWindow wnd;
		if((hWndChild = ::GetDlgItem(m_hWnd, nID)) != NULL)
		{
			if(::GetTopWindow(hWndChild) != NULL)
			{
				// children with the same ID as their parent have priority
				wnd.Attach(hWndChild);
				hWndTmp = wnd.GetDescendantWindow(nID);
				if(hWndTmp != NULL)
					return hWndTmp;
			}
			return hWndChild;
		}

		// walk each child
		for(hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			wnd.Attach(hWndChild);
			hWndTmp = wnd.GetDescendantWindow(nID);
			if(hWndTmp != NULL)
				return hWndTmp;
		}

		return NULL;    // not found
	}

	void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE)
	{
		CWindow wnd;
		for(HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			::SendMessage(hWndChild, message, wParam, lParam);

			if(bDeep && ::GetTopWindow(hWndChild) != NULL)
			{
				// send to child windows after parent
				wnd.Attach(hWndChild);
				wnd.SendMessageToDescendants(message, wParam, lParam, bDeep);
			}
		}
	}

	BOOL CenterWindow(HWND hWndCenter = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));

		// determine owner window to center against
		DWORD dwStyle = GetStyle();
		if(hWndCenter == NULL)
		{
			if(dwStyle & WS_CHILD)
				hWndCenter = ::GetParent(m_hWnd);
			else
				hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
		}

		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect(m_hWnd, &rcDlg);
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;
		if(!(dwStyle & WS_CHILD))
		{
			// don't center against invisible or minimized windows
			if(hWndCenter != NULL)
			{
				DWORD dwStyle = ::GetWindowLong(hWndCenter, GWL_STYLE);
				if(!(dwStyle & WS_VISIBLE) || (dwStyle & WS_MINIMIZE))
					hWndCenter = NULL;
			}

			// center within screen coordinates
			::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
			if(hWndCenter == NULL)
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);
		}
		else
		{
			// center within parent client coordinates
			hWndParent = ::GetParent(m_hWnd);
			ASSERT(::IsWindow(hWndParent));

			::GetClientRect(hWndParent, &rcArea);
			ASSERT(::IsWindow(hWndCenter));
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if(xLeft < rcArea.left)
			xLeft = rcArea.left;
		else if(xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;

		if(yTop < rcArea.top)
			yTop = rcArea.top;
		else if(yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;

		// map screen coordinates to child coordinates
		return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		ASSERT(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		ASSERT(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	HWND GetTopLevelParent() const
	{
		ASSERT(::IsWindow(m_hWnd));

		HWND hWndParent = m_hWnd;
		HWND hWndTmp;
		while((hWndTmp = ::GetParent(hWndParent)) != NULL)
			hWndParent = hWndTmp;

		return hWndParent;
	}

	HWND GetTopLevelWindow() const
	{
		ASSERT(::IsWindow(m_hWnd));

		HWND hWndParent;
		HWND hWndTmp = m_hWnd;

		do
		{
			hWndParent = hWndTmp;
			hWndTmp = (::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWndParent) : ::GetWindow(hWndParent, GW_OWNER);
		}
		while(hWndTmp != NULL);

		return hWndParent;
	}
};

_declspec(selectany) RECT CWindow::rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };

// foreward declares
class CPoint;
class CRect;

/////////////////////////////////////////////////////////////////////////////
// CSize - Wrapper for Windows SIZE structure.

class CSize : public tagSIZE
{
public:
// Constructors
	CSize();
	CSize(int initCX, int initCY);
	CSize(SIZE initSize);
	CSize(POINT initPt);
	CSize(DWORD dwSize);

// Operations
	BOOL operator==(SIZE size) const;
	BOOL operator!=(SIZE size) const;
	void operator+=(SIZE size);
	void operator-=(SIZE size);
	void SetSize(int CX, int CY);

// Operators returning CSize values
	CSize operator+(SIZE size) const;
	CSize operator-(SIZE size) const;
	CSize operator-() const;

// Operators returning CPoint values
	CPoint operator+(POINT point) const;
	CPoint operator-(POINT point) const;

// Operators returning CRect values
	CRect operator+(const RECT* lpRect) const;
	CRect operator-(const RECT* lpRect) const;
};


/////////////////////////////////////////////////////////////////////////////
// CPoint - Wrapper for Windows POINT structure.

class CPoint : public tagPOINT
{
public:
// Constructors
	CPoint();
	CPoint(int initX, int initY);
	CPoint(POINT initPt);
	CPoint(SIZE initSize);
	CPoint(DWORD dwPoint);

// Operations
	void Offset(int xOffset, int yOffset);
	void Offset(POINT point);
	void Offset(SIZE size);
	BOOL operator==(POINT point) const;
	BOOL operator!=(POINT point) const;
	void operator+=(SIZE size);
	void operator-=(SIZE size);
	void operator+=(POINT point);
	void operator-=(POINT point);
	void SetPoint(int X, int Y);

// Operators returning CPoint values
	CPoint operator+(SIZE size) const;
	CPoint operator-(SIZE size) const;
	CPoint operator-() const;
	CPoint operator+(POINT point) const;

// Operators returning CSize values
	CSize operator-(POINT point) const;

// Operators returning CRect values
	CRect operator+(const RECT* lpRect) const;
	CRect operator-(const RECT* lpRect) const;
};


/////////////////////////////////////////////////////////////////////////////
// CRect - Wrapper for Windows RECT structure.

class CRect : public tagRECT
{
public:
// Constructors
	CRect();
	CRect(int l, int t, int r, int b);
	CRect(const RECT& srcRect);
	CRect(LPCRECT lpSrcRect);
	CRect(POINT point, SIZE size);
	CRect(POINT topLeft, POINT bottomRight);

// Attributes (in addition to RECT members)
	int Width() const;
	int Height() const;
	CSize Size() const;
	CPoint& TopLeft();
	CPoint& BottomRight();
	const CPoint& TopLeft() const;
	const CPoint& BottomRight() const;
	CPoint CenterPoint() const;

	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT();
	operator LPCRECT() const;

	BOOL IsRectEmpty() const;
	BOOL IsRectNull() const;
	BOOL PtInRect(POINT point) const;

// Operations
	void SetRect(int x1, int y1, int x2, int y2);
	void SetRect(POINT topLeft, POINT bottomRight);
	void SetRectEmpty();
	void CopyRect(LPCRECT lpSrcRect);
	BOOL EqualRect(LPCRECT lpRect) const;

	void InflateRect(int x, int y);
	void InflateRect(SIZE size);
	void InflateRect(LPCRECT lpRect);
	void InflateRect(int l, int t, int r, int b);
	void DeflateRect(int x, int y);
	void DeflateRect(SIZE size);
	void DeflateRect(LPCRECT lpRect);
	void DeflateRect(int l, int t, int r, int b);

	void OffsetRect(int x, int y);
	void OffsetRect(SIZE size);
	void OffsetRect(POINT point);
	void NormalizeRect();

	// absolute position of rectangle
	void MoveToY(int y);
	void MoveToX(int x);
	void MoveToXY(int x, int y);
	void MoveToXY(POINT point);

	// operations that fill '*this' with result
	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2);
	BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2);
	BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2);

// Additional Operations
	void operator=(const RECT& srcRect);
	BOOL operator==(const RECT& rect) const;
	BOOL operator!=(const RECT& rect) const;
	void operator+=(POINT point);
	void operator+=(SIZE size);
	void operator+=(LPCRECT lpRect);
	void operator-=(POINT point);
	void operator-=(SIZE size);
	void operator-=(LPCRECT lpRect);
	void operator&=(const RECT& rect);
	void operator|=(const RECT& rect);

// Operators returning CRect values
	CRect operator+(POINT point) const;
	CRect operator-(POINT point) const;
	CRect operator+(LPCRECT lpRect) const;
	CRect operator+(SIZE size) const;
	CRect operator-(SIZE size) const;
	CRect operator-(LPCRECT lpRect) const;
	CRect operator&(const RECT& rect2) const;
	CRect operator|(const RECT& rect2) const;
	CRect MulDiv(int nMultiplier, int nDivisor) const;
};


/////////////////////////////////////////////////////////////////////////////
// CSize, CPoint, CRect Implementation

// CSize
inline CSize::CSize()
	{ /* random filled */ }
inline CSize::CSize(int initCX, int initCY)
	{ cx = initCX; cy = initCY; }
inline CSize::CSize(SIZE initSize)
	{ *(SIZE*)this = initSize; }
inline CSize::CSize(POINT initPt)
	{ *(POINT*)this = initPt; }
inline CSize::CSize(DWORD dwSize)
	{
		cx = (short)LOWORD(dwSize);
		cy = (short)HIWORD(dwSize);
	}
inline BOOL CSize::operator==(SIZE size) const
	{ return (cx == size.cx && cy == size.cy); }
inline BOOL CSize::operator!=(SIZE size) const
	{ return (cx != size.cx || cy != size.cy); }
inline void CSize::operator+=(SIZE size)
	{ cx += size.cx; cy += size.cy; }
inline void CSize::operator-=(SIZE size)
	{ cx -= size.cx; cy -= size.cy; }
inline void CSize::SetSize(int CX, int CY)
	{ cx = CX; cy = CY; }	
inline CSize CSize::operator+(SIZE size) const
	{ return CSize(cx + size.cx, cy + size.cy); }
inline CSize CSize::operator-(SIZE size) const
	{ return CSize(cx - size.cx, cy - size.cy); }
inline CSize CSize::operator-() const
	{ return CSize(-cx, -cy); }
inline CPoint CSize::operator+(POINT point) const
	{ return CPoint(cx + point.x, cy + point.y); }
inline CPoint CSize::operator-(POINT point) const
	{ return CPoint(cx - point.x, cy - point.y); }
inline CRect CSize::operator+(const RECT* lpRect) const
	{ return CRect(lpRect) + *this; }
inline CRect CSize::operator-(const RECT* lpRect) const
	{ return CRect(lpRect) - *this; }

// CPoint
inline CPoint::CPoint()
	{ /* random filled */ }
inline CPoint::CPoint(int initX, int initY)
	{ x = initX; y = initY; }
inline CPoint::CPoint(POINT initPt)
	{ *(POINT*)this = initPt; }
inline CPoint::CPoint(SIZE initSize)
	{ *(SIZE*)this = initSize; }
inline CPoint::CPoint(DWORD dwPoint)
	{
		x = (short)LOWORD(dwPoint);
		y = (short)HIWORD(dwPoint);
	}
inline void CPoint::Offset(int xOffset, int yOffset)
	{ x += xOffset; y += yOffset; }
inline void CPoint::Offset(POINT point)
	{ x += point.x; y += point.y; }
inline void CPoint::Offset(SIZE size)
	{ x += size.cx; y += size.cy; }
inline BOOL CPoint::operator==(POINT point) const
	{ return (x == point.x && y == point.y); }
inline BOOL CPoint::operator!=(POINT point) const
	{ return (x != point.x || y != point.y); }
inline void CPoint::operator+=(SIZE size)
	{ x += size.cx; y += size.cy; }
inline void CPoint::operator-=(SIZE size)
	{ x -= size.cx; y -= size.cy; }
inline void CPoint::operator+=(POINT point)
	{ x += point.x; y += point.y; }
inline void CPoint::operator-=(POINT point)
	{ x -= point.x; y -= point.y; }
inline void CPoint::SetPoint(int X, int Y)
	{ x = X; y = Y; }
inline CPoint CPoint::operator+(SIZE size) const
	{ return CPoint(x + size.cx, y + size.cy); }
inline CPoint CPoint::operator-(SIZE size) const
	{ return CPoint(x - size.cx, y - size.cy); }
inline CPoint CPoint::operator-() const
	{ return CPoint(-x, -y); }
inline CPoint CPoint::operator+(POINT point) const
	{ return CPoint(x + point.x, y + point.y); }
inline CSize CPoint::operator-(POINT point) const
	{ return CSize(x - point.x, y - point.y); }
inline CRect CPoint::operator+(const RECT* lpRect) const
	{ return CRect(lpRect) + *this; }
inline CRect CPoint::operator-(const RECT* lpRect) const
	{ return CRect(lpRect) - *this; }

// CRect
inline CRect::CRect()
	{ /* random filled */ }
inline CRect::CRect(int l, int t, int r, int b)
	{ left = l; top = t; right = r; bottom = b; }
inline CRect::CRect(const RECT& srcRect)
	{ ::CopyRect(this, &srcRect); }
inline CRect::CRect(LPCRECT lpSrcRect)
	{ ::CopyRect(this, lpSrcRect); }
inline CRect::CRect(POINT point, SIZE size)
	{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
inline CRect::CRect(POINT topLeft, POINT bottomRight)
	{ left = topLeft.x; top = topLeft.y;
		right = bottomRight.x; bottom = bottomRight.y; }
inline int CRect::Width() const
	{ return right - left; }
inline int CRect::Height() const
	{ return bottom - top; }
inline CSize CRect::Size() const
	{ return CSize(right - left, bottom - top); }
inline CPoint& CRect::TopLeft()
	{ return *((CPoint*)this); }
inline CPoint& CRect::BottomRight()
	{ return *((CPoint*)this + 1); }
inline const CPoint& CRect::TopLeft() const
	{ return *((CPoint*)this); }
inline const CPoint& CRect::BottomRight() const
	{ return *((CPoint*)this + 1); }
inline CPoint CRect::CenterPoint() const
	{ return CPoint((left + right) / 2, (top + bottom) / 2); }
inline CRect::operator LPRECT()
	{ return this; }
inline CRect::operator LPCRECT() const
	{ return this; }
inline BOOL CRect::IsRectEmpty() const
	{ return ::IsRectEmpty(this); }
inline BOOL CRect::IsRectNull() const
	{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
inline BOOL CRect::PtInRect(POINT point) const
	{ return ::PtInRect(this, point); }
inline void CRect::SetRect(int x1, int y1, int x2, int y2)
	{ ::SetRect(this, x1, y1, x2, y2); }
inline void CRect::SetRect(POINT topLeft, POINT bottomRight)
	{ ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
inline void CRect::SetRectEmpty()
	{ ::SetRectEmpty(this); }
inline void CRect::CopyRect(LPCRECT lpSrcRect)
	{ ::CopyRect(this, lpSrcRect); }
inline BOOL CRect::EqualRect(LPCRECT lpRect) const
	{ return ::EqualRect(this, lpRect); }
inline void CRect::InflateRect(int x, int y)
	{ ::InflateRect(this, x, y); }
inline void CRect::InflateRect(SIZE size)
	{ ::InflateRect(this, size.cx, size.cy); }
inline void CRect::DeflateRect(int x, int y)
	{ ::InflateRect(this, -x, -y); }
inline void CRect::DeflateRect(SIZE size)
	{ ::InflateRect(this, -size.cx, -size.cy); }
inline void CRect::OffsetRect(int x, int y)
	{ ::OffsetRect(this, x, y); }
inline void CRect::OffsetRect(POINT point)
	{ ::OffsetRect(this, point.x, point.y); }
inline void CRect::OffsetRect(SIZE size)
	{ ::OffsetRect(this, size.cx, size.cy); }
inline BOOL CRect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{ return ::IntersectRect(this, lpRect1, lpRect2);}
inline BOOL CRect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{ return ::UnionRect(this, lpRect1, lpRect2); }
inline void CRect::operator=(const RECT& srcRect)
	{ ::CopyRect(this, &srcRect); }
inline BOOL CRect::operator==(const RECT& rect) const
	{ return ::EqualRect(this, &rect); }
inline BOOL CRect::operator!=(const RECT& rect) const
	{ return !::EqualRect(this, &rect); }
inline void CRect::operator+=(POINT point)
	{ ::OffsetRect(this, point.x, point.y); }
inline void CRect::operator+=(SIZE size)
	{ ::OffsetRect(this, size.cx, size.cy); }
inline void CRect::operator+=(LPCRECT lpRect)
	{ InflateRect(lpRect); }
inline void CRect::operator-=(POINT point)
	{ ::OffsetRect(this, -point.x, -point.y); }
inline void CRect::operator-=(SIZE size)
	{ ::OffsetRect(this, -size.cx, -size.cy); }
inline void CRect::operator-=(LPCRECT lpRect)
	{ DeflateRect(lpRect); }
inline void CRect::operator&=(const RECT& rect)
	{ ::IntersectRect(this, this, &rect); }
inline void CRect::operator|=(const RECT& rect)
	{ ::UnionRect(this, this, &rect); }
inline CRect CRect::operator+(POINT pt) const
	{ CRect rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }
inline CRect CRect::operator-(POINT pt) const
	{ CRect rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }
inline CRect CRect::operator+(SIZE size) const
	{ CRect rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }
inline CRect CRect::operator-(SIZE size) const
	{ CRect rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }
inline CRect CRect::operator+(LPCRECT lpRect) const
	{ CRect rect(this); rect.InflateRect(lpRect); return rect; }
inline CRect CRect::operator-(LPCRECT lpRect) const
	{ CRect rect(this); rect.DeflateRect(lpRect); return rect; }
inline CRect CRect::operator&(const RECT& rect2) const
	{ CRect rect; ::IntersectRect(&rect, this, &rect2);
		return rect; }
inline CRect CRect::operator|(const RECT& rect2) const
	{ CRect rect; ::UnionRect(&rect, this, &rect2);
		return rect; }
inline BOOL CRect::SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
	{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }

inline void CRect::NormalizeRect()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

inline void CRect::MoveToY(int y)
	{ bottom = Height() + y; top = y; }
inline void CRect::MoveToX(int x)
	{ right = Width() + x; left = x; }
inline void CRect::MoveToXY(int x, int y)
	{ MoveToX(x); MoveToY(y); }
inline void CRect::MoveToXY(POINT pt)
	{ MoveToX(pt.x); MoveToY(pt.y); }

inline void CRect::InflateRect(LPCRECT lpRect)
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

inline void CRect::InflateRect(int l, int t, int r, int b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

inline void CRect::DeflateRect(LPCRECT lpRect)
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

inline void CRect::DeflateRect(int l, int t, int r, int b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

inline CRect CRect::MulDiv(int nMultiplier, int nDivisor) const
{
	return CRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}

}; // namespace TWL

#endif // __WND_H__
