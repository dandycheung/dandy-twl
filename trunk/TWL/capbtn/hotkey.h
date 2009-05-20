#ifndef __HOTKEY_H__
#define __HOTKEY_H__

#define WC_DANDYHOTKEY	TEXT("DandyHotKey")

#ifdef _cplusplus
extern "C" {
#endif

BOOL InitHotKeyClass(HINSTANCE hInstance);

#ifdef _cplusplus
}
#endif

#define CreateDandyHotKey(style, x, y, width, height, parent, id, instance) \
	CreateWindow(TEXT("DandyHotKey"), NULL, style, \
		x, y, width, height, parent, (HMENU)id, instance, NULL)

#endif // __HOTKEY_H__
