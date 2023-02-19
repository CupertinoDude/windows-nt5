/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

    registry.c

Abstract:

    Functions for accessing registry information under:
        HKEY_CURRENT_USER and HKEY_LOCAL_MACHINE

Environment:

        Windows NT fax driver user interface

Revision History:

        01/29/96 -davidx-
                Created it.

        mm/dd/yy -author-
                description

--*/

#include "faxlib.h"
#include "registry.h"


        

LPTSTR
GetRegistryExpandStr(
    HKEY    hRootKey,
    LPTSTR  pKeyName,
    LPTSTR  pValueName
    )

/*++

Routine Description:

    Get a EXPAND_SZ value from the registry

Arguments:

    hRootKey - Specifies a handle to the root registry key
    pKeyName - Specifies the name of the sub registry key
    pValueName - Specifies the name of the registry value

Return Value:

    Pointer to an expanded string, NULL if there is an error

--*/

{
    DWORD   size, type;
    LONG    status;
    HKEY    hRegKey;
    LPTSTR  pRegStr = NULL, pExpandedStr;

    //
    // Get a handle to the user info registry key
    //

    if (! (hRegKey = OpenRegistryKey(hRootKey, pKeyName, FALSE, REG_READONLY)))
        return NULL;

    //
    // Figure out how much memory to allocate
    //

    size = 0;
    status = RegQueryValueEx(hRegKey, pValueName, NULL, &type, NULL, &size);

    if ((status == ERROR_SUCCESS) &&
        (type == REG_EXPAND_SZ || type == REG_SZ) &&
        (pRegStr = MemAlloc(size)))
    {
        //
        // Read the registry value
        //

        status = RegQueryValueEx(hRegKey, pValueName, NULL, NULL, (PBYTE) pRegStr, &size);

        if (status != ERROR_SUCCESS || IsEmptyString(pRegStr)) {

            MemFree(pRegStr);
            pRegStr = NULL;

        } else if (type == REG_EXPAND_SZ) {

            //
            // Substitute any environment variables
            //

            if ((size = ExpandEnvironmentStrings(pRegStr, NULL, 0)) == 0 ||
                (pExpandedStr = MemAlloc(sizeof(TCHAR) * size)) == NULL)
            {
                MemFree(pRegStr);
                pRegStr = NULL;

            } else {

                if (ExpandEnvironmentStrings(pRegStr, pExpandedStr, size) == 0)
                    *pExpandedStr = NUL;

                MemFree(pRegStr);
                pRegStr = pExpandedStr;
            }
        }
    }

    RegCloseKey(hRegKey);
    return pRegStr;
}
        

PDEVMODE
GetPerUserDevmode(
    LPTSTR  pPrinterName
    )

/*++

Routine Description:

    Get per-user devmode information for the specified printer

Arguments:

    pPrinterName - Specifies the name of the printer we're interested in

Return Value:

    Pointer to per-user devmode information read from the registry

--*/

{
    PVOID  pDevmode = NULL;
    HANDLE hPrinter;
    PPRINTER_INFO_9 pPrinterInfo;
    TCHAR  PrinterBuffer[64];
    
    //
    // Make sure the printer name is valid
    //

    if (pPrinterName == NULL) {
        _tcscpy(PrinterBuffer,L"Fax");
    } else {
        _tcscpy(PrinterBuffer,pPrinterName);
    }

    //
    // Open the printer
    //
    if (!OpenPrinter(PrinterBuffer,&hPrinter,NULL) ) {
        return NULL;
    }

    pPrinterInfo = MyGetPrinter(hPrinter,9);
    if (!pPrinterInfo || !pPrinterInfo->pDevMode) {
        ClosePrinter(hPrinter);
        return NULL;
    }

    pDevmode = MemAlloc(sizeof(DRVDEVMODE) );

    if (!pDevmode) {
        MemFree(pPrinterInfo);
        ClosePrinter(hPrinter);
        return NULL;
    }
    
    CopyMemory((PVOID) pDevmode,
               (PVOID) pPrinterInfo->pDevMode,
                sizeof(DRVDEVMODE) );

    MemFree( pPrinterInfo );
    ClosePrinter( hPrinter );
    
    return pDevmode;
}


VOID
SavePerUserDevmode(
    LPTSTR      pPrinterName,
    PDEVMODE    pDevmode
    )

/*++

Routine Description:

    Save per-user devmode information for the specified printer

Arguments:

    pPrinterName - Specifies the name of the printer we're interested in
    pDevmode - Points to the devmode to be saved

Return Value:

    NONE

--*/

{
    HKEY    hRegKey;
    INT     size;

    //
    // Make sure the printer name is valid
    //

    if (pPrinterName == NULL) {

        Error(("Bad printer name\n"));
        return;
    }

    //
    // Open the registry key for write access
    //

    if (! (hRegKey = GetUserInfoRegKey(REGKEY_FAX_DEVMODE, REG_READWRITE)))
        return;

    //
    // Save the devmode information as binary data
    //

    size = pDevmode->dmSize + pDevmode->dmDriverExtra;
    RegSetValueEx(hRegKey, pPrinterName, 0, REG_BINARY, (PBYTE) pDevmode, size);
    RegCloseKey(hRegKey);
}


LPTSTR
GetUserCoverPageDir(
    VOID
    )
{
    LPTSTR CpDirTmp = NULL;
    LPTSTR CpDir = MemAlloc(MAX_PATH*sizeof(TCHAR));

    SHGetSpecialFolderPath(NULL, CpDir , CSIDL_PERSONAL, FALSE);

    CpDirTmp = GetRegistryExpandStr(HKEY_CURRENT_USER, REGKEY_FAX_SETUP, REGVAL_CP_LOCATION);
    if (CpDirTmp == NULL) {
        HKEY hKey = OpenRegistryKey(HKEY_CURRENT_USER,REGKEY_FAX_SETUP,FALSE, REG_READONLY);
        if (hKey) {
            SetRegistryStringExpand(hKey,REGVAL_CP_LOCATION,DEFAULT_COVERPAGE_DIR);
            RegCloseKey(hKey);            
            CpDirTmp = GetRegistryExpandStr(HKEY_CURRENT_USER, REGKEY_FAX_SETUP, REGVAL_CP_LOCATION);
        }
    }

    if (CpDir && CpDirTmp) {
        _tcscat( CpDir, TEXT("\\") );
        _tcscat( CpDir, CpDirTmp );
    }
    
    if (CpDirTmp) {
        MemFree(CpDirTmp);
    }

    return CpDir;
}


LPTSTR
GetCoverPageEditor(
    VOID
    )
{
    LPTSTR CpEd = GetRegistryStringExpand(HKEY_CURRENT_USER, REGKEY_FAX_SETUP, REGVAL_CP_EDITOR);
    if (CpEd == NULL) {
        HKEY hKey = OpenRegistryKey(HKEY_CURRENT_USER,REGKEY_FAX_SETUP,FALSE, REG_READWRITE);
        if (hKey) {
            SetRegistryStringExpand(hKey,REGVAL_CP_EDITOR,DEFAULT_COVERPAGE_EDITOR);
            RegCloseKey(hKey);
            return GetRegistryExpandStr(HKEY_CURRENT_USER, REGKEY_FAX_SETUP, REGVAL_CP_EDITOR);
        }
    }
    return CpEd;
}
