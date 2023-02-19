/********************************************************************

Copyright (c) 1999 Microsoft Corporation

Module Name:
	PCH_PrinterDriver.H

Abstract:
	WBEM provider class definition for PCH_PrinterDriver class

Revision History:

	Ghim-Sim Chua       (gschua)   04/27/99
		- Created

********************************************************************/

// Property set identification
//============================

#ifndef _PCH_PrinterDriver_H_
#define _PCH_PrinterDriver_H_

#define PROVIDER_NAME_PCH_PRINTERDRIVER "PCH_PrinterDriver"

// Property name externs -- defined in PCH_PrinterDriver.cpp
//=================================================

extern const WCHAR* pTimeStamp ;
extern const WCHAR* pChange ;
extern const WCHAR* pDate ;
extern const WCHAR* pFilename ;
extern const WCHAR* pManufacturer ;
extern const WCHAR* pName ;
extern const WCHAR* pSize ;
extern const WCHAR* pVersion ;
extern const WCHAR* pPath;

class CPCH_PrinterDriver : public Provider 
{
	public:
		// Constructor/destructor
		//=======================

		CPCH_PrinterDriver(const CHString& chsClassName, LPCWSTR lpszNameSpace) : Provider(chsClassName, lpszNameSpace) {};
		virtual ~CPCH_PrinterDriver() {};

	protected:
		// Reading Functions
		//============================
		virtual HRESULT EnumerateInstances(MethodContext*  pMethodContext, long lFlags = 0L);
		virtual HRESULT GetObject(CInstance* pInstance, long lFlags = 0L) { return (WBEM_E_PROVIDER_NOT_CAPABLE); };
		virtual HRESULT ExecQuery(MethodContext *pMethodContext, CFrameworkQuery& Query, long lFlags = 0L) { return (WBEM_E_PROVIDER_NOT_CAPABLE); };

		// Writing Functions
		//============================
		virtual HRESULT PutInstance(const CInstance& Instance, long lFlags = 0L) { return (WBEM_E_PROVIDER_NOT_CAPABLE); };
		virtual HRESULT DeleteInstance(const CInstance& Instance, long lFlags = 0L) { return (WBEM_E_PROVIDER_NOT_CAPABLE); };

		// Other Functions
		virtual HRESULT ExecMethod( const CInstance& Instance,
						const BSTR bstrMethodName,
						CInstance *pInParams,
						CInstance *pOutParams,
						long lFlags = 0L ) { return (WBEM_E_PROVIDER_NOT_CAPABLE); };
} ;

#endif
