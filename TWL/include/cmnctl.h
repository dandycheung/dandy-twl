//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __CMNCTL_H__
#define __CMNCTL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <ole2.h>
#include <commctrl.h>

#include <richedit.h>
#include <richole.h>

#include "stdctl.h"

// protect members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef GetNextSibling
#undef GetPrevSibling
#endif //_INC_WINDOWSX

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CImageList
// CListViewCtrl
// CTreeViewCtrl
// CTreeItem
// CTreeViewCtrlEx
// CHeaderCtrl
// CToolBarCtrl
// CStatusBarCtrl
// CTabCtrl
// CToolInfo
// CToolTipCtrl
// CTrackBarCtrl
// CUpDownCtrl
// CProgressBarCtrl
// CHotKeyCtrl
// CAnimateCtrl
// CRichEditCtrl
// CDragListBox
// CReBarCtrl
// CComboBoxEx
// CDateTimePickerCtrl
// CMonthCalendarCtrl
// CIPAddressCtrl
// CPagerCtrl
// CLinkCtrl
//

namespace TWL
{

// --- Windows Common Controls ---

///////////////////////////////////////////////////////////////////////////////
// CImageList

class CImageList
{
public:
	HIMAGELIST m_hImageList;

// Constructor
	CImageList(HIMAGELIST hImageList = NULL) : m_hImageList(hImageList)
	{ }

// Operators, etc.
	CImageList& operator =(HIMAGELIST hImageList)
	{
		m_hImageList = hImageList;
		return *this;
	}

	operator HIMAGELIST() const { return m_hImageList; }

	void Attach(HIMAGELIST hImageList)
	{
		ASSERT(m_hImageList == NULL);
		ASSERT(hImageList != NULL);
		m_hImageList = hImageList;
	}

	HIMAGELIST Detach()
	{
		HIMAGELIST hImageList = m_hImageList;
		m_hImageList = NULL;
		return hImageList;
	}

	bool IsNull() const { return (m_hImageList == NULL); }

// Attributes
	int GetImageCount() const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetImageCount(m_hImageList);
	}

	COLORREF GetBkColor() const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetBkColor(m_hImageList);
	}

	COLORREF SetBkColor(COLORREF cr)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetBkColor(m_hImageList, cr);
	}

	BOOL GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
	}

	HICON GetIcon(int nIndex, UINT uFlags = ILD_NORMAL) const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetIcon(m_hImageList, nIndex, uFlags);
	}

	BOOL GetIconSize(int& cx, int& cy) const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetIconSize(m_hImageList, &cx, &cy);
	}

	BOOL GetIconSize(SIZE& size) const
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_GetIconSize(m_hImageList, (int*)&size.cx, (int*)&size.cy);
	}

	BOOL SetIconSize(int cx, int cy)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetIconSize(m_hImageList, cx, cy);
	}

	BOOL SetIconSize(SIZE size)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetIconSize(m_hImageList, size.cx, size.cy);
	}

	BOOL SetImageCount(UINT uNewCount)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetImageCount(m_hImageList, uNewCount);
	}

	BOOL SetOverlayImage(int nImage, int nOverlay)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetOverlayImage(m_hImageList, nImage, nOverlay);
	}

// Operations
	BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow)
	{
		ASSERT(m_hImageList == NULL);
		m_hImageList = ImageList_Create(cx, cy, nFlags, nInitial, nGrow);
		return (m_hImageList != NULL) ? TRUE : FALSE;
	}

	BOOL Create(CStringOrId bitmap, int cx, int nGrow, COLORREF crMask)
	{
		ASSERT(m_hImageList == NULL);
		m_hImageList = ImageList_LoadBitmap(GetResourceInstance(), bitmap, cx, nGrow, crMask);
		return (m_hImageList != NULL) ? TRUE : FALSE;
	}

	BOOL CreateFromImage(CStringOrId image, int cx, int nGrow, COLORREF crMask, UINT uType, UINT uFlags = LR_DEFAULTCOLOR | LR_DEFAULTSIZE)
	{
		ASSERT(m_hImageList == NULL);
		m_hImageList = ImageList_LoadImage(GetResourceInstance(), image, cx, nGrow, crMask, uType, uFlags);
		return (m_hImageList != NULL) ? TRUE : FALSE;
	}

	BOOL Merge(HIMAGELIST hImageList1, int nImage1, HIMAGELIST hImageList2, int nImage2, int dx, int dy)
	{
		ASSERT(m_hImageList == NULL);
		m_hImageList = ImageList_Merge(hImageList1, nImage1, hImageList2, nImage2, dx, dy);
		return (m_hImageList != NULL) ? TRUE : FALSE;
	}

#if (_WIN32_IE >= 0x0300)
#ifdef __IStream_INTERFACE_DEFINED__
#ifndef _WIN32_WCE
	BOOL CreateFromStream(LPSTREAM lpStream)
	{
		ASSERT(m_hImageList == NULL);
		m_hImageList = ImageList_Read(lpStream);
		return (m_hImageList != NULL) ? TRUE : FALSE;
	}
#endif // !_WIN32_WCE
#endif // __IStream_INTERFACE_DEFINED__
#endif // (_WIN32_IE >= 0x0300)

	BOOL Destroy()
	{
		if (m_hImageList == NULL)
			return FALSE;
		BOOL bRet = ImageList_Destroy(m_hImageList);
		if(bRet)
			m_hImageList = NULL;
		return bRet;
	}

	int Add(HBITMAP hBitmap, HBITMAP hBitmapMask = NULL)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_Add(m_hImageList, hBitmap, hBitmapMask);
	}

	int Add(HBITMAP hBitmap, COLORREF crMask)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_AddMasked(m_hImageList, hBitmap, crMask);
	}

	BOOL Remove(int nImage)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_Remove(m_hImageList, nImage);
	}

	BOOL RemoveAll()
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_RemoveAll(m_hImageList);
	}

	BOOL Replace(int nImage, HBITMAP hBitmap, HBITMAP hBitmapMask)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_Replace(m_hImageList, nImage, hBitmap, hBitmapMask);
	}

	int AddIcon(HICON hIcon)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_AddIcon(m_hImageList, hIcon);
	}

	int ReplaceIcon(int nImage, HICON hIcon)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
	}

	HICON ExtractIcon(int nImage)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_ExtractIcon(NULL, m_hImageList, nImage);
	}

	BOOL Draw(HDC hDC, int nImage, int x, int y, UINT nStyle)
	{
		ASSERT(m_hImageList != NULL);
		ASSERT(hDC != NULL);
		return ImageList_Draw(m_hImageList, nImage, hDC, x, y, nStyle);
	}

	BOOL Draw(HDC hDC, int nImage, POINT pt, UINT nStyle)
	{
		ASSERT(m_hImageList != NULL);
		ASSERT(hDC != NULL);
		return ImageList_Draw(m_hImageList, nImage, hDC, pt.x, pt.y, nStyle);
	}

	BOOL DrawEx(int nImage, HDC hDC, int x, int y, int dx, int dy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle)
	{
		ASSERT(m_hImageList != NULL);
		ASSERT(hDC != NULL);
		return ImageList_DrawEx(m_hImageList, nImage, hDC, x, y, dx, dy, rgbBk, rgbFg, fStyle);
	}

	BOOL DrawEx(int nImage, HDC hDC, RECT& rect, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle)
	{
		ASSERT(m_hImageList != NULL);
		ASSERT(hDC != NULL);
		return ImageList_DrawEx(m_hImageList, nImage, hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, rgbBk, rgbFg, fStyle);
	}

	static BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp)
	{
		return ImageList_DrawIndirect(pimldp);
	}

	BOOL Copy(int nSrc, int nDst, UINT uFlags = ILCF_MOVE)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_Copy(m_hImageList, nDst, m_hImageList, nSrc, uFlags);
	}

#if (_WIN32_IE >= 0x0300)
#ifdef __IStream_INTERFACE_DEFINED__
#ifndef _WIN32_WCE
	static HIMAGELIST Read(LPSTREAM lpStream)
	{
		return ImageList_Read(lpStream);
	}

	BOOL Write(LPSTREAM lpStream)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_Write(m_hImageList, lpStream);
	}
#endif // !_WIN32_WCE
#endif // __IStream_INTERFACE_DEFINED__
#endif // (_WIN32_IE >= 0x0300)

#if (_WIN32_WINNT >= 0x0501)
	static HRESULT ReadEx(DWORD dwFlags, LPSTREAM lpStream, REFIID riid, PVOID* ppv)
	{
		return ImageList_ReadEx(dwFlags, lpStream, riid, ppv);
	}

	HRESULT WriteEx(DWORD dwFlags, LPSTREAM lpStream)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_WriteEx(m_hImageList, dwFlags, lpStream);
	}
#endif // (_WIN32_WINNT >= 0x0501)

	// Drag operations
	BOOL BeginDrag(int nImage, POINT ptHotSpot)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
	}

	BOOL BeginDrag(int nImage, int xHotSpot, int yHotSpot)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_BeginDrag(m_hImageList, nImage, xHotSpot, yHotSpot);
	}

	static void EndDrag()
	{
		ImageList_EndDrag();
	}

	static BOOL DragMove(POINT pt)
	{
		return ImageList_DragMove(pt.x, pt.y);
	}

	static BOOL DragMove(int x, int y)
	{
		return ImageList_DragMove(x, y);
	}

	BOOL SetDragCursorImage(int nDrag, POINT ptHotSpot)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetDragCursorImage(m_hImageList, nDrag, ptHotSpot.x, ptHotSpot.y);
	}

	BOOL SetDragCursorImage(int nDrag, int xHotSpot, int yHotSpot)
	{
		ASSERT(m_hImageList != NULL);
		return ImageList_SetDragCursorImage(m_hImageList, nDrag, xHotSpot, yHotSpot);
	}

	static BOOL DragShowNolock(BOOL bShow = TRUE)
	{
		return ImageList_DragShowNolock(bShow);
	}

	static CImageList GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot)
	{
		return CImageList(ImageList_GetDragImage(lpPoint, lpPointHotSpot));
	}

	static BOOL DragEnter(HWND hWnd, POINT point)
	{
		return ImageList_DragEnter(hWnd, point.x, point.y);
	}

	static BOOL DragEnter(HWND hWnd, int x, int y)
	{
		return ImageList_DragEnter(hWnd, x, y);
	}

	static BOOL DragLeave(HWND hWnd)
	{
		return ImageList_DragLeave(hWnd);
	}

#if (_WIN32_IE >= 0x0400)
	CImageList Duplicate() const
	{
		ASSERT(m_hImageList != NULL);
		return CImageList(ImageList_Duplicate(m_hImageList));
	}

	static CImageList Duplicate(HIMAGELIST hImageList)
	{
		ASSERT(hImageList != NULL);
		return CImageList(ImageList_Duplicate(hImageList));
	}
#endif // (_WIN32_IE >= 0x0400)
};


///////////////////////////////////////////////////////////////////////////////
// CToolTipCtrl

#ifndef _WIN32_WCE

class CToolInfo : public TOOLINFO
{
public:
	CToolInfo(UINT nFlags, HWND hWnd, UINT nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL)
	{
		Init(nFlags, hWnd, nIDTool, lpRect, lpstrText, lUserParam);
	}

	operator LPTOOLINFO() { return this; }

	operator LPARAM() { return (LPARAM)this; }

	void Init(UINT nFlags, HWND hWnd, UINT nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL)
	{
		ASSERT(::IsWindow(hWnd));
		memset(this, 0, sizeof(TOOLINFO));
		cbSize = sizeof(TOOLINFO);
		uFlags = nFlags;
		if(nIDTool == 0)
		{
			hwnd = ::GetParent(hWnd);
			uFlags |= TTF_IDISHWND;
			uId = (UINT_PTR)hWnd;
		}
		else
		{
			hwnd = hWnd;
			uId = nIDTool;
		}
		if(lpRect != NULL)
			rect = *lpRect;
		hinst = GetResourceInstance();
		lpszText = lpstrText;
		lParam = lUserParam;
	}
};

class CToolTipCtrl : public TBase
{
public:
// Constructors
	CToolTipCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CToolTipCtrl& operator =(HWND hWnd)
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
		return TOOLTIPS_CLASS;
	}

	void GetText(LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&lpToolInfo);
	}

	void GetText(LPTSTR lpstrText, HWND hWnd, UINT nIDTool = 0) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);
		CToolInfo ti(0, hWnd, nIDTool, NULL, lpstrText);
		::SendMessage(m_hWnd, TTM_GETTEXT, 0, ti);
	}

	BOOL GetToolInfo(LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)lpToolInfo);
	}

	BOOL GetToolInfo(HWND hWnd, UINT nIDTool, UINT* puFlags, LPRECT lpRect, LPTSTR lpstrText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);
		CToolInfo ti(0, hWnd, nIDTool, NULL, lpstrText);
		BOOL bRet = (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, ti);
		if(bRet)
		{
			*puFlags = ti.uFlags;
			memcpy(lpRect, &(ti.rect), sizeof(RECT));
		}
		return bRet;
	}

	void SetToolInfo(LPTOOLINFO lpToolInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETTOOLINFO, 0, (LPARAM)lpToolInfo);
	}

	void SetToolRect(LPTOOLINFO lpToolInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)lpToolInfo);
	}

	void SetToolRect(HWND hWnd, UINT nIDTool, LPCRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);
		ASSERT(nIDTool != 0);

		CToolInfo ti(0, hWnd, nIDTool, (LPRECT)lpRect, NULL);
		::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, ti);
	}

	int GetToolCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
	}

	int GetDelayTime(DWORD dwType) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TTM_GETDELAYTIME, dwType, 0L);
	}

	void SetDelayTime(DWORD dwType, int nTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETDELAYTIME, dwType, MAKELPARAM(nTime, 0));
	}

	void GetMargin(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_GETMARGIN, 0, (LPARAM)lpRect);
	}

	void SetMargin(LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETMARGIN, 0, (LPARAM)lpRect);
	}

	int GetMaxTipWidth() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TTM_GETMAXTIPWIDTH, 0, 0L);
	}

	int SetMaxTipWidth(int nWidth)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, nWidth);
	}

	COLORREF GetTipBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TTM_GETTIPBKCOLOR, 0, 0L);
	}

	void SetTipBkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETTIPBKCOLOR, (WPARAM)clr, 0L);
	}

	COLORREF GetTipTextColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TTM_GETTIPTEXTCOLOR, 0, 0L);
	}

	void SetTipTextColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETTIPTEXTCOLOR, (WPARAM)clr, 0L);
	}

	BOOL GetCurrentTool(LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_GETCURRENTTOOL, 0, (LPARAM)lpToolInfo);
	}

#if (_WIN32_IE >= 0x0500)
	SIZE GetBubbleSize(LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, TTM_GETBUBBLESIZE, 0, (LPARAM)lpToolInfo);
		SIZE size = { (short)LOWORD(dwRet), (short)HIWORD(dwRet) };
		return size;
	}

	BOOL SetTitle(UINT uIcon, LPCTSTR lpstrTitle)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_SETTITLE, uIcon, (LPARAM)lpstrTitle);
	}
#endif // (_WIN32_IE >= 0x0500)

#if (_WIN32_WINNT >= 0x0501)
	void GetTitle(PTTGETTITLE pTTGetTitle) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_GETTITLE, 0, (LPARAM)pTTGetTitle);
	}

	void SetWindowTheme(LPCWSTR lpstrTheme)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	void Activate(BOOL bActivate)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_ACTIVATE, bActivate, 0L);
	}

	BOOL AddTool(LPTOOLINFO lpToolInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)lpToolInfo);
	}

	BOOL AddTool(HWND hWnd, CStringOrId text = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT nIDTool = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);
		// the toolrect and toolid must both be zero or both valid
		ASSERT((lpRectTool != NULL && nIDTool != 0) || (lpRectTool == NULL && nIDTool == 0));

		CToolInfo ti(0, hWnd, nIDTool, (LPRECT)lpRectTool, (LPTSTR)(LPCTSTR)text);
		return (BOOL)::SendMessage(m_hWnd, TTM_ADDTOOL, 0, ti);
	}

	void DelTool(LPTOOLINFO lpToolInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)lpToolInfo);
	}

	void DelTool(HWND hWnd, UINT nIDTool = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);

		CToolInfo ti(0, hWnd, nIDTool, NULL, NULL);
		::SendMessage(m_hWnd, TTM_DELTOOL, 0, ti);
	}

	BOOL HitTest(LPTTHITTESTINFO lpHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM)lpHitTestInfo);
	}

	BOOL HitTest(HWND hWnd, POINT pt, LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);
		ASSERT(lpToolInfo != NULL);

		TTHITTESTINFO hti = { 0 };
		hti.ti.cbSize = sizeof(TOOLINFO);
		hti.hwnd = hWnd;
		hti.pt.x = pt.x;
		hti.pt.y = pt.y;
		if((BOOL)::SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM)&hti))
		{
			memcpy(lpToolInfo, &hti.ti, sizeof(TOOLINFO));
			return TRUE;
		}
		return FALSE;
	}

	void RelayEvent(LPMSG lpMsg)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)lpMsg);
	}

	void UpdateTipText(LPTOOLINFO lpToolInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)lpToolInfo);
	}

	void UpdateTipText(CStringOrId text, HWND hWnd, UINT nIDTool = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hWnd != NULL);

		CToolInfo ti(0, hWnd, nIDTool, NULL, (LPTSTR)(LPCTSTR)text);
		::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, ti);
	}

	BOOL EnumTools(UINT nTool, LPTOOLINFO lpToolInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_ENUMTOOLS, nTool, (LPARAM)lpToolInfo);
	}

	void Pop()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_POP, 0, 0L);
	}

	void TrackActivate(LPTOOLINFO lpToolInfo, BOOL bActivate)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_TRACKACTIVATE, bActivate, (LPARAM)lpToolInfo);
	}

	void TrackPosition(int xPos, int yPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_TRACKPOSITION, 0, MAKELPARAM(xPos, yPos));
	}

#if (_WIN32_IE >= 0x0400)
	void Update()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_UPDATE, 0, 0L);
	}
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500)
	BOOL AdjustRect(LPRECT lpRect, BOOL bLarger /*= TRUE*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TTM_ADJUSTRECT, bLarger, (LPARAM)lpRect);
	}
#endif // (_WIN32_IE >= 0x0500)

#if (_WIN32_WINNT >= 0x0501)
	void Popup()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_POPUP, 0, 0L);
	}
#endif // (_WIN32_WINNT >= 0x0501)
};

#endif // !_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CHeaderCtrl

class CHeaderCtrl : public TBase
{
public:
// Constructors
	CHeaderCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CHeaderCtrl& operator =(HWND hWnd)
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
		return WC_HEADER;
	}

	int GetItemCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);
	}

	BOOL GetItem(int nIndex, LPHDITEM pHeaderItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_GETITEM, nIndex, (LPARAM)pHeaderItem);
	}

	BOOL SetItem(int nIndex, LPHDITEM pHeaderItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_SETITEM, nIndex, (LPARAM)pHeaderItem);
	}

	CImageList GetImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, HDM_GETIMAGELIST, 0, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, HDM_SETIMAGELIST, 0, (LPARAM)hImageList));
	}

	BOOL GetOrderArray(int nSize, int* lpnArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_GETORDERARRAY, nSize, (LPARAM)lpnArray);
	}

	BOOL SetOrderArray(int nSize, int* lpnArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_SETORDERARRAY, nSize, (LPARAM)lpnArray);
	}

	BOOL GetItemRect(int nIndex, LPRECT lpItemRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM)lpItemRect);
	}

	int SetHotDivider(BOOL bPos, DWORD dwInputValue)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_SETHOTDIVIDER, bPos, dwInputValue);
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	int GetBitmapMargin() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_GETBITMAPMARGIN, 0, 0L);
	}

	int SetBitmapMargin(int nWidth)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_SETBITMAPMARGIN, nWidth, 0L);
	}

	int SetFilterChangeTimeout(DWORD dwTimeOut)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_SETFILTERCHANGETIMEOUT, 0, dwTimeOut);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

// Operations
	int InsertItem(int nIndex, LPHDITEM phdi)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_INSERTITEM, nIndex, (LPARAM)phdi);
	}

	BOOL DeleteItem(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_DELETEITEM, nIndex, 0L);
	}

	BOOL Layout(HD_LAYOUT* pHeaderLayout)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, HDM_LAYOUT, 0, (LPARAM)pHeaderLayout);
	}

	int HitTest(LPHDHITTESTINFO lpHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM)lpHitTestInfo);
	}

	int OrderToIndex(int nOrder)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_ORDERTOINDEX, nOrder, 0L);
	}

	CImageList CreateDragImage(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, HDM_CREATEDRAGIMAGE, nIndex, 0L));
	}

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	int EditFilter(int nColumn, BOOL bDiscardChanges)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_EDITFILTER, nColumn, MAKELPARAM(bDiscardChanges, 0));
	}

	int ClearFilter(int nColumn)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_CLEARFILTER, nColumn, 0L);
	}

	int ClearAllFilters()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, HDM_CLEARFILTER, (WPARAM)-1, 0L);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
};


///////////////////////////////////////////////////////////////////////////////
// CListViewCtrl

class CListViewCtrl : public TBase
{
public:
// Constructors
	CListViewCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CListViewCtrl& operator =(HWND hWnd)
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
		return WC_LISTVIEW;
	}

	COLORREF GetBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0L);
	}

	BOOL SetBkColor(COLORREF cr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, cr);
	}

	CImageList GetImageList(int nImageListType) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageListType, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList, int nImageList)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM)hImageList));
	}

	int GetItemCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETITEMCOUNT, 0, 0L);
	}

	BOOL SetItemCount(int nItems)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, 0L);
	}

	BOOL GetItem(LPLVITEM pItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem);
	}

	BOOL SetItem(const LVITEM* pItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)pItem);
	}

	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvi = { 0 };
		lvi.mask = nMask;
		lvi.iItem = nItem;
		lvi.iSubItem = nSubItem;
		lvi.stateMask = nStateMask;
		lvi.state = nState;
		lvi.pszText = (LPTSTR) lpszItem;
		lvi.iImage = nImage;
		lvi.lParam = lParam;
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
	}

	UINT GetItemState(int nItem, UINT nMask) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, LVM_GETITEMSTATE, nItem, nMask);
	}

	BOOL SetItemState(int nItem, UINT nState, UINT nStateMask)
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvi = { 0 };
		lvi.state = nState;
		lvi.stateMask = nStateMask;
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
	}

	BOOL SetItemState(int nItem, LPLVITEM pItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)pItem);
	}

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetItemText(int nItem, int nSubItem, BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);
		LVITEM lvi = { 0 };
		lvi.iSubItem = nSubItem;

		LPTSTR lpstrText = NULL;
		int nRes = 0;
		for(int nLen = 256; ; nLen *= 2)
		{
			lpstrText = new TCHAR[nLen];
			if(lpstrText == NULL)
				break;
			lpstrText[0] = NULL;
			lvi.cchTextMax = nLen;
			lvi.pszText = lpstrText;
			nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
			if(nRes < nLen - 1)
				break;
			delete [] lpstrText;
			lpstrText = NULL;
		}

		if(lpstrText != NULL)
		{
			if(nRes != 0)
				bstrText = ::SysAllocString(T2OLE(lpstrText));
			delete [] lpstrText;
		}

		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	int GetItemText(int nItem, int nSubItem, CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvi = { 0 };
		lvi.iSubItem = nSubItem;

		strText.Empty();
		int nRes = 0;
		for(int nLen = 256; ; nLen *= 2)
		{
			lvi.cchTextMax = nLen;
			lvi.pszText = strText.GetBufferSetLength(nLen);
			if(lvi.pszText == NULL)
			{
				nRes = 0;
				break;
			}
			nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
			if(nRes < nLen - 1)
				break;
		}
		strText.ReleaseBuffer();
		return nRes;
	}
#endif // defined(__STR_H__)

	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvi = { 0 };
		lvi.iSubItem = nSubItem;
		lvi.cchTextMax = nLen;
		lvi.pszText = lpszText;
		return (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
	}

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(nItem, nSubItem, LVIF_TEXT, lpszText, 0, 0, 0, 0);
	}

	DWORD_PTR GetItemData(int nItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvi = { 0 };
		lvi.iItem = nItem;
		lvi.mask = LVIF_PARAM;
		BOOL bRet = (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvi);
		return (DWORD_PTR)(bRet ? lvi.lParam : NULL);
	}

	BOOL SetItemData(int nItem, DWORD_PTR dwData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(nItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)dwData);
	}

	UINT GetCallbackMask() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, LVM_GETCALLBACKMASK, 0, 0L);
	}

	BOOL SetCallbackMask(UINT nMask)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETCALLBACKMASK, nMask, 0L);
	}

	BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMPOSITION, nItem, (LPARAM)lpPoint);
	}

	BOOL SetItemPosition(int nItem, POINT pt)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(((GetStyle() & LVS_TYPEMASK) == LVS_ICON) || ((GetStyle() & LVS_TYPEMASK) == LVS_SMALLICON));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
	}

	BOOL SetItemPosition(int nItem, int x, int y)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(((GetStyle() & LVS_TYPEMASK) == LVS_ICON) || ((GetStyle() & LVS_TYPEMASK) == LVS_SMALLICON));
		POINT pt = { x, y };
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
	}

	int GetStringWidth(LPCTSTR lpsz) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)lpsz);
	}

#if 0
	HWND GetEditControl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L);
	}
#else
	CEdit GetEditControl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CEdit((HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L));
	}
#endif

	BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMN, nCol, (LPARAM)pColumn);
	}

	BOOL SetColumn(int nCol, const LVCOLUMN* pColumn)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMN, nCol, (LPARAM)pColumn);
	}

	int GetColumnWidth(int nCol) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETCOLUMNWIDTH, nCol, 0L);
	}

	BOOL SetColumnWidth(int nCol, int cx)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMNWIDTH, nCol, MAKELPARAM(cx, 0));
	}

	BOOL GetViewRect(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETVIEWRECT, 0, (LPARAM)lpRect);
	}

	COLORREF GetTextColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETTEXTCOLOR, 0, 0L);
	}

	BOOL SetTextColor(COLORREF cr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETTEXTCOLOR, 0, cr);
	}

	COLORREF GetTextBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETTEXTBKCOLOR, 0, 0L);
	}

	BOOL SetTextBkColor(COLORREF cr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETTEXTBKCOLOR, 0, cr);
	}

	int GetTopIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETTOPINDEX, 0, 0L);
	}

	int GetCountPerPage() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETCOUNTPERPAGE, 0, 0L);
	}

	BOOL GetOrigin(LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETORIGIN, 0, (LPARAM)lpPoint);
	}

	UINT GetSelectedCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0L);
	}

	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
	{
		ASSERT(::IsWindow(m_hWnd));
		lpRect->left = nCode;
		return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMRECT, (WPARAM)nItem, (LPARAM)lpRect);
	}

#ifndef _WIN32_WCE
	HCURSOR GetHotCursor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HCURSOR)::SendMessage(m_hWnd, LVM_GETHOTCURSOR, 0, 0L);
	}

	HCURSOR SetHotCursor(HCURSOR hHotCursor)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HCURSOR)::SendMessage(m_hWnd, LVM_SETHOTCURSOR, 0, (LPARAM)hHotCursor);
	}

	int GetHotItem() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETHOTITEM, 0, 0L);
	}

	int SetHotItem(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SETHOTITEM, nIndex, 0L);
	}
#endif // !_WIN32_WCE

	BOOL GetColumnOrderArray(int nCount, int* lpnArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMNORDERARRAY, nCount, (LPARAM)lpnArray);
	}

	BOOL SetColumnOrderArray(int nCount, int* lpnArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMNORDERARRAY, nCount, (LPARAM)lpnArray);
	}

	CHeaderCtrl GetHeader() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CHeaderCtrl((HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0L));
	}

	BOOL GetSubItemRect(int nItem, int nSubItem, int nFlag, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LVS_TYPEMASK) == LVS_REPORT);
		ASSERT(lpRect != NULL);
		lpRect->top = nSubItem;
		lpRect->left = nFlag;
		return (BOOL)::SendMessage(m_hWnd, LVM_GETSUBITEMRECT, nItem, (LPARAM)lpRect);
	}

	DWORD SetIconSpacing(int cx, int cy)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LVS_TYPEMASK) == LVS_ICON);
		return (DWORD)::SendMessage(m_hWnd, LVM_SETICONSPACING, 0, MAKELPARAM(cx, cy));
	}

	int GetISearchString(LPTSTR lpstr) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETISEARCHSTRING, 0, (LPARAM)lpstr);
	}

	void GetItemSpacing(SIZE& sizeSpacing, BOOL bSmallIconView = FALSE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, LVM_GETITEMSPACING, bSmallIconView, 0L);
		sizeSpacing.cx = (short)LOWORD(dwRet);
		sizeSpacing.cy = (short)HIWORD(dwRet);
	}

#ifdef _WIN32_WCE
	void SetItemSpacing(INT cySpacing)
	{
		ASSERT(::IsWindow(m_hWnd));
		ListView_SetItemSpacing(m_hWnd, cySpacing);
	}
#endif //_WIN32_WCE

	// single-selection only
	int GetSelectedIndex() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LVS_SINGLESEL) != 0);
		return (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));
	}

	BOOL GetSelectedItem(LPLVITEM pItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LVS_SINGLESEL) != 0);
		ASSERT(pItem != NULL);
		pItem->iItem = (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));
		if(pItem->iItem == -1)
			return FALSE;
		return (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem);
	}

	// extended list view styles
	DWORD GetExtendedListViewStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0L);
	}

	// dwExMask = 0 means all styles
	DWORD SetExtendedListViewStyle(DWORD dwExStyle, DWORD dwExMask = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, dwExMask, dwExStyle);
	}

	// checkboxes only
	BOOL GetCheckState(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetExtendedListViewStyle() & LVS_EX_CHECKBOXES) != 0);
		UINT uRet = GetItemState(nIndex, LVIS_STATEIMAGEMASK);
		return (uRet >> 12) - 1;
	}

	BOOL SetCheckState(int nItem, BOOL bCheck)
	{
		int nCheck = bCheck ? 2 : 1;   // one based index
		return SetItemState(nItem, INDEXTOSTATEIMAGEMASK(nCheck), LVIS_STATEIMAGEMASK);
	}

	// view type
	DWORD GetViewType() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (GetStyle() & LVS_TYPEMASK);
	}

	DWORD SetViewType(DWORD dwType)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(dwType == LVS_ICON || dwType == LVS_SMALLICON || dwType == LVS_LIST || dwType == LVS_REPORT);
		DWORD dwOldType = GetViewType();
		if(dwType != dwOldType)
			ModifyStyle(LVS_TYPEMASK, (dwType & LVS_TYPEMASK));
		return dwOldType;
	}

#if (_WIN32_IE >= 0x0400)
#ifndef _WIN32_WCE
	BOOL GetBkImage(LPLVBKIMAGE plvbki) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETBKIMAGE, 0, (LPARAM)plvbki);
	}

	BOOL SetBkImage(LPLVBKIMAGE plvbki)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETBKIMAGE, 0, (LPARAM)plvbki);
	}
#endif // !_WIN32_WCE

	int GetSelectionMark() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETSELECTIONMARK, 0, 0L);
	}

	int SetSelectionMark(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SETSELECTIONMARK, 0, nIndex);
	}

#ifndef _WIN32_WCE
	BOOL GetWorkAreas(int nWorkAreas, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETWORKAREAS, nWorkAreas, (LPARAM)lpRect);
	}

	BOOL SetWorkAreas(int nWorkAreas, LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETWORKAREAS, nWorkAreas, (LPARAM)lpRect);
	}

	DWORD GetHoverTime() const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetExtendedListViewStyle() & (LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE)) != 0);
		return (DWORD)::SendMessage(m_hWnd, LVM_GETHOVERTIME, 0, 0L);
	}

	DWORD SetHoverTime(DWORD dwHoverTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetExtendedListViewStyle() & (LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE)) != 0);
		return (DWORD)::SendMessage(m_hWnd, LVM_SETHOVERTIME, 0, dwHoverTime);
	}

	BOOL GetNumberOfWorkAreas(int* pnWorkAreas) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETNUMBEROFWORKAREAS, 0, (LPARAM)pnWorkAreas);
	}
#endif // !_WIN32_WCE

	BOOL SetItemCountEx(int nItems, DWORD dwFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(((GetStyle() & LVS_OWNERDATA) != 0) && (((GetStyle() & LVS_TYPEMASK) == LVS_REPORT) || ((GetStyle() & LVS_TYPEMASK) == LVS_LIST)));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, dwFlags);
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetToolTips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, LVM_GETTOOLTIPS, 0, 0L));
	}

	CToolTipCtrl SetToolTips(HWND hWndTT)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, (WPARAM)hWndTT, 0L));
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_WINNT >= 0x0501)
	int GetSelectedColumn() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETSELECTEDCOLUMN, 0, 0L);
	}

	void SetSelectedColumn(int nColumn)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_SETSELECTEDCOLUMN, nColumn, 0L);
	}

	DWORD GetView() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, LVM_GETVIEW, 0, 0L);
	}

	int SetView(DWORD dwView)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SETVIEW, dwView, 0L);
	}

	BOOL IsGroupViewEnabled() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_ISGROUPVIEWENABLED, 0, 0L);
	}

	int GetGroupInfo(int nGroupID, PLVGROUP pGroup) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETGROUPINFO, nGroupID, (LPARAM)pGroup);
	}

	int SetGroupInfo(int nGroupID, PLVGROUP pGroup)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SETGROUPINFO, nGroupID, (LPARAM)pGroup);
	}

	void GetGroupMetrics(PLVGROUPMETRICS pGroupMetrics) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_GETGROUPMETRICS, 0, (LPARAM)pGroupMetrics);
	}

	void SetGroupMetrics(PLVGROUPMETRICS pGroupMetrics)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_SETGROUPMETRICS, 0, (LPARAM)pGroupMetrics);
	}

	void GetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_GETTILEVIEWINFO, 0, (LPARAM)pTileViewInfo);
	}

	BOOL SetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETTILEVIEWINFO, 0, (LPARAM)pTileViewInfo);
	}

	void GetTileInfo(PLVTILEINFO pTileInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_GETTILEINFO, 0, (LPARAM)pTileInfo);
	}

	BOOL SetTileInfo(PLVTILEINFO pTileInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETTILEINFO, 0, (LPARAM)pTileInfo);
	}

	BOOL GetInsertMark(LPLVINSERTMARK pInsertMark) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_GETINSERTMARK, 0, (LPARAM)pInsertMark);
	}

	BOOL SetInsertMark(LPLVINSERTMARK pInsertMark)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETINSERTMARK, 0, (LPARAM)pInsertMark);
	}

	int GetInsertMarkRect(LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETINSERTMARKRECT, 0, (LPARAM)lpRect);
	}

	COLORREF GetInsertMarkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETINSERTMARKCOLOR, 0, 0L);
	}

	COLORREF SetInsertMarkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_SETINSERTMARKCOLOR, 0, clr);
	}

	COLORREF GetOutlineColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_GETOUTLINECOLOR, 0, 0L);
	}

	COLORREF SetOutlineColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, LVM_SETOUTLINECOLOR, 0, clr);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_INSERTCOLUMN, nCol, (LPARAM)pColumn);
	}

	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, 
		int nWidth = -1, int nSubItem = -1, int iImage = -1, int iOrder = -1)
	{
		LVCOLUMN column = { 0 };
		column.mask = LVCF_TEXT|LVCF_FMT;
		column.pszText = (LPTSTR)lpszColumnHeading;
		column.fmt = nFormat;
		if (nWidth != -1)
		{
			column.mask |= LVCF_WIDTH;
			column.cx = nWidth;
		}
		if (nSubItem != -1)
		{
			column.mask |= LVCF_SUBITEM;
			column.iSubItem = nSubItem;
		}
		if (iImage != -1)
		{
			column.mask |= LVCF_IMAGE;
			column.iImage = iImage;
		}
		if (iOrder != -1)
		{
			column.mask |= LVCF_ORDER;
			column.iOrder = iOrder;
		}
		return InsertColumn(nCol, &column);
	}

	BOOL DeleteColumn(int nCol)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_DELETECOLUMN, nCol, 0L);
	}

	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM item = { 0 };
		item.mask = nMask;
		item.iItem = nItem;
		item.iSubItem = 0;
		item.pszText = (LPTSTR)lpszItem;
		item.state = nState;
		item.stateMask = nStateMask;
		item.iImage = nImage;
		item.lParam = lParam;
		return InsertItem(&item);
	}

	int InsertItem(const LVITEM* pItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)pItem);
	}

	int InsertItem(int nItem, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0);
	}

	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0);
	}

	int GetNextItem(int nItem, int nFlags) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, nItem, MAKELPARAM(nFlags, 0));
	}

	BOOL DeleteItem(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_DELETEITEM, nItem, 0L);
	}

	BOOL DeleteAllItems()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_DELETEALLITEMS, 0, 0L);
	}

	int FindItem(LVFINDINFO* pFindInfo, int nStart) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_FINDITEM, nStart, (LPARAM)pFindInfo);
	}

	int HitTest(LVHITTESTINFO* pHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)pHitTestInfo);
	}

	int HitTest(POINT pt, UINT* pFlags) const
	{
		ASSERT(::IsWindow(m_hWnd));
		LVHITTESTINFO hti = { 0 };
		hti.pt = pt;
		int nRes = (int)::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&hti);
		if (pFlags != NULL)
			*pFlags = hti.flags;
		return nRes;
	}

	BOOL EnsureVisible(int nItem, BOOL bPartialOK)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0));
	}

	BOOL Scroll(SIZE size)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SCROLL, size.cx, size.cy);
	}

	BOOL RedrawItems(int nFirst, int nLast)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_REDRAWITEMS, nFirst, nLast);
	}

	BOOL Arrange(UINT nCode)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_ARRANGE, nCode, 0L);
	}

#if 0
	HWND EditLabel(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L);
	}
#else
	CEdit EditLabel(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CEdit((HWND)::SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L));
	}
#endif

	BOOL Update(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_UPDATE, nItem, 0L);
	}

	BOOL SortItems(PFNLVCOMPARE pfnCompare, LPARAM lParamSort)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SORTITEMS, (WPARAM)lParamSort, (LPARAM)pfnCompare);
	}

	CImageList RemoveImageList(int nImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, LVM_SETIMAGELIST, (WPARAM)nImageList, NULL));
	}

	CImageList CreateDragImage(int nItem, LPPOINT lpPoint)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, LVM_CREATEDRAGIMAGE, nItem, (LPARAM)lpPoint));
	}

	DWORD ApproximateViewRect(int cx = -1, int cy = -1, int nCount = -1)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, nCount, MAKELPARAM(cx, cy));
	}

	int SubItemHitTest(LPLVHITTESTINFO lpInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)lpInfo);
	}

	int AddColumn(LPCTSTR strItem, int nItem, int nSubItem = -1,
			int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
			int nFmt = LVCFMT_LEFT)
	{
		const int cxOffset = 15;
		ASSERT(::IsWindow(m_hWnd));
		LVCOLUMN lvc = { 0 };
		lvc.mask = nMask;
		lvc.fmt = nFmt;
		lvc.pszText = (LPTSTR)strItem;
		lvc.cx = GetStringWidth(lvc.pszText) + cxOffset;
		if(nMask & LVCF_SUBITEM)
			lvc.iSubItem = (nSubItem != -1) ? nSubItem : nItem;
		return InsertColumn(nItem, &lvc);
	}

	int AddItem(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex = -1)
	{
		ASSERT(::IsWindow(m_hWnd));
		LVITEM lvItem = { 0 };
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = nItem;
		lvItem.iSubItem = nSubItem;
		lvItem.pszText = (LPTSTR)strItem;
		if(nImageIndex != -1)
		{
			lvItem.mask |= LVIF_IMAGE;
			lvItem.iImage = nImageIndex;
		}
		if(nSubItem == 0)
			return InsertItem(&lvItem);
		return SetItem(&lvItem) ? nItem : -1;
	}

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	BOOL SortItemsEx(PFNLVCOMPARE pfnCompare, LPARAM lParamSort)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SORTITEMSEX, (WPARAM)lParamSort, (LPARAM)pfnCompare);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501)
	int InsertGroup(int nItem, PLVGROUP pGroup)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_INSERTGROUP, nItem, (LPARAM)pGroup);
	}

	int RemoveGroup(int nGroupID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_REMOVEGROUP, nGroupID, 0L);
	}

	void MoveGroup(int nGroupID, int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_MOVEGROUP, nGroupID, nItem);
	}

	void MoveItemToGroup(int nItem, int nGroupID)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_MOVEITEMTOGROUP, nItem, nGroupID);
	}

	int EnableGroupView(BOOL bEnable)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_ENABLEGROUPVIEW, bEnable, 0L);
	}

	int SortGroups(PFNLVGROUPCOMPARE pCompareFunc, LPVOID lpVoid = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_SORTGROUPS, (WPARAM)pCompareFunc, (LPARAM)lpVoid);
	}

	void InsertGroupSorted(PLVINSERTGROUPSORTED pInsertGroupSorted)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_INSERTGROUPSORTED, (WPARAM)pInsertGroupSorted, 0L);
	}

	void RemoveAllGroups()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_REMOVEALLGROUPS, 0, 0L);
	}

	BOOL HasGroup(int nGroupID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_HASGROUP, nGroupID, 0L);
	}

	BOOL InsertMarkHitTest(LPPOINT lpPoint, LPLVINSERTMARK pInsertMark) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_INSERTMARKHITTEST, (WPARAM)lpPoint, (LPARAM)pInsertMark);
	}

	BOOL SetInfoTip(PLVSETINFOTIP pSetInfoTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LVM_SETINFOTIP, 0, (LPARAM)pSetInfoTip);
	}

	void CancelEditLabel()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LVM_CANCELEDITLABEL, 0, 0L);
	}

	UINT MapIndexToID(int nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, LVM_MAPINDEXTOID, nIndex, 0L);
	}

	int MapIDToIndex(UINT uID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LVM_MAPIDTOINDEX, uID, 0L);
	}
#endif // (_WIN32_WINNT >= 0x0501)

	// single-selection only
	BOOL SelectItem(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & LVS_SINGLESEL) != 0);

		BOOL bRet = SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		if(bRet)
			bRet = EnsureVisible(nIndex, FALSE);
		return bRet;
	}
};


///////////////////////////////////////////////////////////////////////////////
// CTreeViewCtrl

class CTreeViewCtrl : public TBase
{
public:
// Constructors
	CTreeViewCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CTreeViewCtrl& operator =(HWND hWnd)
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
		return WC_TREEVIEW;
	}

	UINT GetCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0L);
	}

	UINT GetIndent() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TVM_GETINDENT, 0, 0L);
	}

	void SetIndent(UINT nIndent)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TVM_SETINDENT, nIndent, 0L);
	}

	CImageList GetImageList(int nImageListType = TVSIL_NORMAL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM)nImageListType, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList, int nImageListType = TVSIL_NORMAL)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nImageListType, (LPARAM)hImageList));
	}

	BOOL GetItem(LPTVITEM pItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem);
	}

	BOOL SetItem(LPTVITEM pItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem);
	}

	BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = nMask;
		item.pszText = (LPTSTR) lpszItem;
		item.iImage = nImage;
		item.iSelectedImage = nSelectedImage;
		item.state = nState;
		item.stateMask = nStateMask;
		item.lParam = lParam;
		return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
	}

	BOOL GetItemText(HTREEITEM hItem, LPTSTR lpstrText, int nLen) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpstrText != NULL);

		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_TEXT;
		item.pszText = lpstrText;
		item.cchTextMax = nLen;

		return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	}

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetItemText(HTREEITEM hItem, BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_TEXT;

		LPTSTR lpstrText = NULL;
		BOOL bRet = FALSE;
		for(int nLen = 256; ; nLen *= 2)
		{
			lpstrText = new TCHAR[nLen];
			if(lpstrText == NULL)
				break;
			lpstrText[0] = NULL;
			item.pszText = lpstrText;
			item.cchTextMax = nLen;
			bRet = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
			if(!bRet || (lstrlen(item.pszText) < nLen - 1))
				break;
			delete [] lpstrText;
			lpstrText = NULL;
		}

		if(lpstrText != NULL)
		{
			if(bRet)
				bstrText = ::SysAllocString(T2OLE(lpstrText));
			delete [] lpstrText;
		}

		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	BOOL GetItemText(HTREEITEM hItem, CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_TEXT;

		strText.Empty();
		BOOL bRet = FALSE;
		for(int nLen = 256; ; nLen *= 2)
		{
			item.pszText = strText.GetBufferSetLength(nLen);
			if(item.pszText == NULL)
			{
				bRet = FALSE;
				break;
			}
			item.cchTextMax = nLen;
			bRet = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
			if(!bRet || (lstrlen(item.pszText) < nLen - 1))
				break;
		}
		strText.ReleaseBuffer();
		return bRet;
	}
#endif // defined(__STR_H__)

	BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(hItem, TVIF_TEXT, lpszItem, 0, 0, 0, 0, NULL);
	}

	BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		BOOL bRes = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
		if (bRes)
		{
			nImage = item.iImage;
			nSelectedImage = item.iSelectedImage;
		}
		return bRes;
	}

	BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, nImage, nSelectedImage, 0, 0, NULL);
	}

	UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
		return (UINT)::SendMessage(m_hWnd, TVM_GETITEMSTATE, (WPARAM)hItem, (LPARAM)nStateMask);
#else // !((_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE))
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_STATE;
		item.state = 0;
		item.stateMask = nStateMask;
		::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
		return item.state;
#endif // !((_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE))
	}

	BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(hItem, TVIF_STATE, NULL, 0, 0, nState, nStateMask, NULL);
	}

	DWORD_PTR GetItemData(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_PARAM;
		BOOL bRet = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
		return (DWORD_PTR)(bRet ? item.lParam : NULL);
	}

	BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(hItem, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM)dwData);
	}

#if 0
	HWND GetEditControl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0L);
	}
#else
	CEdit GetEditControl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CEdit((HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0L));
	}
#endif

	UINT GetVisibleCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0L);
	}

	BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
	{
		ASSERT(::IsWindow(m_hWnd));
		*(HTREEITEM*)lpRect = hItem;
		return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM)bTextOnly, (LPARAM)lpRect);
	}

	BOOL ItemHasChildren(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item = { 0 };
		item.hItem = hItem;
		item.mask = TVIF_CHILDREN;
		::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
		return item.cChildren;
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetToolTips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L));
	}

	CToolTipCtrl SetToolTips(HWND hWndTT)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)hWndTT, 0L));
	}
#endif // !_WIN32_WCE

	int GetISearchString(LPTSTR lpstr) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TVM_GETISEARCHSTRING, 0, (LPARAM)lpstr);
	}

	// checkboxes only
	BOOL GetCheckState(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & TVS_CHECKBOXES) != 0);
		UINT uRet = GetItemState(hItem, TVIS_STATEIMAGEMASK);
		return (uRet >> 12) - 1;
	}

	BOOL SetCheckState(HTREEITEM hItem, BOOL bCheck)
	{
		int nCheck = bCheck ? 2 : 1;   // one based index
		return SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nCheck), TVIS_STATEIMAGEMASK);
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	COLORREF GetBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0L);
	}

	COLORREF SetBkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)clr);
	}

	COLORREF GetInsertMarkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0L);
	}

	COLORREF SetInsertMarkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM)clr);
	}

	int GetItemHeight() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0L);
	}

	int SetItemHeight(int cyHeight)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TVM_SETITEMHEIGHT, cyHeight, 0L);
	}

	int GetScrollTime() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0L);
	}

	int SetScrollTime(int nScrollTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TVM_SETSCROLLTIME, nScrollTime, 0L);
	}

	COLORREF GetTextColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0L);
	}

	COLORREF SetTextColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)clr);
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	COLORREF GetLineColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0L);
	}

	COLORREF SetLineColor(COLORREF clrNew /*= CLR_DEFAULT*/)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM)clrNew);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL GetItem(LPTVITEMEX pItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem);
	}

	BOOL SetItem(LPTVITEMEX pItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

// Operations
	HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)lpInsertStruct);
	}

	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage,
		int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter); 
	}

	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
	}

	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
		HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVINSERTSTRUCT tvis = { 0 };
		tvis.hParent = hParent;
		tvis.hInsertAfter = hInsertAfter;
		tvis.item.mask = nMask;
		tvis.item.pszText = (LPTSTR) lpszItem;
		tvis.item.iImage = nImage;
		tvis.item.iSelectedImage = nSelectedImage;
		tvis.item.state = nState;
		tvis.item.stateMask = nStateMask;
		tvis.item.lParam = lParam;
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	}

	BOOL DeleteItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem);
	}

	BOOL DeleteAllItems()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
	}

	BOOL Expand(HTREEITEM hItem, UINT nCode = TVE_EXPAND)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_EXPAND, nCode, (LPARAM)hItem);
	}

	HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, nCode, (LPARAM)hItem);
	}

	HTREEITEM GetChildItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
	}

	HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem); 
	}

	HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem);
	}

	HTREEITEM GetParentItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem); 
	}

	HTREEITEM GetFirstVisibleItem() const
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0L);
	}

	HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItem);
	}

	HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItem);
	}

	HTREEITEM GetSelectedItem() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0L);
	}

	HTREEITEM GetDropHilightItem() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0L);
	}

	HTREEITEM GetRootItem() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0L);
	}

	BOOL Select(HTREEITEM hItem, UINT nCode)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, nCode, (LPARAM)hItem);
	}

	BOOL SelectItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
	}

	BOOL SelectDropTarget(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItem);
	}

	BOOL SelectSetFirstVisible(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)hItem);
	}

#if 0
	HWND EditLabel(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem);
	}
#else
	CEdit EditLabel(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CEdit((HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem));
	}
#endif

	BOOL EndEditLabelNow(BOOL bCancel)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_ENDEDITLABELNOW, bCancel, 0L);
	}

	HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pHitTestInfo);
	}

	HTREEITEM HitTest(POINT pt, UINT* pFlags = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVHITTESTINFO hti = { 0 };
		hti.pt = pt;
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)&hti);
		if (pFlags != NULL)
			*pFlags = hti.flags;
		return hTreeItem;
	}

	BOOL SortChildren(HTREEITEM hItem, BOOL bRecurse = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDREN, (WPARAM)bRecurse, (LPARAM)hItem);
	}

	BOOL EnsureVisible(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem);
	}

	BOOL SortChildrenCB(LPTVSORTCB pSort, BOOL bRecurse = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDRENCB, (WPARAM)bRecurse, (LPARAM)pSort);
	}

	CImageList RemoveImageList(int nImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nImageList, NULL));
	}

	CImageList CreateDragImage(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem));
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL SetInsertMark(HTREEITEM hTreeItem, BOOL bAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SETINSERTMARK, bAfter, (LPARAM)hTreeItem);
	}

	BOOL RemoveInsertMark()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TVM_SETINSERTMARK, 0, 0L);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501)
	HTREEITEM MapAccIDToHTREEITEM(UINT uID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HTREEITEM)::SendMessage(m_hWnd, TVM_MAPACCIDTOHTREEITEM, uID, 0L);
	}

	UINT MapHTREEITEMToAccID(HTREEITEM hTreeItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TVM_MAPHTREEITEMTOACCID, (WPARAM)hTreeItem, 0L);
	}
#endif // (_WIN32_WINNT >= 0x0501)
};


///////////////////////////////////////////////////////////////////////////////
// CTreeViewCtrlEx

// forward declaration
class CTreeViewCtrlEx;

// Note: TBase here is for CTreeViewCtrlExT, and not for CTreeItemT itself
class CTreeItem
{
public:
	HTREEITEM m_hTreeItem;
	CTreeViewCtrlEx* m_pTreeView;

// Construction
	CTreeItem(HTREEITEM hTreeItem = NULL, CTreeViewCtrlEx* pTreeView = NULL) : m_hTreeItem(hTreeItem), m_pTreeView(pTreeView)
	{ }
 
	CTreeItem(const CTreeItem& posSrc)
	{
		*this = posSrc;
	}

	operator HTREEITEM() { return m_hTreeItem; }

	CTreeItem& operator =(const CTreeItem& itemSrc)
	{
		m_hTreeItem = itemSrc.m_hTreeItem;
		m_pTreeView = itemSrc.m_pTreeView;
		return *this;
	}

// Attributes
	CTreeViewCtrlEx* GetTreeView() const { return m_pTreeView; }

	BOOL operator !() const { return m_hTreeItem == NULL; }

	BOOL IsNull() const { return m_hTreeItem == NULL; }
	
	BOOL GetRect(LPRECT lpRect, BOOL bTextOnly) const;
	BOOL GetText(LPTSTR lpstrText, int nLen) const;
#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetText(BSTR& bstrText) const;
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM
#if defined(__STR_H__)
	BOOL GetText(CString& strText) const;
#endif // defined(__STR_H__)
	BOOL SetText(LPCTSTR lpszItem);
	BOOL GetImage(int& nImage, int& nSelectedImage) const;
	BOOL SetImage(int nImage, int nSelectedImage);
	UINT GetState(UINT nStateMask) const;
	BOOL SetState(UINT nState, UINT nStateMask);
	DWORD_PTR GetData() const;
	BOOL SetData(DWORD_PTR dwData);
	BOOL SetItem(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);

// Operations
	CTreeItem InsertAfter(LPCTSTR lpstrItem, HTREEITEM hItemAfter, int nImageIndex)
	{
		return _Insert(lpstrItem, nImageIndex, hItemAfter);
	}

	CTreeItem AddHead(LPCTSTR lpstrItem, int nImageIndex)
	{
		return _Insert(lpstrItem, nImageIndex, TVI_FIRST);
	}

	CTreeItem AddTail(LPCTSTR lpstrItem, int nImageIndex)
	{
		return _Insert(lpstrItem, nImageIndex, TVI_LAST);
	}

	CTreeItem GetChild() const;
	CTreeItem GetNext(UINT nCode) const;
	CTreeItem GetNextSibling() const;
	CTreeItem GetPrevSibling() const;
	CTreeItem GetParent() const;
	CTreeItem GetFirstVisible() const;
	CTreeItem GetNextVisible() const;
	CTreeItem GetPrevVisible() const;
	CTreeItem GetSelected() const;
	CTreeItem GetDropHilight() const;
	CTreeItem GetRoot() const;
	BOOL HasChildren() const;
	BOOL Delete();
	BOOL Expand(UINT nCode = TVE_EXPAND);
	BOOL Select(UINT nCode);
	BOOL Select();
	BOOL SelectDropTarget();
	BOOL SelectSetFirstVisible();
	HWND EditLabel();
	HIMAGELIST CreateDragImage();
	BOOL SortChildren(BOOL bRecurse = FALSE);
	BOOL EnsureVisible();
	CTreeItem _Insert(LPCTSTR lpstrItem, int nImageIndex, HTREEITEM hItemAfter);
	int GetImageIndex() const;
#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL SetInsertMark(BOOL bAfter);
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
#if (_WIN32_WINNT >= 0x0501)
	UINT MapHTREEITEMToAccID() const;
#endif // (_WIN32_WINNT >= 0x0501)
};


class CTreeViewCtrlEx : public CTreeViewCtrl
{
public:
// Constructors
	CTreeViewCtrlEx(HWND hWnd = NULL) : CTreeViewCtrl(hWnd)
	{ }

	CTreeViewCtrlEx& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

// Operations (overides that return CTreeItem)
	CTreeItem InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)lpInsertStruct);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem InsertItem(LPCTSTR lpszItem, int nImage,
		int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter); 
	}

	CTreeItem InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		return InsertItem(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
	}

	CTreeItem GetNextItem(HTREEITEM hItem, UINT nCode)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, nCode, (LPARAM)hItem);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetChildItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
		return CTreeItem(hTreeItem, this); 
	}

	CTreeItem GetNextSiblingItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem); 
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetPrevSiblingItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetParentItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem); 
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetFirstVisibleItem()
	{
		ASSERT(::IsWindow(m_hWnd)); 
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0L);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetNextVisibleItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItem);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetPrevVisibleItem(HTREEITEM hItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItem);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetSelectedItem()
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0L);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetDropHilightItem()
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0L);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem GetRootItem()
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0L);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem HitTest(TVHITTESTINFO* pHitTestInfo)
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pHitTestInfo);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
		HTREEITEM hParent, HTREEITEM hInsertAfter)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVINSERTSTRUCT tvis = { 0 };
		tvis.hParent = hParent;
		tvis.hInsertAfter = hInsertAfter;
		tvis.item.mask = nMask;
		tvis.item.pszText = (LPTSTR) lpszItem;
		tvis.item.iImage = nImage;
		tvis.item.iSelectedImage = nSelectedImage;
		tvis.item.state = nState;
		tvis.item.stateMask = nStateMask;
		tvis.item.lParam = lParam;
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
		return CTreeItem(hTreeItem, this);
	}

	CTreeItem HitTest(POINT pt, UINT* pFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVHITTESTINFO hti = { 0 };
		hti.pt = pt;
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)&hti);
		if (pFlags != NULL)
			*pFlags = hti.flags;
		return CTreeItem(hTreeItem, this);
	}

#if (_WIN32_WINNT >= 0x0501)
	CTreeItem MapAccIDToHTREEITEM(UINT uID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_MAPACCIDTOHTREEITEM, uID, 0L);
		return CTreeItem(hTreeItem, (CTreeViewCtrlEx*)this);
	}
#endif // (_WIN32_WINNT >= 0x0501)
};


// CTreeItem inline methods
inline BOOL CTreeItem::GetRect(LPRECT lpRect, BOOL bTextOnly) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemRect(m_hTreeItem,lpRect,bTextOnly);
}

inline CTreeItem CTreeItem::GetNext(UINT nCode) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetNextItem(m_hTreeItem,nCode);
}

inline CTreeItem CTreeItem::GetChild() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetChildItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetNextSibling() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetNextSiblingItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetPrevSibling() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetPrevSiblingItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetParent() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetParentItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetFirstVisible() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetFirstVisibleItem();
}

inline CTreeItem CTreeItem::GetNextVisible() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetNextVisibleItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetPrevVisible() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetPrevVisibleItem(m_hTreeItem);
}

inline CTreeItem CTreeItem::GetSelected() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetSelectedItem();
}

inline CTreeItem CTreeItem::GetDropHilight() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetDropHilightItem();
}

inline CTreeItem CTreeItem::GetRoot() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetRootItem();
}

inline BOOL CTreeItem::GetText(LPTSTR lpstrText, int nLen) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemText(m_hTreeItem, lpstrText, nLen);
}

#ifndef _TWL_NO_COM
#ifdef _OLEAUTO_H_
#ifdef __STRCONV_H__
inline BOOL CTreeItem::GetText(BSTR& bstrText) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemText(m_hTreeItem, bstrText);
}
#endif // __STRCONV_H__
#endif // _OLEAUTO_H_
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
inline BOOL CTreeItem::GetText(CString& strText) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemText(m_hTreeItem, strText);
}
#endif // defined(__STR_H__)

inline BOOL CTreeItem::GetImage(int& nImage, int& nSelectedImage) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemImage(m_hTreeItem,nImage,nSelectedImage);
}

inline UINT CTreeItem::GetState(UINT nStateMask) const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemState(m_hTreeItem,nStateMask);
}

inline DWORD_PTR CTreeItem::GetData() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->GetItemData(m_hTreeItem);
}

inline BOOL CTreeItem::SetItem(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetItem(m_hTreeItem, nMask, lpszItem, nImage, nSelectedImage, nState, nStateMask, lParam);
}

inline BOOL CTreeItem::SetText(LPCTSTR lpszItem)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetItemText(m_hTreeItem,lpszItem);
}

inline BOOL CTreeItem::SetImage(int nImage, int nSelectedImage)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetItemImage(m_hTreeItem,nImage,nSelectedImage);
}

inline BOOL CTreeItem::SetState(UINT nState, UINT nStateMask)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetItemState(m_hTreeItem,nState,nStateMask);
}

inline BOOL CTreeItem::SetData(DWORD_PTR dwData)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetItemData(m_hTreeItem,dwData);
}

inline BOOL CTreeItem::HasChildren() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->ItemHasChildren(m_hTreeItem);
}

inline BOOL CTreeItem::Delete()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->DeleteItem(m_hTreeItem);
}

inline BOOL CTreeItem::Expand(UINT nCode /*= TVE_EXPAND*/)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->Expand(m_hTreeItem,nCode);
}

inline BOOL CTreeItem::Select(UINT nCode)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->Select(m_hTreeItem,nCode);
}

inline BOOL CTreeItem::Select()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SelectItem(m_hTreeItem);
}

inline BOOL CTreeItem::SelectDropTarget()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SelectDropTarget(m_hTreeItem);
}

inline BOOL CTreeItem::SelectSetFirstVisible()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SelectSetFirstVisible(m_hTreeItem);
}

inline HWND CTreeItem::EditLabel()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->EditLabel(m_hTreeItem);
}

inline HIMAGELIST CTreeItem::CreateDragImage()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->CreateDragImage(m_hTreeItem);
}

inline BOOL CTreeItem::SortChildren(BOOL bRecurse /*= FALSE*/)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SortChildren(m_hTreeItem, bRecurse);
}

inline BOOL CTreeItem::EnsureVisible()
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->EnsureVisible(m_hTreeItem);
}

inline CTreeItem CTreeItem::_Insert(LPCTSTR lpstrItem, int nImageIndex, HTREEITEM hItemAfter)
{
	ASSERT(m_pTreeView != NULL);
	TVINSERTSTRUCT ins = { 0 };
	ins.hParent = m_hTreeItem;
	ins.hInsertAfter = hItemAfter;
	ins.item.mask = TVIF_TEXT;
	ins.item.pszText = (LPTSTR)lpstrItem;
	if(nImageIndex != -1)
	{
		ins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		ins.item.iImage = nImageIndex;
		ins.item.iSelectedImage = nImageIndex;
	}
	return CTreeItem(m_pTreeView->InsertItem(&ins), m_pTreeView);
}

inline int CTreeItem::GetImageIndex() const
{
	ASSERT(m_pTreeView != NULL);
	TVITEM item = { 0 };
	item.mask = TVIF_HANDLE | TVIF_IMAGE;
	item.hItem = m_hTreeItem;
	m_pTreeView->GetItem(&item);
	return item.iImage;
}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
inline BOOL CTreeItem::SetInsertMark(BOOL bAfter)
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->SetInsertMark(m_hTreeItem, bAfter);
}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501)
inline UINT CTreeItem::MapHTREEITEMToAccID() const
{
	ASSERT(m_pTreeView != NULL);
	return m_pTreeView->MapHTREEITEMToAccID(m_hTreeItem);
}
#endif // (_WIN32_WINNT >= 0x0501)


///////////////////////////////////////////////////////////////////////////////
// CToolBarCtrl

#define STRING_REFRESH_OFFSET 1000000

class CToolBarCtrl : public TBase
{
public:
// Construction
	CToolBarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CToolBarCtrl& operator =(HWND hWnd)
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
		return TOOLBARCLASSNAME;
	}

	BOOL IsButtonEnabled(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONENABLED, nID, 0L);
	}

	BOOL IsButtonChecked(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONCHECKED, nID, 0L);
	}

	BOOL IsButtonPressed(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONPRESSED, nID, 0L);
	}

	BOOL IsButtonHidden(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return(BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONHIDDEN, nID, 0L);
	}

	BOOL IsButtonIndeterminate(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONINDETERMINATE, nID, 0L);
	}

	int GetState(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETSTATE, nID, 0L);
	}

	BOOL SetState(int nID, UINT nState)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETSTATE, nID, MAKELPARAM(nState, 0));
	}

	BOOL GetButton(int nIndex, LPTBBUTTON lpButton) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)lpButton);
	}

	// ported from MFC
	void SetButton(int nIndex, LPTBBUTTON pButton)
	{
		// get original button state
		TBBUTTON button;
		GetButton(nIndex, &button);

		// prepare for old/new button comparison
		button.bReserved[0] = 0;
		button.bReserved[1] = 0;
		// TBSTATE_ENABLED == TBBS_DISABLED so invert it
		pButton->fsState ^= TBSTATE_ENABLED;
		pButton->bReserved[0] = 0;
		pButton->bReserved[1] = 0;

		// nothing to do if they are the same
		if(memcmp(pButton, &button, sizeof(TBBUTTON)) == 0)
			return;

		// don't redraw everything while setting the button
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle ^ WS_VISIBLE);

		DeleteButton(nIndex);

		// Force a recalc of the toolbar's layout to work around a comctl bug
		if(pButton->iString < -1)
		{
			int iTextRows = (int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0);
			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
			::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows + 1, 0);
			::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows, 0);
			::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
			pButton->iString += STRING_REFRESH_OFFSET;
		}

		InsertButton(nIndex, pButton);

		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

		// invalidate appropriate parts
		if(((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
		else
		{
			// invalidate just the button
			RECT rc;
			if(GetItemRect(nIndex, &rc))
				::InvalidateRect(m_hWnd, &rc, TRUE);
		}
	}

	int GetButtonCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0L);
	}

	BOOL GetItemRect(int nIndex, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	void SetButtonStructSize(int nSize = sizeof(TBBUTTON))
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, nSize, 0L);
	}

	BOOL SetButtonSize(SIZE size)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy));
	}

	BOOL SetButtonSize(int cx, int cy)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy));
	}

	BOOL SetBitmapSize(SIZE size)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy));
	}

	BOOL SetBitmapSize(int cx, int cy)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(cx, cy));
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetToolTips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, TB_GETTOOLTIPS, 0, 0L));
	}

	void SetToolTips(HWND hWndToolTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETTOOLTIPS, (WPARAM)hWndToolTip, 0L);
	}
#endif // !_WIN32_WCE

	void SetNotifyWnd(HWND hWnd)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETPARENT, (WPARAM)hWnd, 0L);
	}

	int GetRows() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
	}

	void SetRows(int nRows, BOOL bLarger, LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM)lpRect);
	}

	BOOL SetCmdID(int nIndex, UINT nID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETCMDID, nIndex, nID);
	}

	DWORD GetBitmapFlags() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_GETBITMAPFLAGS, 0, 0L);
	}

	int GetBitmap(int nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETBITMAP, nID, 0L);
	}

	int GetButtonText(int nID, LPTSTR lpstrText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, nID, (LPARAM)lpstrText);
	}

	CImageList GetImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList));
	}

	CImageList GetDisabledImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0L));
	}

	CImageList SetDisabledImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hImageList));
	}

#ifndef _WIN32_WCE
	CImageList GetHotImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0L));
	}

	CImageList SetHotImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)hImageList));
	}
#endif // !_WIN32_WCE

	DWORD GetStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
	}

	void SetStyle(DWORD dwStyle)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETSTYLE, 0, dwStyle);
	}

	DWORD GetButtonSize() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
	}

	void GetButtonSize(SIZE& size) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
		size.cx = LOWORD(dwRet);
		size.cy = HIWORD(dwRet);
	}

	BOOL GetRect(int nID, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETRECT, nID, (LPARAM)lpRect);
	}

	int GetTextRows() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0L);
	}

	BOOL SetButtonWidth(int cxMin, int cxMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax));
	}

	BOOL SetIndent(int nIndent)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETINDENT, nIndent, 0L);
	}

	BOOL SetMaxTextRows(int nMaxTextRows)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, nMaxTextRows, 0L);
	}

#if (_WIN32_IE >= 0x0400)
#ifndef _WIN32_WCE
	BOOL GetAnchorHighlight() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETANCHORHIGHLIGHT, 0, 0L);
	}

	BOOL SetAnchorHighlight(BOOL bEnable = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETANCHORHIGHLIGHT, bEnable, 0L);
	}
#endif // !_WIN32_WCE

	int GetButtonInfo(int nID, LPTBBUTTONINFO lptbbi) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETBUTTONINFO, nID, (LPARAM)lptbbi);
	}

	BOOL SetButtonInfo(int nID, LPTBBUTTONINFO lptbbi)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM)lptbbi);
	}

	BOOL SetButtonInfo(int nID, DWORD dwMask, BYTE Style, BYTE State, LPCTSTR lpszItem, 
		int iImage, WORD cx, int iCommand, DWORD_PTR lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBBUTTONINFO tbbi = { 0 };
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = dwMask;
		tbbi.idCommand = iCommand;
		tbbi.iImage = iImage;
		tbbi.fsState = State;
		tbbi.fsStyle = Style;
		tbbi.cx = cx;
		tbbi.pszText = (LPTSTR) lpszItem;
		tbbi.lParam = lParam;
		return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM)&tbbi);
	}

#ifndef _WIN32_WCE
	int GetHotItem() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0L);
	}

	int SetHotItem(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_SETHOTITEM, nItem, 0L);
	}
#endif // !_WIN32_WCE

	BOOL IsButtonHighlighted(int nButtonID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONHIGHLIGHTED, nButtonID, 0L);
	}

	DWORD SetDrawTextFlags(DWORD dwMask, DWORD dwFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_SETDRAWTEXTFLAGS, dwMask, dwFlags);
	}

#ifndef _WIN32_WCE
	BOOL GetColorScheme(LPCOLORSCHEME lpcs) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETCOLORSCHEME, 0, (LPARAM)lpcs);
	}

	void SetColorScheme(LPCOLORSCHEME lpcs)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETCOLORSCHEME, 0, (LPARAM)lpcs);
	}

	DWORD GetExtendedStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
	}

	DWORD SetExtendedStyle(DWORD dwStyle)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwStyle);
	}

	void GetInsertMark(LPTBINSERTMARK lptbim) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_GETINSERTMARK, 0, (LPARAM)lptbim);
	}

	void SetInsertMark(LPTBINSERTMARK lptbim)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETINSERTMARK, 0, (LPARAM)lptbim);
	}

	COLORREF GetInsertMarkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TB_GETINSERTMARKCOLOR, 0, 0L);
	}

	COLORREF SetInsertMarkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, TB_SETINSERTMARKCOLOR, 0, (LPARAM)clr);
	}

	BOOL GetMaxSize(LPSIZE lpSize) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)lpSize);
	}

	void GetPadding(LPSIZE lpSizePadding) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpSizePadding != NULL);
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, TB_GETPADDING, 0, 0L);
		lpSizePadding->cx = (short)LOWORD(dwRet);
		lpSizePadding->cy = (short)HIWORD(dwRet);
	}

	void SetPadding(int cx, int cy, LPSIZE lpSizePadding = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, TB_SETPADDING, 0, MAKELPARAM(cx, cy));
		if(lpSizePadding != NULL)
		{
			lpSizePadding->cx = (short)LOWORD(dwRet);
			lpSizePadding->cy = (short)HIWORD(dwRet);
		}
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
#ifndef _TWL_NO_COM
	int GetString(int nString, LPTSTR lpstrString, int cchMaxLen) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(cchMaxLen, nString), (LPARAM)lpstrString);
	}
#ifdef __STRCONV_H__
	int GetStringBSTR(int nString, BSTR& bstrString) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrString == NULL);
		int nLength = (int)(short)LOWORD(::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(0, nString), NULL));
		if(nLength != -1)
		{
			LPTSTR lpszString = (LPTSTR)_alloca((nLength + 1) * sizeof(TCHAR));
			nLength = (int)::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(nLength + 1, nString), (LPARAM)lpszString);
			if(nLength != -1)
				bstrString = ::SysAllocString(T2OLE(lpszString));
		}
		return nLength;
	}
#endif // __STRCONV_H__
#endif // _TWL_NO_COM

#if defined(__STR_H__)
	int GetString(int nString, CString& str) const
	{
		ASSERT(::IsWindow(m_hWnd));
		int nLength = (int)(short)LOWORD(::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(0, nString), NULL));
		if(nLength != -1)
		{
			LPTSTR lpstr = str.GetBufferSetLength(nLength + 1);
			if(lpstr != NULL)
				nLength = (int)::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(nLength + 1, nString), (LPARAM)lpstr);
			else
				nLength = -1;
			str.ReleaseBuffer();
		}
		return nLength;
	}
#endif // defined(__STR_H__)
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501)
	void GetMetrics(LPTBMETRICS lptbm) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_GETMETRICS, 0, (LPARAM)lptbm);
	}

	void SetMetrics(LPTBMETRICS lptbm)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETMETRICS, 0, (LPARAM)lptbm);
	}

	void SetWindowTheme(LPCWSTR lpstrTheme)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	BOOL EnableButton(int nID, BOOL bEnable = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ENABLEBUTTON, nID, MAKELPARAM(bEnable, 0));
	}

	BOOL CheckButton(int nID, BOOL bCheck = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_CHECKBUTTON, nID, MAKELPARAM(bCheck, 0));
	}

	BOOL PressButton(int nID, BOOL bPress = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_PRESSBUTTON, nID, MAKELPARAM(bPress, 0));
	}

	BOOL HideButton(int nID, BOOL bHide = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_HIDEBUTTON, nID, MAKELPARAM(bHide, 0));
	}

	BOOL Indeterminate(int nID, BOOL bIndeterminate = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_INDETERMINATE, nID, MAKELPARAM(bIndeterminate, 0));
	}

	int AddBitmap(int nNumButtons, UINT nBitmapID)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBADDBITMAP tbab = { 0 };
		tbab.hInst = GetResourceInstance();
		ASSERT(tbab.hInst != NULL);
		tbab.nID = nBitmapID;
		return (int)::SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
	}

	int AddBitmap(int nNumButtons, HBITMAP hBitmap)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBADDBITMAP tbab = { 0 };
		tbab.hInst = NULL;
		tbab.nID = (UINT_PTR)hBitmap;
		return (int)::SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
	}

	BOOL AddButtons(int nNumButtons, LPTBBUTTON lpButtons)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_ADDBUTTONS, nNumButtons, (LPARAM)lpButtons);
	}

	BOOL InsertButton(int nIndex, LPTBBUTTON lpButton)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM)lpButton);
	}

	BOOL InsertButton(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap, 
		INT_PTR iString, DWORD_PTR lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBBUTTON tbb = { 0 };
		tbb.fsStyle = Style;
		tbb.fsState = State;
		tbb.idCommand = iCommand;
		tbb.iBitmap = iBitmap;
		tbb.iString = iString;
		tbb.dwData = lParam;
		return (BOOL)::SendMessage(m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM)&tbb);
	}

	BOOL InsertButton(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap, 
		LPCTSTR lpszItem, DWORD_PTR lParam)
	{
		return InsertButton(nIndex, iCommand, Style, State, iBitmap, (INT_PTR)lpszItem, lParam);
	}

	BOOL DeleteButton(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_DELETEBUTTON, nIndex, 0L);
	}

	UINT CommandToIndex(UINT nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, nID, 0L);
	}

#ifndef _WIN32_WCE
	void SaveState(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBSAVEPARAMS tbs = { 0 };
		tbs.hkr = hKeyRoot;
		tbs.pszSubKey = lpszSubKey;
		tbs.pszValueName = lpszValueName;
		::SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM)TRUE, (LPARAM)&tbs);
	}

	void RestoreState(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName)
	{
		ASSERT(::IsWindow(m_hWnd));
		TBSAVEPARAMS tbs = { 0 };
		tbs.hkr = hKeyRoot;
		tbs.pszSubKey = lpszSubKey;
		tbs.pszValueName = lpszValueName;
		::SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM)FALSE, (LPARAM)&tbs);
	}

	void Customize()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_CUSTOMIZE, 0, 0L);
	}
#endif // !_WIN32_WCE

	int AddString(UINT nStringID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_ADDSTRING, (WPARAM)GetResourceInstance(), (LPARAM)nStringID);
	}

	int AddStrings(LPCTSTR lpszStrings)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)lpszStrings);
	}

	void AutoSize()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0L);
	}

	BOOL ChangeBitmap(int nID, int nBitmap)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_CHANGEBITMAP, nID, MAKELPARAM(nBitmap, 0));
	}

	int LoadImages(int nBitmapID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_LOADIMAGES, nBitmapID, (LPARAM)GetResourceInstance());
	}

	int LoadStdImages(int nBitmapID)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_LOADIMAGES, nBitmapID, (LPARAM)HINST_COMMCTRL);
	}

	BOOL ReplaceBitmap(LPTBREPLACEBITMAP ptbrb)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_REPLACEBITMAP, 0, (LPARAM)ptbrb);
	}

#if (_WIN32_IE >= 0x0400)
	int HitTest(LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)lpPoint);
	}

#ifndef _WIN32_WCE
	BOOL InsertMarkHitTest(LPPOINT lpPoint, LPTBINSERTMARK lptbim) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)lpPoint, (LPARAM)lptbim);
	}

	BOOL InsertMarkHitTest(int x, int y, LPTBINSERTMARK lptbim) const
	{
		ASSERT(::IsWindow(m_hWnd));
		POINT pt = { x, y };
		return (BOOL)::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)&pt, (LPARAM)lptbim);
	}

	BOOL MapAccelerator(TCHAR chAccel, int& nID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)&nID);
	}

	BOOL MarkButton(int nID, BOOL bHighlight = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0));
	}

	BOOL MoveButton(int nOldPos, int nNewPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TB_MOVEBUTTON, nOldPos, nNewPos);
	}

	HRESULT GetObject(REFIID iid, LPVOID* ppvObject)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HRESULT)::SendMessage(m_hWnd, TB_GETOBJECT, (WPARAM)&iid, (LPARAM)ppvObject);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)
};


///////////////////////////////////////////////////////////////////////////////
// CStatusBarCtrl

class CStatusBarCtrl : public TBase
{
public:
// Constructors
	CStatusBarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CStatusBarCtrl& operator =(HWND hWnd)
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

// Methods
	LPCTSTR GetClassName()
	{
		return STATUSCLASSNAME;
	}

	int GetParts(int nParts, int* pParts) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SB_GETPARTS, nParts, (LPARAM)pParts);
	}

	BOOL SetParts(int nParts, int* pWidths)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_SETPARTS, nParts, (LPARAM)pWidths);
	}

	int GetTextLength(int nPane, int* pType = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, SB_GETTEXTLENGTH, (WPARAM)nPane, 0L);
		if (pType != NULL)
			*pType = (int)(short)HIWORD(dwRet);
		return (int)(short)LOWORD(dwRet);
	}

	int GetText(int nPane, LPTSTR lpszText, int* pType = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, SB_GETTEXT, (WPARAM)nPane, (LPARAM)lpszText);
		if(pType != NULL)
			*pType = (int)(short)HIWORD(dwRet);
		return (int)(short)LOWORD(dwRet);
	}

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetTextBSTR(int nPane, BSTR& bstrText, int* pType = NULL) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		ASSERT(bstrText == NULL);
		int nLength = (int)(short)LOWORD(::SendMessage(m_hWnd, SB_GETTEXTLENGTH, (WPARAM)nPane, 0L));
		if(nLength == 0)
			return FALSE;

		LPTSTR lpszText = (LPTSTR)_alloca((nLength + 1) * sizeof(TCHAR));
		if(!GetText(nPane, lpszText, pType))
			return FALSE;

		bstrText = ::SysAllocString(T2OLE(lpszText));
		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	int GetText(int nPane, CString& strText, int* pType = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		int nLength = (int)(short)LOWORD(::SendMessage(m_hWnd, SB_GETTEXTLENGTH, (WPARAM)nPane, 0L));
		if(nLength == 0)
			return 0;

		LPTSTR lpstr = strText.GetBufferSetLength(nLength);
		if(lpstr == NULL)
			return 0;
		return GetText(nPane, lpstr, pType);
	}
#endif // defined(__STR_H__)

	BOOL SetText(int nPane, LPCTSTR lpszText, int nType = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		return (BOOL)::SendMessage(m_hWnd, SB_SETTEXT, (nPane | nType), (LPARAM)lpszText);
	}

	BOOL GetRect(int nPane, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		return (BOOL)::SendMessage(m_hWnd, SB_GETRECT, nPane, (LPARAM)lpRect);
	}

	BOOL GetBorders(int* pBorders) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM)pBorders);
	}

	BOOL GetBorders(int& nHorz, int& nVert, int& nSpacing) const
	{
		ASSERT(::IsWindow(m_hWnd));
		int borders[3] = { 0, 0, 0 };
		BOOL bResult = (BOOL)::SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM)&borders);
		if(bResult)
		{
			nHorz = borders[0];
			nVert = borders[1];
			nSpacing = borders[2];
		}
		return bResult;
	}

	void SetMinHeight(int nMin)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SB_SETMINHEIGHT, nMin, 0L);
	}

	BOOL SetSimple(BOOL bSimple = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_SIMPLE, bSimple, 0L);
	}

	BOOL IsSimple() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_ISSIMPLE, 0, 0L);
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	COLORREF SetBkColor(COLORREF clrBk)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, SB_SETBKCOLOR, 0, (LPARAM)clrBk);
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SB_SETUNICODEFORMAT, bUnicode, 0L);
	}

	void GetTipText(int nPane, LPTSTR lpstrText, int nSize) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		::SendMessage(m_hWnd, SB_GETTIPTEXT, MAKEWPARAM(nPane, nSize), (LPARAM)lpstrText);
	}

	void SetTipText(int nPane, LPCTSTR lpstrText)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		::SendMessage(m_hWnd, SB_SETTIPTEXT, nPane, (LPARAM)lpstrText);
	}

	HICON GetIcon(int nPane) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		return (HICON)::SendMessage(m_hWnd, SB_GETICON, nPane, 0L);
	}

	BOOL SetIcon(int nPane, HICON hIcon)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nPane < 256);
		return (BOOL)::SendMessage(m_hWnd, SB_SETICON, nPane, (LPARAM)hIcon);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
};


///////////////////////////////////////////////////////////////////////////////
// CTabCtrl

class CTabCtrl : public TBase
{
public:
// Constructors
	CTabCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CTabCtrl& operator =(HWND hWnd)
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
		return WC_TABCONTROL;
	}

	CImageList GetImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)hImageList));
	}

	int GetItemCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0L);
	}

	BOOL GetItem(int nItem, LPTCITEM pTabCtrlItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_GETITEM, nItem, (LPARAM)pTabCtrlItem);
	}

	BOOL SetItem(int nItem, LPTCITEM pTabCtrlItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_SETITEM, nItem, (LPARAM)pTabCtrlItem);
	}

	int SetItem(int nItem, UINT mask, LPCTSTR lpszItem, DWORD dwState, DWORD dwStateMask, int iImage, DWORD lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		TCITEM tci = { 0 };
		tci.mask = mask;
		tci.pszText = (LPTSTR) lpszItem;
		tci.dwState = dwState;
		tci.dwStateMask = dwStateMask;
		tci.iImage = iImage;
		tci.lParam = lParam;
		return (int)::SendMessage(m_hWnd, TCM_SETITEM, nItem, (LPARAM)&tci);
	}

	BOOL GetItemRect(int nItem, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_GETITEMRECT, nItem, (LPARAM)lpRect);
	}

	int GetCurSel() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0L);
	}

	int SetCurSel(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_SETCURSEL, nItem, 0L);
	}

	SIZE SetItemSize(SIZE size)
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwSize = (DWORD)::SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(size.cx, size.cy));
		SIZE sizeRet = { (short)LOWORD(dwSize), (short)HIWORD(dwSize) };
		return sizeRet;
	}

	void SetItemSize(int cx, int cy)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(cx, cy));
	}

	void SetPadding(SIZE size)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_SETPADDING, 0, MAKELPARAM(size.cx, size.cy));
	}

	int GetRowCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_GETROWCOUNT, 0, 0L);
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetTooltips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, TCM_GETTOOLTIPS, 0, 0L));
	}

	void SetTooltips(HWND hWndToolTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_SETTOOLTIPS, (WPARAM)hWndToolTip, 0L);
	}
#endif // !_WIN32_WCE

	int GetCurFocus() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_GETCURFOCUS, 0, 0L);
	}

	void SetCurFocus(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_SETCURFOCUS, nItem, 0L);
	}

	BOOL SetItemExtra(int cbExtra)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(GetItemCount() == 0);   // must be empty
		return (BOOL)::SendMessage(m_hWnd, TCM_SETITEMEXTRA, cbExtra, 0L);
	}

	int SetMinTabWidth(int nWidth = -1)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_SETMINTABWIDTH, 0, nWidth);
	}

#if (_WIN32_IE >= 0x0400)
	DWORD GetExtendedStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TCM_GETEXTENDEDSTYLE, 0, 0L);
	}

	DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyle)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, TCM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
	}

#ifndef _WIN32_WCE
	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

// Operations
	int InsertItem(int nItem, LPTCITEM pTabCtrlItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM)pTabCtrlItem);
	}

	int InsertItem(int nItem, UINT mask, LPCTSTR lpszItem, int iImage, DWORD lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		TCITEM tci = { 0 };
		tci.mask = mask;
		tci.pszText = (LPTSTR) lpszItem;
		tci.iImage = iImage;
		tci.lParam = lParam;
		return (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM)&tci);
	}

	int InsertItem(int nItem, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		TCITEM tci = { 0 };
		tci.mask = TCIF_TEXT;
		tci.pszText = (LPTSTR) lpszItem;
		return (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM)&tci);
	}

	BOOL DeleteItem(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_DELETEITEM, nItem, 0L);
	}

	BOOL DeleteAllItems()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_DELETEALLITEMS, 0, 0L);
	}

	void AdjustRect(BOOL bLarger, LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_ADJUSTRECT, bLarger, (LPARAM)lpRect);
	}

	void RemoveImage(int nImage)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_REMOVEIMAGE, nImage, 0L);
	}

	int HitTest(TC_HITTESTINFO* pHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)pHitTestInfo);
	}

	void DeselectAll(BOOL bExcludeFocus = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TCM_DESELECTALL, bExcludeFocus, 0L);
	}

#if (_WIN32_IE >= 0x0400)
	BOOL HighlightItem(int nIndex, BOOL bHighlight = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TCM_HIGHLIGHTITEM, nIndex, MAKELPARAM(bHighlight, 0));
	}
#endif // (_WIN32_IE >= 0x0400)
};


///////////////////////////////////////////////////////////////////////////////
// CTrackBarCtrl

class CTrackBarCtrl : public TBase
{
public:
// Constructors
	CTrackBarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CTrackBarCtrl& operator =(HWND hWnd)
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
		return TRACKBAR_CLASS;
	}

	int GetLineSize() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETLINESIZE, 0, 0L);
	}

	int SetLineSize(int nSize)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_SETLINESIZE, 0, nSize);
	}

	int GetPageSize() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0L);
	}

	int SetPageSize(int nSize)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_SETPAGESIZE, 0, nSize);
	}

	int GetRangeMin() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0L);
	}

	void SetRangeMin(int nMin, BOOL bRedraw = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETRANGEMIN, bRedraw, nMin);
	}

	int GetRangeMax() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0L);
	}

	void SetRangeMax(int nMax, BOOL bRedraw = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETRANGEMAX, bRedraw, nMax);
	}

	void GetRange(int& nMin, int& nMax) const
	{
		nMin = GetRangeMin();
		nMax = GetRangeMax();
	}

	void SetRange(int nMin, int nMax, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETRANGE, bRedraw, MAKELPARAM(nMin, nMax));
	}

	int GetSelStart() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETSELSTART, 0, 0L);
	}

	void SetSelStart(int nMin)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETSELSTART, 0, (LPARAM)nMin);
	}

	int GetSelEnd() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETSELEND, 0, 0L);
	}

	void SetSelEnd(int nMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETSELEND, 0, (LPARAM)nMax);
	}

	void GetSelection(int& nMin, int& nMax) const
	{
		nMin = GetSelStart();
		nMax = GetSelEnd();
	}

	void SetSelection(int nMin, int nMax)
	{
		SetSelStart(nMin);
		SetSelEnd(nMax);
	}

	void GetChannelRect(LPRECT lprc) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)lprc);
	}

	void GetThumbRect(LPRECT lprc) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)lprc);
	}

	int GetPos() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETPOS, 0, 0L);
	}

	void SetPos(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETPOS, TRUE, nPos);
	}

	UINT GetNumTics() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, TBM_GETNUMTICS, 0, 0L);
	}

	DWORD* GetTicArray() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD*)::SendMessage(m_hWnd, TBM_GETPTICS, 0, 0L);
	}

	int GetTic(int nTic) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETTIC, nTic, 0L);
	}

	BOOL SetTic(int nTic)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TBM_SETTIC, 0, nTic);
	}

	int GetTicPos(int nTic) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETTICPOS, nTic, 0L);
	}

	void SetTicFreq(int nFreq)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETTICFREQ, nFreq, 0L);
	}

	int GetThumbLength() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_GETTHUMBLENGTH, 0, 0L);
	}

	void SetThumbLength(int nLength)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETTHUMBLENGTH, nLength, 0L);
	}

	void SetSel(int nStart, int nEnd, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & TBS_ENABLESELRANGE) != 0);
		::SendMessage(m_hWnd, TBM_SETSEL, bRedraw, MAKELPARAM(nStart, nEnd));
	}

	HWND GetBuddy(BOOL bLeft = TRUE) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, TBM_GETBUDDY, bLeft, 0L);
	}

	HWND SetBuddy(HWND hWndBuddy, BOOL bLeft = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, TBM_SETBUDDY, bLeft, (LPARAM)hWndBuddy);
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetToolTips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, TBM_GETTOOLTIPS, 0, 0L));
	}

	void SetToolTips(HWND hWndTT)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETTOOLTIPS, (WPARAM)hWndTT, 0L);
	}

	int SetTipSide(int nSide)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, TBM_SETTIPSIDE, nSide, 0L);
	}
#endif // !_WIN32_WCE

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TBM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, TBM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

// Operations
	void ClearSel(BOOL bRedraw = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_CLEARSEL, bRedraw, 0L);
	}

	void VerifyPos()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_SETPOS, FALSE, 0L);
	}

	void ClearTics(BOOL bRedraw = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TBM_CLEARTICS, bRedraw, 0L);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CUpDownCtrl

class CUpDownCtrl : public TBase
{
public:
// Constructors
	CUpDownCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CUpDownCtrl& operator =(HWND hWnd)
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
		return UPDOWN_CLASS;
	}

	UINT GetAccel(int nAccel, UDACCEL* pAccel) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)LOWORD(::SendMessage(m_hWnd, UDM_GETACCEL, nAccel, (LPARAM)pAccel));
	}

	BOOL SetAccel(int nAccel, UDACCEL* pAccel)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)LOWORD(::SendMessage(m_hWnd, UDM_SETACCEL, nAccel, (LPARAM)pAccel));
	}

	UINT GetBase() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)LOWORD(::SendMessage(m_hWnd, UDM_GETBASE, 0, 0L));
	}

	int SetBase(int nBase)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, UDM_SETBASE, nBase, 0L);
	}

	HWND GetBuddy() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0L);
	}

	HWND SetBuddy(HWND hWndBuddy)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM)hWndBuddy, 0L);
	}

	int GetPos(LPBOOL lpbError = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, UDM_GETPOS, 0, 0L);
		// Note: Seems that Windows always sets error to TRUE if
		// UDS_SETBUDDYINT style is not used
		if(lpbError != NULL)
			*lpbError = (HIWORD(dwRet) != 0) ? TRUE : FALSE;
		return (int)(short)LOWORD(dwRet);
	}

	int SetPos(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)(short)LOWORD(::SendMessage(m_hWnd, UDM_SETPOS, 0, MAKELPARAM(nPos, 0)));
	}

	DWORD GetRange() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
	}

	void GetRange(int& nLower, int& nUpper) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
		nLower = (int)(short)HIWORD(dwRet);
		nUpper = (int)(short)LOWORD(dwRet);
	}

	void SetRange(int nLower, int nUpper)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
	}

#if (_WIN32_IE >= 0x0400)
	void SetRange32(int nLower, int nUpper)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, UDM_SETRANGE32, nLower, nUpper);
	}

	void GetRange32(int& nLower, int& nUpper) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, UDM_GETRANGE32, (WPARAM)&nLower, (LPARAM)&nUpper);
	}

#ifndef _WIN32_WCE
	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, UDM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, UDM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	int GetPos32(LPBOOL lpbError = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		// Note: Seems that Windows always sets error to TRUE if
		// UDS_SETBUDDYINT style is not used
		return (int)::SendMessage(m_hWnd, UDM_GETPOS32, 0, (LPARAM)lpbError);
	}

	int SetPos32(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, UDM_SETPOS32, 0, (LPARAM)nPos);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
};


///////////////////////////////////////////////////////////////////////////////
// CProgressBarCtrl

class CProgressBarCtrl : public TBase
{
public:
// Constructors
	CProgressBarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CProgressBarCtrl& operator =(HWND hWnd)
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
		return PROGRESS_CLASS;
	}

	DWORD SetRange(int nLower, int nUpper)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
	}

	int SetPos(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_SETPOS, nPos, 0L));
	}

	int OffsetPos(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_DELTAPOS, nPos, 0L));
	}

	int SetStep(int nStep)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_SETSTEP, nStep, 0L));
	}

	UINT GetPos() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0L);
	}

	void GetRange(PPBRANGE pPBRange) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(pPBRange != NULL);
		::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)pPBRange);
	}

	void GetRange(int& nLower, int& nUpper) const
	{
		ASSERT(::IsWindow(m_hWnd));
		PBRANGE range = { 0 };
		::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&range);
		nLower = range.iLow;
		nUpper = range.iHigh;
	}

	int GetRangeLimit(BOOL bLowLimit) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PBM_GETRANGE, bLowLimit, (LPARAM)NULL);
	}

	DWORD SetRange32(int nMin, int nMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, PBM_SETRANGE32, nMin, nMax);
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	COLORREF SetBarColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)clr);
	}

	COLORREF SetBkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)clr);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

#if (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
	BOOL SetMarquee(BOOL bMarquee, UINT uUpdateTime = 0U)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, PBM_SETMARQUEE, (WPARAM)bMarquee, (LPARAM)uUpdateTime);
	}
#endif // (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)

// Operations
	int StepIt()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_STEPIT, 0, 0L));
	}
};


///////////////////////////////////////////////////////////////////////////////
// CHotKeyCtrl

#ifndef _WIN32_WCE

class CHotKeyCtrl : public TBase
{
public:
// Constructors
	CHotKeyCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CHotKeyCtrl& operator =(HWND hWnd)
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
		return HOTKEY_CLASS;
	}

	DWORD GetHotKey() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, HKM_GETHOTKEY, 0, 0L);
	}

	void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dw = (DWORD)::SendMessage(m_hWnd, HKM_GETHOTKEY, 0, 0L);
		wVirtualKeyCode = LOBYTE(LOWORD(dw));
		wModifiers = HIBYTE(LOWORD(dw));
	}

	void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, HKM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0L);
	}

	void SetRules(WORD wInvalidComb, WORD wModifiers)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, HKM_SETRULES, wInvalidComb, MAKELPARAM(wModifiers, 0));
	}
};

#endif // !_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CAnimateCtrl

#ifndef _WIN32_WCE

class CAnimateCtrl : public TBase
{
public:
// Constructors
	CAnimateCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CAnimateCtrl& operator =(HWND hWnd)
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
		return ANIMATE_CLASS;
	}

// Operations
	BOOL Open(CStringOrId FileName)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ACM_OPEN, 0, (LPARAM)(LPCTSTR)FileName);
	}

	BOOL Play(UINT nFrom, UINT nTo, UINT nRep)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ACM_PLAY, nRep, MAKELPARAM(nFrom, nTo));
	}

	BOOL Stop()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ACM_STOP, 0, 0L);
	}

	BOOL Close()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ACM_OPEN, 0, 0L);
	}

	BOOL Seek(UINT nTo)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ACM_PLAY, 0, MAKELPARAM(nTo, nTo));
	}
};

#endif // !_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CRichEditCtrl

#ifndef _WIN32_WCE

#ifdef _UNICODE
#if (_RICHEDIT_VER == 0x0100)
#undef RICHEDIT_CLASS
#define RICHEDIT_CLASS	L"RICHEDIT"
#endif // (_RICHEDIT_VER == 0x0100)
#endif //_UNICODE

class CRichEditCtrl : public TBase
{
public:
// Constructors
	CRichEditCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CRichEditCtrl& operator =(HWND hWnd)
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
		return RICHEDIT_CLASS;
	}

	static LPCTSTR GetLibraryName()
	{
#if (_RICHEDIT_VER >= 0x0200)
		return _T("RICHED20.DLL");
#else
		return _T("RICHED32.DLL");
#endif
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

	DWORD GetOptions() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_GETOPTIONS, 0, 0L);
	}

	DWORD SetOptions(WORD wOperation, DWORD dwOptions)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_SETOPTIONS, wOperation, dwOptions);
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
		*(LPINT)lpszBuffer = nMaxLength;
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	BOOL CanUndo() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
	}

	BOOL CanPaste(UINT nFormat = 0) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_CANPASTE, nFormat, 0L);
	}

	void GetSel(LONG& nStartChar, LONG& nEndChar) const
	{
		ASSERT(::IsWindow(m_hWnd));
		CHARRANGE cr = { 0, 0 };
		::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
		nStartChar = cr.cpMin;
		nEndChar = cr.cpMax;
	}

	void GetSel(CHARRANGE &cr) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
	}

	int SetSel(LONG nStartChar, LONG nEndChar)
	{
		ASSERT(::IsWindow(m_hWnd));
		CHARRANGE cr = { nStartChar, nEndChar };
		return (int)::SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	}

	int SetSel(CHARRANGE &cr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	}

	int SetSelAll()
	{
		return SetSel(0, -1);
	}

	int SetSelNone()
	{
		return SetSel(-1, 0);
	}

	DWORD GetDefaultCharFormat(CHARFORMAT& cf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
	}

	DWORD GetSelectionCharFormat(CHARFORMAT& cf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
	}

	DWORD GetEventMask() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0L);
	}

	LONG GetLimitText() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
	}

	DWORD GetParaFormat(PARAFORMAT& pf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		pf.cbSize = sizeof(PARAFORMAT);
		return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	}

#if (_RICHEDIT_VER >= 0x0200)
	LONG GetSelText(LPTSTR lpstrBuff) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrBuff);
	}
#else // !(_RICHEDIT_VER >= 0x0200)
	// RichEdit 1.0 EM_GETSELTEXT is ANSI only
	LONG GetSelText(LPSTR lpstrBuff) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrBuff);
	}
#endif // !(_RICHEDIT_VER >= 0x0200)

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetSelTextBSTR(BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);

		CHARRANGE cr = { 0, 0 };
		::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);

#if (_RICHEDIT_VER >= 0x0200)
		LPTSTR lpstrText = (LPTSTR)_alloca((cr.cpMax - cr.cpMin + 1) * sizeof(TCHAR));
		lpstrText[0] = 0;
		if(::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrText) == 0)
			return FALSE;

		bstrText = ::SysAllocString(T2W(lpstrText));
#else // !(_RICHEDIT_VER >= 0x0200)
		LPSTR lpstrText = (char*)_alloca(cr.cpMax - cr.cpMin + 1);
		lpstrText[0] = 0;
		if(::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrText) == 0)
			return FALSE;

		bstrText = ::SysAllocString(A2W(lpstrText));
#endif // !(_RICHEDIT_VER >= 0x0200)

		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	LONG GetSelText(CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));

		CHARRANGE cr = { 0, 0 };
		::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);

#if (_RICHEDIT_VER >= 0x0200)
		LONG lLen = 0;
		LPTSTR lpstrText = strText.GetBufferSetLength(cr.cpMax - cr.cpMin);
		if(lpstrText != NULL)
		{
			lLen = (LONG)::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrText);
			strText.ReleaseBuffer();
		}
#else // !(_RICHEDIT_VER >= 0x0200)
		LPSTR lpstrText = (char*)_alloca(cr.cpMax - cr.cpMin + 1);
		lpstrText[0] = 0;
		LONG lLen = (LONG)::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrText);
		if(lLen == 0)
			return 0;

		USES_CONVERSION;
		strText = A2T(lpstrText);
#endif // !(_RICHEDIT_VER >= 0x0200)

		return lLen;
	}
#endif // defined(__STR_H__)

	WORD GetSelectionType() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (WORD)::SendMessage(m_hWnd, EM_SELECTIONTYPE, 0, 0L);
	}

	COLORREF SetBackgroundColor(COLORREF cr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 0, cr);
	}

	COLORREF SetBackgroundColor()   // sets to system background
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 1, 0);
	}

	BOOL SetCharFormat(CHARFORMAT& cf, WORD wFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM)wFlags, (LPARAM)&cf);
	}

	BOOL SetDefaultCharFormat(CHARFORMAT& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	BOOL SetSelectionCharFormat(CHARFORMAT& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	}

	BOOL SetWordCharFormat(CHARFORMAT& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
	}

	DWORD SetEventMask(DWORD dwEventMask)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_SETEVENTMASK, 0, dwEventMask);
	}

	BOOL SetParaFormat(PARAFORMAT& pf)
	{
		ASSERT(::IsWindow(m_hWnd));
		pf.cbSize = sizeof(PARAFORMAT);
		return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
	}

	BOOL SetTargetDevice(HDC hDC, int cxLineWidth)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETTARGETDEVICE, (WPARAM)hDC, cxLineWidth);
	}

	int GetTextLength() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0L);
	}

	BOOL SetReadOnly(BOOL bReadOnly = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
	}

	int GetFirstVisibleLine() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
	}

	EDITWORDBREAKPROCEX GetWordBreakProcEx() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (EDITWORDBREAKPROCEX)::SendMessage(m_hWnd, EM_GETWORDBREAKPROCEX, 0, 0L);
	}

	EDITWORDBREAKPROCEX SetWordBreakProcEx(EDITWORDBREAKPROCEX pfnEditWordBreakProcEx)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (EDITWORDBREAKPROCEX)::SendMessage(m_hWnd, EM_SETWORDBREAKPROCEX, 0, (LPARAM)pfnEditWordBreakProcEx);
	}

	int GetTextRange(TEXTRANGE* pTextRange) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)pTextRange);
	}

#if (_RICHEDIT_VER >= 0x0200)
	int GetTextRange(LONG nStartChar, LONG nEndChar, LPTSTR lpstrText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TEXTRANGE tr = { 0 };
		tr.chrg.cpMin = nStartChar;
		tr.chrg.cpMax = nEndChar;
		tr.lpstrText = lpstrText;
		return (int)::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
#else // !(_RICHEDIT_VER >= 0x0200)

	int GetTextRange(LONG nStartChar, LONG nEndChar, LPSTR lpstrText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TEXTRANGE tr = { 0 };
		tr.chrg.cpMin = nStartChar;
		tr.chrg.cpMax = nEndChar;
		tr.lpstrText = lpstrText;
		return (int)::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
#endif // !(_RICHEDIT_VER >= 0x0200)

#if (_RICHEDIT_VER >= 0x0200)
	DWORD GetDefaultCharFormat(CHARFORMAT2& cf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
	}

	BOOL SetCharFormat(CHARFORMAT2& cf, WORD wFlags)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM)wFlags, (LPARAM)&cf);
	}

	BOOL SetDefaultCharFormat(CHARFORMAT2& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	DWORD GetSelectionCharFormat(CHARFORMAT2& cf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
	}

	BOOL SetSelectionCharFormat(CHARFORMAT2& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	}

	BOOL SetWordCharFormat(CHARFORMAT2& cf)
	{
		ASSERT(::IsWindow(m_hWnd));
		cf.cbSize = sizeof(CHARFORMAT2);
		return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
	}

	DWORD GetParaFormat(PARAFORMAT2& pf) const
	{
		ASSERT(::IsWindow(m_hWnd));
		pf.cbSize = sizeof(PARAFORMAT2);
		return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	}

	BOOL SetParaFormat(PARAFORMAT2& pf)
	{
		ASSERT(::IsWindow(m_hWnd));
		pf.cbSize = sizeof(PARAFORMAT2);
		return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
	}

	TEXTMODE GetTextMode() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (TEXTMODE)::SendMessage(m_hWnd, EM_GETTEXTMODE, 0, 0L);
	}

	BOOL SetTextMode(TEXTMODE enumTextMode)
	{
		ASSERT(::IsWindow(m_hWnd));
		return !(BOOL)::SendMessage(m_hWnd, EM_SETTEXTMODE, enumTextMode, 0L);
	}

	UNDONAMEID GetUndoName() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UNDONAMEID)::SendMessage(m_hWnd, EM_GETUNDONAME, 0, 0L);
	}

	UNDONAMEID GetRedoName() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UNDONAMEID)::SendMessage(m_hWnd, EM_GETREDONAME, 0, 0L);
	}

	BOOL CanRedo() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_CANREDO, 0, 0L);
	}

	BOOL GetAutoURLDetect() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_GETAUTOURLDETECT, 0, 0L);
	}

	BOOL SetAutoURLDetect(BOOL bAutoDetect = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return !(BOOL)::SendMessage(m_hWnd, EM_AUTOURLDETECT, bAutoDetect, 0L);
	}

	// this method is deprecated, please use SetAutoURLDetect
	BOOL EnableAutoURLDetect(BOOL bEnable = TRUE) { return SetAutoURLDetect(bEnable); }

	UINT SetUndoLimit(UINT uUndoLimit)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, EM_SETUNDOLIMIT, uUndoLimit, 0L);
	}

	void SetPalette(HPALETTE hPalette)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SETPALETTE, (WPARAM)hPalette, 0L);
	}

	int GetTextEx(GETTEXTEX* pGetTextEx, LPTSTR lpstrText) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM)pGetTextEx, (LPARAM)lpstrText);
	}

	int GetTextEx(LPTSTR lpstrText, int nTextLen, DWORD dwFlags = GT_DEFAULT, UINT uCodePage = CP_ACP, LPCSTR lpDefaultChar = NULL, LPBOOL lpUsedDefChar = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		GETTEXTEX gte = { 0 };
		gte.cb = nTextLen * sizeof(TCHAR);
		gte.codepage = uCodePage;
		gte.flags = dwFlags;
		gte.lpDefaultChar = lpDefaultChar;
		gte.lpUsedDefChar = lpUsedDefChar;
		return (int)::SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM)&gte, (LPARAM)lpstrText);
	}

	int GetTextLengthEx(GETTEXTLENGTHEX* pGetTextLengthEx) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)pGetTextLengthEx, 0L);
	}

	int GetTextLengthEx(DWORD dwFlags = GTL_DEFAULT, UINT uCodePage = CP_ACP) const
	{
		ASSERT(::IsWindow(m_hWnd));
		GETTEXTLENGTHEX gtle = { 0 };
		gtle.codepage = uCodePage;
		gtle.flags = dwFlags;
		return (int)::SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtle, 0L);
	}
#endif // (_RICHEDIT_VER >= 0x0200)

#if (_RICHEDIT_VER >= 0x0300)
	int SetTextEx(SETTEXTEX* pSetTextEx, LPCTSTR lpstrText)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)pSetTextEx, (LPARAM)lpstrText);
	}

	int SetTextEx(LPCTSTR lpstrText, DWORD dwFlags = ST_DEFAULT, UINT uCodePage = CP_ACP)
	{
		ASSERT(::IsWindow(m_hWnd));
		SETTEXTEX ste = { 0 };
		ste.flags = dwFlags;
		ste.codepage = uCodePage;
		return (int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)lpstrText);
	}

	int GetEditStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_GETEDITSTYLE, 0, 0L);
	}

	int SetEditStyle(int nStyle, int nMask = -1)
	{
		ASSERT(::IsWindow(m_hWnd));
		if(nMask == -1)
			nMask = nStyle;   // set everything specified
		return (int)::SendMessage(m_hWnd, EM_SETEDITSTYLE, nStyle, nMask);
	}

	BOOL SetFontSize(int nFontSizeDelta)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nFontSizeDelta >= -1637 && nFontSizeDelta <= 1638);
		return (BOOL)::SendMessage(m_hWnd, EM_SETFONTSIZE, nFontSizeDelta, 0L);
	}

	void GetScrollPos(LPPOINT lpPoint) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpPoint != NULL);
		::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)lpPoint);
	}

	void SetScrollPos(LPPOINT lpPoint)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpPoint != NULL);
		::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)lpPoint);
	}

	BOOL GetZoom(int& nNum, int& nDen) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen);
	}

	BOOL SetZoom(int nNum, int nDen)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nNum >= 0 && nNum <= 64);
		ASSERT(nDen >= 0 && nDen <= 64);
		return (BOOL)::SendMessage(m_hWnd, EM_SETZOOM, nNum, nDen);
	}

	BOOL SetZoomOff()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETZOOM, 0, 0L);
	}
#endif // (_RICHEDIT_VER >= 0x0300)

// Operations
	void LimitText(LONG nChars = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_EXLIMITTEXT, 0, nChars);
	}

	int LineFromChar(LONG nIndex) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, EM_EXLINEFROMCHAR, 0, nIndex);
	}

	POINT PosFromChar(LONG nChar) const
	{
		ASSERT(::IsWindow(m_hWnd));
		POINT point = { 0, 0 };
		::SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM)&point, nChar);
		return point;
	}

	int CharFromPos(POINT pt) const
	{
		ASSERT(::IsWindow(m_hWnd));
		POINTL ptl = { pt.x, pt.y };
		return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptl);
	}

	void EmptyUndoBuffer()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
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

	BOOL LineScroll(int nLines, int nChars = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
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

	BOOL DisplayBand(LPRECT pDisplayRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_DISPLAYBAND, 0, (LPARAM)pDisplayRect);
	}

	LONG FindText(DWORD dwFlags, FINDTEXT& ft) const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_RICHEDIT_VER >= 0x0200) && defined(_UNICODE)
		return (LONG)::SendMessage(m_hWnd, EM_FINDTEXTW, dwFlags, (LPARAM)&ft);
#else
		return (LONG)::SendMessage(m_hWnd, EM_FINDTEXT, dwFlags, (LPARAM)&ft);
#endif
	}

	LONG FindText(DWORD dwFlags, FINDTEXTEX& ft) const
	{
		ASSERT(::IsWindow(m_hWnd));
#if (_RICHEDIT_VER >= 0x0200) && defined(_UNICODE)
		return (LONG)::SendMessage(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)&ft);
#else
		return (LONG)::SendMessage(m_hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM)&ft);
#endif
	}

	LONG FormatRange(FORMATRANGE& fr, BOOL bDisplay = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM)&fr);
	}

	LONG FormatRange(FORMATRANGE* pFormatRange, BOOL bDisplay = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM)pFormatRange);
	}

	void HideSelection(BOOL bHide = TRUE, BOOL bChangeStyle = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_HIDESELECTION, bHide, bChangeStyle);
	}

	void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		REPASTESPECIAL reps = { dwAspect, (DWORD_PTR)hMF };
		::SendMessage(m_hWnd, EM_PASTESPECIAL, uClipFormat, (LPARAM)&reps);
	}

	void RequestResize()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_REQUESTRESIZE, 0, 0L);
	}

	LONG StreamIn(UINT uFormat, EDITSTREAM& es)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_STREAMIN, uFormat, (LPARAM)&es);
	}

	LONG StreamOut(UINT uFormat, EDITSTREAM& es)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (LONG)::SendMessage(m_hWnd, EM_STREAMOUT, uFormat, (LPARAM)&es);
	}

	DWORD FindWordBreak(int nCode, LONG nStartChar)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, EM_FINDWORDBREAK, nCode, nStartChar);
	}

	// Additional operations
	void ScrollCaret()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}

	int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, BOOL bCanUndo = FALSE)
	{
		int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
		ReplaceSel(lpstrText, bCanUndo);
		return nRet;
	}

	int AppendText(LPCTSTR lpstrText, BOOL bCanUndo = FALSE)
	{
		return InsertText(GetWindowTextLength(), lpstrText, bCanUndo);
	}

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

	// OLE support
	IRichEditOle* GetOleInterface() const
	{
		ASSERT(::IsWindow(m_hWnd));
		IRichEditOle *pRichEditOle = NULL;
		::SendMessage(m_hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle);
		return pRichEditOle;
	}

	BOOL SetOleCallback(IRichEditOleCallback* pCallback)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)pCallback);
	}

#if (_RICHEDIT_VER >= 0x0200)
	BOOL Redo()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, EM_REDO, 0, 0L);
	}

	void StopGroupTyping()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_STOPGROUPTYPING, 0, 0L);
	}

	void ShowScrollBar(int nBarType, BOOL bVisible = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, EM_SHOWSCROLLBAR, nBarType, bVisible);
	}
#endif // (_RICHEDIT_VER >= 0x0200)

#if (_RICHEDIT_VER >= 0x0300)
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
#endif // (_RICHEDIT_VER >= 0x0300)
};

#endif // !_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CDragListBox

#ifndef _WIN32_WCE

class CDragListBox : public CListBox
{
public:
// Constructors
	CDragListBox(HWND hWnd = NULL) : CListBox(hWnd)
	{ }

	CDragListBox& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, PRECT prc = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			CMenuOrId MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		TBase::Create(dwExStyle, szWindowName, dwStyle, prc, hWndParent, MenuOrID, NULL, lpCreateParam);
		if(m_hWnd != NULL)
			MakeDragList();
		return m_hWnd;
	}

// Operations
	BOOL MakeDragList()
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
		return ::MakeDragList(m_hWnd);
	}

	int LBItemFromPt(POINT pt, BOOL bAutoScroll = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return ::LBItemFromPt(m_hWnd, pt, bAutoScroll);
	}

	void DrawInsert(int nItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		::DrawInsert(GetParent(), m_hWnd, nItem);
	}

	static UINT GetDragListMessage()
	{
		static UINT uDragListMessage = 0;
		if(uDragListMessage == 0)
			uDragListMessage = ::RegisterWindowMessage(DRAGLISTMSGSTRING);

		ASSERT(uDragListMessage != 0);
		return uDragListMessage;
	}
};

#endif // _WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CReBarCtrl

class CReBarCtrl : public TBase
{
public:
// Constructors
	CReBarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CReBarCtrl& operator =(HWND hWnd)
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
		return REBARCLASSNAME;
	}

	UINT GetBandCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0L);
	}

	BOOL GetBandInfo(int nBand, LPREBARBANDINFO lprbbi) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_GETBANDINFO, nBand, (LPARAM)lprbbi);
	}

	BOOL SetBandInfo(int nBand, LPREBARBANDINFO lprbbi)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)lprbbi);
	}

	BOOL GetBarInfo(LPREBARINFO lprbi) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)lprbi);
	}

	BOOL SetBarInfo(LPREBARINFO lprbi)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)lprbi);
	}

	CImageList GetImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		REBARINFO rbi = { 0 };
		rbi.cbSize = sizeof(REBARINFO);
		rbi.fMask = RBIM_IMAGELIST;
		if( (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)&rbi) == FALSE ) return CImageList();
		return CImageList(rbi.himl);
	}

	BOOL SetImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		REBARINFO rbi = { 0 };
		rbi.cbSize = sizeof(REBARINFO);
		rbi.fMask = RBIM_IMAGELIST;
		rbi.himl = hImageList;
		return (BOOL)::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)&rbi);
	}

	UINT GetRowCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, RB_GETROWCOUNT, 0, 0L);
	}

	UINT GetRowHeight(int nBand) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, RB_GETROWHEIGHT, nBand, 0L);
	}

#if (_WIN32_IE >= 0x0400)
	COLORREF GetTextColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, RB_GETTEXTCOLOR, 0, 0L);
	}

	COLORREF SetTextColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, RB_SETTEXTCOLOR, 0, (LPARAM)clr);
	}

	COLORREF GetBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, RB_GETBKCOLOR, 0, 0L);
	}

	COLORREF SetBkColor(COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, RB_SETBKCOLOR, 0, (LPARAM)clr);
	}

	UINT GetBarHeight() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)::SendMessage(m_hWnd, RB_GETBARHEIGHT, 0, 0L);
	}

	BOOL GetRect(int nBand, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_GETRECT, nBand, (LPARAM)lpRect);
	}

#ifndef _WIN32_WCE
	CToolTipCtrl GetToolTips() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CToolTipCtrl((HWND)::SendMessage(m_hWnd, RB_GETTOOLTIPS, 0, 0L));
	}

	void SetToolTips(HWND hwndToolTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_SETTOOLTIPS, (WPARAM)hwndToolTip, 0L);
	}
#endif // !_WIN32_WCE

	void GetBandBorders(int nBand, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpRect != NULL);
		::SendMessage(m_hWnd, RB_GETBANDBORDERS, nBand, (LPARAM)lpRect);
	}

#ifndef _WIN32_WCE
	BOOL GetColorScheme(LPCOLORSCHEME lpColorScheme) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpColorScheme != NULL);
		return (BOOL)::SendMessage(m_hWnd, RB_GETCOLORSCHEME, 0, (LPARAM)lpColorScheme);
	}

	void SetColorScheme(LPCOLORSCHEME lpColorScheme)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpColorScheme != NULL);
		::SendMessage(m_hWnd, RB_SETCOLORSCHEME, 0, (LPARAM)lpColorScheme);
	}

	HPALETTE GetPalette() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HPALETTE)::SendMessage(m_hWnd, RB_GETPALETTE, 0, 0L);
	}

	HPALETTE SetPalette(HPALETTE hPalette)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HPALETTE)::SendMessage(m_hWnd, RB_SETPALETTE, 0, (LPARAM)hPalette);
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_WINNT >= 0x0501)
	// requires uxtheme.h to be included to use MARGINS struct
#ifndef _UXTHEME_H_
	typedef struct _MARGINS*   PMARGINS;
#endif // !_UXTHEME_H_
	void GetBandMargins(PMARGINS pMargins) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_GETBANDMARGINS, 0, (LPARAM)pMargins);
	}

	void SetWindowTheme(LPCWSTR lpstrTheme)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	BOOL InsertBand(int nBand, LPREBARBANDINFO lprbbi)
	{
		ASSERT(::IsWindow(m_hWnd));
		lprbbi->cbSize = sizeof(REBARBANDINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)lprbbi);
	}

	BOOL DeleteBand(int nBand)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_DELETEBAND, nBand, 0L);
	}

	HWND SetNotifyWnd(HWND hWnd)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, RB_SETPARENT, (WPARAM)hWnd, 0L);
	}

#if (_WIN32_IE >= 0x0400)
	void BeginDrag(int nBand, DWORD dwPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_BEGINDRAG, nBand, dwPos);
	}

	void BeginDrag(int nBand, int xPos, int yPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_BEGINDRAG, nBand, MAKELPARAM(xPos, yPos));
	}

	void EndDrag()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_ENDDRAG, 0, 0L);
	}

	void DragMove(DWORD dwPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_DRAGMOVE, 0, dwPos);
	}

	void DragMove(int xPos, int yPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_DRAGMOVE, 0, MAKELPARAM(xPos, yPos));
	}

#ifndef _WIN32_WCE
	void GetDropTarget(IDropTarget** ppDropTarget) const
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_GETDROPTARGET, 0, (LPARAM)ppDropTarget);
	}
#endif // !_WIN32_WCE

	void MaximizeBand(int nBand, BOOL bIdeal = FALSE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_MAXIMIZEBAND, nBand, bIdeal);
	}

	void MinimizeBand(int nBand)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_MINIMIZEBAND, nBand, 0L);
	}

	BOOL SizeToRect(LPRECT lpRect)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_SIZETORECT, 0, (LPARAM)lpRect);
	}

	int IdToIndex(UINT uBandID) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, RB_IDTOINDEX, uBandID, 0L);
	}

	int HitTest(LPRBHITTESTINFO lprbht) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, RB_HITTEST, 0, (LPARAM)lprbht);
	}

	BOOL ShowBand(int nBand, BOOL bShow)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, RB_SHOWBAND, nBand, bShow);
	}

#ifndef _WIN32_WCE
	BOOL MoveBand(int nBand, int nNewPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nNewPos >= 0 && nNewPos <= ((int)GetBandCount() - 1));
		return (BOOL)::SendMessage(m_hWnd, RB_MOVEBAND, nBand, nNewPos);
	}
#endif // !_WIN32_WCE
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	void PushChevron(int nBand, LPARAM lAppValue)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, RB_PUSHCHEVRON, nBand, lAppValue);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

// Extra operations
#if (_WIN32_IE >= 0x0400)
	void LockBands(bool bLock)
	{
		int nBandCount = GetBandCount();
		for(int i =0; i < nBandCount; i++)
		{
			REBARBANDINFO rbbi = { 0 };
			rbbi.cbSize = sizeof(REBARBANDINFO);
			rbbi.fMask = RBBIM_STYLE;
			BOOL bRet = GetBandInfo(i, &rbbi);
			ASSERT(bRet);

			if((rbbi.fStyle & RBBS_GRIPPERALWAYS) == 0)
			{
				rbbi.fStyle |= RBBS_GRIPPERALWAYS;
				bRet = SetBandInfo(i, &rbbi);
				ASSERT(bRet);
				rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
			}

			if(bLock)
				rbbi.fStyle |= RBBS_NOGRIPPER;
			else
				rbbi.fStyle &= ~RBBS_NOGRIPPER;

			bRet = SetBandInfo(i, &rbbi);
			ASSERT(bRet);
		}
	}
#endif // (_WIN32_IE >= 0x0400)
};


///////////////////////////////////////////////////////////////////////////////
// CComboBoxEx

#ifndef _WIN32_WCE

class CComboBoxEx : public CComboBox
{
public:
// Constructors
	CComboBoxEx(HWND hWnd = NULL) : CComboBox(hWnd)
	{ }

	CComboBoxEx& operator =(HWND hWnd)
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
		return WC_COMBOBOXEX;
	}

	CImageList GetImageList() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0L));
	}

	CImageList SetImageList(HIMAGELIST hImageList)
	{
		ASSERT(::IsWindow(m_hWnd));
		return CImageList((HIMAGELIST)::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)hImageList));
	}

#if (_WIN32_IE >= 0x0400)
	DWORD GetExtendedStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0L);
	}

	DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyle)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
	}

	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CBEM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CBEM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_WINNT >= 0x0501)
	void SetWindowTheme(LPCWSTR lpstrTheme)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, CBEM_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
	}
#endif // (_WIN32_WINNT >= 0x0501)

// Operations
	int InsertItem(const COMBOBOXEXITEM FAR* lpcCBItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM)lpcCBItem);
	}

	int InsertItem(UINT nMask, int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, 
		int iIndent, int iOverlay, DWORD lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = nMask;
		cbex.iItem = nIndex;
		cbex.pszText = (LPTSTR) lpszItem;
		cbex.iImage = nImage;
		cbex.iSelectedImage = nSelImage;
		cbex.iIndent = iIndent;
		cbex.iOverlay = iOverlay;
		cbex.lParam = lParam;
		return (int)::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM)&cbex);
	}

	int InsertItem(int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, DWORD lParam = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT | CBEIF_LPARAM;
		cbex.iItem = nIndex;
		cbex.pszText = (LPTSTR) lpszItem;
		cbex.iImage = nImage;
		cbex.iSelectedImage = nSelImage;
		cbex.iIndent = iIndent;
		cbex.lParam = lParam;
		return (int)::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM)&cbex);
	}

	int DeleteItem(int nIndex)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, CBEM_DELETEITEM, nIndex, 0L);
	}

	BOOL GetItem(PCOMBOBOXEXITEM pCBItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM)pCBItem);
	}

	BOOL SetItem(const COMBOBOXEXITEM FAR* lpcCBItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM)lpcCBItem);
	}

	int SetItem(int nIndex, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelImage, 
		int iIndent, int iOverlay, DWORD lParam)
	{
		ASSERT(::IsWindow(m_hWnd));
		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = nMask;
		cbex.iItem = nIndex;
		cbex.pszText = (LPTSTR) lpszItem;
		cbex.iImage = nImage;
		cbex.iSelectedImage = nSelImage;
		cbex.iIndent = iIndent;
		cbex.iOverlay = iOverlay;
		cbex.lParam = lParam;
		return (int)::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM)&cbex);
	}

	BOOL GetItemText(int nIndex, LPTSTR lpszItem, int nLen) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(lpszItem != NULL);

		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = CBEIF_TEXT;
		cbex.iItem = nIndex;
		cbex.pszText = lpszItem;
		cbex.cchTextMax = nLen;

		return (BOOL)::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM)&cbex);
	}

#ifndef _TWL_NO_COM
#ifdef __STRCONV_H__
	BOOL GetItemText(int nIndex, BSTR& bstrText) const
	{
		USES_CONVERSION;
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(bstrText == NULL);

		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = CBEIF_TEXT;
		cbex.iItem = nIndex;

		LPTSTR lpstrText = NULL;
		BOOL bRet = FALSE;
		for(int nLen = 256; ; nLen *= 2)
		{
			lpstrText = new TCHAR[nLen];
			if(lpstrText == NULL)
				break;
			lpstrText[0] = NULL;
			cbex.pszText = lpstrText;
			cbex.cchTextMax = nLen;
			bRet = (BOOL)::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM)&cbex);
			if(!bRet || (lstrlen(cbex.pszText) < nLen - 1))
				break;
			delete [] lpstrText;
			lpstrText = NULL;
		}

		if(lpstrText != NULL)
		{
			if(bRet)
				bstrText = ::SysAllocString(T2OLE(lpstrText));
			delete [] lpstrText;
		}

		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // __STRCONV_H__
#endif // !_TWL_NO_COM

#if defined(__STR_H__)
	BOOL GetItemText(int nIndex, CString& strText) const
	{
		ASSERT(::IsWindow(m_hWnd));

		COMBOBOXEXITEM cbex = { 0 };
		cbex.mask = CBEIF_TEXT;
		cbex.iItem = nIndex;

		strText.Empty();
		BOOL bRet = FALSE;
		for(int nLen = 256; ; nLen *= 2)
		{
			cbex.pszText = strText.GetBufferSetLength(nLen);
			if(cbex.pszText == NULL)
			{
				bRet = FALSE;
				break;
			}
			cbex.cchTextMax = nLen;
			bRet = (BOOL)::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM)&cbex);
			if(!bRet || (lstrlen(cbex.pszText) < nLen - 1))
				break;
		}
		strText.ReleaseBuffer();
		return bRet;
	}
#endif // defined(__STR_H__)

	BOOL SetItemText(int nIndex, LPCTSTR lpszItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return SetItem(nIndex, CBEIF_TEXT, lpszItem, 0, 0, 0, 0, 0);
	}

	CComboBox GetComboCtrl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CComboBox((HWND)::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0L));
	}

#if 0
	HWND GetEditCtrl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0L);
	}
#else
	CEdit GetEditCtrl() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CEdit((HWND)::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0L));
	}
#endif

	BOOL HasEditChanged() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0L);
	}

// Non-functional
	int AddString(LPCTSTR /*lpszItem*/)
	{
		ASSERT(FALSE);  // Not available in CComboBoxEx; use InsertItem
		return 0;
	}

	int InsertString(int /*nIndex*/, LPCTSTR /*lpszString*/)
	{
		ASSERT(FALSE);  // Not available in CComboBoxEx; use InsertItem
		return 0;
	}

	int Dir(UINT /*attr*/, LPCTSTR /*lpszWildCard*/)
	{
		ASSERT(FALSE);  // Not available in CComboBoxEx
		return 0;
	}

	int FindString(int /*nStartAfter*/, LPCTSTR /*lpszString*/) const
	{
		ASSERT(FALSE);  // Not available in CComboBoxEx; try FindStringExact
		return 0;
	}
};

#endif // !_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CMonthCalendarCtrl

class CMonthCalendarCtrl : public TBase
{
public:
// Constructors
	CMonthCalendarCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CMonthCalendarCtrl& operator =(HWND hWnd)
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
		return MONTHCAL_CLASS;
	}

	COLORREF GetColor(int nColorType) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, MCM_GETCOLOR, nColorType, 0L);
	}

	COLORREF SetColor(int nColorType, COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, MCM_SETCOLOR, nColorType, clr);
	}

	BOOL GetCurSel(LPSYSTEMTIME lpSysTime) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM)lpSysTime);
	}

	BOOL SetCurSel(LPSYSTEMTIME lpSysTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM)lpSysTime);
	}

	int GetFirstDayOfWeek(BOOL* pbLocaleVal = NULL) const
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, MCM_GETFIRSTDAYOFWEEK, 0, 0L);
		if(pbLocaleVal != NULL)
			*pbLocaleVal = (BOOL)HIWORD(dwRet);
		return (int)(short)LOWORD(dwRet);
	}

	int SetFirstDayOfWeek(int nDay, BOOL* pbLocaleVal = NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, MCM_SETFIRSTDAYOFWEEK, 0, nDay);
		if(pbLocaleVal != NULL)
			*pbLocaleVal = (BOOL)HIWORD(dwRet);
		return (int)(short)LOWORD(dwRet);
	}

	int GetMaxSelCount() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, MCM_GETMAXSELCOUNT, 0, 0L);
	}

	BOOL SetMaxSelCount(int nMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETMAXSELCOUNT, nMax, 0L);
	}

	int GetMonthDelta() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, MCM_GETMONTHDELTA, 0, 0L);
	}

	int SetMonthDelta(int nDelta)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, MCM_SETMONTHDELTA, nDelta, 0L);
	}

	DWORD GetRange(LPSYSTEMTIME lprgSysTimeArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, MCM_GETRANGE, 0, (LPARAM)lprgSysTimeArray);
	}

	BOOL SetRange(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETRANGE, dwFlags, (LPARAM)lprgSysTimeArray);
	}

	BOOL GetSelRange(LPSYSTEMTIME lprgSysTimeArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_GETSELRANGE, 0, (LPARAM)lprgSysTimeArray);
	}

	BOOL SetSelRange(LPSYSTEMTIME lprgSysTimeArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETSELRANGE, 0, (LPARAM)lprgSysTimeArray);
	}

	BOOL GetToday(LPSYSTEMTIME lpSysTime) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM)lpSysTime);
	}

	void SetToday(LPSYSTEMTIME lpSysTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, MCM_SETTODAY, 0, (LPARAM)lpSysTime);
	}

	BOOL GetMinReqRect(LPRECT lpRectInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM)lpRectInfo);
	}

	int GetMaxTodayWidth() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, MCM_GETMAXTODAYWIDTH, 0, 0L);
	}

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
	BOOL GetUnicodeFormat() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_GETUNICODEFORMAT, 0, 0L);
	}

	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETUNICODEFORMAT, bUnicode, 0L);
	}
#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

// Operations
	int GetMonthRange(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, MCM_GETMONTHRANGE, dwFlags, (LPARAM)lprgSysTimeArray);
	}

	BOOL SetDayState(int nMonths, LPMONTHDAYSTATE lpDayStateArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, MCM_SETDAYSTATE, nMonths, (LPARAM)lpDayStateArray);
	}

	DWORD HitTest(PMCHITTESTINFO pMCHitTest) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, MCM_HITTEST, 0, (LPARAM)pMCHitTest);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CDateTimePickerCtrl

class CDateTimePickerCtrl : public TBase
{
public:
// Constructors
	CDateTimePickerCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CDateTimePickerCtrl& operator =(HWND hWnd)
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

// Operations
	LPCTSTR GetClassName()
	{
		return DATETIMEPICK_CLASS;
	}

	BOOL SetFormat(LPCTSTR lpszFormat)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, DTM_SETFORMAT, 0, (LPARAM)lpszFormat);
	}

	COLORREF GetMonthCalColor(int nColorType) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, DTM_GETMCCOLOR, nColorType, 0L);
	}

	COLORREF SetMonthCalColor(int nColorType, COLORREF clr)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, DTM_SETMCCOLOR, nColorType, clr);
	}

	DWORD GetRange(LPSYSTEMTIME lpSysTimeArray) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, DTM_GETRANGE, 0, (LPARAM)lpSysTimeArray);
	}

	BOOL SetRange(DWORD dwFlags, LPSYSTEMTIME lpSysTimeArray)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, DTM_SETRANGE, dwFlags, (LPARAM)lpSysTimeArray);
	}

	DWORD GetSystemTime(LPSYSTEMTIME lpSysTime) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)lpSysTime);
	}

	BOOL SetSystemTime(DWORD dwFlags, LPSYSTEMTIME lpSysTime)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, dwFlags, (LPARAM)lpSysTime);
	}

	CMonthCalendarCtrl GetMonthCal() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return CMonthCalendarCtrl((HWND)::SendMessage(m_hWnd, DTM_GETMONTHCAL, 0, 0L));
	}

#if (_WIN32_IE >= 0x0400)
	HFONT GetMonthCalFont() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, DTM_GETMCFONT, 0, 0L);
	}

	void SetMonthCalFont(HFONT hFont, BOOL bRedraw = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, DTM_SETMCFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}
#endif // (_WIN32_IE >= 0x0400)
};


///////////////////////////////////////////////////////////////////////////////
// CIPAddressCtrl

#if (_WIN32_IE >= 0x0400)

class CIPAddressCtrl : public TBase
{
public:
// Constructors
	CIPAddressCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CIPAddressCtrl& operator =(HWND hWnd)
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

// Atteributes
	LPCTSTR GetClassName()
	{
		return WC_IPADDRESS;
	}

	BOOL IsBlank() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, IPM_ISBLANK, 0, 0L);
	}

	int GetAddress(LPDWORD lpdwAddress) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, IPM_GETADDRESS, 0, (LPARAM)lpdwAddress);
	}

	void SetAddress(DWORD dwAddress)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, IPM_SETADDRESS, 0, dwAddress);
	}

	void ClearAddress()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, IPM_CLEARADDRESS, 0, 0L);
	}

	void SetRange(int nField, WORD wRange)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, IPM_SETRANGE, nField, wRange);
	}

	void SetRange(int nField, BYTE nMin, BYTE nMax)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, IPM_SETRANGE, nField, MAKEIPRANGE(nMin, nMax));
	}

	void SetFocus(int nField)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, IPM_SETFOCUS, nField, 0L);
	}
};

#endif // (_WIN32_IE >= 0x0400)


///////////////////////////////////////////////////////////////////////////////
// CPagerCtrl

#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)

class CPagerCtrl : public TBase
{
public:
// Constructors
	CPagerCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CPagerCtrl& operator =(HWND hWnd)
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
		return WC_PAGESCROLLER;
	}

	int GetButtonSize() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_GETBUTTONSIZE, 0, 0L);
	}

	int SetButtonSize(int nButtonSize)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_SETBUTTONSIZE, 0, nButtonSize);
	}

	DWORD GetButtonState(int nButton) const
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nButton == PGB_TOPORLEFT || nButton == PGB_BOTTOMORRIGHT);
		return (DWORD)::SendMessage(m_hWnd, PGM_GETBUTTONSTATE, 0, nButton);
	}

	COLORREF GetBkColor() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, PGM_GETBKCOLOR, 0, 0L);
	}

	COLORREF SetBkColor(COLORREF clrBk)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (COLORREF)::SendMessage(m_hWnd, PGM_SETBKCOLOR, 0, (LPARAM)clrBk);
	}

	int GetBorder() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_GETBORDER, 0, 0L);
	}

	int SetBorder(int nBorderSize)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_SETBORDER, 0, nBorderSize);
	}

	int GetPos() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_GETPOS, 0, 0L);
	}

	int SetPos(int nPos)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, PGM_SETPOS, 0, nPos);
	}

// Operations
	void SetChild(HWND hWndChild)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, PGM_SETCHILD, 0, (LPARAM)hWndChild);
	}

	void ForwardMouse(BOOL bForward = TRUE)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, PGM_FORWARDMOUSE, bForward, 0L);
	}

	void RecalcSize()
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, PGM_RECALCSIZE, 0, 0L);
	}

	void GetDropTarget(IDropTarget** ppDropTarget)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(ppDropTarget != NULL);
		::SendMessage(m_hWnd, PGM_GETDROPTARGET, 0, (LPARAM)ppDropTarget);
	}
};

#endif // (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)


///////////////////////////////////////////////////////////////////////////////
// CLinkCtrl - Windows SYSLINK control

#if (_WIN32_WINNT >= 0x0501) && !defined(_WIN32_WCE)

class CLinkCtrl : public TBase
{
public:
// Constructors
	CLinkCtrl(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CLinkCtrl& operator =(HWND hWnd)
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
#ifdef _UNICODE
		return WC_LINK;
#else // !_UNICODE
		return "SysLink";
#endif // !_UNICODE
	}

	int GetIdealHeight() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LM_GETIDEALHEIGHT, 0, 0L);
	}

	BOOL GetItem(PLITEM pLItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LM_GETITEM, 0, (LPARAM)pLItem);
	}

	BOOL SetItem(PLITEM pLItem)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LM_SETITEM, 0, (LPARAM)pLItem);
	}

// Operations
	BOOL HitTest(PLHITTESTINFO pLHitTestInfo) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LM_HITTEST, 0, (LPARAM)pLHitTestInfo);
	}
};

#endif // (_WIN32_WINNT >= 0x0501) && !defined(_WIN32_WCE)

}; // namespace TWL

#endif // __CMNCTL_H__
