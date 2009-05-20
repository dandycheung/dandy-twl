#ifndef __DRAWUTIL_H__
#define __DRAWUTIL_H__

#include <windows.h>

#ifndef BMPTYPE_DDB
#define BMPTYPE_DDB						1
#define BMPTYPE_DIB						2
#endif // BMPTYPE_DDB

#ifndef BMPDRAW_NONE
#define BMPDRAW_NONE					0
#define BMPDRAW_HSTRETCH_VSTRETCH		1
#define BMPDRAW_HTILE_VSTRETCH			2
#define BMPDRAW_HSTRETCH_VTILE			3
#define BMPDRAW_HTILE_VTILE				4
#endif // BMPDRAW_NONE

#define DMPDRAW_WHOLE					0x00010000

#define COLORREF2RGB(clr)				((clr & 0xff00) | ((clr >> 16) & 0xff) | ((clr << 16) & 0xff0000))

void HSBtoRGB(float hue, float saturation, float brightness, int& r, int& g, int& b);
void RGBtoHSB(int r, int g, int b, float& hue, float& saturation, float& brightness);

HBRUSH CreateHalftoneBrush();

void FillGradientRect(HDC hdc, PRECT prc, COLORREF clrBegin, COLORREF clrEnd, BOOL bHorz = TRUE);
BOOL FillSolidRect(HDC hdc, PRECT prc, COLORREF clr);

// If iIndex != -1, then it must be 0, 1, 2 or 3, we assume that the bitmap has four status:
// normal(0), hover(1), down(2), disable(3); -1 for the whole image.
BOOL DrawBitmap(LPVOID pBmp, int iIndex, HDC hdc, PRECT prc, UINT uBmpType, UINT uDrawMode);
BOOL DrawBitmap(LPVOID pBmp, DWORD dwIndex, HDC hdc, PRECT prc, UINT uBmpType, UINT uDrawMode);

void NormalizeRect(PRECT prc);
void DrawDragRect(HDC hdc, LPCRECT prc, SIZE size, LPCRECT prcLast, SIZE sizeLast, HBRUSH hbr = NULL, HBRUSH hbrLast = NULL);
// BOOL DrawDraggingFrame(HWND hParent, PRECT prc, BOOL bSizable);

HBITMAP ColorizeBitmap(HBITMAP hbmpSrc, COLORREF clr, UINT uFactor);
HBITMAP DarkBitmap(HBITMAP hbmpSrc, float level);

HBITMAP CopyBitmap(HBITMAP hbmpSrc);
HBITMAP ReplaceBitmapColor(HBITMAP hbmp, COLORREF clrOld, COLORREF clrNew);

#endif // __DRAWUTIL_H__
