//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __STDCTL_H__
#define __STDCTL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CStatic
// CButton
// CListBox
// CComboBox
// CEdit
// CScrollBar
//

namespace TWL
{

typedef CUserWindow	TBase;

// --- Standard Windows controls ---

///////////////////////////////////////////////////////////////////////////////
// CStatic - client side for a Windows STATIC control

class CStatic : public TBase
{
public:
// Constructors
	CStatic(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CStatic& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("STATIC");
	}

#ifndef _WIN32_WCE
	HICON GetIcon() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, STM_GETICON, 0, 0L);
	}

	HICON SetIcon(HICON hIcon)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)hIcon, 0L);
	}

	HENHMETAFILE GetEnhMetaFile() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HENHMETAFILE)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);
	}

	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HENHMETAFILE)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
	}
#else // CE specific
	HICON GetIcon() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ICON, 0L);
	}

	HICON SetIcon(HICON hIcon)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}
#endif // _WIN32_WCE

	HBITMAP GetBitmap() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HBITMAP)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L);
	}

	HBITMAP SetBitmap(HBITMAP hBitmap)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HBITMAP)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	HCURSOR GetCursor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HCURSOR)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_CURSOR, 0L);
	}

	HCURSOR SetCursor(HCURSOR hCursor)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HCURSOR)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}
};

///////////////////////////////////////////////////////////////////////////////
// CButton - client side for a Windows BUTTON control

class CButton : public TBase
{
public:
// Constructors
	CButton(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CButton& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("BUTTON");
	}

	UINT GetState() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0L);
	}

	void SetState(BOOL bHighlight)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0L);
	}

	int GetCheck() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0L);
	}

	void SetCheck(int nCheck)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0L);
	}

	UINT GetButtonStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF;
	}

	void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, BM_SETSTYLE, nStyle, (LPARAM)bRedraw);
	}

#ifndef _WIN32_WCE
	HICON GetIcon() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L);
	}

	HICON SetIcon(HICON hIcon)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	HBITMAP GetBitmap() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L);
	}

	HBITMAP SetBitmap(HBITMAP hBitmap)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}
#endif // !_WIN32_WCE

#if (_WIN32_WINNT >= 0x0501)
	BOOL GetIdealSize(LPSIZE lpSize) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, BCM_GETIDEALSIZE, 0, (LPARAM)lpSize);
	}

	BOOL GetImageList(PBUTTON_IMAGELIST pButtonImagelist) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, BCM_GETIMAGELIST, 0, (LPARAM)pButtonImagelist);
	}

	BOOL SetImageList(PBUTTON_IMAGELIST pButtonImagelist)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, BCM_SETIMAGELIST, 0, (LPARAM)pButtonImagelist);
	}

	BOOL GetTextMargin(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, BCM_GETTEXTMARGIN, 0, (LPARAM)lpRect);
	}

	BOOL SetTextMargin(LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, BCM_SETTEXTMARGIN, 0, (LPARAM)lpRect);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	void Click()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, BM_CLICK, 0, 0L);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CListBox - client side for a Windows LISTBOX control

class CListBox : public TBase
{
public:
// Constructors
	CListBox(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CListBox& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("LISTBOX");
	}

	// for entire listbox
	int GetCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0L);
	}

#ifndef _WIN32_WCE
	int SetCount(int cItems)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(((GetStyle() & LBS_NODATA) != 0) && ((GetStyle() & LBS_HASSTRINGS) == 0));
		return (int)::SendMessage(m_hWnd, LB_SETCOUNT, cItems, 0L);
	}
#endif // !_WIN32_WCE

	int GetHorizontalExtent() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT, 0, 0L);
	}

	void SetHorizontalExtent(int cxExtent)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, cxExtent, 0L);
	}

	int GetTopIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0L);
	}

	int SetTopIndex(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0L);
	}

	LCID GetLocale() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0L);
	}

	LCID SetLocale(LCID nNewLocale)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0L);
	}

#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
	DWORD GetListBoxInfo() const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
		return (DWORD)::SendMessage(m_hWnd, LB_GETLISTBOXINFO, 0, 0L);
#else // !(_WIN32_WINNT >= 0x0501)
		return ::GetListBoxInfo(m_hWnd);
#endif // !(_WIN32_WINNT >= 0x0501)
	}
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)

	// for single-selection listboxes
	int GetCurSel() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
		return (int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0L);
	}

	int SetCurSel(int nSelect)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
		return (int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0L);
	}

	// for multiple-selection listboxes
	int GetSel(int nIndex) const           // also works for single-selection
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETSEL, nIndex, 0L);
	}

	int SetSel(int nIndex, BOOL bSelect = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		return (int)::SendMessage(m_hWnd, LB_SETSEL, bSelect, nIndex);
	}

	int GetSelCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		return (int)::SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0L);
	}

	int GetSelItems(int nMaxItems, LPINT rgIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		return (int)::SendMessage(m_hWnd, LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex);
	}

	int GetAnchorIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		return (int)::SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0L);
	}

	void SetAnchorIndex(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		::SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0L);
	}

	int GetCaretIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0);
	}

	int SetCaretIndex(int nIndex, BOOL bScroll = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0));
	}

	// for listbox items
	DWORD_PTR GetItemData(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD_PTR)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
	}

	int SetItemData(int nIndex, DWORD_PTR dwItemData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	void* GetItemDataPtr(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (void*)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
	}

	int SetItemDataPtr(int nIndex, void* pData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItemData(nIndex, (DWORD_PTR)pData);
	}

	int GetItemRect(int nIndex, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	int GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
	}

#ifndef _TWL_NO_COM
#ifdef _OLEAUTO_H_
#ifdef __STRCONV_H__
	BOOL GetTextBSTR(int nIndex, BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);

		int nLen = GetTextLen(nIndex);
		if(nLen == LB_ERR)
			return FALSE;

		LPTSTR lpszText = (LPTSTR)_alloca((nLen + 1) * sizeof(TCHAR));

		if(GetText(nIndex, lpszText) == LB_ERR)
			return FALSE;

		bstrText = ::SysAllocString(T2OLE(lpszText));
		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // _OLEAUTO_H_
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	int GetText(int nIndex, CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		int cchLen = GetTextLen(nIndex);
		if(cchLen == LB_ERR)
			return LB_ERR;
		int nRet = LB_ERR;
		LPTSTR lpstr = strText.GetBufferSetLength(cchLen);
		if(lpstr != NULL)
		{
			nRet = GetText(nIndex, lpstr);
			strText.ReleaseBuffer();
		}
		return nRet;
	}
#endif // defined(__STR_H__)

	int GetTextLen(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETTEXTLEN, nIndex, 0L);
	}

	int GetItemHeight(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L);
	}

	int SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	// Settable only attributes
	void SetColumnWidth(int cxWidth)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, cxWidth, 0L);
	}

	BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
		return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	BOOL SetTabStops()
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
		return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 0, 0L);
	}

	BOOL SetTabStops(const int& cxEachStop)    // takes an 'int'
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
		return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

// Operations
	int InitStorage(int nItems, UINT nBytes)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM)nItems, nBytes);
	}

	void ResetContent()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0L);
	}

	UINT ItemFromPoint(POINT pt, BOOL& bOutside) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
		bOutside = (BOOL)HIWORD(dw);
		return (UINT)LOWORD(dw);
	}

	// manipulating listbox items
	int AddString(LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
	}

	int DeleteString(UINT nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0L);
	}

	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
	}

#ifndef _WIN32_WCE
	int Dir(UINT attr, LPCTSTR lpszWildCard)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_DIR, attr, (LPARAM)lpszWildCard);
	}

	int AddFile(LPCTSTR lpstrFileName)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_ADDFILE, 0, (LPARAM)lpstrFileName);
	}
#endif // !_WIN32_WCE

	// selection helpers
	int FindString(int nStartAfter, LPCTSTR lpszItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
	}

	int SelectString(int nStartAfter, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
		ASSERT(nFirstItem <= nLastItem);
		return bSelect ? (int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem) : (int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CComboBox - client side for a Windows COMBOBOX control

class CComboBox : public TBase
{
public:
// Constructors
	CComboBox(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CComboBox& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("COMBOBOX");
	}

	// for entire combo box
	int GetCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0L);
	}

	int GetCurSel() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
	}

	int SetCurSel(int nSelect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0L);
	}

	LCID GetLocale() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0L);
	}

	LCID SetLocale(LCID nNewLocale)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0L);
	}

	int GetTopIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0L);
	}

	int SetTopIndex(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETTOPINDEX, nIndex, 0L);
	}

	UINT GetHorizontalExtent() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0L);
	}

	void SetHorizontalExtent(UINT nExtent)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, nExtent, 0L);
	}

	int GetDroppedWidth() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0L);
	}

	int SetDroppedWidth(UINT nWidth)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, nWidth, 0L);
	}

#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
	BOOL GetComboBoxInfo(PCOMBOBOXINFO pComboBoxInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
		return (BOOL)::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)pComboBoxInfo);
#else // !(_WIN32_WINNT >= 0x0501)
		return ::GetComboBoxInfo(m_hWnd, pComboBoxInfo);
#endif // !(_WIN32_WINNT >= 0x0501)
	}
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)

	// for edit control
	DWORD GetEditSel() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0L);
	}

	BOOL SetEditSel(int nStartChar, int nEndChar)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELONG(nStartChar, nEndChar));
	}

	// for combobox item
	DWORD_PTR GetItemData(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD_PTR)::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0L);
	}

	int SetItemData(int nIndex, DWORD_PTR dwItemData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	void* GetItemDataPtr(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (void*)GetItemData(nIndex);
	}

	int SetItemDataPtr(int nIndex, void* pData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItemData(nIndex, (DWORD_PTR)pData);
	}

	int GetLBText(int nIndex, LPTSTR lpszText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText);
	}

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetLBTextBSTR(int nIndex, BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);

		int nLen = GetLBTextLen(nIndex);
		if(nLen == CB_ERR)
			return FALSE;

		LPTSTR lpszText = (LPTSTR)_alloca((nLen + 1) * sizeof(TCHAR));

		if(GetLBText(nIndex, lpszText) == CB_ERR)
			return FALSE;

		bstrText = ::SysAllocString(T2OLE(lpszText));
		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	int GetLBText(int nIndex, CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		int cchLen = GetLBTextLen(nIndex);
		if(cchLen == CB_ERR)
			return CB_ERR;
		int nRet = CB_ERR;
		LPTSTR lpstr = strText.GetBufferSetLength(cchLen);
		if(lpstr != NULL)
		{
			nRet = GetLBText(nIndex, lpstr);
			strText.ReleaseBuffer();
		}
		return nRet;
	}
#endif // defined(__STR_H__)

	int GetLBTextLen(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0L);
	}

	int GetItemHeight(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L);
	}

	int SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	BOOL GetExtendedUI() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
	}

	int SetExtendedUI(BOOL bExtended = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, bExtended, 0L);
	}

	void GetDroppedControlRect(LPRECT lprect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);
	}

	BOOL GetDroppedState() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
	}

#if (_WIN32_WINNT >= 0x0501)
	int GetMinVisible() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_GETMINVISIBLE, 0, 0L);
	}

	BOOL SetMinVisible(int nMinVisible)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CB_SETMINVISIBLE, nMinVisible, 0L);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	int InitStorage(int nItems, UINT nBytes)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, nBytes);
	}

	void ResetContent()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0L);
	}

	// for edit control
	BOOL LimitText(int nMaxChars)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CB_LIMITTEXT, nMaxChars, 0L);
	}

	// for drop-down combo boxes
	void ShowDropDown(BOOL bShowIt = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, CB_SHOWDROPDOWN, bShowIt, 0L);
	}

	// manipulating listbox items
	int AddString(LPCTSTR lpszString)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString);
	}

	int DeleteString(UINT nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0L);
	}

	int InsertString(int nIndex, LPCTSTR lpszString)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString);
	}

#ifndef _WIN32_WCE
	int Dir(UINT attr, LPCTSTR lpszWildCard)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_DIR, attr, (LPARAM)lpszWildCard);
	}
#endif // !_WIN32_WCE

	// selection helpers
	int FindString(int nStartAfter, LPCTSTR lpszString) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString);
	}

	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
	}

	int SelectString(int nStartAfter, LPCTSTR lpszString)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString);
	}

	// Clipboard operations
	void Clear()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
	}

	void Copy()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_COPY, 0, 0L);
	}

	void Cut()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_CUT, 0, 0L);
	}

	void Paste()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CEdit - client side for a Windows EDIT control

class CEdit : public TBase
{
public:
// Constructors
	CEdit(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CEdit& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("EDIT");
	}

	BOOL CanUndo() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
	}

	int GetLineCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
	}

	BOOL GetModify() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
	}

	void SetModify(BOOL bModified = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
	}

	void GetRect(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
	}

	DWORD GetSel() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_GETSEL, 0, 0L);
	}

	void GetSel(int& nStartChar, int& nEndChar) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nStartChar, (LPARAM)&nEndChar);
	}

#ifndef _WIN32_WCE
	HLOCAL GetHandle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0L);
	}

	void SetHandle(HLOCAL hBuffer)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0L);
	}
#endif // !_WIN32_WCE

	DWORD GetMargins() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0L);
	}

	void SetMargins(UINT nLeft, UINT nRight)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
	}

	UINT GetLimitText() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
	}

	void SetLimitText(UINT nMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0L);
	}

	POINT PosFromChar(UINT nChar) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, EM_POSFROMCHAR, nChar, 0);
		POINT point = { (short)LOWORD(dwRet), (short)HIWORD(dwRet) };
		return point;
	}

	int CharFromPos(POINT pt, int* pLine = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
		if(pLine != NULL)
			*pLine = (int)(short)HIWORD(dwRet);
		return (int)(short)LOWORD(dwRet);
	}

	// NOTE: first word in lpszBuffer must contain the size of the buffer!
	int GetLine(int nIndex, LPTSTR lpszBuffer) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		ASSERT(::IsWindow(m_hWnd));
		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	TCHAR GetPasswordChar() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (TCHAR)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
	}

	void SetPasswordChar(TCHAR ch)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0L);
	}

#ifndef _WIN32_WCE
	EDITWORDBREAKPROC GetWordBreakProc() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (EDITWORDBREAKPROC)::SendMessage(m_hWnd, EM_GETWORDBREAKPROC, 0, 0L);
	}

	void SetWordBreakProc(EDITWORDBREAKPROC ewbprc)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETWORDBREAKPROC, 0, (LPARAM)ewbprc);
	}
#endif // !_WIN32_WCE

	int GetFirstVisibleLine() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
	}

#ifndef _WIN32_WCE
	int GetThumb() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & ES_MULTILINE) != 0);
		return (int)::SendMessage(m_hWnd, EM_GETTHUMB, 0, 0L);
	}
#endif // !_WIN32_WCE

	BOOL SetReadOnly(BOOL bReadOnly = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
	}

#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
	UINT GetImeStatus(UINT uStatus) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, EM_GETIMESTATUS, uStatus, 0L);
	}

	UINT SetImeStatus(UINT uStatus, UINT uData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, EM_SETIMESTATUS, uStatus, uData);
	}
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501)
	BOOL GetCueBannerText(LPCWSTR lpstrText, int cchText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_GETCUEBANNER, (WPARAM)lpstrText, cchText);
	}

	BOOL SetCueBannerText(LPCWSTR lpstrText)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETCUEBANNER, 0, (LPARAM)(lpstrText));
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	void EmptyUndoBuffer()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
	}

	BOOL FmtLines(BOOL bAddEOL)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0L);
	}

	void LimitText(int nChars = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0L);
	}

	int LineFromChar(int nIndex = -1) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0L);
	}

	int LineIndex(int nLine = -1) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
	}

	int LineLength(int nLine = -1) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
	}

	void LineScroll(int nLines, int nChars = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
	}

	void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
	}

	void SetRect(LPCRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
	}

	void SetRectNP(LPCRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
	}

	void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
		if(!bNoScroll)
			::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}

	void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
		if(!bNoScroll)
			::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}

	void SetSelAll(BOOL bNoScroll = FALSE)
	{
		SetSel(0, -1, bNoScroll);
	}

	void SetSelNone(BOOL bNoScroll = FALSE)
	{
		SetSel(-1, 0, bNoScroll);
	}

	BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	BOOL SetTabStops()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
	}

	BOOL SetTabStops(const int& cxEachStop)    // takes an 'int'
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	void ScrollCaret()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}

	int Scroll(int nScrollAction)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & ES_MULTILINE) != 0);
		LRESULT lRet = ::SendMessage(m_hWnd, EM_SCROLL, nScrollAction, 0L);
		if(!(BOOL)HIWORD(lRet))
			return -1;   // failed
		return (int)(short)LOWORD(lRet);
		
	}

	void InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE)
	{
		SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
		ReplaceSel(lpstrText, bCanUndo);
	}

	void AppendText(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE)
	{
		InsertText(GetWindowTextLength(), lpstrText, bNoScroll, bCanUndo);
	}

#if (_WIN32_WINNT >= 0x0501)
	BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SHOWBALLOONTIP, 0, (LPARAM)pEditBaloonTip);
	}

	BOOL HideBalloonTip()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_HIDEBALLOONTIP, 0, 0L);
	}
#endif // (_WIN32_WINNT >= 0x0501)

	// Clipboard operations
	BOOL Undo()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0L);
	}

	void Clear()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
	}

	void Copy()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_COPY, 0, 0L);
	}

	void Cut()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_CUT, 0, 0L);
	}

	void Paste()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CScrollBar - client side for a Windows SCROLLBAR control

class CScrollBar : public TBase
{
public:
// Constructors
	CScrollBar(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CScrollBar& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		return m_hWnd;
	}

// Attributes
	LPCTSTR GetClassName()
	{
		return _T("SCROLLBAR");
	}

#ifndef _WIN32_WCE
	int GetScrollPos() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetScrollPos(m_hWnd, SB_CTL);
	}
#endif // !_WIN32_WCE

	int SetScrollPos(int nPos, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw);
	}

#ifndef _WIN32_WCE
	void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
	}
#endif // !_WIN32_WCE

	void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw);
	}

	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::GetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo);
	}

	int SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::SetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo, bRedraw);
	}

#ifndef _WIN32_WCE
	int GetScrollLimit() const
	{
		int nMin = 0, nMax = 0;
		::GetScrollRange(m_hWnd, SB_CTL, &nMin, &nMax);
		SCROLLINFO info = { 0 };
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_PAGE;
		if(::GetScrollInfo(m_hWnd, SB_CTL, &info))
			nMax -= ((info.nPage - 1) > 0) ? (info.nPage - 1) : 0;

		return nMax;
	}

#if (WINVER >= 0x0500)
	BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
		return (BOOL)::SendMessage(m_hWnd, SBM_GETSCROLLBARINFO, 0, (LPARAM)pScrollBarInfo);
#else // !(_WIN32_WINNT >= 0x0501)
		return ::GetScrollBarInfo(m_hWnd, OBJID_CLIENT, pScrollBarInfo);
#endif // !(_WIN32_WINNT >= 0x0501)
	}
#endif // (WINVER >= 0x0500)

// Operations
	void ShowScrollBar(BOOL bShow = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::ShowScrollBar(m_hWnd, SB_CTL, bShow);
	}

	BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags);
	}
#endif // !_WIN32_WCE
};

}; // namespace TWL

#endif // __STDCTL_H__
