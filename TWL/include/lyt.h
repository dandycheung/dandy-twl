//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __LYT_H__
#define __LYT_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include "ary.h"

namespace TWL
{

#define LAYOUT_BORDER	0xFFFFFFFF
#define LAYOUT_KEEPSIZE	0xFFFFFFFE
#define LAYOUT_HCENTER	0x00000001
#define LAYOUT_VCENTER	0x00000002
#define LAYOUT_FIXED	0x80000000	// internal use, DON'T use it

// Apply for the very first item only, and it's a global flag
#define UPDATE_RIGHTNOW	0x40000000	// internal use, DON'T use it

#define LAYOUT_MFORCEID	0x000000F0	// mask
#define LAYOUT_LFORCEID	0x00000010	// left
#define LAYOUT_TFORCEID	0x00000020	// top
#define LAYOUT_RFORCEID	0x00000040	// right
#define LAYOUT_BFORCEID	0x00000080	// bottom

class CLayout
{
protected:
	HWND m_hWnd;

	typedef struct tagLAYOUTITEM
	{
		UINT uId;

		int l;
		int t;
		int r;
		int b;

		int lm;
		int tm;
		int rm;
		int bm;
		int o;
	} LAYOUTITEM, *PLAYOUTITEM;

	TArray<LAYOUTITEM> m_items;

public:
	CLayout(HWND hWnd = NULL) : m_hWnd(NULL)
	{
		SetHost(hWnd);
	}

	virtual ~CLayout()
	{
		m_items.RemoveAll();
	}

	void SetHost(HWND hWnd)
	{
		if(m_hWnd)
			m_items.RemoveAll();

		m_hWnd = hWnd;
	}

	HWND GetHost()
	{
		return m_hWnd;
	}

	bool AddControl(UINT uId, int lm, int tm, int rm, int bm, int o = 0)
	{
		if(!(o & LAYOUT_MFORCEID) && lm == LAYOUT_BORDER && tm == LAYOUT_BORDER && rm == LAYOUT_KEEPSIZE && bm == LAYOUT_KEEPSIZE)
			o |= LAYOUT_FIXED;

		LAYOUTITEM li;

		li.uId = uId;

		li.lm = lm;
		li.tm = tm;
		li.rm = rm;
		li.bm = bm;
		li.o = o;

		if(!m_items.Add(li))
			return false;

		if(
			((lm != LAYOUT_BORDER && lm != LAYOUT_KEEPSIZE) || (o & LAYOUT_LFORCEID))
			|| ((tm != LAYOUT_BORDER && tm != LAYOUT_KEEPSIZE) || (o & LAYOUT_TFORCEID))
			|| ((rm != LAYOUT_BORDER && rm != LAYOUT_KEEPSIZE) || (o & LAYOUT_RFORCEID))
			|| ((bm != LAYOUT_BORDER && bm != LAYOUT_KEEPSIZE) || (o & LAYOUT_MFORCEID))
			)
		{
			m_items[0].o |= UPDATE_RIGHTNOW;	// we SHOULD NOT use DeferWindowPos() any way
		}

		return true;
	}

	bool RemoveControl(UINT uId)
	{
		for(int i=0; i<m_items.GetSize(); i++)
		{
			if(m_items[i].uId == uId)
			{
				m_items.RemoveAt(i);
				return true;
			}
		}

		return false;
	}

	bool GetSnapshot(PRECT prc = NULL)
	{
		ASSERT(m_hWnd != NULL);
		ASSERT(::IsWindow(m_hWnd));

		if(m_hWnd == NULL || !::IsWindow(m_hWnd))
			return false;

		RECT rcClient, rc, rc2;
		if(prc)
			rcClient = *prc;
		else
			::GetClientRect(m_hWnd, &rcClient);

		HWND hwndChild = NULL;
		PLAYOUTITEM pItem = NULL;

		for(int i=0; i<m_items.GetSize(); i++)
		{
			pItem = &m_items[i];

			GetChildRect(pItem->uId, &rc);

			int iWidth = rc.right - rc.left;
			int iHeight = rc.bottom - rc.top;

			if(pItem->o & LAYOUT_HCENTER)
				pItem->l = iWidth / 2;
			else
			{
				if(pItem->lm == LAYOUT_BORDER && !(pItem->o & LAYOUT_LFORCEID))
					pItem->l = rc.left;
				else if(pItem->lm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_LFORCEID))
					pItem->l = iWidth;
				else
				{
					GetChildRect(pItem->lm, &rc2);
					pItem->l = rc.left - rc2.right;
				}
			}

			if(pItem->o & LAYOUT_VCENTER)
				pItem->t = iHeight / 2;
			else
			{
				if(pItem->tm == LAYOUT_BORDER && !(pItem->o & LAYOUT_TFORCEID))
					pItem->t = rc.top;
				else if(pItem->tm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_TFORCEID))
					pItem->t = iHeight;
				else
				{
					GetChildRect(pItem->tm, &rc2);
					pItem->t = rc.top - rc2.bottom;
				}
			}

			if(pItem->o & LAYOUT_HCENTER)
				pItem->r = iWidth;
			else
			{
				if(pItem->rm == LAYOUT_BORDER && !(pItem->o & LAYOUT_RFORCEID))
					pItem->r = rcClient.right - rc.right;
				else if(pItem->rm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_RFORCEID))
					pItem->r = iWidth;
				else
				{
					GetChildRect(pItem->rm, &rc);
					pItem->r = rc2.left - rc.right;
				}
			}

			if(pItem->o & LAYOUT_VCENTER)
				pItem->b = iHeight;
			else
			{
				if(pItem->bm == LAYOUT_BORDER && !(pItem->o & LAYOUT_BFORCEID))
					pItem->b = rcClient.bottom - rc.bottom;
				else if(pItem->bm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_BFORCEID))
					pItem->b = iHeight;
				else
				{
					GetChildRect(pItem->bm, &rc);
					pItem->b = rc2.top - rc.bottom;
				}
			}
		}

		return true;
	}

	bool Update(PRECT prc = NULL, BOOL bForceUpdate = FALSE)
	{
		ASSERT(m_hWnd != NULL);
		ASSERT(::IsWindow(m_hWnd));

		if(m_hWnd == NULL || !::IsWindow(m_hWnd))
			return false;

		int iCount = m_items.GetSize();
		if(iCount == 0)
			return true;

		RECT rcClient, rc;
		if(prc)
			rcClient = *prc;
		else
			::GetClientRect(m_hWnd, &rcClient);

		HWND hwndChild = NULL;
		PLAYOUTITEM pItem = NULL;

		HDWP hDWP = m_items[0].o & UPDATE_RIGHTNOW ? NULL : ::BeginDeferWindowPos(iCount); // reasonable guess

		for(int i=0; i<iCount; i++)
		{
			pItem = &m_items[i];
			if(pItem->o & LAYOUT_FIXED)
				continue;

			int left = 0, top = 0, right = 0, bottom = 0;
			if(pItem->o & LAYOUT_HCENTER)
			{
				int _a, _b;
				if(pItem->lm == LAYOUT_BORDER && !(pItem->o & LAYOUT_LFORCEID))
					_a = rcClient.left;
				else
				{
					GetChildRect(pItem->lm, &rc);
					_a = rc.right;
				}

				if(pItem->rm == LAYOUT_BORDER && !(pItem->o & LAYOUT_RFORCEID))
					_b = rcClient.right;
				else
				{
					GetChildRect(pItem->rm, &rc);
					_b = rc.left;
				}

				left = _a + ((_b - _a) / 2 - pItem->l);
				right = left + pItem->r;
			}
			else
			{
				if(pItem->lm == LAYOUT_BORDER && !(pItem->o & LAYOUT_LFORCEID))
					left = pItem->l;
				else if(pItem->lm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_LFORCEID))
				{
					right = CalcBottomRight(pItem->uId, pItem->rm, pItem->r, left, rcClient.right, pItem->o, FALSE);
					left = right - pItem->l;
				}
				else
				{
					GetChildRect(pItem->lm, &rc);
					left = rc.right + pItem->l;
				}

				if(pItem->lm != LAYOUT_KEEPSIZE || !(pItem->o & LAYOUT_LFORCEID))
					right = CalcBottomRight(pItem->uId, pItem->rm, pItem->r, left, rcClient.right, pItem->o, FALSE);
			}

			if(pItem->o & LAYOUT_VCENTER)
			{
				int _a, _b;
				if(pItem->tm == LAYOUT_BORDER && !(pItem->o & LAYOUT_TFORCEID))
					_a = rcClient.top;
				else
				{
					GetChildRect(pItem->tm, &rc);
					_a = rc.bottom;
				}

				if(pItem->bm == LAYOUT_BORDER && !(pItem->o & LAYOUT_BFORCEID))
					_b = rcClient.bottom;
				else
				{
					GetChildRect(pItem->bm, &rc);
					_b = rc.top;
				}

				top = _a + ((_b - _a) / 2 - pItem->t);
				bottom = top + pItem->b;
			}
			else
			{
				if(pItem->tm == LAYOUT_BORDER && !(pItem->o & LAYOUT_TFORCEID))
					top = pItem->t;
				else if(pItem->tm == LAYOUT_KEEPSIZE && !(pItem->o & LAYOUT_TFORCEID))
				{
					bottom = CalcBottomRight(pItem->uId, pItem->bm, pItem->b, top, rcClient.bottom, pItem->o, TRUE);
					top = bottom - pItem->t;
				}
				else
				{
					GetChildRect(pItem->tm, &rc);
					top = rc.bottom + pItem->t;
				}

				if(pItem->tm != LAYOUT_KEEPSIZE || !(pItem->o & LAYOUT_TFORCEID))
					bottom = CalcBottomRight(pItem->uId, pItem->bm, pItem->b, top, rcClient.bottom, pItem->o, TRUE);
			}

			hwndChild = ::GetDlgItem(m_hWnd, pItem->uId);
			if(hwndChild == NULL)
				continue;

			if(hDWP)
				hDWP = ::DeferWindowPos(hDWP, hwndChild, NULL, left, top,  right - left, bottom - top, SWP_NOACTIVATE | SWP_NOZORDER);
			else
				::SetWindowPos(hwndChild, NULL, left, top, right - left, bottom - top, SWP_NOACTIVATE | SWP_NOZORDER);
				// ::MoveWindow(hwndChild, left, top, right - left, bottom - top, FALSE);
		}

		if(hDWP)
			::EndDeferWindowPos(hDWP);

		if(bForceUpdate)
		{
			::InvalidateRect(m_hWnd, NULL, TRUE);
			::UpdateWindow(m_hWnd);
		}

		return true;
	}

	void ShowControls(BOOL bShow)
	{
		ASSERT(m_hWnd != NULL);
		ASSERT(::IsWindow(m_hWnd));

		if(m_hWnd == NULL || !::IsWindow(m_hWnd))
			return;

		int iCount = m_items.GetSize();
		if(iCount == 0)
			return;

		HWND hwndChild = NULL;
		for(int i=0; i<iCount; i++)
		{
			hwndChild = ::GetDlgItem(m_hWnd, m_items[i].uId);
			::ShowWindow(hwndChild, bShow ? SW_SHOW : SW_HIDE);
		}
	}

protected:
	int CalcBottomRight(UINT uId, UINT brm, int br, int iTopLeft, int iClientBottomRight, int o, BOOL bBottom)
	{
		if(bBottom ? !(o & LAYOUT_BFORCEID) : !(o & LAYOUT_RFORCEID))
		{
			if(brm == LAYOUT_BORDER)
				return iClientBottomRight - br;

			if(brm == LAYOUT_KEEPSIZE)
				return iTopLeft + br;
		}

		RECT rc2;
		HWND hwndChild = ::GetDlgItem(m_hWnd, brm);
		::GetWindowRect(hwndChild, &rc2);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc2, 2);
		return (bBottom ? rc2.top : rc2.left) - br;
	}

	static BOOL GetChildRect(HWND hWnd, UINT uId, PRECT prc)
	{
		if(prc == NULL)
			return FALSE;

		HWND hwndChild = ::GetDlgItem(hWnd, uId);
		if(hwndChild == NULL)
			return FALSE;

		if(!::GetWindowRect(hwndChild, prc))
			return FALSE;

		return ::MapWindowPoints(NULL, hWnd, (LPPOINT)prc, 2) != 0;
	}

	BOOL GetChildRect(UINT uId, PRECT prc)
	{
		return GetChildRect(m_hWnd, uId, prc);
	}
};

}; // namespace TWL

#endif // __LYT_H__
