//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __DLG_H__
#define __DLG_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "userwnd.h"
#include "resutil.h"
#include "trcdbg.h"

#define FIX_DIALOG_FONT

#ifdef FIX_DIALOG_FONT
#ifdef DialogBoxParam
#undef DialogBoxParam
#endif // DialogBoxParam
#ifdef CreateDialogParam
#undef CreateDialogParam
#endif // CreateDialogParam
#include "dlgtempl.h"
#endif // FIX_DIALOG_FONT

namespace TWL
{

/////////////////////////////////////////////////////////////////////////////
// CDialog - Implements a dialog box

class CDialog : public CUserWindow
{
public:
	bool m_bModal;

	CDialog() : m_bModal(false)
	{
	}

	virtual LPCTSTR GetClassName()
	{
		return WC_DIALOG;
	}

	virtual DLGPROC GetDialogProc()
	{
		return DialogProc;
	}

	virtual LPCTSTR GetDialogTemplate() = 0;

	static INT CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}

	BOOL MapDialogRect(LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::MapDialogRect(m_hWnd, lpRect);
	}

	bool DefInitDialogHandler(WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		// for getting the keyboard focus and let the ESC key work properly
		lResult = true;
		return true;
	}

	bool DefCommandHandler(WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		int iCmd = LOWORD(wParam);
		if(iCmd == IDOK || iCmd == IDCANCEL)
		{
			if(m_bModal)
				EndDialog(iCmd);
			else
				DestroyWindow();

			lResult = iCmd;
			return true;
		}

		return false;
	}

	virtual bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		if(uMsg == WM_INITDIALOG)
			return DefInitDialogHandler(wParam, lParam, lResult);
		else if(uMsg == WM_COMMAND)
			return DefCommandHandler(wParam, lParam, lResult);

		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	// modal dialogs
	int DoModal(HWND hWndParent = ::GetActiveWindow(), HINSTANCE hInstance = GetResourceInstance())
	{
		ASSERT(m_hWnd == NULL);

		m_bModal = true;
		HookWindowCreate(this);
		int iRet = DialogBoxParam(hInstance, GetDialogTemplate(), hWndParent, GetDialogProc(), 0);
		UnhookWindowCreate();

		return iRet;
	}

	BOOL EndDialog(int iRetCode)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(m_bModal);	// must be a modal dialog
		return ::EndDialog(m_hWnd, iRetCode);
	}

	// modeless dialogs
	HWND Create(HWND hWndParent, LPARAM lInitParam = NULL, HINSTANCE hInstance = GetResourceInstance())
	{
		ASSERT(m_hWnd == NULL);

		m_bModal = false;
		HookWindowCreate(this);
		HWND hWnd = CreateDialogParam(hInstance, GetDialogTemplate(), hWndParent, GetDialogProc(), (LPARAM)lInitParam);
		UnhookWindowCreate();
		ASSERT(m_hWnd == hWnd);

		return hWnd;
	}

	BOOL DestroyWindow()
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(!m_bModal);	// must not be a modal dialog
		return ::DestroyWindow(m_hWnd);
	}

#ifdef FIX_DIALOG_FONT

	static
	HGLOBAL WINAPI PreProcessDialogTemplate(LPDLGTEMPLATE& pDlgTempl)
	{
		CString strFace;
		WORD wSize = 0;
		BOOL bSetSysFont = !CDialogTemplate::GetFont(pDlgTempl, strFace, wSize);

		// On DBCS systems, also change "MS Sans Serif" or "Helv" to system font.
		if((!bSetSysFont) && GetSystemMetrics(SM_DBCSENABLED))
		{
			bSetSysFont = (strFace == _T("MS Shell Dlg") ||
				strFace == _T("MS Sans Serif") || strFace == _T("Helv"));

			if(bSetSysFont && (wSize == 8))
				wSize = 0;
		}

		HGLOBAL hDlgTempl = NULL;

		if(bSetSysFont)
		{
			CDialogTemplate dlgTemp(pDlgTempl);
			dlgTemp.SetSystemFont(wSize);
			hDlgTempl = dlgTemp.Detach();
		}

		if(hDlgTempl != NULL)
			pDlgTempl = (LPDLGTEMPLATE)GlobalLock(hDlgTempl);

		return hDlgTempl;
	}

	static
	void WINAPI PostProcessDialogTemplate(HGLOBAL hDlgTempl)
	{
		if(!hDlgTempl)
			return;

		GlobalUnlock(hDlgTempl);
		GlobalFree(hDlgTempl);
	}

#endif // FIX_DIALOG_FONT

	static
	INT_PTR WINAPI DialogBoxParam(HINSTANCE hmod, LPCTSTR pszName, HWND hwndOwner, DLGPROC dlgProc, LPARAM lParam)
	{
		HRSRC hRes = FindResourceEx(hmod, RT_DIALOG, pszName, 0);
		if(!hRes)
			return -1;

		HGLOBAL hRes2 = LoadResource(hmod, hRes);
		if(!hRes2)
			return -1;

		INT_PTR iRet = -1;

		LPDLGTEMPLATE pDlgTempl = (LPDLGTEMPLATE)LockResource(hRes2);
		if(pDlgTempl)
		{
#ifdef FIX_DIALOG_FONT
			HGLOBAL hTemplate = PreProcessDialogTemplate(pDlgTempl);
#endif // FIX_DIALOG_FONT
			iRet = DialogBoxIndirectParam(hmod, pDlgTempl, hwndOwner, dlgProc, lParam);
#ifdef FIX_DIALOG_FONT
			if(hTemplate)
				PostProcessDialogTemplate(hTemplate);
			else
#endif // FIX_DIALOG_FONT
				UnlockResource(hRes2);
		}

		FreeResource(hRes2);

		return iRet;
	}

	static
	HWND WINAPI CreateDialogParam(HINSTANCE hmod, LPCSTR pszName, HWND hwndOwner, DLGPROC dlgProc, LPARAM lParam)
	{
		HRSRC hRes = FindResourceEx(hmod, RT_DIALOG, pszName, 0);
		if(!hRes)
			return NULL;

		HGLOBAL hRes2 = LoadResource(hmod, hRes);
		if(!hRes2)
			return NULL;

		HWND hwnd = NULL;

		LPDLGTEMPLATE pDlgTempl = (LPDLGTEMPLATE)LockResource(hRes2);
		if(pDlgTempl)
		{
#ifdef FIX_DIALOG_FONT
			HGLOBAL hTemplate = PreProcessDialogTemplate(pDlgTempl);
#endif // FIX_DIALOG_FONT
			hwnd = CreateDialogIndirectParam(hmod, pDlgTempl, hwndOwner, dlgProc, lParam);
#ifdef FIX_DIALOG_FONT
			if(hTemplate)
				PostProcessDialogTemplate(hTemplate);
			else
#endif // FIX_DIALOG_FONT
				UnlockResource(hRes2);
		}

		FreeResource(hRes2);

		return hwnd;
	}
};

}; // namespace TWL

#endif // __DLG_H__
