#include "stdafx.h"
#include "CaptionButton.h"
#include "tme.h"

#ifndef ICON_SMALL2
#define ICON_SMALL2		2
#endif // ICON_SMALL2

#define RectWidth(rc)	((rc).right - (rc).left)
#define RectHeight(rc)	((rc).bottom - (rc).top)

inline void FillSolidRect(HDC hdc, PRECT prc, COLORREF clr)
{
	COLORREF clrOld = SetBkColor(hdc, clr);
	if(clrOld != CLR_INVALID)
	{
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, prc, NULL, 0, NULL);
		SetBkColor(hdc, clrOld);
	}
}

inline void FillSolidRect(HDC hdc, int nLeft, int nTop, int nRight, int nBottom, COLORREF clr)
{
	RECT rc = { nLeft, nTop, nRight, nBottom };
	FillSolidRect(hdc, &rc, clr);
}

static BOOL RedrawWindowFrame(HWND hwnd, PRECT prc = NULL, UINT uFlags = RDW_FRAME | RDW_INVALIDATE)
{
	RECT rc;
	
	if(prc)
		rc = *prc;
	else
	{
		GetWindowRect(hwnd, &rc);
		OffsetRect(&rc, -rc.left, -rc.top);
	}
	
	return RedrawWindow(hwnd, &rc, NULL, uFlags);
}

static HICON GetWindowSystemMenuIcon(HWND hwnd)
{
	HICON hIcon = (HICON)SendMessage(hwnd, WM_GETICON, ICON_SMALL, 0);
	if(hIcon == NULL)
	{
		hIcon = (HICON)SendMessage(hwnd, WM_GETICON, ICON_BIG, 0);
		if(hIcon == NULL)
		{
			hIcon = (HICON)SendMessage(hwnd, WM_GETICON, ICON_SMALL2, 0);
			if(hIcon == NULL)
			{
				hIcon = (HICON)GetClassLong(hwnd, GCL_HICONSM);
				if(hIcon == NULL)
				{
					hIcon = (HICON)GetClassLong(hwnd, GCL_HICON);
					if(hIcon == NULL)
						hIcon = (HICON)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
				}
			}
		}
	}

	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CCaptionButton

CMap<HWND, HWND, CCaptionButton*, CCaptionButton*> CCaptionButton::m_mapCaptionFrames;
CCriticalSection CCaptionButton::m_cs;

CCaptionButton::CCaptionButton()
	: m_nBkStyle(CBNBKGNDSTYLE_CAPTION)
	, m_nState(CBNST_NORMAL)
	, m_fActive(TRUE)
	, m_hbmpCaption(NULL)
	, m_hbmpButton(NULL)
	, m_hbmpBackground(NULL)
	, m_hbmpGlyph(NULL)
	, m_nImageCount(0)
	, m_nImageOffset(0)
	, m_hwndCaption(NULL)
	, m_pOldWndProc(NULL)
	, m_bHeld(HELDNONE)
	, m_nHeldIndex(-1)
	, m_nOverIndex(-1)
	, m_dwStyle(0)
	, m_nId(0)
	, m_bEnable(TRUE)
{
	SetRectEmpty(&m_rect);
}

CCaptionButton::~CCaptionButton()
{
	DestroyCaptionFrame();

	::DeleteObject(m_hbmpCaption);
	m_hbmpCaption = NULL;
	
	::DeleteObject(m_hbmpButton);
	m_hbmpButton = NULL;

	::DeleteObject(m_hbmpBackground);
	m_hbmpBackground = NULL;

	::DeleteObject(m_hbmpGlyph);
	m_hbmpGlyph = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CCaptionButton message handlers

int CCaptionButton::CreateCaptionFrame(HWND hwndCaption)
{
	if(hwndCaption == NULL)
		return CBNERR_INCORRECTPARAMETER;

	if(m_pOldWndProc)
		return CBNERR_FRAMEALREADYCREATED;

	LONG dwStyle = GetWindowLong(hwndCaption, GWL_STYLE);
	if(!(dwStyle & WS_CAPTION))
		return CBNERR_INCORRECTFRAMESTYLE;

	CCaptionButton* p;
	m_cs.Lock();
	if(m_mapCaptionFrames.Lookup(hwndCaption, p))
	{
		m_cs.Unlock();
		return CBNERR_INCORRECTPARAMETER;
	}

	if(!(m_pOldWndProc = (WNDPROC)::SetWindowLong(hwndCaption, GWL_WNDPROC, (LONG)CaptionFrameWindowProc)))
	{
		m_cs.Unlock();
		return CBNERR_SETWINDOWLONGFAILED;
	}

	m_mapCaptionFrames.SetAt(hwndCaption, this);
	m_hwndCaption = hwndCaption;
	m_cs.Unlock();

	int nRet;

	RECT rc = { 0, 0, 0, 0 };
	if((nRet = AddButton(&rc, 0, CBNBKGNDSTYLE_CLOSE, FALSE)))		// Close button
	{
		DestroyCaptionFrame();
		return nRet;
	}

	if((nRet = AddButton(&rc, 0, CBNBKGNDSTYLE_CAPTION, FALSE)))	// Maximize button
	{
		DestroyCaptionFrame();
		return nRet;
	}

	if((nRet = AddButton(&rc, 0, CBNBKGNDSTYLE_CAPTION, FALSE)))	// Minimize button
	{
		DestroyCaptionFrame();
		return nRet;
	}

	if((nRet = AddButton(&rc, 0, CBNBKGNDSTYLE_CAPTION, FALSE)))	// Help button
	{
		DestroyCaptionFrame();
		return nRet;
	}

	m_Theme.CloseThemeData();
	m_Theme.OpenThemeData(hwndCaption, L"WINDOW");

	return CBNERR_NOERROR;
}

int CCaptionButton::DestroyCaptionFrame()
{
	if(!m_pOldWndProc)
		return CBNERR_NOERROR;

	m_cs.Lock();
	if(!((WNDPROC)::SetWindowLong(m_hwndCaption, GWL_WNDPROC, (DWORD)m_pOldWndProc)))
	{
		m_cs.Unlock();
		return CBNERR_SETWINDOWLONGFAILED;
	}

	m_mapCaptionFrames.RemoveKey(m_hwndCaption);
	m_pOldWndProc = NULL;
	m_hwndCaption = NULL;
	m_cs.Unlock();

	for(int i = 0; i < m_aButtons.GetSize(); i++)
	{
		CCaptionButton* pButton = m_aButtons[i];
		delete pButton;
	}

	m_aButtons.RemoveAll();
	m_Theme.CloseThemeData();

	return CBNERR_NOERROR;
}

int CCaptionButton::AddButton(LPCRECT prc, UINT nID, int nBkStyle, BOOL fUserDefWidth)
{
	if(!m_pOldWndProc)
		return CBNERR_FRAMENOTCREATED;

	CCaptionButton* pButton = new CCaptionButton();
	if(pButton == NULL)
		return CBNERR_OUTOFMEMORY;

	pButton->m_rect = *prc;
	if(nBkStyle >= CBNBKGNDSTYLE_CAPTION && nBkStyle <= CBNBKGNDSTYLE_SPINDNHOR)
		pButton->m_nBkStyle = nBkStyle;
	else
	{
		delete pButton;
		return CBNERR_INCORRECTPARAMETER;
	}

	DWORD dwStyle = 0;
	if(fUserDefWidth)
		dwStyle |= CBNSTYLE_USERDEFWIDTH;

	pButton->SetStyle(dwStyle);
	pButton->SetDlgCtrlID(nID);

	pButton->m_hwndCaption = m_hwndCaption;
	int nIndex = m_aButtons.Add(pButton);

	pButton->OnThemeChanged(0, 0);

	if(nIndex >= 4)
	{
		RECT rc;
		GetWindowRect(m_hwndCaption, &rc);
		MoveWindow(m_hwndCaption, rc.left, rc.top, RectWidth(rc), RectHeight(rc), TRUE);
	}

	return CBNERR_NOERROR;
}

CCaptionButton* CCaptionButton::GetButton(int nButtonID)
{
	if(!m_pOldWndProc)
		return NULL;
	
	for(int i = 4; i < m_aButtons.GetSize(); i++)
	{
		if(m_aButtons[i]->GetDlgCtrlID() == nButtonID)
			return m_aButtons[i];
	}
	
	return NULL;
}

int CCaptionButton::SetBkStyle(int nBkStyle)
{
	if(m_pOldWndProc)
		return CBNERR_INCORRECTFUNCCALL;

	if(nBkStyle >= CBNBKGNDSTYLE_CAPTION && nBkStyle <= CBNBKGNDSTYLE_SCROLLRIGHT)
	{
		m_nBkStyle = nBkStyle;
		OnThemeChanged(0, 0);

		if(m_hwndCaption)
			RedrawWindow(m_hwndCaption, &m_rect, NULL, RDW_FRAME | RDW_INVALIDATE);

		return CBNERR_NOERROR;
	}

	return CBNERR_INCORRECTPARAMETER;
}

int CCaptionButton::SetGlyphBitmap(HBITMAP hbmpGlyph)
{
	if(m_pOldWndProc)
		return CBNERR_INCORRECTFUNCCALL;

	::DeleteObject(m_hbmpGlyph);
	m_hbmpGlyph = NULL;

	if(hbmpGlyph == NULL)
	{
		if(m_hwndCaption)
			RedrawWindow(m_hwndCaption, &m_rect, NULL, RDW_FRAME | RDW_INVALIDATE);

		return CBNERR_NOERROR;
	}

	BITMAP bm;
	GetObject(hbmpGlyph, sizeof(BITMAP), &bm);
	m_hbmpGlyph = (HBITMAP)::CopyImage(hbmpGlyph, IMAGE_BITMAP, bm.bmWidth, bm.bmHeight, LR_MONOCHROME);
	if(m_hbmpGlyph == NULL)
		return CBNERR_COPYIMAGEFAILED;

	if(m_hwndCaption)
		RedrawWindow(m_hwndCaption, &m_rect, NULL, RDW_FRAME | RDW_INVALIDATE);

	return CBNERR_NOERROR;
}

#ifdef USE_TOOLTIP_SUPPORT
int CCaptionButton::SetTooltipText(LPCTSTR pszText)
{
	if(m_pOldWndProc)
		return CBNERR_INCORRECTFUNCCALL;
	
	if(m_hwndCaption == NULL)
		return CBNERR_INCORRECTBUTTONSTYLE;
	
	CCaptionButton* p;

	m_cs.Lock();
	if(!m_mapCaptionFrames.Lookup(m_hwndCaption, p))
	{
		m_cs.Unlock();
		return CBNERR_INCORRECTBUTTONSTYLE;
	}
	m_cs.Unlock();

	if(p->m_tooltip.m_hWnd == NULL)
	{
		if(!p->m_tooltip.Create(m_hwndCaption))
			return CBNERR_CREATETOOLTIPCTRLFAILED;
		
		p->m_tooltip.Activate(TRUE);
	}

	p->m_tooltip.DelTool(m_hwndCaption, GetDlgCtrlID());

	RECT rc = { 0, 0, 0, 0 };
	p->m_tooltip.AddTool(m_hwndCaption, pszText, &rc, GetDlgCtrlID());

	return CBNERR_NOERROR;
}
#endif // USE_TOOLTIP_SUPPORT

int CCaptionButton::Enable(BOOL fEnable)
{
	if(m_pOldWndProc)
		return CBNERR_INCORRECTFUNCCALL;

	m_bEnable = fEnable;

	if(m_hwndCaption)
		RedrawWindow(m_hwndCaption, &m_rect, NULL, RDW_FRAME | RDW_INVALIDATE);

	return CBNERR_NOERROR;
}

LONG CCaptionButton::OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
	::DeleteObject(m_hbmpBackground);
	m_hbmpBackground = NULL;

	m_Theme.CloseThemeData();

	int nPartID;
	int nGlyphPropID;
	WCHAR sClass[50];
	switch(m_nBkStyle)
	{
	case CBNBKGNDSTYLE_CAPTION:
		m_nImageCount = 8;
		m_nImageOffset = 0;
		nPartID = WP_MAXBUTTON;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"WINDOW");
		break;
	case CBNBKGNDSTYLE_CLOSE:
		m_nImageCount = 8;
		m_nImageOffset = 0;
		nPartID = WP_CLOSEBUTTON;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"WINDOW");
		break;
	case CBNBKGNDSTYLE_MDI:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = WP_MDIMINBUTTON;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"WINDOW");
		break;
	case CBNBKGNDSTYLE_COMBO:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = CP_DROPDOWNBUTTON;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"COMBOBOX");
		break;
	case CBNBKGNDSTYLE_SPINUP:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = SPNP_UP;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"SPIN");
		break;
	case CBNBKGNDSTYLE_SPINDN:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = SPNP_DOWN;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"SPIN");
		break;
	case CBNBKGNDSTYLE_SPINUPHOR:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = SPNP_UPHORZ;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"SPIN");
		break;
	case CBNBKGNDSTYLE_SPINDNHOR:
		m_nImageCount = 4;
		m_nImageOffset = 0;
		nPartID = SPNP_DOWNHORZ;
		nGlyphPropID = TMT_GLYPHIMAGEFILE;
		wcscpy(sClass, L"SPIN");
		break;
	case CBNBKGNDSTYLE_SCROLLUP:
		m_nImageCount = 16;
		m_nImageOffset = 0;
		nPartID = SBP_ARROWBTN;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"SCROLLBAR");
		break;
	case CBNBKGNDSTYLE_SCROLLDOWN:
		m_nImageCount = 16;
		m_nImageOffset = 4;
		nPartID = SBP_ARROWBTN;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"SCROLLBAR");
		break;
	case CBNBKGNDSTYLE_SCROLLLEFT:
		m_nImageCount = 16;
		m_nImageOffset = 8;
		nPartID = SBP_ARROWBTN;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"SCROLLBAR");
		break;
	case CBNBKGNDSTYLE_SCROLLRIGHT:
		m_nImageCount = 16;
		m_nImageOffset = 12;
		nPartID = SBP_ARROWBTN;
		nGlyphPropID = TMT_IMAGEFILE1;
		wcscpy(sClass, L"SCROLLBAR");
		break;
	default:
		return 0;
	}

	HBITMAP hbmpGlyph = NULL;
	HDC hdcGlyph = CreateCompatibleDC(NULL);
	HGDIOBJ hbmpGlyphOld = NULL;

	BOOL bSucceeded = FALSE;
	for(;;)
	{
		if(!m_Theme.OpenThemeData(m_hwndCaption, sClass))
			break;

		{
			// background
			int nBkType;
			if(!m_Theme.GetThemeEnumValue(nPartID, 0, TMT_BGTYPE, &nBkType))
				break;
			if(nBkType != BT_IMAGEFILE)
				break;

			int nImageCount;
			if(!m_Theme.GetThemeInt(nPartID, 0, TMT_IMAGECOUNT, &nImageCount))
				break;
			if(nImageCount != m_nImageCount)
				break;

			if(!m_Theme.GetThemeBitmap(nPartID, 0, TMT_IMAGEFILE, &m_hbmpBackground))
				break;

			int nRet = m_Theme.IsThemeBackgroundPartiallyTransparent(nPartID, 0);
			if(nRet == -1)
				break;

			m_fBkTransparent = nRet;
			if(nRet)
			{
				m_rgbBkTransparent = RGB(255, 0, 255);
				if(!m_Theme.GetThemeColor(nPartID, 0, TMT_TRANSPARENTCOLOR, &m_rgbBkTransparent))
					break;
			}

			int nImageLayout;
			if(!m_Theme.GetThemeEnumValue(nPartID, 0, TMT_IMAGELAYOUT, &nImageLayout))
				break;

			if(nImageLayout == IL_VERTICAL)
				m_fBkImageHorLayout = FALSE;
			else
				m_fBkImageHorLayout = TRUE;

			if(!m_Theme.GetThemeMargins(nPartID, 0, TMT_SIZINGMARGINS, &m_mrgnBk))
				break;

			if(m_Theme.IsLunaTheme() && (m_nBkStyle == CBNBKGNDSTYLE_CAPTION || m_nBkStyle == CBNBKGNDSTYLE_CLOSE))
			{
				// Set the corners to the transparent color
				BITMAP bm;
				GetObject(m_hbmpBackground, sizeof(BITMAP), &bm);

				HDC hdcBackground = CreateCompatibleDC(NULL);
				HGDIOBJ hbmpOld = SelectObject(hdcBackground, m_hbmpBackground);
				if(m_fBkImageHorLayout)
				{
					RECT rc = { 0, 0, bm.bmWidth / 8 - 1, bm.bmHeight - 1 };
					for(int i = 0; i < 8; i++)
					{
						SetPixel(hdcBackground, rc.left, rc.top, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.right, rc.top, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.left, rc.bottom, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.right, rc.bottom, m_rgbBkTransparent);
						OffsetRect(&rc, bm.bmWidth / 8, 0);
					}
				}
				else
				{
					RECT rc = { 0, 0, bm.bmWidth - 1, bm.bmHeight / 8 - 1 };
					for(int i = 0; i < 8; i++)
					{
						SetPixel(hdcBackground, rc.left, rc.top, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.right, rc.top, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.left, rc.bottom, m_rgbBkTransparent);
						SetPixel(hdcBackground, rc.right, rc.bottom, m_rgbBkTransparent);
						OffsetRect(&rc, 0, bm.bmHeight / 8);
					}
				}

				SelectObject(hdcBackground, hbmpOld);
				DeleteDC(hdcBackground);
			}
		}

		{
			// glyph color
			int nGlyphType;
			if(!m_Theme.GetThemeEnumValue(nPartID, 0, TMT_GLYPHTYPE, &nGlyphType))
				break;

			if(nGlyphType != GT_IMAGEGLYPH)
				break;

			COLORREF rgbTransGlyph = RGB(255, 0, 255);
			if(!m_Theme.GetThemeColor(nPartID, 0, TMT_GLYPHTRANSPARENTCOLOR, &rgbTransGlyph))
				break;

			if(!m_Theme.GetThemeBitmap(nPartID, 0, nGlyphPropID, &hbmpGlyph))
				break;

			hbmpGlyphOld = SelectObject(hdcGlyph, hbmpGlyph);

			m_rgbGlyph[0] = rgbTransGlyph;
			m_rgbGlyph[1] = rgbTransGlyph;
			m_rgbGlyph[2] = rgbTransGlyph;
			m_rgbGlyph[3] = rgbTransGlyph;
			m_rgbGlyph[4] = rgbTransGlyph;
			m_rgbGlyph[5] = rgbTransGlyph;
			m_rgbGlyph[6] = rgbTransGlyph;
			m_rgbGlyph[7] = rgbTransGlyph;

			BITMAP bm;
			GetObject(hbmpGlyph, sizeof(BITMAP), &bm);
			
			COLORGLYPH st;
			st.rgbTransGlyph = rgbTransGlyph;
			st.nWidth = bm.bmWidth;
			st.nHeight = bm.bmHeight;
			st.hdc = hdcGlyph;

			BOOL bColorGotton = TRUE;

#define BreakLoopIfFailed(e)	if(!(bColorGotton = (e))) break; else
// #define BreakLoopIfFailed(e)	(e)

			switch(m_nBkStyle)
			{
			case CBNBKGNDSTYLE_CAPTION:
			case CBNBKGNDSTYLE_CLOSE:
				{
					if(m_fBkImageHorLayout)
					{
						for(int i = 0; i < bm.bmWidth; i++)
						{
							if(i < bm.bmWidth / 8)
								BreakLoopIfFailed(GetGlyphColorHor(i, 0, bm.bmWidth / 8, 0, st));
							else if(i < bm.bmWidth / 4)
								BreakLoopIfFailed(GetGlyphColorHor(i, bm.bmWidth / 8, bm.bmWidth / 4, 1, st));
							else if(i < 3 * bm.bmWidth / 8)
								BreakLoopIfFailed(GetGlyphColorHor(i, bm.bmWidth / 4, 3 * bm.bmWidth / 8, 2, st));
							else if(i < bm.bmWidth / 2)
								BreakLoopIfFailed(GetGlyphColorHor(i, 3 * bm.bmWidth / 8, bm.bmWidth / 2, 3, st));
							else if(i < 5 * bm.bmWidth / 8)
								BreakLoopIfFailed(GetGlyphColorHor(i, bm.bmWidth / 2, 5 * bm.bmWidth / 8, 4, st));
							else if(i < 6 * bm.bmWidth / 8)
								BreakLoopIfFailed(GetGlyphColorHor(i, 5 * bm.bmWidth / 8, 6 * bm.bmWidth / 8, 5, st));
							else if(i < 7 * bm.bmWidth / 8)
								BreakLoopIfFailed(GetGlyphColorHor(i, 6 * bm.bmWidth / 8, 7 * bm.bmWidth / 8, 6, st));
							else if(i < bm.bmWidth)
								BreakLoopIfFailed(GetGlyphColorHor(i, 7 * bm.bmWidth / 8, bm.bmWidth, 7, st));
						}

						if(!bColorGotton)
							break;
					}
					else
					{
						for(int i = 0; i < bm.bmHeight; i++)
						{
							if(i < bm.bmHeight / 8)
								BreakLoopIfFailed(GetGlyphColorVer(i, 0, bm.bmHeight / 8, 0, st));
							else if(i < bm.bmHeight / 4)
								BreakLoopIfFailed(GetGlyphColorVer(i, bm.bmHeight / 8, bm.bmHeight / 4, 1, st));
							else if(i < 3 * bm.bmHeight / 8)
								BreakLoopIfFailed(GetGlyphColorVer(i, bm.bmHeight / 4, 3 * bm.bmHeight / 8, 2, st));
							else if(i < bm.bmHeight / 2)
								BreakLoopIfFailed(GetGlyphColorVer(i, 3 * bm.bmHeight / 8, bm.bmHeight / 2, 3, st));
							else if(i < 5 * bm.bmHeight / 8)
								BreakLoopIfFailed(GetGlyphColorVer(i, bm.bmHeight / 2, 5 * bm.bmHeight / 8, 4, st));
							else if(i < 6 * bm.bmHeight / 8)
								BreakLoopIfFailed(GetGlyphColorVer(i, 5 * bm.bmHeight / 8, 6 * bm.bmHeight / 8, 5, st));
							else if(i < 7 * bm.bmHeight / 8)
								BreakLoopIfFailed(GetGlyphColorVer(i, 6 * bm.bmHeight / 8, 7 * bm.bmHeight / 8, 6, st));
							else if(i < bm.bmHeight)
								BreakLoopIfFailed(GetGlyphColorVer(i, 7 * bm.bmHeight / 8, bm.bmHeight, 7, st));
						}
						
						if(!bColorGotton)
							break;
					}
				}
				break;
			default:
				{
					if(m_fBkImageHorLayout)
					{
						int nBnWidth = bm.bmWidth / m_nImageCount;
						int nBnOffset = nBnWidth * m_nImageOffset;
						for(int i = nBnOffset; i < nBnOffset + 4 * nBnWidth; i++)
						{
							if(i < nBnOffset + nBnWidth)
								BreakLoopIfFailed(GetGlyphColorHor(i, nBnOffset, nBnOffset + nBnWidth, 0, st));
							else if(i < nBnOffset + 2 * nBnWidth)
								BreakLoopIfFailed(GetGlyphColorHor(i, nBnOffset + nBnWidth, nBnOffset + 2 * nBnWidth, 1, st));
							else if(i < nBnOffset + 3 * nBnWidth)
								BreakLoopIfFailed(GetGlyphColorHor(i, nBnOffset + 2 * nBnWidth, nBnOffset + 3 * nBnWidth, 2, st));
							else if(i < nBnOffset + 4 * nBnWidth)
								BreakLoopIfFailed(GetGlyphColorHor(i, nBnOffset + 3 * nBnWidth, nBnOffset + 4 * nBnWidth, 3, st));
						}
						
						if(!bColorGotton)
							break;
					}
					else
					{
						int nBnHeight = bm.bmHeight / m_nImageCount;
						int nBnOffset = nBnHeight* m_nImageOffset;
						for(int i = nBnOffset; i < nBnOffset + 4 * nBnHeight; i++)
						{
							if(i < nBnOffset + nBnHeight)
								BreakLoopIfFailed(GetGlyphColorVer(i, nBnOffset, nBnOffset + nBnHeight, 0, st));
							else if(i < nBnOffset + 2 * nBnHeight)
								BreakLoopIfFailed(GetGlyphColorVer(i, nBnOffset + nBnHeight, nBnOffset + 2 * nBnHeight, 1, st));
							else if(i < nBnOffset + 3 * nBnHeight)
								BreakLoopIfFailed(GetGlyphColorVer(i, nBnOffset + 2 * nBnHeight, nBnOffset + 3 * nBnHeight, 2, st));
							else if(i < nBnOffset + 4 * nBnHeight)
								BreakLoopIfFailed(GetGlyphColorVer(i, nBnOffset + 3 * nBnHeight, nBnOffset + 4 * nBnHeight, 3, st));
						}

						if(!bColorGotton)
							break;
					}
				}
				break;
			}

#undef BreakLoopIfFailed

			if(!bColorGotton)
				break;

			SelectObject(hdcGlyph, hbmpGlyphOld);
			hbmpGlyphOld = NULL;

			DeleteObject(hbmpGlyph);
			hbmpGlyph = NULL;
		}

		bSucceeded = TRUE;
		break;
	}

	if(!bSucceeded)
	{
		DeleteObject(m_hbmpBackground);
		m_hbmpBackground = NULL;

		if(hbmpGlyphOld)
			SelectObject(hdcGlyph, hbmpGlyphOld);

		DeleteObject(hbmpGlyph);
		hbmpGlyph = NULL;
	}

	DeleteDC(hdcGlyph);

	return 0;
}

void CCaptionButton::DrawNonClientClassic(HWND hwnd)
{
	LONG dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	LONG dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	SIZE sizeIcon = { GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON) };
	SIZE sizeFrame = { GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME) };

	if(dwStyle & WS_THICKFRAME)
	{
		sizeFrame.cx = GetSystemMetrics(SM_CXFRAME);
		sizeFrame.cy = GetSystemMetrics(SM_CYFRAME);
	}

	HFONT hfnt = NULL;
	int nCY, nPartID, nBtnsWidth;
	if(RecalcLayout(hwnd, nCY, nPartID, &hfnt, nBtnsWidth) == -1)
		return;

	RECT rcWnd;
	GetWindowRect(hwnd, &rcWnd);

	RECT rcCaption = { 0, 0, RectWidth(rcWnd), nCY - 1 + sizeFrame.cy };
	InflateRect(&rcCaption, -sizeFrame.cx, 0);
	rcCaption.top += sizeFrame.cy;

	HRGN hrgn = CreateRectRgnIndirect(&rcWnd);
	HRGN hrgn2 = CreateRectRgn(rcWnd.left + rcCaption.left, rcWnd.top + rcCaption.top, rcWnd.left + rcCaption.right, rcWnd.top + rcCaption.bottom);
	CombineRgn(hrgn, hrgn, hrgn2, RGN_DIFF);
	DeleteObject(hrgn2);
	DefWindowProc(hwnd, WM_NCPAINT, (WPARAM)hrgn, 0);
	DeleteObject(hrgn);

	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	POINT ptOffset = { rcCaption.left, rcCaption.top };
	OffsetRect(&rcCaption, -ptOffset.x, -ptOffset.y);

	HBRUSH hbrCaption = GetSysColorBrush(m_fActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION);
	COLORREF rgbFrom = ::GetSysColor(m_fActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION);
	COLORREF rgbTo = ::GetSysColor(m_fActive ? COLOR_GRADIENTACTIVECAPTION : COLOR_GRADIENTINACTIVECAPTION);

	HDC hdc = GetWindowDC(hwnd);

	// Draw the caption
	HDC hdcMem = CreateCompatibleDC(hdc);

	if(m_hbmpCaption)
	{
		BITMAP bm;
		GetObject(m_hbmpCaption, sizeof(BITMAP), &bm);
		if(bm.bmWidth != RectWidth(rcCaption) || bm.bmHeight != RectHeight(rcCaption))
		{
			DeleteObject(m_hbmpCaption);
			m_hbmpCaption = NULL;
		}
	}

	if(!m_hbmpCaption)
	{
		m_hbmpCaption = CreateCompatibleBitmap(hdc, RectWidth(rcCaption), RectHeight(rcCaption));
		if(!m_hbmpCaption)
		{
			DeleteDC(hdcMem);
			ReleaseDC(hwnd, hdc);
			return;
		}
	}

	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, m_hbmpCaption);

	BOOL fRet = FALSE;
	::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &fRet, 0);

	if(fRet)
	{
		TRIVERTEX vert[2];
		vert[0].x = rcCaption.left;
		vert[0].y = rcCaption.top;
		vert[0].Red = GetRValue(rgbFrom) << 8;
		vert[0].Green = GetGValue(rgbFrom) << 8;
		vert[0].Blue = GetBValue(rgbFrom) << 8;
		vert[0].Alpha = 0x0000;

		vert[1].x = rcCaption.right; // - nBtnsWidth;	// Modified by dandy
		vert[1].y = rcCaption.bottom; 
		vert[1].Red = GetRValue(rgbTo) << 8;
		vert[1].Green = GetGValue(rgbTo) << 8;
		vert[1].Blue = GetBValue(rgbTo) << 8;
		vert[1].Alpha = 0x0000;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		m_Theme.GradientFill(hdcMem, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

		// Commented by dandy
		// FillSolidRect(hdcMem, rcCaption.right - nBtnsWidth, rcCaption.top, nBtnsWidth, RectHeight(rcCaption), rgbTo);
	}
	else
		FillRect(hdcMem, &rcCaption, hbrCaption);

	// Draw the system menu.
	if(dwStyle & WS_SYSMENU)
	{
		HICON hIcon = GetWindowSystemMenuIcon(hwnd);
		if(hIcon != NULL)
			::DrawIconEx(hdcMem, 2, (RectHeight(rcCaption) - sizeIcon.cy) / 2, hIcon, sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);
	}

	// Draw the buttons
	for(int i = 0; i < m_aButtons.GetSize(); i++)
	{
		if(!IsRectEmpty(&m_aButtons[i]->m_rect))
		{
			DWORD dwState = 0;
			if(m_aButtons[i]->m_nState == CBNST_DISABLED)
				dwState = DFCS_INACTIVE;
			else if(m_aButtons[i]->m_nState == CBNST_PRESSED)
				dwState = DFCS_PUSHED;

			switch(i)
			{
			case 0:
				DrawFrameControl(hdcMem, &m_aButtons[i]->m_rect, DFC_CAPTION, DFCS_CAPTIONCLOSE | dwState);
				break;
			case 1:
				DrawFrameControl(hdcMem, &m_aButtons[i]->m_rect, DFC_CAPTION, (IsZoomed(hwnd) ? DFCS_CAPTIONRESTORE : DFCS_CAPTIONMAX) | dwState);
				break;
			case 2:
				DrawFrameControl(hdcMem, &m_aButtons[i]->m_rect, DFC_CAPTION, (IsIconic(hwnd) ? DFCS_CAPTIONRESTORE : DFCS_CAPTIONMIN) | dwState);
				break;
			case 3:
				DrawFrameControl(hdcMem, &m_aButtons[i]->m_rect, DFC_CAPTION, DFCS_CAPTIONHELP | dwState);
				break;
			default:
				m_aButtons[i]->Draw(hdcMem);
				break;
			}
		}
	}

	// Draw the caption text
	TCHAR szTitle[512];
	GetWindowText(hwnd, szTitle, 512);

	HGDIOBJ hfntOld = SelectObject(hdcMem, hfnt);
	COLORREF rgbOldTxtColor = SetTextColor(hdcMem, GetSysColor(m_fActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT));
	int nOldBkMode = SetBkMode(hdcMem, TRANSPARENT);

	UINT uFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
	if(dwExStyle & WS_EX_RIGHT)
		uFormat |= DT_RIGHT;

	int xLeft = rcCaption.left + 4 + (dwStyle & WS_SYSMENU ? sizeIcon.cx : 0);
	RECT rc = { xLeft, 0, rcCaption.right - nBtnsWidth - 2, rcCaption.bottom };
	DrawText(hdcMem, szTitle, -1, &rc, uFormat);

	SetBkMode(hdcMem, nOldBkMode);
	SetTextColor(hdcMem, rgbOldTxtColor);
	SelectObject(hdcMem, hfntOld);

	DeleteObject(hfnt);

	BitBlt(hdc, ptOffset.x, ptOffset.y, RectWidth(rcCaption), RectHeight(rcCaption), hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);

	for(i = 0; i < m_aButtons.GetSize(); i++)
	{
		if(!IsRectEmpty(&m_aButtons[i]->m_rect))
			OffsetRect(&m_aButtons[i]->m_rect, sizeFrame.cx, sizeFrame.cy);
	}
}

void CCaptionButton::DrawNonClientModern(HWND hwnd)
{
	LONG dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	LONG dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	RECT rcWnd;
	GetWindowRect(hwnd, &rcWnd);

	SIZE sizeIcon = { GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON) };
	SIZE sizeFrame = { GetSystemMetrics(SM_CXDLGFRAME), ::GetSystemMetrics(SM_CYDLGFRAME) };

	if(dwStyle & WS_THICKFRAME)
	{
		sizeFrame.cx = GetSystemMetrics(SM_CXFRAME);
		sizeFrame.cy = GetSystemMetrics(SM_CYFRAME);
	}

	HFONT hfnt = NULL;
	int nCY, nPartID, nBtnsWidth;
	if(RecalcLayout(hwnd, nCY, nPartID, &hfnt, nBtnsWidth) == -1)
		return;

	int nFrameState = FS_INACTIVE;
	if(m_fActive)
		nFrameState = FS_ACTIVE;

	RECT rcCaption = { 0, 0, RectWidth(rcWnd), nCY + sizeFrame.cy };

	RECT rcDef = { rcWnd.left + sizeFrame.cx, rcWnd.top + RectHeight(rcCaption), rcWnd.right - sizeFrame.cx, rcWnd.bottom - sizeFrame.cy };
	HRGN hrgn = CreateRectRgnIndirect(&rcDef);
	DefWindowProc(hwnd, WM_NCPAINT, (WPARAM)hrgn, 0);
	DeleteObject(hrgn);

	HDC hdc = GetWindowDC(hwnd);

	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	RECT rcLeftFrm = { rcWnd.left, rcWnd.top + RectHeight(rcCaption), rcWnd.left + sizeFrame.cx, rcWnd.bottom };
	RECT rcRightFrm = { rcWnd.right - sizeFrame.cx, rcWnd.top + RectHeight(rcCaption), rcWnd.right, rcWnd.bottom };
	RECT rcBottomFrm = { rcWnd.left, rcWnd.bottom - sizeFrame.cy, rcWnd.right, rcWnd.bottom };

	if(dwExStyle & WS_EX_TOOLWINDOW)
	{
		m_Theme.DrawThemePart(hdc, WP_SMALLFRAMELEFT, nFrameState, &rcLeftFrm);
		m_Theme.DrawThemePart(hdc, WP_SMALLFRAMERIGHT, nFrameState, &rcRightFrm);
		m_Theme.DrawThemePart(hdc, WP_SMALLFRAMEBOTTOM, nFrameState, &rcBottomFrm);
	}
	else
	{
		m_Theme.DrawThemePart(hdc, WP_FRAMELEFT, nFrameState, &rcLeftFrm);
		m_Theme.DrawThemePart(hdc, WP_FRAMERIGHT, nFrameState, &rcRightFrm);
		m_Theme.DrawThemePart(hdc, WP_FRAMEBOTTOM, nFrameState, &rcBottomFrm);
	}

	// Draw the caption
	HDC hdcMem = CreateCompatibleDC(hdc);

	if(m_hbmpCaption)
	{
		BITMAP bm;
		GetObject(m_hbmpCaption, sizeof(BITMAP), &bm);
		if(bm.bmWidth != RectWidth(rcCaption) || bm.bmHeight != RectHeight(rcCaption))
		{
			DeleteObject(m_hbmpCaption);
			m_hbmpCaption = NULL;
		}
	}

	if(!m_hbmpCaption)
	{
		m_hbmpCaption = CreateCompatibleBitmap(hdc, RectWidth(rcCaption), RectHeight(rcCaption));
		if(!m_hbmpCaption)
		{
			DeleteDC(hdcMem);
			ReleaseDC(hwnd, hdc);
			return;
		}
	}

	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, m_hbmpCaption);

	m_Theme.DrawThemePart(hdcMem, nPartID, nFrameState, &rcCaption);

	// Draw the system menu.
	if(dwStyle & WS_SYSMENU)
	{
		HICON hIcon = GetWindowSystemMenuIcon(hwnd);
		if(hIcon != NULL)
			::DrawIconEx(hdcMem, 2 + sizeFrame.cx, sizeFrame.cy - 1 + (RectHeight(rcCaption) - sizeIcon.cy - sizeFrame.cy) / 2, hIcon, sizeIcon.cx,
			sizeIcon.cy, 0, NULL, DI_NORMAL);
	}

	// Draw the buttons
	for(int i = 0; i < m_aButtons.GetSize(); i++)
	{
		if(!IsRectEmpty(&m_aButtons[i]->m_rect))
		{
			int nStateOffset = 4;
			if(m_aButtons[i]->m_fActive)
				nStateOffset = 0;
			switch(i)
			{
			case 0:
				m_Theme.DrawThemePart(hdcMem, WP_CLOSEBUTTON, m_aButtons[i]->m_nState + nStateOffset, &m_aButtons[i]->m_rect);
				break;
			case 1:
				m_Theme.DrawThemePart(hdcMem, IsZoomed(hwnd) ? WP_RESTOREBUTTON : WP_MAXBUTTON, m_aButtons[i]->m_nState + nStateOffset, &m_aButtons[i]->m_rect);
				break;
			case 2:
				m_Theme.DrawThemePart(hdcMem, IsIconic(hwnd) ? WP_RESTOREBUTTON : WP_MINBUTTON, m_aButtons[i]->m_nState + nStateOffset, &m_aButtons[i]->m_rect);
				break;
			case 3:
				m_Theme.DrawThemePart(hdcMem, WP_HELPBUTTON, m_aButtons[i]->m_nState + nStateOffset, &m_aButtons[i]->m_rect);
				break;
			default:
				m_aButtons[i]->Draw(hdcMem);
				break;
			}
		}
	}

	// Draw the caption text

	TCHAR szTitle[512];
	GetWindowText(hwnd, szTitle, 512);

	int nTextXPos = sizeFrame.cx + (dwStyle & WS_SYSMENU ? sizeIcon.cx + 6 : 4);

	POINT ptShadow = { 0, 0 };
	m_Theme.GetThemePosition(nPartID, nFrameState, TMT_TEXTSHADOWOFFSET, &ptShadow);

	COLORREF rgbTxtShadowColor;
	COLORREF rgbTxtColor = ::GetSysColor(m_fActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT);
	m_Theme.GetThemeColor(nPartID, nFrameState, TMT_TEXTSHADOWCOLOR, &rgbTxtShadowColor);
	m_Theme.GetThemeColor(nPartID, nFrameState, TMT_CAPTIONTEXT, &rgbTxtColor);

	HGDIOBJ hfntOld = SelectObject(hdcMem, hfnt);
	COLORREF rgbOldTxtColor = SetTextColor(hdcMem, rgbTxtShadowColor);
	int nOldBkMode = SetBkMode(hdcMem, TRANSPARENT);

	RECT rcText = { nTextXPos + ptShadow.x, sizeFrame.cy + ptShadow.y, rcCaption.right - nBtnsWidth - 2, RectHeight(rcCaption) + ptShadow.y };
	UINT uFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
	if(dwExStyle & WS_EX_RIGHT)
		uFormat |= DT_RIGHT;

	if(ptShadow.x || ptShadow.y)
		DrawText(hdcMem, szTitle, -1, &rcText, uFormat);

	SetTextColor(hdcMem, rgbTxtColor);
	OffsetRect(&rcText, -ptShadow.x, -ptShadow.y);
	DrawText(hdcMem, szTitle, -1, &rcText, uFormat);

	SetBkMode(hdcMem, nOldBkMode);
	SetTextColor(hdcMem, rgbOldTxtColor);
	SelectObject(hdcMem, hfntOld);

	DeleteObject(hfnt);

	BitBlt(hdc, 0, 0, RectWidth(rcCaption), RectHeight(rcCaption), hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);
}

void CCaptionButton::Draw(HDC hdc)
{
	if(!m_hbmpBackground)
	{
		m_rgbGlyph[0] = ::GetSysColor(COLOR_BTNTEXT);
		m_rgbGlyph[1] = m_rgbGlyph[0];
		m_rgbGlyph[2] = m_rgbGlyph[0];
		m_rgbGlyph[3] = ::GetSysColor(COLOR_GRAYTEXT);
		m_rgbGlyph[4] = m_rgbGlyph[0];
		m_rgbGlyph[5] = m_rgbGlyph[0];
		m_rgbGlyph[6] = m_rgbGlyph[0];
		m_rgbGlyph[7] = m_rgbGlyph[3];
	}

	RECT rc = m_rect;
	POINT pt = { rc.left, rc.top };
	OffsetRect(&rc, -pt.x, -pt.y);

	HDC hdcMem = CreateCompatibleDC(hdc);
	if(!hdcMem)
		return;

	if(m_hbmpButton)
	{
		BITMAP bm;
		GetObject(m_hbmpButton, sizeof(BITMAP), &bm);
		if(bm.bmWidth != RectWidth(rc) || bm.bmHeight != RectHeight(rc))
		{
			DeleteObject(m_hbmpButton);
			m_hbmpButton = NULL;
		}
	}

	if(!m_hbmpButton)
	{
		m_hbmpButton = CreateCompatibleBitmap(hdc, RectWidth(rc), RectHeight(rc));
		if(!m_hbmpButton)
		{
			DeleteDC(hdcMem);
			return;
		}
	}

	DWORD dwState = 0;
	if(IsEnabled())
	{
		if(m_nState == CBNST_DISABLED)
			m_nState = CBNST_NORMAL;

		if(m_nState == CBNST_PRESSED)
			dwState = DFCS_PUSHED;
	}
	else
	{
		dwState = DFCS_INACTIVE;
		m_nState = CBNST_DISABLED;
	}
	
	int nImageOffset = 0;
	if(m_nBkStyle >= CBNBKGNDSTYLE_CAPTION && m_nBkStyle <= CBNBKGNDSTYLE_CLOSE && !m_fActive)
		nImageOffset = 4;
	
	int nState = m_nState - 1;
	
	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, m_hbmpButton);

	if(m_hbmpBackground)
		DrawBackground(hdcMem, rc, nState + nImageOffset + m_nImageOffset);
	else
		DrawFrameControl(hdcMem, &rc, DFC_BUTTON, DFCS_BUTTONPUSH | dwState);

	DrawGlyph(hdcMem, rc, nState + nImageOffset);

	if(m_hbmpBackground && m_fBkTransparent)
		m_Theme.TransparentBlt(hdc, pt.x, pt.y, RectWidth(rc), RectHeight(rc), hdcMem, 0, 0, RectWidth(rc), RectHeight(rc), m_rgbBkTransparent);
	else
		BitBlt(hdc, pt.x, pt.y, RectWidth(rc), RectHeight(rc), hdcMem, rc.left, rc.top, SRCCOPY);

	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);
}

void CCaptionButton::DrawBackground(HDC hdc, RECT& rc, int nImageIndex)
{
	MY_MARGINS m =
	{
		m_mrgnBk.cxLeftWidth,
		m_mrgnBk.cxRightWidth,
		m_mrgnBk.cyBottomHeight,
		m_mrgnBk.cyTopHeight
	};

	int iWidth = RectWidth(rc);
	int iHeight = RectHeight(rc);

	if((m.cxLeftWidth + m.cxRightWidth > iWidth) || (m.cyTopHeight + m.cyBottomHeight > iHeight))
		return;	// the rectangle too small

	if(2 * (m.cxLeftWidth + m.cxRightWidth) > iWidth)
	{
		int nDelta = iWidth - (m.cxLeftWidth + m.cxRightWidth);
		m.cxLeftWidth += nDelta / 2;
		m.cxRightWidth += nDelta / 2 + nDelta % 2;
	}

	if(2 * (m.cyTopHeight + m.cyBottomHeight) > iHeight)
	{
		int nDelta = iHeight - (m.cyTopHeight + m.cyBottomHeight);
		m.cyTopHeight += nDelta / 2;
		m.cyBottomHeight += nDelta / 2 + nDelta % 2;
	}

	BITMAP bm;
	GetObject(m_hbmpBackground, sizeof(BITMAP), &bm);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ hbmpMemOld = SelectObject(hdcMem, m_hbmpBackground);
	if(m_fBkImageHorLayout)
	{
		// left-top
		BitBlt(hdc, rc.left, rc.top, m.cxLeftWidth, m.cyTopHeight, hdcMem, nImageIndex * bm.bmWidth / m_nImageCount, 0, SRCCOPY);

		// right-top
		BitBlt(hdc, rc.right - m.cxRightWidth, rc.top, m.cxRightWidth, m.cyTopHeight, hdcMem, (nImageIndex + 1) * bm.bmWidth / m_nImageCount - m.cxRightWidth, 0, SRCCOPY);

		// left-bottom
		BitBlt(hdc, rc.left, rc.bottom - m.cyBottomHeight, m.cxLeftWidth, m.cyBottomHeight, hdcMem, nImageIndex * bm.bmWidth / m_nImageCount, bm.bmHeight - m.cyBottomHeight, SRCCOPY);

		// right-bottom
		BitBlt(hdc, rc.right - m.cxRightWidth, rc.bottom - m.cyBottomHeight, m.cxRightWidth, m.cyBottomHeight, hdcMem,
			(nImageIndex + 1) * bm.bmWidth / m_nImageCount - m.cxRightWidth, bm.bmHeight - m.cyBottomHeight, SRCCOPY);

		// middle-top
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.top, iWidth - m.cxLeftWidth - m.cxRightWidth, m.cyTopHeight, hdcMem,
			nImageIndex * bm.bmWidth / m_nImageCount + m.cxLeftWidth, 0, bm.bmWidth / m_nImageCount - m.cxLeftWidth - m.cxRightWidth, m.cyTopHeight, SRCCOPY);

		// middle-bottom
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.bottom - m.cyBottomHeight, iWidth - m.cxLeftWidth - m.cxRightWidth, m.cyBottomHeight, hdcMem,
			nImageIndex * bm.bmWidth / m_nImageCount + m.cxLeftWidth, bm.bmHeight - m.cyBottomHeight, bm.bmWidth / m_nImageCount - m.cxLeftWidth - m.cxRightWidth,
			m.cyBottomHeight, SRCCOPY);

		// middle-left
		StretchBlt(hdc, rc.left, rc.top + m.cyTopHeight, m.cxLeftWidth, iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem,
			nImageIndex * bm.bmWidth / m_nImageCount, m.cyTopHeight, m.cxLeftWidth, bm.bmHeight - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);

		// middle-right
		StretchBlt(hdc, rc.right - m.cxRightWidth, rc.top + m.cyTopHeight, m.cxRightWidth, iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem,
			(nImageIndex + 1) * bm.bmWidth / m_nImageCount - m.cxRightWidth, m.cyTopHeight, m.cxRightWidth, bm.bmHeight - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);

		// middle
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.top + m.cyTopHeight, iWidth - m.cxLeftWidth - m.cxRightWidth, iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem,
			nImageIndex * bm.bmWidth / m_nImageCount + m.cxLeftWidth, m.cyTopHeight, bm.bmWidth / m_nImageCount - m.cxLeftWidth - m.cxRightWidth, bm.bmHeight - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);
	}
	else
	{
		// left-top
		BitBlt(hdc, rc.left, rc.top, m.cxLeftWidth, m.cyTopHeight, hdcMem, 0, nImageIndex * bm.bmHeight / m_nImageCount, SRCCOPY);

		// right-top
		BitBlt(hdc, rc.right - m.cxRightWidth, rc.top, m.cxRightWidth, m.cyTopHeight, hdcMem, bm.bmWidth - m.cxRightWidth,
			nImageIndex * bm.bmHeight / m_nImageCount, SRCCOPY);

		// left-bottom
		BitBlt(hdc, rc.left, rc.bottom - m.cyBottomHeight, m.cxLeftWidth, m.cyBottomHeight, hdcMem, 0,
			(nImageIndex + 1) * bm.bmHeight / m_nImageCount - m.cyBottomHeight, SRCCOPY);

		// right-bottom
		BitBlt(hdc, rc.right - m.cxRightWidth, rc.bottom - m.cyBottomHeight, m.cxRightWidth, m.cyBottomHeight, hdcMem, bm.bmWidth - m.cxRightWidth,
			(nImageIndex + 1) * bm.bmHeight / m_nImageCount - m.cyBottomHeight, SRCCOPY);

		// middle-top
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.top, iWidth - m.cxLeftWidth - m.cxRightWidth, m.cyTopHeight, hdcMem, m.cxLeftWidth,
			nImageIndex * bm.bmHeight / m_nImageCount, bm.bmWidth - m.cxLeftWidth - m.cxRightWidth, m.cyTopHeight, SRCCOPY);

		// middle-bottom
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.bottom - m.cyBottomHeight, iWidth - m.cxLeftWidth - m.cxRightWidth, m.cyBottomHeight, hdcMem,
			m.cxLeftWidth, (nImageIndex + 1) * bm.bmHeight / m_nImageCount - m.cyBottomHeight, bm.bmWidth - m.cxLeftWidth - m.cxRightWidth, m.cyBottomHeight,
			SRCCOPY);

		// middle-left
		StretchBlt(hdc, rc.left, rc.top + m.cyTopHeight, m.cxLeftWidth, iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem, 0,
			nImageIndex * bm.bmHeight / m_nImageCount + m.cyTopHeight, m.cxLeftWidth, bm.bmHeight / m_nImageCount - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);

		// middle-right
		StretchBlt(hdc, rc.right - m.cxRightWidth, rc.top + m.cyTopHeight, m.cxRightWidth, iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem,
			bm.bmWidth - m.cxRightWidth, nImageIndex * bm.bmHeight / m_nImageCount + m.cyTopHeight, m.cxRightWidth,
			bm.bmHeight / m_nImageCount - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);

		// middle
		StretchBlt(hdc, rc.left + m.cxLeftWidth, rc.top + m.cyTopHeight, iWidth - m.cxLeftWidth - m.cxRightWidth,
			iHeight - m.cyTopHeight - m.cyBottomHeight, hdcMem, m.cxLeftWidth, nImageIndex * bm.bmHeight / m_nImageCount + m.cyTopHeight,
			bm.bmWidth - m.cxLeftWidth - m.cxRightWidth, bm.bmHeight / m_nImageCount - m.cyTopHeight - m.cyBottomHeight, SRCCOPY);
	}

	SelectObject(hdcMem, hbmpMemOld);
	DeleteDC(hdcMem);
}

void CCaptionButton::DrawGlyph(HDC hdc, RECT& rc, int nColorIndex)
{
	if(m_hbmpGlyph == NULL)
		return;

	BITMAP bm;
	GetObject(m_hbmpGlyph, sizeof(BITMAP), &bm);

	POINT pt = { rc.left + (RectWidth(rc) - bm.bmWidth) / 2, rc.top + (RectHeight(rc) - bm.bmHeight) / 2 };

	// Added by dandy
	if(m_nState == CBNST_PRESSED && !m_Theme.IsThemeActive())
	{
		pt.x++;
		pt.y++;
	}

	HDC hdcMem = CreateCompatibleDC(hdc);
	HDC hdcMemMono = CreateCompatibleDC(hdc);
	HGDIOBJ hbmpGlyphMonoOld = SelectObject(hdcMemMono, m_hbmpGlyph);

	HBITMAP hbmpGlyphColor = CreateCompatibleBitmap(hdc, bm.bmWidth, bm.bmHeight);
	HGDIOBJ hbmpGlyphColorOld = SelectObject(hdcMem, hbmpGlyphColor);

	COLORREF rgbOldTextGlyph = SetTextColor(hdcMem, m_rgbGlyph[nColorIndex]);
	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemMono, 0, 0, SRCCOPY);
	SetTextColor(hdcMem, rgbOldTextGlyph);

	COLORREF rgbOldBk = SetBkColor(hdc, RGB(255, 255, 255));
	COLORREF rgbOldText = SetTextColor(hdc, RGB(0, 0, 0));
	BitBlt(hdc, pt.x, pt.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCINVERT);
	BitBlt(hdc, pt.x, pt.y, bm.bmWidth, bm.bmHeight, hdcMemMono, 0, 0, SRCAND);
	BitBlt(hdc, pt.x, pt.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCINVERT);
	SetBkColor(hdc, rgbOldBk);
	SetTextColor(hdc, rgbOldText);

	SelectObject(hdcMem, hbmpGlyphColorOld);
	SelectObject(hdcMemMono, hbmpGlyphMonoOld);

	DeleteObject(hbmpGlyphColor);

	DeleteDC(hdcMemMono);
	DeleteDC(hdcMem);
}

BOOL CCaptionButton::GetGlyphColorHor(int i, int iMin, int iMax, int iIndex, COLORGLYPH& st)
{
	if(i == iMin)
	{
		for(int k = 0; k < 4; k++)
		{
			st.a[k].rgbGlyph = st.rgbTransGlyph;
			st.a[k].nCnt = 0;
		}
	}

	for(int j = 0; j < st.nHeight; j++)
	{
		if(GetPixel(st.hdc, i, j) != st.rgbTransGlyph)
		{
			for(int k = 0; k < 4; k++)
			{
				if(st.a[k].nCnt == 0)
				{
					st.a[k].rgbGlyph = GetPixel(st.hdc, i, j);
					st.a[k].nCnt = 1;
					break;
				}
				else if(st.a[k].rgbGlyph == GetPixel(st.hdc, i, j))
				{
					st.a[k].nCnt++;
					break;
				}
			}
		}
	}

	if(i == iMax - 1)
	{
		if(st.a[0].nCnt == 0)
			return FALSE;

		int nMaxCnt = 0;
		int nMaxNdx = 0;
		for(int k = 0; k < 4; k++)
		{
			if(st.a[k].nCnt > nMaxCnt)
			{
				nMaxCnt = st.a[k].nCnt;
				nMaxNdx = k;
			}
		}

		m_rgbGlyph[iIndex] = st.a[nMaxNdx].rgbGlyph;
	}

	return TRUE;
}

BOOL CCaptionButton::GetGlyphColorVer(int i, int iMin, int iMax, int iIndex, COLORGLYPH& st)
{
	if(i == iMin)
	{
		for(int k = 0; k < 4; k++)
		{
			st.a[k].rgbGlyph = st.rgbTransGlyph;
			st.a[k].nCnt = 0;
		}
	}

	for(int j = 0; j < st.nWidth; j++)
	{
		if(GetPixel(st.hdc, j, i) != st.rgbTransGlyph)
		{
			for(int k = 0; k < 4; k++)
			{
				if(st.a[k].nCnt == 0)
				{
					st.a[k].rgbGlyph = GetPixel(st.hdc, j, i);
					st.a[k].nCnt = 1;
					break;
				}
				else if(st.a[k].rgbGlyph == GetPixel(st.hdc, j, i))
				{
					st.a[k].nCnt++;
					break;
				}
			}
		}
	}

	if(i == iMax - 1)
	{
		if(st.a[0].nCnt == 0)
			return FALSE;

		int nMaxCnt = 0;
		int nMaxNdx = 0;
		for(int k = 0; k < 4; k++)
		{
			if(st.a[k].nCnt > nMaxCnt)
			{
				nMaxCnt = st.a[k].nCnt;
				nMaxNdx = k;
			}
		}

		m_rgbGlyph[iIndex] = st.a[nMaxNdx].rgbGlyph;
	}

	return TRUE;
}

int CCaptionButton::RecalcLayout(HWND hwnd, int& nCY, int& nPartID, HFONT* phfnt, int& nBtnsWidth)
{
	LONG dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	LONG dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	RECT rc;
	GetWindowRect(hwnd, &rc);

	NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);	 

	SIZE sizeBtns;
	SIZE sizeIcon = { GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON) };
	SIZE sizeFrame = { GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME) };
	if(dwStyle & WS_THICKFRAME)
	{
		sizeFrame.cx = GetSystemMetrics(SM_CXFRAME);
		sizeFrame.cy = GetSystemMetrics(SM_CYFRAME);
	}

	HFONT hfnt = NULL;
	BOOL bThemeActive = m_Theme.IsThemeActive();

	if(dwExStyle & WS_EX_TOOLWINDOW)
	{
		hfnt = CreateFontIndirect(&ncm.lfSmCaptionFont);
		if(!hfnt)
			return -1;

		nCY = ::GetSystemMetrics(SM_CYSMCAPTION);
		nPartID = WP_SMALLCAPTION;
		if(bThemeActive)
		{
			sizeBtns.cx = ::GetSystemMetrics(SM_CXSMSIZE) - 4;
			sizeBtns.cy = ::GetSystemMetrics(SM_CYSMSIZE) - 4;
		}
		else
		{
			sizeBtns.cx = ::GetSystemMetrics(SM_CXSMSIZE) - 2;
			sizeBtns.cy = ::GetSystemMetrics(SM_CYSMSIZE) - 4;
		}
	}
	else
	{
		hfnt = CreateFontIndirect(&ncm.lfCaptionFont);
		if(!hfnt)
			return -1;

		nCY = ::GetSystemMetrics(SM_CYCAPTION);
		nPartID = WP_CAPTION;
		if(bThemeActive)
		{
			sizeBtns.cx = ::GetSystemMetrics(SM_CXSIZE) - 4;
			sizeBtns.cy = ::GetSystemMetrics(SM_CYSIZE) - 4;
		}
		else
		{
			sizeBtns.cx = ::GetSystemMetrics(SM_CXSIZE) - 2;
			sizeBtns.cy = ::GetSystemMetrics(SM_CYSIZE) - 4;
		}
	}

	RECT rcCaption;
	if(bThemeActive)
	{
		nBtnsWidth = sizeFrame.cx;
		SetRect(&rcCaption, 0, 0, RectWidth(rc), nCY + sizeFrame.cy);
	}
	else
	{
		SetRect(&rcCaption, 0, 0, RectWidth(rc), nCY - 1 + sizeFrame.cy);
		InflateRect(&rcCaption, -sizeFrame.cx, 0);
		rcCaption.top += sizeFrame.cy;
		nBtnsWidth = 0;
	}

	if(dwStyle & WS_SYSMENU)
	{
		SetRect(&m_aButtons[0]->m_rect, 0, 0, sizeBtns.cx, sizeBtns.cy);
		nBtnsWidth += RectWidth(m_aButtons[0]->m_rect) + 2;
		OffsetRect(&m_aButtons[0]->m_rect, RectWidth(rcCaption) - nBtnsWidth, bThemeActive ? 2 + sizeFrame.cy : 2);

		if(dwStyle & WS_MAXIMIZEBOX || dwStyle & WS_MINIMIZEBOX)
		{
			SetRect(&m_aButtons[1]->m_rect, 0, 0, sizeBtns.cx, sizeBtns.cy);
			nBtnsWidth += RectWidth(m_aButtons[1]->m_rect) + 2;
			OffsetRect(&m_aButtons[1]->m_rect, RectWidth(rcCaption) - nBtnsWidth, bThemeActive ? 2 + sizeFrame.cy : 2);

			if(!(dwStyle & WS_MAXIMIZEBOX))
				m_aButtons[1]->m_nState = CBNST_DISABLED;
			else if(m_aButtons[1]->m_nState == CBNST_DISABLED)
				m_aButtons[1]->m_nState = CBNST_NORMAL;

			SetRect(&m_aButtons[2]->m_rect, 0, 0, sizeBtns.cx, sizeBtns.cy);

			if(bThemeActive)
				nBtnsWidth += RectWidth(m_aButtons[2]->m_rect) + 2;
			else
				nBtnsWidth += RectWidth(m_aButtons[2]->m_rect);

			OffsetRect(&m_aButtons[2]->m_rect, RectWidth(rcCaption) - nBtnsWidth, bThemeActive ? 2 + sizeFrame.cy : 2);

			if(!(dwStyle & WS_MINIMIZEBOX))
				m_aButtons[2]->m_nState = CBNST_DISABLED;
			else if(m_aButtons[2]->m_nState == CBNST_DISABLED)
				m_aButtons[2]->m_nState = CBNST_NORMAL;

			SetRectEmpty(&m_aButtons[3]->m_rect);
		}
		else if(dwExStyle & WS_EX_CONTEXTHELP)
		{
			SetRectEmpty(&m_aButtons[1]->m_rect);
			SetRectEmpty(&m_aButtons[2]->m_rect);
			SetRect(&m_aButtons[3]->m_rect, 0, 0, sizeBtns.cx, sizeBtns.cy);
			nBtnsWidth += RectWidth(m_aButtons[3]->m_rect) + 2;
			OffsetRect(&m_aButtons[3]->m_rect, RectWidth(rcCaption) - nBtnsWidth, bThemeActive ? 2 + sizeFrame.cy : 2);
		}
		else
		{
			SetRectEmpty(&m_aButtons[1]->m_rect);
			SetRectEmpty(&m_aButtons[2]->m_rect);
			SetRectEmpty(&m_aButtons[3]->m_rect);
		}
	}
	else
	{
		SetRectEmpty(&m_aButtons[0]->m_rect);
		SetRectEmpty(&m_aButtons[1]->m_rect);
		SetRectEmpty(&m_aButtons[2]->m_rect);
		SetRectEmpty(&m_aButtons[3]->m_rect);
	}

	for(int i = 4; i < m_aButtons.GetSize(); i++)
	{
		if(IsIconic(hwnd))
		{
			m_aButtons[i]->m_rect.top = 0;
			m_aButtons[i]->m_rect.bottom = 0;
		}
		else
		{
			if(m_aButtons[i]->GetStyle() & CBNSTYLE_USERDEFWIDTH)
				SetRect(&m_aButtons[i]->m_rect, 0, 0, RectWidth(m_aButtons[i]->m_rect), sizeBtns.cy);
			else
				SetRect(&m_aButtons[i]->m_rect, 0, 0, sizeBtns.cx, sizeBtns.cy);

			nBtnsWidth += RectWidth(m_aButtons[i]->m_rect) + 2;
			OffsetRect(&m_aButtons[i]->m_rect, RectWidth(rcCaption) - nBtnsWidth, bThemeActive ? 2 + sizeFrame.cy : 2);
		}
	}

	HDC hdc = GetWindowDC(hwnd);
	HGDIOBJ hfntOld = SelectObject(hdc, hfnt);
	DrawText(hdc, TEXT("W..."), -1, &rc, DT_CALCRECT | DT_SINGLELINE);
	SelectObject(hdc, hfntOld);
	ReleaseDC(hwnd, hdc);

	if(phfnt)
		*phfnt = hfnt;
	else
		DeleteObject(hfnt);

	return sizeFrame.cx + (dwStyle & WS_SYSMENU ? sizeIcon.cx + 6 : 4) + RectWidth(rc) + nBtnsWidth;
}

LRESULT CALLBACK CCaptionButton::CaptionFrameWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CCaptionButton* p;
	m_cs.Lock();
	if(!m_mapCaptionFrames.Lookup(hwnd, p))
	{
		m_cs.Unlock();
		return 0;
	}
	m_cs.Unlock();

	switch(message)
	{
	case WM_SETTEXT:
		{
			RedrawWindowFrame(hwnd);
		}
		break;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;

			int nCY, nPartID, nBtnsWidth;
			int nMinWidth = p->RecalcLayout(hwnd, nCY, nPartID, NULL, nBtnsWidth);
			if(nMinWidth == -1 || nMinWidth > lpmmi->ptMinTrackSize.x)
				lpmmi->ptMinTrackSize.x = nMinWidth;
		}
		break;
	case WM_THEMECHANGED:
		{
			p->m_Theme.CloseThemeData();
			p->m_Theme.OpenThemeData(hwnd, L"WINDOW");

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
				p->m_aButtons[i]->OnThemeChanged(wParam, lParam);
		}
		break;
	case WM_ACTIVATE:
		{
			p->m_fActive = LOWORD(wParam);
			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
				p->m_aButtons[i]->m_fActive = LOWORD(wParam);

			RedrawWindowFrame(hwnd);

#ifdef USE_MDIFRAME_SUPPORT
			CWnd* pWnd = CWnd::FromHandle(hwnd);

			if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
			{
				CWnd* pActiveWnd = ((CMDIFrameWnd*)pWnd)->MDIGetActive();
				TRACE("ACTIVE %p\n", pActiveWnd);
				if(pActiveWnd != NULL)
					pActiveWnd->PostMessage(WM_NCACTIVATE, LOWORD(wParam), 0);
			}
#endif // USE_MDIFRAME_SUPPORT
			return 0;
		}
		break;
	case WM_NCACTIVATE:
		{
#ifdef USE_MDIFRAME_SUPPORT
			CWnd* pWnd = CWnd::FromHandle(hwnd);
			
			if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
			{
				p->m_fActive = wParam;
				for(int i = 0; i< p->m_aButtons.GetSize(); i++)
					p->m_aButtons[i]->m_fActive = wParam;

				RedrawWindowFrame(hwnd);
			}
#endif // USE_MDIFRAME_SUPPORT
			return TRUE;
		}
		break;
	case WM_NCPAINT:
		{
			if(p->m_Theme.IsThemeActive())
				p->DrawNonClientModern(hwnd);
			else
				p->DrawNonClientClassic(hwnd);

#ifdef USE_TOOLTIP_SUPPORT
			if(p->m_tooltip.m_hWnd)
			{
				for(int i = 4; i< p->m_aButtons.GetSize(); i++)
				{
					if(IsRectEmpty(&p->m_aButtons[i]->m_rect) || p->m_aButtons[i]->m_nState == CBNST_DISABLED)
					{
						RECT rc = { 0, 0, 0, 0 };
						p->m_tooltip.SetToolRect(hwnd, p->m_aButtons[i]->GetDlgCtrlID(), &rc);
					}
					else
					{
						RECT rc = p->m_aButtons[i]->m_rect;

						RECT rcWnd;
						GetWindowRect(hwnd, &rcWnd);

						RECT rcClient;
						GetClientRect(hwnd, &rcClient);
						MapWindowPoints(hwnd, NULL, (LPPOINT)&rcClient, 2);
						OffsetRect(&rc, rcWnd.left - rcClient.left, rcWnd.top - rcClient.top);

						p->m_tooltip.SetToolRect(hwnd, p->m_aButtons[i]->GetDlgCtrlID(), &rc);
					}
				}
			}
#endif // USE_TOOLTIP_SUPPORT
			return 0;
		}
		break;
	case WM_NCHITTEST:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					switch(i)
					{
					case 0:
						return HTCLOSE;
					case 1:
						return HTMAXBUTTON;
					case 2:
						return HTMINBUTTON;
					case 3:
						return HTHELP;
					default:
						return HTCAPTION;
					}
				}
			}
		}
		break;
	case WM_NCMOUSEMOVE:
		{
			TRACKMOUSEEVENT trackmouseevent;
			trackmouseevent.cbSize = sizeof(trackmouseevent);
			trackmouseevent.dwFlags = TME_NONCLIENT;
			trackmouseevent.hwndTrack = hwnd;
			trackmouseevent.dwHoverTime = 0;
			::TrackMouseEvent(&trackmouseevent);

			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					int nState = p->m_aButtons[i]->m_nState;
					if(p->m_aButtons[i]->m_nState != CBNST_DISABLED)
						p->m_aButtons[i]->m_nState = CBNST_HOT;

					if(p->m_aButtons[i]->m_nState != nState)
						RedrawWindowFrame(hwnd, &p->m_aButtons[i]->m_rect);

					break;
				}
			}

			if(p->m_nOverIndex != -1 && p->m_nOverIndex != i)
			{
				int nState = p->m_aButtons[p->m_nOverIndex]->m_nState;
				if(p->m_aButtons[p->m_nOverIndex]->m_nState != CBNST_DISABLED)
					p->m_aButtons[p->m_nOverIndex]->m_nState = CBNST_NORMAL;

				if(p->m_aButtons[p->m_nOverIndex]->m_nState != nState)
					RedrawWindowFrame(hwnd, &p->m_aButtons[p->m_nOverIndex]->m_rect);
			}

			if(i == p->m_aButtons.GetSize())
				p->m_nOverIndex = -1;
			else
				p->m_nOverIndex = i;

#ifdef USE_TOOLTIP_SUPPORT
			if(p->m_tooltip.m_hWnd)
			{
				// MSG msg = { hwnd, message, wParam, lParam };
				POINT pt = MAKEPOINT(lParam);
				ScreenToClient(hwnd, &pt);

				MSG msg = { hwnd, WM_MOUSEMOVE, wParam, MAKELPARAM(pt.x, pt.y) };
				p->m_tooltip.RelayEvent(&msg);
			}
#endif // USE_TOOLTIP_SUPPORT

			if(IsIconic(hwnd))
				return 0;
		}
		break;
	case WM_NCMOUSELEAVE:
		{
			if(p->m_nOverIndex != -1)
			{
				if(p->m_nHeldIndex != -1)
					break;

				if(p->m_aButtons[p->m_nOverIndex]->m_nState != CBNST_DISABLED)
					p->m_aButtons[p->m_nOverIndex]->m_nState = CBNST_NORMAL;

				RedrawWindowFrame(hwnd, &p->m_aButtons[p->m_nOverIndex]->m_rect);
				p->m_nOverIndex = -1;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };
			ClientToScreen(hwnd, &pt);

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			if(p->m_nHeldIndex != -1)
			{
				int nState = p->m_aButtons[p->m_nHeldIndex]->m_nState;
				if(p->m_aButtons[p->m_nHeldIndex]->m_nState != CBNST_DISABLED)
				{
					if(PtInRect(&p->m_aButtons[p->m_nHeldIndex]->m_rect, pt))
						p->m_aButtons[p->m_nHeldIndex]->m_nState = CBNST_PRESSED;
					else
						p->m_aButtons[p->m_nHeldIndex]->m_nState = CBNST_NORMAL;
				}

				if(p->m_aButtons[p->m_nHeldIndex]->m_nState != nState)
					RedrawWindowFrame(hwnd, &p->m_aButtons[p->m_nHeldIndex]->m_rect);
			}
		}
		break;
	case WM_NCLBUTTONDOWN:
		{
#ifdef USE_TOOLTIP_SUPPORT
			if(p->m_tooltip.m_hWnd)
			{
				// MSG msg = { hwnd, message, wParam, lParam };
				POINT pt = MAKEPOINT(lParam);
				ScreenToClient(hwnd, &pt);
				
				MSG msg = { hwnd, WM_LBUTTONDOWN, wParam, MAKELPARAM(pt.x, pt.y) };
				p->m_tooltip.RelayEvent(&msg);
			}
#endif // USE_TOOLTIP_SUPPORT

			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					if(!p->m_fActive)
						SetActiveWindow(hwnd);

					if(p->m_aButtons[i]->m_nState != CBNST_DISABLED)
					{
						SetCapture(hwnd);

						p->m_aButtons[i]->m_nState = CBNST_PRESSED;
						p->m_nHeldIndex = i;
						p->m_bHeld = HELDLCLK;

						RedrawWindowFrame(hwnd, &p->m_aButtons[i]->m_rect);
					}

					return 0;
				}
			}
		}
		break;
	case WM_NCLBUTTONDBLCLK:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					if(p->m_aButtons[i]->m_nState != CBNST_DISABLED)
					{
						SetCapture(hwnd);

						p->m_aButtons[i]->m_nState = CBNST_PRESSED;
						p->m_nHeldIndex = i;
						p->m_bHeld = HELDLDBLCLK;

						RedrawWindowFrame(hwnd, &p->m_aButtons[i]->m_rect);
					}

					return 0;
				}
			}
		}
		break;
	case WM_NCRBUTTONDOWN:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					if(!p->m_fActive)
						SetActiveWindow(hwnd);

					if(p->m_aButtons[i]->m_nState != CBNST_DISABLED)
					{
						SetCapture(hwnd);

						p->m_aButtons[i]->m_nState = CBNST_PRESSED;
						p->m_nHeldIndex = i;
						p->m_bHeld = HELDRCLK;

						RedrawWindowFrame(hwnd, &p->m_aButtons[i]->m_rect);
					}

					return 0;
				}
			}
		}
		break;
	case WM_NCRBUTTONDBLCLK:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			for(int i = 0; i< p->m_aButtons.GetSize(); i++)
			{
				if(PtInRect(&p->m_aButtons[i]->m_rect, pt))
				{
					if(p->m_aButtons[i]->m_nState != CBNST_DISABLED)
					{
						SetCapture(hwnd);

						p->m_aButtons[i]->m_nState = CBNST_PRESSED;
						p->m_nHeldIndex = i;
						p->m_bHeld = HELDRDBLCLK;

						RedrawWindowFrame(hwnd, &p->m_aButtons[i]->m_rect);
					}

					return 0;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
#ifdef USE_TOOLTIP_SUPPORT
			if(p->m_tooltip.m_hWnd)
			{
				MSG msg = { hwnd, message, wParam, lParam };
				p->m_tooltip.RelayEvent(&msg);
			}
#endif // USE_TOOLTIP_SUPPORT

			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };
			ClientToScreen(hwnd, &pt);

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			if(p->m_nHeldIndex != -1 && (p->m_bHeld == HELDLCLK || p->m_bHeld == HELDLDBLCLK))
			{
				if(PtInRect(&p->m_aButtons[p->m_nHeldIndex]->m_rect, pt) && p->m_aButtons[p->m_nHeldIndex]->m_nState != CBNST_DISABLED)
				{
					switch(p->m_nHeldIndex)
					{
					case 0:
						if(p->m_bHeld == HELDLCLK)
							PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, lParam);
						break;
					case 1:
						if(p->m_bHeld == HELDLCLK)
							PostMessage(hwnd, WM_SYSCOMMAND, IsZoomed(hwnd) ? SC_RESTORE : SC_MAXIMIZE, lParam);
						break;
					case 2:
						if(p->m_bHeld == HELDLCLK)
							PostMessage(hwnd, WM_SYSCOMMAND, IsIconic(hwnd) ? SC_RESTORE : SC_MINIMIZE, lParam);
						break;
					case 3:
						if(p->m_bHeld == HELDLCLK)
							PostMessage(hwnd, WM_SYSCOMMAND, SC_CONTEXTHELP, lParam);
						break;
					default:
						// NOTE: we can NOT use WM_NOTIFY message any more because it always need a true control
						SendMessage(hwnd, WM_SYSCOMMAND, SC_NOTIFY,
							(LPARAM)MAKELONG(p->m_aButtons[p->m_nHeldIndex]->GetDlgCtrlID(), p->m_bHeld == HELDLCLK ? NM_CLICK : NM_DBLCLK));
						break;
					}
				}

				p->m_nHeldIndex = -1;
				p->m_bHeld = HELDNONE;

				::ReleaseCapture();

				pt.x += rc.left;
				pt.y += rc.top;

				PostMessage(hwnd, WM_NCMOUSEMOVE, HTCAPTION, MAKELPARAM(pt.x, pt.y));

				return 0;
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };
			ClientToScreen(hwnd, &pt);

			RECT rc;
			GetWindowRect(hwnd, &rc);

			pt.x -= rc.left;
			pt.y -= rc.top;

			if(p->m_nHeldIndex != -1 && (p->m_bHeld == HELDRCLK || p->m_bHeld == HELDRDBLCLK))
			{
				if(PtInRect(&p->m_aButtons[p->m_nHeldIndex]->m_rect, pt) && p->m_aButtons[p->m_nHeldIndex]->m_nState != CBNST_DISABLED)
				{
					SendMessage(hwnd, WM_SYSCOMMAND, SC_NOTIFY,
						(LPARAM)MAKELONG(p->m_aButtons[p->m_nHeldIndex]->GetDlgCtrlID(), p->m_bHeld == HELDRCLK ? NM_RCLICK : NM_RDBLCLK));
				}

				p->m_nHeldIndex = -1;
				p->m_bHeld = HELDNONE;

				::ReleaseCapture();

				pt.x += rc.left;
				pt.y += rc.top;

				PostMessage(hwnd, WM_NCMOUSEMOVE, HTCAPTION, MAKELPARAM(pt.x, pt.y));

				return 0;
			}
		}
		break;
	}

	if(message == 174)
		return 0;

	return ::CallWindowProc(p->m_pOldWndProc, hwnd, message, wParam, lParam);
}
