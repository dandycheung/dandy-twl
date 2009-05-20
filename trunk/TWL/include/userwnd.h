//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __USERWND_H__
#define __USERWND_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef NO_USE_CWINDOW
#include "wnd.h"
#else // NO_USE_CWINDOW
#include <windows.h>
#include <tchar.h>

#include "trcdbg.h"
#endif // NO_USE_CWINDOW

// protect members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif //_INC_WINDOWSX

#include "modutil.h"
#include "tdc.h"

#define TWL_WND_PRIVATE_PROP	TEXT("TwlWndPrivateData")

namespace TWL
{

#ifndef OCM__BASE
#define OCM__BASE		(WM_USER+0x1c00)
#endif // !OCM__BASE

#define _countof(a)	(sizeof(a) / sizeof(a[0]))

class CUserWindow
#ifndef NO_USE_CWINDOW
	: public CWindow
#else // NO_USE_CWINDOW
#ifdef TWL_WNDBASE
	: TWL_WNDBASE
#endif // TWL_WNDBASE
#endif // NO_USE_CWINDOW
{
// data
public:
#ifdef NO_USE_CWINDOW
	HWND m_hWnd;
#endif

	WNDPROC m_wndProcOld;

// methods
public:
	CUserWindow()
#ifdef NO_USE_CWINDOW
		: m_hWnd(NULL)
#endif // NO_USE_CWINDOW
	{
		m_wndProcOld = NULL;
	}

	virtual ~CUserWindow()
	{
	}

	bool RegisterClass(LPCTSTR pszClassName, HINSTANCE hInstance, bool bRegister = true)
	{
		WNDCLASSEX wce = { sizeof(wce) };

		// Check if class is already registered
		if(!GetClassInfoEx(hInstance, pszClassName, &wce))
		{
			if(!bRegister)
				return false;

			wce.hInstance     = hInstance;
			wce.lpszClassName = pszClassName;

			SetWndClassEx(wce);				// fill class attributes

			if(!RegisterClassEx(&wce))		// register
				return false;
		}
		else
		{
			if(bRegister)
				return true;

			if(!UnregisterClass(pszClassName, hInstance))
				return false;
		}

		return true;
	}

	bool Create(DWORD dwExStyle, LPCTSTR pszWindowName, DWORD dwStyle, 
		int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu,
		HINSTANCE hInstance = GetModuleInstance(), PVOID pvCreateParam = NULL)
	{
		return CreateEx(dwExStyle, GetClassName(), pszWindowName, dwStyle, x, y, nWidth, nHeight,
			hParent, hMenu, hInstance, pvCreateParam);
	}

	bool Create(DWORD dwExStyle, LPCTSTR pszWindowName, DWORD dwStyle, 
		PRECT prc, HWND hParent, HMENU hMenu, HINSTANCE hInstance = GetModuleInstance(),
		PVOID pvCreateParam = NULL)
	{
		static RECT rcTopLevel = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		static RECT rcChild = { 0, 0, 0, 0 };

		if(prc == NULL)
			(dwStyle & WS_CHILD) ? (prc = &rcChild) : (prc = &rcTopLevel);

		return Create(dwExStyle, pszWindowName, dwStyle,
			prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top,
			hParent, hMenu, hInstance, pvCreateParam);
	}

	virtual BOOL SubclassWindow(HWND hWnd)
	{
		ASSERT(::IsWindow(hWnd));

		m_hWnd = hWnd;
		::SetProp(hWnd, TWL_WND_PRIVATE_PROP, (HANDLE)this);

		// now hook into the unique WndProc
		WNDPROC oldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WindowProc);
		if(oldWndProc == NULL)
		{
			::RemoveProp(hWnd, TWL_WND_PRIVATE_PROP);
			return FALSE;
		}

		ASSERT(oldWndProc != WindowProc);
		if(oldWndProc != WindowProc)
			m_wndProcOld = oldWndProc;

		return TRUE;
	}

	virtual HWND UnsubclassWindow(BOOL bForce = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(m_wndProcOld != NULL);

		if(m_wndProcOld == NULL)
			return NULL;

		WNDPROC wndProc = (WNDPROC)::GetWindowLong(m_hWnd, GWL_WNDPROC);

		HWND hWnd = NULL;
		if(bForce || wndProc == WindowProc)
		{
			// set WNDPROC back to original value
			if(::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_wndProcOld))
			{
				m_wndProcOld = NULL;

				// and Detach the HWND from the this object
				hWnd = m_hWnd;
				m_hWnd = NULL;

				::RemoveProp(hWnd, TWL_WND_PRIVATE_PROP);
			}
		}

		return hWnd;
	}

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CUserWindow* pWindow = (CUserWindow*)::GetProp(hWnd, TWL_WND_PRIVATE_PROP);

		if(pWindow == NULL)
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);

		bool bPrecessed = false;

		LRESULT lResult = 0;
		if(pWindow)
			bPrecessed = pWindow->ProcessMessage(uMsg, wParam, lParam, lResult);

		if(!bPrecessed)
			lResult = pWindow->DefWindowProc(uMsg, wParam, lParam);

		if(uMsg == WM_NCDESTROY)
		{
			// NOTE: Both of ATL and MFC do the unsubclassing work here, but I think
			//  that was meaningless.

			// NOTE: We should provide a chance for the 'delete this', so, here is a
			//  thick, we use a strange window handle
			pWindow->m_hWnd = HWND_BROADCAST;
			::RemoveProp(hWnd, TWL_WND_PRIVATE_PROP);
		}

		if(pWindow->m_hWnd == HWND_BROADCAST)
		{
			pWindow->m_hWnd = NULL;
			pWindow->OnFinalMessage(hWnd);
		}

		return lResult; 
	}

	virtual bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		return false;
	}

	// If your window is a frame, a MDI child or a dialog, you SHOULD override it because
	// you should pass unhandled message to DefFrameProc/DefMDIChildProc/DefDlgProc.
	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(m_wndProcOld)
			return CallWindowProc(m_wndProcOld, m_hWnd, uMsg, wParam, lParam);

		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	virtual OnFinalMessage(HWND hWnd)
	{
	}

	virtual void SetWndClassEx(WNDCLASSEX& wce)
	{
		wce.lpfnWndProc   = WindowProc;
		wce.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	}

	virtual LPCTSTR GetClassName() = 0;

	static LRESULT CALLBACK CbtHookProc(int code, WPARAM wParam, LPARAM lParam);

	bool CreateEx(DWORD dwExStyle, LPCTSTR pszClassName, LPCTSTR pszWindowName, DWORD dwStyle, 
		int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInstance, PVOID pvCreateParam)
	{
		if(!RegisterClass(pszClassName, hInstance))
			return false;

		HookWindowCreate(this);

		// fix the misuse of NULL instance handle when creating a window
		if(hInstance == NULL)
			hInstance = GetModuleInstance();

		// m_hWnd should be assigned value while processing WM_NCCREATE message
		CreateWindowEx(dwExStyle, pszClassName, 
			pszWindowName, dwStyle, x, y, nWidth, nHeight, 
			hParent, hMenu, hInstance, pvCreateParam);

		UnhookWindowCreate();

		return m_hWnd != NULL;
	}

	static bool HookWindowCreate(CUserWindow* pThis)
	{
		HHOOK hkCbt = ::SetWindowsHookEx(WH_CBT, CbtHookProc, NULL, ::GetCurrentThreadId());
		if(hkCbt == NULL)
			return false;

		AddThreadData(pThis);
		AddThreadData((void*)hkCbt, 1);

		return true;
	}

	static void UnhookWindowCreate()
	{
		HHOOK hkCbt = (HHOOK)ExtractThreadData(1);	// should be NULL
		PVOID pWindow = ExtractThreadData();		// should be NULL too

		if(hkCbt)
			::UnhookWindowsHookEx(hkCbt);
	}

	bool ReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
	bool ForwardNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
};

inline
LRESULT CALLBACK CUserWindow::CbtHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	HHOOK hkCbt = NULL;

	if(code == HCBT_CREATEWND)
	{
		ASSERT(lParam != NULL);
		LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
		ASSERT(lpcs != NULL);

		// Note: special check to avoid subclassing the IME window
		BOOL bDBCS = TRUE;
		if(bDBCS)
		{
			// check for cheap CS_IME style first...
			if(GetClassLong((HWND)wParam, GCL_STYLE) & CS_IME)
				goto lCallNextHook;

			// get class name of the window that is being created
			LPCTSTR pszClassName;
			TCHAR szClassName[_countof(_T("ime"))+1];
			if(HIWORD(lpcs->lpszClass))
			{
				pszClassName = lpcs->lpszClass;
			}
			else
			{
				szClassName[0] = _T('\0');
				GlobalGetAtomName((ATOM)lpcs->lpszClass, szClassName, _countof(szClassName));
				pszClassName = szClassName;
			}

			// a little more expensive to test this way, but necessary...
			if(lstrcmpi(pszClassName, _T("ime")) == 0)
				goto lCallNextHook;
		}

		ASSERT(wParam != NULL); // should be non-NULL HWND
		HWND hWnd = (HWND)wParam;

		hkCbt = (HHOOK)ExtractThreadData(1);
		CUserWindow* pWindow = (CUserWindow*)ExtractThreadData();
		ASSERT(pWindow != NULL);

		if(pWindow)
		{
			pWindow->m_hWnd = hWnd;
			::SetProp(hWnd, TWL_WND_PRIVATE_PROP, (HANDLE)pWindow);

			// subclass the window
			WNDPROC oldWndProc = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
			if(oldWndProc != NULL && oldWndProc != WindowProc)
			{
				pWindow->m_wndProcOld = oldWndProc;
				oldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WindowProc);
				ASSERT(oldWndProc != NULL);
			}
		}
	}

lCallNextHook:
	LRESULT lResult = CallNextHookEx(hkCbt, code, wParam, lParam);

	if(hkCbt)
		::UnhookWindowsHookEx(hkCbt);

	return lResult;
}

//////////////////////////////////////////////////////////////////////////

inline
bool CUserWindow::ReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	HWND hWndChild = NULL;

	switch(uMsg)
	{
	case WM_COMMAND:
		if(lParam != NULL)	// not from a menu
			hWndChild = (HWND)lParam;
		break;
	case WM_NOTIFY:
		hWndChild = ((LPNMHDR)lParam)->hwndFrom;
		break;
	case WM_PARENTNOTIFY:
		switch(LOWORD(wParam))
		{
		case WM_CREATE:
		case WM_DESTROY:
			hWndChild = (HWND)lParam;
			break;
		default:
			hWndChild = ::GetDlgItem(m_hWnd, HIWORD(wParam));
			break;
		}
		break;
	case WM_DRAWITEM:
		if(wParam)	// not from a menu
			hWndChild = ((LPDRAWITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_MEASUREITEM:
		if(wParam)	// not from a menu
			hWndChild = ::GetDlgItem(m_hWnd, ((LPMEASUREITEMSTRUCT)lParam)->CtlID);
		break;
	case WM_COMPAREITEM:
		if(wParam)	// not from a menu
			hWndChild = ::GetDlgItem(m_hWnd, ((LPCOMPAREITEMSTRUCT)lParam)->CtlID);
		break;
	case WM_DELETEITEM:
		if(wParam)	// not from a menu
			hWndChild = ::GetDlgItem(m_hWnd, ((LPDELETEITEMSTRUCT)lParam)->CtlID);
		break;
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
		hWndChild = (HWND)lParam;
		break;
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		hWndChild = (HWND)lParam;
		break;
	default:
		break;
	}

	if(hWndChild == NULL)
	{
		lResult = 1;
		return false;
	}

	ASSERT(::IsWindow(hWndChild));
	lResult = ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
	return true;
}

inline
bool CUserWindow::ForwardNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	switch(uMsg)
	{
	case WM_COMMAND:
	case WM_NOTIFY:
	case WM_PARENTNOTIFY:
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
	case WM_COMPAREITEM:
	case WM_DELETEITEM:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		lResult = ::SendMessage(::GetParent(m_hWnd), uMsg, wParam, lParam);
		break;
	default:
		break;
	}
	return false;
}

}; // namespace TWL

#endif // __USERWND_H__
