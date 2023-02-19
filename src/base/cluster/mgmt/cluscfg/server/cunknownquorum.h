//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2001 Microsoft Corporation
//
//  Module Name:
//      CUnknownQuorum.h
//
//  Description:
//      This file contains the declaration of the CUnknownQuorum
//      class.
//
//      The class CUnknownQuorum represents a cluster quorum
//      device. It implements the IClusCfgManagaedResourceInfo interface.
//
//  Documentation:
//
//  Implementation Files:
//      CUnknownQuorum.cpp
//
//  Maintained By:
//      Galen Barbee (GalenB) 18-DEC-2000
//
//////////////////////////////////////////////////////////////////////////////


// Make sure that this file is included only once per compile path.
#pragma once


//////////////////////////////////////////////////////////////////////////////
// Include Files
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Constant Declarations
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//++
//
//  class CUnknownQuorum
//
//  Description:
//      The class CUnknownQuorum represents a cluster quorum
//      device.
//
//  Interfaces:
//      IClusCfgManagedResourceInfo
//      IClusCfgInitialize
//--
//////////////////////////////////////////////////////////////////////////////
class CUnknownQuorum
    : public IClusCfgManagedResourceInfo
    , public IClusCfgInitialize
    , public IClusCfgManagedResourceCfg
{
private:

    //
    // Private member functions and data
    //

    LONG                m_cRef;
    LCID                m_lcid;
    IClusCfgCallback *  m_picccCallback;
    BOOL                m_fIsQuorum:1;
    BOOL                m_fIsJoinable:1;
    BOOL                m_fIsManaged:1;
    BSTR                m_bstrName;

    // Private constructors and destructors
    CUnknownQuorum( void );
    ~CUnknownQuorum( void );

    // Private copy constructor to prevent copying.
    CUnknownQuorum( const CUnknownQuorum & nodeSrc );

    // Private assignment operator to prevent copying.
    const CUnknownQuorum & operator = ( const CUnknownQuorum & nodeSrc );

    HRESULT HrInit( LPCWSTR pcszNameIn, BOOL fMakeQuorumIn = FALSE );

public:

    //
    // Public, non interface methods.
    //

    static HRESULT S_HrCreateInstance( IUnknown ** ppunkOut );

    static HRESULT S_HrCreateInstance( LPCWSTR pcszNameIn, BOOL fMakeQuorumIn, IUnknown ** ppunkOut );

    //
    // IUnknown Interface
    //

    STDMETHOD( QueryInterface )( REFIID riid, void ** ppvObject );

    STDMETHOD_( ULONG, AddRef )( void );

    STDMETHOD_( ULONG, Release )( void );

    //
    // IClusCfgInitialize Interfaces
    //

    // Register callbacks, locale id, etc.
    STDMETHOD( Initialize )( IUnknown * punkCallbackIn, LCID lcidIn );

    //
    // IClusCfgManagedResourceInfo Interface
    //

    STDMETHOD( GetUID )( BSTR * pbstrUIDOut );

    STDMETHOD( GetName )( BSTR * pbstrNameOut );

    STDMETHOD( SetName )( LPCWSTR pcszNameIn );

    STDMETHOD( IsManaged )( void );

    STDMETHOD( SetManaged )( BOOL fIsManagedIn );

    STDMETHOD( IsQuorumDevice )( void );

    STDMETHOD( SetQuorumedDevice )( BOOL fIsQuorumDeviceIn );

    STDMETHOD( IsQuorumCapable )( void );

    STDMETHOD( GetDriveLetterMappings )( SDriveLetterMapping * pdlmDriveLetterMappingOut );

    STDMETHOD( SetDriveLetterMappings )( SDriveLetterMapping dlmDriveLetterMappings );

    STDMETHOD( IsDeviceJoinable )( void );

    STDMETHOD( SetDeviceJoinable )( BOOL fIsJoinableIn );

    //
    //  IClusCfgManagedResourceCfg
    //

    STDMETHOD( PreCreate )( IUnknown * punkServicesIn );

    STDMETHOD( Create )( IUnknown * punkServicesIn );

    STDMETHOD( PostCreate )( IUnknown * punkServicesIn );

    STDMETHOD( Evict )( IUnknown * punkServicesIn );

}; //*** Class CUnknownQuorum
