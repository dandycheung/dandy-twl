//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __ONLYINST_H__
#define __ONLYINST_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

class CSingleInstance
{
	HANDLE m_hMutex;
	DWORD m_dwError;

public:
	CSingleInstance(LPCTSTR pszMutexName = NULL)
	{
		TCHAR szAppPath[MAX_PATH];

		if(pszMutexName == NULL)
		{
			GetModuleFileName(NULL, szAppPath, MAX_PATH);
			pszMutexName = szAppPath;

			// The Windows DO NOT allows the '\\' char was in the object name
			for(LPTSTR pszPath = szAppPath; *pszPath; pszPath++)
				if(*pszPath == _T('\\'))
					*pszPath = _T('/');
		}

		m_hMutex = CreateMutex(NULL, TRUE, pszMutexName);
		m_dwError = GetLastError();
	}

	~CSingleInstance()
	{
		if(m_hMutex != NULL)
			CloseHandle(m_hMutex);
	}

	BOOL IsFirstInstance() const
	{
		return m_dwError == 0;
	}

private:
	// Not implemeted
	CSingleInstance(const CSingleInstance&);
	CSingleInstance& operator=(const CSingleInstance&);
};

}; // namespace TWL

#endif // __ONLYINST_H__
