//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __DLGTEMPL_H__
#define __DLGTEMPL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "str.h"		// for CString
#include "resutil.h"	// for GetResourceInstance()

namespace TWL
{

#pragma pack(push, 1)
typedef struct tagDLGTEMPLATEEX
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;

	// Everything else in this structure is variable length,
	// and therefore must be determined dynamically

	// sz_Or_Ord menu;			// name or ordinal of a menu resource
	// sz_Or_Ord windowClass;	// name or ordinal of a window class
	// WCHAR title[titleLen];	// title string of the dialog box
	// short pointsize;			// only if DS_SETFONT is set
	// short weight;			// only if DS_SETFONT is set
	// short bItalic;			// only if DS_SETFONT is set
	// WCHAR font[fontLen];		// typeface name, if DS_SETFONT is set
} DLGTEMPLATEEX, *PDLGTEMPLATEEX, *LPDLGTEMPLATEEX;

typedef struct DLGITEMTEMPLATEEX
{
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	short x;
	short y;
	short cx;
	short cy;
	DWORD id;

	// Everything else in this structure is variable length,
	// and therefore must be determined dynamically

	// sz_Or_Ord windowClass;	// name or ordinal of a window class
	// sz_Or_Ord title;			// title string or ordinal of a resource
	// WORD extraCount;			// bytes following creation data
} DLGITEMTEMPLATEEX, *PDLGITEMTEMPLATEEX, *LPDLGITEMTEMPLATEEX;
#pragma pack(pop)

class CDialogTemplate
{
// Constructors
public:
	CDialogTemplate(const DLGTEMPLATE* pTemplate = NULL)
	{
		if(pTemplate == NULL)
		{
			m_hTemplate = NULL;
			m_dwTemplateSize = 0;
			m_bSystemFont = FALSE;
		}
		else
		{
			SetTemplate(pTemplate, GetTemplateSize(pTemplate));
		}
	}

	CDialogTemplate(HGLOBAL hTemplate)
	{
		if(hTemplate == NULL)
		{
			m_hTemplate = NULL;
			m_dwTemplateSize = 0;
			m_bSystemFont = FALSE;
		}
		else
		{
			DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);
			SetTemplate(pTemplate, GetTemplateSize(pTemplate));
			GlobalUnlock(hTemplate);
		}
	}

// Attributes
	BOOL HasFont() const
	{
		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(m_hTemplate);
		BOOL bHasFont = HasFont(pTemplate);
		GlobalUnlock(m_hTemplate);
		return bHasFont;
	}

	BOOL SetFont(LPCTSTR pszFaceName, WORD nFontSize)
	{
		ASSERT(m_hTemplate != NULL);

		if(m_dwTemplateSize == 0)
			return FALSE;

		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(m_hTemplate);

		BOOL bDialogEx = IsDialogEx(pTemplate);
		BOOL bHasFont = HasFont(pTemplate);
		int cbFontAttr = FontAttrSize(bDialogEx);

		if(bDialogEx)
			((DLGTEMPLATEEX*)pTemplate)->style |= DS_SETFONT;
		else
			pTemplate->style |= DS_SETFONT;

#ifdef _UNICODE
		int cbNew = cbFontAttr + ((lstrlen(pszFaceName) + 1) * sizeof(TCHAR));
		BYTE* pbNew = (BYTE*)pszFaceName;
#else
		WCHAR wszFaceName[LF_FACESIZE];
		int cbNew = cbFontAttr + 2 * MultiByteToWideChar(CP_ACP, 0, pszFaceName, -1, wszFaceName, LF_FACESIZE);
		BYTE* pbNew = (BYTE*)wszFaceName;
#endif

		BYTE* pb = GetFontSizeField(pTemplate);
		int cbOld = bHasFont ? cbFontAttr + 2 * (wcslen((WCHAR*)(pb + cbFontAttr)) + 1) : 0;

		BYTE* pOldControls = (BYTE*)(((DWORD)pb + cbOld + 3) & ~3);
		BYTE* pNewControls = (BYTE*)(((DWORD)pb + cbNew + 3) & ~3);

		WORD nCtrl = bDialogEx ? (WORD)((DLGTEMPLATEEX*)pTemplate)->cDlgItems : (WORD)pTemplate->cdit;

		if(cbNew != cbOld && nCtrl > 0)
			memmove(pNewControls, pOldControls, (size_t)(m_dwTemplateSize - (pOldControls - (BYTE*)pTemplate)));

		*(WORD*)pb = nFontSize;
		memmove(pb + cbFontAttr, pbNew, cbNew - cbFontAttr);

		m_dwTemplateSize += (pNewControls - pOldControls);

		GlobalUnlock(m_hTemplate);
		m_bSystemFont = FALSE;

		return TRUE;
	}

	BOOL SetSystemFont(WORD nFontSize = 0)
	{
		LPCTSTR pszFace = _T("System");
		WORD wDefSize = 10;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		if(hFont == NULL)
			hFont = (HFONT)::GetStockObject(SYSTEM_FONT);

		if(hFont != NULL)
		{
			LOGFONT lf;
			if(::GetObject(hFont, sizeof(LOGFONT), &lf) != 0)
			{
				pszFace = lf.lfFaceName;
				HDC hdc = ::GetDC(NULL);

				if(lf.lfHeight < 0)
					lf.lfHeight = -lf.lfHeight;

				wDefSize = (WORD)MulDiv(lf.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));

				::ReleaseDC(NULL, hdc);
			}
		}

		if(nFontSize == 0)
			nFontSize = wDefSize;

		return SetFont(pszFace, nFontSize);
	}

	BOOL GetFont(CString& strFaceName, WORD& nFontSize) const
	{
		ASSERT(m_hTemplate != NULL);

		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(m_hTemplate);
		BOOL bResult = GetFont(pTemplate, strFaceName, nFontSize);
		GlobalUnlock(m_hTemplate);

		return bResult;
	}

	void GetSizeInDialogUnits(SIZE* pSize) const
	{
		ASSERT(m_hTemplate != NULL);
		ASSERT(pSize != NULL);

		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(m_hTemplate);

		if(IsDialogEx(pTemplate))
		{
			pSize->cx = ((DLGTEMPLATEEX*)pTemplate)->cx;
			pSize->cy = ((DLGTEMPLATEEX*)pTemplate)->cy;
		}
		else
		{
			pSize->cx = pTemplate->cx;
			pSize->cy = pTemplate->cy;
		}

		GlobalUnlock(m_hTemplate);
	}

	void GetSizeInPixels(SIZE* pSize) const
	{
		ASSERT(m_hTemplate != NULL);
		ASSERT(pSize != NULL);

		if(m_bSystemFont)
		{
			GetSizeInDialogUnits(pSize);

			DWORD dwDLU = GetDialogBaseUnits();
			pSize->cx = (pSize->cx * LOWORD(dwDLU)) / 4;
			pSize->cy = (pSize->cy * HIWORD(dwDLU)) / 8;
		}
		else
		{
			CString strFace;
			WORD wSize;
			GetFont(strFace, wSize);

			SIZE size;
			GetSizeInDialogUnits(&size);
			ConvertDialogUnitsToPixels(strFace, wSize, size.cx, size.cy, pSize);
		}
	}

	static BOOL GetFont(const DLGTEMPLATE* pTemplate, CString& strFaceName, WORD& nFontSize)
	{
		ASSERT(pTemplate != NULL);

		if(!HasFont(pTemplate))
			return FALSE;

		BYTE* pb = GetFontSizeField(pTemplate);
		nFontSize = *(WORD*)pb;
		pb += FontAttrSize(IsDialogEx(pTemplate));

#if defined(_UNICODE) || defined(OLE2ANSI)
		// Copy font name
		strFaceName = (LPCTSTR)pb;
#else
		// Convert Unicode font name to MBCS
		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pb, -1,
			strFaceName.GetBufferSetLength(LF_FACESIZE), LF_FACESIZE, NULL, NULL);
		strFaceName.ReleaseBuffer();
#endif

		return TRUE;
	}

// Operations
	BOOL Load(LPCTSTR lpDialogTemplateID)
	{
		HINSTANCE hInst = GetResourceInstance(); // AfxFindResourceHandle(lpDialogTemplateID, RT_DIALOG);
		if(hInst == NULL)
			return FALSE;

		HRSRC hRsrc = FindResource(hInst, lpDialogTemplateID, RT_DIALOG);
		if(hRsrc == NULL)
			return FALSE;

		HGLOBAL hTemplate = LoadResource(hInst, hRsrc);
		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)LockResource(hTemplate);
		SetTemplate(pTemplate, (UINT)SizeofResource(hInst, hRsrc));

		UnlockResource(hTemplate);
		FreeResource(hTemplate);

		return TRUE;
	}

	HGLOBAL Detach()
	{
		HGLOBAL hTmp = m_hTemplate;
		m_hTemplate = NULL;

		return hTmp;
	}

// Implementation
public:
	~CDialogTemplate()
	{
		if(m_hTemplate != NULL)
			GlobalFree(m_hTemplate);
	}

	HGLOBAL m_hTemplate;
	DWORD m_dwTemplateSize;
	BOOL m_bSystemFont;

protected:
	static BYTE* GetFontSizeField(const DLGTEMPLATE* pTemplate)
	{
		BOOL bDialogEx = IsDialogEx(pTemplate);
		WORD* pw;

		if(bDialogEx)
			pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
		else
			pw = (WORD*)(pTemplate + 1);

		if(*pw == (WORD)-1)        // Skip menu name string or ordinal
			pw += 2; // WORDs
		else
			while(*pw++);

		if(*pw == (WORD)-1)        // Skip class name string or ordinal
			pw += 2; // WORDs
		else
			while(*pw++);

		while(*pw++);          // Skip caption string

		return (BYTE*)pw;
	}

	static WCHAR* _SkipString(WCHAR* p)
	{
		while(*p++);
		return p;
	}

	static UINT GetTemplateSize(const DLGTEMPLATE* pTemplate)
	{
		BOOL bDialogEx = IsDialogEx(pTemplate);
		BYTE* pb = GetFontSizeField(pTemplate);

		if(HasFont(pTemplate))
		{
			// Skip font size and name
			pb += FontAttrSize(bDialogEx);  // Skip font size, weight, (italic, charset)
			pb += 2 * (wcslen((WCHAR*)pb) + 1);
		}

		WORD nCtrl = bDialogEx ? (WORD)((DLGTEMPLATEEX*)pTemplate)->cDlgItems :
			(WORD)pTemplate->cdit;

		while(nCtrl > 0)
		{
			pb = (BYTE*)(((DWORD)pb + 3) & ~3); // DWORD align

			pb += (bDialogEx ? sizeof(DLGITEMTEMPLATEEX) : sizeof(DLGITEMTEMPLATE));

			if(*(WORD*)pb == (WORD)-1)     // Skip class name string or ordinal
				pb += 2 * sizeof(WORD);
			else
				pb = (BYTE*)_SkipString((WCHAR*)pb);

			if(*(WORD*)pb == (WORD)-1)     // Skip text string or ordinal
				pb += 2 * sizeof(WORD);
			else
				pb = (BYTE*)_SkipString((WCHAR*)pb);

			WORD cbExtra = *(WORD*)pb;      // Skip extra data
			pb += sizeof(WORD) + cbExtra;
			--nCtrl;
		}

		return pb - (BYTE*)pTemplate;
	}

	BOOL SetTemplate(const DLGTEMPLATE* pTemplate, UINT cb)
	{
		m_dwTemplateSize = cb;

		if((m_hTemplate = GlobalAlloc(GPTR, m_dwTemplateSize + LF_FACESIZE * 2)) == NULL)
			return FALSE;

		DLGTEMPLATE* pNew = (DLGTEMPLATE*)GlobalLock(m_hTemplate);
		memcpy((BYTE*)pNew, pTemplate, (size_t)m_dwTemplateSize);
		m_bSystemFont = (HasFont(pNew) == 0);
		GlobalUnlock(m_hTemplate);

		return TRUE;
	}

public:
	static void ConvertDialogUnitsToPixels(LPCTSTR pszFontFace, WORD wFontSize,
		int cxDlg, int cyDlg, SIZE* pSizePixel)
	{
		// Attempt to create the font to be used in the dialog box
		UINT cxSysChar, cySysChar;
		LOGFONT lf;
		HDC hdc = ::GetDC(NULL);
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = -MulDiv(wFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = DEFAULT_CHARSET;
		lstrcpy(lf.lfFaceName, pszFontFace);

		HFONT hNewFont = CreateFontIndirect(&lf);
		if(hNewFont != NULL)
		{
			HFONT hFontOld = (HFONT)SelectObject(hdc, hNewFont);

			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			cySysChar = tm.tmHeight + tm.tmExternalLeading;

			SIZE size;
			::GetTextExtentPoint32(hdc, _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52, &size);

			cxSysChar = (size.cx + 26) / 52;

			SelectObject(hdc, hFontOld);
			DeleteObject(hNewFont);
		}
		else
		{
			// Could not create the font so just use the system's values
			cxSysChar = LOWORD(GetDialogBaseUnits());
			cySysChar = HIWORD(GetDialogBaseUnits());
		}

		::ReleaseDC(NULL, hdc);

		// Translate dialog units to pixels
		pSizePixel->cx = MulDiv(cxDlg, cxSysChar, 4);
		pSizePixel->cy = MulDiv(cyDlg, cySysChar, 8);
	}

	static BOOL IsDialogEx(const DLGTEMPLATE* pTemplate)
	{
		return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
	}

	static BOOL HasFont(const DLGTEMPLATE* pTemplate)
	{
		return (DS_SETFONT & (IsDialogEx(pTemplate) ? ((DLGTEMPLATEEX*)pTemplate)->style : pTemplate->style));
	}

	static int FontAttrSize(BOOL bDialogEx)
	{
		return sizeof(WORD) * (bDialogEx ? 3 : 1);
	}
};

}; // namespace

#endif // __DLGTEMPL_H__
