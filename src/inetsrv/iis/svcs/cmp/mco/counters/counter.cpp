// Counter.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f Counterps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "Counter.h"

#include "Counter_i.c"
#include <initguid.h>
#include "CountCtl.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_Counters, CCounterCtl)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
        CCounter::LoadCounters();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
        CCounter::Terminate();
		_Module.Term();
	}
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	// When the DLL unregisers, clean up the Counters.txt file that we use to store the data.

	char fileNameBuffer[MAX_PATH];

    GetSystemDirectory(fileNameBuffer, MAX_PATH-sizeof(COUNTERS_TXT));
	strcat(fileNameBuffer, COUNTERS_TXT );

	DeleteFile(fileNameBuffer); 

	return S_OK;
}


