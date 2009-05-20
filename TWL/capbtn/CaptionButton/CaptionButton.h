////////////////////////////////////////////////////////////////////////////////
//  Original Author: Andrzej Markowski
//

#ifndef __CAPTIONBUTTON_H__
#define __CAPTIONBUTTON_H__

#pragma once

#include "themeutil.h"

#ifdef _AFX
#include <afxtempl.h>
#include <afxmt.h>
#else
#include "ext\afxtempl.h"
class CCriticalSection : public CRITICAL_SECTION
{
public:
	CCriticalSection() { ::InitializeCriticalSection(this); }
	virtual ~CCriticalSection() { ::DeleteCriticalSection(this); }
	
public:
	operator CRITICAL_SECTION*() { return this; }
	
	void Lock() { ::EnterCriticalSection(this); }
	void Unlock() { ::LeaveCriticalSection(this); }
};
#endif

// our notify message is WM_SYSCOMMAND.
// wParam: SC_NOTIFY
// lParam: HIWORD -> NM_CLICK or NM_DBLCLK or NM_RCLICK or NM_RDBLCLK
//         LOWORD -> control id
#define SC_NOTIFY						0xFF00
#define IsLeftClick(lParam)				(((int)(short)HIWORD(lParam)) == NM_CLICK)
#define IsLeftDoubleClick(lParam)		(((int)(short)HIWORD(lParam)) == NM_DBLCLK)
#define IsRightClick(lParam)			(((int)(short)HIWORD(lParam)) == NM_RCLICK)
#define IsRightDoubleClick(lParam)		(((int)(short)HIWORD(lParam)) == NM_RDBLCLK)
#define GetSysCommandNotifyId(lParam)	((int)(short)LOWORD(lParam))

#define CBNST_NORMAL					DNHZS_NORMAL
#define CBNST_HOT						DNHZS_HOT
#define CBNST_PRESSED					DNHZS_PRESSED
#define CBNST_DISABLED					DNHZS_DISABLED

// button styles
#define CBNSTYLE_CAPTION				1	// caption button style
#define CBNSTYLE_USERDEFWIDTH			2	// user defined button width, used with CBNSTYLE_CAPTION style

// background styles
#define CBNBKGNDSTYLE_CAPTION			0
#define CBNBKGNDSTYLE_CLOSE				1
#define CBNBKGNDSTYLE_MDI				2
#define CBNBKGNDSTYLE_COMBO				3
#define CBNBKGNDSTYLE_SPINUP			4
#define CBNBKGNDSTYLE_SPINDN			5
#define CBNBKGNDSTYLE_SPINUPHOR			6
#define CBNBKGNDSTYLE_SPINDNHOR			7
#define CBNBKGNDSTYLE_SCROLLUP			8
#define CBNBKGNDSTYLE_SCROLLDOWN		9
#define CBNBKGNDSTYLE_SCROLLLEFT		10
#define CBNBKGNDSTYLE_SCROLLRIGHT		11

// error codes
#define CBNERR_NOERROR					0
#define CBNERR_OUTOFMEMORY				-1
#define CBNERR_INCORRECTPARAMETER		-2
#define CBNERR_INCORRECTBUTTONSTYLE		-3
#define CBNERR_INCORRECTFRAMESTYLE		-4
#define CBNERR_INCORRECTFUNCCALL		-5
#define CBNERR_CREATEBUTTONFAILED		-6
#define CBNERR_CREATETOOLTIPCTRLFAILED	-7
#define CBNERR_COPYIMAGEFAILED			-8
#define CBNERR_SETWINDOWLONGFAILED		-9
#define CBNERR_FRAMEALREADYCREATED		-10
#define CBNERR_FRAMENOTCREATED			-11

#define HELDNONE						0
#define HELDLCLK						1
#define HELDLDBLCLK						2
#define HELDRCLK						3
#define HELDRDBLCLK						4

#ifndef WM_NCMOUSEHOVER
#define WM_NCMOUSEHOVER					0x02A0
#endif

#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE					0x02A2
#endif

#ifndef TME_NONCLIENT
#define TME_NONCLIENT					0x00000010
#endif

#define USE_TOOLTIP_SUPPORT

#ifdef USE_TOOLTIP_SUPPORT
class CTooltipWrapper
{
public:
	HWND m_hWnd;

	CTooltipWrapper()
		: m_hWnd(NULL)
	{
	}

	~CTooltipWrapper()
	{
		if(m_hWnd)
			DestroyWindow(m_hWnd);
	}

	BOOL Create(HWND hwndOwner, DWORD dwStyle = 0)
	{
		m_hWnd = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
			WS_POPUP | dwStyle, // force WS_POPUP
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hwndOwner, NULL, NULL, NULL);

		return m_hWnd != NULL;
	}

	void Activate(BOOL bActivate)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_ACTIVATE, bActivate, 0L);
	}

	BOOL AddTool(HWND hwnd, LPCTSTR pszText = LPSTR_TEXTCALLBACK, PRECT prc = NULL, UINT uToolId = 0)
	{
		ASSERT(hwnd != NULL);
		ASSERT(pszText != NULL);

		// the toolrect and toolid must both be zero or both valid
		ASSERT((prc != NULL && uToolId != 0) || (prc == NULL) && (uToolId == 0));

		TOOLINFO ti;
		FillInToolInfo(ti, hwnd, uToolId);

		if(prc != NULL)
			ti.rect = *prc;

		ti.lpszText = (LPTSTR)pszText;

		return (BOOL)::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
	}

	void DelTool(HWND hwnd, UINT uToolId = 0)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hwnd != NULL);

		TOOLINFO ti;
		FillInToolInfo(ti, hwnd, uToolId);
		::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
	}
	
	void SetToolRect(HWND hwnd, UINT uToolId, LPCRECT prc)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(hwnd != NULL);
		ASSERT(uToolId != 0);
		ASSERT(prc != NULL);

		TOOLINFO ti;
		FillInToolInfo(ti, hwnd, uToolId);
		ti.rect = *prc;
		::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
	}
	
	void RelayEvent(PMSG pMsg)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)pMsg);
	}

	void FillInToolInfo(TOOLINFO& ti, HWND hwnd, UINT uToolId) const
	{
		ZeroMemory(&ti, sizeof(TOOLINFO));
		ti.cbSize = sizeof(TOOLINFO);

		if(uToolId == 0)
		{
			ti.hwnd = ::GetParent(hwnd);
			ti.uFlags = TTF_IDISHWND;
			ti.uId = (UINT)hwnd;
		}
		else
		{
			ti.hwnd = hwnd;
			ti.uFlags = 0;
			ti.uId = uToolId;
		}
	}
};
#endif // USE_TOOLTIP_SUPPORT

class CCaptionButton
{
public:
	CCaptionButton();
	virtual ~CCaptionButton();

	int CreateCaptionFrame(HWND hwndCaption);
	int DestroyCaptionFrame();

	int AddButton(LPCRECT prc, UINT nId, int nBkStyle, BOOL fUserDefWidth);
	CCaptionButton* GetButton(int nButtonID);

	int SetBkStyle(int nBkStyle);
	int GetBkStyle() { return m_nBkStyle; }

	int SetGlyphBitmap(HBITMAP hBmpGlyph);
	HBITMAP GetGlyphBitmap() { return m_hbmpGlyph; }

#ifdef USE_TOOLTIP_SUPPORT
	int SetTooltipText(LPCTSTR pszText);
#endif // USE_TOOLTIP_SUPPORT

	int Enable(BOOL bEnable = TRUE);

	LONG OnThemeChanged(WPARAM wParam, LPARAM lParam);

private:
	void DrawNonClientClassic(HWND hwnd);
	void DrawNonClientModern(HWND hwnd);
	void Draw(HDC hdc);
	void DrawBackground(HDC hdc, RECT& rc, int nImageIndex);
	void DrawGlyph(HDC hdc, RECT& rc, int nColorIndex);
	BOOL GetGlyphColorHor(int i, int iMin, int iMax, int iIndex, COLORGLYPH& st);
	BOOL GetGlyphColorVer(int i, int iMin, int iMax, int iIndex, COLORGLYPH& st);
	int RecalcLayout(HWND hwnd, int& nCY, int& nPartID, HFONT* phfnt, int& nBtnsWidth);

private:
	static LRESULT CALLBACK CaptionFrameWindowProc(HWND, UINT, WPARAM, LPARAM);
	static CMap<HWND, HWND, CCaptionButton*, CCaptionButton*> m_mapCaptionFrames;
	static CCriticalSection m_cs;

private:
	RECT m_rect;
	int m_nBkStyle;
	int m_nState;
	BOOL m_fActive;

	HBITMAP m_hbmpCaption;
	HBITMAP m_hbmpButton;
	HBITMAP m_hbmpBackground;
	HBITMAP m_hbmpGlyph;

	MY_MARGINS m_mrgnBk;
	CThemeUtil m_Theme;
	int m_nImageCount;
	int m_nImageOffset;

	BOOL m_fBkImageHorLayout;
	BOOL m_fBkTransparent;
	COLORREF m_rgbBkTransparent;
	COLORREF m_rgbGlyph[8];

	//
	HWND m_hwndCaption;
	WNDPROC m_pOldWndProc;
	CArray<CCaptionButton*, CCaptionButton*> m_aButtons;
	BYTE m_bHeld;
	int m_nHeldIndex;
	int m_nOverIndex;

#ifdef USE_TOOLTIP_SUPPORT
	CTooltipWrapper m_tooltip;
#endif // USE_TOOLTIP_SUPPORT

protected:
	DWORD m_dwStyle;
	UINT m_nId;
	BOOL m_bEnable;

	DWORD GetStyle()
	{
		return m_dwStyle;
	}

	void SetStyle(DWORD dwStyle)
	{
		m_dwStyle = dwStyle;
	}

	int GetDlgCtrlID()
	{
		return m_nId;
	}

	void SetDlgCtrlID(int nId)
	{
		m_nId = nId;
	}

	BOOL IsEnabled()
	{
		return m_bEnable;
	}

	HWND GetSafeHwnd()
	{
		if(this)
			return m_hwndCaption;

		return NULL;
	}
};

#endif // __CAPTIONBUTTON_H__
