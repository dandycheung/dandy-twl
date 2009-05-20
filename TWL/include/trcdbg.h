//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __TRCDBG_H__
#define __TRCDBG_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <stdio.h>		// for _vsnprintf/_vsnwprintf
#include <crtdbg.h>

namespace TWL
{

#ifndef TWLASSERT
#define TWLASSERT(e) _ASSERTE(e)
#endif

enum twlTraceFlags
{
	// Application defined categories
	twlTraceUser        = 0x00000001,
	twlTraceUser2       = 0x00000002,
	twlTraceUser3       = 0x00000004,
	twlTraceUser4       = 0x00000008,
	// TWL defined categories
	twlTraceGeneral     = 0x00000020,
	twlTraceCOM         = 0x00000040,
	twlTraceQI          = 0x00000080,
	twlTraceRegistrar   = 0x00000100,
	twlTraceRefcount    = 0x00000200,
	twlTraceWindowing   = 0x00000400,
	twlTraceControls    = 0x00000800,
	twlTraceHosting     = 0x00001000,
	twlTraceDBClient    = 0x00002000,
	twlTraceDBProvider  = 0x00004000,
	twlTraceSnapin      = 0x00008000,
	twlTraceNotImpl     = 0x00010000,
	twlTraceUI          = 0x00020000, // Added by dandy, maybe another value in TRUE ATL
};

#ifndef TWL_TRACE_CATEGORY
#define TWL_TRACE_CATEGORY 0xFFFFFFFF
#endif

#ifdef _DEBUG
#ifndef TWL_TRACE_LEVEL
#define TWL_TRACE_LEVEL 0
#endif

inline
void _cdecl TwlTrace(LPCSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	char szBuffer[512];

	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	TWLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringA(szBuffer);
	va_end(args);
}

inline
void _cdecl TwlTrace2(DWORD category, UINT level, LPCSTR lpszFormat, ...)
{
	if (category & TWL_TRACE_CATEGORY && level <= TWL_TRACE_LEVEL)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;
		char szBuffer[512];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		TWLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

		OutputDebugStringA("TWL: ");
		OutputDebugStringA(szBuffer);
		va_end(args);
	}
}

#ifndef OLE2ANSI
inline
void _cdecl TwlTrace(LPCWSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	WCHAR szBuffer[512];

	nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
	TWLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringW(szBuffer);
	va_end(args);
}

inline
void _cdecl TwlTrace2(DWORD category, UINT level, LPCWSTR lpszFormat, ...)
{
	if (category & TWL_TRACE_CATEGORY && level <= TWL_TRACE_LEVEL)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;
		WCHAR szBuffer[512];

		nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
		TWLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

		OutputDebugStringW(L"TWL: ");
		OutputDebugStringW(szBuffer);
		va_end(args);
	}
}
#endif // !OLE2ANSI

#ifndef TWLTRACE
#define TWLTRACE  TwlTrace
#define TWLTRACE2 TwlTrace2
#endif

#define TWLTRACENOTIMPL(f) TWLTRACE2(twlTraceNotImpl, 2, _T("TWL: %s not implemented.\n"), f); return E_NOTIMPL

#else // !DEBUG

inline void _cdecl TwlTrace(LPCSTR , ...){}
inline void _cdecl TwlTrace2(DWORD, UINT, LPCSTR , ...){}

#ifndef OLE2ANSI
inline void _cdecl TwlTrace(LPCWSTR , ...){}
inline void _cdecl TwlTrace2(DWORD, UINT, LPCWSTR , ...){}
#endif // OLE2ANSI

#ifndef TWLTRACE
#define TWLTRACE  1 ? (void)0 : TwlTrace
#define TWLTRACE2 1 ? (void)0 : TwlTrace2
#endif // TWLTRACE

#define TWLTRACENOTIMPL(f) return E_NOTIMPL

#endif // _DEBUG

#ifndef ASSERT
#define ASSERT TWLASSERT
#endif

#ifndef TRACE
#define TRACE TWLTRACE
#endif

}; // namespace TWL

#endif // __TRCDBG_H__
