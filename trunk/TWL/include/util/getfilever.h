#ifndef __GETFILEVER_H__
#define __GETFILEVER_H__

#include <windows.h>

#ifndef _WIN32_WCE
#pragma comment(lib, "version.lib")
#endif

class CFileVersion
{
public:
	// 取文件版本信息
	static BOOL Get(LPCTSTR pszFile, PDWORD pdwVer)
	{
		DWORD dwZero = 0;
		DWORD dwSize = GetFileVersionInfoSize((LPTSTR)pszFile, &dwZero);
		LPVOID pData = HeapAlloc(GetProcessHeap(), 0, dwSize);

		if(!pData)
			return FALSE;

		if(GetFileVersionInfo((LPTSTR)pszFile, 0, dwSize, pData))
		{
			VS_FIXEDFILEINFO* pVer = NULL;
			UINT uVerLen = 0;
			VerQueryValue(pData, TEXT("\\"), (LPVOID *)&pVer, &uVerLen);
			*pdwVer = 0;
			*pdwVer += (HIWORD(pVer->dwFileVersionMS) << 8) | (LOWORD(pVer->dwFileVersionMS) & 0x00ff);
			*pdwVer <<= 16;
			*pdwVer += (HIWORD(pVer->dwFileVersionLS) << 8) | (LOWORD(pVer->dwFileVersionLS) & 0x00ff);

			HeapFree(GetProcessHeap(), 0, pData);
			return TRUE;
		}

		return FALSE;
	}
};

#define GetFileVersion	CFileVersion::Get

#endif // __GETFILEVER_H__
