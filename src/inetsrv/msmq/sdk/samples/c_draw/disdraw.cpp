// disdraw.cpp : Defines the class behaviors for the application.
//
//
//=--------------------------------------------------------------------------=
// Copyright  1997-1999  Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//=--------------------------------------------------------------------------=


#include "stdafx.h"
#include "disdraw.h"
#include "drawdlg.h"
#include "logindlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_USERNAME_LEN 32

/////////////////////////////////////////////////////////////////////////////
// CDisdrawApp

BEGIN_MESSAGE_MAP(CDisdrawApp, CWinApp)
	//{{AFX_MSG_MAP(CDisdrawApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisdrawApp construction

CDisdrawApp::CDisdrawApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDisdrawApp object

CDisdrawApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDisdrawApp initialization

BOOL CDisdrawApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//
	// Prompt for the user's name
	//
	char mbsUserName[MAX_USERNAME_LEN];
	DWORD dwNumBytes = MAX_USERNAME_LEN;
	GetUserName(mbsUserName, &dwNumBytes);
	CLoginDlg dlgLogin;
	dlgLogin.m_strLogin = mbsUserName;
	if (dlgLogin.DoModal() == IDCANCEL || dlgLogin.m_strLogin == "" )
	{
		return FALSE;
	}

	dlgLogin.m_strLogin.MakeUpper();

	CDisdrawDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_strLogin = dlgLogin.m_strLogin;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
