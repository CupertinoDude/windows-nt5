//////////////////////////////////////////////////////////////////////
//
// custconDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_CUSTCONDLG_H__106594D7_028D_11D2_8D1D_0000C06C2A54__INCLUDED_)
#define AFX_CUSTCONDLG_H__106594D7_028D_11D2_8D1D_0000C06C2A54__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCustconDlg dialog

class CCustconDlg : public CDialog
{
// �\�z
public:
    CCustconDlg(CWnd* pParent = NULL);  // �W���̃R���X�g���N�^

// Dialog Data
    //{{AFX_DATA(CCustconDlg)
    enum { IDD = IDD_CUSTCON_DIALOG };
    CEdit   m_wordDelimCtrl;
    //}}AFX_DATA

    // ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
    //{{AFX_VIRTUAL(CCustconDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
    HICON m_hIcon;

    // �������ꂽ���b�Z�[�W �}�b�v�֐�
    //{{AFX_MSG(CCustconDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnApply();
    afx_msg void OnDefaultValue();
    afx_msg void OnChangeWordDelim();
    afx_msg void OnUseExtendedEditKey();
    afx_msg void OnTrimLeadingZeros();
    afx_msg void OnReset();
    //}}AFX_MSG
    afx_msg void OnSelChange(UINT id);
    DECLARE_MESSAGE_MAP()

protected:
    void InitContents(BOOL isDefault);
    void CharInUse(UINT id, TCHAR c);
    void CharReturn(UINT id, TCHAR c);

    bool Update();

    int m_cWordDelimChanging;
    void EnableApply(BOOL fEnable = TRUE);

protected:
    CFont m_font;   // font for word delimiter edit control
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTCONDLG_H__106594D7_028D_11D2_8D1D_0000C06C2A54__INCLUDED_)
