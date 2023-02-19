//---------------------------------------------------------------------------

//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:  cclsobj.cxx
//
//  Contents:  Microsoft ADs NDS Provider Generic Object
//
//
//  History:   01-30-95     krishnag    Created.
//
//----------------------------------------------------------------------------
#include "nds.hxx"
#pragma hdrstop


// Class CNDSProperty

DECLARE_INFOLEVEL( Syntax );
DECLARE_DEBUG( Syntax );
#define SyntaxDebugOut(x) SyntaxInlineDebugOut x

DEFINE_IDispatch_Implementation(CNDSProperty)
DEFINE_IADs_Implementation(CNDSProperty)

CNDSProperty::CNDSProperty()
    : _pDispMgr( NULL ),
      _bstrOID( NULL ),
      _bstrSyntax( NULL ),
      _lMaxRange( 0 ),
      _lMinRange( 0 ),
      _fMultiValued( FALSE )
{

    ENLIST_TRACKING(CNDSProperty);
}

CNDSProperty::~CNDSProperty()
{
    delete _pDispMgr;
}

HRESULT
CNDSProperty::CreateProperty(
    BSTR   bstrParent,
    BSTR   bstrName,
    LPNDS_ATTR_DEF lpAttrDef,
    CCredentials& Credentials,
    DWORD  dwObjectState,
    REFIID riid,
    void **ppvObj
    )
{
    CNDSProperty FAR * pProperty = NULL;
    HRESULT hr = S_OK;
    WCHAR szADsSyntax[MAX_PATH];
    WCHAR szNDSSyntax[MAX_PATH];

    hr = AllocatePropertyObject( &pProperty );
    BAIL_ON_FAILURE(hr);

    hr = pProperty->InitializeCoreObject(
             bstrParent,
             bstrName,
             PROPERTY_CLASS_NAME,
             L"",
             CLSID_NDSProperty,
             dwObjectState
             );
    BAIL_ON_FAILURE(hr);

    hr = pProperty->QueryInterface( riid, ppvObj );
    BAIL_ON_FAILURE(hr);

#if DBG
        SyntaxDebugOut((DEB_TRACE,
                           "Property %s : SyntaxId %d\n",
                           lpAttrDef->szAttributeName,
                           lpAttrDef->dwSyntaxID));
#endif

    MapSyntaxIdtoADsSyntax(
            lpAttrDef->dwSyntaxID,
            szADsSyntax
            );

    hr = ADsAllocString(
                szADsSyntax,
                &pProperty->_bstrSyntax
                );
    BAIL_ON_FAILURE(hr);

    MapSyntaxIdtoNDSSyntax(
            lpAttrDef->dwSyntaxID,
            szNDSSyntax
            );

    hr = ADsAllocString(
                szNDSSyntax,
                &pProperty->_bstrOID
                );
    BAIL_ON_FAILURE(hr);

    pProperty->_lMaxRange = lpAttrDef->dwUpperLimit;
    pProperty->_lMinRange = lpAttrDef->dwLowerLimit;
    pProperty->_fMultiValued  = !(lpAttrDef->dwFlags & NDS_SINGLE_VALUED_ATTR);

    pProperty->Release();

    RRETURN(hr);

error:

    delete pProperty;
    RRETURN(hr);
}

HRESULT
CNDSProperty::CreateProperty(
    BSTR   bstrParent,
    BSTR   bstrName,
    NDS_CONTEXT_HANDLE hADsContext,
    CCredentials& Credentials,
    DWORD  dwObjectState,
    REFIID riid,
    void **ppvObj
    )
{
    DWORD dwStatus = 0;
    HRESULT hr = S_OK;
    LPNDS_ATTR_DEF lpAttrDefs = NULL;
    DWORD dwNumberOfEntries = 0;
    DWORD dwInfoType = 0;
    HANDLE hOperationData = NULL;

    hr = ADsNdsReadAttrDef(
                    hADsContext,
                    DS_ATTR_DEFS,
                    &bstrName,
                    1,
                    &hOperationData
                    );
    BAIL_ON_FAILURE(hr);

    hr = ADsNdsGetAttrDefListFromBuffer(
                    hADsContext,
                    hOperationData,
                    &dwNumberOfEntries,
                    &dwInfoType,
                    &lpAttrDefs
                    );
    BAIL_ON_FAILURE(hr);

    if (!lpAttrDefs) {
        hr = E_FAIL;
        BAIL_ON_FAILURE(hr);
    }

    hr = CNDSProperty::CreateProperty(
                bstrParent,
                bstrName,
                lpAttrDefs,
                Credentials,
                dwObjectState,
                riid,
                ppvObj
                );


error:
    if (hOperationData) {
        ADsNdsFreeBuffer(hOperationData);
    }

    ADsNdsFreeAttrDefList(lpAttrDefs, dwNumberOfEntries);

    RRETURN(hr);
}

STDMETHODIMP
CNDSProperty::QueryInterface(
    REFIID iid,
    LPVOID FAR* ppv
    )
{
    if (ppv == NULL) {
        RRETURN(E_POINTER);
    }

    if (IsEqualIID(iid, IID_IUnknown))
    {
        *ppv = (IADsProperty FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IDispatch))
    {
        *ppv = (IADsProperty FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IADsProperty))
    {
        *ppv = (IADsProperty FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IADs))
    {
        *ppv = (IADsProperty FAR *) this;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return NOERROR;
}

/* IADs methods */

STDMETHODIMP
CNDSProperty::SetInfo(THIS)
{
    RRETURN(E_NOTIMPL);
}

STDMETHODIMP
CNDSProperty::GetInfo(THIS)
{
    RRETURN(S_OK);
}

STDMETHODIMP
CNDSProperty::GetInfoEx(THIS_ VARIANT vProperties, long lnReserved)
{
    RRETURN(E_NOTIMPL);
}


HRESULT
CNDSProperty::AllocatePropertyObject(
    CNDSProperty FAR * FAR * ppProperty
    )
{
    CNDSProperty FAR *pProperty = NULL;
    CDispatchMgr FAR *pDispMgr = NULL;
    HRESULT hr = S_OK;

    pProperty = new CNDSProperty();
    if ( pProperty == NULL )
        hr = E_OUTOFMEMORY;
    BAIL_ON_FAILURE(hr);

    pDispMgr = new CDispatchMgr;
    if ( pDispMgr == NULL )
        hr = E_OUTOFMEMORY;
    BAIL_ON_FAILURE(hr);

    hr = LoadTypeInfoEntry(
            pDispMgr,
            LIBID_ADs,
            IID_IADsProperty,
            (IADsProperty *) pProperty,
            DISPID_REGULAR
            );
    BAIL_ON_FAILURE(hr);

    pProperty->_pDispMgr = pDispMgr;
    *ppProperty = pProperty;

    RRETURN(hr);

error:

    delete pDispMgr;
    delete pProperty;

    RRETURN(hr);

}


STDMETHODIMP
CNDSProperty::Get(
    THIS_ BSTR bstrName,
    VARIANT FAR* pvProp
    )
{
    RRETURN(E_NOTIMPL);
}



STDMETHODIMP
CNDSProperty::Put(
    THIS_ BSTR bstrName,
    VARIANT vProp
    )
{
    RRETURN(E_NOTIMPL);
}


STDMETHODIMP
CNDSProperty::GetEx(
    THIS_ BSTR bstrName,
    VARIANT FAR* pvProp
    )
{
    RRETURN(E_NOTIMPL);
}


STDMETHODIMP
CNDSProperty::PutEx(
    THIS_ long lnControlCode,
    BSTR bstrName,
    VARIANT vProp
    )
{
    RRETURN(E_NOTIMPL);
}


/* IADsProperty methods */

STDMETHODIMP
CNDSProperty::get_OID( THIS_ BSTR FAR *pbstrOID )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::put_OID( THIS_ BSTR bstrOID )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::get_Syntax( THIS_ BSTR FAR *pbstrSyntax )
{
    if ( !pbstrSyntax )
        RRETURN(E_ADS_BAD_PARAMETER);

    RRETURN( ADsAllocString( _bstrSyntax, pbstrSyntax ));
}

STDMETHODIMP
CNDSProperty::put_Syntax( THIS_ BSTR bstrSyntax )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::get_MaxRange( THIS_ long FAR *plMaxRange )
{
    if ( !plMaxRange )
        RRETURN(E_ADS_BAD_PARAMETER);

    *plMaxRange = _lMaxRange;
    RRETURN(S_OK);
}

STDMETHODIMP
CNDSProperty::put_MaxRange( THIS_ long lMaxRange )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::get_MinRange( THIS_ long FAR *plMinRange )
{
    if ( !plMinRange )
        RRETURN(E_ADS_BAD_PARAMETER);

    *plMinRange = _lMinRange;
    RRETURN(S_OK);
}

STDMETHODIMP
CNDSProperty::put_MinRange( THIS_ long lMinRange )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::get_MultiValued( THIS_ VARIANT_BOOL FAR *pfMultiValued )
{
    if ( !pfMultiValued )
        RRETURN(E_ADS_BAD_PARAMETER);

    *pfMultiValued = _fMultiValued? VARIANT_TRUE : VARIANT_FALSE;
    RRETURN(S_OK);
}

STDMETHODIMP
CNDSProperty::put_MultiValued( THIS_ VARIANT_BOOL fMultiValued )
{
    RRETURN(E_ADS_PROPERTY_NOT_SUPPORTED);
}

STDMETHODIMP
CNDSProperty::Qualifiers(THIS_ IADsCollection FAR* FAR* ppQualifiers)
{
    RRETURN(E_NOTIMPL);
}


HRESULT
MapSyntaxIdtoADsSyntax(
    DWORD dwSyntaxId,
    LPWSTR pszADsSyntax
    )
{

    if (dwSyntaxId >= g_cNDSSyntaxMap) {
        wcscpy(pszADsSyntax, L"Out of Bounds");
    }else {
        wcscpy(pszADsSyntax, g_aNDSSyntaxMap[dwSyntaxId].bstrName);
    }
    RRETURN(S_OK);

}

HRESULT
MapSyntaxIdtoNDSSyntax(
    DWORD dwSyntaxId,
    LPWSTR pszNDSSyntax
    )
{

    if (dwSyntaxId > g_cNDSSyntaxMap) {
        wcscpy(pszNDSSyntax, L"Out of Bounds");
    }else {
        wcscpy(pszNDSSyntax, g_aNDSSyntaxMap[dwSyntaxId].bstrNDSName);
    }
    RRETURN(S_OK);

}
