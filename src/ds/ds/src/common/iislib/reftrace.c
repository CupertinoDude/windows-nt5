/*++

Copyright (c) 1997 Microsoft Corporation

Module Name:

    reftrace.c

Abstract:

    This module implements a reference count tracing facility.

Author:

    Keith Moore (keithmo)        01-May-1997

Revision History:

--*/


#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <pudebug.h>
#include <reftrace.h>
#include <stktrace.h>



PTRACE_LOG
CreateRefTraceLog(
    IN LONG LogSize,
    IN LONG ExtraBytesInHeader
    )
/*++

Routine Description:

    Creates a new (empty) ref count trace log buffer.

Arguments:

    LogSize - The number of entries in the log.

    ExtraBytesInHeader - The number of extra bytes to include in the
        log header. This is useful for adding application-specific
        data to the log.

Return Value:

    PTRACE_LOG - Pointer to the newly created log if successful,
        NULL otherwise.

--*/
{

    return CreateTraceLog(
               LogSize,
               ExtraBytesInHeader,
               sizeof(REF_TRACE_LOG_ENTRY)
               );

}   // CreateRefTraceLog


VOID
DestroyRefTraceLog(
    IN PTRACE_LOG Log
    )
/*++

Routine Description:

    Destroys a ref count trace log buffer created with CreateRefTraceLog().

Arguments:

    Log - The ref count trace log buffer to destroy.

Return Value:

    None.

--*/
{

    DestroyTraceLog( Log );

}   // DestroyRefTraceLog


//
// N.B. For PuDbgCaptureContext() to work properly, the calling function
// *must* be __cdecl, and must have a "normal" stack frame. So, we decorate
// PuDbgAssertFailed() with the __cdecl modifier and disable the frame pointer
// omission (FPO) optimization.
//

#pragma optimize( "y", off )    // disable frame pointer omission (FPO)

VOID
WriteRefTraceLog(
    IN PTRACE_LOG Log,
    IN LONG NewRefCount,
    IN PVOID Context
    )
/*++

Routine Description:

    Writes a new entry to the specified ref count trace log. The entry
    written contains the updated reference count and a stack backtrace
    leading up to the current caller.

Arguments:

    Log - The log to write to.

    NewRefCount - The updated reference count.

    Context - An uninterpreted context to associate with the log entry.

Return Value:

    None.

--*/
{

    WriteRefTraceLogEx(
        Log,
        NewRefCount,
        Context,
        (PVOID) -1, // suppress use of optional extra contexts
        (PVOID) -1,
        (PVOID) -1
        );

}   // WriteRefTraceLog




VOID
WriteRefTraceLogEx(
    IN PTRACE_LOG Log,
    IN LONG NewRefCount,
    IN PVOID Context,
    IN PVOID Context1, // optional extra context
    IN PVOID Context2, // optional extra context
    IN PVOID Context3  // optional extra context
    )
/*++

Routine Description:

    Writes a new "extended" entry to the specified ref count trace log.
    The entry written contains the updated reference count, stack backtrace
    leading up to the current caller and extra context information.

Arguments:

    Log - The log to write to.

    NewRefCount - The updated reference count.

    Context  - An uninterpreted context to associate with the log entry.
    Context1 - An uninterpreted context to associate with the log entry.
    Context2 - An uninterpreted context to associate with the log entry.
    Context3 - An uninterpreted context to associate with the log entry.

    NOTE Context1/2/3 are "optional" in that the caller may suppress
    debug display of these values by passing -1 for each of them.

Return Value:

    None

--*/
{

    REF_TRACE_LOG_ENTRY entry;
    ULONG hash;
    DWORD cStackFramesSkipped;

    //
    // Initialize the entry.
    //

    RtlZeroMemory(
        &entry,
        sizeof(entry)
        );

    //
    //  Set log entry members.
    //

    entry.NewRefCount = NewRefCount;
    entry.Context = Context;
    entry.Thread = GetCurrentThreadId();
    entry.Context1 = Context1;
    entry.Context2 = Context2;
    entry.Context3 = Context3;

    //
    // Capture the stack backtrace. Normally, we skip two stack frames:
    // one for this routine, and one for IISCaptureStackBackTrace() itself.
    // For non-Ex callers who come in via WriteRefTraceLog,
    // we skip three stack frames.
    //

    if (    entry.Context1 == (PVOID) -1 
         && entry.Context2 == (PVOID) -1
         && entry.Context3 == (PVOID) -1
         ) {

         cStackFramesSkipped = 3;

    } else {
    
         cStackFramesSkipped = 2;

    }

    IISCaptureStackBackTrace(
        cStackFramesSkipped,
        REF_TRACE_LOG_STACK_DEPTH,
        entry.Stack,
        &hash
        );

    //
    // Write it to the log.
    //

    WriteTraceLog(
        Log,
        &entry
        );

}   // WriteRefTraceLogEx
#pragma optimize( "", on )      // restore frame pointer omission (FPO)

