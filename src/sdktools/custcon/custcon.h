//////////////////////////////////////////////////////////////////////
//
// custcon.h : CUSTCON �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_CUSTCON_H__106594D5_028D_11D2_8D1D_0000C06C2A54__INCLUDED_)
#define AFX_CUSTCON_H__106594D5_028D_11D2_8D1D_0000C06C2A54__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CCustconApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� custcon.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CCustconApp : public CWinApp
{
public:
    CCustconApp();

// �I�[�o�[���C�h
    // ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
    //{{AFX_VIRTUAL(CCustconApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// �C���v�������e�[�V����

    //{{AFX_MSG(CCustconApp)
        // ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
        //        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

extern int gExMode;     // default mode

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTCON_H__106594D5_028D_11D2_8D1D_0000C06C2A54__INCLUDED_)
