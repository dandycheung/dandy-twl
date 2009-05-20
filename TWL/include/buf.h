//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __BUF_H__
#define __BUF_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

template <typename _T, size_t sizDef>
class TDataBuffer
{
protected:
	_T* _p;
	size_t _size;

public:
	TDataBuffer(size_t size = sizDef) : _p(NULL), _size(0)
	{
		GetBuffer(size);
	}

	~TDataBuffer()
	{
		ReleaseBuffer();
	}

	BOOL GetBuffer(size_t size)
	{
		if(_size >= size)
			return TRUE;

		_T* p = new _T[size];
		if(!p)
			return FALSE;

		memset(p, 0, sizeof(_T) * size);

		if(_p)
		{
			memcpy(p, _p, sizeof(_T) * _size);

			delete[] _p;
			_p = NULL;
			_size = 0;
		}

		_p = p;
		_size = size;

		return TRUE;
	}

	void ReleaseBuffer()
	{
		if(_p)
		{
			delete[] _p;
			_p = NULL;
			_size = 0;
		}
	}

	size_t GetSize() const
	{
		return _size;
	}

	operator const _T*() const
	{
		return _p;
	}

	operator _T*()
	{
		return _p;
	}
};

typedef TDataBuffer<TCHAR, 128> CStringBuffer;
typedef TDataBuffer<BYTE, 512> CByteBuffer;

}; // namespace TWL

#endif // __BUF_H__
