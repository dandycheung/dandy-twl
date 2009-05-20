//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//
// Ported from MFC's CStringArray class, :-)
//

#ifndef __STRARY_H__
#define __STRARY_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <new>

#include "trcdbg.h"

namespace TWL
{

#define AfxThrowInvalidArgException()	throw "Invalid argument."

class CStringArray
{
public:
	CStringArray()
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}

	~CStringArray()
	{
		_DestructElements(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
	}

	INT_PTR GetSize() const
	{
		return m_nSize;
	}

	INT_PTR GetCount() const
	{
		return m_nSize;
	}

	BOOL IsEmpty() const
	{
		return m_nSize == 0;
	}

	INT_PTR GetUpperBound() const
	{
		return m_nSize - 1;
	}

	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1)
	{
		ASSERT(nNewSize >= 0);

		if(nNewSize < 0 )
			AfxThrowInvalidArgException();

		if(nGrowBy >= 0)
			m_nGrowBy = nGrowBy;  // set new size

		if(nNewSize == 0)
		{
			// shrink to nothing
			_DestructElements(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;

			m_pData = NULL;
			m_nSize = m_nMaxSize = 0;
		}
		else if(m_pData == NULL)
		{
			// create one with exact size
#ifdef SIZE_T_MAX
			ASSERT(nNewSize <= SIZE_T_MAX / sizeof(CString));    // no overflow
#endif
			m_pData = (CString*) new BYTE[nNewSize * sizeof(CString)];

			_ConstructElements(m_pData, nNewSize);

			m_nSize = m_nMaxSize = nNewSize;
		}
		else if(nNewSize <= m_nMaxSize)
		{
			// it fits
			if(nNewSize > m_nSize)
			{
				// initialize the new elements
				_ConstructElements(&m_pData[m_nSize], nNewSize - m_nSize);
			}
			else if(m_nSize > nNewSize)  // destroy the old elements
				_DestructElements(&m_pData[nNewSize], m_nSize - nNewSize);

			m_nSize = nNewSize;
		}
		else
		{
			// otherwise, grow array
			INT_PTR nGrowBy = m_nGrowBy;
			if(nGrowBy == 0)
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = min(1024, max(4, m_nSize / 8));
			}
			INT_PTR nNewMax;
			if(nNewSize < m_nMaxSize + nGrowBy)
				nNewMax = m_nMaxSize + nGrowBy;  // granularity
			else
				nNewMax = nNewSize;  // no slush

			ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
			
			if(nNewMax < m_nMaxSize)
				AfxThrowInvalidArgException();

#ifdef SIZE_T_MAX
			ASSERT(nNewMax <= SIZE_T_MAX / sizeof(CString)); // no overflow
#endif
			CString* pNewData = (CString*) new BYTE[(size_t)nNewMax * sizeof(CString)];

			// copy new data from old
			memcpy(pNewData, m_pData, (size_t)m_nSize * sizeof(CString));

			// construct remaining elements
			ASSERT(nNewSize > m_nSize);

			_ConstructElements(&pNewData[m_nSize], nNewSize - m_nSize);

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
			m_nMaxSize = nNewMax;
		}
	}

	// Clean up
	void FreeExtra()
	{
		if(m_nSize != m_nMaxSize)
		{
			// shrink to desired size
#ifdef SIZE_T_MAX
			ASSERT(m_nSize <= SIZE_T_MAX / sizeof(CString)); // no overflow
#endif
			CString* pNewData = NULL;
			if(m_nSize != 0)
			{
				pNewData = (CString*) new BYTE[m_nSize * sizeof(CString)];
				// copy new data from old
				memcpy(pNewData, m_pData, m_nSize * sizeof(CString));
			}

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}

	void RemoveAll()
	{
		SetSize(0, -1);
	}

	// Accessing elements
	const CString& GetAt(INT_PTR nIndex) const
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex];

		AfxThrowInvalidArgException();
	}

	void SetAt(INT_PTR nIndex, LPCTSTR newElement)
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			m_pData[nIndex] = newElement; 
		else
			AfxThrowInvalidArgException();
	}

	void SetAt(INT_PTR nIndex, const CString& newElement)
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			m_pData[nIndex] = newElement;
		else
			AfxThrowInvalidArgException();
	}

	CString& ElementAt(INT_PTR nIndex)
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex];
		
		AfxThrowInvalidArgException();
	}

	// Direct Access to the element data(may return NULL)
	const CString* GetData() const
	{
		return (const CString*)m_pData;
	}

	CString* GetData()
	{
		return (CString*)m_pData;
	}

	// Potentially growing the array
	void SetAtGrow(INT_PTR nIndex, LPCTSTR newElement)
	{
		ASSERT(nIndex >= 0);

		if(nIndex < 0)
			AfxThrowInvalidArgException();

		if(nIndex >= m_nSize)
			SetSize(nIndex + 1);

		m_pData[nIndex] = newElement;
	}

	void SetAtGrow(INT_PTR nIndex, const CString& newElement)
	{
		ASSERT(nIndex >= 0);

		if(nIndex < 0)
			AfxThrowInvalidArgException();

		if(nIndex >= m_nSize)
			SetSize(nIndex + 1);

		m_pData[nIndex] = newElement;
	}

	INT_PTR Add(LPCTSTR newElement)
	{
		INT_PTR nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex;
	}

	INT_PTR Add(const CString& newElement)
	{
		INT_PTR nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex;
	}

	INT_PTR Append(const CStringArray& src)
	{
		ASSERT(this != &src);   // cannot append to itself

		if(this == &src)
			AfxThrowInvalidArgException();

		INT_PTR nOldSize = m_nSize;
		SetSize(m_nSize + src.m_nSize);

		_CopyElements(m_pData + nOldSize, src.m_pData, src.m_nSize);

		return nOldSize;
	}

	void Copy(const CStringArray& src)
	{
		ASSERT(this != &src);   // cannot append to itself

		if(this != &src)
		{
			SetSize(src.m_nSize);
			_CopyElements(m_pData, src.m_pData, src.m_nSize);
		}
	}

	// overloaded operator helpers
	const CString& operator[](INT_PTR nIndex) const
	{
		return GetAt(nIndex);
	}

	CString& operator[](INT_PTR nIndex)
	{
		return ElementAt(nIndex);
	}

	// Operations that move elements around
	void InsertAt(INT_PTR nIndex, LPCTSTR newElement, INT_PTR nCount = 1)
	{
		// make room for new elements
		InsertEmpty(nIndex, nCount);

		// copy elements into the empty space
		CString temp = newElement;
		while(nCount--)
			m_pData[nIndex++] = temp;
	}

	void InsertAt(INT_PTR nIndex, const CString& newElement, INT_PTR nCount = 1)
	{
		// make room for new elements
		InsertEmpty(nIndex, nCount);

		// copy elements into the empty space
		while(nCount--)
			m_pData[nIndex++] = newElement;
	}

	void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1)
	{
		ASSERT(nIndex >= 0);
		ASSERT(nCount >= 0);
		ASSERT(nIndex + nCount <= m_nSize);

		if(nIndex < 0 || nCount < 0 || (nIndex + nCount > m_nSize))
			AfxThrowInvalidArgException();
			
		// just remove a range
		INT_PTR nMoveCount = m_nSize - (nIndex + nCount);

		_DestructElements(&m_pData[nIndex], nCount);

		if(nMoveCount)
			memmove(&m_pData[nIndex], &m_pData[nIndex + nCount], nMoveCount * sizeof(CString));

		m_nSize -= nCount;
	}

	void InsertAt(INT_PTR nStartIndex, CStringArray* pNewArray)
	{
		ASSERT(pNewArray != NULL);
		ASSERT(nStartIndex >= 0);

		if(pNewArray == NULL || nStartIndex < 0)
			AfxThrowInvalidArgException();

		if(pNewArray->GetSize() > 0)
		{
			InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
			for(INT_PTR i = 0; i < pNewArray->GetSize(); i++)
				SetAt(nStartIndex + i, pNewArray->GetAt(i));
		}
	}

protected:
	CString* m_pData;    // the actual array of data
	INT_PTR m_nSize;     // # of elements (upperBound - 1)
	INT_PTR m_nMaxSize;  // max allocated
	INT_PTR m_nGrowBy;   // grow amount

	void InsertEmpty(INT_PTR nIndex, INT_PTR nCount)
	{
		ASSERT(nIndex >= 0);    // will expand to meet need
		ASSERT(nCount > 0);     // zero or negative size not allowed

		if(nIndex < 0 || nCount <= 0)
			AfxThrowInvalidArgException();

		if(nIndex >= m_nSize)
		{
			// adding after the end of the array
			SetSize(nIndex + nCount);  // grow so nIndex is valid
		}
		else
		{
			// inserting in the middle of the array
			INT_PTR nOldSize = m_nSize;
			SetSize(m_nSize + nCount);  // grow it to new size

			// shift old data up to fill gap
			memmove(&m_pData[nIndex + nCount], &m_pData[nIndex], (nOldSize - nIndex) * sizeof(CString));

			// re-init slots we copied from
			_ConstructElements(&m_pData[nIndex], nCount);
		}

		// insert new value in the gap
		ASSERT(nIndex + nCount <= m_nSize);
	}

protected:
	// Special implementations for CStrings
	// it is faster to bit-wise copy a CString than to call an official
	//   constructor - since an empty CString can be bit-wise copied

	static void ConstructElement(CString* pNewData)
	{
		new(pNewData) CString;
	}

	static void DestructElement(CString* pOldData)
	{
		pOldData->~CString();
	}

	static void CopyElement(CString* pSrc, CString* pDest)
	{
		*pSrc = *pDest;
	}

	static void _ConstructElements(CString* pNewData, INT_PTR nCount)
	{
		ASSERT(nCount >= 0);

		while(nCount--)
		{
			ConstructElement(pNewData);
			pNewData++;
		}
	}

	static void _DestructElements(CString* pOldData, INT_PTR nCount)
	{
		ASSERT(nCount >= 0);

		while(nCount--)
		{
			DestructElement(pOldData);
			pOldData++;
		}
	}

	static void _CopyElements(CString* pDest, CString* pSrc, INT_PTR nCount)
	{
		ASSERT(nCount >= 0);

		while(nCount--)
		{
			*pDest = *pSrc;
			++pDest;
			++pSrc;
		}
	}
};

}; // namespace TWL

#endif // __STRARY_H__
