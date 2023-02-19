//////////////////////////////////////////////////////////////////////
//
// custcon.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//
// 1998 Jun, Hiro Yamamoto
//

#include "stdafx.h"
#include "custcon.h"
#include "custconDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustconApp

BEGIN_MESSAGE_MAP(CCustconApp, CWinApp)
    //{{AFX_MSG_MAP(CCustconApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustconApp �N���X�̍\�z

CCustconApp::CCustconApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CCustconApp �I�u�W�F�N�g

CCustconApp theApp;

int gExMode;

/////////////////////////////////////////////////////////////////////////////
// CCustconApp �N���X�̏�����

inline bool strequ(LPCTSTR a, LPCTSTR b)
{
    return !_tcscmp(a, b);
}

BOOL CCustconApp::InitInstance()
{
#ifdef _AFXDLL
    Enable3dControls();         // ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
    Enable3dControlsStatic();   // MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

    //
    // Parse command line
    //

    if (strequ(m_lpCmdLine, _T("-e"))) {
        gExMode = 1;
    }

    CCustconDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
    return FALSE;
}
