//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __OSINFO_H__
#define __OSINFO_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

/*
typedef struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
#ifdef UNICODE
typedef OSVERSIONINFOW OSVERSIONINFO;
typedef POSVERSIONINFOW POSVERSIONINFO;
typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
#else
typedef OSVERSIONINFOA OSVERSIONINFO;
typedef POSVERSIONINFOA POSVERSIONINFO;
typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
#endif // UNICODE

typedef struct _OSVERSIONINFOEXA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
    WORD   wServicePackMajor;
    WORD   wServicePackMinor;
    WORD   wSuiteMask;
    BYTE  wProductType;
    BYTE  wReserved;
} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;
typedef struct _OSVERSIONINFOEXW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
    WORD   wServicePackMajor;
    WORD   wServicePackMinor;
    WORD   wSuiteMask;
    BYTE  wProductType;
    BYTE  wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW, RTL_OSVERSIONINFOEXW, *PRTL_OSVERSIONINFOEXW;
#ifdef UNICODE
typedef OSVERSIONINFOEXW OSVERSIONINFOEX;
typedef POSVERSIONINFOEXW POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXW LPOSVERSIONINFOEX;
#else
typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
#endif // UNICODE

// */

namespace TWL
{

#define BUFSIZE 80

class COSVer
{
	BOOL m_bIsWindows9xSeries			:1;
	BOOL m_bIsWindowsNtSeries			:1;

	BOOL m_bIsServer					:1;
		BOOL m_bIsDataCenterServer		:1;	// 2003, 2000
		BOOL m_bIsEnterpriseEdition		:1;	// 2003, NT4
		BOOL m_bIsWebEdition			:1;	// 2003
		BOOL m_bIsStandardEdition		:1;	// 2003, 2000 Server, NT4 Server
		BOOL m_bIsAdvancedServer		:1;	// 2000

	BOOL m_bIsWorkstation				:1;
		BOOL m_bIsHomeEdition			:1;
		BOOL m_bIsProfessionalEdition	:1;

	BOOL m_bIsBeforeWindows95			:1;

	UINT m_uMajor;
	UINT m_uMinor;
	UINT m_uSPOrOSROrEdition;
	UINT m_uBuild;

public:
	COSVer()
	{
		OSVERSIONINFOEX osvi;
		BOOL bOsVersionInfoEx;

		// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
		// If that fails, try using the OSVERSIONINFO structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi)))
		{
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx((OSVERSIONINFO *)&osvi)) 
		}

		m_uMajor = osvi.dwMajorVersion;
		m_uMinor = osvi.dwMinorVersion;

		switch(osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			m_bIsWindowsNtSeries = TRUE;

			// Test for specific product on Windows NT 4.0 SP6 and later.
			if(bOsVersionInfoEx)
			{
				if(osvi.wProductType == VER_NT_WORKSTATION)
				{
					m_bIsWorkstation = TRUE;
					if(osvi.dwMajorVersion != 4)
					{
						if(osvi.wSuiteMask & VER_SUITE_PERSONAL)
							m_bIsHomeEdition = TRUE;
						else
							m_bIsProfessionalEdition = TRUE;
					}
				}
				else if(osvi.wProductType == VER_NT_SERVER)
				{
					m_bIsServer = TRUE;
					if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
					{
						if(osvi.wSuiteMask & VER_SUITE_DATACENTER)
							m_bIsDataCenterServer = TRUE;
						else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							m_bIsEnterpriseEdition = TRUE;
						else if(osvi.wSuiteMask == VER_SUITE_BLADE)
							m_bIsWebEdition = TRUE;
						else
							m_bIsStandardEdition = TRUE;
					}
					else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
					{
						if(osvi.wSuiteMask & VER_SUITE_DATACENTER)
							m_bIsDataCenterServer = TRUE;
						else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							m_bIsEnterpriseEdition = TRUE;
						else
							m_bIsStandardEdition = TRUE;
					}
					else  // Windows NT 4.0
					{
						if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							m_bIsEnterpriseEdition = TRUE;
						else
							m_bIsStandardEdition = TRUE;
					}
				}
			}
			else  // Test for specific product on Windows NT 4.0 SP5 and earlier
			{
				HKEY hKey = NULL;
				if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
					0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
				{
					TCHAR szProductType[BUFSIZE];
					DWORD dwBufLen = BUFSIZE;
					
					if((RegQueryValueEx(hKey, TEXT("ProductType"), NULL, NULL,
						(LPBYTE)szProductType, &dwBufLen) == ERROR_SUCCESS)
						&& (dwBufLen <= BUFSIZE))
					{
						RegCloseKey( hKey );

						if(lstrcmpi(TEXT("WINNT"), szProductType) == 0)
							m_bIsWorkstation = TRUE;
						if(lstrcmpi(TEXT("LANMANNT"), szProductType) == 0)
							m_bIsServer = TRUE;
						if(lstrcmpi(TEXT("SERVERNT"), szProductType) == 0)
						{
							m_bIsServer = TRUE;
							m_bIsAdvancedServer = TRUE;
						}
					}
				}
			}

			// We treat the Windows NT 4 SP6a as same as SP6
			m_uSPOrOSROrEdition = osvi.szCSDVersion[lstrlen(osvi.szCSDVersion)-1] - TEXT('0');
			m_uBuild = osvi.dwBuildNumber & 0xFFFF;

			/* // Test for SP6 versus SP6a.
			if(osvi.dwMajorVersion == 4 && lstrcmpi(osvi.szCSDVersion, "Service Pack 6") == 0)
			{
				HKEY hKey = NULL;
				BOOL bSP6a = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
					TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
					0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS;
				RegCloseKey( hKey );
			}
			// */
			break;
			
		case VER_PLATFORM_WIN32_WINDOWS:
			m_bIsWindows9xSeries = TRUE;
			if(m_uMajor == 4 && m_uMinor == 0)			// Windows 95
			{
				if(osvi.szCSDVersion[1] == TEXT('C') || osvi.szCSDVersion[1] == TEXT('B'))
					m_uSPOrOSROrEdition = 2;
			} 
			else if(m_uMajor == 4 && m_uMinor == 10)	// Windows 98
			{
				if(osvi.szCSDVersion[1] == TEXT('A'))
					m_uSPOrOSROrEdition = 2;
			} 
			break;

		case VER_PLATFORM_WIN32s:
			m_bIsBeforeWindows95 = TRUE;
			break;
		}
	}

	~COSVer()
	{
	}

	BOOL IsWindows9xSeries()					{ return m_bIsWindows9xSeries; }
	BOOL IsWindowsNtSeries()					{ return m_bIsWindowsNtSeries; }

	BOOL IsBeforeWindows95()					{ return m_bIsBeforeWindows95; }
	BOOL IsBeforeWindowsNt4()					{ return (IsWindowsNtSeries() && (m_uMajor < 4)); }

	BOOL IsWindows95(PUINT puOSR = NULL)		{ return IsWindows9x(0, puEdition); }
	BOOL IsWindows98(PUINT puEdition = NULL)	{ return IsWindows9x(10, puEdition); }
	BOOL IsWindowsMe()							{ return IsWindows9x(90); }

	BOOL IsWindowsNt4(PUINT puServicePack = NULL, PUINT puBuild = NULL)
	{
		if(m_bIsWindowsNtSeries && m_uMajor == 4)
		{
			if(puServicePack)
				*puServicePack = m_uSPOrOSROrEdition;

			if(puBuild)
				*puBuild = m_uBuild;

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsWindows2000(PUINT puServicePack = NULL, PUINT puBuild = NULL)
	{
		if(m_bIsWindowsNtSeries && m_uMajor == 5 && m_uMinor == 0)
		{
			if(puServicePack)
				*puServicePack = m_uSPOrOSROrEdition;

			if(puBuild)
				*puBuild = m_uBuild;

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsWindowsXP(PUINT puServicePack = NULL, PUINT puBuild = NULL)
	{
		if(m_bIsWindowsNtSeries && m_uMajor == 5 && m_uMinor == 1)
		{
			if(puServicePack)
				*puServicePack = m_uSPOrOSROrEdition;

			if(puBuild)
				*puBuild = m_uBuild;

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsWindows2003(PUINT puServicePack = NULL, PUINT puBuild = NULL)
	{
		if(m_bIsWindowsNtSeries && m_uMajor == 5 && m_uMinor == 2)
		{
			if(puServicePack)
				*puServicePack = m_uSPOrOSROrEdition;

			if(puBuild)
				*puBuild = m_uBuild;

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsServer()
	{
		return m_bIsWindowsNtSeries && m_bIsServer;
	}

	BOOL IsWorkstation(BOOL b9xIncluded = FALSE)
	{
		BOOL bRet = FALSE;
		if(m_bIsWindowsNtSeries)
			bRet = m_bIsWorkstation;
		else
		{
			if(b9xIncluded)
				bRet = TRUE;
		}

		return bRet;
	}

	// helper functions
	BOOL IsWindowsOSR2()
	{
		UINT uOSR;
		return (IsWindows95(&uOSR) && (uOSR == 2));
	}
	BOOL IsWindows97()								{ return IsWindowsOSR2(); }
	BOOL IsWindows98SE()
	{
		UINT uEdition;
		return (IsWindows98(&uEdition) && (uEdition == 2));
	}

	BOOL IsWindows95OrLater()
	{
		return (IsWindows9xSeries() && (m_uMajor >= 4));
	}
	BOOL IsWindows98OrLater()
	{
		return (IsWindows9xSeries() && (m_uMajor > 4 || (m_uMajor == 4 && m_uMinor >= 10)));
	}
	BOOL IsWindowsNt4SP3()							{ return IsWindowsNt4SPx(3); }
	BOOL IsWindowsNt4SP4()							{ return IsWindowsNt4SPx(4); }
	BOOL IsWindowsNt4SP5()							{ return IsWindowsNt4SPx(5); }
	BOOL IsWindowsNt4SP6()							{ return IsWindowsNt4SPx(6); }

	BOOL IsWindowsNt4SP3OrLater()					{ return IsWindowsNt4SPx(3, TRUE); }
	BOOL IsWindowsNt4SP4OrLater()					{ return IsWindowsNt4SPx(4, TRUE); }
	BOOL IsWindowsNt4SP5OrLater()					{ return IsWindowsNt4SPx(5, TRUE); }
	BOOL IsWindowsNt4SP6OrLater()					{ return IsWindowsNt4SPx(6, TRUE); }

	BOOL IsWindows2000OrLater()
	{
		return (IsWindowsNtSeries() && (m_uMajor >= 5));
	}
	BOOL IsWindowsXPOrLater()
	{
		return (IsWindowsNtSeries() && (m_uMajor > 5 || (m_uMajor == 5 && m_uMinor >= 1)));
	}
	BOOL IsWindows2003OrLater()
	{
		return (IsWindowsNtSeries() && (m_uMajor > 5 || (m_uMajor == 5 && m_uMinor >= 2)));
	}
	BOOL IsNewOS()
	{
		return (IsWindows98OrLater() || IsWindows2000OrLater());
	}

private:
	BOOL IsWindows9x(UINT uMinor, PUINT puEdition = NULL)
	{
		BOOL bRet = FALSE;
		if(m_bIsWindows9xSeries && m_uMajor == 4 && m_uMinor == uMinor)
		{
			bRet = TRUE;
			if(puOSR)
				*puOSR = m_uSPOrOSROrEdition;
		}

		return bRet;
	}

	BOOL IsWindowsNt4SPx(UINT uSPx, BOOL bLaterIncluded = FALSE)
	{
		BOOL bRet = FALSE;
		UINT uSP;
		if(IsWindowsNt4(&uSP))
		{
			bRet = bLaterIncluded ? (uSP >= uSPx) : (uSP == uSPx);
		}
		else if(bLaterIncluded)
		{
			if(!IsBeforeWindowsNt4())
				bRet = TRUE;
		}

		return bRet;
	}
};

class CShellVer
{
};

#undef BUFSIZE

}; // namespace TWL

#endif // __OSINFO_H__
