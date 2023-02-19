//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1998 - 1999
//
//  File:       d3drmmeshinterobj.h
//
//--------------------------------------------------------------------------

// d3drmViewport2Obj.h : Declaration of the C_dxj_Direct3dRMMeshInterpolatorObject

#include "resource.h"       // main symbols
//#include "d3drmObjectObj.h"

#define typedef__dxj_Direct3dRMMeshInterpolator LPDIRECT3DRMINTERPOLATOR

/////////////////////////////////////////////////////////////////////////////
// Direct

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.
	  
class C_dxj_Direct3dRMMeshInterpolatorObject : 
	public I_dxj_Direct3dRMMeshInterpolator,
	public I_dxj_Direct3dRMInterpolator,
	//public I_dxj_Direct3dRMObject,
	public CComObjectRoot
{
public:
	C_dxj_Direct3dRMMeshInterpolatorObject() ;
	virtual ~C_dxj_Direct3dRMMeshInterpolatorObject() ;
	DWORD InternalAddRef();
	DWORD InternalRelease();

	BEGIN_COM_MAP(C_dxj_Direct3dRMMeshInterpolatorObject)
		COM_INTERFACE_ENTRY(I_dxj_Direct3dRMMeshInterpolator)
		COM_INTERFACE_ENTRY(I_dxj_Direct3dRMInterpolator)
		//COM_INTERFACE_ENTRY(I_dxj_Direct3dRMObject)
	END_COM_MAP()

 

	DECLARE_AGGREGATABLE(C_dxj_Direct3dRMMeshInterpolatorObject)

// I_dxj_Direct3dRMViewport2
public:

         /* [hidden] */ HRESULT STDMETHODCALLTYPE InternalSetObject( 
            /* [in] */ IUnknown __RPC_FAR *lpdd);
        
         /* [hidden] */ HRESULT STDMETHODCALLTYPE InternalGetObject( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *lpdd);
        
         HRESULT STDMETHODCALLTYPE attachObject( 
            /* [in] */ I_dxj_Direct3dRMObject __RPC_FAR *rmObject);
        
        
         HRESULT STDMETHODCALLTYPE detachObject( 
            /* [in] */ I_dxj_Direct3dRMObject __RPC_FAR *rmObject);
        
         HRESULT STDMETHODCALLTYPE getAttachedObjects( 
            /* [retval][out] */ I_dxj_Direct3dRMObjectArray __RPC_FAR *__RPC_FAR *rmArray);
        
         HRESULT STDMETHODCALLTYPE setIndex( 
            /* [in] */ float val);
        
         HRESULT STDMETHODCALLTYPE getIndex( 
            /* [retval][out] */ float __RPC_FAR *val);
        
         HRESULT STDMETHODCALLTYPE interpolate( 
            /* [in] */ float val,
            /* [in] */ I_dxj_Direct3dRMObject __RPC_FAR *rmobject,
            /* [in] */ long options);
        

         HRESULT STDMETHODCALLTYPE translate( 
            /* [in] */ float tx,
            /* [in] */ float ty,
            /* [in] */ float tz) ;
        
         HRESULT STDMETHODCALLTYPE setVertices( 
            /* [in] */ d3drmGroupIndex id,
            /* [in] */ long idx,
            /* [in] */ long count,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *vertexArray);
        
         HRESULT STDMETHODCALLTYPE setGroupColor( 
            /* [in] */ d3drmGroupIndex id,
            /* [in] */ d3dcolor val);
        
         HRESULT STDMETHODCALLTYPE setGroupColorRGB( 
            /* [in] */ d3drmGroupIndex id,
            /* [in] */ float r,
            /* [in] */ float g,
            /* [in] */ float b) ;

////////////////////////////////////////////////////////////////////////////////////
//
	// note: this is public for the callbacks



private:
    DECL_VARIABLE(_dxj_Direct3dRMMeshInterpolator);
	LPDIRECT3DRMMESH	m__dxj_Direct3dRMMesh;	


public:
	DX3J_GLOBAL_LINKS( _dxj_Direct3dRMMeshInterpolator )
};


