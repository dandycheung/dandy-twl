#ifndef __STRUTIL_H__
#define __STRUTIL_H__

/*
class CStringBuffer
{
protected:
	LPTSTR _psz;
	size_t _size;

public:
	CStringBuffer(size_t size = 128) : _psz(NULL), _size(0)
	{
		GetBuffer(size);
	}

	virtual ~CStringBuffer()
	{
		ReleaseBuffer();
	}

	BOOL GetBuffer(size_t size)
	{
		if(_size > size)
			return TRUE;

		LPTSTR psz = new TCHAR[size];
		if(!psz)
			return FALSE;

		memset(psz, 0, sizeof(TCHAR) * size);

		if(_psz)
		{
			memcpy(psz, _psz, sizeof(TCHAR) * _size);

			delete[] _psz;
			_psz = NULL;
			_size = 0;
		}

		_psz = psz;
		_size = size;

		return TRUE;
	}

	void ReleaseBuffer()
	{
		if(_psz)
		{
			delete[] _psz;
			_psz = NULL;
			_size = 0;
		}
	}

	operator LPCTSTR() const
	{
		return _psz;
	}

	operator LPTSTR()
	{
		return _psz;
	}
};
// */

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

#if defined(_INC_COMMCTRL) && !defined(NOLISTVIEW)
class CListViewItemTextGetter : public CStringBuffer
{
public:
	LPCTSTR Get(HWND hwnd, int iItem, int iSubItem = 0)
	{
		LVITEM lvi = { 0 };
		lvi.iSubItem = iSubItem;

		int nLen = 128;
		int nRes;
		do
		{
			nLen *= 2;
			if(!GetBuffer(nLen))
				break;

			lvi.cchTextMax = nLen;
			lvi.pszText = *this;
			nRes  = (int)::SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&lvi);
		} while (nRes == nLen-1);

		return *this;
	}
};
#endif // _INC_COMMCTRL && !NOLISTVIEW

class CRegDataGetter : public CByteBuffer
{
	DWORD _dwDataType;
	DWORD _dwDataSize;

public:
	CRegDataGetter() : _dwDataSize(0), _dwDataType(0)
	{
	}

public:
	PBYTE Get(HKEY hkReg, LPCTSTR pszName)
	{
		LONG lRet = ERROR_SUCCESS;

		int nSize = 512;
		do
		{
			if(!GetBuffer(nSize))
				break;

			_dwDataSize = nSize;
			lRet = RegQueryValueEx(hkReg, pszName, 0, &_dwDataType, _p, &_dwDataSize);
			nSize *= 2;
		} while(lRet == ERROR_MORE_DATA);

		if(lRet != ERROR_SUCCESS)
			ReleaseBuffer();

		return *this;
	}

	void ReleaseBuffer()
	{
		CByteBuffer::ReleaseBuffer();
		_dwDataSize = 0;
	}

	DWORD GetDataSize() const
	{
		return _dwDataSize;
	}

	DWORD GetDataType() const
	{
		return _dwDataType;
	}
};

#endif // __STRUTIL_H__
