//---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1996 - 1997
//
//  File:  cmacro.h
//
//  Contents:  Macros for adsi IIS provider
//
//  History:   21-04-97     sophiac    Created.
//
//----------------------------------------------------------------------------
#define BAIL_ON_NULL(p)       \
        if (!(p)) {           \
                goto error;   \
        }

#define BAIL_IF_ERROR(hr) \
        if (FAILED(hr)) {       \
                goto cleanup;   \
        }\

#define BAIL_ON_FAILURE(hr) \
        if (FAILED(hr)) {       \
                goto error;   \
        }\

#define CONTINUE_ON_FAILURE(hr) \
        if (FAILED(hr)) {       \
                hr = NO_ERROR; \
                continue;   \
        }\

#define BAIL_ON_SUCCESS(hr) \
        if (SUCCEEDED(hr)) {       \
                goto error;   \
        }\

#define QUERY_INTERFACE(hr, ptr, iid, ppObj) \
        hr = ptr->QueryInterface(iid, (void **)ppObj); \
        if (FAILED(hr)) {    \
                goto cleanup;\
        }\


#define RRETURN_ENUM_STATUS(hr) \
        if (hr != S_OK) {       \
            RRETURN(S_FALSE);   \
        }                       \
        RRETURN(S_OK);




#define DEFINE_IDispatch_Implementation_Unimplemented(cls) \
STDMETHODIMP                                                          \
cls::GetTypeInfoCount(unsigned int FAR* pctinfo)        \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetTypeInfo(unsigned int itinfo, LCID lcid,        \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,   \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)         \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Invoke(DISPID dispidMember, REFIID iid, LCID lcid, \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,           \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,           \
        unsigned int FAR* puArgErr)                                   \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}




#define DEFINE_IDispatch_Implementation(cls)                          \
STDMETHODIMP                                                          \
cls::GetTypeInfoCount(unsigned int FAR* pctinfo)                      \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfoCount(pctinfo));                \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetTypeInfo(unsigned int itinfo, LCID lcid,                      \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfo(itinfo,                        \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,                 \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)         \
{                                                                     \
        DWORD MetaID;                                                 \
                                                                      \
        HRESULT hr = (_pDispMgr->GetIDsOfNames(iid,                   \
                                       rgszNames,                     \
                                       cNames,                        \
                                       lcid,                          \
                                       rgdispid                       \
                                       ));                            \
                                                                      \
                                                                      \
        RRETURN(hr);                                                  \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Invoke(DISPID dispidMember, REFIID iid, LCID lcid,               \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,           \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,           \
        unsigned int FAR* puArgErr)                                   \
{                                                                     \
        HRESULT hr;                                                   \
        WCHAR szPropName[64];                                         \
        BSTR bstrPropName = NULL;                                     \
                                                                      \
		RRETURN (_pDispMgr->Invoke(dispidMember,                     \
                                    iid,                              \
                                    lcid,                             \
                                    wFlags,                           \
                                    pdispparams,                      \
                                    pvarResult,                       \
                                    pexcepinfo,                       \
                                    puArgErr                          \
                                    ));                               \
}
 

#define DEFINE_Simple_IDispatch_Implementation(cls)                   \
STDMETHODIMP                                                          \
cls::GetTypeInfoCount(unsigned int FAR* pctinfo)                      \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfoCount(pctinfo));                \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetTypeInfo(unsigned int itinfo, LCID lcid,                      \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfo(itinfo,                        \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,                 \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)         \
{                                                                     \
        RRETURN(_pDispMgr->GetIDsOfNames(iid,                         \
                                         rgszNames,                   \
                                         cNames,                      \
                                         lcid,                        \
                                         rgdispid                     \
                                         ));                          \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Invoke(DISPID dispidMember, REFIID iid, LCID lcid,               \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,           \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,           \
        unsigned int FAR* puArgErr)                                   \
{                                                                     \
        RRETURN (_pDispMgr->Invoke(dispidMember,                      \
                                   iid,                               \
                                   lcid,                              \
                                   wFlags,                            \
                                   pdispparams,                       \
                                   pvarResult,                        \
                                   pexcepinfo,                        \
                                   puArgErr                           \
                                   ));                                \
}



#define DEFINE_IADs_Implementation(cls)                             \
STDMETHODIMP                                                          \
cls::get_Name(THIS_ BSTR FAR* retval)                                 \
{                                                                     \
    RRETURN(get_CoreName(retval));                                    \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::get_ADsPath(THIS_ BSTR FAR* retval)                            \
{                                                                     \
                                                                      \
    RRETURN(get_CoreADsPath(retval));                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::get_Class(THIS_ BSTR FAR* retval)                                \
{                                                                     \
                                                                      \
    RRETURN(get_CoreADsClass(retval));                              \
}                                                                     \
                                                                      \
                                                                      \
STDMETHODIMP                                                          \
cls::get_Parent(THIS_ BSTR FAR* retval)                               \
{                                                                     \
    RRETURN(get_CoreParent(retval));                                  \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::get_Schema(THIS_ BSTR FAR* retval)                               \
{                                                                     \
    RRETURN(get_CoreSchema(retval));                                  \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::get_GUID(THIS_ BSTR FAR* retval)                                 \
{                                                                     \
    RRETURN(get_CoreGUID(retval));                                    \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetInfoEx(THIS_ VARIANT vProperties, long lnReserved)            \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}


#define CHECK_AND_SET_EXTENDED_ERROR(dwStatus, hr)                    \
    if (dwStatus) {                                                   \
        hr = CheckAndSetExtendedError(dwStatus);                      \
        BAIL_ON_FAILURE(hr);                                          \
    }



#define DEFINE_IADs_PutGetUnImplementation(cls)                       \
STDMETHODIMP                                                          \
cls::Get(THIS_ BSTR bstrName, VARIANT FAR* pvProp)                    \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Put(THIS_ BSTR bstrName, VARIANT vProp)                          \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetEx(THIS_ BSTR bstrName, VARIANT FAR* pvProp)                  \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::PutEx(THIS_ long lnControlCode, BSTR bstrName, VARIANT vProp)    \
{                                                                     \
    RRETURN(E_NOTIMPL);                                               \
}


#define DEFINE_IDispatch_ExtMgr_Implementation(cls)                   \
STDMETHODIMP                                                          \
cls::GetTypeInfoCount(unsigned int FAR* pctinfo)                      \
{                                                                     \
        RRETURN(_pExtMgr->GetTypeInfoCount(pctinfo));                 \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetTypeInfo(unsigned int itinfo, LCID lcid,                      \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
        RRETURN(_pExtMgr->GetTypeInfo(itinfo,                         \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,                 \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)         \
{                                                                     \
        RRETURN(_pExtMgr->GetIDsOfNames(iid,                          \
                                         rgszNames,                   \
                                         cNames,                      \
                                         lcid,                        \
                                         rgdispid                     \
                                         ));                          \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Invoke(DISPID dispidMember, REFIID iid, LCID lcid,               \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,           \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,           \
        unsigned int FAR* puArgErr)                                   \
{                                                                     \
        RRETURN (_pExtMgr->Invoke(dispidMember,                       \
                                   iid,                               \
                                   lcid,                              \
                                   wFlags,                            \
                                   pdispparams,                       \
                                   pvarResult,                        \
                                   pexcepinfo,                        \
                                   puArgErr                           \
                                   ));                                \
}
