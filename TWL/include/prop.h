//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __PROP_H__
#define __PROP_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "ary.h"		// for TArray
#include "resutil.h"	// for CStringOrId and GetResourceInstance()
#include "dlgtempl.h"	// for CDialogTemplate
#include "tdc.h"		// for PeekThreadData()

namespace TWL
{

#define IDD_PROPSHEET   1006
#define IDD_WIZARD      1020

class CFontHelper
{
public:
	static int CALLBACK FontEnumProc(const LOGFONT*, const TEXTMETRIC*, DWORD, LPARAM lParam)
	{
		if(lParam != NULL)
			*(BOOL*)lParam = TRUE;

		return 0;
	}

	static BOOL IsFontInstalled(LPCTSTR pszFace)
	{
		LOGFONT lf = { 0 };
		lstrcpy(lf.lfFaceName, pszFace);
		lf.lfCharSet = DEFAULT_CHARSET;

		BOOL bInstalled = FALSE;
		HDC hdc = ::GetDC(NULL);
		if(hdc != NULL)
		{
			::EnumFontFamiliesEx(hdc, &lf, FontEnumProc, (LPARAM)&bInstalled, 0);
			::ReleaseDC(NULL, hdc);
		}

		return bInstalled;
	}

	struct CPropPageFontInfo
	{
		typedef CPropPageFontInfo thisClass;

		LPTSTR m_pszFaceName;
		WORD m_wSize;

		CPropPageFontInfo()
			: m_pszFaceName(NULL), m_wSize(0)
		{
		}

		~CPropPageFontInfo()
		{
			if(m_pszFaceName)
				GlobalFree(m_pszFaceName);
		}
	};

	static CPropPageFontInfo* GetPropPageFontInfo()
	{
		static CPropPageFontInfo ppfi;
		return &ppfi;
	}

	typedef LANGID (WINAPI* PFNGETUSERDEFAULTUILANGUAGE)();
	static BOOL GetPropSheetFont(CString& strFace, WORD& wSize, BOOL bWizard)
	{
		CPropPageFontInfo* pFontInfo = GetPropPageFontInfo();

		// determine which font property sheet will use
		if(pFontInfo->m_wSize == 0)
		{
			ASSERT(pFontInfo->m_pszFaceName == NULL);

			HINSTANCE hInst = GetModuleHandleA("COMCTL32.DLL");
			if(hInst != NULL)
			{
				HRSRC hResource = NULL;
				WORD wLang = 0;
				HMODULE hKernel32 = ::GetModuleHandleA("KERNEL32.DLL");
				PFNGETUSERDEFAULTUILANGUAGE pfnGetUserDefaultUILanguage;
				(FARPROC&)pfnGetUserDefaultUILanguage = ::GetProcAddress(hKernel32, "GetUserDefaultUILanguage");
				if(pfnGetUserDefaultUILanguage != NULL)
				{
					LANGID langid;
					langid = pfnGetUserDefaultUILanguage();
					if ((PRIMARYLANGID(langid) == LANG_JAPANESE) && IsFontInstalled(_T("MS UI Gothic")))
						wLang = MAKELANGID(LANG_JAPANESE, 0x3f);
				}
				if(wLang != 0)
				{
					hResource = ::FindResourceEx(hInst, RT_DIALOG, 
						MAKEINTRESOURCE(bWizard ? IDD_WIZARD : IDD_PROPSHEET), wLang);
				}
				if(hResource == NULL)
				{
					hResource = ::FindResource(hInst,
						MAKEINTRESOURCE(bWizard ? IDD_WIZARD : IDD_PROPSHEET), RT_DIALOG);
				}
				HGLOBAL hTemplate = LoadResource(hInst, hResource);
				if(hTemplate != NULL)
					CDialogTemplate::GetFont((DLGTEMPLATE*)hTemplate, strFace, wSize);
			}

			pFontInfo->m_pszFaceName = (LPTSTR)GlobalAlloc(GPTR, sizeof(TCHAR) * (strFace.GetLength() + 1));
			lstrcpy(pFontInfo->m_pszFaceName, strFace);
			pFontInfo->m_wSize = wSize;
		}

		strFace = pFontInfo->m_pszFaceName;
		wSize = pFontInfo->m_wSize;

		return (wSize != 0xFFFF);
	}

	static DLGTEMPLATE* ChangePropPageFont(const DLGTEMPLATE* pTemplate, BOOL bWizard)
	{
		CString strFaceDefault;
		WORD wSizeDefault;

		if(!CFontHelper::GetPropSheetFont(strFaceDefault, wSizeDefault, bWizard))
			return NULL;

		// set font of property page to same font used by property sheet
		CString strFace;
		WORD wSize;
		if((!CDialogTemplate::GetFont(pTemplate, strFace, wSize)) ||
			(strFace != strFaceDefault) || (wSize != wSizeDefault))
		{
			CDialogTemplate dlgTemplate(pTemplate);
			dlgTemplate.SetFont(strFaceDefault, wSizeDefault);
			return (DLGTEMPLATE*)dlgTemplate.Detach();
		}

		return NULL;
	}
};

///////////////////////////////////////////////////////////////////////////////
// TPropertySheet - client side for a property sheet

template <class T>
class TPropertySheet
{
public:
// Constructors
	TPropertySheet(HWND hWnd = NULL)
	{
		T* pT = static_cast<T*>(this);
		pT->m_hWnd = hWnd;
	}

	TPropertySheet& operator =(HWND hWnd)
	{
		T* pT = static_cast<T*>(this);
		pT->m_hWnd = hWnd;
		return *this;
	}

// Attributes
	int GetPageCount() const
	{
		const T* pT = static_cast<const T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		HWND hWndTabCtrl = GetTabControl();
		ASSERT(hWndTabCtrl != NULL);
		return (int)::SendMessage(hWndTabCtrl, TCM_GETITEMCOUNT, 0, 0L);
	}

	HWND GetActivePage() const
	{
		const T* pT = static_cast<const T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (HWND)::SendMessage(pT->m_hWnd, PSM_GETCURRENTPAGEHWND, 0, 0L);
	}

	int GetActiveIndex() const
	{
		const T* pT = static_cast<const T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		HWND hWndTabCtrl = GetTabControl();
		ASSERT(hWndTabCtrl != NULL);
		return (int)::SendMessage(hWndTabCtrl, TCM_GETCURSEL, 0, 0L);
	}

	BOOL SetActivePage(int nPageIndex)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_SETCURSEL, nPageIndex, 0L);
	}

	BOOL SetActivePage(HPROPSHEETPAGE hPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(hPage != NULL);
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_SETCURSEL, 0, (LPARAM)hPage);
	}

	BOOL SetActivePageByID(int nPageID)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_SETCURSELID, 0, nPageID);
	}

	void SetTitle(LPCTSTR lpszText, UINT nStyle = 0)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT((nStyle & ~PSH_PROPTITLE) == 0); // only PSH_PROPTITLE is valid
		ASSERT(lpszText != NULL);
		::SendMessage(pT->m_hWnd, PSM_SETTITLE, nStyle, (LPARAM)lpszText);
	}

	HWND GetTabControl() const
	{
		const T* pT = static_cast<const T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (HWND)::SendMessage(pT->m_hWnd, PSM_GETTABCONTROL, 0, 0L);
	}

	void SetFinishText(LPCTSTR lpszText)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_SETFINISHTEXT, 0, (LPARAM)lpszText);
	}

	void SetWizardButtons(DWORD dwFlags)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::PostMessage(pT->m_hWnd, PSM_SETWIZBUTTONS, 0, dwFlags);
	}

// Operations
	BOOL AddPage(HPROPSHEETPAGE hPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(hPage != NULL);
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_ADDPAGE, 0, (LPARAM)hPage);
	}

	BOOL AddPage(LPCPROPSHEETPAGE pPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if(hPage == NULL)
			return FALSE;
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_ADDPAGE, 0, (LPARAM)hPage);
	}

#ifndef _WIN32_WCE
	BOOL InsertPage(int nNewPageIndex, HPROPSHEETPAGE hPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(hPage != NULL);
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_INSERTPAGE, nNewPageIndex, (LPARAM)hPage);
	}

	BOOL InsertPage(int nNewPageIndex, LPCPROPSHEETPAGE pPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if(hPage == NULL)
			return FALSE;
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_INSERTPAGE, nNewPageIndex, (LPARAM)hPage);
	}

	BOOL InsertPage(HPROPSHEETPAGE hPageInsertAfter, HPROPSHEETPAGE hPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(hPage != NULL);
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_INSERTPAGE, (WPARAM)hPageInsertAfter, (LPARAM)hPage);
	}

	BOOL InsertPage(HPROPSHEETPAGE hPageInsertAfter, LPCPROPSHEETPAGE pPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if(hPage == NULL)
			return FALSE;
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_INSERTPAGE, (WPARAM)hPageInsertAfter, (LPARAM)hPage);
	}
#endif // !_WIN32_WCE

	void RemovePage(int nPageIndex)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_REMOVEPAGE, nPageIndex, 0L);
	}

	void RemovePage(HPROPSHEETPAGE hPage)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(hPage != NULL);
		::SendMessage(pT->m_hWnd, PSM_REMOVEPAGE, 0, (LPARAM)hPage);
	}

	BOOL PressButton(int nButton)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_PRESSBUTTON, nButton, 0L);
	}

	BOOL Apply()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_APPLY, 0, 0L);
	}

	void CancelToClose()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_CANCELTOCLOSE, 0, 0L);
	}

	void SetModified(HWND hWndPage, BOOL bChanged = TRUE)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(::IsWindow(hWndPage));
		UINT uMsg = bChanged ? PSM_CHANGED : PSM_UNCHANGED;
		::SendMessage(pT->m_hWnd, uMsg, (WPARAM)hWndPage, 0L);
	}

	LRESULT QuerySiblings(WPARAM wParam, LPARAM lParam)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return ::SendMessage(pT->m_hWnd, PSM_QUERYSIBLINGS, wParam, lParam);
	}

	void RebootSystem()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_REBOOTSYSTEM, 0, 0L);
	}

	void RestartWindows()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_RESTARTWINDOWS, 0, 0L);
	}

	BOOL IsDialogMessage(LPMSG lpMsg)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_ISDIALOGMESSAGE, 0, (LPARAM)lpMsg);
	}

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	int HwndToIndex(HWND hWnd) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (int)::SendMessage(pT->m_hWnd, PSM_HWNDTOINDEX, (WPARAM)hWnd, 0L);
	}

	HWND IndexToHwnd(int nIndex) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (HWND)::SendMessage(pT->m_hWnd, PSM_INDEXTOHWND, nIndex, 0L);
	}

	int PageToIndex(HPROPSHEETPAGE hPage) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (int)::SendMessage(pT->m_hWnd, PSM_PAGETOINDEX, 0, (LPARAM)hPage);
	}

	HPROPSHEETPAGE IndexToPage(int nIndex) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (HPROPSHEETPAGE)::SendMessage(pT->m_hWnd, PSM_INDEXTOPAGE, nIndex, 0L);
	}

	int IdToIndex(int nID) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (int)::SendMessage(pT->m_hWnd, PSM_IDTOINDEX, 0, nID);
	}

	int IndexToId(int nIndex) const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (int)::SendMessage(pT->m_hWnd, PSM_INDEXTOID, nIndex, 0L);
	}

	int GetResult() const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (int)::SendMessage(pT->m_hWnd, PSM_GETRESULT, 0, 0L);
	}

	BOOL RecalcPageSizes()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return (BOOL)::SendMessage(pT->m_hWnd, PSM_RECALCPAGESIZES, 0, 0L);
	}

	void SetHeaderTitle(int nIndex, LPCTSTR lpstrHeaderTitle)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_SETHEADERTITLE, nIndex, (LPARAM)lpstrHeaderTitle);
	}

	void SetHeaderSubTitle(int nIndex, LPCTSTR lpstrHeaderSubTitle)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		::SendMessage(pT->m_hWnd, PSM_SETHEADERSUBTITLE, nIndex, (LPARAM)lpstrHeaderSubTitle);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
};

typedef TPropertySheet<CWindow> CPropertySheetWindow;

///////////////////////////////////////////////////////////////////////////////
// CPropertySheet - implements a property sheet

class CPropertyPage; // foreward declare, for AddPage(CPropertyPage&) overload

class CPropertySheet : public CUserWindow, public TPropertySheet<CPropertySheet>
{
	typedef CPropertySheet thisClass;
	typedef TPropertySheet<CPropertySheet> baseClass;

	DECLARE_CLASS_NAME(TEXT("#32770"));

public:
	typedef int (CALLBACK *fnPropSheetCallback)(HWND hWnd, UINT uMsg, LPARAM lParam);
	typedef HRESULT (CALLBACK *fnDllGetVersion)(PVOID); // DLLVERSIONINFO *pdvi

public:
	PROPSHEETHEADER m_psh;
	TArray<HPROPSHEETPAGE> m_arrPages;
	fnDllGetVersion m_fnGetDllVer;

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific
#ifndef PROPSHEET_LINK_SIZE
#define PROPSHEET_LINK_SIZE 128
#endif // PROPSHEET_LINK_SIZE
	TCHAR m_szLink[PROPSHEET_LINK_SIZE];
	static LPCTSTR m_pszTitle;
	static LPCTSTR m_pszLink;
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) 

// Construction/Destruction
	CPropertySheet(CStringOrId title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
	{
		(FARPROC&)m_fnGetDllVer = GetProcAddress(GetModuleHandle(TEXT("comctl32.dll")), "DllGetVersion");

		memset(&m_psh, 0, sizeof(PROPSHEETHEADER));
		m_psh.dwSize = m_fnGetDllVer ? sizeof(PROPSHEETHEADER) : PROPSHEETHEADER_V1_SIZE;
		m_psh.dwFlags = PSH_USECALLBACK;
		m_psh.hInstance = GetResourceInstance();
		m_psh.phpage = NULL;   // will be set later
		m_psh.nPages = 0;      // will be set later
		m_psh.pszCaption = title;
		m_psh.nStartPage = uStartPage;
		m_psh.hwndParent = hWndParent;   // if NULL, will be set in DoModal/Create
		m_psh.pfnCallback = GetPropSheetCallback();

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific 
		m_psh.dwFlags |= PSH_MAXIMIZE;
		m_szLink[0] = 0;
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__)
	}

	~CPropertySheet()
	{
		if(m_arrPages.GetSize() > 0)   // sheet never created, destroy all pages
		{
			for(int i = 0; i < m_arrPages.GetSize(); i++)
				::DestroyPropertySheetPage((HPROPSHEETPAGE)m_arrPages[i]);
		}
	}

	BOOL IsWizard() const
	{
		return (m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97)) != 0;
	}

	virtual fnPropSheetCallback GetPropSheetCallback()
	{
		return PropSheetCallback;
	}

// Callback function and overrideables
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam)
	{
		lParam;   // avoid level 4 warning
		int nRet = 0;

		if(uMsg == PSCB_PRECREATE)
		{
			thisClass* pThis = (thisClass*)PeekThreadData();
			pThis->OnSheetPreCreate(lParam);
		}
		else if(uMsg == PSCB_INITIALIZED)
		{
			ASSERT(hWnd != NULL);
			thisClass* pThis = (thisClass*)::GetProp(hWnd, TWL_WND_PRIVATE_PROP);

			// remove page handles array
			pThis->CleanUpPages();

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific
			m_pszTitle = pThis->m_psh.pszCaption;
			if(*pThis->m_szLink != 0)
				m_pszLink = pThis->m_szLink;
#endif  // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific

			pThis->OnSheetInitialized();
		}
#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific uMsg
		else
		{
			switch(uMsg)
			{
			case PSCB_GETVERSION :
				nRet = COMCTL32_VERSION;
				break;
			case PSCB_GETTITLE :
				if(m_pszTitle != NULL)
				{
					lstrcpy((LPTSTR)lParam, m_pszTitle);
					m_pszTitle = NULL;
				}
				break;
			case PSCB_GETLINKTEXT:
				if(m_pszLink != NULL)
				{
					lstrcpy((LPTSTR)lParam, m_pszLink);
					m_pszLink = NULL;
				}
				break;
			default:
				break;
			}
		}
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) 

		return nRet;
	}

	virtual void OnSheetPreCreate(LPARAM lParam)
	{
	}

	virtual void OnSheetInitialized()
	{
	}

// Create method
	HWND Create(HWND hWndParent = NULL)
	{
		ASSERT(m_hWnd == NULL);

		m_psh.dwFlags |= PSH_MODELESS;
		if(m_psh.hwndParent == NULL)
			m_psh.hwndParent = hWndParent;
		m_psh.phpage = (HPROPSHEETPAGE*)m_arrPages.GetData();
		m_psh.nPages = m_arrPages.GetSize();

		HookWindowCreate(this);
		HWND hWnd = (HWND)::PropertySheet(&m_psh);
		UnhookWindowCreate();
		CleanUpPages();   // ensure clean-up, required if call failed

		ASSERT(m_hWnd == hWnd);

		return hWnd;
	}

	INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow())
	{
		ASSERT(m_hWnd == NULL);

		m_psh.dwFlags &= ~PSH_MODELESS;
		if(m_psh.hwndParent == NULL)
			m_psh.hwndParent = hWndParent;
		m_psh.phpage = (HPROPSHEETPAGE*)m_arrPages.GetData();
		m_psh.nPages = m_arrPages.GetSize();

		HookWindowCreate(this);
		INT_PTR nRet = ::PropertySheet(&m_psh);
		UnhookWindowCreate();
		CleanUpPages();   // ensure clean-up, required if call failed

		return nRet;
	}

	// implementation helper - clean up pages array
	virtual void CleanUpPages()
	{
		m_psh.nPages = 0;
		m_psh.phpage = NULL;
		m_arrPages.RemoveAll();
	}

// Attributes (extended overrides of client class methods)
// These now can be called before the sheet is created
// Note: Calling these after the sheet is created gives unpredictable results
	int GetPageCount() const
	{
		if(m_hWnd == NULL)   // not created yet
			return m_arrPages.GetSize();
		return baseClass::GetPageCount();
	}

	int GetActiveIndex() const
	{
		if(m_hWnd == NULL)   // not created yet
			return m_psh.nStartPage;
		return baseClass::GetActiveIndex();
	}

	HPROPSHEETPAGE GetPage(int nPageIndex) const
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created
		return (HPROPSHEETPAGE)m_arrPages[nPageIndex];
	}

	int GetPageIndex(HPROPSHEETPAGE hPage) const
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created
		return m_arrPages.Find((HPROPSHEETPAGE&)hPage);
	}

	BOOL SetActivePage(int nPageIndex)
	{
		if(m_hWnd == NULL)   // not created yet
		{
			ASSERT(nPageIndex >= 0 && nPageIndex < m_arrPages.GetSize());
			m_psh.nStartPage = nPageIndex;
			return TRUE;
		}
		return baseClass::SetActivePage(nPageIndex);
	}

	BOOL SetActivePage(HPROPSHEETPAGE hPage)
	{
		ASSERT(hPage != NULL);
		if (m_hWnd == NULL)   // not created yet
		{
			int nPageIndex = GetPageIndex(hPage);
			if(nPageIndex == -1)
				return FALSE;

			return SetActivePage(nPageIndex);
		}
		return baseClass::SetActivePage(hPage);
	}

	void SetTitle(LPCTSTR lpszText, UINT nStyle = 0)
	{
		ASSERT((nStyle & ~PSH_PROPTITLE) == 0);   // only PSH_PROPTITLE is valid
		ASSERT(lpszText != NULL);

		if(m_hWnd == NULL)
		{
			// set internal state
			m_psh.pszCaption = lpszText;   // must exist until sheet is created
			m_psh.dwFlags &= ~PSH_PROPTITLE;
			m_psh.dwFlags |= nStyle;
		}
		else
		{
			// set external state
			baseClass::SetTitle(lpszText, nStyle);
		}
	}

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC specific Link field	
	void SetLinkText(LPCTSTR lpszText)
	{
		ASSERT(lpszText != NULL);
		ASSERT(lstrlen(lpszText) < PROPSHEET_LINK_SIZE);
		lstrcpy(m_szLink, lpszText);
	}
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) 

	void SetWizardMode()
	{
		/// TODO: version problem
		m_psh.dwFlags |= PSH_WIZARD;
	}

	void EnableHelp()
	{
		m_psh.dwFlags |= PSH_HASHELP;
	}

// Operations
	BOOL AddPage(HPROPSHEETPAGE hPage)
	{
		ASSERT(hPage != NULL);
		BOOL bRet = FALSE;
		if(m_hWnd != NULL)
			bRet = baseClass::AddPage(hPage);
		else	// sheet not created yet, use internal data
			bRet = m_arrPages.Add((HPROPSHEETPAGE&)hPage);
		return bRet;
	}

	BOOL AddPage(LPCPROPSHEETPAGE pPage)
	{
		ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if(hPage == NULL)
			return FALSE;
		BOOL bRet = AddPage(hPage);
		if(!bRet)
			::DestroyPropertySheetPage(hPage);
		return bRet;
	}

	/// TODO:
	inline BOOL AddPage(CPropertyPage& page);

	BOOL RemovePage(HPROPSHEETPAGE hPage)
	{
		ASSERT(hPage != NULL);
		if (m_hWnd == NULL)   // not created yet
		{
			int nPage = GetPageIndex(hPage);
			if(nPage == -1)
				return FALSE;
			return RemovePage(nPage);
		}
		baseClass::RemovePage(hPage);
		return TRUE;
	}

	BOOL RemovePage(int nPageIndex)
	{
		BOOL bRet = TRUE;
		if(m_hWnd != NULL)
			baseClass::RemovePage(nPageIndex);
		else	// sheet not created yet, use internal data
			bRet = m_arrPages.RemoveAt(nPageIndex);
		return bRet;
	}

	//
	/// TODO: version problem
	//
#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	void SetHeader(LPCTSTR szbmHeader)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created

		m_psh.dwFlags &= ~PSH_WIZARD;
		m_psh.dwFlags |= (PSH_HEADER | PSH_WIZARD97);
		m_psh.pszbmHeader = szbmHeader;
	}

	void SetHeader(HBITMAP hbmHeader)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created

		m_psh.dwFlags &= ~PSH_WIZARD;
		m_psh.dwFlags |= (PSH_HEADER | PSH_USEHBMHEADER | PSH_WIZARD97);
		m_psh.hbmHeader = hbmHeader;
	}

	void SetWatermark(LPCTSTR szbmWatermark, HPALETTE hplWatermark = NULL)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created

		m_psh.dwFlags &= ~PSH_WIZARD;
		m_psh.dwFlags |= PSH_WATERMARK | PSH_WIZARD97;
		m_psh.pszbmWatermark = szbmWatermark;

		if (hplWatermark != NULL)
		{
			m_psh.dwFlags |= PSH_USEHPLWATERMARK;
			m_psh.hplWatermark = hplWatermark;
		}
	}

	void SetWatermark(HBITMAP hbmWatermark, HPALETTE hplWatermark = NULL)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created

		m_psh.dwFlags &= ~PSH_WIZARD;
		m_psh.dwFlags |= (PSH_WATERMARK | PSH_USEHBMWATERMARK | PSH_WIZARD97);
		m_psh.hbmWatermark = hbmWatermark;

		if (hplWatermark != NULL)
		{
			m_psh.dwFlags |= PSH_USEHPLWATERMARK;
			m_psh.hplWatermark = hplWatermark;
		}
	}

	void StretchWatermark(bool bStretchWatermark)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created
		if (bStretchWatermark)
			m_psh.dwFlags |= PSH_STRETCHWATERMARK;
		else
			m_psh.dwFlags &= ~PSH_STRETCHWATERMARK;
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

// Message map and handlers
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
	END_MSG_MAP()

	bool OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		lResult = DefWindowProc(uMsg, wParam, lParam);
		if(HIWORD(wParam) == BN_CLICKED && (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) &&
		   ((m_psh.dwFlags & PSH_MODELESS) != 0) && (GetActivePage() == NULL))
			DestroyWindow();
		return false;
	}

	bool OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, LRESULT& lResult)
	{
		if(((m_psh.dwFlags & PSH_MODELESS) == PSH_MODELESS) && ((wParam & 0xFFF0) == SC_CLOSE))
		{
			SendMessage(WM_CLOSE);
			return true;
		}
		return false;
	}
};

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__) // PPC static pointers
__declspec(selectany) LPCWSTR CPropertySheet::m_pszTitle = NULL;
__declspec(selectany) LPCWSTR CPropertySheet::m_pszLink = NULL;
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__)

///////////////////////////////////////////////////////////////////////////////
// TPropertyPage - client side for a property page

template<typename T>
class TPropertyPage
{
public:
// Constructors
	TPropertyPage(HWND hWnd = NULL)
	{
		T* pT = static_cast<T*>(this);
		pT->m_hWnd = hWnd;
	}

	TPropertyPage& operator =(HWND hWnd)
	{
		T* pT = static_cast<T*>(this);
		pT->m_hWnd = hWnd;
		return *this;
	}

// Attributes
	CPropertySheetWindow GetPropertySheet() const
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		return CPropertySheetWindow(GetParent());
	}

// Operations
	BOOL Apply()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		return GetPropertySheet().Apply();
	}

	void CancelToClose()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		GetPropertySheet().CancelToClose();
	}

	void SetModified(BOOL bChanged = TRUE)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		GetPropertySheet().SetModified(pT->m_hWnd, bChanged);
	}

	LRESULT QuerySiblings(WPARAM wParam, LPARAM lParam)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		return GetPropertySheet().QuerySiblings(wParam, lParam);
	}

	void RebootSystem()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		GetPropertySheet().RebootSystem();
	}

	void RestartWindows()
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		GetPropertySheet().RestartWindows();
	}

	void SetWizardButtons(DWORD dwFlags)
	{
		T* pT = static_cast<T*>(this);
		ASSERT(::IsWindow(pT->m_hWnd));
		ASSERT(GetParent() != NULL);
		GetPropertySheet().SetWizardButtons(dwFlags);
	}
};

typedef TPropertyPage<CWindow> CPropertyPageWindow;

///////////////////////////////////////////////////////////////////////////////
// CPropertyPage - implements a property page

class CPropertyPage : public CDialog, public TPropertyPage<CPropertyPage>
{
public:
	typedef CPropertyPage thisClass;
	typedef UINT (CALLBACK *fnPropPageCallback)(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp);

public:
	PROPSHEETPAGE m_psp;

	operator LPPROPSHEETPAGE()
	{
		return &m_psp;
	}

	CStringOrId m_dlgTempl;

// Construction
	CPropertyPage(CStringOrId dlgTempl, CStringOrId title = (LPCTSTR)NULL)
	{
		// initialize PROPSHEETPAGE struct
		memset(&m_psp, 0, sizeof(PROPSHEETPAGE));
		m_psp.dwSize = sizeof(PROPSHEETPAGE);
		m_psp.dwFlags = PSP_USECALLBACK;
		m_psp.hInstance = GetResourceInstance();
		m_psp.pszTemplate = dlgTempl;
		m_psp.pfnDlgProc = GetDialogProc();
		m_psp.pfnCallback = GetPropPageCallback();
		m_psp.lParam = (LPARAM)this;

		if(title != NULL)
			SetTitle(title);

		m_dlgTempl = dlgTempl;
		m_hDialogTemplate = NULL;
	}

	~CPropertyPage()
	{
		PostProcessPageTemplate();
	}

	virtual fnPropPageCallback GetPropPageCallback()
	{
		return PropPageCallback;
	}

// Callback function and overrideables
	static UINT CALLBACK PropPageCallback(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
	{
		hWnd;   // avoid level 4 warning
		ASSERT(hWnd == NULL);
		thisClass* pThis = (thisClass*)ppsp->lParam;
		UINT uRet = 0;

		switch(uMsg)
		{
		case PSPCB_CREATE:
			{
				uRet = pThis->OnPageCreate() ? 1 : 0;
				if(uRet)
					HookWindowCreate(pThis);
			}
			break;
#if (_WIN32_IE >= 0x0500)
		case PSPCB_ADDREF:
			pThis->OnPageAddRef();
			break;
#endif // (_WIN32_IE >= 0x0500)
		case PSPCB_RELEASE:
			UnhookWindowCreate();
			pThis->OnPageRelease();
			break;
		default:
			break;
		}

		return uRet;
	}

	virtual bool OnPageCreate()
	{
		return true;   // true - allow page to be created, false - prevent creation
	}

#if (_WIN32_IE >= 0x0500)
	virtual void OnPageAddRef()
	{
	}
#endif // (_WIN32_IE >= 0x0500)

	virtual void OnPageRelease()
	{
	}

// Create method
	HPROPSHEETPAGE Create()
	{
		return ::CreatePropertySheetPage(&m_psp);
	}

// Attributes
	void SetTitle(CStringOrId title)
	{
		m_psp.pszTitle = title;
		m_psp.dwFlags |= PSP_USETITLE;
	}

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	void SetHeaderTitle(LPCTSTR lpstrHeaderTitle)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created
		m_psp.dwFlags |= PSP_USEHEADERTITLE;
		m_psp.pszHeaderTitle = lpstrHeaderTitle;
	}

	void SetHeaderSubTitle(LPCTSTR lpstrHeaderSubTitle)
	{
		ASSERT(m_hWnd == NULL);   // can't do this after it's created
		m_psp.dwFlags |= PSP_USEHEADERSUBTITLE;
		m_psp.pszHeaderSubTitle = lpstrHeaderSubTitle;
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

// Operations
	void EnableHelp()
	{
		m_psp.dwFlags |= PSP_HASHELP;
	}

// Message map and handlers
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	END_MSG_MAP()

	bool OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, LRESULT& lResult)
	{
		ASSERT(::IsWindow(m_hWnd));
		NMHDR* pNMHDR = (NMHDR*)lParam;

		// don't handle messages not from the page/sheet itself
		if(pNMHDR->hwndFrom != m_hWnd && pNMHDR->hwndFrom != ::GetParent(m_hWnd))
		{
			lResult = 1;
			return false;
		}

		bool bRet = true;
		switch(pNMHDR->code)
		{
		case PSN_SETACTIVE:
			lResult = OnSetActive();
			break;
		case PSN_KILLACTIVE:
			lResult = OnKillActive();
			break;
		case PSN_APPLY:
			lResult = OnApply();
			break;
		case PSN_RESET:
			OnReset();
			break;
		case PSN_QUERYCANCEL:
			lResult = OnQueryCancel();
			break;
		case PSN_WIZNEXT:
			lResult = OnWizardNext();
			break;
		case PSN_WIZBACK:
			lResult = OnWizardBack();
			break;
		case PSN_WIZFINISH:
			lResult = OnWizardFinish();
			break;
		case PSN_HELP:
			OnHelp();
			break;
#ifndef _WIN32_WCE
#if (_WIN32_IE >= 0x0400)
		case PSN_GETOBJECT:
			if(!OnGetObject((LPNMOBJECTNOTIFY)lParam))
				bRet = false;
			break;
#endif // (_WIN32_IE >= 0x0400)
#if (_WIN32_IE >= 0x0500)
		case PSN_TRANSLATEACCELERATOR:
			{
				LPPSHNOTIFY lpPSHNotify = (LPPSHNOTIFY)lParam;
				lResult = OnTranslateAccelerator((LPMSG)lpPSHNotify->lParam);
			}
			break;
		case PSN_QUERYINITIALFOCUS:
			{
				LPPSHNOTIFY lpPSHNotify = (LPPSHNOTIFY)lParam;
				lResult = (LRESULT)OnQueryInitialFocus((HWND)lpPSHNotify->lParam);
			}
			break;
#endif // (_WIN32_IE >= 0x0500)
#endif // !_WIN32_WCE
		default:
			bRet = false;   // not handled
		}

		return bRet;
	}

// Overridables
	virtual int OnSetActive()
	{
		// 0 = allow activate
		// -1 = go back that was active
		// page ID = jump to page
		return 0;
	}

	virtual BOOL OnKillActive()
	{
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	virtual int OnApply()
	{
		// PSNRET_NOERROR = apply OK
		// PSNRET_INVALID = apply not OK, return to this page
		// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
		return PSNRET_NOERROR;
	}

	virtual void OnReset()
	{
	}

	virtual BOOL OnQueryCancel()
	{
		// FALSE = allow cancel
		// TRUE = prevent cancel
		return FALSE;
	}

	virtual int OnWizardBack()
	{
		// 0  = goto previous page
		// -1 = prevent page change
		// >0 = jump to page by dlg ID
		return 0;
	}

	virtual int OnWizardNext()
	{
		// 0  = goto next page
		// -1 = prevent page change
		// >0 = jump to page by dlg ID
		return 0;
	}

	virtual INT_PTR OnWizardFinish()
	{
		// FALSE = allow finish
		// TRUE = prevent finish
		// HWND = prevent finish and set focus to HWND (CommCtrl 5.80 only)
		return FALSE;
	}

	virtual void OnHelp()
	{
	}

#ifndef _WIN32_WCE
#if (_WIN32_IE >= 0x0400)
	virtual BOOL OnGetObject(LPNMOBJECTNOTIFY /*lpObjectNotify*/)
	{
		return FALSE;   // not processed
	}
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500)
	virtual int OnTranslateAccelerator(LPMSG /*lpMsg*/)
	{
		// PSNRET_NOERROR - message not handled
		// PSNRET_MESSAGEHANDLED - message handled
		return PSNRET_NOERROR;
	}

	virtual HWND OnQueryInitialFocus(HWND /*hWndFocus*/)
	{
		// NULL = set focus to default control
		// HWND = set focus to HWND
		return NULL;
	}
#endif // (_WIN32_IE >= 0x0500)
#endif // !_WIN32_WCE

public:
	LPCTSTR GetDialogTemplate()
	{
		return m_dlgTempl;
	}

	HGLOBAL m_hDialogTemplate;
	void PreProcessPageTemplate(PROPSHEETPAGE& psp, BOOL bWizard)
	{
		const DLGTEMPLATE* pTemplate;

		if(psp.dwFlags & PSP_DLGINDIRECT)
		{
			pTemplate = psp.pResource;
		}
		else
		{
			HRSRC hResource = ::FindResource(psp.hInstance, psp.pszTemplate, RT_DIALOG);
			HGLOBAL hTemplate = LoadResource(psp.hInstance, hResource);
			pTemplate = (LPCDLGTEMPLATE)LockResource(hTemplate);
		}

		ASSERT(pTemplate != NULL);

		// set font of property page to same font used by property sheet
		HGLOBAL hTemplate = CFontHelper::ChangePropPageFont(pTemplate, bWizard);

		if(hTemplate != NULL)
		{
			pTemplate = (LPCDLGTEMPLATE)hTemplate;
			m_hDialogTemplate = hTemplate;
		}

		psp.pResource = pTemplate;
		psp.dwFlags |= PSP_DLGINDIRECT;
	}

	void PostProcessPageTemplate()
	{
		if(m_hDialogTemplate)
			GlobalFree(m_hDialogTemplate);

		m_hDialogTemplate = NULL;
	}
};


inline BOOL CPropertySheet::AddPage(CPropertyPage& page)
{
	// process property page's font
	page.PreProcessPageTemplate(page.m_psp, IsWizard());
	BOOL bRet = AddPage(&page.m_psp);
	return bRet;
}

/*
///////////////////////////////////////////////////////////////////////////////
// Wizard97 Support

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

// Sample wizard dialog resources:
//
// IDD_WIZ97_INTERIOR_BLANK DIALOG  0, 0, 317, 143
// STYLE DS_SETFONT | WS_CHILD | WS_DISABLED | WS_CAPTION
// CAPTION "Wizard97 Property Page - Interior"
// FONT 8, "MS Shell Dlg"
// BEGIN
// END
//
// IDD_WIZ97_EXTERIOR_BLANK DIALOGEX 0, 0, 317, 193
// STYLE DS_SETFONT | DS_FIXEDSYS | WS_CHILD | WS_DISABLED | WS_CAPTION
// CAPTION "Wizard97 Property Page - Welcome/Complete"
// FONT 8, "MS Shell Dlg", 0, 0, 0x0
// BEGIN
//    LTEXT           "Welcome to the X Wizard",IDC_WIZ97_EXTERIOR_TITLE,115,8,
//                    195,24
//    LTEXT           "Wizard Explanation\r\n(The height of the static text should be in multiples of 8 dlus)",
//                    IDC_STATIC,115,40,195,16
//    LTEXT           "h",IDC_WIZ97_BULLET1,118,64,8,8
//    LTEXT           "List Item 1 (the h is turned into a bullet)",IDC_STATIC,
//                    127,63,122,8
//    LTEXT           "h",IDC_WIZ97_BULLET2,118,79,8,8
//    LTEXT           "List Item 2. Keep 7 dlus between paragraphs",IDC_STATIC,
//                    127,78,33,8
//    CONTROL         "&Do not show this Welcome page again",
//                    IDC_WIZ97_WELCOME_NOTAGAIN,"Button",BS_AUTOCHECKBOX | 
//                    WS_TABSTOP,115,169,138,10
// END
//
// GUIDELINES DESIGNINFO 
// BEGIN
//    IDD_WIZ97_INTERIOR_BLANK, DIALOG
//    BEGIN
//        LEFTMARGIN, 7
//        RIGHTMARGIN, 310
//        VERTGUIDE, 21
//        VERTGUIDE, 31
//        VERTGUIDE, 286
//        VERTGUIDE, 296
//        TOPMARGIN, 7
//        BOTTOMMARGIN, 136
//        HORZGUIDE, 8
//    END
//
//    IDD_WIZ97_EXTERIOR_BLANK, DIALOG
//    BEGIN
//        RIGHTMARGIN, 310
//        VERTGUIDE, 115
//        VERTGUIDE, 118
//        VERTGUIDE, 127
//        TOPMARGIN, 7
//        BOTTOMMARGIN, 186
//        HORZGUIDE, 8
//        HORZGUIDE, 32
//        HORZGUIDE, 40
//        HORZGUIDE, 169
//    END
// END

///////////////////////////////////////////////////////////////////////////////
// CWizard97SheetWindow - client side for a Wizard 97 style wizard sheet

template<typename T>
class TWizard97Sheet : public TPropertySheet<T>
{
public:
// Constructors
	TWizard97Sheet(HWND hWnd = NULL) : TPropertySheet<T>(hWnd)
	{ }

	TWizard97Sheet<T>& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

// Operations
	HFONT GetExteriorPageTitleFont(void)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, GetMessage_GetExteriorPageTitleFont(), 0, 0L);
	}

	HFONT GetBulletFont(void)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, GetMessage_GetBulletFont(), 0, 0L);
	}

// Helpers
	static UINT GetMessage_GetExteriorPageTitleFont()
	{
		static UINT uGetExteriorPageTitleFont = 0;
		if(uGetExteriorPageTitleFont == 0)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CWizard97SheetWindow::GetMessage_GetExteriorPageTitleFont().\n"));
				ASSERT(FALSE);
				return 0;
			}

			if(uGetExteriorPageTitleFont == 0)
				uGetExteriorPageTitleFont = ::RegisterWindowMessage(_T("GetExteriorPageTitleFont_531AF056-B8BE-4c4c-B786-AC608DF0DF12"));

			lock.Unlock();
		}
		ASSERT(uGetExteriorPageTitleFont != 0);
		return uGetExteriorPageTitleFont;
	}

	static UINT GetMessage_GetBulletFont()
	{
		static UINT uGetBulletFont = 0;
		if(uGetBulletFont == 0)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CWizard97SheetWindow::GetMessage_GetBulletFont().\n"));
				ASSERT(FALSE);
				return 0;
			}

			if(uGetBulletFont == 0)
				uGetBulletFont = ::RegisterWindowMessage(_T("GetBulletFont_AD347D08-8F65-45ef-982E-6352E8218AD5"));

			lock.Unlock();
		}
		ASSERT(uGetBulletFont != 0);
		return uGetBulletFont;
	}
};

///////////////////////////////////////////////////////////////////////////////
// CWizard97SheetImpl - implements a Wizard 97 style wizard sheet

class CWizard97Sheet : public CUserWindow, public TPropertySheet<CWizard97Sheet>
{
protected:
// Typedefs
	typedef CWizard97Sheet thisClass;
	typedef TPropertySheet<CWizard97Sheet> baseClass;

// Member variables
	CFont m_fontExteriorPageTitle;   // Welcome and Completion page title font
	CFont m_fontBullet;              // Bullet font (used on static text 'h' to produce a small bullet)
	bool m_bReceivedFirstSizeMessage;   

public:
	CWizard97Sheet(CStringOrId title, CStringOrId headerBitmap, CStringOrId watermarkBitmap, UINT uStartPage = 0, HWND hWndParent = NULL) :
			baseClass(title, uStartPage, hWndParent),
			m_bReceivedFirstSizeMessage(false)
	{
		m_psh.dwFlags &= ~(PSH_NOCONTEXTHELP);
		m_psh.dwFlags &= ~(PSH_WIZARD | PSH_WIZARD_LITE);

		m_psh.dwFlags |= (PSH_HASHELP | PSH_WIZARDCONTEXTHELP);
		m_psh.dwFlags |= PSH_WIZARD97;

		baseClass::SetHeader(headerBitmap.m_lpstr);
		baseClass::SetWatermark(watermarkBitmap.m_lpstr);
	}

// Overrides from base class
	void OnSheetInitialized()
	{
		InitializeFonts();

		// We'd like to center the wizard here, but its too early.
		// Instead, we'll do CenterWindow upon our first WM_SIZE message
	}

// Initialization
	virtual void InitializeFonts()
	{
		// Setup the Title and Bullet Font
		// (Property pages can send the "get external page title font" and "get bullet font" messages)
		// The derived class needs to do the actual SetFont for the dialog items)

		CFontHandle fontThisDialog = this->GetFont();
		CClientDC dcScreen(NULL);

		LOGFONT titleLogFont = {0};
		LOGFONT bulletLogFont = {0};
		fontThisDialog.GetLogFont(&titleLogFont);
		fontThisDialog.GetLogFont(&bulletLogFont);

		// The Wizard 97 Spec recommends to do the Title Font
		// as Verdana Bold, 12pt.
		titleLogFont.lfCharSet = DEFAULT_CHARSET;
		titleLogFont.lfWeight = FW_BOLD;
		lstrcpy(titleLogFont.lfFaceName, _T("Verdana Bold"));

		INT titleFontPointSize = 12;
		titleLogFont.lfHeight = -::MulDiv(titleFontPointSize, dcScreen.GetDeviceCaps(LOGPIXELSY), 72);
		m_fontExteriorPageTitle.CreateFontIndirect(&titleLogFont);

		// The Wizard 97 Spec recommends to do Bullets by having
		// static text of "h" in the Marlett font.
		bulletLogFont.lfCharSet = DEFAULT_CHARSET;
		bulletLogFont.lfWeight = FW_NORMAL;
		lstrcpy(bulletLogFont.lfFaceName, _T("Marlett"));

		INT bulletFontSize = 8;
		bulletLogFont.lfHeight = -::MulDiv(bulletFontSize, dcScreen.GetDeviceCaps(LOGPIXELSY), 72);
		m_fontBullet.CreateFontIndirect(&bulletLogFont);
	}

// Message Handling
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(CWizard97SheetWindow::GetMessage_GetExteriorPageTitleFont(), OnGetExteriorPageTitleFont)
		MESSAGE_HANDLER(CWizard97SheetWindow::GetMessage_GetBulletFont(), OnGetBulletFont)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnGetExteriorPageTitleFont(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return (LRESULT)(HFONT)m_fontExteriorPageTitle;
	}

	LRESULT OnGetBulletFont(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return (LRESULT)(HFONT)m_fontBullet;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(!m_bReceivedFirstSizeMessage)
		{
			m_bReceivedFirstSizeMessage = true;
			CenterWindow();
		}

		bHandled = FALSE;
		return 0;
	}
};

///////////////////////////////////////////////////////////////////////////////
// CWizard97PageWindow - client side for a Wizard 97 style wizard page

#define WIZARD97_EXTERIOR_CXDLG 317
#define WIZARD97_EXTERIOR_CYDLG 193

#define WIZARD97_INTERIOR_CXDLG 317
#define WIZARD97_INTERIOR_CYDLG 143

template<typename T>
class TWizard97Page : public TPropertyPage<T>
{
public:
// Constructors
	TWizard97Page(HWND hWnd = NULL) : TPropertyPage<T>(hWnd)
	{ }

	TWizard97Page<T>& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

// Attributes
	CWizard97SheetWindow GetPropertySheet() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CWizard97SheetWindow(GetParent());
	}

// Operations
	HFONT GetExteriorPageTitleFont(void)
	{
		ASSERT(::IsWindow(m_hWnd));
		return GetPropertySheet().GetExteriorPageTitleFont();
	}

	HFONT GetBulletFont(void)
	{
		ASSERT(::IsWindow(m_hWnd));
		return GetPropertySheet().GetBulletFont();
	}
};


///////////////////////////////////////////////////////////////////////////////
// CWizard97PageImpl - implements a Wizard 97 style wizard page

template <class T, class TBase = CWizard97PageWindow>
class CWizard97PageImpl : public CPropertyPageImpl< T, TBase >
{
protected:
// Typedefs
	typedef CWizard97PageImpl< T, TBase > thisClass;
	typedef CPropertyPageImpl< T, TBase > baseClass;

public:
	CWizard97PageImpl(ATL::_U_STRINGorID title = (LPCTSTR)NULL) : baseClass(title)
	{ }

// Message Handling
	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CWizard97ExteriorPageImpl - implements a Wizard 97 style exterior wizard page

template <class T, class TBase = CWizard97PageWindow>
class CWizard97ExteriorPageImpl : public CPropertyPageImpl< T, TBase >
{
protected:
// Typedefs
	typedef CWizard97ExteriorPageImpl< T, TBase > thisClass;
	typedef CPropertyPageImpl< T, TBase > baseClass;

public:
// Constructors
	CWizard97ExteriorPageImpl(ATL::_U_STRINGorID title = (LPCTSTR)NULL) : baseClass(title)
	{
		m_psp.dwFlags |= PSP_HASHELP;
		m_psp.dwFlags |= PSP_HIDEHEADER;
	}

// Message Handling
	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CWizard97InteriorPageImpl - implements a Wizard 97 style interior wizard page

template <class T, class TBase = CWizard97PageWindow>
class CWizard97InteriorPageImpl : public CPropertyPageImpl< T, TBase >
{
protected:
// Typedefs
	typedef CWizard97InteriorPageImpl< T, TBase > thisClass;
	typedef CPropertyPageImpl< T, TBase > baseClass;

public:
// Constructors
	CWizard97InteriorPageImpl(ATL::_U_STRINGorID title = (LPCTSTR)NULL) : baseClass(title)
	{
		m_psp.dwFlags |= PSP_HASHELP;
		m_psp.dwFlags &= ~PSP_HIDEHEADER;
		m_psp.dwFlags |= PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;

		// Be sure to have the derived class define this in the constructor.
		// We'll default it to something obvious in case its forgotten.
		baseClass::SetHeaderTitle(_T("Call SetHeaderTitle in Derived Class"));
		baseClass::SetHeaderSubTitle(_T("Call SetHeaderSubTitle in the constructor of the Derived Class."));
	}

// Message Handling
	BEGIN_MSG_MAP()
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};

#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
// */

}; // namespace

#endif // __PROP_H__
