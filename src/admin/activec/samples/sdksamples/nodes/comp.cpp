//==============================================================;
//
//  This source code is only intended as a supplement to existing Microsoft documentation. 
//
// 
//
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
//
//
//
//==============================================================;

#include "Comp.h"
#include "DataObj.h"
#include <commctrl.h>        // Needed for button styles...
#include <crtdbg.h>
#include "globals.h"
#include "resource.h"
#include "DeleBase.h"
#include "CompData.h"

CComponent::CComponent(CComponentData *parent)
: m_pComponentData(parent), m_cref(0), m_ipConsole(NULL), m_pLastNode(NULL)
{
    OBJECT_CREATED
}

CComponent::~CComponent()
{
    OBJECT_DESTROYED
}

STDMETHODIMP CComponent::QueryInterface(REFIID riid, LPVOID *ppv)
{
    if (!ppv)
        return E_FAIL;
    
    *ppv = NULL;
    
    if (IsEqualIID(riid, IID_IUnknown))
        *ppv = static_cast<IComponent *>(this);
    else if (IsEqualIID(riid, IID_IComponent))
        *ppv = static_cast<IComponent *>(this);
    else if (IsEqualIID(riid, IID_IResultOwnerData))
        *ppv = static_cast<IResultOwnerData *>(this);
    
    if (*ppv)
    {
        reinterpret_cast<IUnknown *>(*ppv)->AddRef();
        return S_OK;
    }
    
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CComponent::AddRef()
{
    return InterlockedIncrement((LONG *)&m_cref);
}

STDMETHODIMP_(ULONG) CComponent::Release()
{
    if (InterlockedDecrement((LONG *)&m_cref) == 0)
    {
        delete this;
        return 0;
    }
    return m_cref;
    
}

///////////////////////////////
// Interface IComponent
///////////////////////////////
STDMETHODIMP CComponent::Initialize( 
                                    /* [in] */ LPCONSOLE lpConsole)
{
    HRESULT hr = S_OK;
    
    // Save away all the interfaces we'll need.
    // Fail if we can't QI the required interfaces.
    
    m_ipConsole = lpConsole;
    m_ipConsole->AddRef();
    
    return hr;
}

STDMETHODIMP CComponent::Notify( 
                                /* [in] */ LPDATAOBJECT lpDataObject,
                                /* [in] */ MMC_NOTIFY_TYPE event,
                                /* [in] */ LPARAM arg,
                                /* [in] */ LPARAM param)
{
    MMCN_Crack(FALSE, lpDataObject, NULL, this, event, arg, param);

    HRESULT hr = S_FALSE;

	//Get our data object. If it is NULL, we return with S_FALSE.
	//See implementation of GetOurDataObject() to see how to
	//handle special data objects.
	CDataObject *pDataObject = GetOurDataObject(lpDataObject);
	if (NULL == pDataObject)
		return S_FALSE;
	
	CDelegationBase *base = pDataObject->GetBaseNodeObject();
    
    switch (event)	{
    case MMCN_SHOW: 
        hr = base->OnShow(m_ipConsole, (BOOL)arg, (HSCOPEITEM)param);
        break;
        
    case MMCN_ADD_IMAGES:
        hr = base->OnAddImages((IImageList *)arg, (HSCOPEITEM)param);
        break;
        
    case MMCN_SELECT:
        hr = base->OnSelect(m_ipConsole, (BOOL)LOWORD(arg), (BOOL)HIWORD(arg));
        break;
        
    case MMCN_RENAME:
        hr = base->OnRename((LPOLESTR)param);
        break;
        
    case MMCN_REFRESH:
        hr = base->OnRefresh();
        break;
    }
    
    return hr;
}

STDMETHODIMP CComponent::Destroy( 
                                 /* [in] */ MMC_COOKIE cookie)
{
    if (m_ipConsole) {
        m_ipConsole->Release();
        m_ipConsole = NULL;
    }
    
    return S_OK;
}


STDMETHODIMP CComponent::QueryDataObject( 
                                         /* [in] */ MMC_COOKIE cookie,
                                         /* [in] */ DATA_OBJECT_TYPES type,
                                         /* [out] */ LPDATAOBJECT __RPC_FAR *ppDataObject)
{
    CDataObject *pObj = NULL;
	CDelegationBase *pBase = NULL;

	if (IsBadReadPtr((void *)cookie, sizeof(CDelegationBase))) {
		if (NULL == m_pLastNode)
			return E_FAIL;

		pBase = m_pLastNode->GetChildPtr((int)cookie);
	} else {
		pBase = (cookie == 0) ? m_pComponentData->m_pStaticNode : (CDelegationBase *)cookie;
	}
    
	if (pBase == NULL)
		return E_FAIL;

    pObj = new CDataObject((MMC_COOKIE)pBase, type);

    if (!pObj)
        return E_OUTOFMEMORY;
    
    pObj->QueryInterface(IID_IDataObject, (void **)ppDataObject);
    
    return S_OK;
}

STDMETHODIMP CComponent::GetResultViewType( 
                                           /* [in] */ MMC_COOKIE cookie,
                                           /* [out] */ LPOLESTR __RPC_FAR *ppViewType,
                                           /* [out] */ long __RPC_FAR *pViewOptions)
{
    CDelegationBase *base = m_pLastNode = (CDelegationBase *)cookie;
    
    //
    // Ask for default listview.
    //
    if (base == NULL)
    {
        *pViewOptions = MMC_VIEW_OPTIONS_NONE;
        *ppViewType = NULL;
    }
    else
        return base->GetResultViewType(ppViewType, pViewOptions);
    
    return S_OK;
}

STDMETHODIMP CComponent::GetDisplayInfo( 
                                        /* [out][in] */ RESULTDATAITEM __RPC_FAR *pResultDataItem)
{
    HRESULT hr = S_OK;
    CDelegationBase *base = NULL;

    // if they are asking for the RDI_STR we have one of those to give

    if (pResultDataItem->lParam) {
        base = (CDelegationBase *)pResultDataItem->lParam;
        if (pResultDataItem->mask & RDI_STR) {
			LPCTSTR pszT = base->GetDisplayName(pResultDataItem->nCol);
			MAKE_WIDEPTR_FROMTSTR_ALLOC(pszW, pszT);
            pResultDataItem->str = pszW;
        }

        if (pResultDataItem->mask & RDI_IMAGE) {
            pResultDataItem->nImage = base->GetBitmapIndex();
        }
    } else {
        m_pLastNode->GetChildColumnInfo(pResultDataItem);
    }

    return hr;
}


STDMETHODIMP CComponent::CompareObjects( 
                                        /* [in] */ LPDATAOBJECT lpDataObjectA,
                                        /* [in] */ LPDATAOBJECT lpDataObjectB)
{
    CDelegationBase *baseA = GetOurDataObject(lpDataObjectA)->GetBaseNodeObject();
    CDelegationBase *baseB = GetOurDataObject(lpDataObjectB)->GetBaseNodeObject();
    
    // compare the object pointers
    if (baseA->GetCookie() == baseB->GetCookie())
        return S_OK;
    
    return S_FALSE;
}

///////////////////////////////
// Interface IComponent
///////////////////////////////
STDMETHODIMP CComponent::FindItem( 
/* [in] */ LPRESULTFINDINFO pFindInfo,
/* [out] */ int __RPC_FAR *pnFoundIndex)
{
    return E_NOTIMPL;
}

STDMETHODIMP CComponent::CacheHint( 
/* [in] */ int nStartIndex,
/* [in] */ int nEndIndex)
{
    return E_NOTIMPL;
}

STDMETHODIMP CComponent::SortItems( 
/* [in] */ int nColumn,
/* [in] */ DWORD dwSortOptions,
/* [in] */ LPARAM lUserParam)
{
    return E_NOTIMPL;
}
