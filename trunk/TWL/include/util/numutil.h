#ifndef __NUMUTIL_H__
#define __NUMUTIL_H__

class NumUtil
{
public:
	static 
	LPCTSTR StringChar(TCHAR ch, LPCTSTR psz, INT nLength, BOOL bCaseSensitive)
	{
		if(!psz)
			return NULL;

		if(!bCaseSensitive)
			ch = (TCHAR)CharUpper((LPTSTR)ch);

		LPCTSTR p = psz;

		TCHAR ch2;
		while(*p)
		{
			ch2 = *p;

			if(!bCaseSensitive)
				ch2 = (TCHAR)CharUpper((LPTSTR)ch2);

			if(ch == ch2)
				return p;

			p++;

			if(p - psz > nLength)
				break;
		}

		return NULL;
	}

	static
	BOOL StringToDWORD(LPCTSTR pszIn, DWORD& rdwOut)
	{
		if(!pszIn || !*pszIn)
			return FALSE;

		LPCTSTR pszNumbers = _T("0123456789ABCDEF");

		UINT nBase = 10;

		if(*pszIn == _T('0'))
		{
			if((pszIn[1] == _T('x')) || (pszIn[1] == _T('X')))
			{
				nBase = 16;
				pszIn += 2;
			}
			else
			{
				nBase = 8;
				pszIn++;
			}
		}

		LPCTSTR pszDigit = pszIn;
		pszDigit += lstrlen(pszDigit);

		rdwOut = 0;

		LPCTSTR pszNumber;
		DWORD dwAdd;
		DWORD nMul = 1;
		while(pszDigit > pszIn)
		{
			pszDigit--;
			pszNumber = StringChar(*pszDigit, pszNumbers, nBase, FALSE);
			if(!pszNumber)
				return FALSE;		// wrong char in input string

			dwAdd = (pszNumber - pszNumbers) * nMul;

			if(rdwOut + dwAdd < rdwOut)
				return FALSE;		// overflow

			rdwOut += dwAdd;

			if(pszDigit > pszIn)
			{
				if(nMul * nBase < nMul)
					return FALSE;	// overflow

				nMul *= nBase;
			}
		}

		return TRUE;
	}
};

class CIntergerParser
{
public:
	static LONG StringToLong(LPCTSTR pszString)
	{
		// skip whitespace
		while(*pszString == TEXT(' ') || *pszString == TEXT('\t'))
			pszString++;

		TCHAR chChar = *pszString++;						// current char
		TCHAR chSign = chChar;								// save sign indication, if '-',
															// then negative, otherwise positive

		// skip sign
		if(chChar == TEXT('-') || chChar == TEXT('+'))
			chChar = *pszString++;

		LONG lTotal = 0;									// current total

		while(chChar >= TEXT('0') && chChar <= TEXT('9'))
		{
			lTotal = 10 * lTotal + (chChar - TEXT('0'));	// accumulate digit
			chChar = *pszString++;							// get next char
		}

		return (chSign == TEXT('-')) ? -lTotal : lTotal;	// return result, negated if necessary
	}

	static BOOL GetIntegerByIndex(LPCTSTR pszString, int iIndex, PDWORD pdwValue, LPCTSTR pszDelimiters = TEXT(",;"))
	{
		if(pszString == NULL || pdwValue == NULL)
			return FALSE;

		if(iIndex < 0)
			return FALSE;

		BOOL bRet = FALSE;

		LPCTSTR pBegin = pszString;
		LPCTSTR pTracker = pszString;
		LPCTSTR pEnd = NULL;

		int iDelimiter = 0;
		for(; ; pTracker++)
		{
			BOOL bIsDelimiter = FALSE;
			for(int i=0; i<lstrlen(pszDelimiters); i++)
			{
				if(*pTracker == pszDelimiters[i])
				{
					bIsDelimiter = TRUE;
					break;
				}
			}

			if(bIsDelimiter)
			{
				iDelimiter++;
				if(iDelimiter == iIndex)
					pBegin = pTracker + 1;
				else if(iDelimiter == (iIndex + 1))
					pEnd = pTracker;
			}
			else if(*pTracker == TEXT('\0'))
			{
				if(iDelimiter == iIndex)
					pEnd = pTracker;
				else
					break;
			}

			if(pEnd != NULL)
			{
				bRet = TRUE;
				break;
			}
		}

		if(bRet)
		{
			DWORD dwSize = (PBYTE)pEnd - (PBYTE)pBegin;
			if(dwSize == 0)
				bRet = FALSE;
			else
			{
				int iCount = dwSize / sizeof(TCHAR) + 1;
				LPTSTR pDigit = new TCHAR[iCount];
#ifdef WIN32
				lstrcpyn(pDigit, pBegin, iCount);
#else // WIN32
				_tcsncpy(pDigit, pBegin, iCount);
#endif // WIN32
				*pdwValue = StringToLong(pDigit);
				delete pDigit;
			}
		}

		return bRet;
	}

public:
	CIntergerParser(LPCTSTR pszSource)
		: _pszSource(NULL), _iIndex(0)
	{
		_pszSource = pszSource;
	}

	BOOL Get(PDWORD pdwValue)
	{
		return GetIntegerByIndex(_pszSource, _iIndex++, pdwValue);
	}

	void Reset()
	{
		_iIndex = 0;
	}

	CIntergerParser& operator= (LPCTSTR pszSource)
	{
		Reset();
		_pszSource = pszSource;

		return *this;
	}

protected:
	LPCTSTR _pszSource;
	int _iIndex;
};

#endif // __NUMUTIL_H__
