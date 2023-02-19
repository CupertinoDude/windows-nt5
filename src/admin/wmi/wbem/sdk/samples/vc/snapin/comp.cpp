//==============================================================;

//

//      This source code is only intended as a supplement to existing Microsoft documentation.

//

//      Use of this code is NOT supported.

//

//

//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY

//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE

//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR

//  PURPOSE.

//

// Copyright (c) 1999-2001 Microsoft Corporation, All Rights Reserved 
//
//    Microsoft Premier Support for Developers
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
: m_pComponentData(parent), m_cref(0), m_ipConsole(NULL)
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

    // first things first, make sure that when MMC
    // asks if we do property sheets, that we actually
    // say "yes"
    else if (IsEqualIID(riid, IID_IExtendPropertySheet))
        *ppv = static_cast<IExtendPropertySheet2 *>(this);
    else if (IsEqualIID(riid, IID_IExtendPropertySheet2))
        *ppv = static_cast<IExtendPropertySheet2 *>(this);

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
    CDelegationBase *base = NULL;

    // we need to watch for property change and delegate it
    // a little differently, we're actually going to send
    // the CDelegationBase object pointer in the property page
    // PSN_APPLY handler via MMCPropPageNotify()
    if (MMCN_PROPERTY_CHANGE != event) {
        if (NULL == lpDataObject)
            return S_FALSE;

        base = GetOurDataObject(lpDataObject)->GetBaseNodeObject();
    } else {
        base = (CDelegationBase *)param;
    }

    switch (event)      {
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
        hr = base->OnRefresh(lpDataObject);
        break;

	// handle the property change notification if we need to do anything
    // special with it
    case MMCN_PROPERTY_CHANGE:
        hr = base->OnPropertyChange();
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

    if (cookie == 0)
        pObj = new CDataObject((MMC_COOKIE)m_pComponentData->m_pStaticNode, type);
    else
        pObj = new CDataObject(cookie, type);

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
    CDelegationBase *base = (CDelegationBase *)cookie;

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

///////////////////////////////////
// Interface IExtendPropertySheet2
///////////////////////////////////
HRESULT CComponent::CreatePropertyPages(
                                        /* [in] */ LPPROPERTYSHEETCALLBACK lpProvider,
                                        /* [in] */ LONG_PTR handle,
                                        /* [in] */ LPDATAOBJECT lpIDataObject)
{
    CDelegationBase *base = GetOurDataObject(lpIDataObject)->GetBaseNodeObject();

    return base->CreatePropertyPages(lpProvider, handle);
}

HRESULT CComponent::QueryPagesFor(
                                  /* [in] */ LPDATAOBJECT lpDataObject)
{
    CDelegationBase *base = GetOurDataObject(lpDataObject)->GetBaseNodeObject();

    return base->HasPropertySheets();
}

HRESULT CComponent::GetWatermarks(
                                  /* [in] */ LPDATAOBJECT lpIDataObject,
                                  /* [out] */ HBITMAP __RPC_FAR *lphWatermark,
                                  /* [out] */ HBITMAP __RPC_FAR *lphHeader,
                                  /* [out] */ HPALETTE __RPC_FAR *lphPalette,
                                  /* [out] */ BOOL __RPC_FAR *bStretch)
{
    CDelegationBase *base = GetOurDataObject(lpIDataObject)->GetBaseNodeObject();

    return base->GetWatermarks(lphWatermark, lphHeader, lphPalette, bStretch);
}
