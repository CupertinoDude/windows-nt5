#if !defined(AFX_DLLDATAX_H__CA9BAE5D_9A39_11D0_8D3F_00A0C90DCAE7__INCLUDED_)
#define AFX_DLLDATAX_H__CA9BAE5D_9A39_11D0_8D3F_00A0C90DCAE7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _MERGE_PROXYSTUB

extern "C" 
{
BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, 
	LPVOID lpReserved);
STDAPI PrxDllCanUnloadNow(void);
STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
STDAPI PrxDllRegisterServer(void);
STDAPI PrxDllUnregisterServer(void);
}

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDATAX_H__CA9BAE5D_9A39_11D0_8D3F_00A0C90DCAE7__INCLUDED_)
