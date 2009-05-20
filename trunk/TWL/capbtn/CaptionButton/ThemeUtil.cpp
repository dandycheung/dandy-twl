#include "stdafx.h"
#include <shlwapi.h>
#include "themeutil.h"

CThemeUtil::CThemeUtil()
{
	m_fLuna = FALSE;
	m_hUxThemeDll = NULL;
	m_hRcDll = NULL;
	m_hTheme = NULL;
	m_hDll = NULL;

	Cleanup();

	if(IsWinXP())
		m_hUxThemeDll = LoadLibrary(_T("UxTheme.dll"));

	m_hDll = LoadLibrary(_T("msimg32.dll"));
}

CThemeUtil::~CThemeUtil()
{
	Cleanup();
}

void CThemeUtil::Cleanup()
{
	CloseThemeData();

	if(m_hUxThemeDll != NULL)
		FreeLibrary(m_hUxThemeDll);

	if(m_hDll != NULL)
		FreeLibrary(m_hDll);

	m_hUxThemeDll = NULL;
}

BOOL CThemeUtil::OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
{
	if(m_hUxThemeDll == NULL || hWnd == NULL || m_hTheme) 
		return FALSE;

	UINT (PASCAL* pfnIsThemeActive)();	// IsThemeActive
	(FARPROC&)pfnIsThemeActive = GetProcAddress(m_hUxThemeDll, "IsThemeActive");
	if(pfnIsThemeActive && pfnIsThemeActive())
	{
		UINT (PASCAL* pfnOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		(FARPROC&)pfnOpenThemeData = GetProcAddress(m_hUxThemeDll, "OpenThemeData");
		if(pfnOpenThemeData)
			m_hTheme = pfnOpenThemeData(hWnd, pszClassList);

		if(m_hTheme)
		{
			WCHAR szThemeFileName[MAX_PATH];
			WCHAR szColorBuff[MAX_PATH];
			WCHAR szSizeBuff[MAX_PATH];

			if(GetCurrentThemeName(szThemeFileName, MAX_PATH, szColorBuff, MAX_PATH, szSizeBuff, MAX_PATH))
			{
				CString strFile(szThemeFileName);
				if(strFile.Right(19) == CString("\\Luna\\Luna.msstyles"))
					m_fLuna = TRUE;
				else
					m_fLuna = FALSE;

				m_hRcDll = LoadLibrary(strFile);
				if(m_hRcDll)
					return TRUE;				
			}

			CloseThemeData();
			return FALSE;
		}
	}

	return FALSE;
}

void CThemeUtil::CloseThemeData()
{
	if(m_hTheme)
	{
		UINT (PASCAL* pfnCloseThemeData)(HANDLE hTheme);
		(FARPROC&)pfnCloseThemeData = GetProcAddress(m_hUxThemeDll, "CloseThemeData");
		if(pfnCloseThemeData)
			pfnCloseThemeData((HANDLE)m_hTheme);
	}

	m_hTheme = NULL;

	if(m_hRcDll != NULL)
		FreeLibrary(m_hRcDll);

	m_hRcDll = NULL;
}

int CThemeUtil::GetThemeSysSize(int iSizeId)
{
	if(m_hTheme == NULL)
		return ::GetSystemMetrics(iSizeId);

	UINT (PASCAL* pfnGetThemeSysSize)(UINT hTheme, int iSizeId);
	(FARPROC&)pfnGetThemeSysSize = GetProcAddress(m_hUxThemeDll, "GetThemeSysSize");
	if(pfnGetThemeSysSize)
		return pfnGetThemeSysSize(m_hTheme, iSizeId);

	return ::GetSystemMetrics(iSizeId);
}

BOOL CThemeUtil::GetThemeColor(int iPartId, int iStateId, int iPropId, const COLORREF* pColor)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeColor)(UINT hTheme, int iPartId, int iStateId, int iPropId, const COLORREF *pColor);
	(FARPROC&)pfnGetThemeColor = GetProcAddress(m_hUxThemeDll, "GetThemeColor");
	if(pfnGetThemeColor)
	{
		pfnGetThemeColor(m_hTheme, iPartId, iStateId, iPropId, pColor);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeBool(int iPartId, int iStateId, int iPropId, OUT BOOL* pfVal)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeBool)(UINT hTheme, int iPartId, int iStateId, int iPropId, OUT BOOL *pfVal);
	(FARPROC&)pfnGetThemeBool = GetProcAddress(m_hUxThemeDll, "GetThemeBool");
	if(pfnGetThemeBool)
	{
		pfnGetThemeBool(m_hTheme, iPartId, iStateId, iPropId, pfVal);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemePosition(int iPartId, int iStateId, int iPropId, OUT POINT* pPoint)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemePosition)(UINT hTheme, int iPartId, int iStateId, int iPropId, OUT POINT* pPoint);
	(FARPROC&)pfnGetThemePosition = GetProcAddress(m_hUxThemeDll, "GetThemePosition");
	if(pfnGetThemePosition)
	{
		pfnGetThemePosition(m_hTheme, iPartId, iStateId, iPropId, pPoint);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeEnumValue(int iPartId, int iStateId, int iPropId, const int* piVal)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeEnumValue)(UINT hTheme, int iPartId, int iStateId, int iPropId, const int* piVal);
	(FARPROC&)pfnGetThemeEnumValue = GetProcAddress(m_hUxThemeDll, "GetThemeEnumValue");
	if(pfnGetThemeEnumValue)
	{
		pfnGetThemeEnumValue(m_hTheme, iPartId, iStateId, iPropId, piVal);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeBitmap(int iPartId, int iStateId, int iPropId, HBITMAP *phbmp)
{
	UINT (PASCAL* pfnGetThemeBitmap)(UINT hTheme, int iPartId, int iStateId, int iPropId, ULONG dwFlags, HBITMAP *phBitmap);
	(FARPROC&)pfnGetThemeBitmap = GetProcAddress(m_hUxThemeDll, "GetThemeBitmap");
	if(pfnGetThemeBitmap)
	{
		pfnGetThemeBitmap(m_hTheme, iPartId, iStateId,
			0, // or TMT_DIBDATA(2),
			2, // GBF_COPY,
			phbmp);

		return TRUE;
	}

	WCHAR szBkBitmapFilename[MAX_PATH];
	if(!GetThemeFilename(iPartId, iStateId, iPropId, szBkBitmapFilename, MAX_PATH))
		return FALSE;

	if(!(*phbmp = LoadBitmap(szBkBitmapFilename)))
		return FALSE;

	return TRUE;
}

BOOL CThemeUtil::GetThemeInt(int iPartId, int iStateId, int iPropId, const int* piVal)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeInt)(UINT hTheme, int iPartId, int iStateId, int iPropId, const int* piVal);
	(FARPROC&)pfnGetThemeInt = GetProcAddress(m_hUxThemeDll, "GetThemeInt");
	if(pfnGetThemeInt)
	{
		pfnGetThemeInt(m_hTheme, iPartId, iStateId, iPropId, piVal);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeMargins(int iPartId, int iStateId, int iPropId, const MY_MARGINS* pMargins)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeMargins)(UINT hTheme, OPTIONAL HDC hdc, int iPartId, 
				int iStateId, int iPropId, OPTIONAL RECT *prc, const MY_MARGINS* pMargins);
	(FARPROC&)pfnGetThemeMargins = GetProcAddress(m_hUxThemeDll, "GetThemeMargins");
	if(pfnGetThemeMargins)
	{
		pfnGetThemeMargins(m_hTheme, NULL, iPartId, iStateId, iPropId, NULL, pMargins);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeFilename(int iPartId, int iStateId, int iPropId, OUT LPWSTR pszThemeFileName, int cchMaxBuffChars)
{
	if(m_hTheme == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetThemeFilename)(UINT hTheme, int iPartId, int iStateId, int iPropId, OUT LPWSTR pszThemeFileName, int cchMaxBuffChars);
	(FARPROC&)pfnGetThemeFilename = GetProcAddress(m_hUxThemeDll, "GetThemeFilename");
	if(pfnGetThemeFilename)
	{
		pfnGetThemeFilename(m_hTheme, iPartId, iStateId, iPropId, pszThemeFileName, cchMaxBuffChars);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::GetThemeBackgroundContentRect(OPTIONAL HDC hdc, int iPartId, int iStateId, const RECT *pBoundingRect, OUT RECT *pContentRect)
{
	if(m_hTheme == NULL)
		return FALSE;
	
	UINT (PASCAL* pfnGetThemeBackgroundContentRect)(UINT hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pBoundingRect, const RECT* pContentRect);
	(FARPROC&)pfnGetThemeBackgroundContentRect = GetProcAddress(m_hUxThemeDll, "GetThemeBackgroundContentRect");
	if(pfnGetThemeBackgroundContentRect)
	{
		pfnGetThemeBackgroundContentRect(m_hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CThemeUtil::DrawThemePart(HDC hdc, int iPartId, int iStateId, const RECT *pRect)
{
	return DrawThemeBackground(hdc, iPartId, iStateId, pRect, NULL);
}

BOOL CThemeUtil::DrawThemeBackground(HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect)
{
	if(m_hTheme == NULL)
		return FALSE;
	
	UINT (PASCAL* pfnDrawThemeBackground)(UINT hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT* pClipRect);
	(FARPROC&)pfnDrawThemeBackground = GetProcAddress(m_hUxThemeDll, "DrawThemeBackground");
	if(pfnDrawThemeBackground)
	{
		pfnDrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CThemeUtil::DrawThemeParentBackground(HWND hwnd, HDC hdc, OPTIONAL RECT* prc)
{
	if(m_hTheme == NULL)
		return FALSE;
	
	UINT (PASCAL* pfnDrawThemeParentBackground)(HWND hwnd, HDC hdc, RECT* prc);
	(FARPROC&)pfnDrawThemeParentBackground = GetProcAddress(m_hUxThemeDll, "DrawThemeParentBackground");
	if(pfnDrawThemeParentBackground)
	{
		pfnDrawThemeParentBackground(hwnd, hdc, prc);
		return TRUE;
	}

	return FALSE;
}

BOOL CThemeUtil::IsThemeActive()
{
	BOOL (PASCAL* pfnIsThemeActive)();
	(FARPROC&)pfnIsThemeActive = GetProcAddress(m_hUxThemeDll, "IsThemeActive");
	if(pfnIsThemeActive)
		return pfnIsThemeActive();

	return FALSE;
}

int CThemeUtil::IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId)
{
	if(m_hTheme == NULL)
		return -1;

	BOOL (PASCAL* pfnIsThemeBackgroundPartiallyTransparent)(UINT hTheme, int iPartId, int iStateId);
	(FARPROC&)pfnIsThemeBackgroundPartiallyTransparent = GetProcAddress(m_hUxThemeDll, "IsThemeBackgroundPartiallyTransparent");
	if(pfnIsThemeBackgroundPartiallyTransparent)
	{
		if(pfnIsThemeBackgroundPartiallyTransparent(m_hTheme, iPartId, iStateId))
			return 1;

		return 0;
	}

	return -1;
}

BOOL CThemeUtil::GetCurrentThemeName(OUT LPWSTR pszThemeFileName, int cchMaxNameChars, 
							OUT OPTIONAL LPWSTR pszColorBuff, int cchMaxColorChars,
							OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
	if(m_hUxThemeDll == NULL)
		return FALSE;

	UINT (PASCAL* pfnGetCurrentThemeName)(OUT LPWSTR pszThemeFileName, int cchMaxNameChars, 
										OUT OPTIONAL LPWSTR pszColorBuff, int cchMaxColorChars,
										OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars);
	(FARPROC&)pfnGetCurrentThemeName = GetProcAddress(m_hUxThemeDll, "GetCurrentThemeName");
	if(pfnGetCurrentThemeName)
	{
		pfnGetCurrentThemeName(pszThemeFileName, cchMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
		return TRUE;
	}

	return FALSE;
}

HBITMAP CThemeUtil::LoadBitmap(LPWSTR pszBitmapName)
{
	if(m_hRcDll == NULL)
		return NULL;

	CString strFile(pszBitmapName);
	strFile.Replace(_T('\\'), _T('_'));
	strFile.Replace(_T('.'), _T('_'));

	return ::LoadBitmap(m_hRcDll, strFile);
}

BOOL CThemeUtil::TransparentBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int hHeightDest,
						HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent)
{
	UINT (PASCAL* pfnTransparentBlt)(HDC, int, int, int, int, HDC, int, int, int, int, UINT);
	if(m_hDll)
	{
		(FARPROC&)pfnTransparentBlt = GetProcAddress(m_hDll, "TransparentBlt");
		if(pfnTransparentBlt)
		{
			pfnTransparentBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, hHeightDest,
						hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, crTransparent);
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

BOOL CThemeUtil::GradientFill(HDC hdc, CONST PTRIVERTEX pVertex, DWORD dwNumVertex, CONST PVOID pMesh, DWORD dwNumMesh, DWORD dwMode)
{
	UINT (PASCAL* pfnGradientFill)(HDC, CONST PTRIVERTEX, DWORD, CONST PVOID, DWORD, DWORD);
	if(m_hDll)
	{
		(FARPROC&)pfnGradientFill = GetProcAddress(m_hDll, "GradientFill");
		if(pfnGradientFill)
		{
			pfnGradientFill(hdc, pVertex, dwNumVertex, pMesh, dwNumMesh, dwMode);
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

LONG CThemeUtil::GetWinVersion()
{
	OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&osvi);
	return MAKELONG(osvi.dwMinorVersion, osvi.dwMajorVersion);
}

BOOL CThemeUtil::IsWinXP(void)
{
	return GetWinVersion() >= MAKELONG(1, 5);
}

BOOL IsThemed()
{
    BOOL bRet = FALSE;
	
    OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
    GetVersionEx(&ovi);
    if(ovi.dwMajorVersion > 5 || (ovi.dwMajorVersion == 5 && ovi.dwMinorVersion >= 1))
    {
        // Windows XP detected
        typedef BOOL WINAPI ISAPPTHEMED();
        typedef BOOL WINAPI ISTHEMEACTIVE();
		
        ISAPPTHEMED* pISAPPTHEMED = NULL;
        ISTHEMEACTIVE* pISTHEMEACTIVE = NULL;
		
        HMODULE hMod = GetModuleHandle(TEXT("uxtheme.dll"));
        if(hMod)
        {
            pISAPPTHEMED = (ISAPPTHEMED*)(GetProcAddress(hMod, TEXT("IsAppThemed")));
            pISTHEMEACTIVE = (ISTHEMEACTIVE*)(GetProcAddress(hMod, TEXT("IsThemeActive")));
            if(pISAPPTHEMED && pISTHEMEACTIVE)
            {
                if(pISAPPTHEMED() && pISTHEMEACTIVE())                
                {                
                    typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
                    DLLGETVERSION* pDLLGETVERSION = NULL;
					
                    HMODULE hModComCtl = LoadLibrary(TEXT("comctl32.dll"));
                    if(hModComCtl)
                    {
                        pDLLGETVERSION = (DLLGETVERSION*)(GetProcAddress(hModComCtl, TEXT("DllGetVersion")));
                        if(pDLLGETVERSION)
                        {
                            DLLVERSIONINFO dvi = { sizeof(DLLVERSIONINFO) };
                            if(pDLLGETVERSION(&dvi) == NOERROR )
                                bRet = dvi.dwMajorVersion >= 6;
                        }
						
                        FreeLibrary(hModComCtl);                    
                    }
                }
            }
        }
    }    
	
    return bRet;
}
