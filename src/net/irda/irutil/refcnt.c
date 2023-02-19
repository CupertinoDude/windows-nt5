/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

    refcnt.c

Abstract:

    This module exports Reference Counting support functions. By 
    including a Reference Count Control Block (REF_CNT) in a
    dynamic type, and using this API, a Reference scheme can be
    implemented for that type.

Author:

    Edward Buchwalter (v-edbuc)    14-Aug-1996

Revision History:

--*/

//
// Include Files
//

#include "irda.h"

#define EXPAND_TAG(t)   ((CHAR *)(&Tag))[0],((CHAR *)(&Tag))[1],((CHAR *)(&Tag))[2],((CHAR *)(&Tag))[3]
VOID
ReferenceInit 
(
    IN PREF_CNT pRefCnt,
    PVOID       InstanceHandle,
    VOID        (*DeleteHandler)( PVOID )
)

/*++

Routine Description:

    ReferenceInit initializes and adds one reference to the
    supplied Reference Control Block. If provided, an instance
    handle and delete handler are saved for use by the ReferenceRemove 
    function when all references to the instance are removed.

Arguments:

    pRefCnt - pointer to uninitialized Reference Control Block
    InstanceHandle - handle to the managed instance.
    DeleteHandler - pointer to delete function, NULL is OK.

Return Value:

    The function's value is VOID.

--*/

{
    DEBUGMSG( DBG_REF,( TEXT("ReferenceInit( 0x%x, 0x%x, 0x%x )\n"), 
    pRefCnt, InstanceHandle, DeleteHandler ));
    ASSERT( pRefCnt );

    // Set the reference to 1 and save the instance 
    // handle and the delete handler.

    pRefCnt->Count         = 0;
    pRefCnt->Instance      = InstanceHandle;
    pRefCnt->DeleteHandler = DeleteHandler;
    
#if DBG
    pRefCnt->Sig = REF_SIG;

    RtlZeroMemory(pRefCnt->Tags, sizeof(REF_TAG) * TAG_CNT);
    
    pRefCnt->Tags[0].Tag = 'LTOT';
    
    CTEInitLock(&pRefCnt->Lock);
    
#endif
        
}

VOID
ReferenceAdd
(
    IN  PREF_CNT pRefCnt
)

/*++

Routine Description:


Arguments:


Return Value:


--*/

{

    ASSERT( pRefCnt );

    InterlockedIncrement(&pRefCnt->Count);
//    DEBUGMSG( DBG_REF,( TEXT("R+%d\n"), pRefCnt->Count ));    
}

VOID
ReferenceAddCount
(
    IN  PREF_CNT    pRefCnt,
    IN  UINT        Count
)

/*++

Routine Description:


Arguments:


Return Value:


--*/

{
    ASSERT( pRefCnt->Count > 0 );

    CTEInterlockedExchangeAdd(&pRefCnt->Count, Count);
}

PVOID
ReferenceRemove 
(
    IN PREF_CNT  pRefCnt
)

/*++

Routine Description:


Arguments:


Return Value:


--*/

{
    ASSERT( pRefCnt );

    // Trap remove reference on a zero count

    ASSERT( pRefCnt->Count > 0 );

    // If the decremented count is non zero return the instance handle

    if (InterlockedDecrement(&pRefCnt->Count) > 0 )
    {
//        DEBUGMSG( DBG_REF,( TEXT("R-%d\n"), pRefCnt->Count ));        
//        DEBUGMSG( DBG_REF,( TEXT("ReferenceRemove:remaining: %d\n"), pRefCnt->Count ));
        return( pRefCnt->Instance );
    }

    // Delete this instance if a delete handler is available

    if( pRefCnt->DeleteHandler )
    {
        DEBUGMSG( DBG_REF,( TEXT("Executing DeleteHandler\n") ));

        (pRefCnt->DeleteHandler)( pRefCnt->Instance );
    }

    // Indicate no active references to this instance

    return( NULL );
}

//
// API Test Support
//

#if DBG

VOID
ReferenceApiTest( VOID )
{
REF_CNT  RefCnt;

    DEBUGMSG( 1,( TEXT("\nReferenceApiTest\n") ));
    DEBUGMSG( 1,( TEXT("\nTest #1: NULL delete handler\n") ));

    ReferenceInit( &RefCnt, &RefCnt, NULL );

    ReferenceAdd( &RefCnt );
    ReferenceAdd( &RefCnt );
    ReferenceAdd( &RefCnt );

    while( ReferenceRemove( &RefCnt ) )
    {
        ;
    }

    DEBUGMSG( 1,( TEXT("\nTest #2: Delete Handler - TBD\n") ));
}

VOID
ReferenceAddDbg(PREF_CNT pRefCnt, ULONG Tag)
{
    int             i;
    CTELockHandle   hLock;
    int             TotalPerArray = 0;
    
    ASSERT(pRefCnt->Sig == REF_SIG);
    
    DEBUGMSG(DBG_REF, (TEXT("IRREF: add %X (%c%c%c%c) %d\n"),
             pRefCnt, EXPAND_TAG(Tag), pRefCnt->Count));    
    
    CTEGetLock(&pRefCnt->Lock, &hLock);
    
    for (i = 1; i < TAG_CNT; i++)
    {
        if (pRefCnt->Tags[i].Tag == 0 || pRefCnt->Tags[i].Tag == Tag)
        {
            pRefCnt->Tags[i].Tag = Tag;
            InterlockedIncrement(&pRefCnt->Tags[i].Count);
            break;
        }
    }
    
    ASSERT(i < TAG_CNT);
    
    InterlockedIncrement(&pRefCnt->Tags[0].Count);
    
    InterlockedIncrement(&pRefCnt->Count);
    
    ASSERT(pRefCnt->Tags[0].Count == pRefCnt->Count);

    // sanity check
/*    
    for (i = 1; i < TAG_CNT; i++)
    {
        if (pRefCnt->Tags[i].Tag != 0)
        {
            TotalPerArray += pRefCnt->Tags[i].Count;
            continue;
        }
    }

    ASSERT(TotalPerArray == pRefCnt->Tags[0].Count);
    
    if (TotalPerArray != pRefCnt->Tags[0].Count)
    {
        DbgBreakPoint();
    }        
    

*/    
    CTEFreeLock(&pRefCnt->Lock, hLock);
}

VOID
ReferenceRemoveDbg(PREF_CNT pRefCnt, ULONG Tag)
{
    int             i;
    CTELockHandle   hLock;
    int             TotalPerArray = 0;
    BOOLEAN         FoundIt = FALSE;
    
    ASSERT(pRefCnt->Sig == REF_SIG);

    DEBUGMSG(DBG_REF, (TEXT("IRREF: remove %X (%c%c%c%c) %d\n"),
             pRefCnt, EXPAND_TAG(Tag), pRefCnt->Count));
             
    CTEGetLock(&pRefCnt->Lock, &hLock);
        
    for (i = 1; i < TAG_CNT; i++)
    {
        if (pRefCnt->Tags[i].Tag == Tag)
        {
            FoundIt = TRUE;
            
            ASSERT(pRefCnt->Tags[i].Count > 0);
            
            InterlockedDecrement(&pRefCnt->Tags[i].Count);
            if (pRefCnt->Tags[i].Count == 0)
                pRefCnt->Tags[i].Tag = Tag; 
            break;
        }
    }

    ASSERT(FoundIt);
  
    ASSERT(pRefCnt->Tags[0].Count > 0);
        
    ASSERT(pRefCnt->Tags[0].Count == pRefCnt->Count);
      
    InterlockedDecrement(&pRefCnt->Tags[0].Count);
    
    if (InterlockedDecrement(&pRefCnt->Count) > 0 )
    {
        CTEFreeLock(&pRefCnt->Lock, hLock);
    } 
    else if (pRefCnt->DeleteHandler)
    {
        DEBUGMSG( DBG_REF,( TEXT("Executing DeleteHandler\n") ));
        
        CTEFreeLock(&pRefCnt->Lock, hLock);
        
        (pRefCnt->DeleteHandler)( pRefCnt->Instance );
    }
    else
    {
        CTEFreeLock(&pRefCnt->Lock, hLock);   
    }
        
/*
    // sanity check
    for (i = 1; i < TAG_CNT; i++)
    {
        if (pRefCnt->Tags[i].Tag != 0)
        {
            TotalPerArray += pRefCnt->Tags[i].Count;
            continue;
        }
    }
    
    ASSERT(TotalPerArray == pRefCnt->Tags[0].Count);
    
    if (TotalPerArray != pRefCnt->Tags[0].Count)
    {
        DbgPrint(TEXT("Tag %X, RefCnt %X, perArray %d, total %d\n"), Tag, pRefCnt,
                  TotalPerArray, pRefCnt->Tags[0].Count);
                  
        DbgBreakPoint();
    }    
    

*/    
}

#endif
