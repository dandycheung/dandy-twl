//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __GDI_H__
#define __GDI_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <windows.h>
#include <tchar.h>

// For abs()
#include <stdlib.h>

#include "resutil.h"
#include "trcdbg.h"

// protect class members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef CopyRgn
#undef CreateBrush
#undef CreatePen
#undef SelectBrush
#undef SelectPen
#undef SelectFont
#undef SelectBitmap
#endif //_INC_WINDOWSX

// required libraries
#ifndef _NO_MSIMG
#pragma comment(lib, "msimg32.lib")
#endif //!_NO_MSIMG

/////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CPen
// CBrush
// CFont
// CBitmap
// CPalette
// CRgn
// CDC
// CPaintDC
// CClientDC
// CWindowDC
// CEnhMetaFileInfo
// CEnhMetaFile
// CEnhMetaFileDC

// NOTE: All dtors' behavior is more like WTL than MFC except that you call
// any creating function explicitly.

namespace TWL
{

/////////////////////////////////////////////////////////////////////////////
// CPen

class CPen
{
public:
// Data members
	HPEN m_hPen;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CPen(HPEN hPen = NULL) : m_hPen(hPen)
	{
		m_bManaged = FALSE;
	}

	~CPen()
	{
		if(m_bManaged && m_hPen != NULL)
			DeleteObject();
	}

	CPen& operator=(HPEN hPen)
	{
		m_hPen = hPen;
		return *this;
	}

	void Attach(HPEN hPen)
	{
		if(m_bManaged && m_hPen != NULL)
			::DeleteObject(m_hPen);
		m_hPen = hPen;
	}

	HPEN Detach()
	{
		HPEN hPen = m_hPen;
		m_hPen = NULL;
		return hPen;
	}

	operator HPEN() const { return m_hPen; }

	bool IsNull() const { return (m_hPen == NULL); }

// Create methods
	HPEN CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
	{
		ASSERT(m_hPen == NULL);
		m_hPen = ::CreatePen(nPenStyle, nWidth, crColor);
		m_bManaged = TRUE;
		return m_hPen;
	}

	HPEN CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL)
	{
		ASSERT(m_hPen == NULL);
		m_hPen = ::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle);
		m_bManaged = TRUE;
		return m_hPen;
	}

	HPEN CreatePenIndirect(LPLOGPEN lpLogPen)
	{
		ASSERT(m_hPen == NULL);
		m_hPen = ::CreatePenIndirect(lpLogPen);
		m_bManaged = TRUE;
		return m_hPen;
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hPen != NULL);
		if(m_hPen == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hPen);
		if(bRet)
			m_hPen = NULL;
		return bRet;
	}

// Attributes
	int GetLogPen(LOGPEN* pLogPen) const
	{
		ASSERT(m_hPen != NULL);
		return ::GetObject(m_hPen, sizeof(LOGPEN), pLogPen);
	}
	bool GetLogPen(LOGPEN& LogPen) const
	{
		ASSERT(m_hPen != NULL);
		return (::GetObject(m_hPen, sizeof(LOGPEN), &LogPen) == sizeof(LOGPEN));
	}
	int GetExtLogPen(EXTLOGPEN* pLogPen) const
	{
		ASSERT(m_hPen != NULL);
		return ::GetObject(m_hPen, sizeof(EXTLOGPEN), pLogPen);
	}

	bool GetExtLogPen(EXTLOGPEN& ExtLogPen) const
	{
		ASSERT(m_hPen != NULL);
		return (::GetObject(m_hPen, sizeof(EXTLOGPEN), &ExtLogPen) == sizeof(EXTLOGPEN));
	}
};


/////////////////////////////////////////////////////////////////////////////
// CBrush

class CBrush
{
public:
// Data members
	HBRUSH m_hBrush;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CBrush(HBRUSH hBrush = NULL) : m_hBrush(hBrush)
	{
		m_bManaged = FALSE;
	}

	~CBrush()
	{
		if(m_bManaged && m_hBrush != NULL)
			DeleteObject();
	}

	CBrush& operator=(HBRUSH hBrush)
	{
		m_hBrush = hBrush;
		return *this;
	}

	void Attach(HBRUSH hBrush)
	{
		if(m_bManaged && m_hBrush != NULL)
			::DeleteObject(m_hBrush);
		m_hBrush = hBrush;
	}

	HBRUSH Detach()
	{
		HBRUSH hBrush = m_hBrush;
		m_hBrush = NULL;
		return hBrush;
	}

	operator HBRUSH() const { return m_hBrush; }

	bool IsNull() const { return (m_hBrush == NULL); }

// Create methods
	HBRUSH CreateSolidBrush(COLORREF crColor)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::CreateSolidBrush(crColor);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreateHatchBrush(int nIndex, COLORREF crColor)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::CreateHatchBrush(nIndex, crColor);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::CreateBrushIndirect(lpLogBrush);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreatePatternBrush(HBITMAP hBitmap)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::CreatePatternBrush(hBitmap);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
	{
		ASSERT(hPackedDIB != NULL);
		const void* lpPackedDIB = ::GlobalLock(hPackedDIB);
		ASSERT(lpPackedDIB != NULL);
		m_hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
		::GlobalUnlock(hPackedDIB);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	HBRUSH CreateSysColorBrush(int nIndex)
	{
		ASSERT(m_hBrush == NULL);
		m_hBrush = ::GetSysColorBrush(nIndex);
		m_bManaged = TRUE;
		return m_hBrush;
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hBrush != NULL);
		if(m_hBrush == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hBrush);
		if(bRet)
			m_hBrush = NULL;
		return bRet;
	}

// Attributes
	int GetLogBrush(LOGBRUSH* pLogBrush) const
	{
		ASSERT(m_hBrush != NULL);
		return ::GetObject(m_hBrush, sizeof(LOGBRUSH), pLogBrush);
	}
	bool GetLogBrush(LOGBRUSH& LogBrush) const
	{
		ASSERT(m_hBrush != NULL);
		return (::GetObject(m_hBrush, sizeof(LOGBRUSH), &LogBrush) == sizeof(LOGBRUSH));
	}
};

/////////////////////////////////////////////////////////////////////////////
// CFont

class CFont
{
public:
// Data members
	HFONT m_hFont;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CFont(HFONT hFont = NULL) : m_hFont(hFont)
	{
		m_bManaged = FALSE;
	}

	~CFont()
	{
		if(m_bManaged && m_hFont != NULL)
			DeleteObject();
	}

	CFont& operator=(HFONT hFont)
	{
		m_hFont = hFont;
		return *this;
	}

	void Attach(HFONT hFont)
	{
		if(m_bManaged && m_hFont != NULL)
			::DeleteObject(m_hFont);
		m_hFont = hFont;
	}

	HFONT Detach()
	{
		HFONT hFont = m_hFont;
		m_hFont = NULL;
		return hFont;
	}

	operator HFONT() const { return m_hFont; }

	bool IsNull() const { return (m_hFont == NULL); }

// Create methods
	HFONT CreateFontIndirect(const LOGFONT* lpLogFont)
	{
		ASSERT(m_hFont == NULL);
		m_hFont = ::CreateFontIndirect(lpLogFont);
		m_bManaged = TRUE;
		return m_hFont;
	}

#if (_WIN32_WINNT >= 0x0500)
	HFONT CreateFontIndirectEx(CONST ENUMLOGFONTEXDV* penumlfex)
	{
		ASSERT(m_hFont == NULL);
		m_hFont = ::CreateFontIndirectEx(penumlfex);
		m_bManaged = TRUE;
		return m_hFont;
	}
#endif //(_WIN32_WINNT >= 0x0500)

	HFONT CreateFont(int nHeight, int nWidth, int nEscapement,
			int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
			BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
			BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
			LPCTSTR lpszFacename)
	{
		ASSERT(m_hFont == NULL);
		m_hFont = ::CreateFont(nHeight, nWidth, nEscapement,
			nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
			nCharSet, nOutPrecision, nClipPrecision, nQuality,
			nPitchAndFamily, lpszFacename);
		m_bManaged = TRUE;
		return m_hFont;
	}

	HFONT CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC = NULL)
	{
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(LOGFONT));
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = nPointSize;
		lstrcpyn(logFont.lfFaceName, lpszFaceName, sizeof(logFont.lfFaceName)/sizeof(TCHAR));
		return CreatePointFontIndirect(&logFont, hDC);
	}

	HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hDC = NULL)
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);

		// convert nPointSize to logical units based on hDC
		LOGFONT logFont = *lpLogFont;
		POINT pt;
		pt.y = ::GetDeviceCaps(hDC1, LOGPIXELSY) * logFont.lfHeight;
		pt.y /= 720;    // 72 points/inch, 10 decipoints/point
		::DPtoLP(hDC1, &pt, 1);
		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);
		logFont.lfHeight = -abs(pt.y - ptOrg.y);

		if(hDC == NULL)
			::ReleaseDC(NULL, hDC1);

		return CreateFontIndirect(&logFont);
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hFont != NULL);
		if(m_hFont == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hFont);
		if(bRet)
			m_hFont = NULL;
		return bRet;
	}

// Attributes
	int GetLogFont(LOGFONT* pLogFont) const
	{
		ASSERT(m_hFont != NULL);
		return ::GetObject(m_hFont, sizeof(LOGFONT), pLogFont);
	}

	bool GetLogFont(LOGFONT& LogFont) const
	{
		ASSERT(m_hFont != NULL);
		return (::GetObject(m_hFont, sizeof(LOGFONT), &LogFont) == sizeof(LOGFONT));
	}
};

/////////////////////////////////////////////////////////////////////////////
// CBitmap

class CBitmap
{
public:
// Data members
	HBITMAP m_hBitmap;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CBitmap(HBITMAP hBitmap = NULL) : m_hBitmap(hBitmap)
	{
		m_bManaged = FALSE;
	}

	~CBitmap()
	{
		if(m_bManaged && m_hBitmap != NULL)
			DeleteObject();
	}

	CBitmap& operator=(HBITMAP hBitmap)
	{
		m_hBitmap = hBitmap;
		return *this;
	}

	void Attach(HBITMAP hBitmap)
	{
		if(m_bManaged && m_hBitmap != NULL)
			::DeleteObject(m_hBitmap);
		m_hBitmap = hBitmap;
	}

	HBITMAP Detach()
	{
		HBITMAP hBitmap = m_hBitmap;
		m_hBitmap = NULL;
		return hBitmap;
	}

	operator HBITMAP() const { return m_hBitmap; }

	bool IsNull() const { return (m_hBitmap == NULL); }

// Create and load methods

	// modified by dandy
	HBITMAP LoadBitmap(LPCTSTR lpszBitmap, HINSTANCE hInstance = GetResourceInstance())
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::LoadBitmap(hInstance, lpszBitmap);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP LoadBitmap(UINT uId, HINSTANCE hInstance = GetResourceInstance())
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::LoadBitmap(hInstance, MAKEINTRESOURCE(uId));
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP LoadOEMBitmap(UINT nIDBitmap) // for OBM_/OCR_/OIC_
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap));
		m_bManaged = TRUE;
		return m_hBitmap;
	}

#ifdef _INC_COMMCTRL
	HBITMAP LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0, HINSTANCE hInstance = GetResourceInstance())
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateMappedBitmap(hInstance, nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize);
		m_bManaged = TRUE;
		return m_hBitmap;
	}
#endif // _INC_COMMCTRL

	HBITMAP CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP CreateBitmapIndirect(LPBITMAP lpBitmap)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateBitmapIndirect(lpBitmap);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP CreateDiscardableBitmap(HDC hDC, int nWidth, int nHeight)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateDiscardableBitmap(hDC, nWidth, nHeight);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hBitmap != NULL);
		if(m_hBitmap == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hBitmap);
		if(bRet)
			m_hBitmap = NULL;
		return bRet;
	}

// Attributes
	int GetBitmap(BITMAP* pBitMap) const
	{
		ASSERT(m_hBitmap != NULL);
		return ::GetObject(m_hBitmap, sizeof(BITMAP), pBitMap);
	}

	bool GetBitmap(BITMAP& bm) const
	{
		ASSERT(m_hBitmap != NULL);
		return (::GetObject(m_hBitmap, sizeof(BITMAP), &bm) == sizeof(BITMAP));
	}

	bool GetSize(SIZE& size) const
	{
		ASSERT(m_hBitmap != NULL);
		BITMAP bm;
		if(!GetBitmap(&bm))
			return false;
		size.cx = bm.bmWidth;
		size.cy = bm.bmHeight;
		return true;
	}

	DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
	{
		ASSERT(m_hBitmap != NULL);
		return ::GetBitmapBits(m_hBitmap, dwCount, lpBits);
	}

	DWORD SetBitmapBits(DWORD dwCount, const void* lpBits)
	{
		ASSERT(m_hBitmap != NULL);
		return ::SetBitmapBits(m_hBitmap, dwCount, lpBits);
	}

	BOOL GetBitmapDimension(LPSIZE lpSize) const
	{
		ASSERT(m_hBitmap != NULL);
		return ::GetBitmapDimensionEx(m_hBitmap, lpSize);
	}

	BOOL SetBitmapDimension(int nWidth, int nHeight, LPSIZE lpSize = NULL)
	{
		ASSERT(m_hBitmap != NULL);
		return ::SetBitmapDimensionEx(m_hBitmap, nWidth, nHeight, lpSize);
	}

// DIB support
	HBITMAP CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* lpbmih, DWORD dwInit, CONST VOID* lpbInit, CONST BITMAPINFO* lpbmi, UINT uColorUse)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateDIBitmap(hDC, lpbmih, dwInit, lpbInit, lpbmi, uColorUse);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	HBITMAP CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset)
	{
		ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset);
		m_bManaged = TRUE;
		return m_hBitmap;
	}

	int GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const
	{
		ASSERT(m_hBitmap != NULL);
		return ::GetDIBits(hDC, m_hBitmap, uStartScan, cScanLines,  lpvBits, lpbmi, uColorUse);
	}

	int SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse)
	{
		ASSERT(m_hBitmap != NULL);
		return ::SetDIBits(hDC, m_hBitmap, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CPalette

class CPalette
{
public:
// Data members
	HPALETTE m_hPalette;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CPalette(HPALETTE hPalette = NULL) : m_hPalette(hPalette)
	{
		m_bManaged = FALSE;
	}

	~CPalette()
	{
		if(m_bManaged && m_hPalette != NULL)
			DeleteObject();
	}

	CPalette& operator=(HPALETTE hPalette)
	{
		m_hPalette = hPalette;
		return *this;
	}

	void Attach(HPALETTE hPalette)
	{
		if(m_bManaged && m_hPalette != NULL)
			::DeleteObject(m_hPalette);
		m_hPalette = hPalette;
	}

	HPALETTE Detach()
	{
		HPALETTE hPalette = m_hPalette;
		m_hPalette = NULL;
		return hPalette;
	}

	operator HPALETTE() const { return m_hPalette; }

	bool IsNull() const { return (m_hPalette == NULL); }

// Create methods
	HPALETTE CreatePalette(LPLOGPALETTE lpLogPalette)
	{
		ASSERT(m_hPalette == NULL);
		m_hPalette = ::CreatePalette(lpLogPalette);
		m_bManaged = TRUE;
		return m_hPalette;
	}

	HPALETTE CreateHalftonePalette(HDC hDC)
	{
		ASSERT(m_hPalette == NULL);
		ASSERT(hDC != NULL);
		m_hPalette = ::CreateHalftonePalette(hDC);
		m_bManaged = TRUE;
		return m_hPalette;
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hPalette != NULL);
		if(m_hPalette == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hPalette);
		if(bRet)
			m_hPalette = NULL;
		return bRet;
	}

// Attributes
	int GetEntryCount() const
	{
		ASSERT(m_hPalette != NULL);
		WORD nEntries;
		::GetObject(m_hPalette, sizeof(WORD), &nEntries);
		return (int)nEntries;
	}

	UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	{
		ASSERT(m_hPalette != NULL);
		return ::GetPaletteEntries(m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	{
		ASSERT(m_hPalette != NULL);
		return ::SetPaletteEntries(m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

// Operations
	void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	{
		ASSERT(m_hPalette != NULL);
		::AnimatePalette(m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	BOOL ResizePalette(UINT nNumEntries)
	{
		ASSERT(m_hPalette != NULL);
		return ::ResizePalette(m_hPalette, nNumEntries);
	}
	UINT GetNearestPaletteIndex(COLORREF crColor) const
	{
		ASSERT(m_hPalette != NULL);
		return ::GetNearestPaletteIndex(m_hPalette, crColor);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CRgn

class CRgn
{
public:
// Data members
	HRGN m_hRgn;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CRgn(HRGN hRgn = NULL) : m_hRgn(hRgn)
	{
		m_bManaged = FALSE;
	}

	~CRgn()
	{
		if(m_bManaged && m_hRgn != NULL)
			DeleteObject();
	}

	CRgn& operator=(HRGN hRgn)
	{
		m_hRgn = hRgn;
		return *this;
	}

	void Attach(HRGN hRgn)
	{
		if(m_bManaged && m_hRgn != NULL)
			::DeleteObject(m_hRgn);
		m_hRgn = hRgn;
	}

	HRGN Detach()
	{
		HRGN hRgn = m_hRgn;
		m_hRgn = NULL;
		return hRgn;
	}

	operator HRGN() const { return m_hRgn; }

	bool IsNull() const { return (m_hRgn == NULL); }

// Create methods
	HRGN CreateRectRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgn(x1, y1, x2, y2);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateRectRgnIndirect(LPCRECT lpRect)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgnIndirect(lpRect);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateEllipticRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateEllipticRgnIndirect(LPCRECT lpRect)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgnIndirect(lpRect);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateFromPath(HDC hDC)
	{
		ASSERT(m_hRgn == NULL);
		ASSERT(hDC != NULL);
		m_hRgn = ::PathToRegion(hDC);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	HRGN CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
		m_bManaged = TRUE;
		return m_hRgn;
	}

	BOOL DeleteObject()
	{
		ASSERT(m_hRgn != NULL);
		if(m_hRgn == NULL)
			return FALSE;

		BOOL bRet = ::DeleteObject(m_hRgn);
		if(bRet)
			m_hRgn = NULL;
		return bRet;
	}

// Operations
	void SetRectRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn != NULL);
		::SetRectRgn(m_hRgn, x1, y1, x2, y2);
	}

	void SetRectRgn(LPCRECT lpRect)
	{
		ASSERT(m_hRgn != NULL);
		::SetRectRgn(m_hRgn, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	int CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, hRgnSrc1, hRgnSrc2, nCombineMode);
	}

	int CombineRgn(HRGN hRgnSrc, int nCombineMode)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, m_hRgn, hRgnSrc, nCombineMode);
	}

	int CopyRgn(HRGN hRgnSrc)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, hRgnSrc, NULL, RGN_COPY);
	}

	BOOL EqualRgn(HRGN hRgn) const
	{
		ASSERT(m_hRgn != NULL);
		return ::EqualRgn(m_hRgn, hRgn);
	}

	int OffsetRgn(int x, int y)
	{
		ASSERT(m_hRgn != NULL);
		return ::OffsetRgn(m_hRgn, x, y);
	}

	int OffsetRgn(POINT point)
	{
		ASSERT(m_hRgn != NULL);
		return ::OffsetRgn(m_hRgn, point.x, point.y);
	}

	int GetRgnBox(LPRECT lpRect) const
	{
		ASSERT(m_hRgn != NULL);
		return ::GetRgnBox(m_hRgn, lpRect);
	}

	BOOL PtInRegion(int x, int y) const
	{
		ASSERT(m_hRgn != NULL);
		return ::PtInRegion(m_hRgn, x, y);
	}

	BOOL PtInRegion(POINT point) const
	{
		ASSERT(m_hRgn != NULL);
		return ::PtInRegion(m_hRgn, point.x, point.y);
	}

	BOOL RectInRegion(LPCRECT lpRect) const
	{
		ASSERT(m_hRgn != NULL);
		return ::RectInRegion(m_hRgn, lpRect);
	}

	int GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
	{
		ASSERT(m_hRgn != NULL);
		return (int)::GetRegionData(m_hRgn, nDataSize, lpRgnData);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDC - The device context class

class CDC
{
public:
// Data members
	HDC m_hDC;
	BOOL m_bManaged;

// Constructor/destructor/operators
	CDC(HDC hDC = NULL) : m_hDC(hDC)
	{
		m_bManaged = FALSE;
	}

	~CDC()
	{
		if(m_bManaged && m_hDC != NULL)
			::DeleteDC(Detach());
	}

	CDC& operator=(HDC hDC)
	{
		m_hDC = hDC;
		return *this;
	}

	void Attach(HDC hDC)
	{
		if(m_bManaged && m_hDC != NULL)
			::DeleteDC(m_hDC);
		m_hDC = hDC;
	}

	HDC Detach()
	{
		HDC hDC = m_hDC;
		m_hDC = NULL;
		return hDC;
	}

	operator HDC() const { return m_hDC; }

	bool IsNull() const { return (m_hDC == NULL); }

// Operations
	HWND WindowFromDC() const
	{
		ASSERT(m_hDC != NULL);
		return ::WindowFromDC(m_hDC);
	}

	HPEN GetCurrentPen() const
	{
		ASSERT(m_hDC != NULL);
		return (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
	}

	HBRUSH GetCurrentBrush() const
	{
		ASSERT(m_hDC != NULL);
		return (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);
	}

	HPALETTE GetCurrentPalette() const
	{
		ASSERT(m_hDC != NULL);
		return (HPALETTE)::GetCurrentObject(m_hDC, OBJ_PAL);
	}

	HFONT GetCurrentFont() const
	{
		ASSERT(m_hDC != NULL);
		return (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
	}

	HBITMAP GetCurrentBitmap() const
	{
		ASSERT(m_hDC != NULL);
		return (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);
	}

	HDC CreateDC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const DEVMODE* lpInitData)
	{
		ASSERT(m_hDC == NULL);
		m_hDC = ::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, lpInitData);
		m_bManaged = TRUE;
		return m_hDC;
	}

	HDC CreateCompatibleDC(HDC hDC = NULL)
	{
		ASSERT(m_hDC == NULL);
		m_hDC = ::CreateCompatibleDC(hDC);
		m_bManaged = TRUE;
		return m_hDC;
	}

	BOOL DeleteDC()
	{
		if(m_hDC == NULL)
			return FALSE;
		BOOL bRet = ::DeleteDC(m_hDC);
		if(bRet)
			m_hDC = NULL;
		return bRet;
	}

// Device-Context Functions
	int SaveDC()
	{
		ASSERT(m_hDC != NULL);
		return ::SaveDC(m_hDC);
	}

	BOOL RestoreDC(int nSavedDC)
	{
		ASSERT(m_hDC != NULL);
		return ::RestoreDC(m_hDC, nSavedDC);
	}

	int GetDeviceCaps(int nIndex) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetDeviceCaps(m_hDC, nIndex);
	}

	UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags)
	{
		ASSERT(m_hDC != NULL);
		return ::SetBoundsRect(m_hDC, lpRectBounds, flags);
	}

	UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetBoundsRect(m_hDC, lpRectBounds, flags);
	}

	BOOL ResetDC(const DEVMODE* lpDevMode)
	{
		ASSERT(m_hDC != NULL);
		return ::ResetDC(m_hDC, lpDevMode) != NULL;
	}

// Drawing-Tool Functions
	BOOL GetBrushOrg(LPPOINT lpPoint) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetBrushOrgEx(m_hDC, lpPoint);
	}

	BOOL SetBrushOrg(int x, int y, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetBrushOrgEx(m_hDC, x, y, lpPoint);
	}

	BOOL SetBrushOrg(POINT point, LPPOINT lpPointRet = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetBrushOrgEx(m_hDC, point.x, point.y, lpPointRet);
	}

	int EnumObjects(int nObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
	{
		ASSERT(m_hDC != NULL);
#ifdef STRICT
		return ::EnumObjects(m_hDC, nObjectType, (GOBJENUMPROC)lpfn, lpData);
#else
		return ::EnumObjects(m_hDC, nObjectType, (GOBJENUMPROC)lpfn, (LPVOID)lpData);
#endif
	}

// Type-safe selection helpers
	HPEN SelectPen(HPEN hPen)
	{
		ASSERT(m_hDC != NULL);
		ASSERT(hPen == NULL || ::GetObjectType(hPen) == OBJ_PEN || ::GetObjectType(hPen) == OBJ_EXTPEN);
		return (HPEN)::SelectObject(m_hDC, hPen);
	}

	HBRUSH SelectBrush(HBRUSH hBrush)
	{
		ASSERT(m_hDC != NULL);
		ASSERT(hBrush == NULL || ::GetObjectType(hBrush) == OBJ_BRUSH);
		return (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	HFONT SelectFont(HFONT hFont)
	{
		ASSERT(m_hDC != NULL);
		ASSERT(hFont == NULL || ::GetObjectType(hFont) == OBJ_FONT);
		return (HFONT)::SelectObject(m_hDC, hFont);
	}

	HBITMAP SelectBitmap(HBITMAP hBitmap)
	{
		ASSERT(m_hDC != NULL);
		ASSERT(hBitmap == NULL || ::GetObjectType(hBitmap) == OBJ_BITMAP);
		return (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	int SelectRgn(HRGN hRgn)       // special return for regions
	{
		ASSERT(m_hDC != NULL);
		ASSERT(hRgn == NULL || ::GetObjectType(hRgn) == OBJ_REGION);
		return PtrToInt(::SelectObject(m_hDC, hRgn));
	}

// Type-safe selection helpers for stock objects
	HPEN SelectStockPen(int nPen)
	{
		ASSERT(m_hDC != NULL);
#if (_WIN32_WINNT >= 0x0500)
		ASSERT(nPen == WHITE_PEN || nPen == BLACK_PEN || nPen == NULL_PEN || nPen == DC_PEN);
#else
		ASSERT(nPen == WHITE_PEN || nPen == BLACK_PEN || nPen == NULL_PEN);
#endif //!(_WIN32_WINNT >= 0x0500)
		return SelectPen((HPEN)::GetStockObject(nPen));
	}

	HBRUSH SelectStockBrush(int nBrush)
	{
#if (_WIN32_WINNT >= 0x0500)
		ASSERT((nBrush >= WHITE_BRUSH && nBrush <= HOLLOW_BRUSH) || nBrush == DC_BRUSH);
#else
		ASSERT(nBrush >= WHITE_BRUSH && nBrush <= HOLLOW_BRUSH);
#endif //!(_WIN32_WINNT >= 0x0500)
		return SelectBrush((HBRUSH)::GetStockObject(nBrush));
	}

	HFONT SelectStockFont(int nFont)
	{
		ASSERT((nFont >= OEM_FIXED_FONT && nFont <= SYSTEM_FIXED_FONT) || nFont == DEFAULT_GUI_FONT);
		return SelectFont((HFONT)::GetStockObject(nFont));
	}

	HPALETTE SelectStockPalette(int nPalette, BOOL bForceBackground)
	{
		ASSERT(nPalette == DEFAULT_PALETTE); // the only one supported
		return SelectPalette((HPALETTE)::GetStockObject(nPalette), bForceBackground);
	}

// Color and Color Palette Functions
	COLORREF GetNearestColor(COLORREF crColor) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetNearestColor(m_hDC, crColor);
	}

	HPALETTE SelectPalette(HPALETTE hPalette, BOOL bForceBackground)
	{
		ASSERT(m_hDC != NULL);
		return ::SelectPalette(m_hDC, hPalette, bForceBackground);
	}

	UINT RealizePalette()
	{
		ASSERT(m_hDC != NULL);
		return ::RealizePalette(m_hDC);
	}

	void UpdateColors()
	{
		ASSERT(m_hDC != NULL);
		::UpdateColors(m_hDC);
	}

// Drawing-Attribute Functions
	COLORREF GetBkColor() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetBkColor(m_hDC);
	}

	int GetBkMode() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetBkMode(m_hDC);
	}

	int GetPolyFillMode() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetPolyFillMode(m_hDC);
	}

	int GetROP2() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetROP2(m_hDC);
	}

	int GetStretchBltMode() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetStretchBltMode(m_hDC);
	}

	COLORREF GetTextColor() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextColor(m_hDC);
	}

	COLORREF SetBkColor(COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetBkColor(m_hDC, crColor);
	}

	int SetBkMode(int nBkMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SetBkMode(m_hDC, nBkMode);
	}

	int SetPolyFillMode(int nPolyFillMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SetPolyFillMode(m_hDC, nPolyFillMode);
	}

	int SetROP2(int nDrawMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SetROP2(m_hDC, nDrawMode);
	}

	int SetStretchBltMode(int nStretchMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SetStretchBltMode(m_hDC, nStretchMode);
	}

	COLORREF SetTextColor(COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetTextColor(m_hDC, crColor);
	}

	BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetColorAdjustment(m_hDC, lpColorAdjust);
	}
	BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
	{
		ASSERT(m_hDC != NULL);
		return ::SetColorAdjustment(m_hDC, lpColorAdjust);
	}

// Mapping Functions
	int GetMapMode() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetMapMode(m_hDC);
	}

	BOOL GetViewportOrg(LPPOINT lpPoint) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetViewportOrgEx(m_hDC, lpPoint);
	}

	int SetMapMode(int nMapMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SetMapMode(m_hDC, nMapMode);
	}

	// Viewport Origin
	BOOL SetViewportOrg(int x, int y, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetViewportOrgEx(m_hDC, x, y, lpPoint);
	}

	BOOL SetViewportOrg(POINT point, LPPOINT lpPointRet = NULL)
	{
		ASSERT(m_hDC != NULL);
		return SetViewportOrg(point.x, point.y, lpPointRet);
	}

	BOOL OffsetViewportOrg(int nWidth, int nHeight, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, lpPoint);
	}

	// Viewport Extent
	BOOL GetViewportExt(LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetViewportExtEx(m_hDC, lpSize);
	}
	
	BOOL SetViewportExt(int x, int y, LPSIZE lpSize = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetViewportExtEx(m_hDC, x, y, lpSize);
	}

	BOOL SetViewportExt(SIZE size, LPSIZE lpSizeRet = NULL)
	{
		ASSERT(m_hDC != NULL);
		return SetViewportExt(size.cx, size.cy, lpSizeRet);
	}

	BOOL ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}

	// Window Origin
	BOOL GetWindowOrg(LPPOINT lpPoint) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetWindowOrgEx(m_hDC, lpPoint);
	}

	BOOL SetWindowOrg(int x, int y, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetWindowOrgEx(m_hDC, x, y, lpPoint);
	}

	BOOL SetWindowOrg(POINT point, LPPOINT lpPointRet = NULL)
	{
		ASSERT(m_hDC != NULL);
		return SetWindowOrg(point.x, point.y, lpPointRet);
	}

	BOOL OffsetWindowOrg(int nWidth, int nHeight, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, lpPoint);
	}

	// Window extent
	BOOL GetWindowExt(LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetWindowExtEx(m_hDC, lpSize);
	}

	BOOL SetWindowExt(int x, int y, LPSIZE lpSize = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::SetWindowExtEx(m_hDC, x, y, lpSize);
	}

	BOOL SetWindowExt(SIZE size, LPSIZE lpSizeRet)
	{
		ASSERT(m_hDC != NULL);
		return SetWindowExt(size.cx, size.cy, lpSizeRet);
	}

	BOOL ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}

// Coordinate Functions
	BOOL DPtoLP(LPPOINT lpPoints, int nCount = 1) const
	{
		ASSERT(m_hDC != NULL);
		return ::DPtoLP(m_hDC, lpPoints, nCount);
	}

	BOOL DPtoLP(LPRECT lpRect) const
	{
		ASSERT(m_hDC != NULL);
		return ::DPtoLP(m_hDC, (LPPOINT)lpRect, 2);
	}

	BOOL DPtoLP(LPSIZE lpSize) const
	{
		SIZE sizeWinExt;
		if(!GetWindowExt(&sizeWinExt))
			return FALSE;
		SIZE sizeVpExt;
		if(!GetViewportExt(&sizeVpExt))
			return FALSE;
		lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
		lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
		return TRUE;
	}

	BOOL LPtoDP(LPPOINT lpPoints, int nCount = 1) const
	{
		ASSERT(m_hDC != NULL);
		return ::LPtoDP(m_hDC, lpPoints, nCount);
	}

	BOOL LPtoDP(LPRECT lpRect) const
	{
		ASSERT(m_hDC != NULL);
		return ::LPtoDP(m_hDC, (LPPOINT)lpRect, 2);
	}

	BOOL LPtoDP(LPSIZE lpSize) const
	{
		SIZE sizeWinExt;
		if(!GetWindowExt(&sizeWinExt))
			return FALSE;
		SIZE sizeVpExt;
		if(!GetViewportExt(&sizeVpExt))
			return FALSE;
		lpSize->cx = ::MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
		lpSize->cy = ::MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
		return TRUE;
	}

// Special Coordinate Functions (useful for dealing with metafiles and OLE)
	#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

	void DPtoHIMETRIC(LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		int nMapMode;
		if((nMapMode = GetMapMode()) < MM_ISOTROPIC && nMapMode != MM_TEXT)
		{
			// when using a constrained map mode, map against physical inch
			((CDC*)this)->SetMapMode(MM_HIMETRIC);
			DPtoLP(lpSize);
			((CDC*)this)->SetMapMode(nMapMode);
		}
		else
		{
			// map against logical inch for non-constrained mapping modes
			int cxPerInch = GetDeviceCaps(LOGPIXELSX);
			int cyPerInch = GetDeviceCaps(LOGPIXELSY);
			ASSERT(cxPerInch != 0 && cyPerInch != 0);
			lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
			lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
		}
	}

	void HIMETRICtoDP(LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		int nMapMode;
		if((nMapMode = GetMapMode()) < MM_ISOTROPIC && nMapMode != MM_TEXT)
		{
			// when using a constrained map mode, map against physical inch
			((CDC*)this)->SetMapMode(MM_HIMETRIC);
			LPtoDP(lpSize);
			((CDC*)this)->SetMapMode(nMapMode);
		}
		else
		{
			// map against logical inch for non-constrained mapping modes
			int cxPerInch = GetDeviceCaps(LOGPIXELSX);
			int cyPerInch = GetDeviceCaps(LOGPIXELSY);
			ASSERT(cxPerInch != 0 && cyPerInch != 0);
			lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
			lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
		}
	}

	void LPtoHIMETRIC(LPSIZE lpSize) const
	{
		LPtoDP(lpSize);
		DPtoHIMETRIC(lpSize);
	}

	void HIMETRICtoLP(LPSIZE lpSize) const
	{
		HIMETRICtoDP(lpSize);
		DPtoLP(lpSize);
	}

// Region Functions
	BOOL FillRgn(HRGN hRgn, HBRUSH hBrush)
	{
		ASSERT(m_hDC != NULL);
		return ::FillRgn(m_hDC, hRgn, hBrush);
	}

	BOOL FrameRgn(HRGN hRgn, HBRUSH hBrush, int nWidth, int nHeight)
	{
		ASSERT(m_hDC != NULL);
		return ::FrameRgn(m_hDC, hRgn, hBrush, nWidth, nHeight);
	}

	BOOL InvertRgn(HRGN hRgn)
	{
		ASSERT(m_hDC != NULL);
		return ::InvertRgn(m_hDC, hRgn);
	}

	BOOL PaintRgn(HRGN hRgn)
	{
		ASSERT(m_hDC != NULL);
		return ::PaintRgn(m_hDC, hRgn);
	}

// Clipping Functions
	int GetClipBox(LPRECT lpRect) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetClipBox(m_hDC, lpRect);
	}

	BOOL PtVisible(int x, int y) const
	{
		ASSERT(m_hDC != NULL);
		return ::PtVisible(m_hDC, x, y);
	}

	BOOL PtVisible(POINT point) const
	{
		ASSERT(m_hDC != NULL);
		return ::PtVisible(m_hDC, point.x, point.y);
	}

	BOOL RectVisible(LPCRECT lpRect) const
	{
		ASSERT(m_hDC != NULL);
		return ::RectVisible(m_hDC, lpRect);
	}

	int SelectClipRgn(HRGN hRgn)
	{
		ASSERT(m_hDC != NULL);
		return ::SelectClipRgn(m_hDC, (HRGN)hRgn);
	}

	int ExcludeClipRect(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hDC != NULL);
		return ::ExcludeClipRect(m_hDC, x1, y1, x2, y2);
	}

	int ExcludeClipRect(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		return ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	int ExcludeUpdateRgn(HWND hWnd)
	{
		ASSERT(m_hDC != NULL);
		return ::ExcludeUpdateRgn(m_hDC, hWnd);
	}

	int IntersectClipRect(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hDC != NULL);
		return ::IntersectClipRect(m_hDC, x1, y1, x2, y2);
	}

	int IntersectClipRect(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		return ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	int OffsetClipRgn(int x, int y)
	{
		ASSERT(m_hDC != NULL);
		return ::OffsetClipRgn(m_hDC, x, y);
	}

	int OffsetClipRgn(SIZE size)
	{
		ASSERT(m_hDC != NULL);
		return ::OffsetClipRgn(m_hDC, size.cx, size.cy);
	}

	int SelectClipRgn(HRGN hRgn, int nMode)
	{
		ASSERT(m_hDC != NULL);
		return ::ExtSelectClipRgn(m_hDC, hRgn, nMode);
	}

// Line-Output Functions
	BOOL GetCurrentPosition(LPPOINT lpPoint) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCurrentPositionEx(m_hDC, lpPoint);
	}

	BOOL MoveTo(int x, int y, LPPOINT lpPoint = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::MoveToEx(m_hDC, x, y, lpPoint);
	}

	BOOL MoveTo(POINT point, LPPOINT lpPointRet = NULL)
	{
		ASSERT(m_hDC != NULL);
		return MoveTo(point.x, point.y, lpPointRet);
	}

	BOOL LineTo(int x, int y)
	{
		ASSERT(m_hDC != NULL);
		return ::LineTo(m_hDC, x, y);
	}

	BOOL LineTo(POINT point)
	{
		ASSERT(m_hDC != NULL);
		return LineTo(point.x, point.y);
	}

	BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		ASSERT(m_hDC != NULL);
		return ::Arc(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{
		ASSERT(m_hDC != NULL);
		return ::Arc(m_hDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
			ptEnd.x, ptEnd.y);
	}

	BOOL Polyline(LPPOINT lpPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::Polyline(m_hDC, lpPoints, nCount);
	}

	BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle)
	{
		ASSERT(m_hDC != NULL);
		return ::AngleArc(m_hDC, x, y, nRadius, fStartAngle, fSweepAngle);
	}

	BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		ASSERT(m_hDC != NULL);
		return ::ArcTo(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	BOOL ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{
		ASSERT(m_hDC != NULL);
		return ArcTo(lpRect->left, lpRect->top, lpRect->right,
		lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	int GetArcDirection() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetArcDirection(m_hDC);
	}

	int SetArcDirection(int nArcDirection)
	{
		ASSERT(m_hDC != NULL);
		return ::SetArcDirection(m_hDC, nArcDirection);
	}

	BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolyDraw(m_hDC, lpPoints, lpTypes, nCount);
	}

	BOOL PolylineTo(const POINT* lpPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolylineTo(m_hDC, lpPoints, nCount);
	}

	BOOL PolyPolyline(const POINT* lpPoints,
		const DWORD* lpPolyPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolyPolyline(m_hDC, lpPoints, lpPolyPoints, nCount);
	}

	BOOL PolyBezier(const POINT* lpPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolyBezier(m_hDC, lpPoints, nCount);
	}

	BOOL PolyBezierTo(const POINT* lpPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolyBezierTo(m_hDC, lpPoints, nCount);
	}

// Simple Drawing Functions
	BOOL FillRect(LPCRECT lpRect, HBRUSH hBrush)
	{
		ASSERT(m_hDC != NULL);
		return ::FillRect(m_hDC, lpRect, hBrush);
	}

	BOOL FillRect(LPCRECT lpRect, int nColorIndex)
	{
		ASSERT(m_hDC != NULL);
		return ::FillRect(m_hDC, lpRect, (HBRUSH)LongToPtr(nColorIndex + 1));
	}

	BOOL FrameRect(LPCRECT lpRect, HBRUSH hBrush)
	{
		ASSERT(m_hDC != NULL);
		return ::FrameRect(m_hDC, lpRect, hBrush);
	}

	BOOL InvertRect(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		return ::InvertRect(m_hDC, lpRect);
	}

	BOOL DrawIcon(int x, int y, HICON hIcon)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawIcon(m_hDC, x, y, hIcon);
	}

	BOOL DrawIcon(POINT point, HICON hIcon)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawIcon(m_hDC, point.x, point.y, hIcon);
	}

	BOOL DrawIconEx(int x, int y, HICON hIcon, int cxWidth, int cyWidth, UINT uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, UINT uFlags = DI_NORMAL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawIconEx(m_hDC, x, y, hIcon, cxWidth, cyWidth, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}

	BOOL DrawIconEx(POINT point, HICON hIcon, SIZE size, UINT uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, UINT uFlags = DI_NORMAL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawIconEx(m_hDC, point.x, point.y, hIcon, size.cx, size.cy, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}

	BOOL DrawState(POINT pt, SIZE size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawState(m_hDC, hBrush, NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_BITMAP);
	}

	BOOL DrawState(POINT pt, SIZE size, HICON hIcon, UINT nFlags, HBRUSH hBrush = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawState(m_hDC, hBrush, NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_ICON);
	}

	BOOL DrawState(POINT pt, SIZE size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawState(m_hDC, hBrush, NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags | (bPrefixText ? DST_PREFIXTEXT : DST_TEXT));
	}

	BOOL DrawState(POINT pt, SIZE size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawState(m_hDC, hBrush, lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_COMPLEX);
	}

// Ellipse and Polygon Functions
	BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		ASSERT(m_hDC != NULL);
		return ::Chord(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{
		ASSERT(m_hDC != NULL);
		return ::Chord(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	void DrawFocusRect(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		::DrawFocusRect(m_hDC, lpRect);
	}

	BOOL Ellipse(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hDC != NULL);
		return ::Ellipse(m_hDC, x1, y1, x2, y2);
	}

	BOOL Ellipse(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		return ::Ellipse(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		ASSERT(m_hDC != NULL);
		return ::Pie(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	
	BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{
		ASSERT(m_hDC != NULL);
		return ::Pie(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	BOOL Polygon(LPPOINT lpPoints, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::Polygon(m_hDC, lpPoints, nCount);
	}

	BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount)
	{
		ASSERT(m_hDC != NULL);
		return ::PolyPolygon(m_hDC, lpPoints, lpPolyCounts, nCount);
	}

	BOOL Rectangle(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hDC != NULL);
		return ::Rectangle(m_hDC, x1, y1, x2, y2);
	}

	BOOL Rectangle(LPCRECT lpRect)
	{
		ASSERT(m_hDC != NULL);
		return ::Rectangle(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		ASSERT(m_hDC != NULL);
		return ::RoundRect(m_hDC, x1, y1, x2, y2, x3, y3);
	}

	BOOL RoundRect(LPCRECT lpRect, POINT point)
	{
		ASSERT(m_hDC != NULL);
		return ::RoundRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, point.x, point.y);
	}

// Bitmap Functions
	BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop)
	{
		ASSERT(m_hDC != NULL);
		return ::PatBlt(m_hDC, x, y, nWidth, nHeight, dwRop);
	}

	BOOL BitBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC,
		int xSrc, int ySrc, DWORD dwRop)
	{
		ASSERT(m_hDC != NULL);
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
	}

	BOOL StretchBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
	{
		ASSERT(m_hDC != NULL);
		return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
	}

	COLORREF GetPixel(int x, int y) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetPixel(m_hDC, x, y);
	}

	COLORREF GetPixel(POINT point) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetPixel(m_hDC, point.x, point.y);
	}

	COLORREF SetPixel(int x, int y, COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetPixel(m_hDC, x, y, crColor);
	}

	COLORREF SetPixel(POINT point, COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetPixel(m_hDC, point.x, point.y, crColor);
	}

	BOOL FloodFill(int x, int y, COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::FloodFill(m_hDC, x, y, crColor);
	}

	BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
	{
		ASSERT(m_hDC != NULL);
		return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType);
	}

	BOOL MaskBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, HBITMAP hMaskBitmap, int xMask, int yMask, DWORD dwRop)
	{
		ASSERT(m_hDC != NULL);
		return ::MaskBlt(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, hMaskBitmap, xMask, yMask, dwRop);
	}

	BOOL PlgBlt(LPPOINT lpPoint, HDC hSrcDC, int xSrc, int ySrc, int nWidth, int nHeight, HBITMAP hMaskBitmap, int xMask, int yMask)
	{
		ASSERT(m_hDC != NULL);
		return ::PlgBlt(m_hDC, lpPoint, hSrcDC, xSrc, ySrc, nWidth, nHeight, hMaskBitmap, xMask, yMask);
	}

	BOOL SetPixelV(int x, int y, COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetPixelV(m_hDC, x, y, crColor);
	}
	
	BOOL SetPixelV(POINT point, COLORREF crColor)
	{
		ASSERT(m_hDC != NULL);
		return ::SetPixelV(m_hDC, point.x, point.y, crColor);
	}

#ifndef _NO_MSIMG
	BOOL AlphaBlend(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BLENDFUNCTION bf)
	{
		ASSERT(m_hDC != NULL);
		return ::AlphaBlend(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, bf);
	}
	BOOL TransparentBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent)
	{
		ASSERT(m_hDC != NULL);
		return ::TransparentBlt(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent);
	}
	BOOL GradientFill(const PTRIVERTEX pVertices, DWORD nVertices, void* pMeshElements, DWORD nMeshElements, DWORD dwMode)
	{
		ASSERT(m_hDC != NULL);
		return ::GradientFill(m_hDC, pVertices, nVertices, pMeshElements, nMeshElements, dwMode);
	}
#endif //!_NO_MSIMG

// Extra bitmap functions
	// Helper function for painting a disabled toolbar or menu bitmap
	// This function can take either an HBITMAP (for SS) or a DC with 
	//           the bitmap already painted (for cmdbar)
	BOOL DitherBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC, HBITMAP hBitmap, int xSrc, int ySrc,
			HBRUSH hBrushBackground = ::GetSysColorBrush(COLOR_3DFACE),
			HBRUSH hBrush3DEffect = ::GetSysColorBrush(COLOR_3DHILIGHT),
			HBRUSH hBrushDisabledImage = ::GetSysColorBrush(COLOR_3DSHADOW))
	{
		ASSERT(m_hDC != NULL || hBitmap != NULL);
		ASSERT(nWidth > 0 && nHeight > 0);
		
		// Create a generic DC for all BitBlts
		CDC dc;
		if(hSrcDC != NULL)
			dc = hSrcDC;
		else
			dc = ::CreateCompatibleDC(m_hDC);

		ASSERT(dc.m_hDC != NULL);
		if(dc.m_hDC == NULL)
			return FALSE;
		
		// Create a DC for the monochrome DIB section
		CDC dcBW = ::CreateCompatibleDC(m_hDC);
		ASSERT(dcBW.m_hDC != NULL);
		if(dcBW.m_hDC == NULL)
		{
			if(hSrcDC != NULL)
				dc.Detach();
			return FALSE;
		}

		// Create the monochrome DIB section with a black and white palette
		struct RGBBWBITMAPINFO
		{
			BITMAPINFOHEADER bmiHeader; 
			RGBQUAD bmiColors[2]; 
		};

		RGBBWBITMAPINFO rgbBWBitmapInfo = 
		{
			{ sizeof(BITMAPINFOHEADER), nWidth, nHeight, 1, 1, BI_RGB, 0, 0, 0, 0, 0 },
			{ { 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 } }
		};

		VOID* pbitsBW;
		CBitmap bmpBW = ::CreateDIBSection(dcBW, (LPBITMAPINFO)&rgbBWBitmapInfo, DIB_RGB_COLORS, &pbitsBW, NULL, 0);
		ASSERT(bmpBW.m_hBitmap != NULL);
		if(bmpBW.m_hBitmap == NULL)
		{
			if(hSrcDC != NULL)
				dc.Detach();
			return FALSE;
		}
		
		// Attach the monochrome DIB section and the bitmap to the DCs
		HBITMAP hbmOldBW = dcBW.SelectBitmap(bmpBW);
		HBITMAP hbmOldDC = NULL;
		if(hBitmap != NULL)
			hbmOldDC = dc.SelectBitmap(hBitmap);

		// Block: Dark gray removal: we want (128, 128, 128) pixels to become black and not white
		{
			CDC dcTemp1 = ::CreateCompatibleDC(m_hDC);
			CDC dcTemp2 = ::CreateCompatibleDC(m_hDC);
			CBitmap bmpTemp1;
			bmpTemp1.CreateCompatibleBitmap(dc, nWidth, nHeight);
			CBitmap bmpTemp2;
			bmpTemp2.CreateBitmap(nWidth, nHeight, 1, 1, NULL);
			HBITMAP hOldBmp1 = dcTemp1.SelectBitmap(bmpTemp1);
			HBITMAP hOldBmp2 = dcTemp2.SelectBitmap(bmpTemp2);
			// Let's copy our image, it will be altered
			dcTemp1.BitBlt(0, 0, nWidth, nHeight, dc, xSrc, ySrc, SRCCOPY);

			// All dark gray pixels will become white, the others black
			dcTemp1.SetBkColor(RGB(128, 128, 128));
			dcTemp2.BitBlt(0, 0, nWidth, nHeight, dcTemp1, 0, 0, SRCCOPY);
			// Do an XOR to set to black these white pixels
			dcTemp1.BitBlt(0, 0, nWidth, nHeight, dcTemp2, 0, 0, SRCINVERT);

			// BitBlt the bitmap into the monochrome DIB section
			// The DIB section will do a true monochrome conversion
			// The magenta background being closer to white will become white
			dcBW.BitBlt(0, 0, nWidth, nHeight, dcTemp1, 0, 0, SRCCOPY);

			// Cleanup
			dcTemp1.SelectBitmap(hOldBmp1);
			dcTemp2.SelectBitmap(hOldBmp2);
		}
		
		// Paint the destination rectangle using hBrushBackground
		RECT rc = { x, y, x + nWidth, y + nHeight };
		FillRect(&rc, hBrushBackground);

		// BitBlt the black bits in the monochrome bitmap into hBrush3DEffect color in the destination DC
		// The magic ROP comes from the Charles Petzold's book
		HBRUSH hOldBrush = SelectBrush(hBrush3DEffect);
		BitBlt(x + 1, y + 1, nWidth, nHeight, dcBW, 0, 0, 0xB8074A);

		// BitBlt the black bits in the monochrome bitmap into hBrushDisabledImage color in the destination DC
		SelectBrush(hBrushDisabledImage);
		BitBlt(x, y, nWidth, nHeight, dcBW, 0, 0, 0xB8074A);

		SelectBrush(hOldBrush);
		dcBW.SelectBitmap(hbmOldBW);
		dc.SelectBitmap(hbmOldDC);

		if(hSrcDC != NULL)
			dc.Detach();

		return TRUE;
	}

// Text Functions
	BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount = -1)
	{
		ASSERT(m_hDC != NULL);
		if(nCount == -1)
			nCount = lstrlen(lpszString);
		return ::TextOut(m_hDC, x, y, lpszString, nCount);
	}

	BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount = -1, LPINT lpDxWidths = NULL)
	{
		ASSERT(m_hDC != NULL);
		if(nCount == -1)
			nCount = lstrlen(lpszString);
		return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths);
	}

	SIZE TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount = -1, int nTabPositions = 0, LPINT lpnTabStopPositions = NULL, int nTabOrigin = 0)
	{
		ASSERT(m_hDC != NULL);
		if(nCount == -1)
			nCount = lstrlen(lpszString);
		LONG lRes = ::TabbedTextOut(m_hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
		SIZE size = { (short)LOWORD(lRes), (short)HIWORD(lRes) };
		return size;
	}

	int DrawText(LPCTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawText(m_hDC, lpstrText, cchText, lpRect, uFormat);
	}

	int DrawTextEx(LPTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat, LPDRAWTEXTPARAMS lpDTParams = NULL)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawTextEx(m_hDC, lpstrText, cchText, lpRect, uFormat, lpDTParams);
	}

#if (_WIN32_WINNT >= 0x0501)
	int DrawShadowText(LPCWSTR lpstrText, int cchText, LPRECT lpRect, DWORD dwFlags, COLORREF clrText, COLORREF clrShadow, int xOffset, int yOffset)
	{
		ASSERT(m_hDC != NULL);
		// This function is present only if comctl32.dll version 6 is loaded;
		// we use LoadLibrary/GetProcAddress to allow apps compiled with
		// _WIN32_WINNT >= 0x0501 to run on older Windows/CommCtrl
		int nRet = 0;
		HMODULE hCommCtrlDLL = ::LoadLibrary(_T("comctl32.dll"));
		ASSERT(hCommCtrlDLL != NULL);
		if(hCommCtrlDLL != NULL)
		{
			typedef int (WINAPI *PFN_DrawShadowText)(HDC hDC, LPCWSTR lpstrText, UINT cchText, LPRECT lpRect, DWORD dwFlags, COLORREF clrText, COLORREF clrShadow, int xOffset, int yOffset);
			PFN_DrawShadowText pfnDrawShadowText = (PFN_DrawShadowText)::GetProcAddress(hCommCtrlDLL, "DrawShadowText");
			ASSERT(pfnDrawShadowText != NULL);	// this function requires CommCtrl6
			if(pfnDrawShadowText != NULL)
				nRet = pfnDrawShadowText(m_hDC, lpstrText, cchText, lpRect, dwFlags, clrText, clrShadow, xOffset, yOffset);
			::FreeLibrary(hCommCtrlDLL);
		}
		return nRet;
	}
#endif //(_WIN32_WINNT >= 0x0501)

	BOOL GetTextExtent(LPCTSTR lpszString, int nCount, LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		if(nCount == -1)
			nCount = lstrlen(lpszString);
		return ::GetTextExtentPoint32(m_hDC, lpszString, nCount, lpSize);
	}

	DWORD GetTabbedTextExtent(LPCTSTR lpszString, int nCount = -1, int nTabPositions = 0, LPINT lpnTabStopPositions = NULL) const
	{
		ASSERT(m_hDC != NULL);
		if(nCount == -1)
			nCount = lstrlen(lpszString);
		return ::GetTabbedTextExtent(m_hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions);
	}

	BOOL GrayString(HBRUSH hBrush, BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight)
	{
		ASSERT(m_hDC != NULL);
		return ::GrayString(m_hDC, hBrush, (GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight);
	}

	UINT GetTextAlign() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextAlign(m_hDC);
	}

	UINT SetTextAlign(UINT nFlags)
	{
		ASSERT(m_hDC != NULL);
		return ::SetTextAlign(m_hDC, nFlags);
	}

	int GetTextFace(LPTSTR lpszFacename, int nCount) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextFace(m_hDC, nCount, lpszFacename);
	}

	int GetTextFaceLen() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextFace(m_hDC, 0, NULL);
	}

	BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextMetrics(m_hDC, lpMetrics);
	}

	int SetTextJustification(int nBreakExtra, int nBreakCount)
	{
		ASSERT(m_hDC != NULL);
		return ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
	}

	int GetTextCharacterExtra() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextCharacterExtra(m_hDC);
	}

	int SetTextCharacterExtra(int nCharExtra)
	{
		ASSERT(m_hDC != NULL);
		return ::SetTextCharacterExtra(m_hDC, nCharExtra);
	}

// Advanced Drawing
	BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawEdge(m_hDC, lpRect, nEdge, nFlags);
	}

	BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawFrameControl(m_hDC, lpRect, nType, nState);
	}

// Scrolling Functions
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate)
	{
		ASSERT(m_hDC != NULL);
		return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate);
	}

// Font Functions
	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharWidth(m_hDC, nFirstChar, nLastChar, lpBuffer);
	}

	// GetCharWidth32 is not supported under Win9x
	BOOL GetCharWidth32(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharWidth32(m_hDC, nFirstChar, nLastChar, lpBuffer);
	}

	DWORD SetMapperFlags(DWORD dwFlag)
	{
		ASSERT(m_hDC != NULL);
		return ::SetMapperFlags(m_hDC, dwFlag);
	}

	BOOL GetAspectRatioFilter(LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetAspectRatioFilterEx(m_hDC, lpSize);
	}

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharABCWidths(m_hDC, nFirstChar, nLastChar, lpabc);
	}

	DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetFontData(m_hDC, dwTable, dwOffset, lpData, cbData);
	}

	int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetKerningPairs(m_hDC, nPairs, lpkrnpair);
	}

	UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetOutlineTextMetrics(m_hDC, cbData, lpotm);
	}

	DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm, DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetGlyphOutline(m_hDC, nChar, nFormat, lpgm, cbBuffer, lpBuffer, lpmat2);
	}

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABCFLOAT lpABCF) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharABCWidthsFloat(m_hDC, nFirstChar, nLastChar, lpABCF);
	}

	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharWidthFloat(m_hDC, nFirstChar, nLastChar, lpFloatBuffer);
	}

// Printer/Device Escape Functions
	int Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData)
	{
		ASSERT(m_hDC != NULL);
		return ::Escape(m_hDC, nEscape, nCount, lpszInData, lpOutData);
	}

	int Escape(int nEscape, int nInputSize, LPCSTR lpszInputData,
		int nOutputSize, LPSTR lpszOutputData)
	{
		ASSERT(m_hDC != NULL);
		return ::ExtEscape(m_hDC, nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);
	}

	int DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData)
	{
		ASSERT(m_hDC != NULL);
		return ::DrawEscape(m_hDC, nEscape, nInputSize, lpszInputData);
	}

	// Escape helpers
	int StartDoc(LPCTSTR lpszDocName)  // old Win3.0 version
	{
		DOCINFO di;
		ZeroMemory(&di, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = lpszDocName;
		return StartDoc(&di);
	}

	int StartDoc(LPDOCINFO lpDocInfo)
	{
		ASSERT(m_hDC != NULL);
		return ::StartDoc(m_hDC, lpDocInfo);
	}

	int StartPage()
	{
		ASSERT(m_hDC != NULL);
		return ::StartPage(m_hDC);
	}

	int EndPage()
	{
		ASSERT(m_hDC != NULL);
		return ::EndPage(m_hDC);
	}

	int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int))
	{
		ASSERT(m_hDC != NULL);
		return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn);
	}

	int AbortDoc()
	{
		ASSERT(m_hDC != NULL);
		return ::AbortDoc(m_hDC);
	}

	int EndDoc()
	{
		ASSERT(m_hDC != NULL);
		return ::EndDoc(m_hDC);
	}

// MetaFile Functions
	BOOL PlayMetaFile(HMETAFILE hMF)
	{
		ASSERT(m_hDC != NULL);
		if(::GetDeviceCaps(m_hDC, TECHNOLOGY) == DT_METAFILE)
		{
			// playing metafile in metafile, just use core windows API
			return ::PlayMetaFile(m_hDC, hMF);
		}

		// for special playback, lParam == pDC
		return ::EnumMetaFile(m_hDC, hMF, EnumMetaFileProc, (LPARAM)this);
	}

	BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds)
	{
		ASSERT(m_hDC != NULL);
		return ::PlayEnhMetaFile(m_hDC, hEnhMetaFile, lpBounds);
	}

	BOOL AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData) // can be used for enhanced metafiles only
	{
		ASSERT(m_hDC != NULL);
		return ::GdiComment(m_hDC, nDataSize, pCommentData);
	}

	// Special handling for metafile playback
	static int CALLBACK EnumMetaFileProc(HDC hDC, HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int nHandles, LPARAM lParam)
	{
		CDC* pDC = (CDC*)lParam;

		switch (pMetaRec->rdFunction)
		{
		case META_SETMAPMODE:
			pDC->SetMapMode((int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETWINDOWEXT:
			pDC->SetWindowExt((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETWINDOWORG:
			pDC->SetWindowOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETVIEWPORTEXT:
			pDC->SetViewportExt((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETVIEWPORTORG:
			pDC->SetViewportOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SCALEWINDOWEXT:
			pDC->ScaleWindowExt((int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2], 
				(int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SCALEVIEWPORTEXT:
			pDC->ScaleViewportExt((int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
				(int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_OFFSETVIEWPORTORG:
			pDC->OffsetViewportOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SAVEDC:
			pDC->SaveDC();
			break;
		case META_RESTOREDC:
			pDC->RestoreDC((int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETBKCOLOR:
			pDC->SetBkColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
			break;
		case META_SETTEXTCOLOR:
			pDC->SetTextColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
			break;

		// need to watch out for SelectObject(HFONT), for custom font mapping
		case META_SELECTOBJECT:
			{
				HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
				UINT nObjType = ::GetObjectType(hObject);
				if(nObjType == 0)
				{
					// object type is unknown, determine if it is a font
					HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
					HFONT hFontOld = (HFONT)::SelectObject(pDC->m_hDC, hStockFont);
					HGDIOBJ hObjOld = ::SelectObject(pDC->m_hDC, hObject);
					if(hObjOld == hStockFont)
					{
						// got the stock object back, so must be selecting a font
						pDC->SelectFont((HFONT)hObject);
						break;  // don't play the default record
					}
					else
					{
						// didn't get the stock object back, so restore everything
						::SelectObject(pDC->m_hDC, hFontOld);
						::SelectObject(pDC->m_hDC, hObjOld);
					}
					// and fall through to PlayMetaFileRecord...
				}
				else if(nObjType == OBJ_FONT)
				{
					// play back as CDCHandle::SelectFont(HFONT)
					pDC->SelectFont((HFONT)hObject);
					break;  // don't play the default record
				}
			}
			// fall through...

		default:
			::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
			break;
		}

		return 1;
	}

// Path Functions
	BOOL AbortPath()
	{
		ASSERT(m_hDC != NULL);
		return ::AbortPath(m_hDC);
	}

	BOOL BeginPath()
	{
		ASSERT(m_hDC != NULL);
		return ::BeginPath(m_hDC);
	}

	BOOL CloseFigure()
	{
		ASSERT(m_hDC != NULL);
		return ::CloseFigure(m_hDC);
	}

	BOOL EndPath()
	{
		ASSERT(m_hDC != NULL);
		return ::EndPath(m_hDC);
	}

	BOOL FillPath()
	{
		ASSERT(m_hDC != NULL);
		return ::FillPath(m_hDC);
	}

	BOOL FlattenPath()
	{
		ASSERT(m_hDC != NULL);
		return ::FlattenPath(m_hDC);
	}

	BOOL StrokeAndFillPath()
	{
		ASSERT(m_hDC != NULL);
		return ::StrokeAndFillPath(m_hDC);
	}

	BOOL StrokePath()
	{
		ASSERT(m_hDC != NULL);
		return ::StrokePath(m_hDC);
	}

	BOOL WidenPath()
	{
		ASSERT(m_hDC != NULL);
		return ::WidenPath(m_hDC);
	}

	BOOL GetMiterLimit(PFLOAT pfMiterLimit) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetMiterLimit(m_hDC, pfMiterLimit);
	}

	BOOL SetMiterLimit(float fMiterLimit)
	{
		ASSERT(m_hDC != NULL);
		return ::SetMiterLimit(m_hDC, fMiterLimit, NULL);
	}

	int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetPath(m_hDC, lpPoints, lpTypes, nCount);
	}

	BOOL SelectClipPath(int nMode)
	{
		ASSERT(m_hDC != NULL);
		return ::SelectClipPath(m_hDC, nMode);
	}

// Misc Helper Functions
	static HBRUSH PASCAL GetHalftoneBrush()
	{
		HBRUSH halftoneBrush = NULL;
		WORD grayPattern[8];
		for(int i = 0; i < 8; i++)
			grayPattern[i] = (WORD)(0x5555 << (i & 1));
		HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, &grayPattern);
		if(grayBitmap != NULL)
		{
			halftoneBrush = ::CreatePatternBrush(grayBitmap);
			DeleteObject(grayBitmap);
		}
		return halftoneBrush;
	}

	void DrawDragRect(LPCRECT lpRect, SIZE size, LPCRECT lpRectLast, SIZE sizeLast, HBRUSH hBrush = NULL, HBRUSH hBrushLast = NULL)
	{
		// first, determine the update region and select it
		CRgn rgnOutside;
		rgnOutside.CreateRectRgnIndirect(lpRect);
		RECT rect = *lpRect;
		::InflateRect(&rect, -size.cx, -size.cy);
		::IntersectRect(&rect, &rect, lpRect);
		CRgn rgnInside;
		rgnInside.CreateRectRgnIndirect(&rect);
		CRgn rgnNew;
		rgnNew.CreateRectRgn(0, 0, 0, 0);
		rgnNew.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

		HBRUSH hBrushOld = NULL;
		CBrush brushHalftone;
		if(hBrush == NULL)
			brushHalftone = hBrush = CDC::GetHalftoneBrush();
		if(hBrushLast == NULL)
			hBrushLast = hBrush;

		CRgn rgnLast;
		CRgn rgnUpdate;
		if(lpRectLast != NULL)
		{
			// find difference between new region and old region
			rgnLast.CreateRectRgn(0, 0, 0, 0);
			rgnOutside.SetRectRgn(lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->bottom);
			rect = *lpRectLast;
			::InflateRect(&rect, -sizeLast.cx, -sizeLast.cy);
			::IntersectRect(&rect, &rect, lpRectLast);
			rgnInside.SetRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			rgnLast.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

			// only diff them if brushes are the same
			if(hBrush == hBrushLast)
			{
				rgnUpdate.CreateRectRgn(0, 0, 0, 0);
				rgnUpdate.CombineRgn(rgnLast, rgnNew, RGN_XOR);
			}
		}
		if(hBrush != hBrushLast && lpRectLast != NULL)
		{
			// brushes are different -- erase old region first
			SelectClipRgn(rgnLast);
			GetClipBox(&rect);
			hBrushOld = SelectBrush(hBrushLast);
			PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
			SelectBrush(hBrushOld);
			hBrushOld = NULL;
		}

		// draw into the update/new region
		SelectClipRgn(rgnUpdate.IsNull() ? rgnNew : rgnUpdate);
		GetClipBox(&rect);
		hBrushOld = SelectBrush(hBrush);
		PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);

		// cleanup DC
		if(hBrushOld != NULL)
			SelectBrush(hBrushOld);
		SelectClipRgn(NULL);
	}

	void FillSolidRect(LPCRECT lpRect, COLORREF clr)
	{
		ASSERT(m_hDC != NULL);

		::SetBkColor(m_hDC, clr);
		::ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	}

	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
	{
		ASSERT(m_hDC != NULL);

		::SetBkColor(m_hDC, clr);
		RECT rect = { x, y, x + cx, y + cy };
		::ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	}

	void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
	{
		Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
	}

	void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
	{
		FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
		FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
		FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
		FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
	}

// DIB support
	int SetDIBitsToDevice(int x, int y, DWORD dwWidth, DWORD dwHeight, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse)
	{
		ASSERT(m_hDC != NULL);
		return ::SetDIBitsToDevice(m_hDC, x, y, dwWidth, dwHeight, xSrc, ySrc, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
	}

	int StretchDIBits(int x, int y, int nWidth, int nHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse, DWORD dwRop)
	{
		ASSERT(m_hDC != NULL);
		return ::StretchDIBits(m_hDC, x, y, nWidth, nHeight, xSrc, ySrc, nSrcWidth, nSrcHeight, lpvBits, lpbmi, uColorUse, dwRop);
	}

	UINT GetDIBColorTable(UINT uStartIndex, UINT cEntries, RGBQUAD* pColors) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetDIBColorTable(m_hDC, uStartIndex, cEntries, pColors);
	}

	UINT SetDIBColorTable(UINT uStartIndex, UINT cEntries, CONST RGBQUAD* pColors)
	{
		ASSERT(m_hDC != NULL);
		return ::SetDIBColorTable(m_hDC, uStartIndex, cEntries, pColors);
	}

// New for Windows 2000 only
#if (_WIN32_WINNT >= 0x0500)

	COLORREF GetDCPenColor() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetDCPenColor(m_hDC);
	}

	COLORREF SetDCPenColor(COLORREF clr)
	{
		ASSERT(m_hDC != NULL);
		return ::SetDCPenColor(m_hDC, clr);
	}

	COLORREF GetDCBrushColor() const
	{
		ASSERT(m_hDC != NULL);
		return ::GetDCBrushColor(m_hDC);
	}

	COLORREF SetDCBrushColor(COLORREF clr)
	{
		ASSERT(m_hDC != NULL);
		return ::SetDCBrushColor(m_hDC, clr);
	}

	DWORD GetFontUnicodeRanges(LPGLYPHSET lpgs) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetFontUnicodeRanges(m_hDC, lpgs);
	}

	DWORD GetGlyphIndices(LPCTSTR lpstr, int cch, LPWORD pgi, DWORD dwFlags) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetGlyphIndices(m_hDC, lpstr, cch, pgi, dwFlags);
	}

	BOOL GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextExtentPointI(m_hDC, pgiIn, cgi, lpSize);
	}

	BOOL GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetTextExtentExPointI(m_hDC, pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize);
	}

	BOOL GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharWidthI(m_hDC, giFirst, cgi, pgi, lpBuffer);
	}

	BOOL GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const
	{
		ASSERT(m_hDC != NULL);
		return ::GetCharABCWidthsI(m_hDC, giFirst, cgi, pgi, lpabc);
	}

#endif //(_WIN32_WINNT >= 0x0500)

// New for Windows 2000 and Windows 98
#if (WINVER >= 0x0500)

	BOOL ColorCorrectPalette(HPALETTE hPalette, DWORD dwFirstEntry, DWORD dwNumOfEntries)
	{
		ASSERT(m_hDC != NULL);
		return ::ColorCorrectPalette(m_hDC, hPalette, dwFirstEntry, dwNumOfEntries);
	}

#endif //(WINVER >= 0x0500)
};

/////////////////////////////////////////////////////////////////////////////
// CDC Helpers

class CPaintDC : public CDC
{
public:
// Data members
	HWND m_hWnd;
	PAINTSTRUCT m_ps;

// Constructor/destructor
	CPaintDC(HWND hWnd)
	{
		ASSERT(::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::BeginPaint(hWnd, &m_ps);
	}
	~CPaintDC()
	{
		ASSERT(m_hDC != NULL);
		ASSERT(::IsWindow(m_hWnd));
		::EndPaint(m_hWnd, &m_ps);
		Detach();
	}
};

class CClientDC : public CDC
{
public:
// Data members
	HWND m_hWnd;

// Constructor/destructor
	CClientDC(HWND hWnd)
	{
		ASSERT(hWnd == NULL || ::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::GetDC(hWnd);
	}
	~CClientDC()
	{
		ASSERT(m_hDC != NULL);
		::ReleaseDC(m_hWnd, Detach());
	}
};

class CWindowDC : public CDC
{
public:
// Data members
	HWND m_hWnd;

// Constructor/destructor
	CWindowDC(HWND hWnd)
	{
		ASSERT(hWnd == NULL || ::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::GetWindowDC(hWnd);
	}
	~CWindowDC()
	{
		ASSERT(m_hDC != NULL);
		::ReleaseDC(m_hWnd, Detach());
	}
};

/////////////////////////////////////////////////////////////////////////////
// Enhanced metafile support

class CEnhMetaFileInfo
{
public:
// Data members
	HENHMETAFILE m_hEMF;
	BYTE* m_pBits;
	TCHAR* m_pDesc;
	ENHMETAHEADER m_header;
	PIXELFORMATDESCRIPTOR m_pfd;

// Constructor/destructor
	CEnhMetaFileInfo(HENHMETAFILE hEMF) : m_pBits(NULL), m_pDesc(NULL), m_hEMF(hEMF)
	{ }

	~CEnhMetaFileInfo()
	{
		delete [] m_pBits;
		delete [] m_pDesc;
	}

// Operations
	BYTE* GetEnhMetaFileBits()
	{
		ASSERT(m_hEMF != NULL);
		UINT nBytes = ::GetEnhMetaFileBits(m_hEMF, 0, NULL);
		delete [] m_pBits;
		m_pBits = NULL;
		m_pBits = new BYTE[nBytes];
		if (m_pBits != NULL)
			::GetEnhMetaFileBits(m_hEMF, nBytes, m_pBits);
		return m_pBits;
	}

	LPTSTR GetEnhMetaFileDescription()
	{
		ASSERT(m_hEMF != NULL);
		UINT nLen = ::GetEnhMetaFileDescription(m_hEMF, 0, NULL);
		delete [] m_pDesc;
		m_pDesc = NULL;
		m_pDesc = new TCHAR[nLen];
		if (m_pDesc != NULL)
			nLen = ::GetEnhMetaFileDescription(m_hEMF, nLen, m_pDesc);
		return m_pDesc;
	}

	ENHMETAHEADER* GetEnhMetaFileHeader()
	{
		ASSERT(m_hEMF != NULL);
		ZeroMemory(&m_header, sizeof(m_header));
		m_header.iType = EMR_HEADER;
		m_header.nSize = sizeof(ENHMETAHEADER);
		UINT n = ::GetEnhMetaFileHeader(m_hEMF, sizeof(ENHMETAHEADER), &m_header);
		return (n != 0) ? &m_header : NULL;
	}

	PIXELFORMATDESCRIPTOR* GetEnhMetaFilePixelFormat()
	{
		ASSERT(m_hEMF != NULL);
		ZeroMemory(&m_pfd, sizeof(m_pfd));
		UINT n = ::GetEnhMetaFilePixelFormat(m_hEMF, sizeof(m_pfd), &m_pfd);
		return (n != 0) ? &m_pfd : NULL;
	}
};


class CEnhMetaFile
{
public:
// Data members
	HENHMETAFILE m_hEMF;
	BOOL m_bManaged;

// Constructor/destructor
	CEnhMetaFile(HENHMETAFILE hEMF = NULL) : m_hEMF(hEMF)
	{
		m_bManaged = FALSE;
	}

	~CEnhMetaFile()
	{
		if(m_bManaged && m_hEMF != NULL)
			DeleteObject();
	}

// Operations
	CEnhMetaFile& operator=(HENHMETAFILE hEMF)
	{
		Attach(hEMF);
		return *this;
	}

	void Attach(HENHMETAFILE hEMF)
	{
		if(m_bManaged && m_hEMF != NULL)
			DeleteObject();
		m_hEMF = hEMF;
	}

	HENHMETAFILE Detach()
	{
		HENHMETAFILE hEMF = m_hEMF;
		m_hEMF = NULL;
		return hEMF;
	}

	operator HENHMETAFILE() const { return m_hEMF; }

	bool IsNull() const { return (m_hEMF == NULL); }

	BOOL DeleteObject()
	{
		ASSERT(m_hEMF != NULL);
		if(m_hEMF == NULL)
			return FALSE;

		BOOL bRet = ::DeleteEnhMetaFile(m_hEMF);
		m_hEMF = NULL;
		return bRet;
	}

	UINT GetEnhMetaFileBits(UINT cbBuffer, LPBYTE lpbBuffer) const
	{
		ASSERT(m_hEMF != NULL);
		return ::GetEnhMetaFileBits(m_hEMF, cbBuffer, lpbBuffer);
	}

	UINT GetEnhMetaFileDescription(UINT cchBuffer, LPTSTR lpszDescription) const
	{
		ASSERT(m_hEMF != NULL);
		return ::GetEnhMetaFileDescription(m_hEMF, cchBuffer, lpszDescription);
	}

	UINT GetEnhMetaFileHeader(LPENHMETAHEADER lpemh) const
	{
		ASSERT(m_hEMF != NULL);
		lpemh->iType = EMR_HEADER;
		lpemh->nSize = sizeof(ENHMETAHEADER);
		return ::GetEnhMetaFileHeader(m_hEMF, sizeof(ENHMETAHEADER), lpemh);
	}

	UINT GetEnhMetaFilePaletteEntries(UINT cEntries, LPPALETTEENTRY lppe) const
	{
		ASSERT(m_hEMF != NULL);
		return ::GetEnhMetaFilePaletteEntries(m_hEMF, cEntries, lppe);
	}

	UINT GetEnhMetaFilePixelFormat(DWORD cbBuffer, PIXELFORMATDESCRIPTOR* ppfd) const
	{
		ASSERT(m_hEMF != NULL);
		return ::GetEnhMetaFilePixelFormat(m_hEMF, cbBuffer, ppfd);
	}
};

class CEnhMetaFileDC : public CDC
{
public:
// Constructor/destructor
	CEnhMetaFileDC()
	{
	}

	CEnhMetaFileDC(HDC hdc, LPCRECT lpRect)
	{
		Create(hdc, NULL, lpRect, NULL);
		ASSERT(m_hDC != NULL);
	}

	CEnhMetaFileDC(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect, LPCTSTR lpDescription)
	{
		Create(hdcRef, lpFilename, lpRect, lpDescription);
		ASSERT(m_hDC != NULL);
	}

	~CEnhMetaFileDC()
	{
		HENHMETAFILE hEMF = Close();
		if (hEMF != NULL)
			::DeleteEnhMetaFile(hEMF);
	}

// Operations
	void Create(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect, LPCTSTR lpDescription)
	{
		ASSERT(m_hDC == NULL);
		m_hDC = ::CreateEnhMetaFile(hdcRef, lpFilename, lpRect, lpDescription);
	}

	HENHMETAFILE Close()
	{
		HENHMETAFILE hEMF = NULL;
		if (m_hDC != NULL)
		{
			hEMF = ::CloseEnhMetaFile(m_hDC);
			m_hDC = NULL;
		}
		return hEMF;
	}
};

}; // namespace TWL

#endif // __GDI_H__
