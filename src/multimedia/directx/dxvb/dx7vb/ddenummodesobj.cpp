//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1999 - 1999
//
//  File:       ddenummodesobj.cpp
//
//--------------------------------------------------------------------------


#include "stdafx.h"
#include "Direct.h"
#include "dms.h"
#include "DDEnumModesObj.h"


extern		BOOL IsAllZeros(void *pStruct,DWORD size); 

extern "C" HRESULT PASCAL objEnumModes2Callback(LPDDSURFACEDESC2 lpDesc, void *lpArg)
{
	DPF(1,"Entered objEnumModes2Callback\r\n");



	
	C_dxj_DirectDrawEnumModesObject *pObj=(C_dxj_DirectDrawEnumModesObject*)lpArg;
	if (pObj==NULL) return DDENUMRET_OK;

	if (pObj->m_nCount >= pObj->m_nMax) 
	{
		pObj->m_nMax += 10;

		if (pObj->m_pList)
			pObj->m_pList=(DDSurfaceDesc2*)realloc(pObj->m_pList,sizeof(DDSurfaceDesc2)* pObj->m_nMax);
		else
			pObj->m_pList=(DDSurfaceDesc2*)malloc(sizeof(DDSurfaceDesc2)* pObj->m_nMax);

		if (pObj->m_pList==NULL) 
		{
			pObj->m_bProblem=TRUE;
			return FALSE;
		}
	}
	
	if (lpDesc==NULL) {
			pObj->m_bProblem=TRUE;
			return FALSE;
	}
	
	
	HRESULT hr=CopyOutDDSurfaceDesc2(&(pObj->m_pList[pObj->m_nCount]),lpDesc);
	if (FAILED(hr)){
			pObj->m_bProblem=TRUE;
			return FALSE;
	}
		
	
	
	pObj->m_nCount++;
	
	return TRUE;
}


C_dxj_DirectDrawEnumModesObject::C_dxj_DirectDrawEnumModesObject()
{	
	m_nMax=0;
	m_pList=NULL;
	m_nCount=0;
	m_bProblem=FALSE;
}
C_dxj_DirectDrawEnumModesObject::~C_dxj_DirectDrawEnumModesObject()
{
	//empty list
	if (m_pList){
		free(m_pList);
	}

}


HRESULT C_dxj_DirectDrawEnumModesObject::create(LPDIRECTDRAW7 pdd, long flags, DDSurfaceDesc2 *desc, I_dxj_DirectDrawEnumModes **ppRet)
{
	HRESULT hr;
	C_dxj_DirectDrawEnumModesObject *pNew=NULL;

	//ASSERT(ppRet,"C_dxj_DDEnumObject::create passed invalid arg");
	*ppRet=NULL;

	if (!pdd) return E_INVALIDARG;

	pNew= new CComObject<C_dxj_DirectDrawEnumModesObject>;			
	if (!pNew) return E_OUTOFMEMORY;

	pNew->m_bProblem=FALSE;

	//note DDSurfaceDesc as a DDSURFACEDESC structure inside it.
	if ((desc==NULL)|| (IsAllZeros(desc,sizeof(DDSurfaceDesc2)))) {
		hr=pdd->EnumDisplayModes((DWORD)flags,NULL,pNew,objEnumModes2Callback);
	}
	else {
		DDSURFACEDESC2 desc2;
		hr=CopyInDDSurfaceDesc2(&desc2,desc);
		if (hr==S_OK){
			hr=pdd->EnumDisplayModes((DWORD)flags,&desc2,pNew,objEnumModes2Callback);
		}
	}
	if (pNew->m_bProblem) hr=E_OUTOFMEMORY;

	if FAILED(hr) 
	{
		free(pNew->m_pList);
		pNew->m_pList=NULL;
		delete pNew;	
		return hr;
	}

	hr=pNew->QueryInterface(IID_I_dxj_DirectDrawEnumModes,(void**)ppRet);
	return hr;
}


HRESULT C_dxj_DirectDrawEnumModesObject::create(LPDIRECTDRAW4 pdd, long flags, DDSurfaceDesc2 *desc, I_dxj_DirectDrawEnumModes **ppRet)
{
	HRESULT hr;
	C_dxj_DirectDrawEnumModesObject *pNew=NULL;

	//ASSERT(ppRet,"C_dxj_DDEnumObject::create passed invalid arg");
	*ppRet=NULL;

	if (!pdd) return E_INVALIDARG;

	pNew= new CComObject<C_dxj_DirectDrawEnumModesObject>;			
	if (!pNew) return E_OUTOFMEMORY;

	pNew->m_bProblem=FALSE;

	//note DDSurfaceDesc as a DDSURFACEDESC structure inside it.
	if ((desc==NULL)|| (IsAllZeros(desc,sizeof(DDSurfaceDesc2)))) {
		hr=pdd->EnumDisplayModes((DWORD)flags,NULL,pNew,objEnumModes2Callback);
	}
	else {
		DDSURFACEDESC2 desc2;
		hr=CopyInDDSurfaceDesc2(&desc2,desc);
		if (hr==S_OK){
			hr=pdd->EnumDisplayModes((DWORD)flags,&desc2,pNew,objEnumModes2Callback);
		}
	}
	if (pNew->m_bProblem) hr=E_OUTOFMEMORY;

	if FAILED(hr) 
	{
		free(pNew->m_pList);
		pNew->m_pList=NULL;
		delete pNew;	
		return hr;
	}

	hr=pNew->QueryInterface(IID_I_dxj_DirectDrawEnumModes,(void**)ppRet);
	return hr;
}


HRESULT C_dxj_DirectDrawEnumModesObject::getItem( long index, DDSurfaceDesc2 *info)
{
	if (m_pList==NULL) return E_FAIL;
	if (index < 1) return E_INVALIDARG;
	if (index > m_nCount) return E_INVALIDARG;

	memcpy(info,&(m_pList[index-1]),sizeof(DDSurfaceDesc2));

	return S_OK;
}

HRESULT C_dxj_DirectDrawEnumModesObject::getCount(long *retVal)
{
	*retVal=m_nCount;
	return S_OK;
}
