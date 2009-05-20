//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MSGLOOP_H__
#define __MSGLOOP_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "ary.h"	// for TArray

namespace TWL
{

#ifndef USE_MESSAGELOOP_MINI

class CMessageFilter	// Interface for message filter support
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;
};

class CIdleHandler		// Interface for idle processing
{
public:
	virtual BOOL OnIdle() = 0;
};

typedef TArray<CMessageFilter*> CMessageFilterArray;
typedef TArray<CIdleHandler*> CIdleHandlerArray;

class CMessageLoop
{
public:
	CMessageFilterArray m_aMsgFilter;
	CIdleHandlerArray m_aIdleHandler;
	MSG m_msg;

// Message filter operations
	BOOL AddMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Add(pMessageFilter);
	}

	BOOL RemoveMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Remove(pMessageFilter);
	}

// Idle handler operations
	BOOL AddIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Add(pIdleHandler);
	}

	BOOL RemoveIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Remove(pIdleHandler);
	}

// message loop
	int Run()
	{
		BOOL bDoIdle = TRUE;
		int nIdleCount = 0;
		BOOL bRet;

		for(;;)
		{
			while(!::PeekMessage(&m_msg, NULL, 0, 0, PM_NOREMOVE) && bDoIdle)
			{
				if(!OnIdle(nIdleCount++))
					bDoIdle = FALSE;
			}

			bRet = ::GetMessage(&m_msg, NULL, 0, 0);

			if(bRet == -1)	// error, don't process
				continue;
			else if(!bRet)	// WM_QUIT, exit message loop
				break;

			if(!PreTranslateMessage(&m_msg))
			{
				::TranslateMessage(&m_msg);
				::DispatchMessage(&m_msg);
			}

			if(IsIdleMessage(&m_msg))
			{
				bDoIdle = TRUE;
				nIdleCount = 0;
			}
		}

		return (int)m_msg.wParam;
	}

	static BOOL IsIdleMessage(MSG* pMsg)
	{
		// These messages should NOT cause idle processing
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return FALSE;
		}

		return TRUE;
	}

// Overrideables
	// Override to change message filtering
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		// loop backwards
		for(int i = m_aMsgFilter.GetSize() - 1; i >= 0; i--)
		{
			CMessageFilter* pMessageFilter = m_aMsgFilter[i];
			if(pMessageFilter != NULL && pMessageFilter->PreTranslateMessage(pMsg))
				return TRUE;
		}

		return FALSE;	// not translated
	}

	// override to change idle processing
	virtual BOOL OnIdle(int /*nIdleCount*/)
	{
		for(int i = 0; i < m_aIdleHandler.GetSize(); i++)
		{
			CIdleHandler* pIdleHandler = m_aIdleHandler[i];
			if(pIdleHandler != NULL)
				pIdleHandler->OnIdle();
		}

		return FALSE;	// don't continue
	}
};

#else // USE_MESSAGELOOP_MINI

class CMessageLoopMini
{
public:
	MSG m_msg;

// message loop
	int Run()
	{
		BOOL bRet;

		while(bRet = ::GetMessage(&m_msg, NULL, 0, 0))
		{
			if(bRet == -1)	// error, don't process
				continue;

			::TranslateMessage(&m_msg);
			::DispatchMessage(&m_msg);
		}

		return (int)m_msg.wParam;
	}
};

typedef CMessageLoopMini CMessageLoop;

#endif // USE_MESSAGELOOP_MINI

}; // namespace TWL

#endif // __MSGLOOP_H__
