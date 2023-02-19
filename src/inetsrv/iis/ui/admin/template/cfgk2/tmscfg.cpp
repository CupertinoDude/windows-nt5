///////////////////////////////////////////////////////////////////////////////
//
// File:    tmscfg.cpp
// Purpose: API Entry points for service ui version 2.0
//

#include "stdafx.h"

#define DLL_BASED __declspec(dllexport)

#include "tmscfg.h"
#include "tmservic.h"
#include "tmsessio.h"

// ===========================================================================
//
// Standard configuration Information
//
// ===========================================================================

//
// Is this server discoverable by INETSLOC?
//
#define INETSLOC_DISCOVERY          FALSE

#if (INETSLOC_DISCOVERY) && !defined(_SVCLOC_)
    #error You must include svcloc.h.
#endif

//
// If INETSLOC_DISCOVERY == TRUE, define the discovery MASK here.
//
#if (INETSLOC_DISCOVERY) 
    #define INETSLOC_MASK               
#else  // (!INETSLOC_DISCOVERY) 
    #define INETSLOC_MASK           (ULONGLONG)(0x00000000)
#endif // (INETSLOC_DISCOVERY) 

//
// Do we use the service control manager to control
// the service we administer?
//
#define USE_SC_MANAGER              FALSE

//
// Can we change the service state (start/pause/continue)?
//
#define CAN_CHANGE_SERVICE_STATE    TRUE

//
// Can we pause this service?
//
#define CAN_PAUSE_SERVICE           FALSE

//
// Short descriptive name of the service.  This
// is what will show up as the name of the service
// in the internet manager tool.
//
// Issue: I'm assuming here that this name does NOT
//        require localisation.
//
#define SERVICE_SHORT_NAME      _T("Template K2")

//
// Longer name.  This is the text that shows up in
// the tooltips text on the internet manager
// tool.  This probably should be localised.
//

#define SERVICE_LONG_NAME      _T("Template K2 Service")

//
// Use normal colour mapping.
//
#define NORMAL_TB_MAPPING          TRUE

//
// Toolbar button background mask. This is
// the colour that gets masked out in
// the bitmap file and replaced with the
// actual button background.  This setting
// is automatically assumed to be lt. gray
// if NORMAL_TB_MAPPING (above) is TRUE
//
#define BUTTON_BMP_BACKGROUND       RGB(192, 192, 192)      // Lt. Gray

//
// Resource ID of the toolbar button bitmap.
//
// The bitmap must be 17x17
//
#define BUTTON_BMP_ID               IDB_TEMPLATE

//
// Similar to BUTTON_BMP_BACKGROUND, this is the
// background mask for the service ID
//
#define SERVICE_BMP_BACKGROUND      RGB(192, 192, 192)      // Lt. Gray

//
// Bitmap id which is used in the service view
// of the service manager.  
//
// The bitmap must be 17x17, and can be the
// same as BUTTON_BMP_ID bitmap.
//
#define SERVICE_BMP_ID              IDB_TEMPLATE_VIEW

//
// /* K2 */
//
// Similar to BUTTON_BMP_BACKGROUND, this is the
// background mask for the vdir bitmap
//
#define VDIR_BMP_BACKGROUND         RGB(192, 192, 192)      // Lt. Gray

//
// /* K2 */
//
// Bitmap id which is used virtual directory
//
// The bitmap must be 17x17.
//
#define VDIR_BMP_ID                 IDB_TEMPLATE_DIR


// ===========================================================================
//
// End Of Standard configuration Information
//
// ===========================================================================

//
// Helper Macros
//
#define IS_NETBIOS_NAME(lpstr) (*lpstr == _T('\\'))
//
// Return the portion of a computer name without the backslashes
//
#define PURE_COMPUTER_NAME(lpstr) (IS_NETBIOS_NAME(lpstr) ? lpstr + 2 : lpstr)
 
//
// Global DLL instance
//
HINSTANCE hInstance;

TCHAR g_szComment[ MAX_COMMENT_LEN + 1 ] = _T("Test Entry");

// ============================================================================
//
// ISM API Functions
//
// ============================================================================

///////////////////////////////////////////////////////////////////////////////
//
// Return service-specific information back to
// to the application.  This function is called
// by the service manager immediately after
// LoadLibary();  The size element must be
// set prior to calling this API.
//
DLL_BASED DWORD  APIENTRY
ISMQueryServiceInfo(
    ISMSERVICEINFO * psi        // Service information returned.
    )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState() );

    if ( psi == NULL
      || psi->dwSize < ISMSERVICEINFO_SIZE
       )
    {
        TRACE(_T("ISMQueryServiceInfo: ISMSERVICEINFO invalid\n"));
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

#ifdef _DEBUG

    if (psi->dwSize != ISMSERVICEINFO_SIZE)
    {
        TRACE(_T("Warning: internet manager is newer than DLL\n"));
    }

#endif // _DEBUG

    psi->dwSize = ISMSERVICEINFO_SIZE;
    psi->dwVersion = ISM_VERSION;

    psi->flServiceInfoFlags = 0

        | ISMI_VIRTUALROOTS

#if (INETSLOC_DISCOVERY)
        | ISMI_INETSLOCDISCOVER
#endif 
#if (CAN_CHANGE_SERVICE_STATE)
        | ISMI_CANCONTROLSERVICE   
#endif 
#if (CAN_PAUSE_SERVICE)
        | ISMI_CANPAUSESERVICE
#endif 
#if (NORMAL_TB_MAPPING)
        | ISMI_NORMALTBMAPPING
#endif
        ; /**/

    ASSERT(::lstrlen(SERVICE_LONG_NAME) <= MAX_LNLEN);
    ASSERT(::lstrlen(SERVICE_SHORT_NAME) <= MAX_SNLEN);

    psi->ullDiscoveryMask = INETSLOC_MASK;
    psi->rgbButtonBkMask = BUTTON_BMP_BACKGROUND;
    psi->nButtonBitmapID = BUTTON_BMP_ID;
    psi->rgbServiceBkMask = SERVICE_BMP_BACKGROUND;
    psi->nServiceBitmapID = SERVICE_BMP_ID;
    ::lstrcpy(psi->atchShortName, SERVICE_SHORT_NAME);
    ::lstrcpy(psi->atchLongName, SERVICE_LONG_NAME);

    //
    // /* K2 */
    //
    psi->rgbVDirBkMask = VDIR_BMP_BACKGROUND;
    psi->nVDirBitmapID = VDIR_BMP_ID ;

    return ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// Discover machines running this service.  This is
// only necessary for services not discovered with
// inetscloc (which don't give a mask)
//
DLL_BASED DWORD APIENTRY
ISMDiscoverServers(
    ISMSERVERINFO * psi,        // Server info buffer.
    DWORD * pdwBufferSize,      // Size required/available.  
    int * cServers              // Number of servers in buffer.
    )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState() );

#if (INETSLOC_DISCOVERY)

    #error You do not need to provide a discovery method.

#else
    //
    // Dummy example, which discovers 2 servers running our service
    //
    int cServersFound = 2;
    DWORD dwRequiredSize = sizeof(ISMSERVERINFO) * cServersFound;

    if (*pdwBufferSize < dwRequiredSize || psi == NULL)
    {
        *pdwBufferSize = dwRequiredSize;
        *cServers = cServersFound;

        return ERROR_MORE_DATA;
    }

    psi[0].dwSize = ISMSERVERINFO_SIZE;
    ::lstrcpy(psi[0].atchServerName, _T("\\FOO"));
    ::lstrcpy(psi[0].atchComment, _T("Dummy Discovery 1"));
    psi[0].nState = INetServiceUnknown;

    psi[1].dwSize = ISMSERVERINFO_SIZE;
    ::lstrcpy(psi[1].atchServerName, _T("\\BAR"));
    ::lstrcpy(psi[1].atchComment, _T("Dummy Discovery 2"));
    psi[1].nState = INetServiceUnknown;

    return ERROR_SUCCESS;

#endif // (INETSLOC_DISCOVERY)
}

///////////////////////////////////////////////////////////////////////////////
//
// Get information about a specific server with
// regards to this service.  Usually this uses
// the service control manager
//
DLL_BASED DWORD APIENTRY
ISMQueryServerInfo(
    LPCTSTR lpstrServerName,    // Name of server.
    ISMSERVERINFO * psi         // Server information returned.
    )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState() );

    if ( psi == NULL
      || psi->dwSize < ISMSERVERINFO_SIZE
      || ::lstrlen(lpstrServerName) > MAX_SERVERNAME_LEN
       )
    {
        TRACE(_T("ISMQueryServerInfo: bad parameters\n"));
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

#ifdef _DEBUG

    if (psi->dwSize != ISMSERVERINFO_SIZE)
    {
        TRACE(_T("Warning internet manager is newer than DLL\n"));
    }

#endif // _DEBUG

    psi->dwSize = ISMSERVERINFO_SIZE;
    ::lstrcpy(psi->atchServerName, lpstrServerName);
    ::lstrcpy(psi->atchComment, _T("Test Entry"));
    psi->nState = INetServiceRunning;

    return ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// Change the service state of the servers (to paused/continue, started,
// stopped, etc)
//
DLL_BASED DWORD APIENTRY
ISMChangeServiceState(
    int nNewState,              // INetService* definition.
    int * pnCurrentState,       // Ptr to current state (will be changed
    DWORD dwReserved,           // Reserved: must be 0
    LPCTSTR lpstrServers        // Double NULL terminated list of servers.
    )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState() );

    if ( dwReserved != 0L
      || nNewState < INetServiceStopped 
      || nNewState > INetServicePaused
       )
    {
        TRACE(_T("ISMChangeServiceState: Invalid information passed\n"));
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

    ::Sleep(1000L);
    *pnCurrentState = nNewState;

    return ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// Display configuration property sheet.
//
DLL_BASED DWORD APIENTRY
ISMConfigureServers(
    HWND hWnd,                  // Main app window handle
    DWORD dwReserved,           // Reserved: must be 0
    LPCTSTR lpstrServers        // Double NULL terminated list of servers
    )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState() );

    DWORD err;

    if (dwReserved != 0L)
    {
        TRACE(_T("ISMConfigureServers: Invalid information passed\n"));
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

    //
    // Convert the list of servers to a 
    // more manageable CStringList.
    //
    LPCTSTR pBuf = lpstrServers;
    CStringList strlServers;
    while (*pBuf != TCHAR('\0'))
    {
        CString strTmp = pBuf;
        strlServers.AddTail( strTmp );
        pBuf += strTmp.GetLength() + 1;
    }

    if (strlServers.GetCount() == 0)
    {
        TRACE(_T("Error: strlServers Count == 0.\n"));
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

    CString strCaption;

    if (strlServers.GetCount() == 1)
    {
        CString str;

        VERIFY(str.LoadString(IDS_CAPTION));
        strCaption.Format(str, PURE_COMPUTER_NAME(lpstrServers));
    }
    else // Multiple server caption
    {
        VERIFY(strCaption.LoadString(IDS_CAPTION_MULTIPLE));
    }

    //
    // Show property sheet with some dummy pages on it.
    //
    CPropertySheet sheet(strCaption, CWnd::FromHandle(hWnd));
    CTMServicePage page1;
    CTMSessionsPage page2;

    sheet.AddPage(&page1);
    sheet.AddPage(&page2);

    sheet.DoModal();

    //
    // Didn't do anything, but what we did, we did
    // perfectly.
    //
    err = ERROR_SUCCESS;

    return err;
}

ISMVROOTINFO vd[] = 
{
    { ISMVROOTINFO_SIZE, _T("/"),       _T("C:\\INETPUB") },
    { ISMVROOTINFO_SIZE, _T("Scripts"), _T("C:\\INETPUB\\SCRIPTS") },
};

//
// /* K2 */
//
// Enumerate vroots.  This API should first be called with a buffer
// size of 0, which will return the required number of bytes
//
DLL_BASED DWORD APIENTRY
ISMEnumerateVirtualRoots(
    ISMVROOTINFO * pii,         // VRoot info buffer
    DWORD * pdwBufferSize,      // Size required/available.  
    int * pcVRoots,             // Number of vroots in buffer.
    DWORD dwReserved,           // Reserved: must be 0
    LPCTSTR lpstrServer         // A single server
    )
{
    AFX_MANAGE_STATE(::AfxGetStaticModuleState() );

    DWORD err = ERROR_SUCCESS;

    if(pii == NULL)
    {
        *pcVRoots = sizeof(vd) / sizeof(vd[0]);
        *pdwBufferSize = *pcVRoots * sizeof(ISMVROOTINFO);

        return err;
    }

    //
    // Closing call to IISEnumerateInstances
    //

    //
    // Re-compute vroot count, since it's not guaranteed
    // to be correct
    //
    *pcVRoots = *pdwBufferSize / ISMVROOTINFO_SIZE;
    if (*pcVRoots <= 0)
    {
        //
        // VRoot count is invalid
        //
        ASSERT(FALSE);

        return ERROR_INVALID_PARAMETER;
    }

    for(int i = 0; i < *pcVRoots; ++i)
    {
        pii[i].dwSize = ISMVROOTINFO_SIZE;

        ::lstrcpy(pii[i].atchAlias, vd[i].atchAlias);
        ::lstrcpy(pii[i].atchPath, vd[i].atchPath);
    }

    return ERROR_SUCCESS;
}


// ============================================================================
//
// End of ISM API Functions
//
// ============================================================================

//
// Perform additional initialisation as necessary
//
void
InitializeDLL()
{
#ifdef _DEBUG

    afxMemDF |= checkAlwaysMemDF;

#endif // _DEBUG
}

//
// Declare the one and only dll object
//
CConfigDll NEAR theApp;

CConfigDll::CConfigDll(
    IN LPCTSTR pszAppName OPTIONAL
    )
/*++

Routine Description:

    Constructor for USRDLL

Arguments:

    LPCTSTR pszAppName : Name of the app or NULL to load from resources

Return Value:

    N/A

--*/
    : CWinApp(pszAppName)
{
}

BOOL
CConfigDll::InitInstance()
/*++

Routine Description:

    Initialise current instance of the DLL

Arguments:

    None

Return Value:

    TRUE for successful initialisation, FALSE otherwise

--*/
{
    BOOL bInit = CWinApp::InitInstance();

    hInstance = ::AfxGetInstanceHandle();
    ASSERT(hInstance);
    InitializeDLL();

    return bInit;
}

int
CConfigDll::ExitInstance()
/*++

Routine Description:

    Clean up current instance

Arguments:

    None

Return Value:

    The application�s exit code; 0 indicates no errors, and values greater
    than 0 indicate an error.

--*/
{
    return CWinApp::ExitInstance();
}

//
// Message Map
//
BEGIN_MESSAGE_MAP(CConfigDll, CWinApp)
    //{{AFX_MSG_MAP(CConfigDll)
    //}}AFX_MSG_MAP
    //
    // Global help commands (must be present)
    //
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
    ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp)
END_MESSAGE_MAP()
