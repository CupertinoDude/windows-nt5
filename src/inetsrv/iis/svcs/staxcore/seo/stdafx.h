/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

	StdAfx.h

Abstract:

	This module contains the definitions for the base
	ATL methods.

Author:

	Don Dumitru     (dondu@microsoft.com)

Revision History:

	dondu   12/04/96        created

--*/


// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif


#ifdef _ATL_NO_DEBUG_CRT
	#include "windows.h"
	#include "dbgtrace.h"
	#define _ASSERTE	_ASSERT
#endif


//#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CSEOComModule : public CComModule {
	public:
		const GUID *GetAPPID();
		HRESULT WriteAPPID();
		HRESULT EraseAPPID();
};
extern CSEOComModule _Module;
#include <atlcom.h>


#if defined(_ATL_SINGLE_THREADED)
	#define ATL_THREADING_MODEL_VALUE	L"Single"
#elif defined(_ATL_APARTMENT_THREADED)
	#define ATL_THREADING_MODEL_VALUE	L"Apartment"
#else
	#define ATL_THREADING_MODEL_VALUE	L"Both"
#endif


#define DECLARE_REGISTRY_RESOURCEID_EX(x,desc,progid,viprogid)			\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) {				\
		HRESULT hrRes;													\
		_ATL_REGMAP_ENTRY *parme;										\
																		\
		hrRes = AtlAllocRegMapEx(&parme,								\
								 &GetObjectCLSID(),						\
								 &_Module,								\
								 NULL,									\
								 L"DESCRIPTION",						\
								 desc,									\
								 L"PROGID",								\
								 progid,								\
								 L"VIPROGID",							\
								 viprogid,								\
								 L"THREADINGMODEL",						\
								 ATL_THREADING_MODEL_VALUE,				\
								 NULL,									\
								 NULL);									\
		if (!SUCCEEDED(hrRes)) {										\
			return (hrRes);												\
		}																\
		hrRes = _Module.UpdateRegistryFromResource(x,bRegister,parme);	\
		CoTaskMemFree(parme);											\
		return (hrRes);													\
	}


#define DECLARE_REGISTRY_RESOURCE_EX(x,desc,progid,viprogid)				\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) {					\
		HRESULT hrRes;														\
		_ATL_REGMAP_ENTRY *parme;											\
																			\
		hrRes = AtlAllocRegMapEx(&parme,									\
								 &GetObjectCLSID(),							\
								 &_Module,									\
								 NULL,										\
								 L"DESCRIPTION",							\
								 desc,										\
								 L"PROGID",									\
								 progid,									\
								 L"VIPROGID",								\
								 viprogid,									\
								 L"THREADINGMODEL",							\
								 ATL_THREADING_MODEL_VALUE,					\
								 NULL,										\
								 NULL);										\
		if (!SUCCEEDED(hrRes)) {											\
			return (hrRes);													\
		}																	\
		hrRes = _Module.UpdateRegistryFromResource(_T(#x),bRegister,parme);	\
		CoTaskMemFree(parme);												\
		return (hrRes);														\
	}


HRESULT AtlAllocRegMapEx(_ATL_REGMAP_ENTRY **pparmeResult,
						 const CLSID *pclsid,
						 CSEOComModule *pmodule,
						 LPCOLESTR pszIndex,
						 ...);
