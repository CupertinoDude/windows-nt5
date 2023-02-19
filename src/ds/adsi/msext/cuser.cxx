//----------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1996
//
//  File:  cuser.cxx
//
//  Contents:  Host user object code
//
//  History:   11-1-95     krishnag    Created.
//
//----------------------------------------------------------------------------

#include "ldap.hxx"
#pragma hdrstop

struct _propmap
{
    LPTSTR pszADsProp;
    LPTSTR pszLDAPProp;
} aUserPropMapping[] =
{
  { TEXT("BadLoginCount"), TEXT("badPwdCount") },
  { TEXT("LastLogin"), TEXT("lastLogon") },
  { TEXT("LastLogoff"), TEXT("lastLogoff") },
  { TEXT("LastFailedLogin"), TEXT("badPasswordTime") },
  { TEXT("PasswordLastChanged"), TEXT("pwdLastSet") },
  { TEXT("Description"), TEXT("description") },
  { TEXT("Division"), TEXT("division") },
  { TEXT("Department"), TEXT("department") },
  { TEXT("EmployeeID"), TEXT("employeeID") },
  { TEXT("FullName"), TEXT("displayName") },
  { TEXT("FirstName"), TEXT("givenName") },
  { TEXT("LastName"), TEXT("sn") },
  { TEXT("OtherName"), TEXT("middleName") },
  { TEXT("NamePrefix"), TEXT("personalTitle") },
  { TEXT("NameSuffix"), TEXT("generationQualifier") },
  { TEXT("Title"), TEXT("title") },
  { TEXT("Manager"), TEXT("manager") },
  { TEXT("TelephoneNumber"), TEXT("telephoneNumber") },
  { TEXT("TelephoneHome"), TEXT("homePhone") },
  { TEXT("TelephoneMobile"), TEXT("mobile") },
  { TEXT("TelephonePager"), TEXT("pager") },
  { TEXT("FaxNumber"), TEXT("facsimileTelephoneNumber") },
  { TEXT("OfficeLocations"), TEXT("physicalDeliveryOfficeName") },
  { TEXT("PostalAddresses"), TEXT("postalAddress") },
  { TEXT("PostalCodes"), TEXT("postalCode") },
  { TEXT("SeeAlso"), TEXT("seeAlso") },
  { TEXT("AccountExpirationDate"), TEXT("accountExpires") },
  { TEXT("LoginHours"), TEXT("logonHours") },
  { TEXT("LoginWorkstations"), TEXT("userWorkstations") },
  { TEXT("MaxStorage"), TEXT("maxStorage") },
  { TEXT("EmailAddress"), TEXT("mail") },
  { TEXT("HomeDirectory"), TEXT("homeDirectory") },
  { TEXT("Profile"), TEXT("profilePath") },
  { TEXT("LoginScript"), TEXT("scriptPath") },
  { TEXT("Picture"), TEXT("thumbnailPhoto") },
  { TEXT("HomePage"), TEXT("wWWHomePage") }
};

DWORD dwNumUserPropMapping = sizeof(aUserPropMapping)/sizeof(_propmap);


//  Class CLDAPUser

DEFINE_IPrivateDispatch_Implementation(CLDAPUser)

// IADsExtension::PrivateGetIDsOfNames()/Invoke(), Operate() not included
DEFINE_IADsExtension_Implementation(CLDAPUser)

DEFINE_DELEGATING_IDispatch_Implementation(CLDAPUser)
DEFINE_CONTAINED_IADs_Implementation(CLDAPUser)
DEFINE_CONTAINED_IADsPutGet_Implementation(CLDAPUser,aUserPropMapping)


CLDAPUser::CLDAPUser():
        _pUnkOuter(NULL),
        _pADs(NULL),
        _fDispInitialized(FALSE),
        _pDispMgr(NULL)
{
    ENLIST_TRACKING(CLDAPUser);
}

HRESULT
CLDAPUser::CreateUser(
    IUnknown *pUnkOuter,
    REFIID riid,
    void **ppvObj
    )
{

    HRESULT hr = S_OK;
    CLDAPUser FAR * pUser = NULL;
    IADs FAR *  pADs = NULL;
    CAggregateeDispMgr FAR * pDispMgr = NULL;


    //
    // our extension object only works in a provider (aggregator) environment
    // environment
    //

    ASSERT(pUnkOuter);
    ASSERT(ppvObj);
    ASSERT(IsEqualIID(riid, IID_IUnknown));


    pUser = new CLDAPUser();
    if (pUser == NULL) {
        hr = E_OUTOFMEMORY;
        BAIL_ON_FAILURE(hr);
    }

    //
    // Ref Count = 1 from object tracker
    //

    //
    // CAggregateeDispMgr to handle
    // IADsExtension::PrivateGetIDsOfNames()/PrivatInovke()
    //

    pDispMgr = new CAggregateeDispMgr;
    if (pDispMgr == NULL) {
        hr = E_OUTOFMEMORY;
        BAIL_ON_FAILURE(hr);
    }

    pUser->_pDispMgr = pDispMgr;

    hr = pDispMgr->LoadTypeInfoEntry(
                LIBID_ADs,
                IID_IADsUser,
                (IADsUser *)pUser,
                DISPID_REGULAR
                );
    BAIL_ON_FAILURE(hr);


    //
    // Store the pointer to the pUnkOuter object to delegate all IUnknown
    // calls to the aggregator AND DO NOT add ref this pointer
    //
    pUser->_pUnkOuter = pUnkOuter;


    //
    // Ccache pADs Pointer to delegate all IDispatch calls to
    // the aggregator. But release immediately to avoid the aggregatee
    // having a reference count on the aggregator -> cycle ref counting
    //

    hr = pUnkOuter->QueryInterface(
                IID_IADs,
                (void **)&pADs
                );

    //
    // Our spec stated extesnion writers can expect the aggregator in our
    // provider ot support IDispatch. If not, major bug.
    //

    ASSERT(SUCCEEDED(hr));
    pADs->Release();            // see doc above pUnkOuter->QI
    pUser->_pADs = pADs;


    //
    // pass non-delegating IUnknown back to the aggregator
    //

    *ppvObj = (INonDelegatingUnknown FAR *) pUser;


    RRETURN(hr);


error:

    //
    // do NOT clean up dispMgr here. Iff created, cleaned up Once inside pUser
    //

    if (pUser)
        delete  pUser;

    *ppvObj = NULL;

    RRETURN(hr);

}



STDMETHODIMP
CLDAPUser::ADSIInitializeObject(
    THIS_ BSTR lpszUserName,
    BSTR lpszPassword,
    long lnReserved
    )
{

    CCredentials NewCredentials(lpszUserName, lpszPassword, lnReserved);

    _Credentials = NewCredentials;

    RRETURN(S_OK);
}


CLDAPUser::~CLDAPUser( )
{
    //
    // Remember that the aggregatee has no reference counts to
    // decrement.
    //

    delete _pDispMgr;

}


STDMETHODIMP
CLDAPUser::QueryInterface(
    REFIID iid,
    LPVOID FAR* ppv
    )
{
    HRESULT hr = S_OK;

    hr = _pUnkOuter->QueryInterface(iid,ppv);

    RRETURN(hr);

}


STDMETHODIMP
CLDAPUser::NonDelegatingQueryInterface(
    REFIID iid,
    LPVOID FAR* ppv
    )
{
    ASSERT(ppv);

    if (IsEqualIID(iid, IID_IADsUser)) {

        *ppv = (IADsUser FAR *) this;

    } else if (IsEqualIID(iid, IID_IADsExtension)) {

        *ppv = (IADsExtension FAR *) this;

    } else if (IsEqualIID(iid, IID_IUnknown)) {

        //
        // probably not needed since our 3rd party extension does not stand
        // alone and provider does not ask for this, but to be safe
        //
        *ppv = (INonDelegatingUnknown FAR *) this;

    } else {

        *ppv = NULL;
        return E_NOINTERFACE;
    }


    //
    // Delegating AddRef to aggregator for IADsExtesnion and IADsUser.
    // AddRef on itself for IPrivateUnknown.   (both tested.)
    //

    ((IUnknown *) (*ppv)) -> AddRef();

    return S_OK;
}


STDMETHODIMP
CLDAPUser::ADSIInitializeDispatchManager(
    long dwExtensionId
    )
{
    HRESULT hr = S_OK;

    if (_fDispInitialized) {

        RRETURN(E_FAIL);
    }


    hr = _pDispMgr->InitializeDispMgr(dwExtensionId);


    if (SUCCEEDED(hr)) {

        _fDispInitialized = TRUE;
    }

    RRETURN(hr);
}

STDMETHODIMP
CLDAPUser::ADSIReleaseObject()
{
    delete this;

    RRETURN(S_OK);
}


//
// IADsExtension::Operate()
//

STDMETHODIMP
CLDAPUser::Operate(
    THIS_ DWORD dwCode,
    VARIANT varData1,
    VARIANT varData2,
    VARIANT varData3
    )
{
    HRESULT hr = S_OK;

    switch (dwCode) {

    case ADS_EXT_INITCREDENTIALS:

        hr = InitCredentials(
                &varData1,
                &varData2,
                &varData3
                );
        break;

    default:

        hr = E_FAIL;
        break;
    }

    RRETURN(hr);
}


HRESULT
CLDAPUser::InitCredentials(
    VARIANT * pvarUserName,
    VARIANT * pvarPassword,
    VARIANT * pvarFlags
    )
{

        BSTR bstrUser = NULL;
        BSTR bstrPwd = NULL;
        DWORD dwFlags = 0;

        ASSERT(V_VT(pvarUserName) == VT_BSTR);
        ASSERT(V_VT(pvarPassword) == VT_BSTR);
        ASSERT(V_VT(pvarFlags) == VT_I4);

        bstrUser = V_BSTR(pvarUserName);
        bstrPwd = V_BSTR(pvarPassword);
        dwFlags = V_I4(pvarFlags);

        CCredentials NewCredentials(bstrUser, bstrPwd, dwFlags);
        _Credentials = NewCredentials;


       RRETURN(S_OK);
}

