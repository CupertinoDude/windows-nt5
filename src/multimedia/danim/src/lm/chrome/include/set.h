#pragma once
#ifndef __SETBVR_H_
#define __SETBVR_H_
//*****************************************************************************
//
// Microsoft Trident3D
// Copyright (C) Microsoft Corporation, 1998
//
// Filename:    setbvr.h
//
// Author:	jeffort
//
// Created:	10/07/98
//
// Abstract:    set behavior class definition
// Modifications:
// 10/07/98 jeffort created file
// 11/20/98 markhal	converted to work with actor
//
//*****************************************************************************

#include <resource.h>

#include "basebvr.h"

#define NUM_SET_PROPS 3

//*****************************************************************************

class ATL_NO_VTABLE CSetBvr : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSetBvr, &CLSID_CrSetBvr>,
    public IConnectionPointContainerImpl<CSetBvr>,
    public IPropertyNotifySinkCP<CSetBvr>,
    public IPersistPropertyBag2,
#ifdef CRSTANDALONE
	public IDispatchImpl<ICrSetBvr, &IID_ICrSetBvr, &LIBID_ChromeBehavior>,
    error me here
#else
	public IDispatchImpl<ICrSetBvr, &IID_ICrSetBvr, &LIBID_LiquidMotion>,
#endif // CRSTANDALONE
	public IElementBehavior,
    public CBaseBehavior
	
{

BEGIN_COM_MAP(CSetBvr)
	COM_INTERFACE_ENTRY(ICrSetBvr)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IElementBehavior)
	COM_INTERFACE_ENTRY(IPersistPropertyBag2)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// Connection Point to allow IPropertyNotifySink 
BEGIN_CONNECTION_POINT_MAP(CSetBvr)
    CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP();

public:
DECLARE_REGISTRY_RESOURCEID(IDR_SETBVR)

	CSetBvr();
    virtual ~CSetBvr();
    HRESULT FinalConstruct();
    // IElementBehavior
    //
	STDMETHOD(Init)(IElementBehaviorSite *pBehaviorSite);
	STDMETHOD(Notify)(LONG event, VARIANT *pVar);
	STDMETHOD(Detach)();

    // Needed by CBaseBehavior
    void * 	GetInstance() { return (ICrSetBvr *) this ; }
	
    HRESULT GetTypeInfo(ITypeInfo ** ppInfo)
    { return GetTI(GetUserDefaultLCID(), ppInfo); }

    // ICrSetBehavior
	STDMETHOD(put_value)(VARIANT varValue);
	STDMETHOD(get_value)(VARIANT *pRetValue);
	STDMETHOD(put_property)(VARIANT varProperty);
	STDMETHOD(get_property)(VARIANT *pRetProperty);
    STDMETHOD(put_type)(VARIANT varType);
    STDMETHOD(get_type)(VARIANT *pRetType);
    STDMETHOD(put_animates)(VARIANT varAnimates);
	STDMETHOD(get_animates)(VARIANT *pRetAnimates);
	STDMETHOD(buildBehaviorFragments)( IDispatch* pActorDisp );

	//IPersistPropertyBag2 methods
    STDMETHOD(GetClassID)(CLSID* pclsid);
	STDMETHOD(InitNew)(void);
    STDMETHOD(IsDirty)(void){return S_OK;};
    STDMETHOD(Load)(IPropertyBag2 *pPropBag,IErrorLog *pErrorLog);
    STDMETHOD(Save)(IPropertyBag2 *pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties);

protected:
    virtual HRESULT BuildAnimationAsDABehavior();
    virtual VARIANT *VariantFromIndex(ULONG iIndex);
    virtual HRESULT GetPropertyBagInfo(ULONG *pulProperties, WCHAR ***pppPropName);
    virtual HRESULT GetConnectionPoint(REFIID riid, IConnectionPoint **ppICP);

private:

    HRESULT ExtractColor(VARIANT, IDAColor **);
    HRESULT ExtractNumber(VARIANT, IDANumber **);

    static WCHAR                *m_rgPropNames[NUM_SET_PROPS]; 
    VARIANT                     m_varValue;
    VARIANT                     m_varProperty;
    VARIANT                     m_varType;

    long                        m_lCookie;
    IDispatch                   *m_pdispActor;
}; // CSetBvr

//*****************************************************************************
//
// End of File
//
//*****************************************************************************
#endif //__SETBVR_H_ 
