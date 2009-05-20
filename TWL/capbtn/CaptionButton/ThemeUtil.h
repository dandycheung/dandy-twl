/**********************************************************************
**
**	ThemeUtil.h : include file
**
**	by Andrzej Markowski June 2004
**
**********************************************************************/

#pragma once

#include <TmSchema.h>

#define _OpenThemeData(wnd, cls)	(GetModuleHandle(TEXT("uxtheme.dll")) ? OpenThemeData(wnd, cls) : NULL)
#define _CloseThemeData(thm)		(GetModuleHandle(TEXT("uxtheme.dll")) ? CloseThemeData(thm) : NULL)

BOOL IsThemed();

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED      0x031A
#endif

#ifndef SPI_GETGRADIENTCAPTIONS 
#define SPI_GETGRADIENTCAPTIONS             0x1008
#define COLOR_GRADIENTACTIVECAPTION			27
#define COLOR_GRADIENTINACTIVECAPTION		28
#endif

typedef struct _MY_MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} MY_MARGINS;

typedef struct _COLORGLYPH
{
	HDC hdc;
	COLORREF rgbTransGlyph;
	int nWidth;
	int nHeight;
	struct _COLORCNT
	{
		COLORREF rgbGlyph;
		int	nCnt;
	} a[4];
} COLORGLYPH;

class CThemeUtil
{
public:
	CThemeUtil();
	virtual ~CThemeUtil();

	BOOL OpenThemeData(HWND hWnd, LPCWSTR pszClassList);
	void CloseThemeData();

	int  GetThemeSysSize(int iSizeId);
	BOOL GetThemeColor(int iPartId, int iStateId, int iPropId, const COLORREF *pColor);
	BOOL GetThemeBool(int iPartId, int iStateId, int iPropId, OUT BOOL *pfVal);
	BOOL GetThemePosition(int iPartId, int iStateId, int iPropId, OUT POINT *pPoint);
	BOOL GetThemeInt(int iPartId, int iStateId, int iPropId, const int *piVal);
	BOOL GetThemeMargins(int iPartId, int iStateId, int iPropId, const MY_MARGINS *pMargins);
	BOOL GetThemeEnumValue(int iPartId, int iStateId, int iPropId, const int *piVal);
	BOOL GetThemeBitmap(int iPartId, int iStateId, int iPropId, HBITMAP *phbmp);
	BOOL GetThemeFilename(int iPartId, int iStateId, int iPropId, 
							OUT LPWSTR pszThemeFileName, int cchMaxBuffChars);
	BOOL GetCurrentThemeName(OUT LPWSTR pszThemeFileName, int cchMaxNameChars, 
							OUT OPTIONAL LPWSTR pszColorBuff, int cchMaxColorChars,
							OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars);

	BOOL GetThemeBackgroundContentRect(OPTIONAL HDC hdc, int iPartId, int iStateId,
										const RECT *pBoundingRect, OUT RECT *pContentRect);

	BOOL DrawThemePart(HDC hdc, int iPartId, int iStateId, const RECT* pRect);
	BOOL DrawThemeBackground(HDC hdc, int iPartId, int iStateId,
								const RECT *pRect, OPTIONAL const RECT *pClipRect);

	BOOL DrawThemeParentBackground(HWND hwnd, HDC hdc, OPTIONAL RECT* prc);

	int IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId);
	BOOL IsThemeActive();

	BOOL IsLunaTheme() { return m_fLuna; }

	HBITMAP LoadBitmap(LPWSTR pszBitmapName);

	BOOL TransparentBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int hHeightDest,
		HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent);
	BOOL GradientFill(HDC hdc, CONST PTRIVERTEX pVertex, DWORD dwNumVertex, CONST PVOID pMesh, DWORD dwNumMesh, DWORD dwMode);

	static LONG GetWinVersion();

private:
	BOOL IsWinXP();
	void Cleanup();

private:
	HINSTANCE m_hUxThemeDll;
	HINSTANCE m_hRcDll;
	HINSTANCE m_hDll;
	UINT m_hTheme;
	BOOL m_fLuna;
};
