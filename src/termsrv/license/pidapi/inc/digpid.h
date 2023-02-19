/*++ 

Copyright (c) 1985-1998, Microsoft Corporation

Module Name:


    digpid.h

Abstract:

--*/

// DigPid.h

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_PID 0xFFFFFFFF

typedef enum {
    ltFPP,
    ltCCP,
    ltOEM,
    ltSelect,
    ltMLP,
    ltMOLP,
    ltMSDN
} LICENSETYPE;
typedef DWORD DWLICENSETYPE;


typedef struct {
    DWORD dwLength;
    WORD  wVersionMajor;
    WORD  wVersionMinor;
    char  szPid2[24];
    DWORD dwKeyIdx;
    char  szSku[16];
    BYTE  abCdKey[16];
    DWORD dwCloneStatus;
    DWORD dwTime;
    DWORD dwRandom;
    DWLICENSETYPE dwlt;
    DWORD adwLicenseData[2];
    char  szOemId[8];
    DWORD dwBundleId;

    char  aszHardwareIdStatic[8];

    DWORD dwHardwareIdTypeStatic;
    DWORD dwBiosChecksumStatic;
    DWORD dwVolSerStatic;
    DWORD dwTotalRamStatic;
    DWORD dwVideoBiosChecksumStatic;

    char  aszHardwareIdDynamic[8];

    DWORD dwHardwareIdTypeDynamic;
    DWORD dwBiosChecksumDynamic;
    DWORD dwVolSerDynamic;
    DWORD dwTotalRamDynamic;
    DWORD dwVideoBiosChecksumDynamic;

    DWORD dwCrc32;
} DIGITALPID, *PDIGITALPID, FAR *LPDIGITALPID;

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */
