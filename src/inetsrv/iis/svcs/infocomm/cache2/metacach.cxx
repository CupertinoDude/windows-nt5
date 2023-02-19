/*++

   Copyright    (c)    1995    Microsoft Corporation

   Module  Name :

        metacach.cxx

   Abstract:
        This module contains the tsunami caching routines for metadata.

   Author:
        Henry Sanders    ( henrysa )     15-Oct-1996

--*/
//

#include <tsunami.hxx>
#include "TsunamiP.Hxx"
#pragma hdrstop
#include <dbgutil.h>
#include <issched.hxx>
#include <metacach.hxx>

extern TCHAR * FlipSlashes( TCHAR * pszPath );

//
// The number of buckets in our hash table.
//

#define METACACHE_TABLE_SIZE    127
#define METACACHE_ENTRY_SIGN    ((DWORD)'ECEM')
#define METACACHE_ENTRY_FREE    ((DWORD)'ECEf')


// time in minutes to keep entry in metachace after it was used last time
#define TIME_TO_KEEP_METACACHE_ENTRY    (5)

//
// Structure of a metacache table entry.
//

typedef struct _METACACHE_ENTRY {

    DWORD                   Signature;
    struct _METACACHE_ENTRY *pNext;
    DWORD                   dwDataSetNumber;
    DWORD                   dwServiceID;
    PVOID                   pMetaData;
    DWORD                   dwRefCount;
    DWORD                   dwLastUsedTime;
    PMDFREERTN              pFreeRoutine;
    BOOL                    bValid;

} METACACHE_ENTRY, *PMETACACHE_ENTRY;

//
// Structure of a hash table bucket.
//

typedef struct _METACACHE_BUCKET {

    PMETACACHE_ENTRY        pEntry;
    CRITICAL_SECTION        csCritSec;

} METACACHE_BUCKET;

METACACHE_BUCKET    MetaCacheTable[METACACHE_TABLE_SIZE];

DWORD   MetaCacheTimerCookie = 0;
DWORD   g_dwMetaCacheScavengeCnt = 1;
/************************************************************
 *    Functions
 ************************************************************/
dllexp
PVOID
TsFindMetaData(

    IN      DWORD           dwDataSetNumber,
    IN      DWORD           dwServiceID

    )
/*++

  Routine Description:

    This function takes a data set number and service ID, and tries to find
    a formatted chunk of metadata in the cache. If it does so, it returns a
    pointer to it, otherwise it returns NULL.

  Arguments

    dwDataSetNumber         - The data set number to be found.
    dwServiceID             - ID of calling service.
--*/
{
    DWORD               dwIndex;
    PMETACACHE_ENTRY    pCurrentEntry;

    dwIndex = dwDataSetNumber % METACACHE_TABLE_SIZE;

    //
    // This needes to be protected, we use a critical section per bucket.
    //
    EnterCriticalSection(&MetaCacheTable[dwIndex].csCritSec);

    pCurrentEntry = MetaCacheTable[dwIndex].pEntry;

    // Walk the chain on the bucket. If we find a match, return it.
    //
    while (pCurrentEntry != NULL )
    {
        PCOMMON_METADATA        pCMD;

        pCMD = (PCOMMON_METADATA)pCurrentEntry->pMetaData;

        pCMD->CheckSignature();
        ASSERT(pCMD->QueryCacheInfo() == pCurrentEntry);


        if (pCurrentEntry->dwDataSetNumber == dwDataSetNumber &&
            pCurrentEntry->dwServiceID == dwServiceID &&
            pCurrentEntry->bValid)
        {

            ASSERT( pCurrentEntry->Signature == METACACHE_ENTRY_SIGN );

            // Found a match. Increment the refcount and return a pointer
            // to the metadata.
            InterlockedIncrement((LONG *)&pCurrentEntry->dwRefCount);
            pCurrentEntry->dwLastUsedTime = g_dwMetaCacheScavengeCnt;

            LeaveCriticalSection(&MetaCacheTable[dwIndex].csCritSec);

            return pCurrentEntry->pMetaData;
        }

        // Otherwise try the next one.
        pCurrentEntry = pCurrentEntry->pNext;
    }


    // Didn't find a match, so we'll return NULL.
    LeaveCriticalSection(&MetaCacheTable[dwIndex].csCritSec);

    return NULL;
}

dllexp
PVOID
TsAddMetaData(

    IN      PCOMMON_METADATA pMetaData,
    IN      PMDFREERTN      pFreeRoutine,
    IN      DWORD           dwDataSetNumber,
    IN      DWORD           dwServiceID
    )
/*++

  Routine Description:

    Add a chunk of formatted metadata to our cache.

  Arguments

    pMetaData               - MetaData to be added.
    dwDataSetNumber         - The data set number to be found.
    dwServiceID             - ID of calling service.

  Returns
    Pointer to metacache 'handle' to be used when freeing information.

--*/
{
    PMETACACHE_ENTRY        pNewEntry;
    DWORD                   dwIndex;

    pMetaData->CheckSignature();

    dwIndex = dwDataSetNumber % METACACHE_TABLE_SIZE;


    pNewEntry = (PMETACACHE_ENTRY)ALLOC(sizeof(METACACHE_ENTRY));

    if (pNewEntry == NULL)
    {
        // Couldn't add the entry. No big deal, just return.
        return NULL;
    }

    pNewEntry->Signature = METACACHE_ENTRY_SIGN;
    pNewEntry->dwDataSetNumber = dwDataSetNumber;
    pNewEntry->dwServiceID = dwServiceID;
    pNewEntry->pMetaData = pMetaData;
    pNewEntry->pFreeRoutine = pFreeRoutine;
    pNewEntry->dwRefCount = 1;
    pNewEntry->bValid = TRUE;
    pNewEntry->dwLastUsedTime = g_dwMetaCacheScavengeCnt;

    pMetaData->SetCacheInfo(pNewEntry);

    EnterCriticalSection(&MetaCacheTable[dwIndex].csCritSec);

    pNewEntry->pNext = MetaCacheTable[dwIndex].pEntry;

    MetaCacheTable[dwIndex].pEntry = pNewEntry;

    LeaveCriticalSection(&MetaCacheTable[dwIndex].csCritSec);

    return pNewEntry;

}

dllexp
VOID
TsFreeMetaData(

    IN      PVOID           pCacheEntry

    )
/*++

  Routine Description:

    Free a chunk of formatted metadata to the cache. What we really do here
    is decrement the ref count. If it goes to 0 and the cache element is
    marked deleted, we'll free it here.

  Arguments

    pMetaData               - MetaData to be freed.
--*/
{
    PMETACACHE_ENTRY        pEntry = (PMETACACHE_ENTRY)pCacheEntry;
    PCOMMON_METADATA        pCMD;


    ASSERT( pEntry->Signature == METACACHE_ENTRY_SIGN );

    pCMD = (PCOMMON_METADATA)pEntry->pMetaData;

    pCMD->CheckSignature();

    ASSERT(pCMD->QueryCacheInfo() == pEntry);

    InterlockedDecrement((LONG *)&pEntry->dwRefCount);

}

dllexp
VOID
TsAddRefMetaData(

    IN      PVOID           pCacheEntry

    )
/*++

  Routine Description:

    Increment reference count to chunk of formatted metadata

  Arguments

    pMetaData               - MetaData to be AddRef'ed
--*/
{
    PMETACACHE_ENTRY        pEntry = (PMETACACHE_ENTRY)pCacheEntry;

    ASSERT( pEntry->Signature == METACACHE_ENTRY_SIGN );

    InterlockedIncrement((LONG *)&pEntry->dwRefCount);
}

dllexp
VOID
TsFlushMetaCache(
    DWORD       dwService,
    BOOL        bTerminating
    )
/*++

  Routine Description:

    Called when we need to flush all of our cached metainformation. We walk
    the table, and for each entry we check to see if it's in use. If it's not
    we'll free it, otherwise we'll mark it as deleted.

    If the passed in dwService ID is non-zero, then we'll only
    flush those entries that match the service. Also, if we're terminating,
    we'll do some additional checking, and also cancle any callbacks if we need
    to.

  Arguments

        dwService       - Service ID of entries to be flushed, 0 for all
                            services.
        bTerminating    - TRUE if the caller is terminating.


--*/
{
    UINT                i;
    PMETACACHE_ENTRY    pEntry;
    PMETACACHE_ENTRY    pTrailer;
    PCOMMON_METADATA        pCMD;

    for (i = 0; i < METACACHE_TABLE_SIZE; i++)
    {
        EnterCriticalSection(&MetaCacheTable[i].csCritSec);

        pTrailer = CONTAINING_RECORD(&MetaCacheTable[i].pEntry,
                                        METACACHE_ENTRY, pNext);

        // Walk the chain on the bucket. For every entry, if it's not in
        // use, free it.
        //
        while (pTrailer->pNext != NULL )
        {
            pEntry = pTrailer->pNext;

            ASSERT( pEntry->Signature == METACACHE_ENTRY_SIGN );

            pCMD = (PCOMMON_METADATA)pEntry->pMetaData;

            pCMD->CheckSignature();
            ASSERT(pCMD->QueryCacheInfo() == pEntry);

            if (dwService == 0 || dwService == pEntry->dwServiceID)
            {
                if (pEntry->dwRefCount == 0)
                {
                    // This entry is not in use.

                    // If whoever added it gave us a free routine, call it now.
                    if (pEntry->pFreeRoutine != NULL)
                    {
                        (*(pEntry->pFreeRoutine))(pEntry->pMetaData);
                    }

                    // Look at the next one.
                    pTrailer->pNext = pEntry->pNext;

                    pEntry->Signature = METACACHE_ENTRY_FREE;
                    FREE(pEntry);
                }
                else
                {
                    // In a debug build we'll assert here if we're terminating,
                    // since that shouldn't happen. In a free build we won't
                    // assert for that but we will NULL out the free routine to
                    // keep it from getting called, since presumably the owner is
                    // going away.

                    if (bTerminating)
                    {
                        DBGPRINTF(( DBG_CONTEXT,
                            "\n=========================================\n"
                            "Leftover item in metacache - %8p, bValid = %s\n"
                            "\t dwServiceID    = %8d  pMetaData    = %8p\n"
                            "\t dwRefCount     = %8d  pFreeRoutine = %8p\n"
                            ,
                            pEntry,
                            (pEntry->bValid ? "TRUE" : "FALSE"),
                            pEntry->dwServiceID,
                            pEntry->pMetaData,
                            pEntry->dwRefCount,
                            pEntry->pFreeRoutine ));

                        pEntry->pFreeRoutine = NULL;
                    }


                    pEntry->bValid = FALSE;
                    pTrailer = pEntry;
                }
            }
            else
            {
                pTrailer = pEntry;
            }
        }


        LeaveCriticalSection(&MetaCacheTable[i].csCritSec);
    }
}

dllexp
VOID
TsReferenceMetaData(
    IN      PVOID           pEntry
    )
/*++

  Routine Description:

    Called when we need to reference a metadata cache entry. The caller
    must have already referenced it once.

  Arguments

    pEntry          - Entry to be referenced.


--*/
{
    PMETACACHE_ENTRY        pCacheEntry = (PMETACACHE_ENTRY)pEntry;
    PCOMMON_METADATA        pCMD;


    ASSERT( pCacheEntry->Signature == METACACHE_ENTRY_SIGN );

    pCMD = (PCOMMON_METADATA)pCacheEntry->pMetaData;

    pCMD->CheckSignature();

    ASSERT(pCMD->QueryCacheInfo() == pCacheEntry);

    InterlockedIncrement((LONG *)&pCacheEntry->dwRefCount);

}

VOID
WINAPI
MetaCacheScavenger(
    PVOID       pContext
    )
/*++

  Routine Description:

    Called periodically to time out metacache information. We scan the table;
    if we find an object that's not in use we free it.

  Arguments

    None.


--*/
{
    UINT                i;
    PMETACACHE_ENTRY    pEntry;
    PMETACACHE_ENTRY    pTrailer;
    PCOMMON_METADATA        pCMD;


    for (i = 0; i < METACACHE_TABLE_SIZE; i++)
    {
        if (MetaCacheTable[i].pEntry == NULL)
        {
            continue;
        }

        EnterCriticalSection(&MetaCacheTable[i].csCritSec);

        pTrailer = CONTAINING_RECORD(&MetaCacheTable[i].pEntry,
                                        METACACHE_ENTRY, pNext);


        // Walk the chain on the bucket. For every entry, if it's not in
        // use, free it.
        //
        while (pTrailer->pNext != NULL )
        {
            pEntry = pTrailer->pNext;

            ASSERT( pEntry->Signature == METACACHE_ENTRY_SIGN );
            pCMD = (PCOMMON_METADATA)pEntry->pMetaData;

            pCMD->CheckSignature();
            ASSERT(pCMD->QueryCacheInfo() == pEntry);


            if (pEntry->dwRefCount == 0 &&
                (pEntry->dwLastUsedTime + TIME_TO_KEEP_METACACHE_ENTRY < g_dwMetaCacheScavengeCnt))
            {

                // This entry is not in use.

                // If whoever added it gave us a free routine, call it now.
                if (pEntry->pFreeRoutine != NULL)
                {
                    (*(pEntry->pFreeRoutine))(pEntry->pMetaData);
                }

                // Free the entry and look at the next one.
                pTrailer->pNext = pEntry->pNext;

                pEntry->Signature = METACACHE_ENTRY_FREE;

                FREE(pEntry);
            }
            else
            {
                pTrailer = pEntry;
            }
        }

        LeaveCriticalSection(&MetaCacheTable[i].csCritSec);
    }
    InterlockedIncrement ((long *)&g_dwMetaCacheScavengeCnt);
}


dllexp
VOID
_TsValidateMetaCache(
    VOID
    )
/*++


--*/
{
    UINT                i;
    PMETACACHE_ENTRY    pEntry;
    PCOMMON_METADATA        pCMD;

    for (i = 0; i < METACACHE_TABLE_SIZE; i++)
    {
        if (MetaCacheTable[i].pEntry == NULL)
        {
            continue;
        }

        EnterCriticalSection(&MetaCacheTable[i].csCritSec);

        pEntry = MetaCacheTable[i].pEntry;

        while (pEntry != NULL )
        {

            ASSERT( pEntry->Signature == METACACHE_ENTRY_SIGN );
            pCMD = (PCOMMON_METADATA)pEntry->pMetaData;

            pCMD->CheckSignature();
            ASSERT(pCMD->QueryCacheInfo() == pEntry);

            pEntry = pEntry->pNext;
        }

        LeaveCriticalSection(&MetaCacheTable[i].csCritSec);
    }
}


BOOL
MetaCache_Initialize(
    VOID
    )
/*++

  Routine Description:

    Initialize our metacache code.

  Arguments

    Nothing.

--*/
{
    UINT        i;

    for (i = 0; i < METACACHE_TABLE_SIZE; i++)
    {
        INITIALIZE_CRITICAL_SECTION(&MetaCacheTable[i].csCritSec);
        MetaCacheTable[i].pEntry = NULL;

    }

    MetaCacheTimerCookie = ScheduleWorkItem(
                                       MetaCacheScavenger,
                                       NULL,
                                       60 * 1000,  // 1 minute
                                       TRUE );     // Periodic

    if (!MetaCacheTimerCookie)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL
MetaCache_Terminate(
    VOID
    )
/*++

  Routine Description:

    Terminate our metacache code.

  Arguments

    Nothing.

--*/
{
    UINT i;

    if (MetaCacheTimerCookie != 0)
    {
        RemoveWorkItem(MetaCacheTimerCookie);
        MetaCacheTimerCookie = 0;
    }

    TsFlushMetaCache(0, TRUE);

    for (i = 0; i < METACACHE_TABLE_SIZE; i++)
    {
        DeleteCriticalSection(&MetaCacheTable[i].csCritSec);
    }

    return TRUE;
}




COMMON_METADATA::COMMON_METADATA(VOID)
: m_IpDnsAccessCheckSize( 0 ),
  m_IpDnsAccessCheckPtr ( NULL ),
  m_IpDnsAccessCheckTag ( 0 ),
  m_fDontLog            ( FALSE ),
  m_dwAccessPerm        ( MD_ACCESS_READ ),
  m_dwSslAccessPerm     ( 0 ),
  m_pAcl                ( NULL ),
  m_dwAclTag            ( 0 ),
  m_dwVrLevel           ( 0 ),
  m_dwVrLen             ( 0 ),
  m_hVrToken            ( NULL ),
  m_fVrPassThrough      ( FALSE ),
  m_dwVrError           ( 0 ),
  m_Signature           ( CMD_SIG ),
  m_fDoCache            ( TRUE )
{
    //
    //  Hmmm, since most of these values aren't getting initialized, if
    //  somebody went and deleted all the metadata items from the tree, then
    //  bad things could happen.  We should initialize with defaults things
    //  that might cause us trouble us
    //

} // COMMON_METADATA::COMMON_METADATA()


COMMON_METADATA::~COMMON_METADATA(VOID)
{
    CheckSignature();

    if ( m_IpDnsAccessCheckTag )
        {
            FreeMdTag( m_IpDnsAccessCheckTag );
            m_IpDnsAccessCheckTag = 0;
        }
    else if ( m_IpDnsAccessCheckPtr != NULL )
        {
            LocalFree( m_IpDnsAccessCheckPtr );
            m_IpDnsAccessCheckPtr = NULL;
        }

    if ( m_dwAclTag )
        {
            FreeMdTag( m_dwAclTag );
            m_dwAclTag = 0;
        }

    if ( m_hVrToken )
        {
            TsDeleteUserToken( m_hVrToken );
            m_hVrToken = NULL;
        }

} // COMMON_METADATA::~COMMON_METADATA()


VOID
COMMON_METADATA::FreeMdTag(
    DWORD dwTag
    )
/*++

Routine Description:

    Free a metadata object accessed by reference

Arguments:

    dwTag - tag of metadata object reference

Returns:

    Nothing

--*/
{
    MB  mb( (IMDCOM*) m_pInstance->m_Service->QueryMDObject() );

    CheckSignature();
    mb.ReleaseReferenceData( dwTag );
}


//
//  Private constants.
//

#define DEFAULT_MD_RECORDS          40
#define DEFAULT_RECORD_SIZE         50

# define DEF_MD_REC_SIZE   ((1 + DEFAULT_MD_RECORDS) * \
                            (sizeof(METADATA_RECORD) + DEFAULT_RECORD_SIZE))

#define RMD_ASSERT(x) if (!(x)) {DBG_ASSERT(FALSE); return FALSE; }


BOOL
COMMON_METADATA::ReadMetaData(
    PIIS_SERVER_INSTANCE    pInstance,
    MB *                    pmb,
    LPSTR                   pszURL,
    PMETADATA_ERROR_INFO    pMDError
    )
{
    METADATA_GETALL_INTERNAL_RECORD *pMDRecord;
    DWORD               dwNumMDRecords;
    BYTE                tmpBuffer[ DEF_MD_REC_SIZE];
    BUFFER              TempBuff( tmpBuffer, DEF_MD_REC_SIZE);
    DWORD               i;
    DWORD               dwDataSetNumber;
    INT                 ch;
    LPSTR               pszInVr;
    LPSTR               pszMinInVr;
    DWORD               dwNeed;
    DWORD               dwL;
    DWORD               dwVRLen;
    LPSTR               pszVrUserName;
    LPSTR               pszVrPassword;
    BYTE                tmpPrivateBuffer[ 20 ];
    BUFFER              PrivateBuffer( tmpPrivateBuffer, 20 );
    DWORD               dwPrivateBufferUsed;



    CheckSignature();
    TsValidateMetaCache();

    m_pInstance = pInstance;

    DBG_ASSERT( TempBuff.QuerySize() >=
                (DEFAULT_MD_RECORDS *
                 (sizeof(METADATA_RECORD) + DEFAULT_RECORD_SIZE))
                );

    if ( !pmb->Open( pInstance->QueryMDVRPath() ))
    {
        return FALSE;
    }

    if ( !pmb->GetAll( pszURL,
                     METADATA_INHERIT | METADATA_PARTIAL_PATH | METADATA_REFERENCE,
                     IIS_MD_UT_FILE,
                     &TempBuff,
                     &dwNumMDRecords,
                     &dwDataSetNumber ))
    {
        return FALSE;
    }

    pMDRecord = (METADATA_GETALL_INTERNAL_RECORD *)TempBuff.QueryPtr();
    i = 0;

    //
    // Check from where we got VR_PATH
    //

    pszMinInVr = pszURL ;
    if ( *pszURL )
    {
        for ( pszInVr = pszMinInVr + strlen(pszMinInVr) ;; )
        {
            ch = *pszInVr;
            *pszInVr = '\0';
            dwNeed = 0;
            if ( !pmb->GetString( pszURL, MD_VR_PATH, IIS_MD_UT_FILE, NULL, &dwNeed, 0 ) &&
                 GetLastError() == ERROR_INSUFFICIENT_BUFFER )
            {
                *pszInVr = (CHAR)ch;
                // VR_PATH was defined at this level !

                break;
            }
            *pszInVr = (CHAR)ch;

            if ( ch )
            {
                if ( pszInVr > pszMinInVr )
                {
                    pszInVr = CharPrev( pszMinInVr, pszInVr );
                }
                else
                {
                    //
                    // VR_PATH was defined above Instance vroot
                    // or not at all. If defined above, then the reference
                    // path is empty, so we can claim we found it.
                    // if not defined, then this will be catch later.
                    //

                    break;
                }
            }

            // scan for previous delimiter

            while ( *pszInVr != '/' && *pszInVr != '\\' )
            {
                if ( pszInVr > pszMinInVr )
                {
                    pszInVr = CharPrev( pszMinInVr, pszInVr );
                }
                else
                {
                    //
                    // VR_PATH was defined above Instance vroot
                    // or not at all. If defined above, then the reference
                    // path is empty, so we can claim we found it.
                    // if not defined, then this will be catch later.
                    //

                    break;
                }
            }
        }

        dwVRLen = DIFF(pszInVr - pszMinInVr);
    }
    else
    {
        dwVRLen = 0;
        pszInVr = pszMinInVr;
    }

    // Close this now to minimize lock contention.
    DBG_REQUIRE(pmb->Close());

    for ( dwL = 0 ; pszMinInVr < pszInVr - 1 ; pszMinInVr = CharNext(pszMinInVr) )
    {
        if ( *pszMinInVr == '/' || *pszMinInVr == '\\' )
        {
            ++dwL;
        }
    }

    // Now walk through the array of returned metadata objects and format
    // each one into our predigested form.

    SetVrLevelAndLen( dwL, dwVRLen );
    pszVrPassword = NULL;
    pszVrUserName = NULL;
    dwPrivateBufferUsed = 0;
    pMDError->IsValid = FALSE;

    for ( ; i < dwNumMDRecords; i++, pMDRecord++ ) {

        PVOID       pDataPointer;
        CHAR        *pszMimePtr;
        CHAR        *pszTemp;
        DWORD       dwTemp;


        pDataPointer = (PVOID) ((PCHAR)TempBuff.QueryPtr() +
                                    pMDRecord->dwMDDataOffset);

        switch ( pMDRecord->dwMDIdentifier ) {

        case MD_IP_SEC:
            RMD_ASSERT( pMDRecord->dwMDDataType == BINARY_METADATA );

            if ( pMDRecord->dwMDDataLen )
            {
                if ( !SetIpDnsAccessCheck( pMDRecord->dwMDDataTag ?
                                                pMDRecord->pbMDData :
                                                pDataPointer,
                                           pMDRecord->dwMDDataLen,
                                           pMDRecord->dwMDDataTag ) )
                {
                    goto FreeRefs;
                }
            }
            break;

        case MD_ACCESS_PERM:
            DBG_ASSERT( pMDRecord->dwMDDataTag == NULL );
            RMD_ASSERT( pMDRecord->dwMDDataType == DWORD_METADATA );
            // 64bit alignment fix
            SetAccessPerms(*(UNALIGNED DWORD*) pDataPointer);
            break;

        case MD_SSL_ACCESS_PERM:
            DBG_ASSERT( pMDRecord->dwMDDataTag == NULL );
            RMD_ASSERT( pMDRecord->dwMDDataType == DWORD_METADATA );
            // 64bit alignment fix
            SetSslAccessPerms( *((UNALIGNED DWORD *) pDataPointer) );
            break;

        case MD_DONT_LOG:
            DBG_ASSERT( pMDRecord->dwMDDataTag == NULL );
            DBG_ASSERT( pMDRecord->dwMDDataType == DWORD_METADATA );
            // 64bit alignment fix
            SetDontLogFlag( *((UNALIGNED DWORD *) pDataPointer ));
            break;

        case MD_VR_PATH:
            RMD_ASSERT( pMDRecord->dwMDDataType == STRING_METADATA );
            if (!QueryVrPath()->Copy((const CHAR *)pDataPointer))
            {
                goto FreeRefs;
            }
            break;

                case MD_APP_ROOT:
            RMD_ASSERT( pMDRecord->dwMDDataType == STRING_METADATA );
            if (!QueryAppPath()->Copy((const CHAR *)pDataPointer))
            {
                goto FreeRefs;
            }
            break;

        case MD_VR_USERNAME:
            RMD_ASSERT( pMDRecord->dwMDDataType == STRING_METADATA );
            pszVrUserName = (LPSTR)pDataPointer;
            break;

        case MD_VR_PASSWORD:
            RMD_ASSERT( pMDRecord->dwMDDataType == STRING_METADATA );
            pszVrPassword = (LPSTR)pDataPointer;
            break;

        case MD_VR_PASSTHROUGH:
            RMD_ASSERT( pMDRecord->dwMDDataType == DWORD_METADATA );
            // 64bit alignment fix
            SetVrPassThrough( !!*((UNALIGNED DWORD *) pDataPointer) );
            break;

        case MD_VR_ACL:
            DBG_ASSERT( pMDRecord->dwMDDataTag );
            RMD_ASSERT( pMDRecord->dwMDDataType == BINARY_METADATA );
            if ( pMDRecord->dwMDDataTag )
            {
                SetAcl( pMDRecord->pbMDData,
                        pMDRecord->dwMDDataLen,
                        pMDRecord->dwMDDataTag );
            }
            break;

        case MD_VR_NO_CACHE:
            RMD_ASSERT( pMDRecord->dwMDDataType == DWORD_METADATA );
            // 64bit alignment fix
            SetDoCache( !*(UNALIGNED DWORD *) pDataPointer );
            break;

        default:
            if ( !HandlePrivateProperty( pszURL, pInstance, pMDRecord, pDataPointer, &PrivateBuffer, &dwPrivateBufferUsed, pMDError ) )
            {
                goto FreeRefs;
            }
            CheckSignature();
            break;
        }
    }


    if (!FinishPrivateProperties(&PrivateBuffer, dwPrivateBufferUsed, TRUE))
    {
        goto FreeRefs;
    }

    if ( QueryVrPath()->IsEmpty() )
    {
        DBGPRINTF(( DBG_CONTEXT,
                    "[ReadMetaData] Virtual Dir Path mapping not found\n" ));
        SetLastError( ERROR_FILE_NOT_FOUND );
        return FALSE;
    }

    //
    // If this is an UNC share, logon using associated credentials
    // keep a reference to this access token in the cache
    //

    if ( QueryVrPath()->QueryStr()[0] == '\\' &&
         QueryVrPath()->QueryStr()[1] == '\\' )
    {
        if ( pszVrUserName != NULL && pszVrPassword != NULL &&
             pszVrUserName[0] )
        {

            if ( !SetVrUserNameAndPassword( pInstance, pszVrUserName, pszVrPassword ) )
            {
                return FALSE;
            }
        }
    }

    CheckSignature();
    TsValidateMetaCache();
    return TRUE;

FreeRefs:

    FinishPrivateProperties(&PrivateBuffer, dwPrivateBufferUsed, FALSE);
    CheckSignature();
    TsValidateMetaCache();

    for ( ; i < dwNumMDRecords; i++, pMDRecord++ )
    {
        if ( pMDRecord->dwMDDataTag )
        {
            pmb->ReleaseReferenceData( pMDRecord->dwMDDataTag );
        }
    }

    return FALSE;
}


BOOL
COMMON_METADATA::SetVrUserNameAndPassword(
    PIIS_SERVER_INSTANCE    pInstance,
    LPSTR                   pszUserName,
    LPSTR                   pszPassword
    )
/*++
    Description:

        Set the account used to access the virtual root
        associated with this metadata

    Arguments:
        pInstance - current instance
        pszUserName - User name
        pszPassword - password

    Returns:
        TRUE if success, otherwise FALSE

--*/
{
    LARGE_INTEGER       liPwdExpiry;
    BOOL                fHaveExp;
    BOOL                fAsGuest;
    BOOL                fAsAnonymous;
    TCP_AUTHENT_INFO    TAI;

    CheckSignature();
    TAI.fDontUseAnonSubAuth = TRUE;

    m_hVrToken = TsLogonUser( pszUserName,
                              pszPassword,
                              &fAsGuest,
                              &fAsAnonymous,
                              pInstance,
                              &TAI,
                              NULL,
                              &liPwdExpiry,
                              &fHaveExp );

    //
    // If fail to logo, we remember the error and return SUCCESS
    // Caller will have to check metadata after creating it to check
    // the error code. Necessary because metadata needs to be set in HTTP_REQUEST
    // to send back proper auth status even if virtual root init failed.
    //

    if ( !m_hVrToken )
    {
        m_dwVrError = GetLastError();
    }

    return TRUE;
}


BOOL
COMMON_METADATA::BuildApplPhysicalPath(
  MB *            pmb,
  STR *           pstrApplPhysicalPath
  ) const
/*++
  Description:
    This function builds the physical path for the ApplPath of the current
    METADATA object. The ApplPath is a metbase path of the form
     /LM/W3Svc/<instance>/app-root-path
    This function uses the VR_PATH & portion of the APPL_PATH to
      construct the appropriate physical path.

  Arguments:
    pmb                  - pointer to MB object (Metabase pointer)
    pstrApplPhysicalPath - pointer to STR object that will contain
                           the physical path on return.


  Returns:
    TRUE on success and FALSE if there are errors.
    Use GetLastError() to get the appropriate error code.
--*/
{
    BOOL    fRet;
        INT     cOffSet = 0;

    CheckSignature();
    DBG_ASSERT(NULL != pmb && NULL != pstrApplPhysicalPath);

    //    m_strAppPath is in the format of /LM/W3SVC/X/ROOT/AppRoot.....
        //    Now, the next code segment split this format into
        //    strInstanceMDPath = /LM/W3SVC/X/ROOT/
        //    strVrPath = /AppRoot....
    STR      strInstanceMDPath = STR(m_strAppPath);
    LPSTR    pszVrPath = strInstanceMDPath.QueryStr();
        INT      VrLevel = 0;

    while(*pszVrPath != NULL)
        {
        if ('/' == *pszVrPath)
                {
                        VrLevel++;
                        if (5 == VrLevel)
                        {
                                break;
                        }
                }
                pszVrPath++;
    }

        STR strVrPath = STR(pszVrPath);
    // Case: /LM/W3SVC/1/ROOT
        if (4 == VrLevel)
        {
            strVrPath.Append('/');
                // OffSet needs to minus 1 because strInstanceMDPath does not have
                // ending '/'
                cOffSet = strInstanceMDPath.QueryCCH()-1;
        }
        else if ( 5 == VrLevel)
        {
        // Make a copy of VRPath
            // Caculate the cOffSet of /LM/W3SVC/X/Root/
        // Set the strInstanceMDPath
        cOffSet = DIFF(pszVrPath-strInstanceMDPath.QueryStr());
        strInstanceMDPath.SetLen(cOffSet);
        }
        else
        {
            // Can not resolve Application Physical Path.
                DBG_ASSERT(FALSE);
                pstrApplPhysicalPath->SetLen(0);
        return FALSE;
        }

        // Open the metabase key from /LM/W3SVC/X/ROOT/
    fRet = pmb->Open(strInstanceMDPath.QueryStr());
    if (TRUE == fRet)
    {
            // Get VR Path first. (Note, this is not the final VRpath).
        fRet = pmb->GetStr(strVrPath.QueryStr(),
                        MD_VR_PATH,
                        IIS_MD_UT_FILE,
                        pstrApplPhysicalPath,
                        METADATA_INHERIT,
                        NULL);

        if (TRUE == fRet)
        {
            BOOL    fFound = FALSE;
            DWORD   dwBufferSizeNeeded;
            LPSTR   pszTempMDPath = strVrPath.QueryStr();
            LPSTR   pszTemp = pszTempMDPath + strVrPath.QueryCCH() - 1;
            BOOL    fNoMoreVR = FALSE;  // used to break out infinite loop if there is no VR_PATH
                                        // defined at /LM/W3SVC/X/ROOT

            // Find where we get the VR path
            do
            {
                            dwBufferSizeNeeded = 0;
                if ( !pmb->GetString(strVrPath.QueryStr(),
                                    MD_VR_PATH,
                                    IIS_MD_UT_FILE,
                                                                        NULL,
                                    &dwBufferSizeNeeded,
                                    0
                                    )
                     && GetLastError() == ERROR_INSUFFICIENT_BUFFER )
                {
                    fFound = TRUE;
                                        // cOffSet is the strlen(closest Metabase path that defined
                                        // a MDVrPath property).
                                        // For Example:
                                        // /LM/W3SVC/1/ROOT/VR1/VR2/VR3
                                        // VR2 defines MDVrPath. cOffSet is strlen(/VR2)
                                        // so, based on cOffSet, we can find VR3 later.
                                        cOffSet += strVrPath.QueryCCH();
                }
                 else
                {
                    // '/' is not a DBCS trailing byte.
                                        // going backwords in /VR1/VR2/VR3/.../VRn and search '/'
                                        // and shrink one VR Level
                    pszTemp = strrchr(pszTempMDPath, '/');
                    strVrPath.SetLen(DIFF(pszTemp-pszTempMDPath));
                    // if this is TRUE, then, strVrPath is empty now, and there is no VR_PATH
                    // defined.
                    if (fNoMoreVR)
                    {
                        break;
                    }

                    if (0 == strVrPath.QueryCCH())
                    {
                        fNoMoreVR = TRUE;
                    }
                }
            } while (!fFound);

            pmb->Close();
                    if (!fFound && 0 == strVrPath.QueryCCH())
                        {
                            DBG_ASSERT(FALSE);
                            pstrApplPhysicalPath->SetLen(0);
                            return FALSE;
                        }
            pstrApplPhysicalPath->Append(m_strAppPath.QueryStr()+cOffSet);
            if (pstrApplPhysicalPath->QueryCCH())
                {
                CHAR ch;

                ch = *CharPrev(pstrApplPhysicalPath->QueryStr(),
                               pstrApplPhysicalPath->QueryStr()+pstrApplPhysicalPath->QueryCCH());

                if (ch != '\\' && ch != '/')
                    {
                    pstrApplPhysicalPath->Append("\\");
                    }
                }

            FlipSlashes(pstrApplPhysicalPath->QueryStr());
        }
        else
        {
            pmb->Close();
        }
    }

        return fRet;
} // COMMON_METADATA::BuildApplPhysicalPath()



BOOL
COMMON_METADATA::BuildPhysicalPath(
    LPSTR           pszURL,
    STR *           pstrPhysicalPath
    )
{
    return BuildPhysicalPathWithAltRoot( pszURL, pstrPhysicalPath, NULL );
}


BOOL
COMMON_METADATA::BuildPhysicalPathWithAltRoot(
    LPSTR           pszURL,
    STR *           pstrPhysicalPath,
    PCSTR           pstrAltRoot
    )
/*++
    Description:

        Construct a physical path of the following components:
        - the virtual root mapping
        - if only '/' was used off the URI to match the VR, than insert an alternate root
        - the significant part of the URI

    Arguments:
        pszURL           - The URL to be converted
        pstrPhysicalPath - the resulting physical path
        pstrAltRoot      - the alternat root

    Returns:
        TRUE if success, otherwise FALSE

--*/
{
    LPSTR               pszInVr;
    DWORD               dwL;
    INT                 ch;


    CheckSignature();
    TsValidateMetaCache();

    //
    // Build physical path from VR_PATH & portion of URI not used to define VR_PATH
    //


    //
    // skip the URI components used to locate the virtual root
    //

    pszInVr = pszURL ;
    dwL = QueryVrLevel();
    while ( dwL-- )
    {
        if ( *pszInVr )
        {
            DBG_ASSERT( *pszInVr == '/' || *pszInVr == '\\' );

            ++pszInVr;

            while ( (ch = *pszInVr) && ch != '/' && ch !='\\' )
            {
                pszInVr = CharNext( pszInVr );
            }
        }
    }

    DBG_ASSERT( dwL == (DWORD)-1 );

    if ( !pstrPhysicalPath->Copy( m_strVrPath ) )
    {
        return FALSE;
    }

    //
    // Add the alternate root
    //

    if ( pstrAltRoot &&
         (pszInVr - pszURL) <= 1  )
    {
        //
        // no significant part of the URL has been used as VR alias, so we can add in the
        // alternate root
        //

        if ( pstrPhysicalPath->QueryCCH() )
        {
            //
            // ensure there is one and only one separator
            //

            ch = *CharPrev(pstrPhysicalPath->QueryStr(), pstrPhysicalPath->QueryStr() +
                                                         pstrPhysicalPath->QueryCCH());

            if ( (ch != '/') && (ch != '\\') &&
                 (*pstrAltRoot != '/') && (*pstrAltRoot != '\\') )
            {
                if ( !pstrPhysicalPath->Append( "\\" ) )
                {
                    return FALSE;
                }
            } else if ( (ch == '/' || ch == '\\') &&
                        (*pstrAltRoot == '/' || *pstrAltRoot == '\\') )
            {
                ++pstrAltRoot;
            }
        }

        if ( !pstrPhysicalPath->Append( pstrAltRoot ) )
        {
            return FALSE;
        }
    }

    //
    // Add a path delimiter char between virtual root mount point & significant part of URI
    //

    if ( pstrPhysicalPath->QueryCCH() )
    {
        ch = *CharPrev(pstrPhysicalPath->QueryStr(), pstrPhysicalPath->QueryStr() +
                                                     pstrPhysicalPath->QueryCCH());
        if ( (ch == '/' || ch == '\\') && *pszInVr )
        {
            ++pszInVr;
        }
    }

    if ( !pstrPhysicalPath->Append( pszInVr ) )
    {
        return FALSE;
    }

    //
    // insure physical path last char uses standard directory delimiter
    //

    FlipSlashes( pstrPhysicalPath->QueryStr() );

    CheckSignature();
    TsValidateMetaCache();
    return TRUE;
}


