//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

//
// A MFC CPtrArray compatible array class, for porting code from MFC more quickly.
//

#ifndef __PTRARY_H__
#define __PTRARY_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWLEX
{

class CPtrArray
{
public:
// Construction
	CPtrArray()
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}

// Attributes
	int GetSize() const
	{
		return m_nSize;
	}

	int GetUpperBound() const
	{
		return m_nSize - 1;
	}

	void SetSize(int nNewSize, int nGrowBy = -1)
	{
		ASSERT(nNewSize >= 0);

		if(nGrowBy != -1)
			m_nGrowBy = nGrowBy;  // set new size

		if(nNewSize == 0)
		{
			// shrink to nothing
			delete[] (BYTE*)m_pData;

			m_pData = NULL;
			m_nSize = m_nMaxSize = 0;
		}
		else if(m_pData == NULL)
		{
			// create one with exact size
#ifdef SIZE_T_MAX
			ASSERT(nNewSize <= SIZE_T_MAX / sizeof(void*));    // no overflow
#endif
			m_pData = (void**) new BYTE[nNewSize * sizeof(void*)];

			memset(m_pData, 0, nNewSize * sizeof(void*));  // zero fill

			m_nSize = m_nMaxSize = nNewSize;
		}
		else if(nNewSize <= m_nMaxSize)
		{
			// it fits
			if(nNewSize > m_nSize)
			{
				// initialize the new elements
				memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));
			}

			m_nSize = nNewSize;
		}
		else
		{
			// otherwise, grow array
			int nGrowBy = m_nGrowBy;
			if(nGrowBy == 0)
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = min(1024, max(4, m_nSize / 8));
			}

			int nNewMax;
			if(nNewSize < m_nMaxSize + nGrowBy)
				nNewMax = m_nMaxSize + nGrowBy;  // granularity
			else
				nNewMax = nNewSize;  // no slush

			ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
			ASSERT(nNewMax <= SIZE_T_MAX/sizeof(void*)); // no overflow
#endif
			void** pNewData = (void**) new BYTE[nNewMax * sizeof(void*)];

			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(void*));

			// construct remaining elements
			ASSERT(nNewSize > m_nSize);

			memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
			m_nMaxSize = nNewMax;
		}
	}

// Operations
	// Clean up
	void FreeExtra()
	{
		if(m_nSize != m_nMaxSize)
		{
			// shrink to desired size
#ifdef SIZE_T_MAX
			ASSERT(m_nSize <= SIZE_T_MAX / sizeof(void*)); // no overflow
#endif
			void** pNewData = NULL;
			if(m_nSize != 0)
			{
				pNewData = (void**) new BYTE[m_nSize * sizeof(void*)];
				// copy new data from old
				memcpy(pNewData, m_pData, m_nSize * sizeof(void*));
			}

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}

	void RemoveAll()
	{
		SetSize(0);
	}

	// Accessing elements
	void* GetAt(int nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}

	void SetAt(int nIndex, void* newElement)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement;
	}

	void*& ElementAt(int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}

	// Direct Access to the element data (may return NULL)
	const void** GetData() const
	{
		return (const void**)m_pData;
	}

	void** GetData()
	{
		return (void**)m_pData;
	}

	// Potentially growing the array
	void SetAtGrow(int nIndex, void* newElement)
	{
		ASSERT(nIndex >= 0);

		if(nIndex >= m_nSize)
			SetSize(nIndex+1);
		m_pData[nIndex] = newElement;
	}

	int Add(void* newElement)
	{
		int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex;
	}

	int Append(const CPtrArray& src)
	{
		ASSERT(this != &src);   // cannot append to itself

		int nOldSize = m_nSize;
		SetSize(m_nSize + src.m_nSize);

		memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));

		return nOldSize;
	}

	void Copy(const CPtrArray& src)
	{
		ASSERT(this != &src);   // cannot append to itself

		SetSize(src.m_nSize);

		memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void*));
	}

	// overloaded operator helpers
	void* operator[](int nIndex) const
	{
		return GetAt(nIndex);
	}

	void*& operator[](int nIndex)
	{
		return ElementAt(nIndex);
	}

	// Operations that move elements around
	void InsertAt(int nIndex, void* newElement, int nCount = 1)
	{
		ASSERT(nIndex >= 0);    // will expand to meet need
		ASSERT(nCount > 0);     // zero or negative size not allowed

		if(nIndex >= m_nSize)
		{
			// adding after the end of the array
			SetSize(nIndex + nCount);  // grow so nIndex is valid
		}
		else
		{
			// inserting in the middle of the array
			int nOldSize = m_nSize;
			SetSize(m_nSize + nCount);  // grow it to new size
			// shift old data up to fill gap
			memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
				(nOldSize-nIndex) * sizeof(void*));

			// re-init slots we copied from
			memset(&m_pData[nIndex], 0, nCount * sizeof(void*));
		}

		// insert new value in the gap
		ASSERT(nIndex + nCount <= m_nSize);

		// copy elements into the empty space
		while(nCount--)
			m_pData[nIndex++] = newElement;
	}

	void RemoveAt(int nIndex, int nCount = 1)
	{
		ASSERT(nIndex >= 0);
		ASSERT(nCount >= 0);
		ASSERT(nIndex + nCount <= m_nSize);

		// just remove a range
		int nMoveCount = m_nSize - (nIndex + nCount);

		if(nMoveCount)
			memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
				nMoveCount * sizeof(void*));
		m_nSize -= nCount;
	}

	void InsertAt(int nStartIndex, CPtrArray* pNewArray)
	{
		ASSERT(pNewArray != NULL);
		ASSERT(nStartIndex >= 0);

		if(pNewArray->GetSize() > 0)
		{
			InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
			for(int i = 0; i < pNewArray->GetSize(); i++)
				SetAt(nStartIndex + i, pNewArray->GetAt(i));
		}
	}

// Implementation
protected:
	void** m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CPtrArray()
	{
		delete[] (BYTE*)m_pData;
	}

protected:
	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};

} // namespace TWLEX

#endif // __PTRARY_H__
