/*++

Copyright (C) 1998-1999 Microsoft Corporation

Module Name:

    common.h

Abstract:

    SMONLOG common definitions

--*/

#ifndef _SMONLOG_COMMON_H_
#define _SMONLOG_COMMON_H_

#include <wtypes.h>

#define _CONFIG_DLL_NAME_W_     L"SmLogCfg.dll"
#define _SERVICE_EXE_NAME_W_    L"SmLogSvc.exe"   

// Communication between smlogcfg and smlogsvc

#define SERVICE_CONTROL_SYNCHRONIZE 128
#define SLQ_QUERY_STOPPED       ((DWORD)0x00000000)              
#define SLQ_QUERY_RUNNING       ((DWORD)0x00000001)
#define SLQ_QUERY_START_PENDING ((DWORD)0x00000002)

#define SLQ_NEW_LOG         ((DWORD)0xFFFFFFFF)
#define SLQ_FIRST_LOG_TYPE  ((DWORD)0x00000000)
#define SLQ_COUNTER_LOG     ((DWORD)0x00000000)
#define SLQ_TRACE_LOG       ((DWORD)0x00000001)
#define SLQ_ALERT           ((DWORD)0x00000002)
#define SLQ_LAST_LOG_TYPE   ((DWORD)0x00000002)
#define SLQ_NUM_LOG_TYPES   ((DWORD)0x00000003)

// Sysmon log output file configuration definitions

#define SLF_FIRST_FILE_TYPE ((DWORD)0x00000000)
#define SLF_CSV_FILE        ((DWORD)0x00000000)
#define SLF_TSV_FILE        ((DWORD)0x00000001)
#define SLF_BIN_FILE        ((DWORD)0x00000002)
#define SLF_BIN_CIRC_FILE   ((DWORD)0x00000003)
#define SLF_CIRC_TRACE_FILE ((DWORD)0x00000004)
#define SLF_SEQ_TRACE_FILE  ((DWORD)0x00000005)
#define SLF_SQL_LOG         ((DWORD)0x00000006)
#define SLF_FILE_OVERWRITE  ((DWORD)0x00010000)
#define SLF_NUM_FILE_TYPES  ((DWORD)0x00000007)

#define SLF_NAME_NONE           ((DWORD)0xFFFFFFFF)
#define SLF_NAME_FIRST_AUTO     ((DWORD)0x00000000)
#define SLF_NAME_MMDDHH         ((DWORD)0x00000000)
#define SLF_NAME_NNNNNN         ((DWORD)0x00000001)
#define SLF_NAME_YYYYDDD        ((DWORD)0x00000002)
#define SLF_NAME_YYYYMM         ((DWORD)0x00000003)
#define SLF_NAME_YYYYMMDD       ((DWORD)0x00000004)
#define SLF_NAME_YYYYMMDDHH     ((DWORD)0x00000005)
#define SLF_NAME_MMDDHHMM       ((DWORD)0x00000006)
#define SLF_NUM_AUTO_NAME_TYPES ((DWORD)0x00000007)

// Sysmon log query types and constants

// Constants
#define SLQ_DISK_MAX_SIZE   ((DWORD)-1)

#define SLQ_TLI_ENABLE_BUFFER_FLUSH         ((DWORD)0x00000001)
#define SLQ_TLI_ENABLE_KERNEL_TRACE         ((DWORD)0x00000002)
#define SLQ_TLI_ENABLE_MEMMAN_TRACE         ((DWORD)0x00000004)
#define SLQ_TLI_ENABLE_FILEIO_TRACE         ((DWORD)0x00000008)
#define SLQ_TLI_ENABLE_PROCESS_TRACE        ((DWORD)0x00000010)
#define SLQ_TLI_ENABLE_THREAD_TRACE         ((DWORD)0x00000020)
#define SLQ_TLI_ENABLE_DISKIO_TRACE         ((DWORD)0x00000040)
#define SLQ_TLI_ENABLE_NETWORK_TCPIP_TRACE  ((DWORD)0x00000080)

#define SLQ_TLI_ENABLE_MASK                 ((DWORD)0x000000FF)
#define SLQ_TLI_ENABLE_KERNEL_MASK          ((DWORD)0x000000FE)


// dwMode values
#define SLQ_AUTO_MODE_NONE  ((DWORD)0x00000000)       // Manual
#define SLQ_AUTO_MODE_SIZE  ((DWORD)0x00000001)       // Size
#define SLQ_AUTO_MODE_AT    ((DWORD)0x00000002)       // Time
#define SLQ_AUTO_MODE_AFTER ((DWORD)0x00000003)       // Value & unit type

// wDataType values
#define SLQ_TT_DTYPE_DATETIME   ((WORD)0x0001)
#define SLQ_TT_DTYPE_UNITS      ((WORD)0x0002)

// wTimeType values
#define SLQ_TT_TTYPE_START          ((WORD)0x0001)
#define SLQ_TT_TTYPE_STOP           ((WORD)0x0002)
#define SLQ_TT_TTYPE_RESTART        ((WORD)0x0003)
#define SLQ_TT_TTYPE_SAMPLE         ((WORD)0x0004)
#define SLQ_TT_TTYPE_LAST_MODIFIED  ((WORD)0x0005)

// dwUnitType values
#define SLQ_TT_UTYPE_SECONDS        ((DWORD)0x00000001)    
#define SLQ_TT_UTYPE_MINUTES        ((DWORD)0x00000002)   
#define SLQ_TT_UTYPE_HOURS          ((DWORD)0x00000003)   
#define SLQ_TT_UTYPE_DAYS           ((DWORD)0x00000004)   

#pragma warning ( disable : 4201 )

typedef struct _SLQ_TIME_INFO {
    WORD    wDataType;
    WORD    wTimeType;
    DWORD   dwAutoMode;
    union {
        LONGLONG    llDateTime; // filetime stored as a LONGLONG
        struct {
            DWORD   dwValue;
            DWORD   dwUnitType;
        };
    };
} SLQ_TIME_INFO, *PSLQ_TIME_INFO;

#pragma warning ( default : 4201 )

// alert action flags
#define ALRT_ACTION_LOG_EVENT   ((DWORD)0x00000001)
#define ALRT_ACTION_SEND_MSG    ((DWORD)0x00000002)
#define ALRT_ACTION_EXEC_CMD    ((DWORD)0x00000004)
#define ALRT_ACTION_START_LOG   ((DWORD)0x00000008)
#define ALRT_ACTION_MASK        ((DWORD)0x0000000F)

#define ALRT_CMD_LINE_SINGLE    ((DWORD)0x00000100)
#define ALRT_CMD_LINE_A_NAME    ((DWORD)0x00000200)
#define ALRT_CMD_LINE_C_NAME    ((DWORD)0x00000400)
#define ALRT_CMD_LINE_D_TIME    ((DWORD)0x00000800)
#define ALRT_CMD_LINE_L_VAL     ((DWORD)0x00001000)
#define ALRT_CMD_LINE_M_VAL     ((DWORD)0x00002000)
#define ALRT_CMD_LINE_U_TEXT    ((DWORD)0x00004000)
#define ALRT_CMD_LINE_MASK      ((DWORD)0x00007F00)

#define ALRT_DEFAULT_ACTION     ((DWORD)0x00000001) // log event is default

#define AIBF_UNDER  0L
#define AIBF_OVER   ((DWORD)0x00000001) // true when "over" limit is selected
#define AIBF_SEEN   ((DWORD)0x00000002) // set when the user has seen this value
#define AIBF_SAVED  ((DWORD)0x00000004) // true when user has saved this entry in an edit box

#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct _ALERT_INFO_BLOCK {
    DWORD   dwSize;
    LPTSTR  szCounterPath;
    DWORD   dwFlags;
    double  dLimit;
} ALERT_INFO_BLOCK, *PALERT_INFO_BLOCK;

// functions found in utils.c
// Replace spaces with underscores.
void __stdcall ReplaceBlanksWithUnderscores ( LPWSTR szString);
BOOL __stdcall MakeInfoFromString (LPCTSTR szBuffer, PALERT_INFO_BLOCK pInfo, LPDWORD pdwBufferSize);
BOOL __stdcall MakeStringFromInfo (PALERT_INFO_BLOCK pInfo, LPTSTR szBuffer, LPDWORD pcchBufferLength);
BOOL __stdcall GetLocalFileTime (LONGLONG    *pFileTime );
DWORD __stdcall ParseFolderPath (LPCTSTR szOrigPath, LPTSTR szBuffer, INT* piBufLen );

void __stdcall TimeInfoToMilliseconds ( SLQ_TIME_INFO* pTimeInfo, LONGLONG* pllmsecs );

#ifdef __cplusplus
}
#endif

#endif //_SMONLOG_COMMON_H_
