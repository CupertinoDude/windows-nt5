/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       newdpf.h
 *  Content:    new debug printf
 *@@BEGIN_MSINTERNAL
 *  History:
 *   Date       By      Reason
 *   ====       ==      ======
 *   10-oct-95  jeffno  initial implementation
 *   15-jun-98  linstev added DPF2
 *@@END_MSINTERNAL
 *
 ***************************************************************************/

#ifndef __DEBUGPF_H
#define __DEBUGPF_H

#include <stdarg.h>
#include <windows.h>

#ifndef DPF_MODNAME
    extern char * DPF_MODNAME;
#endif

#if defined(DEBUG) || defined(DBG)

    #undef DEBUG_TOPIC
    #define DEBUG_TOPIC(flag,name) flag,
    typedef enum DEBUG_TOPICS {
        A=1,    /* For API Usage */
    #include "DBGTOPIC.H"
        LAST_TOPIC
    };

    #line 21
/*
    #undef DBG
    #define DBG 1
    */
    #undef DEBUG
    #define DEBUG
    #if defined( _WIN32 ) && defined(WIN95)
        #define DEBUG_BREAK()       _try { _asm { int 3 } } _except (EXCEPTION_EXECUTE_HANDLER) {;}
    #else
        #define DEBUG_BREAK()       DebugBreak()
    #endif


    /*
     * DEBUG_TOPIC
     * This structure describes a debugging topic and associates a single-letter key to it.
     */

    #define DEBUG_TOPIC_NAME_LENGTH 59
    typedef struct
    {
        DWORD   dwSize; //for versioning
        char    cKey;
        char    cName[DEBUG_TOPIC_NAME_LENGTH];
    } DPF_DEBUG_TOPIC;

    /*
     * PROC_STATS
     * This struct saves statistics about each proc as maintained by DebugEnterProc
     */
    typedef struct
    {
        char            cName[DEBUG_TOPIC_NAME_LENGTH];
        DWORD           dwCalls;
#ifdef WIN32
        LARGE_INTEGER   liTotalTime;
        LARGE_INTEGER   liLastEnteredAt;
#else
        DWORD           dwFiller[8];
#endif
    } DPF_PROC_STATS;

    #define MAX_PROC_ORDINAL 100

#ifdef __cplusplus
        extern "C" {
#endif
    /*
     * Debugging output/flow-control calls
     */
   extern void DebugSetTopicsAndLevels(char * pcTopicsAndLevelsToDisplay);
   extern void DebugHalt(void);       //Break if control level allows
   extern int  DebugSetFileLineEtc(LPSTR szFile, DWORD dwLineNumber,LPSTR szFnName);
   extern int  DebugPrintf(DWORD dwDetail, ...);
   extern void DebugSetMute(BOOL bMuteFlag);
   extern void DebugPrintfInit(void);
   extern void DebugEnterAPI(char *,LPDWORD);
   extern void _DDAssert(LPCSTR szFile, int nLine, LPCSTR szCondition);
   extern int  DebugPrintf2(LPSTR cFilter, ...);
   extern void cdecl D3DInfoPrintf( UINT lvl, LPSTR szFormat, ...);
   extern void cdecl D3DWarnPrintf( UINT lvl, LPSTR szFormat, ...);
   extern void cdecl D3DErrorPrintf( LPSTR szFormat, ...);
#ifdef __cplusplus
    }
#endif

    #define dprintf(a,b)                            DebugPrintf(a,b);
    #define DPFINIT()                               DebugPrintfInit();
    #define DPF                                     DebugSetFileLineEtc(__FILE__,__LINE__,DPF_MODNAME),DebugPrintf
    #define DPF_ERR(a)                              DebugSetFileLineEtc(__FILE__,__LINE__,DPF_MODNAME),DebugPrintf( 0, a )
    #define DPF_ENTERAPI(pIface)                    DebugEnterAPI(DPF_MODNAME,(LPDWORD)pIface)
    #define DPF_APIRETURNS(hr)                      DPF(3,"   %s returns %08x (%d)",DPF_MODNAME,hr,hr&0xfff)
    #define DDASSERT(condition)                     if (!(condition)) _DDAssert(__FILE__, __LINE__, #condition)
    #define DPF_MUTEWHEN(c)                         {DebugSetMute((BOOL) (c) );}
    #define DPF_SPEWWHEN(c)                         {DebugSetMute((BOOL) (!(c)) );}
    #define DPF_MUTE()                              DPF_MUTEWHEN(TRUE)
    #define DPF_UNMUTE()                            DPF_MUTEWHEN(FALSE)

    #define DPF_SETTOPICS(t)
    #define DPF_STRUCT(level,topic,struct_identifier,struct_ptr)    {extern void DUMP_##struct_identifier(DWORD,DWORD,LP##struct_identifier);\
                                                    DUMP_##struct_identifier(level,topic,struct_ptr);}

    #define D3D_ERR       D3DErrorPrintf
    #define D3D_WARN      D3DWarnPrintf
    #define D3D_INFO      D3DInfoPrintf
#else
    #pragma warning(disable:4002)
    #define dprintf()
    #define DPFINIT()
    #define DPFFINI()
    #define DPF()
    #define DPF_ENTERAPI()
    #define DPF_APIRETURNS()
    #define DPF_DUMP()
    #define DPF_DUMPHEX()
    #define DPF_DUMPGUID()
    #define DDASSERT()
    #define DPF_ERR()
    #define DEBUG_BREAK()
    #define DPF_MUTEWHEN()
    #define DPF_SPEWWHEN()
    #define DPF_MUTE()
    #define DPF_UNMUTE()

    #define DPF_SETTOPICS()
    #define DPF_DUMPGUID()
    #define DPF_STRUCT()
    #define D3D_ERR(a)
    #define D3D_WARN()
    #define D3D_INFO()
#endif


#ifdef _WIN32

#ifdef DEBUG
    __inline DWORD clockrate() {LARGE_INTEGER li; QueryPerformanceFrequency(&li); return li.LowPart;}
    __inline DWORD clock()     {LARGE_INTEGER li; QueryPerformanceCounter(&li);   return li.LowPart;}

    #define TIMEVAR(t)    DWORD t ## T; DWORD t ## N
    #define TIMEZERO(t)   t ## T = 0, t ## N = 0
    #define TIMESTART(t)  t ## T -= clock(), t ## N ++
    #define TIMESTOP(t)   t ## T += clock()
    #define TIMEFMT(t)    ((DWORD)(t) / clockrate()), (((DWORD)(t) * 1000 / clockrate())%1000)
    #define TIMEOUT(t)    if (t ## N) DPF(4, #t ": %ld calls, %ld.%03ld sec (%ld.%03ld)", t ## N, TIMEFMT(t ## T), TIMEFMT(t ## T / t ## N))
#else
    #define TIMEVAR(t)
    #define TIMEZERO(t)
    #define TIMESTART(t)
    #define TIMESTOP(t)
    #define TIMEFMT(t)
    #define TIMEOUT(t)
#endif

#endif


#endif //__DEBUG_PF
