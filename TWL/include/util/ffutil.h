#ifndef __FFUTIL_H__
#define __FFUTIL_H__

#ifndef UNUSED_ALWAYS
#define UNUSED_ALWAYS(x) x
#endif // UNUSED_ALWAYS

class CFileFind
{
public:
	CFileFind()
	{
		m_pFoundInfo = NULL;
		m_pNextInfo = NULL;
		m_hContext = NULL;
		m_pszRoot = NULL;
		m_chDirSeparator = TEXT('\\');
	}

	virtual ~CFileFind()
	{
		Close();
	}

// Attributes
public:
	DWORD GetLength() const
	{
		ASSERT(m_hContext != NULL);

		if(m_pFoundInfo != NULL)
			return ((LPWIN32_FIND_DATA)m_pFoundInfo)->nFileSizeLow;

		return 0;
	}

#if defined(_X86_) || defined(_ALPHA_)
	__int64 GetLength64() const
	{
		ASSERT(m_hContext != NULL);

		if(m_pFoundInfo != NULL)
			return ((LPWIN32_FIND_DATA)m_pFoundInfo)->nFileSizeLow +
					(((LPWIN32_FIND_DATA)m_pFoundInfo)->nFileSizeHigh << 32);

		return 0;
	}
#endif

	virtual LPCTSTR GetFileName() const
	{
		ASSERT(m_hContext != NULL);

		if(m_pFoundInfo != NULL)
			return ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;

		return NULL;
	}

	virtual BOOL GetFilePath(LPTSTR pszPath, PDWORD pdwPath) const
	{
		ASSERT(m_hContext != NULL);

		if(pdwPath == NULL)
			return FALSE;

		DWORD dwLengthRoot = lstrlen(m_pszRoot);
		DWORD dwLength = dwLengthRoot;
		if(m_pszRoot[dwLength - 1] != TEXT('\\') && m_pszRoot[dwLength - 1] != TEXT('/'))
			dwLength++;
		dwLength += lstrlen(GetFileName());
		dwLength++;	// for NULL terminator

		if(*pdwPath < dwLength)
		{
			*pdwPath = dwLength;
			return FALSE;
		}

		lstrcpy(pszPath, m_pszRoot);
		if(m_pszRoot[dwLengthRoot - 1] != TEXT('\\') && m_pszRoot[dwLengthRoot - 1] != TEXT('/'))
		{
			pszPath[dwLengthRoot] = m_chDirSeparator;
			pszPath[dwLengthRoot + 1] = TEXT('\0');
		}

		lstrcat(pszPath, GetFileName());

		*pdwPath = dwLength;
		return TRUE;
	}

	virtual LPCTSTR GetRoot() const
	{
		ASSERT(m_hContext != NULL);

		return m_pszRoot;
	}

	virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp) const
	{
		ASSERT(m_hContext != NULL);
		ASSERT(pTimeStamp != NULL);

		if(m_pFoundInfo != NULL && pTimeStamp != NULL)
		{
			*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastWriteTime;
			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp) const
	{
		ASSERT(m_hContext != NULL);
		ASSERT(pTimeStamp != NULL);

		if(m_pFoundInfo != NULL && pTimeStamp != NULL)
		{
			*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastAccessTime;
			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL GetCreationTime(FILETIME* pTimeStamp) const
	{
		ASSERT(m_hContext != NULL);

		if(m_pFoundInfo != NULL && pTimeStamp != NULL)
		{
			*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftCreationTime;
			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL MatchesMask(DWORD dwMask) const
	{
		ASSERT(m_hContext != NULL);

		if(m_pFoundInfo != NULL)
			return (!!(((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes & dwMask));

		return FALSE;
	}

	virtual BOOL IsDots() const
	{
		ASSERT(m_hContext != NULL);

		// return TRUE if the file name is "." or ".." and
		// the file is a directory

		BOOL bResult = FALSE;
		if(m_pFoundInfo != NULL && IsDirectory())
		{
			LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA)m_pFoundInfo;
			if(pFindData->cFileName[0] == TEXT('.'))
			{
				if(pFindData->cFileName[1] == TEXT('\0') ||
					(pFindData->cFileName[1] == TEXT('.') &&
					 pFindData->cFileName[2] == TEXT('\0')))
				{
					bResult = TRUE;
				}
			}
		}

		return bResult;
	}

	// these aren't virtual because they all use MatchesMask(), which is
	BOOL IsReadOnly() const
		{ return MatchesMask(FILE_ATTRIBUTE_READONLY); }
	BOOL IsDirectory() const
		{ return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
	BOOL IsCompressed() const
		{ return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
	BOOL IsSystem() const
		{ return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
	BOOL IsHidden() const
		{ return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
	BOOL IsTemporary() const
		{ return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
	BOOL IsNormal() const
		{ return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
	BOOL IsArchived() const
		{ return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }


// Operations
	void Close()
	{
		if(m_pFoundInfo != NULL)
		{
			delete m_pFoundInfo;
			m_pFoundInfo = NULL;
		}

		if(m_pNextInfo != NULL)
		{
			delete m_pNextInfo;
			m_pNextInfo = NULL;
		}

		if(m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
		{
			CloseContext();
			m_hContext = NULL;
		}

		if(m_pszRoot != NULL)
		{
			delete[] m_pszRoot;
			m_pszRoot = NULL;
		}
	}

	virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0)
	{
		UNUSED_ALWAYS(dwUnused);
		Close();

		m_pNextInfo = new WIN32_FIND_DATA;
		m_bGotLast = FALSE;

		if(pstrName == NULL)
			pstrName = _T("*.*");

		lstrcpy(((LPWIN32_FIND_DATA)m_pNextInfo)->cFileName, pstrName);

		m_hContext = ::FindFirstFile(pstrName, (LPWIN32_FIND_DATA)m_pNextInfo);

		if(m_hContext == INVALID_HANDLE_VALUE)
		{
			DWORD dwTemp = ::GetLastError();
			Close();
			::SetLastError(dwTemp);

			return FALSE;
		}

		m_pszRoot = new TCHAR[MAX_PATH];
		if(m_pszRoot == NULL)
			return FALSE;

		LPTSTR pstrRoot = m_pszRoot;
#ifndef _WIN32_WCE
		LPCTSTR pstr = _tfullpath(pstrRoot, pstrName, MAX_PATH);
#else
		LPCTSTR pstr = lstrcpy(pstrRoot, pstrName);
#endif

		// passed name isn't a valid path but was found by the API
		ASSERT(pstr != NULL);
		if(pstr == NULL)
		{
			Close();
			::SetLastError(ERROR_INVALID_NAME);

			return FALSE;
		}

		// find the last forward or backward whack
		LPTSTR pstrBack = _tcsrchr(pstrRoot, _T('\\'));
		LPTSTR pstrFront = _tcsrchr(pstrRoot, _T('/'));

		if(pstrFront != NULL || pstrBack != NULL)
		{
			if(pstrFront == NULL)
				pstrFront = pstrRoot;

			if(pstrBack == NULL)
				pstrBack = pstrRoot;

			// from the start to the last whack is the root

			if(pstrFront >= pstrBack)
				*pstrFront = TEXT('\0');
			else
				*pstrBack = TEXT('\0');
		}

		return TRUE;
	}

	virtual BOOL FindNextFile()
	{
		ASSERT(m_hContext != NULL);

		if(m_hContext == NULL)
			return FALSE;

		if(m_pFoundInfo == NULL)
			m_pFoundInfo = new WIN32_FIND_DATA;

		void* pTemp = m_pFoundInfo;
		m_pFoundInfo = m_pNextInfo;
		m_pNextInfo = pTemp;

		return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA)m_pNextInfo);
	}

protected:
	virtual void CloseContext()
	{
		::FindClose(m_hContext);
	}

// Implementation
protected:
	void* m_pFoundInfo;
	void* m_pNextInfo;
	HANDLE m_hContext;
	BOOL m_bGotLast;
	LPTSTR m_pszRoot;
	TCHAR m_chDirSeparator;     // not '\\' for Internet classes
};

#endif // __FFUTIL_H__
