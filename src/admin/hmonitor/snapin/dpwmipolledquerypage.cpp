// DPWmiPolledQueryPage.cpp : implementation file
//
// 03/05/00 v-marfin bug 59643 : Make this the default starting page.
// 03/29/00 v-marfin bug 62585 : Set new Data collector's ENABLED to TRUE if user presses OK.
// 03/30/00 v-marfin bug 59237 : If user does not change the default name of the data
//                               collector when they first create it, change it for 
//                               them to a more meaningful name based on the data
//                               they select in the property pages.


#include "stdafx.h"
#include "snapin.h"
#include "DPWmiPolledQueryPage.h"
#include "HMObject.h"
#include "WmiBrowseDlg.h"
#include "WmiPropertyBrowseDlg.h"
#include "DataElement.h"
#include "DataGroupScopeItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPWmiPolledQueryPage property page

IMPLEMENT_DYNCREATE(CDPWmiPolledQueryPage, CHMPropertyPage)

CDPWmiPolledQueryPage::CDPWmiPolledQueryPage() : CHMPropertyPage(CDPWmiPolledQueryPage::IDD)
{
	//{{AFX_DATA_INIT(CDPWmiPolledQueryPage)
	m_sClass = _T("");
	m_sNamespace = _T("");
	m_sQuery = _T("");
	m_bRequireReset = FALSE;
	//}}AFX_DATA_INIT
	
	m_sHelpTopic = _T("HMon21.chm::/dDEquery.htm");
}

CDPWmiPolledQueryPage::~CDPWmiPolledQueryPage()
{
}

inline void CDPWmiPolledQueryPage::ConstructQuery()
{
	m_sQuery.Format(_T("SELECT * FROM %s"), m_sClass.IsEmpty() ? _T("<<your_class_name_here>>") : m_sClass);

	GetDlgItem(IDC_EDIT_QUERY)->SetWindowText(m_sQuery);
}

void CDPWmiPolledQueryPage::UpdateProperties()
{
	m_Properties.DeleteAllItems();

	if( m_sClass.IsEmpty() )
	{
		return;
	}

	CWbemClassObject classobject;

	classobject.SetNamespace(_T("\\\\") + GetObjectPtr()->GetSystemName() + _T("\\") + m_sNamespace);

	HRESULT hr = classobject.GetObject(m_sClass);

	if( hr != S_OK )
	{
		return;
	}

	CStringArray saNames;
	classobject.GetPropertyNames(saNames);
	classobject.Destroy();

	for( int i = 0; i < saNames.GetSize(); i++ )
	{
		m_Properties.InsertItem(0,saNames[i]);
	}

	m_Properties.SetColumnWidth(0,LVSCW_AUTOSIZE);
}

void CDPWmiPolledQueryPage::DoDataExchange(CDataExchange* pDX)
{
	CHMPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPWmiPolledQueryPage)
	DDX_Control(pDX, IDC_LIST_PROPERTIES, m_Properties);
	DDX_Text(pDX, IDC_EDIT_CLASS, m_sClass);
	DDX_Text(pDX, IDC_EDIT_NAMESPACE, m_sNamespace);
	DDX_Text(pDX, IDC_EDIT_QUERY, m_sQuery);
	DDX_Check(pDX, IDC_CHECK_REQUIRE_RESET, m_bRequireReset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDPWmiPolledQueryPage, CHMPropertyPage)
	//{{AFX_MSG_MAP(CDPWmiPolledQueryPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_NAMESPACE, OnButtonBrowseNamespace)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_CLASS, OnButtonBrowseClass)
	ON_EN_CHANGE(IDC_EDIT_CLASS, OnChangeEditClass)
	ON_EN_CHANGE(IDC_EDIT_NAMESPACE, OnChangeEditNamespace)
	ON_EN_CHANGE(IDC_EDIT_QUERY, OnChangeEditQuery)
	ON_BN_CLICKED(IDC_CHECK_REQUIRE_RESET, OnCheckRequireReset)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROPERTIES, OnClickListProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPWmiPolledQueryPage message handlers

BOOL CDPWmiPolledQueryPage::OnInitDialog() 
{
	// v-marfin : bug 59643 : This will be the default starting page for the property
	//                        sheet so call CnxPropertyPageCreate() to unmarshal the 
	//                        connection for this thread. This function must be called
	//                        by the first page of the property sheet. It used to 
	//                        be called by the "General" page and its call still remains
	//                        there as well in case the general page is loaded by a 
	//                        different code path that does not also load this page.
	//                        The CnxPropertyPageCreate function has been safeguarded
	//                        to simply return if the required call has already been made.
	//                        CnxPropertyPageDestory() must be called from this page's
	//                        OnDestroy function.
	// unmarshal connmgr
	CnxPropertyPageCreate();

	CHMPropertyPage::OnInitDialog();

	// initialize the list view
	m_Properties.SetExtendedStyle(LVS_EX_CHECKBOXES);


	CString sColumnTitle;
	sColumnTitle.LoadString(IDS_STRING_NAME);
	m_Properties.InsertColumn(0,sColumnTitle);
	
	CWbemClassObject* pClassObject = GetObjectPtr()->GetClassObject();	
	if( ! GfxCheckObjPtr(pClassObject,CWbemClassObject) )
	{
		return TRUE;
	}

    //-------------------------------------------------------------------------
    // v-marfin 59237 : Store original name in case this data collector is
    //                  just being created. When they save, we will modify the 
    //                  name if they haven't.
	pClassObject->GetProperty(IDS_STRING_MOF_NAME,m_sOriginalName);
    //-------------------------------------------------------------------------

	pClassObject->GetProperty(IDS_STRING_MOF_TARGETNAMESPACE,m_sNamespace);

	bool bReset;
	pClassObject->GetProperty(IDS_STRING_MOF_REQUIRERESET,bReset);
	m_bRequireReset = bReset;

	pClassObject->GetProperty(IDS_STRING_MOF_QUERY,m_sQuery);
	m_sQuery.MakeUpper();

	int iIndex = m_sQuery.Find(_T("SELECT * FROM "));
	if( iIndex != -1 )
	{
		m_sClass = m_sQuery.Right(m_sQuery.GetLength()-iIndex-14);
		iIndex = m_sClass.Find(_T(" "));
		if( iIndex != -1 )
		{
			m_sClass = m_sClass.Left(iIndex);
		}
	}

	UpdateProperties();

	CStringArray saProperties;
	pClassObject->GetProperty(IDS_STRING_MOF_STATISTICSPROPERTYNAMES,saProperties);
	for( int i=0; i < saProperties.GetSize(); i++ )
	{
		LVFINDINFO lvfi;
		ZeroMemory(&lvfi,sizeof(LVFINDINFO));
		lvfi.flags = LVFI_WRAP|LVFI_STRING;
		lvfi.psz = saProperties[i];
		int iListIndex = m_Properties.FindItem(&lvfi);
		if( iListIndex >= 0 )
		{
			m_Properties.SetCheck(iListIndex);
		}
	}

	delete pClassObject;

	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDPWmiPolledQueryPage::OnDestroy() 
{
    // v-marfin 62585 : For this new data collector, set its Enabled property to TRUE, but
    //                  only if the user is not cancelling these property pages.
    if (m_bOnApplyUsed)
    {
        ClearStatistics(); // 62548

        CDataElement* pElement = (CDataElement*)GetObjectPtr();
        if (pElement && pElement->IsStateSetToEnabledOnOK())
        {
            TRACE(_T("CDPWmiPolledQueryPage::OnDestroy - New Perfmon Collector: Setting to Enabled\n"));

            pElement->SetStateToEnabledOnOK(FALSE); // don't do this again
	        
	        CWbemClassObject* pClassObject = GetObjectPtr()->GetClassObject();

	        if( ! GfxCheckObjPtr(pClassObject,CWbemClassObject) )
	        {
                TRACE(_T("ERROR: CDPWmiPolledQueryPage::OnDestroy - Failed to GetClassObject()\n"));
		        return;
	        }

            // Set the new collector to enabled.
            BOOL bEnabled=TRUE;
	        HRESULT hr = pClassObject->GetProperty(IDS_STRING_MOF_ENABLE,bEnabled);
	        hr = pClassObject->SetProperty(IDS_STRING_MOF_ENABLE,TRUE);
            
            if (!CHECKHRESULT(hr))
            {   
                TRACE(_T("ERROR: CDPWmiPolledQueryPage::OnDestroy - Failed to set ENABLED property on new collector\n"));
            }
            

            //-------------------------------------------------------------------
            // v-marfin 59237 : If the user has not changed the original default
            //                  name, do so for them. Compare against original
            //                  name we fetched during OnInitDialog.
            CString sName;
	        pClassObject->GetProperty(IDS_STRING_MOF_NAME,sName);

            // Did the user change the default name?
            if (m_sOriginalName.CompareNoCase(sName)==0)
            {       
                // No, so set the new name 
                if (!m_sQuery.IsEmpty())
                {
                    // Use parent to ensure name is unique
                    //CDataGroup* pParent = (CDataGroup*) pElement->GetCollectorsParentClassObject();
	                if(pElement->GetScopeItemCount())
	                {
		                CDataElementScopeItem* pItem = (CDataElementScopeItem*)pElement->GetScopeItem(0);
		                if( pItem )
                        {
                            CDataGroupScopeItem* pDGItem = (CDataGroupScopeItem*)pItem->GetParent();
                            sName = pDGItem->GetUniqueDisplayName(m_sQuery);
                        }
                    }

                    // Set the local element's object data
                    pElement->SetName(sName);
                    // Set its WMI property
                    pClassObject->SetProperty(IDS_STRING_MOF_NAME,sName);

                    // Refresh to show the new name in the IU
                    //pElement->Refresh();      // 63005
                    pElement->UpdateStatus();   // 63005
                }
            }
            //-------------------------------------------------------------------


            pClassObject->SaveAllProperties();
            delete pClassObject;

        } // if (pElement && pElement->IsStateSetToEnabledOnOK())
    } // if (m_bOnApplyUsed)


	CHMPropertyPage::OnDestroy();
	
	// v-marfin : bug 59643 : CnxPropertyPageDestory() must be called from this page's
	//                        OnDestroy function.
	CnxPropertyPageDestroy();	
	
}

void CDPWmiPolledQueryPage::OnOK() 
{
	CHMPropertyPage::OnOK();
}

void CDPWmiPolledQueryPage::OnButtonBrowseNamespace() 
{
	CWmiNamespaceBrowseDlg dlg;

	// set the dialog window title
	CString sItem;
	sItem.LoadString(IDS_STRING_NAMESPACES);
	dlg.m_sDlgTitle.Format(IDS_STRING_WMI_BROWSE_TITLE,sItem,GetObjectPtr()->GetSystemName());

	// create the namespace enumerator
	if( ! CHECKHRESULT(dlg.m_ClassObject.Create(GetObjectPtr()->GetSystemName())) )
	{
		return;
	}

	if( m_sNamespace.IsEmpty() )
	{
		m_sNamespace.Format(IDS_STRING_MOF_NAMESPACE_FORMAT,GetObjectPtr()->GetSystemName());
	}

	dlg.m_sTitle.Format(_T("\\\\%s\\%s"),GetObjectPtr()->GetSystemName(),m_sNamespace);
	dlg.m_ClassObject.SetNamespace(dlg.m_sTitle);

	// set the listbox title
	dlg.m_sTitle = m_sNamespace;

	// display dialog
	if( dlg.DoModal() == IDOK )
	{
		m_sNamespace = dlg.m_sSelectedItem;
		m_sClass.Empty();
		UpdateData(FALSE);
		UpdateProperties();
		SetModified();
		ConstructQuery();
	}
}

void CDPWmiPolledQueryPage::OnButtonBrowseClass() 
{
	CWmiClassBrowseDlg dlg;	

	// set the dialog window title
	CString sItem;
	sItem.LoadString(IDS_STRING_CLASSES);
	dlg.m_sDlgTitle.Format(IDS_STRING_WMI_BROWSE_TITLE,sItem,GetObjectPtr()->GetSystemName());

	// create the class enumerator
	if( ! CHECKHRESULT(dlg.m_ClassObject.Create(GetObjectPtr()->GetSystemName())) )
	{
		return;
	}

	CString sTemp;
	sTemp.Format(_T("\\\\%s\\%s"),GetObjectPtr()->GetSystemName(),m_sNamespace);

	dlg.m_ClassObject.SetNamespace(sTemp);

	if( ! CHECKHRESULT(dlg.m_ClassObject.CreateClassEnumerator(NULL)) )
	{
		return;
	}

	// set the listbox title
	dlg.m_sTitle = m_sNamespace;

	// display the dialog	
	if( dlg.DoModal() == IDOK )
	{
		m_sClass = dlg.m_sSelectedItem;		
		UpdateData(FALSE);
		UpdateProperties();
		SetModified();
		ConstructQuery();
	}
}

void CDPWmiPolledQueryPage::OnChangeEditClass() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CHMPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	UpdateProperties();
	SetModified();
	ConstructQuery();
}

void CDPWmiPolledQueryPage::OnChangeEditNamespace() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CHMPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

  if( m_sNamespace.Find(_T("\\\\")) != -1 )
  {
    AfxMessageBox(IDS_STRING_NO_REMOTE_NAMESPACES);
    GetDlgItem(IDC_EDIT_NAMESPACE)->SetWindowText(_T(""));
  }
	
	GetDlgItem(IDC_EDIT_CLASS)->SetWindowText(_T(""));	
	
	UpdateData();
	UpdateProperties();
	SetModified();
	ConstructQuery();
}

void CDPWmiPolledQueryPage::OnChangeEditQuery() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CHMPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();	
	SetModified();
}

BOOL CDPWmiPolledQueryPage::OnApply() 
{
    // v-marfin 62585 : So we can set the collector's state to enabled when OK pressed.
    m_bOnApplyUsed=TRUE;

	if( ! CHMPropertyPage::OnApply() )
	{
		return FALSE;
	}

	UpdateData();

	CStringArray saProperties;

	for( int i = 0; i < m_Properties.GetItemCount(); i++ )
	{
		if( m_Properties.GetCheck(i) )
		{			
			saProperties.Add(m_Properties.GetItemText(i,0));
		}
	}

//	if( ! saProperties.GetSize() )
//	{
//		return FALSE;
//	}

	CWbemClassObject* pClassObject = GetObjectPtr()->GetClassObject();	
	if( ! GfxCheckObjPtr(pClassObject,CWbemClassObject) )
	{
		return FALSE;
	}

	pClassObject->SetProperty(IDS_STRING_MOF_TARGETNAMESPACE,m_sNamespace);	

	bool bReset = m_bRequireReset ? true : false;
	pClassObject->SetProperty(IDS_STRING_MOF_REQUIRERESET,bReset);

	pClassObject->SetProperty(IDS_STRING_MOF_STATISTICSPROPERTYNAMES,saProperties);
	
	pClassObject->SetProperty(IDS_STRING_MOF_QUERY,m_sQuery);

	pClassObject->SaveAllProperties();

	delete pClassObject;

  SetModified(FALSE);
	
	return TRUE;
}

void CDPWmiPolledQueryPage::OnCheckRequireReset() 
{
	UpdateData();
	SetModified();	
}

void CDPWmiPolledQueryPage::OnClickListProperties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetModified();	
	*pResult = 0;
}
