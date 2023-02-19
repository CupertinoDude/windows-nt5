/*++

Copyright (c) 1999, Microsoft Corporation

Module Name:

    elport.h

Abstract:
    This module contains declarations for port management for EAPOL, 
    r/w to ports


Revision History:

    sachins, Apr 23 2000, Created

--*/


#ifndef _EAPOL_PORT_H_
#define _EAPOL_PORT_H_

//
// EAPOL PCB Flags
//

#define EAPOL_PORT_FLAG_DELETED     0x8000
#define EAPOL_PORT_DELETED(i) \
    ((i)->dwFlags & EAPOL_PORT_FLAG_DELETED)

#define EAPOL_PORT_FLAG_ACTIVE      0x4000
#define EAPOL_PORT_ACTIVE(i) \
        ((i)->dwFlags & EAPOL_PORT_FLAG_ACTIVE)

//
// EAPOL Timer Flags
//

#define EAPOL_AUTH_TIMER            0x8000
#define EAPOL_AUTH_TIMER_SET(i) \
    ((i)->dwTimerFlags & EAPOL_AUTH_TIMER)

#define EAPOL_HELD_TIMER            0x4000
#define EAPOL_HELD_TIMER_SET(i) \
    ((i)->dwTimerFlags & EAPOL_HELD_TIMER)

#define EAPOL_START_TIMER           0x2000
#define EAPOL_START_TIMER_SET(i) \
    ((i)->dwTimerFlags & EAPOL_START_TIMER)

#define EAPOL_NO_TIMER              0x0000
#define EAPOL_NO_TIMER_SET(i) \
    ((i)->dwTimerFlags & EAPOL_NO_TIMER)

#define SET_EAPOL_AUTH_TIMER(i) \
    ((i)->dwTimerFlags = EAPOL_AUTH_TIMER)

#define SET_EAPOL_HELD_TIMER(i) \
    ((i)->dwTimerFlags = EAPOL_HELD_TIMER)

#define SET_EAPOL_START_TIMER(i) \
    ((i)->dwTimerFlags = EAPOL_START_TIMER)

#define SET_EAPOL_NO_TIMER(i) \
    ((i)->dwTimerFlags = EAPOL_NO_TIMER)

#define CHECK_EAPOL_TIMER(i) \
    ((i)->dwTimerFlags & (EAPOL_AUTH_TIMER|EAPOL_HELD_TIMER|EAPOL_START_TIMER))


//
// Structure:   EAPOL_PCB
//
// EAPOL Port Control Block
// This structure holds the operational information for an interface/port
// from the EAPOL protocol standpoint.
// It also maintains state information for EAP protocol.
//
// Each PCB is inserted in a hash bucket list, one for each interface
//
// Synchronization on PCBs is done using a read-write PCB list lock, 
// and a per-PCB read-write lock, and a per-port ref count. 
// The locks are single-write, multiple-read. Currently, locks are used 
// in write mode only
//
// If PCB's are to be added or deleted, the PCB list lock should 
// be acquired. 
//
// If any PCB needs to be modified, the per-PCB list lock should be acquired
// in write mode. 
//
// Acquiring a reference to a port guarantees the PCBs existence;
// acquiring the PCB lock guarantees consistency of the PCB fields
//
//

typedef struct _EAPOL_PCB 
{
    // Pointer to next PCB in the hash bucket
    struct _EAPOL_PCB       *pNext;         

    // Handle to NDIS UIO device
    HANDLE                  hPort;          

    // Port number on the system Will be an integer value cast
    DWORD                   dwPortIndex;    

    // Friendly name of the interface on which this port is opened
    CHAR                    *pszFriendlyName;

    // GUID string uniquely identifying the interface 
    CHAR                    *pszDeviceGUID;   

    // Additional identiifier for a connected port e.g. MSFTWLAN
    CHAR                    *pszSSID;       

    // Version of EAPOL supported on this port 
    DWORD                   dwEapolVersion; 

    // Pointer to EAP Work Buffer for this PCB
    PVOID                   pEapWorkBuffer; 

    // Per PCB read-write lock
    READ_WRITE_LOCK         rwLock;         

    // Number of references made to this port
    DWORD                   dwRefCount;

    // Indicates whether port is ACTIVE or DISABLED
    DWORD                   dwFlags;

    // EAPOL state
    EAPOL_STATE             State;

    // EAPOL statistics for this port     
    EAPOL_STATS             EapolStats;     

    // EAPOL configuration parameters for this port
    EAPOL_CONFIG            EapolConfig;    

    // Version of EAPOL supported
    BYTE                    bProtocolVersion;   
    
    // Handle to EAPOL timer currently running on this machine
    HANDLE                  hTimer;         

    // Ethertype for this LAN
    BYTE                    bEtherType[SIZE_ETHERNET_TYPE];   
    
    // Mac Addr of peer (switch port access point)
    BYTE                    bSrcMacAddr[SIZE_MAC_ADDR];  

    // Mac Addr of peer (switch port or access point)
    BYTE                    bDestMacAddr[SIZE_MAC_ADDR]; 

    // Media State
    NDIS_MEDIA_STATE        MediaState;

    // Physical Medium Type
    NDIS_PHYSICAL_MEDIUM    PhysicalMediumType;

    DWORD                   dwTimerFlags;

    // Time in milliseconds next timeout should occur
    ULONG                   ulTimeout;      

    // Number of EAPOL_Start messages that have been sent without
    // receiving response    
    ULONG                   ulStartCount;   

    // Identifier in the most recently received EAP Request frame
    DWORD                   dwPreviousId; 

    // Copy of last sent out EAPOL packet
    // Used for retransmission
    BYTE                    *pbPreviousEAPOLPkt;
    DWORD                   dwSizeOfPreviousEAPOLPkt;

    // Has Identity for the user obtained using RasEapGetIdentity ?
    BOOL                    fGotUserIdentity;

    // Is the port on a authenticated network i.e. is the remote end
    // EAPOL aware
    BOOL                    fIsRemoteEndEAPOLAware;

    //
    // EAP related variables
    //

    BOOL                    fEapInitialized;

    BOOL                    fLogon;

    BOOL                    fUserLoggedIn;

    // Authentication identity using RasGetUserIdentity or other means
    CHAR                    *pszIdentity;

    // User Password for EAP MD5 CHAP
    CHAR                    *pszPassword;

    // Token for interactively logged-on user obtained using 
    // GetCurrentUserToken
    HANDLE                  hUserToken;             

    // EAP configuration blob stored for each GUID
    EAPOL_CUSTOM_AUTH_DATA  *pCustomAuthConnData;    

    // User blob stored for GUID 
    EAPOL_CUSTOM_AUTH_DATA  *pCustomAuthUserData;    

    // Data obtained using RasEapInvokeInteractiveUI
    EAPOL_EAP_UI_DATA       EapUIData;                  

    // Interactive data received from InvokeInteractiveUI
    BOOL                    fEapUIDataReceived;                  

    // EAP type for the connection
    DWORD                   dwEapTypeToBeUsed;      
                                                        
    // Index for current EAP type in index table
    DWORD                   dwEapIndex;             
                                                        
    // Unique identifier for UI invocation
    DWORD                   dwUIInvocationId;       

    // Interactive dialog allowed?
    BOOL                    fNonInteractive;        

    // EAP state for the port
    EAPSTATE                EapState;           
     
    // Work space for EAP implementation DLL
    // PCB just holds the pointer, the memory allocation is done by the EAP DLL
    // during RasEapBegin and should be passed to RasEapEnd for cleanup
    LPVOID                  lpEapDllWorkBuffer;  
                                                
    // Notification message
    CHAR                    *pszEapReplyMessage;     

    // Encryption keys
    BYTE                    *pbMPPESendKey;
    DWORD                   dwMPPESendKeyLength;
    BYTE                    *pbMPPERecvKey;
    DWORD                   dwMPPERecvKeyLength;
    
    // Last replay counter. Used to guard against security attacks
    ULONGLONG               ullLastReplayCounter; 

    // EAPOL to run on this port or not
    DWORD                   dwEapolEnabled;

    // Has EAPOL_Logoff packet been sent out on this port?
    DWORD                   dwLogoffSent;

    // Authentication type last performed - Used with MACHINE_AUTH
    EAPOL_AUTHENTICATION_TYPE       PreviousAuthenticationType; 

    // Number of current authentication failures for the port - MACHINE_AUTH
    DWORD                   dwAuthFailCount;

    // Is authentication being done on a new AP/Switch/Network?
    BOOLEAN                 fAuthenticationOnNewNetwork;

    // Tick count, the last time the port was restart
    DWORD                   dwLastRestartTickCount;

    // Implementation version supported by remote end
    // Default is Draft 7, Draft 8 if detected
    BOOLEAN                 fRemoteEnd8021XD8;

} EAPOL_PCB, *PEAPOL_PCB;


//
// Structure: ETH_HEADER
//

typedef struct _ETH_HEADER 
{
    BYTE            bDstAddr[SIZE_MAC_ADDR];
    BYTE            bSrcAddr[SIZE_MAC_ADDR];
} ETH_HEADER, *PETH_HEADER;


//
// Structure:   EAPOL_BUFFER
//
// This structure holds a buffer used for I/O to the ndis uio driver
// EAPOL_BUFFER structure is used in the OVERLAPPED read-write operations. 
// On the OVERLAPPED read/write completion, pvContext is used to 
// identity the port on which the I/O occured
//

typedef struct _EAPOL_BUFFER 
{
    //
    // This is the pointer to the EAPOL_PCB structure of the interface on which 
    // I/O was performed
    //
    PVOID pvContext;

    // Send/Recv data buffer

    CHAR  pBuffer[MAX_PACKET_SIZE]; 
    
    //
    // Passed as the system context area for any I/O using the buffer
    //        
    OVERLAPPED Overlapped;

    //
    // Pointer to Completion Routine
    // 
    VOID    (CALLBACK *CompletionRoutine)
                    (DWORD, DWORD, struct _EAPOL_BUFFER *);

    // Fields which are filled on IoCompletion
    DWORD   dwErrorCode;
    DWORD   dwBytesTransferred;
    
} EAPOL_BUFFER, *PEAPOL_BUFFER;


//
// Synchronization
//
#define EAPOL_REFERENCE_PORT(PCB) \
    (EAPOL_PORT_DELETED(PCB) ? FALSE : (InterlockedIncrement(&(PCB)->dwRefCount), TRUE))

#define EAPOL_DEREFERENCE_PORT(PCB) \
    (InterlockedDecrement(&(PCB)->dwRefCount) ? TRUE : (ElCleanupPort(PCB), FALSE))


//
// FUNCTION DECLARATIONS
//

DWORD
ElHashPortToBucket (
        IN  CHAR             *pszDeviceGUID
        );

VOID
ElRemovePCBFromTable (
        IN  EAPOL_PCB        *pPCB
        );

PEAPOL_PCB
ElGetPCBPointerFromPortGUID (
        IN CHAR             *pszDeviceGUID
        );

DWORD
ElCreatePort (
        IN  HANDLE          hDevice,
        IN  CHAR            *pszGUID,
        IN  CHAR            *pszFriendlyName,
        IN  BYTE            *psSrcMacAddr
        );

DWORD
ElDeletePort (
        IN  CHAR            *pszDeviceName,
        OUT HANDLE          *hDevice
        );

VOID
ElCleanupPort (
        IN  EAPOL_PCB       *pPCB
        );

DWORD
ElReStartPort (
        IN  PEAPOL_PCB      pPCB
        );

DWORD
ElReadFromPort (
        IN PEAPOL_PCB       pPCB,
        IN PCHAR            pBuffer,
        IN DWORD            dwBufferLength
        );

DWORD
ElWriteToPort (
        IN PEAPOL_PCB       pPCB,
        IN PCHAR            pBuffer,
        IN DWORD            dwBufferLength
        );

DWORD
ElInitializeEAPOL (
        );

DWORD
ElEAPOLDeInit (
        );

VOID
ElReadPortStatistics (
        IN  CHAR            *pszDeviceName,
        OUT PEAPOL_STATS    pEapolStats
        );

VOID
ElReadPortConfiguration (
        IN  CHAR            *pszDeviceName,
        OUT PEAPOL_CONFIG   pEapolConfig
        );

ULONG
ElSetPortConfiguration (
        IN  CHAR            *pszDeviceName,
        IN  PEAPOL_CONFIG   pEapolConfig
        );

VOID CALLBACK
ElReadCompletionRoutine (
        IN  DWORD           dwError,
        IN  DWORD           dwBytesReceived,
        IN  PEAPOL_BUFFER   pEapolBuffer 
        );

VOID CALLBACK
ElWriteCompletionRoutine (
        IN  DWORD           dwError,
        IN  DWORD           dwBytesSent,
        IN  PEAPOL_BUFFER   pEapolBuffer 
        );

VOID CALLBACK
ElIoCompletionRoutine (
        IN  DWORD           dwError,
        IN  DWORD           dwBytesTransferred,
        IN  LPOVERLAPPED    lpOverlapped
        );

DWORD
ElReadPerPortRegistryParams(
        IN  CHAR            *pszDeviceGUID,
        IN  EAPOL_PCB       *pNewPCB
        );


#endif  // _EAPOL_PORT_H_
