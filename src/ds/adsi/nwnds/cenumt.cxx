//---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:      cenumdom.cxx
//
//  Contents:  NDS Object Enumeration Code
//
//              CNDSTreeEnum::CNDSTreeEnum()
//              CNDSTreeEnum::CNDSTreeEnum
//              CNDSTreeEnum::EnumObjects
//              CNDSTreeEnum::EnumObjects
//
//  History:
//----------------------------------------------------------------------------
#include "NDS.hxx"
#pragma hdrstop


//+---------------------------------------------------------------------------
//
//  Function:   CNDSEnumVariant::Create
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
//  History:    01-30-95   krishnag     Created.
//
//----------------------------------------------------------------------------
HRESULT
CNDSTreeEnum::Create(
    CNDSTreeEnum FAR* FAR* ppenumvariant,
    BSTR ADsPath,
    VARIANT var,
    CCredentials& Credentials
    )
{
    HRESULT hr = NOERROR;
    CNDSTreeEnum FAR* penumvariant = NULL;
    WCHAR szObjectFullName[MAX_PATH];
    WCHAR szObjectClassName[MAX_PATH];
    LPWSTR pszTreeName = NULL, pszDn = NULL;
    DWORD dwModificationTime = 0L;
    DWORD dwNumberOfEntries = 0L;
    DWORD dwStatus = 0L;

    *ppenumvariant = NULL;

    penumvariant = new CNDSTreeEnum();
    if (!penumvariant) {
        hr = E_OUTOFMEMORY;
        BAIL_ON_FAILURE(hr);
    }

    hr = ADsAllocString( ADsPath, &penumvariant->_ADsPath);
    BAIL_ON_FAILURE(hr);

    hr = BuildNDSFilterArray(
                var,
                (LPBYTE *)&penumvariant->_pNdsFilterList
                );
    if (FAILED(hr)) {
        penumvariant->_pNdsFilterList = NULL;
    }

    /*
    hr = ObjectTypeList::CreateObjectTypeList(
            var,
            &penumvariant->_pObjList
            );
    BAIL_ON_FAILURE(hr);
    */
    penumvariant->_Credentials = Credentials;

    *ppenumvariant = penumvariant;

    hr = BuildNDSPathFromADsPath2(
                ADsPath,
                &pszTreeName,
                &pszDn
                );
    BAIL_ON_FAILURE(hr);

    hr = ADsNdsOpenContext(
             pszTreeName,
             Credentials,
             &penumvariant->_hADsContext
             );

    if (pszTreeName) {
        FreeADsStr(pszTreeName);
    }

    if (pszDn) {
        FreeADsStr(pszDn);
    }

    RRETURN(hr);

error:

    if (penumvariant) {

    }

    delete penumvariant;
    *ppenumvariant = NULL;

    if (pszTreeName) {

        FreeADsStr(pszTreeName);
    }

    if (pszDn) {

        FreeADsStr(pszDn);
    }

    RRETURN(hr);
}

CNDSTreeEnum::CNDSTreeEnum():
                    _ADsPath(NULL)
{
    _pObjList = NULL;
    _dwObjectReturned = 0;
    _dwObjectCurrentEntry = 0;
    _dwObjectTotal = 0;
    _hADsContext = NULL;
    _hOperationData = NULL;
    _lpObjects = NULL;
    _pNdsFilterList = NULL;

    _fSchemaReturned = NULL;
    _bNoMore = FALSE;
}


CNDSTreeEnum::~CNDSTreeEnum()
{
    if (_ADsPath) {
            ADsFreeString(_ADsPath);
    }

    if (_pNdsFilterList) {
        FreeFilterList((LPBYTE)_pNdsFilterList);
    }

    if (_hADsContext) {
        ADsNdsCloseContext(_hADsContext);
    }

    ADsNdsFreeNdsObjInfoList(_lpObjects, _dwObjectReturned);
 
}

HRESULT
CNDSTreeEnum::EnumObjects(
    DWORD ObjectType,
    ULONG cElements,
    VARIANT FAR * pvar,
    ULONG FAR * pcElementFetched
    )
{

    //
    // Multi-level detection of Objects may not be necessary for NDS code
    //

    RRETURN(EnumGenericObjects(cElements, pvar, pcElementFetched));


    //
    // BugBug- commenting out this layer of code
    //

    /*
    switch (ObjectType) {

    default:
        RRETURN(EnumObjects(cElements, pvar, pcElementFetched));

    } */
}

HRESULT
CNDSTreeEnum::EnumObjects(
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
    HRESULT         hr;
    DWORD           ObjectType;

    for (i = 0; i < cElements; i++)  {
        VariantInit(&pvar[i]);
    }
    cRequested = cElements;

    while (SUCCEEDED(_pObjList->GetCurrentObject(&ObjectType)) &&
            ((hr = EnumObjects(ObjectType,
                               cRequested,
                               pPathvar,
                               &cFetchedByPath)) == S_FALSE )) {

        pPathvar += cFetchedByPath;
        cRequested -= cFetchedByPath;
        cTotalFetched += cFetchedByPath;

        cFetchedByPath = 0;

        if (FAILED(_pObjList->Next())){
            if (pcElementFetched)
                *pcElementFetched = cTotalFetched;
            RRETURN(S_FALSE);
        }

    }

    if (pcElementFetched) {
        *pcElementFetched = cTotalFetched + cFetchedByPath;
    }

    RRETURN(hr);
}

HRESULT
CNDSTreeEnum::EnumGenericObjects(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    HRESULT hr = S_OK;
    IDispatch *pDispatch = NULL;
    DWORD i = 0;

    while (i < cElements) {

        hr = GetGenObject(&pDispatch);
        if (hr == S_FALSE) {
            break;
        }

        VariantInit(&pvar[i]);
        pvar[i].vt = VT_DISPATCH;
        pvar[i].pdispVal = pDispatch;
        (*pcElementFetched)++;
        i++;
    }
    return(hr);
}


HRESULT
CNDSTreeEnum::GetGenObject(
    IDispatch ** ppDispatch
    )
{
    HRESULT hr = S_OK;
    DWORD dwStatus = 0L;
    PADSNDS_OBJECT_INFO lpCurrentObject = NULL;
    IADs * pADs = NULL;

    *ppDispatch = NULL;

    if (!_hOperationData || (_dwObjectCurrentEntry == _dwObjectReturned)) {

        if (_hOperationData) {

            ADsNdsFreeNdsObjInfoList(_lpObjects, _dwObjectReturned);
            _lpObjects = NULL;
        }

        _dwObjectCurrentEntry = 0;
        _dwObjectReturned = 0;

        if (_bNoMore) {

            hr = S_FALSE;
            goto error;
        }

        
        hr = ADsNdsListObjects(
                 _hADsContext,
                 L"[Root]",
                 L"", 
                 L"",
                 NULL,
                 FALSE,
                 &_hOperationData
                 );

        BAIL_ON_FAILURE(hr);

        if (hr == S_ADS_NOMORE_ROWS) {
            _bNoMore = TRUE;
        }

        hr = ADsNdsGetObjectListFromBuffer(
                 _hADsContext,   
                 _hOperationData,
                 &_dwObjectReturned,
                 &_lpObjects
                 );
        BAIL_ON_FAILURE(hr);

        if (_dwObjectReturned == 0 ) {

            RRETURN (S_FALSE);
            goto error;
        }

    }

    //
    // Now send back the current object
    //

    lpCurrentObject = _lpObjects + _dwObjectCurrentEntry;

    //
    // Bump up the object count. The instantiation of this object
    // may fail; if we come into this function again, we do not want
    // to pick up the same object.
    //

    _dwObjectCurrentEntry++;

    hr = CNDSGenObject::CreateGenericObject(
                        _ADsPath,
                        lpCurrentObject->szObjectName,
                        lpCurrentObject->szObjectClass,
                        _Credentials,
                        ADS_OBJECT_BOUND,
                        IID_IADs,
                        (void **)&pADs
                        );
    BAIL_ON_FAILURE(hr);


    //
    // InstantiateDerivedObject should addref this pointer for us.
    //

    hr = InstantiateDerivedObject(
                    pADs,
                    _Credentials,
                    IID_IDispatch,
                    (void **)ppDispatch
                    );

    if (FAILED(hr)) {
        hr = pADs->QueryInterface(
                        IID_IDispatch,
                        (void **)ppDispatch
                        );
        BAIL_ON_FAILURE(hr);
    }


error:

    if (hr == S_FALSE) {
        ADsNdsFreeBuffer(_hOperationData);
        _hOperationData = NULL;
    }

    //
    // GetGenObject returns only S_FALSE
    //

    if (FAILED(hr)) {
        hr = S_FALSE;
    }

    //
    // Free the intermediate pADs pointer.
    //
    if (pADs) {
        pADs->Release();
    }


    RRETURN(hr);
}


//+---------------------------------------------------------------------------
//
//  Function:   CNDSTreeEnum::Next
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
//  History:    11-3-95   krishnag     Created.
//
//----------------------------------------------------------------------------
STDMETHODIMP
CNDSTreeEnum::Next(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
    )
{
    ULONG cElementFetched = 0;
    HRESULT hr = S_OK;

    hr = EnumGenericObjects(
            cElements,
            pvar,
            &cElementFetched
            );


    if (pcElementFetched) {
        *pcElementFetched = cElementFetched;
    }
    RRETURN(hr);
}


HRESULT
CNDSTreeEnum::EnumSchema(
    ULONG cElements,
    VARIANT FAR* pvar,
    ULONG FAR* pcElementFetched
)
{
    HRESULT hr = S_OK;
    IDispatch *pDispatch = NULL;

    if ( _fSchemaReturned )
        RRETURN(S_FALSE);

    if ( cElements > 0 )
    {
        hr = CNDSSchema::CreateSchema(
                  _ADsPath,
                  TEXT("Schema"),
                  _Credentials,
                  ADS_OBJECT_BOUND,
                  IID_IDispatch,
                  (void **)&pDispatch
                  );

        if ( hr == S_OK )
        {
            VariantInit(&pvar[0]);
            pvar[0].vt = VT_DISPATCH;
            pvar[0].pdispVal = pDispatch;
            (*pcElementFetched)++;
            _fSchemaReturned = TRUE;
        }
    }

    RRETURN(hr);
}

