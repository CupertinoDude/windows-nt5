/*++

Copyright (c) 1996 Microsoft Corporation

Module Name:

    precomp.hxx

Abstract:

    Master include file for the IIS Connection Module

Author:

    Saurab Nog (SaurabN)       08-Feb-1999

Revision History:

--*/



#ifndef _PRECOMP_H_
#define _PRECOMP_H_

//
// System related headers
//
#include <iis.h>
#include <ul.h>

# include "dbgutil.h"

//
// General C runtime libraries
//
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

//
// Headers for this project
//

#include <iModule.hxx>
#include <iWorkerRequest.hxx>

#include "ConnectionRecord.hxx"
#include "ConnectionHashTable.hxx"
#include "ConnectionModule.hxx"

#endif  // _PRECOMP_H_

