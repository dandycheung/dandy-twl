//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __CCUTIL_H__
#define __CCUTIL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <stdlib.h>		// for abs()

#include "resutil.h"
#include "modutil.h"

namespace TWL
{
#ifndef RC_INVOKED
  // #define RT_DLGINIT  MAKEINTRESOURCE(240)
  #define RT_TOOLBAR  MAKEINTRESOURCE(241)
#endif // RC_INVOKED

// standard toolbar styles
#define TWL_SIMPLE_TOOLBAR_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS)
// toolbar in a rebar pane
#define TWL_SIMPLE_TOOLBAR_PANE_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT)
// standard rebar styles
#if (_WIN32_IE >= 0x0400)
  #define TWL_SIMPLE_REBAR_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_VARHEIGHT | RBS_BANDBORDERS | RBS_AUTOSIZE)
#else
  #define TWL_SIMPLE_REBAR_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_VARHEIGHT | RBS_BANDBORDERS)
#endif //!(_WIN32_IE >= 0x0400)
// rebar without borders
#if (_WIN32_IE >= 0x0400)
  #define TWL_SIMPLE_REBAR_NOBORDER_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_VARHEIGHT | RBS_BANDBORDERS | RBS_AUTOSIZE | CCS_NODIVIDER)
#else
  #define TWL_SIMPLE_REBAR_NOBORDER_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_VARHEIGHT | RBS_BANDBORDERS | CCS_NODIVIDER)
#endif //!(_WIN32_IE >= 0x0400)

typedef struct tagTOOLBARDATA
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	// WORD aItems[wItemCount]

	PWORD items()
		{ return (WORD*)(this + 1); }
} TOOLBARDATA, *PTOOLBARDATA;

class CCCUtil
{
public:
	static 
	HWND CreateSimpleToolbar(HWND hwndParent, UINT uResId, BOOL bInitialSeparator = FALSE, 
			DWORD dwStyle = TWL_SIMPLE_TOOLBAR_STYLE, UINT uId = TWL_IDW_TOOLBAR)
	{
		HINSTANCE hInst = GetResourceInstance();
		HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uResId), RT_TOOLBAR);
		if(hRsrc == NULL)
			return NULL;

		HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
		if (hGlobal == NULL)
			return NULL;

		PTOOLBARDATA pData = (PTOOLBARDATA)::LockResource(hGlobal);
		if (pData == NULL)
			return NULL;

		ASSERT(pData->wVersion == 1);

		PWORD pItems = pData->items();
		int nItems = pData->wItemCount + (bInitialSeparator ? 1 : 0);
		PTBBUTTON pTBBtn = new TBBUTTON[nItems];

		const int cxSeparator = 8;

		// set initial separator (half width)
		if(bInitialSeparator)
		{
			pTBBtn[0].iBitmap = cxSeparator / 2;
			pTBBtn[0].idCommand = 0;
			pTBBtn[0].fsState = 0;
			pTBBtn[0].fsStyle = TBSTYLE_SEP;
			pTBBtn[0].dwData = 0;
			pTBBtn[0].iString = 0;
		}

		int nBmp = 0;
		for(int i = 0, j = bInitialSeparator ? 1 : 0; i < pData->wItemCount; i++, j++)
		{
			if(pItems[i] != 0)
			{
				pTBBtn[j].iBitmap = nBmp++;
				pTBBtn[j].idCommand = pItems[i];
				pTBBtn[j].fsState = TBSTATE_ENABLED;
				pTBBtn[j].fsStyle = TBSTYLE_BUTTON;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
			else
			{
				pTBBtn[j].iBitmap = cxSeparator;
				pTBBtn[j].idCommand = 0;
				pTBBtn[j].fsState = 0;
				pTBBtn[j].fsStyle = TBSTYLE_SEP;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
		}

		HWND hWnd = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, dwStyle, 0,0,100,100,
				hwndParent, (HMENU)uId, GetModuleInstance(), NULL);

		if(hWnd == NULL)
		{
			if(pTBBtn)
				delete[] pTBBtn;

			return NULL;
		}

		::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

		// check if font is taller than our bitmaps
		HFONT font = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L);
		if(font == NULL)
			font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);;

		LOGFONT lf = { 0 };
		::GetObject(font, sizeof(LOGFONT), &lf);
		WORD cyFontHeight = (WORD)abs(lf.lfHeight);

		WORD bitsPerPixel = GetBitmapResourceBitDepth(uResId);
		if(bitsPerPixel > 4)
		{
			COLORREF crMask = CLR_DEFAULT;
			if(bitsPerPixel == 32)
			{
				// 32-bit color bitmap with alpha channel (valid for Windows XP and later)
				crMask = CLR_NONE;
			}

			HIMAGELIST hImageList = ImageList_LoadImage(GetResourceInstance(), MAKEINTRESOURCE(uResId), pData->wWidth, 1, crMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			ASSERT(hImageList != NULL);
			::SendMessage(hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
		}
		else
		{
			TBADDBITMAP tbab = { 0 };
			tbab.hInst = hInst;
			tbab.nID = uResId;
			::SendMessage(hWnd, TB_ADDBITMAP, nBmp, (LPARAM)&tbab);
		}

		::SendMessage(hWnd, TB_ADDBUTTONS, nItems, (LPARAM)pTBBtn);
		::SendMessage(hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(pData->wWidth, max(pData->wHeight, cyFontHeight)));

		const int cxyButtonMargin = 7;
		::SendMessage(hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(pData->wWidth + cxyButtonMargin, max(pData->wHeight, cyFontHeight) + cxyButtonMargin));

		if(pTBBtn)
			delete[] pTBBtn;

		return hWnd;
	}

	static 
	HWND CreateSimpleRebar(HWND hwndParent, DWORD dwStyle = TWL_SIMPLE_REBAR_STYLE, UINT uId = TWL_IDW_TOOLBAR)
	{
		// Ensure style combinations for proper rebar painting
		if(dwStyle & CCS_NODIVIDER && dwStyle & WS_BORDER)
			dwStyle &= ~WS_BORDER;
		else if(!(dwStyle & WS_BORDER) && !(dwStyle & CCS_NODIVIDER))
			dwStyle |= CCS_NODIVIDER;

		// Create rebar window
		HWND hwndRebar = ::CreateWindowEx(0, REBARCLASSNAME, NULL, dwStyle, 0, 0, 100, 100, hwndParent, (HMENU)uId, GetModuleInstance(), NULL);
		if(hwndRebar == NULL)
			return NULL;

		// Initialize and send the REBARINFO structure
		REBARINFO rbi = { 0 };
		rbi.cbSize = sizeof(REBARINFO);
		rbi.fMask  = 0;
		if(!::SendMessage(hwndRebar, RB_SETBARINFO, 0, (LPARAM)&rbi))
		{
			::DestroyWindow(hwndRebar);
			return NULL;
		}

		return hwndRebar;
	}

	static 
	BOOL AddSimpleRebarBand(HWND hwndRebar, HWND hwndBand, int uId = 0, LPTSTR pszTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		ASSERT(::IsWindow(hwndRebar));   // must be already created
#ifdef _DEBUG
		// block - check if this is really a rebar
		{
			TCHAR lpszClassName[sizeof(REBARCLASSNAME)] = { 0 };
			::GetClassName(hwndRebar, lpszClassName, sizeof(REBARCLASSNAME));
			ASSERT(lstrcmpi(lpszClassName, REBARCLASSNAME) == 0);
		}
#endif //_DEBUG
		ASSERT(::IsWindow(hwndBand));   // must be already created

		// Get number of buttons on the toolbar
		int nBtnCount = (int)::SendMessage(hwndBand, TB_BUTTONCOUNT, 0, 0L);

		// Set band info structure
		REBARBANDINFO rbBand = { 0 };
		rbBand.cbSize = sizeof(REBARBANDINFO);
#if (_WIN32_IE >= 0x0400)
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE | RBBIM_IDEALSIZE;
#else
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE;
#endif //!(_WIN32_IE >= 0x0400)
		if(pszTitle != NULL)
			rbBand.fMask |= RBBIM_TEXT;
		rbBand.fStyle = RBBS_CHILDEDGE;
#if (_WIN32_IE >= 0x0500)
		if(nBtnCount > 0)   // add chevron style for toolbar with buttons
			rbBand.fStyle |= RBBS_USECHEVRON;
#endif //(_WIN32_IE >= 0x0500)
		if(bNewRow)
			rbBand.fStyle |= RBBS_BREAK;

		rbBand.lpText = pszTitle;
		rbBand.hwndChild = hwndBand;
		if(uId == 0)   // calc band ID
			uId = TWL_IDW_BAND_FIRST + (int)::SendMessage(hwndRebar, RB_GETBANDCOUNT, 0, 0L);
		rbBand.wID = uId;

		// Calculate the size of the band
		BOOL bRet = FALSE;
		RECT rcTmp = { 0 };
		if(nBtnCount > 0)
		{
			bRet = (BOOL)::SendMessage(hwndBand, TB_GETITEMRECT, nBtnCount - 1, (LPARAM)&rcTmp);
			ASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : rcTmp.right;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
			if(bFullWidthAlways)
			{
				rbBand.cxMinChild = rbBand.cx;
			}
			else if(pszTitle == 0)
			{
				bRet = (BOOL)::SendMessage(hwndBand, TB_GETITEMRECT, 0, (LPARAM)&rcTmp);
				ASSERT(bRet);
				rbBand.cxMinChild = rcTmp.right;
			}
			else
			{
				rbBand.cxMinChild = 0;
			}
		}
		else	// no buttons, either not a toolbar or really has no buttons
		{
			bRet = ::GetWindowRect(hwndBand, &rcTmp);
			ASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : (rcTmp.right - rcTmp.left);
			rbBand.cxMinChild = bFullWidthAlways ? rbBand.cx : 0;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
		}

#if (_WIN32_IE >= 0x0400)
		rbBand.cxIdeal = rbBand.cx;
#endif //(_WIN32_IE >= 0x0400)

		// Add the band
		LRESULT lRes = ::SendMessage(hwndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
		if(lRes == 0)
			return FALSE;

#if (_WIN32_IE >= 0x0501)
		DWORD dwExStyle = (DWORD)::SendMessage(hwndBand, TB_GETEXTENDEDSTYLE, 0, 0L);
		::SendMessage(hwndBand, TB_SETEXTENDEDSTYLE, 0, dwExStyle | TBSTYLE_EX_HIDECLIPPEDBUTTONS);
#endif //(_WIN32_IE >= 0x0501)

		return TRUE;
	}

#if (_WIN32_IE >= 0x0400)
	static 
	void SizeSimpleRebarBands(HWND hwndRebar)
	{
		ASSERT(::IsWindow(hwndRebar));   // must be an existing rebar

		int nCount = (int)::SendMessage(hwndRebar, RB_GETBANDCOUNT, 0, 0L);
		for(int i = 0; i < nCount; i++)
		{
			REBARBANDINFO rbBand = { 0 };
			rbBand.cbSize = sizeof(REBARBANDINFO);
			rbBand.fMask = RBBIM_SIZE;

			BOOL bRet = (BOOL)::SendMessage(hwndRebar, RB_GETBANDINFO, i, (LPARAM)&rbBand);
			ASSERT(bRet);

			RECT rect = { 0, 0, 0, 0 };
			::SendMessage(hwndRebar, RB_GETBANDBORDERS, i, (LPARAM)&rect);

			rbBand.cx += rect.left + rect.right;
			bRet = (BOOL)::SendMessage(hwndRebar, RB_SETBANDINFO, i, (LPARAM)&rbBand);

			ASSERT(bRet);
		}
	}
#endif //(_WIN32_IE >= 0x0400)

	static 
	HWND CreateSimpleStatusBar(HWND hwndParent, LPCTSTR pszText, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP, UINT uId = TWL_IDW_STATUS_BAR)
	{
		HWND hwndStatusBar = ::CreateStatusWindow(dwStyle, pszText, hwndParent, uId);
		return hwndStatusBar;
	}

	static 
	HWND CreateSimpleStatusBar(HWND hwndParent, UINT uTextId, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP, UINT uId = TWL_IDW_STATUS_BAR)
	{
		const int cchMax = 128;   // max text length is 127 for status bars (+1 for null)
		TCHAR szText[cchMax];
		szText[0] = 0;
		::LoadString(GetResourceInstance(), uTextId, szText, cchMax);
		return CreateSimpleStatusBar(hwndParent, szText, dwStyle, uId);
	}
};

}; // namespace TWL

#endif // __CCUTIL_H__
