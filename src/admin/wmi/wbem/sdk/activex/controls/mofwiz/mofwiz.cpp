// Copyright (c) 1997-2001 Microsoft Corporation, All Rights Reserved
// MOFWiz.cpp : Implementation of CMOFWizApp and DLL registration.

#include "precomp.h"
#include "MOFWiz.h"
#include "CatHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMOFWizApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xf3b3a400, 0x3419, 0x11d0, { 0x95, 0xf8, 0, 0xc0, 0x4f, 0xd9, 0xb1, 0x5b } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

const GUID CDECL BASED_CODE _ctlid =   { 0xf3b3a403, 0x3419, 0x11d0,
           { 0x95, 0xf8, 0, 0xc0, 0x4f, 0xd9, 0xb1, 0x5b} };

const CATID CATID_SafeForScripting     =
   {0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const CATID CATID_SafeForInitializing  =
   {0x7dd95802,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

////////////////////////////////////////////////////////////////////////////
// CMOFWizApp::InitInstance - DLL initialization

BOOL CMOFWizApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CMOFWizApp::ExitInstance - DLL termination

int CMOFWizApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( CreateComponentCategory(CATID_SafeForScripting,
               L"Controls that are safely scriptable") ))
             return ResultFromScode(SELFREG_E_CLASS);
   if (FAILED( CreateComponentCategory(
           CATID_SafeForInitializing,
           L"Controls safely initializable from persistent data") ))
         return ResultFromScode(SELFREG_E_CLASS);
   if (FAILED( RegisterCLSIDInCategory(
           _ctlid, CATID_SafeForScripting) ))
         return ResultFromScode(SELFREG_E_CLASS);
   if (FAILED( RegisterCLSIDInCategory(
           _ctlid, CATID_SafeForInitializing) ))
         return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
