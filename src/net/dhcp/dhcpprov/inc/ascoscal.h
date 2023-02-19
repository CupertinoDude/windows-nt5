/******************************************************************
   SNetScal.h -- WBEM provider class declaration

   MODULE:
        DhcpProv.dll

   DESCRIPTION:
        Contains the declaration of the DHCP_Subnet_Scalar class and
        the indices definitions for its static table of manageable objects.

   REVISION:
        08/03/98 - created

******************************************************************/
#include "Props.h"      // needed for CDHCP_Property definition

#ifndef _ASCOSCAL_H
#define _ASCOSCAL_H

class CDHCP_SuperScopeToSubnet : public Provider 
{
	protected:
		// Reading Functions
		virtual HRESULT EnumerateInstances(MethodContext*  pMethodContext, long lFlags = 0L);
		virtual HRESULT GetObject(CInstance* pInstance, long lFlags = 0L);
		virtual HRESULT ExecQuery(MethodContext *pMethodContext, CFrameworkQuery& Query, long lFlags = 0L);

		// Writing Functions
		virtual HRESULT PutInstance(const CInstance& Instance, long lFlags = 0L);
		virtual HRESULT DeleteInstance(const CInstance& Instance, long lFlags = 0L);

		// Other Functions
		virtual HRESULT ExecMethod( const CInstance& Instance,
						const BSTR bstrMethodName,
						CInstance *pInParams,
						CInstance *pOutParams,
						long lFlags = 0L );
    public:
		// Constructor/destructor
		CDHCP_SuperScopeToSubnet(const CHString& chsClassName, LPCSTR lpszNameSpace);
		virtual ~CDHCP_SuperScopeToSubnet();

};

#endif
