/*++

Copyright (c) 1990  Microsoft Corporation
Copyright (c) 1992  Digital Equipment Corporation

Module Name:

   dataalpha.c

Abstract:

    This module contains the private hardware specific global storage for
    the memory management subsystem.

Author:

    Lou Perazzoli (loup) 27-Mar-1990
    Joe Notarangelo  23-April-1992

Revision History:

--*/

#include "mi.h"

//
// A zero Pte.
//

const MMPTE ZeroPte = {0};

//
// A kernel zero PTE.
//

const MMPTE ZeroKernelPte = {0};

MMPTE ValidKernelPte =
        {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK | MM_PTE_GLOBAL_MASK) ^ MM_PTE_DIRTY_MASK};

const MMPTE ValidKernelPteLocal = {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK) ^ MM_PTE_DIRTY_MASK};

const MMPTE ValidUserPte =
        {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK | MM_PTE_OWNER_MASK) ^ MM_PTE_DIRTY_MASK};

const MMPTE ValidPtePte = {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK) ^ MM_PTE_DIRTY_MASK};

const MMPTE ValidPdePde = {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK) ^ MM_PTE_DIRTY_MASK};

MMPTE ValidKernelPde =
        {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK | MM_PTE_GLOBAL_MASK) ^ MM_PTE_DIRTY_MASK};

const MMPTE ValidKernelPdeLocal = {(MM_PTE_VALID_MASK | MM_PTE_WRITE_MASK) ^ MM_PTE_DIRTY_MASK};

MMPTE DemandZeroPde = {MM_READWRITE << MM_PROTECT_FIELD_SHIFT};

const MMPTE DemandZeroPte = {MM_READWRITE << MM_PROTECT_FIELD_SHIFT};

const MMPTE TransitionPde =
        {MM_PTE_TRANSITION_MASK | (MM_READWRITE << MM_PROTECT_FIELD_SHIFT)};

MMPTE PrototypePte = {(MI_PTE_LOOKUP_NEEDED << 32) | (MM_READWRITE << MM_PROTECT_FIELD_SHIFT) | MM_PTE_PROTOTYPE_MASK};

//
// PTE which generates an access violation when referenced.
//

const MMPTE NoAccessPte = {MM_NOACCESS << MM_PROTECT_FIELD_SHIFT};

//
// Pool start and end.
//

PVOID MmNonPagedPoolStart;

PVOID MmNonPagedPoolEnd = (PVOID)(MM_NONPAGED_POOL_END);

const PVOID MmPagedPoolStart =  (PVOID)(MM_PAGED_POOL_START);

PVOID MmPagedPoolEnd;

//
// PTE reserved for mapping physical data for debugger.
//

PMMPTE MmDebugPte = MiGetPteAddress(0xfffdf000);

//
// 16 PTEs reserved for mapping MDLs (128k max).
//

PMMPTE MmCrashDumpPte = (MiGetPteAddress(MM_NONPAGED_POOL_END));

#if MM_MAXIMUM_NUMBER_OF_COLORS > 1
MMPFNLIST MmFreePagesByPrimaryColor[2][MM_MAXIMUM_NUMBER_OF_COLORS];
#endif

PMMCOLOR_TABLES MmFreePagesByColor[2];

MMPFNLIST MmModifiedPageListByColor[MM_MAXIMUM_NUMBER_OF_COLORS] = {
                            0, ModifiedPageList, MM_EMPTY_LIST, MM_EMPTY_LIST};

//
// Color tables for modified pages destined for the paging file.
//

PFN_NUMBER MmTotalPagesForPagingFile;
