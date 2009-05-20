//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MCR_H__
#define __MCR_H__

#define MCR_INLINE	inline
#define DECLARE_CLASS_NAME(c)	LPCTSTR GetClassName() { return (c); }

#define GetRectWidth(rc)		((rc).right - (rc).left)
#define GetRectHeight(rc)		((rc).bottom - (rc).top)

MCR_INLINE
POINT MakePointFromLPARAM(const LPARAM lParam)
{
	POINT pt = { (int)(short)(WORD)((DWORD)lParam & 0xFFFF), (int)(short)(WORD)((DWORD)lParam >> 16) };
	return pt;
}
#define MAKEPOINT(lParam)		MakePointFromLPARAM(lParam)

#endif // __MCR_H__
