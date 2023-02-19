//----------------------------------------------------------------------------
//
// Copyright (c) 1997-1999  Microsoft Corporation
// All rights reserved.
//
// File Name:
//      service.c
//
// Description:
//
//
//----------------------------------------------------------------------------

#include "pch.h"
#include "resource.h"

//----------------------------------------------------------------------------
//
// Function: InitSelectServiceListView
//
// Purpose:
//
//----------------------------------------------------------------------------
VOID
InitSelectServiceListView( HWND hDlg, HINSTANCE hInst )
{

    LV_ITEM lvI;                    // list view item structure
    HWND hServiceListView;          // handle to list view window
    NETWORK_COMPONENT *pNetComponent;

    hServiceListView = GetDlgItem( hDlg, IDC_SELECT_SERVICE_LIST );

    for( pNetComponent = NetSettings.NetComponentsList;
         pNetComponent;
         pNetComponent = pNetComponent->next )
    {

        if( pNetComponent->bInstalled == FALSE &&
            pNetComponent->ComponentType == SERVICE )
        {

            //
            //  If it is not a sysprep then just go ahead and add it to the
            //  list view.  If we are doing a sysprep, check to see if this
            //  component is supported by sysprep to see if we should add it
            //  or not
            //
            if( WizGlobals.iProductInstall == PRODUCT_SYSPREP )
            {
                if( pNetComponent->bSysprepSupport )
                {
                    InsertEntryIntoListView( hServiceListView,
                                             (LPARAM) pNetComponent );
                }
            }
            else
            {

                InsertEntryIntoListView( hServiceListView,
                                         (LPARAM) pNetComponent );

            }

        }

    }

    SetListViewSelection( hDlg, IDC_SELECT_SERVICE_LIST, 0 );

}

//----------------------------------------------------------------------------
//
// Function:  OnServiceOk
//
// Purpose:
//
// Arguments: IN HWND hwnd - handle to the dialog
//
// Returns:  VOID
//
//----------------------------------------------------------------------------
static VOID
OnServiceOk( IN HWND hwnd )
{

    LV_ITEM lvI;

    // ISSUE-2002/02/28-stelo- if there is no item selected and the user clicks OK the dialog should NOT close

    // see if there is an item selected
    if(GetSelectedItemFromListView(hwnd, IDC_SELECT_SERVICE_LIST, &lvI)) {

        NETWORK_COMPONENT *pEntry = (NETWORK_COMPONENT *)lvI.lParam;

        pEntry->bInstalled = TRUE;

        // return a 1 to show an item was actually added
        EndDialog(hwnd, 1);
    }
    else {
        // return a 0 to show no items were added because the list is empty
        EndDialog(hwnd, 0);
    }

}

//----------------------------------------------------------------------------
//
// Function:  NotifyHandler
//
// Purpose:
//
// Arguments: IN HWND hwnd - handle to the dialog
//            IN WPARAM wParam -
//            IN LPARAM lParam -
//
// Returns:  BOOL - whether the message was handled or not
//
//----------------------------------------------------------------------------
static BOOL
NotifyHandler( IN HWND hwnd, IN WPARAM wParam, IN LPARAM lParam )
{

    LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
    NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
    NETWORK_COMPONENT *pListViewString = (NETWORK_COMPONENT *)(pLvdi->item.lParam);
    BOOL bStatus = TRUE;

    if( wParam == IDC_SELECT_SERVICE_LIST )
    {
        switch( pLvdi->hdr.code )
        {
            case LVN_GETDISPINFO:
                pLvdi->item.pszText = pListViewString->StrComponentName;
                break;
        }

        switch( pNm->hdr.code )
        {

            case NM_DBLCLK:
            {

                NMITEMACTIVATE *pNmItemActivate = (NMITEMACTIVATE *) lParam;

                //
                // see if the user has double clicked inside the list view
                //

                if( pNm->hdr.idFrom == IDC_SELECT_SERVICE_LIST )
                {

                    //
                    //  Make sure they actually clicked on an item and not just
                    //  empty space
                    //

                    if( pNmItemActivate->iItem != -1 )
                    {
                        OnServiceOk( hwnd );
                    }

                }
                break;

            }

            default:

                bStatus = FALSE;

                break;

        }


    }

    return( bStatus );

}

//----------------------------------------------------------------------------
//
// Function: SelectNetworkServiceDlgProc
//
// Purpose:
//
//----------------------------------------------------------------------------
INT_PTR CALLBACK SelectNetworkServiceDlgProc(
    IN HWND     hwnd,
    IN UINT     uMsg,
    IN WPARAM   wParam,
    IN LPARAM   lParam)
{

    BOOL bStatus = TRUE;

    switch (uMsg) {
        case WM_DESTROY:
            // do nothing
            break;

        case WM_INITDIALOG:
            InitSelectServiceListView(hwnd, FixedGlobals.hInstance);
            break;

        case WM_COMMAND:
            {
                int nButtonId;

                switch ( nButtonId = LOWORD(wParam) ) {
                    case IDOK:
                        OnServiceOk( hwnd );
                        break;

                    case IDCANCEL:
                        // return a 0 to show no items were added
                        EndDialog(hwnd, 0);
                        break;

                    case IDC_HAVEDISK:
                        //  ISSUE-2002/02/28-stelo- this needs to be implemented
                        AssertMsg(FALSE,
                                  "This button has not been implemented yet.");
                        break;

                    default:
                        bStatus = FALSE;
                        break;
                }
                break;
            }
        case WM_NOTIFY:
            NotifyHandler( hwnd, wParam, lParam );
            break;

        default:
            bStatus = FALSE;
            break;
    }
    return bStatus;
}
