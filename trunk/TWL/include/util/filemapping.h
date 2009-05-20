#ifndef __FILEMAPPING_H__
#define __FILEMAPPING_H__

class CFileMapping
{
protected:
	HANDLE _hFile;
	HANDLE _hFileMapping;
	PVOID _pvView;

public:
	// Ctor for real file mapping object
	CFileMapping(
		LPCTSTR pszFile,
		DWORD dwFileAccess = GENERIC_READ | GENERIC_WRITE,
		DWORD dwFileShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
		DWORD dwFileMappingProtect = PAGE_READWRITE,
		DWORD dwFileMappingAccess = FILE_MAP_ALL_ACCESS,
		PSECURITY_ATTRIBUTES psa = NULL,
		BOOL bMap = TRUE,
		LPCTSTR pszObject = NULL)
		: _hFile(NULL), _hFileMapping(NULL), _pvView(NULL)
	{
		_hFile = CreateFile(pszFile, dwFileAccess, dwFileShareMode, psa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(!_hFile || _hFile == INVALID_HANDLE_VALUE)
			return;

		_hFileMapping = CreateFileMapping(_hFile, psa, dwFileMappingProtect, 0, 0, pszObject);
		if(_hFileMapping && bMap)
			_pvView = Map(0, 0, 0, dwFileMappingAccess);
	}

	// Ctor for shared memory object, not implemented, your duty? :-)
	CFileMapping(DWORD dwMaximumSizeHigh = 0, DWORD dwMaximumSizeLow = 0, LPCTSTR pszObject = NULL)
		: _hFile(NULL), _hFileMapping(NULL), _pvView(NULL)
	{
		_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwMaximumSizeLow, pszObject);
		if(_hFileMapping)
			_pvView = Map(dwMaximumSizeLow, 0, 0, FILE_MAP_ALL_ACCESS, NULL);
	}

	~CFileMapping()
	{
		Unmap();

		if(_hFileMapping)
			CloseHandle(_hFileMapping);

		if(_hFile && _hFile != INVALID_HANDLE_VALUE)
			CloseHandle(_hFile);
	}

	PVOID Map(SIZE_T dwNumberOfBytesToMap = 0, DWORD dwFileOffsetHigh = 0, DWORD dwFileOffsetLow = 0,
		DWORD dwAccess = FILE_MAP_ALL_ACCESS, PVOID pvBaseAddress = NULL)
	{
		Unmap();

		if(_hFileMapping == NULL)
			return NULL;

		_pvView = MapViewOfFile(_hFileMapping, dwAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);

		return _pvView;
	}

	void Unmap()
	{
		if(_pvView)
		{
			UnmapViewOfFile(_pvView);
			_pvView = NULL;
		}
	}

	BOOL IsMapped()
	{
		return _pvView != NULL;
	}

	PVOID GetView()
	{
		return _pvView;
	}
};

#endif // __FILEMAPPING_H__
