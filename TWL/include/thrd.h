//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __THRD_H__
#define __THRD_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _WINDOWS_
#include <windows.h>
#endif // _WINDOWS_

namespace TWL
{

#define CloseThread			CloseHandle
#define CloseEvent			CloseHandle
#define GetLastWin32Error()	HRESULT_FROM_WIN32(GetLastError())

class CThread
{
	DWORD m_dwThreadId;
	HANDLE m_hThread;

	HANDLE m_hEvent;
	HRESULT m_hrInit;

public:
	CThread()
	{
		m_dwThreadId = 0;
		m_hThread = NULL;
		m_hEvent  = NULL;
		m_hrInit  = S_OK;
	}

	virtual ~CThread()
	{
		CloseThread(m_hThread);
		CloseEvent(m_hEvent);
	}

protected:
	static DWORD WINAPI StaticThreadProc(void* pv)
	{
		return(((CThread*)pv)->ThreadProc());
	}

	DWORD ThreadProc()
	{
		m_hrInit = ThreadInit();

		if(m_hEvent)
			SetEvent(m_hEvent);

		if(m_hrInit == S_OK)
			ThreadExec();

		ThreadTerm();
		ThreadExit();

		return 0;
	}

public:
	virtual HRESULT ThreadInit()
	{
		return S_OK;
	}

	virtual void ThreadExec()
	{
	}

	virtual void ThreadTerm()
	{
	}

	virtual void ThreadExit()
	{
	}

	/// Operations
	HRESULT Launch(BOOL fWait)
	{
		DWORD dwResult;
		DWORD dwStackSize = 0;

		if(fWait)
		{
			m_hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);

			if(m_hEvent == NULL)
				return GetLastWin32Error();
		}

		m_hThread = CreateThread(NULL, dwStackSize, &CThread::StaticThreadProc, this, 0, &m_dwThreadId);

		if(m_hThread == NULL)
			return GetLastWin32Error();

		if(fWait)
		{
			dwResult = WaitForSingleObject(m_hEvent, INFINITE);

			ASSERT(dwResult == WAIT_OBJECT_0);

			CloseEvent(m_hEvent);
			m_hEvent = NULL;

			return m_hrInit;
		}

		return S_OK;
	}

	void Shutdown(DWORD dwTimeOut)
	{
		if(m_hThread && GetCurrentThreadId() != m_dwThreadId)
		{
			DWORD dwExitCode;

			WaitForSingleObject(m_hThread, dwTimeOut);

			if(GetExitCodeThread(m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
				TerminateThread(m_hThread, 1);
		}
	}
};

}; // namespace TWL

#endif // __THRD_H__
