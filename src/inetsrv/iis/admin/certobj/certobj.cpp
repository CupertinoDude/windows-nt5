// CertObj.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To merge the proxy/stub code into the object DLL, add the file 
//      dlldatax.c to the project.  Make sure precompiled headers 
//      are turned off for this file, and add _MERGE_PROXYSTUB to the 
//      defines for the project.  
//
//      If you are not running WinNT4.0 or Win95 with DCOM, then you
//      need to remove the following define from dlldatax.c
//      #define _WIN32_WINNT 0x0400
//
//      Further, if you are running MIDL without /Oicf switch, you also 
//      need to remove the following define from dlldatax.c.
//      #define USE_STUBLESS_PROXY
//
//      Modify the custom build rule for CertObj.idl by adding the following 
//      files to the Outputs.
//          CertObj_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f CertObjps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "dlldatax.h"
#include "common.h"
#include "CertObj.h"
#include "CertObj_i.c"
#include "IISCertObj.h"
#include "certlog.h"
#include "dcomperm.h"

#ifdef _MERGE_PROXYSTUB
    extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
    OBJECT_ENTRY(CLSID_IISCertObj, CIISCertObj)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    lpReserved;
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
        return FALSE;
#endif
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_CERTOBJLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
        return S_FALSE;
#endif
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    HRESULT hRes = E_FAIL;

#ifdef _MERGE_PROXYSTUB
    hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    if (RunningAsAdministrator())
    {
        // Add the event log entry
        EventlogRegistryInstall();

        // registers object, typelib and all interfaces in typelib
        hRes = _Module.RegisterServer(TRUE);
        if (SUCCEEDED(hRes))
        {
            if (ERROR_SUCCESS != ChangeAppIDLaunchACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("Administrators"),TRUE,TRUE))
            {
                _Module.UnregisterServer(TRUE);
                hRes = E_FAIL;
            }
            else
            {
                if (ERROR_SUCCESS != ChangeAppIDAccessACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("Administrators"),TRUE,TRUE))
                {
                    _Module.UnregisterServer(TRUE);
                    hRes = E_FAIL;
                }
                else
                {
                    hRes = S_OK;
                }
            }
        }
    }
    else
    {
        hRes = E_FAIL;
    }
   
    return hRes;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    HRESULT hRes = E_FAIL;

#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    if (RunningAsAdministrator())
    {
        EventlogRegistryUnInstall();
        ChangeAppIDLaunchACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("Administrators"),FALSE,FALSE);
        ChangeAppIDLaunchACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("everyone"),FALSE,FALSE);
        ChangeAppIDAccessACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("Administrators"),FALSE,FALSE);
        ChangeAppIDAccessACL(TEXT("{62B8CCBE-5A45-4372-8C4A-6A87DD3EDD60}"),TEXT("everyone"),FALSE,FALSE);
        hRes = _Module.UnregisterServer(TRUE);
    }
    else
    {
        hRes = E_FAIL;
    }
    return hRes;
}
