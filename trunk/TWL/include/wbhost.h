//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __WBHOST_H__
#define __WBHOST_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <exdispid.h>
#include <exdisp.h>	// for WebBrowser related stuff

#include "axhost.h"

namespace TWL
{

class CDWebBrowserEvents2 : public IDispatchImpl
{
	typedef IDispatchImpl super;

public:
    CDWebBrowserEvents2(CHost* pFrameSite) : super(pFrameSite)
	{
	}

    ~CDWebBrowserEvents2()
	{
	}

    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                                     DISPPARAMS* pDispParams, VARIANT* parResult,
                                     EXCEPINFO* pExcepInfo, unsigned int* puArgErr)
	{
		if(dispIdMember == DISPID_DOCUMENTCOMPLETE)
		{
			if(pDispParams->rgvarg[0].pvarVal->vt & VT_BYREF)
				MessageBoxW(NULL, (LPCWSTR)*pDispParams->rgvarg[0].pvarVal->pbstrVal, (LPCWSTR)*pDispParams->rgvarg[0].pvarVal->pbstrVal, MB_OK);
			else
				MessageBoxW(NULL, (LPCWSTR)pDispParams->rgvarg[0].pvarVal->bstrVal, (LPCWSTR)pDispParams->rgvarg[0].pvarVal->bstrVal, MB_OK);
		}

		return E_NOTIMPL;
	}

private:
    CHost* m_pHost;
};

class CWebBrowserHost : public CHost
{
	typedef CHost super;

public:
	CWebBrowserHost(HWND hwndParent) : super(hwndParent)
	{
		m_pSink = NULL;
		m_dwCookie = 0;
	}

	~CWebBrowserHost()
	{
		Unadvise();
	}

    STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject)
	{
		if(ppvObject == NULL)
			return E_INVALIDARG;

		*ppvObject = NULL;

		if(iid == IID_IDispatch || iid == DIID_DWebBrowserEvents2 && m_pSink)
			*ppvObject = m_pSink;

		if(*ppvObject)
		{
			AddRef();
			return S_OK;
		}

		return super::QueryInterface(iid, ppvObject);
	}

	BOOL CreateWebBrowser(BOOL bAdvise = FALSE)
	{
		BOOL br = CreateControl(CLSID_WebBrowser, NULL, CLSCTX_INPROC);
		if(br && bAdvise)
			Advise();

		return br;
	}

	IWebBrowser2* QueryWebBrowser2()
	{
		if(!m_pObject)
			return NULL;

		IWebBrowser2* pWB2;
		HRESULT hr = m_pObject->QueryInterface(IID_IWebBrowser2, (void**)&pWB2);
		if(SUCCEEDED(hr))
			return pWB2;

		return NULL;
	}

	// NOTE: if you wanna provide another sink implemented by yourself, override
	// this method and create a new class derived from CDWebBrowserEvents2
	virtual IDispatchImpl* CreateSink()
	{
		return new CDWebBrowserEvents2(this);
	}

	BOOL Advise()
	{
		m_pSink = CreateSink(); 
		if(m_pSink)
			m_dwCookie = ConnectSink(DIID_DWebBrowserEvents2, m_pSink);

		return m_dwCookie != 0;
	}

	void Unadvise()
	{
		if(m_dwCookie)
		{
			DisconnectSink(DIID_DWebBrowserEvents2, m_dwCookie);
			m_dwCookie = 0;
		}
	}

private:
	IDispatchImpl* m_pSink;
	DWORD m_dwCookie;
};

}; // namespace TWL

#endif // __WBHOST_H__
