
#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <shlwapi.h>

#include <shellapi.h>
#include <srconfig.h>
#include <snapshot.h>
#include <stdio.h>
#include <dbgtrace.h>
#include <utils.h>
#include <srdefs.h>
#include <respoint.h>
#include <srapi.h>
#include <evthandler.h>
#include <ntservice.h>
#include <datastormgr.h>

CEventHandler * g_pEventHandler = NULL;
CSRConfig * g_pSRConfig = NULL;
CNTService * g_pSRService = NULL;
CDataStoreMgr * g_pDataStoreMgr = NULL;

DWORD CEventHandler::SRUpdateMonitoredListS (LPWSTR pszXMLFile)
{
    return 0;
}

void CEventHandler::RefreshCurrentRp (BOOL fScanAllDrives)
{
}

void CNTService::LogEvent(WORD wType, DWORD dwID,
                  void * pRawData,
                  DWORD dwDataSize,
                  const WCHAR* pszS1,
                  const WCHAR* pszS2,
                  const WCHAR* pszS3)
{
}

DWORD CDriveTable::ForAllDrives (PDATASTOREMETHOD pMethod, LONG_PTR lParam)
{
    return 0;
}

DWORD CDataStore::SwitchRestorePoint(LONG_PTR pRestorePoint)
{
    return 0;
}

DWORD CDataStoreMgr::TriggerFreezeOrFifo() 
{
    return 0;
}

