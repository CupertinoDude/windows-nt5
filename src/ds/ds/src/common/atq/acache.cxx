/*++

   Copyright    (c)    1995-1996    Microsoft Corporation

   Module  Name :
       acache.cxx

   Abstract:
       This module implements the Allocation cache handler and associated
        objects.

   Author:

       Murali R. Krishnan    ( MuraliK )     12-Sept-1996

   Environment:
       Win32 - User Mode

   Project:

       Internet Server DLL

   Functions Exported:



   Revision History:

--*/


/************************************************************
 *     Include Headers
 ************************************************************/

# include <isatq.hxx>
# include "acache.hxx"
# include "dbgutil.h"

// 
// # of CPUs in machine (for allocation threshold scaling)
// 
DWORD g_cCPU = 0;

//
// specifies the registry location to use for getting the ATQ Configuration
//   (Global overrides)
//
CHAR g_PSZ_ATQ_CONFIG_PARAMS_REG_KEY[] =
 TEXT("System\\CurrentControlSet\\Services\\InetInfo\\Parameters");


//
// Lookaside cleanup interval
//

#define ACACHE_REG_LOOKASIDE_CLEANUP_INTERVAL \
                     TEXT( "LookasideCleanupInterval" )
#define ACACHE_REG_DEFAULT_CLEANUP_INTERVAL              ((15)*(60))

/************************************************************
 *    Inlined Documentation on Alloc-Cache
 *
 * Allocation Cache:
 *    This module is to cache the commonly allocated objects
 *    to serve following goals 
 *      1) we can have maximum reuse of blocks
 *      2) avoid traffic to the process heap manager
 *      3) gather statistics for understanding of usage
 *
 * Details on Allocation Cache:
 *   There is one ALLOC_CACHE_HANDLER (shortly ACH) object per 
 *   object that we decide to cache. The ACH is initialized by
 *   the configuration supplied during its construction. ACH serves
 *   as the main object for allocation/free of the objects it is created
 *   to cache. ACH gathers statistics of various operations and provides
 *   hooks to export the gathered statistics. There is a periodic cleanup
 *   scavenger that frees up long unused blocks thus reducing the working
 *   set of the system.
 *
 *   All ACH objects created are chained and maintained in the global
 *   list of allocation cache handler objects. This global list is used
 *   for enumeration, debugging, and statistics dumps
 *   
 * Allocation cache Configuration:
 * 
 *   Each ACH object is created with the ALLOC_CACHE_CONFIGURATION that
 *   specifies the (concurrency factor, threshold, size) desired.
 *   The concurrency factor ensures that we support the specified level 
 *   of concurrency in allocations. The threshold specifies the number 
 *   of objects that we will maintain (max) in the free-list. When the
 *   threshold is exceeded the freed objects are pushed to the process
 *   pool until the currently active objects fall below the threshold.
 *   In addition, each ACH object also retains a read-only name for the
 *   object allocated - for friendly tracking purposes.
 *
 *   There is also a global configuration parameter that specifies the 
 *   Lookaside cleanup interval. 
 *
 * Allocation and Free:
 *   Allocation allocates one free object from the free-list if any exist.
 *   Otherwise the allocation will result in fetching a new object from
 *   the process heap manager.
 *   A free adds the freed object to the free-list if the # free objects
 *   is less than the threshold specified. Otherwise the object is freed
 *   to the process heap manager.
 *   Statistics are gathered during both allocation and free operations.
 *
 * Statisitcs:
 *   Statistics are gathered during the alloc/free operations throughout
 *   the life-time of the ACH. These statistics are reported via the 
 *   DumpStatsToHtml() exported function. The statistics can also be 
 *   gathered by the NTSD helper function.
 *
 * Scheduled List cleanup:
 *   There is a scheduled work item for the lookaside cleanup interval. 
 *   The callback function walks through the list of ACH items on global
 *   list and takes snapshot of the # allocation calls. On a subsequent
 *   walk-through, if the # allocation calls remains the same (which will
 *   be the case if there is no allocation activity), then, the entire
 *   list of alloced objects is pruned. This pruning reduces the working
 *   set of the process.
 ************************************************************/

/************************************************************
 *    Static Functions of ALLOC_CACHE_HANDLER
 ************************************************************/

CRITICAL_SECTION ALLOC_CACHE_HANDLER::sm_csItems;
LIST_ENTRY       ALLOC_CACHE_HANDLER::sm_lItemsHead;
DWORD            ALLOC_CACHE_HANDLER::sm_dwScheduleCookie;

/* class static */
BOOL
ALLOC_CACHE_HANDLER::Initialize(VOID)
{
    // get the number of processors for this machine
    // do it only for NT Server only (don't scale workstation)
    if ( TsIsNtServer() ) {
        SYSTEM_INFO si;
        GetSystemInfo( &si );
        g_cCPU = si.dwNumberOfProcessors;
    } else {
        g_cCPU = 1;
    }

    // initialize the class statics
    InitializeListHead( &sm_lItemsHead);
    InitializeCriticalSection( &sm_csItems);
    SET_CRITICAL_SECTION_SPIN_COUNT( &sm_csItems, IIS_DEFAULT_CS_SPIN_COUNT);
    return ( TRUE);
} // ALLOC_CACHE_HANDLER::Initialize()


/* class static */
BOOL
ALLOC_CACHE_HANDLER::Cleanup(VOID)
{
    DBG_ASSERT( sm_dwScheduleCookie == 0);

    DBG_ASSERT( IsListEmpty(&sm_lItemsHead));
    DeleteCriticalSection( &sm_csItems);

    return ( TRUE);

} // ALLOC_CACHE_HANDLER::Cleanup()


/* class static */
VOID
ALLOC_CACHE_HANDLER::InsertNewItem( IN ALLOC_CACHE_HANDLER * pach)
{
    EnterCriticalSection( &sm_csItems);

    InsertTailList( &sm_lItemsHead, &pach->m_lItemsEntry);
    LeaveCriticalSection( &sm_csItems);

    return;
} // ALLOC_CACHE_HANDLER::InsertNewItem()



/* class static */
VOID
ALLOC_CACHE_HANDLER::RemoveItem( IN ALLOC_CACHE_HANDLER * pach)
{
    EnterCriticalSection( &sm_csItems);

    RemoveEntryList( &pach->m_lItemsEntry);
    LeaveCriticalSection( &sm_csItems);

    return;
} // ALLOC_CACHE_HANDLER::RemoveItem()



/* class static */
BOOL
ALLOC_CACHE_HANDLER::DumpStatsToHtml(
   OUT CHAR * pchBuffer,
   IN OUT LPDWORD lpcchBuffer )
/*++
  Description:
    This function dumps the stats on all allocation cached objects
     to HTML format for diagnostics

  Arguments:
    pchBuffer - pointer to buffer that will contain the html results
    lpcchBuffer - pointer to DWORD containing the size of buffer on entry
               On return this contains the # of bytes written out to buffer

  Return:
    TRUE for success and FALSE for failure
    Look at GetLastError() for the error code.
--*/
{
    LIST_ENTRY  * pEntry;
    DWORD  iCount, cch;
    DWORD  cbTotalMem = 0;
    BOOL   fRet = TRUE;

    if ( (lpcchBuffer == NULL) ) {
        SetLastError( ERROR_INVALID_PARAMETER);
        return ( FALSE);
    }

    EnterCriticalSection( &sm_csItems);

    if ( 300 < *lpcchBuffer ) {

        // Print the header blob
        cch = wsprintf( pchBuffer,
                        " AllocCacheTable Data <br>"
                        "<TABLE BORDER> <TR> "
                        "<TH> Item Name </TH> "
                        "<TH> Config(concurr, threshold, size) </TH> "
                        "<TH> # Total Items </TH> "
                        "<TH> # Alloc Calls </TH> "
                        "<TH> # Free Calls </TH> "
                        "<TH> # Free Entries </TH> "
                        "<TH> # Total Size (bytes) </TH> "
                        " </TR>"
                        );
    } else {
        cch = 300;
    }

    for ( pEntry = sm_lItemsHead.Flink, iCount = 0;
          pEntry != &sm_lItemsHead;
          pEntry = pEntry->Flink, iCount++
          ) {

        ALLOC_CACHE_HANDLER * pach =
            CONTAINING_RECORD( pEntry,
                               ALLOC_CACHE_HANDLER,
                               m_lItemsEntry
                               );

        cbTotalMem += pach->m_acConfig.cbSize * pach->m_nTotal;

        if ( (cch + 150 + strlen( pach->m_pszName)) < *lpcchBuffer) {
            cch += wsprintf( pchBuffer + cch,
                             " <TR> <TD> [%d] %s </TD> <TD> (%d, %d, %d) </TD>"
                             " <TD> %4d </TD>"
                             " <TD> %4d </TD>"
                             " <TD> %4d </TD>"
                             " <TD> %4d </TD>"
                             " <TD> %4d </TD>"
                             " </TR>"
                             ,
                             iCount,
                             pach->m_pszName,
                             pach->m_acConfig.nConcurrency,
                             pach->m_acConfig.nThreshold,
                             pach->m_acConfig.cbSize,
                             pach->m_nTotal,
                             pach->m_nAllocCalls,
                             pach->m_nFreeCalls,
                             pach->m_nFreeEntries,
                             pach->m_acConfig.cbSize * pach->m_nTotal
                             );
        } else {
            cch += 150 + strlen( pach->m_pszName);
        }
    } // for

    LeaveCriticalSection( &sm_csItems);

    //
    // dump the final summary
    //
    if ( (cch + 100 ) < *lpcchBuffer) {
        cch += wsprintf( pchBuffer + cch,
                         " <b>"
                         " <TR> </TR>"
                         " <TR> <TD> Total </TD> <TD> </TD>"
                         " <TD> </TD>"
                         " <TD> </TD>"
                         " <TD> </TD>"
                         " <TD> </TD>"
                         " <TD> %4d </TD>"
                         " </TR>"
                         "</b>"
                         " </TABLE>"
                         ,
                         cbTotalMem
                         );
    } else {
        cch += 100;
    }

    if ( *lpcchBuffer < cch ) {
        SetLastError( ERROR_INSUFFICIENT_BUFFER);
        fRet = FALSE;
    }

    *lpcchBuffer = cch;

    return (fRet);
} // ALLOC_CACHE_HANDLER::DumpStatsToHtml()

extern "C"
BOOL AllocCacheDumpStatsToHtml( OUT CHAR * pch, 
                                IN OUT LPDWORD lpcchBuff)
{
    return ( ALLOC_CACHE_HANDLER::DumpStatsToHtml( pch, lpcchBuff));
} 
    
/* class static */
BOOL
ALLOC_CACHE_HANDLER::SetLookasideCleanupInterval( VOID )
{
    DWORD               dwError;
    DWORD               dwVal = 0;
    HKEY                hkey;

    dwError = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            g_PSZ_ATQ_CONFIG_PARAMS_REG_KEY,
                            0,
                            KEY_READ,
                            &hkey);

    if ( dwError == NO_ERROR ) {

        //
        // get the lookaside list cleanup period
        //

        dwVal = I_AtqReadRegDword( hkey,
                                   ACACHE_REG_LOOKASIDE_CLEANUP_INTERVAL,
                                   ACACHE_REG_DEFAULT_CLEANUP_INTERVAL );

        ATQ_REQUIRE( !RegCloseKey( hkey ) );
    }

    if ( dwVal != 0 )
    {
        sm_dwScheduleCookie =
            ScheduleWorkItem( ALLOC_CACHE_HANDLER::CleanupAllLookasides,
                              NULL,
                              dwVal * 1000,
                              TRUE );

        if ( sm_dwScheduleCookie == 0 )
        {
            return FALSE;
        }
    }

    return TRUE;
} // ALLOC_CACHE_HANDLER::SetLookasideCleanupInterval()


/* class static */
BOOL
ALLOC_CACHE_HANDLER::ResetLookasideCleanupInterval( VOID )
{
    BOOL fReturn = TRUE;
    if ( sm_dwScheduleCookie )
    {
        fReturn = RemoveWorkItem( sm_dwScheduleCookie );
        if (fReturn) {
            sm_dwScheduleCookie = 0;
        }
    }

    return ( fReturn);
} // ALLOC_CACHE_HANDLER::ResetLookasideCleanupInterval()



/* class static */
VOID
ALLOC_CACHE_HANDLER::CleanupAllLookasides(
    IN PVOID            pvContext
)
{
    LIST_ENTRY *        pEntry;

    EnterCriticalSection( &sm_csItems);

    for ( pEntry = sm_lItemsHead.Flink;
          pEntry != &sm_lItemsHead;
          pEntry = pEntry->Flink )
    {
        ALLOC_CACHE_HANDLER * pach =
            CONTAINING_RECORD( pEntry,
                               ALLOC_CACHE_HANDLER,
                               m_lItemsEntry
                               );

        IF_DEBUG( ALLOC_CACHE) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "Cleaning lookaside list for '%s' handler\n",
                         pach->m_pszName ));
        }

        pach->CleanupLookaside( FALSE );
    }

    LeaveCriticalSection( &sm_csItems);
} // ALLOC_CACHE_HANDLER::CleanupAllLookasides()




/************************************************************
 *    Member Functions of ALLOC_CACHE_HANDLER
 ************************************************************/

ALLOC_CACHE_HANDLER::ALLOC_CACHE_HANDLER(
    IN LPCSTR pszName,
    IN const ALLOC_CACHE_CONFIGURATION * pacConfig
    )
    : m_fValid ( FALSE),
      m_nTotal       (0),
      m_nAllocCalls  (0),
      m_nFreeCalls   (0),
      m_nFreeEntries (0),
      m_pszName      (pszName),
      m_nLastAllocCount(0)
{
    DBG_ASSERT( NULL != pacConfig );
    m_acConfig = *pacConfig;

    if ( pacConfig->nThreshold == INFINITE) {
        // this will be compared against a signed value. So be careful.
        m_acConfig.nThreshold = 0x7FFFFFFF;
    } else {
        // scale by the number of processors on MP machines
        m_acConfig.nThreshold *= g_cCPU;
    }

    InitializeCriticalSection( & m_csLock);
    SET_CRITICAL_SECTION_SPIN_COUNT( &m_csLock, IIS_DEFAULT_CS_SPIN_COUNT);

    m_lHead.Next = NULL;

    ALLOC_CACHE_HANDLER::InsertNewItem( this);
    m_fValid = TRUE;
    return;
} // ALLOC_CACHE_HANDLER::ALLOC_CACHE_HANDLER()



ALLOC_CACHE_HANDLER::~ALLOC_CACHE_HANDLER(VOID)
{
    if ( m_fValid) {

        CleanupLookaside( TRUE );

        DeleteCriticalSection( & m_csLock);
        ALLOC_CACHE_HANDLER::RemoveItem( this);
    }

    DBG_ASSERT( 0 == m_nTotal );
    DBG_ASSERT( m_lHead.Next == NULL);
    return;
} // ALLOC_CACHE_HANDLER::~ALLOC_CACHE_HANDLER()



VOID
ALLOC_CACHE_HANDLER::CleanupLookaside(
    IN BOOL             fForceCleanup
)
/*++
  Description:
    This function cleans up the lookaside list by removing excess storage space
    used by the objects allocated by this instance. This function is
    used by the periodic scavenging operation as well as for final cleanup.

  Arguments:
    fForceCleanup - forces a cleanup operation always.

  Returns:
     None
--*/
{
    if ( !fForceCleanup )
    {
        //
        // We are called for the regular scavenging operation
        // Take a snapshot of the # allocation calls so that
        //  we may cleanup space when services are idle.
        //

        IF_DEBUG( ALLOC_CACHE) {
            ATQ_PRINTF(( DBG_CONTEXT,
                         "AllocCalls = %ld, LastAllocCount = %ld\n",
                         m_nAllocCalls,
                         m_nLastAllocCount ));
        }

        if ( m_nAllocCalls != m_nLastAllocCount )
        {
            InterlockedExchange( &m_nLastAllocCount,
                                 m_nAllocCalls );
            return;
        }
    }

    SINGLE_LIST_ENTRY listHeadCopy;

    //
    // make a copy of the first element in the list inside the lock
    // Free the entire chain outside the locked section.
    // Otherwise on a busy system the threads will be waiting for 
    //  this thread to complete
    //

    Lock();
    listHeadCopy.Next = m_lHead.Next;

    //
    // we are about to cleanup all entries -> so set state back properly.
    //
    m_lHead.Next      = NULL;
    m_nFreeEntries    = 0; // no more free-entries available
    Unlock();

    //
    // free up all the entries in the list
    //

    PSINGLE_LIST_ENTRY pl;
    pl = PopEntryList( &listHeadCopy);
    while ( pl != NULL) {

        InterlockedDecrement( &m_nTotal);
        ::LocalFree( pl);
        pl = PopEntryList( &listHeadCopy);
    } // for

    DBG_ASSERT( listHeadCopy.Next == NULL);

    return;
} // ALLOC_CACHE_HANDLER::CleanupLookaside()



LPVOID
ALLOC_CACHE_HANDLER::Alloc( VOID )
{
    LPVOID pv = NULL;

    if ( m_nFreeEntries > 0) {

        // 
        // There are free entries available - allocate from the free pool
        //

        Lock();

        // Check again if the free entry is available.
        if ( m_nFreeEntries > 0) {
            pv = (LPVOID )PopEntryList( & m_lHead);  // get the real object
            m_nFreeEntries--;
        }
        Unlock();
    }

    if ( NULL == pv) {

        //
        // No free entry. Need to alloc a new object.
        //

        pv = (LPVOID ) LocalAlloc( LPTR, m_acConfig.cbSize);

        if ( NULL != pv) {

            // initialize the list head properly and extract exact object.
            ((PSINGLE_LIST_ENTRY ) pv)->Next = NULL;

            // pv = (LPVOID ) (((PSINGLE_LIST_ENTRY ) pv));

            // update counters
            InterlockedIncrement( &m_nTotal);
        }
    }

    if ( NULL != pv ) { InterlockedIncrement( &m_nAllocCalls); }

    return ( pv);
} // ALLOC_CACHE_HANDLER::Alloc()



BOOL
ALLOC_CACHE_HANDLER::Free( LPVOID pv)
{
    // Assume that this is allocated using the Alloc() function
    DBG_ASSERT( NULL != pv);

    // store the items in the alloc cache.

    if ( m_nFreeEntries >= m_acConfig.nThreshold) {

        //
        // threshold for free entries is exceeded. free the object to 
        //  process pool
        //

        ::LocalFree( pv);
        InterlockedDecrement( &m_nTotal);
    } else {

        //
        // Store the given pointer in the single linear list
        //

        Lock();
        PushEntryList( &m_lHead,  (((PSINGLE_LIST_ENTRY ) pv)));
        m_nFreeEntries++;
        Unlock();
    }

    InterlockedIncrement( &m_nFreeCalls);

    return ( TRUE);
} // ALLOC_CACHE_HANDLER::Free()




VOID
ALLOC_CACHE_HANDLER::Print( VOID)
{
    CHAR  rgchBuffer[8192];
    DWORD cchBuffer = sizeof(rgchBuffer);

    DBG_REQUIRE( IpPrint( rgchBuffer, &cchBuffer));

    DBGDUMP(( DBG_CONTEXT, rgchBuffer));

    return;
} // ALLOC_CACHE_HANDLER::Print()



BOOL
ALLOC_CACHE_HANDLER::IpPrint( OUT CHAR * pchBuffer, IN OUT LPDWORD pcchSize)
{
    DWORD cchUsed;

    cchUsed = wsprintfA( pchBuffer,
                         "[%d]ALLOC_CACHE_HANDLER[%08x]. Config: "
                         " ObjSize = %d. Concurrency=%d. Thres=%d.\n"
                         " TotalObjs = %d. Calls: Alloc(%d), Free(%d)."
                         " FreeEntries = %d.\n"
                         ,
                         GetCurrentThreadId(),
                         this,
                         m_acConfig.cbSize, m_acConfig.nConcurrency,
                         m_acConfig.nThreshold,
                         m_nTotal, m_nAllocCalls, m_nFreeCalls, m_nFreeEntries
                         );
    Lock();

    // NYI: Print the list of individual pointers
    Unlock();

    DBG_ASSERT( *pcchSize > cchUsed);
    *pcchSize = cchUsed;

    return (TRUE);
} // ALLOC_CACHE_HANDLER::IpPrint()



VOID
ALLOC_CACHE_HANDLER::QueryStats( IN ALLOC_CACHE_STATISTICS * pacStats )
{
    DBG_ASSERT( pacStats != NULL );

    pacStats->acConfig      = m_acConfig;
    pacStats->nTotal        = m_nTotal;
    pacStats->nAllocCalls   = m_nAllocCalls;
    pacStats->nFreeCalls    = m_nFreeCalls;
    pacStats->nFreeEntries  = m_nFreeEntries;

    return;
} // ALLOC_CACHE_HANDLER::QueryStats()


//
// Global functions
//


DWORD
I_AtqReadRegDword(
   IN HKEY     hkey,
   IN LPCSTR   pszValueName,
   IN DWORD    dwDefaultValue )
/*++

    NAME:       I_AtqReadRegDword

    SYNOPSIS:   Reads a DWORD value from the registry.

    ENTRY:      hkey - Openned registry key to read

                pszValueName - The name of the value.

                dwDefaultValue - The default value to use if the
                    value cannot be read.

    RETURNS     DWORD - The value from the registry, or dwDefaultValue.

--*/
{
    DWORD  err;
    DWORD  dwBuffer;

    DWORD  cbBuffer = sizeof(dwBuffer);
    DWORD  dwType;

    if( hkey != NULL ) {
        err = RegQueryValueExA( hkey,
                               pszValueName,
                               NULL,
                               &dwType,
                               (LPBYTE)&dwBuffer,
                               &cbBuffer );

        if( ( err == NO_ERROR ) && ( dwType == REG_DWORD ) ) {
            dwDefaultValue = dwBuffer;
        }
    }

    return dwDefaultValue;

} // I_AtqReadRegDword()


/************************ End of File ***********************/
