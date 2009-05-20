#ifndef __REGUTIL_H__
#define __REGUTIL_H__

#include <windows.h>

#ifndef RegDeleteTree	// This is a system API on Windows Vista and later.

class CRegUtil
{
public:
	static
	LONG WINAPI RegDeleteTreeHelper(HKEY hParentKey, LPCTSTR pszKeyName)
	{
		TCHAR szSubKeyName[MAX_PATH + 1];
		HKEY hCurrentKey;
		DWORD dwResult;

		if((dwResult = RegOpenKey(hParentKey, pszKeyName, &hCurrentKey)) == ERROR_SUCCESS)
		{
			// Remove all subkeys of the key to delete
			while((dwResult = RegEnumKey(hCurrentKey, 0, szSubKeyName, MAX_PATH)) == ERROR_SUCCESS)
			{
				if((dwResult = RegDeleteTreeHelper(hCurrentKey, szSubKeyName)) != ERROR_SUCCESS)
					break;
			}

			// If all went well, we should now be able to delete the requested key
			if((dwResult == ERROR_NO_MORE_ITEMS) || (dwResult == ERROR_BADKEY))
				dwResult = RegDeleteKey(hParentKey, pszKeyName);

			RegCloseKey(hCurrentKey);
		}

		return dwResult;
	}
};

#define RegDeleteTree CRegUtil::RegDeleteTreeHelper

#endif // RegDeleteTree

#endif // __REGUTIL_H__
