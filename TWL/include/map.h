//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MAP_H__
#define __MAP_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

template<class Key, class Value> class TMapIter;

////////////////////////////////////
// class TMap

template<class Key, class Value> 
class TMap
{
public:
	TMap(UINT uBucketArrayLength = 99);
	virtual ~TMap();

	// Operations
	Value* Lookup(const Key& key);
	const Value* Lookup(const Key& key) const;
	bool Lookup(const Key& key, Value& targ);
	
	void SetAt(const Key& key, Value value);
	void RemoveAll();
	void RemoveAt(const Key& key);

	UINT GetSize() const { return m_uCount; };

//
//	**Implementation**, do NOT use anything under here. the only reason why some of this is
//	public is that there seems to be a bug in the MSVC 6.0 compiler that even though the
//	TMapIter is a friend it still cannot access CMaps privates.
public:
	struct BucketEntry
	{
		Value m_Value;
		Key	  m_Key;

		BucketEntry* m_pNext;
	};
	const UINT m_uBucketArrayLength;
	BucketEntry** m_pEntrys;

private:
	// richg - 19990224 - VC5 chokes if the declarator 'class' is specified here.
	// it produces a 'redefinition error'. Everything works fine if the
	// 'class' declarator is omitted, though.

	friend class TMapIter<class Key, class Value>;
	// friend class TMapIter<Key, Value>;

	BucketEntry* Find(UINT uHash, const Key& key) const;

	UINT m_uCount;

// Prevent copy constructors.
private:
	TMap(const TMap&);
	TMap& operator=(const TMap&);
};

template<class Key, class Value>
TMap<Key, Value>::TMap(UINT uBucketArrayLength)
	: m_uBucketArrayLength(uBucketArrayLength),	m_uCount(0)
{
	m_pEntrys = new BucketEntry*[m_uBucketArrayLength];

	for(UINT i=0; i<m_uBucketArrayLength; i++)
		m_pEntrys[i] = NULL;
}

template<class Key, class Value>
TMap<Key, Value>::~TMap()
{
	RemoveAll();
	delete[] m_pEntrys;
	m_pEntrys = NULL;
}

template<class Key, class Value>
void TMap<Key, Value>::RemoveAll()
{
	if(m_uCount )
	{
		for(UINT i=0; i<m_uBucketArrayLength; i++)
		{
			BucketEntry* pEntry = m_pEntrys[i];
			while(pEntry)
			{
				BucketEntry* pEntryNext = pEntry->m_pNext;
				delete pEntry;
				pEntry = pEntryNext;
			}

			m_pEntrys[i] = NULL;
		}

		m_uCount = 0;
	}
}

template<class Key, class Value>
Value* TMap<Key, Value>::Lookup(const Key& key)
{
	UINT uHash = HashIt(key) % m_uBucketArrayLength;
	BucketEntry* pEntry = Find(uHash, key);
	return (pEntry != NULL) ? &pEntry->m_Value : NULL;
}

template<class Key, class Value>
bool TMap<Key, Value>::Lookup(const Key& key, Value& targ)
{
	UINT uHash = HashIt(key) % m_uBucketArrayLength;
	BucketEntry* pEntry = Find(uHash, key);
	if(pEntry != NULL)
	{
		targ = pEntry->m_Value;
		return true;
	}
	return false;
}

template<class Key, class Value>
const Value* TMap<Key, Value>::Lookup(const Key& key) const
{
	UINT uHash = HashIt(key) % m_uBucketArrayLength;
	BucketEntry* pEntry = Find(uHash, key);
	return (pEntry != NULL) ? &pEntry->m_Value : NULL;
}


template<class Key, class Value>
TMap<Key, Value>::BucketEntry* TMap<Key, Value>::Find(UINT uHash, const Key& key) const
{
	ASSERT(m_pEntrys);

	for(BucketEntry* pEntry = m_pEntrys[uHash]; pEntry != NULL; pEntry = pEntry->m_pNext)
	{
		if(IsEqualElements(pEntry->m_Key, key))
			return pEntry;
	}

	return NULL;
}

template<class Key, class Value>
void TMap<Key, Value>::SetAt(const Key &key, Value value)
{
	UINT uHash = HashIt(key) % m_uBucketArrayLength;
	BucketEntry* pEntry = Find(uHash, key);

	if(pEntry == NULL)
	{
		pEntry = new BucketEntry;
		m_uCount++;
		pEntry->m_Key = key;

		BucketEntry* pRootEntry = m_pEntrys[uHash];
		m_pEntrys[uHash] = pEntry;
		pEntry->m_pNext = pRootEntry;
	}

	pEntry->m_Value = value;
}

template<class Key, class Value>
void TMap<Key, Value>::RemoveAt(const Key& key)
{
	UINT uHash = HashIt(key) % m_uBucketArrayLength;
	BucketEntry* pLastItem = NULL;
	for(BucketEntry* pEntry = m_pEntrys[uHash]; pEntry != NULL; pLastItem = pEntry, pEntry = pEntry->m_pNext)
	{
		if(IsEqualElements(pEntry->m_Key, key))
		{
			if(pLastItem)
				pLastItem->m_pNext = pEntry->m_pNext;
			else
				m_pEntrys[uHash] = pEntry->m_pNext;

			delete pEntry;
			m_uCount--;

			break;
		}
	}
}

////////////////////////////////////
// class TMapIter

template<class Key, class Value> 
class TMapIter
{
public:
	TMapIter(const TMap<Key, Value>& map);

	// Operations
	void Reset();

	//	Navigation
	void Next();

	// State
	bool End() const;

	// Content
	Value& GetValue() const;
	const Key& GetKey() const;	 

private:
	// (this is init'd on construct)
	const TMap<Key, Value>& m_Map;
	TMap<Key, Value>::BucketEntry* m_pBucket;
	UINT m_iEntry;

// Private construcotrs to prevent accidently copy
private:
	TMapIter();
	TMapIter(const TMapIter&);
	TMapIter& operator=(const TMapIter&);
};

template<class Key, class Value>
TMapIter<Key, Value>::TMapIter(const TMap<Key, Value>& map)
	: m_Map(map), m_pBucket(NULL), m_iEntry(0)
{
	Reset();
}

//	Operations
template<class Key, class Value>
void TMapIter<Key, Value>::Reset()
{
	m_pBucket = NULL;

	// Set pointers to first entry that contains some data
	for(m_iEntry=0; m_iEntry<m_Map.m_uBucketArrayLength; m_iEntry++)
	{
		TMap<Key,Value>::BucketEntry* pEntry = m_Map.m_pEntrys[m_iEntry];
		if(pEntry)
		{
			m_pBucket = pEntry;
			return;
		}
	}
}

//	Navigation
template<class Key, class Value>
void TMapIter<Key, Value>::Next()
{
	if(m_pBucket->m_pNext)
	{
		m_pBucket = m_pBucket->m_pNext;
		return;
	}

	// Get next array bucket
	while(++m_iEntry < m_Map.m_uBucketArrayLength)
	{
		TMap<Key, Value>::BucketEntry* pEntry = m_Map.m_pEntrys[m_iEntry];
		if(pEntry)
		{
			m_pBucket = pEntry;
			return;
		}
	}

	m_pBucket = NULL;
}

// State
template<class Key, class Value>
bool TMapIter<Key, Value>::End() const
{
	return m_pBucket == NULL;
}

//
// Content
template<class Key, class Value>
Value& TMapIter<Key, Value>::GetValue() const
{
	ASSERT(m_pBucket);
	return m_pBucket->m_Value;
}

template<class Key, class Value>
const Key& TMapIter<Key, Value>::GetKey() const
{
	ASSERT(m_pBucket);
	return m_pBucket->m_Key;
}

////////////////////////////////////
// Helper functions

//	Hash value for simple types
template<class Key>
inline UINT HashIt(const Key& key)
{
	// Default implementation.
	return ((UINT)(void*)(DWORD)key) >> 4;
}


//	Hash value for a LPCTSTR
inline UINT HashIt(LPCTSTR pcszText)
{
	UINT uHash = 0;
	while(*pcszText)
	{
		uHash = uHash << 1 ^ (UINT)toupper(*pcszText++);
	}

	return uHash;
}

// Default implementation for comparison
template<class Key>
inline bool IsEqualElements(const Key& lhs, const Key& rhs)
{
	return lhs == rhs;
}

//	Overriden comparison for LPCTSTR 
inline bool IsEqualElements(LPCTSTR pcszLHS, LPCTSTR pcszRHS)
{
	return lstrcmpi(pcszLHS, pcszRHS) == 0;
}

}; // namespace TWL

#endif // __MAP_H__
