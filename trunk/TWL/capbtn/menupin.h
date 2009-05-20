#ifndef __MENUPIN_H__
#define __MENUPIN_H__

#include "CaptionButton\drawutil.h"	// for DrawBitmap, ReplaceBitmapColor

#define SetWindowTopmost(hwnd, topmost)	SetWindowPos(hwnd, topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

#define SWAP_WORD_BYTES(A)	((A) = ((((A) & 0xFF00) >> 8) | (((A) & 0x00FF) << 8)))
#define SWAP_DWORD_BYTES(A) ((A) = ((((A) & 0xFF000000) >> 24) | (((A) & 0x00FF0000) >> 8) | (((A) & 0x0000FF00) << 8) | (((A) & 0x000000FF) << 24))

class CMenuPin
{
	HWND _hwnd;
	HMENU _hmnu;
	UINT _uCommand;
	HBITMAP _hbmp;

public:
	CMenuPin()
		: _hwnd(NULL)
		, _hmnu(NULL)
		, _uCommand(-1)
		, _hbmp(NULL)
	{
	}

	~CMenuPin()
	{
		Uninstall();
	}

	BOOL Install(HWND hwnd, HMENU hmnu, UINT uCommand)
	{
		Uninstall();

		BOOL bRet = InsertMenu(hmnu, -1, MF_BYPOSITION | MF_HELP | MF_OWNERDRAW, uCommand, (LPCTSTR)this);
		if(bRet)
		{
			_hmnu = hmnu;
			_uCommand = uCommand;
			_hwnd = hwnd;
		}

		if(_hwnd)
			DrawMenuBar(_hwnd);

		return bRet;
	}

	void Uninstall()
	{
		Cleanup();
	}

	// pszBitmap: a six-stated bitmap
	BOOL SetBitmap(HINSTANCE hInstance, LPCTSTR pszBitmap, BOOL bReplaceColor = TRUE)
	{
		if(_hbmp)
			DeleteObject(_hbmp);

		_hbmp = LoadBitmap(hInstance, pszBitmap);
		if(!_hbmp)
			return FALSE;

		if(bReplaceColor)
		{
			WORD wVer = LOWORD(GetVersion());
			SWAP_WORD_BYTES(wVer);

			COLORREF clrNew = GetSysColor(wVer >= 0x0501 ? 30 : COLOR_MENU); // 30 - COLOR_MENUBAR
			HBITMAP hbmp = ReplaceBitmapColor(_hbmp, RGB(255, 0, 255), clrNew);
			if(hbmp)
			{
				DeleteObject(_hbmp);
				_hbmp = hbmp;
			}
		}

		if(_hwnd)
			DrawMenuBar(_hwnd);

		return TRUE;
	}

	BOOL SetBitmap(HINSTANCE hInstance, UINT uBitmap, BOOL bReplaceColor = TRUE)
	{
		return SetBitmap(hInstance, MAKEINTRESOURCE(uBitmap), bReplaceColor);
	}

	bool OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;
		if(pmis->CtlType != ODT_MENU || pmis->itemID != _uCommand)
			return false;
		
		pmis->itemWidth = 12;
		pmis->itemHeight = GetSystemMetrics(SM_CYMENU);
		return true;
	}
	
	bool OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
		if(pdis->CtlType != ODT_MENU || pdis->itemID != _uCommand)
			return false;
		
		HDC hdc = pdis->hDC;
		RECT rc = pdis->rcItem;
		
		enum
		{
			BI_NORMAL = 0,
			BI_NORMAL_HOTLIGHT = 1,
			BI_NORMAL_SELECTED = 2,
			BI_CHECKED = 3,
			BI_CHECKED_HOTLIGHT = 4,
			BI_CHECKED_SELECTED = 5,
		};
		
		HBITMAP hbmpPin = _hbmp;
		if(!hbmpPin)
			return true;

		DWORD dwDrawFlag = DT_LEFT | DT_SINGLELINE;
		if(pdis->itemState & ODS_HOTLIGHT)
		{
			if(pdis->itemState & ODS_CHECKED)
			{
				// DrawText(hdc, TEXT("Checked Hotlight"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_CHECKED_HOTLIGHT, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
			else
			{
				// DrawText(hdc, TEXT("Hotlight"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_NORMAL_HOTLIGHT, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
		}
		else if(pdis->itemState & ODS_SELECTED)
		{
			if(pdis->itemState & ODS_CHECKED)
			{
				// DrawText(hdc, TEXT("Checked Selected"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_CHECKED_SELECTED, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
			else
			{
				// DrawText(hdc, TEXT("Selected"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_NORMAL_SELECTED, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
		}
		else
		{
			if(pdis->itemState & ODS_CHECKED)
			{
				// DrawText(hdc, TEXT("Checked Normal"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_CHECKED, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
			else
			{
				// DrawText(hdc, TEXT("Normal"), -1, &rc, dwDrawFlag);
				DrawBitmap((LPVOID)hbmpPin, (DWORD)MAKELONG(BI_NORMAL, 6),
					hdc, &rc, BMPTYPE_DDB, BMPDRAW_HSTRETCH_VSTRETCH);
			}
		}
		
		return true;
	}

	bool OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		if(LOWORD(wParam) != _uCommand)
			return false;

		BOOL bChecked = (GetMenuState(_hmnu, _uCommand, MF_BYCOMMAND) & MF_CHECKED);
		bChecked = !bChecked;
		CheckMenuItem(_hmnu, _uCommand, MF_BYCOMMAND | (bChecked ? MF_CHECKED : MF_UNCHECKED));

		SetWindowTopmost(_hwnd, bChecked);

		if(_hwnd)
			DrawMenuBar(_hwnd);

		return true;
	}

protected:
	void Cleanup()
	{
		if(_hmnu)
		{
			RemoveMenu(_hmnu, _uCommand, MF_BYCOMMAND);
			_hmnu = NULL;
			_uCommand = -1;
		}

		if(_hbmp)
		{
			DeleteObject(_hbmp);
			_hbmp = NULL;
		}
		
		if(_hwnd)
		{
			DrawMenuBar(_hwnd);
			_hwnd = NULL;
		}
	}
};

#endif // __MENUPIN_H__
