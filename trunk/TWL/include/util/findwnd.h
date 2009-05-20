#ifndef __FINDWINDOW_H__
#define __FINDWINDOW_H__

#define WFF_CLASSNAME			0x00000001
#define WFF_WINDOWNAME			0x00000002
#define WFF_MATCHMASK			(WFF_CLASSNAME | WFF_WINDOWNAME)
#define WFF_SKIPCHILDREN		0x00000004
#define WFF_SKIPHIDDEN			0x00000008
#define WFF_SKIPDISABLED		0x00000010
#define WFF_FIRSTONLY			0x00000020

class CWindowFind
{
	typedef CWindowFind thisClass;

	LPCTSTR _pszClass;
	LPCTSTR _pszName;
	UINT _uFlags;

	WNDENUMPROC _wep;
	LPARAM _lParam;

	HWND _hwndFind;

public:
	CWindowFind()
	{
		_pszClass = NULL;
		_pszName = NULL;
		_uFlags = 0;

		_wep = NULL;
		_lParam = 0;

		_hwndFind = NULL;
	}

	BOOL Find(LPCTSTR pszClass, LPCTSTR pszName, UINT uFlags)
	{
		_pszClass = pszClass;
		_pszName = pszName;
		_uFlags = uFlags;

		_hwndFind = NULL;

		BOOL bRet = EnumWindows(EnumWindowsProc, (LPARAM)this);
		return bRet || (GetLastError() == ERROR_SUCCESS);
	}

	void SetWindowFindCallback(WNDENUMPROC wep, LPARAM lParam)
	{
		_wep = wep;
		_lParam = lParam;
	}

	HWND GetFirstWindow() const
	{
		return _hwndFind;
	}

protected:
#ifdef _WIN32_WCE
	// Enumerate all children, this can be done via calling EnumChildWindows() on PC,
	// but it's absent on Windows CE...
	static
		BOOL WINAPI EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam)
	{
		if(!lpEnumFunc)
			return TRUE;

		BOOL bContinue = TRUE;
		BOOL bRet;

		HWND hwndChild = GetWindow(hWndParent, GW_CHILD);
		while(hwndChild && bContinue)
		{
			bRet = lpEnumFunc(hwndChild, lParam);
			bContinue = bRet || (GetLastError() == ERROR_SUCCESS);
			if(!bContinue)
				break;

			if(GetWindow(hwndChild, GW_CHILD))
			{
				bRet = EnumChildWindows(hwndChild, lpEnumFunc, lParam);
				bContinue = bRet || (GetLastError() == ERROR_SUCCESS);
				if(!bContinue)
					break;
			}

			hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);
		}

		return bContinue;
	}
#endif // _WIN32_WCE

	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		thisClass* pThis = (thisClass*)lParam;
		if(!pThis)
		{
			SetLastError(ERROR_SUCCESS);		// ERROR_INVALID_PARAMETER
			return FALSE;
		}

		// we allow the callback is NULL if only the first matched window
		// was requested. But, if you request all, and supply a NULL callback,
		// we think you are wasting time...
		if(!pThis->_wep && !(pThis->_uFlags & WFF_FIRSTONLY))
		{
			SetLastError(ERROR_SUCCESS);		// ERROR_PROC_NOT_FOUND
			return FALSE;	// no callback, bail out
		}

		DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
		if(!(dwStyle & WS_VISIBLE) && (pThis->_uFlags & WFF_SKIPHIDDEN))
			return TRUE;

		if(!IsWindowEnabled(hwnd) && (pThis->_uFlags & WFF_SKIPDISABLED))
			return TRUE;

		BOOL bClassMatched = TRUE;
		BOOL bNameMatched = TRUE;

		TCHAR szText[MAX_PATH];
		if(pThis->_uFlags & WFF_CLASSNAME)
		{
			GetClassName(hwnd, szText, MAX_PATH);
			bClassMatched = (lstrcmpi(szText, pThis->_pszClass) == 0);
		}

		if(pThis->_uFlags & WFF_WINDOWNAME)
		{
			GetWindowText(hwnd, szText, MAX_PATH);
			bNameMatched = (lstrcmpi(szText, pThis->_pszClass) == 0);
		}

		BOOL bContinue = TRUE;
		if(bClassMatched && bNameMatched)
		{
			if(pThis->_wep)
				bContinue = (pThis->_wep)(hwnd, pThis->_lParam);

			if(pThis->_uFlags & WFF_FIRSTONLY)
			{
				pThis->_hwndFind = hwnd;
				SetLastError(ERROR_SUCCESS);	// ERROR_CANCELLED
				return FALSE;
			}
		}

		if(!bContinue)
		{
			SetLastError(ERROR_SUCCESS);		// ERROR_CANCELLED
			return FALSE;
		}

		if(!(pThis->_uFlags & WFF_SKIPCHILDREN) && GetWindow(hwnd, GW_CHILD))
		{
			BOOL bRet = EnumChildWindows(hwnd, EnumWindowsProc, lParam);
			bContinue = bRet || (GetLastError() == ERROR_SUCCESS);
		}

		return bContinue;
	}
};

class CChildWindowFind : public CWindowFind
{
public:
	BOOL Find(HWND hwndParent, LPCTSTR pszClass, LPCTSTR pszName, UINT uFlags)
	{
		if(!GetWindow(hwndParent, GW_CHILD))
			return TRUE;

		BOOL bRet = EnumChildWindows(hwndParent, EnumWindowsProc, (LPARAM)this);
		return bRet || (GetLastError() == ERROR_SUCCESS);
	}
};

#endif // __FINDWINDOW_H__
