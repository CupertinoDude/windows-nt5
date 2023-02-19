/*++

Copyright (c) 1996 Microsoft Corporation

Module Name:

    unicode.c

Abstract:

    Simplified Unicode-Ansi conversion functions.

    Externally exposed routines:
      In-Place Conversion:
        KnownSizeDbcsToUnicodeN
        KnownSizeUnicodeToDbcsN
        KnownSizeWtoA
        KnownSizeAtoW

      In-Place Conversion without nul checks:
        DirectDbcsToUnicodeN
        DirectUnicodeToDbcsN
        DirectAtoW
        DirectWtoA

      Length/pool options:
        DbcsToUnicodeN
        UnicodeToDbcsN
        DbcsToUnicode
        UnicodeToDbcs
        FreeConvertedPoolStr

      Simplified type conversions:
        ConvertWtoA
        ConvertAtoW
        FreeConvertedStr

      TCHAR routines that can be compiled both ways:
        CreateDbcs
        CreateUnicode
        DestroyDbcs
        DestroyUnicode

Author:

    Jim Schmidt (jimschm)   04-Aug-1997

Revision History:

    jimschm     15-Feb-1999 Eliminated MikeCo's routines, since they are
                            broken on FE
    jimschm     23-Sep-1998 Added in-place routines

--*/

#include "pch.h"
#include <locale.h>
#include <mbctype.h>

extern POOLHANDLE g_TextPool;
extern DWORD g_MigutilWCToMBFlags;

WORD g_GlobalCodePage = CP_ACP;

typedef VOID(WINAPI SETACP)(WORD CodePage);
typedef SETACP * PSETACP;

VOID
SetGlobalCodePage (
    IN      WORD CodePage,
    IN      LCID Locale
    )
{
    PSETACP SetACP;
    HANDLE Lib;

    g_GlobalCodePage = CodePage;


    if (ISNT()) {
        Lib = LoadLibrary (TEXT("kernel32.dll"));
        if (Lib) {
            SetACP = (PSETACP) GetProcAddress (Lib, "SetCPGlobal");
            if (SetACP) {
                SetACP (CodePage);
            }

            FreeLibrary (Lib);
        }
    }

    SetThreadLocale (Locale);
    setlocale(LC_ALL,"");
    _setmbcp(CodePage);
}


VOID
GetGlobalCodePage (
    OUT     PWORD CodePage,             OPTIONAL
    OUT     PLCID Locale                OPTIONAL
    )
{
    if (CodePage) {
        if (g_GlobalCodePage == CP_ACP) {
            *CodePage = (WORD)GetACP();
        } else {
            *CodePage = g_GlobalCodePage;
        }
    }

    if (Locale) {
        *Locale = GetThreadLocale();
    }
}


PCSTR
RealUnicodeToDbcsN (
    IN      POOLHANDLE Pool,            OPTIONAL
    IN      PCWSTR StrIn,
    IN      DWORD Chars
    )

/*++

Routine Description:

  Converts a UNICODE string to DBCS.

  WARNING: Currently supports the ANSI code page only.  Later we can fix this.

Arguments:

  Pool  - Specifies the pool where memory is allocated from.  If not specified,
          g_TextPool is used instead.

  StrIn - Specifies the inbound UNICODE string

  Chars - Specifies the number of characters, excluding the nul, to
          convert.

Return Value:

  A pointer to the ANSI string, or NULL if an error occurred.

--*/

{
    PSTR DbcsStr;
    DWORD Size;
    DWORD rc;

    if (!Pool) {
        Pool = g_TextPool;
    }

    if (INVALID_CHAR_COUNT == Chars) {
        Chars = CharCountW (StrIn);
    }

    Size = (Chars + 1) * sizeof (WCHAR);

    DbcsStr = (PSTR) PoolMemGetAlignedMemory (Pool, Size);
    if (!DbcsStr) {
        DEBUGMSG ((DBG_ERROR, "UnicodeToDbcsN could not allocate string"));
        return NULL;
    }

    rc = WideCharToMultiByte (
             g_GlobalCodePage,
             g_MigutilWCToMBFlags,
             StrIn,
             Chars,              // wc input count
             DbcsStr,
             Size,
             NULL,
             NULL
             );

    // Report error returns from WideCharToMultiByte
    if (!rc && Chars) {
        PushError();
        PoolMemReleaseMemory (Pool, DbcsStr);
        PopError();

        DEBUGMSG ((
            DBG_WARNING,
            "UnicodeToDbcsN error caused memory to be released in pool; may cause harmless PoolMem warnings."
            ));

        return NULL;
    }

    *CharCountToPointerA (DbcsStr, Chars) = 0;

    return DbcsStr;
}


PCWSTR
RealDbcsToUnicodeN (
    IN      POOLHANDLE Pool,            OPTIONAL
    IN      PCSTR StrIn,
    IN      DWORD Chars
    )

/*++

Routine Description:

  Converts a DBCS string to UNICODE.

  WARNING: Currently supports the ANSI code page only.  Later we can fix this.

Arguments:

  Pool      - Specifies pool to allocate UNICODE string from.  If not specified,
              g_TextPool is used.

  StrIn     - Specifies string to be converted

  Chars     - Specifies the number of multibyte characters, excluding the nul,
              to convert.  If -1, all of StrIn will be converted.

Return Value:

  A pointer to the converted UNICODE string, or NULL if an error ocurred.

--*/

{
    PWSTR UnicodeStr;
    DWORD UnicodeStrBufLenBytes;
    DWORD WcharsConverted;
    DWORD StrInBytesToConvert;

    //
    // Find number of multi-byte characters to convert. Punt on case where
    // caller asks for more chars than available.
    //
    if (INVALID_CHAR_COUNT == Chars) {
        Chars = CharCountA (StrIn);
    }

    //
    // Count bytes to convert from the input string (excludes delimiter)
    //
    StrInBytesToConvert = CharCountToPointerA(StrIn, Chars) - StrIn;

    //
    // Get output buffer size, in bytes, including delimiter
    //
    UnicodeStrBufLenBytes = (Chars + 1) * sizeof (WCHAR);

    if (!Pool) {
        Pool = g_TextPool;
    }

    //
    // Get buffer
    //
    UnicodeStr = (PWSTR) PoolMemGetAlignedMemory (Pool, UnicodeStrBufLenBytes);
    if (!UnicodeStr) {
        DEBUGMSG ((DBG_ERROR, "DbcsToUnicodeN could not allocate string"));
        return NULL;
    }

    //
    // Convert
    //
    WcharsConverted = MultiByteToWideChar (
             g_GlobalCodePage,
             0, // MB_ERR_INVALID_CHARS,
             StrIn,
             StrInBytesToConvert,
             UnicodeStr,
             UnicodeStrBufLenBytes
             );

    //
    // Check for conversion error (>0 chars in, 0 chars out)
    //
    if (0 == WcharsConverted && 0 != Chars) {
        PushError();
        PoolMemReleaseMemory (Pool, UnicodeStr);
        PopError();

        DEBUGMSG ((
            DBG_WARNING,
            "DbcsToUnicodeN error caused memory to be released in pool; may cause harmless PoolMem warnings."
            ));

        return NULL;
    }

    //
    // Write delimiter on the output string
    //
    UnicodeStr[WcharsConverted] = 0;

    return UnicodeStr;
}


VOID
FreeConvertedPoolStr (
    IN      POOLHANDLE Pool,            OPTIONAL
    IN      PVOID StrIn
    )

/*++

Routine Description:

  Frees the memory allocated by UnicodeToDbcsN or DbcsToUnicodeN.

Arguments:

  Pool      - Specifies pool to allocate UNICODE string from.  If not specified,
              g_TextPool is used.

  StrIn     - Specifies string that was returned by UnicodeToDebcsN or
              DbcsToUnicodeN.

Return Value:

  none

--*/

{
    if (!StrIn) {
        return;
    }

    if (!Pool) {
        Pool = g_TextPool;
    }

    PoolMemReleaseMemory (Pool, (PVOID) StrIn);
}


PSTR
KnownSizeUnicodeToDbcsN (
    OUT     PSTR StrOut,
    IN      PCWSTR StrIn,
    IN      DWORD Chars
    )

/*++

Routine Description:

  KnownSizeUnicodeToDbcsN converts a UNICODE string to DBCS.  The caller
  manages the outbound buffer.

Arguments:

  StrOut - Receives the DBCS result.
  StrIn  - Specifies the UNICODE string to convert.
  Chars  - Specifies the character count of StrIn (not the byte count), or
           INVALID_CHAR_COUNT for the complete string.

Return Value:

  Returns StrOut.

--*/

{
    DWORD rc;

    if (INVALID_CHAR_COUNT == Chars) {
        Chars = CharCountW (StrIn);
    }

    rc = WideCharToMultiByte (
             g_GlobalCodePage,
             g_MigutilWCToMBFlags,
             StrIn,
             Chars,              // wc input count
             StrOut,
             Chars * 2,
             NULL,
             NULL
             );

    DEBUGMSG_IF ((
        !rc && Chars,
        DBG_WARNING,
        "KnownSizeUnicodeToDbcsN failed."
        ));

    StrOut[rc] = 0;

    return StrOut;
}


PWSTR
KnownSizeDbcsToUnicodeN (
    OUT     PWSTR StrOut,
    IN      PCSTR StrIn,
    IN      DWORD Chars
    )

/*++

Routine Description:

  KnownSizeDbcsToUnicodeN converts a DBCS string to UNICODE.  The caller
  manages the outbound buffer.

Arguments:

  StrOut - Receives the UNICODE result.
  StrIn  - Specifies the DBCS string to convert.
  Chars  - Specifies the character count of StrIn (not the byte count), or
           INVALID_CHAR_COUNT for the complete string.

Return Value:

  Returns StrOut.

--*/

{
    DWORD rc;
    DWORD StrInBytesToConvert;

    if (INVALID_CHAR_COUNT == Chars) {
        StrInBytesToConvert = ByteCountA (StrIn);
    } else {
        StrInBytesToConvert = CharCountToPointerA (StrIn, Chars) - StrIn;
    }

    rc = MultiByteToWideChar (
             g_GlobalCodePage,
             0, // MB_ERR_INVALID_CHARS,
             StrIn,
             StrInBytesToConvert,
             StrOut,
             StrInBytesToConvert * 2
             );

    DEBUGMSG_IF ((
        !rc && Chars,
        DBG_WARNING,
        "KnownSizeDbcsToUnicodeN failed."
        ));

    StrOut[rc] = 0;

    return StrOut;
}


PSTR
DirectUnicodeToDbcsN (
    OUT     PSTR StrOut,
    IN      PCWSTR StrIn,
    IN      DWORD Bytes
    )

/*++

Routine Description:

  DirectUnicodeToDbcsN converts a UNICODE string to DBCS.  The caller
  manages the outbound buffer.  This function does not check for nuls
  in StrIn when Bytes is non-zero, and it does not terminate the
  string.

Arguments:

  StrOut - Receives the DBCS result.
  StrIn  - Specifies the UNICODE string to convert.
  Bytes  - Specifies the byte count of StrIn, or INVALID_CHAR_COUNT
           for the complete string.

Return Value:

  Returns StrOut.

--*/

{
    DWORD rc;

    if (INVALID_CHAR_COUNT == Bytes) {
        Bytes = ByteCountW (StrIn);
    }

    rc = WideCharToMultiByte (
             g_GlobalCodePage,
             g_MigutilWCToMBFlags,
             StrIn,
             Bytes / sizeof (WCHAR),
             StrOut,
             Bytes,
             NULL,
             NULL
             );

    DEBUGMSG_IF ((
        !rc && Bytes,
        DBG_WARNING,
        "DirectUnicodeToDbcsN failed."
        ));

    return StrOut;
}


PWSTR
DirectDbcsToUnicodeN (
    OUT     PWSTR StrOut,
    IN      PCSTR StrIn,
    IN      DWORD Bytes
    )

/*++

Routine Description:

  DirectDbcsToUnicodeN converts a DBCS string to UNICODE.  The caller
  manages the outbound buffer.  This function does not check for nuls
  in StrIn when Bytes is non-zero, and it does not terminate the string.

Arguments:

  StrOut - Receives the UNICODE result.
  StrIn  - Specifies the DBCS string to convert.
  Bytes  - Specifies the byte count of StrIn, or INVALID_CHAR_COUNT
           for the complete string.

Return Value:

  Returns StrOut.

--*/

{
    DWORD rc;

    if (INVALID_CHAR_COUNT == Bytes) {
        Bytes = ByteCountA (StrIn);
    }

    rc = MultiByteToWideChar (
             g_GlobalCodePage,
             0, // MB_ERR_INVALID_CHARS,
             StrIn,
             Bytes,
             StrOut,
             Bytes * 2
             );

    DEBUGMSG_IF ((
        !rc && Bytes,
        DBG_WARNING,
        "DirectDbcsToUnicodeN failed."
        ));

    return StrOut;
}
