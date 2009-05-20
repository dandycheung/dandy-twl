//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __TKNZR_H__
#define __TKNZR_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "str.h"
#include "strary.h"

namespace TWL
{

class CTokenizer
{
	CString m_strSource;
	CString m_strDelimiter;

public:
	CTokenizer(LPCTSTR pszSource = NULL, LPCTSTR pszDelimiter = TEXT(" "))
	{
		SetSource(pszSource);
		SetDelimiter(pszDelimiter);
	}

	~CTokenizer()
	{
	}

	void SetSource(LPCTSTR pszSource)
	{
		m_strSource = pszSource;
	}

	void SetDelimiter(LPCTSTR pszDelimiter)
	{
		m_strDelimiter = pszDelimiter;
	}

	void Tokenize(CStringArray& astrTokens)
	{
		int i = 0;
		int l = m_strSource.GetLength();

		CString strToken;
		while(i < l)
		{
			if(-1 == m_strDelimiter.Find(m_strSource[i]))
				strToken += m_strSource[i];
			else if(strToken.GetLength())
			{
				astrTokens.Add(strToken);
				strToken = TEXT("");
			}

			i++;

			if(i == l && strToken.GetLength())
				astrTokens.Add(strToken);
		}
	}
};

/*
void Demo()
{
	LPCTSTR pszSource = TEXT("I am your father, and you are my son.");

	CStringArray astr;
	CTokenizer tknzr(pszSource);
	tknzr.Tokenize(astr);

	for(int i=0; i<astr.GetSize(); i++)
		_tprintf(TEXT("%s\n"), (LPCTSTR)astr[i]);
}
// */

}; // namespace TWL

#endif // __TKNZR_H__
