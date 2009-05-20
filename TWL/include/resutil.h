//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __RESUTIL_H__
#define __RESUTIL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "modutil.h"
#include "trcdbg.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
//

namespace TWL
{

class CMenuOrId
{
protected:
	HMENU m_hmnu;

public:
	CMenuOrId() : m_hmnu(NULL) {}
	~CMenuOrId() {}

	CMenuOrId(HMENU hmnu) : m_hmnu(hmnu) {}
	CMenuOrId(UINT uId) : m_hmnu((HMENU)uId) {}

	CMenuOrId& operator=(HMENU hmnu)
	{
		m_hmnu = hmnu;
		return *this;
	}

	CMenuOrId& operator=(UINT uId)
	{
		m_hmnu = (HMENU)uId;
		return *this;
	}

	operator const HMENU()
	{
		return m_hmnu;
	}

	operator UINT()
	{
		return (UINT)m_hmnu;
	}
};

class CStringOrId
{
protected:
	LPCTSTR m_psz;

public:
	CStringOrId() : m_psz(NULL) {}
	~CStringOrId() {}

	CStringOrId(LPCTSTR psz) : m_psz(psz) {}
	CStringOrId(UINT uId) : m_psz(MAKEINTRESOURCE(uId)) {}

	CStringOrId& operator=(LPCTSTR psz)
	{
		m_psz = psz;
		return *this;
	}

	CStringOrId& operator=(UINT uId)
	{
		m_psz = MAKEINTRESOURCE(uId);
		return *this;
	}

	operator LPCTSTR()
	{
		return m_psz;
	}

	operator UINT()
	{
		return (UINT)m_psz;
	}
};

class CResourceInstance
{
	static HINSTANCE m_hinst;

public:
	static void Set(HINSTANCE hinst)
	{
		m_hinst = hinst;
	}

	static HINSTANCE Get()
	{
		if(m_hinst == NULL)
			m_hinst = GetModuleHandle(NULL);

		return m_hinst;
	}
};

__declspec(selectany) HINSTANCE CResourceInstance::m_hinst = NULL;

#define GetResourceInstance()	(CResourceInstance::Get() ? CResourceInstance::Get() : GetModuleInstance())

///////////////////////////////////////////////////////////////////////////////
// Bitmap resource helpers to extract bitmap information for a bitmap resource

inline
PBITMAPINFOHEADER GetBitmapResourceInfo(CStringOrId image, HMODULE hModule)
{
	HRSRC hResource = ::FindResource(hModule, image, RT_BITMAP);
	ASSERT(hResource != NULL);

	HGLOBAL hGlobal = ::LoadResource(hModule, hResource);
	ASSERT(hGlobal != NULL);

	PBITMAPINFOHEADER pbih = (LPBITMAPINFOHEADER)::LockResource(hGlobal);
	ASSERT(pbih != NULL);

	return pbih;
}

inline
WORD GetBitmapResourceBitDepth(CStringOrId image, HMODULE hModule = GetResourceInstance())
{
	PBITMAPINFOHEADER pbih = GetBitmapResourceInfo(image, hModule);
	ASSERT(pbih != NULL);

	return pbih->biBitCount;
}

///////////////////////////////////////////////////////////////////////////////
// 32-bit (alpha channel) bitmap resource helper

// Note: 32-bit (alpha channel) images work only on Windows XP with Common Controls version 6.
// If you want your app to work on older version of Windows, load non-alpha images if Common
// Controls version is less than 6.

#define IsAlphaBitmapResource(img)	(GetBitmapResourceBitDepth(img) == 32)

#ifndef RC_INVOKED  // code only
// Standard control bars (IDW = window ID)
#define TWL_IDW_COMMON_FIRST            0xE800
#define TWL_IDW_COMMON_LAST             0xE8FF

#define TWL_IDW_TOOLBAR                 0xE800  // main Toolbar for window
#define TWL_IDW_STATUS_BAR              0xE801  // Status bar window
#define TWL_IDW_REBAR                   0xE804  // COMCTL32 "rebar" Bar

// parts of Main Frame
#define TWL_IDW_SPLITTER                0xE900  // splitter/layout window

#define TWL_IDW_SIZE_BOX                0xEA20  // size box for splitters
#define TWL_IDW_PANE_SAVE               0xEA21  // to shift TWL_IDW_PANE_FIRST

// bands for a rebar
#define TWL_IDW_BAND_FIRST              0xEB00
#define TWL_IDW_BAND_LAST               0xEBFF

#endif // !RC_INVOKED

}; // namespace TWL

#endif // __RESUTIL_H__
