//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __SYSFNT_H__
#define __SYSFNT_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

#define SF_CAPTION		1
#define SF_SMCAPTION	2
#define SF_MENU			3
#define SF_STATUS		4
#define SF_MESSAGE		5
#define SF_FIRST		SF_CAPTION
#define SF_LAST			SF_MESSAGE

class CSystemFont
{
protected:
	HFONT m_hfnt;
	LOGFONT m_lf;
	bool m_bValid;

public:
	CSystemFont(UINT uType, bool bCreate = true) : m_hfnt(NULL), m_bValid(false)
	{
		ZeroMemory(&m_lf, sizeof(LOGFONT));

		if(uType < SF_FIRST || uType > SF_LAST)
			return;

		NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
		if(!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, FALSE))
			return;

		switch(uType)
		{
		case SF_CAPTION:
			m_lf = ncm.lfCaptionFont;
			break;
		case SF_SMCAPTION:
			m_lf = ncm.lfSmCaptionFont;
			break;
		case SF_MENU:
			m_lf = ncm.lfMenuFont;
			break;
		case SF_STATUS:
			m_lf = ncm.lfStatusFont;
			break;
		case SF_MESSAGE:
			m_lf = ncm.lfMessageFont;
			break;
		}

		if(bCreate)
			m_hfnt = CreateFontIndirect(&m_lf);

		m_bValid = true;
	}

	~CSystemFont()
	{
		if(m_hfnt)
			DeleteObject(m_hfnt);
	}

	operator HFONT() const
	{
		return m_hfnt;
	}

	const LOGFONT& GetLogFont() const
	{
		return m_lf;
	}
};

}; // namespace TWL

#endif // __SYSFNT_H__
