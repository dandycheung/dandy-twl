#ifndef __DLDLL_H__
#define __DLDLL_H__

// Delay loading mechanism.  This allows you to write code as if you are
// calling implicitly linked APIs, and yet have these APIs really be
// explicitly linked.  You can reduce the initial number of DLLs that 
// are loaded (load on demand) using this technique.
//
// Use the following macros to indicate which APIs/DLLs are delay-linked
// and -loaded.
//
//      DELAY_LOAD
//      DELAY_LOAD_HRESULT
//      DELAY_LOAD_SAFEARRAY
//      DELAY_LOAD_UINT
//      DELAY_LOAD_INT
//      DELAY_LOAD_VOID
//
// Use these macros for APIs that are exported by ordinal only.
//
//      DELAY_LOAD_ORD
//      DELAY_LOAD_ORD_VOID     
//
// Sample code:
/*----------------------------------------------------------------------//
#include "dldll.h"

HINSTANCE g_hinstShdocvw = NULL;

// 101 int IEWinMain(LPTSTR lpszCmdLine, int nCmdShow);
DELAY_LOAD_ORD(g_hinstShdocvw, shdocvw.dll, int, IEWinMain, 101,
    (LPTSTR pszCmdLine, int nCmdShow),
    (pszCmdLine, nCmdShow))

int ie(LPTSTR pszCmdLine, int nCmdShow)
{
	return IEWinMain(pszCmdLine, nCmdShow);
}
//----------------------------------------------------------------------*/

#define ENSURE_LOADED(_hinst, _dll, pszfn)         (_hinst ? _hinst : (_hinst = LoadLibrary(TEXT( #_dll ))))


// IEUNIX : changed "## #" to "#" in two places -- JayF
#define DELAY_LOAD_MAP(_hinst, _dll, _ret, _fnpriv, _fn, _args, _nargs, _err)  \
_ret __stdcall _fnpriv _args                                                   \
{                                                                              \
    static _ret (__stdcall * _pfn##_fn) _args = NULL;                          \
    if (!ENSURE_LOADED(_hinst, _dll, #_fn))                                    \
    {                                                                          \
        return (_ret)_err;                                                     \
    }                                                                          \
    if (_pfn##_fn == NULL)                                                     \
    {                                                                          \
        *(FARPROC*)&(_pfn##_fn) = GetProcAddress(_hinst, #_fn);                \
        if (_pfn##_fn == NULL)                                                 \
            return (_ret)_err;                                                 \
    }                                                                          \
    return _pfn##_fn _nargs;                                                   \
}

#define DELAY_LOAD_ERR(_hinst, _dll, _ret, _fn, _args, _nargs, _err)           \
        DELAY_LOAD_MAP(_hinst, _dll, _ret, _fn, _fn, _args, _nargs, _err)

#define DELAY_LOAD(_hinst, _dll, _ret, _fn, _args, _nargs)                     \
        DELAY_LOAD_ERR(_hinst, _dll, _ret, _fn, _args, _nargs, 0)
#define DELAY_LOAD_HRESULT(_hinst, _dll, _fn, _args, _nargs)                   \
        DELAY_LOAD_ERR(_hinst, _dll, HRESULT, _fn, _args, _nargs, E_FAIL)
#define DELAY_LOAD_SAFEARRAY(_hinst, _dll, _fn, _args, _nargs)                 \
        DELAY_LOAD_ERR(_hinst, _dll, SAFEARRAY *, _fn, _args, _nargs, NULL)
#define DELAY_LOAD_DWORD(_hinst, _dll, _fn, _args, _nargs)                     \
        DELAY_LOAD_ERR(_hinst, _dll, DWORD, _fn, _args, _nargs, 0)
#define DELAY_LOAD_UINT(_hinst, _dll, _fn, _args, _nargs)                      \
        DELAY_LOAD_ERR(_hinst, _dll, UINT, _fn, _args, _nargs, 0)
#define DELAY_LOAD_INT(_hinst, _dll, _fn, _args, _nargs)                       \
        DELAY_LOAD_ERR(_hinst, _dll, INT, _fn, _args, _nargs, 0)

#define DELAY_MAP_DWORD(_hinst, _dll, _fnpriv, _fn, _args, _nargs)             \
        DELAY_LOAD_MAP(_hinst, _dll, DWORD, _fnpriv, _fn, _args, _nargs, 0)

#define DELAY_LOAD_VOID(_hinst, _dll, _fn, _args, _nargs)                      \
void __stdcall _fn _args                                                       \
{                                                                              \
    static void (__stdcall * _pfn##_fn) _args = NULL;                          \
    if (!ENSURE_LOADED(_hinst, _dll, #_fn))                                    \
        return;                                                                \
    if (_pfn##_fn == NULL)                                                     \
    {                                                                          \
        *(FARPROC*)&(_pfn##_fn) = GetProcAddress(_hinst, #_fn);                \
        if (_pfn##_fn == NULL)                                                 \
            return;                                                            \
    }                                                                          \
    _pfn##_fn _nargs;                                                          \
 }


//
// For private entrypoints exported by ordinal.
// 

#define GET_PRIVATE_PROC_ADDRESS(_hinst, _fname, _ord) GetProcAddress(_hinst, (LPSTR) _ord)

#define DELAY_LOAD_ORD_ERR(_hinst, _dll, _ret, _fn, _ord, _args, _nargs, _err) \
_ret __stdcall _fn _args                                                       \
{                                                                              \
    static _ret (__stdcall * _pfn##_fn) _args = NULL;                          \
    if (!ENSURE_LOADED(_hinst, _dll, "(ordinal " #_ord ## ")"))                \
    {                                                                          \
        return (_ret)_err;                                                     \
    }                                                                          \
    if (_pfn##_fn == NULL)                                                     \
    {                                                                          \
        *(FARPROC*)&(_pfn##_fn) = GET_PRIVATE_PROC_ADDRESS(_hinst, _fn, _ord); \
                                                                               \
        /* GetProcAddress always returns non-NULL, even for bad ordinals.      \
           But do the check anyways...  */                                     \
                                                                               \
        if (_pfn##_fn == NULL)                                                 \
            return (_ret)_err;                                                 \
    }                                                                          \
    return _pfn##_fn _nargs;                                                   \
 }
        
#define DELAY_LOAD_ORD(_hinst, _dll, _ret, _fn, _ord, _args, _nargs)           \
        DELAY_LOAD_ORD_ERR(_hinst, _dll, _ret, _fn, _ord, _args, _nargs, 0)


#define DELAY_LOAD_ORD_VOID(_hinst, _dll, _fn, _ord, _args, _nargs)            \
void __stdcall _fn _args                                                       \
{                                                                              \
    static void (__stdcall * _pfn##_fn) _args = NULL;                          \
    if (!ENSURE_LOADED(_hinst, _dll, "(ordinal " #_ord ## ")"))                \
        return;                                                                \
    if (_pfn##_fn == NULL)                                                     \
    {                                                                          \
        *(FARPROC*)&(_pfn##_fn) = GetProcAddress(_hinst, (LPSTR) _ord);        \
                                                                               \
        /* GetProcAddress always returns non-NULL, even for bad ordinals.      \
           But do the check anyways...  */                                     \
                                                                               \
        if (_pfn##_fn == NULL)                                                 \
            return;                                                            \
    }                                                                          \
    _pfn##_fn _nargs;                                                          \
}

#endif // __DLDLL_H__
