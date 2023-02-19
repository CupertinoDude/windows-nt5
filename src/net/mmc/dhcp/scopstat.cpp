/**********************************************************************/
/**                       Microsoft Windows/NT                       **/
/**                Copyright(c) Microsoft Corporation, 1997 - 1997 **/
/**********************************************************************/

/*
	ScopStat.cpp
		The scope statistics dialog
		
    FILE HISTORY:
        
*/

#include "stdafx.h"
#include "ScopStat.h"
#include "scope.h"
#include "server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum 
{
    SCOPE_STAT_TOTAL_ADDRESSES = 0,
    SCOPE_STAT_IN_USE,
    SCOPE_STAT_AVAILABLE,
    SCOPE_STAT_MAX
};

/*---------------------------------------------------------------------------
	CScopeStats implementation
 ---------------------------------------------------------------------------*/
const ContainerColumnInfo s_rgScopeStatsColumnInfo[] =
{
	{ IDS_STATS_TOTAL_ADDRESSES,		0,		TRUE },
	{ IDS_STATS_IN_USE,   		        0,		TRUE },
	{ IDS_STATS_AVAILABLE, 		        0,		TRUE },
};

CScopeStats::CScopeStats()
	: StatsDialog(STATSDLG_VERTICAL)
{
    SetColumnInfo(s_rgScopeStatsColumnInfo,
				  DimensionOf(s_rgScopeStatsColumnInfo));
}

CScopeStats::~CScopeStats()
{
}

BEGIN_MESSAGE_MAP(CScopeStats, StatsDialog)
	//{{AFX_MSG_MAP(CScopeStats)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_NEW_STATS_AVAILABLE, OnNewStatsAvailable)
END_MESSAGE_MAP()

HRESULT CScopeStats::RefreshData(BOOL fGrabNewData)
{
    if (fGrabNewData)
    {
	    DWORD dwError = 0;
	    LPDHCP_MIB_INFO pMibInfo = NULL;
	    
        BEGIN_WAIT_CURSOR;
        dwError = ::DhcpGetMibInfo(m_strServerAddress, &pMibInfo);
        END_WAIT_CURSOR;
    
        if (dwError != ERROR_SUCCESS)
	    {
		    ::DhcpMessageBox(dwError);
		    return hrOK;
	    }

        UpdateWindow(pMibInfo);

        if (pMibInfo)
		    ::DhcpRpcFreeMemory(pMibInfo);
    }

    return hrOK;
}

BOOL CScopeStats::OnInitDialog()
{
	CString	st, strScopeAddress;
    BOOL bRet;

    UtilCvtIpAddrToWstr(m_dhcpSubnetAddress, &strScopeAddress);
    AfxFormatString1(st, IDS_SCOPE_STATS_TITLE, strScopeAddress);

    SetWindowText((LPCTSTR) st);
	
    bRet = StatsDialog::OnInitDialog();

    // Set the default column widths to the width of the widest column
    SetColumnWidths(2 /* Number of Columns */);

    return bRet;
}

void CScopeStats::Sort(UINT nColumnId)
{
    // we don't sort any of our stats
}


afx_msg long CScopeStats::OnNewStatsAvailable(UINT wParam, LONG lParam)
{
    SPITFSNode    spNode;
    CDhcpScope *  pScope;
    CDhcpServer * pServer;

    pScope = GETHANDLER(CDhcpScope, m_spNode);
    pServer = pScope->GetServerObject();

    LPDHCP_MIB_INFO pMibInfo = pServer->DuplicateMibInfo();

    Assert(pMibInfo);
    if (!pMibInfo)
        return 0;

    UpdateWindow(pMibInfo);

    pServer->FreeDupMibInfo(pMibInfo);

    return 0;
}

void CScopeStats::UpdateWindow(LPDHCP_MIB_INFO pMibInfo)
{
	Assert (pMibInfo);

    UINT i;
    int nTotalAddresses = 0, nTotalInUse = 0, nTotalAvailable = 0;

    if (pMibInfo)
    {
        LPSCOPE_MIB_INFO pScopeMibInfo = pMibInfo->ScopeInfo;

	    // walk the list of scopes and calculate totals
	    for (i = 0; i < pMibInfo->Scopes; i++)
	    {
		    if (pScopeMibInfo[i].Subnet == m_dhcpSubnetAddress)
		    {
			    nTotalAddresses += (pScopeMibInfo[i].NumAddressesInuse + pScopeMibInfo[i].NumAddressesFree);
			    nTotalInUse = pScopeMibInfo[i].NumAddressesInuse;
			    nTotalAvailable = pScopeMibInfo[i].NumAddressesFree;

			    break;
		    }
	    }
    }

    int     nPercent;
	CString	st;
    TCHAR   szFormat[] = _T("%d");
    TCHAR   szPercentFormat[] =  _T("%d (%d%%)");

    for (i = 0; i < SCOPE_STAT_MAX; i++)
	{
        if (!pMibInfo)
            st = _T("---");
        else
        {
		    switch (i)
		    {
                case SCOPE_STAT_TOTAL_ADDRESSES:
            	    st.Format(szFormat, nTotalAddresses);
                    break;

                case SCOPE_STAT_IN_USE:
	                if (nTotalAddresses > 0)
		                nPercent = (int)(((LONGLONG)nTotalInUse * (LONGLONG)100) / nTotalAddresses);
	                else
		                nPercent = 0;

            	    st.Format(szPercentFormat, nTotalInUse, nPercent);
                    break;

                case SCOPE_STAT_AVAILABLE:
	                if (nTotalAddresses > 0)
		                nPercent = (int)(((LONGLONG)nTotalAvailable * (LONGLONG)100) / nTotalAddresses);
	                else
		                nPercent = 0;

            	    st.Format(szPercentFormat, nTotalAvailable, nPercent);
                    break;
            
    		    default:
				    Panic1("Unknown scope stat id : %d", i);
				    break;
		    }
        }
        
		m_listCtrl.SetItemText(i, 1, (LPCTSTR) st);
	}
}

