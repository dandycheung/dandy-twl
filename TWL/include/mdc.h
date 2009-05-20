//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MEMDC_H__
#define __MEMDC_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

#ifndef GetRectWidth
#define GetRectWidth(rc)	((rc).right - (rc).left)
#endif // GetRectWidth
#ifndef GetRectHeight
#define GetRectHeight(rc)	((rc).bottom - (rc).top)
#endif// GetRectHeight

class CMemDC
{
protected:
	HDC		m_hdcSrc;
	HDC		m_hdcMem;
	HBITMAP	m_hbmpMem;		// Offscreen bitmap
	HGDIOBJ	m_hbmpOldMem;	// bitmap originally found in CMemDC
	RECT	m_rcMem;		// Rectangle of drawing area.

public:
	CMemDC(HDC hdc, LPCRECT prc = NULL)
	{
		m_hdcSrc = hdc;
		m_hbmpOldMem = NULL;

		// Get the rectangle to draw
		if(prc == NULL)
			::GetClipBox(hdc, &m_rcMem);
		else
			m_rcMem = *prc;

		// Create a memory DC
		m_hdcMem = ::CreateCompatibleDC(hdc);
		::LPtoDP(hdc, (LPPOINT)&m_rcMem, 2);

		m_hbmpMem = ::CreateCompatibleBitmap(hdc, GetRectWidth(m_rcMem), GetRectHeight(m_rcMem));
		m_hbmpOldMem = ::SelectObject(m_hdcMem, m_hbmpMem);

		SetMapMode(m_hdcMem, ::GetMapMode(hdc));

		SIZE size;
		::GetWindowExtEx(hdc, &size);
		::SetWindowExtEx(m_hdcMem, size.cx, size.cy, NULL);

		::GetViewportExtEx(hdc, &size);
		::SetViewportExtEx(m_hdcMem, size.cx, size.cy, NULL);

		DPtoLP(hdc, (LPPOINT)&m_rcMem, 2);
		::SetWindowOrgEx(m_hdcMem, m_rcMem.left, m_rcMem.top, NULL);

		// Fill background 
		::SetBkColor(m_hdcMem, GetBkColor(hdc));
		::ExtTextOut(m_hdcMem, 0, 0, ETO_OPAQUE, &m_rcMem, NULL, 0, NULL);
	}
	
	~CMemDC()	
	{		
		// Copy the offscreen bitmap onto the screen.
		Output();
		
		// Swap back the original bitmap.
		::SelectObject(m_hdcMem, m_hbmpOldMem);
		::DeleteObject(m_hbmpMem);

		::DeleteDC(m_hdcMem);
	}
	
	// Allow usage as a normal HDC
	operator HDC() 
	{
		return m_hdcMem;
	}

	BOOL Output()
	{
		return ::BitBlt(m_hdcSrc, m_rcMem.left, m_rcMem.top, GetRectWidth(m_rcMem), GetRectHeight(m_rcMem),
			m_hdcMem, m_rcMem.left, m_rcMem.top, SRCCOPY);
	}

	HBITMAP GetBitmap()
	{
		return m_hbmpMem;
	}

	PRECT GetDrawRect()
	{
		return &m_rcMem;
	}
};

}; // namespace TWL

#endif // __MEMDC_H__
