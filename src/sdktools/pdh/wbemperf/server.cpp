//***************************************************************************
//
//  Copyright (c) 1997-1999, Microsoft Corporation, All rights reserved
//
//  SERVER.CPP
//
//  Generic COM server framework, adapted for the NT perf counter sample
//
//  This module contains nothing specific to the NT perf counter provider
//  except what is defined in the section bracketed by the CLSID SPECIFIC
//  comments below.
//
//  History:
//  raymcc        25-Nov-97     Created.
//  raymcc        18-Feb-98     Updated for NT5 Beta 2 version.
//
//***************************************************************************
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <objbase.h>
#include <initguid.h>
/////////////////////////////////////////////////////////////////////////////
//
//  BEGIN CLSID SPECIFIC SECTION
//
//
#pragma warning ( disable : 4268)
#include <wbemidl.h>
#pragma warning ( default : 4268)
#include <wbemint.h>
#include "ntperf.h"
#include "strings.h"

//
//  this is going to be the ClientLoadable one
//

// {FF37A93C-C28E-11d1-AEB6-00C04FB68820}
DEFINE_GUID(CLSID_NT5PerfProvider_V1,
0xff37a93c, 0xc28e, 0x11d1, 0xae, 0xb6, 0x0, 0xc0, 0x4f, 0xb6, 0x88, 0x20);

//
// this is going to be the Server Loadable
//

// {76A94DE3-7C26-44f5-8E98-C5AEA48186CB}
DEFINE_GUID(CLSID_NT5PerfProvider_V1_Srv, 
0x76a94de3, 0x7c26, 0x44f5, 0x8e, 0x98, 0xc5, 0xae, 0xa4, 0x81, 0x86, 0xcb);


#define IMPLEMENTED_CLSID_1         CLSID_NT5PerfProvider_V1
#define IMPLEMENTED_CLSID_2         CLSID_NT5PerfProvider_V1_Srv 
#define SERVER_REGISTRY_COMMENT     cszOleRegistryComment
#define CPP_CLASS_NAME              CNt5PerfProvider
#define INTERFACE_CAST              (IWbemHiPerfProvider *)

//
//  END CLSID SPECIFIC SECTION
//
/////////////////////////////////////////////////////////////////////////////




HINSTANCE g_hInstance;
static ULONG g_cLock = 0;

void ObjectCreated()    { g_cLock++; }
void ObjectDestroyed() { g_cLock--; }

//***************************************************************************
//
//  class CFactory
//
//  Generic implementation of IClassFactory for CWbemLocator.
//
//***************************************************************************

class CFactory : public IClassFactory
{
    ULONG           m_cRef;
    CLSID           m_ClsId;

public:
    CFactory(const CLSID & ClsId);
    ~CFactory();

    //
    // IUnknown members
    //
    STDMETHODIMP         QueryInterface(REFIID, LPVOID *);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //
    // IClassFactory members
    //
    STDMETHODIMP     CreateInstance(LPUNKNOWN, REFIID, LPVOID *);
    STDMETHODIMP     LockServer(BOOL);
};

//***************************************************************************
//
//  DllMain
//
//  Dll entry point.
//
//  PARAMETERS:
//
//      HINSTANCE hinstDLL      The handle to our DLL.
//      DWORD dwReason          DLL_PROCESS_ATTACH on load,
//                              DLL_PROCESS_DETACH on shutdown,
//                              DLL_THREAD_ATTACH/DLL_THREAD_DETACH otherwise.
//      LPVOID lpReserved       Reserved
//
//  RETURN VALUES:
//
//      TRUE is successful, FALSE if a fatal error occured.
//      NT behaves very ugly if FALSE is returned.
//
//***************************************************************************
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD dwReason,
    LPVOID lpReserved
    )
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        setlocale(LC_ALL, "");      // Set to the 'current' locale
        g_hInstance = hinstDLL;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }

    return TRUE;
}

//***************************************************************************
//
//  DllGetClassObject
//
//  Standard OLE In-Process Server entry point to return an class factory
//  instance.
//
//  PARAMETERS:
//
//  RETURNS:
//
//      S_OK                Success
//      E_NOINTERFACE       An interface other that IClassFactory was asked for
//      E_OUTOFMEMORY
//      E_FAILED            Initialization failed, or an unsupported clsid was
//                          asked for.
//
//***************************************************************************
extern "C"
HRESULT APIENTRY DllGetClassObject(
    REFCLSID rclsid,
    REFIID riid,
    LPVOID * ppv
    )
{
    CFactory *pFactory;

    //
    //  Verify the caller is asking for our type of object.
    //
    if (IMPLEMENTED_CLSID_1 != rclsid &&
        IMPLEMENTED_CLSID_2 != rclsid) 
            return ResultFromScode(E_FAIL);

    //
    // Check that we can provide the interface.
    //
    if (IID_IUnknown != riid && IID_IClassFactory != riid)
        return ResultFromScode(E_NOINTERFACE);

    //
    // Get a new class factory.
    //
    pFactory = new CFactory(rclsid);

    if (!pFactory)
        return ResultFromScode(E_OUTOFMEMORY);

    //
    // Verify we can get an instance.
    //
    HRESULT hRes = pFactory->QueryInterface(riid, ppv);

    if (FAILED(hRes))
        delete pFactory;

    return hRes;
}

//***************************************************************************
//
//  DllCanUnloadNow
//
//  Standard OLE entry point for server shutdown request. Allows shutdown
//  only if no outstanding objects or locks are present.
//
//  RETURN VALUES:
//
//      S_OK        May unload now.
//      S_FALSE     May not.
//
//***************************************************************************

extern "C"
HRESULT APIENTRY DllCanUnloadNow(void)
{
    SCODE sc = TRUE;

    if (g_cLock)
        sc = S_FALSE;

    return sc;
}

//***************************************************************************
//
//  DllRegisterServer
//
//  Standard OLE entry point for registering the server.
//
//  RETURN VALUES:
//
//      S_OK        Registration was successful
//      E_FAIL      Registration failed.
//
//***************************************************************************

extern "C"
HRESULT APIENTRY DllRegisterServer(void)
{
    wchar_t Path[1024];
    wchar_t *pGuidStr = 0;
    wchar_t KeyPath[1024];
    HKEY hKey;
    LONG lRes;
    wchar_t *pName;
    HKEY hSubkey;
    
    // Where are we?
    // =============
    GetModuleFileNameW(g_hInstance, Path, 1024);

    // Convert CLSID to string.
    // ========================

    // Place it in registry.
    // CLSID\\CLSID_Nt5PerProvider_v1 : <no_name> : "name"
    //      \\CLSID_Nt5PerProvider_v1\\InProcServer32 : <no_name> : "path to DLL"
    //                                    : ThreadingModel : "both"
    // ==============================================================

    StringFromCLSID(IMPLEMENTED_CLSID_1, &pGuidStr);
    swprintf(KeyPath, cszClsidFormatString, pGuidStr);

    lRes = RegCreateKeyW(HKEY_LOCAL_MACHINE, KeyPath, &hKey);
    if (lRes)
        return E_FAIL;

    pName = (LPWSTR)SERVER_REGISTRY_COMMENT; 
    RegSetValueExW(hKey, 0, 0, REG_SZ, (const BYTE *) pName, (DWORD)(((DWORD)(wcslen(pName)) + 1) * 2));


    lRes = RegCreateKey(hKey, "InprocServer32", &hSubkey);

    RegSetValueExW(hSubkey, 0, 0, REG_SZ, (const BYTE *) Path, (DWORD)(((DWORD)(wcslen(Path)) + 1) * 2));
    RegSetValueExW(hSubkey, cszThreadingModel, 0, REG_SZ, (const BYTE *) cszBoth, (DWORD)(((DWORD)(wcslen(cszBoth)) + 1) * 2));

    RegCloseKey(hSubkey);
    RegCloseKey(hKey);

    CoTaskMemFree(pGuidStr);

    // Place it in registry.
    // CLSID\\CLSID_Nt5PerProvider_v1_Srv : <no_name> : "name"
    //      \\CLSID_Nt5PerProvider_v1_Srv\\InProcServer32 : <no_name> : "path to DLL"
    //                                    : ThreadingModel : "both"
    // ==============================================================    

    StringFromCLSID(IMPLEMENTED_CLSID_2, &pGuidStr);
    swprintf(KeyPath, cszClsidFormatString, pGuidStr);

    lRes = RegCreateKeyW(HKEY_LOCAL_MACHINE, KeyPath, &hKey);
    if (lRes)
        return E_FAIL;

    pName = (LPWSTR)SERVER_REGISTRY_COMMENT; 
    RegSetValueExW(hKey, 0, 0, REG_SZ, (const BYTE *) pName, (DWORD)(((DWORD)(wcslen(pName)) + 1) * 2));

    lRes = RegCreateKey(hKey, "InprocServer32", &hSubkey);

    RegSetValueExW(hSubkey, 0, 0, REG_SZ, (const BYTE *) Path, (DWORD)(((DWORD)(wcslen(Path)) + 1) * 2));
    RegSetValueExW(hSubkey, cszThreadingModel, 0, REG_SZ, (const BYTE *) cszBoth, (DWORD)(((DWORD)(wcslen(cszBoth)) + 1) * 2));

    RegCloseKey(hSubkey);
    RegCloseKey(hKey);

    CoTaskMemFree(pGuidStr);

    return S_OK;
}

//***************************************************************************
//
//  DllUnregisterServer
//
//  Standard OLE entry point for unregistering the server.
//
//  RETURN VALUES:
//
//      S_OK        Unregistration was successful
//      E_FAIL      Unregistration failed.
//
//***************************************************************************

extern "C"
HRESULT APIENTRY DllUnregisterServer(void)
{
    wchar_t *pGuidStr = 0;
    HKEY hKey;
    wchar_t KeyPath[256];
    LONG lRes;

    // Delete InProcServer32 subkey.
    // =============================
    // Delete CLSID GUID key.
    // ======================
    
    StringFromCLSID(IMPLEMENTED_CLSID_1, &pGuidStr);
    swprintf(KeyPath, cszClsidFormatString, pGuidStr);

    lRes = RegOpenKeyW(HKEY_LOCAL_MACHINE, KeyPath, &hKey);
    if (lRes)
        return E_FAIL;

    RegDeleteKeyW(hKey, cszInprocServer);
    RegCloseKey(hKey);

    lRes = RegOpenKeyW(HKEY_LOCAL_MACHINE, cszClsidKey, &hKey);
    if (lRes)
        return E_FAIL;

    RegDeleteKeyW(hKey, pGuidStr);
    RegCloseKey(hKey);

    CoTaskMemFree(pGuidStr);

    StringFromCLSID(IMPLEMENTED_CLSID_2, &pGuidStr);
    swprintf(KeyPath, cszClsidFormatString, pGuidStr);

    lRes = RegOpenKeyW(HKEY_LOCAL_MACHINE, KeyPath, &hKey);
    if (lRes)
        return E_FAIL;

    RegDeleteKeyW(hKey, cszInprocServer);
    RegCloseKey(hKey);

    lRes = RegOpenKeyW(HKEY_LOCAL_MACHINE, cszClsidKey, &hKey);
    if (lRes)
        return E_FAIL;

    RegDeleteKeyW(hKey, pGuidStr);
    RegCloseKey(hKey);

    CoTaskMemFree(pGuidStr);

    return S_OK;
}

//***************************************************************************
//
//  CFactory::CFactory
//
//  Constructs the class factory given the CLSID of the objects it is supposed
//  to create.
//
//  PARAMETERS:
//
//      const CLSID & ClsId     The CLSID. 
//
//***************************************************************************
CFactory::CFactory(const CLSID & ClsId)
{
    m_cRef = 0;
    ObjectCreated();
    m_ClsId = ClsId;
}

//***************************************************************************
//
//  CFactory::~CFactory
//
//  Destructor.
//
//***************************************************************************
CFactory::~CFactory()
{
    ObjectDestroyed();
}

//***************************************************************************
//
//  CFactory::QueryInterface, AddRef and Release
//
//  Standard IUnknown methods.
//
//***************************************************************************
STDMETHODIMP CFactory::QueryInterface(REFIID riid, LPVOID * ppv)
{
    *ppv = 0;

    if (IID_IUnknown==riid || IID_IClassFactory==riid)
    {
        *ppv = this;
        AddRef();
        return NOERROR;
    }

    return ResultFromScode(E_NOINTERFACE);
}


ULONG CFactory::AddRef()
{
    return ++m_cRef;
}


ULONG CFactory::Release()
{
    if (0 != --m_cRef)
        return m_cRef;
    delete this;
    return 0;
}

//***************************************************************************
//
//  CFactory::CreateInstance
//
//  PARAMETERS:
//
//      LPUNKNOWN pUnkOuter     IUnknown of the aggregator. Must be NULL.
//      REFIID riid             Interface ID required.
//      LPVOID * ppvObj         Destination for the interface pointer.
//
//  RETURN VALUES:
//
//      S_OK                        Success
//      CLASS_E_NOAGGREGATION       pUnkOuter must be NULL
//      E_NOINTERFACE               No such interface supported.
//      
//***************************************************************************

STDMETHODIMP CFactory::CreateInstance(
    LPUNKNOWN pUnkOuter,
    REFIID riid,
    LPVOID * ppvObj)
{
    IUnknown* pObj;
    HRESULT  hr;

    //
    //  Defaults
    //
    *ppvObj=NULL;
    hr = ResultFromScode(E_OUTOFMEMORY);

    //
    // We aren't supporting aggregation.
    //
    if (pUnkOuter)
        return ResultFromScode(CLASS_E_NOAGGREGATION);

    if (m_ClsId == IMPLEMENTED_CLSID_1) {
        pObj = INTERFACE_CAST new CPP_CLASS_NAME(CPP_CLASS_NAME::CLSID_CLIENT);
    } else if (m_ClsId == IMPLEMENTED_CLSID_2) {
        pObj = INTERFACE_CAST new CPP_CLASS_NAME(CPP_CLASS_NAME::CLSID_SERVER);
    } else {
		pObj = NULL;
	}

    if (!pObj)
        return hr;

    //
    //  Initialize the object and verify that it can return the
    //  interface in question.
    //
    hr = pObj->QueryInterface(riid, ppvObj);

    //
    // Kill the object if initial creation or Init failed.
    //
    if (FAILED(hr))
        delete pObj;

    return hr;
}

//***************************************************************************
//
//  CFactory::LockServer
//
//  Increments or decrements the lock count of the server. The DLL will not
//  unload while the lock count is positive.
//
//  PARAMETERS:
//
//      BOOL fLock      If TRUE, locks; otherwise, unlocks.
//
//  RETURN VALUES:
//
//      S_OK
//
//***************************************************************************
STDMETHODIMP CFactory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement((LONG *) &g_cLock);
    else
        InterlockedDecrement((LONG *) &g_cLock);

    return NOERROR;
}
