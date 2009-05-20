//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MODUTIL_H__
#define __MODUTIL_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "trcdbg.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
//

namespace TWL
{

#define GetModuleInstance	GetCurrentModuleHandle

static HMODULE WINAPI GetCurrentModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;

	DWORD dwRet = VirtualQuery(GetCurrentModuleHandle,
		&mbi, sizeof(MEMORY_BASIC_INFORMATION));

	if(dwRet > 0)
	{
		ASSERT(mbi.Type & MEM_IMAGE);
		return (HMODULE)mbi.AllocationBase;
	}

	return NULL;
}

}; // namespace TWL

#endif // __MODUTIL_H__
