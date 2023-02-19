/*++

Copyright (c) 1997 Microsoft Corporation

Module Name:

  macros.h

Abstract:

  This module contains the global macros

--*/

#ifndef _MACROS_H
#define _MACROS_H

HANDLE  g_HeapHandle;  // g_HeapHandle is the global handle to the heap

// MemInitializeMacro is a macro to get the handle to the heap
#define MemInitializeMacro(hHeap) (g_HeapHandle = hHeap)

// MemAllocMacro is a macro to allocate dwBytes bytes of memory from the heap
#define MemAllocMacro(dwBytes) (HeapAlloc(g_HeapHandle, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, dwBytes))

// MemReAllocMacro is a macro to reallocate dwBytes bytes of memory from the heap
#define MemReAllocMacro(lpMem, dwBytes) (HeapReAlloc(g_HeapHandle, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, lpMem, dwBytes))

// MemFreeMacro is a macro to free a memory block allocated from the heap
#define MemFreeMacro(lpMem) (HeapFree(g_HeapHandle, 0, lpMem))

#endif
