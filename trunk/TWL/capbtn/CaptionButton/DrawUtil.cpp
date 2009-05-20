// 此文件来自于 NeoSkin 工程的 DrawHelper.* ，并做了改进。

#include <math.h>
#include "drawutil.h"

void HSBtoRGB(float hue, float saturation, float brightness, int& r, int& g, int& b)
{
	if(saturation == 0) 
	{
		r = g = b = (int) (brightness * 255.0f + 0.5f);
	}
	else 
	{
		float h = (hue - (float)floor(hue)) * 6.0f;
		float f = h - (float)floor(h);
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));
		
		switch ((int) h)
		{
		case 0:
			r = (int) (brightness * 255.0f + 0.5f);
			g = (int) (t * 255.0f + 0.5f);
			b = (int) (p * 255.0f + 0.5f);
			break;
			
		case 1:
			r = (int) (q * 255.0f + 0.5f);
			g = (int) (brightness * 255.0f + 0.5f);
			b = (int) (p * 255.0f + 0.5f);
			break;
			
		case 2:
			r = (int) (p * 255.0f + 0.5f);
			g = (int) (brightness * 255.0f + 0.5f);
			b = (int) (t * 255.0f + 0.5f);
			break;
			
		case 3:
			r = (int) (p * 255.0f + 0.5f);
			g = (int) (q * 255.0f + 0.5f);
			b = (int) (brightness * 255.0f + 0.5f);
			break;
			
		case 4:
			r = (int) (t * 255.0f + 0.5f);
			g = (int) (p * 255.0f + 0.5f);
			b = (int) (brightness * 255.0f + 0.5f);
			break;
			
		case 5:
			r = (int) (brightness * 255.0f + 0.5f);
			g = (int) (p * 255.0f + 0.5f);
			b = (int) (q * 255.0f + 0.5f);
			break;
		}
	}
}

void RGBtoHSB(int r, int g, int b, float& hue, float& saturation, float& brightness)
{
	int cmax = (r > g) ? r : g;
	if (b > cmax)
		cmax = b;
	
	int cmin = (r < g) ? r : g;
	if (b < cmin)
		cmin = b;
	
	brightness = ((float) cmax) / 255.0f;
	if (cmax != 0)
		saturation = ((float) (cmax - cmin)) / ((float) cmax);
	else
		saturation = 0;
	
	if (saturation == 0)
		hue = 0;
	else
	{
		float redc = ((float) (cmax - r)) / ((float) (cmax - cmin));
		float greenc = ((float) (cmax - g)) / ((float) (cmax - cmin));
		float bluec = ((float) (cmax - b)) / ((float) (cmax - cmin));
		
		if (r == cmax)
			hue = bluec - greenc;
		else if (g == cmax)
			hue = 2.0f + redc - bluec;
		else
			hue = 4.0f + greenc - redc;
		
		hue = hue / 6.0f;
		if (hue < 0)
			hue = hue + 1.0f;
	}
}

HBRUSH CreateHalftoneBrush()
{
	HBRUSH hbrRet = NULL;
	
	WORD grayPattern[8];
	for(int i=0; i<8; i++)
		grayPattern[i] = (WORD)(0x5555 << (i & 1));
	
	HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, &grayPattern);
	if(grayBitmap != NULL)
	{
		hbrRet = CreatePatternBrush(grayBitmap);
		DeleteObject(grayBitmap);
	}
	
	return hbrRet;
}

void FillGradientRect(HDC hdc, PRECT prc, COLORREF clrBegin, COLORREF clrEnd, BOOL bHorz)
{
	int nShift = 6;
	int nSteps = 1 << nShift;

	for(int i=0; i<nSteps; i++)
	{
		BYTE bR = (BYTE) ((GetRValue(clrBegin) * (nSteps - i) +
				   GetRValue(clrEnd) * i) >> nShift);
		BYTE bG = (BYTE) ((GetGValue(clrBegin) * (nSteps - i) +
				   GetGValue(clrEnd) * i) >> nShift);
		BYTE bB = (BYTE) ((GetBValue(clrBegin) * (nSteps - i) +
				   GetBValue(clrEnd) * i) >> nShift);

		HBRUSH hbr = CreateSolidBrush(RGB(bR, bG, bB));

		RECT rc = *prc;
		if(bHorz)
		{
			rc.left = prc->left + ((i * (prc->right - prc->left)) >> nShift);
			rc.right = prc->left + (((i + 1) * (prc->right - prc->left)) >> nShift);
			if(rc.right - rc.left > 0)
				FillRect(hdc, &rc, hbr);
		}
		else
		{
			rc.bottom = prc->bottom - ((i * (prc->bottom - prc->top)) >> nShift);
			rc.top = prc->bottom - (((i + 1) * (prc->bottom - prc->top)) >> nShift);
			if(rc.bottom - rc.top > 0)
				FillRect(hdc, &rc, hbr);
		}

		DeleteObject(hbr);
	}
}

BOOL FillSolidRect(HDC hdc, PRECT prc, COLORREF clr)
{
	// Fill background 
	SetBkColor(hdc, clr);
	return ExtTextOut(hdc, 0, 0, ETO_OPAQUE, prc, NULL, 0, NULL);
}

BOOL DrawBitmap(LPVOID pBmp, int iIndex, HDC hdc, PRECT prc, UINT uBmpType, UINT uDrawMode)
{
	if(iIndex < -1 || iIndex >= 4)
		return FALSE;

	DWORD dwIndex = iIndex == -1 ? MAKELONG(0, 1) : MAKELONG(iIndex, 4);
	return DrawBitmap(pBmp, dwIndex, hdc, prc, uBmpType, uDrawMode);
}

BOOL DrawBitmap(LPVOID pBmp, DWORD dwIndex, HDC hdc, PRECT prc, UINT uBmpType, UINT uDrawMode)
{
	if(pBmp == NULL || hdc == NULL || prc == NULL)
		return FALSE;

	// check uBmpType, DIB is not supported now
	if(uBmpType == BMPTYPE_DIB)
		return FALSE;

	int iCount = HIWORD(dwIndex);
	int iIndex = LOWORD(dwIndex);

	if(iCount == 0)
		iCount = 4;

	if(iIndex >= iCount)
		return FALSE;

	BITMAP bm = { 0 };
	GetObject((HBITMAP)pBmp, sizeof BITMAP, &bm);

	if(bm.bmWidth == 0 || bm.bmHeight == 0 || bm.bmWidth < iCount)
		return FALSE;

	HDC hdcMem = CreateCompatibleDC(hdc);
	if(hdcMem == NULL)
		return FALSE;

	HGDIOBJ hbmpOld = SelectObject(hdcMem, (HBITMAP)pBmp);

	LONG lWidth = bm.bmWidth / iCount;
	LONG lHeight = bm.bmHeight;

	int x = prc->left;
	int y = prc->top;

	int xs = lWidth * iIndex;
	int ys = 0;

	if(uDrawMode == BMPDRAW_NONE)
	{
		if(lWidth > (prc->right - prc->left))
			lWidth = prc->right - prc->left;
		
		if(lHeight > (prc->bottom - prc->top))
			lHeight = prc->bottom - prc->top;
		
		BitBlt(hdc, x, y, lWidth, lHeight, hdcMem, xs, ys, SRCCOPY);
	}
	else if(uDrawMode == BMPDRAW_HTILE_VTILE)
	{
		// 水平垂直都不拉伸
		int w, h;
		while(y < prc->bottom)
		{
			w = (x + lWidth > prc->right) ? (prc->right - x) : lWidth;
			h = (y + lHeight > prc->bottom) ? (prc->bottom - y) : lHeight;

			BitBlt(hdc, x, y, w, h, hdcMem, xs, ys, SRCCOPY);

			x += lWidth;
			if(x >= prc->right)
			{
				x = prc->left;
				y += lHeight;
			}
		}
	}
	else if(uDrawMode == BMPDRAW_HSTRETCH_VTILE)
	{
		// 水平拉伸
		int h;
		while(y < prc->bottom)
		{
			h = (y + lHeight > prc->bottom) ? (prc->bottom - y) : lHeight;
			StretchBlt(hdc, x, y, prc->right - prc->left, h, hdcMem, xs, ys, lWidth, h, SRCCOPY);
			y += lHeight;
		}
	}
	else if(uDrawMode == BMPDRAW_HTILE_VSTRETCH)
	{
		// 垂直拉伸
		int w;
		while(x < prc->right)
		{
			w = (x + lWidth > prc->right) ? (prc->right - x) : lWidth;
			StretchBlt(hdc, x, y, w, prc->bottom - prc->top, hdcMem, xs, ys, w, lHeight, SRCCOPY);
			x += lWidth;
		}
	}
	else if(uDrawMode == BMPDRAW_HSTRETCH_VSTRETCH)
	{
		// 水平垂直都拉伸
		StretchBlt(hdc, x, y, prc->right - prc->left, prc->bottom - prc->top, 
			hdcMem, xs, ys, lWidth, lHeight, SRCCOPY);
	}

	SelectObject(hdcMem, hbmpOld);
	DeleteDC(hdcMem);

	return TRUE;
}

void NormalizeRect(PRECT prc)
{
	if(prc == NULL)
		return;

	int nTemp;
	if(prc->left > prc->right)
	{
		nTemp = prc->left;
		prc->left = prc->right;
		prc->right = nTemp;
	}

	if(prc->top > prc->bottom)
	{
		nTemp = prc->top;
		prc->top = prc->bottom;
		prc->bottom = nTemp;
	}
}

void DrawDragRect(HDC hdc, LPCRECT prc, SIZE size, LPCRECT prcLast, SIZE sizeLast, HBRUSH hbr, HBRUSH hbrLast)
{
	if(hdc == NULL || prc == NULL)
		return;
/*
	NormalizeRect(prc);
	if(prcLast)
		NormalizeRect(prcLast);
// */
	// first, determine the update region and select it
	HRGN rgnOutside = CreateRectRgnIndirect(prc);
	RECT rc = *prc;
	InflateRect(&rc, -size.cx, -size.cy);
	IntersectRect(&rc, &rc, prc);

	HRGN rgnInside = CreateRectRgnIndirect(&rc);
	HRGN rgnNew = CreateRectRgn(0, 0, 0, 0);
	CombineRgn(rgnNew, rgnOutside, rgnInside, RGN_XOR);

	HBRUSH hbrCreate = NULL;
	HGDIOBJ hBrushOld = NULL;
	if(hbr == NULL)
	{
		hbr = CreateHalftoneBrush();
		hbrCreate = hbr;
	}

	if(hbrLast == NULL)
		hbrLast = hbr;

	HRGN rgnLast = NULL, rgnUpdate = NULL;
	if(prcLast != NULL)
	{
		// find difference between new region and old region
		rgnLast = CreateRectRgn(0, 0, 0, 0);
		SetRectRgn(rgnOutside, prcLast->left, prcLast->top, prcLast->right, prcLast->bottom);

		rc = *prcLast;
		InflateRect(&rc, -sizeLast.cx, -sizeLast.cy);
		IntersectRect(&rc, &rc, prcLast);
		SetRectRgn(rgnInside, rc.left, rc.top, rc.right, rc.bottom);
		CombineRgn(rgnLast, rgnOutside, rgnInside, RGN_XOR);

		// only diff them if brushes are the same
		if(hbr == hbrLast)
		{
			rgnUpdate = CreateRectRgn(0, 0, 0, 0);
			CombineRgn(rgnUpdate, rgnLast, rgnNew, RGN_XOR);
		}
	}

	if(hbr != hbrLast && prcLast != NULL)
	{
		// brushes are different -- erase old region first
		SelectClipRgn(hdc, rgnLast);
		GetClipBox(hdc, &rc);

		hBrushOld = SelectObject(hdc, hbrLast);
		PatBlt(hdc, rc.left, rc.top, rc.right -rc.left, rc.bottom - rc.top, PATINVERT);

		SelectObject(hdc, hBrushOld);
		hBrushOld = NULL;
	}

	// draw into the update/new region
	SelectClipRgn(hdc, rgnUpdate != NULL ? rgnUpdate : rgnNew);
	GetClipBox(hdc, &rc);

	hBrushOld = SelectObject(hdc, hbr);
	PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, PATINVERT);

	// cleanup DC
	if(hBrushOld != NULL)
		SelectObject(hdc, hBrushOld);

	if(hbrCreate != NULL)
		DeleteObject(hbrCreate);

	SelectClipRgn(hdc, NULL);

	if(rgnOutside != NULL)
		DeleteObject(rgnOutside);

	if(rgnInside != NULL)
		DeleteObject(rgnInside);

	if(rgnUpdate != NULL)
		DeleteObject(rgnUpdate);

	if(rgnNew != NULL)
		DeleteObject(rgnNew);

	if(rgnLast != NULL)
		DeleteObject(rgnLast);
}

HBITMAP ColorizeBitmap(HBITMAP hbmpSrc, COLORREF clr, UINT uFactor)
{
	if(!hbmpSrc)
		return NULL;

	BITMAP bm;
	if(GetObject(hbmpSrc, sizeof(BITMAP), &bm) != sizeof(BITMAP))
		return NULL;

	int iWidth = bm.bmWidth;
	int iHeight = bm.bmHeight;

	BITMAPINFO bi =
	{
		{
			sizeof(BITMAPINFOHEADER),
			iWidth,
			iHeight,
			1,
			32, 
			BI_RGB,
			iWidth * iHeight * 4
		}
	};

	COLORREF* pBits = NULL;
	HBITMAP hBitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (PVOID*)&pBits, NULL, NULL); 
	if(!hBitmap)
		return NULL;

	HDC hdcMem = CreateCompatibleDC(NULL);
	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, hbmpSrc);
	::GetDIBits(hdcMem, hbmpSrc, 0, iHeight, pBits, &bi, DIB_RGB_COLORS); 
	::SelectObject(hdcMem, hbmpMemOld);
	::DeleteDC(hdcMem);

	PBYTE dst = (PBYTE)pBits;
	int nSize = iWidth * iHeight;
	int R = GetRValue(clr);
	int G = GetGValue(clr);
	int B = GetBValue(clr);
	
	while(nSize--)
	{
		dst[0] = (BYTE)(((B-dst[0])*uFactor+(dst[0]<<8))>>8);
		dst[1] = (BYTE)(((G-dst[1])*uFactor+(dst[1]<<8))>>8);
		dst[2] = (BYTE)(((R-dst[2])*uFactor+(dst[2]<<8))>>8);	
		dst += 4;
	}

	return hBitmap;
}

HBITMAP DarkBitmap(HBITMAP hbmpSrc, float level)
{
	BITMAP bm;
	if(GetObject(hbmpSrc, sizeof(BITMAP), &bm) != sizeof(BITMAP))
		return NULL;

	int iWidth = bm.bmWidth;
	int iHeight = bm.bmHeight;
	
	BITMAPINFO bi = { {
		sizeof(BITMAPINFOHEADER),
		iWidth,
		iHeight,
		1,
		32, 
		BI_RGB,
		iWidth * iHeight * 4 } };

	COLORREF* pBits = NULL;
	HBITMAP hBitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (PVOID*)&pBits, NULL, NULL); 
	if(!hBitmap)
		return NULL;
	
	HDC hdcMem = CreateCompatibleDC(NULL);
	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, hbmpSrc);
	GetDIBits(hdcMem, hbmpSrc, 0, iHeight, pBits, &bi, DIB_RGB_COLORS); 
	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);

	int nLineBytes = bm.bmWidthBytes;
	while(nLineBytes % 4)
		nLineBytes++;

	int nBytesPerPixel = bm.bmBitsPixel / 8;

	int r, g, b;
	// float fh, fs, fb;
	for(int y = 0; y < iHeight; y++)
	{
		PBYTE pRow = (PBYTE)pBits + nLineBytes * (iHeight - y - 1);
		for(int x = 0; x < iWidth; x++)
		{
			PBYTE p = pRow + x * nBytesPerPixel;
			r = *(p + 2);
			g = *(p + 1);
			b = *(p + 0);

			// RGBtoHSB(r, g, b, fh, fs, fb);
			// fb *= level;
			// HSBtoRGB(fh, fs, fb, r, g, b);
			r /= 2;
			g /= 2;
			b /= 2;

			*(p + 2) = r;
			*(p + 1) = g;
			*(p + 0) = b;
		}
	}

	return hBitmap;
}

HBITMAP CopyBitmap(HBITMAP hbmpSrc)
{
	BITMAP bm;
	if(GetObject(hbmpSrc, sizeof(BITMAP), &bm) != sizeof(BITMAP))
		return NULL;
	
	int iWidth = bm.bmWidth;
	int iHeight = bm.bmHeight;
	
	BITMAPINFO bi =
	{
		{
			sizeof(BITMAPINFOHEADER),
			iWidth,
			iHeight,
			1,
			32, 
			BI_RGB,
			iWidth * iHeight * 4
		}
	};

	COLORREF* pBits = NULL;
	HBITMAP hBitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (PVOID*)&pBits, NULL, NULL); 
	if(!hBitmap)
		return NULL;
	
	HDC hdcMem = CreateCompatibleDC(NULL);
	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, hbmpSrc);
	GetDIBits(hdcMem, hbmpSrc, 0, iHeight, pBits, &bi, DIB_RGB_COLORS); 
	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);

	return hBitmap;
}

HBITMAP ReplaceBitmapColor(HBITMAP hbmp, COLORREF clrOld, COLORREF clrNew)
{
	BITMAP bm;
	if(!hbmp || !GetObject(hbmp, sizeof(bm),	&bm))
		return NULL;
	
	HDC hdcMemSrc = CreateCompatibleDC(NULL);	// DC for the source bitmap
	if(!hdcMemSrc)
		return NULL;
	
	HDC hdcMemDest = CreateCompatibleDC(NULL);	// DC for working
	if(!hdcMemDest)
	{
		DeleteDC(hdcMemSrc);
		return NULL;
	}
	
	// Create a BITMAPINFO with minimal initialization for the CreateDIBSection
	BITMAPINFO bi =
	{
		{									// bmiHeader
			sizeof(BITMAPINFOHEADER),		// 	.biSize
			bm.bmWidth,						// 	.biWidth
			bm.bmHeight,					//	.biHeight
			1,								//	.biPlanes
			32								//	.biBitCount
		}
	};
	
	UINT* pPixels;							// pointer for direct bitmap pixels access
	HBITMAP hbmpRet = CreateDIBSection(hdcMemDest, (LPBITMAPINFO)&bi, DIB_RGB_COLORS, (VOID**)&pPixels, NULL, 0);
	if(hbmpRet)
	{
		HGDIOBJ hbmpMemSrcOld = SelectObject(hdcMemSrc, hbmp);
		HGDIOBJ hbmpMemDestOld = SelectObject(hdcMemDest, hbmpRet);
		
		BitBlt(hdcMemDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemSrc, 0, 0, SRCCOPY);
		
		// Convert COLORREF to RGB(invert Red and Blue)
		clrOld = COLORREF2RGB(clrOld);
		clrNew = COLORREF2RGB(clrNew);
		
		// Do the job: replace color
		for(int i=((bm.bmWidth * bm.bmHeight) - 1); i>=0; i--)
		{
			if(pPixels[i] == clrOld)
				pPixels[i] = clrNew;
		}
		
		// Restore DCs
		SelectObject(hdcMemDest, hbmpMemDestOld);
		SelectObject(hdcMemSrc, hbmpMemSrcOld);
	}
	
	// Clean up
	DeleteDC(hdcMemDest);
	DeleteDC(hdcMemSrc);
	
	return hbmpRet;
}
