//---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:      cenumsch.cxx
//
//  Contents:  NDS Schema Enumeration Code
//
//             CNDSSchemaEnum::CNDSSchemaEnum()
//             CNDSSchemaEnum::CNDSSchemaEnum
//             CNDSSchemaEnum::EnumObjects
//             CNDSSchemaEnum::EnumObjects
//
//  History:
//----------------------------------------------------------------------------
#include "NDS.hxx"
#pragma hdrstop

//+---------------------------------------------------------------------------
//
//  Function:   CNDSSchemaEnum::Create
//
//  Synopsis:
//
//  Arguments:  [pCollection]
//              [ppEnumVariant]
//
//  Returns:    HRESULT
//
//  Modifies:
//
//  History:    01-30-95   yihsins     Created.
//
//----------------------------------------------------------------------------
HRESULT
CNDSSchemaEnum::Create(
    CNDSSchemaEnum FAR* FAR* ppenumvariant,
    BSTR bstrNDSTreeName,
    BSTR bstrADsPath,
    BSTR bstrName,
    VARIANT var,
    CCredentials& Credentials
    )
{
    HRESULT hr = S_OK;
    CNDSSchemaEnum FAR* penumvariant = NULL;

    *ppenumvariant = NULL;

    penumvariant = new CNDSSchemaEnum();
    if (!penumvariant)
    {
        hr = E_OUTOFMEMORY;
        BAIL_ON_FAILURE(hr);
    }

    hr = ADsAllocString( bstrNDSTreeName, &penumvariant->_bstrNDSTreeName);
    BAIL_ON_FAILURE(hr);

    hr = ADsNdsOpenContext(
             penumvariant->_bstrNDSTreeName,
             Credentials,
             &penumvariant->_hADsContext
             );
    BAIL_ON_FAILURE(hr);

    hr = ADsAllocString( bstrADsPath, &penumvariant->_bstrADsPath);
    BAIL_ON_FAILURE(hr);

    hr = ADsAllocString( bstrName, &penumvariant->_bstrName);
    BAIL_ON_FAILURE(hr);

    hr = ObjectTypeList::CreateObjectTypeList(
            var,
            &penumvariant->_pObjList
            );
    BAIL_ON_FAILURE(hr);

    penumvariant->_Credentials = Credentials;

    *ppenumvariant = penumvariant;

    RRETURN(hr);

error:

    delete penumvariant;

    RRETURN(hr);
}

CNDSSchemaEnum::CNDSSchemaEnum()
    : _bstrADsPath( NULL ),
      _bstrName( NULL ),
      _bstrNDSTreeName( NULL ),
      _pObjList( NULL ),
      _dwCurrentEntry( 0 ),
      _dwSyntaxCurrentEntry( 0 )
{
    _hOperationData = NULL;
    _hADsContext = NULL;
    _lpClassDefs = NULL;

    _dwObjectCurrentEntry = 0;
    _dwObjectReturned = 0;

    _dwInfoType = 0;


    _dwPropCurrentEntry = 0;

    _hPropOperationData = NULL;
    _lpAttrDefs = NULL;

    _dwPropObjectCurrentEntry = 0;
    _dwPropObjectReturned = 0;
    _dwPropInfoType = 0;

    _bNoMore = FALSE;
    _bNoMoreProp = FALSE;

}

CNDSSchemaEnum::~CNDSSchemaEnum()
{
   ADsFreeString( _bstrName );
   ADsFreeString( _bstrADsPath );
   ADsFreeString( _bstrNDSTreeName );

   ADsNdsFreeClassDefList(_lpClassDefs, _dwObjectReturned);
   ADsNdsFreeAttrDefList(_lpAttrDefs, _dwPropObjectReturned);

   ADsNdsFreeBuffer( _hOperationData );
   ADsNdsFreeBuffer( _hPropOperationData );

   if (_hADsContext) {
       ADsNdsCloseContext(_hADsContext);
   }


   if ( _pObjList != NULL )
   {
       delete _pObjList;
       _pObjList = NULL;
   }
}

//+---------------------------------------------------------------------------
//
//  Function:   CNDSSchemaEnum::Next
//
//  Synopsis:   Returns cElements number of requested NetOle objects in the
//              array supplied in pvar.
//
//  Arguments:  [cElements] -- The number of elements requested by client
//              [pvar] -- ptr to array of VARIANTs to for return objects
//              [pcElementFetched] -- if non-NULL, then number of elements
//                                 -- actually returned is placed here
//
//  Returns:    HRESULT -- S_OK if number of elements requested are returned
//                      -- S_FALSE if number of elements is < requested
//
//  Modifies:
//
//  History:    11-3-95   yihsins     Created.
//
//----------------------------------------------------------------------------
STDMETHODIMP
CNDSSchemaEnum::Next(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    ULONG cElementFetched = 0;
    HRESULT hr = S_OK;

    hr = EnumObjects(
                cElements,
                pvar,
                &cElementFetched
                );

    if ( pcElementFetched )
        *pcElementFetched = cElementFetched;

    RRETURN(hr);
}

HRESULT
CNDSSchemaEnum::EnumObjects(
    DWORD ObjectType,
    ULONG cElements,
    VARIANT FAR * pvar,
    ULONG FAR * pcElementFetched
    )
{
    switch (ObjectType)
    {
        case NDS_CLASS_ID:
            RRETURN (EnumClasses(cElements, pvar, pcElementFetched));

        case NDS_PROPERTY_ID:
            RRETURN (EnumProperties(cElements, pvar, pcElementFetched));

    case NDS_SYNTAX_ID:
            RRETURN(EnumSyntaxes(cElements, pvar, pcElementFetched));

        default:
            RRETURN(S_FALSE);
    }
}

HRESULT
CNDSSchemaEnum::EnumObjects(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    DWORD           i;
    ULONG           cRequested = 0;
    ULONG           cFetchedByPath = 0;
    ULONG           cTotalFetched = 0;
    VARIANT FAR*    pPathvar = pvar;
    HRESULT         hr = S_OK;
    DWORD           ObjectType;

    for (i = 0; i < cElements; i++)
        VariantInit(&pvar[i]);

    cRequested = cElements;

    while (  SUCCEEDED( _pObjList->GetCurrentObject(&ObjectType))
          && ((hr = EnumObjects( ObjectType,
                                 cRequested,
                                 pPathvar,
                                 &cFetchedByPath)) == S_FALSE )
          )
    {
        pPathvar += cFetchedByPath;
        cRequested -= cFetchedByPath;
        cTotalFetched += cFetchedByPath;

        cFetchedByPath = 0;

        if ( FAILED(_pObjList->Next()) )
        {
            if ( pcElementFetched )
                *pcElementFetched = cTotalFetched;
            RRETURN(S_FALSE);
        }

        _dwCurrentEntry = 0;
    }

    if ( pcElementFetched )
        *pcElementFetched = cTotalFetched + cFetchedByPath;

    RRETURN(hr);
}

HRESULT
CNDSSchemaEnum::EnumClasses(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    HRESULT hr = S_OK;
    DWORD i = 0;
    IDispatch *pDispatch = NULL;

    while ( i < cElements )
    {
        hr = GetClassObject(&pDispatch);
        if ( hr == S_FALSE )
            break;

        VariantInit( &pvar[i] );
        pvar[i].vt = VT_DISPATCH;
        pvar[i].pdispVal = pDispatch;
        (*pcElementFetched)++;
        i++;
    }

    RRETURN(hr);
}

HRESULT
CNDSSchemaEnum::GetClassObject(
    IDispatch ** ppDispatch
    )
{
    HRESULT hr = S_OK;
    PNDS_CLASS_DEF lpCurrentObject = NULL;

    if (!_hOperationData || (_dwObjectCurrentEntry == _dwObjectReturned)) {

        _dwObjectCurrentEntry = 0;
        _dwObjectReturned = 0;

        if (_hOperationData) {

            ADsNdsFreeClassDefList(_lpClassDefs, _dwObjectReturned);
            _lpClassDefs = NULL;
        }

        if (_bNoMore) {

            hr = S_FALSE;
            goto error;
        }

        hr = ADsNdsReadClassDef(
                        _hADsContext,
                        DS_CLASS_DEFS,
                        NULL,
                        (DWORD) -1,
                        &_hOperationData
                        );
        BAIL_ON_FAILURE(hr);

        if (hr == S_ADS_NOMORE_ROWS) {
            _bNoMore = TRUE;
        }

        hr = ADsNdsGetClassDefListFromBuffer(
                        _hADsContext,
                        _hOperationData,
                        &_dwObjectReturned,
                        &_dwInfoType,
                        &_lpClassDefs
                        );
        BAIL_ON_FAILURE(hr);

        if (_dwObjectReturned == 0 ) {

            RRETURN (S_FALSE);
            goto error;
        }
    }

    if (_dwObjectCurrentEntry < _dwObjectReturned) {

        //
        // Now send back the current object
        //

        lpCurrentObject = _lpClassDefs + _dwObjectCurrentEntry;

        hr = CNDSClass::CreateClass(
                            _bstrADsPath,
                            lpCurrentObject->szClassName,
                            lpCurrentObject,
                            _Credentials,
                            ADS_OBJECT_BOUND,
                            IID_IDispatch,
                            (void **)ppDispatch
                            );
        BAIL_ON_FAILURE(hr);
        _dwObjectCurrentEntry++;

        RRETURN(S_OK);

    }

error:

    *ppDispatch = NULL;
    RRETURN(S_FALSE);
}



HRESULT
CNDSSchemaEnum::EnumProperties(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    HRESULT hr = S_OK;
    DWORD i = 0;
    IDispatch *pDispatch = NULL;

    while ( i < cElements )
    {
        hr = GetPropertyObject(&pDispatch);
        if ( hr == S_FALSE )
            break;

        VariantInit( &pvar[i] );
        pvar[i].vt = VT_DISPATCH;
        pvar[i].pdispVal = pDispatch;
        (*pcElementFetched)++;
        i++;
    }

    RRETURN(hr);
}

HRESULT
CNDSSchemaEnum::GetPropertyObject(
    IDispatch ** ppDispatch
    )
{
    HRESULT hr = S_OK;
    LPNDS_ATTR_DEF lpCurrentPropObject = NULL;
    DWORD dwStatus;

    if (!_hPropOperationData || (_dwPropObjectCurrentEntry == _dwPropObjectReturned)) {

        _dwPropObjectCurrentEntry = 0;
        _dwPropObjectReturned = 0;

        if (_hPropOperationData) {

            ADsNdsFreeAttrDefList(_lpAttrDefs, _dwPropObjectReturned);
            _lpAttrDefs = NULL;
        }

        if (_bNoMoreProp) {

            hr = S_FALSE;
            goto error;
        }

        hr = ADsNdsReadAttrDef(
                        _hADsContext,
                        DS_ATTR_DEFS,
                        NULL,
                        (DWORD) -1,
                        &_hPropOperationData
                        );
        BAIL_ON_FAILURE(hr);

        if (hr == S_ADS_NOMORE_COLUMNS) {
            _bNoMoreProp = TRUE;
        }

        hr = ADsNdsGetAttrDefListFromBuffer(
                        _hADsContext,
                        _hPropOperationData,
                        &_dwPropObjectReturned,
                        &_dwInfoType,
                        &_lpAttrDefs
                        );
        BAIL_ON_FAILURE(hr);

        if (_dwPropObjectReturned == 0 ) {

            RRETURN (S_FALSE);
            goto error;
        }

    }

    if (_dwPropObjectCurrentEntry < _dwPropObjectReturned) {

        //
        // Now send back the current object
        //

        lpCurrentPropObject = _lpAttrDefs + _dwPropObjectCurrentEntry;

        hr = CNDSProperty::CreateProperty(
                            _bstrADsPath,
                            lpCurrentPropObject->szAttributeName,
                            lpCurrentPropObject,
                            _Credentials,
                            ADS_OBJECT_BOUND,
                            IID_IDispatch,
                            (void **)ppDispatch
                            );
        BAIL_ON_FAILURE(hr);
        _dwPropObjectCurrentEntry++;

        RRETURN(S_OK);

    }

error:

    *ppDispatch = NULL;
    RRETURN(S_FALSE);
}




HRESULT
CNDSSchemaEnum::EnumSyntaxes(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    HRESULT hr = S_OK;
    DWORD i = 0;
    IDispatch *pDispatch = NULL;

    while ( i < cElements )
    {
        hr = GetSyntaxObject(&pDispatch);
        if ( hr == S_FALSE )
            break;

        VariantInit( &pvar[i] );
        pvar[i].vt = VT_DISPATCH;
        pvar[i].pdispVal = pDispatch;
        (*pcElementFetched)++;
        i++;
    }

    RRETURN(hr);
}

HRESULT
CNDSSchemaEnum::GetSyntaxObject(
    IDispatch ** ppDispatch
    )
{
    HRESULT hr = S_OK;

    //
    // Now send back the current object
    //
    if ( _dwSyntaxCurrentEntry >= g_cNDSSyntax )
        goto error;

    hr = CNDSSyntax::CreateSyntax(
                        _bstrADsPath,
                        &g_aNDSSyntax[_dwSyntaxCurrentEntry],
                        ADS_OBJECT_BOUND,
                        IID_IDispatch,
                        (void **)ppDispatch
                        );
    BAIL_ON_FAILURE(hr);

    _dwSyntaxCurrentEntry++;

    RRETURN(S_OK);

error:

    *ppDispatch = NULL;
    RRETURN(S_FALSE);
}
