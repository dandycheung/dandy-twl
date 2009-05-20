#ifndef __PATHUTIL_H__
#define __PATHUTIL_H__

void WINAPI PathUnquoteSpaces(LPTSTR lpsz);
void WINAPI PathRemoveBlanks(LPTSTR lpszString);

int WINAPI PathParseIconLocation(IN OUT LPTSTR pszIconFile);

#endif // __PATHUTIL_H__
