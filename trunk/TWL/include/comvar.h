#ifndef __COMVAR_H__
#define __COMVAR_H__

/////////////////////////////////////////////////////////////////////////////
// BSTR related classes and inline functions

class CComBSTRLite
{
	BSTR m_bstr;

public:
	CComBSTRLite()
		: m_bstr(NULL)
	{
	}

	~CComBSTRLite()
	{
		::SysFreeString(m_bstr);
	}
	
	operator BSTR() const throw()
	{
		return m_bstr;
	}
	
	void Attach(BSTR bstr) throw()
	{
		if(m_bstr != bstr)
		{
			::SysFreeString(m_bstr);
			m_bstr = bstr;
		}
	}
	
	BSTR Detach() throw()
	{
		BSTR bstr = m_bstr;
		m_bstr = NULL;
		return bstr;
	}

	void Empty() throw()
	{
		::SysFreeString(m_bstr);
		m_bstr = NULL;
	}

	bool operator!() const throw()
	{
		return m_bstr == NULL;
	}
};

inline ULONG SysGetStringStreamSize(BSTR bstr)
{
	ULONG ulSize = sizeof(ULONG);
	if(bstr != NULL)
		ulSize += SysStringByteLen(bstr) + sizeof(OLECHAR);
	
	return ulSize;
}

inline HRESULT SysWriteStringToStream(BSTR bstr, IStream* pStream)
{
	ASSERT(pStream != NULL);
	if(pStream == NULL)
		return E_INVALIDARG;
	
	ULONG cb;
	ULONG cbStrLen = SysGetStringStreamSize(bstr) - sizeof(ULONG);
	HRESULT hr = pStream->Write((void*)&cbStrLen, sizeof(cbStrLen), &cb);
	if(FAILED(hr))
		return hr;
	
	return cbStrLen ? pStream->Write((void*)bstr, cbStrLen, &cb) : S_OK;
}

inline HRESULT SysReadStringFromStream(BSTR& bstr, IStream* pStream)
{
	ASSERT(pStream != NULL);
	if(pStream == NULL)
		return E_INVALIDARG;
	
	ASSERT(!bstr); // should be empty
	
	::SysFreeString(bstr);
	bstr = NULL;
	
	ULONG cbStrLen = 0;
	HRESULT hr = pStream->Read((void*) &cbStrLen, sizeof(cbStrLen), NULL);
	if((hr == S_OK) && (cbStrLen != 0))
	{
		// subtract size for terminating NULL which we wrote out
		// since SysAllocStringByteLen over-allocates for the NULL
		bstr = SysAllocStringByteLen(NULL, cbStrLen - sizeof(OLECHAR));
		if(!bstr)
			hr = E_OUTOFMEMORY;
		else
			hr = pStream->Read((void*)bstr, cbStrLen, NULL);
		
		// If SysAllocStringByteLen or IStream::Read failed, reset seek 
		// pointer to start of BSTR size.
		if(hr != S_OK)
		{
			LARGE_INTEGER nOffset;
			nOffset.QuadPart = -(static_cast<LONGLONG>(sizeof(cbStrLen)));
			pStream->Seek(nOffset, STREAM_SEEK_CUR, NULL);
		}
	}
	
	if(hr == S_FALSE)
		hr = E_FAIL;
	
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CComVariant

#define TWL_VARIANT_TRUE	VARIANT_BOOL(-1)
#define TWL_VARIANT_FALSE	VARIANT_BOOL(0)

template<typename T>
class CVarTypeInfo
{
//	static const VARTYPE VT;  // VARTYPE corresponding to type T
//	static T VARIANT::* const pmField;  // Pointer-to-member of corresponding field in VARIANT struct
};

template<>
class CVarTypeInfo<char>
{
public:
	static const VARTYPE VT = VT_I1;
	static char VARIANT::* const pmField;
};

__declspec(selectany) char VARIANT::* const CVarTypeInfo<char>::pmField = &VARIANT::cVal;

template<>
class CVarTypeInfo<unsigned char>
{
public:
	static const VARTYPE VT = VT_UI1;
	static unsigned char VARIANT::* const pmField;
};

__declspec(selectany) unsigned char VARIANT::* const CVarTypeInfo<unsigned char>::pmField = &VARIANT::bVal;

template<>
class CVarTypeInfo<char*>
{
public:
	static const VARTYPE VT = VT_I1 | VT_BYREF;
	static char* VARIANT::* const pmField;
};

__declspec(selectany) char* VARIANT::* const CVarTypeInfo<char*>::pmField = &VARIANT::pcVal;

template<>
class CVarTypeInfo<unsigned char*>
{
public:
	static const VARTYPE VT = VT_UI1 | VT_BYREF;
	static unsigned char* VARIANT::* const pmField;
};

__declspec(selectany) unsigned char* VARIANT::* const CVarTypeInfo<unsigned char*>::pmField = &VARIANT::pbVal;

template<>
class CVarTypeInfo<short>
{
public:
	static const VARTYPE VT = VT_I2;
	static short VARIANT::* const pmField;
};

__declspec(selectany) short VARIANT::* const CVarTypeInfo<short>::pmField = &VARIANT::iVal;

template<>
class CVarTypeInfo<short*>
{
public:
	static const VARTYPE VT = VT_I2 | VT_BYREF;
	static short* VARIANT::* const pmField;
};

__declspec(selectany) short* VARIANT::* const CVarTypeInfo<short*>::pmField = &VARIANT::piVal;

template<>
class CVarTypeInfo<unsigned short>
{
public:
	static const VARTYPE VT = VT_UI2;
	static unsigned short VARIANT::* const pmField;
};

__declspec(selectany) unsigned short VARIANT::* const CVarTypeInfo<unsigned short>::pmField = &VARIANT::uiVal;

#ifdef _NATIVE_WCHAR_T_DEFINED  // Only treat unsigned short* as VT_UI2 | VT_BYREF if BSTR isn't the same as unsigned short*
template<>
class CVarTypeInfo<unsigned short*>
{
public:
	static const VARTYPE VT = VT_UI2 | VT_BYREF;
	static unsigned short* VARIANT::* const pmField;
};

__declspec(selectany) unsigned short* VARIANT::* const CVarTypeInfo<unsigned short*>::pmField = &VARIANT::puiVal;
#endif  // _NATIVE_WCHAR_T_DEFINED

template<>
class CVarTypeInfo<int>
{
public:
	static const VARTYPE VT = VT_I4;
	static int VARIANT::* const pmField;
};

__declspec(selectany) int VARIANT::* const CVarTypeInfo<int>::pmField = &VARIANT::intVal;

template<>
class CVarTypeInfo<int*>
{
public:
	static const VARTYPE VT = VT_I4 | VT_BYREF;
	static int* VARIANT::* const pmField;
};

__declspec(selectany) int* VARIANT::* const CVarTypeInfo<int*>::pmField = &VARIANT::pintVal;

template<>
class CVarTypeInfo<unsigned int>
{
public:
	static const VARTYPE VT = VT_UI4;
	static unsigned int VARIANT::* const pmField;
};

__declspec(selectany) unsigned int VARIANT::* const CVarTypeInfo<unsigned int>::pmField = &VARIANT::uintVal;

template<>
class CVarTypeInfo<unsigned int*>
{
public:
	static const VARTYPE VT = VT_UI4 | VT_BYREF;
	static unsigned int* VARIANT::* const pmField;
};

__declspec(selectany) unsigned int* VARIANT::* const CVarTypeInfo<unsigned int*>::pmField = &VARIANT::puintVal;

template<>
class CVarTypeInfo<long>
{
public:
	static const VARTYPE VT = VT_I4;
	static long VARIANT::* const pmField;
};

__declspec(selectany) long VARIANT::* const CVarTypeInfo<long>::pmField = &VARIANT::lVal;

template<>
class CVarTypeInfo<long*>
{
public:
	static const VARTYPE VT = VT_I4 | VT_BYREF;
	static long* VARIANT::* const pmField;
};

__declspec(selectany) long* VARIANT::* const CVarTypeInfo<long*>::pmField = &VARIANT::plVal;

template<>
class CVarTypeInfo<unsigned long>
{
public:
	static const VARTYPE VT = VT_UI4;
	static unsigned long VARIANT::* const pmField;
};

__declspec(selectany) unsigned long VARIANT::* const CVarTypeInfo<unsigned long>::pmField = &VARIANT::ulVal;

template<>
class CVarTypeInfo<unsigned long*>
{
public:
	static const VARTYPE VT = VT_UI4 | VT_BYREF;
	static unsigned long* VARIANT::* const pmField;
};

__declspec(selectany) unsigned long* VARIANT::* const CVarTypeInfo<unsigned long*>::pmField = &VARIANT::pulVal;

template<>
class CVarTypeInfo<__int64>
{
public:
	static const VARTYPE VT = VT_I8;
	static __int64 VARIANT::* const pmField;
};

__declspec(selectany) __int64 VARIANT::* const CVarTypeInfo<__int64>::pmField = &VARIANT::llVal;

template<>
class CVarTypeInfo<__int64*>
{
public:
	static const VARTYPE VT = VT_I8 | VT_BYREF;
	static __int64* VARIANT::* const pmField;
};

__declspec(selectany) __int64* VARIANT::* const CVarTypeInfo<__int64*>::pmField = &VARIANT::pllVal;

template<>
class CVarTypeInfo<unsigned __int64>
{
public:
	static const VARTYPE VT = VT_UI8;
	static unsigned __int64 VARIANT::* const pmField;
};

__declspec(selectany) unsigned __int64 VARIANT::* const CVarTypeInfo<unsigned __int64>::pmField = &VARIANT::ullVal;

template<>
class CVarTypeInfo<unsigned __int64*>
{
public:
	static const VARTYPE VT = VT_UI8 | VT_BYREF;
	static unsigned __int64* VARIANT::* const pmField;
};

__declspec(selectany) unsigned __int64* VARIANT::* const CVarTypeInfo<unsigned __int64*>::pmField = &VARIANT::pullVal;

template<>
class CVarTypeInfo<float>
{
public:
	static const VARTYPE VT = VT_R4;
	static float VARIANT::* const pmField;
};

__declspec(selectany) float VARIANT::* const CVarTypeInfo<float>::pmField = &VARIANT::fltVal;

template<>
class CVarTypeInfo<float*>
{
public:
	static const VARTYPE VT = VT_R4 | VT_BYREF;
	static float* VARIANT::* const pmField;
};

__declspec(selectany) float* VARIANT::* const CVarTypeInfo<float*>::pmField = &VARIANT::pfltVal;

template<>
class CVarTypeInfo<double>
{
public:
	static const VARTYPE VT = VT_R8;
	static double VARIANT::* const pmField;
};

__declspec(selectany) double VARIANT::* const CVarTypeInfo<double>::pmField = &VARIANT::dblVal;

template<>
class CVarTypeInfo<double*>
{
public:
	static const VARTYPE VT = VT_R8 | VT_BYREF;
	static double* VARIANT::* const pmField;
};

__declspec(selectany) double* VARIANT::* const CVarTypeInfo<double*>::pmField = &VARIANT::pdblVal;

template<>
class CVarTypeInfo<VARIANT>
{
public:
	static const VARTYPE VT = VT_VARIANT;
};

template<>
class CVarTypeInfo<BSTR>
{
public:
	static const VARTYPE VT = VT_BSTR;
	static BSTR VARIANT::* const pmField;
};

__declspec(selectany) BSTR VARIANT::* const CVarTypeInfo<BSTR>::pmField = &VARIANT::bstrVal;

template<>
class CVarTypeInfo<BSTR*>
{
public:
	static const VARTYPE VT = VT_BSTR | VT_BYREF;
	static BSTR* VARIANT::* const pmField;
};

__declspec(selectany) BSTR* VARIANT::* const CVarTypeInfo<BSTR*>::pmField = &VARIANT::pbstrVal;

template<>
class CVarTypeInfo<IUnknown*>
{
public:
	static const VARTYPE VT = VT_UNKNOWN;
	static IUnknown* VARIANT::* const pmField;
};

__declspec(selectany) IUnknown* VARIANT::* const CVarTypeInfo<IUnknown*>::pmField = &VARIANT::punkVal;

template<>
class CVarTypeInfo<IUnknown**>
{
public:
	static const VARTYPE VT = VT_UNKNOWN | VT_BYREF;
	static IUnknown** VARIANT::* const pmField;
};

__declspec(selectany) IUnknown** VARIANT::* const CVarTypeInfo<IUnknown**>::pmField = &VARIANT::ppunkVal;

template<>
class CVarTypeInfo<IDispatch*>
{
public:
	static const VARTYPE VT = VT_DISPATCH;
	static IDispatch* VARIANT::* const pmField;
};

__declspec(selectany) IDispatch* VARIANT::* const CVarTypeInfo<IDispatch*>::pmField = &VARIANT::pdispVal;

template<>
class CVarTypeInfo<IDispatch**>
{
public:
	static const VARTYPE VT = VT_DISPATCH | VT_BYREF;
	static IDispatch** VARIANT::* const pmField;
};

__declspec(selectany) IDispatch** VARIANT::* const CVarTypeInfo<IDispatch**>::pmField = &VARIANT::ppdispVal;

template<>
class CVarTypeInfo<CY>
{
public:
	static const VARTYPE VT = VT_CY;
	static CY VARIANT::* const pmField;
};

__declspec(selectany) CY VARIANT::* const CVarTypeInfo<CY>::pmField = &VARIANT::cyVal;

template<>
class CVarTypeInfo<CY*>
{
public:
	static const VARTYPE VT = VT_CY | VT_BYREF;
	static CY* VARIANT::* const pmField;
};

__declspec(selectany) CY* VARIANT::* const CVarTypeInfo<CY*>::pmField = &VARIANT::pcyVal;

class CComVariant : public tagVARIANT
{
// Constructors
public:
	CComVariant() throw()
	{
		::VariantInit(this);
	}

	~CComVariant() throw()
	{
		Clear();
	}

	CComVariant(const VARIANT& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

	CComVariant(const CComVariant& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

	CComVariant(LPCOLESTR lpszSrc)
	{
		vt = VT_EMPTY;
		*this = lpszSrc;
	}

	CComVariant(LPCSTR lpszSrc)
	{
		vt = VT_EMPTY;
		*this = lpszSrc;
	}

	CComVariant(bool bSrc)
	{
		vt = VT_BOOL;
		boolVal = bSrc ? TWL_VARIANT_TRUE : TWL_VARIANT_FALSE;
	}

	CComVariant(int nSrc, VARTYPE vtSrc = VT_I4) throw()
	{
		ASSERT(vtSrc == VT_I4 || vtSrc == VT_INT);
		vt = vtSrc;
		intVal = nSrc;
	}

	CComVariant(BYTE nSrc) throw()
	{
		vt = VT_UI1;
		bVal = nSrc;
	}

	CComVariant(short nSrc) throw()
	{
		vt = VT_I2;
		iVal = nSrc;
	}

	CComVariant(long nSrc, VARTYPE vtSrc = VT_I4) throw()
	{
		ASSERT(vtSrc == VT_I4 || vtSrc == VT_ERROR);
		vt = vtSrc;
		lVal = nSrc;
	}

	CComVariant(float fltSrc) throw()
	{
		vt = VT_R4;
		fltVal = fltSrc;
	}

	CComVariant(double dblSrc, VARTYPE vtSrc = VT_R8) throw()
	{
		ASSERT(vtSrc == VT_R8 || vtSrc == VT_DATE);
		vt = vtSrc;
		dblVal = dblSrc;
	}

#if (_WIN32_WINNT >= 0x0501) || defined(_TWL_SUPPORT_VT_I8)
	CComVariant(LONGLONG nSrc) throw()
	{
		vt = VT_I8;
		llVal = nSrc;
	}

	CComVariant(ULONGLONG nSrc) throw()
	{
		vt = VT_UI8;
		ullVal = nSrc;
	}
#endif

	CComVariant(CY cySrc) throw()
	{
		vt = VT_CY;
		cyVal.Hi = cySrc.Hi;
		cyVal.Lo = cySrc.Lo;
	}

	CComVariant(IDispatch* pSrc) throw()
	{
		vt = VT_DISPATCH;
		pdispVal = pSrc;

		// Need to AddRef as VariantClear will Release
		if(pdispVal != NULL)
			pdispVal->AddRef();
	}

	CComVariant(IUnknown* pSrc) throw()
	{
		vt = VT_UNKNOWN;
		punkVal = pSrc;

		// Need to AddRef as VariantClear will Release
		if(punkVal != NULL)
			punkVal->AddRef();
	}

	CComVariant(char cSrc) throw()
	{
		vt = VT_I1;
		cVal = cSrc;
	}

	CComVariant(unsigned short nSrc) throw()
	{
		vt = VT_UI2;
		uiVal = nSrc;
	}

	CComVariant(unsigned long nSrc) throw()
	{
		vt = VT_UI4;
		ulVal = nSrc;
	}

	CComVariant(unsigned int nSrc, VARTYPE vtSrc = VT_UI4) throw()
	{
		ASSERT(vtSrc == VT_UI4 || vtSrc == VT_UINT);
		vt = vtSrc;
		uintVal= nSrc;
	}

	CComVariant(const SAFEARRAY* pSrc)
	{
		LPSAFEARRAY pCopy;
		if (pSrc != NULL)
		{
			HRESULT hRes = ::SafeArrayCopy((LPSAFEARRAY)pSrc, &pCopy);
			if (SUCCEEDED(hRes) && pCopy != NULL)
			{
                ::ATL::AtlSafeArrayGetActualVartype((LPSAFEARRAY)pSrc, &vt);
				vt |= VT_ARRAY;
				parray = pCopy;
			}
			else
			{
				vt = VT_ERROR;
				scode = hRes;
			}
		}
		else
		{
			vt = VT_EMPTY;
		}
	}

// Assignment Operators
public:
	CComVariant& operator=(const CComVariant& varSrc)
	{
        if(this != &varSrc)
		    InternalCopy(&varSrc);

		return *this;
	}

	CComVariant& operator=(const VARIANT& varSrc)
	{
        if(static_cast<VARIANT*>(this) != &varSrc)
		    InternalCopy(&varSrc);

		return *this;
	}

	CComVariant& operator=(LPCOLESTR lpszSrc)
	{
		Clear();

		vt = VT_BSTR;
		bstrVal = ::SysAllocString(lpszSrc);

		if(bstrVal == NULL && lpszSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}

		return *this;
	}

	/// TODO:
	CComVariant& operator=(LPCSTR lpszSrc)
	{
		USES_CONVERSION_EX;

		Clear();

		vt = VT_BSTR;
		bstrVal = ::SysAllocString(A2COLE_EX(lpszSrc, _ATL_SAFE_ALLOCA_DEF_THRESHOLD));

		if(bstrVal == NULL && lpszSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}

		return *this;
	}

	CComVariant& operator=(bool bSrc)
	{
		if(vt != VT_BOOL)
		{
			Clear();
			vt = VT_BOOL;
		}

		boolVal = bSrc ? TWL_VARIANT_TRUE : TWL_VARIANT_FALSE;
		return *this;
	}

	CComVariant& operator=(int nSrc) throw()
	{
		if(vt != VT_I4)
		{
			Clear();
			vt = VT_I4;
		}

		intVal = nSrc;
		return *this;
	}

	CComVariant& operator=(BYTE nSrc) throw()
	{
		if(vt != VT_UI1)
		{
			Clear();
			vt = VT_UI1;
		}

		bVal = nSrc;
		return *this;
	}

	CComVariant& operator=(short nSrc) throw()
	{
		if(vt != VT_I2)
		{
			Clear();
			vt = VT_I2;
		}

		iVal = nSrc;
		return *this;
	}

	CComVariant& operator=(long nSrc) throw()
	{
		if(vt != VT_I4)
		{
			Clear();
			vt = VT_I4;
		}

		lVal = nSrc;
		return *this;
	}

	CComVariant& operator=(float fltSrc) throw()
	{
		if(vt != VT_R4)
		{
			Clear();
			vt = VT_R4;
		}

		fltVal = fltSrc;
		return *this;
	}

	CComVariant& operator=(double dblSrc) throw()
	{
		if(vt != VT_R8)
		{
			Clear();
			vt = VT_R8;
		}

		dblVal = dblSrc;
		return *this;
	}

	CComVariant& operator=(CY cySrc) throw()
	{
		if(vt != VT_CY)
		{
			Clear();
			vt = VT_CY;
		}

		cyVal.Hi = cySrc.Hi;
		cyVal.Lo = cySrc.Lo;
		return *this;
	}

	CComVariant& operator=(IDispatch* pSrc) throw()
	{
		Clear();

		vt = VT_DISPATCH;
		pdispVal = pSrc;

		// Need to AddRef as VariantClear will Release
		if(pdispVal != NULL)
			pdispVal->AddRef();

		return *this;
	}

	CComVariant& operator=(IUnknown* pSrc) throw()
	{
		Clear();

		vt = VT_UNKNOWN;
		punkVal = pSrc;

		// Need to AddRef as VariantClear will Release
		if(punkVal != NULL)
			punkVal->AddRef();

		return *this;
	}

	CComVariant& operator=(char cSrc) throw()
	{
		if(vt != VT_I1)
		{
			Clear();
			vt = VT_I1;
		}

		cVal = cSrc;
		return *this;
	}

	CComVariant& operator=(unsigned short nSrc) throw()
	{
		if(vt != VT_UI2)
		{
			Clear();
			vt = VT_UI2;
		}

		uiVal = nSrc;
		return *this;
	}

	CComVariant& operator=(unsigned long nSrc) throw()
	{
		if(vt != VT_UI4)
		{
			Clear();
			vt = VT_UI4;
		}

		ulVal = nSrc;
		return *this;
	}

	CComVariant& operator=(unsigned int nSrc) throw()
	{
		if(vt != VT_UI4)
		{
			Clear();
			vt = VT_UI4;
		}

		uintVal= nSrc;
		return *this;
	}

	CComVariant& operator=(BYTE* pbSrc) throw()
	{
		if(vt != (VT_UI1 | VT_BYREF))
		{
			Clear();
			vt = VT_UI1 | VT_BYREF;
		}

		pbVal = pbSrc;
		return *this;
	}

	CComVariant& operator=(short* pnSrc) throw()
	{
		if(vt != (VT_I2 | VT_BYREF))
		{
			Clear();
			vt = VT_I2 | VT_BYREF;
		}

		piVal = pnSrc;
		return *this;
	}

#ifdef _NATIVE_WCHAR_T_DEFINED
	CComVariant& operator=(USHORT* pnSrc) throw()
	{
		if(vt != (VT_UI2 | VT_BYREF))
		{
			Clear();
			vt = VT_UI2 | VT_BYREF;
		}

		puiVal = pnSrc;
		return *this;
	}
#endif

	CComVariant& operator=(int* pnSrc) throw()
	{
		if(vt != (VT_I4 | VT_BYREF))
		{
			Clear();
			vt = VT_I4 | VT_BYREF;
		}

		pintVal = pnSrc;
		return *this;
	}

	CComVariant& operator=(UINT* pnSrc) throw()
	{
		if(vt != (VT_UI4 | VT_BYREF))
		{
			Clear();
			vt = VT_UI4 | VT_BYREF;
		}

		puintVal = pnSrc;
		return *this;
	}

	CComVariant& operator=(long* pnSrc) throw()
	{
		if(vt != (VT_I4 | VT_BYREF))
		{
			Clear();
			vt = VT_I4 | VT_BYREF;
		}

		plVal = pnSrc;
		return *this;
	}

	CComVariant& operator=(ULONG* pnSrc) throw()
	{
		if(vt != (VT_UI4 | VT_BYREF))
		{
			Clear();
			vt = VT_UI4 | VT_BYREF;
		}

		pulVal = pnSrc;
		return *this;
	}

#if (_WIN32_WINNT >= 0x0501) || defined(_TWL_SUPPORT_VT_I8)
	CComVariant& operator=(LONGLONG nSrc) throw()
	{
		if(vt != VT_I8)
		{
			Clear();
			vt = VT_I8;
		}

		llVal = nSrc;
		return *this;
	}

	CComVariant& operator=(LONGLONG* pnSrc) throw()
	{
		if(vt != (VT_I8 | VT_BYREF))
		{
			Clear();
			vt = VT_I8 | VT_BYREF;
		}

		pllVal = pnSrc;
		return *this;
	}

	CComVariant& operator=(ULONGLONG nSrc) throw()
	{
		if(vt != VT_UI8)
		{
			Clear();
			vt = VT_UI8;
		}

		ullVal = nSrc;
		return *this;
	}

	CComVariant& operator=(ULONGLONG* pnSrc) throw()
	{
		if(vt != (VT_UI8 | VT_BYREF))
		{
			Clear();
			vt = VT_UI8 | VT_BYREF;
		}

		pullVal = pnSrc;
		return *this;
	}
#endif

	CComVariant& operator=(float* pfSrc) throw()
	{
		if(vt != (VT_R4 | VT_BYREF))
		{
			Clear();
			vt = VT_R4 | VT_BYREF;
		}

		pfltVal = pfSrc;
		return *this;
	}

	CComVariant& operator=(double* pfSrc) throw()
	{
		if(vt != (VT_R8 | VT_BYREF))
		{
			Clear();
			vt = VT_R8 | VT_BYREF;
		}

		pdblVal = pfSrc;
		return *this;
	}

	CComVariant& operator=(const SAFEARRAY *pSrc)
	{
		Clear();

		LPSAFEARRAY pCopy;
		if(pSrc != NULL)
		{
			HRESULT hRes = ::SafeArrayCopy((LPSAFEARRAY)pSrc, &pCopy);
			if(SUCCEEDED(hRes) && pCopy != NULL)
			{
				::ATL::AtlSafeArrayGetActualVartype((LPSAFEARRAY)pSrc, &vt);
				vt |= VT_ARRAY;
				parray = pCopy;
			}
			else
			{
				vt = VT_ERROR;
				scode = hRes;
			}
		}

		return *this;
	}

// Comparison Operators
public:
	bool operator==(const VARIANT& varSrc) const throw()
	{
		// For backwards compatibility
		if(vt == VT_NULL && varSrc.vt == VT_NULL)
			return true;

		// Variants not equal if types don't match
		if(vt != varSrc.vt)
			return false;

		return VarCmp((VARIANT*)this, (VARIANT*)&varSrc, LOCALE_USER_DEFAULT, 0) == static_cast<HRESULT>(VARCMP_EQ);
	}

	bool operator!=(const VARIANT& varSrc) const throw()
	{
		return !operator==(varSrc);
	}

	bool operator<(const VARIANT& varSrc) const throw()
	{
		if(vt == VT_NULL && varSrc.vt == VT_NULL)
			return false;

		return VarCmp((VARIANT*)this, (VARIANT*)&varSrc, LOCALE_USER_DEFAULT, 0) == static_cast<HRESULT>(VARCMP_LT);
	}

	bool operator>(const VARIANT& varSrc) const throw()
	{
		if(vt == VT_NULL && varSrc.vt == VT_NULL)
			return false;

		return VarCmp((VARIANT*)this, (VARIANT*)&varSrc, LOCALE_USER_DEFAULT, 0) == static_cast<HRESULT>(VARCMP_GT);
	}

// Operations
public:
	HRESULT Clear()
	{
		return ::VariantClear(this);
	}

	HRESULT ClearToZero() 
	{
		HRESULT hr = ::VariantClear(this); 
		if(FAILED(hr))
			return hr;

		memset(this, 0, sizeof(tagVARIANT));
		vt = VT_EMPTY;

		return hr;
	}

	HRESULT Copy(const VARIANT* pSrc)
	{
		return ::VariantCopy(this, const_cast<VARIANT*>(pSrc));
	}

	// copy VARIANT to BSTR
	HRESULT CopyTo(BSTR* pstrDest) const
	{
		ASSERT(pstrDest != NULL && vt == VT_BSTR);

		HRESULT hr = E_POINTER;
		if(pstrDest != NULL && vt == VT_BSTR)
		{
			*pstrDest = ::SysAllocStringByteLen((char*)bstrVal, ::SysStringByteLen(bstrVal));
			if(*pstrDest == NULL)
				hr = E_OUTOFMEMORY;
			else
				hr = S_OK;
		}
		else if(vt != VT_BSTR)
			hr = DISP_E_TYPEMISMATCH;

		return hr;
	}

	HRESULT Attach(VARIANT* pSrc)
	{
		if(pSrc == NULL)
			return E_INVALIDARG;
			
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
		ASSERT(pDest != NULL);
		if(pDest == NULL)
			return E_POINTER;
	
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

	template<typename T>
	void SetByRef(T* pT) throw()
	{
		Clear();

		vt = CVarTypeInfo<T>::VT | VT_BYREF;
		byref = pT;
	}

	HRESULT WriteToStream(IStream* pStream);

	HRESULT WriteToStream(IStream* pStream, VARTYPE vtWrite)
	{
		if(vtWrite != VT_EMPTY && vtWrite != vt)
		{
			CComVariant varConv;
			HRESULT hr = varConv.ChangeType(vtWrite, this);
			if(FAILED(hr))
				return hr;

			return varConv.WriteToStream(pStream);
		}

		return WriteToStream(pStream);
	}

	HRESULT ReadFromStream(IStream* pStream, VARTYPE vtExpected = VT_EMPTY);

	// Return the size in bytes of the current contents
	ULONG GetSize() const;

// Implementation
public:
	HRESULT InternalClear()
	{
		HRESULT hr = Clear();
		ASSERT(SUCCEEDED(hr));

		if(FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}

		return hr;
	}

	void InternalCopy(const VARIANT* pSrc)
	{
		HRESULT hr = Copy(pSrc);
		if(FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
	}
};

#pragma warning(push)
#pragma warning(disable: 4702)
inline HRESULT CComVariant::WriteToStream(IStream* pStream)
{
	if(pStream == NULL)
		return E_INVALIDARG;
		
	HRESULT hr = pStream->Write(&vt, sizeof(VARTYPE), NULL);
	if(FAILED(hr))
		return hr;

	int cbWrite = 0;
	switch (vt)
	{
	case VT_UNKNOWN:
	case VT_DISPATCH:
		{
			CComPtr<IPersistStream> spStream;
			if(punkVal != NULL)
			{
				hr = punkVal->QueryInterface(__uuidof(IPersistStream), (void**)&spStream);
				if(FAILED(hr))
				{
					hr = punkVal->QueryInterface(__uuidof(IPersistStreamInit), (void**)&spStream);
					if(FAILED(hr))
						return hr;
				}
			}

			if(spStream != NULL)
				return OleSaveToStream(spStream, pStream);

			return WriteClassStm(pStream, CLSID_NULL);
		}
	case VT_UI1:
	case VT_I1:
		cbWrite = sizeof(BYTE);
		break;
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		cbWrite = sizeof(short);
		break;
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
		cbWrite = sizeof(long);
		break;
	case VT_I8:
	case VT_UI8:
		cbWrite = sizeof(LONGLONG);
		break;
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		cbWrite = sizeof(double);
		break;
	default:
		break;
	}

	if(cbWrite != 0)
		return pStream->Write((void*)&bVal, cbWrite, NULL);

	CComBSTRLite bstrWrite;
	CComVariant varBSTR;
	if(vt != VT_BSTR)
	{
		hr = VariantChangeType(&varBSTR, this, VARIANT_NOVALUEPROP, VT_BSTR);
		if(FAILED(hr))
			return hr;

		bstrWrite.Attach(varBSTR.bstrVal);
	}
	else
		bstrWrite.Attach(bstrVal);

	hr = SysWriteStringToStream(bstrWrite, pStream);
	bstrWrite.Detach();

	return hr;
}
#pragma warning(pop)	// C4702

inline HRESULT CComVariant::ReadFromStream(IStream* pStream, VARTYPE vtExpected /* = VT_EMPTY */ )
{
	ASSERT(pStream != NULL);
	if(pStream == NULL)
		return E_INVALIDARG;

	HRESULT hr = VariantClear(this);
	if(FAILED(hr))
		return hr;

	VARTYPE vtRead = VT_EMPTY;
	ULONG cbRead = 0;
	hr = pStream->Read(&vtRead, sizeof(VARTYPE), &cbRead);
	if(hr == S_FALSE || (cbRead != sizeof(VARTYPE) && hr == S_OK))
		hr = E_FAIL;

	if(FAILED(hr))
		return hr;

	if(vtExpected != VT_EMPTY && vtRead != vtExpected)
		return E_FAIL;

	vt = vtRead;
	cbRead = 0;
	switch(vtRead)
	{
	case VT_UNKNOWN:
	case VT_DISPATCH:
		{
			punkVal = NULL;
			hr = OleLoadFromStream(pStream, (vtRead == VT_UNKNOWN) ? __uuidof(IUnknown) : __uuidof(IDispatch), (void**)&punkVal);

			// If IPictureDisp or IFontDisp property is not set, 
			// OleLoadFromStream() will return REGDB_E_CLASSNOTREG.
			if(hr == REGDB_E_CLASSNOTREG)
				hr = S_OK;

			return hr;
		}
	case VT_UI1:
	case VT_I1:
		cbRead = sizeof(BYTE);
		break;
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		cbRead = sizeof(short);
		break;
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
		cbRead = sizeof(long);
		break;
	case VT_I8:
	case VT_UI8:
		cbRead = sizeof(LONGLONG);
		break;
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		cbRead = sizeof(double);
		break;
	default:
		break;
	}

	if(cbRead != 0)
	{
		hr = pStream->Read((void*) &bVal, cbRead, NULL);
		if(hr == S_FALSE)
			hr = E_FAIL;

		return hr;
	}

	CComBSTRLite bstrRead;
	hr = SysReadStringFromStream(bstrRead, pStream);
	if(FAILED(hr))
	{
		// If CComBSTR::ReadFromStream failed, reset seek pointer to start of variant type.
		LARGE_INTEGER nOffset;
		nOffset.QuadPart = -(static_cast<LONGLONG>(sizeof(VARTYPE)));
		pStream->Seek(nOffset, STREAM_SEEK_CUR, NULL);

		return hr;
	}

	vt = VT_BSTR;
	bstrVal = bstrRead.Detach();
	if(vtRead != VT_BSTR)
		hr = ChangeType(vtRead);

	return hr;
}

inline ULONG CComVariant::GetSize() const
{
	ULONG nSize = sizeof(VARTYPE);
	HRESULT hr;

	switch(vt)
	{
	case VT_UNKNOWN:
	case VT_DISPATCH:
		{
			CComPtr<IPersistStream> spStream;
			if(punkVal != NULL)
			{
				hr = punkVal->QueryInterface(__uuidof(IPersistStream), (void**)&spStream);
				if(FAILED(hr))
				{
					hr = punkVal->QueryInterface(__uuidof(IPersistStreamInit), (void**)&spStream);
					if(FAILED(hr))
						break;
				}
			}

			if(spStream != NULL)
			{
				ULARGE_INTEGER nPersistSize;
				nPersistSize.QuadPart = 0;
				spStream->GetSizeMax(&nPersistSize);
				nSize += nPersistSize.LowPart + sizeof(CLSID);
			}
			else
				nSize += sizeof(CLSID);
		}
		break;
	case VT_UI1:
	case VT_I1:
		nSize += sizeof(BYTE);
		break;
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		nSize += sizeof(short);
		break;
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
		nSize += sizeof(long);
		break;
	case VT_I8:
	case VT_UI8:
		nSize += sizeof(LONGLONG);
		break;
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		nSize += sizeof(double);
		break;
	default:
		break;
	}

	if(nSize == sizeof(VARTYPE))
	{
		VARTYPE vtTmp = vt;
		BSTR bstr = NULL;
		CComVariant varBSTR;
		if(vtTmp != VT_BSTR)
		{
			hr = VariantChangeType(&varBSTR, const_cast<VARIANT*>((const VARIANT*)this), VARIANT_NOVALUEPROP, VT_BSTR);
			if(SUCCEEDED(hr))
			{
				bstr = varBSTR.bstrVal;
				vtTmp = VT_BSTR;
			}
		}
		else
		{
			bstr = bstrVal;
		}

		if(vtTmp == VT_BSTR)
		{
			// Add the size of the length + string (in bytes) + NULL terminator.
			nSize += SysGetStringStreamSize(bstr);			
		}
	}

	return nSize;
}

#endif // __COMVAR_H__
