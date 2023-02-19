//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1998 - 1998
//
//  File:       ddenumplayersobj.cpp
//
//--------------------------------------------------------------------------


#include "stdafx.h"
#include "Direct.h"
#include "dms.h"
#include "dxglobobj.h"
#include "DDEnumObj.h"

extern "C" BSTR GUIDtoBSTR(LPGUID pGuid);


extern "C" BOOL PASCAL  objDirectDrawEnumCallback( 
  GUID FAR *lpGUID,           
  LPSTR lpDriverDescription,  
  LPSTR lpDriverName,         
  LPVOID lpArg            
)
{

	#ifdef _DEBUG
	OutputDebugString("Entered objDirectDrawEnumCallback \r\n");
	#endif
	
	C_dxj_DDEnumObject *pObj=(C_dxj_DDEnumObject*)lpArg;
	if (pObj==NULL) return DDENUMRET_OK;

	if (pObj->m_nCount >= pObj->m_nMax) 
	{
		pObj->m_nMax += 10;
		pObj->m_pList=(DxDriverInfo*)realloc(pObj->m_pList,sizeof(DxDriverInfo)* pObj->m_nMax);
		if (pObj->m_pList==NULL) 
		{
			pObj->m_bProblem=TRUE;
			return FALSE;
		}
	}

	USES_CONVERSION;
	ZeroMemory(&(pObj->m_pList[pObj->m_nCount]),sizeof(DxDriverInfo));
	pObj->m_pList[pObj->m_nCount].strGuid=GUIDtoBSTR((GUID*)lpGUID);
	
	if (lpDriverDescription!=NULL) {
		pObj->m_pList[pObj->m_nCount].strDescription=T2BSTR(lpDriverDescription);
	}
	if (lpDriverName!=NULL){
		pObj->m_pList[pObj->m_nCount].strName=T2BSTR(lpDriverName);
	}

	pObj->m_nCount++;
	
	return TRUE;
}


C_dxj_DDEnumObject::C_dxj_DDEnumObject()
{	
	m_nMax=0;
	m_pList=NULL;
	m_nCount=0;
	m_bProblem=FALSE;
}
C_dxj_DDEnumObject::~C_dxj_DDEnumObject()
{
	//empty list
	if (m_pList){
		for (int i=0;i<m_nCount;i++)
		{
			if (m_pList[i].guid) SysFreeString(m_pList[i].strGuid);
			if (m_pList[i].description) SysFreeString(m_pList[i].strDescription);
			if (m_pList[i].name) SysFreeString(m_pList[i].strName);
		}
		free(m_pList);
	}

}


HRESULT C_dxj_DDEnumObject::create(DDENUMERATE pcbFunc,I_dxj_DDEnum **ppRet)
{
	HRESULT hr;
	C_dxj_DDEnumObject *pNew=NULL;

	//ASSERT(ppRet,"C_dxj_DDEnumObject::create passed invalid arg");
	*ppRet=NULL;

	pNew= new CComObject<C_dxj_DDEnumObject>;			
	if (!pNew) return E_OUTOFMEMORY;

	pNew->m_bProblem=FALSE;

	hr=pcbFunc(objDirectDrawEnumCallback,(void*)pNew);

	if (pNew->m_bProblem) hr=E_OUTOFMEMORY;

	if FAILED(hr) 
	{
		free(pNew->m_pList);
		pNew->m_pList=NULL;
		delete pNew;	
		return hr;
	}

	hr=pNew->QueryInterface(IID_I_dxj_DDEnum,(void**)ppRet);
	return hr;
}

HRESULT C_dxj_DDEnumObject::getItem( long index, DxDriverInfo *info)
{
	if (m_pList==NULL) return E_FAIL;
	if (index < 0) return E_INVALIDARG;
	if (index >= m_nCount) return E_INVALIDARG;

	memcpy(info,&(m_pList[index]),sizeof(DxDriverInfo));

	return S_OK;
}

HRESULT C_dxj_DDEnumObject::getCount(long *retVal)
{
	*retVal=m_nCount;
	return S_OK;
}
