/*++

   Copyright    (c)    1996    Microsoft Corporation

   Module  Name :

      atqendp.cxx

   Abstract:
      This module implements ATQ endpoints

   Author:

       Johnson Apacible    (JohnsonA)     13-May-1996

   Environment:

       User Mode -- Win32

--*/

#include "isatq.hxx"

//
// Forward declarations
//

BOOL
I_CreateListenSocket(
    IN PATQ_ENDPOINT Endpoint
    );

BOOL
I_CloseListenSocket(
    IN PATQ_ENDPOINT Endpoint
    );

BOOL
StartListenThread(
    IN PATQ_ENDPOINT Endpoint
    );

extern DWORD g_cCPU;

# define ATQ_MIN_ACCEPTEX_TIMEOUT    (120)  // 2 minutes = 120 seconds

#define ATQ_CLOSE_ENDPOINT_SLEEP_TIME (200) // 200ms = 1/5 second
#define ATQ_CLOSE_ENDPOINT_TIMEOUT    ((100 * 1000) / ATQ_CLOSE_ENDPOINT_SLEEP_TIME)
                                            // 100 seconds

#define ATQ_MIN_ACCEPTEX_SOCKETS         (5)
#define ATQ_MAX_ACCEPTEX_SOCKETS         (150) // just seems like a lot.


PVOID
AtqCreateEndpoint(
    IN PATQ_ENDPOINT_CONFIGURATION Configuration,
    IN PVOID EndpointContext
    )

/*++

Routine Description:

    Creates a server instance.

Arguments:

    Context - Context value returned

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/

{

    DWORD timeout = Configuration->AcceptExTimeout;
    PATQ_ENDPOINT endpoint;
    IF_DEBUG(API_ENTRY) {
        ATQ_PRINTF((DBG_CONTEXT,"AtqCreateEndpoint entered\n"));
    }

    //
    // Allocate list
    //

    endpoint = (PATQ_ENDPOINT)LocalAlloc(0,sizeof(ATQ_ENDPOINT));
    if ( endpoint == NULL ) {
        ATQ_PRINTF(( DBG_CONTEXT,"Unable to allocate ATQ Endpoint\n"));
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        goto error;
    }

    //
    // Initialize
    //

    ZeroMemory(endpoint,sizeof(ATQ_ENDPOINT));
    endpoint->Signature = ATQ_ENDPOINT_SIGNATURE;
    endpoint->m_refCount = 1;
    SET_BLOCK_STATE(endpoint, AtqStateActive);

    endpoint->EnableBw = FALSE;
    endpoint->ConnectCompletion = Configuration->pfnConnect;
    endpoint->ConnectExCompletion = Configuration->pfnConnectEx;
    endpoint->IoCompletion = Configuration->pfnIoCompletion;
    endpoint->fDatagram = Configuration->fDatagram;
    endpoint->ListenSocket = INVALID_SOCKET;
    endpoint->pListenAtqContext = NULL;
    endpoint->nAvailDuringTimeOut = 0;
    endpoint->nSocketsAvail = 0;
    endpoint->Context = EndpointContext;
    endpoint->fExclusive = Configuration->fLockDownPort;
    endpoint->fReverseQueuing = Configuration->fReverseQueuing;
    endpoint->cbDatagramWSBufSize = Configuration->cbDatagramWSBufSize;
    
    endpoint->InitialRecvSize = Configuration->cbAcceptExRecvBuffer;

    // we need to maintain at least 5 outstanding accept ex sockets
    // for our auto-tune algo to work.
    endpoint->nAcceptExOutstanding = 
        min((g_cCPU * (max(ATQ_MIN_ACCEPTEX_SOCKETS, Configuration->nAcceptExOutstanding))),
             ATQ_MAX_ACCEPTEX_SOCKETS);

    endpoint->fAddingSockets = FALSE;

    //
    // Check and set the timeout to be atleast minimum timeout for AcceptEx
    //
    if ( timeout <= ATQ_MIN_ACCEPTEX_TIMEOUT)  {
        timeout = ATQ_MIN_ACCEPTEX_TIMEOUT;
    }

    endpoint->AcceptExTimeout = CanonTimeout( timeout);

    endpoint->Port = Configuration->ListenPort;
    endpoint->IpAddress = Configuration->IpAddress;
    endpoint->ConsumerType = AtqConsumerOther;

    //endpoint->ContextList.Initialize( );

#if DBG
    endpoint->RefTraceLog = CreateRefTraceLog( TRACE_LOG_SIZE, 0 );
#endif

#if 0
    ATQ_PRINTF(( DBG_CONTEXT,"port %d nAX %d nAT %d nLB %d\n",
        endpoint->Port, endpoint->nAcceptExOutstanding,
        endpoint->AcceptExTimeout, g_cListenBacklog));
#endif

    //
    // Create the socket
    //

    if (!I_CreateListenSocket(endpoint) ) {

        goto error;
    }

    return((PVOID)endpoint);

error:

    if ( endpoint != NULL ) {
#if DBG
        if( endpoint->RefTraceLog != NULL ) {
            DestroyRefTraceLog( endpoint->RefTraceLog );
        }
#endif
        LocalFree( endpoint );
    }

    return(NULL);

} // AtqCreateEndpoint



BOOL
AtqStartEndpoint(
    IN PVOID Endpoint
    )
{
    BOOL useAcceptEx;
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    BOOL fReturn;

    ATQ_ASSERT(IS_BLOCK_ACTIVE(pEndpoint));

    pEndpoint->UseAcceptEx = useAcceptEx =
        ((g_fUseAcceptEx) && (pEndpoint->ConnectExCompletion != NULL)) ||
        pEndpoint->fDatagram;

    IF_DEBUG(API_ENTRY) {
        ATQ_PRINTF((DBG_CONTEXT,"AtqStartEndpoint called. UseAcceptEx[%d]\n",
            useAcceptEx));
    }

    //
    // if AcceptEx is supported, create AcceptEx contexts
    //

    if ( useAcceptEx ) {

        //
        // Add AcceptEx sockets
        //

        fReturn = pEndpoint->ActivateEndpoint();

        if ( !fReturn ) {
            DBGPRINTF(( DBG_CONTEXT,"Error %d in %08x::ActivateEndpoint()\n",
                GetLastError(), pEndpoint));
        }

    } else {

        //
        // We need to start a listen thread
        //

        fReturn = StartListenThread( pEndpoint );

        if ( !fReturn ) {
            DBGPRINTF(( DBG_CONTEXT,"Error %d in %08x::StartListenThread()\n",
                GetLastError(), pEndpoint));
        }
    }

    return (fReturn);

} // AtqStartEndpoint



DWORD_PTR
AtqEndpointGetInfo(
    IN PVOID Endpoint,
    IN ATQ_ENDPOINT_INFO EndpointInfo
    )
/*++

Routine Description:

    Gets various bits of information for the ATQ endpoint

Arguments:

    Endpoint    - endpoint to get data from
    EndpointInfo - type of info to get

Return Value:

    The old value of the parameter

--*/
{
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    DWORD_PTR     dwVal = 0;

    switch ( EndpointInfo ) {

      case EndpointInfoListenPort:
        dwVal = pEndpoint->Port;
        break;

      case EndpointInfoListenSocket:
        dwVal = pEndpoint->ListenSocket;
        break;

      default:
        ATQ_ASSERT( FALSE );
    }

    return dwVal;
} // AtqEndpointGetInfo()



DWORD_PTR
AtqEndpointSetInfo(
    IN PVOID                Endpoint,
    IN ATQ_ENDPOINT_INFO    EndpointInfo,
    IN DWORD_PTR            dwInfo
    )
/*++

Routine Description:

    Gets various bits of information for the ATQ module

Arguments:

    Endpoint    - endpoint to set info on
    EndpointInfo - type of info to set
    dwInfo       - info to set

Return Value:

    The old value of the parameter

--*/
{
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    DWORD_PTR     dwVal = 0;

    switch ( EndpointInfo ) {

        case EndpointInfoAcceptExOutstanding:

        dwVal = pEndpoint->nAcceptExOutstanding;

        if ( dwVal < dwInfo ) {

            //
            // Make up for increased limit
            //

            if ( (DWORD ) pEndpoint->nSocketsAvail < (dwInfo >> 2) ) {
                (VOID ) I_AtqPrepareAcceptExSockets(
                                        pEndpoint,
                                        ((DWORD)dwInfo>>2) - pEndpoint->nSocketsAvail
                                        );
            }
            pEndpoint->nAcceptExOutstanding = (DWORD)dwInfo;
        }
        break;

      default:
        ATQ_ASSERT( FALSE );
    }

    return dwVal;
} // AtqEndpointSetInfo()




BOOL
AtqStopEndpoint(
    IN PVOID     Endpoint
    )
/*++

Routine Description:

    Stops the endpoint - marks the Endpoint as to be shutdown and closes
    the listening socket -> forcing new connections to stop for this endpoint

Arguments:

    Endpoint - endpoint to be stopped

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/
{
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    DWORD  nClosed;

    IF_DEBUG( API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqStopEndpoint( %08x)\n", pEndpoint));
    }

    //
    //  Find the listen socket info
    //

    AcquireLock( &AtqEndpointLock );

    if ( !IS_BLOCK_ACTIVE(pEndpoint) ) {

        ATQ_PRINTF(( DBG_CONTEXT,
                     "Attempt to Stop Endpoint (%08x) more than once",
                     pEndpoint
                     ));
        ReleaseLock( &AtqEndpointLock );
        return(FALSE);
    }

    //
    //  Mark the listen info as no longer accepting connections
    //

    SET_BLOCK_STATE(pEndpoint, AtqStateClosed);

    //
    //  Remove the entry from the end points list
    //

    RemoveEntryList(&pEndpoint->ListEntry);
    ReleaseLock( &AtqEndpointLock );

    //
    // Close the listen socket which forces the cleanup for all the
    //  pending LISTEN ATQ contexts. We do this early on so that
    //  we can prevent any new entrant connections into the processing code.
    //

    I_CloseListenSocket( pEndpoint );

    //
    // Forcibly close all the pending LISTEN contexts tied to this endpoint
    //

    nClosed = pEndpoint->CloseAtqContexts( TRUE);

    DBGPRINTF(( DBG_CONTEXT,
                "ATQ_ENDPOINT(%08x)::Closed %d pending Listen sockets\n",
                pEndpoint, nClosed));

    //
    // if this is a non-acceptex socket, wait for the listen thread to die
    //

    if ( !pEndpoint->UseAcceptEx ) {
        WaitForSingleObject(pEndpoint->hListenThread, 10*1000);
    }

    return ( TRUE);

} // AtqStopEndpoint()


BOOL
AtqCloseEndpoint(
    IN PVOID     Endpoint
    )
/*++

Routine Description:

    Closes the endpoint - it forcefully fress up all references to the
    endpoint (held by ATQ Contexts) by shutting down the ATQ contexts.
    In the course of this operation if some context does not go away, this
    code will end up looping forever!

    Note: Should be called *after* AtqStopEndpoint()

Arguments:

    Endpoint - endpoint to be stopped

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/
{
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    DWORD  nClosed;
    DWORD i;

    ASSERT( pEndpoint->Signature == ATQ_ENDPOINT_SIGNATURE );


    IF_DEBUG( API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqCloseEndpoint( %08x)\n", pEndpoint));
    }

    if ( pEndpoint->State != AtqStateClosed) {

        ATQ_PRINTF(( DBG_CONTEXT,
                     "Attempt to Close Endpoint (%08x) when it is "
                     " not stopped yet!\n'",
                     pEndpoint
                     ));
        return(FALSE);
    }

    //
    // wait for all the contexts for this endpoint to go away
    // or for about two minutes
    //

    i = 0;
    while (( pEndpoint->m_refCount > 1) && (i < ATQ_CLOSE_ENDPOINT_TIMEOUT)) {

        ATQ_PRINTF(( DBG_CONTEXT, " Endpoint(%08x) has %d refs\n",
                     pEndpoint, pEndpoint->m_refCount));

        //
        // Forcibly close all the contexts tied to this endpoint again!
        // Sometimes for some random reasons ATQ contexts get left out
        //  during the first clean we did above. In such case it is important
        //  to retry again
        // THIS IS UGLY. But if we did not do this then the Endpoint
        //  structure might get freed => ATQ contexts will be hanging on to
        //  dead ATQ endpoint
        //

        nClosed = pEndpoint->CloseAtqContexts();

        DBGPRINTF(( DBG_CONTEXT, " ATQ_ENDPOINT(%08x)::Closed %d sockets\n",
                    pEndpoint, nClosed));

        //
        // NYI: I need to auto-tune this sleep function
        //
        Sleep( ATQ_CLOSE_ENDPOINT_SLEEP_TIME); // sleep and check again.

#if DBG
        //
        // loop forever for checked builds
        //
#else
        //
        // loop until timeout for retail
        //
        i++;
#endif

        // wake up and check again.
    } // while (busy wait)

    //
    //  Undo the reference for being on the listen info list.
    //    decr final ref count => the endpoint will be cleaned up & freed
    //
    //  If we timed out just leak the endpoints!
    //

    if ( pEndpoint->m_refCount == 1 ) {
        pEndpoint->Dereference();

        return TRUE;
    } else {
        return FALSE;
    }

} // AtqCloseEndpoint()



BOOL
AtqStopAndCloseEndpoint(
    IN PVOID                    Endpoint,
    IN LPTHREAD_START_ROUTINE   lpCompletion,
    IN PVOID                    lpCompletionContext
    )
/*++

Routine Description:

    Stops the endpoint and closes it after forcing close of
    associated ATQ contexts.

Arguments:

    Endpoint - endpoint to shutdown.
    lpCompletion - routine to be called when endpoint is completely shutdown.
    lpCompletionContext - Context to be returned with the routine

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/
{
    PATQ_ENDPOINT pEndpoint = (PATQ_ENDPOINT)Endpoint;
    BOOL fReturn;

    //
    // Warn all the callers of this to be deprecated API and pray that
    // they will all switch over
    //
    OutputDebugStringA( "\n-----------------------------------------------\n");
    OutputDebugStringA( " AtqStopAndCloseEndpoint() should NOT be called\n");
    OutputDebugStringA( "         Call 1) AtqStopEndpoint()  and \n");
    OutputDebugStringA( "              2) AtqCloseEndpoint() instead\n");
    OutputDebugStringA( "   For Now, this call will simulate 1 & 2\n");
    OutputDebugStringA( "-----------------------------------------------\n");


    IF_DEBUG( API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqStopAndCloseEndpoint( %08x)\n", pEndpoint));
    }

    fReturn = AtqStopEndpoint( Endpoint);

    if ( fReturn) {

        //
        // Call any custom shutdown function
        // NYI: Too Bad the Endpoint object is not a base class object
        //

        if ( lpCompletion != NULL ) {
            pEndpoint->ShutdownCallback = lpCompletion;
            pEndpoint->ShutdownCallbackContext = lpCompletionContext;
        }

        //
        // Now that the Endpoint is stopped and callback functions are called,
        //  Let us call the AtqCloseEndpoint() to cleanup the endpoint itself.
        //
        fReturn = AtqCloseEndpoint( Endpoint);
    }

    return (fReturn);

} // AtqStopAndCloseEndpoint()



BOOL
ATQ_ENDPOINT::ActivateEndpoint( VOID)
/*++

Routine Description:


    This function creates the initial listening socket & ATQ context for given
    endpoint. It also adds initial set of AcceptEx Sockets to the ATQ listening
    pool (if we are using the AcceptEx())

Arguments:
    None

Return Value:

    TRUE on success, FALSE on failure

--*/
{
    PATQ_CONT   patqContext = NULL;
    BOOL        fReturn;
    DWORD       cInitial = this->nAcceptExOutstanding;

    //
    //  Add the listen socket
    //
    DBG_ASSERT( this->pListenAtqContext == NULL);

    fReturn =
        I_AtqAddListenEndpointToPort(
                                     (PATQ_CONT*)&this->pListenAtqContext,
                                     this
                                     );

    if ( !fReturn) {

        if ( this->pListenAtqContext ) {
            AtqFreeContext( this->pListenAtqContext, FALSE);
            this->pListenAtqContext = NULL;
        }

        return FALSE;
    }

    cInitial = max(cInitial, 1);

    if ( !TsIsNtServer( ) ) {

        //
        // Limit what a workstation can specify
        //

        cInitial = min(cInitial, ATQ_MIN_CTX_INC);
        this->nAcceptExOutstanding = cInitial;

    }

    //
    // start with 1/4 of the intended
    //

    cInitial = max( cInitial >> 2, 1);

    //
    //  Now add the acceptex sockets for this ListenInfo object
    //

    return(I_AtqPrepareAcceptExSockets(this, cInitial));

} // ATQ_ENDPOINT::ActivateEndpoint()



DWORD
ATQ_ENDPOINT::CloseAtqContexts( IN BOOL fPendingOnly)
/*++
  Description:
    This function searches for all ATQ contexts associated
     with the given endpoint and forcibly closes them all.

  Arguments:
    fPendingOnly - close only the pending sockets

  Returns:
    DWORD containing the number of ATQ contexts closed.
--*/
{
    DWORD  nClosed = 0;
    DWORD  i;
    PLIST_ENTRY   pEntry;
    PATQ_CONT     pContext;

    //
    //  Force a close on all the connected sockets so that all the holders
    //   and use of such contexts will bail out of this endpoint entirely.
    // NYI: We need a way to tag on all these lists on per-endpoint basis
    //

    for ( i = 0; i < g_dwNumContextLists; i++) {

        PLIST_ENTRY pListHead;

        AtqActiveContextList[i].Lock();

        //
        // Hard close sockets in the pending list
        //

        pListHead = &AtqActiveContextList[i].PendingAcceptExListHead;
        for ( pEntry = pListHead->Flink;
             pEntry != pListHead;
             pEntry  = pEntry->Flink ) {

            pContext = CONTAINING_RECORD( pEntry, ATQ_CONTEXT, m_leTimeout );

            ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );

            if ( (pContext->pEndpoint == this) &&
                 (pContext->IsState( ACS_SOCK_CONNECTED) ||
                  pContext->IsState( ACS_SOCK_LISTENING)
                  ) &&
                 (pContext->hAsyncIO  != NULL) ) {

                nClosed++;
                pContext->HardCloseSocket();
            }
        } // for items in pending list

        if ( !fPendingOnly) {
            //
            // Hard close sockets in the active list
            // Active list includes sockets in ACS_SOCK_CLOSED state
            //  that ought to be freed up, because we could have reached
            //  this through the optimizations for TransmitFile()
            //

            pListHead = &AtqActiveContextList[i].ActiveListHead;
            for ( pEntry = pListHead->Flink;
                  pEntry != pListHead;
                  pEntry  = pEntry->Flink ) {

                pContext = CONTAINING_RECORD( pEntry, ATQ_CONTEXT,
                                              m_leTimeout );

                ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );

                if ( (pContext->pEndpoint == this)  &&
                     (pContext->IsState( ACS_SOCK_CONNECTED) ||
                      pContext->IsState( ACS_SOCK_LISTENING) ||
                      pContext->IsState( ACS_SOCK_CLOSED) ||
                      pContext->IsState( ACS_SOCK_UNCONNECTED)
                      ) &&
                     (pContext->hAsyncIO  != NULL) ) {

                    nClosed++;
                    pContext->HardCloseSocket();
                }
            } // for items in active list
        } // if (! fPendingOnly)

        AtqActiveContextList[i].Unlock();

    } // for

    return ( nClosed);

} // ATQ_ENDPOINT::CloseAtqContexts()


/************************************************************
 * Internal Functions
 ************************************************************/


BOOL
I_CreateListenSocket(
    IN PATQ_ENDPOINT pEndpoint
    )
/*++

    Creates a socket for listening to connections on given address.

    Arguments:

       lpSockAddress    pointer to local socket address structure used to bind
                           the given connection.
       lenSockAddress   length of the socket address structure.
       socketType       integer containing the type of the socket ( stream )
       socketProtocol   protocol to be used for the socket.
       nBackLog         Maximum length to which a queue of pending connections
                           may grow.

    Returns:
       NO_ERROR on success; otherwise returns Sockets error code.

--*/
{
    INT serr;
    SOCKET  sNew;
    SOCKADDR_IN inAddr;
    PSOCKADDR addr;
    INT addrLength;
    PLIST_ENTRY listEntry;
    PATQ_ENDPOINT scanEndpoint;
    DWORD     cbOut;

    IF_DEBUG(API_ENTRY) {
        ATQ_PRINTF((DBG_CONTEXT,"I_CreateListenEndpoint called. \n"));
    }

    //
    // Create a new socket
    //

#if WINSOCK11
    sNew =  socket(
                AF_INET,
                SOCK_STREAM,
                IPPROTO_TCP
                );
#else
    sNew = WSASocket(
                  AF_INET,
                  pEndpoint->fDatagram ? SOCK_DGRAM : SOCK_STREAM,
                  pEndpoint->fDatagram ? IPPROTO_UDP: IPPROTO_TCP,
                  NULL,  // protocol info
                  0,     // Group ID = 0 => no constraints
                  WSA_FLAG_OVERLAPPED    // completion port notifications
                  );
# endif // WINSOCK11

    if ( sNew == INVALID_SOCKET ) {
        serr = WSAGetLastError();
        ATQ_PRINTF(( DBG_CONTEXT,
                    "Error %d in socket( %d, %d, %d)\n",
                    serr,
                    AF_INET,
                    SOCK_STREAM,
                    IPPROTO_TCP
                    ));

        goto cleanup;
    }

    //
    // Set EXCLUSIVE use if needed
    //

    if ( pEndpoint->fExclusive ) {
        ATQ_PRINTF(( DBG_CONTEXT,
                    " Setting %s port %d to exclusive access\n",
                    pEndpoint->fDatagram ? "UDP" : "TCP",
                    pEndpoint->Port));

        if ( setsockopt( sNew, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
                        (const CHAR *) &pEndpoint->fExclusive,
                        sizeof( pEndpoint->fExclusive )) != 0) {

            serr = WSAGetLastError();

            ATQ_PRINTF(( DBG_CONTEXT,
                        " setsockopt( %d, EXCLUSIVEADDR, FALSE) failed."
                        " Error = %d\n",
                        sNew, serr));
            serr = 0;
        }
    }

    if ( pEndpoint->fDatagram ) {

        //
        // ReverseQueuing means that if winsock runs out of buffer space for
        // receives then it will drop the oldest datagrams first.  The default 
        // is for it to drop the newest datagrams.
        //
        if ( pEndpoint->fReverseQueuing
            && WSAIoctl(sNew,                          // socket
                        SIO_ENABLE_CIRCULAR_QUEUEING,  // IoControlCode
                        NULL,                          // pvInBuffer
                        0,                             // cbInBuffer
                        NULL,                          // pvOutBuffer
                        0,                             // cbOutBuffer
                        &cbOut,                        // cbActualBytesReturned
                        NULL,                          // pvOverlapped
                        NULL)) {                       // pCompletionRoutine

            serr = WSAGetLastError();

            ATQ_PRINTF(( DBG_CONTEXT,
                         " WSAIoctl( %d, SIO_ENABLE_CIRCULAR_QUEUEING...) failed.  Error = 0x%x\n",
                         sNew, serr));

            serr = 0;
        }

        //
        // If they have specified a winsock receive buffer size use that instead
        // the winsock default of 8K.
        //
        if ( pEndpoint->cbDatagramWSBufSize
             && setsockopt(sNew,
                           SOL_SOCKET,
                           SO_RCVBUF,
                           (char *)&pEndpoint->cbDatagramWSBufSize,
                           sizeof(pEndpoint->cbDatagramWSBufSize))) {
            
            serr = WSAGetLastError();

            ATQ_PRINTF(( DBG_CONTEXT,
                         " setsockopt( %d, SO_RECVBUF, %d...) failed.  Error = 0x%x\n",
                         sNew, pEndpoint->cbDatagramWSBufSize, serr));

            serr = 0;
        }
    }
    
    //
    // See which address family we're dealing with
    //

    addr = (PSOCKADDR)&inAddr;
    addrLength = sizeof(inAddr);
    ZeroMemory(addr, addrLength);

    inAddr.sin_family = AF_INET;
    inAddr.sin_port = htons(pEndpoint->Port);
    inAddr.sin_addr.s_addr = pEndpoint->IpAddress;

    //
    // Bind an address to socket
    //

    if ( bind( sNew, addr, addrLength) != 0) {

        serr = WSAGetLastError();

        ATQ_PRINTF(( DBG_CONTEXT,
                    "bind ( socket = %d, Address = %08x, len = %d) "
                    " returns error = %u\n",
                    sNew, addr, addrLength, serr));

        goto cleanup;
    }

    //
    // Put the socket in listen mode
    //

    if ( !pEndpoint->fDatagram ) {

        if ( listen( sNew, g_cListenBacklog) != 0) {

            serr = WSAGetLastError();

            ATQ_PRINTF(( DBG_CONTEXT,
                        " listen( %d, %d) returned %d.\n",
                        sNew, g_cListenBacklog, serr));
            goto cleanup;
        }
    }
    pEndpoint->ListenSocket = sNew;

    //
    // Link to server listen list
    //

    AcquireLock( &AtqEndpointLock);

    InsertTailList(
                &AtqEndpointList,
                &pEndpoint->ListEntry
                );

    ReleaseLock( &AtqEndpointLock);
    return(TRUE);

cleanup:

    if ( sNew != INVALID_SOCKET) {
        closesocket( sNew);
    }

    SetLastError(serr);
    return(FALSE);

} // I_CreateListenSocket




BOOL
I_CloseListenSocket(
    IN PATQ_ENDPOINT Endpoint
    )
/*++

  Closes the socket on which a listen was possibly established.

  Returns:
    TRUE, if successful,
    FALSE, otherwise

--*/
{
    INT  serr = NO_ERROR;
    LINGER linger;
    SOCKET s;

    IF_DEBUG(API_ENTRY) {
        ATQ_PRINTF((DBG_CONTEXT,"I_CloseListenSocket called.\n"));
    }

    s = (SOCKET)InterlockedExchangePointer(
                    (PVOID *)&Endpoint->ListenSocket,
                    (PVOID)INVALID_SOCKET
                    );

    if ( s == INVALID_SOCKET) {
        return(TRUE);
    }

    //
    //  Enable linger with timeout of ZERO for "hard" close
    //
    //  Error code from sock option is ignored, since we are
    //   anyway closing the socket
    //

    linger.l_onoff = TRUE;
    linger.l_linger = 0;
    setsockopt( s, SOL_SOCKET, SO_LINGER, (PCHAR)&linger,sizeof(linger));

    //
    // Close the socket
    //

    if (closesocket(s) != 0) {
        serr = WSAGetLastError();
        ATQ_PRINTF(( DBG_CONTEXT,"error %d in closesocket\n",serr));
    } else {

        // Remove the socket from the ListenAtq Context as well
        //  since the socket is now closed here in this function.
        PATQ_CONTEXT patqc = Endpoint->pListenAtqContext;
        if ( patqc != NULL) {
            patqc->hAsyncIO = NULL;
        }
    }

    return (TRUE);
} // I_CloseListenSocket()




DWORD
ListenThreadFunc(
        LPVOID Context
        )
/*++

    Main loop waiting for connections. ( The core of server)
    The thread loops around waiting on an accept() call on
     listenSocket.
    If there is a new message on socket, it invokes the
     callback function for connection.

    NEVER returns untill it is requested to stop by someother
      thread using a call to TS_CONNECTION_INFO::StopConnectionThread().

    Returns:

      0 on success and error code if there is a fatal error.


--*/
{

    INT serr;
    register SOCKET  sNewConnection;
    SOCKADDR_IN sockAddrRemote;
    PATQ_ENDPOINT endpoint = (PATQ_ENDPOINT)Context;

    IF_DEBUG(ENDPOINT) {
        ATQ_PRINTF((DBG_CONTEXT,"ListenThreadFunc() running.\n"));
    }

    //
    //  Loop Forever
    //

    for( ; ;) {

        int cbAddr = sizeof( sockAddrRemote);

        //
        //  Wait for a connection
        //

        IF_DEBUG(ENDPOINT) {
            ATQ_PRINTF((DBG_CONTEXT,"Listening for new connection\n"));
        }

        if ((sNewConnection = WSAAccept(
                                    endpoint->ListenSocket,
                                    (LPSOCKADDR ) &sockAddrRemote,
                                    &cbAddr,
                                    NULL,
                                    0)) != INVALID_SOCKET) {

            //
            // Valid Connection has been established.
            // Invoke the callback function to process this connection
            //   and then continue the loop
            //

            IF_DEBUG(ENDPOINT) {
                ATQ_PRINTF((DBG_CONTEXT,"Got new connection. sock[%d]\n",
                    sNewConnection));
            }

            (*endpoint->ConnectCompletion)(
                                sNewConnection,
                                &sockAddrRemote,
                                endpoint->Context,
                                (PVOID)endpoint
                                );

        } else {

            //
            // Some low level error has occured.
            //

            serr = WSAGetLastError();
            ATQ_PRINTF((DBG_CONTEXT,"Error %d in accept\n", serr));

            if ( serr == WSAEINTR) {

                //
                // Socket was closed by low-level call. Get out.
                //

                break;
            }

            //
            // Check if we are shutting down and if so QUIT
            //

            if (!IS_BLOCK_ACTIVE(endpoint)) {
                IF_DEBUG(ENDPOINT) {
                    ATQ_PRINTF((DBG_CONTEXT,"ListenThread shutting down\n"));
                }
                break;
            }

            //
            // Perform a graceful recovery from failure. NYI
            //  ( Tricky code). Both FTP and Web server are to test it!
            //    Will add this code later. ( MuraliK)
            //

            IF_DEBUG(ENDPOINT) {
                ATQ_PRINTF((DBG_CONTEXT,"Unexpected error %d on accept\n",
                    serr));
            }
        }
    }

    //
    // Cleanup & Exit. Cleanup is done by the code which called the shut down.
    //

    IF_DEBUG(ENDPOINT) {
        ATQ_PRINTF((DBG_CONTEXT,"ListenThread exiting.\n"));
    }
    return ( 0);  // No errors

} // ListenThreadFunc()



BOOL
StartListenThread(
    IN PATQ_ENDPOINT Endpoint
    )
{
    DWORD  id;

    Endpoint->hListenThread = CreateThread(
                                        NULL,
                                        0,
                                        ListenThreadFunc,
                                        (PVOID )Endpoint,
                                        0,
                                        &id
                                        );

    if ( Endpoint->hListenThread != NULL) {
        return(TRUE);
    }

    return(FALSE);

} // StartListenThread



VOID
ATQ_ENDPOINT::CleanupEndpoint(
    VOID
    )
/*++

  Description:
     This function cleansup the internal state of the object and prepares
     it for the deletion.
     All endpoints should pass through this function when the ref count
     this zero.

--*/
{
    DBG_ASSERT( this->m_refCount == 0);
    ATQ_ASSERT( !IS_BLOCK_ACTIVE( this) );
    ASSERT( this->Signature == ATQ_ENDPOINT_SIGNATURE );

    // the following free will throw away the listen atq context
    if ( this->pListenAtqContext != NULL) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "Endpoint(%08x) frees listen context %08x\n",
                     this, this->pListenAtqContext));
        AtqFreeContext( this->pListenAtqContext, FALSE);
        this->pListenAtqContext = NULL;
    }

    if ( this->ShutdownCallback != NULL ) {
        //
        // This only happens when someone calls AtqStopAndCloseEndpoint which should
        // never happen in K2.
        //
        ASSERT( FALSE );
        this->ShutdownCallback( this->ShutdownCallbackContext);
    }

    this->Signature = ATQ_ENDPOINT_SIGNATURE_FREE;

#if DBG
    if( this->RefTraceLog != NULL ) {
        DestroyRefTraceLog( this->RefTraceLog );
    }
#endif

    return;

} // ATQ_ENDPOINT::CleanupEndpoint()

