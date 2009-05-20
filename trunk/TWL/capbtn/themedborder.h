#ifndef __THEMEDBORDER_H__
#define __THEMEDBORDER_H__

#include "CaptionButton/ThemeUtil.h"

class CThemedBorder
{
	CThemeUtil m_theme;
	LPCWSTR m_pszThemeClass;

public:
	CThemedBorder(LPCWSTR pszThemeClass)
		: m_pszThemeClass(pszThemeClass)
	{
	}

	bool OnNcCalcSize(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		RECT rcWindow = *(PRECT)lParam;
		RECT rcClient;

		int iPartId, iStateId;
		GetIds(hwnd, uMsg, iPartId, iStateId);

		m_theme.OpenThemeData(hwnd, m_pszThemeClass);
		m_theme.GetThemeBackgroundContentRect(NULL, iPartId, iStateId, &rcWindow, &rcClient);
		InflateRect(&rcClient, -1, -1);

		*(PRECT)lParam = rcClient;

		lResult = WVR_REDRAW;
		m_theme.CloseThemeData();

		return true;
	}

	bool OnNcPaint(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);

		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		MapWindowPoints(hwnd, NULL, (LPPOINT)&rcClient, 2);

		OffsetRect(&rcClient, -rcWindow.left, -rcWindow.top);
		OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);

		HDC hdc = GetWindowDC(hwnd);
		ExcludeClipRect(hdc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		
		int iPartId, iStateId;
		GetIds(hwnd, uMsg, iPartId, iStateId);

		m_theme.OpenThemeData(hwnd, m_pszThemeClass);

		// make sure the background is updated when transparent background is used
		if(m_theme.IsThemeBackgroundPartiallyTransparent(EP_EDITTEXT, ETS_NORMAL))
			m_theme.DrawThemeParentBackground(hwnd, hdc, &rcWindow);

		// draw background
		m_theme.DrawThemeBackground(hdc, iPartId, iStateId, &rcWindow, NULL);

		m_theme.CloseThemeData();

		lResult = 0;
		return true;
	}

	virtual void GetIds(HWND hwnd, UINT uMsg, int& iPartId, int& iStateId) = 0;
};

class CEditThemedBorder : public CThemedBorder
{
public:
	CEditThemedBorder()
		: CThemedBorder(L"Edit")
	{
	}

	void GetIds(HWND hwnd, UINT uMsg, int& iPartId, int& iStateId)
	{
		if(uMsg == WM_NCCALCSIZE)
		{
			iPartId = EP_EDITTEXT;
			iStateId = ETS_NORMAL;
		}
		else if(uMsg == WM_NCPAINT)
		{
			iPartId = EP_EDITTEXT;
			if(IsWindowEnabled(hwnd))
			{
				// if(GetWindowLong(hwnd, GWL_STYLE) & ES_READONLY)
				// 	iStateId = ETS_READONLY;
				// else
				iStateId = ETS_NORMAL;
			}
			else
				iStateId = ETS_DISABLED;
		}
	}
};

#endif // __THEMEDBORDER_H__
