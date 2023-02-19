/*++

Copyright (c) 2001  Microsoft Corporation

Module Name:

    vbajet32.c

Abstract:

   This module implements stub functions for shell32 interfaces.

Author:

    David N. Cutler (davec) 11-Jun-2001

Environment:

    Kernel mode only.

Revision History:

--*/

#include "windows.h"

#define STUBFUNC(x)     \
int                     \
x(                      \
    void                \
    )                   \
{                       \
    return 0;           \
}

STUBFUNC(LoadExprSrvDll)
STUBFUNC(VBAGetExprSrv)
