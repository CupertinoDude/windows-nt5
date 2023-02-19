#include "faxmapip.h"
#pragma hdrstop

LIST_ENTRY          ProfileListHead;
CRITICAL_SECTION    CsProfile;
DWORD               MapiServiceThreadId;

VOID
MapiServiceThread(
    LPVOID EventHandle
    );

BOOL
MyInitializeMapi(
);

BOOL
InitializeEmail(
    VOID
    )
{
    HANDLE MapiThreadHandle;
    HANDLE MapiInitEvent;


    InitializeListHead( &ProfileListHead );
    InitializeCriticalSection( &CsProfile );

    //
    // create an event for the service thread to set after it has initialized mapi
    //

    MapiInitEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

    if (!MapiInitEvent) {
        DebugPrint(( TEXT("InitializeEmailRouting(): CreateEvent() failed: err = %d\n"), GetLastError ));
        return FALSE;
    }

    MapiThreadHandle = CreateThread(
        NULL,
        1024*100,
        (LPTHREAD_START_ROUTINE) MapiServiceThread,
        (LPVOID) MapiInitEvent,
        0,
        &MapiServiceThreadId
        );

    if (MapiThreadHandle == NULL) {
        DebugPrint(( TEXT("InitializeEmailRouting(): Cannot create MapiServiceThread thread") ));
        return FALSE;
    }

    CloseHandle( MapiThreadHandle );

    //
    // wait for MAPI to initialize
    //

    if (WaitForSingleObject( MapiInitEvent, MILLISECONDS_PER_SECOND * 60) != WAIT_OBJECT_0) {
        DebugPrint(( TEXT("InitializeEmailRouting(): WaitForSingleObject failed - ec = %d"), GetLastError() ));
        return FALSE;
    }

    CloseHandle( MapiInitEvent);

    return TRUE;
}


PPROFILE_INFO
FindProfileByName(
    LPCTSTR ProfileName
    )
{
    PLIST_ENTRY Next;
    PPROFILE_INFO ProfileInfo = NULL;


    if (ProfileName == NULL) {
        return NULL;
    }

    EnterCriticalSection( &CsProfile );

    Next = ProfileListHead.Flink;
    if (Next) {
        while (Next != &ProfileListHead) {
            ProfileInfo = CONTAINING_RECORD( Next, PROFILE_INFO, ListEntry );
            Next = ProfileInfo->ListEntry.Flink;
            if (_tcscmp( ProfileInfo->ProfileName, ProfileName ) == 0) {
                LeaveCriticalSection( &CsProfile );
                return ProfileInfo;
            }
        }
    }

    LeaveCriticalSection( &CsProfile );
    return NULL;
}


extern "C"
LPCWSTR WINAPI
GetProfileName(
    IN LPVOID ProfileInfo
    )
{
    return ((PPROFILE_INFO)ProfileInfo)->ProfileName;
}


extern "C"
LPVOID WINAPI
AddNewMapiProfile(
    LPCTSTR ProfileName,
    BOOL UseMail,
    BOOL ShowPopUp
    )
{
    PPROFILE_INFO ProfileInfo = NULL;

    if (!MapiIsInitialized) {
        return NULL;
    }

    if (ProfileName == NULL) {
        return NULL;
    }

    ProfileInfo = FindProfileByName( ProfileName );
    if (ProfileInfo) {
        return ProfileInfo;
    }

    EnterCriticalSection( &CsProfile );

    ProfileInfo = (PPROFILE_INFO) MemAlloc( sizeof(PROFILE_INFO) );
    if (ProfileInfo) {

        // put the profile name into ProfileInfo and create an event for
        // DoMapiLogon to set when its call to MapiLogonEx has completed

        _tcscpy( ProfileInfo->ProfileName, ProfileName );

        ProfileInfo->EventHandle = CreateEvent( NULL, FALSE, FALSE, NULL );
        ProfileInfo->UseMail = UseMail;

        if(ProfileInfo->EventHandle != NULL) {

            // post a message to the mapi service thread

            PostThreadMessage( MapiServiceThreadId, WM_MAPILOGON, 0, (LPARAM) ProfileInfo );

            // wait for the logon to complete

            if (WaitForSingleObject( ProfileInfo->EventHandle, INFINITE) != WAIT_OBJECT_0) {
                DebugPrint(( TEXT("AddNewMapiProfile - WaitForSingleObject failed - ec = %d"), GetLastError() ));
                ProfileInfo->Session = NULL;
            }

            CloseHandle( ProfileInfo->EventHandle );

            if (!ProfileInfo->Session) {
                DebugPrint(( TEXT("DoMapiLogon() failed: [%s] 0x%08x"), ProfileName, GetLastError() ));
                MemFree( ProfileInfo );
                ProfileInfo = NULL;
                if (ShowPopUp) {
                    ServiceMessageBox(
                        GetString( IDS_NO_MAPI_LOGON ),
                        MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND,
                        TRUE,
                        NULL,
                        ProfileName[0] ? ProfileName : GetString( IDS_DEFAULT )
                        );
                }
            } else {
                InsertTailList( &ProfileListHead, &ProfileInfo->ListEntry );
            }
        }
    }

    LeaveCriticalSection( &CsProfile );

    return ProfileInfo;
}


VOID
MapiServiceThread(
    LPVOID EventHandle
    )
/*++

Routine Description:

    Initializes MAPI and services messages.  MAPI/OLE create windows under the
    covers.

Arguments:

    EventHandle -   Event to set once MAPI is initialized.

Return Value:

    NONE

--*/

{
    BOOL Result;

    Result = MyInitializeMapi();

    SetEvent((HANDLE) EventHandle);

    if (!Result) {
        return;
    }

    while (TRUE) {
        MSG msg;

        Result = GetMessage( &msg, NULL, 0, 0 );

        if (Result == (BOOL) -1) {
            DebugPrint(( TEXT("GetMessage returned an error - ec = %d"), GetLastError() ));
            return;
        }

        if (Result) {
            if (msg.message == WM_MAPILOGON) {
                DoMapiLogon( (PPROFILE_INFO) msg.lParam );
            } else {
                DispatchMessage( &msg );
            }
        }
    }

    return;
}
