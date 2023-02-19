//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:       stemcf.hxx
//
//  Contents:   Stemmer 'class factory'.
//
//  History:    01-July-1996     PatHal   Created
//
//--------------------------------------------------------------------------

#if !defined( __STEMCF_HXX__ )
#define __STEMCF_HXX__

//+-------------------------------------------------------------------------
//
//  Class:      CStemmerCF
//
//  Purpose:    Class factory for FE Stemmers
//
//--------------------------------------------------------------------------

class CStemmerCF : public IClassFactory
{

public:

    CStemmerCF( LCID lcid );

    //
    // From IUnknown
    //

    virtual  SCODE STDMETHODCALLTYPE
        QueryInterface( REFIID riid, void  ** ppvObject);

    virtual  ULONG STDMETHODCALLTYPE
        AddRef();

    virtual  ULONG STDMETHODCALLTYPE
        Release();

    //
    // From IClassFactory
    //

    virtual  SCODE STDMETHODCALLTYPE
        CreateInstance( IUnknown * pUnkOuter, REFIID riid, void  * * ppvObject );

    virtual  SCODE STDMETHODCALLTYPE
        LockServer( BOOL fLock );

protected:

    virtual ~CStemmerCF();

    long _cRefs;
    LCID _lcid;
};


#endif // __STEMCF_HXX__

