//---------------------------------------------------------------------------

//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:  cschema.cxx
//
//  Contents:  Microsoft ADs NDS Provider Schema Object
//
//
//  History:   01-30-95     krishnag    Created.
//
//----------------------------------------------------------------------------
#include "nds.hxx"
#pragma hdrstop

//  Class CNDSSchema

DEFINE_IDispatch_Implementation(CNDSSchema)
DEFINE_IADs_Implementation(CNDSSchema)


CNDSSchema::CNDSSchema()
{

    VariantInit(&_vFilter);

    _NDSTreeName = NULL;

    ENLIST_TRACKING(CNDSSchema);
}

HRESULT
CNDSSchema::CreateSchema(
    BSTR Parent,
    BSTR CommonName,
    CCredentials& Credentials,
    DWORD dwObjectState,
    REFIID riid,
    void **ppvObj
    )
{
    CNDSSchema FAR * pSchema = NULL;
    HRESULT hr = S_OK;
    LPWSTR pszNDSTreeName = NULL;

    hr = AllocateSchema(&pSchema, Credentials);
    BAIL_ON_FAILURE(hr);

    hr = pSchema->InitializeCoreObject(
                Parent,
                CommonName,
                SCHEMA_CLASS_NAME,
                L"",
                CLSID_NDSSchema,
                dwObjectState
                );
    BAIL_ON_FAILURE(hr);


    hr = BuildNDSPathFromADsPath(
                Parent,
                &pszNDSTreeName
                );
    BAIL_ON_FAILURE(hr);

    hr = ADsAllocString( pszNDSTreeName,  &pSchema->_NDSTreeName);
    BAIL_ON_FAILURE(hr);

    hr = pSchema->QueryInterface(riid, ppvObj);
    BAIL_ON_FAILURE(hr);

    pSchema->Release();

    if (pszNDSTreeName) {
        FreeADsStr(pszNDSTreeName);
    }

    RRETURN(hr);

error:

    if (pszNDSTreeName) {
        FreeADsStr(pszNDSTreeName);
    }

    delete pSchema;
    RRETURN_EXP_IF_ERR(hr);
}

CNDSSchema::~CNDSSchema( )
{
    VariantClear(&_vFilter);

    if (_NDSTreeName) {
        ADsFreeString(_NDSTreeName);
    }
    delete _pDispMgr;
}

STDMETHODIMP
CNDSSchema::QueryInterface(REFIID iid, LPVOID FAR* ppv)
{
    if (ppv == NULL) {
        RRETURN(E_POINTER);
    }

    if (IsEqualIID(iid, IID_IUnknown))
    {
        *ppv = (IADs FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IADsContainer))
    {
        *ppv = (IADsContainer FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IADs))
    {
        *ppv = (IADs FAR *) this;
    }
    else if (IsEqualIID(iid, IID_IDispatch))
    {
        *ppv = (IADs FAR *) this;
    }
    else if (IsEqualIID(iid, IID_ISupportErrorInfo)) 
    {
        *ppv = (ISupportErrorInfo FAR *) this;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return NOERROR;
}

/* ISupportErrorInfo method */
STDMETHODIMP
CNDSSchema::InterfaceSupportsErrorInfo(
    THIS_ REFIID riid
    )
{
    if (IsEqualIID(riid, IID_IADs) ||
        IsEqualIID(riid, IID_IADsContainer)) {
        RRETURN(S_OK);
    } else {
        RRETURN(S_FALSE);
    }
}

STDMETHODIMP
CNDSSchema::SetInfo(THIS)
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::GetInfo(THIS)
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::GetInfoEx(THIS_ VARIANT vProperties, long lnReserved)
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

/* IADsContainer methods */

STDMETHODIMP
CNDSSchema::get_Count(long FAR* retval)
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::get_Filter(THIS_ VARIANT FAR* pVar)
{
    HRESULT hr;
    VariantInit(pVar);
    hr = VariantCopy(pVar, &_vFilter);
    RRETURN_EXP_IF_ERR(hr);
}

STDMETHODIMP
CNDSSchema::put_Filter(THIS_ VARIANT Var)
{
    HRESULT hr;
    hr = VariantCopy(&_vFilter, &Var);
    RRETURN_EXP_IF_ERR(hr);
}

STDMETHODIMP
CNDSSchema::put_Hints(THIS_ VARIANT Var)
{
    RRETURN_EXP_IF_ERR( E_NOTIMPL);
}


STDMETHODIMP
CNDSSchema::get_Hints(THIS_ VARIANT FAR* pVar)
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::GetObject(
    THIS_ BSTR ClassName,
    BSTR RelativeName,
    IDispatch * FAR* ppObject
    )
{
    HRESULT hr;
    hr = ::RelativeGetObject(_ADsPath,
                             ClassName,
                             RelativeName,
                             _Credentials,
                             ppObject,
                             FALSE);
    RRETURN_EXP_IF_ERR(hr);
}

STDMETHODIMP
CNDSSchema::get__NewEnum(
    THIS_ IUnknown * FAR* retval
    )
{
    HRESULT hr;
    IUnknown FAR* punkEnum=NULL;
    IEnumVARIANT * penum = NULL;


    *retval = NULL;

    hr = CNDSSchemaEnum::Create(
                (CNDSSchemaEnum **)&penum,
                _NDSTreeName,
                _ADsPath,
                _Name,
                _vFilter,
                _Credentials
                );
    BAIL_ON_FAILURE(hr);

    hr = penum->QueryInterface(
                IID_IUnknown,
                (VOID FAR* FAR*)retval
                );
    BAIL_ON_FAILURE(hr);

    if (penum) {
        penum->Release();
    }

    RRETURN(NOERROR);

error:

    if (penum) {
        delete penum;
    }

    RRETURN_EXP_IF_ERR(hr);
}


STDMETHODIMP
CNDSSchema::Create(
    THIS_ BSTR ClassName,
    BSTR RelativeName,
    IDispatch * FAR* ppObject
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::Delete(
    THIS_ BSTR bstrClassName,
    BSTR bstrRelativeName
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::CopyHere(
    THIS_ BSTR SourceName,
    BSTR NewName,
    IDispatch * FAR* ppObject
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

STDMETHODIMP
CNDSSchema::MoveHere(
    THIS_ BSTR SourceName,
    BSTR NewName,
    IDispatch * FAR* ppObject
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}


HRESULT
CNDSSchema::AllocateSchema(
    CNDSSchema ** ppSchema,
    CCredentials& Credentials
    )
{
    CNDSSchema FAR * pSchema = NULL;
    CDispatchMgr FAR * pDispMgr = NULL;
    HRESULT hr = S_OK;

    pSchema = new CNDSSchema();
    if (pSchema == NULL) {
        hr = E_OUTOFMEMORY;
    }
    BAIL_ON_FAILURE(hr);

    pDispMgr = new CDispatchMgr;
    if (pDispMgr == NULL) {
        hr = E_OUTOFMEMORY;
    }
    BAIL_ON_FAILURE(hr);

    hr = LoadTypeInfoEntry(pDispMgr,
                           LIBID_ADs,
                           IID_IADs,
                           (IADsDomain *)pSchema,
                           DISPID_REGULAR
                           );
    BAIL_ON_FAILURE(hr);

    hr = LoadTypeInfoEntry(pDispMgr,
                           LIBID_ADs,
                           IID_IADsContainer,
                           (IADsContainer *)pSchema,
                           DISPID_NEWENUM
                           );
    BAIL_ON_FAILURE(hr);

    pSchema->_Credentials = Credentials;
    pSchema->_pDispMgr = pDispMgr;
    *ppSchema = pSchema;

    RRETURN(hr);

error:
    delete  pDispMgr;

    RRETURN(hr);

}


STDMETHODIMP
CNDSSchema::GetInfo(
    THIS_ DWORD dwApiLevel,
    BOOL fExplicit
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}


STDMETHODIMP
CNDSSchema::Get(
    THIS_ BSTR bstrName,
    VARIANT FAR* pvProp
    )
{

    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}



STDMETHODIMP
CNDSSchema::Put(
    THIS_ BSTR bstrName,
    VARIANT vProp
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}



STDMETHODIMP
CNDSSchema::GetEx(
    THIS_ BSTR bstrName,
    VARIANT FAR* pvProp
    )
{

    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}


STDMETHODIMP
CNDSSchema::PutEx(
    THIS_ long lnControlCode,
    BSTR bstrName,
    VARIANT vProp
    )
{
    RRETURN_EXP_IF_ERR(E_NOTIMPL);
}

