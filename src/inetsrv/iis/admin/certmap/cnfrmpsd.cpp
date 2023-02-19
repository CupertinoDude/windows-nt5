// CnfrmPsD.cpp : implementation file
//
// NOTE that file Passdlg.h/cpp is very similar to this
//      file!
//
//       CnfrmPsD class has an OnOK that will complain
//       to the user if the passwds dont match
//       This is above whats in PassDlg class
//
//       And class PassDlg has an OnInitDialog that Cnfrmpsd
//       does not have. This simply puts focus on the edit
//       field for the passwd
//
#include "stdafx.h"
#include "certmap.h"
#include "CnfrmPsD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmPassDlg dialog

CConfirmPassDlg::CConfirmPassDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CConfirmPassDlg::IDD, pParent)
    {
    //{{AFX_DATA_INIT(CConfirmPassDlg)
    m_sz_password_new = _T("");
    //}}AFX_DATA_INIT
    }

void CConfirmPassDlg::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CConfirmPassDlg)
    DDX_Text(pDX, IDC_CONFIRM_PASSWORD, m_sz_password_new);
    //}}AFX_DATA_MAP
    }

BEGIN_MESSAGE_MAP(CConfirmPassDlg, CDialog)
    //{{AFX_MSG_MAP(CConfirmPassDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmPassDlg message handlers

void CConfirmPassDlg::OnOK() 
    {
    UpdateData( TRUE );

    // confirm it
    if ( m_sz_password_new != m_szOrigPass )
        {
        AfxMessageBox( IDS_PASS_CONFIRM_FAIL );
        return;
        }

    CDialog::OnOK();
    }
