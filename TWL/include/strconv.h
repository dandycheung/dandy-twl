//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __STRCONV_H__
#define __STRCONV_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif // _INC_MALLOC

#ifndef ASSERT
#include <crtdbg.h>
#define ASSERT(e)	_ASSERTE(e)
#endif // ASSERT

#pragma pack(push, 8)

#ifdef _CONVERSION_USES_THREAD_LOCALE
#define CP_DEFCONV	TwlGetConversionACP(TRUE)
#else
#define CP_DEFCONV	TwlGetConversionACP(FALSE)
#endif

inline UINT WINAPI TwlGetConversionACP(BOOL bUseThreadLocale = FALSE)
{
	if(!bUseThreadLocale)
		return CP_ACP;

	OSVERSIONINFO osvi = { sizeof(osvi) };
	GetVersionEx(&osvi);

	// On Windows 2000+, CP_THREAD_ACP is supported
	if((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion >= 5))
		return CP_THREAD_ACP;

	UINT nACP = 0;
	CHAR szACP[7];

	// GetLocaleInfoA will fail for a Unicode-only LCID, but those are only supported on 
	// Windows 2000+. Fortunately, this code path is never reached on Windows 2000+.
	if(GetLocaleInfoA(GetThreadLocale(), LOCALE_IDEFAULTANSICODEPAGE, szACP, sizeof(szACP)))
	{
		PSTR pch = szACP;
		while(*pch)
		{
			nACP *= 10;
			nACP += *pch++ - '0';
		}
	}

	// Use the Default ANSI Code Page if we were unable to get the thread ACP or if one does not exist.
	if(nACP == 0)
		nACP = GetACP();

	return nACP;
}

#ifndef _DEBUG
#define USES_CONVERSION int _convert; _convert; UINT _acp = CP_DEFCONV; _acp; PCWSTR _pw; _pw; PCSTR _pa; _pa
#else
#define USES_CONVERSION int _convert = 0; _convert; UINT _acp = CP_DEFCONV; _acp; PCWSTR _pw = NULL; _pw; PCSTR _pa = NULL; _pa
#endif

// Global UNICODE <---> ANSI translation helpers
inline PWSTR WINAPI TwlA2WHelper(PWSTR pw, PCSTR pa, int nChars, UINT acp = CP_ACP)
{
	ASSERT(pa != NULL);
	ASSERT(pw != NULL);
	if(pw == NULL || pa == NULL)
		return NULL;

	// verify that no illegal character present since pw was allocated 
	// based on the size of pa don't worry about the number of chars

	*pw = L'\0';
	int iRet = MultiByteToWideChar(acp, 0, pa, -1, pw, nChars);
	if(iRet == 0)
	{
		ASSERT(FALSE);
		return NULL;
	}

	return pw;
}

inline PSTR WINAPI TwlW2AHelper(PSTR pa, PCWSTR pw, int nChars, UINT acp = CP_ACP)
{
	ASSERT(pw != NULL);
	ASSERT(pa != NULL);
	if(pa == NULL || pw == NULL)
		return NULL;

	// verify that no illegal character present since pa was allocated 
	// based on the size of pw don't worry about the number of chars

	*pa = '\0';
	int iRet = WideCharToMultiByte(acp, 0, pw, -1, pa, nChars, NULL, NULL);
	if(iRet == 0)
	{
		ASSERT(FALSE);
		return NULL;
	}

	return pa;
}

#define TWLA2WHELPER TwlA2WHelper
#define TWLW2AHELPER TwlW2AHelper

#define A2WEX(pa, cp) (\
	((_pa = pa) == NULL) ? NULL : (\
		_convert = (lstrlenA(_pa) + 1),\
		(INT_MAX / 2 < _convert) ? NULL :\
		TWLA2WHELPER((PWSTR)alloca(_convert * sizeof(WCHAR)), _pa, _convert, cp)))

#define W2AEX(pw, cp) (\
	((_pw = pw) == NULL) ? NULL : (\
		_convert = (lstrlenW(_pw) + 1),\
		(_convert > INT_MAX / 2) ? NULL :\
		TWLW2AHELPER((PSTR)alloca(_convert * sizeof(WCHAR)), _pw, _convert * sizeof(WCHAR), cp)))

#define A2CWEX(pa, cp) ((PCWSTR)A2WEX(pa, cp))
#define W2CAEX(pw, cp) ((PCSTR)W2AEX(pw, cp))

#define A2W(pa) A2WEX(pa, _acp)
#define W2A(pw) W2AEX(pw, _acp)

#define A2CW(pa) ((PCWSTR)A2W(pa))
#define W2CA(pw) ((PCSTR)W2A(pw))

#if defined(_UNICODE) // in these cases the default (TCHAR) is the same as OLECHAR
inline size_t ocslen(LPCOLESTR x) { return lstrlenW(x); }
inline LPOLESTR ocscpy(LPOLESTR dest, LPCOLESTR src) { return lstrcpyW(dest, src); }
inline LPOLESTR ocscat(LPOLESTR dest, LPCOLESTR src) { return lstrcatW(dest, src); }
inline LPCOLESTR T2COLE(LPCTSTR p) { return p; }
inline LPCTSTR OLE2CT(LPCOLESTR p) { return p; }
inline LPOLESTR T2OLE(LPTSTR p) { return p; }
inline LPTSTR OLE2T(LPOLESTR p) { return p; }
inline LPOLESTR CharNextO(LPCOLESTR p) { return CharNextW(p); }
#elif defined(OLE2ANSI) // in these cases the default (TCHAR) is the same as OLECHAR
inline size_t ocslen(LPCOLESTR x) { return lstrlen(x); }
inline LPOLESTR ocscpy(LPOLESTR dest, LPCOLESTR src) { return lstrcpy(dest, src); }
inline LPOLESTR ocscat(LPOLESTR dest, LPCOLESTR src) { return ocscpy(dest + ocslen(dest), src); }
inline LPCOLESTR T2COLE(LPCTSTR p) { return p; }
inline LPCTSTR OLE2CT(LPCOLESTR p) { return p; }
inline LPOLESTR T2OLE(LPTSTR p) { return p; }
inline LPTSTR OLE2T(LPOLESTR p) { return p; }
inline LPOLESTR CharNextO(LPCOLESTR p) { return CharNext(p); }
#else
inline size_t ocslen(LPCOLESTR x) { return lstrlenW(x); }
// lstrcpyW doesn't work on Win95, so we do this
inline LPOLESTR ocscpy(LPOLESTR dest, LPCOLESTR src)
	{ return (LPOLESTR) memcpy(dest, src, (lstrlenW(src) + 1) * sizeof(WCHAR)); }
inline LPOLESTR ocscat(LPOLESTR dest, LPCOLESTR src) { return ocscpy(dest + ocslen(dest), src); }
#define T2COLE(pa) A2CW(pa)
#define T2OLE(pa) A2W(pa)
#define OLE2CT(po) W2CA(po)
#define OLE2T(po) W2A(po)
// CharNextW doesn't work on Win95 so we use this
inline LPOLESTR CharNextO(LPCOLESTR p) { return (LPOLESTR) ((*p) ? (p + 1) : p); }
#endif

#ifdef OLE2ANSI
inline LPOLESTR A2OLE(PSTR p) { return p; }
inline PSTR OLE2A(LPOLESTR p) { return p; }
#define W2OLE W2A
#define OLE2W A2W
inline LPCOLESTR A2COLE(PCSTR p) { return p; }
inline PCSTR OLE2CA(LPCOLESTR p) { return p; }
#define W2COLE W2CA
#define OLE2CW A2CW
#else
inline LPOLESTR W2OLE(PWSTR p) { return p; }
inline PWSTR OLE2W(LPOLESTR p) { return p; }
#define A2OLE A2W
#define OLE2A W2A
inline LPCOLESTR W2COLE(PCWSTR p) { return p; }
inline PCWSTR OLE2CW(LPCOLESTR p) { return p; }
#define A2COLE A2CW
#define OLE2CA W2CA
#endif

#ifdef _UNICODE
#define T2A W2A
#define A2T A2W
inline PWSTR T2W(PTSTR p) { return p; }
inline PTSTR W2T(PWSTR p) { return p; }
#define T2CA W2CA
#define A2CT A2CW
inline PCWSTR T2CW(PCTSTR p) { return p; }
inline PCTSTR W2CT(PCWSTR p) { return p; }
#else
#define T2W A2W
#define W2T W2A
inline PSTR T2A(PTSTR p) { return p; }
inline PTSTR A2T(PSTR p) { return p; }
#define T2CW A2CW
#define W2CT W2CA
inline PCSTR T2CA(PCTSTR p) { return p; }
inline PCTSTR A2CT(PCSTR p) { return p; }
#endif

inline BSTR A2WBSTR(PCSTR p, int nLen = -1)
{
	USES_CONVERSION;
	int nConvertedLen = MultiByteToWideChar(_acp, 0, p, nLen, NULL, NULL) - 1;
	BSTR bstr = ::SysAllocStringLen(NULL, nConvertedLen);
	if(bstr != NULL)
		MultiByteToWideChar(_acp, 0, p, -1, bstr, nConvertedLen);

	return bstr;
}

inline BSTR OLE2BSTR(LPCOLESTR p) { return ::SysAllocString(p); }
#if defined(_UNICODE) // in these cases the default (TCHAR) is the same as OLECHAR
inline BSTR T2BSTR(PCTSTR p) { return ::SysAllocString(p); }
inline BSTR A2BSTR(PCSTR p) { USES_CONVERSION; return A2WBSTR(p); }
inline BSTR W2BSTR(PCWSTR p) { return ::SysAllocString(p); }
#elif defined(OLE2ANSI) // in these cases the default (TCHAR) is the same as OLECHAR
inline BSTR T2BSTR(PCTSTR p) { return ::SysAllocString(p); }
inline BSTR A2BSTR(PCSTR p) { return ::SysAllocString(p); }
inline BSTR W2BSTR(PCWSTR p) { USES_CONVERSION; return ::SysAllocString(W2COLE(p)); }
#else
inline BSTR T2BSTR(PCTSTR p) { USES_CONVERSION; return A2WBSTR(p); }
inline BSTR A2BSTR(PCSTR p) { USES_CONVERSION; return A2WBSTR(p); }
inline BSTR W2BSTR(PCWSTR p) { return ::SysAllocString(p); }
#endif

#pragma pack(pop)

#endif // __STRCONV_H__
