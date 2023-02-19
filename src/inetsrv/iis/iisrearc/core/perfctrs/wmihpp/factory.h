/*++

Copyright (c) 1998-2000 Microsoft Corporation

Module Name:

    factory.h

Abstract:

    This is definition of the class factory class for
    the high performance provider for WMI

Author:

    Cezary Marcjan (cezarym)        23-Feb-2000

Revision History:

--*/


#ifndef _factory_h__
#define _factory_h__


class CPerfClassFactory
    : public IClassFactory
{

public:

    CPerfClassFactory();
    ~CPerfClassFactory();

    virtual
    HRESULT
    STDMETHODCALLTYPE
    QueryInterface(
        IN REFIID iid,
        OUT PVOID * ppObject
        );

    virtual
    ULONG
    STDMETHODCALLTYPE
    AddRef(
        );

    virtual
    ULONG
    STDMETHODCALLTYPE
    Release(
        );

    virtual
    HRESULT
    STDMETHODCALLTYPE
    CreateInstance(
        IN IUnknown * pControllingUnknown,
        IN REFIID iid,
        OUT PVOID * ppObject
        );

    virtual
    HRESULT
    STDMETHODCALLTYPE
    LockServer(
        IN BOOL Lock
        );


private:


    LONG m_RefCount;


};  // class CPerfClassFactory



#endif  // _factory_h__

