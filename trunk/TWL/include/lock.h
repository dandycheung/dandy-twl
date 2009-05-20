//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __LOCK_H__
#define __LOCK_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWLEX
{

class CNullSyncObject
{
public:
	CNullSyncObject() {}
	virtual ~CNullSyncObject() {}

	void Lock() {}
	void Unlock() {}
};

/////////////////////////////////////////////////////////////////////////////
// CCriticalSection

class CCriticalSection
{
public:
	CCriticalSection() { ::InitializeCriticalSection(&m_cs); }
	virtual ~CCriticalSection() { ::DeleteCriticalSection(&m_cs); }

public:
	operator CRITICAL_SECTION*() { return (CRITICAL_SECTION*)&m_cs; }

	void Lock() { ::EnterCriticalSection(&m_cs); }
	void Unlock() { ::LeaveCriticalSection(&m_cs); }

protected:
	CRITICAL_SECTION m_cs;
};

/////////////////////////////////////////////////////////////////////////////
// CLocker

template<typename T>
class TLocker
{
public:
	TLocker(T& lock, BOOL bLock = TRUE) : _lock(lock) { if(bLock) Lock(); }
	~TLocker() { Unlock(); }

	void Lock() { _lock.Lock(); }
	void Unlock() { _lock.Unlock(); }

protected:
	T& _lock;
};

} // namespace TWLEX

#endif // __LOCK_H__
