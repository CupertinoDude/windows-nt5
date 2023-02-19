/*++

   Copyright    (c)    1995    Microsoft Corporation

   Module  Name :

      atqmain.cxx

   Abstract:
      This module implements entry points for ATQ - Asynchronous Thread Queue.

   Author:

       Murali R. Krishnan    ( MuraliK )     8-Apr-1996

   Environment:

       User Mode -- Win32

   Project:

       Internet Services Common DLL

   Functions Exported:

       BOOL  AtqInitialize();
       BOOL  AtqTerminate();

       DWORD AtqSetInfo();
       DWORD AtqGetInfo();
       BOOL  AtqGetStatistics();
       BOOL  AtqClearStatistics();

       BOOL  AtqAddAcceptExSockets();
       BOOL  AtqAddAsyncHandle();
       DWORD AtqContextSetInfo();
       VOID  AtqCloseSocket();
       VOID  AtqFreeContext();

       BOOL  AtqReadFile();
       BOOL  AtqWriteFile();
       BOOL  AtqTransmitFile();
       BOOL  AtqPostCompletionStatus();

       PVOID AtqAllocateBandwidthInfo();
       BOOL  AtqFreeBandwidthInfo();
       DWORD AtqBandwidthSetInfo();
--*/

#include "isatq.hxx"


# define ATQ_REG_DEF_THREAD_TIMEOUT_PWS    (30*60)  // 30 minutes

/************************************************************
 * Globals
 ************************************************************/

// ------------------------------
// Configuration for ATQ package
// ------------------------------

extern CHAR g_PSZ_ATQ_CONFIG_PARAMS_REG_KEY[];

//
// concurrent # of threads to run per processor
//

DWORD g_cConcurrency = ATQ_REG_DEF_PER_PROCESSOR_CONCURRENCY;

//
//  Amount of time (in ms) a worker thread will be idle before suicide
//

DWORD g_msThreadTimeout = ATQ_REG_DEF_THREAD_TIMEOUT * 1000;

DWORD g_cCPU = 0;               // # of CPUs in machine (for thread-tuning)
BOOL  g_fUseAcceptEx = TRUE;    // Use AcceptEx if available

//
// The absolute thread limit
//

LONG   g_cMaxThreadLimit = ATQ_REG_DEF_POOL_THREAD_LIMIT;

//
// Should we use fake completion port
//

BOOL g_fUseFakeCompletionPort =  FALSE;

//
// Assumed minimum file transfer rate
//

DWORD g_cbMinKbSec = ATQ_REG_DEF_MIN_KB_SEC;

//
// Size of buffers for fake xmits
//

DWORD g_cbXmitBufferSize = ATQ_REG_DEF_NONTF_BUFFER_SIZE;

//
// number of active context list
//

DWORD g_dwNumContextLists = ATQ_NUM_CONTEXT_LIST;

/*
   g_pfnExitThreadCallback()
    This routine sets the callback routine to be called when one of the
    Atq threads exit so that thread state data can be cleaned up. Currently
    support is for a single routine. One way to support multiple routines would
    be for the caller to save the return value. Such an application would not
    be able to delete the "saved" callback routine.
 */
ATQ_THREAD_EXIT_CALLBACK g_pfnExitThreadCallback = NULL;

// ----------------------------------
// Fake Completion port
// -----------------------------------
//
// Used to gauge pool thread creation. This variable shows number of
// ATQ contexts // ready to be processed by ATQ pool thread. Basically
// this is length of outcoming queue in SIO module and is modified by
// routines there
//

DWORD   g_AtqWaitingContextsCount = 0;

//
// mswsock entry points
//

HINSTANCE g_hMSWsock = NULL;
PFN_ACCEPTEX g_pfnAcceptEx = NULL;
PFN_GETACCEPTEXSOCKADDRS g_pfnGetAcceptExSockaddrs = NULL;
PFN_TRANSMITFILE g_pfnTransmitFile = NULL;
PFN_GET_QUEUED_COMPLETION_STATUS g_pfnGetQueuedCompletionStatus = NULL;
PFN_CREATE_COMPLETION_PORT g_pfnCreateCompletionPort = NULL;
PFN_CLOSE_COMPLETION_PORT  g_pfnCloseCompletionPort = NULL;
PFN_POST_COMPLETION_STATUS g_pfnPostCompletionStatus = NULL;

HINSTANCE   g_hNtdll = NULL;
PFN_RTL_INIT_UNICODE_STRING g_pfnRtlInitUnicodeString = NULL;
PFN_NT_LOAD_DRIVER          g_pfnNtLoadDriver = NULL;
PFN_RTL_NTSTATUS_TO_DOSERR  g_pfnRtlNtStatusToDosError = NULL;
PFN_RTL_INIT_ANSI_STRING    g_pfnRtlInitAnsiString = NULL;
PFN_RTL_ANSI_STRING_TO_UNICODE_STRING g_pfnRtlAnsiStringToUnicodeString = NULL;
PFN_RTL_DOS_PATHNAME_TO_NT_PATHNAME g_pfnRtlDosPathNameToNtPathName_U = NULL;
PFN_RTL_FREE_HEAP g_pfnRtlFreeHeap = NULL;

//
// NT specific
//

PFN_READ_DIR_CHANGES_W g_pfnReadDirChangesW = NULL;

// ------------------------------
// Current State Information
// ------------------------------


HANDLE  g_hIoCompPort = NULL;      // Handle for IO completion port
HANDLE  g_hOplockCompPort = NULL;  // Handle for oplock completion port
LONG    g_cThreads = 0;          // number of thread in the pool
LONG    g_cAvailableThreads = 0; // # of threads waiting on the port.

//
// Is the NTS driver in use
//

BOOL    g_fUseDriver = FALSE;

//
// Current thread limit
//

LONG    g_cMaxThreads = ATQ_REG_DEF_PER_PROCESSOR_ATQ_THREADS;

DWORD   g_cListenBacklog = ATQ_REG_DEF_LISTEN_BACKLOG;

BOOL    g_fShutdown = FALSE;   // if set, indicates that we are shutting down
                               // in that case, all threads should exit.

HANDLE  g_hShutdownEvent = NULL; // set when all running threads shutdown

// ------------------------------
// Bandwidth Throttling Info
// ------------------------------

PBANDWIDTH_INFO     g_pBandwidthInfo = NULL;

// ------------------------------
// Various State/Object Lists
// ------------------------------

//
// Used to switch context between lists
//

DWORD AtqGlobalContextCount = 0;

//
// List of active context
//

ATQ_CONTEXT_LISTHEAD AtqActiveContextList[ATQ_NUM_CONTEXT_LIST];

//
// List of Endpoints in ATQ - one per listen socket
//

LIST_ENTRY AtqEndpointList;
CRITICAL_SECTION AtqEndpointLock;

PALLOC_CACHE_HANDLER  g_pachAtqContexts;

#ifdef IIS_AUX_COUNTERS

LONG g_AuxCounters[NUM_AUX_COUNTERS];

#endif // IIS_AUX_COUNTERS

// ------------------------------
// local to this module
// ------------------------------

LONG  sg_AtqInitializeCount = -1;
BOOL  g_fSpudInitialized = FALSE;

DWORD
I_AtqGetGlobalConfiguration(VOID);

DWORD
I_NumAtqEndpointsOpen(VOID);


/************************************************************
 * Functions
 ************************************************************/

BOOL
AtqInitialize(
    IN DWORD   dwFlags
    )
/*++
Routine Description:

    Initializes the ATQ package

Arguments:
    dwFlags - DWORD containing the flags for use to initialize ATQ library.
    Notably in many cases one may not need the SPUD driver initialized
      for processes other than the IIS main process. This dword helps
      to shut off the unwanted flags.

    This is an ugly way to initialize/shutdown SPUD, but that is what we
    will do. SPUD supports only ONE completion port and hence when we use
    ATQ in multiple processes we should be careful to initialize SPUD only
    once and hopefully in the IIS main process!

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

Note:
    As of 4/16/97 the pszRegKey that is sent is no more utilized.
    We always load the internal configuration parameters from
     one single registry entry specified by PSZ_ATQ_CONFIG_PARAMS_REG_KEY
    The parameter is left in the command line for compatibility
      with old callers :( - NYI: Need to change this.
--*/
{
    DWORD       i;

    if ( InterlockedIncrement( &sg_AtqInitializeCount) != 0) {

        IF_DEBUG( API_ENTRY) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "AtqInitialize( %08x). ATQ is already initialized.\n",
                         dwFlags));
        }

        //
        // we are already initialized. Ignore the new registry settings
        //

        return ( TRUE);
    }

    IF_DEBUG( API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqInitialize[%08x]. Initializing....\n",
                     dwFlags));
    }

    //
    // Initialize context lists and crit sects
    //

    ATQ_CONTEXT_LISTHEAD * pacl;

    for ( pacl = AtqActiveContextList;
          pacl < (AtqActiveContextList + g_dwNumContextLists);
          pacl++) {
        pacl->Initialize();
    }

    InitializeListHead( &AtqEndpointList );
    InitializeCriticalSection( &AtqEndpointLock );

    //
    // init bandwidth throttling
    //

    ATQ_REQUIRE( BANDWIDTH_INFO::AbwInitialize() );

    //
    //  Read registry configurable Atq options.  We have to read these now
    //  because concurrency is set for the completion port at creation time.
    //

    DWORD dwError = I_AtqGetGlobalConfiguration();

    if ( NO_ERROR != dwError) {
        SetLastError( dwError);
        return ( FALSE);
    }

    //
    // Setup an allocation cache for the ATQ Contexts
    // NYI: Auto-tune the threshold limit
    //

    {
        ALLOC_CACHE_CONFIGURATION acConfig;

        DWORD nCachedAtq = ATQ_CACHE_LIMIT_NTS;

        if ( TsIsWindows95()) { nCachedAtq = ATQ_CACHE_LIMIT_W95; }

        acConfig.nConcurrency = 1;
        acConfig.nThreshold = nCachedAtq;
        acConfig.cbSize = sizeof(ATQ_CONTEXT);

        g_pachAtqContexts = new ALLOC_CACHE_HANDLER( "ATQ", &acConfig);

        if ( NULL == g_pachAtqContexts) {
            goto cleanup;
        }
    }


    //
    //  Create the shutdown event
    //

    g_hShutdownEvent = CreateEvent( NULL,    // lpsa (Security Attributes)
                                    TRUE,    // Manual reset
                                    FALSE,   // Not signalled
                                    NULL );  // Name for event.

    //
    //  Create the IO completion port
    //

    g_hIoCompPort = g_pfnCreateCompletionPort(INVALID_HANDLE_VALUE,
                                              g_hIoCompPort,
                                              (DWORD) NULL,
                                              g_cConcurrency
                                              );

    if ( !g_hShutdownEvent || !g_hIoCompPort ) {

        OutputDebugString( " Create IoComp port failed\n");
        goto cleanup;
    }

    //
    //  If we'll be using SPUD, then create the oplock completion port
    //  and start the oplock notification thread.
    //

    if( g_fUseDriver ) {
        g_hOplockCompPort = g_pfnCreateCompletionPort(INVALID_HANDLE_VALUE,
                                                      g_hOplockCompPort,
                                                      (DWORD) NULL,
                                                      1
                                                      );

        if( !g_hOplockCompPort ) {
            OutputDebugString( " Create OplockComp port failed\n" );
            goto cleanup;
        }

        if( !I_AtqStartOplockThread() ) {
            OutputDebugString( " Create oplock notification thread failed\n" );
            goto cleanup;
        }
    }

    //
    // initialize spud driver
    //

    if ( dwFlags & ATQ_INIT_SPUD_FLAG ) {
        (VOID) I_AtqSpudInitialize(g_hIoCompPort, g_hOplockCompPort);
        g_fSpudInitialized = TRUE;
    }

    //
    // Ensure all other initializations also are done
    //

    g_cThreads  = 0;
    g_fShutdown = FALSE;
    g_cAvailableThreads = 0;

    if ( !I_AtqStartTimeoutProcessing( NULL ) ) {
        goto cleanup;
    }

    IF_DEBUG(INIT_CLEAN) {
        DBGPRINTF(( DBG_CONTEXT,
                    "fUseAcceptEx[%d] NT CompPort[%d] Platform[%d]"
                    " fUseDriver[%d]\n",
                    g_fUseAcceptEx,  !g_fUseFakeCompletionPort,
                    IISPlatformType(),
                    g_fUseDriver
                    ));
    }

    //
    // Create the initial ATQ thread.
    //

    (VOID)I_AtqCheckThreadStatus( (PVOID)ATQ_INITIAL_THREAD );

    //
    // Create a second thread if we are NTS
    //

    if ( TsIsNtServer() ) {
        (VOID)I_AtqCheckThreadStatus( (PVOID)ATQ_INITIAL_THREAD );
    }

    IF_DEBUG( API_EXIT) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqInitialize( %08x) returns %d.\n",
                     dwFlags, TRUE));
    }

    return TRUE;

cleanup:

    for (i=0; i<g_dwNumContextLists; i++) {

        AtqActiveContextList[i].Cleanup();
    }

    DeleteCriticalSection( &AtqEndpointLock);

    if ( g_hShutdownEvent != NULL ) {
        CloseHandle( g_hShutdownEvent );
        g_hShutdownEvent = NULL;
    }

    if ( g_hOplockCompPort != NULL ) {
        I_AtqKillOplockThread();
        g_pfnCloseCompletionPort( g_hOplockCompPort );
        g_hOplockCompPort = NULL;
    }

    if ( g_hIoCompPort != NULL ) {
        g_pfnCloseCompletionPort( g_hIoCompPort );
        g_hIoCompPort = NULL;
    }

    if ( NULL != g_pachAtqContexts) {
        delete g_pachAtqContexts;
        g_pachAtqContexts = NULL;
    }

    ATQ_REQUIRE( BANDWIDTH_INFO::AbwTerminate());

    IF_DEBUG( API_EXIT) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqInitialize( %08x) returns %d.\n",
                     dwFlags, FALSE));
    }

    return(FALSE);

} // AtqInitialize()





BOOL
AtqTerminate(
    VOID
    )
/*++

Routine Description:

    Cleans up the ATQ package.  Should only be called after all of the
    clients of ATQ have been shutdown.

Arguments:

    None.

Return Value:

    TRUE, if ATQ was shutdown properly
    FALSE, otherwise

--*/
{
    DWORD       currentThreadCount;
    ATQ_CONTEXT_LISTHEAD * pacl;

    // there are outstanding users, don't fully terminate
    if ( InterlockedDecrement( &sg_AtqInitializeCount) >= 0) {

        IF_DEBUG( API_ENTRY) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "AtqTerminate() - there are other users."
                         " Not terminating now\n"
                         ));
        }
        return (TRUE);
    }

    IF_DEBUG( API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqTerminate() - Terminating ATQ ...\n"
                     ));
    }


    //
    // All the ATQ endpoints should have been terminated before calling
    //  this ATQTerminate() function. If not, sorry return failure.
    //
    DWORD nEndpointsToBeClosed = I_NumAtqEndpointsOpen();

    if ( nEndpointsToBeClosed > 0) {

        DBGPRINTF(( DBG_CONTEXT,
                    " There are %d endpoints remaining to be closed."
                    " Somebody above stream did not close endpoints."
                    " BUG IN CODE ABOVE ATQ\n"
                    ,
                    nEndpointsToBeClosed
                    ));
        SetLastError( ERROR_NETWORK_BUSY);
        return ( FALSE);
    }


    if ( (g_hShutdownEvent == NULL) || g_fShutdown ) {

        //
        // We have not been intialized or have already terminated.
        //
        SetLastError( ERROR_NOT_READY );
        return FALSE;
    }

    // Cleanup variables in ATQ Bandwidth throttle module
    if ( !BANDWIDTH_INFO::AbwTerminate()) {

        // there may be a few blocked IO. We should avoid them all.
        // All clients should have cleaned themselves up before coming here.
        return (FALSE);
    }

    //
    // All clients should have cleaned themselves up before calling us.
    //

    for ( pacl = AtqActiveContextList;
          pacl < (AtqActiveContextList + g_dwNumContextLists);
          pacl++) {

        pacl->Lock();

        if ( !IsListEmpty(&pacl->ActiveListHead)) {

            ATQ_ASSERT( IsListEmpty( &pacl->ActiveListHead));
            pacl->Unlock();

            IF_DEBUG( API_EXIT) {
                ATQ_PRINTF(( DBG_CONTEXT,
                             "AtqTerminate() - ContextList(%08x) has "
                             "Active Contexts. Failed Termination.\n",
                             pacl
                             ));
            }

            return FALSE;
        }

        pacl->Unlock();
    } // for

    //
    // Note that we are shutting down and prevent any more handles from
    // being added to the completion port.
    //

    g_fShutdown = TRUE;

    //
    // Attempt and remove the TimeOut Context from scheduler queue
    //
    DBG_REQUIRE( I_AtqStopTimeoutProcessing());

    currentThreadCount = g_cThreads;
    if (currentThreadCount > 0) {

        DWORD       i;
        BOOLEAN     fRes;
        OVERLAPPED  overlapped;

        //
        // Post a message to the completion port for each worker thread
        // telling it to exit. The indicator is a NULL context in the
        // completion.
        // BUGBUG: Each thread cannot exit with the first error received.
        //  Some thread has to loop until all contexts have gone away.
        //

        ZeroMemory( &overlapped, sizeof(OVERLAPPED) );

        for (i=0; i<currentThreadCount; i++) {

            fRes = g_pfnPostCompletionStatus( g_hIoCompPort,
                                               0,
                                               0,
                                               &overlapped );

            ATQ_ASSERT( (fRes == TRUE) ||
                       ( (fRes == FALSE) &&
                        (GetLastError() == ERROR_IO_PENDING) )
                       );
        }
    }

    //
    // Now wait for the pool threads to shutdown.
    //

    DWORD dwErr =
        WaitForSingleObject( g_hShutdownEvent, ATQ_WAIT_FOR_THREAD_DEATH);
#if 0
    DWORD dwWaitCount = 0;
    while ( dwErr == WAIT_TIMEOUT) {

        dwWaitCount++;
        DebugBreak();
        Sleep( 10*1000);  // sleep for some time
        dwErr =
            WaitForSingleObject( g_hShutdownEvent, ATQ_WAIT_FOR_THREAD_DEATH);
    } // while
# endif // 0

    //
    // At this point, no other threads should be left running.
    //
    //
    //  g_cThreads counter is decremented by AtqPoolThread().
    //  AtqTerminate() is called during the DLL termination
    //  But at DLL termination, all ATQ pool threads are killed =>
    //    no one is decrementing the count. Hence this assert will always fail.
    //

    // ATQ_ASSERT( !g_cThreads );

    if( g_hIoCompPort != NULL ) {
        I_AtqKillOplockThread();
        g_pfnCloseCompletionPort( g_hOplockCompPort );
        g_hOplockCompPort = NULL;
    }

    ATQ_REQUIRE( CloseHandle( g_hShutdownEvent ) );
    g_pfnCloseCompletionPort( g_hIoCompPort );

    g_hShutdownEvent = NULL;
    g_hIoCompPort = NULL;

    //
    // Cleanup our synchronization resources
    //

    for ( pacl = AtqActiveContextList;
          pacl < (AtqActiveContextList + g_dwNumContextLists);
          pacl++) {
        PLIST_ENTRY pEntry;

        pacl->Lock();

        if ( !IsListEmpty( &pacl->PendingAcceptExListHead)) {
            for ( pEntry = pacl->PendingAcceptExListHead.Flink;
                  pEntry != &pacl->PendingAcceptExListHead;
                  pEntry  = pEntry->Flink ) {

                PATQ_CONT pContext =
                    CONTAINING_RECORD( pEntry, ATQ_CONTEXT, m_leTimeout );

                ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
                pContext->Print();
            } // for
        }

        pacl->Unlock();
        pacl->Cleanup();
    }


    //
    // Free all the elements in the Allocation caching list
    //
    if ( NULL != g_pachAtqContexts) {
        delete g_pachAtqContexts;
        g_pachAtqContexts = NULL;
    }

    DeleteCriticalSection( &AtqEndpointLock);

    //
    // cleanup driver
    //

    if ( g_fSpudInitialized ) {
        (VOID)I_AtqSpudTerminate();
        g_fSpudInitialized = FALSE;
    }

    if ( g_hNtdll != NULL ) {
        FreeLibrary(g_hNtdll);
        g_hNtdll = NULL;
    }

    if ( g_hMSWsock != NULL ) {
        FreeLibrary(g_hMSWsock);
        g_hMSWsock = NULL;
    }

    IF_DEBUG( API_EXIT) {
        ATQ_PRINTF(( DBG_CONTEXT,
                     "AtqTerminate() - Successfully cleaned up.\n"
                     ));
    }

    return TRUE;
} // AtqTerminate()





DWORD
AtqSetInfo(
    IN ATQ_INFO         atqInfo,
    IN DWORD            Data
    )
/*++

Routine Description:

    Sets various bits of information for the ATQ module

Arguments:

    atqInfo     - Data item to set
    data        - New value for item

Return Value:

    The old value of the parameter

--*/
{
    DWORD     dwOldVal = 0;

    switch ( atqInfo ) {

    case AtqBandwidthThrottle:

        ATQ_ASSERT( g_pBandwidthInfo != NULL );
        dwOldVal = g_pBandwidthInfo->SetBandwidthLevel( Data );
        break;

    case AtqBandwidthThrottleMaxBlocked:

        ATQ_ASSERT( g_pBandwidthInfo != NULL );
        dwOldVal = g_pBandwidthInfo->SetMaxBlockedListSize( Data );
        break;

    case AtqExitThreadCallback:

        dwOldVal = (DWORD ) g_pfnExitThreadCallback;
        g_pfnExitThreadCallback =  (ATQ_THREAD_EXIT_CALLBACK ) Data;
        break;

    case AtqMaxPoolThreads:
          // the value is per processor values
        // internally we maintain value for all processors
        dwOldVal = g_cMaxThreads/g_cCPU;
        g_cMaxThreads = Data * g_cCPU;
        break;

      //
      //  Increment or decrement the max thread count.  In this instance, we
      //  do not scale by the number of CPUs
      //

      case AtqIncMaxPoolThreads:
        InterlockedIncrement( (LONG *) &g_cMaxThreads );
        dwOldVal = TRUE;
        break;

      case AtqDecMaxPoolThreads:
        InterlockedDecrement( (LONG *) &g_cMaxThreads );
        dwOldVal = TRUE;
        break;


      case AtqMaxConcurrency:
        dwOldVal = g_cConcurrency;
        g_cConcurrency = Data;
        break;

      case AtqThreadTimeout:
        dwOldVal = g_msThreadTimeout/1000;  // convert back to seconds
        g_msThreadTimeout = Data * 1000;    // convert value to millisecs
        break;

      case AtqUseAcceptEx:
        dwOldVal = g_fUseAcceptEx;
        if ( !TsIsWindows95() ) {
            g_fUseAcceptEx = Data;
        }
        break;

      case AtqMinKbSec:

        //
        //  Ignore it if the value is zero
        //

        if ( Data ) {
            dwOldVal = g_cbMinKbSec;
            g_cbMinKbSec = Data;
        }
        break;

      default:
        ATQ_ASSERT( FALSE );
        break;
    }

    return dwOldVal;
} // AtqSetInfo()





DWORD
AtqGetInfo(
    IN ATQ_INFO  atqInfo
    )
/*++

Routine Description:

    Gets various bits of information for the ATQ module

Arguments:

    atqInfo     - Data item to set

Return Value:

    The old value of the parameter

--*/
{
    DWORD     dwVal = 0;

    switch ( atqInfo ) {

      case AtqBandwidthThrottle:
        ATQ_ASSERT( g_pBandwidthInfo != NULL );
        dwVal = g_pBandwidthInfo->QueryBandwidthLevel();
        break;

      case AtqExitThreadCallback:

        dwVal = (DWORD ) g_pfnExitThreadCallback;
        break;

      case AtqMaxPoolThreads:
        dwVal = g_cMaxThreads/g_cCPU;
        break;

      case AtqMaxConcurrency:
        dwVal = g_cConcurrency;
        break;

      case AtqThreadTimeout:
        dwVal = g_msThreadTimeout/1000; // convert back to seconds
        break;

      case AtqUseAcceptEx:
        dwVal = g_fUseAcceptEx;
        break;

      case AtqMinKbSec:
        dwVal = g_cbMinKbSec;
        break;

      default:
        ATQ_ASSERT( FALSE );
        break;
    } // switch

    return dwVal;
} // AtqGetInfo()





BOOL
AtqGetStatistics(IN OUT ATQ_STATISTICS * pAtqStats)
{
    if ( pAtqStats != NULL) {

        return g_pBandwidthInfo->GetStatistics( pAtqStats );

    } else {

        SetLastError( ERROR_INVALID_PARAMETER);
        return (FALSE);
    }
} // AtqGetStatistics()





BOOL
AtqClearStatistics( VOID)
{
    return g_pBandwidthInfo->ClearStatistics();

} // AtqClearStatistics()





DWORD
AtqContextSetInfo(
    PATQ_CONTEXT           patqContext,
    enum ATQ_CONTEXT_INFO  atqInfo,
    DWORD                  Data
    )
/*++

Routine Description:

    Sets various bits of information for this context

Arguments:

    patqContext - pointer to ATQ context
    atqInfo     - Data item to set
    data        - New value for item

Return Value:

    The old value of the parameter

--*/
{
    PATQ_CONT pContext = (PATQ_CONT) patqContext;
    DWORD     dwOldVal = 0;

    ATQ_ASSERT( pContext );
    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );

    if ( pContext && pContext->Signature == ATQ_CONTEXT_SIGNATURE )
    {
        switch ( atqInfo ) {

        case ATQ_INFO_TIMEOUT:
            dwOldVal = pContext->TimeOut;
            pContext->TimeOut = CanonTimeout( Data );
            break;

        case ATQ_INFO_RESUME_IO:

            //
            // set back the max timeout from pContext->TimeOut
            // This will ensure that timeout processing can go on
            //   peacefully.
            //

            {
                DWORD currentTime = AtqGetCurrentTick( );
                DWORD timeout;
                dwOldVal = pContext->NextTimeout;
                timeout = pContext->TimeOut;

                //
                // Set the new timeout
                //

                I_SetNextTimeout(pContext);

                //
                // Return the old
                //

                if ( currentTime >= dwOldVal ) {
                    ATQ_ASSERT((dwOldVal & ATQ_INFINITE) == 0);
                    dwOldVal = 0;
                } else if ( (dwOldVal & ATQ_INFINITE) == 0 ) {
                    dwOldVal -= currentTime;
                }

                // return correct units
                dwOldVal = UndoCanonTimeout( dwOldVal );
            }
            break;

        case ATQ_INFO_COMPLETION:
            dwOldVal = (DWORD) pContext->pfnCompletion;
            pContext->pfnCompletion = (ATQ_COMPLETION) Data;
            break;

        case ATQ_INFO_COMPLETION_CONTEXT:

            ATQ_ASSERT( Data != 0 );        // NULL context not allowed

            dwOldVal = (DWORD) pContext->ClientContext;
            pContext->ClientContext = (void *) Data;
            break;

        case ATQ_INFO_BANDWIDTH_INFO:
        {
            ATQ_ASSERT( Data != 0 );

            PBANDWIDTH_INFO pBandwidthInfo = (PBANDWIDTH_INFO) Data;

            ATQ_ASSERT( pBandwidthInfo->QuerySignature() ==
                                                ATQ_BW_INFO_SIGNATURE );

            if ( !pBandwidthInfo->IsFreed() )
            {
                pContext->m_pBandwidthInfo = (PBANDWIDTH_INFO) Data;
                pContext->m_pBandwidthInfo->Reference();
            }
            break;
        }

        case ATQ_INFO_ABORTIVE_CLOSE:
            dwOldVal = pContext->IsFlag( ACF_ABORTIVE_CLOSE );
            if ( Data )
            {
                pContext->SetFlag( ACF_ABORTIVE_CLOSE );
            }
            else
            {
                pContext->ResetFlag( ACF_ABORTIVE_CLOSE );
            }
            break;

        default:
            ATQ_ASSERT( FALSE );
        }
    }

    return dwOldVal;

} // AtqContextSetInfo()



BOOL
AtqAddAsyncHandle(
    PATQ_CONTEXT * ppatqContext,
    PVOID          EndpointObject,
    PVOID          ClientContext,
    ATQ_COMPLETION pfnCompletion,
    DWORD          TimeOut,
    HANDLE         hAsyncIO
    )
/*++

Routine Description:

    Adds a handle to the thread queue

    The client should call this after the IO handle is opened
    and before the first IO request is made

    Even in the case of failure, client should call AtqFreeContext() and
     free the memory associated with this object.

Arguments:

    ppatqContext - Receives allocated ATQ Context
    Context - Context to call client with
    pfnCompletion - Completion to call when IO completes
    TimeOut - Time to wait (sec) for IO completion (INFINITE is valid)
    hAsyncIO - Handle with pending read or write

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/
{
    return ( I_AtqAddAsyncHandle( (PATQ_CONT *) ppatqContext,
                                  (PATQ_ENDPOINT) EndpointObject,
                                  ClientContext,
                                  pfnCompletion,
                                  TimeOut,
                                  hAsyncIO)
             &&
             I_AddAtqContextToPort( *((PATQ_CONT *) ppatqContext))
            );

} // AtqAddAsyncHandle()




VOID
AtqGetAcceptExAddrs(
    IN  PATQ_CONTEXT patqContext,
    OUT SOCKET *     pSock,
    OUT PVOID *      ppvBuff,
    OUT PVOID *      pEndpointContext,
    OUT SOCKADDR * * ppsockaddrLocal,
    OUT SOCKADDR * * ppsockaddrRemote
    )
{
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    INT       cbsockaddrLocal;
    INT       cbsockaddrRemote;
    DWORD     cb;

    ATQ_ASSERT( g_fUseAcceptEx);
    ATQ_ASSERT( pContext->pEndpoint);

    *pSock   = (SOCKET) pContext->hAsyncIO;
    *pEndpointContext = pContext->pEndpoint->Context;

    //
    //  The buffer not only receives the initial received data, it also
    //  gets the sock addrs, which must be at least sockaddr_in + 16 bytes
    //  large
    //

    g_pfnGetAcceptExSockaddrs( pContext->pvBuff,
                               (cb = pContext->pEndpoint->InitialRecvSize),
                               MIN_SOCKADDR_SIZE,
                               MIN_SOCKADDR_SIZE,
                               ppsockaddrLocal,
                               &cbsockaddrLocal,
                               ppsockaddrRemote,
                               &cbsockaddrRemote );

    *ppvBuff = ( ( cb == 0) ? NULL : pContext->pvBuff);

    return;
} // AtqGetAcceptExAddrs()




BOOL
AtqCloseSocket(
    PATQ_CONTEXT patqContext,
    BOOL         fShutdown
    )
/*++

  Routine Description:

    Closes the socket in this atq structure if it wasn't
    closed by transmitfile. This function should be called only
    if the embedded handle in AtqContext is a Socket.

  Arguments:

    patqContext - Context whose socket should be closed.
    fShutdown - If TRUE, means we call shutdown and always close the socket.
        Note that if TransmitFile closed the socket, it will have done the
        shutdown for us

  Returns:
    TRUE on success and FALSE if there is a failure.
--*/
{
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;

    if ( pContext ) {

        ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );

        BOOL fAbortiveClose;

        fAbortiveClose = pContext->IsFlag( ACF_ABORTIVE_CLOSE );
        pContext->ResetFlag( ACF_ABORTIVE_CLOSE );

        //
        //  Don't delete the socket if we don't have to
        //

        if ( pContext->IsState( ACS_SOCK_UNCONNECTED |
                                ACS_SOCK_CLOSED)
             ) {

            //
            //  Do nothing
            //

        } else {

            // default:
            // case ACS_SOCK_LISTENING:
            // case ACS_SOCK_CONNECTED: {

            HANDLE hIO;
            PATQ_ENDPOINT pEndpoint;

            pEndpoint = pContext->pEndpoint;

            pContext->MoveState( ACS_SOCK_CLOSED);

            //
            //  During shutdown, the socket may be closed while this thread
            //  is doing processing, so only give a warning if any of the
            //  following fail
            //

            hIO = (HANDLE )InterlockedExchange((LPLONG) &pContext->hAsyncIO,
                                               (LONG) NULL);

            if ( hIO == NULL ) {

                //
                // No socket - it is already closed - do nothing.
                //

            } else {

                if (fAbortiveClose || fShutdown ) {

                    //
                    //  If this is an AcceptEx socket, we must first force a
                    //  user mode context update before we can call shutdown
                    //

                    if ( (pEndpoint != NULL) && (pEndpoint->UseAcceptEx) ) {

                        if ( setsockopt( (SOCKET) hIO,
                                         SOL_SOCKET,
                                         SO_UPDATE_ACCEPT_CONTEXT,
                                         (char *) &pEndpoint->ListenSocket,
                                         sizeof(SOCKET) ) == SOCKET_ERROR ) {

                            ATQ_PRINTF(( DBG_CONTEXT,
                                         "[AtqCloseSocket] Warning- setsockopt "
                                         "failed, error %d, socket = %x,"
                                         " Context= %08x, Listen = %lx\n",
                                         GetLastError(),
                                         hIO,
                                         pContext,
                                         pEndpoint->ListenSocket ));
                        }
                    }
                } // setsock-opt call

                if ( fAbortiveClose ) {
                    LINGER  linger;

                    linger.l_onoff = TRUE;
                    linger.l_linger = 0;

                    if ( setsockopt( (SOCKET) hIO,
                                     SOL_SOCKET,
                                     SO_LINGER,
                                     (char *) &linger,
                                     sizeof(linger) ) == SOCKET_ERROR
                         ) {
                        ATQ_PRINTF(( DBG_CONTEXT,
                                     "[AtqCloseSocket] Warning- setsockopt "
                                     "failed, error %d, socket = %x,"
                                     " Context= %08x, Listen = %lx\n",
                                     GetLastError(),
                                     hIO,
                                     pContext,
                                     pEndpoint->ListenSocket ));
                    }
                    else {
                        ATQ_PRINTF(( DBG_CONTEXT,
                                     "[AtqCloseSocket(%08x)] requested"
                                     " abortive close\n",
                                     pContext));
                    }
                } // set up linger

                if ( fShutdown ) {

                    //
                    //  Note that shutdown can fail in instances where the
                    //  client aborts in the middle of a TransmitFile.
                    //  This is an acceptable failure case
                    //

                    shutdown( (int) hIO, 1 );
                }

                DBG_ASSERT( hIO != NULL);

                if ( closesocket( (int) hIO ) ) {

                    ATQ_PRINTF(( DBG_CONTEXT,
                                 "[AtqCloseSocket] Warning- closesocket "
                                 " failed, Context = %08x, error %d,"
                                 " socket = %x\n",
                                 pContext,
                                 GetLastError(),
                                 hIO ));
                }
            } // if (hIO != NULL)
        }

        return TRUE;
    }

    DBGPRINTF(( DBG_CONTEXT, "[AtqCloseSocket] Warning - NULL Atq context\n"));
    SetLastError( ERROR_INVALID_PARAMETER );
    return FALSE;
} // AtqCloseSocket()



BOOL
AtqCloseFileHandle(
    PATQ_CONTEXT patqContext
    )
/*++

  Routine Description:

    Closes the file handle in this atq structure.
    This function should be called only if the embedded handle
    in AtqContext is a file handle.

  Arguments:

    patqContext - Context whose file handle should be closed.

  Returns:
    TRUE on success and FALSE if there is a failure.

  Note:
   THIS FUNCTIONALITY IS ADDED TO SERVE A SPECIAL REQUEST!!!
   Most of the ATQ code thinks that the handle here is a socket.
   Except of course this function...
--*/
{
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;

    if ( pContext != NULL ) {

        HANDLE hIO;

        ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
        ATQ_ASSERT( !pContext->IsAcceptExRootContext());
        ATQ_ASSERT( !TsIsWindows95() );  // NYI

        hIO =
          (HANDLE ) InterlockedExchange((LPLONG ) &pContext->hAsyncIO,
                                        (LONG ) NULL);

        if ( (hIO == NULL) || !CloseHandle( hIO ) ) {

            ATQ_PRINTF(( DBG_CONTEXT,
                        "[AtqCloseFileHandle] Warning- CloseHandle failed, "
                        " Context = %08x, error %d, handle = %x\n",
                        pContext,
                        GetLastError(),
                        hIO ));
        }

        return TRUE;
    }

    DBGPRINTF(( DBG_CONTEXT, "[AtqCloseSocket] Warning - NULL Atq context\n"));
    SetLastError( ERROR_INVALID_PARAMETER );
    return FALSE;
} // AtqCloseFileHandle()





VOID
AtqFreeContext(
    PATQ_CONTEXT patqContext,
    BOOL         fReuseContext
    )
/*++

Routine Description:

    Frees the context created in AtqAddAsyncHandle.
    Call this after the async handle has been closed and all outstanding
    IO operations have been completed. The context is invalid after this call.
    Call AtqFreeContext() for same context only ONCE.

Arguments:

    patqContext - Context to free
    fReuseContext - TRUE if this can context can be reused in the context of
        the calling thread.  Should be FALSE if the calling thread will exit
        soon (i.e., isn't an AtqPoolThread).

--*/
{
    PATQ_CONT pContext = (PATQ_CONT)patqContext;

    ATQ_ASSERT( pContext != NULL );

    IF_DEBUG( API_ENTRY) {

        ATQ_PRINTF(( DBG_CONTEXT, "AtqFreeContext( %08x (handle=%08x,"
                     " nIOs = %d), fReuse=%d)\n",
                     patqContext, patqContext->hAsyncIO,
                     pContext->m_nIO,
                     fReuseContext));
    }

    if ( pContext ) {

        ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );

        if ( fReuseContext ) {
            pContext->SetFlag( ACF_REUSE_CONTEXT);
        } else {
            pContext->ResetFlag( ACF_REUSE_CONTEXT);
        }

        if ( InterlockedDecrement(  &pContext->m_nIO) == 0) {

            //
            // The number of outstanding ref holders is ZERO.
            // Free up this ATQ context.
            //
            // We really do not free up the context - but try to reuse
            //  it if possible
            //

            DBG_ASSERT( pContext->lSyncTimeout == 0);
            AtqpReuseOrFreeContext( pContext, fReuseContext);
        }
    }

    return;
} // AtqFreeContext()



BOOL
AtqReadFile(
        IN PATQ_CONTEXT patqContext,
        IN LPVOID       lpBuffer,
        IN DWORD        BytesToRead,
        IN OVERLAPPED * lpo OPTIONAL
        )
/*++

  Routine Description:

    Does an async read using the handle defined in the context.

  Arguments:

    patqContext - pointer to ATQ context
    lpBuffer - Buffer to put read data in
    BytesToRead - number of bytes to read
    lpo - Overlapped structure to use

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{
    BOOL fRes;
    DWORD cbRead;     // discarded after usage ( since this is Async)
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pContext->m_pBandwidthInfo;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);
    ATQ_ASSERT( !TsIsWindows95() );  // NYI
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    InterlockedIncrement( &pContext->m_nIO);

    I_SetNextTimeout(pContext);
    pContext->BytesSent = 0;

    if ( !lpo ) {
        lpo = &pContext->Overlapped;
    }

    switch ( pBandwidthInfo->QueryStatus( AtqIoRead ) ) {

      case StatusAllowOperation:

        pBandwidthInfo->IncTotalAllowedRequests();
        fRes = ( ReadFile( pContext->hAsyncIO,
                          lpBuffer,
                          BytesToRead,
                          &cbRead,
                          lpo ) ||
                GetLastError() == ERROR_IO_PENDING);

        if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };

        break;

      case StatusBlockOperation:

        // store data for restarting the operation.
        pContext->arInfo.atqOp        = AtqIoRead;
        pContext->arInfo.lpOverlapped = lpo;
        pContext->arInfo.uop.opReadWrite.buf1.len = BytesToRead;
        pContext->arInfo.uop.opReadWrite.buf1.buf = (CHAR * ) lpBuffer;
        pContext->arInfo.uop.opReadWrite.dwBufferCount = 1;
        pContext->arInfo.uop.opReadWrite.pBufAll  = NULL;

        // Put this request in queue of blocked requests.
        fRes = pBandwidthInfo->BlockRequest( pContext );
        if ( fRes )
        {
            pBandwidthInfo->IncTotalBlockedRequests();
            break;
        }
        // fall through

    case StatusRejectOperation:
        InterlockedDecrement( &pContext->m_nIO);
        pBandwidthInfo->IncTotalRejectedRequests();
        SetLastError( ERROR_NETWORK_BUSY);
        fRes = FALSE;
        break;

      default:
        ATQ_ASSERT( FALSE);
        SetLastError( ERROR_INVALID_PARAMETER);
        fRes = FALSE;
        break;

    } // switch()

    return fRes;
} // AtqReadFile()



BOOL
AtqReadSocket(
        IN PATQ_CONTEXT  patqContext,
        IN LPWSABUF     pwsaBuffers,
        IN DWORD        dwBufferCount,
        IN OVERLAPPED *  lpo OPTIONAL
        )
/*++

  Routine Description:

    Does an async recv using the handle defined in the context
     as a socket.

  Arguments:

    patqContext - pointer to ATQ context
    lpBuffer - Buffer to put read data in
    BytesToRead - number of bytes to read
    lpo - Overlapped structure to use

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{
    BOOL fRes;
    DWORD cbRead;     // discarded after usage ( since this is Async)
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pContext->m_pBandwidthInfo;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    IF_DEBUG(API_ENTRY) {
        ATQ_PRINTF(( DBG_CONTEXT,
                 "AtqReadSocket(%08lx) called.\n", pContext));
    }

    if (pContext->IsFlag( ACF_RECV_ISSUED)) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "AtqReadSocket -> WSARecv bypassed.\n"));
        pContext->BytesSent = 0;
        pContext->SetFlag( ACF_RECV_CALLED);
        return TRUE;
    }

    I_SetNextTimeout(pContext);


    // count the number of bytes
    DBG_ASSERT( dwBufferCount >= 1);
    pContext->BytesSent = 0;

    InterlockedIncrement( &pContext->m_nIO);

    if ( !lpo ) {
        lpo = &pContext->Overlapped;
    }

    //
    // NYI: Create an optimal function table
    //

    if ( !g_fUseFakeCompletionPort &&
         (StatusAllowOperation == pBandwidthInfo->QueryStatus( AtqIoRead ) ) ) {

        DWORD  lpFlags = 0;

        fRes = ( (WSARecv( (SOCKET ) pContext->hAsyncIO,
                           pwsaBuffers,
                           dwBufferCount,
                           &cbRead,
                           &lpFlags,  // no flags
                           lpo,
                           NULL       // no completion routine
                           ) == 0) ||
                 (WSAGetLastError() == WSA_IO_PENDING));
        if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };
    } else {

        switch ( pBandwidthInfo->QueryStatus( AtqIoRead ) ) {

        case StatusAllowOperation:

            DBG_ASSERT(g_fUseFakeCompletionPort);

            fRes = SIOWSARecv(
                        pContext,
                        pwsaBuffers,
                        dwBufferCount,
                        lpo
                        );

            break;

        case StatusBlockOperation:

            // store data for restarting the operation.
            pContext->arInfo.atqOp        = AtqIoRead;
            pContext->arInfo.lpOverlapped = lpo;

            pContext->arInfo.uop.opReadWrite.dwBufferCount = dwBufferCount;
            if ( dwBufferCount == 1) {
                pContext->arInfo.uop.opReadWrite.buf1.len = pwsaBuffers->len;
                pContext->arInfo.uop.opReadWrite.buf1.buf = pwsaBuffers->buf;
                pContext->arInfo.uop.opReadWrite.pBufAll  = NULL;
            } else {
                DBG_ASSERT( dwBufferCount > 1);

                //
                // Inefficient: But we will burn CPU for b/w throttling.
                //
                WSABUF * pBuf = (WSABUF *)
                    ::LocalAlloc( LPTR, dwBufferCount * sizeof (WSABUF));
                if ( NULL != pBuf) {
                    pContext->arInfo.uop.opReadWrite.pBufAll = pBuf;
                    CopyMemory( pBuf, pwsaBuffers,
                                dwBufferCount * sizeof(WSABUF));
                } else {
                    return ( FALSE);
                }
            }

            // Put this request in queue of blocked requests.
            fRes = pBandwidthInfo->BlockRequest( pContext );
            if ( fRes )
            {
                pBandwidthInfo->IncTotalBlockedRequests();
                break;
            }
            // fall through

        case StatusRejectOperation:
            InterlockedDecrement( &pContext->m_nIO);
            pBandwidthInfo->IncTotalRejectedRequests();
            SetLastError( ERROR_NETWORK_BUSY);
            fRes = FALSE;
            break;

        default:
            ATQ_ASSERT( FALSE);
            SetLastError( ERROR_INVALID_PARAMETER);
            fRes = FALSE;
            break;
        } // switch()
    }

    return fRes;
} // AtqReadSocket()



BOOL
AtqWriteFile(
    IN PATQ_CONTEXT patqContext,
    IN LPCVOID      lpBuffer,
    IN DWORD        BytesToWrite,
    IN OVERLAPPED * lpo OPTIONAL
    )
/*++

  Routine Description:

    Does an async write using the handle defined in the context.

  Arguments:

    patqContext - pointer to ATQ context
    lpBuffer - Buffer to write
    BytesToWrite - number of bytes to write
    lpo - Overlapped structure to use

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{
    BOOL fRes;
    DWORD cbWritten; // discarded after usage ( since this is Async)
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pContext->m_pBandwidthInfo;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);
    ATQ_ASSERT( !TsIsWindows95() );  // NYI
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    I_SetNextTimeout(pContext);
    pContext->BytesSent = BytesToWrite;

    if ( !lpo ) {
        lpo = &pContext->Overlapped;
    }

    InterlockedIncrement( &pContext->m_nIO);

    switch ( pBandwidthInfo->QueryStatus( AtqIoWrite) ) {

      case StatusAllowOperation:

        pBandwidthInfo->IncTotalAllowedRequests();
        fRes = ( WriteFile( pContext->hAsyncIO,
                            lpBuffer,
                            BytesToWrite,
                            &cbWritten,
                            lpo ) ||
                 GetLastError() == ERROR_IO_PENDING);
        if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };

        break;

      case StatusBlockOperation:

        // store data for restarting the operation.
        pContext->arInfo.atqOp        = AtqIoWrite;
        pContext->arInfo.lpOverlapped = lpo;

        pContext->arInfo.uop.opReadWrite.buf1.len = BytesToWrite;
        pContext->arInfo.uop.opReadWrite.buf1.buf = (CHAR * ) lpBuffer;
        pContext->arInfo.uop.opReadWrite.dwBufferCount = 1;
        pContext->arInfo.uop.opReadWrite.pBufAll  = NULL;

        // Put this request in queue of blocked requests.
        fRes = pBandwidthInfo->BlockRequest( pContext );
        if ( fRes )
        {
            pBandwidthInfo->IncTotalBlockedRequests();
            break;
        }
        // fall through

    case StatusRejectOperation:
        InterlockedDecrement( &pContext->m_nIO);
        pBandwidthInfo->IncTotalRejectedRequests();
        SetLastError( ERROR_NETWORK_BUSY);
        fRes = FALSE;
        break;

    default:
        ATQ_ASSERT( FALSE);
        SetLastError( ERROR_INVALID_PARAMETER);
        fRes = FALSE;
        break;

    } // switch()

    return fRes;
} // AtqWriteFile()



BOOL
AtqWriteSocket(
    IN PATQ_CONTEXT  patqContext,
    IN  LPWSABUF     pwsaBuffers,
    IN  DWORD        dwBufferCount,
    IN OVERLAPPED *  lpo OPTIONAL
    )
/*++

  Routine Description:

    Does an async write using the handle defined in the context as a socket.

  Arguments:

    patqContext - pointer to ATQ context
    pwsaBuffer  - pointer to Winsock Buffers for scatter/gather
    dwBufferCount - DWORD containing the count of buffers pointed
                   to by pwsaBuffer
    lpo - Overlapped structure to use

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{
    BOOL fRes;
    DWORD cbWritten; // discarded after usage ( since this is Async)
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pContext->m_pBandwidthInfo;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    I_SetNextTimeout(pContext);

    //
    // count the number of bytes
    //

    DBG_ASSERT( dwBufferCount >= 1);
    pContext->BytesSent = pwsaBuffers->len;
    if ( dwBufferCount > 1) {
        LPWSABUF pWsaBuf;
        for ( pWsaBuf = pwsaBuffers + 1;
              pWsaBuf <= (pwsaBuffers + dwBufferCount);
              pWsaBuf++) {
            pContext->BytesSent += pWsaBuf->len;
        }
    }

    if ( lpo == NULL ) {
        lpo = &pContext->Overlapped;
    }

    InterlockedIncrement( &pContext->m_nIO);

    if ( !g_fUseFakeCompletionPort &&
         (StatusAllowOperation ==
          pBandwidthInfo->QueryStatus( AtqIoWrite ) ) ) {

        pBandwidthInfo->IncTotalAllowedRequests();
        fRes = ( (WSASend( (SOCKET ) pContext->hAsyncIO,
                           pwsaBuffers,
                           dwBufferCount,
                           &cbWritten,
                           0,               // no flags
                           lpo,
                           NULL             // no completion routine
                           ) == 0) ||
                 (WSAGetLastError() == WSA_IO_PENDING));
        if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };
    } else {
        switch ( pBandwidthInfo->QueryStatus( AtqIoWrite ) ) {

        case StatusAllowOperation:

            pBandwidthInfo->IncTotalAllowedRequests();
            DBG_ASSERT(g_fUseFakeCompletionPort);

            fRes = SIOWSASend(
                        pContext,
                        pwsaBuffers,
                        dwBufferCount,
                        lpo
                        );
            break;

        case StatusBlockOperation:

            // store data for restarting the operation.
            pContext->arInfo.atqOp        = AtqIoWrite;
            pContext->arInfo.lpOverlapped = lpo;

            pContext->arInfo.uop.opReadWrite.dwBufferCount = dwBufferCount;
            if ( dwBufferCount == 1) {
                pContext->arInfo.uop.opReadWrite.buf1.len = pwsaBuffers->len;
                pContext->arInfo.uop.opReadWrite.buf1.buf = pwsaBuffers->buf;
                pContext->arInfo.uop.opReadWrite.pBufAll  = NULL;
            } else {
                DBG_ASSERT( dwBufferCount > 1);

                //
                // Inefficient: But we will burn CPU for b/w throttling.
                //
                WSABUF * pBuf = (WSABUF *)
                    ::LocalAlloc( LPTR, dwBufferCount * sizeof (WSABUF));
                if ( NULL != pBuf) {
                    pContext->arInfo.uop.opReadWrite.pBufAll = pBuf;
                    CopyMemory( pBuf, pwsaBuffers,
                                dwBufferCount * sizeof(WSABUF));
                } else {
                    return ( FALSE);
                }
            }

            // Put this request in queue of blocked requests.
            fRes = pBandwidthInfo->BlockRequest( pContext );
            if ( fRes )
            {
                pBandwidthInfo->IncTotalBlockedRequests();
                break;
            }
            // fall through

        case StatusRejectOperation:
            InterlockedDecrement( &pContext->m_nIO);
            pBandwidthInfo->IncTotalRejectedRequests();
            SetLastError( ERROR_NETWORK_BUSY);
            fRes = FALSE;
            break;

        default:
            ATQ_ASSERT( FALSE);
            SetLastError( ERROR_INVALID_PARAMETER);
            fRes = FALSE;
            break;

        } // switch()
    }

    return fRes;
} // AtqWriteSocket()




BOOL
AtqSyncWsaSend(
    IN  PATQ_CONTEXT patqContext,
    IN  LPWSABUF     pwsaBuffers,
    IN  DWORD        dwBufferCount,
    OUT LPDWORD      pcbWritten
    )
/*++

  Routine Description:

    Does a sync write of an array of wsa buffers using WSASend.

  Arguments:

    patqContext - pointer to ATQ context
    pwsaBuffer  - pointer to Winsock Buffers for scatter/gather
    dwBufferCount - DWORD containing the count of buffers pointed
                   to by pwsaBuffer
    pcbWritten - ptr to count of bytes written

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{

    BOOL fRes = FALSE;
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);

    fRes = ( WSASend(  (SOCKET ) pContext->hAsyncIO,
                       pwsaBuffers,
                       dwBufferCount,
                       pcbWritten,
                       0,               // no flags
                       NULL,            // lpo == NULL for sync write
                       NULL             // no completion routine
                       ) == 0);

    return fRes;

} // AtqSyncWsaSend()




BOOL
AtqTransmitFile(
    IN PATQ_CONTEXT            patqContext,
    IN HANDLE                  hFile,
    IN DWORD                   dwBytesInFile,
    IN LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
    IN DWORD                   dwFlags
    )
/*++

  Routine Description:

    Does a TransmitFile  using the handle defined in the context.

  Arguments:

    patqContext - pointer to ATQ context
    hFile - handle of file to read from
    dwBytesInFile - Bytes to transmit
    lpTransmitBuffers - transmit buffer structure
    dwFlags - Transmit file flags

  Returns:
    TRUE on success and FALSE if there is a failure.

--*/
{
    BOOL fRes;
    PATQ_CONT pContext = (PATQ_CONT) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pContext->m_pBandwidthInfo;

    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    //
    //  For large file sends, the client's default timeout may not be
    //  adequte for slow links.  Scale based on bytes being sent
    //

    I_SetNextTimeout(pContext);
    pContext->BytesSent = dwBytesInFile;

    if ( dwFlags == 0 ) {

        //
        // If no flags are set, then we can attempt to use the special
        // write-behind flag.  This flag can cause the TransmitFile to
        // complete immediately, before the send actually completes.
        // This can be a significant performance improvement inside the
        // system.
        //

        dwFlags = TF_WRITE_BEHIND;

    } else if ( dwFlags & TF_DISCONNECT ) {

        //
        //  If the socket is getting disconnected, mark it appropriately
        //

        pContext->MoveState( ( ( dwFlags & TF_REUSE_SOCKET )?
                               ACS_SOCK_UNCONNECTED:
                               ACS_SOCK_CLOSED
                               )
                             );
    }

    InterlockedIncrement( &pContext->m_nIO);

    if ( !g_fUseFakeCompletionPort &&
         (StatusAllowOperation == pBandwidthInfo->QueryStatus( AtqIoXmitFile ) ) ) {

        pBandwidthInfo->IncTotalAllowedRequests();
        fRes = (g_pfnTransmitFile( (SOCKET ) pContext->hAsyncIO,
                                hFile,
                                dwBytesInFile,
                                0,
                                &pContext->Overlapped,
                                lpTransmitBuffers,
                                dwFlags ) ||
                (GetLastError() == ERROR_IO_PENDING));
        if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };
    } else {
        switch ( pBandwidthInfo->QueryStatus( AtqIoXmitFile ) ) {

        case StatusAllowOperation:

            DBG_ASSERT(g_fUseFakeCompletionPort);
            pBandwidthInfo->IncTotalAllowedRequests();

            fRes = SIOTransmitFile(
                            pContext,
                            hFile,
                            dwBytesInFile,
                            lpTransmitBuffers
                            );
            break;

        case StatusBlockOperation:

            // store data for restarting the operation.
            pContext->arInfo.atqOp        = AtqIoXmitFile;
            pContext->arInfo.lpOverlapped = &pContext->Overlapped;
            pContext->arInfo.uop.opXmit.hFile = hFile;
            pContext->arInfo.uop.opXmit.dwBytesInFile = dwBytesInFile;
            pContext->arInfo.uop.opXmit.lpXmitBuffers = lpTransmitBuffers;
            pContext->arInfo.uop.opXmit.dwFlags       = dwFlags;

            // Put this request in queue of blocked requests.
            fRes = pBandwidthInfo->BlockRequest( pContext);
            if ( fRes )
            {
                pBandwidthInfo->IncTotalBlockedRequests();
                break;
            }
            // fall through

        case StatusRejectOperation:
            InterlockedDecrement( &pContext->m_nIO);
            pBandwidthInfo->IncTotalRejectedRequests();
            SetLastError( ERROR_NETWORK_BUSY);
            fRes = FALSE;
            break;

        default:
            ATQ_ASSERT( FALSE);
            SetLastError( ERROR_INVALID_PARAMETER);
            fRes = FALSE;
            break;

        } // switch()
    }

    //
    //  Restore the socket state if we failed so that the handle gets freed
    //

    if ( !fRes )
    {
        pContext->MoveState( ACS_SOCK_CONNECTED);
    }

    return fRes;

} // AtqTransmitFile()


BOOL
AtqReadDirChanges(IN PATQ_CONTEXT patqContext,
                  IN LPVOID       lpBuffer,
                  IN DWORD        BytesToRead,
                  IN BOOL         fWatchSubDir,
                  IN DWORD        dwNotifyFilter,
                  IN OVERLAPPED * lpo
                  )
/*++
  AtqReadDirChanges()

  Description:
    This function submits an Async ReadDirectoryChanges() call for
    the Async handle in the ATQ context supplied.
    It always requires a non-NULL overlapped pointer for processing
    this call.

  Arguments:
    patqContext  - pointer to ATQ Context
    lpBuffer     - buffer for the data to be read from ReadDirectoryChanges()
    BytesToRead  - count of bytes to read into buffer
    fWatchSubDir - should we watch for sub directory changes
    dwNotifyFilter - DWORD containing the flags for Notification
    lpo          - pointer to overlapped structure.

  Returns:
    TRUE if ReadDirectoryChanges() is successfully submitted.
    FALSE if there is any failure in submitting IO.
--*/
{
    BOOL fRes;
    DWORD cbRead;     // discarded after usage ( since this is Async)
    PATQ_CONT pContext = (PATQ_CONT ) patqContext;
    ATQ_ASSERT( pContext->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pContext->arInfo.atqOp == AtqIoNone);

    if ( g_pfnReadDirChangesW == NULL ) {
        ATQ_PRINTF((DBG_CONTEXT,"ReadDirChanges entry point NULL\n"));
        SetLastError(ERROR_NOT_SUPPORTED);
        return(FALSE);
    }

    if ( lpo == NULL ) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    I_SetNextTimeout(pContext);
    pContext->BytesSent = 0;

    InterlockedIncrement( &pContext->m_nIO);
    fRes = g_pfnReadDirChangesW( pContext->hAsyncIO,
                          lpBuffer,
                          BytesToRead,
                          fWatchSubDir,
                          dwNotifyFilter,
                          &cbRead,
                          lpo,
                          NULL);
    if (!fRes) { InterlockedDecrement( &pContext->m_nIO); };
    return fRes;
} // AtqReadDirChanges()



BOOL
AtqPostCompletionStatus(
        IN PATQ_CONTEXT patqContext,
        IN DWORD        BytesTransferred
        )
/*++

Routine Description:

    Posts a completion status on the completion port queue

    An IO pending error code is treated as a success error code

Arguments:

    patqContext - pointer to ATQ context
    Everything else as in the Win32 API

    NOTES:

Return Value:

    TRUE if successful, FALSE on error (call GetLastError)

--*/

{
    BOOL fRes;
    PATQ_CONT  pAtqContext = (PATQ_CONT ) patqContext;
    PBANDWIDTH_INFO pBandwidthInfo = pAtqContext->m_pBandwidthInfo;

    ATQ_ASSERT( (pAtqContext)->Signature == ATQ_CONTEXT_SIGNATURE );
    ATQ_ASSERT( pBandwidthInfo != NULL );
    ATQ_ASSERT( pBandwidthInfo->QuerySignature() == ATQ_BW_INFO_SIGNATURE );

    if ( !pAtqContext->IsBlocked()) {

        InterlockedIncrement( &pAtqContext->m_nIO);

        fRes = ( g_pfnPostCompletionStatus( g_hIoCompPort,
                                          BytesTransferred,
                                          (DWORD) patqContext,
                                          &pAtqContext->Overlapped ) ||
                (GetLastError() == ERROR_IO_PENDING));
        if (!fRes) { InterlockedDecrement( &pAtqContext->m_nIO); };
    } else {

        //
        // Forcibly remove the context from blocking list.
        //

        fRes = pBandwidthInfo->RemoveFromBlockedList(pAtqContext);

        // There is a possibility of race conditions!
        //  If we cant remove an item from blocking list before
        //         its IO operation is scheduled.
        // there wont be any call back generated for this case!
    }

    return fRes;

} // AtqPostCompletionStatus



DWORD
I_AtqGetGlobalConfiguration(VOID)
/*++
Description:
   This function sets several global config params for the ATQ package.
   It also reads the global configuration from registry for ATQ.
   The values if present will override the defaults

Returns:
   Win32 Errorcode - NO_ERROR on success and anything else for error
--*/
{
    DWORD       dwError = NO_ERROR;
    DWORD       dwDefaultThreadTimeout = ATQ_REG_DEF_THREAD_TIMEOUT;

    //
    // If this is a NTW, do the right thing
    //

    if ( !TsIsNtServer() ) {
        g_cCPU = 1;
        g_cMaxThreadLimit = ATQ_REG_MIN_POOL_THREAD_LIMIT;

        //
        // chicago does not have transmitfile/acceptex support
        //

        if ( TsIsWindows95() ) {

            g_fUseAcceptEx = FALSE;
            g_fUseFakeCompletionPort = TRUE;
            g_dwNumContextLists = ATQ_NUM_CONTEXT_LIST_W95;
            dwDefaultThreadTimeout = ATQ_REG_DEF_THREAD_TIMEOUT_PWS;
            g_cMaxThreadLimit = ATQ_MAX_THREAD_LIMIT_W95;
            g_cMaxThreads = ATQ_MAX_THREAD_LIMIT_W95;
        }

    } else {

        SYSTEM_INFO si;
        MEMORYSTATUS ms;

        //
        // get the count of CPUs for Thread Tuning.
        //

        GetSystemInfo( &si );
        g_cCPU = si.dwNumberOfProcessors;

        //
        // get the memory size
        //

        ms.dwLength = sizeof(MEMORYSTATUS);
        GlobalMemoryStatus( &ms );

        //
        // attempt to use driver
        //

        g_fUseDriver = TRUE;

        //
        // Alloc two threads per MB of memory.
        //

        g_cMaxThreadLimit = (ms.dwTotalPhys >> 19) + 2;

        if ( g_cMaxThreadLimit < ATQ_REG_MIN_POOL_THREAD_LIMIT ) {
            g_cMaxThreadLimit = ATQ_REG_MIN_POOL_THREAD_LIMIT;
        } else if ( g_cMaxThreadLimit > ATQ_REG_MAX_POOL_THREAD_LIMIT ) {
            g_cMaxThreadLimit = ATQ_REG_MAX_POOL_THREAD_LIMIT;
        }
    }

    //
    // Get entry points for NT
    //

    if ( !TsIsWindows95() ) {

        if ( !I_AtqInitializeNtEntryPoints( ) ) {
            dwError = ERROR_MOD_NOT_FOUND;
            return ( dwError);
        }

        g_pfnCreateCompletionPort = CreateIoCompletionPort;
        g_pfnGetQueuedCompletionStatus = GetQueuedCompletionStatus;
        g_pfnCloseCompletionPort = CloseHandle;
        g_pfnPostCompletionStatus = PostQueuedCompletionStatus;

    } else {

        //
        // win95 entry points
        //

        g_pfnCreateCompletionPort = SIOCreateCompletionPort;
        g_pfnGetQueuedCompletionStatus = SIOGetQueuedCompletionStatus;
        g_pfnCloseCompletionPort = SIODestroyCompletionPort;
        g_pfnPostCompletionStatus = SIOPostCompletionStatus;
    }

    if ( !TsIsWindows95() ) {

        HKEY        hkey = NULL;
        DWORD       dwVal;

        dwError = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                g_PSZ_ATQ_CONFIG_PARAMS_REG_KEY,
                                0,
                                KEY_READ,
                                &hkey);

        if ( dwError == NO_ERROR ) {

            //
            // Read the Concurrency factor per processor
            //

            dwVal = I_AtqReadRegDword( hkey,
                                       ATQ_REG_PER_PROCESSOR_CONCURRENCY,
                                       ATQ_REG_DEF_PER_PROCESSOR_CONCURRENCY);

            AtqSetInfo( AtqMaxConcurrency, dwVal);


            //
            // Read the count of threads to be allowed per processor
            //

            dwVal = I_AtqReadRegDword( hkey,
                                       ATQ_REG_PER_PROCESSOR_ATQ_THREADS,
                                       ATQ_REG_DEF_PER_PROCESSOR_ATQ_THREADS
                                       );

            if ( dwVal != 0 ) {
                AtqSetInfo( AtqMaxPoolThreads, dwVal);
            }


            //
            // Read the Data transfer rate value for our calculations
            //

            dwVal = I_AtqReadRegDword( hkey,
                                       ATQ_REG_MIN_KB_SEC,
                                       ATQ_REG_DEF_MIN_KB_SEC );

            AtqSetInfo( AtqMinKbSec, dwVal);


            //
            // read the max thread limit
            //

            g_cMaxThreadLimit = I_AtqReadRegDword( hkey,
                                                   ATQ_REG_POOL_THREAD_LIMIT,
                                                   g_cMaxThreadLimit);

            //
            // read the listen backlog
            //

            g_cListenBacklog = I_AtqReadRegDword( hkey,
                                                  ATQ_REG_LISTEN_BACKLOG,
                                                  g_cListenBacklog);


            //
            // Read the time (in seconds) of how long the threads
            //   can stay alive when there is no IO operation happening on
            //   that thread.
            //

            dwVal = I_AtqReadRegDword( hkey,
                                       ATQ_REG_THREAD_TIMEOUT,
                                       dwDefaultThreadTimeout
                                       );

            AtqSetInfo( AtqThreadTimeout, dwVal);


            ATQ_REQUIRE( !RegCloseKey( hkey ) );
            hkey = NULL;
        }

        DBG_ASSERT( NULL == hkey);

    } else {

        g_cListenBacklog = 5;
        AtqSetInfo( AtqMaxConcurrency, ATQ_REG_DEF_PER_PROCESSOR_CONCURRENCY);
        AtqSetInfo( AtqUseAcceptEx, ATQ_REG_DEF_USE_ACCEPTEX);
    }

    return ( dwError);
} // I_AtqGetGlobalConfiguration()



DWORD
I_NumAtqEndpointsOpen(VOID)
/*++
  Description:
    This function counts the number of Enpoints that remain open.

  Arguments:
     None

  Returns:
     DWORD containing the number of endpoints that are open.
--*/
{
    DWORD nEPOpen = 0;
    AcquireLock( &AtqEndpointLock);

    PLIST_ENTRY plEP;
    for( plEP  = AtqEndpointList.Flink;
         plEP != &AtqEndpointList;
         plEP  = plEP->Flink ) {

        nEPOpen++;
    } // for

    ReleaseLock( &AtqEndpointLock);
    return ( nEPOpen);
} // I_NumAtqEndpointsOpen()

