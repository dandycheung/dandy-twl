//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __FRMBASE_H__
#define __FRMBASE_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "userwnd.h"

namespace TWL
{

class CFrameWndBase : public CUserWindow
{
	typedef CUserWindow _superClass;

public:
	CFrameWndBase()
	{
	}

	virtual ~CFrameWndBase()
	{
	}


	LPCTSTR GetClassName()
	{
		return _T("TWLFrameWndBase");
	}

	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		if(uMsg == WM_NCDESTROY)
		{
			PostQuitMessage(0);

			lResult = 0;
			return true;
		}

		return _superClass::ProcessMessage(uMsg, wParam, lParam, lResult);
	}

// frame window special stuff
	bool CreateEx(LPCTSTR pszTitle = NULL, LPCRECT prc = NULL, HMENU hMenu = NULL)
	{
		RECT rc = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		if(prc != NULL)
			rc = *prc;

		if(hMenu == NULL)
			hMenu = GetCreateMenu();

		bool bRet = Create(0, pszTitle, WS_VISIBLE | WS_OVERLAPPEDWINDOW, &rc, NULL, hMenu, GetModuleInstance());
		if(bRet)
			PostCreate();

		return bRet;
	}

	virtual HMENU GetCreateMenu()
	{
		return NULL;
	}

	virtual void PostCreate()
	{
	}

#ifdef NO_USE_CWINDOW
	BOOL ShowWindow(int iCmdShow)
	{
		return ::ShowWindow(m_hWnd, iCmdShow);
	}

	operator HWND()
	{
		return m_hWnd;
	}
#endif // NO_USE_CWINDOW
};

}; // namespace TWL

#endif // __FRMBASE_H__
