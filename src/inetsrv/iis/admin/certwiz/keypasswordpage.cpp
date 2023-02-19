// KeyPasswordPage.cpp : implementation file
//

#include "stdafx.h"
#include "CertWiz.h"
#include "KeyPasswordPage.h"
#include "Certificat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyPasswordPage property page

IMPLEMENT_DYNCREATE(CKeyPasswordPage, CIISWizardPage)

CKeyPasswordPage::CKeyPasswordPage(CCertificate * pCert) 
	: CIISWizardPage(CKeyPasswordPage::IDD, IDS_CERTWIZ, TRUE),
	m_pCert(pCert)
{
	//{{AFX_DATA_INIT(CKeyPasswordPage)
	m_Password = _T("");
	//}}AFX_DATA_INIT
}

CKeyPasswordPage::~CKeyPasswordPage()
{
}

void CKeyPasswordPage::DoDataExchange(CDataExchange* pDX)
{
	CIISWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyPasswordPage)
	DDX_Text(pDX, IDC_KEYPASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 64);
	//}}AFX_DATA_MAP
}

LRESULT 
CKeyPasswordPage::OnWizardBack()
/*++
Routine Description:
    Prev button handler

Arguments:
    None

Return Value:
	0 to automatically advance to the prev page;
	1 to prevent the page from changing. 
	To jump to a page other than the prev one, 
	return the identifier of the dialog to be displayed.
--*/
{
	return IDD_PAGE_PREV;
}

LRESULT 
CKeyPasswordPage::OnWizardNext()
{
	UpdateData(TRUE);
	if (0 != m_Password.Compare(m_pCert->m_KeyPassword))
	{
		m_pCert->DeleteKeyRingCert();
		m_pCert->m_KeyPassword = m_Password;
	}

	if (NULL == m_pCert->GetKeyRingCert())
	{
		// probably password was wrong
		CString txt;
		txt.LoadString(IDS_FAILED_IMPORT_KEY_FILE);
		ASSERT(GetDlgItem(IDC_ERROR_TEXT) != NULL);
		SetDlgItemText(IDC_ERROR_TEXT, txt);
		GetDlgItem(IDC_KEYPASSWORD)->SetFocus();
		GetDlgItem(IDC_KEYPASSWORD)->SendMessage(EM_SETSEL, 0, -1);
		SetWizardButtons(PSWIZB_BACK);
		return 1;
	}
	return IDD_PAGE_NEXT;
}

BOOL 
CKeyPasswordPage::OnSetActive() 
{
	ASSERT(m_pCert != NULL);
	m_Password = m_pCert->m_KeyPassword;
	UpdateData(FALSE);
	SetWizardButtons(m_Password.IsEmpty() || m_pCert->GetKeyRingCert() == NULL ?
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	return CIISWizardPage::OnSetActive();
}

BOOL
CKeyPasswordPage::OnKillActive()
{
	UpdateData();
	m_pCert->m_KeyPassword = m_Password;
	return CIISWizardPage::OnKillActive();
}

BEGIN_MESSAGE_MAP(CKeyPasswordPage, CIISWizardPage)
	//{{AFX_MSG_MAP(CKeyPasswordPage)
	ON_EN_CHANGE(IDC_KEYPASSWORD, OnEditchangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSiteNamePage message handlers

void CKeyPasswordPage::OnEditchangePassword() 
{
	UpdateData(TRUE);	
	SetWizardButtons(m_Password.IsEmpty() ? 
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	CString str;
	SetDlgItemText(IDC_ERROR_TEXT, str);
}




/////////////////////////////////////////////////////////////////////////////
// CKeyPasswordPage property page

IMPLEMENT_DYNCREATE(CImportPFXPasswordPage, CIISWizardPage)

CImportPFXPasswordPage::CImportPFXPasswordPage(CCertificate * pCert) 
	: CIISWizardPage(CImportPFXPasswordPage::IDD, IDS_CERTWIZ, TRUE),
	m_pCert(pCert)
{
	//{{AFX_DATA_INIT(CImportPFXPasswordPage)
	m_Password = _T("");
	//}}AFX_DATA_INIT
}

CImportPFXPasswordPage::~CImportPFXPasswordPage()
{
}

void CImportPFXPasswordPage::DoDataExchange(CDataExchange* pDX)
{
	CIISWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportPFXPasswordPage)
	DDX_Text(pDX, IDC_KEYPASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 64);
	//}}AFX_DATA_MAP
}

LRESULT 
CImportPFXPasswordPage::OnWizardBack()
/*++
Routine Description:
    Prev button handler

Arguments:
    None

Return Value:
	0 to automatically advance to the prev page;
	1 to prevent the page from changing. 
	To jump to a page other than the prev one, 
	return the identifier of the dialog to be displayed.
--*/
{
	return IDD_PAGE_PREV;
}

LRESULT 
CImportPFXPasswordPage::OnWizardNext()
{
	UpdateData(TRUE);
	if (0 != m_Password.Compare(m_pCert->m_KeyPassword))
	{
		m_pCert->DeleteKeyRingCert();
		m_pCert->m_KeyPassword = m_Password;
	}
	if (NULL == m_pCert->GetPFXFileCert())
	{
		// probably password was wrong
		CString txt;
		txt.LoadString(IDS_FAILED_IMPORT_PFX_FILE);
		ASSERT(GetDlgItem(IDC_ERROR_TEXT) != NULL);
		SetDlgItemText(IDC_ERROR_TEXT, txt);
		GetDlgItem(IDC_KEYPASSWORD)->SetFocus();
		GetDlgItem(IDC_KEYPASSWORD)->SendMessage(EM_SETSEL, 0, -1);
		SetWizardButtons(PSWIZB_BACK);
		return 1;
	}
	return IDD_PAGE_NEXT;
}

BOOL 
CImportPFXPasswordPage::OnSetActive() 
{
	ASSERT(m_pCert != NULL);
	m_Password = m_pCert->m_KeyPassword;
	UpdateData(FALSE);
	SetWizardButtons(m_Password.IsEmpty() || m_pCert->GetKeyRingCert() == NULL ?
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	return CIISWizardPage::OnSetActive();
}

BOOL
CImportPFXPasswordPage::OnKillActive()
{
	UpdateData();
	m_pCert->m_KeyPassword = m_Password;
	return CIISWizardPage::OnKillActive();
}

BEGIN_MESSAGE_MAP(CImportPFXPasswordPage, CIISWizardPage)
	//{{AFX_MSG_MAP(CImportPFXPasswordPage)
	ON_EN_CHANGE(IDC_KEYPASSWORD, OnEditchangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSiteNamePage message handlers

void CImportPFXPasswordPage::OnEditchangePassword() 
{
	UpdateData(TRUE);	
	SetWizardButtons(m_Password.IsEmpty() ? 
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	CString str;
	SetDlgItemText(IDC_ERROR_TEXT, str);
}







/////////////////////////////////////////////////////////////////////////////
// CExportPFXPasswordPage property page

IMPLEMENT_DYNCREATE(CExportPFXPasswordPage, CIISWizardPage)

CExportPFXPasswordPage::CExportPFXPasswordPage(CCertificate * pCert) 
	: CIISWizardPage(CExportPFXPasswordPage::IDD, IDS_CERTWIZ, TRUE),
	m_pCert(pCert)
{
	//{{AFX_DATA_INIT(CExportPFXPasswordPage)
	m_Password = _T("");
    m_Export_Private_key = FALSE;
	//}}AFX_DATA_INIT
}

CExportPFXPasswordPage::~CExportPFXPasswordPage()
{
}

void CExportPFXPasswordPage::DoDataExchange(CDataExchange* pDX)
{
	CIISWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportPFXPasswordPage)
	DDX_Text(pDX, IDC_KEYPASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 64);
    DDX_Check(pDX, IDC_CHK_EXPORT_PRIVATE, m_Export_Private_key);
	//}}AFX_DATA_MAP
}

LRESULT 
CExportPFXPasswordPage::OnWizardBack()
/*++
Routine Description:
    Prev button handler

Arguments:
    None

Return Value:
	0 to automatically advance to the prev page;
	1 to prevent the page from changing. 
	To jump to a page other than the prev one, 
	return the identifier of the dialog to be displayed.
--*/
{
	return IDD_PAGE_PREV;
}

LRESULT 
CExportPFXPasswordPage::OnWizardNext()
{
	UpdateData(TRUE);
	if (0 != m_Password.Compare(m_pCert->m_KeyPassword))
	{
		m_pCert->DeleteKeyRingCert();
		m_pCert->m_KeyPassword = m_Password;
        m_pCert->m_ExportPFXPrivateKey = m_Export_Private_key;
        m_pCert->m_ExportPFXPrivateKey = TRUE;
	}
    /*
	if (NULL == m_pCert->GetKeyRingCert())
	{
		// probably password was wrong
		CString txt;
		txt.LoadString(IDS_FAILED_IMPORT_KEY_FILE);
		ASSERT(GetDlgItem(IDC_ERROR_TEXT) != NULL);
		SetDlgItemText(IDC_ERROR_TEXT, txt);
		GetDlgItem(IDC_KEYPASSWORD)->SetFocus();
		GetDlgItem(IDC_KEYPASSWORD)->SendMessage(EM_SETSEL, 0, -1);
		SetWizardButtons(PSWIZB_BACK);
		return 1;
	}
    */
	return IDD_PAGE_NEXT;
}

BOOL 
CExportPFXPasswordPage::OnSetActive() 
{
	ASSERT(m_pCert != NULL);
	m_Password = m_pCert->m_KeyPassword;
	UpdateData(FALSE);
	SetWizardButtons(m_Password.IsEmpty() || m_pCert->GetKeyRingCert() == NULL ?
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	return CIISWizardPage::OnSetActive();
}

BOOL
CExportPFXPasswordPage::OnKillActive()
{
	UpdateData();
	m_pCert->m_KeyPassword = m_Password;
	return CIISWizardPage::OnKillActive();
}

BEGIN_MESSAGE_MAP(CExportPFXPasswordPage, CIISWizardPage)
	//{{AFX_MSG_MAP(CExportPFXPasswordPage)
	ON_EN_CHANGE(IDC_KEYPASSWORD, OnEditchangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportPFXPasswordPage::OnEditchangePassword() 
{
	UpdateData(TRUE);	
	SetWizardButtons(m_Password.IsEmpty() ? 
			PSWIZB_BACK : PSWIZB_BACK | PSWIZB_NEXT);
	CString str;
	SetDlgItemText(IDC_ERROR_TEXT, str);
}
