/*++

Copyright (c) 1995 Microsoft Corporation

Module Name:

    stdh.h

Abstract:

    Standard include file for QM componenet

Author:

    Erez Haba (erezh) 17-Jan-96

--*/

#ifndef __STDH_H
#define __STDH_H

#include <_stdh.h>

#include "transact.h"

#include <mqsymbls.h>
#include <mqprops.h>
#include <mqtypes.h>
#include <_mqdef.h>
#include <mqutil.h>
#include <cs.h>
#include <mqlog.h>

// [adsrv] Globally-available functions for finding out current machine capabilities
bool IsRoutingServer(void);
bool IsDepClientsServer(void);
bool IsNonServer(void);

#endif // __STDH_H

