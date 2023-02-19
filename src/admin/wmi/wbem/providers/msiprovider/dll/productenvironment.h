// ProductEnvironment.h: interface for the CProductEnvironment class.

//

// Copyright (c) 1997-2001 Microsoft Corporation, All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODUCTENVIRONMENT_H__EAA368A4_DEA4_11D1_8B60_00A0C9954921__INCLUDED_)
#define AFX_PRODUCTENVIRONMENT_H__EAA368A4_DEA4_11D1_8B60_00A0C9954921__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GenericClass.h"

class CProductEnvironment : public CGenericClass  
{
public:
	CProductEnvironment(CRequestObject *pObj, IWbemServices *pNamespace, IWbemContext *pCtx = NULL);
	virtual ~CProductEnvironment();

	virtual HRESULT PutInst(CRequestObject *pObj, IWbemClassObject *pInst, IWbemObjectSink *pHandler, IWbemContext *pCtx)
		{return WBEM_E_NOT_SUPPORTED;}

	virtual HRESULT CreateObject(IWbemObjectSink *pHandler, ACTIONTYPE atAction);
};

#endif // !defined(AFX_PRODUCTENVIRONMENT_H__EAA368A4_DEA4_11D1_8B60_00A0C9954921__INCLUDED_)
