//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __AXHOST_H__
#define __AXHOST_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

#include <ocidl.h>

namespace TWL
{

#if !defined(_DEBUG) && !defined(DEBUG) && !defined(_DBG) && !defined(DBG)
#define Message(x)
#else // _DEBUG
#define Message(x) MessageBox(NULL, x, x, MB_OK)
#endif // _DEBUG

#define AlertNotImpl(x) Message(x); return E_NOTIMPL

class CHost;
class IOleClientSiteImpl;
class IOleInPlaceSiteImpl;
class IDispatchImpl;

class CHost : public IUnknown
{
public:
    CHost(HWND hwndParent)
	{
		// Initialize COM library
		OleInitialize(NULL);

		// Setup member variables
		m_dwRefCount = 1;
		m_hwndParent = hwndParent;

		m_pObject = NULL;

		m_pClientSite = NULL;
		m_pInPlaceSite = NULL;
	}

    virtual ~CHost();

	BOOL CreateControl(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);


    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if(ppvObject == NULL)
			return E_INVALIDARG;

		*ppvObject = NULL;

		if(riid == IID_IUnknown)
			*ppvObject = this;
		else if(riid == IID_IOleClientSite && m_pClientSite)
			*ppvObject = m_pClientSite;
		else if(riid == IID_IOleInPlaceSite && m_pInPlaceSite)
			*ppvObject = m_pInPlaceSite;

		if(*ppvObject == NULL)
			return E_NOINTERFACE;

		AddRef();

		return S_OK;
	}

    ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_dwRefCount;
	}

    ULONG STDMETHODCALLTYPE Release()
	{
		DWORD dwRefCount = --m_dwRefCount;

		if(!dwRefCount)
			delete this;

		return dwRefCount;
	}

	DWORD ConnectSink(REFIID iid, LPUNKNOWN punkSink)
	{
		LPCONNECTIONPOINTCONTAINER pConnPtCont;
		if((m_pObject != NULL) && SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pConnPtCont)))
		{
			LPCONNECTIONPOINT pConnPt = NULL;
			DWORD dwCookie = 0;

			if(SUCCEEDED(pConnPtCont->FindConnectionPoint(iid, &pConnPt)))
			{
				pConnPt->Advise(punkSink, &dwCookie);
				pConnPt->Release();
			}

			pConnPtCont->Release();
			return dwCookie;
		}

		return 0;
	}

	void DisconnectSink(REFIID iid, DWORD dwCookie)
	{
		if(dwCookie == 0 || m_pObject == NULL)
			return;

		LPCONNECTIONPOINTCONTAINER pConnPtCont;
		if(SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pConnPtCont)))
		{
			LPCONNECTIONPOINT pConnPt = NULL;
			if(SUCCEEDED(pConnPtCont->FindConnectionPoint(iid, &pConnPt)))
			{
				pConnPt->Unadvise(dwCookie);
				pConnPt->Release();
			}

			pConnPtCont->Release();
		}
	}

    // Classes Implemented
    IOleClientSiteImpl*  m_pClientSite;
    IOleInPlaceSiteImpl* m_pInPlaceSite;

	IOleObject*          m_pObject;

    // Master reference count
    DWORD                m_dwRefCount;

    // Window to host IE in
    HWND                 m_hwndParent;
};

class IOleClientSiteImpl : public IOleClientSite
{
public:
    IOleClientSiteImpl(CHost* pFrameSite)
	{
		m_pHost = pFrameSite;
	}

    ~IOleClientSiteImpl()
	{
	}

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject)
	{
		return m_pHost->QueryInterface(iid, ppvObject);
	}

    ULONG STDMETHODCALLTYPE AddRef()
	{
		return m_pHost->AddRef();
	}

    ULONG STDMETHODCALLTYPE Release()
	{
		return m_pHost->Release();
	}

    // IOleClientSite
    STDMETHODIMP GetContainer(LPOLECONTAINER* ppContainer)
	{
		AlertNotImpl("IOleClientSiteImpl::GetContainer");
	}

    STDMETHODIMP SaveObject()
	{
		AlertNotImpl("IOleClientSiteImpl::SaveObject");
	}

    STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
	{
		AlertNotImpl("IOleClientSiteImpl::GetMoniker");
	}

    STDMETHODIMP ShowObject()
	{
		return S_OK;
	}

    STDMETHODIMP OnShowWindow(BOOL fShow)
	{
		AlertNotImpl("IOleClientSiteImpl::OnShowWindow");
	}

    STDMETHODIMP RequestNewObjectLayout()
	{
		AlertNotImpl("IOleClientSiteImpl::RequestNewObjectLayout");
	}

private:
    CHost* m_pHost;
};

class IOleInPlaceSiteImpl : public IOleInPlaceSite
{
public:
    IOleInPlaceSiteImpl(CHost* pFrameSite)
	{
		m_pHost = pFrameSite;
	}

    ~IOleInPlaceSiteImpl()
	{
	}

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject)
	{
		return m_pHost->QueryInterface(iid, ppvObject);
	}

    ULONG STDMETHODCALLTYPE AddRef()
	{
		return m_pHost->AddRef();
	}

    ULONG STDMETHODCALLTYPE Release()
	{
		return m_pHost->Release();
	}

    // IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow(HWND* phwnd)
	{
		*phwnd = m_pHost->m_hwndParent;
		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::ContextSensitiveHelp");
	}

    // IOleInPlaceSite
    HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void)
	{
		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void)
	{
		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE OnUIActivate(void)
	{
		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame,
                                               IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
                                               LPRECT lprcClipRect,
                                               LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::Scroll");
	}

    HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::OnUIDeactivate");
	}

    HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::OnInPlaceDeactivate");
	}

    HRESULT STDMETHODCALLTYPE DiscardUndoState(void)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::DiscardUndoState");
	}

    HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::DeactivateAndUndo");
	}

    HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect)
	{
		AlertNotImpl("IOleInPlaceSiteImpl::OnPosRectChange");
	}

private:
    CHost* m_pHost;
};

class IDispatchImpl : public IDispatch
{
public:
    IDispatchImpl(CHost* pFrameSite)
	{
		m_pHost = pFrameSite;
	}

    ~IDispatchImpl()
	{
	}

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject)
	{
		return m_pHost->QueryInterface(iid, ppvObject);
	}

    ULONG STDMETHODCALLTYPE AddRef()
	{
		return m_pHost->AddRef();
	}

    ULONG STDMETHODCALLTYPE Release()
	{
		return m_pHost->Release();
	}

    // IDispatch
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int* pctinfo)
	{
		AlertNotImpl("IDispatchImpl::GetTypeInfoCount");
	}

    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID  lcid, ITypeInfo**  ppTInfo)
	{
		AlertNotImpl("IDispatchImpl::GetTypeInfo");
	}

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR** rgszNames,
                                            unsigned int cNames, LCID lcid, DISPID* rgDispId)
	{
		AlertNotImpl("IDispatchImpl::GetIDsOfNames");
	}

    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                                     DISPPARAMS* pDispParams, VARIANT* parResult,
                                     EXCEPINFO* pExcepInfo, unsigned int* puArgErr)
	{
		AlertNotImpl("IDispatchImpl::Invoke");
	}

private:
    CHost* m_pHost;
};


CHost::~CHost()
{
	if(m_pObject)
	{
		m_pObject->SetClientSite(NULL);
		m_pObject->Close(OLECLOSE_NOSAVE);
		m_pObject->Release();
	}

	// Clean up hosted classes
	if(m_pClientSite)
		delete m_pClientSite;

	if(m_pInPlaceSite)
		delete m_pInPlaceSite;

	// Release COM library
	OleUninitialize();
}

inline
BOOL CHost::CreateControl(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext)
{
	if(m_pObject) // An object has been created
		return FALSE;

	// Instantiate requested control
	HRESULT hr = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, IID_IOleObject, (void**)&m_pObject);
	if(!SUCCEEDED(hr))
		return FALSE;

	BOOL bRet = FALSE;

	// Instantiate all needed interfaces
	m_pClientSite = new IOleClientSiteImpl(this);
	m_pInPlaceSite = new IOleInPlaceSiteImpl(this);

	// Set the client site
	hr = m_pObject->SetClientSite(m_pClientSite);
	if(!SUCCEEDED(hr))
		goto exit;

	// Set the site limits
	RECT rc;
	GetClientRect(m_hwndParent, &rc);

	rc.right -= rc.left;
	rc.bottom -= rc.top;
	rc.left = 0;
	rc.top = 0;

	IOleInPlaceObject* pInPlaceObject;
	hr = m_pObject->QueryInterface(IID_IOleInPlaceObject, (void**)(&pInPlaceObject));
	if(!SUCCEEDED(hr))
		goto exit;

	hr = pInPlaceObject->SetObjectRects(&rc, &rc);
	if(!SUCCEEDED(hr))
		goto exit;

	pInPlaceObject->Release();

	// Activate the site
	hr = m_pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, m_pClientSite, 0, m_hwndParent, &rc);
	if(!SUCCEEDED(hr))
		goto exit;

	bRet = TRUE;

exit:
	if(!bRet && m_pObject)
	{
		m_pObject->Close(OLECLOSE_NOSAVE);
		m_pObject->Release();
		m_pObject = NULL;
	}

	return bRet;
}

}; // namespace TWL

#endif // __AXHOST_H__
