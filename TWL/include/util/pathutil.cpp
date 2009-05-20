#include <windows.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "numutil.h"

#define IsDigit(c) ((c) >= TEXT('0') && c <= TEXT('9'))

//
// Takes a location string ("shell32.dll,3") and parses
// it into a file-component and an icon index.
// Returns the icon index
//
int WINAPI PathParseIconLocation(IN OUT LPTSTR pszIconFile)
{
    int iIndex = 0;

    if(pszIconFile)
    {
        LPTSTR pszComma, pszEnd;

        // look for the last comma in the string
        pszEnd = pszIconFile + lstrlen(pszIconFile);
        pszComma = StrRChr(pszIconFile, pszEnd, TEXT(','));

        if(pszComma && *pszComma)
        {
            LPTSTR pszComma2 = pszComma + 1;
            BOOL fIsDigit = FALSE;

            // Sometimes we get something like: "C:\path, comma\path\file.ico"
            // where the ',' is in the path and does not indicates that an icon index follows
            while(*pszComma2)
            {
                if((TEXT(' ') == *pszComma2) || (TEXT('-') == *pszComma2))
                    ++pszComma2;
                else
                {
                    if(IsDigit(*pszComma2))
                        fIsDigit = TRUE;

                    break;
                }
            }

            if(fIsDigit)
            {
                *pszComma++ = 0;            // terminate the icon file name.
                iIndex = (int)CIntergerParser::StringToLong(pszComma);
            }
        }

        PathUnquoteSpaces(pszIconFile);
        PathRemoveBlanks(pszIconFile);
    }

    return iIndex;
}

//
// If a path is contained in quotes then remove them.
//
void WINAPI PathUnquoteSpaces(LPTSTR lpsz)
{
    if(lpsz)
    {
        int cch = lstrlen(lpsz);

        // Are the first and last chars quotes?
        if(lpsz[0] == TEXT('"') && lpsz[cch-1] == TEXT('"'))
        {
            // Yep, remove them.
            lpsz[cch - 1] = TEXT('\0');
            MoveMemory(lpsz, lpsz + 1, sizeof(TCHAR) * (cch - 1));
        }
    }
}

//
// Strips leading and trailing blanks from a string.
// Alters the memory where the string sits.
//
// in:
//  lpszString  string to strip
//
// out:
//  lpszString  string sans leading/trailing blanks
//
//
void WINAPI PathRemoveBlanks(LPTSTR lpszString)
{
    if(lpszString)
    {
        LPTSTR lpszPosn = lpszString;
		
        /* strip leading blanks */
        while(*lpszPosn == TEXT(' '))
            lpszPosn++;

        if(lpszPosn != lpszString)
            lstrcpy(lpszString, lpszPosn);

        /* strip trailing blanks */

        // Find the last non-space
        // Note that AnsiPrev is cheap is non-DBCS, but very expensive otherwise
        for(lpszPosn=lpszString; *lpszString; lpszString=CharNext(lpszString))
        {
            if(*lpszString != TEXT(' '))
                lpszPosn = lpszString;
        }

        // Note AnsiNext is a macro for non-DBCS, so it will not stop at NULL
        if(*lpszPosn)
            *CharNext(lpszPosn) = TEXT('\0');
    }
}

#ifndef _INC_SHLWAPI

////////////////////////////////////////////////////////////////////////////////
// for StrRChr()

//
// ChrCmp -  Case sensitive character comparison for DBCS
// Assumes   w1, wMatch are characters to be compared
// Return    FALSE if they match, TRUE if no match
//
__inline
BOOL ChrCmpA_inline(WORD w1, WORD wMatch)
{
    //
	// Most of the time this won't match, so test it first for speed.
    //
    if(LOBYTE(w1) == LOBYTE(wMatch))
    {
        if(IsDBCSLeadByte(LOBYTE(w1)))
            return w1 != wMatch;

        return FALSE;
    }

    return TRUE;
}

__inline
BOOL ChrCmpW_inline(WCHAR w1, WCHAR wMatch)
{
    return w1 != wMatch;
}

//
// StrRChr - Find last occurrence of character in string
// Assumes   lpStart points to start of string
//           lpEnd   points to end of string (NOT included in search)
//           wMatch  is the character to match
// returns ptr to the last occurrence of ch in str, NULL if not found.
//

#define READNATIVEWORD(x) (*(UNALIGNED WORD *)x)

LPSTR StrRChrA(LPCSTR lpStart, LPCSTR lpEnd, WORD wMatch)
{
    LPCSTR lpFound = NULL;

    ASSERT(lpStart);
    ASSERT(!lpEnd || lpEnd <= lpStart + lstrlenA(lpStart));

    if(!lpEnd)
        lpEnd = lpStart + lstrlenA(lpStart);

    for( ; lpStart < lpEnd; lpStart = CharNextA(lpStart))
    {
        // ChrCmp returns FALSE when characters match
        if(!ChrCmpA_inline(READNATIVEWORD(lpStart), wMatch))
            lpFound = lpStart;
    }

    return (LPSTR)lpFound;
}

LPWSTR StrRChrW(LPCWSTR lpStart, LPCWSTR lpEnd, WCHAR wMatch)
{
    LPCWSTR lpFound = NULL;

    if(!lpEnd)
        lpEnd = lpStart + lstrlenW(lpStart);

    for( ; lpStart < lpEnd; lpStart++)
    {
        if(!ChrCmpW_inline(*lpStart, wMatch))
            lpFound = lpStart;
    }

    return (LPWSTR)lpFound;
}

#endif // !_INC_SHLWAPI
