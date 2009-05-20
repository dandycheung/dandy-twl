//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

//
// A MFC CMap<...> compatible map class, for porting code from MFC more quickly.
// But, it can be thread-safe.
//

#ifndef __MAP_H__
#define __MAP_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <new>
#include "..\lock.h"

#ifndef ASSERT
#include <crtdbg.h>
#define ASSERT(e)	_ASSERTE(e)
#endif // ASSERT

namespace TWLEX
{

#ifdef USE_THREAD_SAFE_MAP
typedef TLocker<CCriticalSection> CLocker;
#define DECLARE_LOCK	CCriticalSection m_cs
#else // USE_THREAD_SAFE_MAP
typedef TLocker<CNullSyncObject> CLocker;
#define DECLARE_LOCK	CNullSyncObject m_cs
#endif // USE_THREAD_SAFE_MAP
#define USE_LOCKER		CLocker locker(m_cs)

// abstract iteration position
struct __POSITION { };
typedef __POSITION* POSITION;

#define BEFORE_START_POSITION ((POSITION)-1L)

template<class TYPE>
void ConstructElements(TYPE* pElements, int nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
	for(; nCount--; pElements++)
		::new((void*)pElements) TYPE;
}

template<class TYPE>
void DestructElements(TYPE* pElements, int nCount)
{
	// call the destructor(s)
	for(; nCount--; pElements++)
		pElements->~TYPE();
}

template<class TYPE>
void CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	// default is element-copy using assignment
	while(nCount--)
		*pDest++ = *pSrc++;
}

template<class TYPE, class ARG_TYPE>
BOOL CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	return *pElement1 == *pElement2;
}

template<class ARG_KEY>
UINT HashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return ((UINT)(void*)(DWORD)key) >> 4;
}

struct CPlex     // warning variable length structure
{
	CPlex* pNext;

	void* data()
	{
		return this + 1;
	}

	static CPlex* PASCAL Create(CPlex*& pHead, UINT nMax, UINT cbElement)
	{
		// like 'calloc' but no zero fill, and may throw memory exceptions
		ASSERT(nMax > 0 && cbElement > 0);
		CPlex* p = (CPlex*) new BYTE[sizeof(CPlex) + nMax * cbElement];
		p->pNext = pHead;
		pHead = p;  // change head (adds in reverse order for simplicity)
		return p;
	}

	void FreeDataChain()       // free this one and links
	{
		CPlex* p = this;
		while(p != NULL)
		{
			BYTE* bytes = (BYTE*)p;
			CPlex* pNext = p->pNext;
			delete[] bytes;
			p = pNext;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class TYPE>
class CHasher
{
public:
	TYPE& _obj;
	CHasher(TYPE obj) : _obj(obj)
	{
	}

	operator const UINT() const
	{
		return ((UINT)(void*)(DWORD)_obj) >> 4;
	}
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE, class HASHER=CHasher<KEY> >
class CMap
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
		KEY key;
		VALUE value;
	};

	// Critical section for thread safe operating
	DECLARE_LOCK;

public:
// Construction
	CMap(int nBlockSize = 10)
	{
		USE_LOCKER;
		ASSERT(nBlockSize > 0);

		m_pHashTable = NULL;
		m_nHashTableSize = 17;  // default size
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
	}

// Attributes
	// number of elements
	int GetCount() // const
	{
		USE_LOCKER;
		return m_nCount;
	}
	
	BOOL IsEmpty() // const
	{
		USE_LOCKER;
		return m_nCount == 0;
	}

	// Lookup
	BOOL Lookup(ARG_KEY key, VALUE& rValue) // const
	{
		USE_LOCKER;

		UINT nHash;
		CAssoc* pAssoc = GetAssocAt(key, nHash);
		if(pAssoc == NULL)
			return FALSE;  // not in map

		rValue = pAssoc->value;
		return TRUE;
	}

// Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key)
	{
		USE_LOCKER;

		UINT nHash;
		CAssoc* pAssoc;
		if((pAssoc = GetAssocAt(key, nHash)) == NULL)
		{
			if (m_pHashTable == NULL)
				InitHashTable(m_nHashTableSize);

			// it doesn't exist, add a new Association
			pAssoc = NewAssoc();
			pAssoc->nHashValue = nHash;
			pAssoc->key = key;
			// 'pAssoc->value' is a constructed object, nothing more

			// put into hash table
			pAssoc->pNext = m_pHashTable[nHash];
			m_pHashTable[nHash] = pAssoc;
		}

		return pAssoc->value;  // return new reference
	}

	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue)
	{
		(*this)[key] = newValue;
	}

	// removing existing (key, ?) pair
	BOOL RemoveKey(ARG_KEY key)
	{
		USE_LOCKER;

		// remove key - return TRUE if removed
		if(m_pHashTable == NULL)
			return FALSE;  // nothing in the table

		CAssoc** ppAssocPrev;
		ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];

		CAssoc* pAssoc;
		for(pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if(CompareElements(&pAssoc->key, &key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				FreeAssoc(pAssoc);
				return TRUE;
			}

			ppAssocPrev = &pAssoc->pNext;
		}

		return FALSE;  // not found
	}

	void RemoveAll()
	{
		USE_LOCKER;

		if(m_pHashTable != NULL)
		{
			// destroy elements (values and keys)
			for(UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
			{
				CAssoc* pAssoc;
				for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
				  pAssoc = pAssoc->pNext)
				{
					DestructElements<VALUE>(&pAssoc->value, 1);
					DestructElements<KEY>(&pAssoc->key, 1);
				}
			}
		}

		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;

		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;
	}

	// iterating all (key, value) pairs
	POSITION GetStartPosition() // const
	{
		USE_LOCKER;
		return (m_nCount == 0) ? NULL : BEFORE_START_POSITION;
	}

	void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) // const
	{
		USE_LOCKER;
		ASSERT(m_pHashTable != NULL);  // never call on empty map

		CAssoc* pAssocRet = (CAssoc*)rNextPosition;

		if(pAssocRet == (CAssoc*)BEFORE_START_POSITION)
		{
			// find the first association
			for(UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
				if((pAssocRet = m_pHashTable[nBucket]) != NULL)
					break;
		}

		// find next association
		CAssoc* pAssocNext;
		if((pAssocNext = pAssocRet->pNext) == NULL)
		{
			// go to next bucket
			for(UINT nBucket = pAssocRet->nHashValue + 1;
			  nBucket < m_nHashTableSize; nBucket++)
				if((pAssocNext = m_pHashTable[nBucket]) != NULL)
					break;
		}

		rNextPosition = (POSITION)pAssocNext;

		// fill in return data
		rKey = pAssocRet->key;
		rValue = pAssocRet->value;
	}

	// advanced features for derived classes
	UINT GetHashTableSize() const
	{
		return m_nHashTableSize;
	}

	void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE)
	{
		USE_LOCKER;

		// Used to force allocation of a hash table or to override the default
		//   hash table size of (which is fairly small)
		ASSERT(m_nCount == 0);
		ASSERT(nHashSize > 0);

		if(m_pHashTable != NULL)
		{
			// free hash table
			delete[] m_pHashTable;
			m_pHashTable = NULL;
		}

		if(bAllocNow)
		{
			m_pHashTable = new CAssoc*[nHashSize];
			memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
		}

		m_nHashTableSize = nHashSize;
	}

	UINT HashKey(ARG_KEY key)
	{
#if 0
		return HashKey<ARG_KEY>(key);
#else
		return UINT(HASHER(key));
#endif
	}

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CAssoc* NewAssoc()
	{
		USE_LOCKER;

		if(m_pFreeList == NULL)
		{
			// add another block
			CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));
			// chain them into free list
			CMap::CAssoc* pAssoc = (CMap::CAssoc*)newBlock->data();
			// free in reverse order to make it easier to debug
			pAssoc += m_nBlockSize - 1;
			for(int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
			{
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
			}
		}

		CMap::CAssoc* pAssoc = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		m_nCount++;

		ConstructElements<KEY>(&pAssoc->key, 1);
		ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

		return pAssoc;
	}

	void FreeAssoc(CAssoc* pAssoc)
	{
		USE_LOCKER;

		DestructElements<VALUE>(&pAssoc->value, 1);
		DestructElements<KEY>(&pAssoc->key, 1);
		pAssoc->pNext = m_pFreeList;
		m_pFreeList = pAssoc;
		m_nCount--;

		// if no more elements, cleanup completely
		if(m_nCount == 0)
			RemoveAll();
	}

	CAssoc* GetAssocAt(ARG_KEY key, UINT& nHash) // const
	{
		USE_LOCKER;

		// find association (or return NULL)
		nHash = HashKey(key) % m_nHashTableSize;

		if(m_pHashTable == NULL)
			return NULL;

		// see if it exists
		CAssoc* pAssoc;
		for(pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if(CompareElements(&pAssoc->key, &key))
				return pAssoc;
		}

		return NULL;
	}

public:
	~CMap()
	{
		RemoveAll();
	}
};

} // namespace TWLEX

#endif // __MAP_H__
