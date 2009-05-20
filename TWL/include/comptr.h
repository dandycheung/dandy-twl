//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2005 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __COMPTR_H__
#define __COMPTR_H__

#pragma once

#ifndef __cplusplus
#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

class CComVariantLite : public tagVARIANT
{
	// Constructors
public:
	CComVariantLite()
	{
		vt = VT_EMPTY;
	}

	~CComVariantLite()
	{
		Clear();
	}
	
	CComVariantLite(const VARIANT& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

	// Operations
public:
	HRESULT Clear()
	{
		return ::VariantClear(this);
	}
	
	HRESULT Copy(const VARIANT* pSrc)
	{
		return ::VariantCopy(this, const_cast<VARIANT*>(pSrc));
	}
	
	HRESULT Attach(VARIANT* pSrc)
	{
		// Clear out the variant
		HRESULT hr = Clear();
		if(!FAILED(hr))
		{
			// Copy the contents and give control to CComVariant
			memcpy(this, pSrc, sizeof(VARIANT));
			pSrc->vt = VT_EMPTY;
			hr = S_OK;
		}
		
		return hr;
	}
	
	HRESULT Detach(VARIANT* pDest)
	{
		// Clear out the variant
		HRESULT hr = ::VariantClear(pDest);
		if(!FAILED(hr))
		{
			// Copy the contents and remove control from CComVariant
			memcpy(pDest, this, sizeof(VARIANT));
			vt = VT_EMPTY;
			hr = S_OK;
		}
		
		return hr;
	}
	
	HRESULT ChangeType(VARTYPE vtNew, const VARIANT* pSrc = NULL)
	{
		VARIANT* pVar = const_cast<VARIANT*>(pSrc);
		
		// Convert in place if pSrc is NULL
		if(pVar == NULL)
			pVar = this;
		
		// Do nothing if doing in place convert and vts not different
		return ::VariantChangeType(this, pVar, 0, vtNew);
	}
	
	// Implementation
public:
	HRESULT InternalClear()
	{
		HRESULT hr = Clear();
		ATLASSERT(SUCCEEDED(hr));
		if (FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
		return hr;
	}
	
	void InternalCopy(const VARIANT* pSrc)
	{
		HRESULT hr = Copy(pSrc);
		if (FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
	}
};
	
IUnknown* TwlComPtrAssign(IUnknown** ppUnk, IUnknown* pUnk);
IUnknown* TwlComQIPtrAssign(IUnknown** ppUnk, IUnknown* pUnk, REFIID riid);

HRESULT TwlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw)
HRESULT TwlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw);

inline IUnknown* TwlComPtrAssign(IUnknown** ppUnk, IUnknown* pUnk)
{
	if(ppUnk == NULL)
		return NULL;

	if(pUnk != NULL)
		pUnk->AddRef();

	if(*ppUnk)
		(*ppUnk)->Release();

	*ppUnk = pUnk;
	return pUnk;
}

inline IUnknown* TwlComQIPtrAssign(IUnknown** ppUnk, IUnknown* pUnk, REFIID riid)
{
	if(ppUnk == NULL)
		return NULL;

	IUnknown* pTemp = *ppUnk;
	*ppUnk = NULL;

	if(pUnk != NULL)
		pUnk->QueryInterface(riid, (void**)ppUnk);

	if(pTemp)
		pTemp->Release();

	return *ppUnk;
}

template <class T>
class _NoAddRefReleaseOnCComPtr : public T
{
	STDMETHOD_(ULONG, AddRef)() = 0;
	STDMETHOD_(ULONG, Release)() = 0;
};

inline HRESULT TwlSetChildSite(IUnknown* punkChild, IUnknown* punkParent)
{
	if(punkChild == NULL)
		return E_POINTER;

	IObjectWithSite* pChildSite = NULL;
	HRESULT hr = punkChild->QueryInterface(__uuidof(IObjectWithSite), (void**)&pChildSite);
	if(SUCCEEDED(hr) && pChildSite != NULL)
	{
		hr = pChildSite->SetSite(punkParent);
		pChildSite->Release();
	}

	return hr;
}

// CComPtrBase provides the basis for all other smart pointers
// The other smart pointers add their own constructors and operators
template <class T>
class CComPtrBase
{
protected:
	CComPtrBase() throw()
	{
		p = NULL;
	}

	CComPtrBase(int nNull) throw()
	{
		ASSERT(nNull == 0);
		(void)nNull;
		p = NULL;
	}

	CComPtrBase(T* lp) throw()
	{
		p = lp;
		if(p != NULL)
			p->AddRef();
	}

public:
	typedef T _PtrClass;

	~CComPtrBase() throw()
	{
		if(p)
			p->Release();
	}

	operator T*() const throw()
	{
		return p;
	}

	T& operator*() const
	{
		ATLENSURE(p != NULL);
		return *p;
	}

	// The assert on operator& usually indicates a bug.  If this is really
	// what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		ASSERT(p == NULL);
		return &p;
	}

	_NoAddRefReleaseOnCComPtr<T>* operator->() const throw()
	{
		ASSERT(p != NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}

	bool operator!() const throw()
	{
		return (p == NULL);
	}

	bool operator<(T* pT) const throw()
	{
		return p < pT;
	}

	bool operator!=(T* pT) const
	{
		return !operator==(pT);
	}

	bool operator==(T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if(pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}

	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther) throw()
	{
		if(p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if(p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;

		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);

		return punk1 == punk2;
	}

	// Attach to an existing interface (does not AddRef)
	void Attach(T* p2) throw()
	{
		if(p)
			p->Release();

		p = p2;
	}

	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;

		return pt;
	}

	HRESULT CopyTo(T** ppT) throw()
	{
		ASSERT(ppT != NULL);
		if(ppT == NULL)
			return E_POINTER;

		*ppT = p;
		if(p)
			p->AddRef();

		return S_OK;
	}

	HRESULT SetSite(IUnknown* punkParent) throw()
	{
		return TwlSetChildSite(p, punkParent);
	}

	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw) throw()
	{
		return TwlAdvise(p, pUnk, iid, pdw);
	}

	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}

	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ASSERT(p == NULL);

		CLSID clsid;
		HRESULT hr = ::CLSIDFromProgID(szProgID, &clsid);
		if(SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);

		return hr;
	}

	template <class Q>
	HRESULT QueryInterface(Q** pp) const throw()
	{
		ASSERT(pp != NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}

	T* p;
};

template <class T>
class CComPtr : public CComPtrBase<T>
{
public:
	CComPtr() throw()
	{
	}

	CComPtr(int nNull) throw() :
		CComPtrBase<T>(nNull)
	{
	}

	CComPtr(T* lp) throw() :
		CComPtrBase<T>(lp)
	{
	}

	CComPtr(const CComPtr<T>& lp) throw() :
		CComPtrBase<T>(lp.p)
	{
	}

	T* operator=(T* lp) throw()
	{
        if(*this != lp)
    		return static_cast<T*>(TwlComPtrAssign((IUnknown**)&p, lp));

        return *this;
	}
	
	template <typename Q>
	T* operator=(const CComPtr<Q>& lp) throw()
	{
        if(!IsEqualObject(lp))
    		return static_cast<T*>(TwlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(T)));

        return *this;
	}
	
	T* operator=(const CComPtr<T>& lp) throw()
	{
        if(*this != lp)
    		return static_cast<T*>(TwlComPtrAssign((IUnknown**)&p, lp));

        return *this;
	}
};

// specialization for IDispatch
template <>
class CComPtr<IDispatch> : public CComPtrBase<IDispatch>
{
public:
	CComPtr() throw()
	{
	}

	CComPtr(IDispatch* lp) throw() :
		CComPtrBase<IDispatch>(lp)
	{
	}

	CComPtr(const CComPtr<IDispatch>& lp) throw() :
		CComPtrBase<IDispatch>(lp.p)
	{
	}

	IDispatch* operator=(IDispatch* lp) throw()
	{
        if(*this != lp)
            return static_cast<IDispatch*>(TwlComPtrAssign((IUnknown**)&p, lp));

        return *this;
	}
	
	IDispatch* operator=(const CComPtr<IDispatch>& lp) throw()
	{
        if(*this != lp)
    		return static_cast<IDispatch*>(TwlComPtrAssign((IUnknown**)&p, lp.p));

        return *this;
	}

// IDispatch specific stuff
	HRESULT GetPropertyByName(LPCOLESTR lpsz, VARIANT* pVar) throw()
	{
		ASSERT(p);
		ASSERT(pVar);

		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if(SUCCEEDED(hr))
			hr = GetProperty(dwDispID, pVar);

		return hr;
	}

	HRESULT GetProperty(DISPID dwDispID, VARIANT* pVar) throw()
	{
		return GetProperty(p, dwDispID, pVar);
	}

	HRESULT PutPropertyByName(LPCOLESTR lpsz, VARIANT* pVar) throw()
	{
		ASSERT(p);
		ASSERT(pVar);

		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if(SUCCEEDED(hr))
			hr = PutProperty(dwDispID, pVar);

		return hr;
	}

	HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar) throw()
	{
		return PutProperty(p, dwDispID, pVar);
	}

	HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid) throw()
	{
		return p->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&lpsz), 1, LOCALE_USER_DEFAULT, pdispid);
	}

	// Invoke a method by DISPID with no parameters
	HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { NULL, NULL, 0, 0 };
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}

	// Invoke a method by name with no parameters
	HRESULT Invoke0(LPCOLESTR lpszName, VARIANT* pvarRet = NULL) throw()
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if(SUCCEEDED(hr))
			hr = Invoke0(dispid, pvarRet);

		return hr;
	}

	// Invoke a method by DISPID with a single parameter
	HRESULT Invoke1(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0 };
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}

	// Invoke a method by name with a single parameter
	HRESULT Invoke1(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarRet = NULL) throw()
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if(SUCCEEDED(hr))
			hr = Invoke1(dispid, pvarParam1, pvarRet);

		return hr;
	}

	// Invoke a method by DISPID with two parameters
	HRESULT Invoke2(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL) throw()
	{
		if(pvarParam1 == NULL || pvarParam2 == NULL)
			return E_INVALIDARG;
		
		CComVariantLite varArgs[2] = { *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 2, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}

	// Invoke a method by name with two parameters
	HRESULT Invoke2(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL) throw()
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if(SUCCEEDED(hr))
			hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);

		return hr;
	}

	// Invoke a method by DISPID with N parameters
	HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0 };
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}

	// Invoke a method by name with N parameters
	HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if(SUCCEEDED(hr))
			hr = InvokeN(dispid, pvarParams, nParams, pvarRet);

		return hr;
	}

	static HRESULT PutProperty(IDispatch* p, DISPID dwDispID, VARIANT* pVar) throw()
	{
		ASSERT(p);
		ASSERT(pVar != NULL);

		if(pVar == NULL)
			return E_POINTER;

		if(p == NULL)
			return E_INVALIDARG;

		DISPPARAMS dispparams = { NULL, NULL, 1, 1 };
		dispparams.rgvarg = pVar;

		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		if(pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || (pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
		{
			HRESULT hr = p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF, &dispparams, NULL, NULL, NULL);
			if(SUCCEEDED(hr))
				return hr;
		}

		return p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams, NULL, NULL, NULL);
	}

	static HRESULT GetProperty(IDispatch* p, DISPID dwDispID, VARIANT* pVar) throw()
	{
		ASSERT(p);
		ASSERT(pVar != NULL);

		if(pVar == NULL)
			return E_POINTER;
		
		if(p == NULL)
			return E_INVALIDARG;
	
		DISPPARAMS dispparamsNoArgs = { NULL, NULL, 0, 0 };
		return p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, pVar, NULL, NULL);
	}
};

template <class T, const IID* piid = &__uuidof(T)>
class CComQIPtr : public CComPtr<T>
{
public:
	CComQIPtr() throw()
	{
	}

	CComQIPtr(T* lp) throw() :
		CComPtr<T>(lp)
	{
	}

	CComQIPtr(const CComQIPtr<T,piid>& lp) throw() :
		CComPtr<T>(lp.p)
	{
	}

	CComQIPtr(IUnknown* lp) throw()
	{
		if(lp != NULL)
			lp->QueryInterface(*piid, (void**)&p);
	}

	T* operator=(T* lp) throw()
	{
        if(*this != lp)
		    return static_cast<T*>(TwlComPtrAssign((IUnknown**)&p, lp));

        return *this;
	}

	T* operator=(const CComQIPtr<T, piid>& lp) throw()
	{
        if(*this != lp)
    		return static_cast<T*>(TwlComPtrAssign((IUnknown**)&p, lp.p));

        return *this;
	}

	T* operator=(IUnknown* lp) throw()
	{
        if(*this != lp)
    		return static_cast<T*>(TwlComQIPtrAssign((IUnknown**)&p, lp, *piid));

        return *this;
	}
};

// Specialization to make it work
template<>
class CComQIPtr<IUnknown, &IID_IUnknown> : public CComPtr<IUnknown>
{
public:
	CComQIPtr() throw()
	{
	}

	CComQIPtr(IUnknown* lp) throw()
	{
		// Actually do a QI to get identity
		if(lp != NULL)
			lp->QueryInterface(__uuidof(IUnknown), (void**)&p);
	}

	CComQIPtr(const CComQIPtr<IUnknown, &IID_IUnknown>& lp) throw() :
		CComPtr<IUnknown>(lp.p)
	{
	}

	IUnknown* operator=(IUnknown* lp) throw()
	{
		// Actually do a QI to get identity
        if(*this != lp)
		    return TwlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(IUnknown));

        return *this;
	}

	IUnknown* operator=(const CComQIPtr<IUnknown, &IID_IUnknown>& lp) throw()
	{
        if(*this != lp)
    		return TwlComPtrAssign((IUnknown**)&p, lp.p);

        return *this;
	}
};

typedef CComQIPtr<IDispatch, &__uuidof(IDispatch)> CComDispatchDriver;

inline HRESULT TwlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw)
{
	if(pUnkCP == NULL)
		return E_INVALIDARG;

	CComPtr<IConnectionPointContainer> pCPC;
	CComPtr<IConnectionPoint> pCP;

	HRESULT hr = pUnkCP->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
	if(SUCCEEDED(hr))
		hr = pCPC->FindConnectionPoint(iid, &pCP);

	if(SUCCEEDED(hr))
		hr = pCP->Advise(pUnk, pdw);

	return hr;
}

inline HRESULT TwlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw)
{
	if(pUnkCP == NULL)
		return E_INVALIDARG;
	
	CComPtr<IConnectionPointContainer> pCPC;
	CComPtr<IConnectionPoint> pCP;

	HRESULT hr = pUnkCP->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
	if(SUCCEEDED(hr))
		hr = pCPC->FindConnectionPoint(iid, &pCP);

	if(SUCCEEDED(hr))
		hr = pCP->Unadvise(dw);

	return hr;
}

}; // namespace TWL

#endif // __COMPTR_H__
