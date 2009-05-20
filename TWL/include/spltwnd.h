//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

//
// Features:
// 1. nested splitting
// 2. interruptable, by WM_CANCELMODE, WM_KEYDOWN(VK_ESCAPE) and WM_RBUTTONUP
// 3. Static/dynamic splitting
// 4. Dragging offset and splitter width support
// 5. Double click for middle-splitting
// 6. Keyboard support
// 7. Hide pane(non-splitter only)
//
// TODO list:
// 8. Active pane support
// 9. Delete pane - ???
//

#ifndef __SPLTWND_H__
#define __SPLTWND_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "userwnd.h"
#include "msgmap.h"
#include "modutil.h"

namespace TWL
{

#define ENABLE_KEYBOARD_SPLIT
#define ENABLE_HIDE_PANE

#ifndef IDC_STATIC
#define IDC_STATIC		(-1)
#endif // IDC_STATIC

#ifndef TWL_IDW_SPLITTER
#define TWL_IDW_SPLITTER                0xE900  // splitter/layout window
#endif // TWL_IDW_SPLITTER

#define TWL_IDW_SPLITTER_FIRST          TWL_IDW_SPLITTER

// Note: Don't modify these values
#define SS_UNDEFINED	0x7FFFFFFF
#define SS_VERTICAL		0
#define SS_HORIZONTAL	1

#define SPLIT_PANE_LEFT		0
#define SPLIT_PANE_RIGHT	1
#define SPLIT_PANE_TOP		SPLIT_PANE_LEFT
#define SPLIT_PANE_BOTTOM	SPLIT_PANE_RIGHT
#define SPLIT_PANE_NONE		(-1)

#define loedge	loEdge()
#define hiedge	hiEdge()
#define lobar	loBar()
#define hibar	hiBar()

class CSplitterWnd : public CUserWindow
{
	typedef CUserWindow _superClass;

public:
	// type definitions
	typedef struct _PANE
	{
		SIZE sizMin;			// how small the pane can be?
		BOOL bSplitter;			// a window or a sub-splitter?

		union				
		{
			HWND hChildWnd;
			CSplitterWnd* pSub;
		};
	} PANE, *PPANE;

	typedef struct _SPLITINFO	
	{
		CSplitterWnd* pParent;	// for sub-splitter only
		UINT uOrient;			// is this splitter bar vertical or horizontal?
		RECT rc;				// splitter rectange, relative to the client area of frame window		
		float fRatio;			// the ratio of dividing pane0 and pane1
		int iSize;				// splitter bar's width ( or half width?)

		// m_panes[] can only tell who the children are. To find out which pane
		// this splitter sits in parent splitter, we have to check parent's m_panes[]
		// and compare the pointers to it's children. I don't want to write extra
		// code to do this, instead, I add an index here.
		UINT uIndex;			// which pane in parent splitter? (for sub splitter)
	} SPLITINFO, *PSPLITINFO;

protected:
	//
	// data for root splitter and all sub-splitters
	//

	PANE m_panes[2];			// two panes the splitter owns, either a window or a sub-splitter
	SPLITINFO m_info;			// attributes about this splitter
	HCURSOR m_hCursor;			// a handle to the actual vertical or horizontal cursor

	HBRUSH m_hbrGhostBar;

	// splitter bar's position	relative to the frame window's client area 
	int m_iPos;					// current position
	int m_iDragOffset;

	BOOL m_bDynamic;
	int m_iOldPos;				// old position
	HWND m_hPrevFocus;

	//
	// data for root splitter only
	//

	BOOL m_bRoot;
	BOOL m_bAutoSize;			// automatic size all splitters when the host was sized
	CSplitterWnd* m_pDragging;	// pointer to the splitter being dragged
	CSplitterWnd* m_pTopSplitter;

	static HCURSOR m_hcurVert;	// vertical cursor
	static HCURSOR m_hcurHorz;	// horizontal cursor

#ifdef ENABLE_HIDE_PANE
	UINT m_nHiddenPane;
#endif // ENABLE_HIDE_PANE

public:
	CSplitterWnd(BOOL bRoot = TRUE)
	{
		ZeroMemory(&m_panes, sizeof(PANE) * 2);
		ZeroMemory(&m_info, sizeof(SPLITINFO));

		m_info.uOrient = SS_UNDEFINED;
		m_hCursor = NULL;

		m_hbrGhostBar = NULL;

		m_iPos = 0;
		m_iDragOffset = 0;

		m_bDynamic = FALSE;
		m_iOldPos = 0;
		m_hPrevFocus = NULL;

		m_bRoot = bRoot;
		m_bAutoSize	= FALSE;
		m_pDragging	= NULL;
		m_pTopSplitter = NULL;

#ifdef ENABLE_HIDE_PANE
		m_nHiddenPane = SPLIT_PANE_NONE;
#endif // ENABLE_HIDE_PANE
	}

	virtual ~CSplitterWnd()
	{
		if(m_bRoot)
			CleanUp();
	}

	LPCTSTR GetClassName()
	{
		return _T("TWLSplitterWnd");
	}

	void SetWndClassEx(WNDCLASSEX& wce)
	{
		_superClass::SetWndClassEx(wce);

		wce.style |= CS_DBLCLKS;
	}

	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		CSplitterWnd* pTopSplitter = GetTopSplitter();

		switch(uMsg)
		{
		case WM_SIZE:
		{
			// only the top level splitter intercepts the frame window's WM_SIZE message
			RECT rc = {	0, 0, LOWORD(lParam), HIWORD(lParam) };

			// Note: an unnormalized rect is considered empty!!
			if(!IsRectEmpty(&rc))
				pTopSplitter->MoveSplitter(&rc);

			break;
		}
		case WM_LBUTTONDOWN:
		{
			// each splitter handles this message separately
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
#ifdef ENABLE_KEYBOARD_SPLIT
			if(!pTopSplitter->m_pDragging)
#endif // ENABLE_KEYBOARD_SPLIT
			StartTracking(&pt);

			break;
		}
		case WM_KEYDOWN:
		{
			if(wParam != VK_ESCAPE && wParam != VK_RETURN)
			{
				bool bProcessed = true;

				POINT pt;
				GetCursorPos(&pt);

				int cz = GetKeyState(VK_CONTROL) < 0 ? 1 : 16;
				int dx = 0;
				int dy = 0;

				switch(wParam)
				{
				case VK_LEFT:	dx = -1;	break;
				case VK_RIGHT:	dx = +1;	break;
				case VK_UP:		dy = -1;	break;
				case VK_DOWN:	dy = +1;	break;
				default:		bProcessed = false;
				}

				if(!bProcessed)
					break;

				CSplitterWnd* pSplitter = pTopSplitter->m_pDragging;
				pSplitter->m_info.uOrient == SS_VERTICAL ? (dy = 0) : (dx = 0);

				// adjust to new position
				pt.x += dx * cz;
				pt.y += dy * cz;

				/*
				// make sure within valid limits
				RECT rcLimit;
				// GetLimitRect(&rcLimit);
				::ScreenToClient(m_hWnd, &pt);
				if(pt.y < rcLimit.top)
					pt.y = rcLimit.top;
				else if(pt.y > rcLimit.bottom)
					pt.y = rcLimit.bottom;
				if(pt.x < rcLimit.left)
					pt.x = rcLimit.left;
				else if(pt.x > rcLimit.right)
					pt.x = rcLimit.right;
				::ClientToScreen(m_hWnd, &pt);
				// */

				// cause WM_MOUSEMOVE to filter through
				SetCursorPos(pt.x, pt.y);

				lResult = 0;
				return true;
			}
			// Attention: Fall down!!!
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_CANCELMODE:
		{
			StopTracking(uMsg == WM_LBUTTONUP || (uMsg == WM_KEYDOWN && wParam == VK_RETURN));
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			CSplitterWnd* pSplitter = pTopSplitter->GetBarFromPoint(&pt);
			if(pSplitter)
			{
				int iBarSize = pSplitter->m_info.iSize + (1 * 2);					// 1 for border? - dandy
				int iPos = pSplitter->loedge + (pSplitter->hiedge - pSplitter->loedge - iBarSize) / 2;
				pSplitter->SetBarPos(iPos);
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			CSplitterWnd* pSplitter = pTopSplitter->m_pDragging;
			if(pSplitter)
			{
#ifdef ENABLE_KEYBOARD_SPLIT
				if(!(wParam & MK_LBUTTON))
					SetCursor(pSplitter->m_hCursor);
#endif // ENABLE_KEYBOARD_SPLIT

				int iPos = *(SHORT*)((WORD*)&lParam + pSplitter->m_info.uOrient);

				iPos -= pSplitter->m_iDragOffset;

				if(iPos < pSplitter->lobar)
					iPos = pSplitter->lobar;
				else if(iPos > pSplitter->hibar)
					iPos = pSplitter->hibar;

				if(iPos != pSplitter->m_iPos)
				{
					if(pSplitter->m_bDynamic)
					{
						pSplitter->m_iPos = iPos;
						pSplitter->MoveBar();
					}
					else
					{
						pSplitter->DrawGhostBar(pSplitter->m_iPos);		// Xor the old bar
						pSplitter->m_iPos = iPos;
						pSplitter->DrawGhostBar(pSplitter->m_iPos);
					}
				}
			}
			break;
		}
		case WM_SETCURSOR:
		{
			POINT pt;
			GetCursorPos(&pt);
			::ScreenToClient(m_hWnd, &pt);

			CSplitterWnd* pSplitter = pTopSplitter->GetBarFromPoint(&pt);

			if(pSplitter)
			{
				RECT rc;
				pSplitter->GetBarRect(&rc);

				if(PtInRect(&rc, pt))
					SetCursor(pSplitter->m_hCursor);

				lResult = 0;
				return true;
			}
			break;
		}
		case WM_ERASEBKGND:
		{
			lResult = 1;
			return true;
		}
		case WM_PAINT:
		{
			// walk through all splitters and draw them
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(pTopSplitter->m_hWnd, &ps);
			pTopSplitter->PaintSplitter(hdc, TRUE);
			::EndPaint(pTopSplitter->m_hWnd, &ps);

			break;
		}
		default:
		{
			bool bHandled = false;
			FORWARD_NOTIFICATIONS()
			break;
		}
		}

		return _superClass::ProcessMessage(uMsg, wParam, lParam, lResult);
	}

#ifdef NO_USE_CWINDOW
	BOOL ShowWindow(int iCmdShow)
	{
		return ::ShowWindow(m_hWnd, iCmdShow);
	}

	operator HWND()
	{
		return m_hWnd;
	}
#endif // NO_USE_CWINDOW

// splitter window special stuff
public:
	bool CreateEx(HWND hwndParent, BOOL bVisible = TRUE, BOOL bVertical = TRUE,
		float fRatio = 0.38, UINT uMin = 10, UINT uSize = 3, PRECT prc = NULL, UINT uId = TWL_IDW_SPLITTER)
	{
		ASSERT(m_info.pParent == NULL);

		if(!m_bRoot)
			return false;

		// initialize top-splitter only stuff

		RECT rc;
		if(prc)
			CopyRect(&rc, prc);
		else
			::GetClientRect(hwndParent, &rc);

		m_pTopSplitter = this;

		bool bRet = Create(0, NULL,
			(bVisible ? WS_VISIBLE : 0) | (hwndParent ? WS_CHILD : WS_POPUP),
			&rc, hwndParent, (HMENU)uId, GetModuleInstance());

		// initialize general stuff
		m_info.pParent = NULL;
		m_info.uIndex = -1;								// top splitter doesn't belong to any pane

		OffsetRect(&rc, -rc.left, -rc.top);				// Bug fix: Geng found it.
		SetSplitInfo(bVertical, fRatio, uMin, uSize, &rc, NULL);

		return bRet;
	}

	void CleanUp()
	{
		if(m_bRoot)
		{
			if(m_hbrGhostBar)
			{
				DeleteObject(m_hbrGhostBar);
				m_hbrGhostBar = NULL;
			}

			DestroyChildren();
		}
	}

	CSplitterWnd* Split(UINT uIndex, BOOL bVertical = TRUE,
		float fRatio = 0.50, UINT uMin = 10, UINT uSize = 3)
	{
		ASSERT(m_info.uOrient != SS_UNDEFINED);			// parent must have been initialized
		if(m_info.uOrient == SS_UNDEFINED)
			return NULL;

		ASSERT(uIndex == 0 || uIndex == 1);
		if(uIndex != 0 && uIndex != 1)
			return NULL;

		ASSERT(m_panes[uIndex].pSub == NULL);
		if(m_panes[uIndex].pSub != NULL)				// there is a sub-splitter or window already
			return NULL;

		/// TODO: check if there is enough space for splitting

		CSplitterWnd* pSub = new CSplitterWnd(FALSE);
		if(!pSub)
			return NULL;

		RECT rc;
		GetPaneRect(uIndex, &rc);

		pSub->m_hWnd = m_hWnd;
		pSub->m_info.pParent = this;
		pSub->m_info.uIndex = uIndex;
		pSub->m_pTopSplitter = m_pTopSplitter;

		pSub->m_bDynamic = m_bDynamic;

		SIZE szMin;
		pSub->SetSplitInfo(bVertical, fRatio, uMin, uSize, &rc, &szMin);
		OnChildChanged(uIndex, szMin);					// the pane size has been changed

		m_panes[uIndex].bSplitter = TRUE;
		m_panes[uIndex].pSub = pSub;

		return pSub;
	}

	BOOL SetPane(UINT uIndex, HWND hChildWnd)
	{
		ASSERT(m_info.uOrient != SS_UNDEFINED);			// this splitter must be initialized

		if(hChildWnd == NULL || !::IsWindow(hChildWnd))
			return FALSE;

		m_panes[uIndex].bSplitter = FALSE;
		m_panes[uIndex].hChildWnd = hChildWnd;

		MovePane(uIndex);

		return TRUE;
	}

	CSplitterWnd* GetSubSplitter(UINT uIndex)
	{
		if(m_panes[uIndex].bSplitter)
			return m_panes[uIndex].pSub;

		return NULL;
	}

	CSplitterWnd* GetTopSplitter()
	{
		return m_pTopSplitter;
	}

	void SetBarPos(int iPos)
	{
		/// TODO: check iPos
		if(m_iPos == iPos)
			return;

		m_iPos = iPos;
		MoveBar();
	}

#ifdef ENABLE_KEYBOARD_SPLIT
	void DoKeyboardSplit()
	{
		POINT pt, ptCursor;
		if(m_info.uOrient == SS_VERTICAL)
		{
			pt.x = m_iPos + 1; // 1 for border
			pt.y = m_info.rc.top + (m_info.rc.bottom - m_info.rc.top) / 2;
		}
		else
		{
			pt.x = m_info.rc.left + (m_info.rc.right - m_info.rc.left) / 2;
			pt.y = m_iPos + 1; // 1 for border
		}

		ptCursor = pt;
		::ClientToScreen(m_hWnd, &ptCursor);
		SetCursorPos(ptCursor.x, ptCursor.y);

		// NOTE: Because no mouse button was pressed, SetCapture() will change the 
		// cursor back to the normal one.
		// For fixing it, we changed the cursor again while processing WM_MOUSEMOVE.
		StartTracking(&pt);
	}
#endif // ENABLE_KEYBOARD_SPLIT

	BOOL GetAutoSize() { return m_bAutoSize; }
	void SetAutoSize(BOOL bAutoSize) { m_bAutoSize = bAutoSize; }

	BOOL GetDynamicMode() { return m_bDynamic; }
	void SetDynamicMode(BOOL bDynamic) { m_bDynamic = bDynamic; }

#ifdef ENABLE_HIDE_PANE
	bool HidePane(UINT nPane)
	{
		ASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);
		if(!(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT))
			return false;

		if(m_nHiddenPane == nPane)
			return true;

		if(m_nHiddenPane != SPLIT_PANE_NONE)	// cannot hide both two panes concurrently
			return false;

		if(m_panes[nPane].bSplitter)			// cannot hide a sub-splitter
			return false;

		if(m_panes[nPane].hChildWnd != NULL)
			::ShowWindow(m_panes[nPane].hChildWnd, SW_HIDE);

		int iOtherPane = SPLIT_PANE_RIGHT - nPane;
		if(m_panes[iOtherPane].bSplitter)
			m_panes[iOtherPane].pSub->MoveSplitter(&m_info.rc);
		else if(m_panes[iOtherPane].hChildWnd != NULL)
			::SetWindowPos(m_panes[iOtherPane].hChildWnd, NULL,
				m_info.rc.left, m_info.rc.top, m_info.rc.right - m_info.rc.left, m_info.rc.bottom - m_info.rc.top,
				SWP_NOZORDER | SWP_SHOWWINDOW);

		m_nHiddenPane = nPane;

		return true;
	}

	bool ShowPane(UINT nPane)
	{
		ASSERT(nPane == m_nHiddenPane);
		if(nPane != m_nHiddenPane)
			return true;

		ASSERT(!m_panes[nPane].bSplitter);

		m_nHiddenPane = SPLIT_PANE_NONE;

		RECT rc;

		if(m_panes[nPane].hChildWnd != NULL)
		{
			GetPaneRect(nPane, &rc);
			::SetWindowPos(m_panes[nPane].hChildWnd, NULL,
				rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				SWP_NOZORDER | SWP_SHOWWINDOW);
		}
		else
		{
			GetPaneRect(nPane, &rc);
			::InvalidateRect(m_hWnd, &rc, TRUE);
		}

		GetBarRect(&rc);
		::InvalidateRect(m_hWnd, &rc, TRUE);

		int iOtherPane = SPLIT_PANE_RIGHT - nPane;

		GetPaneRect(iOtherPane, &rc);
		if(m_panes[iOtherPane].bSplitter)
			m_panes[iOtherPane].pSub->MoveSplitter(&rc);
		else if(m_panes[iOtherPane].hChildWnd != NULL)
			::SetWindowPos(m_panes[iOtherPane].hChildWnd, NULL,
				rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				SWP_NOZORDER | SWP_SHOWWINDOW);

		return true;
	}

	int GetHiddenPane() const
	{
		return m_nHiddenPane;
	}
#endif // ENABLE_HIDE_PANE

	void GetPaneRect(UINT uIndex, PRECT prc)
	{
		*prc = m_info.rc;

#ifdef ENABLE_HIDE_PANE
		if(m_nHiddenPane != SPLIT_PANE_NONE)
		{
			if(uIndex == m_nHiddenPane)
				SetRectEmpty(prc);

			return;
		}
#endif // ENABLE_HIDE_PANE

		PLONG lpedge = &prc->left + m_info.uOrient;

		(uIndex == 0) ?
			*(lpedge + 2) = max(loedge, m_iPos) :
			*lpedge       = min(hiedge, m_iPos + m_info.iSize);
	}

	// called only if pane is empty
	virtual void DrawPane(HDC hdc, PRECT prc)
	{
		DrawEdge(hdc, prc, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
		FillRect(hdc, prc, GetSysColor(COLOR_APPWORKSPACE));
	}

protected:
	void SetSplitInfo(BOOL bVertical, float	fRatio, UINT uMin, UINT	uSize, PRECT prc, PSIZE psz)		// [out]
	{
		ASSERT(m_info.uOrient == SS_UNDEFINED);			// can only be initialized once
		ASSERT(fRatio > 0 && fRatio < 1);

		m_info.iSize = uSize;
		m_info.rc = *prc;								// rcAdjust or rc
		m_info.fRatio = fRatio;							// (float) 0.38 or 0.5

		uSize += uMin;
		m_panes[0].sizMin.cx = m_panes[0].sizMin.cy = uSize;
		m_panes[1].sizMin.cx = m_panes[1].sizMin.cy = uSize;

		if(!m_hcurHorz)
			m_hcurHorz = LoadCursor(NULL, IDC_SIZENS);

		if(!m_hcurVert)
			m_hcurVert = LoadCursor(NULL, IDC_SIZEWE);

		if(bVertical)
		{
			m_iPos = int(fRatio * (prc->right - prc->left) + prc->left);
			m_hCursor = m_hcurVert;
			m_info.uOrient = SS_VERTICAL;

			if(psz)
			{
				psz->cx = 2 * uSize;
				psz->cy = uSize;
			}
		}
		else
		{
			m_iPos = int(fRatio * (prc->bottom - prc->top) + prc->top);
			m_hCursor = m_hcurHorz;
			m_info.uOrient = SS_HORIZONTAL;

			if(psz)
			{
				psz->cx = uSize;
				psz->cy = 2 * uSize;
			}
		}
	}

	void MoveSplitter(PRECT prc)
	{
		if(m_info.uOrient == SS_UNDEFINED)
			return;

		RECT rc;
		GetBarRect(&rc);
		::InvalidateRect(m_hWnd, &rc, TRUE);

		m_info.rc = *prc;

		/// TODO: check sizMin
		if(m_bAutoSize)
			m_iPos = int((hiedge - loedge) * m_info.fRatio + loedge); // new position

		MovePane(0);
		MovePane(1);

		GetBarRect(&rc);
		::InvalidateRect(m_hWnd, &rc, TRUE);
	}

	void MovePane(UINT uIndex)
	{
		RECT rc;
		GetPaneRect(uIndex, &rc);

		if(m_panes[uIndex].bSplitter)
			m_panes[uIndex].pSub->MoveSplitter(&rc);
		else
		{
			HWND hWnd = m_panes[uIndex].hChildWnd;
			if(hWnd)
			{
				// revise the coordinates of the non-child panes
				HWND hParentWnd = ::GetParent(hWnd);
				if(hParentWnd != m_hWnd)
					::MapWindowPoints(m_hWnd, hParentWnd, (LPPOINT)&rc, 2);

				::MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
			}
			else
				::InvalidateRect(m_hWnd, &rc, TRUE);
		}
	}

	void GetBarRect(PRECT prc)
	{
		*prc = m_info.rc;
		PLONG lpedge  = &prc->left + m_info.uOrient;
		*(lpedge)	  = m_iPos;
		*(lpedge + 2) = m_iPos + m_info.iSize;
	}

	void DrawPane(HDC hdc, UINT uPane)
	{
		RECT rc;
		GetPaneRect(uPane, &rc);

		DrawPane(hdc, &rc);
	}

	void DrawGhostBar(int x)
	{

		HBRUSH& rhbr = m_pTopSplitter->m_hbrGhostBar;

		if(rhbr == NULL)
		{
			// create an 8 x 8 bitmap for the rubber band
			static WORD awDots[] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
			HBITMAP hbmp = CreateBitmap(8, 8, 1, 1, awDots);

			// create patterned brush from the bitmap
			rhbr = CreatePatternBrush(hbmp);
			DeleteObject(hbmp);
		}

		if(rhbr == NULL)
			return;

		HDC hdc = ::GetDC(m_hWnd);
		HGDIOBJ hbrOld = SelectObject(hdc, rhbr);

		RECT rc;
		GetBarRect(&rc);

		(m_info.uOrient == SS_VERTICAL) ?											// if
			PatBlt(hdc, x, rc.top, m_info.iSize, rc.bottom - rc.top, PATINVERT):	// then
			PatBlt(hdc, rc.left, x, rc.right - rc.left, m_info.iSize, PATINVERT);	// else

		SelectObject(hdc, hbrOld);

		::ReleaseDC(m_hWnd, hdc);
	}

	virtual void DrawBar(HDC hdc, PRECT prc, BOOL bVertical)
	{
#ifdef _DEBUG
		FillRect(hdc, prc, RGB(255, 0, 0));
#else
		FillRect(hdc, prc, GetSysColor(COLOR_BTNFACE));
#endif
	}

	void PaintSplitter(HDC hdc, BOOL bRecursive = FALSE)
	{
		RECT rc;
		GetBarRect(&rc);
		
		int iBarSize = m_info.iSize + (1 * 2);					// 1 for border? - dandy
		if(iBarSize > (m_info.rc.right - m_info.rc.left) ||		// the bar out of margin, quit.
			iBarSize > (m_info.rc.bottom - m_info.rc.top))		// don't paint on toolbar etc.
			return;

#ifdef ENABLE_HIDE_PANE
		if(m_nHiddenPane == SPLIT_PANE_NONE)
#endif // ENABLE_HIDE_PANE
		// draw the bar
		DrawBar(hdc, &rc, m_info.uOrient == SS_VERTICAL);

		for(int i=0; i<2; i++)
		{
			if(m_panes[i].bSplitter)
			{
				if(bRecursive)
					m_panes[i].pSub->PaintSplitter(hdc, bRecursive);
			}
			else if(m_panes[i].hChildWnd == NULL)
				DrawPane(hdc, i);
		}
	}

	BOOL PtInBar(PPOINT ppt)
	{
		RECT rc;
		GetBarRect(&rc);

		return PtInRect(&rc, *ppt);
	}

	CSplitterWnd* GetBarFromPoint(PPOINT ppt, CSplitterWnd* pBase = NULL)
	{
		CSplitterWnd* pSplitter = pBase ? pBase : GetTopSplitter();

		if(pSplitter)
		{
			if(!pSplitter->PtInBar(ppt))
			{
				CSplitterWnd* pSubSplitter = NULL;
				if(pSplitter->m_panes[0].bSplitter)
					pSubSplitter = GetBarFromPoint(ppt, pSplitter->m_panes[0].pSub);

				if(!pSubSplitter && pSplitter->m_panes[1].bSplitter)
					pSubSplitter = GetBarFromPoint(ppt, pSplitter->m_panes[1].pSub);

				pSplitter = pSubSplitter;
			}
		}

#ifdef ENABLE_HIDE_PANE
		if(pSplitter && pSplitter->m_nHiddenPane != SPLIT_PANE_NONE)
			pSplitter = NULL;
#endif // ENABLE_HIDE_PANE

		return pSplitter;
	}

	void DestroyChildren(BOOL bDestroyWindow = FALSE)
	{
		for(int i=0; i<2; i++)
		{
			if(m_panes[i].bSplitter && m_panes[i].pSub)	// a sub-splitter
			{
				m_panes[i].pSub->DestroyChildren();		// delete its children
				delete m_panes[i].pSub;					// delete itself
				m_panes[i].pSub = NULL;
			}
			else if(bDestroyWindow)
			{
				::DestroyWindow(m_panes[i].hChildWnd);	// destroy window
				m_panes[i].hChildWnd = NULL;
			}
		}
	}

	void OnChildChanged(UINT uIndex, const SIZE& csz)
	{
		m_panes[uIndex].sizMin.cx = csz.cx;				// update child's minimum size
		m_panes[uIndex].sizMin.cy = csz.cy;

		if(m_info.pParent)								// calculate itself's new size
		{
			SIZE sz;
			if(m_info.uOrient == SS_VERTICAL)
			{
				sz.cx = m_panes[0].sizMin.cx + m_panes[1].sizMin.cx;
				sz.cy = max(m_panes[0].sizMin.cy, m_panes[1].sizMin.cy);
			}
			else
			{
				sz.cx = max(m_panes[0].sizMin.cx, m_panes[1].sizMin.cx);
				sz.cy = m_panes[0].sizMin.cy + m_panes[1].sizMin.cy;
			}

			// recursively notify parent to update
			m_info.pParent->OnChildChanged(m_info.uIndex, sz);
		}
	}

	bool StartTracking(PPOINT ppt)
	{
		if(!ppt)
			return false;

		POINT pt = *ppt;

		CSplitterWnd* pTopSplitter = GetTopSplitter();
		if(!pTopSplitter)
			return false;

		CSplitterWnd* pSplitter = pTopSplitter->GetBarFromPoint(&pt);
		if(!pSplitter)
			return false;

		RECT rc;
		pSplitter->GetBarRect(&rc);

		if(!PtInRect(&rc, pt))
			return false;

		pTopSplitter->m_pDragging = pSplitter;

		// calculate the cursor offset
		if(pSplitter->m_info.uOrient == SS_VERTICAL)
			pSplitter->m_iDragOffset = pt.x - pSplitter->m_iPos;
		else
			pSplitter->m_iDragOffset = pt.y - pSplitter->m_iPos;

		pSplitter->m_iOldPos = pSplitter->m_iPos;

		if(!pSplitter->m_bDynamic)
			pSplitter->DrawGhostBar(pSplitter->m_iPos);

		::SetCapture(pSplitter->m_hWnd);

		pSplitter->m_hPrevFocus = ::GetFocus();
		::SetFocus(m_hWnd);

		return true;
	}

	void StopTracking(BOOL bAccept = TRUE)
	{
		// only the splitter whose bar is being dragged responses to this message
		CSplitterWnd* pTopSplitter = GetTopSplitter();
		if(!pTopSplitter)
			return;

		CSplitterWnd* pSplitter = pTopSplitter->m_pDragging;
		if(!pSplitter)
			return;

		::SetFocus(pSplitter->m_hPrevFocus);
		pSplitter->m_hPrevFocus = NULL;

		ReleaseCapture();

		if(!pSplitter->m_bDynamic)
			pSplitter->DrawGhostBar(pSplitter->m_iPos);

		if(!bAccept)
			pSplitter->m_iPos = pSplitter->m_iOldPos;

		pSplitter->m_iDragOffset = 0;
		pTopSplitter->m_pDragging = NULL;

		pSplitter->MoveBar();
	}

	void MoveBar()
	{
		m_info.fRatio = (loedge == hiedge) ?
			float(0.50) :	 // calculate the low/high ratio
			float(m_iPos - loedge) / float(hiedge - loedge);

		RECT rc;
		GetBarRect(&rc);
		::InvalidateRect(m_hWnd, &rc, TRUE);

		for(int i=0; i<2; i++)
			MovePane(i);
	}

	int loEdge()
	{
		return (*(&m_info.rc.left  + m_info.uOrient));
	}

	int hiEdge()
	{
		return (*(&m_info.rc.right + m_info.uOrient));
	}

	int loBar()
	{
		return (loEdge() + (*(&m_panes[0].sizMin.cx + m_info.uOrient)));
	}

	int hiBar()
	{
		return (hiEdge() - (*(&m_panes[1].sizMin.cx + m_info.uOrient)));
	}

	static void FillRect(HDC hdc, const RECT* prc, COLORREF clrFill)
	{
		SetBkColor(hdc, clrFill);
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, prc, NULL, 0, NULL);
	}
};

__declspec(selectany) HCURSOR CSplitterWnd::m_hcurVert = NULL;
__declspec(selectany) HCURSOR CSplitterWnd::m_hcurHorz = NULL;

}; // namespace TWL

#endif // __SPLTWND_H__
