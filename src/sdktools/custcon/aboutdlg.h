// AboutDlg.h

#ifndef _ABOUTDLG_H
#define _ABOUTDLG_H

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// �_�C�A���O �f�[�^
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
    //}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
    //{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif
