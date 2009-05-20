//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __MSGMAP_H__
#define __MSGMAP_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

#define BEGIN_MSG_MAP() \
public: \
	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult) \
	{ \
		bool bHandled = false; \
		uMsg; \
		wParam; \
		lParam; \
		lResult; \
		bHandled;

#define MESSAGE_HANDLER(msg, func) \
	if(uMsg == msg) \
	{ \
		bHandled = func(uMsg, wParam, lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define MESSAGE_RANGE_HANDLER(msgFirst, msgLast, func) \
	if(uMsg >= msgFirst && uMsg <= msgLast) \
	{ \
		bHandled = func(uMsg, wParam, lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define COMMAND_HANDLER(id, code, func) \
	if(uMsg == WM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam)) \
	{ \
		bHandled = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define COMMAND_ID_HANDLER(id, func) \
	if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
	{ \
		bHandled = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define COMMAND_CODE_HANDLER(code, func) \
	if(uMsg == WM_COMMAND && code == HIWORD(wParam)) \
	{ \
		bHandled = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define COMMAND_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast) \
	{ \
		bHandled = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define NOTIFY_HANDLER(id, cd, func) \
	if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = func((int)wParam, (LPNMHDR)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define NOTIFY_ID_HANDLER(id, func) \
	if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		bHandled = func((int)wParam, (LPNMHDR)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define NOTIFY_CODE_HANDLER(cd, func) \
	if(uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = func((int)wParam, (LPNMHDR)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define NOTIFY_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		bHandled = func((int)wParam, (LPNMHDR)lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define CHAIN_MSG_MAP(theChainClass) \
	{ \
		if(theChainClass::ProcessMessage(uMsg, wParam, lParam, lResult)) \
			return true; \
	}

#define CHAIN_MSG_MAP_MEMBER(theChainMember) \
	{ \
		if(theChainMember.ProcessMessage(uMsg, wParam, lParam, lResult)) \
			return true; \
	}

#define END_MSG_MAP() \
		return false; \
	}


// Handler prototypes:
//  bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
//  bool CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, LRESULT& lResult);
//  bool NotifyHandler(int idCtrl, LPNMHDR pnmh, LRESULT& lResult);


// Empty message map macro

#define DECLARE_EMPTY_MSG_MAP() \
public: \
	bool ProcessMessage(UINT, WPARAM, LPARAM, LRESULT&) \
	{ \
		return false; \
	}

// Message reflection macros

#define REFLECT_NOTIFICATIONS() \
	{ \
		bHandled = ReflectNotifications(uMsg, wParam, lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define DEFAULT_REFLECTION_HANDLER() \
	if(DefaultReflectionHandler(hWnd, uMsg, wParam, lParam, lResult)) \
		return true;

// forward notifications support
#define FORWARD_NOTIFICATIONS() \
	{ \
		bHandled = ForwardNotifications(uMsg, wParam, lParam, lResult); \
		if(bHandled) \
			return true; \
	}

#define BEGIN_REFLECTION_MAP() \
	{ \
		UINT uMsgAlt = uMsg - OCM__BASE; \
		{ \
			UINT uMsg = uMsgAlt;

#define END_REFLECTION_MAP() \
		} \
	}

#ifndef _AFX
#define BEGIN_MESSAGE_MAP	BEGIN_MSG_MAP
#define END_MESSAGE_MAP		END_MSG_MAP
#endif // _AFX

} // namespace TWL

#endif // __MSGMAP_H__
