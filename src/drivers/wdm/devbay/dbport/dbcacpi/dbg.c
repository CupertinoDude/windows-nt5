/*++

Copyright (c) 1998  Microsoft Corporation

Module Name:

    DBG.C

Abstract:

    This module contains debug only code for DBC driver

Author:

    jdunn

Environment:

    kernel mode only

Notes:


Revision History:

    11-5-96 : created

--*/
#include <wdm.h>
#include "stdarg.h"
#include "stdio.h"
#include "dbci.h"
#include "dbcacpi.h"

#ifdef MAX_DEBUG
#define DEBUG_HEAP
#endif

typedef struct _HEAP_TAG_BUFFER {
    ULONG Sig;
    ULONG Length;
} HEAP_TAG_BUFFER, *PHEAP_TAG_BUFFER;


#if DBG 

extern ULONG DBCACPI_Debug_Trace_Level;
extern ULONG DBCACPI_W98_Debug_Trace;

VOID
DBCACPI_Assert(
    IN PVOID FailedAssertion,
    IN PVOID FileName,
    IN ULONG LineNumber,
    IN PCHAR Message
    )
/*++

Routine Description:

    Debug Assert function. 

Arguments:

    DeviceObject - pointer to a device object

    Irp          - pointer to an I/O Request Packet

Return Value:


--*/
{
#ifdef NTKERN  
    // this makes the compiler generate a ret
    ULONG stop = 1;
    
assert_loop:
#endif
    // just call the NT assert function and stop
    // in the debugger.
    RtlAssert( FailedAssertion, FileName, LineNumber, Message );

    // loop here to prevent users from going past
    // are assert before we can look at it
#ifdef NTKERN    
    TRAP();
    if (stop) {
        goto assert_loop;
    }        
#endif
    return;
}


ULONG
_cdecl
DBCACPI_KdPrintX(
    ULONG l,
    PCH Format,
    ...
    )
{
    va_list list;
    int i;
    int arg[6];
    
    if (DBCACPI_Debug_Trace_Level >= l) {    
        if (l <= 1) {
            // this code is useful for debugging on win9x where
            // the ntkernel emulator has a trace buffer...
            //
            // override the ntkern trace buffer for 
            // level 1 output -- ie go to terminal
            if (DBCACPI_W98_Debug_Trace) {    
                DbgPrint("DBCACPI.SYS: ");
                *Format = ' ';
            } else {
                DbgPrint("'DBCACPI.SYS: ");
            }
        } else {
            DbgPrint("'DBCACPI.SYS: ");
        }
        va_start(list, Format);
        for (i=0; i<6; i++) 
            arg[i] = va_arg(list, int);
        
        DbgPrint(Format, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);    
    } 

    return 0;
}


NTSTATUS
DBCACPI_GetClassGlobalDebugRegistryParameters(
    )
/*++

Routine Description:

Arguments:

Return Value:

--*/
{
    NTSTATUS ntStatus;
    RTL_QUERY_REGISTRY_TABLE QueryTable[3];
    PWCHAR usb = L"class\\dbc";

    PAGED_CODE();
    
    //
    // Set up QueryTable to do the following:
    //

    // spew level
    QueryTable[0].QueryRoutine = DBCACPI_GetConfigValue;
    QueryTable[0].Flags = 0;
    QueryTable[0].Name = DEBUG_LEVEL_KEY;
    QueryTable[0].EntryContext = &DBCACPI_Debug_Trace_Level;
    QueryTable[0].DefaultType = REG_DWORD;
    QueryTable[0].DefaultData = &DBCACPI_Debug_Trace_Level;
    QueryTable[0].DefaultLength = sizeof(DBCACPI_Debug_Trace_Level);

    // ntkern trace buffer
    QueryTable[1].QueryRoutine = DBCACPI_GetConfigValue;
    QueryTable[1].Flags = 0;
    QueryTable[1].Name = DEBUG_WIN9X_KEY;
    QueryTable[1].EntryContext = &DBCACPI_W98_Debug_Trace;
    QueryTable[1].DefaultType = REG_DWORD;
    QueryTable[1].DefaultData = &DBCACPI_W98_Debug_Trace;
    QueryTable[1].DefaultLength = sizeof(DBCACPI_W98_Debug_Trace);
    
    //
    // Stop
    //
    QueryTable[2].QueryRoutine = NULL;
    QueryTable[2].Flags = 0;
    QueryTable[2].Name = NULL;

    ntStatus = RtlQueryRegistryValues(
                RTL_REGISTRY_SERVICES,
                usb,
                QueryTable,     // QueryTable
                NULL,           // Context
                NULL);          // Environment

    if (NT_SUCCESS(ntStatus)) {
         DBCACPI_KdPrint((1, "'Debug Trace Level Set: (%d)\n", DBCACPI_Debug_Trace_Level));
  
        if (DBCACPI_W98_Debug_Trace) {
            DBCACPI_KdPrint((1, "'NTKERN Trace is ON\n"));
        } else {
            DBCACPI_KdPrint((1, "'NTKERN Trace is OFF\n"));
        }
    
        if (DBCACPI_Debug_Trace_Level > 0) {
            ULONG DBCACPI_Debug_Asserts = 1;
        }
    }
    
    if ( STATUS_OBJECT_NAME_NOT_FOUND == ntStatus ) {
        ntStatus = STATUS_SUCCESS;
    }

    return ntStatus;
}


PVOID
DBCACPI_GetHeap(
    IN POOL_TYPE PoolType,
    IN ULONG NumberOfBytes,
    IN ULONG Signature,
    IN PLONG TotalAllocatedHeapSpace
    )
/*++

Routine Description:

    Debug routine, used to debug heap problems.  We are using this since 
    most NT debug functions are not supported by NTKERN.
    
Arguments:

    PoolType - pool type passed to ExAllocatePool
    
    NumberOfBytes - number of bytes for item

    Signature - four byte signature supplied by caller

    TotalAllocatedHeapSpace - pointer to variable where client stores
                the total accumulated heap space allocated.

Return Value:

    pointer to allocated memory

--*/
{
    PUCHAR p;
#ifdef DEBUG_HEAP
    PHEAP_TAG_BUFFER tagBuffer;
    
    // we call ExAllocatePoolWithTag but no tag will be added
    // when running under NTKERN

    p = (PUCHAR) ExAllocatePoolWithTag(PoolType,
                                       NumberOfBytes + sizeof(HEAP_TAG_BUFFER)*2,
                                       Signature);

    if (p) {
        tagBuffer = (PHEAP_TAG_BUFFER) p;
        tagBuffer->Sig = Signature;
        tagBuffer->Length = NumberOfBytes;        
        p += sizeof(HEAP_TAG_BUFFER);
        *TotalAllocatedHeapSpace += NumberOfBytes;

        tagBuffer = (PHEAP_TAG_BUFFER) (p + NumberOfBytes);
        tagBuffer->Sig = Signature;
        tagBuffer->Length = NumberOfBytes;     
    }                                            

//    LOGENTRY(LOG_MISC, (PUCHAR) &Signature, 0, 0, 0);
//    LOGENTRY(LOG_MISC, "GetH", p, NumberOfBytes, stk[1] & 0x00FFFFFF);
#else    
    p = (PUCHAR) ExAllocatePoolWithTag(PoolType,
                                       NumberOfBytes,
                                       Signature);

#endif /* DEBUG_HEAP */                
    return p;
}


VOID
UsbhRetHeap(
    IN PVOID P,
    IN ULONG Signature,
    IN PLONG TotalAllocatedHeapSpace
    )
/*++

Routine Description:

    Debug routine, used to debug heap problems.  We are using this since 
    most NT debug functions are not supported by NTKERN.
    
Arguments:

    P - pointer to free

Return Value:

    none.

--*/
{
#ifdef DEBUG_HEAP
    PHEAP_TAG_BUFFER endTagBuffer;    
    PHEAP_TAG_BUFFER beginTagBuffer;

    DBCACPI_ASSERT(P != 0);
    
    beginTagBuffer = (PHEAP_TAG_BUFFER) ((PUCHAR)P  - sizeof(HEAP_TAG_BUFFER));
    endTagBuffer = (PHEAP_TAG_BUFFER) ((PUCHAR)P + beginTagBuffer->Length);

    *TotalAllocatedHeapSpace -= beginTagBuffer->Length;

//    LOGENTRY(LOG_MISC, (PUCHAR) &Signature, 0, 0, 0);    
//    LOGENTRY(LOG_MISC, "RetH", P, tagBuffer->Length, stk[1] & 0x00FFFFFF);

    DBCACPI_ASSERT(*TotalAllocatedHeapSpace >= 0);
    DBCACPI_ASSERT(beginTagBuffer->Sig == Signature);
    DBCACPI_ASSERT(endTagBuffer->Sig == Signature);
    DBCACPI_ASSERT(endTagBuffer->Length == beginTagBuffer->Length);
    
    // fill the buffer with bad data
    RtlFillMemory(P, beginTagBuffer->Length, 0xff);
    beginTagBuffer->Sig = DBCACPI_FREE_TAG;

    // free the original block
    ExFreePool(beginTagBuffer);    
#else
    ExFreePool(P);        
#endif /* DEBUG_HEAP */
}

#endif /* DBG */

#ifdef DEBUG_LOG

KSPIN_LOCK LogSpinLock;

struct DBG_LOG_ENTRY {
    CHAR    le_name[4];        // Identifying string
    ULONG    le_info1;        // entry specific info
    ULONG    le_info2;        // entry specific info
    ULONG    le_info3;        // entry specific info
}; /* USBD_LOG_ENTRY */


struct DBG_LOG_ENTRY *DbcALStart = 0;    // No log yet
struct DBG_LOG_ENTRY *DbcALPtr;
struct DBG_LOG_ENTRY *DbcALEnd;
#ifdef PROFILE
ULONG LogMask = LOG_PROFILE;
#else 
ULONG LogMask = 0xFFFFFFFF;
#endif

VOID
DBCACPI_Debug_LogEntry(
    IN ULONG Mask,
    IN CHAR *Name, 
    IN ULONG Info1, 
    IN ULONG Info2, 
    IN ULONG Info3
    )
/*++

Routine Description:

    Adds an Entry to USBH log.

Arguments:

Return Value:

    None.

--*/
{
    KIRQL irql;

    if (DbcALStart == 0) {
        return;
    }        

    if ((Mask & LogMask) == 0) {
        return;
    }

    irql = KeGetCurrentIrql();
    if (irql < DISPATCH_LEVEL) {
        KeAcquireSpinLock(&LogSpinLock, &irql);
    } else {
        KeAcquireSpinLockAtDpcLevel(&LogSpinLock);
    }        
    
    if (DbcALPtr > DbcALStart) {
        DbcALPtr -= 1;    // Decrement to next entry
    } else {
        DbcALPtr = DbcALEnd;
    }        

    DBCACPI_ASSERT(DbcALPtr >= DbcALStart);
    
    RtlCopyMemory(DbcALPtr->le_name, Name, 4);
//    LPtr->le_ret = (stk[1] & 0x00ffffff) | (CurVMID()<<24);
    DbcALPtr->le_info1 = Info1;
    DbcALPtr->le_info2 = Info2;
    DbcALPtr->le_info3 = Info3;

    if (irql < DISPATCH_LEVEL) {
        KeReleaseSpinLock(&LogSpinLock, irql);
    } else {
        KeReleaseSpinLockFromDpcLevel(&LogSpinLock);
    }        

    return;
}


VOID
DBCACPI_LogInit(
    )
/*++

Routine Description:

    Init the debug log - remember interesting information in a circular buffer

Arguments:
    
Return Value:

    None.

--*/
{
#ifdef MAX_DEBUG
    ULONG logSize = 4096*6;    
#else
    ULONG logSize = 4096*3;    
#endif

    
    KeInitializeSpinLock(&LogSpinLock);

    DbcALStart = ExAllocatePool(NonPagedPool, 
                               logSize); 

    if (DbcALStart) {
        DbcALPtr = DbcALStart;

        // Point the end (and first entry) 1 entry from the end of the segment
        DbcALEnd = DbcALStart + (logSize / sizeof(struct DBG_LOG_ENTRY)) - 1;
    } else {
        TRAP(); //(("no mem for log!\n"));
    }

    return;
}

VOID
DBCACPI_LogFree(
    )
/*++

Routine Description:

    Init the debug log - remember interesting information in a circular buffer

Arguments:
    
Return Value:

    None.

--*/
{
    if (DbcALStart) {
        ExFreePool(DbcALStart);
    }
}

#endif /* DEBUG_LOG */


