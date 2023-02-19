/*++

Copyright (c) 1997 Microsoft Corporation

Module Name:

  reg.c

Abstract:

  This module implements registry functions for the newfsp service provider

--*/

#include "newfsp.h"

BOOL
GetNewFspRegistryData(
    BOOL          *bLoggingEnabled,
    LPWSTR        lpszLoggingDirectory,
    PDEVICE_INFO  *pDeviceInfo,
    LPDWORD       pdwNumDevices
)
/*++

Routine Description:

  Get the registry data for the newfsp service provider

Arguments:

  bLoggingEnabled - indicates if logging is enabled
  lpszLoggingDirectory - indicates the logging directory
  pDeviceInfo - pointer to the virtual fax devices
  pdwNumDevice - pointer to the number of virtual fax devices

Return Value:

  TRUE on success

--*/
{
    // hServiceProvidersKey is the handle to the fax service providers registry key
    HKEY          hServiceProvidersKey;
    // hNewFspKey is the handle to the newfsp service provider registry key
    HKEY          hNewFspKey;
    // hDevicesKey is the handle to the virtual fax devices registry key
    HKEY          hDevicesKey;
    // dwSubkeys is the number of virtual fax device registry subkeys
    DWORD         dwSubkeys;
    // dwIndex is a counter to enumerate each virtual fax device registry subkey
    DWORD         dwIndex;
    // szDeviceSubkey is the name of a virtual fax device registry subkey
    WCHAR         szDeviceSubkey[MAX_PATH];
    // hDeviceSubkey is the handle to a virtual fax device registry subkey
    HKEY          hDeviceSubkey;
    DWORD         dwType;

    // pCurrentDeviceInfo is a pointer to the current virtual fax device
    PDEVICE_INFO  pCurrentDeviceInfo;

    DWORD         dwLoggingEnabledSize;
    DWORD         dwLoggingDirectorySize;
    DWORD         dwDirectorySize;

    if (bLoggingEnabled != NULL) {
        *bLoggingEnabled = FALSE;
    }

    if (lpszLoggingDirectory != NULL) {
        ZeroMemory(lpszLoggingDirectory, sizeof(WCHAR) * MAX_PATH_LEN);
    }

    if (pDeviceInfo != NULL) {
        *pDeviceInfo = NULL;
    }

    if (pdwNumDevices != NULL) {
        *pdwNumDevices = 0;
    }


    // Open the fax service providers registry key
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, FAX_PROVIDERS_REGKEY, 0, KEY_ALL_ACCESS, &hServiceProvidersKey) != ERROR_SUCCESS) {
        return FALSE;
    }

    // Open the newfsp service provider registry key
    if (RegOpenKeyEx(hServiceProvidersKey, NEWFSP_PROVIDER, 0, KEY_ALL_ACCESS, &hNewFspKey) != ERROR_SUCCESS) {
        RegCloseKey(hServiceProvidersKey);
        return FALSE;
    }

    if (bLoggingEnabled != NULL) {
        // Get the logging enabled
        dwLoggingEnabledSize = sizeof(BOOL);
        RegQueryValueEx(hNewFspKey, NEWFSP_LOGGING_ENABLED, NULL, &dwType, (LPBYTE) bLoggingEnabled, &dwLoggingEnabledSize);
    }

    if (lpszLoggingDirectory != NULL) {
        // Get the logging directory
        dwLoggingDirectorySize = sizeof(WCHAR) * MAX_PATH_LEN;
        RegQueryValueEx(hNewFspKey, NEWFSP_LOGGING_DIRECTORY, NULL, &dwType, (LPBYTE) lpszLoggingDirectory, &dwLoggingDirectorySize);
    }

    if ((pDeviceInfo != NULL) || (pdwNumDevices != NULL)) {
        // Open the virtual fax devices registry key
        if (RegOpenKeyEx(hNewFspKey, NEWFSP_DEVICES, 0, KEY_ALL_ACCESS, &hDevicesKey) != ERROR_SUCCESS) {
            RegCloseKey(hNewFspKey);
            RegCloseKey(hServiceProvidersKey);
            return FALSE;
        }

        // Determine the number of virtual fax device registry subkeys
        if (RegQueryInfoKey(hDevicesKey, NULL, NULL, NULL, &dwSubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            RegCloseKey(hDevicesKey);
            RegCloseKey(hNewFspKey);
            RegCloseKey(hServiceProvidersKey);
            return FALSE;
        }
    }

    if (pdwNumDevices != NULL) {
        if (dwSubkeys < NEWFSP_DEVICE_LIMIT) {
            *pdwNumDevices = dwSubkeys;
        }
        else {
            *pdwNumDevices = NEWFSP_DEVICE_LIMIT;
        }
    }

    if (pDeviceInfo != NULL) {
        if (dwSubkeys > 0) {
            // Allocate a block of memory for the first virtual fax device data
            *pDeviceInfo = MemAllocMacro(sizeof(DEVICE_INFO));
        }

        // Enumerate the virtual fax device registry subkeys
        for (pCurrentDeviceInfo = *pDeviceInfo, dwIndex = 0; (dwIndex < dwSubkeys) && (dwIndex < NEWFSP_DEVICE_LIMIT); pCurrentDeviceInfo = pCurrentDeviceInfo->pNextDeviceInfo, dwIndex++) {
            if (pCurrentDeviceInfo == NULL) {
                // A memory allocation for virtual fax device data failed, so go with what we have so far
                *pdwNumDevices = dwIndex;

                break;
            }

            // Set the name of the virtual fax device registry subkey
            wsprintf(szDeviceSubkey, L"%d", dwIndex);
            // Set the identifier of the virtual fax device
            pCurrentDeviceInfo->DeviceId = dwIndex;

            if (RegOpenKeyEx(hDevicesKey, szDeviceSubkey, 0, KEY_ALL_ACCESS, &hDeviceSubkey) == ERROR_SUCCESS) {
                // Get the incoming fax directory for the virtual fax device
                dwDirectorySize = sizeof(WCHAR) * MAX_PATH_LEN;
                RegQueryValueEx(hDeviceSubkey, NEWFSP_DEVICE_DIRECTORY, NULL, &dwType, (LPBYTE) pCurrentDeviceInfo->Directory, &dwDirectorySize);

                RegCloseKey(hDeviceSubkey);
            }

            // Allocate a block of memory for the next virtual fax device data
            if ((dwIndex < (dwSubkeys - 1)) && (dwIndex < (NEWFSP_DEVICE_LIMIT - 1))) {
                pCurrentDeviceInfo->pNextDeviceInfo = MemAllocMacro(sizeof(DEVICE_INFO));
            }
            else {
                pCurrentDeviceInfo->pNextDeviceInfo = NULL;
            }
        }
    }

    if ((pDeviceInfo != NULL) || (pdwNumDevices != NULL)) {
        RegCloseKey(hDevicesKey);
    }

    RegCloseKey(hNewFspKey);
    RegCloseKey(hServiceProvidersKey);

    return TRUE;
}

VOID
SetNewFspRegistryData(
    BOOL          bLoggingEnabled,
    LPWSTR        lpszLoggingDirectory,
    PDEVICE_INFO  pDeviceInfo
)
/*++

Routine Description:

  Set the registry data for the newfsp service provider

Arguments:

  bLoggingEnabled - indicates if logging is enabled
  lpszLoggingDirectory - indicates the logging directory
  pDeviceInfo - pointer to the virtual fax devices

Return Value:

  None

--*/
{
    // hServiceProvidersKey is the handle to the fax service providers registry key
    HKEY          hServiceProvidersKey;
    // hNewFspKey is the handle to the newfsp service provider registry key
    HKEY          hNewFspKey;
    // hDevicesKey is the handle to the virtual fax devices registry key
    HKEY          hDevicesKey;
    // dwSubkeys is the number of virtual fax device registry subkeys
    DWORD         dwSubkeys;
    // dwIndex is a counter to enumerate each virtual fax device registry subkey
    DWORD         dwIndex;
    // szDeviceSubkey is the name of a virtual fax device registry subkey
    WCHAR         szDeviceSubkey[MAX_PATH];
    // hDeviceSubkey is the handle to a virtual fax device registry subkey
    HKEY          hDeviceSubkey;
    DWORD         dwDisposition;

    // pCurrentDeviceInfo is a pointer to the current virtual fax device
    PDEVICE_INFO  pCurrentDeviceInfo;

    // Open the fax service providers registry key
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, FAX_PROVIDERS_REGKEY, 0, KEY_ALL_ACCESS, &hServiceProvidersKey) != ERROR_SUCCESS) {
        return;
    }

    // Open the newfsp service provider registry key
    if (RegCreateKeyEx(hServiceProvidersKey, NEWFSP_PROVIDER, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hNewFspKey, &dwDisposition) != ERROR_SUCCESS) {
        RegCloseKey(hServiceProvidersKey);
        return;
    }

    // Set the logging enabled
    RegSetValueEx(hNewFspKey, NEWFSP_LOGGING_ENABLED, 0, REG_DWORD, (LPBYTE) &bLoggingEnabled, sizeof(bLoggingEnabled));

    // Set the logging directory
    if (lpszLoggingDirectory != NULL) {
        RegSetValueEx(hNewFspKey, NEWFSP_LOGGING_DIRECTORY, 0, REG_SZ, (LPBYTE) lpszLoggingDirectory, (lstrlen(lpszLoggingDirectory) + 1) * sizeof(WCHAR));
    }

    // Open the virtual fax devices registry key
    if (RegCreateKeyEx(hNewFspKey, NEWFSP_DEVICES, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hDevicesKey, &dwDisposition) != ERROR_SUCCESS) {
        RegCloseKey(hNewFspKey);
        RegCloseKey(hServiceProvidersKey);
        return;
    }

    // Determine the number of virtual fax device registry subkeys
    if (RegQueryInfoKey(hDevicesKey, NULL, NULL, NULL, &dwSubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
        RegCloseKey(hDevicesKey);
        RegCloseKey(hNewFspKey);
        RegCloseKey(hServiceProvidersKey);
        return;
    }

    // Enumerate the virtual fax device registry subkeys
    for (pCurrentDeviceInfo = pDeviceInfo, dwIndex = 0; pCurrentDeviceInfo; pCurrentDeviceInfo = pCurrentDeviceInfo->pNextDeviceInfo, dwIndex++) {
        // Set the name of the virtual fax device registry subkey
        wsprintf(szDeviceSubkey, L"%d", pCurrentDeviceInfo->DeviceId);
        if (RegCreateKeyEx(hDevicesKey, szDeviceSubkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hDeviceSubkey, &dwDisposition) == ERROR_SUCCESS) {
            // Set the incoming fax directory for the virtual fax device
            RegSetValueEx(hDeviceSubkey, NEWFSP_DEVICE_DIRECTORY, 0, REG_SZ, (LPBYTE) pCurrentDeviceInfo->Directory, (lstrlen(pCurrentDeviceInfo->Directory) + 1) * sizeof(WCHAR));

            RegCloseKey(hDeviceSubkey);
        }
    }

    // Delete any removed virtual fax device registry subkeys
    for ( ; dwIndex < dwSubkeys; dwIndex++) {
        // Set the name of the virtual fax device registry subkey
        wsprintf(szDeviceSubkey, L"%d", dwIndex);
        RegDeleteKey(hDevicesKey, szDeviceSubkey);
    }

    RegCloseKey(hDevicesKey);
    RegCloseKey(hNewFspKey);
    RegCloseKey(hServiceProvidersKey);
}
