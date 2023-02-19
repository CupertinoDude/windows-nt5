/*++

Copyright (C) 1999- Microsoft Corporation

Module Name:

    trace.h

Abstract:

    This module declares software tracing stuff

Author:

    Dave Parsons (davepar)

Revision History:


--*/

#ifndef TRACE__H_
#define TRACE__H_

//
// Software tracing setup
//

#define WPP_CONTROL_GUIDS       \
    WPP_DEFINE_CONTROL_GUID(Regular,(E4F5B1AE,0699,41c2,BC5F,DFEEB3A1BAF6), \
    WPP_DEFINE_BIT(Error)       \
    WPP_DEFINE_BIT(Warning)     \
    WPP_DEFINE_BIT(Trace)       \
    WPP_DEFINE_BIT(Entry)       \
    WPP_DEFINE_BIT(Exit)        \
    WPP_DEFINE_BIT(DumpPtp)        \
    )

class CTraceProc {
private:
    CHAR   m_szMessage[MAX_PATH];

public:
    CTraceProc(CHAR *pszMsg);
    ~CTraceProc();
};

VOID DoTraceHresult(HRESULT hr);

#endif // TRACE__H_
