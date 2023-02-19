/*++

Copyright (c) 2000 Microsoft Corporation

Module Name:

    entry.c

Abstract:

    Implements the DLL entry point that provides all Cobra module entry points
    to the engine.

Author:

    Jim Schmidt (jimschm) 11-Aug-2000

Revision History:

    <alias> <date> <comments>

--*/

#include "pch.h"
#include "unctrans.h"

typedef struct {
    PCTSTR Name;
    TRANSPORT_ENTRYPOINTS EntryPoints;
} TRANSPORT_TABLE, *PTRANSPORT_TABLE;

#define NOPROGBAR NULL
#define NORESET NULL
#define NORESUME NULL

//
// Add an entry for each transport module in the DLL
//

TRANSPORT_TABLE g_TransportEntryPoints[] = {
    {   TEXT("INF_BASED_UNC_TRANSPORT"), ISM_VERSION,
        InfTransTransportInitialize,
        InfTransTransportEstimateProgressBar,
        InfTransTransportQueryCapabilities,
        InfTransTransportSetStorage,
        NORESET,
        InfTransTransportTerminate,
        InfTransTransportSaveState,
        NORESUME,
        InfTransTransportBeginApply,
        NORESUME,
        InfTransTransportAcquireObject,
        InfTransTransportReleaseObject,
        InfTransTransportEndApply
    },

    {   TEXT("OPAQUE_UNC_TRANSPORT"), ISM_VERSION,
        OpaqueTransportInitialize,
        OpaqueTransportEstimateProgressBar,
        OpaqueTransportQueryCapabilities,
        OpaqueTransportSetStorage,
        NORESET,
        OpaqueTransportTerminate,
        OpaqueTransportSaveState,
        NORESUME,
        OpaqueTransportBeginApply,
        NORESUME,
        OpaqueTransportAcquireObject,
        OpaqueTransportReleaseObject,
        OpaqueTransportEndApply
    },

    {NULL}
};

EXPORT
BOOL
WINAPI
DllMain (
    IN      HINSTANCE hInstance,
    IN      DWORD dwReason,
    IN      LPVOID lpReserved
    )
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        g_hInst = hInstance;
    }

    return TRUE;
}


EXPORT
BOOL
WINAPI
ModuleInitialize (
    VOID
    )
{
    TCHAR memDbDir[MAX_PATH];

    UtInitialize (NULL);
    FileEnumInitialize ();

    IsmGetTempDirectory (memDbDir, ARRAYSIZE (memDbDir));
    if (!MemDbInitializeEx (memDbDir)) {
        DEBUGMSG ((DBG_ERROR, "Failing to initialize unc transports because MemDb failed to initialize"));
        IsmSetCancel();
        return FALSE;
    }

    InfGlobalInit (FALSE);

    return TRUE;
}

EXPORT
VOID
WINAPI
ModuleTerminate (
    VOID
    )
{
    InfGlobalInit (TRUE);
    MemDbTerminateEx (TRUE);
    FileEnumTerminate ();
    UtTerminate ();
}


BOOL
WINAPI
pFindModule (
    IN      PCTSTR ModuleId,
    OUT     PVOID IsmBuffer,
    IN      PCTSTR *TableEntry,
    IN      UINT StructureSize
    )
{

    while (*TableEntry) {
        if (StringIMatch (*TableEntry, ModuleId)) {
            CopyMemory (
                IsmBuffer,
                (PBYTE) (TableEntry + 1),
                StructureSize
                );
            return TRUE;
        }

        TableEntry = (PCTSTR *) ((PBYTE) (TableEntry + 1) + StructureSize);
    }

    return FALSE;
}



EXPORT
BOOL
WINAPI
TransportModule (
    IN      PCTSTR ModuleId,
    IN OUT  PTRANSPORT_ENTRYPOINTS TransportEntryPoints
    )
{
    return pFindModule (
                ModuleId,
                (PVOID) TransportEntryPoints,
                (PCTSTR *) g_TransportEntryPoints,
                sizeof (TRANSPORT_ENTRYPOINTS)
                );
}
