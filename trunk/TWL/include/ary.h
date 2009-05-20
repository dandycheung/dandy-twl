//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __ARY_H__
#define __ARY_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#if (_MSC_VER < 1300)	// because of a visual c++ 6.0 bug
#pragma warning(disable: 4291) // allow placement new
#endif

#define USE_INSERT_AT

namespace TWL
{

template <class T>
class TArray
{
public:
	T* m_aT;
	int m_nSize;
	int m_nAllocSize;

// Construction/destruction
	TArray() : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{
	}

	~TArray()
	{
		RemoveAll();
	}

// Operations
	int GetSize() const
	{
		return m_nSize;
	}

	BOOL Add(T& t)
	{
#ifndef USE_INSERT_AT
		if(m_nSize == m_nAllocSize)
		{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);

			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)
				return FALSE;

			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}

		m_nSize++;
		SetAtIndex(m_nSize - 1, t);

		return TRUE;
#else
		return InsertAt(m_nSize, t);
#endif
	}

#ifdef USE_INSERT_AT
	BOOL InsertAt(int nIndex, T& t)
	{
		if(nIndex < 0 || nIndex > m_nSize)
			return FALSE;

		if(m_nSize == m_nAllocSize)
		{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);

			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)
				return FALSE;

			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}

		if(nIndex != m_nSize)
			memmove((void*)&m_aT[nIndex + 1], (void*)&m_aT[nIndex], (m_nSize - nIndex) * sizeof(T));

		m_nSize++;
		SetAtIndex(nIndex, t);

		return TRUE;
	}
#endif

	BOOL Remove(T& t)
	{
		int nIndex = Find(t);
		if(nIndex == -1)
			return FALSE;

		return RemoveAt(nIndex);
	}

	BOOL RemoveAt(int nIndex)
	{
/* // ATL original code
		if(nIndex != (m_nSize - 1))
		{
			m_aT[nIndex].~T();
			memmove((void*)&m_aT[nIndex], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
		}

		m_nSize--;

		return TRUE;
// */
		if(nIndex < 0 || nIndex >= m_nSize)
			return FALSE;

		m_aT[nIndex].~T();
		if(nIndex != (m_nSize - 1))
			memmove((void*)&m_aT[nIndex], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));

		m_nSize--;

		return TRUE;
	}

	void RemoveAll()
	{
		if(m_aT != NULL)
		{
			for(int i=0; i<m_nSize; i++)
				m_aT[i].~T();

			free(m_aT);
			m_aT = NULL;
		}

		m_nSize = 0;
		m_nAllocSize = 0;
	}

	T& operator[] (int nIndex) const
	{
		// ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aT[nIndex];
	}

	T* GetData() const
	{
		return m_aT;
	}

// Implementation
	class Wrapper
	{
	public:
		Wrapper(T& _t) : t(_t)
		{
		}

		template <class _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}

		T t;
	};
	void SetAtIndex(int nIndex, T& t)
	{
		// ASSERT(nIndex >= 0 && nIndex < m_nSize);
#if (_MSC_VER >= 1300)	// because of a visual c++ 6.0 bug
#pragma warning(disable: 4291) // allow placement new
#endif
		new(&m_aT[nIndex]) Wrapper(t);
#if (_MSC_VER >= 1300)
#pragma warning(default: 4291)
#endif
	}

	int Find(T& t) const
	{
		for(int i=0; i<m_nSize; i++)
		{
			if(m_aT[i] == t)
				return i;
		}

		return -1;  // not found
	}
};

// for arrays of simple types
template <class T>
class TValArray : public TArray<T>
{
public:
	BOOL Add(T t)
	{
		return TArray<T>::Add(t);
	}

	BOOL Remove(T t)
	{
		return TArray<T>::Remove(t);
	}

	T operator[] (int nIndex) const
	{
		return TArray<T>::operator[](nIndex);
	}
};

}; // namespace TWL

#endif // __ARY_H__
