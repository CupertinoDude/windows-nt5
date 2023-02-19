#include "resource.h"       // main symbols

#define typedef__dxj_DirectSoundFXEcho LPDIRECTSOUNDFXECHO8

/////////////////////////////////////////////////////////////////////////////
// Direct

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

class C_dxj_DirectSoundFXEchoObject : 

#ifdef USING_IDISPATCH
	public CComDualImpl<I_dxj_DirectSoundFXEcho, &IID_I_dxj_DirectSoundFXEcho, &LIBID_DIRECTLib>, 
	public ISupportErrorInfo,
#else
	public I_dxj_DirectSoundFXEcho,
#endif

	public CComObjectRoot
{
public:
	C_dxj_DirectSoundFXEchoObject() ;
	virtual ~C_dxj_DirectSoundFXEchoObject() ;

BEGIN_COM_MAP(C_dxj_DirectSoundFXEchoObject)
	COM_INTERFACE_ENTRY(I_dxj_DirectSoundFXEcho)
#ifdef USING_IDISPATCH
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
#endif
END_COM_MAP()

DECLARE_AGGREGATABLE(C_dxj_DirectSoundFXEchoObject)

#ifdef USING_IDISPATCH
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
#endif

// I_dxj_DirectSoundFXEcho
public:
		 /* [hidden] */ HRESULT STDMETHODCALLTYPE InternalSetObject( 
            /* [in] */ IUnknown __RPC_FAR *lpdd);
        
         /* [hidden] */ HRESULT STDMETHODCALLTYPE InternalGetObject( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *lpdd);

		HRESULT STDMETHODCALLTYPE SetAllParameters(DSFXECHO_CDESC *params);
		HRESULT STDMETHODCALLTYPE GetAllParameters(DSFXECHO_CDESC *params);

////////////////////////////////////////////////////////////////////////
//
	// note: this is public for the callbacks
    DECL_VARIABLE(_dxj_DirectSoundFXEcho);

private:

public:

	DX3J_GLOBAL_LINKS(_dxj_DirectSoundFXEcho);

	DWORD InternalAddRef();
	DWORD InternalRelease();
};




