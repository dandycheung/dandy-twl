//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __CTRLFNT_H__
#define __CTRLFNT_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

class CControlFontImpl
{
protected:
	BOOL m_bCopy;
	HFONT m_hfnt;

public:
	CControlFontImpl() : m_hfnt(NULL), m_bCopy(TRUE)
	{
	}

	~CControlFontImpl()
	{
		if(m_hfnt && m_bCopy)
			DeleteObject(m_hfnt);
	}

public:
	void SetFont(HFONT hfnt, BOOL bCopy = TRUE)
	{
		if(m_hfnt)
		{
			if(m_bCopy)
				DeleteObject(m_hfnt);

			m_hfnt = NULL;
		}

		if(!hfnt)
			return;

		if(!bCopy)
			m_hfnt = hfnt;
		else
		{
			LOGFONT lf;
			GetObject(hfnt, sizeof(LOGFONT), &lf);

			m_hfnt = CreateFontIndirect(&lf);
		}

		m_bCopy = bCopy;
	}

	HFONT GetFont() const
	{
		return m_hfnt;
	}

	operator HFONT()
	{
		return GetFont();
	}

	CControlFontImpl& operator=(HFONT hfnt)
	{
		SetFont(hfnt);
		return *this;
	}
};

}; // namespace TWL

#endif // __CTRLFNT_H__
