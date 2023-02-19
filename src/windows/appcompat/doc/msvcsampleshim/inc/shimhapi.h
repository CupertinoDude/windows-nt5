////////////////////////////////////////////////////////////////////////////////////
//
// File:    shim.h
//
// History:    May-99   clupu       Created.
//             Aug-99   v-johnwh    Various bug fixes.
//          23-Nov-99   markder     Support for multiple shim DLLs, chaining
//                                  of hooks. General clean-up.
//          11-Feb-00   markder     Reverted to W2K shipped shim structures.
// 
// Desc:    Contains all structure and function definitions for the shim mechanism.
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SHIM_H__
#define __SHIM_H__

typedef struct tagHOOKAPI {
    
    char*   pszModule;          // the name of the module
    char*   pszFunctionName;    // the name of the API in the module
    PVOID   pfnNew;             // pointer to the new stub API
    PVOID   pfnOld;             // pointer to the old API
    DWORD   dwIndex;            // used internally
    char*   pszModToPatch;      // list of modules to hook. NULL means all.

} HOOKAPI, *PHOOKAPI;

/*
 * If the hook DLL ever patches LoadLibraryA/W it must call PatchNewModules
 * so that the shim knows to patch any new loaded DLLs
 */
typedef VOID (*PFNPATCHNEWMODULES)(VOID);

typedef PHOOKAPI (*PFNGETHOOKAPIS)(LPSTR pszCmdLine,
                                   PFNPATCHNEWMODULES pfnPatchNewModules,
                                   DWORD* pdwHooksCount);

#endif