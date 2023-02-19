/*******************************************************************************
*
* busssrvvw.h
*
* declarations for the CBusyServerView class
*
* copyright notice: Copyright 1997, Citrix Systems Inc.
* Copyright (c) 1998 - 1999 Microsoft Corporation
*
* $Author:   butchd  $  Don Messerli
*
* $Log:   M:\NT\PRIVATE\UTILS\CITRIX\WINUTILS\WINADMIN\VCS\BUSSRVVW.H  $
*  
*     Rev 1.0   30 Jul 1997 17:11:16   butchd
*  Initial revision.
*
*******************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CBusyServerView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBusyServerView : public CFormView
{
friend class CRightPane;

protected:
	CBusyServerView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBusyServerView)

// Form Data
public:
	//{{AFX_DATA(CBusyServerView)
	enum { IDD = IDD_BUSY_SERVER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBusyServerView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBusyServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBusyServerView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
