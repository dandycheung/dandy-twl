//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __THRDMGR_H__
#define __THRDMGR_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

template<typename T, UINT WM_NEWFRAME = WM_USER>
class TThreadManager
{
	// thread init param
	struct _RunData
	{
		LPTSTR pszCmdLine;
		int iCmdShow;
	};

public:
	TThreadManager() : m_dwCount(0)
	{
	}

	// thread proc
	static DWORD WINAPI RunThread(LPVOID lpData)
	{
		_RunData* pData = (_RunData*)lpData;

		T wndFrame;
		if(wndFrame.CreateEx() == NULL)
		{
			// Frame window creation failed
			return 0;
		}

		wndFrame.ShowWindow(pData->iCmdShow);
		::SetForegroundWindow(wndFrame);	// Win95 needs this

		delete pData;

		CMessageLoop theLoop;
		return theLoop.Run();
	}

	DWORD m_dwCount;
	HANDLE m_arrThreadHandles[MAXIMUM_WAIT_OBJECTS - 1];

// Operations
	DWORD AddThread(LPTSTR pszCmdLine, int iCmdShow)
	{
		if(m_dwCount == (MAXIMUM_WAIT_OBJECTS - 1))
		{
			// Can not create any more threads
			return 0;
		}

		_RunData* pData = new _RunData;
		pData->pszCmdLine = pszCmdLine;
		pData->iCmdShow = iCmdShow;

		DWORD dwThreadId;
		HANDLE hThread = ::CreateThread(NULL, 0, RunThread, pData, 0, &dwThreadId);
		if(hThread == NULL)
		{
			// Can not create thread
			return 0;
		}

		m_arrThreadHandles[m_dwCount] = hThread;
		m_dwCount++;

		return dwThreadId;
	}

	void RemoveThread(DWORD dwIndex)
	{
		::CloseHandle(m_arrThreadHandles[dwIndex]);

		if(dwIndex != (m_dwCount - 1))
			m_arrThreadHandles[dwIndex] = m_arrThreadHandles[m_dwCount - 1];

		m_dwCount--;
	}

	int Run(LPTSTR pszCmdLine, int iCmdShow)
	{
		// force message queue to be created
		MSG msg;
		::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

		AddThread(pszCmdLine, iCmdShow);

		int iRet = m_dwCount;
		DWORD dwRet;
		while(m_dwCount > 0)
		{
			dwRet = ::MsgWaitForMultipleObjects(m_dwCount, m_arrThreadHandles, FALSE, INFINITE, QS_ALLINPUT);

			if(dwRet == 0xFFFFFFFF)
				::MessageBox(NULL, _T("A system error occured!"), NULL, MB_OK);
			else if(dwRet >= WAIT_OBJECT_0 && dwRet <= (WAIT_OBJECT_0 + m_dwCount - 1))
				RemoveThread(dwRet - WAIT_OBJECT_0);
			else if(dwRet == (WAIT_OBJECT_0 + m_dwCount))
			{
				::GetMessage(&msg, NULL, 0, 0);
				if(msg.message == WM_NEWFRAME)
					AddThread(_T(""), SW_SHOWNORMAL);
				else
					::MessageBeep((UINT)-1);
			}
			else
				::MessageBeep((UINT)-1);
		}

		return iRet;
	}
};

}; // namespace TWL

#endif // __THRDMGR_H__
