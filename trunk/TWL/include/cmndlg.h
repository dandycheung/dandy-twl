//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __CMNDLG_H__
#define __CMNDLG_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <commdlg.h>

namespace TWL
{

///////////////////////////////////////////////////////////////////////////////
// CFileDialog - used for File Open or File Save As

// compatibility with the old (vc6.0) headers
#if (_WIN32_WINNT >= 0x0500) && !defined(OPENFILENAME_SIZE_VERSION_400)
#ifndef CDSIZEOF_STRUCT
#define CDSIZEOF_STRUCT(structname, member)  (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))
#endif
#define OPENFILENAME_SIZE_VERSION_400A  CDSIZEOF_STRUCT(OPENFILENAMEA,lpTemplateName)
#define OPENFILENAME_SIZE_VERSION_400W  CDSIZEOF_STRUCT(OPENFILENAMEW,lpTemplateName)
#ifdef UNICODE
#define OPENFILENAME_SIZE_VERSION_400  OPENFILENAME_SIZE_VERSION_400W
#else
#define OPENFILENAME_SIZE_VERSION_400  OPENFILENAME_SIZE_VERSION_400A
#endif // !UNICODE
#endif // (_WIN32_WINNT >= 0x0500) && !defined(OPENFILENAME_SIZE_VERSION_400)

// Windows version helper
inline bool TwlIsOldWindows()
{
	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bRet = ::GetVersionEx(&ovi);
	return (!bRet || !((ovi.dwMajorVersion >= 5) || (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion >= 90)));
}

class CFileDialog : public CDialog
{
public:
	OPENFILENAME m_ofn;
	BOOL m_bOpenFileDialog;            // TRUE for file open, FALSE for file save
	TCHAR m_szFileTitle[_MAX_FNAME];   // contains file title after return
	TCHAR m_szFileName[_MAX_PATH];     // contains full path name after return

	CFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
			LPCTSTR pszDefExt = NULL,
			LPCTSTR pszFileName = NULL,
			DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			LPCTSTR pszFilter = NULL,
			HWND hWndParent = NULL)
	{
		memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL
		m_szFileName[0] = TEXT('\0');
		m_szFileTitle[0] = TEXT('\0');

		m_bOpenFileDialog = bOpenFileDialog;

		m_ofn.lStructSize = sizeof(m_ofn);
#if (_WIN32_WINNT >= 0x0500)
		// adjust struct size if running on older version of Windows
		if(TwlIsOldWindows())
		{
			ASSERT(sizeof(m_ofn) > OPENFILENAME_SIZE_VERSION_400);   // must be
			m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
		}
#endif //(_WIN32_WINNT >= 0x0500)
		m_ofn.lpstrFile = m_szFileName;
		m_ofn.nMaxFile = _MAX_PATH;
		m_ofn.lpstrDefExt = pszDefExt;
		m_ofn.lpstrFileTitle = (LPTSTR)m_szFileTitle;
		m_ofn.nMaxFileTitle = _MAX_FNAME;
		m_ofn.Flags = dwFlags | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING;
		m_ofn.lpstrFilter = pszFilter;
		m_ofn.hInstance = GetModuleInstance();
		m_ofn.lpfnHook = (LPOFNHOOKPROC)GetDialogProc();
		m_ofn.hwndOwner = hWndParent;

		// setup initial file name
		if(pszFileName != NULL)
			lstrcpyn(m_szFileName, pszFileName, _MAX_PATH);
	}

	INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow())
	{
		ASSERT((m_ofn.Flags & OFN_ENABLEHOOK) != 0);
		ASSERT(m_ofn.lpfnHook != NULL);   // can still be a user hook

		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		if(m_ofn.hwndOwner == NULL)   // set only if not specified before
			m_ofn.hwndOwner = hWndParent;

		ASSERT(m_hWnd == NULL);
		AddThreadData(this, 2);

		BOOL bRet;
		if(m_bOpenFileDialog)
			bRet = ::GetOpenFileName(&m_ofn);
		else
			bRet = ::GetSaveFileName(&m_ofn);

		m_hWnd = NULL;

		return bRet ? IDOK : IDCANCEL;
	}

	// useless, but for the damned pure virtual function of the base class
	LPCTSTR GetDialogTemplate() { return NULL; }

// Attributes
	CWindow GetFileDialogWindow() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CWindow(GetParent());
	}

	int GetFilePath(LPTSTR pszFilePath, int nLength) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFILEPATH, nLength, (LPARAM)pszFilePath);
	}

	int GetFolderIDList(LPVOID lpBuff, int nLength) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERIDLIST, nLength, (LPARAM)lpBuff);
	}

	int GetFolderPath(LPTSTR pszFolderPath, int nLength) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERPATH, nLength, (LPARAM)pszFolderPath);
	}

	int GetSpec(LPTSTR pszSpec, int nLength) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETSPEC, nLength, (LPARAM)pszSpec);
	}

	void SetControlText(int nCtrlId, LPCTSTR pszText)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		GetFileDialogWindow().SendMessage(CDM_SETCONTROLTEXT, nCtrlId, (LPARAM)pszText);
	}

	void SetDefExt(LPCTSTR pszExt)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		GetFileDialogWindow().SendMessage(CDM_SETDEFEXT, 0, (LPARAM)pszExt);
	}

	BOOL GetReadOnlyPref() const	// return TRUE if readonly checked
	{
		return ((m_ofn.Flags & OFN_READONLY) != 0) ? TRUE : FALSE;
	}

// Operations
	void HideControl(int nCtrlId)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((m_ofn.Flags & OFN_EXPLORER) != 0);

		GetFileDialogWindow().SendMessage(CDM_HIDECONTROL, nCtrlId);
	}

// Special override for common dialogs
	BOOL EndDialog(INT_PTR /*nRetCode*/ = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		GetFileDialogWindow().SendMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0));
		return TRUE;
	}

// Message map and handlers
	BEGIN_MSG_MAP()
		NOTIFY_CODE_HANDLER(CDN_FILEOK, _OnFileOK)
		NOTIFY_CODE_HANDLER(CDN_FOLDERCHANGE, _OnFolderChange)
		NOTIFY_CODE_HANDLER(CDN_HELP, _OnHelp)
		NOTIFY_CODE_HANDLER(CDN_INITDONE, _OnInitDone)
		NOTIFY_CODE_HANDLER(CDN_SELCHANGE, _OnSelChange)
		NOTIFY_CODE_HANDLER(CDN_SHAREVIOLATION, _OnShareViolation)
		NOTIFY_CODE_HANDLER(CDN_TYPECHANGE, _OnTypeChange)
	END_MSG_MAP()

	bool _OnFileOK(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& lResult)
	{
		ASSERT(::IsWindow(m_hWnd));
		lResult = !OnFileOK((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnFolderChange(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& /*lResult*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		OnFolderChange((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnHelp(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& /*lResult*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		OnHelp((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnInitDone(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& /*lResult*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		OnInitDone((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnSelChange(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& /*lResult*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		OnSelChange((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnShareViolation(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& lResult)
	{
		ASSERT(::IsWindow(m_hWnd));
		lResult = OnShareViolation((LPOFNOTIFY)pnmh);
		return true;
	}

	bool _OnTypeChange(int /*idCtrl*/, LPNMHDR pnmh, LRESULT& /*lResult*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		OnTypeChange((LPOFNOTIFY)pnmh);
		return true;
	}

// Overrideables
	virtual BOOL OnFileOK(LPOFNOTIFY /*lpon*/)
	{
		return TRUE;
	}

	virtual void OnFolderChange(LPOFNOTIFY /*lpon*/)
	{
	}

	virtual void OnHelp(LPOFNOTIFY /*lpon*/)
	{
	}

	virtual void OnInitDone(LPOFNOTIFY /*lpon*/)
	{
	}

	virtual void OnSelChange(LPOFNOTIFY /*lpon*/)
	{
	}

	virtual int OnShareViolation(LPOFNOTIFY /*lpon*/)
	{
		return 0;
	}

	virtual void OnTypeChange(LPOFNOTIFY /*lpon*/)
	{
	}
};

}; // namespace TWL

#endif // __CMNDLG_H__
