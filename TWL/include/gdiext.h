//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __GDIEXT_H__
#define __GDIEXT_H__

#define GE_INLINE inline

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

/// Part 1: Windows GDI API overload extensions

typedef struct tagRECT2
{
	LONG left;
	LONG top;
	LONG width;
	LONG height;
} RECT2, *PRECT2, *LPRECT2;

// 1.
/*
BOOL AlphaBlend(
  HDC hdcDest,                 // handle to destination DC
  int nXOriginDest,            // x-coord of upper-left corner
  int nYOriginDest,            // y-coord of upper-left corner
  int nWidthDest,              // destination width
  int nHeightDest,             // destination height
  HDC hdcSrc,                  // handle to source DC
  int nXOriginSrc,             // x-coord of upper-left corner
  int nYOriginSrc,             // y-coord of upper-left corner
  int nWidthSrc,               // source width
  int nHeightSrc,              // source height
  BLENDFUNCTION blendFunction  // alpha-blending function
);
//*/
GE_INLINE
BOOL AlphaBlend(
  HDC hdcDest,
  POINT ptOriginDest,
  SIZE sizOriginDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  SIZE sizOriginSrc,
  BLENDFUNCTION blendFunction
)
{
	return ::AlphaBlend(hdcDest, ptOriginDest.x, ptOriginDest.y, sizOriginDest.cx, sizOriginDest.cy,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, sizOriginSrc.cx, sizOriginSrc.cy, blendFunction);
}
GE_INLINE
BOOL AlphaBlend(
  HDC hdcDest,
  POINT ptOriginDest,
  int nWidthDest,
  int nHeightDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  int nWidthSrc,
  int nHeightSrc,
  BLENDFUNCTION blendFunction
)
{
	return ::AlphaBlend(hdcDest, ptOriginDest.x, ptOriginDest.y, nWidthDest, nHeightDest,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, nWidthSrc, nHeightSrc, blendFunction);
}
GE_INLINE
BOOL AlphaBlend(
  HDC hdcDest,
  int nXOriginDest,
  int nYOriginDest,
  SIZE sizOriginDest,
  HDC hdcSrc,
  int nXOriginSrc,
  int nYOriginSrc,
  SIZE sizOriginSrc,
  BLENDFUNCTION blendFunction
)
{
	return ::AlphaBlend(hdcDest, nXOriginDest, nYOriginDest, sizOriginDest.cx, sizOriginDest.cy,
		hdcSrc, nXOriginSrc, nYOriginSrc, sizOriginSrc.cx, sizOriginSrc.cy, blendFunction);
}
GE_INLINE
BOOL AlphaBlend(
  HDC hdcDest,
  PRECT prcDest,
  HDC hdcSrc,
  PRECT prcSrc,
  BLENDFUNCTION blendFunction
)
{
	return ::AlphaBlend(hdcDest, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top,
		hdcSrc, prcSrc->left, prcSrc->top, prcSrc->right - prcSrc->left, prcSrc->bottom - prcSrc->top, blendFunction);
}
GE_INLINE
BOOL AlphaBlend(
  HDC hdcDest,
  PRECT2 prcDest,
  HDC hdcSrc,
  PRECT2 prcSrc,
  BLENDFUNCTION blendFunction
)
{
	return ::AlphaBlend(hdcDest, prcDest->left, prcDest->top, prcDest->width, prcDest->height,
		hdcSrc, prcSrc->left, prcSrc->top, prcSrc->width, prcSrc->height, blendFunction);
}

// 2.
/*
BOOL BitBlt(
  HDC hdcDest, // handle to destination DC
  int nXDest,  // x-coord of destination upper-left corner
  int nYDest,  // y-coord of destination upper-left corner
  int nWidth,  // width of destination rectangle
  int nHeight, // height of destination rectangle
  HDC hdcSrc,  // handle to source DC
  int nXSrc,   // x-coordinate of source upper-left corner
  int nYSrc,   // y-coordinate of source upper-left corner
  DWORD dwRop  // raster operation code
);
// */
GE_INLINE
BOOL BitBlt(
  HDC hdcDest,
  POINT ptDest,
  int nWidth,
  int nHeight,
  HDC hdcSrc,
  POINT ptSrc,
  DWORD dwRop
)
{
	return ::BitBlt(hdcDest, ptDest.x, ptDest.y, nWidth, nHeight, hdcSrc, ptSrc.x, ptSrc.y, dwRop);
}
GE_INLINE
BOOL BitBlt(
  HDC hdcDest,
  int nXDest,
  int nYDest,
  SIZE size,
  HDC hdcSrc,
  int nXSrc,
  int nYSrc,
  DWORD dwRop
)
{
	return ::BitBlt(hdcDest, nXDest, nYDest, size.cx, size.cy, hdcSrc, nXSrc, nYSrc, dwRop);
}
GE_INLINE
BOOL BitBlt(
  HDC hdcDest,
  POINT ptDest,
  SIZE size,
  HDC hdcSrc,
  POINT ptSrc,
  DWORD dwRop
)
{
	return ::BitBlt(hdcDest, ptDest.x, ptDest.y, size.cx, size.cy, hdcSrc, ptSrc.x, ptSrc.y, dwRop);
}
GE_INLINE
BOOL BitBlt(
  HDC hdcDest,
  PRECT prcDest,
  HDC hdcSrc,
  POINT ptSrc,
  DWORD dwRop
)
{
	return ::BitBlt(hdcDest, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top, hdcSrc, ptSrc.x, ptSrc.y, dwRop);
}

// 3.
/*
HBITMAP CreateBitmap(
  int nWidth,         // bitmap width, in pixels
  int nHeight,        // bitmap height, in pixels
  UINT cPlanes,       // number of color planes
  UINT cBitsPerPel,   // number of bits to identify color
  CONST VOID *lpvBits // color data array
);
// */
GE_INLINE
HBITMAP CreateBitmap(
  SIZE size,
  UINT cPlanes,
  UINT cBitsPerPel,
  CONST VOID *lpvBits
)
{
	return ::CreateBitmap(size.cx, size.cy, cPlanes, cBitsPerPel, lpvBits);
}

// 4.
/*
HBITMAP CreateCompatibleBitmap(
  HDC hdc,        // handle to DC
  int nWidth,     // width of bitmap, in pixels
  int nHeight     // height of bitmap, in pixels
);
// */
GE_INLINE
HBITMAP CreateCompatibleBitmap(
  HDC hdc,
  SIZE size
)
{
	return ::CreateCompatibleBitmap(hdc, size.cx, size.cy);
}

// 5.
/*
HBITMAP CreateDiscardableBitmap(
  HDC hdc,     // handle to DC
  int nWidth,  // bitmap width
  int nHeight  // bitmap height
);
// */
GE_INLINE
HBITMAP CreateDiscardableBitmap(
  HDC hdc,
  SIZE size
)
{
	return ::CreateDiscardableBitmap(hdc, size.cx, size.cy);
}

// 6.
/*
BOOL ExtFloodFill(
  HDC hdc,          // handle to DC
  int nXStart,      // starting x-coordinate 
  int nYStart,      // starting y-coordinate 
  COLORREF crColor, // fill color
  UINT fuFillType   // fill type
);
// */
GE_INLINE
BOOL ExtFloodFill(
  HDC hdc,
  POINT pt,
  COLORREF crColor,
  UINT fuFillType
)
{
	return ::ExtFloodFill(hdc, pt.x, pt.y, crColor, fuFillType);
}

// 7.
/*
BOOL FloodFill(
  HDC hdc,          // handle to DC
  int nXStart,      // starting x-coordinate
  int nYStart,      // starting y-coordinate
  COLORREF crFill   // fill color
);
// */
GE_INLINE
BOOL FloodFill(
  HDC hdc,
  POINT pt,
  COLORREF crFill
)
{
	return ::FloodFill(hdc, pt.x, pt.y, crFill);
}

// 9.
/*
COLORREF GetPixel(
  HDC hdc,    // handle to DC
  int nXPos,  // x-coordinate of pixel
  int nYPos   // y-coordinate of pixel
);
// */
GE_INLINE
COLORREF GetPixel(
  HDC hdc,
  POINT pt
)
{
	return ::GetPixel(hdc, pt.x, pt.y);
}

// 10.
/*
BOOL MaskBlt(
  HDC hdcDest,     // handle to destination DC
  int nXDest,      // x-coord of destination upper-left corner
  int nYDest,      // y-coord of destination upper-left corner 
  int nWidth,      // width of source and destination
  int nHeight,     // height of source and destination
  HDC hdcSrc,      // handle to source DC
  int nXSrc,       // x-coord of upper-left corner of source
  int nYSrc,       // y-coord of upper-left corner of source
  HBITMAP hbmMask, // handle to monochrome bit mask
  int xMask,       // horizontal offset into mask bitmap
  int yMask,       // vertical offset into mask bitmap
  DWORD dwRop      // raster operation code
);
// */
GE_INLINE
BOOL MaskBlt(
  HDC hdcDest,
  POINT ptDest,
  int nWidth,
  int nHeight,
  HDC hdcSrc,
  POINT ptSrc,
  HBITMAP hbmMask,
  POINT ptMask,
  DWORD dwRop
)
{
	return ::MaskBlt(hdcDest, ptDest.x, ptDest.y, nWidth, nHeight,
		hdcSrc, ptSrc.x, ptSrc.y,
		hbmMask, ptMask.x, ptMask.y, dwRop);
}
GE_INLINE
BOOL MaskBlt(
  HDC hdcDest,
  int nXDest,
  int nYDest,
  SIZE size,
  HDC hdcSrc,
  int nXSrc,
  int nYSrc,
  HBITMAP hbmMask,
  int xMask,
  int yMask,
  DWORD dwRop
)
{
	return ::MaskBlt(hdcDest, nXDest, nYDest, size.cx, size.cy,
		hdcSrc, nXSrc, nYSrc,
		hbmMask, xMask, yMask, dwRop);
}
GE_INLINE
BOOL MaskBlt(
  HDC hdcDest,
  POINT ptDest,
  SIZE size,
  HDC hdcSrc,
  POINT ptSrc,
  HBITMAP hbmMask,
  POINT ptMask,
  DWORD dwRop
)
{
	return ::MaskBlt(hdcDest, ptDest.x, ptDest.y, size.cx, size.cy,
		hdcSrc, ptSrc.x, ptSrc.y,
		hbmMask, ptMask.x, ptMask.y, dwRop);
}
GE_INLINE
BOOL MaskBlt(
  HDC hdcDest,
  PRECT prcDest,
  HDC hdcSrc,
  POINT ptSrc,
  HBITMAP hbmMask,
  POINT ptMask,
  DWORD dwRop
)
{
	return ::MaskBlt(hdcDest, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top,
		hdcSrc, ptSrc.x, ptSrc.y,
		hbmMask, ptMask.x, ptMask.y, dwRop);
}

// 11.
/*
BOOL PlgBlt(
  HDC hdcDest,          // handle to destination DC
  CONST POINT *lpPoint, // destination vertices
  HDC hdcSrc,           // handle to source DC
  int nXSrc,            // x-coord of source upper-left corner
  int nYSrc,            // y-coord of source upper-left corner
  int nWidth,           // width of source rectangle
  int nHeight,          // height of source rectangle
  HBITMAP hbmMask,      // handle to bitmask
  int xMask,            // x-coord of bitmask upper-left corner
  int yMask             // y-coord of bitmask upper-left corner
);
// */
GE_INLINE
BOOL PlgBlt(
  HDC hdcDest,
  CONST POINT *lpPoint,
  HDC hdcSrc,
  POINT ptSrc,
  int nWidth,
  int nHeight,
  HBITMAP hbmMask,
  POINT ptMask
)
{
	return ::PlgBlt(hdcDest, lpPoint, hdcSrc, ptSrc.x, ptSrc.y, nWidth, nHeight, hbmMask, ptMask.x, ptMask.y);
}
GE_INLINE
BOOL PlgBlt(
  HDC hdcDest,
  CONST POINT *lpPoint,
  HDC hdcSrc,
  int nXSrc,
  int nYSrc,
  SIZE size,
  HBITMAP hbmMask,
  int xMask,
  int yMask
)
{
	return ::PlgBlt(hdcDest, lpPoint, hdcSrc, nXSrc, nYSrc, size.cx, size.cy, hbmMask, xMask, yMask);
}
GE_INLINE
BOOL PlgBlt(
  HDC hdcDest,
  CONST POINT *lpPoint,
  HDC hdcSrc,
  POINT ptSrc,
  SIZE size,
  HBITMAP hbmMask,
  POINT ptMask
)
{
	return ::PlgBlt(hdcDest, lpPoint, hdcSrc, ptSrc.x, ptSrc.y, size.cx, size.cy, hbmMask, ptMask.x, ptMask.y);
}
GE_INLINE
BOOL PlgBlt(
  HDC hdcDest,
  CONST POINT *lpPoint,
  HDC hdcSrc,
  PRECT prcSrc,
  HBITMAP hbmMask,
  POINT ptMask
)
{
	return ::PlgBlt(hdcDest, lpPoint, hdcSrc, prcSrc->left, prcSrc->top, prcSrc->right - prcSrc->left, prcSrc->bottom - prcSrc->top, hbmMask, ptMask.x, ptMask.y);
}

// 12. (return?)
/*
BOOL SetBitmapDimensionEx(
  HBITMAP hBitmap,  // handle to bitmap
  int nWidth,       // bitmap width in .01-mm units
  int nHeight,      // bitmap height in .01-mm units
  LPSIZE lpSize     // original dimensions
);
// */
GE_INLINE
BOOL SetBitmapDimensionEx(
  HBITMAP hBitmap,
  SIZE size,
  LPSIZE lpSize = NULL
)
{
	return ::SetBitmapDimensionEx(hBitmap, size.cx, size.cy, lpSize);
}
GE_INLINE
BOOL SetBitmapDimension(
  HBITMAP hBitmap,
  int nWidth,
  int nHeight,
  LPSIZE lpSize = NULL
)
{
	return ::SetBitmapDimensionEx(hBitmap, nWidth, nHeight, lpSize);
}
GE_INLINE
BOOL SetBitmapDimension(
  HBITMAP hBitmap,
  SIZE size,
  LPSIZE lpSize = NULL
)
{
	return ::SetBitmapDimensionEx(hBitmap, size.cx, size.cy, lpSize);
}

#define SetBitmapSize SetBitmapDimension

// 13.
/*
int SetDIBitsToDevice(
  HDC hdc,                 // handle to DC
  int XDest,               // x-coord of destination upper-left corner
  int YDest,               // y-coord of destination upper-left corner 
  DWORD dwWidth,           // source rectangle width
  DWORD dwHeight,          // source rectangle height
  int XSrc,                // x-coord of source lower-left corner
  int YSrc,                // y-coord of source lower-left corner
  UINT uStartScan,         // first scan line in array
  UINT cScanLines,         // number of scan lines
  CONST VOID *lpvBits,     // array of DIB bits
  CONST BITMAPINFO *lpbmi, // bitmap information
  UINT fuColorUse          // RGB or palette indexes
);
// */
GE_INLINE
int SetDIBitsToDevice(
  HDC hdc,
  POINT ptDest,
  DWORD dwWidth,
  DWORD dwHeight,
  POINT ptSrc,
  UINT uStartScan,
  UINT cScanLines,
  CONST VOID *lpvBits,
  CONST BITMAPINFO *lpbmi,
  UINT fuColorUse
)
{
	return ::SetDIBitsToDevice(hdc, ptDest.x, ptDest.y, dwWidth, dwHeight, ptSrc.x, ptSrc.y,
		uStartScan, cScanLines, lpvBits, lpbmi, fuColorUse);
}
GE_INLINE
int SetDIBitsToDevice(
  HDC hdc,
  int XDest,
  int YDest,
  SIZE size,
  int XSrc,
  int YSrc,
  UINT uStartScan,
  UINT cScanLines,
  CONST VOID *lpvBits,
  CONST BITMAPINFO *lpbmi,
  UINT fuColorUse
)
{
	return ::SetDIBitsToDevice(hdc, XDest, YDest, size.cx, size.cy, XSrc, YSrc,
		uStartScan, cScanLines, lpvBits, lpbmi, fuColorUse);
}
GE_INLINE
int SetDIBitsToDevice(
  HDC hdc,
  POINT ptDest,
  SIZE size,
  POINT ptSrc,
  UINT uStartScan,
  UINT cScanLines,
  CONST VOID *lpvBits,
  CONST BITMAPINFO *lpbmi,
  UINT fuColorUse
)
{
	return ::SetDIBitsToDevice(hdc, ptDest.x, ptDest.y, size.cx, size.cy, ptSrc.x, ptSrc.y,
		uStartScan, cScanLines, lpvBits, lpbmi, fuColorUse);
}
GE_INLINE
int SetDIBitsToDevice(
  HDC hdc,
  PRECT prcDest,
  POINT ptSrc,
  UINT uStartScan,
  UINT cScanLines,
  CONST VOID *lpvBits,
  CONST BITMAPINFO *lpbmi,
  UINT fuColorUse
)
{
	return ::SetDIBitsToDevice(hdc, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top, ptSrc.x, ptSrc.y,
		uStartScan, cScanLines, lpvBits, lpbmi, fuColorUse);
}

// 14.
/*
COLORREF SetPixel(
  HDC hdc,           // handle to DC
  int X,             // x-coordinate of pixel
  int Y,             // y-coordinate of pixel
  COLORREF crColor   // pixel color
);
// */
GE_INLINE
COLORREF SetPixel(
  HDC hdc,
  POINT pt,
  COLORREF crColor
)
{
	return ::SetPixel(hdc, pt.x, pt.y, crColor);
}

// 15.
/*
BOOL SetPixelV(
  HDC hdc,           // handle to device context
  int X,             // x-coordinate of pixel
  int Y,             // y-coordinate of pixel
  COLORREF crColor   // new pixel color
);
// */
GE_INLINE
BOOL SetPixelV(
  HDC hdc,
  POINT pt,
  COLORREF crColor
)
{
	return ::SetPixelV(hdc, pt.x, pt.y, crColor);
}

// 16.
/*
BOOL StretchBlt(
  HDC hdcDest,      // handle to destination DC
  int nXOriginDest, // x-coord of destination upper-left corner
  int nYOriginDest, // y-coord of destination upper-left corner
  int nWidthDest,   // width of destination rectangle
  int nHeightDest,  // height of destination rectangle
  HDC hdcSrc,       // handle to source DC
  int nXOriginSrc,  // x-coord of source upper-left corner
  int nYOriginSrc,  // y-coord of source upper-left corner
  int nWidthSrc,    // width of source rectangle
  int nHeightSrc,   // height of source rectangle
  DWORD dwRop       // raster operation code
);
// */
GE_INLINE
BOOL StretchBlt(
  HDC hdcDest,
  POINT ptOriginDest,
  int nWidthDest,
  int nHeightDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  int nWidthSrc,
  int nHeightSrc,
  DWORD dwRop
)
{
	return ::StretchBlt(hdcDest, ptOriginDest.x, ptOriginDest.y, nWidthDest, nHeightDest,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, nWidthSrc, nHeightSrc, dwRop);
}
GE_INLINE
BOOL StretchBlt(
  HDC hdcDest,
  int nXOriginDest,
  int nYOriginDest,
  SIZE sizDest,
  HDC hdcSrc,
  int nXOriginSrc,
  int nYOriginSrc,
  SIZE sizSrc,
  DWORD dwRop
)
{
	return ::StretchBlt(hdcDest, nXOriginDest, nYOriginDest, sizDest.cx, sizDest.cy,
		hdcSrc, nXOriginSrc, nYOriginSrc, sizSrc.cx, sizSrc.cy, dwRop);
}
GE_INLINE
BOOL StretchBlt(
  HDC hdcDest,
  POINT ptOriginDest,
  SIZE sizDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  SIZE sizSrc,
  DWORD dwRop
)
{
	return ::StretchBlt(hdcDest, ptOriginDest.x, ptOriginDest.y, sizDest.cx, sizDest.cy,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, sizSrc.cx, sizSrc.cy, dwRop);
}
GE_INLINE
BOOL StretchBlt(
  HDC hdcDest,
  PRECT prcDest,
  HDC hdcSrc,
  PRECT prcSrc,
  DWORD dwRop
)
{
	return ::StretchBlt(hdcDest, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top,
		hdcSrc, prcSrc->left, prcSrc->top, prcSrc->right - prcSrc->left, prcSrc->bottom - prcSrc->top, dwRop);
}

// 17.
/*
int StretchDIBits(
  HDC hdc,                      // handle to DC
  int XDest,                    // x-coord of destination upper-left corner
  int YDest,                    // y-coord of destination upper-left corner
  int nDestWidth,               // width of destination rectangle
  int nDestHeight,              // height of destination rectangle
  int XSrc,                     // x-coord of source upper-left corner
  int YSrc,                     // y-coord of source upper-left corner
  int nSrcWidth,                // width of source rectangle
  int nSrcHeight,               // height of source rectangle
  CONST VOID *lpBits,           // bitmap bits
  CONST BITMAPINFO *lpBitsInfo, // bitmap data
  UINT iUsage,                  // usage options
  DWORD dwRop                   // raster operation code
);
// */
GE_INLINE
int StretchDIBits(
  HDC hdc,
  POINT ptDest,
  int nDestWidth,
  int nDestHeight,
  POINT ptSrc,
  int nSrcWidth,
  int nSrcHeight,
  CONST VOID *lpBits,
  CONST BITMAPINFO *lpBitsInfo,
  UINT iUsage,
  DWORD dwRop
)
{
	return ::StretchDIBits(hdc, ptDest.x, ptDest.y, nDestWidth, nDestHeight, ptSrc.x, ptSrc.y, nSrcWidth, nSrcHeight,
		lpBits, lpBitsInfo, iUsage, dwRop);
}
GE_INLINE
int StretchDIBits(
  HDC hdc,
  int XDest,
  int YDest,
  SIZE sizDest,
  int XSrc,
  int YSrc,
  SIZE sizSrc,
  CONST VOID *lpBits,
  CONST BITMAPINFO *lpBitsInfo,
  UINT iUsage,
  DWORD dwRop
)
{
	return ::StretchDIBits(hdc, XDest, YDest, sizDest.cx, sizDest.cy, XSrc, YSrc, sizSrc.cx, sizSrc.cy,
		lpBits, lpBitsInfo, iUsage, dwRop);
}
GE_INLINE
int StretchDIBits(
  HDC hdc,
  POINT ptDest,
  SIZE sizDest,
  POINT ptSrc,
  SIZE sizSrc,
  CONST VOID *lpBits,
  CONST BITMAPINFO *lpBitsInfo,
  UINT iUsage,
  DWORD dwRop
)
{
	return ::StretchDIBits(hdc, ptDest.x, ptDest.y, sizDest.cx, sizDest.cy, ptSrc.x, ptSrc.y, sizSrc.cx, sizSrc.cy,
		lpBits, lpBitsInfo, iUsage, dwRop);
}
GE_INLINE
int StretchDIBits(
  HDC hdc,
  PRECT prcDest,
  PRECT prcSrc,
  CONST VOID *lpBits,
  CONST BITMAPINFO *lpBitsInfo,
  UINT iUsage,
  DWORD dwRop
)
{
	return ::StretchDIBits(hdc, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top,
		prcSrc->left, prcSrc->top, prcSrc->right - prcSrc->left, prcSrc->bottom - prcSrc->top,
		lpBits, lpBitsInfo, iUsage, dwRop);
}

// 18.
/*
BOOL TransparentBlt(
  HDC hdcDest,        // handle to destination DC
  int nXOriginDest,   // x-coord of destination upper-left corner
  int nYOriginDest,   // y-coord of destination upper-left corner
  int nWidthDest,     // width of destination rectangle
  int hHeightDest,    // height of destination rectangle
  HDC hdcSrc,         // handle to source DC
  int nXOriginSrc,    // x-coord of source upper-left corner
  int nYOriginSrc,    // y-coord of source upper-left corner
  int nWidthSrc,      // width of source rectangle
  int nHeightSrc,     // height of source rectangle
  UINT crTransparent  // color to make transparent
);
// */
GE_INLINE
BOOL TransparentBlt(
  HDC hdcDest,
  POINT ptOriginDest,
  int nWidthDest,
  int hHeightDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  int nWidthSrc,
  int nHeightSrc,
  UINT crTransparent
)
{
	return ::TransparentBlt(hdcDest, ptOriginDest.x, ptOriginDest.y, nWidthDest, hHeightDest,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, nWidthSrc, nHeightSrc, crTransparent);
}
GE_INLINE
BOOL TransparentBlt(
  HDC hdcDest,
  int nXOriginDest,
  int nYOriginDest,
  SIZE sizDest,
  HDC hdcSrc,
  int nXOriginSrc,
  int nYOriginSrc,
  SIZE sizSrc,
  UINT crTransparent
)
{
	return ::TransparentBlt(hdcDest, nXOriginDest, nYOriginDest, sizDest.cx, sizDest.cy,
		hdcSrc, nXOriginSrc, nYOriginSrc, sizSrc.cx, sizSrc.cy, crTransparent);
}
GE_INLINE
BOOL TransparentBlt(
  HDC hdcDest,
  POINT ptOriginDest,
  SIZE sizDest,
  HDC hdcSrc,
  POINT ptOriginSrc,
  SIZE sizSrc,
  UINT crTransparent
)
{
	return ::TransparentBlt(hdcDest, ptOriginDest.x, ptOriginDest.y, sizDest.cx, sizDest.cy,
		hdcSrc, ptOriginSrc.x, ptOriginSrc.y, sizSrc.cx, sizSrc.cy, crTransparent);
}
GE_INLINE
BOOL TransparentBlt(
  HDC hdcDest,
  PRECT prcDest,
  HDC hdcSrc,
  PRECT prcSrc,
  UINT crTransparent
)
{
	return ::TransparentBlt(hdcDest, prcDest->left, prcDest->top, prcDest->right - prcDest->left, prcDest->bottom - prcDest->top,
		hdcSrc, prcSrc->left, prcSrc->top, prcSrc->right - prcSrc->left, prcSrc->bottom - prcSrc->top, crTransparent);
}

// 20. 
/*
BOOL PatBlt(
  HDC hdc,      // handle to DC
  int nXLeft,   // x-coord of upper-left rectangle corner
  int nYLeft,   // y-coord of upper-left rectangle corner
  int nWidth,   // width of rectangle
  int nHeight,  // height of rectangle
  DWORD dwRop   // raster operation code
);
// */
GE_INLINE
BOOL PatBlt(
  HDC hdc,
  POINT pt,
  SIZE size,
  DWORD dwRop
)
{
	return ::PatBlt(hdc, pt.x, pt.y, size.cx, size.cy, dwRop);
}
GE_INLINE
BOOL PatBlt(
  HDC hdc,
  PRECT prc,
  DWORD dwRop
)
{
	return ::PatBlt(hdc, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, dwRop);
}

// 21. (return?)
/*
BOOL SetBrushOrgEx(
  HDC hdc,       // handle to device context
  int nXOrg,     // x-coord of new origin
  int nYOrg,     // y-coord of new origin
  LPPOINT lppt   // points to previous brush origin
);
// */
GE_INLINE
BOOL SetBrushOrgEx(
  HDC hdc,
  POINT ptOrg,
  LPPOINT lppt = NULL
)
{
	return ::SetBrushOrgEx(hdc, ptOrg.x, ptOrg.y, lppt);
}
GE_INLINE
BOOL SetBrushOrg(
  HDC hdc,
  int nXOrg,
  int nYOrg,
  LPPOINT lppt = NULL
)
{
	return ::SetBrushOrgEx(hdc, nXOrg, nYOrg, lppt);
}
GE_INLINE
BOOL SetBrushOrg(
  HDC hdc,
  POINT ptOrg,
  LPPOINT lppt = NULL
)
{
	return ::SetBrushOrgEx(hdc, ptOrg.x, ptOrg.y, lppt);
}

// 22.
/*
int ExcludeClipRect(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner
  int nTopRect,    // y-coord of upper-left corner
  int nRightRect,  // x-coord of lower-right corner
  int nBottomRect  // y-coord of lower-right corner
);
// */
GE_INLINE
int ExcludeClipRect(
  HDC hdc,
  PRECT prc
)
{
	return ::ExcludeClipRect(hdc, prc->left, prc->top, prc->right, prc->bottom);
}

// 24.
/*
int IntersectClipRect(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner
  int nTopRect,    // y-coord of upper-left corner
  int nRightRect,  // x-coord of lower-right corner
  int nBottomRect  // y-coord of lower-right corner
);
// */
GE_INLINE
int IntersectClipRect(
  HDC hdc,
  PRECT prc
)
{
	return ::IntersectClipRect(hdc, prc->left, prc->top, prc->right, prc->bottom);
}

// 25.
/*
int OffsetClipRgn(
  HDC hdc,       // handle to DC
  int nXOffset,  // offset along x-axis
  int nYOffset   // offset along y-axis
);
// */
GE_INLINE
int OffsetClipRgn(
  HDC hdc,
  POINT pt
)
{
	return ::OffsetClipRgn(hdc, pt.x, pt.y);
}
GE_INLINE
int OffsetClipRgn(
  HDC hdc,
  SIZE size
)
{
	return ::OffsetClipRgn(hdc, size.cx, size.cy);
}

// 26.
/*
BOOL PtVisible(
  HDC hdc,  // handle to DC
  int X,    // x-coordinate of point
  int Y     // y-coordinate of point
);
// */
GE_INLINE
BOOL PtVisible(
  HDC hdc,
  POINT pt
)
{
	return ::PtVisible(hdc, pt.x, pt.y);
}

// 28.
/*
BOOL ClientToScreen(
  HWND hWnd,       // handle to window
  LPPOINT lpPoint  // screen coordinates
);
// */
GE_INLINE
BOOL ClientToScreen(
  HWND hWnd,
  LPRECT lpRect
)
{
	return ::ClientToScreen(hWnd, (LPPOINT)lpRect) && ::ClientToScreen(hWnd, ((LPPOINT)lpRect) + 1);
}

// 29.
/*
BOOL DPtoLP(
  HDC hdc,           // handle to device context
  LPPOINT lpPoints,  // array of points
  int nCount         // count of points in array
);
// */
GE_INLINE
BOOL DPtoLP(
  HDC hdc,
  LPRECT lpRect
)
{
	return ::DPtoLP(hdc, (LPPOINT)lpRect, 2);
}

// 35.
/*
BOOL LPtoDP(
  HDC hdc,           // handle to device context
  LPPOINT lpPoints,  // array of points
  int nCount         // count of points in array
);
// */
GE_INLINE
BOOL LPtoDP(
  HDC hdc,
  LPRECT lpRect
)
{
	return ::DPtoLP(hdc, (LPPOINT)lpRect, 2);
}

// 36. (return?)
/*
BOOL OffsetViewportOrgEx(
  HDC hdc,         // handle to device context
  int nXOffset,    // horizontal offset
  int nYOffset,    // vertical offset
  LPPOINT lpPoint  // original origin
);
// */
GE_INLINE
BOOL OffsetViewportOrgEx(
  HDC hdc,
  POINT ptOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetViewportOrgEx(hdc, ptOffset.x, ptOffset.y, lpPoint);
}
GE_INLINE
BOOL OffsetViewportOrgEx(
  HDC hdc,
  SIZE sizOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetViewportOrgEx(hdc, sizOffset.cx, sizOffset.cy, lpPoint);
}
GE_INLINE
BOOL OffsetViewportOrg(
  HDC hdc,
  int nXOffset,
  int nYOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetViewportOrgEx(hdc, nXOffset, nYOffset, lpPoint);
}
GE_INLINE
BOOL OffsetViewportOrg(
  HDC hdc,
  POINT ptOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetViewportOrgEx(hdc, ptOffset.x, ptOffset.y, lpPoint);
}
GE_INLINE
BOOL OffsetViewportOrg(
  HDC hdc,
  SIZE sizOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetViewportOrgEx(hdc, sizOffset.cx, sizOffset.cy, lpPoint);
}

// 37. (return?)
/*
BOOL OffsetWindowOrgEx(
  HDC hdc,          // handle to device context
  int nXOffset,     // horizontal offset
  int nYOffset,     // vertical offset
  LPPOINT lpPoint   // original origin
);
// */
GE_INLINE
BOOL OffsetWindowOrgEx(
  HDC hdc,
  POINT ptOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetWindowOrgEx(hdc, ptOffset.x, ptOffset.y, lpPoint);
}
GE_INLINE
BOOL OffsetWindowOrgEx(
  HDC hdc,
  SIZE sizOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetWindowOrgEx(hdc, sizOffset.cx, sizOffset.cy, lpPoint);
}
GE_INLINE
BOOL OffsetWindowOrg(
  HDC hdc,
  int nXOffset,
  int nYOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetWindowOrgEx(hdc, nXOffset, nYOffset, lpPoint);
}
GE_INLINE
BOOL OffsetWindowOrg(
  HDC hdc,
  POINT ptOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetWindowOrgEx(hdc, ptOffset.x, ptOffset.y, lpPoint);
}
GE_INLINE
BOOL OffsetWindowOrg(
  HDC hdc,
  SIZE sizOffset,
  LPPOINT lpPoint = NULL
)
{
	return ::OffsetWindowOrgEx(hdc, sizOffset.cx, sizOffset.cy, lpPoint);
}

// 40.
/*
BOOL ScreenToClient(
  HWND hWnd,        // handle to window
  LPPOINT lpPoint   // screen coordinates
);
// */
GE_INLINE
BOOL ScreenToClient(
  HWND hWnd,
  LPRECT lpRect
)
{
	return ::ScreenToClient(hWnd, (LPPOINT)lpRect) && ::ScreenToClient(hWnd, ((LPPOINT)lpRect) + 1);
}

// 41. (return?)
/*
BOOL SetViewportExtEx(
  HDC hdc,       // handle to device context
  int nXExtent,  // new horizontal viewport extent
  int nYExtent,  // new vertical viewport extent
  LPSIZE lpSize  // original viewport extent
);
// */
GE_INLINE
BOOL SetViewportExtEx(
  HDC hdc,
  SIZE sizExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetViewportExtEx(hdc, sizExtent.cx, sizExtent.cy, lpSize);
}
GE_INLINE
BOOL SetViewportExt(
  HDC hdc,
  int nXExtent,
  int nYExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetViewportExtEx(hdc, nXExtent, nYExtent, lpSize);
}
GE_INLINE
BOOL SetViewportExt(
  HDC hdc,
  SIZE sizExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetViewportExtEx(hdc, sizExtent.cx, sizExtent.cy, lpSize);
}

// 42. (return?)
/*
BOOL SetViewportOrgEx(
  HDC hdc,        // handle to device context
  int X,          // new x-coordinate of viewport origin
  int Y,          // new y-coordinate of viewport origin
  LPPOINT lpPoint // original viewport origin
);
// */
GE_INLINE
BOOL SetViewportOrgEx(
  HDC hdc,
  POINT ptOrg,
  LPPOINT lpPoint = NULL
)
{
	return ::SetViewportOrgEx(hdc, ptOrg.x, ptOrg.y, lpPoint);
}
GE_INLINE
BOOL SetViewportOrg(
  HDC hdc,
  int X,
  int Y,
  LPPOINT lpPoint = NULL
)
{
	return ::SetViewportOrgEx(hdc, X, Y, lpPoint);
}
GE_INLINE
BOOL SetViewportOrg(
  HDC hdc,
  POINT ptOrg,
  LPPOINT lpPoint = NULL
)
{
	return ::SetViewportOrgEx(hdc, ptOrg.x, ptOrg.y, lpPoint);
}

// 43. (return?)
/*
BOOL SetWindowExtEx(
  HDC hdc,       // handle to device context
  int nXExtent,  // new horizontal window extent
  int nYExtent,  // new vertical window extent
  LPSIZE lpSize  // original window extent
);
// */
GE_INLINE
BOOL SetWindowExtEx(
  HDC hdc,
  SIZE sizExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetWindowExtEx(hdc, sizExtent.cx, sizExtent.cy, lpSize);
}
GE_INLINE
BOOL SetWindowExt(
  HDC hdc,
  int nXExtent,
  int nYExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetWindowExtEx(hdc, nXExtent, nYExtent, lpSize);
}
GE_INLINE
BOOL SetWindowExt(
  HDC hdc,
  SIZE sizExtent,
  LPSIZE lpSize = NULL
)
{
	return ::SetWindowExtEx(hdc, sizExtent.cx, sizExtent.cy, lpSize);
}

// 44. (return?)
/*
BOOL SetWindowOrgEx(
  HDC hdc,        // handle to device context
  int X,          // new x-coordinate of window origin
  int Y,          // new y-coordinate of window origin
  LPPOINT lpPoint // original window origin
);
// */
GE_INLINE
BOOL SetWindowOrgEx(
  HDC hdc,
  POINT pt,
  LPPOINT lpPoint = NULL
)
{
	return ::SetWindowOrgEx(hdc, pt.x, pt.y, lpPoint);
}
GE_INLINE
BOOL SetWindowOrg(
  HDC hdc,
  int X,
  int Y,
  LPPOINT lpPoint = NULL
)
{
	return ::SetWindowOrgEx(hdc, X, Y, lpPoint);
}
GE_INLINE
BOOL SetWindowOrg(
  HDC hdc,
  POINT pt,
  LPPOINT lpPoint = NULL
)
{
	return ::SetWindowOrgEx(hdc, pt.x, pt.y, lpPoint);
}

// 46.
/*
BOOL Chord(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect, // y-coord of lower-right corner of rectangle
  int nXRadial1,   // x-coord of first radial's endpoint
  int nYRadial1,   // y-coord of first radial's endpoint
  int nXRadial2,   // x-coord of second radial's endpoint
  int nYRadial2    // y-coord of second radial's endpoint
);
// */
GE_INLINE
BOOL Chord(
  HDC hdc,
  PRECT prc,
  int nXRadial1,
  int nYRadial1,
  int nXRadial2,
  int nYRadial2
)
{
	return ::Chord(hdc, prc->left, prc->top, prc->right, prc->bottom,
		nXRadial1, nYRadial1, nXRadial2, nYRadial2);
}
GE_INLINE
BOOL Chord(
  HDC hdc,
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::Chord(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}
GE_INLINE
BOOL Chord(
  HDC hdc,
  PRECT prc,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::Chord(hdc, prc->left, prc->top, prc->right, prc->bottom,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}

// 47.
/*
BOOL Ellipse(
  HDC hdc,        // handle to DC
  int nLeftRect,  // x-coord of upper-left corner of rectangle
  int nTopRect,   // y-coord of upper-left corner of rectangle
  int nRightRect, // x-coord of lower-right corner of rectangle
  int nBottomRect // y-coord of lower-right corner of rectangle
);
// */
GE_INLINE
BOOL Ellipse(
  HDC hdc,
  PRECT prc
)
{
	return ::Ellipse(hdc, prc->left, prc->top, prc->right, prc->bottom);
}

// 51.
/*
BOOL Pie(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect, // y-coord of lower-right corner of rectangle
  int nXRadial1,   // x-coord of first radial's endpoint
  int nYRadial1,   // y-coord of first radial's endpoint
  int nXRadial2,   // x-coord of second radial's endpoint
  int nYRadial2    // y-coord of second radial's endpoint
);
// */
GE_INLINE
BOOL Pie(
  HDC hdc,
  PRECT prc,
  int nXRadial1,
  int nYRadial1,
  int nXRadial2,
  int nYRadial2
)
{
	return ::Pie(hdc, prc->left, prc->top, prc->right, prc->bottom,
		nXRadial1, nYRadial1, nXRadial2, nYRadial2);
}
GE_INLINE
BOOL Pie(
  HDC hdc,
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::Pie(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}
GE_INLINE
BOOL Pie(
  HDC hdc,
  PRECT prc,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::Pie(hdc, prc->left, prc->top, prc->right, prc->bottom,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}

// 52.
/*
BOOL Rectangle(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect  // y-coord of lower-right corner of rectangle
);
// */
GE_INLINE
BOOL Rectangle(
  HDC hdc,
  PRECT prc
)
{
	return ::Rectangle(hdc, prc->left, prc->top, prc->right, prc->bottom);
}

// 53.
/*
BOOL RoundRect(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect, // y-coord of lower-right corner of rectangle
  int nWidth,      // width of ellipse
  int nHeight      // height of ellipse
);
// */
GE_INLINE
BOOL RoundRect(
  HDC hdc,
  PRECT prc,
  int nWidth,
  int nHeight
)
{
	return ::RoundRect(hdc, prc->left, prc->top, prc->right, prc->bottom, nWidth, nHeight);
}
GE_INLINE
BOOL RoundRect(
  HDC hdc,
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  SIZE size
)
{
	return ::RoundRect(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect, size.cx, size.cy);
}
GE_INLINE
BOOL RoundRect(
  HDC hdc,
  PRECT prc,
  SIZE size
)
{
	return ::RoundRect(hdc, prc->left, prc->top, prc->right, prc->bottom, size.cx, size.cy);
}

// 59.
/*
LONG TabbedTextOut(
  HDC hDC,                         // handle to DC
  int X,                           // x-coord of start
  int Y,                           // y-coord of start
  LPCTSTR lpString,                // character string
  int nCount,                      // number of characters
  int nTabPositions,               // number of tabs in array
  CONST LPINT lpnTabStopPositions, // array of tab positions
  int nTabOrigin                   // start of tab expansion
);
// */
GE_INLINE
LONG TabbedTextOut(
  HDC hDC,
  POINT pt,
  LPCTSTR lpString,
  int nCount,
  int nTabPositions,
  CONST LPINT lpnTabStopPositions,
  int nTabOrigin
)
{
	return ::TabbedTextOut(hDC, pt.x, pt.y, lpString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
}

// 60.
/*
BOOL TextOut(
  HDC hdc,           // handle to DC
  int nXStart,       // x-coordinate of starting position
  int nYStart,       // y-coordinate of starting position
  LPCTSTR lpString,  // character string
  int cbString       // number of characters
);
// */
GE_INLINE
BOOL TextOut(
  HDC hdc,
  POINT pt,
  LPCTSTR lpString,
  int cbString
)
{
	return ::TextOut(hdc, pt.x, pt.y, lpString, cbString);
}

// 61.
/*
BOOL AngleArc(
  HDC hdc,            // handle to device context
  int X,              // x-coordinate of circle's center
  int Y,              // y-coordinate of circle's center
  DWORD dwRadius,     // circle's radius
  FLOAT eStartAngle,  // arc's start angle
  FLOAT eSweepAngle   // arc's sweep angle
);
// */
GE_INLINE
BOOL AngleArc(
  HDC hdc,
  POINT pt,
  DWORD dwRadius,
  FLOAT eStartAngle,
  FLOAT eSweepAngle
)
{
	return ::AngleArc(hdc, pt.x, pt.y, dwRadius, eStartAngle, eSweepAngle);
}

// 62.
/*
BOOL Arc(
  HDC hdc,         // handle to device context
  int nLeftRect,   // x-coord of rectangle's upper-left corner
  int nTopRect,    // y-coord of rectangle's upper-left corner
  int nRightRect,  // x-coord of rectangle's lower-right corner
  int nBottomRect, // y-coord of rectangle's lower-right corner
  int nXStartArc,  // x-coord of first radial ending point
  int nYStartArc,  // y-coord of first radial ending point
  int nXEndArc,    // x-coord of second radial ending point
  int nYEndArc     // y-coord of second radial ending point
);
// */
GE_INLINE
BOOL Arc(
  HDC hdc,
  PRECT prc,
  int nXStartArc,
  int nYStartArc,
  int nXEndArc,
  int nYEndArc
)
{
	return ::Arc(hdc, prc->left, prc->top, prc->right, prc->bottom,
		nXStartArc, nYStartArc, nXEndArc, nYEndArc);
}
GE_INLINE
BOOL Arc(
  HDC hdc,
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  POINT ptStartArc,
  POINT ptEndArc
)
{
	return ::Arc(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect,
		ptStartArc.x, ptStartArc.y, ptEndArc.x, ptEndArc.y);
}
GE_INLINE
BOOL Arc(
  HDC hdc,
  PRECT prc,
  POINT ptStartArc,
  POINT ptEndArc
)
{
	return ::Arc(hdc, prc->left, prc->top, prc->right, prc->bottom,
		ptStartArc.x, ptStartArc.y, ptEndArc.x, ptEndArc.y);
}

// 63.
/*
BOOL ArcTo(
  HDC hdc,          // handle to device context
  int nLeftRect,    // x-coord of rectangle's upper-left corner
  int nTopRect,     // y-coord of rectangle's upper-left corner
  int nRightRect,   // x-coord of rectangle's lower-right corner
  int nBottomRect,  // y-coord of rectangle's lower-right corner
  int nXRadial1,    // x-coord of first radial ending point
  int nYRadial1,    // y-coord of first radial ending point
  int nXRadial2,    // x-coord of second radial ending point
  int nYRadial2     // y-coord of second radial ending point
);
// */
GE_INLINE
BOOL ArcTo(
  HDC hdc,
  PRECT prc,
  int nXRadial1,
  int nYRadial1,
  int nXRadial2,
  int nYRadial2
)
{
	return ::ArcTo(hdc, prc->left, prc->top, prc->right, prc->bottom,
		nXRadial1, nYRadial1, nXRadial2, nYRadial2);
}
GE_INLINE
BOOL ArcTo(
  HDC hdc,
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::ArcTo(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}
GE_INLINE
BOOL ArcTo(
  HDC hdc,
  PRECT prc,
  POINT ptRadial1,
  POINT ptRadial2
)
{
	return ::ArcTo(hdc, prc->left, prc->top, prc->right, prc->bottom,
		ptRadial1.x, ptRadial1.y, ptRadial2.x, ptRadial2.y);
}

// 64.
/*
BOOL LineDDA(
  int nXStart,             // x-coordinate of starting point
  int nYStart,             // y-coordinate of starting point
  int nXEnd,               // x-coordinate of ending point
  int nYEnd,               // y-coordinate of ending point
  LINEDDAPROC lpLineFunc,  // callback function
  LPARAM lpData            // application-defined data
);
// */
GE_INLINE
BOOL LineDDA(
  POINT ptStart,
  POINT ptEnd,
  LINEDDAPROC lpLineFunc,
  LPARAM lpData
)
{
	return ::LineDDA(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, lpLineFunc, lpData);
}

// 65.
/*
BOOL LineTo(
  HDC hdc,    // device context handle
  int nXEnd,  // x-coordinate of ending point
  int nYEnd   // y-coordinate of ending point
);
// */
GE_INLINE
BOOL LineTo(
  HDC hdc,
  POINT pt
)
{
	return ::LineTo(hdc, pt.x, pt.y);
}

// 66.
/*
BOOL MoveToEx(
  HDC hdc,          // handle to device context
  int X,            // x-coordinate of new current position
  int Y,            // y-coordinate of new current position
  LPPOINT lpPoint   // old current position
);
// */
GE_INLINE
BOOL MoveTo(
  HDC hdc,
  int X,
  int Y,
  LPPOINT lpPoint = NULL
)
{
	return ::MoveToEx(hdc, X, Y, lpPoint);
}
GE_INLINE
BOOL MoveToEx(
  HDC hdc,
  POINT pt,
  LPPOINT lpPoint = NULL
)
{
	return ::MoveToEx(hdc, pt.x, pt.y, lpPoint);
}
GE_INLINE
BOOL MoveTo(
  HDC hdc,
  POINT pt,
  LPPOINT lpPoint = NULL
)
{
	return MoveToEx(hdc, pt, lpPoint);
}

// 67.
/*
HMONITOR MonitorFromPoint(
  POINT pt,      // point 
  DWORD dwFlags  // determine return value
);
// */
GE_INLINE
HMONITOR MonitorFromPoint(
  int x,
  int y,
  DWORD dwFlags
)
{
	POINT pt = { x, y };
	return ::MonitorFromPoint(pt, dwFlags);
}

// 73.
/*
BOOL DrawState(
  HDC hdc,                     // handle to device context
  HBRUSH hbr,                  // handle to brush
  DRAWSTATEPROC lpOutputFunc,  // callback function
  LPARAM lData,                // image information
  WPARAM wData,                // more image information
  int x,                       // horizontal location
  int y,                       // vertical location
  int cx,                      // image width
  int cy,                      // image height
  UINT fuFlags                 // image type and state
);
// */
GE_INLINE
BOOL DrawState(
  HDC hdc,
  HBRUSH hbr,
  DRAWSTATEPROC lpOutputFunc,
  LPARAM lData,
  WPARAM wData,
  POINT pt,
  int cx,
  int cy,
  UINT fuFlags
)
{
	return ::DrawState(hdc, hbr, lpOutputFunc, lData, wData, pt.x, pt.y, cx, cy, fuFlags);
}
GE_INLINE
BOOL DrawState(
  HDC hdc,
  HBRUSH hbr,
  DRAWSTATEPROC lpOutputFunc,
  LPARAM lData,
  WPARAM wData,
  int x,
  int y,
  SIZE size,
  UINT fuFlags
)
{
	return ::DrawState(hdc, hbr, lpOutputFunc, lData, wData, x, y, size.cx, size.cy, fuFlags);
}
GE_INLINE
BOOL DrawState(
  HDC hdc,
  HBRUSH hbr,
  DRAWSTATEPROC lpOutputFunc,
  LPARAM lData,
  WPARAM wData,
  POINT pt,
  SIZE size,
  UINT fuFlags
)
{
	return ::DrawState(hdc, hbr, lpOutputFunc, lData, wData, pt.x, pt.y, size.cx, size.cy, fuFlags);
}
GE_INLINE
BOOL DrawState(
  HDC hdc,
  HBRUSH hbr,
  DRAWSTATEPROC lpOutputFunc,
  LPARAM lData,
  WPARAM wData,
  PRECT prc,
  UINT fuFlags
)
{
	return ::DrawState(hdc, hbr, lpOutputFunc, lData, wData, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, fuFlags);
}

// 77.
/*
BOOL GrayString(
  HDC hDC,                      // handle to DC
  HBRUSH hBrush,                // handle to the brush
  GRAYSTRINGPROC lpOutputFunc,  // callback function
  LPARAM lpData,                // application-defined data
  int nCount,                   // number of characters
  int X,                        // horizontal position
  int Y,                        // vertical position
  int nWidth,                   // width
  int nHeight                   // height
);
// */
GE_INLINE
BOOL GrayString(
  HDC hDC,
  HBRUSH hBrush,
  GRAYSTRINGPROC lpOutputFunc,
  LPARAM lpData,
  int nCount,
  POINT pt,
  int nWidth,
  int nHeight
)
{
	return ::GrayString(hDC, hBrush, lpOutputFunc, lpData, nCount, pt.x, pt.y, nWidth, nHeight);
}
GE_INLINE
BOOL GrayString(
  HDC hDC,
  HBRUSH hBrush,
  GRAYSTRINGPROC lpOutputFunc,
  LPARAM lpData,
  int nCount,
  int X,
  int Y,
  SIZE size
)
{
	return ::GrayString(hDC, hBrush, lpOutputFunc, lpData, nCount, X, Y, size.cx, size.cy);
}
GE_INLINE
BOOL GrayString(
  HDC hDC,
  HBRUSH hBrush,
  GRAYSTRINGPROC lpOutputFunc,
  LPARAM lpData,
  int nCount,
  POINT pt,
  SIZE size
)
{
	return ::GrayString(hDC, hBrush, lpOutputFunc, lpData, nCount, pt.x, pt.y, size.cx, size.cy);
}
GE_INLINE
BOOL GrayString(
  HDC hDC,
  HBRUSH hBrush,
  GRAYSTRINGPROC lpOutputFunc,
  LPARAM lpData,
  int nCount,
  PRECT prc
)
{
	return ::GrayString(hDC, hBrush, lpOutputFunc, lpData, nCount, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top);
}

// 84.
/*
BOOL InflateRect(
  LPRECT lprc,  // rectangle
  int dx,       // amount to adjust width
  int dy        // amount to adjust height
);
// */
GE_INLINE
BOOL InflateRect(
  LPRECT lprc,
  SIZE size
)
{
	return ::InflateRect(lprc, size.cx, size.cy);
}
GE_INLINE
BOOL InflateRect(
  LPRECT lprc,
  int dxLeft,
  int dyTop,
  int dxRight,
  int dyBottom
)
{
	if(lprc == NULL)
		return FALSE;

	lprc->left -= dxLeft;
	lprc->top -= dyTop;
	lprc->right += dxRight;
	lprc->bottom += dyBottom;

	return TRUE;
}
GE_INLINE
BOOL DeflateRect(
  LPRECT lprc,  // rectangle
  int dx,       // amount to adjust width
  int dy        // amount to adjust height
)
{
	return ::InflateRect(lprc, -dx, -dy);
}
GE_INLINE
BOOL DeflateRect(
  LPRECT lprc,
  SIZE size
)
{
	return ::InflateRect(lprc, -size.cx, -size.cy);
}
GE_INLINE
BOOL DeflateRect(
  LPRECT lprc,
  int dxLeft,
  int dyTop,
  int dxRight,
  int dyBottom
)
{
	if(lprc == NULL)
		return FALSE;

	lprc->left += dxLeft;
	lprc->top += dyTop;
	lprc->right -= dxRight;
	lprc->bottom -= dyBottom;

	return TRUE;
}

// 87.
/*
BOOL OffsetRect(
  LPRECT lprc,  // rectangle
  int dx,       // horizontal offset
  int dy        // vertical offset
);
// */
GE_INLINE
BOOL OffsetRect(
  LPRECT lprc,
  POINT pt
)
{
	return ::OffsetRect(lprc, pt.x, pt.y);
}
GE_INLINE
BOOL OffsetRect(
  LPRECT lprc,
  SIZE siz
)
{
	return ::OffsetRect(lprc, siz.cx, siz.cy);
}

// 92.
/*
HRGN CreateEllipticRgn(
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect  // y-coord of lower-right corner of rectangle
);
// */
GE_INLINE
HRGN CreateEllipticRgn(
  PRECT prc
)
{
	return ::CreateEllipticRgn(prc->left, prc->top, prc->right, prc->bottom);
}

// 93.
/*
HRGN CreateRectRgn(
  int nLeftRect,   // x-coordinate of upper-left corner
  int nTopRect,    // y-coordinate of upper-left corner
  int nRightRect,  // x-coordinate of lower-right corner
  int nBottomRect  // y-coordinate of lower-right corner
);
// */
GE_INLINE
HRGN CreateRectRgn(
  PRECT prc
)
{
	return ::CreateRectRgn(prc->left, prc->top, prc->right, prc->bottom);
}

// 94.
/*
HRGN CreateRoundRectRgn(
  int nLeftRect,      // x-coordinate of upper-left corner
  int nTopRect,       // y-coordinate of upper-left corner
  int nRightRect,     // x-coordinate of lower-right corner
  int nBottomRect,    // y-coordinate of lower-right corner
  int nWidthEllipse,  // height of ellipse
  int nHeightEllipse  // width of ellipse
);
// */
GE_INLINE
HRGN CreateRoundRectRgn(
  PRECT prc,
  int nWidthEllipse,
  int nHeightEllipse
)
{
	return ::CreateRoundRectRgn(prc->left, prc->top, prc->right, prc->bottom,
		nWidthEllipse, nHeightEllipse);
}
GE_INLINE
HRGN CreateRoundRectRgn(
  int nLeftRect,
  int nTopRect,
  int nRightRect,
  int nBottomRect,
  SIZE size
)
{
	return ::CreateRoundRectRgn(nLeftRect, nTopRect, nRightRect, nBottomRect,
		size.cx, size.cy);
}
GE_INLINE
HRGN CreateRoundRectRgn(
  PRECT prc,
  SIZE size
)
{
	return ::CreateRoundRectRgn(prc->left, prc->top, prc->right, prc->bottom,
		size.cx, size.cy);
}

// 96.
/*
int OffsetRgn(
  HRGN hrgn,     // handle to region
  int nXOffset,  // offset along x-axis
  int nYOffset   // offset along y-axis
);
// */
GE_INLINE
int OffsetRgn(
  HRGN hrgn,
  POINT pt
)
{
	return ::OffsetRgn(hrgn, pt.x, pt.y);
}
GE_INLINE
int OffsetRgn(
  HRGN hrgn,
  SIZE size
)
{
	return ::OffsetRgn(hrgn, size.cx, size.cy);
}

// 97.
/*
BOOL PtInRegion(
  HRGN hrgn,  // handle to region
  int X,      // x-coordinate of point
  int Y       // y-coordinate of point
);
// */
GE_INLINE
BOOL PtInRegion(
  HRGN hrgn,
  POINT pt
)
{
	return ::PtInRegion(hrgn, pt.x, pt.y);
}

// 99.
/*
BOOL SetRectRgn(
  HRGN hrgn,       // handle to region
  int nLeftRect,   // x-coordinate of upper-left corner of rectangle
  int nTopRect,    // y-coordinate of upper-left corner of rectangle
  int nRightRect,  // x-coordinate of lower-right corner of rectangle
  int nBottomRect  // y-coordinate of lower-right corner of rectangle
);
// */
GE_INLINE
BOOL SetRectRgn(
  HRGN hrgn,
  PRECT prc
)
{
	return ::SetRectRgn(hrgn, prc->left, prc->top, prc->right, prc->bottom);
}

//////////////////////////////////////////////////////////////////////////

// 8.
/*
BOOL GetBitmapDimensionEx(
  HBITMAP hBitmap,     // handle to bitmap
  LPSIZE lpDimension   // dimensions
);
// */
GE_INLINE
SIZE GetBitmapDimensionEx(
  HBITMAP hBitmap
)
{
	SIZE size = { 0, 0 };
	::GetBitmapDimensionEx(hBitmap, &size);
	return size;
}
GE_INLINE
BOOL GetBitmapDimension(
  HBITMAP hBitmap,
  LPSIZE lpDimension
)
{
	return ::GetBitmapDimensionEx(hBitmap, lpDimension);
}
GE_INLINE
SIZE GetBitmapDimension(
  HBITMAP hBitmap
)
{
	SIZE size = { 0, 0 };
	::GetBitmapDimensionEx(hBitmap, &size);
	return size;
}

#define GetBitmapSize GetBitmapDimension

// 19.
/*
BOOL GetBrushOrgEx(
  HDC hdc,       // handle to DC
  LPPOINT lppt   // coordinates of origin
);
// */
GE_INLINE
POINT GetBrushOrgEx(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetBrushOrgEx(hdc, &pt);
	return pt;
}
GE_INLINE
BOOL GetBrushOrg(
  HDC hdc,
  LPPOINT lppt
)
{
	return ::GetBrushOrgEx(hdc, lppt);
}
GE_INLINE
POINT GetBrushOrg(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetBrushOrgEx(hdc, &pt);
	return pt;
}

// 30.
/*
BOOL GetCurrentPositionEx(
  HDC hdc,        // handle to device context
  LPPOINT lpPoint // current position
);
// */
GE_INLINE
POINT GetCurrentPositionEx(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetCurrentPositionEx(hdc, &pt);
	return pt;
}
GE_INLINE
BOOL GetCurrentPosition(
  HDC hdc,
  LPPOINT lpPoint
)
{
	return ::GetCurrentPositionEx(hdc, lpPoint);
}
GE_INLINE
POINT GetCurrentPosition(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetCurrentPositionEx(hdc, &pt);
	return pt;
}

// 31.
/*
BOOL GetViewportExtEx(
  HDC hdc,      // handle to device context
  LPSIZE lpSize // viewport dimensions
);
// */
GE_INLINE
SIZE GetViewportExtEx(
  HDC hdc
)
{
	SIZE size = { 0, 0 };
	::GetViewportExtEx(hdc, &size);
	return size;
}
GE_INLINE
BOOL GetViewportExt(
  HDC hdc,
  LPSIZE lpSize
)
{
	return ::GetViewportExtEx(hdc, lpSize);
}
GE_INLINE
SIZE GetViewportExt(
  HDC hdc
)
{
	SIZE size = { 0, 0 };
	::GetViewportExtEx(hdc, &size);
	return size;
}

// 32.
/*
BOOL GetViewportOrgEx(
  HDC hdc,        // handle to device context
  LPPOINT lpPoint // viewport origin
);
// */
GE_INLINE
POINT GetViewportOrgEx(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetViewportOrgEx(hdc, &pt);
	return pt;
}
GE_INLINE
BOOL GetViewportOrg(
  HDC hdc,
  LPPOINT lpPoint
)
{
	return ::GetViewportOrgEx(hdc, lpPoint);
}
GE_INLINE
POINT GetViewportOrg(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetViewportOrgEx(hdc, &pt);
	return pt;
}

// 33.
/*
BOOL GetWindowExtEx(
  HDC hdc,        // handle to device context
  LPSIZE lpSize   // window extents
);
// */
GE_INLINE
SIZE GetWindowExtEx(
  HDC hdc
)
{
	SIZE size = { 0, 0 };
	::GetWindowExtEx(hdc, &size);
	return size;
}
GE_INLINE
BOOL GetWindowExt(
  HDC hdc,
  LPSIZE lpSize
)
{
	return ::GetWindowExtEx(hdc, lpSize);
}
GE_INLINE
SIZE GetWindowExt(
  HDC hdc
)
{
	SIZE size = { 0, 0 };
	::GetWindowExtEx(hdc, &size);
	return size;
}

// 34.
/*
BOOL GetWindowOrgEx(
  HDC hdc,         // handle to device context
  LPPOINT lpPoint  // window origin
);
// */
GE_INLINE
POINT GetWindowOrgEx(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetWindowOrgEx(hdc, &pt);
	return pt;
}
GE_INLINE
BOOL GetWindowOrg(
  HDC hdc,
  LPPOINT lpPoint
)
{
	return ::GetWindowOrgEx(hdc, lpPoint);
}
GE_INLINE
POINT GetWindowOrg(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetWindowOrgEx(hdc, &pt);
	return pt;
}

// 38. (return?)
/*
BOOL ScaleViewportExtEx(
  HDC hdc,        // handle to device context
  int Xnum,       // horizontal multiplicand
  int Xdenom,     // horizontal divisor
  int Ynum,       // vertical multiplicand
  int Ydenom,     // vertical divisor
  LPSIZE lpSize   // previous viewport extents
);
// */
GE_INLINE
SIZE ScaleViewportExtEx(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom
)
{
	SIZE size = { 0, 0 };
	::ScaleViewportExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, &size);
	return size;
}
GE_INLINE
BOOL ScaleViewportExt(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom,
  LPSIZE lpSize
)
{
	return ::ScaleViewportExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, lpSize);
}
GE_INLINE
SIZE ScaleViewportExt(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom
)
{
	SIZE size = { 0, 0 };
	::ScaleViewportExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, &size);
	return size;
}

// 39. (return?)
/*
BOOL ScaleWindowExtEx(
  HDC hdc,        // handle to device context
  int Xnum,       // horizontal multiplicand
  int Xdenom,     // horizontal divisor
  int Ynum,       // vertical multiplicand
  int Ydenom,     // vertical divisor
  LPSIZE lpSize   // previous window extents
);
// */
GE_INLINE
SIZE ScaleWindowExtEx(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom
)
{
	SIZE size = { 0, 0 };
	::ScaleWindowExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, &size);
	return size;
}
GE_INLINE
BOOL ScaleWindowExt(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom,
  LPSIZE lpSize
)
{
	return ::ScaleWindowExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, lpSize);
}
GE_INLINE
SIZE ScaleWindowExt(
  HDC hdc,
  int Xnum,
  int Xdenom,
  int Ynum,
  int Ydenom
)
{
	SIZE size = { 0, 0 };
	::ScaleWindowExtEx(hdc, Xnum, Xdenom, Ynum, Ydenom, &size);
	return size;
}

// 45.
/*
BOOL GetDCOrgEx(
  HDC hdc,          // handle to a DC
  LPPOINT lpPoint   // translation origin
);
// */
GE_INLINE
POINT GetDCOrgEx(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetDCOrgEx(hdc, &pt);
	return pt;
}
GE_INLINE
BOOL GetDCOrg(
  HDC hdc,
  LPPOINT lpPoint
)
{
	return ::GetDCOrgEx(hdc, lpPoint);
}
GE_INLINE
POINT GetDCOrg(
  HDC hdc
)
{
	POINT pt = { 0, 0 };
	::GetDCOrgEx(hdc, &pt);
	return pt;
}

// 54.
/*
BOOL GetTextExtentExPoint(
  HDC hdc,         // handle to DC
  LPCTSTR lpszStr, // character string
  int cchString,   // number of characters
  int nMaxExtent,  // maximum width of formatted string
  LPINT lpnFit,    // maximum number of characters
  LPINT alpDx,     // array of partial string widths
  LPSIZE lpSize    // string dimensions
);
// */
GE_INLINE
SIZE GetTextExtentExPoint(
  HDC hdc,
  LPCTSTR lpszStr,
  int cchString,
  int nMaxExtent,
  LPINT lpnFit,
  LPINT alpDx
)
{
	SIZE size = { 0, 0 };
	::GetTextExtentExPoint(hdc, lpszStr, cchString, nMaxExtent, lpnFit, alpDx, &size);
	return size;
}

#if (_WIN32_WINNT >= 0x0500)
// 55.
/*
BOOL GetTextExtentExPointI(
  HDC hdc,         // handle to DC
  LPWORD pgiIn,    // array of glyph indices
  int cgi,         // number of glyphs in array
  int nMaxExtent,  // maximum width of formatted string
  LPINT lpnFit,    // maximum number of characters
  LPINT alpDx,     // array of partial string widths
  LPSIZE lpSize    // string dimensions
);
// */
GE_INLINE
SIZE GetTextExtentExPointI(
  HDC hdc,
  LPWORD pgiIn,
  int cgi,
  int nMaxExtent,
  LPINT lpnFit,
  LPINT alpDx
)
{
	SIZE size = { 0, 0 };
	::GetTextExtentExPointI(hdc, pgiIn, cgi, nMaxExtent, lpnFit, alpDx, &size);
	return size;
}
#endif // _WIN32_WINNT >= 0x0500

// 56.
/*
BOOL GetTextExtentPoint(
  HDC hdc,           // handle to DC
  LPCTSTR lpString,  // text string
  int cbString,      // number of characters in string
  LPSIZE lpSize      // string size
);
// */
GE_INLINE
SIZE GetTextExtentPoint(
  HDC hdc,
  LPCTSTR lpString,
  int cbString
)
{
	SIZE size = { 0, 0 };
	::GetTextExtentPoint(hdc, lpString, cbString, &size);
	return size;
}

// 57.
/*
BOOL GetTextExtentPoint32(
  HDC hdc,           // handle to DC
  LPCTSTR lpString,  // text string
  int cbString,      // characters in string
  LPSIZE lpSize      // string size
);
// */
GE_INLINE
SIZE GetTextExtentPoint32(
  HDC hdc,
  LPCTSTR lpString,
  int cbString
)
{
	SIZE size = { 0, 0 };
	::GetTextExtentPoint32(hdc, lpString, cbString, &size);
	return size;
}

#if (_WIN32_WINNT >= 0x0500)
// 58.
/*
BOOL GetTextExtentPointI(
  HDC hdc,           // handle to DC
  LPWORD pgiIn,      // glyph indices
  int cgi,           // number of indices in array
  LPSIZE lpSize      // string size
);
// */
GE_INLINE
SIZE GetTextExtentPointI(
  HDC hdc,
  LPWORD pgiIn,
  int cgi
)
{
	SIZE size = { 0, 0 };
	::GetTextExtentPointI(hdc, pgiIn, cgi, &size);
	return size;
}
#endif // _WIN32_WINNT >= 0x0500

/// Part 2: Windows GDI API utility extensions
GE_INLINE
BOOL FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
{
	SetBkColor(hdc, clr);
	RECT rc = { x, y, x + cx, y + cy };
	return ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
}
GE_INLINE
BOOL FillSolidRect(HDC hdc, PRECT prc, COLORREF clr)
{
	// Fill background 
	SetBkColor(hdc, clr);
	return ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, prc, NULL, 0, NULL);
}
GE_INLINE
BOOL Draw3dRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(hdc, x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(hdc, x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(hdc, x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(hdc, x, y + cy, cx, -1, clrBottomRight);

	return TRUE;
}
GE_INLINE
BOOL Draw3dRect(HDC hdc, PRECT prc, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	return Draw3dRect(hdc, prc->left, prc->top, prc->right - prc->left,
		prc->bottom - prc->top, clrTopLeft, clrBottomRight);
}

/// Part 3: Common control API extensions
GE_INLINE
BOOL ImageList_Draw(HIMAGELIST himl, int i, HDC hdcDst, POINT pt, UINT fStyle)
{
	return ::ImageList_Draw(himl, i, hdcDst, pt.x, pt.y, fStyle);
}

}; // namespace TWL

#endif // __GDIEXT_H__
