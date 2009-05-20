//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2009 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __FILE_H__
#define __FILE_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "handle.h"

#define HRESULT_FROM_LAST_ERROR()	HRESULT_FROM_WIN32(::GetLastError())

#pragma pack(push, 8)

namespace TWL
{

class CFile : public CHandle
{
public:
	CFile() throw()
	{
	}
	
	CFile(CFile& rf) throw()
		: CHandle(rf)		// Transfers ownership
	{
	}
	
	explicit CFile(HANDLE hFile) throw()
		: CHandle(hFile)	// Takes ownership
	{
	}

	HRESULT Create(
		LPCTSTR szFilename,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		LPSECURITY_ATTRIBUTES lpsa = NULL,
		HANDLE hTemplateFile = NULL) throw()
	{
		HANDLE hFile = ::CreateFile(
			szFilename,
			dwDesiredAccess,
			dwShareMode,
			lpsa,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);

		if(hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_LAST_ERROR();

		Attach(hFile);
		return S_OK;
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize) throw()
	{
		DWORD nBytesRead = 0;
		BOOL bSuccess = ::ReadFile(m_handle, pBuffer, nBufSize, &nBytesRead, NULL);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();
			
		if(nBytesRead != nBufSize)
			return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);

		return S_OK;
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD& nBytesRead) throw()
	{
		BOOL bSuccess = ::ReadFile(m_handle, pBuffer, nBufSize, &nBytesRead, NULL);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

#ifndef _WIN32_WCE
	// this function will usually return HRESULT_FROM_WIN32(ERROR_IO_PENDING)
	// indicating succesful queueing of the operation
	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		LPOVERLAPPED pOverlapped) throw()
	{
		BOOL bSuccess = ::ReadFile(m_handle, pBuffer, nBufSize, NULL, pOverlapped);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		LPOVERLAPPED pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine) throw()
	{
		BOOL bSuccess = ::ReadFileEx(m_handle, pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}
#endif // _WIN32_WCE

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD* pnBytesWritten = NULL) throw()
	{
		DWORD nBytesWritten;
		if(pnBytesWritten == NULL)
			pnBytesWritten = &nBytesWritten;
			
		BOOL bSuccess = ::WriteFile(m_handle, pBuffer, nBufSize, pnBytesWritten, NULL);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

#ifndef _WIN32_WCE
	// this function will usually return HRESULT_FROM_WIN32(ERROR_IO_PENDING)
	// indicating succesful queueing of the operation
	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		LPOVERLAPPED pOverlapped) throw()
	{
		BOOL bSuccess = ::WriteFile(m_handle, pBuffer, nBufSize, NULL, pOverlapped);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		LPOVERLAPPED pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine) throw()
	{
		BOOL bSuccess = ::WriteFileEx(m_handle, pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	// this function returns HRESULT_FROM_WIN32(ERROR_IO_INCOMPLETE)
	// if bWait is false and the operation is still pending
	HRESULT GetOverlappedResult(
		LPOVERLAPPED pOverlapped,
		DWORD& dwBytesTransferred,
		BOOL bWait) throw()
	{
		BOOL bSuccess = ::GetOverlappedResult(m_handle, pOverlapped, &dwBytesTransferred, bWait);
		if(!bSuccess)
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}
#endif // _WIN32_WCE

	HRESULT Seek(LONGLONG nOffset, DWORD dwFrom = FILE_CURRENT) throw()
	{
		TWLASSERT(dwFrom == FILE_BEGIN || dwFrom == FILE_END || dwFrom == FILE_CURRENT);

		LARGE_INTEGER liOffset;
		liOffset.QuadPart = nOffset;
		DWORD nNewPos = ::SetFilePointer(m_handle, liOffset.LowPart, &liOffset.HighPart, dwFrom);
		if(nNewPos == INVALID_SET_FILE_POINTER)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			if(FAILED(hr))
				return hr;
		}

		return S_OK;
	}

	HRESULT GetPosition(ULONGLONG& nPos) const throw()
	{
		LARGE_INTEGER liOffset;
		liOffset.QuadPart = 0;
		liOffset.LowPart = ::SetFilePointer(m_handle, 0, &liOffset.HighPart, FILE_CURRENT);
		if(liOffset.LowPart == INVALID_SET_FILE_POINTER)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			if (FAILED(hr))
				return hr;
		}
		
		nPos = liOffset.QuadPart;

		return S_OK;
	}

	HRESULT Flush() throw()
	{
		if(!::FlushFileBuffers(m_handle))
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

#ifndef _WIN32_WCE
	HRESULT LockRange(ULONGLONG nPos, ULONGLONG nCount) throw()
	{
		LARGE_INTEGER liPos;
		liPos.QuadPart = nPos;

		LARGE_INTEGER liCount;
		liCount.QuadPart = nCount;

		if(!::LockFile(m_handle, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart))
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	HRESULT UnlockRange(ULONGLONG nPos, ULONGLONG nCount) throw()
	{
		LARGE_INTEGER liPos;
		liPos.QuadPart = nPos;

		LARGE_INTEGER liCount;
		liCount.QuadPart = nCount;

		if(!::UnlockFile(m_handle, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart))
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}
#endif // _WIN32_WCE

	HRESULT SetSize(ULONGLONG nNewLen) throw()
	{
		HRESULT hr = Seek(nNewLen, FILE_BEGIN);
		if(FAILED(hr))
			return hr;

		if(!::SetEndOfFile(m_handle))
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	HRESULT GetSize(ULONGLONG& nLen) const throw()
	{
		ULARGE_INTEGER liFileSize;
		liFileSize.LowPart = ::GetFileSize(m_handle, &liFileSize.HighPart);
		if(liFileSize.LowPart == INVALID_FILE_SIZE)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			if(FAILED(hr))
				return hr;
		}

		nLen = liFileSize.QuadPart;

		return S_OK;
	}
};

// This class allows the creation of a temporary file that is written to.
// When the entire file has been successfully written it will be closed and given
// it's proper file name if required.
class CTemporaryFile
{
public:
	CTemporaryFile() throw()
	{
	}

	~CTemporaryFile() throw()
	{
		// Ensure that the temporary file is closed and deleted, if necessary.
		if(m_file.m_handle != NULL)
			Close();
	}

	HRESULT Create(LPCTSTR pszDir = NULL, DWORD dwDesiredAccess = GENERIC_WRITE) throw()
	{
		TCHAR szPath[_MAX_PATH]; 
		TCHAR tmpFileName[_MAX_PATH]; 

		if(pszDir == NULL)
		{
			DWORD dwRet = GetTempPath(_MAX_DIR, szPath);
			if(dwRet == 0)	// Couldn't find temporary path;
				return HRESULT_FROM_LAST_ERROR();

			if(dwRet > _MAX_DIR)
				return DISP_E_BUFFERTOOSMALL;
		}
		else
		{
			if(lstrlen(pszDir) >= _countof(szPath))
				return DISP_E_BUFFERTOOSMALL;

			lstrcpyn(szPath, pszDir, _countof(szPath));
		}

		if(!GetTempFileName(szPath, _T("TFR"), 0, tmpFileName))	// Couldn't create temporary filename;
			return HRESULT_FROM_LAST_ERROR();

		tmpFileName[_countof(tmpFileName)-1] = _T('\0');

		lstrcpyn(m_szTempFileName, tmpFileName, _countof(m_szTempFileName));

		SECURITY_ATTRIBUTES secatt;
		secatt.nLength = sizeof(secatt);
		secatt.lpSecurityDescriptor = NULL;
		secatt.bInheritHandle = TRUE;

		m_dwAccess = dwDesiredAccess;

		return m_file.Create(
			m_szTempFileName,
			m_dwAccess,
			0,
			CREATE_ALWAYS,
#ifndef _WIN32_WCE
			FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_ATTRIBUTE_TEMPORARY,
#else // _WIN32_WCE
			FILE_ATTRIBUTE_NORMAL,
#endif // _WIN32_WCE
			&secatt);
	}

	HRESULT Close(LPCTSTR szNewName = NULL) throw()
	{
		// This routine is called when we are finished writing to the 
		// temporary file, so we now just want to close it and copy
		// it to the actual filename we want it to be called.

		// So let's close it first.
		m_file.Close();

		// no new name so delete it
		if(szNewName == NULL)
		{
			::DeleteFile(m_szTempFileName);
			return S_OK;
		}

		// delete any existing file and move our temp file into it's place
		if(!::DeleteFile(szNewName))
		{
			DWORD dwError = GetLastError();
			if(dwError != ERROR_FILE_NOT_FOUND)
				return HRESULT_FROM_WIN32(dwError);
		}

		if(!::MoveFile(m_szTempFileName, szNewName))
			return HRESULT_FROM_LAST_ERROR();

		return S_OK;
	}

	HRESULT HandsOff() throw()
	{
		m_file.Flush();
		m_file.Close();

		return S_OK;
	}

	HRESULT HandsOn() throw()
	{
		HRESULT hr = m_file.Create(
			m_szTempFileName,
			m_dwAccess,
			0,
			OPEN_EXISTING);
		if(FAILED(hr))
			return hr;

		return m_file.Seek(0, FILE_END);
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD& nBytesRead) throw()
	{
		return m_file.Read(pBuffer, nBufSize, nBytesRead);
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD* pnBytesWritten = NULL) throw()
	{
		return m_file.Write(pBuffer, nBufSize, pnBytesWritten);
	}

	HRESULT Seek(LONGLONG nOffset, DWORD dwFrom = FILE_CURRENT) throw()
	{
		return m_file.Seek(nOffset, dwFrom);
	}

	HRESULT GetPosition(ULONGLONG& nPos) const throw()
	{
		return m_file.GetPosition(nPos);
	}

	HRESULT Flush() throw()
	{
		return m_file.Flush();
	}

#ifndef _WIN32_WCE
	HRESULT LockRange(ULONGLONG nPos, ULONGLONG nCount) throw()
	{
		return m_file.LockRange(nPos, nCount);
	}

	HRESULT UnlockRange(ULONGLONG nPos, ULONGLONG nCount) throw()
	{
		return m_file.UnlockRange(nPos, nCount);
	}
#endif // _WIN32_WCE

	HRESULT SetSize(ULONGLONG nNewLen) throw()
	{
		return m_file.SetSize(nNewLen);
	}

	HRESULT GetSize(ULONGLONG& nLen) const throw()
	{
		return m_file.GetSize(nLen);
	}

	operator HANDLE() throw()
	{
		return m_file;
	}

	LPCTSTR TempFileName() throw()
	{
		return m_szTempFileName;
	}

private:
	CFile m_file;
	TCHAR m_szTempFileName[_MAX_FNAME + 1];
	DWORD m_dwAccess;
};

class CFileMappingBase
{
public:
	CFileMappingBase() throw()
	{
		m_pData = NULL;
		m_hMapping = NULL;
	}

	~CFileMappingBase() throw()
	{
		Unmap();
	}

	HRESULT MapFile(
		HANDLE hFile,
		SIZE_T nMappingSize = 0,
		ULONGLONG nOffset = 0,
		DWORD dwMappingProtection = PAGE_READONLY,
		DWORD dwViewDesiredAccess = FILE_MAP_READ) throw()
	{
		TWLASSERT(hFile != INVALID_HANDLE_VALUE && hFile != NULL);

		ULARGE_INTEGER liFileSize;
		liFileSize.LowPart = ::GetFileSize(hFile, &liFileSize.HighPart);
		if(liFileSize.QuadPart < nMappingSize)
			liFileSize.QuadPart = nMappingSize;

		m_hMapping = ::CreateFileMapping(hFile, NULL, dwMappingProtection, liFileSize.HighPart, liFileSize.LowPart, 0);
		if(m_hMapping == NULL)
			return HRESULT_FROM_LAST_ERROR();

		if(nMappingSize == 0)
			m_nMappingSize = (SIZE_T)(liFileSize.QuadPart - nOffset);
		else
			m_nMappingSize = nMappingSize;

		m_dwViewDesiredAccess = dwViewDesiredAccess;
		m_nOffset.QuadPart = nOffset;

		m_pData = ::MapViewOfFile(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize);
		if(m_pData == NULL)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}

	HRESULT MapSharedMem(
		SIZE_T nMappingSize,
		LPCTSTR szName,
		BOOL* pbAlreadyExisted = NULL,
		LPSECURITY_ATTRIBUTES lpsa = NULL,
		DWORD dwMappingProtection = PAGE_READWRITE,
		DWORD dwViewDesiredAccess = FILE_MAP_ALL_ACCESS) throw()
	{
		TWLASSERT(nMappingSize > 0);
		TWLASSERT(szName != NULL); // if you just want a regular chunk of memory, use a heap allocator

		m_nMappingSize = nMappingSize;

		ULARGE_INTEGER nSize;
		nSize.QuadPart = nMappingSize;
		m_hMapping = ::CreateFileMapping(INVALID_HANDLE_VALUE, lpsa, dwMappingProtection, nSize.HighPart, nSize.LowPart, szName);
		if(m_hMapping == NULL)
			return HRESULT_FROM_LAST_ERROR();

		if(pbAlreadyExisted != NULL)
			*pbAlreadyExisted = (GetLastError() == ERROR_ALREADY_EXISTS);

		m_dwViewDesiredAccess = dwViewDesiredAccess;
		m_nOffset.QuadPart = 0;

		m_pData = ::MapViewOfFile(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize);
		if(m_pData == NULL)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			::CloseHandle(m_hMapping);
			return hr;
		}

		return S_OK;
	}

#ifndef _WIN32_WCE
	HRESULT OpenMapping(
		LPCTSTR szName,
		SIZE_T nMappingSize,
		ULONGLONG nOffset = 0,
		DWORD dwViewDesiredAccess = FILE_MAP_ALL_ACCESS) throw()
	{
		TWLASSERT(szName != NULL); // if you just want a regular chunk of memory, use a heap allocator

		m_nMappingSize = nMappingSize;
		m_dwViewDesiredAccess = dwViewDesiredAccess;

		m_hMapping = ::OpenFileMapping(m_dwViewDesiredAccess, FALSE, szName);
		if(m_hMapping == NULL)
			return HRESULT_FROM_LAST_ERROR();

		m_dwViewDesiredAccess = dwViewDesiredAccess;
		m_nOffset.QuadPart = nOffset;

		m_pData = ::MapViewOfFile(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize);
		if(m_pData == NULL)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			::CloseHandle(m_hMapping);
			return hr;
		}

		return S_OK;
	}
#endif // _WIN32_WCE

	HRESULT Unmap() throw()
	{
		HRESULT hr = S_OK;

		if(m_pData != NULL)
		{
			if(!::UnmapViewOfFile(m_pData))
				hr = HRESULT_FROM_LAST_ERROR();
				
			m_pData = NULL;
		}
		
		if (m_hMapping != NULL)
		{
			if(!::CloseHandle(m_hMapping) && SUCCEEDED(hr))
				hr = HRESULT_FROM_LAST_ERROR();

			m_hMapping = NULL;
		}
		return hr;
	}

	void* GetData() const throw()
	{
		return m_pData;
	}

	HANDLE GetHandle() const throw()
	{
		return m_hMapping;
	}

	SIZE_T GetMappingSize() throw()
	{
		return m_nMappingSize;
	}

	HRESULT CopyFrom(CFileMappingBase& orig) throw()
	{
		if(this == &orig)
			return S_OK;
			
		TWLASSERT(orig.m_pData != NULL);
		TWLASSERT(orig.m_hMapping != NULL);

		m_dwViewDesiredAccess = orig.m_dwViewDesiredAccess;
		m_nOffset.QuadPart = orig.m_nOffset.QuadPart;
		m_nMappingSize = orig.m_nMappingSize;

		if(!::DuplicateHandle(GetCurrentProcess(), orig.m_hMapping, GetCurrentProcess(),
				&m_hMapping, NULL, TRUE, DUPLICATE_SAME_ACCESS))
			return HRESULT_FROM_LAST_ERROR();

		m_pData = ::MapViewOfFile(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize);
		if(m_pData == NULL)
		{
			HRESULT hr = HRESULT_FROM_LAST_ERROR();
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}

private:
	CFileMappingBase(CFileMappingBase& orig);
	CFileMappingBase& operator=(CFileMappingBase& orig);

private:
	void* m_pData;
	SIZE_T m_nMappingSize;
	HANDLE m_hMapping;
	ULARGE_INTEGER m_nOffset;
	DWORD m_dwViewDesiredAccess;
};

template <typename T = BYTE>
class CFileMapping : public CFileMappingBase
{
public:
	operator T*() const throw()
	{
		return reinterpret_cast<T*>(GetData());
	}
};

}; // namespace TWL

#pragma pack(pop)

#endif //__FILE_H__
