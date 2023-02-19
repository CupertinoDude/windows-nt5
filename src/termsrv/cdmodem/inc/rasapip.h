/*++

Copyright (C) 1994-98 Microsft Corporation. All rights reserved.

Module Name:

    rasapip.h

Abstract:

    This file has definitions for private apis for ras connections.
    These apis are exported from rasapi32.dll

Author:

    Rao Salapaka (raos) 30-Jan-1998

Revision History:

--*/

#ifndef _RASAPIP_
#define _RASAPIP_

#include <windef.h> //for MAX_PATH

#ifdef __cplusplus
extern "C" {
#endif


#define RASAPIP_MAX_DEVICE_NAME     128
#define RASAPIP_MAX_ENTRY_NAME      256

#define RAS_DEVICE_TYPE(_x)     ((_x) & 0x0000FFFF)

#define RAS_DEVICE_CLASS(_x)    ((_x) & 0xFFFF0000)

enum _RASDEVICETYPE
{
    RDT_Modem = 0,

    RDT_X25,

    RDT_Isdn,

    RDT_Serial,

    RDT_FrameRelay,

    RDT_Atm,

    RDT_Sonet,

    RDT_Sw56,

    RDT_Tunnel_Pptp,

    RDT_Tunnel_L2tp,

    RDT_Irda,

    RDT_Parallel,

    RDT_Other,

    //
    // The following flags when set
    // specify the class of the device
    //
    RDT_Tunnel = 0x00010000,

    RDT_Direct  = 0x00020000,

    RDT_Null_Modem = 0x000040000
};


typedef enum _RASDEVICETYPE RASDEVICETYPE;

typedef struct _RASENUMENTRYDETAILS
{
    DWORD   dwSize;
    DWORD   dwFlags;                    // same as RASENTRYNAME.dwFlags
    DWORD   dwType;                     // same as RASENTRY.dwType
    GUID    guidId;                     // same as RASENTRY.guidId
    BOOL    fShowMonitorIconInTaskBar;  // same as RASENTRY.fShowMonitorIconInTaskBar
    RASDEVICETYPE rdt;
    WCHAR   szDeviceName[RASAPIP_MAX_DEVICE_NAME];
    WCHAR   szEntryName[RASAPIP_MAX_ENTRY_NAME + 1];
    WCHAR   szPhonebookPath[MAX_PATH + 1];

} RASENUMENTRYDETAILS, *LPRASENUMENTRYDETAILS;

DWORD
APIENTRY
DwEnumEntryDetails (
    IN     LPCWSTR               lpszPhonebookPath,
    OUT    LPRASENUMENTRYDETAILS lprasentryname,
    IN OUT LPDWORD               lpcb,
    OUT    LPDWORD               lpcEntries
    );

DWORD
DwCloneEntry(
    IN      LPCWSTR lpwszPhonebookPath,
    IN      LPCWSTR lpwszSrcEntryName,
    IN      LPCWSTR lpwszDstEntryName
    );


// Implemented in rasman.dll
//
DWORD
APIENTRY
RasReferenceRasman (
    IN BOOL fAttach
    );


// Implemented in netcfgx.dll
//
HRESULT
WINAPI
RasAddBindings (
    IN OUT UINT*    pcIpOut,
    IN OUT UINT*    pcNbfIn,
    IN OUT UINT*    pcNbfOut);

HRESULT
WINAPI
RasCountBindings (
    OUT UINT*   pcIpOut,
    OUT UINT*   pcNbfIn,
    OUT UINT*   pcNbfOut);

HRESULT
WINAPI
RasRemoveBindings (
    IN OUT UINT*        pcIpOutBindings,
    IN     const GUID*  pguidIpOutBindings,
    IN OUT UINT*        pcNbfIn,
    IN OUT UINT*        pcNbfOut);

typedef struct _RASCONFIGENDPOINTS
{
    DWORD   dwSize;
    DWORD   dwFlags;
    INT     cTotalEndpoints;
    INT     cLimitSimultaneousIncomingCalls;
    INT     cLimitSimultaneousOutgoingCalls;
} RASCONFIGENDPOINTS;

HRESULT
APIENTRY
RasConfigureEndpoints (
    IN OUT  RASCONFIGENDPOINTS* pInfo
    );


//+---------------------------------------------------------------------------
// RAS Event notifications into netman.
//
typedef enum _RASEVENTTYPE
{
    ENTRY_ADDED,
    ENTRY_DELETED,
    ENTRY_MODIFIED,
    ENTRY_RENAMED,
    ENTRY_CONNECTED,
    ENTRY_CONNECTING,
    ENTRY_DISCONNECTING,
    ENTRY_DISCONNECTED,
    INCOMING_CONNECTED,
    INCOMING_DISCONNECTED,
    SERVICE_EVENT,
} RASEVENTTYPE;

typedef enum _SERVICEEVENTTYPE
{
    RAS_SERVICE_STARTED,
    RAS_SERVICE_STOPPED,
} SERVICEEVENTTYPE;

typedef enum _RASSERVICE
{
    RASMAN,
    RASAUTO,
    REMOTEACCESS,
} RASSERVICE;

typedef struct _RASEVENT
{
    RASEVENTTYPE    Type;

    union
    {
    // ENTRY_ADDED,
    // ENTRY_MODIFIED,
        struct
        {
            RASENUMENTRYDETAILS     Details;
        };

    // ENTRY_DELETED,
    // ENTRY_CONNECTED,
    // ENTRY_CONNECTING,
    // ENTRY_DISCONNECTING,
    // ENTRY_DISCONNECTED,
    // INCOMING_CONNECTED,
    // INCOMING_DISCONNECTED,
    //  guidId is valid

    // ENTRY_RENAMED,
        struct
        {
            GUID    guidId;
            WCHAR   pszwNewName [RASAPIP_MAX_ENTRY_NAME + 1];
        };

    // SERVICE_EVENT,
        struct
        {
            SERVICEEVENTTYPE    Event;
            RASSERVICE          Service;
        };
    };
} RASEVENT;

typedef struct _RASENTRYHEADER
{
    DWORD dwEntryType;
    WCHAR szEntryName[RASAPIP_MAX_ENTRY_NAME + 1];
} RASENTRYHEADER, *PRASENTRYHEADER;

// Implemented in netman.dll
//
typedef VOID (APIENTRY *RASEVENTNOTIFYPROC)(const RASEVENT* pEvent);

VOID
APIENTRY
RasEventNotify (
    const RASEVENT* pEvent);

DWORD
DwEnumEntriesForAllUsers(
            DWORD *lpcb,
            DWORD *lpcEntries,
            RASENTRYHEADER * pRasEntryHeader);


#ifdef __cplusplus
}
#endif


#endif  // _RASAPIP_
