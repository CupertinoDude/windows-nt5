/*++

Copyright (c) 1998, Microsoft Corporation

Module Name:

    compref.h

Abstract:

    This module contains declarations for maintaining reference-count
    on a component. It provides an asynchronous thread-safe means of
    handling cleanup in a module.

    The mechanism defined uses a locked reference count and cleanup-routine
    to manage the lifetime of the component. When the reference-count
    is dropped to zero, the associated cleanup-routine is invoked.

Author:

    Abolade Gbadegesin (aboladeg)   6-Mar-1998

Revision History:

--*/

#ifndef _NATHLP_COMPREF_H_
#define _NATHLP_COMPREF_H_

typedef
VOID
(*PCOMPONENT_CLEANUP_ROUTINE)(
    VOID
    );

//
// Structure:   COMPONENT_REFERENCE
//
// This structure must reside in memory for the lifetime of the component
// to which it refers. It is used to synchronize the component's execution.
//

typedef struct _COMPONENT_REFERENCE {
    CRITICAL_SECTION Lock;
    ULONG ReferenceCount;
    BOOLEAN Deleted;
    PCOMPONENT_CLEANUP_ROUTINE CleanupRoutine;
#if COMPREF_TRACKING
    struct _COMPREF_RECORD* RecordArray;
    ULONG RecordIndex;
#endif
} COMPONENT_REFERENCE, *PCOMPONENT_REFERENCE;

#if COMPREF_TRACKING
#define COMPREF_RECORD_COUNT    (1024)
typedef struct _COMPREF_RECORD {
    PCHAR File;
    USHORT Line;
    UCHAR ReferenceCount;
    enum {
        ComprefAcquireRecord,
        ComprefReleaseRecord
    } Type;
} COMPREF_RECORD, *PCOMPREF_RECORD;
#endif


//
// FUNCTION DECLARATIONS
//

__inline
BOOLEAN
AcquireComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    );

VOID
__inline
DeleteComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    );

ULONG
__inline
InitializeComponentReference(
    PCOMPONENT_REFERENCE ComponentReference,
    PCOMPONENT_CLEANUP_ROUTINE CleanupRoutine
    );

#if COMPREF_TRACKING
__inline
BOOLEAN
RecordComponentReference(
    PCOMPONENT_REFERENCE ComponentReference,
    PCHAR File,
    ULONG Line,
    UCHAR Type
    );
#endif

__inline
BOOLEAN
ReleaseComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    );

__inline
BOOLEAN
ReleaseInitialComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    );

__inline
VOID
ResetComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    );

//
// MACRO DECLARATIONS
//

#define RETURN_VOID

#if COMPREF_TRACKING

#define REFERENCE_COMPONENT(c) \
    RecordComponentReference(c, __FILE__, __LINE__, ComprefAcquireRecord)

#define DEREFERENCE_COMPONENT(c) \
    RecordComponentReference(c, __FILE__, __LINE__, ComprefReleaseRecord)

#define REFERENCE_COMPONENT_OR_RETURN(c,retcode) \
    if (!RecordComponentReference(c, __FILE__, __LINE__, ComprefAcquireRecord)) { return retcode; }

#define DEREFERENCE_COMPONENT_AND_RETURN(c,retcode) \
    RecordComponentReference(c, __FILE__, __LINE__, ComprefReleaseRecord); \
    return retcode

#else

#define REFERENCE_COMPONENT(c) \
    AcquireComponentReference(c)

#define DEREFERENCE_COMPONENT(c) \
    ReleaseComponentReference(c)

#define REFERENCE_COMPONENT_OR_RETURN(c,retcode) \
    if (!AcquireComponentReference(c)) { return retcode; }

#define DEREFERENCE_COMPONENT_AND_RETURN(c,retcode) \
    ReleaseComponentReference(c); return retcode

#endif


//
// INLINE ROUTINE IMPLEMENTATIONS
//

__inline
BOOLEAN
AcquireComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    )

/*++

Routine Description:

    This routine is called to increment the reference-count to a component.
    The attempt may fail if the initial reference has been released
    and the component is therefore being deleted.

Arguments:

    ComponentReference - the component to be referenced

Return Value:

    BOOLEAN - TRUE if the component was referenced, FALSE otherwise.

--*/

{
    EnterCriticalSection(&ComponentReference->Lock);
    if (ComponentReference->Deleted) {
        LeaveCriticalSection(&ComponentReference->Lock);
        return FALSE;
    }
    ++ComponentReference->ReferenceCount;
    LeaveCriticalSection(&ComponentReference->Lock);
    return TRUE;

} // AcquireComponentReference


VOID
__inline
DeleteComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    )

/*++

Routine Description:

    This routine is called to destroy a component reference.
    It may only be called after the last reference to the component is released,
    i.e. after 'ReleaseComponentReference' has returned 'TRUE'.
    It may also be called from within the component's 'CleanupRoutine'.

Arguments:

    ComponentReference - the component to be destroyed

Return Value:

    none.

--*/

{
    DeleteCriticalSection(&ComponentReference->Lock);
#if COMPREF_TRACKING
    HeapFree(GetProcessHeap(), 0, ComponentReference->RecordArray);
#endif

} // DeleteComponentReference


ULONG
__inline
InitializeComponentReference(
    PCOMPONENT_REFERENCE ComponentReference,
    PCOMPONENT_CLEANUP_ROUTINE CleanupRoutine
    )

/*++

Routine Description:

    This routine is called to initialize a component reference.

Arguments:

    ComponentReference - the component to be initialized

    CleanupRoutine - the routine to be called when the component
        is to be cleaned up (within the final 'ReleaseComponentReference').

Return Value:

    none.

--*/

{
    __try {
        InitializeCriticalSection(&ComponentReference->Lock);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return GetExceptionCode();
    }
    ComponentReference->Deleted = FALSE;
    ComponentReference->ReferenceCount = 1;
    ComponentReference->CleanupRoutine = CleanupRoutine;
#if COMPREF_TRACKING
    ComponentReference->RecordIndex = 0;
    ComponentReference->RecordArray =
        HeapAlloc(
            GetProcessHeap(), 0, sizeof(COMPREF_RECORD) * COMPREF_RECORD_COUNT
            );
#endif
    return NO_ERROR;

} // InitializeComponentReference



#if COMPREF_TRACKING
__inline
BOOLEAN
RecordComponentReference(
    PCOMPONENT_REFERENCE ComponentReference,
    PCHAR File,
    ULONG Line,
    UCHAR Type
    )
{
    BOOLEAN Success;
    ULONG i = InterlockedIncrement(&ComponentReference->RecordIndex);
    i %= COMPREF_RECORD_COUNT;
    ComponentReference->RecordArray[i].File = File;
    ComponentReference->RecordArray[i].Line = (USHORT)Line;
    ComponentReference->RecordArray[i].Type = Type;
    if (Type == ComprefAcquireRecord) {
        Success = AcquireComponentReference(ComponentReference);
    } else {
        Success = ReleaseComponentReference(ComponentReference);
    }
    ComponentReference->RecordArray[i].ReferenceCount =
        (UCHAR)ComponentReference->ReferenceCount;
    return Success;
}

#endif

__inline
BOOLEAN
ReleaseComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    )

/*++

Routine Description:

    This routine is called to drop a reference to a component.
    If the reference drops to zero, cleanup is performed.
    Otherwise, cleanup occurs later when the last reference is released.

Arguments:

    ComponentReference - the component to be referenced

Return Value:

    BOOLEAN - TRUE if the component was cleaned up, FALSE otherwise.

--*/

{
    EnterCriticalSection(&ComponentReference->Lock);
    if (--ComponentReference->ReferenceCount) {
        LeaveCriticalSection(&ComponentReference->Lock);
        return FALSE;
    }
    LeaveCriticalSection(&ComponentReference->Lock);
    ComponentReference->CleanupRoutine();
    return TRUE;
} // ReleaseComponentReference


__inline
BOOLEAN
ReleaseInitialComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    )

/*++

Routine Description:

    This routine is called to drop the initial reference to a component,
    and mark the component as deleted.
    If the reference drops to zero, cleanup is performed right away.

Arguments:

    ComponentReference - the component to be referenced

Return Value:

    BOOLEAN - TRUE if the component was cleaned up, FALSE otherwise.

--*/

{
    EnterCriticalSection(&ComponentReference->Lock);
    if (ComponentReference->Deleted) {
        LeaveCriticalSection(&ComponentReference->Lock);
        return TRUE;
    }
    ComponentReference->Deleted = TRUE;
    if (--ComponentReference->ReferenceCount) {
        LeaveCriticalSection(&ComponentReference->Lock);
        return FALSE;
    }
    LeaveCriticalSection(&ComponentReference->Lock);
    ComponentReference->CleanupRoutine();
    return TRUE;
} // ReleaseInitialComponentReference


__inline
VOID
ResetComponentReference(
    PCOMPONENT_REFERENCE ComponentReference
    )

/*++

Routine Description:

    This routine is called to reset a component reference
    to an initial state.

Arguments:

    ComponentReference - the component to be reset

Return Value:

    none.

--*/

{
    EnterCriticalSection(&ComponentReference->Lock);
    ComponentReference->ReferenceCount = 1;
    ComponentReference->Deleted = FALSE;
#if COMPREF_TRACKING
    ComponentReference->RecordIndex = 0;
    ZeroMemory(
        ComponentReference->RecordArray,
        sizeof(COMPREF_RECORD) * COMPREF_RECORD_COUNT
        );
#endif
    LeaveCriticalSection(&ComponentReference->Lock);
} // ReleaseComponentReference



#endif // _NATHLP_COMPREF_H_
