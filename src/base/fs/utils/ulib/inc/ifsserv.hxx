/*++

Copyright (c) 1990-2000 Microsoft Corporation

Module Name:

        ifsserv.hxx

Abstract:

        Contains prototypes for entry points to the IFS
        utility DLLs.


Author:

        Bill McJohn (billmc) 04-June-1991

Environment:

        User Mode

--*/


#if !defined ( _IFS_SERV_ )

#define _IFS_SERV_

#if defined( _AUTOCHECK_ ) && !defined( _SETUP_LOADER_ )
#define FAR
#define WINAPI __stdcall
#define APIENTRY WINAPI
#endif  //  _AUTOCHECK_ || _SETUP_LOADER_


#if !defined(_IFS_ENTRY_)

//
// Internal definitions for Flags field in CHKDSKEX_FN_PARAM
//
#define CHKDSK_RECOVER_FREE_SPACE       0x00000002UL
#define CHKDSK_RECOVER_ALLOC_SPACE      0x00000004UL

//
// External definitions for Flags field in CHKDSKEX_FN_PARAM
//

// CHKDSK_VERBOSE
//  - For FAT, chkdsk will print every filename being processed
//  - For NTFS, chkdsk will print clean up messages
// CHKDSK_RECOVER
//  - Perform sector checking on free and allocated space
// CHKDSK_EXTEND
//  - For NTFS, chkdsk will extend a volume
// CHKDSK_DOWNGRADE (for NT 5 or later but obsolete anyway)
//  - For NTFS, this downgrade a volume from most recent NTFS version
// CHKDSK_ENABLE_UPGRADE (for NT 5 or later but obsolete anyway)
//  - For NTFS, this upgrades a volume to most recent NTFS version
// CHKDSK_RESIZE_LOGFILE
//  - For NTFS, this changes the size of the logfile
// CHKDSK_CHECK_IF_DIRTY
//  - Perform consistency check only if the volume is dirty
// CHKDSK_FORCE
//  - Forces the volume to dismount first if necessary (implies /F)
// CHKDSK_SKIP_INDEX_SCAN
//  - Skip the scanning of each index entry
// CHKDSK_SKIP_CYCLE_SCAN
//  - Skip the checking of cycles within the directory tree
// CHKDSK_ALGORITHM_SPECIFIED
//  - For NTFS, this indicates an algorithm value is specified.
//    This bit cannot be used together with CHKDSK_SKIP_INDEX_SCAN

#define CHKDSK_VERBOSE                  0x00000001UL
#define CHKDSK_RECOVER                  (CHKDSK_RECOVER_FREE_SPACE | \
                                         CHKDSK_RECOVER_ALLOC_SPACE)
#define CHKDSK_EXTEND                   0x00000008UL
#define CHKDSK_DOWNGRADE                0x00000010UL
#define CHKDSK_ENABLE_UPGRADE           0x00000020UL
#define CHKDSK_RESIZE_LOGFILE           0x00000040UL
#define CHKDSK_CHECK_IF_DIRTY           0x00000080UL
#define CHKDSK_FORCE                    0x00000100UL
#define CHKDSK_SKIP_INDEX_SCAN          0x00000200UL
#define CHKDSK_SKIP_CYCLE_SCAN          0x00000400UL
#define CHKDSK_ALGORITHM_SPECIFIED      0x00000800UL

//
// Values related to the above bits
//
#define CHKDSK_MAX_ALGORITHM_VALUE      50

typedef struct _CHKDSKEX_FN_PARAM {
    UCHAR       Major;          // initial version is 1.0
    UCHAR       Minor;
    ULONG       Flags;
    ULONG       LogFileSize;
    PPATH       PathToCheck;    // path including mount points
    PPATH       RootPath;       // path from the root of the actual drive
    USHORT      Algorithm;      // number of passes to use during stage 2 of NTFS chkdsk
} CHKDSKEX_FN_PARAM, *PCHKDSKEX_FN_PARAM;

//
// External definitions for Flags field in FORMATEX_FN_PARAM
//

#define FORMAT_QUICK                    0x00000001UL
#define FORMAT_BACKWARD_COMPATIBLE      0x00000002UL
#define FORMAT_FORCE                    0x00000004UL
#define FORMAT_YES                      0x80000000UL

// FORMAT_QUICK
//  - Performs a quick format
// FORMAT_BACKWARD_COMPATIBLE
//  - Formats a volume to be backward compatible to an older version of NTFS volume
// FORMAT_FORCE
//  - Forces the volume to dismount first if necessary
// FORMAT_YES
//  - Ignore all warnings and safety prompts.  Just go ahead and format.


typedef struct _FORMATEX_FN_PARAM {
    UCHAR       Major;          // initial version is 1.0
    UCHAR       Minor;
    ULONG       Flags;
    PCWSTRING   LabelString;    // supplies the volume's label
    ULONG       ClusterSize;    // supplies the cluster size for the volume
} FORMATEX_FN_PARAM, *PFORMATEX_FN_PARAM;

#endif // _IFS_ENTRY_

extern "C"
BOOLEAN
FAR APIENTRY
Chkdsk(
    IN      PCWSTRING   NtDriveName,
    IN OUT  PMESSAGE    Message,
    IN      BOOLEAN     Fix,
    IN      BOOLEAN     Verbose,
    IN      BOOLEAN     OnlyIfDirty,
    IN      BOOLEAN     Recover,
    IN      PPATH       PathToCheck,
    IN      BOOLEAN     Extend,
    IN      BOOLEAN     ResizeLogFile,
    IN      ULONG       LogFilesize,
    OUT     PULONG      ExitStatus
        );

extern "C"
BOOLEAN
FAR APIENTRY
ChkdskEx(
    IN      PCWSTRING           NtDriveName,
    IN OUT  PMESSAGE            Message,
    IN      BOOLEAN             Fix,
    IN      PCHKDSKEX_FN_PARAM  Param,
    OUT     PULONG              ExitStatus
        );

extern "C"
BOOLEAN
FAR APIENTRY
Format(
    IN      PCWSTRING   NtDriveName,
    IN OUT  PMESSAGE    Message,
    IN      BOOLEAN     Quick,
    IN      BOOLEAN     BackwardCompatible,
    IN      MEDIA_TYPE  MediaType,
    IN      PCWSTRING   LabelString,
    IN      ULONG       ClusterSize
        );

extern "C"
BOOLEAN
FAR APIENTRY
FormatEx(
    IN      PCWSTRING           NtDriveName,
    IN OUT  PMESSAGE            Message,
    IN      PFORMATEX_FN_PARAM  Param,
    IN      MEDIA_TYPE          MediaType
        );

extern "C"
BOOLEAN
FAR APIENTRY
Recover(
        IN PPATH                RecFilePath,
        IN OUT PMESSAGE Message
        );

extern "C"
BOOLEAN
FAR APIENTRY
Extend(
    IN      PCWSTRING   NtDriveName,
    IN OUT  PMESSAGE    Message,
    IN      BOOLEAN     Verify
    );


#if !defined(_IFS_ENTRY_)

//
//      Convert status code
//
typedef enum _CONVERT_STATUS {

        CONVERT_STATUS_CONVERTED,
        CONVERT_STATUS_INVALID_FILESYSTEM,
        CONVERT_STATUS_CONVERSION_NOT_AVAILABLE,
        CONVERT_STATUS_CANNOT_LOCK_DRIVE,
        CONVERT_STATUS_ERROR,
        CONVERT_STATUS_INSUFFICIENT_SPACE,
        CONVERT_STATUS_NTFS_RESERVED_NAMES,
        CONVERT_STATUS_DRIVE_IS_DIRTY,
        CONVERT_STATUS_INSUFFICIENT_FREE_SPACE

} CONVERT_STATUS, *PCONVERT_STATUS;

#endif // _IFS_ENTRY_

extern "C"
BOOLEAN
FAR APIENTRY
Convert(
    IN      PCWSTRING           NtDriveName,
    IN      PCWSTRING           FsName,
    IN OUT  PMESSAGE            Message,
    IN      BOOLEAN             Verbose,
    IN      BOOLEAN             Pause,
    OUT     PCONVERT_STATUS     Status
        );


#endif // _IFS_SERV_
