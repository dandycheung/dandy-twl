#ifndef __LOCK_H__
#define __LOCK_H__

#ifndef ASSERT
#define ASSERT(e)
#endif // ASSERT

/////////////////////////////////////////////////////////////////////////////
// CNullSyncObj

class CNullSyncObj
{
public:
	CNullSyncObj() {}
	virtual ~CNullSyncObj() {}

// Operations
public:
	BOOL Lock() { return TRUE; }
	BOOL Lock(DWORD /* dwTimeOut */) { return TRUE; }
	BOOL Unlock() { return TRUE; }
};

/////////////////////////////////////////////////////////////////////////////
// CCriticalSection

class CCriticalSection
{
public:
	CCriticalSection() { ::InitializeCriticalSection(&m_sect); }
	virtual ~CCriticalSection() { ::DeleteCriticalSection(&m_sect); }

// Attributes
public:
	operator CRITICAL_SECTION*() { return (CRITICAL_SECTION*)&m_sect; }
	CRITICAL_SECTION m_sect;

// Operations
public:
	BOOL Lock() { ::EnterCriticalSection(&m_sect); return TRUE; }
	BOOL Lock(DWORD /* dwTimeOut */) { return Lock(); }
	BOOL Unlock() { ::LeaveCriticalSection(&m_sect); return TRUE; }
};

/////////////////////////////////////////////////////////////////////////////
// CSingleLock

template <typename T>
class CSingleLock
{
public:
	CSingleLock(T* pObject, BOOL bInitialLock = FALSE)
	{
		ASSERT(pObject != NULL);

		m_pObject = pObject;
		m_bAcquired = FALSE;

		if(bInitialLock)
			Lock();
	}

	~CSingleLock()
	{
		Unlock();
	}

// Operations
public:
	BOOL Lock(DWORD dwTimeOut = INFINITE)
	{
		ASSERT(m_pObject != NULL);
		ASSERT(!m_bAcquired);

		m_bAcquired = m_pObject->Lock(dwTimeOut);
		return m_bAcquired;
	}

	BOOL Unlock()
	{
		ASSERT(m_pObject != NULL);
		if(m_bAcquired)
			m_bAcquired = !m_pObject->Unlock();

		// successfully unlocking means it isn't acquired
		return !m_bAcquired;
	}

	BOOL IsLocked()
	{
		return m_bAcquired;
	}

protected:
	T* m_pObject;
	BOOL m_bAcquired;
};

#endif // __LOCK_H__
