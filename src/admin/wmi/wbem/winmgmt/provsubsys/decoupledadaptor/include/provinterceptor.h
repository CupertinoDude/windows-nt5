/*++

Copyright (C) 1996-2001 Microsoft Corporation

Module Name:

	ProvWsv.H

Abstract:


History:

--*/

#ifndef _Decopled_Interceptor_IWbemServices_H
#define _Decopled_Interceptor_IWbemServices_H

#include <Allocator.h>
#include <Algorithms.h>
#include <PQueue.h>
#include <TPQueue.h>
#include <BasicTree.h>
#include <Cache.h>

#include <CGlobals.h>
#include <ProvRegInfo.h>
#include <ProvObjectSink.h>

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

typedef WmiContainerController <void *>										CWbemGlobal_VoidPointerController ;
typedef CWbemGlobal_VoidPointerController :: Container						CWbemGlobal_VoidPointerController_Container ;
typedef CWbemGlobal_VoidPointerController :: Container_Iterator				CWbemGlobal_VoidPointerController_Container_Iterator ;
typedef CWbemGlobal_VoidPointerController :: WmiContainerElement			VoidPointerContainerElement ;

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

class CInterceptor_IWbemObjectSink ;

#define CWbemGlobal_IWmiObjectSinkController						CWbemGlobal_VoidPointerController
#define CWbemGlobal_IWmiObjectSinkController_Container				CWbemGlobal_VoidPointerController_Container
#define CWbemGlobal_IWmiObjectSinkController_Container_Iterator		CWbemGlobal_VoidPointerController_Container_Iterator
#define ObjectSinkContainerElement									VoidPointerContainerElement

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

#define ProxyIndex_IWbemServices					0
#define ProxyIndex_IWbemPropertyProvider			1
#define ProxyIndex_IWbemEventProvider				2
#define ProxyIndex_IWbemEventProviderQuerySink		3
#define ProxyIndex_IWbemEventProviderSecurity		4
#define ProxyIndex_IWbemProviderIdentity			5
#define ProxyIndex_IWbemEventConsumerProvider		6
#define ProxyIndex_IWbemEventConsumerProviderEx		7
#define ProxyIndex_IWbemUnboundObjectSink			8

#define ProxyIndex_Internal_IWbemServices					9
#define ProxyIndex_Internal_IWbemPropertyProvider			10
#define ProxyIndex_Internal_IWbemEventProvider				11
#define ProxyIndex_Internal_IWbemEventProviderQuerySink		12
#define ProxyIndex_Internal_IWbemEventProviderSecurity		13
#define ProxyIndex_Internal_IWbemEventConsumerProvider		14
#define ProxyIndex_Internal_IWbemEventConsumerProviderEx	15
#define ProxyIndex_Internal_IWbemUnboundObjectSink			16

#define ProxyIndex_IWbemShutdown					17

#define ProxyIndex_Provider_Size					18

#define ProxyIndex_UnBoundSync_IWbemUnboundObjectSink			0
#define ProxyIndex_UnBoundSync_Size								1

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

class CDecoupled_IWbemSyncObjectSink :	public CCommon_IWbemSyncObjectSink
{
private:
protected:
public:

	CDecoupled_IWbemSyncObjectSink (

		WmiAllocator &a_Allocator ,
		IWbemObjectSink *a_InterceptedSink ,
		IUnknown *a_Unknown ,
		CWbemGlobal_IWmiObjectSinkController *a_Controller ,
		ULONG a_Dependant = FALSE
	) ;

	~CDecoupled_IWbemSyncObjectSink () ;
} ;

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

class CDecoupled_Batching_IWbemSyncObjectSink :	public CCommon_Batching_IWbemSyncObjectSink
{
private:
protected:
public:

	CDecoupled_Batching_IWbemSyncObjectSink (

		WmiAllocator &a_Allocator ,
		IWbemObjectSink *a_InterceptedSink ,
		IUnknown *a_Unknown ,
		CWbemGlobal_IWmiObjectSinkController *a_Controller ,
		ULONG a_Dependant = FALSE
	) ;

	~CDecoupled_Batching_IWbemSyncObjectSink () ;
} ;

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

class CInterceptor_IWbemDecoupledUnboundObjectSink : public IWbemUnboundObjectSink ,
												public Internal_IWbemUnboundObjectSink ,

												public IWbemShutdown , 
												public VoidPointerContainerElement
{
private:

	WmiAllocator &m_Allocator ;
	CServerObject_ProviderRegistrationV1 *m_Registration ;

	IUnknown *m_Unknown ;
	IWbemUnboundObjectSink *m_Provider_IWbemUnboundObjectSink ;

	ProxyContainer m_ProxyContainer ;

protected:
public:

	CInterceptor_IWbemDecoupledUnboundObjectSink (

		WmiAllocator &a_Allocator ,
		IUnknown *a_ServerSideProvider , 
		CWbemGlobal_IWmiObjectSinkController *a_Controller ,
		CServerObject_ProviderRegistrationV1 &a_Registration
	) ;

	~CInterceptor_IWbemDecoupledUnboundObjectSink () ;

	HRESULT Initialize () ;

public:

	STDMETHODIMP QueryInterface ( 

		REFIID iid , 
		LPVOID FAR *iplpv 
	) ;

	STDMETHODIMP_( ULONG ) AddRef () ;

	STDMETHODIMP_( ULONG ) Release () ;

	HRESULT STDMETHODCALLTYPE Internal_IndicateToConsumer (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_LogicalConsumer ,
		long a_ObjectCount ,
		IWbemClassObject **a_Objects
	) ;

	HRESULT STDMETHODCALLTYPE IndicateToConsumer (

		IWbemClassObject *a_LogicalConsumer ,
		long a_ObjectCount ,
		IWbemClassObject **a_Objects
	) ;

	/* _IWmiProviderSite */

	HRESULT STDMETHODCALLTYPE GetSite ( DWORD *a_ProcessIdentifier ) ;

	HRESULT STDMETHODCALLTYPE SetContainer ( IUnknown *a_Container ) ;

	HRESULT STDMETHODCALLTYPE Shutdown (

		LONG a_Flags ,
		ULONG a_MaxMilliSeconds ,
		IWbemContext *a_Context
	) ; 
} ;

/******************************************************************************
 *
 *	Name:
 *
 *	
 *  Description:
 *
 *	
 *****************************************************************************/

class CInterceptor_DecoupledClient :	public IWbemServices , 
										public IWbemPropertyProvider ,
										public IWbemEventProvider ,
										public IWbemEventProviderQuerySink ,
										public IWbemEventProviderSecurity ,
										public IWbemProviderIdentity ,
										public IWbemEventConsumerProviderEx ,
										public IWbemUnboundObjectSink ,

										public IWbemProviderInit ,
										public Internal_IWbemProviderInit ,

										public Internal_IWbemServices , 
										public Internal_IWbemPropertyProvider ,
										public Internal_IWbemEventProvider ,
										public Internal_IWbemEventProviderQuerySink ,
										public Internal_IWbemEventProviderSecurity ,
										public Internal_IWbemEventConsumerProviderEx ,
										public Internal_IWbemUnboundObjectSink ,

										public _IWmiProviderSite ,
										public IWbemShutdown , 
										public CWbemGlobal_IWmiObjectSinkController 
{
private:

	LONG m_ReferenceCount ;

	IUnknown *m_Unknown ;
	IWbemServices *m_Provider_IWbemServices ;
	IWbemPropertyProvider *m_Provider_IWbemPropertyProvider ;
	IWbemEventProvider *m_Provider_IWbemEventProvider ;
	IWbemEventProviderQuerySink *m_Provider_IWbemEventProviderQuerySink ;
	IWbemEventProviderSecurity *m_Provider_IWbemEventProviderSecurity ;
	IWbemProviderIdentity *m_Provider_IWbemProviderIdentity ;
	IWbemEventConsumerProvider *m_Provider_IWbemEventConsumerProvider ;
	IWbemEventConsumerProviderEx *m_Provider_IWbemEventConsumerProviderEx ;
	IWbemUnboundObjectSink *m_Provider_IWbemUnboundObjectSink ;

	IWbemServices *m_CoreStub ;

	WmiAllocator &m_Allocator ;

	ProxyContainer m_ProxyContainer ;

	BSTR m_Locale ;
	BSTR m_User ;
	BSTR m_Namespace ;
	BSTR m_TransactionIdentifier ;

public:

	CServerObject_ProviderRegistrationV1 *m_Registration ;

	UINT64 m_ProviderOperation_GetObjectAsync ;
	UINT64 m_ProviderOperation_PutClassAsync ;
	UINT64 m_ProviderOperation_DeleteClassAsync ;
	UINT64 m_ProviderOperation_CreateClassEnumAsync ;
	UINT64 m_ProviderOperation_PutInstanceAsync ;
	UINT64 m_ProviderOperation_DeleteInstanceAsync ;
	UINT64 m_ProviderOperation_CreateInstanceEnumAsync ;
	UINT64 m_ProviderOperation_ExecQueryAsync ;
	UINT64 m_ProviderOperation_ExecNotificationQueryAsync ;
	UINT64 m_ProviderOperation_ExecMethodAsync ;

	UINT64 m_ProviderOperation_Begin ;
	UINT64 m_ProviderOperation_Rollback ;
	UINT64 m_ProviderOperation_Commit ;
	UINT64 m_ProviderOperation_QueryState ;

	UINT64 m_ProviderOperation_QueryInstances ;
	UINT64 m_ProviderOperation_CreateRefresher ;
	UINT64 m_ProviderOperation_CreateRefreshableObject ;
	UINT64 m_ProviderOperation_StopRefreshing ;
	UINT64 m_ProviderOperation_CreateRefreshableEnum ;
	UINT64 m_ProviderOperation_GetObjects ;

	UINT64 m_ProviderOperation_GetProperty ;
	UINT64 m_ProviderOperation_PutProperty ;

	UINT64 m_ProviderOperation_ProvideEvents ;
	UINT64 m_ProviderOperation_NewQuery ;
	UINT64 m_ProviderOperation_CancelQuery ;
	UINT64 m_ProviderOperation_AccessCheck ;
	UINT64 m_ProviderOperation_SetRegistrationObject ;
	UINT64 m_ProviderOperation_FindConsumer ;
	UINT64 m_ProviderOperation_ValidateSubscription ;

	void Increment_ProviderOperation_GetObjectAsync () { m_ProviderOperation_GetObjectAsync ++ ; }
 	void Increment_ProviderOperation_PutClassAsync () { m_ProviderOperation_PutClassAsync ++ ; }
	void Increment_ProviderOperation_DeleteClassAsync () { m_ProviderOperation_DeleteClassAsync ++ ; }
	void Increment_ProviderOperation_CreateClassEnumAsync () { m_ProviderOperation_CreateClassEnumAsync ++ ; }
	void Increment_ProviderOperation_PutInstanceAsync () { m_ProviderOperation_PutInstanceAsync ++ ; }
	void Increment_ProviderOperation_DeleteInstanceAsync () { m_ProviderOperation_DeleteInstanceAsync ++ ; }
	void Increment_ProviderOperation_CreateInstanceEnumAsync () { m_ProviderOperation_CreateInstanceEnumAsync ++ ; }
	void Increment_ProviderOperation_ExecQueryAsync () { m_ProviderOperation_ExecQueryAsync ++ ; }
	void Increment_ProviderOperation_ExecNotificationQueryAsync () { m_ProviderOperation_ExecNotificationQueryAsync ++ ; }
	void Increment_ProviderOperation_ExecMethodAsync () { m_ProviderOperation_ExecMethodAsync ++ ; }

	void Increment_ProviderOperation_Begin () { m_ProviderOperation_Begin ++ ; }
	void Increment_ProviderOperation_Rollback () { m_ProviderOperation_Rollback ++ ; }
	void Increment_ProviderOperation_Commit () { m_ProviderOperation_Commit ++ ; }
	void Increment_ProviderOperation_QueryState () { m_ProviderOperation_QueryState ++ ; }

	void Increment_ProviderOperation_QueryInstances () { m_ProviderOperation_QueryInstances ++ ; }
	void Increment_ProviderOperation_CreateRefresher () { m_ProviderOperation_CreateRefresher ++ ; }
	void Increment_ProviderOperation_CreateRefreshableObject () { m_ProviderOperation_CreateRefreshableObject ++ ; }
	void Increment_ProviderOperation_StopRefreshing () { m_ProviderOperation_StopRefreshing ++ ; }
	void Increment_ProviderOperation_CreateRefreshableEnum () { m_ProviderOperation_CreateRefreshableEnum ++ ; }
	void Increment_ProviderOperation_GetObjects () { m_ProviderOperation_GetObjects ++ ; }

	void Increment_ProviderOperation_GetProperty () { m_ProviderOperation_GetProperty ++ ; }
	void Increment_ProviderOperation_PutProperty () { m_ProviderOperation_PutProperty ++ ; }

	void Increment_ProviderOperation_ProvideEvents () { m_ProviderOperation_ProvideEvents ++ ; }
	void Increment_ProviderOperation_NewQuery () { m_ProviderOperation_NewQuery ++ ; }
	void Increment_ProviderOperation_CancelQuery () { m_ProviderOperation_CancelQuery ++ ; }
	void Increment_ProviderOperation_AccessCheck () { m_ProviderOperation_AccessCheck ++ ; }
	void Increment_ProviderOperation_SetRegistrationObject () { m_ProviderOperation_SetRegistrationObject ++ ; }
	void Increment_ProviderOperation_FindConsumer () { m_ProviderOperation_FindConsumer ++ ; }
	void Increment_ProviderOperation_ValidateSubscription () { m_ProviderOperation_ValidateSubscription ++ ; }

	UINT64 Get_ProviderOperation_GetObjectAsync () { return m_ProviderOperation_GetObjectAsync ; }
	UINT64 Get_ProviderOperation_PutClassAsync () { return m_ProviderOperation_PutClassAsync ; }
	UINT64 Get_ProviderOperation_DeleteClassAsync () { return m_ProviderOperation_DeleteClassAsync ; }
	UINT64 Get_ProviderOperation_CreateClassEnumAsync () { return m_ProviderOperation_CreateClassEnumAsync ; }
	UINT64 Get_ProviderOperation_PutInstanceAsync () { return m_ProviderOperation_PutInstanceAsync ; }
	UINT64 Get_ProviderOperation_DeleteInstanceAsync () { return m_ProviderOperation_DeleteInstanceAsync ; }
	UINT64 Get_ProviderOperation_CreateInstanceEnumAsync () { return m_ProviderOperation_CreateInstanceEnumAsync ; }
	UINT64 Get_ProviderOperation_ExecQueryAsync () { return m_ProviderOperation_ExecQueryAsync ; }
	UINT64 Get_ProviderOperation_ExecNotificationQueryAsync () { return m_ProviderOperation_ExecNotificationQueryAsync ; }
	UINT64 Get_ProviderOperation_ExecMethodAsync () { return m_ProviderOperation_ExecMethodAsync ; }

	UINT64 Get_ProviderOperation_Begin () { return m_ProviderOperation_Begin ; }
	UINT64 Get_ProviderOperation_Rollback () { return m_ProviderOperation_Rollback ; }
	UINT64 Get_ProviderOperation_Commit () { return m_ProviderOperation_Commit ; }
	UINT64 Get_ProviderOperation_QueryState () { return m_ProviderOperation_QueryState ; }

	UINT64 Get_ProviderOperation_QueryInstances () { return m_ProviderOperation_QueryInstances ; }
	UINT64 Get_ProviderOperation_CreateRefresher () { return m_ProviderOperation_CreateRefresher ; }
	UINT64 Get_ProviderOperation_CreateRefreshableObject () { return m_ProviderOperation_CreateRefreshableObject ; }
	UINT64 Get_ProviderOperation_StopRefreshing () { return m_ProviderOperation_StopRefreshing ; }
	UINT64 Get_ProviderOperation_CreateRefreshableEnum () { return m_ProviderOperation_CreateRefreshableEnum ; }
	UINT64 Get_ProviderOperation_GetObjects () { return m_ProviderOperation_GetObjects ; }

	UINT64 Get_ProviderOperation_GetProperty () { return m_ProviderOperation_GetProperty ; }
	UINT64 Get_ProviderOperation_PutProperty () { return m_ProviderOperation_PutProperty ; }

	UINT64 Get_ProviderOperation_ProvideEvents () { return m_ProviderOperation_ProvideEvents ; }
	UINT64 Get_ProviderOperation_NewQuery () { return m_ProviderOperation_NewQuery ; }
	UINT64 Get_ProviderOperation_CancelQuery () { return m_ProviderOperation_CancelQuery ; }
	UINT64 Get_ProviderOperation_AccessCheck () { return m_ProviderOperation_AccessCheck ; }
	UINT64 Get_ProviderOperation_SetRegistrationObject () { return m_ProviderOperation_SetRegistrationObject ; }
	UINT64 Get_ProviderOperation_FindConsumer () { return m_ProviderOperation_FindConsumer ; }
	UINT64 Get_ProviderOperation_ValidateSubscription () { return m_ProviderOperation_ValidateSubscription ; }

private:

	HRESULT SetStatus ( 

		LPWSTR a_Operation ,
		LPWSTR a_Parameters ,
		LPWSTR a_Description ,
		HRESULT a_Result ,
		IWbemObjectSink *a_Sink
	) ;

	HRESULT Begin_IWbemServices (

		BOOL &a_Impersonating ,
		IUnknown *&a_OldContext ,
		IServerSecurity *&a_OldSecurity ,
		BOOL &a_IsProxy ,
		IWbemServices *&a_Interface ,
		BOOL &a_Revert ,
		IUnknown *&a_Proxy
	) ;

	HRESULT End_IWbemServices (

		BOOL a_Impersonating ,
		IUnknown *a_OldContext ,
		IServerSecurity *a_OldSecurity ,
		BOOL a_IsProxy ,
		IWbemServices *a_Interface ,
		BOOL a_Revert ,
		IUnknown *a_Proxy
	) ;

	HRESULT AdjustGetContext (

		IWbemContext *a_Context
	) ;

	HRESULT Helper_GetObjectAsync (

		BOOL a_IsProxy ,
 		const BSTR a_ObjectPath ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_PutClassAsync (

		BOOL a_IsProxy ,
		IWbemClassObject *a_Object , 
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_DeleteClassAsync (

		BOOL a_IsProxy ,
		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_CreateClassEnumAsync (

		BOOL a_IsProxy ,
		const BSTR a_SuperClass ,
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_PutInstanceAsync (

		BOOL a_IsProxy ,
		IWbemClassObject *a_Instance ,
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_DeleteInstanceAsync (

		BOOL a_IsProxy ,
		const BSTR a_ObjectPath ,
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

    HRESULT Helper_CreateInstanceEnumAsync (

		BOOL a_IsProxy ,
		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service
	) ;

	HRESULT Helper_ExecQueryAsync (

		BOOL a_IsProxy ,
		const BSTR a_QueryLanguage ,
		const BSTR a_Query, 
		long a_Flags ,
		IWbemContext FAR *a_Context ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

	HRESULT Helper_ExecMethodAsync (

		BOOL a_IsProxy ,
		const BSTR a_ObjectPath ,
		const BSTR a_MethodName ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemClassObject *a_InParams ,
		IWbemObjectSink *a_Sink ,
		IWbemServices *a_Service 
	) ;

public:

	CInterceptor_DecoupledClient ( 

		WmiAllocator &a_Allocator ,
		IUnknown *a_ServerSideUnknown , 
		IWbemServices *a_CoreStub ,
		CServerObject_ProviderRegistrationV1 &a_Registration
	) ;

    ~CInterceptor_DecoupledClient () ;

	HRESULT ProviderInitialize () ;

public:

	//Non-delegating object IUnknown

    STDMETHODIMP QueryInterface ( REFIID , LPVOID FAR * ) ;
    STDMETHODIMP_( ULONG ) AddRef () ;
    STDMETHODIMP_( ULONG ) Release () ;

    /* IWbemServices methods */

    HRESULT STDMETHODCALLTYPE OpenNamespace ( 

        const BSTR a_Namespace ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemServices **a_Service ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE CancelAsyncCall ( 

        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE QueryObjectSink ( 

        long a_Flags ,
        IWbemObjectSink **a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE GetObject ( 

		const BSTR a_ObjectPath ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemClassObject **ppObject ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE GetObjectAsync (
        
		const BSTR a_ObjectPath ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;

    HRESULT STDMETHODCALLTYPE PutClass ( 

        IWbemClassObject *a_Object ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE PutClassAsync ( 

        IWbemClassObject *a_Object ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE DeleteClass ( 

        const BSTR a_Class ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE DeleteClassAsync ( 

        const BSTR a_Class ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE CreateClassEnum ( 

        const BSTR a_Superclass ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE CreateClassEnumAsync ( 

		const BSTR a_Superclass ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE PutInstance (

		IWbemClassObject *a_Instance ,
		long a_Flags , 
		IWbemContext *a_Context ,
		IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE PutInstanceAsync (

		IWbemClassObject *a_Instance ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink 
	) ;
    
    HRESULT STDMETHODCALLTYPE DeleteInstance ( 

		const BSTR a_ObjectPath ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE DeleteInstanceAsync ( 

		const BSTR a_ObjectPath,
		long a_Flags,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE CreateInstanceEnum (

		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE CreateInstanceEnumAsync (

		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecQuery ( 

		const BSTR a_QueryLanguage,
		const BSTR a_Query,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecQueryAsync (

		const BSTR a_QueryLanguage ,
		const BSTR a_Query ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecNotificationQuery (

		const BSTR a_QueryLanguage ,
		const BSTR a_Query ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecNotificationQueryAsync ( 

        const BSTR a_QueryLanguage ,
        const BSTR a_Query ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecMethod (

        const BSTR a_ObjectPath ,
        const BSTR a_MethodName ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemClassObject *a_InParams ,
        IWbemClassObject **a_OutParams ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE ExecMethodAsync ( 

		const BSTR a_ObjectPath ,
		const BSTR a_MethodName ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemClassObject *a_InParams ,
		IWbemObjectSink *a_Sink
	) ;

	/* IWbemPropertyProvider */

    HRESULT STDMETHODCALLTYPE GetProperty (

        long a_Flags ,
        const BSTR a_Locale ,
        const BSTR a_ClassMapping ,
        const BSTR a_InstanceMapping ,
        const BSTR a_PropertyMapping ,
        VARIANT *a_Value
	) ;
        
    HRESULT STDMETHODCALLTYPE PutProperty (

        long a_Flags ,
        const BSTR a_Locale ,
        const BSTR a_ClassMapping ,
        const BSTR a_InstanceMapping ,
        const BSTR a_PropertyMapping ,
        const VARIANT *a_Value
	) ;

	/* IWbemEventProvider */

	HRESULT STDMETHODCALLTYPE ProvideEvents (

		IWbemObjectSink *a_Sink ,
		long a_Flags
	) ;

	/* IWbemEventProviderQuerySink */

	HRESULT STDMETHODCALLTYPE NewQuery (

		unsigned long a_Id ,
		WBEM_WSTR a_QueryLanguage ,
		WBEM_WSTR a_Query
	);

	HRESULT STDMETHODCALLTYPE CancelQuery (

		unsigned long a_Id
	) ;

	/* IWbemEventProviderSecurity */

	HRESULT STDMETHODCALLTYPE AccessCheck (

		WBEM_CWSTR a_QueryLanguage ,
		WBEM_CWSTR a_Query ,
		long a_SidLength ,
		const BYTE *a_Sid
	) ;

	/* IWbemProviderIdentity */

	HRESULT STDMETHODCALLTYPE SetRegistrationObject (

		long a_Flags ,
		IWbemClassObject *a_ProviderRegistration
	) ;

	/* IWbemEventConsumerProvider */

	HRESULT STDMETHODCALLTYPE FindConsumer (

		IWbemClassObject *a_LogicalConsumer ,
		IWbemUnboundObjectSink **a_Consumer
	);

	/* IWbemEventConsumerProviderEx */

	HRESULT STDMETHODCALLTYPE ValidateSubscription (

		IWbemClassObject *a_LogicalConsumer
	) ;

	/* IWbemUnboundObjectSink */

	HRESULT STDMETHODCALLTYPE IndicateToConsumer (

		IWbemClassObject *a_LogicalConsumer ,
		long a_ObjectCount ,
		IWbemClassObject **a_Objects
	) ;

/* Internal_IWbemServices */
 
     HRESULT STDMETHODCALLTYPE Internal_OpenNamespace ( 

		WmiInternalContext a_InternalContext ,
        const BSTR a_Namespace ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemServices **a_Service ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_CancelAsyncCall ( 

		WmiInternalContext a_InternalContext ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_QueryObjectSink ( 

		WmiInternalContext a_InternalContext ,
        long a_Flags ,
        IWbemObjectSink **a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_GetObject ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_ObjectPath ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemClassObject **ppObject ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_GetObjectAsync (
        
		WmiInternalContext a_InternalContext ,
		const BSTR a_ObjectPath ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;

    HRESULT STDMETHODCALLTYPE Internal_PutClass ( 

		WmiInternalContext a_InternalContext ,
        IWbemClassObject *a_Object ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_PutClassAsync ( 

		WmiInternalContext a_InternalContext ,
        IWbemClassObject *a_Object ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_DeleteClass ( 

		WmiInternalContext a_InternalContext ,
        const BSTR a_Class ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_DeleteClassAsync ( 

		WmiInternalContext a_InternalContext ,
        const BSTR a_Class ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_CreateClassEnum ( 

		WmiInternalContext a_InternalContext ,
        const BSTR a_Superclass ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_CreateClassEnumAsync ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_Superclass ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_PutInstance (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_Instance ,
		long a_Flags , 
		IWbemContext *a_Context ,
		IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_PutInstanceAsync (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_Instance ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink 
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_DeleteInstance ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_ObjectPath ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_DeleteInstanceAsync ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_ObjectPath,
		long a_Flags,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_CreateInstanceEnum (

		WmiInternalContext a_InternalContext ,
		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_CreateInstanceEnumAsync (

		WmiInternalContext a_InternalContext ,
		const BSTR a_Class ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecQuery ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_QueryLanguage,
		const BSTR a_Query,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecQueryAsync (

		WmiInternalContext a_InternalContext ,
		const BSTR a_QueryLanguage ,
		const BSTR a_Query ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecNotificationQuery (

		WmiInternalContext a_InternalContext ,
		const BSTR a_QueryLanguage ,
		const BSTR a_Query ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IEnumWbemClassObject **a_Enum
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecNotificationQueryAsync ( 

		WmiInternalContext a_InternalContext ,
        const BSTR a_QueryLanguage ,
        const BSTR a_Query ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemObjectSink *a_Sink
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecMethod (

		WmiInternalContext a_InternalContext ,
        const BSTR a_ObjectPath ,
        const BSTR a_MethodName ,
        long a_Flags ,
        IWbemContext *a_Context ,
        IWbemClassObject *a_InParams ,
        IWbemClassObject **a_OutParams ,
        IWbemCallResult **a_CallResult
	) ;
    
    HRESULT STDMETHODCALLTYPE Internal_ExecMethodAsync ( 

		WmiInternalContext a_InternalContext ,
		const BSTR a_ObjectPath ,
		const BSTR a_MethodName ,
		long a_Flags ,
		IWbemContext *a_Context ,
		IWbemClassObject *a_InParams ,
		IWbemObjectSink *a_Sink
	) ;

	/* Internal_IWbemPropertyProvider */

    HRESULT STDMETHODCALLTYPE Internal_GetProperty (

		WmiInternalContext a_InternalContext ,
        long a_Flags ,
        const BSTR a_Locale ,
        const BSTR a_ClassMapping ,
        const BSTR a_InstanceMapping ,
        const BSTR a_PropertyMapping ,
        VARIANT *a_Value
	) ;
        
    HRESULT STDMETHODCALLTYPE Internal_PutProperty (

		WmiInternalContext a_InternalContext ,
        long a_Flags ,
        const BSTR a_Locale ,
        const BSTR a_ClassMapping ,
        const BSTR a_InstanceMapping ,
        const BSTR a_PropertyMapping ,
        const VARIANT *a_Value
	) ;

	/* Internal_IWbemEventProvider */

	HRESULT STDMETHODCALLTYPE Internal_ProvideEvents (

		WmiInternalContext a_InternalContext ,
		IWbemObjectSink *a_Sink ,
		long a_Flags
	) ;

	/* Internal_IWbemEventProviderQuerySink */

	HRESULT STDMETHODCALLTYPE Internal_NewQuery (

		WmiInternalContext a_InternalContext ,
		unsigned long a_Id ,
		WBEM_WSTR a_QueryLanguage ,
		WBEM_WSTR a_Query
	);

	HRESULT STDMETHODCALLTYPE Internal_CancelQuery (

		WmiInternalContext a_InternalContext ,
		unsigned long a_Id
	) ;

	/* Internal_IWbemEventProviderSecurity */

	HRESULT STDMETHODCALLTYPE Internal_AccessCheck (

		WmiInternalContext a_InternalContext ,
		WBEM_CWSTR a_QueryLanguage ,
		WBEM_CWSTR a_Query ,
		long a_SidLength ,
		const BYTE *a_Sid
	) ;

	/* Internal_IWbemEventConsumerProvider */

	HRESULT STDMETHODCALLTYPE Internal_FindConsumer (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_LogicalConsumer ,
		IWbemUnboundObjectSink **a_Consumer
	);

	/* Internal_IWbemEventConsumerProviderEx */

	HRESULT STDMETHODCALLTYPE Internal_ValidateSubscription (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_LogicalConsumer
	) ;

	/* Internal_IWbemUnboundObjectSink */

	HRESULT STDMETHODCALLTYPE Internal_IndicateToConsumer (

		WmiInternalContext a_InternalContext ,
		IWbemClassObject *a_LogicalConsumer ,
		long a_ObjectCount ,
		IWbemClassObject **a_Objects
	) ;


	/* IWbemProviderInit methods */

	HRESULT STDMETHODCALLTYPE Initialize (

		LPWSTR a_User ,
		LONG a_Flags ,
		LPWSTR a_Namespace ,
		LPWSTR a_Locale ,
		IWbemServices *a_CoreService ,
		IWbemContext *a_Context ,
		IWbemProviderInitSink *a_Sink
	) ;

	HRESULT STDMETHODCALLTYPE Internal_Initialize (

		WmiInternalContext a_InternalContext ,
		LPWSTR a_User ,
		LONG a_Flags ,
		LPWSTR a_Namespace ,
		LPWSTR a_Locale ,
		IWbemServices *a_CoreService ,
		IWbemContext *a_Context ,
		IWbemProviderInitSink *a_Sink
	) ;

	/* _IWmiProviderSite */

	HRESULT STDMETHODCALLTYPE GetSite ( DWORD *a_ProcessIdentifier ) ;

	HRESULT STDMETHODCALLTYPE SetContainer ( IUnknown *a_Container ) ;

	// IWmi_UnInitialize members

	HRESULT STDMETHODCALLTYPE Shutdown (

		LONG a_Flags ,
		ULONG a_MaxMilliSeconds ,
		IWbemContext *a_Context
	) ; 
} ;

#endif // _Decopled_Interceptor_IWbemServices_H
