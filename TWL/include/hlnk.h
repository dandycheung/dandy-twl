//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __HLNK_H__
#define __HLNK_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

///////////////////////////////////////////////////////////////////////////////
// CHyperLink - hyper link control implementation
//

// 
// usage sample:
// 
//	CHyperLink _hl;
// 
//	_hl.Create(0, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 80, 20, hwndParent, uId);
//	// _hl.SubclassWindow(GetDlgItem(IDC_STATIC1));
//	_hl.SetLabel(TEXT("Everest"));
//	_hl.SetHyperLink(TEXT("http://spaces.msn.com/members/sluttery"));
// 

#define USE_TOOLTIP

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE                0x0128
#endif //!WM_UPDATEUISTATE

namespace TWL
{

__declspec(selectany) struct
{
	enum { cxWidth = 32, cyHeight = 32 };
	int xHotSpot;
	int yHotSpot;
	unsigned char arrANDPlane[cxWidth * cyHeight / 8];
	unsigned char arrXORPlane[cxWidth * cyHeight / 8];
} _hyperLinkCursorData = 
{
	5, 0, 
	{
		0xF9, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 
		0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xF0, 0x01, 0xFF, 0xFF, 
		0xF0, 0x00, 0xFF, 0xFF, 0x10, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 
		0x80, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x7F, 0xFF, 
		0xE0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 
		0xF8, 0x01, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	},
	{
		0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
		0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x06, 0xD8, 0x00, 0x00, 
		0x06, 0xDA, 0x00, 0x00, 0x06, 0xDB, 0x00, 0x00, 0x67, 0xFB, 0x00, 0x00, 0x77, 0xFF, 0x00, 0x00, 
		0x37, 0xFF, 0x00, 0x00, 0x17, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 
		0x0F, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 
		0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}
};

#define HLINK_UNDERLINED      0x00000000
#define HLINK_NOTUNDERLINED   0x00000001
#define HLINK_UNDERLINEHOVER  0x00000002
#define HLINK_COMMANDBUTTON   0x00000004
#define HLINK_NOTIFYBUTTON    0x0000000C
#define HLINK_USETAGS         0x00000010
#define HLINK_USETAGSBOLD     0x00000030
#define HLINK_NOTOOLTIP       0x00000040

// Notes:
// - HLINK_USETAGS and HLINK_USETAGSBOLD are always left-aligned
// - When HLINK_USETAGSBOLD is used, the underlined styles will be ignored

class CHyperLink : public CUserWindow
{
public:
	LPTSTR m_pszLabel;
	LPTSTR m_pszHyperLink;

	HCURSOR m_hCursor;
	HFONT m_hFont;
	HFONT m_hFontNormal;

	RECT m_rcLink;
#ifdef USE_TOOLTIP
	CToolTipCtrl m_tip;
#endif // USE_TOOLTIP

	COLORREF m_clrLink;
	COLORREF m_clrVisited;

	DWORD m_dwExtendedStyle;   // Hyper Link specific extended styles

	bool m_bPaintLabel:1;
	bool m_bVisited:1;
	bool m_bHover:1;
	bool m_bInternalLinkFont:1;

// Constructor/Destructor
	CHyperLink(DWORD dwExtendedStyle = HLINK_UNDERLINED) : 
			m_pszLabel(NULL), m_pszHyperLink(NULL),
			m_hCursor(NULL), m_hFont(NULL), m_hFontNormal(NULL),
			m_clrLink(RGB(0, 0, 255)), m_clrVisited(RGB(128, 0, 128)),
			m_dwExtendedStyle(dwExtendedStyle),
			m_bPaintLabel(true), m_bVisited(false),
			m_bHover(false), m_bInternalLinkFont(false)
	{
		::SetRectEmpty(&m_rcLink);
	}

	~CHyperLink()
	{
		free(m_pszLabel);
		free(m_pszHyperLink);

		if(m_bInternalLinkFont && m_hFont != NULL)
			::DeleteObject(m_hFont);

		if(m_hCursor != NULL)
			::DestroyCursor(m_hCursor);
	}

	LPCTSTR GetClassName()
	{
		return TEXT("TwlHyperLink");
	}

	// overridden to provide proper initialization
	BOOL SubclassWindow(HWND hWnd)
	{
		BOOL bRet = CUserWindow::SubclassWindow(hWnd);
		if(bRet)
			Init();

		return bRet;
	}

// Attributes
	DWORD GetHyperLinkExtendedStyle() const
	{
		return m_dwExtendedStyle;
	}

	DWORD SetHyperLinkExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0)
	{
		DWORD dwPrevStyle = m_dwExtendedStyle;

		if(dwMask == 0)
			m_dwExtendedStyle = dwExtendedStyle;
		else
			m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
		
		return dwPrevStyle;
	}

	bool GetLabel(LPTSTR pszBuffer, int nLength) const
	{
		if(m_pszLabel == NULL)
			return false;

		ASSERT(pszBuffer != NULL);

		if(nLength > lstrlen(m_pszLabel) + 1)
		{
			lstrcpy(pszBuffer, m_pszLabel);
			return true;
		}

		return false;
	}

	bool SetLabel(LPCTSTR pszLabel)
	{
		free(m_pszLabel);
		m_pszLabel = NULL;

		m_pszLabel = (LPTSTR)malloc((lstrlen(pszLabel) + 1) * sizeof(TCHAR));
		if(m_pszLabel == NULL)
			return false;

		lstrcpy(m_pszLabel, pszLabel);
		CalcLabelRect();

		if(m_hWnd != NULL)
			SetWindowText(pszLabel);   // Set this for accessibility

		return true;
	}

	bool GetHyperLink(LPTSTR pszBuffer, int nLength) const
	{
		if(m_pszHyperLink == NULL)
			return false;

		ASSERT(pszBuffer != NULL);

		if(nLength > lstrlen(m_pszHyperLink) + 1)
		{
			lstrcpy(pszBuffer, m_pszHyperLink);
			return true;
		}

		return false;
	}

	bool SetHyperLink(LPCTSTR pszLink)
	{
		free(m_pszHyperLink);
		m_pszHyperLink = NULL;

		m_pszHyperLink = (LPTSTR)malloc((lstrlen(pszLink) + 1) * sizeof(TCHAR));
		if(m_pszHyperLink == NULL)
			return false;

		lstrcpy(m_pszHyperLink, pszLink);
		if(m_pszLabel == NULL)
			CalcLabelRect();

#ifdef USE_TOOLTIP
		if(m_tip.IsWindow())
		{
			m_tip.Activate(TRUE);
			m_tip.AddTool(m_hWnd, m_pszHyperLink, &m_rcLink, 1);
		}
#endif // USE_TOOLTIP
		return true;
	}

	HFONT GetLinkFont() const
	{
		return m_hFont;
	}

	void SetLinkFont(HFONT hFont)
	{
		if(m_bInternalLinkFont && m_hFont != NULL)
		{
			::DeleteObject(m_hFont);
			m_bInternalLinkFont = false;
		}

		m_hFont = hFont;
	}

	int GetIdealHeight() const
	{
		ASSERT(::IsWindow(m_hWnd));

		if(m_pszLabel == NULL && m_pszHyperLink == NULL)
			return -1;

		if(!m_bPaintLabel)
			return -1;

		HDC hdc = ::GetDC(m_hWnd);
		RECT rect = { 0 };
		GetClientRect(&rect);

		HGDIOBJ hFontOld = ::SelectObject(hdc, m_hFontNormal);
		RECT rcText = rect;
		::DrawText(hdc, _T("NS"), -1, &rcText, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
		::SelectObject(hdc, m_hFont);

		RECT rcLink = rect;
		::DrawText(hdc, _T("NS"), -1, &rcLink, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
		::SelectObject(hdc, hFontOld);
		::ReleaseDC(m_hWnd, hdc);

		return max(rcText.bottom - rcText.top, rcLink.bottom - rcLink.top);
	}

	bool GetIdealSize(SIZE& size) const
	{
		int cx = 0, cy = 0;

		bool bRet = GetIdealSize(cx, cy);
		if(bRet)
		{
			size.cx = cx;
			size.cy = cy;
		}

		return bRet;
	}

	bool GetIdealSize(int& cx, int& cy) const
	{
		ASSERT(::IsWindow(m_hWnd));

		if(m_pszLabel == NULL && m_pszHyperLink == NULL)
			return false;

		if(!m_bPaintLabel)
			return false;

		HDC hdc = ::GetDC(m_hWnd);

		RECT rcClient = { 0 };
		GetClientRect(&rcClient);

		RECT rcAll = rcClient;

		if(IsUsingTags())
		{
			// find tags and label parts
			LPTSTR pszLeft = NULL;
			int cchLeft = 0;
			LPTSTR pszLink = NULL;
			int cchLink = 0;
			LPTSTR pszRight = NULL;
			int cchRight = 0;

			CalcLabelParts(pszLeft, cchLeft, pszLink, cchLink, pszRight, cchRight);

			// get label part rects
			HGDIOBJ hFontOld = ::SelectObject(hdc, m_hFontNormal);
			RECT rcLeft = rcClient;
			::DrawText(hdc, pszLeft, cchLeft, &rcLeft, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			::SelectObject(hdc, m_hFont);
			RECT rcLink = { rcLeft.right, rcLeft.top, rcClient.right, rcClient.bottom };
			::DrawText(hdc, pszLink, cchLink, &rcLink, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			::SelectObject(hdc, m_hFontNormal);
			RECT rcRight = { rcLink.right, rcLink.top, rcClient.right, rcClient.bottom };
			::DrawText(hdc, pszRight, cchRight, &rcRight, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			::SelectObject(hdc, hFontOld);

			int cyMax = max(rcLeft.bottom, max(rcLink.bottom, rcRight.bottom));
			::SetRect(&rcAll, rcLeft.left, rcLeft.top, rcRight.right, cyMax);
		}
		else
		{
			HGDIOBJ hOldFont = NULL;
			if(m_hFont != NULL)
				hOldFont = ::SelectObject(hdc, m_hFont);

			LPTSTR pszText = (m_pszLabel != NULL) ? m_pszLabel : m_pszHyperLink;
			DWORD dwStyle = GetStyle();

			int nDrawStyle = DT_LEFT;
			if(dwStyle & SS_CENTER)
				nDrawStyle = DT_CENTER;
			else if(dwStyle & SS_RIGHT)
				nDrawStyle = DT_RIGHT;

			::DrawText(hdc, pszText, -1, &rcAll, nDrawStyle | DT_WORDBREAK | DT_CALCRECT);

			if(m_hFont != NULL)
				::SelectObject(hdc, hOldFont);

			if(dwStyle & SS_CENTER)
			{
				int dx = (rcClient.right - rcAll.right) / 2;
				::OffsetRect(&rcAll, dx, 0);
			}
			else if(dwStyle & SS_RIGHT)
			{
				int dx = rcClient.right - rcAll.right;
				::OffsetRect(&rcAll, dx, 0);
			}
		}

		::ReleaseDC(m_hWnd, hdc);

		cx = rcAll.right - rcAll.left;
		cy = rcAll.bottom - rcAll.top;

		return true;
	}

	// for command buttons only
	bool GetToolTipText(LPTSTR pszBuffer, int nLength) const
	{
		ASSERT(IsCommandButton());
		return GetHyperLink(pszBuffer, nLength);
	}

	bool SetToolTipText(LPCTSTR pszToolTipText)
	{
		ASSERT(IsCommandButton());
		return SetHyperLink(pszToolTipText);
	}

	virtual bool Navigate()
	{
		ASSERT(::IsWindow(m_hWnd));

		bool bRet = true;
		if(IsNotifyButton())
		{
			NMHDR nmhdr = { m_hWnd, GetDlgCtrlID(), NM_CLICK };
			::SendMessage(GetParent(), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
		}
		else if(IsCommandButton())
		{
			::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
		}
		else
		{
			ASSERT(m_pszHyperLink != NULL);

			DWORD_PTR dwRet = (DWORD_PTR)::ShellExecute(0, _T("open"), m_pszHyperLink, 0, 0, SW_SHOWNORMAL);
			bRet = (dwRet > 32);
			ASSERT(bRet);
			if(bRet)
			{
				m_bVisited = true;
				Invalidate();
			}
		}

		return bRet;
	}

// Message map and handlers
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
#ifdef USE_TOOLTIP
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
#endif // USE_TOOLTIP
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_ENABLE, OnEnable)
		MESSAGE_HANDLER(WM_GETFONT, OnGetFont)
		MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
		MESSAGE_HANDLER(WM_UPDATEUISTATE, OnUpdateUiState)
	END_MSG_MAP()

	bool OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& /*lResult*/)
	{
		Init();
		return false;
	}

#ifdef USE_TOOLTIP
	bool OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		if(m_tip.IsWindow())
		{
			m_tip.DestroyWindow();
			m_tip.m_hWnd = NULL;
		}

		lResult = 1;
		return false;
	}

	bool OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		MSG msg = { m_hWnd, uMsg, wParam, lParam };
		if(m_tip.IsWindow() && IsUsingToolTip())
			m_tip.RelayEvent(&msg);

		lResult = 1;
		return false;
	}
#endif // USE_TOOLTIP

	bool OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		lResult = 1;   // no background painting needed (we do it all during WM_PAINT)
		return true;
	}

	bool OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, LRESULT& lResult)
	{
		if(!m_bPaintLabel)
		{
			lResult = 1;
			return false;
		}

		if(wParam != NULL)
		{
			DoEraseBackground((HDC)wParam);
			DoPaint((HDC)wParam);
		}
		else
		{
			PAINTSTRUCT ps;
			::BeginPaint(m_hWnd, &ps);
			DoEraseBackground(ps.hdc);
			DoPaint(ps.hdc);
			::EndPaint(m_hWnd, &ps);
		}

		return true;
	}

	bool OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		if(!m_bPaintLabel)
			return false;

		Invalidate();

		return true;
	}

	bool OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, LRESULT& lResult)
	{
		POINT pt = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
		if((m_pszHyperLink != NULL  || IsCommandButton()) && ::PtInRect(&m_rcLink, pt))
		{
			::SetCursor(m_hCursor);
			if(IsUnderlineHover())
			{
				if(!m_bHover)
				{
					m_bHover = true;
					InvalidateRect(&m_rcLink);
					UpdateWindow();
					StartTrackMouseLeave();
				}
			}
		}
		else
		{
			if(IsUnderlineHover())
			{
				if(m_bHover)
				{
					m_bHover = false;
					InvalidateRect(&m_rcLink);
					UpdateWindow();
				}
			}

			return false;
		}

		return true;
	}

	bool OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& /*lResult*/)
	{
		if(IsUnderlineHover() && m_bHover)
		{
			m_bHover = false;
			InvalidateRect(&m_rcLink);
			UpdateWindow();
		}

		return true;
	}

	bool OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, LRESULT& /*lResult*/)
	{
		POINT pt = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
		if(::PtInRect(&m_rcLink, pt))
		{
			SetFocus();
			SetCapture();
		}

		return true;
	}

	bool OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, LRESULT& /*lResult*/)
	{
		if(GetCapture() == m_hWnd)
		{
			ReleaseCapture();

			POINT pt = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
			if(::PtInRect(&m_rcLink, pt))
				Navigate();
		}

		return true;
	}

	bool OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, LRESULT& /*lResult*/)
	{
		if(wParam == VK_RETURN || wParam == VK_SPACE)
			Navigate();

		return true;
	}

	bool OnGetDlgCode(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		lResult = DLGC_WANTCHARS;
		return true;
	}

	bool OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		POINT pt = { 0, 0 };
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		if((m_pszHyperLink != NULL  || IsCommandButton()) && ::PtInRect(&m_rcLink, pt))
		{
			lResult = TRUE;
			return true;
		}

		return false;
	}

	bool OnEnable(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& /*lResult*/)
	{
		Invalidate();
		UpdateWindow();

		return true;
	}

	bool OnGetFont(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& lResult)
	{
		lResult = (LRESULT)m_hFontNormal;
		return true;
	}

	bool OnSetFont(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, LRESULT& /*lResult*/)
	{
		m_hFontNormal = (HFONT)wParam;
		if((BOOL)lParam)
		{
			Invalidate();
			UpdateWindow();
		}

		return true;
	}

	bool OnUpdateUiState(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT& /*lResult*/)
	{
		// If the control is subclassed or superclassed, this message can cause
		// repainting without WM_PAINT. We don't use this state, so just do nothing.
		return true;
	}

// Implementation
	virtual void Init()
	{
		ASSERT(::IsWindow(m_hWnd));

		// Check if we should paint a label
		const int cchBuff = 8;
		TCHAR szBuffer[cchBuff] = { 0 };
		if(::GetClassName(m_hWnd, szBuffer, cchBuff))
		{
			if(lstrcmpi(szBuffer, _T("static")) == 0)
			{
				ModifyStyle(0, SS_NOTIFY);   // we need this
				DWORD dwStyle = GetStyle() & 0x000000FF;
				if(dwStyle == SS_ICON || dwStyle == SS_BLACKRECT || dwStyle == SS_GRAYRECT || 
						dwStyle == SS_WHITERECT || dwStyle == SS_BLACKFRAME || dwStyle == SS_GRAYFRAME || 
						dwStyle == SS_WHITEFRAME || dwStyle == SS_OWNERDRAW || 
						dwStyle == SS_BITMAP || dwStyle == SS_ENHMETAFILE)
					m_bPaintLabel = false;
			}
		}

		// create or load a cursor
		m_hCursor = ::CreateCursor(GetModuleInstance(), _hyperLinkCursorData.xHotSpot, _hyperLinkCursorData.yHotSpot, _hyperLinkCursorData.cxWidth, _hyperLinkCursorData.cyHeight, _hyperLinkCursorData.arrANDPlane, _hyperLinkCursorData.arrXORPlane);
		ASSERT(m_hCursor != NULL);

		// set font
		if(m_bPaintLabel)
		{
			HWND hwndParent = ::GetParent(m_hWnd);
			m_hFontNormal = (HFONT)::SendMessage(hwndParent, WM_GETFONT, 0, 0);
			if(m_hFontNormal == NULL)
				m_hFontNormal = (HFONT)::GetStockObject(SYSTEM_FONT);
			if(m_hFontNormal != NULL && m_hFont == NULL)
			{
				LOGFONT lf = { 0 };
				::GetObject(m_hFontNormal, sizeof(LOGFONT), &lf);

				if(IsUsingTagsBold())
					lf.lfWeight = FW_BOLD;
				else if(!IsNotUnderlined())
					lf.lfUnderline = TRUE;

				m_hFont = ::CreateFontIndirect(&lf);
				m_bInternalLinkFont = true;

				ASSERT(m_hFont != NULL);
			}
		}

#ifdef USE_TOOLTIP
		// create a tool tip
		m_tip.Create(m_hWnd);
		ASSERT(m_tip.IsWindow());
#endif // USE_TOOLTIP

		// set label (defaults to window text)
		if(m_pszLabel == NULL)
		{
			int nLen = GetWindowTextLength();
			if(nLen > 0)
			{
				LPTSTR pszText = new TCHAR[nLen + 1];
				if(pszText != NULL)
				{
					if(GetWindowText(pszText, nLen + 1))
						SetLabel(pszText);

					delete[] pszText;
				}
			}
		}

		CalcLabelRect();

		// set hyperlink (defaults to label), or just activate tool tip if already set
		if(m_pszHyperLink == NULL && !IsCommandButton())
		{
			if(m_pszLabel != NULL)
				SetHyperLink(m_pszLabel);
		}
#ifdef USE_TOOLTIP
		else
		{
			m_tip.Activate(TRUE);
			m_tip.AddTool(m_hWnd, m_pszHyperLink, &m_rcLink, 1);
		}
#endif // USE_TOOLTIP

		// set link colors
		if(m_bPaintLabel)
		{
			HKEY hKey = NULL;
			LONG lRet = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Settings"),
				0, KEY_ALL_ACCESS, &hKey);

			if(lRet == ERROR_SUCCESS)
			{
				const int cchBuff = 12;
				TCHAR szBuff[cchBuff] = { 0 };
				DWORD dwCount = cchBuff * sizeof(TCHAR);

				DWORD dwType = 0;
				lRet = RegQueryValueEx(hKey, _T("Anchor Color"), NULL, &dwType, (LPBYTE)szBuff, &dwCount);
				if(lRet == ERROR_SUCCESS)
				{
					COLORREF clr = _ParseColorString(szBuff);
					ASSERT(clr != CLR_INVALID);
					if(clr != CLR_INVALID)
						m_clrLink = clr;
				}

				dwCount = cchBuff * sizeof(TCHAR);
				lRet = RegQueryValueEx(hKey, _T("Anchor Color Visited"), NULL, &dwType, (LPBYTE)szBuff, &dwCount);
				if(lRet == ERROR_SUCCESS)
				{
					COLORREF clr = _ParseColorString(szBuff);
					ASSERT(clr != CLR_INVALID);
					if(clr != CLR_INVALID)
						m_clrVisited = clr;
				}

				RegCloseKey(hKey);
			}
		}
	}

	static COLORREF _ParseColorString(LPTSTR psz)
	{
		int c[3] = { -1, -1, -1 };
		LPTSTR p = NULL;

		for(int i = 0; i < 2; i++)
		{
			for(p = psz; *p != _T('\0'); p = ::CharNext(p))
			{
				if(*p == _T(','))
				{
					*p = _T('\0');
					c[i] = _xttoi(psz);
					psz = &p[1];
					break;
				}
			}

			if(c[i] == -1)
				return CLR_INVALID;
		}

		if(*psz == _T('\0'))
			return CLR_INVALID;

		c[2] = _xttoi(psz);

		return RGB(c[0], c[1], c[2]);
	}

	virtual bool CalcLabelRect()
	{
		if(!::IsWindow(m_hWnd))
			return false;

		if(m_pszLabel == NULL && m_pszHyperLink == NULL)
			return false;

		HDC hdc = ::GetDC(m_hWnd);
		RECT rcClient = { 0 };
		GetClientRect(&rcClient);

		m_rcLink = rcClient;

		if(!m_bPaintLabel)
			return true;

		if(IsUsingTags())
		{
			// find tags and label parts
			LPTSTR pszLeft = NULL;
			int cchLeft = 0;
			LPTSTR pszLink = NULL;
			int cchLink = 0;
			LPTSTR pszRight = NULL;
			int cchRight = 0;

			CalcLabelParts(pszLeft, cchLeft, pszLink, cchLink, pszRight, cchRight);
			ASSERT(pszLink != NULL);
			ASSERT(cchLink > 0);

			// get label part rects
			HGDIOBJ hFontOld = ::SelectObject(hdc, m_hFontNormal);

			RECT rcLeft = rcClient;
			if(pszLeft != NULL)
				::DrawText(hdc, pszLeft, cchLeft, &rcLeft, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			::SelectObject(hdc, m_hFont);
			RECT rcLink = rcClient;
			if(pszLeft != NULL)
				rcLink.left = rcLeft.right;
			::DrawText(hdc, pszLink, cchLink, &rcLink, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			::SelectObject(hdc, hFontOld);

			m_rcLink = rcLink;
		}
		else
		{
			HGDIOBJ hOldFont = NULL;
			if(m_hFont != NULL)
				hOldFont = ::SelectObject(hdc, m_hFont);

			LPTSTR pszText = (m_pszLabel != NULL) ? m_pszLabel : m_pszHyperLink;
			DWORD dwStyle = GetStyle();

			int nDrawStyle = DT_LEFT;
			if(dwStyle & SS_CENTER)
				nDrawStyle = DT_CENTER;
			else if(dwStyle & SS_RIGHT)
				nDrawStyle = DT_RIGHT;

			::DrawText(hdc, pszText, -1, &m_rcLink, nDrawStyle | DT_WORDBREAK | DT_CALCRECT);

			if(m_hFont != NULL)
				::SelectObject(hdc, hOldFont);

			if(dwStyle & SS_CENTER)
			{
				int dx = (rcClient.right - m_rcLink.right) / 2;
				::OffsetRect(&m_rcLink, dx, 0);
			}
			else if(dwStyle & SS_RIGHT)
			{
				int dx = rcClient.right - m_rcLink.right;
				::OffsetRect(&m_rcLink, dx, 0);
			}
		}

		::ReleaseDC(m_hWnd, hdc);

		return true;
	}

	virtual void CalcLabelParts(LPTSTR& pszLeft, int& cchLeft, LPTSTR& pszLink, int& cchLink, LPTSTR& pszRight, int& cchRight) const
	{
		pszLeft = NULL;
		cchLeft = 0;
		pszLink = NULL;
		cchLink = 0;
		pszRight = NULL;
		cchRight = 0;

		LPTSTR pszText = (m_pszLabel != NULL) ? m_pszLabel : m_pszHyperLink;
		int cchText = lstrlen(pszText);
		bool bOutsideLink = true;
		for(int i = 0; i < cchText; i++)
		{
			if(pszText[i] != _T('<'))
				continue;

			if(bOutsideLink)
			{
				if(::CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, &pszText[i], 3, _T("<A>"), 3) == CSTR_EQUAL)
				{
					if(i > 0)
					{
						pszLeft = pszText;
						cchLeft = i;
					}
					pszLink = &pszText[i + 3];
					bOutsideLink = false;
				}
			}
			else
			{
				if(::CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, &pszText[i], 4, _T("</A>"), 4) == CSTR_EQUAL)
				{
					cchLink = i - 3 - cchLeft;
					if(pszText[i + 4] != 0)
					{
						pszRight = &pszText[i + 4];
						cchRight = cchText - (i + 4);
						break;
					}
				}
			}
		}
	}

	virtual void DoEraseBackground(HDC hdc)
	{
		HBRUSH hBrush = (HBRUSH)::SendMessage(GetParent(), WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)m_hWnd);
		if(hBrush != NULL)
		{
			RECT rect = { 0 };
			GetClientRect(&rect);
			::FillRect(hdc, &rect, hBrush);
		}
	}

	virtual void DoPaint(HDC hdc)
	{
		if(IsUsingTags())
		{
			// find tags and label parts
			LPTSTR pszLeft = NULL;
			int cchLeft = 0;
			LPTSTR pszLink = NULL;
			int cchLink = 0;
			LPTSTR pszRight = NULL;
			int cchRight = 0;

			CalcLabelParts(pszLeft, cchLeft, pszLink, cchLink, pszRight, cchRight);

			// get label part rects
			RECT rcClient = { 0 };
			GetClientRect(&rcClient);

			::SetBkMode(hdc, TRANSPARENT);
			HGDIOBJ hFontOld = ::SelectObject(hdc, m_hFontNormal);

			if(pszLeft != NULL)
				::DrawText(hdc, pszLeft, cchLeft, &rcClient, DT_LEFT | DT_WORDBREAK);

			COLORREF clrOld = ::SetTextColor(hdc, IsWindowEnabled() ? (m_bVisited ? m_clrVisited : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));
			if(m_hFont != NULL && (!IsUnderlineHover() || (IsUnderlineHover() && m_bHover)))
				::SelectObject(hdc, m_hFont);
			else
				::SelectObject(hdc, m_hFontNormal);

			::DrawText(hdc, pszLink, cchLink, &m_rcLink, DT_LEFT | DT_WORDBREAK);

			::SetTextColor(hdc, clrOld);
			::SelectObject(hdc, m_hFontNormal);
			if(pszRight != NULL)
			{
				RECT rcRight = { m_rcLink.right, m_rcLink.top, rcClient.right, rcClient.bottom };
				::DrawText(hdc, pszRight, cchRight, &rcRight, DT_LEFT | DT_WORDBREAK);
			}

			if(GetFocus() == m_hWnd)
				::DrawFocusRect(hdc, &m_rcLink);

			::SelectObject(hdc, hFontOld);
		}
		else
		{
			::SetBkMode(hdc, TRANSPARENT);
			COLORREF clrOld = ::SetTextColor(hdc, IsWindowEnabled() ? (m_bVisited ? m_clrVisited : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));

			HGDIOBJ hFontOld = NULL;
			if(m_hFont != NULL && (!IsUnderlineHover() || (IsUnderlineHover() && m_bHover)))
				hFontOld = ::SelectObject(hdc, m_hFont);
			else
				hFontOld = ::SelectObject(hdc, m_hFontNormal);

			LPTSTR pszText = (m_pszLabel != NULL) ? m_pszLabel : m_pszHyperLink;

			DWORD dwStyle = GetStyle();
			int nDrawStyle = DT_LEFT;
			if(dwStyle & SS_CENTER)
				nDrawStyle = DT_CENTER;
			else if(dwStyle & SS_RIGHT)
				nDrawStyle = DT_RIGHT;

			::DrawText(hdc, pszText, -1, &m_rcLink, nDrawStyle | DT_WORDBREAK);

			if(GetFocus() == m_hWnd)
				::DrawFocusRect(hdc, &m_rcLink);

			::SetTextColor(hdc, clrOld);
			::SelectObject(hdc, hFontOld);
		}
	}

	BOOL StartTrackMouseLeave()
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		return _TrackMouseEvent(&tme);
	}

// Implementation helpers
	bool IsUnderlined() const
	{
		return ((m_dwExtendedStyle & (HLINK_NOTUNDERLINED | HLINK_UNDERLINEHOVER)) == 0);
	}

	bool IsNotUnderlined() const
	{
		return ((m_dwExtendedStyle & HLINK_NOTUNDERLINED) != 0);
	}

	bool IsUnderlineHover() const
	{
		return ((m_dwExtendedStyle & HLINK_UNDERLINEHOVER) != 0);
	}

	bool IsCommandButton() const
	{
		return ((m_dwExtendedStyle & HLINK_COMMANDBUTTON) != 0);
	}

	bool IsNotifyButton() const
	{
		return ((m_dwExtendedStyle & HLINK_NOTIFYBUTTON) == HLINK_NOTIFYBUTTON);
	}

	bool IsUsingTags() const
	{
		return ((m_dwExtendedStyle & HLINK_USETAGS) != 0);
	}

	bool IsUsingTagsBold() const
	{
		return ((m_dwExtendedStyle & HLINK_USETAGSBOLD) == HLINK_USETAGSBOLD);
	}

	bool IsUsingToolTip() const
	{
		return ((m_dwExtendedStyle & HLINK_NOTOOLTIP) == 0);
	}

	static int _xttoi(const TCHAR* nptr)
	{
		// return _ttoi(nptr);
		while(*nptr == _T(' '))   // skip spaces
			++nptr;

		int c = (int)(_TUCHAR)*nptr++;
		int sign = c;   // save sign indication
		if (c == _T('-') || c == _T('+'))
			c = (int)(_TUCHAR)*nptr++;   // skip sign

		int total = 0;
		while((TCHAR)c >= _T('0') && (TCHAR)c <= _T('9'))
		{
			total = 10 * total + ((TCHAR)c - _T('0'));   // accumulate digit
			c = (int)(_TUCHAR)*nptr++;        // get next char
		}

		// return result, negated if necessary
		return ((TCHAR)sign != _T('-')) ? total : -total;
	}
};

}; // namespace TWL

#endif // __HLNK_H__
