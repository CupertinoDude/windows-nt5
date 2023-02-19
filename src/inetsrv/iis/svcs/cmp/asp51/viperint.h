/*===================================================================
Microsoft Denali

Microsoft Confidential.
Copyright 1997 Microsoft Corporation. All Rights Reserved.

Component: Viper Integration Objects

File: viperint.h

Owner: DmitryR

This file contains the definiton of viper integration classes
===================================================================*/

#ifndef VIPERINT_H
#define VIPERINT_H

#include "mtx.h"
#include "mtxpriv.h"
#include "package.h"

#include "asptlb.h"     // needed to define interface pointers

#include "memcls.h"

class CHitObj;  // forward decl

/*===================================================================
  Transaction Support Types
===================================================================*/
#define TransType       DWORD

#define ttUndefined     0x00000000
#define ttNotSupported  0x00000001
#define ttSupported     0x00000002
#define ttRequired      0x00000004
#define ttRequiresNew   0x00000008

/*===================================================================
CViperAsyncRequest class implements IMTSCall interface.
Its OnCall() method does HitObj processing.
This is a private class used in CViperActivity class
===================================================================*/

class CViperAsyncRequest : public IMTSCall
	{
private:
	DWORD          m_cRefs;	          // reference count
	CHitObj       *m_pHitObj;         // request
	
private:
	CViperAsyncRequest();
	~CViperAsyncRequest();

	HRESULT Init(CHitObj *pHitObj);

public:
#ifdef DBG
	virtual void AssertValid() const;
#else
	virtual void AssertValid() const {}
#endif

public:
	// IUnknown Methods
	STDMETHODIMP		 QueryInterface(REFIID iid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IMTSCall Method
	STDMETHODIMP OnCall();

friend class CViperActivity;

	// Cache on per-class basis
    ACACHE_INCLASS_DEFINITIONS()
	};

extern volatile LONG g_nViperRequests;

#ifdef UNUSED
/*===================================================================
CViperThreadEvents class implements IThreadEvents interface.
It allows us to do work on each thread that Viper starts up in their
threadpool.
===================================================================*/

class CViperThreadEvents : public IThreadEvents
	{
private:
	DWORD          m_cRefs;	          // reference count
	
public:
	CViperThreadEvents();
	~CViperThreadEvents() {};

public:
	// IUnknown Methods
	STDMETHODIMP		 QueryInterface(REFIID iid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IThreadEvents Method
	STDMETHODIMP OnStartup();
	STDMETHODIMP OnShutdown();

private:
#ifdef DBG
	virtual void AssertValid() const;
#else
	virtual void AssertValid() const {}
#endif


	};
#endif //UNUSED

/*===================================================================
CViperActivity corresponds to a Session.
It creates MTS activity, and launches async requests
===================================================================*/

class CViperActivity
    {
private:
    IMTSActivity *m_pActivity;
    DWORD m_cBind;    // inited-flag + bind-to-thread count

    inline BOOL FInited() const { return (m_cBind > 0); }

public:
    CViperActivity();
    ~CViperActivity();

    // Create Viper activity
    HRESULT Init(BOOL  fCreateInMTA = FALSE);
    
    // Clone Viper activity
    HRESULT InitClone(CViperActivity *pActivity);

    // Bind/Unbind
    HRESULT BindToThread();
    HRESULT UnBindFromThread();

    // Release Viper activity
    HRESULT UnInit(); 

    // Check if thread-bound
    inline BOOL FThreadBound() const { return (m_cBind > 1); }

    // Post async request within this activity
    HRESULT PostAsyncRequest(CHitObj *pHitObj);

    // post async request without an activity
    static HRESULT PostGlobalAsyncRequest(CHitObj *pHitObj);

public:
#ifdef DBG
	virtual void AssertValid() const;
#else
	virtual void AssertValid() const {}
#endif

	// Cache on per-class basis
    ACACHE_INCLASS_DEFINITIONS()
    };

/*===================================================================
Misc. Functions
===================================================================*/

HRESULT ViperAttachIntrinsicsToContext
    (
    IApplicationObject *pAppln,
    ISessionObject     *pSession  = NULL,
    IRequest           *pRequest  = NULL,
    IResponse          *pResponse = NULL,
    IServer            *pServer   = NULL
    );

HRESULT ViperGetObjectFromContext
    (
    BSTR bstrName,
    IDispatch **ppdisp
    );

HRESULT ViperGetHitObjFromContext
    (
    CHitObj **ppHitObj
    );

HRESULT ViperCreateInstance
    (
    REFCLSID rclsid,
    REFIID   riid,
    void   **ppv
    );

HRESULT ViperConfigure(DWORD cThreads, BOOL fAllowOopComponents);

#ifdef UNUSED
HRESULT SetViperThreadEvents();
#endif //UNUSED

/*===================================================================
COM Helper API
===================================================================*/

BOOL ViperCoObjectIsaProxy
    (
    IUnknown *pUnk
    );

BOOL ViperCoObjectAggregatesFTM
    (
    IUnknown *pUnk
    );

#endif // VIPERINT
