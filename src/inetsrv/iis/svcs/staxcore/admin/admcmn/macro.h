
//
//              This NonDelegating interface is completely for 3rd-party 
//              extension objects based on our implementation only. NO need 
//              to export to ADISclients or 3rd part writer or typelib.  
//              Will put it here for now. Move -> ?? 
//

#ifndef _INonDelegatingUnknown
#define _INonDelegatingUnknown
interface INonDelegatingUnknown
{
    virtual HRESULT STDMETHODCALLTYPE
    NonDelegatingQueryInterface(const IID&, void **) = 0; 

    virtual ULONG STDMETHODCALLTYPE 
    NonDelegatingAddRef() = 0;

    virtual ULONG STDMETHODCALLTYPE 
    NonDelegatingRelease() = 0;
};
#endif


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
}

#define DEFINE_IADsPutGet_UnImplementation(cls)                          \
STDMETHODIMP                                                             \
cls::Get(THIS_ BSTR bstrName, VARIANT FAR* pvProp)                       \
{                                                                        \
    RRETURN(E_NOTIMPL);                                                  \
}                                                                        \
                                                                         \
STDMETHODIMP                                                             \
cls::Put(THIS_ BSTR bstrName, VARIANT vProp)                             \
{                                                                        \
    RRETURN(E_NOTIMPL);                                                  \
}                                                                        \
                                                                         \
STDMETHODIMP                                                             \
cls::GetEx(THIS_ BSTR bstrName, VARIANT FAR* pvProp)                     \
{                                                                        \
    RRETURN(E_NOTIMPL);                                                  \
}                                                                        \
                                                                         \
STDMETHODIMP                                                             \
cls::PutEx(THIS_ long lnControlCode, BSTR bstrName, VARIANT vProp)       \
{                                                                        \
    RRETURN(E_NOTIMPL);                                                  \
}


#define DEFINE_IPrivateDispatch_Implementation(cls)                   \
STDMETHODIMP                                                          \
cls::ADSIGetTypeInfoCount(unsigned int FAR* pctinfo)                      \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfoCount(pctinfo));                \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::ADSIGetTypeInfo(unsigned int itinfo, LCID lcid,                      \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
        RRETURN(_pDispMgr->GetTypeInfo(itinfo,                        \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
}                                                                     \
STDMETHODIMP                                                          \
cls::ADSIGetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,                 \
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
cls::ADSIInvoke(DISPID dispidMember, REFIID iid, LCID lcid,               \
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


#define DEFINE_IADsExtension_Implementation(cls)                        \
STDMETHODIMP                                                            \
cls::PrivateGetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,            \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)           \
{                                                                       \
        RRETURN(_pDispMgr->GetIDsOfNames(iid,                           \
                                         rgszNames,                     \
                                         cNames,                        \
                                         lcid,                          \
                                         rgdispid                       \
                                         ));                            \
}                                                                       \
                                                                        \
STDMETHODIMP                                                            \
cls::PrivateInvoke(DISPID dispidMember, REFIID iid, LCID lcid,          \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,             \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,             \
        unsigned int FAR* puArgErr)                                     \
{                                                                       \
        RRETURN (_pDispMgr->Invoke(dispidMember,                        \
                                   iid,                                 \
                                   lcid,                                \
                                   wFlags,                              \
                                   pdispparams,                         \
                                   pvarResult,                          \
                                   pexcepinfo,                          \
                                   puArgErr                             \
                                   ));                                  \
}


#define DEFINE_DELEGATING_IDispatch_Implementation(cls)                          \
STDMETHODIMP                                                          \
cls::GetTypeInfoCount(unsigned int FAR* pctinfo)                      \
{                                                                     \
	if (_pADs == (LPVOID)this)                                        \
	{                                                                 \
        RRETURN(ADSIGetTypeInfoCount(pctinfo));                       \
	}                                                                 \
	else                                                              \
	{                                                                 \
        RRETURN(_pADs->GetTypeInfoCount(pctinfo));                    \
	}                                                                 \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::GetTypeInfo(unsigned int itinfo, LCID lcid,                      \
        ITypeInfo FAR* FAR* pptinfo)                                  \
{                                                                     \
	if (_pADs == (LPVOID)this)                                        \
	{                                                                 \
        RRETURN(ADSIGetTypeInfo(itinfo,                               \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
	}                                                                 \
	else                                                              \
	{                                                                 \
        RRETURN(_pADs->GetTypeInfo(itinfo,                            \
                                       lcid,                          \
                                       pptinfo                        \
                                       ));                            \
	}                                                                 \
}                                                                     \
STDMETHODIMP                                                          \
cls::GetIDsOfNames(REFIID iid, LPWSTR FAR* rgszNames,                 \
        unsigned int cNames, LCID lcid, DISPID FAR* rgdispid)         \
{                                                                     \
	if (_pADs == (LPVOID)this)                                        \
	{                                                                 \
        RRETURN(ADSIGetIDsOfNames(iid,                                \
                                         rgszNames,                   \
                                         cNames,                      \
                                         lcid,                        \
                                         rgdispid                     \
                                         ));                          \
	}                                                                 \
	else                                                              \
	{                                                                 \
        RRETURN(_pADs->GetIDsOfNames(iid,                             \
                                         rgszNames,                   \
                                         cNames,                      \
                                         lcid,                        \
                                         rgdispid                     \
                                         ));                          \
	}                                                                 \
}                                                                     \
                                                                      \
STDMETHODIMP                                                          \
cls::Invoke(DISPID dispidMember, REFIID iid, LCID lcid,               \
        unsigned short wFlags, DISPPARAMS FAR* pdispparams,           \
        VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,           \
        unsigned int FAR* puArgErr)                                   \
{                                                                     \
	if (_pADs == (LPVOID)this)                                        \
	{                                                                 \
        RRETURN (ADSIInvoke(dispidMember,                             \
                                   iid,                               \
                                   lcid,                              \
                                   wFlags,                            \
                                   pdispparams,                       \
                                   pvarResult,                        \
                                   pexcepinfo,                        \
                                   puArgErr                           \
                                   ));                                \
	}                                                                 \
	else                                                              \
	{                                                                 \
        RRETURN (_pADs->Invoke(dispidMember,                          \
                                   iid,                               \
                                   lcid,                              \
                                   wFlags,                            \
                                   pdispparams,                       \
                                   pvarResult,                        \
                                   pexcepinfo,                        \
                                   puArgErr                           \
                                   ));                                \
	}                                                                 \
}                                                                      
