//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __TDC_H__
#define __TDC_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

#define THREAD_DATA_WINDOW	0

#define PTD_NOREMOVE		0
#define PTD_REMOVE			1

struct CThreadData
{
	void* m_pData;
	UINT m_uType;
	DWORD m_dwThreadId;
	CThreadData* m_pNext;
};

struct CThreadDataCenter
{
	CThreadData* m_pDataList;
	CRITICAL_SECTION m_cs;

	CThreadDataCenter() : m_pDataList(NULL)
	{
		::InitializeCriticalSection(&m_cs);
	}

	virtual ~CThreadDataCenter()
	{
		::DeleteCriticalSection(&m_cs);
	}
};

inline
CThreadDataCenter* GetThreadDataCenter()
{
	static CThreadDataCenter center;
	return &center;
}

inline
void AddThreadData(void* pData, UINT uType = THREAD_DATA_WINDOW, CThreadDataCenter* pCenter = GetThreadDataCenter())
{
	::EnterCriticalSection(&pCenter->m_cs);

	CThreadData* pEntry = new CThreadData;
	if(pEntry)
	{
		pEntry->m_pData = pData;
		pEntry->m_uType = uType;
		pEntry->m_dwThreadId = ::GetCurrentThreadId();

		pEntry->m_pNext = pCenter->m_pDataList;
		pCenter->m_pDataList = pEntry;
	}

	::LeaveCriticalSection(&pCenter->m_cs);
}

inline
void* PeekThreadData(UINT uType = THREAD_DATA_WINDOW, CThreadDataCenter* pCenter = GetThreadDataCenter(), UINT uFlag = PTD_NOREMOVE)
{
	void* pData = NULL;

	::EnterCriticalSection(&pCenter->m_cs);

	CThreadData* pEntry = pCenter->m_pDataList;
	if(pEntry != NULL)
	{
		DWORD dwThreadId = ::GetCurrentThreadId();
		CThreadData* pPrev = NULL;
		while(pEntry != NULL)
		{
			if(pEntry->m_dwThreadId == dwThreadId && pEntry->m_uType == uType)
			{
				pData = pEntry->m_pData;

				if(uFlag & PTD_REMOVE)
				{
					if(pPrev == NULL)
						pCenter->m_pDataList = pEntry->m_pNext;
					else
						pPrev->m_pNext = pEntry->m_pNext;

					delete pEntry;
				}

				break;
			}

			pPrev = pEntry;
			pEntry = pEntry->m_pNext;
		}
	}

	::LeaveCriticalSection(&pCenter->m_cs);

	return pData;
}

inline
void* ExtractThreadData(UINT uType = THREAD_DATA_WINDOW, CThreadDataCenter* pCenter = GetThreadDataCenter())
{
	return PeekThreadData(uType, pCenter, PTD_REMOVE);
}

}; // namespace TWL

#endif // __TDC_H__
