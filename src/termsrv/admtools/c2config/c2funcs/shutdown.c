/*++

Copyright (c) 1994  Microsoft Corporation

Module Name:

    shutdown.C

Abstract:

    functions used to enable/disable the shutdown button in the logon screen
    on the current system.

Author:

    Bob Watson (a-robw)

Revision History:

    23 Dec 94

--*/
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <c2dll.h>
#include <c2inc.h>
#include <c2utils.h>
#include "c2funcs.h"
#include "c2funres.h"

// local constants

// define action codes here. They are only meaningful in the
// context of this module.

#define AC_SHUTDOWN_BTN_NOCHANGE    0
#define AC_SHUTDOWN_BTN_UPDATE      1

#define SECURE    C2DLL_SECURE

static
BOOL
SetShutDownButton (
    DWORD   dwNewValue
)
{
    HKEY    hKeyWinlogon = NULL;
    LONG    lStatus = ERROR_SUCCESS;
    BOOL    bReturn = FALSE;
    UINT    nNewData;
    LPCTSTR szNewString;

    SET_WAIT_CURSOR;

    lStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE,
        GetStringResource (GetDllInstance(), IDS_WINLOGON_KEY),
        0L,
        KEY_SET_VALUE,
        &hKeyWinlogon);

    if (lStatus == ERROR_SUCCESS) {
        // key opened OK so set value
        nNewData = (dwNewValue ? IDS_1 : IDS_0);
        szNewString = GetStringResource (GetDllInstance(), nNewData);
        lStatus = RegSetValueEx (
            hKeyWinlogon,
            GetStringResource (GetDllInstance(), IDS_SHUTDOWN_BTN_VALUE),
            0L,
            REG_SZ,
            (CONST LPBYTE)szNewString,
            (lstrlen (szNewString) + 1) * sizeof(TCHAR));

        if (lStatus == ERROR_SUCCESS) {
            bReturn = TRUE;
        } else {
            bReturn = FALSE;
        }
        RegCloseKey (hKeyWinlogon);
    } else {
        bReturn = FALSE;
        SetLastError (ERROR_BADKEY);
    }

    SET_ARROW_CURSOR;

    return bReturn;
}

static
BOOL
GetShutDownButton (
)
{
    HKEY    hKeyWinlogon = NULL;
    LONG    lStatus = ERROR_SUCCESS;
    DWORD   dwType = 0;
    BOOL    bReturn = FALSE;
    TCHAR   szValue[MAX_PATH];
    DWORD   dwValueSize = MAX_PATH * sizeof(TCHAR);

    SET_WAIT_CURSOR;

    lStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE,
        GetStringResource (GetDllInstance(), IDS_WINLOGON_KEY),
        0L,
        KEY_READ,
        &hKeyWinlogon);

    if (lStatus == ERROR_SUCCESS) {
        // key opened OK so check value
        lStatus = RegQueryValueEx (
            hKeyWinlogon,
            (LPTSTR)GetStringResource (GetDllInstance(), IDS_SHUTDOWN_BTN_VALUE),
            (LPDWORD)NULL,
            &dwType,
            (LPBYTE)szValue,
            &dwValueSize);

        if (lStatus == ERROR_SUCCESS) {
            // value read successfully so check it out
            if (dwType == REG_SZ) {
                // if the button is visible, then this value is 1
                if (lstrcmp(szValue, GetStringResource(GetDllInstance(), IDS_1)) == 0) {
                    bReturn = TRUE;
                } else {
                    bReturn = FALSE;
                }
                SetLastError (ERROR_SUCCESS);
            } else {
                // wrong data type returned
                bReturn = FALSE;
                SetLastError (ERROR_CANTREAD);
            }
        } else {
            // no value present
            bReturn = FALSE;
            SetLastError (ERROR_CANTREAD);
        }
        RegCloseKey (hKeyWinlogon);
    } else {
        bReturn = FALSE;
        SetLastError (ERROR_BADKEY);
    }

    SET_ARROW_CURSOR;

    return bReturn;
}

BOOL CALLBACK
C2ShutDownButtonDlgProc(
    IN  HWND hDlg,           // window handle of the dialog box
	IN  UINT message,        // type of message
    IN  WPARAM  wParam,
    IN  LPARAM  lParam
)
/*++

Routine Description:

    Window procedure for Last User Display dialog box

Arguments:

    Standard DlgProc arguments

ReturnValue:

    TRUE    the message was handled by this routine
    FALSE   DefDialogProc should handle the message

--*/
{
    static  LPDWORD   lpdwNewValue;
    DWORD   dwLogSetting = 0;

    switch (message) {
        case WM_INITDIALOG:
            // save the pointer to the new value
            lpdwNewValue = (LPDWORD)lParam;

            // get Shutdown button state for check box
            CheckDlgButton (hDlg, IDC_HIDE_SHUTDOWN_BUTTON,
                (GetShutDownButton() ? UNCHECKED : CHECKED));

            SetFocus (GetDlgItem (hDlg, IDOK)); // set focus to OK Button
            return FALSE; // we don't want Windows to set the focus

        case WM_COMMAND:
            switch (LOWORD(wParam)){
                case IDOK:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        // exit and return button that caused exit
                        if (IsDlgButtonChecked (hDlg, IDC_HIDE_SHUTDOWN_BUTTON) == CHECKED) {
                            *lpdwNewValue = FALSE; // hide the button
                            EndDialog (hDlg, (int)LOWORD(wParam));
                        } else {
                            *lpdwNewValue = TRUE; // show the button
                            EndDialog (hDlg, (int)LOWORD(wParam));
                        }
                        return TRUE;
                    } else {
                        return FALSE;
                    }

                case IDCANCEL:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        // exit and return button that caused exit
                        *lpdwNewValue = 0;
                        EndDialog (hDlg, (int)LOWORD(wParam));
                        return TRUE;
                    } else {
                        return FALSE;
                    }

                case IDC_C2:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        CheckDlgButton (hDlg, IDC_HIDE_SHUTDOWN_BUTTON, CHECKED);
                        return TRUE;
                    } else {
                        return FALSE;
                    }

                case IDC_HELP:
                    PostMessage (GetParent(hDlg), UM_SHOW_CONTEXT_HELP, 0, 0);
                    return TRUE;

                default:
                    return FALSE;
            }

        default:
	        return (FALSE); // Didn't process the message
    }
}

LONG
C2QueryShutDownButton (
    IN  LPARAM   lParam
)
/*++

Routine Description:

    Function called to find out if the OS/2 subsystem is installed
        on the system. For C2 compliance, OS/2 must not be
        allowed on the system.

Arguments:

    Pointer to the Dll data block passed as an LPARAM.

ReturnValue:

    ERROR_SUCCESS if the function succeeds otherwise a
    WIN32 error is returned if an error occurs

--*/
{
    PC2DLL_DATA  pC2Data;
    DWORD        dwLogSetting = 0;
    UINT         nMsgString;

    if (lParam != 0) {
        pC2Data = (PC2DLL_DATA)lParam;
        pC2Data->lC2Compliance = SECURE;   // assume true for now
        if (!GetShutDownButton()) {
            // shutdown button is not displayed on the logon dialog
            pC2Data->lC2Compliance = SECURE;
            nMsgString = IDS_SHUTDOWN_BTN_HIDDEN;
        } else {
            // shutdown button is displayed on the logon dialog
            pC2Data->lC2Compliance = C2DLL_NOT_SECURE;
            nMsgString = IDS_SHUTDOWN_BTN_DISPLAYED;
        }
        lstrcpy (pC2Data->szStatusName,
            GetStringResource (GetDllInstance(), nMsgString));
    } else {
        return ERROR_BAD_ARGUMENTS;
    }

    return ERROR_SUCCESS;
}

LONG
C2SetShutDownButton (
    IN  LPARAM   lParam
)
/*++

Routine Description:

    Function called to change the current state of this configuration
        item based on an action code passed in the DLL data block. If
        this function successfully sets the state of the configuration
        item, then the C2 Compliance flag and the Status string to reflect
        the new value of the configuration item.

Arguments:

    Pointer to the Dll data block passed as an LPARAM.

ReturnValue:

    ERROR_SUCCESS if the function succeeds otherwise a
    WIN32 error is returned if an error occurs

--*/
{
    PC2DLL_DATA  pC2Data;
    UINT         nMsgString;

    if (lParam != 0) {
        pC2Data = (PC2DLL_DATA)lParam;
        // action valie = the new value of the wrap setting
        if (pC2Data->lActionCode != AC_SHUTDOWN_BTN_NOCHANGE) {
            nMsgString = 0;
            if (pC2Data->lActionCode == AC_SHUTDOWN_BTN_UPDATE) {
                if (pC2Data->lActionValue) {
                    if (SetShutDownButton (TRUE)) {
                        pC2Data->lC2Compliance = C2DLL_NOT_SECURE;
                        nMsgString = IDS_SHUTDOWN_BTN_DISPLAYED;
                    } else {
                        DisplayDllMessageBox (
                            pC2Data->hWnd,
                            IDS_SHUTDOWN_ERROR_NOT_SET,
                            IDS_SHUTDOWN_CAPTION,
                            MBOK_EXCLAIM);
                    }
                } else {
                    if (SetShutDownButton (FALSE)) {
                        pC2Data->lC2Compliance = SECURE;
                        nMsgString = IDS_SHUTDOWN_BTN_HIDDEN;
                    } else {
                        DisplayDllMessageBox (
                            pC2Data->hWnd,
                            IDS_SHUTDOWN_ERROR_NOT_SET,
                            IDS_SHUTDOWN_CAPTION,
                            MBOK_EXCLAIM);
                    }
                }
            }
            if (nMsgString != 0) {
                // update status string if set was successful
                lstrcpy (pC2Data->szStatusName,
                    GetStringResource (GetDllInstance(), nMsgString));
            }
        }
        // update action values
        pC2Data->lActionCode = 0;
        pC2Data->lActionValue = 0;
    } else {
        return ERROR_BAD_ARGUMENTS;
    }

    return ERROR_SUCCESS;
}

LONG
C2DisplayShutDownButton (
    IN  LPARAM   lParam
)
/*++

Routine Description:

    Function called to display more information on the configuration
        item and provide the user with the option to change the current
        setting  (if appropriate). If the User "OK's" out of the UI,
        then the action code field in the DLL data block is set to the
        appropriate (and configuration item-specific) action code so the
        "Set" function can be called to perform the desired action. If
        the user Cancels out of the UI, then the Action code field is
        set to 0 (no action) and no action is performed.

Arguments:

    Pointer to the Dll data block passed as an LPARAM.

ReturnValue:

    ERROR_SUCCESS if the function succeeds otherwise a
    WIN32 error is returned if an error occurs

--*/
{
    PC2DLL_DATA pC2Data;
    DWORD       dwNewValue;

    if (lParam != 0) {
        pC2Data = (PC2DLL_DATA)lParam;
        if (DialogBoxParam (
            GetDllInstance(),
            MAKEINTRESOURCE (IDD_SHUTDOWN_BUTTON),
            pC2Data->hWnd,
            C2ShutDownButtonDlgProc,
            (LPARAM)&dwNewValue) == IDOK) {
            pC2Data->lActionValue = dwNewValue;
            pC2Data->lActionCode = AC_SHUTDOWN_BTN_UPDATE;
        } else {
            // no action
            pC2Data->lActionCode = AC_SHUTDOWN_BTN_NOCHANGE;
        }
    } else {
        return ERROR_BAD_ARGUMENTS;
    }
    return ERROR_SUCCESS;
}









