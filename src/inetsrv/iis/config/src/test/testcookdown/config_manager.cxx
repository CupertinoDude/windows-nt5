/*++

Copyright (c) 1998-1999 Microsoft Corporation

Module Name:

    config_manager.cxx

Abstract:

    The IIS web admin service configuration manager class implementation. 
    This class manages access to configuration metadata, as well as 
    handling dynamic configuration changes.

    Threading: Access to configuration metadata is done on the main worker 
    thread. Configuration changes arrive on COM threads (i.e., secondary 
    threads), and so they post work items to process the changes on the main 
    worker thread.

Author:

    Seth Pollack (sethp)        5-Jan-1999

Revision History:

--*/

#include    "precomp.h"

#define		REGSYSDEFNS_DEFINE
#include	"RegSysDefns.h"
#include	"stable_i.c"

#define cAPPPOOLS_BASEVERSION		1
#define cSITES_BASEVERSION			1
#define cAPPS_BASEVERSION			1


LPCWSTR	g_wszCookDownFile		= L"WASMB.CLB";

STDAPI CookDown();

/***************************************************************************++

Routine Description:

    Constructor for the CONFIG_MANAGER class.

Arguments:

    None.

Return Value:

    None.

--***************************************************************************/

CONFIG_MANAGER::CONFIG_MANAGER(
    )
{

    m_Signature = CONFIG_MANAGER_SIGNATURE;


    m_CoInitialized = FALSE;

	m_pISTDisp	= NULL;

}   // CONFIG_MANAGER::CONFIG_MANAGER



/***************************************************************************++

Routine Description:

    Destructor for the CONFIG_MANAGER class.

Arguments:

    None.

Return Value:

    None.

--***************************************************************************/

CONFIG_MANAGER::~CONFIG_MANAGER(
    )
{

    DBG_ASSERT( m_Signature == CONFIG_MANAGER_SIGNATURE );


    //
    // Make sure we CoUninitialize on the same thread we CoInitialize'd on
    // originally.
    //

    DBG_ASSERT( ON_MAIN_WORKER_THREAD );


    if ( m_CoInitialized )
    {
        CoUninitialize();
        m_CoInitialized = FALSE;
    }


	if(NULL != m_pISTDisp)
		m_pISTDisp->Release();

    m_Signature = CONFIG_MANAGER_SIGNATURE_FREED;

}   // CONFIG_MANAGER::~CONFIG_MANAGER



/***************************************************************************++

Routine Description:

    Initialize the configuration manager.

Arguments:

    None.

Return Value:

    HRESULT

--***************************************************************************/

HRESULT
CONFIG_MANAGER::Initialize(
    )
{

    HRESULT hr = S_OK;


    DBG_ASSERT( ON_MAIN_WORKER_THREAD );


    hr = CoInitializeEx(
				NULL,                   // reserved
                COINIT_MULTITHREADED    // threading model
                );

    if ( FAILED( hr ) )
    {
    
        DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Initializing COM failed\n"
            ));
	
        goto exit;
    }

    m_CoInitialized = TRUE;
    

    hr = CookDown();

    if ( FAILED( hr ) )
    {
    
        DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Cook Down of configuration information failed - Will attempt to read information from the old Cooked down format, if it exists.\n"
            ));

        goto exit;
    }


    hr = ReadAllConfiguration();

    if ( FAILED( hr ) )
    {
    
       DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Reading initial configuration failed\n"
            ));

        goto exit;
    }


exit:

    return hr;

}   // CONFIG_MANAGER::Initialize



/***************************************************************************++

Routine Description:

    Read the initial web server configuration from the metabase.

Arguments:

    None.

Return Value:

    HRESULT

--***************************************************************************/

HRESULT
CONFIG_MANAGER::ReadAllConfiguration(
    )
{
	HRESULT hr;

    hr = ReadAllAppPools();

    if ( FAILED( hr ) )
    {
    
      DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Reading app pools failed\n"
            ));
        goto exit;
    }


    hr = ReadAllVirtualSites();

    if ( FAILED( hr ) )
    {
    
      DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Reading virtual sites failed\n"
            ));

        goto exit;
    }


exit:

    return hr;

}   // CONFIG_MANAGER::ReadAllConfiguration



/***************************************************************************++

Routine Description:

    Enumerate and read all app pools configured in the metabase.

Arguments:

    pMetabase - An instance of the MB class, opened to IIS_MD_W3SVC.

Return Value:

    HRESULT

--***************************************************************************/

HRESULT
CONFIG_MANAGER::ReadAllAppPools(
    )
{
	HRESULT							hr;
	ISimpleTableRead2				*pISTAppPools	= NULL;
	ULONG							cBaseVersion;
	ULONG							i;
	WCHAR*							wszAppPoolName;
	APP_POOL_CONFIG					AppPoolConfig;
    DWORD					        ValidAppPoolCount	= 0;
	static const ULONG				aiAppPoolColumns [] = { 
															iDT_AppPoolID, 
															iDT_PeriodicRestartTime,
															iDT_PeriodicRestartRequests,
															iDT_MaxProcesses,
															iDT_PingingEnabled,
															iDT_IdleTimeout,
															iDT_RapidFailProtection	
															};
	static const ULONG				cAppPoolColumns		= sizeof (aiAppPoolColumns) / sizeof (ULONG);
	static const ULONG				cmaxAppPoolColumns	= iDT_RapidFailProtection + 1;
	void*							apvAppPoolValues [cmaxAppPoolColumns];

	hr = GetTable (	
 				    didURTGLOBAL, 
				    tidAPPPOOLS,
				    0,
				    (void**) &pISTAppPools	
				  );

    if ( FAILED( hr ) )
    {
    
      DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Error getting AppPools table\n"
            ));

        goto exit;
    }

// TODO: Check with Robert as to what this version check means.
/*	#ifdef _DEBUG
		hr = pISTAppPools->GetTableMeta (
										  &cBaseVersion, 
										  NULL, 
										  NULL, 
										  NULL
										);
	
		if (cAPPPOOLS_BASEVERSION != cBaseVersion)
		{
			hr = E_UNEXPECTED;
			goto exit;
		}

	#endif // _DEBUG
*/
	for (i = 0;; i++)
	{
		hr = pISTAppPools->MoveToRowByIndex (i);

		if (E_ST_NOMOREROWS == hr)
		{
			hr = S_OK;
			break;
		}

		if ( FAILED( hr ) )
		{
    
		DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Error reading an AppPool row. Ignoring this row. Will continue with next row.\n"
				));
		
		// TODO: Should we be continuing here?
			goto exit;
		}

		hr = pISTAppPools->GetColumnValues (
											 cAppPoolColumns, 
											 (ULONG *)aiAppPoolColumns, 
											 NULL, 
											 apvAppPoolValues
										   );

		if ( FAILED( hr ) )
		{
    
		DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Error reading an AppPool row. Ignoring this row. Will continue with next row.\n"
				));
		
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;
		}


		wszAppPoolName										=	(WCHAR*) apvAppPoolValues[iDT_AppPoolID];
		AppPoolConfig.PeriodicProcessRestartPeriodInMinutes	=	*((ULONG *)(apvAppPoolValues[iDT_PeriodicRestartTime]));
		AppPoolConfig.PeriodicProcessRestartRequestCount	=	*((ULONG *)(apvAppPoolValues[iDT_PeriodicRestartRequests]));
		AppPoolConfig.MaxSteadyStateProcessCount			=	*((ULONG *)(apvAppPoolValues[iDT_MaxProcesses]));
		AppPoolConfig.PingingEnabled						=	*((ULONG *)(apvAppPoolValues[iDT_PingingEnabled]));
		AppPoolConfig.IdleTimeoutInMinutes					=	*((ULONG *)(apvAppPoolValues[iDT_IdleTimeout]));
		AppPoolConfig.RapidFailProtectionEnabled			=	*((ULONG *)(apvAppPoolValues[iDT_RapidFailProtection]));
        IF_DEBUG( WEB_ADMIN_SERVICE )
        {
            DBGPRINTF((
                DBG_CONTEXT, 
                "Reading from metabase app pool with ID: %S\n",
                wszAppPoolName
                ));
        }

		hr = GetWebAdminService()->GetUlAndWorkerManager()->CreateAppPool (wszAppPoolName, &AppPoolConfig);

	    if ( FAILED( hr ) )
		{

		DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Creating app pool failed. Ignoring this. Will contiue with next AppPool\n"
				));
			
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;

		}

        ValidAppPoolCount++;

	} // End For all AppPool rows

    if ( ValidAppPoolCount == 0 )
    {
    
        hr = HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

        DPERROR(( 
            DBG_CONTEXT,
            hr,
            "No valid app pools found in Cooked down metabase\n"
            ));
	
        goto exit;
    }


exit: 

	if(NULL != pISTAppPools)
		pISTAppPools->Release();

    return hr;

}   // CONFIG_MANAGER::ReadAllAppPools



/***************************************************************************++

Routine Description:

    Enumerate and read all virtual sites configured in the metabase.

Arguments:

    pMetabase - An instance of the MB class, opened to IIS_MD_W3SVC.

Return Value:

    HRESULT

--***************************************************************************/

HRESULT
CONFIG_MANAGER::ReadAllVirtualSites(
    )
{

	HRESULT							hr;
	ISimpleTableRead2				*pISTSites = NULL;
	ULONG							cBaseVersion;
	ULONG							i;
	DWORD							ValidVirtualSiteCount = 0;
	static const ULONG				aiSiteColumns []	= { 
														  iDT_VirtualSiteID, 
														  iDT_URLPrefixes	
														};
	static const ULONG				cSiteColumns		= sizeof (aiSiteColumns) / sizeof (ULONG);
	static const ULONG				cmaxSiteColumns		= iDT_URLPrefixes + 1;
	void*							apvSiteValues [cmaxSiteColumns];
	ULONG							acbSiteValues [cmaxSiteColumns];

	hr = GetTable (	
					didURTGLOBAL, 
					tidSITES, 
					0,
					(void**) &pISTSites
				  );

    if ( FAILED( hr ) )
    {

       DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Error getting Virtual Sites table\n"
            ));

        goto exit;
    }

/*	#ifdef _DEBUG

		hr = pISTSites->GetTableMeta (
									   &cBaseVersion, 
									   NULL, 
									   NULL, 
									   NULL
									 );

		if (cSITES_BASEVERSION != cBaseVersion)
		{
			hr = E_UNEXPECTED;
			goto exit;
		}

	#endif // _DEBUG
*/
	for (i = 0;; i++)
	{
		hr = pISTSites->MoveToRowByIndex (i);

		if (E_ST_NOMOREROWS == hr)
		{
			hr = S_OK;
			break;
		}

		if ( FAILED( hr ) )
		{
    
		DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Error reading an Virtual Site row. Ignoring this row. Will continue with next row.\n"
				));
		
		// TODO: Should we be continuing here?
			goto exit;
		}

		hr = pISTSites->GetColumnValues (
										  cSiteColumns, 
										  (ULONG *)aiSiteColumns, 
										  (ULONG *)acbSiteValues, 
										  apvSiteValues
										);

		if ( FAILED( hr ) )
		{
    
		DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Error reading an Virtual Site row. Ignoring this row. Will continue with next row.\n"
				));
		
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;
		}

		hr = GetWebAdminService()->GetUlAndWorkerManager()->CreateVirtualSite (
																			*((ULONG*)(apvSiteValues[iDT_VirtualSiteID])),
																			(BYTE*) (apvSiteValues[iDT_URLPrefixes]),
																			acbSiteValues [iDT_URLPrefixes]	
																		  );

		if ( FAILED( hr ) )
		{

			DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Creating virtual site failed\n"
				));
		
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;
		}

		ValidVirtualSiteCount++;

	}

    if ( ValidVirtualSiteCount == 0 )
    {
    
        hr = HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

        DPERROR(( 
            DBG_CONTEXT,
            hr,
            "No valid virtual sites found in Cooked Down metabase\n"
            ));
	
        goto exit;
    }

     IF_DEBUG( WEB_ADMIN_SERVICE )
    {
       DBGPRINTF((
            DBG_CONTEXT, 
            "Total number of valid virtual sites found: %lu\n",
            ValidVirtualSiteCount
            )); 
    } 


exit: 

	if(NULL != pISTSites)
		pISTSites->Release();

    return hr;

}   // CONFIG_MANAGER::ReadAllVirtualSites


/***************************************************************************++

Routine Description:

    Read and create a set of applications of a site.

Arguments:

    pMetabase - An instance of the MB class, opened to IIS_MD_W3SVC.

    VirtualSiteId - The id for the virtual site which contains these
    applications.

    pApplicationPaths - A MULTISZ containing the metabase paths to the 
    applications under a virtual site.

    pSiteRootPath - The metabase path fragment containing "/<SiteId>/Root".

    pValidRootApplicationExists - Returns TRUE if the root application (the 
    one at the URL "/" within the site) exists and is valid, FALSE otherwise.

Return Value:

    HRESULT

--***************************************************************************/

HRESULT
CONFIG_MANAGER::ReadApplications(
    OUT BOOL * pValidRootApplicationExists
    )
{

	HRESULT							hr;

	ISimpleTableRead2				*pISTApps;
	ULONG							cBaseVersion;
	ULONG							i;
	DWORD							ValidApplicationCount = 0;
	static const ULONG				aiAppColumns [] =   { 
														  iDT_APP_VirtualSiteID, 
														  iDT_APP_AppPoolID,
														  iDT_APP_AppURL	
														};
	static const ULONG				cAppColumns		= sizeof (aiAppColumns) / sizeof (ULONG);
	static const ULONG				cmaxAppColumns	= iDT_APP_AppURL + 1;
	void*							apvAppValues [cmaxAppColumns];

	hr = GetTable (	
					didURTGLOBAL, 
					tidAPPS, 
					0,
					(void**) &pISTApps	
				  );

    if ( FAILED( hr ) )
    {

        DPERROR(( 
            DBG_CONTEXT,
            hr,
            "Error getting Applications table\n"
            ));
	
        goto exit;
    }

/*	#ifdef _DEBUG
		hr = pISTApps->GetTableMeta (&cBaseVersion, NULL, NULL, NULL);
		if (cAPPS_BASEVERSION != cBaseVersion)
		{
			hr = E_UNEXPECTED;
			goto exit;
		}
	#endif // _DEBUG
*/
	for (i = 0;; i++)
	{
		hr = pISTApps->MoveToRowByIndex (i);

		if (E_ST_NOMOREROWS == hr)
		{
			hr = S_OK;
			break;
		}

		if ( FAILED( hr ) )
		{
    
			DPERROR(( 
			DBG_CONTEXT,
				hr,
				"Error reading an Applications row. Ignoring this row. Will continue with next row.\n"
				));
	
		// TODO: Should we be continuing here?
			goto exit;
		}

		hr = pISTApps->GetColumnValues (
										 cAppColumns, 
										 (ULONG *)aiAppColumns, 
										 NULL, 
										 apvAppValues
									   );

		if ( FAILED( hr ) )
		{
    
			DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Error reading an Applications row. Ignoring this row. Will continue with next row.\n"
				));
	
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;
		}

		hr = GetWebAdminService()->GetUlAndWorkerManager()->CreateApplication (
																			    *((DWORD *)(apvAppValues[iDT_APP_VirtualSiteID])),
																				(WCHAR*) apvAppValues[iDT_APP_AppPoolID],
																				(WCHAR*) apvAppValues[iDT_APP_AppURL]	
																			  );

		if ( FAILED( hr ) )
		{

			DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Creating application failed\n"
				));
	
		// TODO: Should we be continuing here?
			hr = S_OK;
			continue;
		}

		// TODO: Check if each application has a valid root site.
		// fail if it doesn't

		ValidApplicationCount++;
    

	}

    IF_DEBUG( WEB_ADMIN_SERVICE )
    {
        DBGPRINTF((
            DBG_CONTEXT, 
            "Number of valid apps (including root app if present) found for site: %lu was: %lu\n",
            *((DWORD *)(apvAppValues[iDT_APP_VirtualSiteID])),
            ValidApplicationCount
            ));  
    } 

exit:

	if(NULL != pISTApps)
		pISTApps->Release();

    return hr;

}   // CONFIG_MANAGER::ReadApplications


/***************************************************************************++

Routine Description:

    Helper function to obtain ISimpleTable* pointers

Arguments:

    None.

Return Value:

    HRESULT.

--***************************************************************************/
HRESULT CONFIG_MANAGER::GetTable(REFGUID	i_did,
						  REFGUID	i_tid,
						  DWORD		i_fServiceRequests,
						  LPVOID	*o_ppIST)
{

	HRESULT hr;
    STQueryCell		QueryCell[1];
	ULONG			cCell	= sizeof(QueryCell)/sizeof(STQueryCell);
	
	static LPWSTR wszFullCookedDownFileName	= NULL;

	if(NULL == m_pISTDisp)
	{
		hr = CoCreateInstance (CLSID_STDispenser,			
							   NULL,
							   CLSCTX_INPROC_SERVER, 
							   IID_ISimpleTableDispenser2,  
							   (void**)&m_pISTDisp);
		if(FAILED(hr) || (NULL == m_pISTDisp))
		{
			DPERROR(( 
				DBG_CONTEXT,
				hr,
				"Creating dispenser failed\n"
				)); 
			goto exit;
		}
	}

	if(NULL == wszFullCookedDownFileName)
	{
		LPCWSTR wszSubDir	= L"\\XSPDT\\";
		LPWSTR	wszCurPos	= NULL;
		UINT	cb			= GetWindowsDirectoryW(NULL, 0);

		if (cb == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto exit;
		}

		cb++;			// What if they dont include NULL
		cb += lstrlenW(wszSubDir);
		cb += lstrlenW(g_wszCookDownFile);

		wszFullCookedDownFileName = new WCHAR[cb];
		if(wszFullCookedDownFileName == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto exit;
		}

		cb = GetWindowsDirectoryW(wszFullCookedDownFileName, cb);

		if (cb == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			return E_OUTOFMEMORY;
		}
		
		cb++;			// What if they dont include NULL

		wszCurPos = wszFullCookedDownFileName;
		while (*wszCurPos != '\0')
		{
			wszCurPos = CharNextW(wszCurPos);
		}

		wszCurPos = CharPrevW(wszFullCookedDownFileName, wszCurPos);
		if (*wszCurPos == '\\')
		{
			*wszCurPos = '\0';	// If we had a back slash we remove it.
		}

		lstrcatW(wszFullCookedDownFileName, wszSubDir);
		lstrcatW(wszFullCookedDownFileName, g_wszCookDownFile);

	}

    QueryCell[0].pData     = (LPVOID)wszFullCookedDownFileName;
    QueryCell[0].eOperator = eST_OP_EQUAL;
    QueryCell[0].iCell     = iST_CELL_FILE;
    QueryCell[0].dbType    = DBTYPE_WSTR;
    QueryCell[0].cbSize    = (lstrlenW(wszFullCookedDownFileName)+1)*sizeof(WCHAR);

	hr = m_pISTDisp->GetTable(i_did, 
							  i_tid, 
							  (LPVOID)QueryCell, 
						      (LPVOID)&cCell, 
							  eST_QUERYFORMAT_CELLS, 
							  i_fServiceRequests,	
							  o_ppIST);
	if(FAILED(hr) || (NULL == m_pISTDisp))
	{
		DPERROR(( 
			DBG_CONTEXT,
			hr,
			"Getting table failed\n"
			));		
		goto exit; 
	}

exit:
	return hr;


} // CCooker::GetTable




