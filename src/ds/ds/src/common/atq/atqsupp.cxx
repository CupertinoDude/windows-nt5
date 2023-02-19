/*++

   Copyright    (c)    1994-1996    Microsoft Corporation

   Module  Name :

       atqsupp.cxx

   Abstract:

        Contains internal support routines for the ATQ package
        From atqnew.c

   Author:
        Murali R. Krishnan     (MuraliK)     02-Apr-1996

   Project:
        Internet Server Common DLL
--*/

#include "isatq.hxx"

typedef GUID UUID;

extern "C" {

#include <ntdsa.h>

}

DWORD AtqPoolThread( LPDWORD param );

VOID I_AtqThreadMonitor( IN PVOID NotUsed );

DWORD g_dwThreadMonitorCookie = 0;

extern PBANDWIDTH_INFO g_pBandwidthInfo;

# ifdef ENABLE_REUSE_THROTTLE

DWORD g_fAlwaysReuseSockets = TRUE;

# endif // ENABLE_REUSE_THROTTLE

//
// If this is true then no AtqPoolThreads have completed
// servicing an IO since the last time I_AtqThreadMonitor
// ran.
//
BOOL  g_fDeadManSwitch = FALSE;


/************************************************************
 * Functions for ATQ_CONTEXT
 ************************************************************/


PATQ_CONT
I_AtqAllocContextFromCache( VOID);

VOID
I_AtqFreeContextToCache(
            IN PATQ_CONT pAtqContext,
            IN BOOL UnlinkContext
            );
VOID
I_ResetListenSocket(
    IN PATQ_ENDPOINT pEndpoint
    );



PATQ_CONT
I_AtqAllocContextFromCache( VOID)
/*++
  This function attempts to allocate an ATQ context from the allocation cache.
  It then initializes the state information in the ATQ context object and
    returns the context on success.

  Arguments:
    None

  Returns:
    On success a valid pointer to ATQ_CONT. Otherwise NULL.

--*/
{
    PATQ_CONT  pAtqContext;

    DBG_ASSERT( NULL != g_pachAtqContexts);

    pAtqContext = (ATQ_CONTEXT * ) g_pachAtqContexts->Alloc();

    if ( NULL != pAtqContext ) {


        pAtqContext->ContextList =
            &AtqActiveContextList[(++AtqGlobalContextCount %
                                   g_dwNumContextLists)];

        pAtqContext->Signature = ATQ_CONTEXT_SIGNATURE;
    }

    return (pAtqContext);
} // I_AtqAllocContextFromCache()




VOID
I_AtqFreeContextToCache(
        IN PATQ_CONT pAtqContext
        )
/*++
  This function releases the given context to the allocation cache.

  Arguments:
    pAtqContext  pointer to the ATQ_CONTEXT that is being freed.

  Returns:
    None

  Issues:
    This function also performs some other cleanup specific to AtqContexts.
--*/
{

#if 0
      ATQ_PRINTF(( DBG_CONTEXT,
                 "[I_AtqFreeCtxtToCache] Freed up %08x\n",
                 pAtqContext
                 ));
#endif

    DBG_ASSERT( pAtqContext->Signature == ATQ_FREE_CONTEXT_SIGNATURE);
    DBG_ASSERT( pAtqContext->lSyncTimeout ==0);
    DBG_ASSERT( pAtqContext->m_nIO ==0);
    DBG_ASSERT( pAtqContext->m_acFlags == 0);
    DBG_ASSERT( pAtqContext->m_acState == 0);
    DBG_ASSERT( pAtqContext->m_leTimeout.Flink == NULL);
    DBG_ASSERT( pAtqContext->m_leTimeout.Blink == NULL);
    DBG_ASSERT( pAtqContext->pvBuff == NULL);
    DBG_ASSERT( pAtqContext->pEndpoint == NULL);
    DBG_ASSERT( pAtqContext->hAsyncIO == NULL);

    DBG_REQUIRE( g_pachAtqContexts->Free( pAtqContext));

    return;

} // I_AtqFreeContextToCache



void
ATQ_CONTEXT::Print( void) const
{
    DBGPRINTF(( DBG_CONTEXT,
                " ATQ_CONTEXT (%08x)\n"
                "\thAsyncIO            = %08lx   Signature        = %08lx\n"
                "\tOverlapped.Internal = %08lx   Overlapped.Offset= %08lx\n"
                "\tm_leTimeout.Flink   = %08lx   m_leTimeout.Blink= %08lx\n"
                "\tClientContext       = %08lx   ContextList      = %08lx\n"
                "\tpfnCompletion       = %08lx ()\n"
                "\tpEndPoint           = %08lx   fAcceptExContext = %s\n"
                "\tlSyncTimeout        = %8d     fInTimeout       = %s\n"

                "\tTimeOut             = %08lx   NextTimeout      = %08lx\n"
                "\tBytesSent           = %d (0x%08lx)\n"

                "\tpvBuff              = %08lx   JraAsyncIo       = %08lx\n"
                "\tfConnectionIndicated= %s      fBlocked         = %8lx\n"

                "\tState               = %8lx    Flags            = %8lx\n",
                this,
                hAsyncIO,
                Signature,
                Overlapped.Internal,
                Overlapped.Offset,
                m_leTimeout.Flink,
                m_leTimeout.Blink,
                ClientContext,
                ContextList,
                pfnCompletion,
                pEndpoint,
                (IsAcceptExRootContext() ? "TRUE" : "FALSE"),
                lSyncTimeout,
                (IsFlag( ACF_IN_TIMEOUT) ? "TRUE" : "FALSE"),
                TimeOut,
                NextTimeout,
                BytesSent,
                BytesSent,
                pvBuff,
                hJraAsyncIO,
                (IsFlag( ACF_CONN_INDICATED) ? "TRUE" : "FALSE"),
                IsBlocked(),
                m_acState, m_acFlags
                ));

    // Print the buffer if necessary.

    return;
} // ATQ_CONTEXT::Print()




VOID
ATQ_CONTEXT::HardCloseSocket( VOID)
/*++
  Description:
     This socket closes the socket by forcibly calling closesocket() on
     the socket. This function is used during the endpoint shutdown
     stage for an atq context

  Arguments:
     None

  Returns:
     None

--*/
{
    HANDLE haio = (HANDLE )
        InterlockedExchangePointer( (PVOID *) &hAsyncIO, NULL);

    DBG_ASSERT( IsState( ACS_SOCK_LISTENING) ||
                IsState( ACS_SOCK_CONNECTED) ||
                IsState( ACS_SOCK_CLOSED) ||
                IsState( ACS_SOCK_UNCONNECTED)
                );

    MoveState( ACS_SOCK_CLOSED);


    //
    //  Let us do a hard close on the socket (handle).
    //  This should generate an IO completion which will free this
    //     ATQ context
    //

    if ( !this->fDatagramContext ) {
        if ( (haio != NULL) &&
             (closesocket((SOCKET) haio) == SOCKET_ERROR)
             ) {

            ATQ_PRINTF(( DBG_CONTEXT,
                         "Warning - "
                         " Context=%08x closesocket failed,"
                         " error %d, socket = %x\n",
                         this,
                         GetLastError(),
                         haio ));
            Print();
        }
    }

    return;
} // ATQ_CONTEXT::HardCloseSocket()



VOID
ATQ_CONTEXT::InitWithDefaults(
    IN ATQ_COMPLETION pfnCompletion,
    IN DWORD TimeOut,
    IN HANDLE hAsyncIO
    )
{
    DBG_ASSERT( this->Signature == ATQ_CONTEXT_SIGNATURE);

    this->InitTimeoutListEntry();

    this->Signature    = ATQ_CONTEXT_SIGNATURE;

     // start life at 1. This ref count will be freed up by AtqFreeContext()
    this->m_nIO = 1;

    this->pfnCompletion   = pfnCompletion;

    this->TimeOut         = TimeOut;
    this->TimeOutScanID   = 0;
    this->lSyncTimeout    = 0;

    this->hAsyncIO        = hAsyncIO;
    this->hJraAsyncIO     = HandleToUlong(hAsyncIO) | 0x80000000;

    this->m_acState       = 0;
    this->m_acFlags       = 0;

    this->fDatagramContext  = FALSE;

    // Initialize pbandwidthinfo to point to global object

    this->m_pBandwidthInfo  = g_pBandwidthInfo;

    ZeroMemory(
               &this->Overlapped,
               sizeof( this->Overlapped )
               );

    DBG_ASSERT( this->lSyncTimeout == 0);

    //
    // Following added for bandwidth throttling purposes
    //

    DBG_ASSERT( !this->IsBlocked());
    this->arInfo.atqOp    = AtqIoNone;
    this->arInfo.lpOverlapped = NULL;
    // bandwidth throttling initialization ends here.

} // ATQ_CONTEXT::InitWithDefaults()




VOID
ATQ_CONTEXT::InitNonAcceptExState(
    IN PVOID pClientContext
    )
{
    //
    //  Note that if we're not using AcceptEx, then we consider the client
    //  to have been notified externally (thus ACF_CONN_INDICATED is set).
    //  Also we set the next timeout to be infinite, which may be reset
    //   when the next IO is submitted.
    //

    this->NextTimeout          = ATQ_INFINITE;
    this->ClientContext        = pClientContext;
    this->pEndpoint            = NULL;
    this->SetFlag( ACF_CONN_INDICATED);
    this->SetState( ACS_SOCK_CONNECTED);
    this->ResetFlag( ACF_ACCEPTEX_ROOT_CONTEXT);

    //
    // Insert this into the active list - since this is a non-acceptex socket
    //

    DBG_ASSERT( this->ContextList != NULL);
    this->ContextList->InsertIntoActiveList( &this->m_leTimeout );

    return;

} // ATQ_CONTEXT::InitNonAcceptExState()



VOID
ATQ_CONTEXT::InitAcceptExState(
            IN DWORD NextTimeOut
            )
{
    this->NextTimeout          = NextTimeOut;
    this->ClientContext        = NULL;
    this->lSyncTimeout         = 0;

    this->ResetFlag( ACF_CONN_INDICATED);
    this->SetState( ACS_SOCK_LISTENING);

    //
    //  Add it to the pending accept ex list
    //
    DBG_ASSERT( this->ContextList != NULL);

    this->ContextList->InsertIntoPendingList( &this->m_leTimeout);

    return;
} // ATQ_CONTEXT::InitAcceptExState()



BOOL
ATQ_CONTEXT::PrepareAcceptExContext(
    PATQ_ENDPOINT          pEndpoint
    )
/*++

Routine Description:

    Initializes the state for completely initializing the state and
    hence prepares the context for AcceptEx

    It expects the caller to send a AtqContext with certain characteristics
      1) this is not NULL
      2) this->pvBuff has valid values

    In the case of failure, caller should call
     pAtqContext->CleanupAndRelese() to free the memory associated with
     this object.

Arguments:

    pEndpoint     - pointer to endpoint object for this context

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

    The caller should free the object on a failure.

--*/
{
    DBG_ASSERT( g_fUseAcceptEx); // only support AcceptEx() cases
    DBG_ASSERT( pEndpoint != NULL);
    DBG_ASSERT( this != NULL);
    DBG_ASSERT( this->pvBuff != NULL);

    //
    //  Make sure that we are adding a AcceptEx() version of AtqContext
    //

    DBG_ASSERT( pEndpoint->ConnectExCompletion != NULL);
    DBG_ASSERT( pEndpoint->UseAcceptEx);

    //
    //  Fill out the context.  We set NextTimeout to INFINITE
    //  so the timeout thread will ignore this entry until an IO
    //  request is made unless this is an AcceptEx socket, that means
    //  we're about to submit the IO.
    //

    this->
        InitWithDefaults(
                         pEndpoint->IoCompletion,
                         pEndpoint->AcceptExTimeout, // canonical Timeout
                         this->hAsyncIO
                         );


    //
    // TBD: What is the circumstance in which this->pEndpoint!= NULL?
    //

    if ( this->pEndpoint == NULL ) {
        pEndpoint->Reference();
        this->pEndpoint  = pEndpoint;
    }

    this->InitDatagramState( );

    this->ResetFlag( ACF_ACCEPTEX_ROOT_CONTEXT );

    this->InitAcceptExState( AtqGetCurrentTick() + TimeOut);

    DBG_ASSERT( this->pvBuff != NULL);

    return (TRUE);

} // ATQ_CONTEXT::PrepareAcceptExContext()




VOID
ATQ_CONTEXT::CleanupAndRelease( VOID)
/*++
  Routine Description:
     This function does the cleanup of the ATQ context. It does not
     attempt to do any reuse of the atq context. After cleanup
     the context is freed to the ATQ pool. Supplied context
     is not valid after calling this function.

  Arguments:
     None

  Returns:
     None
--*/
{
    DBG_ASSERT( this->m_nIO == 0);

    //
    //  Cleanup and free the ATQ Context entirely
    //

    if ( this->hAsyncIO != NULL ) {

        // It is too dangerous to assume that the handle is a socket!
        // But we will do that for fast-pathing IIS operations.

        SOCKET hIO =
            (SOCKET ) InterlockedExchangePointer( (PVOID *) &this->hAsyncIO,
                                                  NULL);
        //
        // no cleanups for datagram
        //

        if ( !this->fDatagramContext ) {

            if ( hIO != NULL &&
                 (closesocket( hIO ) == SOCKET_ERROR ) ) {

                ATQ_PRINTF(( DBG_CONTEXT,
                             "ATQ_CONTEXT(%08x)::CleanupAndRelease() : Warning"
                             " - Context=%08x, "
                             " closesocket failed, error %d, socket = %x\n",
                             this,
                             GetLastError(),
                             hIO ));
                this->Print();
            }
        }
    }

    DBG_ASSERT( this->hAsyncIO == NULL);

    if ( this->pvBuff != NULL ) {
        LocalFree( this->pvBuff );
        this->pvBuff = NULL;
    }

    //
    // Unlink from the list
    //

    DBG_ASSERT( this->ContextList != NULL);

    // NYI: Can I avoid this comparison?
    //
    // Check if this context is part of a timeout list.
    // If it is then remove it from the list
    // Only during shutdown code path, we will see trouble here.
    //
    if ( this->m_leTimeout.Flink != NULL ) {
        this->ContextList->RemoveFromList( &this->m_leTimeout);
    }

    //
    //  Deref the listen info if this context is associated with one
    //

    if ( this->pEndpoint != NULL ) {
        this->pEndpoint->Dereference();
        this->pEndpoint = NULL;
    }

    this->Signature    = ATQ_FREE_CONTEXT_SIGNATURE;
    this->lSyncTimeout = 0;
    this->m_acState    = 0;
    this->m_acFlags    = 0;

    I_AtqFreeContextToCache( this);

    return;
} // ATQ_CONTEXT::CleanupAndRelease()





inline VOID
DBG_PRINT_ATQ_SPUDCONTEXT( IN PATQ_CONT  pAtqContext,
                           IN PSPUD_REQ_CONTEXT reqContext)
{
    ATQ_PRINTF(( DBG_CONTEXT,
                 "[AtqPoolThread] pAtqContext = %08lx\n"
                 "[AtqPoolThread] IoStatus1.Status = %08lx\n"
                 "[AtqPoolThread] IoStatus1.Information = %08lx\n"
                 "[AtqPoolThread] IoStatus2.Status = %08lx\n"
                 "[AtqPoolThread] IoStatus2.Information = %08lx\n"
                 ,
                 pAtqContext,
                 reqContext->IoStatus1.Status,
                 reqContext->IoStatus1.Information,
                 reqContext->IoStatus2.Status,
                 reqContext->IoStatus2.Information
                 ));
    return;
} // DBG_PRINT_ATQ_SPUDCONTEXT()


VOID
AtqpUpdateBandwidth( IN PATQ_CONT  pAtqContext,
                    IN DWORD      cbWritten)
{
    PBANDWIDTH_INFO pBandwidthInfo = pAtqContext->m_pBandwidthInfo;

    DBG_ASSERT( pBandwidthInfo != NULL );
    DBG_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    // add the bandwidth info to active list if necessary

    pBandwidthInfo->AddToActiveList();

    //this will have problems when we use XmitFile for large files.

    pBandwidthInfo->UpdateBytesXfered( pAtqContext, cbWritten );
} // AtqpUpdateBandwidth()


VOID
AtqpCallOplockCompletion( IN PATQ_CONT pAtqContext,
                          IN DWORD cbWritten)
{
    ATQ_OPLOCK_COMPLETION pfnOplockCompletion;
    PVOID OplockContext;
    POPLOCK_INFO pOplock;

    IF_DEBUG( SPUD) {
        DBGPRINTF(( DBG_CONTEXT,
                    "CallOplockCompletion on OpLockInfo=%08x.cbWritten = %d\n",
                    (POPLOCK_INFO ) pAtqContext, cbWritten));
    }

    //
    // The ATQ context object received is a fake one. We actually get
    //  back POPLOCK_INFO object that is used to extract the callback
    //  function & context for the callback
    //

    pOplock = (POPLOCK_INFO)pAtqContext;
    pfnOplockCompletion = (ATQ_OPLOCK_COMPLETION)pOplock->pfnOplockCompletion;
    OplockContext = (PVOID)pOplock->Context;

    LocalFree(pOplock);


    (*pfnOplockCompletion)(OplockContext, (DWORD)cbWritten);
    return;

} // AtqpCallOplockCompletion()



VOID
AtqpProcessContext( IN PATQ_CONT  pAtqContext,
                    IN DWORD      cbWritten,
                    IN LPOVERLAPPED lpo,
                    IN BOOL       fRet)
{
    BOOL fDriverCall = FALSE;
    BOOL fRecvCalled = FALSE;
    PSPUD_REQ_CONTEXT  reqContext = NULL;
    DWORD dwError;

    DBG_ASSERT( pAtqContext != NULL);

    //
    // Check to see if this is a completion request from the
    // NTS kernel driver.
    //

    if ( lpo == NULL ) {

        if ( cbWritten == 0xffffffff ) {

            //
            // One of the SPUD's IO completion. Handle it appropriately.
            //

            reqContext = (PSPUD_REQ_CONTEXT)pAtqContext;

            pAtqContext = CONTAINING_RECORD( reqContext, ATQ_CONTEXT,
                                             spudContext );

            IF_DEBUG( SPUD) {
                DBG_PRINT_ATQ_SPUDCONTEXT( pAtqContext, reqContext);
            }
#if CC_REF_TRACKING
            //
            // ATQ notification trace
            //
            // Notify client context of all non-oplock notification.
            // This is for debugging purpose only.
            //
            // Code 0xfcfcfcfc indicates a SPUD I/O Completion
            //

            pAtqContext->NotifyIOCompletion( cbWritten, reqContext->IoStatus1.Status, 0xfcfcfcfc );
#endif

            cbWritten = (DWORD)reqContext->IoStatus1.Information;
            fRet = (reqContext->IoStatus1.Status == STATUS_SUCCESS);
            SetLastError(RtlNtStatusToDosError(reqContext->IoStatus1.Status));

            lpo = &pAtqContext->Overlapped;

            //
            // If the TransmitFile fails then the receive is not issued.
            //

            if ( fRet ) {
                fDriverCall = TRUE;
            } else {
                DBG_ASSERT( fDriverCall == FALSE);
                pAtqContext->ResetFlag( ACF_RECV_ISSUED);
            }
        } else {

            //
            // An Oplock notification - handle it via oplock path.
            //

            AtqpCallOplockCompletion( pAtqContext, cbWritten);
            return;
        }
    }

    dwError = (fRet) ? NO_ERROR: GetLastError();

    //
    //  If this is an AcceptEx listen socket atq completion, then the
    //  client Atq context we really want is keyed from the overlapped
    //  structure that is stored in the client's Atq context.
    //

    if ( pAtqContext->IsAcceptExRootContext() || pAtqContext->fDatagramContext ) {

        pAtqContext = CONTAINING_RECORD( lpo, ATQ_CONTEXT, Overlapped );

    }


#if CC_REF_TRACKING
    //
    // ATQ notification trace
    //
    // Notify client context of all non-oplock notification.
    // This is for debugging purpose only.
    //

    pAtqContext->NotifyIOCompletion( cbWritten, (fRet) ? NO_ERROR: GetLastError(), 0xfefefefe );
#endif

    DBG_CODE(
             if ( ATQ_CONTEXT_SIGNATURE != pAtqContext->Signature) {
                 pAtqContext->Print();
                 DBG_ASSERT( FALSE);
             });


    //
    //  m_nIO also acts as the reference count for the atq contexts
    //  So, increment the count now, so that there is no other thread
    //   that will free up this ATQ context accidentally.
    //

    InterlockedIncrement( &pAtqContext->m_nIO);

    //
    // Busy wait for timeout processing to complete!
    //  This is ugly :( A fix in time for IIS 2.0/Catapult 1.0 release
    //

    InterlockedIncrement(  &pAtqContext->lSyncTimeout);
    while ( pAtqContext->IsFlag( ACF_IN_TIMEOUT)) {

        AcIncrement( CacAtqWaitsForTimeout);

        Sleep( ATQ_WAIT_FOR_TIMEOUT_PROCESSING);
    };

    //
    //  We need to make sure the timeout thread doesn't time this
    //  request out so reset the timeout value
    //

    InterlockedExchange( (LPLONG )&pAtqContext->NextTimeout,
                         (LONG ) ATQ_INFINITE);

    //
    // Update Bandwidth information on successful completion, if needed
    //

    if ( BANDWIDTH_INFO::GlobalEnabled() && fRet && cbWritten > 0)
    {
        AtqpUpdateBandwidth( pAtqContext, cbWritten);
    }

    //
    // Since the IO completion means that one of the async operation finished
    //  decrement our internal ref count appropriately to balance the addition
    //  when the IO operation was submitted.
    //
    InterlockedDecrement( &pAtqContext->m_nIO);

    //
    //  Is this a connection indication?
    //

    if ( !pAtqContext->IsFlag( ACF_CONN_INDICATED) &&
         !pAtqContext->fDatagramContext ) {

        PATQ_ENDPOINT pEndpoint = pAtqContext->pEndpoint;

        if ( NULL == pEndpoint) {
            pAtqContext->Print();
            OutputDebugString( "Found an ATQ context with bad Endpoint\n");
            DBG_ASSERT( FALSE);
            DBG_REQUIRE( InterlockedDecrement(  &pAtqContext->lSyncTimeout) == 0);
            InterlockedDecrement( &pAtqContext->m_nIO); // balance entry count
            return;
        }

        DBG_ASSERT( pEndpoint != NULL );

        //
        //  Indicate this socket is in use
        //

        InterlockedDecrement( &pEndpoint->nSocketsAvail );

        //
        //  If we're running low on sockets, add some more now
        //

        if ( pEndpoint->nSocketsAvail <
             (LONG )(pEndpoint->nAcceptExOutstanding >> 2) ) {

            AcIncrement( CacAtqPrepareContexts);

            (VOID ) I_AtqPrepareAcceptExSockets(pEndpoint,
                                                pEndpoint->nAcceptExOutstanding
                                                );
        }

        //
        //  If an error occurred on this completion,
        //    shutdown the socket
        //

        if ( !fRet ) {

            IF_DEBUG( ERROR) {
                if ( dwError != ERROR_OPERATION_ABORTED ) {
                    ATQ_PRINTF(( DBG_CONTEXT,
                                 " Free Context(%08x, EP=%08x) to cache. "
                                 "Err=%d, sock=%08x\n",
                                 pAtqContext, pEndpoint,
                                 dwError,
                                 pAtqContext->hAsyncIO));
                }
            }

            DBG_REQUIRE( InterlockedDecrement(  &pAtqContext->lSyncTimeout) == 0);

            InterlockedDecrement( &pAtqContext->m_nIO); // balance entry count

            // balance original count
            InterlockedDecrement( &pAtqContext->m_nIO);
            // Free up the atq context without Reuse
            pAtqContext->CleanupAndRelease();
            return;
        }

        //
        //  Shutdown may close the socket from underneath us so don't
        //  assert, just warn.
        //

        if ( !pAtqContext->IsState( ACS_SOCK_LISTENING) ) {

            ATQ_PRINTF(( DBG_CONTEXT,
                         "[AtqPoolThread] Warning-Socket state not listening\n"
                         ));
            DBG_CODE( pAtqContext->Print());
        }

        pAtqContext->MoveState( ACS_SOCK_CONNECTED);

        //
        // Remove the context from the pending list and put
        // it on the active list
        //

        DBG_ASSERT( pAtqContext->ContextList != NULL);
        pAtqContext->ContextList->MoveToActiveList( &pAtqContext->m_leTimeout);

        //
        //  Set the connection indicated flag.  After we return from
        //  the connection completion routine we assume it's
        //  safe to call the IO completion routine
        //  (or the connection indication routine should do cleanup
        //  and never issue an IO request).  This is primarily for
        //  the timeout thread.
        //

        pAtqContext->ConnectionCompletion( cbWritten, lpo);
    } else {


        //
        //  Not a connection completion indication. I/O completion.
        //

        //
        //  If an error occurred on a TransmitFile (or other IO),
        //  set the state to connected so the socket will get
        //  closed on cleanup
        //

        if ( !fRet &&
             pAtqContext->IsState( ACS_SOCK_UNCONNECTED)
             ){
            pAtqContext->MoveState( ACS_SOCK_CONNECTED);
        }

#if 0
        if (fDriverCall) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "[AtqPoolThread] pfnCompletion1(%08lx)\n",
                         pAtqContext ));
        }
#endif

        //
        // !!! DS
        // Indicate that we have completed a datagram context
        //

        if ( !pAtqContext->IsFlag( ACF_CONN_INDICATED) ) {

            PATQ_ENDPOINT pEndpoint = pAtqContext->pEndpoint;

            pAtqContext->SetFlag( ACF_CONN_INDICATED);
            if ( pEndpoint != NULL ) {

                ATQ_ASSERT(pAtqContext->fDatagramContext);

                //
                // Decrement the in-use count here for
                //

                InterlockedDecrement( &pEndpoint->nSocketsAvail );

                //
                //  If we're running low on sockets, add some more now
                //

                if ( pEndpoint->nSocketsAvail <
                     (LONG )(pEndpoint->nAcceptExOutstanding >> 2) ) {

                    (VOID ) I_AtqPrepareAcceptExSockets(pEndpoint,
                                                        pEndpoint->nAcceptExOutstanding
                                                        );
                }
            }
        }

        pAtqContext->IOCompletion( cbWritten, dwError, lpo);

        if (fDriverCall) {
            pAtqContext->ResetFlag( ACF_RECV_ISSUED);
            fRet = (reqContext->IoStatus2.Status == STATUS_SUCCESS);
            SetLastError( RtlNtStatusToDosError(
                                         reqContext->IoStatus2.Status));

            //
            //  If an error occurred on a TransmitFile (or other IO),
            //  set the state to connected so the socket will get
            //  closed on cleanup
            //

            if ( !fRet &&
                 pAtqContext->IsState( ACS_SOCK_UNCONNECTED) ) {
                pAtqContext->MoveState( ACS_SOCK_CONNECTED);
            }

#if CC_REF_TRACKING
            //
            // ATQ notification trace
            //
            // Notify client context of status after 1st notification
            // This is for debugging purpose only.
            //
            // Code 0xfafafafa means we're processing a recv that
            // SPUD combined with another notification
            //

            pAtqContext->NotifyIOCompletion( pAtqContext->m_acFlags, reqContext->IoStatus1.Status, 0xfafafafa );
#endif
            if ( pAtqContext->IsFlag( ACF_RECV_CALLED ) ) {
                fRecvCalled = TRUE;
                pAtqContext->ResetFlag( ACF_RECV_CALLED);
            }

            if ((reqContext->IoStatus1.Status == STATUS_SUCCESS) &&
                (pAtqContext->ClientContext != NULL) &&
                fRecvCalled ) {

#if CC_REF_TRACKING
                //
                // ATQ notification trace
                //
                // Notify client context of all non-oplock notification.
                // This is for debugging purpose only.
                //
                // Code 0xfdfdfdfd means we're processing a recv that
                // SPUD combined with another notification
                //

                pAtqContext->NotifyIOCompletion( cbWritten, (fRet) ? NO_ERROR: GetLastError(), 0xfdfdfdfd );
#endif


                IF_DEBUG( SPUD) {

                    ATQ_PRINTF(( DBG_CONTEXT,
                                 "[AtqPoolThread] pfnCompletion2(%08lx)\n",
                                 pAtqContext ));
                };

                pAtqContext->IOCompletion( (DWORD)reqContext->IoStatus2.Information,
                                        (fRet) ? NO_ERROR : GetLastError(),
                                        lpo
                                        );
            }
        }
    }

    DBG_ASSERT( pAtqContext->lSyncTimeout > 0);
    InterlockedDecrement( &pAtqContext->lSyncTimeout);

    //
    // We do an interlocked decrement on m_nIO to sync up state
    //  so that the context is not prematurely deleted.
    //

    if ( InterlockedDecrement(  &pAtqContext->m_nIO) == 0) {

        //
        // The number of outstanding ref holders is ZERO.
        // Free up this ATQ context.
        //
        // We really do not free up the context - but try to reuse
        //  it if possible
        //

        // free the atq context now or reuse if possible.
        AtqpReuseOrFreeContext( pAtqContext,
                                (pAtqContext->
                                 IsFlag( ACF_REUSE_CONTEXT) != 0)
                                );
    }

    return;
} // AtqpProcessContext()



DWORD
AtqPoolThread(
    LPDWORD ThreadType
    )
/*++

Routine Description:

    This is the pool thread wait and dispatch routine

  Arguments:
    ThreadType : Indicates whether this is the initial pool thread, 
    a temp thread, or just a joe blow pool thread.

  Return Value:

    Thread return value (ignored)

--*/
{
    PATQ_CONT    pAtqContext;
    BOOL         fRet;
    LPOVERLAPPED lpo;
    DWORD        cbWritten;
    DWORD        returnValue = NO_ERROR;
    DWORD        availThreads;
    DWORD        dwIOsServiced;


    AtqUpdatePerfStats(AtqConsumerAtq, FLAG_COUNTER_SET, g_cThreads);

    for (;;) {

        pAtqContext = NULL;
        InterlockedIncrement( &g_cAvailableThreads );

        fRet = GetQueuedCompletionStatus( g_hCompPort,
                                          &cbWritten,
                                          (PDWORD_PTR)&pAtqContext,
                                          &lpo,
                                          g_msThreadTimeout );

        availThreads = InterlockedDecrement( &g_cAvailableThreads );

        if ( fRet || lpo ) {

            if ( pAtqContext == NULL) {
                if ( g_fShutdown ) {

                    //
                    // This is our signal to exit.
                    //

                    returnValue = NO_ERROR;
                    break;
                }

                OutputDebugString( "A null context received\n");
                continue;  // some error in the context has occured.
            }

            //
            // Make sure we're not running out of threads
            //

            if ( availThreads == 0 ) {

                //
                //  Make sure there are pool threads to service the request
                //

                (VOID)I_AtqCheckThreadStatus( NULL );
            }

            AtqpProcessContext( pAtqContext, cbWritten, lpo, fRet);

            // Clear the dead man switch
            g_fDeadManSwitch = FALSE;
        } else {

            //
            // don't kill the initial thread
            //

            if ( ((DWORD_PTR)ThreadType == ATQ_INITIAL_THREAD) && !g_fShutdown ) {
                continue;
            }

            //
            //  An error occurred.  Either the thread timed out, the handle
            //  is going away or something bad happened.  Let the thread exit.
            //

            returnValue = GetLastError();

            break;
        }

    } // for

    if ( NULL != g_pfnExitThreadCallback) {

        //
        //  Client wishes to be told when ATQ threads terminate.
        //

        g_pfnExitThreadCallback();
    }

    if ( InterlockedDecrement( &g_cThreads ) == 0 ) {

        //
        // Wake up ATQTerminate()
        //
        IF_DEBUG( ERROR) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "AtqPoolThread() - setting shutdown event %08x."
                         " g_cThreads = %d\n",
                         g_hShutdownEvent, g_cThreads
                         ));
        }

        SetEvent( g_hShutdownEvent );
    }

    AtqUpdatePerfStats(AtqConsumerAtq, FLAG_COUNTER_SET, g_cThreads);

    return returnValue;
} // AtqPoolThread




BOOL
I_AtqCheckThreadStatus(
    PVOID Context
    )
/*++

Routine Description:

    This routine makes sure there is at least one thread in
    the thread pool.  We're fast and loose so a couple of extra
    threads may be created.

Arguments:

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/
{
    BOOL fRet = TRUE;

    //
    //  If no threads are available, kick a new one off up to the limit
    //
    //  WE NEED TO CHANGE THE CONDITIONS FOR STARTING ANOTHER THREAD
    //  IT SHOULD NOT BE VERY EASY TO START A THREAD ....
    //

    if ( (g_cAvailableThreads == 0) &&
         (g_cThreads < g_cMaxThreads) &&
         (g_cThreads < g_cMaxThreadLimit) ) {

        HANDLE hThread;
        DWORD  dwThreadID;

        InterlockedIncrement( &g_cThreads );

        hThread = CreateThread( NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)AtqPoolThread,
                                Context,
                                0,
                                &dwThreadID );

        if ( hThread ) {
            CloseHandle( hThread );     // Free system resources
        } else {

            //
            // We fail if there are no threads running
            //

            if ( InterlockedDecrement( &g_cThreads ) == 0) {
                ATQ_PRINTF(( DBG_CONTEXT,
                    "AtqCheckThread: Cannot create ATQ threads\n"));
                fRet = FALSE;
            }
        }
    }

    return fRet;
} // I_AtqCheckThreadStatus()



/************************************************************
 *  Functions to Add/Delete Atq Contexts
 ************************************************************/


BOOL
I_AtqAddAsyncHandle(
    IN OUT PATQ_CONT  *    ppAtqContext,
    IN PATQ_ENDPOINT       pEndpoint,
    PVOID                  ClientContext,
    ATQ_COMPLETION         pfnCompletion,
    DWORD                  TimeOut,
    HANDLE                 hAsyncIO
    )
/*++

  Description:
    This functio adds creates a new NON-AcceptEx() based Atq Context,
     and includes it in proper lists fo ATQ Context management.


  Note:
    The client should call this after the IO handle is openned
    and before the first IO request is made

    Even in the case of failure, client should call AtqFreeContext() and
     free the memory associated with this object.

--*/
{
    BOOL         fReturn = TRUE;

    DBG_ASSERT( ppAtqContext != NULL);
    DBG_ASSERT( ClientContext != NULL);

    *ppAtqContext = NULL; // initialize

    if ( g_fShutdown) {

        SetLastError( ERROR_NOT_READY);
        return (FALSE);

    } else {

        PATQ_CONT    pAtqContext;

        //
        //  Note we take and release the lock here as we're
        //  optimizing for the reuseable context case
        //

        pAtqContext = I_AtqAllocContextFromCache();
        if ( pAtqContext == NULL) {

            return (FALSE);
        }

        //
        //  Fill out the context.  We set NextTimeout to INFINITE
        //  so the timeout thread will ignore this entry until an IO
        //  request is made unless this is an AcceptEx socket, that means
        //  we're about to submit the IO.
        //


        pAtqContext->InitWithDefaults(pfnCompletion,
                                      CanonTimeout( TimeOut ), hAsyncIO);

        //
        //  These data members are used if we're doing AcceptEx processing
        //

        pAtqContext->SetAcceptExBuffer( NULL);

        pAtqContext->InitNonAcceptExState(ClientContext);

        //
        // If an endpoint is provided, reference it
        //

        if ( pEndpoint != NULL ) {
            pEndpoint->Reference();
            pAtqContext->pEndpoint = pEndpoint;
        }

        *ppAtqContext = pAtqContext;
    }

    return (TRUE);

} // I_AtqAddAsyncHandle()




BOOL
I_AtqAddListenEndpointToPort(
    IN OUT PATQ_CONT    * ppAtqContext,
    IN PATQ_ENDPOINT    pEndpoint
    )
/*++

  Description:
    This function creates a new AtqContext for the given ListenSocket.
    It uses the listen socket as the AcceptEx() socket too for adding
     the atq context to the completion port.
    It assumes
      TimeOut to be INFINITE, with no Endpoint structure.

  Arguments:
    ppAtqContext - pointer to location that will contain the atq context
                   on successful return.
    pEndpoint - pointer to the endpoint.

  Returns:
    TRUE on success
    FALSE if there is a failure.

  Note:
    The caller should free the *ppAtqContext if there is a failure.

--*/
{
    BOOL         fReturn = TRUE;
    PATQ_CONT    pAtqContext;

    DBG_ASSERT( g_fUseAcceptEx); // only support AcceptEx() cases

    *ppAtqContext = NULL; // initialize

    if ( g_fShutdown) {

        SetLastError( ERROR_NOT_READY);
        return (FALSE);

    } else {

        //
        //  Note we take and release the lock here as we're
        //  optimizing for the reuseable context case
        //

        pAtqContext = I_AtqAllocContextFromCache();

        if ( pAtqContext == NULL) {

            return (FALSE);
        }

        //
        //  Fill out the context.
        //  We set the TimeOut for this object to be ATQ_INFINITE,
        //   since we do not want any interference from the Timeout loop.
        //

        pAtqContext->InitWithDefaults(
                                      pEndpoint->IoCompletion,
                                      ATQ_INFINITE,
                                      (HANDLE)pEndpoint->ListenSocket
                                      );

        //
        //  These data members are used if we're doing AcceptEx processing
        //


        pAtqContext->SetAcceptExBuffer( NULL);

        //
        // Among AcceptEx ATQ Contexts,
        //  only the listen ATQ context will have the Endpoint field as NULL
        //
        pAtqContext->pEndpoint       = NULL;
        pAtqContext->SetFlag( ACF_ACCEPTEX_ROOT_CONTEXT );

        //
        // We set NextTimeout to INFINITE
        //  so the timeout thread will ignore this entry until an IO
        //  request is made unless this is an AcceptEx socket, that means
        //  we're about to submit the IO.

        DBG_ASSERT( g_fUseAcceptEx && pEndpoint->ConnectExCompletion != NULL);

        pAtqContext->InitAcceptExState( ATQ_INFINITE);

        *ppAtqContext = pAtqContext;
    }

    fReturn = I_AddAtqContextToPort( pAtqContext);

    return (fReturn);

} // I_AtqAddListenEndpointToPort()



BOOL
I_AtqAddAcceptExSocket(
    IN PATQ_ENDPOINT          pEndpoint,
    IN PATQ_CONT              pAtqContext
    )
/*++

Routine Description:

    Adds the AtqContext to the AcceptEx() waiters list,
    after allocating a new socket, since pAtqContext->hAsyncIO = NULL.

Arguments:

    pEndpoint - Information about this listenning socket
    patqReusedContext - optional context to use

Return Value:

    TRUE on success, FALSE on failure.
    On failure the caller should free the pAtqContext

--*/
{
    BOOL   fAddToPort = FALSE;
    BOOL   fSuccess = TRUE;

    DBG_ASSERT( pAtqContext != NULL);
    DBG_ASSERT( pAtqContext->pvBuff != NULL);
    DBG_ASSERT( !TsIsWindows95() );

    //
    //  If this listen socket isn't accepting new connections, just return
    //

    if ( !IS_BLOCK_ACTIVE(pEndpoint) ) {

        SetLastError( ERROR_NOT_READY );
        return ( FALSE);
    }

    //
    //  Use the supplied socket if any.
    //  Otherwise create a new socket
    //

    if ( pAtqContext->hAsyncIO == NULL) {

        HANDLE sAcceptSocket;

        sAcceptSocket = (HANDLE )
#if WINSOCK11
            socket(
                   AF_INET,
                   SOCK_STREAM,
                   IPPROTO_TCP
                   );
#else
        WSASocketW(
                   AF_INET,
                   SOCK_STREAM,
                   IPPROTO_TCP,
                   NULL,  // protocol info
                   0,     // Group ID = 0 => no constraints
                   (g_fUseFakeCompletionPort ?
                    0:
                    WSA_FLAG_OVERLAPPED // completion port notifications
                    )
                   );
#endif // WINSOCK11

        if ( (SOCKET ) sAcceptSocket == INVALID_SOCKET ) {

            fSuccess = FALSE;
            sAcceptSocket = NULL;

            //
            // no need to unlink from any list, since we did not add it to any
            //

        } else {

            //
            // Setup the accept ex socket in the atq context.
            //

            pAtqContext->hAsyncIO = sAcceptSocket;
            pAtqContext->hJraAsyncIO  = HandleToUlong(sAcceptSocket) | 0x80000000;
            fAddToPort = TRUE;
            DBG_ASSERT( fSuccess);
        }
    }

    if ( fSuccess) {

        DWORD        cbRecvd;

        if ( g_fShutdown) {

            //
            // no need to unlink from any list, since we did not add it to any
            //

            SetLastError( ERROR_NOT_READY);
            return (FALSE);
        }

        DBG_ASSERT( pAtqContext->hAsyncIO != NULL);

        //
        // 1. Call I_AtqAddAsyncHandleEx() to establish the links with
        //  proper AcceptEx & AtqContext processing lists.
        //
        //  After 1, the atqcontext will be in the lists, so
        //    cleanup should remove the context from proper lists.
        //
        // 2. Add the socket to Completion Port (if new),
        //    i.e. if fAddToPort is true)
        //
        // 3. Submit the new socket to AcceptEx() so that it may be
        //  used for processing about the new connections.
        //

        // 1.
        DBG_ASSERT( pAtqContext->m_nIO == 0);
        DBG_REQUIRE( pAtqContext->PrepareAcceptExContext(pEndpoint));

        // increment outstanding async io operations before AcceptEx() call
        InterlockedIncrement( &pAtqContext->m_nIO);

        // 2.
        if ( fAddToPort ) {
            ATQ_ASSERT(!pAtqContext->fDatagramContext);
            fSuccess = I_AddAtqContextToPort( pAtqContext);
        }
        // 3.
        if (fSuccess) {
            if (!pEndpoint->fDatagram) {
                 fSuccess = AcceptEx(
                                (SOCKET ) pEndpoint->ListenSocket,
                                (SOCKET ) pAtqContext->hAsyncIO,
                                pAtqContext->pvBuff,
                                pEndpoint->InitialRecvSize,
                                MIN_SOCKADDR_SIZE,
                                MIN_SOCKADDR_SIZE,
                                &cbRecvd,
                                &pAtqContext->Overlapped );

                 if (!fSuccess) {
                     fSuccess = (GetLastError() == ERROR_IO_PENDING);
                 }

            } else {

                WSABUF Buffer;
                DWORD lpFlags;
                int RetCode = 0;
                DWORD retry=0;

                //
                // For datagram, do a WSARecvFrom instead of AcceptEx.
                // All the output parameters have to be in the context
                // structure
                //

                for (;;) {

                    lpFlags = 0;
                    Buffer.len = pEndpoint->InitialRecvSize;
                    Buffer.buf = (char *) pAtqContext->pvBuff;

                    RetCode = WSARecvFrom(
                            (SOCKET) pAtqContext->hAsyncIO,
                            &Buffer,
                            1,                          // buffer count
                            &cbRecvd,
                            &lpFlags,
                            (PSOCKADDR) pAtqContext->AddressInformation,
                            &pAtqContext->AddressLength,
                            &pAtqContext->Overlapped,
                            NULL
                            );

                    //
                    // If this is a conn reset, then retry until we
                    // don't get this error anymore.  It looks like we 
                    // get this everytime someone sends us something and
                    // then shuts down the socket before we can respond.
                    // The sockets layer seems to give us a max of 160 of 
                    // these at a time, but for now retry infinitely since
                    // we don't know what this 160 is based on.
                    // RRandall - 7/16/99
                    //

                    if ( (RetCode == 0) || (WSAGetLastError() != WSAECONNRESET) ) {
                        break;
                    }

                    ATQ_PRINTF((DBG_CONTEXT,"AtqRecvFrom[%x] reset error. Retrying[%d]\n",
                                pAtqContext, retry));
                    retry++;
                }

                if (RetCode != 0) {

                    fSuccess = (WSAGetLastError() == ERROR_IO_PENDING);
                }
            }
        }

        if ( fSuccess) {

            //
            //  We've successfully added this socket, increment the count
            //

            InterlockedIncrement( &pEndpoint->nSocketsAvail );

        } else {

            ATQ_PRINTF(( DBG_CONTEXT,
                        "[AtqAddAcceptExSocket] Reusing an old context (%08x)"
                        " failed; error %d:%d, sAcceptSocket = %x, "
                        " pEndpoint = %lx, parm4 = %d, parm7 = %lx,"
                        " parm8 = %lx\n",
                        pAtqContext,
                        GetLastError(),
                        WSAGetLastError(),
                        pAtqContext->hAsyncIO,
                        pEndpoint,
                        pEndpoint->InitialRecvSize,
                        &cbRecvd,
                        &pAtqContext->Overlapped ));

            //
            // Unlink from the current list, where it was added as a result of
            //  step 1 above.
            //
            DBG_ASSERT( pAtqContext->ContextList != NULL);

            // balance the increment of the async operations outstanding
            DBG_REQUIRE( InterlockedDecrement( &pAtqContext->m_nIO) > 0);

            DBG_ASSERT( pAtqContext->m_leTimeout.Flink != NULL);
            pAtqContext->ContextList->
                RemoveFromList( &pAtqContext->m_leTimeout);

            //
            // balance the increment done
            // by pAtqContext->PrepareAcceptExContext()
            //
            DBG_REQUIRE( InterlockedDecrement( &pAtqContext->m_nIO) == 0);
            DBG_ASSERT( !fSuccess);

            //
            // the caller will free the Atq context on failure
            //
        }
    }

    return ( fSuccess);
} // I_AtqAddAcceptExSocket()




VOID
AtqpReuseContext( PATQ_CONT  pAtqContext)
/*++
  Description:
     This function attempts to reuse the ATQ context.
     It first cleans up the state and then uses the function
      I_AtqAddAccetpEx() socket to re-add the context to acceptex pool

  Arguments:
     pAtqContext - pointer to ATQ context that can be reused

  Returns:
     None
--*/
{
    PATQ_ENDPOINT pEndpoint = pAtqContext->pEndpoint;

    DBG_ASSERT( pEndpoint != NULL);
    DBG_ASSERT( pEndpoint->UseAcceptEx);

    //
    // Complete connection has been processed prior to coming here
    //

    DBG_ASSERT(pAtqContext->IsFlag( ACF_CONN_INDICATED));

    //
    // Remove from the current active list
    //
    if ( pAtqContext->m_leTimeout.Flink != NULL ) {
        pAtqContext->ContextList->RemoveFromList( &pAtqContext->m_leTimeout );
    }

    DBG_ASSERT( pAtqContext->m_leTimeout.Flink == NULL);
    DBG_ASSERT( pAtqContext->m_leTimeout.Blink == NULL);

    DBG_ASSERT( pEndpoint->Signature == ATQ_ENDPOINT_SIGNATURE );

        //
        //  Either there is no socket or the socket must be in the
        //  unconnected state (meaning reused after TransmitFile)
        //


    DBG_ASSERT( !pAtqContext->hAsyncIO ||
                (  pAtqContext->hAsyncIO &&
                  (  pAtqContext->fDatagramContext ||
                     pAtqContext->IsState( ACS_SOCK_UNCONNECTED |
                                           ACS_SOCK_TOBE_FREED)
                  )
                )
              );

    //
    // !! DS
    //

    if ( pAtqContext->fDatagramContext ) {

        DBG_ASSERT(pEndpoint->fDatagram);
        pAtqContext->hAsyncIO = (HANDLE)pEndpoint->ListenSocket;
    }

    //
    // I need to make sure that the state information is cleaned up
    //  before re-adding the context to the list
    //

    if ( !I_AtqAddAcceptExSocket(pEndpoint, pAtqContext) ) {

        //
        //  Failed to add the socket, free up the context without reuse
        //

        ATQ_PRINTF(( DBG_CONTEXT,
                     "[AtqpReuseContext] for (%08x) failed with "
                     " Error = %d;  Now freeing the context ...\n",
                     pAtqContext, GetLastError()
                     ));

        DBG_ASSERT( pAtqContext->m_nIO == 0);

        // free without reuse
        pAtqContext->CleanupAndRelease();
    }

    return;
} // AtqpReuseContext()



VOID
AtqpReuseOrFreeContext(
    PATQ_CONT    pAtqContext,
    BOOL         fReuseContext
    )
/*++
  Routine Description:
     This function does a free-up of the ATQ contexts. During the free-up
     path, we also attempt to reuse the ATQ context if the fReuseContext is
     set.

  Arguments:
     pAtqContext - pointer to the ATQ context that needs to be freedup
     fReuseContext - BOOLEAN flag indicating if this context should be reused

  Returns:
     None
--*/
{
    //
    // Get this object out of the Blocked Requests List.
    //

    if ( pAtqContext->IsBlocked()) {
        ATQ_REQUIRE( pAtqContext->m_pBandwidthInfo
                      ->RemoveFromBlockedList( pAtqContext ));
        DBG_ASSERT( !pAtqContext->IsBlocked());
    }

    DBG_ASSERT( pAtqContext->m_pBandwidthInfo != NULL);
    pAtqContext->m_pBandwidthInfo->Dereference();

    //
    //  Conditions for Reuse:
    //   1) fReuseContext == TRUE => caller wants us to reuse context
    //   2) pAtqContext->pEndpoint != NULL => valid endpoint exists
    //   3)  pEndpoint->UseAcceptEx => AcceptEx is enabled
    //   4)  pEndpoint->nSocketsAvail < nAcceptExOutstanding * 2 =>
    //           We do not have lots of outstanding idle sockets
    //       Condition (4) ensures that we do not flood the system
    //         with too many AcceptEx sockets as a result of some spike.
    //       AcceptEx sockets once added to the pool are hard to
    //         remove, because of various timing problems.
    //       Hence we want to prevent arbitrarily adding AcceptEx sockets.
    //
    //    In condition (4) I use a fudge factor of "2", so that
    //     we do continue to prevent reuse of sockets prematurely.
    //

    if ( fReuseContext &&
         (pAtqContext->pEndpoint != NULL)  &&
         (pAtqContext->pEndpoint->UseAcceptEx)
#ifdef ENABLE_REUSE_THROTTLE
         //
         // Reuse Throttle causes trouble in bursty traffic mode
         // especially with benchmarks like SpecWeb96
         //
         &&
         ( g_fAlwaysReuseSockets ||
           ((DWORD )pAtqContext->pEndpoint->nSocketsAvail <
            pAtqContext->pEndpoint->nAcceptExOutstanding * 2)
           )
#endif // ENABLE_REUSE_THROTTLE
         ) {

        //
        // Call the function to reuse context. On failure
        // the AtqpReuseContext will free up the context
        //

        AcIncrement( CacAtqContextsReused);

        AtqpReuseContext( pAtqContext);

    } else {

        AcIncrement( CacAtqContextsCleanedup);

        pAtqContext->CleanupAndRelease();

    }

    return;
} // AtqpReuseOrFreeContext()



BOOL
I_AtqPrepareAcceptExSockets(
    IN PATQ_ENDPOINT          pEndpoint,
    IN DWORD                  nSockets
    )
/*++

Routine Description:

    Prepare specified number of AcceptEx sockets for the given
      ListenSocket in [pEndpoint]

Arguments:

    pEndpoint - Information about this listenning socket
    nSockets    - number of AcceptEx() sockets to be created.

Return Value:

    TRUE on success, FALSE on failure.

--*/
{
    BOOL   fReturn;
    DWORD  cbBuffer;
    DWORD  i;

    if ( !g_fUseAcceptEx ) {
        SetLastError( ERROR_NOT_SUPPORTED );
        return FALSE;
    }

    //
    //  If this listen socket isn't accepting new connections, just return
    //

    if ( pEndpoint->State != AtqStateActive ) {
        SetLastError( ERROR_NOT_READY );
        return(FALSE);
    }

    if ( pEndpoint->fAddingSockets) {
        //
        // Someone is already adding sockets. Do not add more
        // Just return success
        //
        return ( TRUE);
    }

    pEndpoint->fAddingSockets = TRUE;

    // calculate the buffer size
    cbBuffer = pEndpoint->InitialRecvSize + 2* MIN_SOCKADDR_SIZE;

    for ( fReturn = TRUE, i = 0 ; fReturn && i++ < nSockets; ) {

        PVOID        pvBuff;
        PATQ_CONT    pAtqContext;

        //
        //  Alloc a buffer for receive data
        //  TBD: Pool all these buffers into one large buffer.
        //

        pvBuff = LocalAlloc( LPTR, cbBuffer);

        //
        //  Get the ATQ context now because we need its overlapped structure
        //

        pAtqContext = I_AtqAllocContextFromCache();


        //
        // Now check if allocations are valid and do proper cleanup on failure
        //

        if ( pvBuff == NULL || pAtqContext == NULL) {

            if ( pvBuff ) {
                LocalFree( pvBuff );
                pvBuff = NULL;
            }

            if ( pAtqContext ) {
                I_AtqFreeContextToCache( pAtqContext );
                pAtqContext = NULL;
            }

            SetLastError( ERROR_NOT_ENOUGH_MEMORY );
            fReturn = FALSE;
            break;
        } else {

            //
            // Add this socket to AtqContext lists & completion ports
            // From now on the called function will take care of freeing up
            //  pAtqContext, if there is a failure.
            //

            pAtqContext->SetAcceptExBuffer( pvBuff);
            if ( pEndpoint->fDatagram ) {
                pAtqContext->hAsyncIO = (HANDLE)pEndpoint->ListenSocket;
            } else {
                pAtqContext->hAsyncIO = NULL;
            }
            pAtqContext->hJraAsyncIO = 0;

            if ( !pEndpoint->fDatagram ) {
            
                if ( !I_AtqAddAcceptExSocket(pEndpoint, pAtqContext) ) {

                    //
                    //  Failed to add the socket, free up the context without reuse
                    //

                    ATQ_PRINTF(( DBG_CONTEXT,
                                "[I_AtqPrepareAcceptExSockets] for Endpoint %08x"
                                " and AtqContext (%08x) failed with "
                                " Error = %d;  Now freeing the context ...",
                                pEndpoint, pAtqContext, GetLastError()
                                ));

                    // free without reuse
                    DBG_ASSERT( pAtqContext->m_nIO == 0);
                    pAtqContext->CleanupAndRelease();
                    fReturn = FALSE;
                }

            } else {
                while ( !I_AtqAddAcceptExSocket(pEndpoint, pAtqContext) ) {
                    // UDP is more fragile than TCP, so keep trying.  If
                    // we give up, then we might run out of posted receives,
                    // and there's no way to recover from that.
                    Sleep(100);
                }
            }
        }
    } // for

    //
    // Finished Adding sockets. Indicate that by resetting the flab
    //

    pEndpoint->fAddingSockets = FALSE;

    ATQ_PRINTF(( DBG_CONTEXT,
                "PrepareAcceptExSockets( Endpoint[%08x], nSockets = %d)==>"
                " avail = %d; Total Refs = %d.\n",
                pEndpoint,
                nSockets,
                pEndpoint->nSocketsAvail,
                pEndpoint->m_refCount
                ));

    return ( fReturn);

} // I_AtqPrepareAcceptExSockets()


BOOL
I_AtqStartThreadMonitor(
    VOID
    )
/*++

Routine Description:

    Adds the ThreadMonitor function to the scheduler.
    
    Note: The scheduler should be initialized before getting to this function.

Arguments:

    None.

Return Value:

    TRUE if successful, FALSE on error 

--*/
{

    g_dwThreadMonitorCookie =
        ScheduleWorkItem(
                         (PFN_SCHED_CALLBACK)I_AtqThreadMonitor,
                         NULL, // No need for a context
                         TimeToWait(ATQ_THREAD_MONITOR_PERIOD),
                         TRUE  // ask for periodic timeout
                         );

    if ( g_dwThreadMonitorCookie == 0 ) {

        ATQ_PRINTF(( DBG_CONTEXT,
                     "Error %d scheduling ThreadMonitor\n",GetLastError()));
        return(FALSE);
    }

    return(TRUE);
}



BOOL
I_AtqStopThreadMonitor(
    VOID
    )
/*++

Routine Description:

    Removes the ThreadMonitor function from the scheduler.
    
    Note: The scheduler should be initialized before getting to this function.

Arguments:

    None.

Return Value:

    TRUE if successful, FALSE on error 

--*/
{
    if ( 0 != g_dwThreadMonitorCookie) {
        DBG_REQUIRE( RemoveWorkItem( g_dwThreadMonitorCookie ));
        g_dwThreadMonitorCookie = 0;
    }

    return ( TRUE);
}


VOID
I_AtqThreadMonitor(
    IN PVOID NotUsed
    )
/*++

Routine Description:

    Checks to see whether the AtqPoolThreads have stalled out for longer than
    ATQ_THREAD_MONITOR_PERIOD, and if so starts up a temporary AtqPoolThread.
    This is to prevent deadlocks due to all the atq threads being used by
    processes that are trying to call a network service on this same machine
    that also used atq.

Arguments:

    NotUsed - This argument is only here because sheduler callback functions
    require a PVOID argument.  It is not actually used in the body of the
    function.

Return Value:

    None.

--*/
{
    HANDLE  hThread;
    DWORD   dwThreadID;

    if (!g_fDeadManSwitch || 0 != g_cAvailableThreads) {
        // The switch has been cleared since
        // the last time or there are still available threads, the AtqPoolThreads
        // must not be stuck.
        g_fDeadManSwitch = TRUE;
        return;
    }

    ATQ_ASSERT( ATQ_REG_MAX_POOL_THREAD_LIMIT > g_cThreads );

    if ( ATQ_REG_MAX_POOL_THREAD_LIMIT > g_cThreads ) { 
        // There are no free threads and there hasn't been any
        // for ATQ_THREAD_MONITOR_PERIOD.  Start up a temporary
        // thread to service calls until hopefully the rest of 
        // the threads make some progress.
        ATQ_PRINTF(( DBG_CONTEXT, "AtqThreadMonitor: Starting a new thread.\n"));

        InterlockedIncrement( &g_cThreads );

        hThread = CreateThread( NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)AtqPoolThread,
                                (PVOID)UIntToPtr(ATQ_TEMP_THREAD),  // Let the thread know that it is temporary.
                                0,
                                &dwThreadID );

        if ( hThread ) {
            CloseHandle( hThread );     // Free system resources
        } else {
            InterlockedDecrement( &g_cThreads );
            ATQ_PRINTF(( DBG_CONTEXT,
                "AtqThreadMonitor: Unable to create a temp thread.\n"));                    
        }
    } 

}

