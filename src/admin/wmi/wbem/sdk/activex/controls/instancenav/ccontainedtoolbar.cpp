// ***************************************************************************

//

// Copyright (c) 1996-2001 Microsoft Corporation, All Rights Reserved 
//
// File: CContainedToolBar.h
//
// Description:
//	This file implements the CContainedToolBar class which is a part of the Class
//	Navigator OCX, it is a subclass of the Microsoft CToolBar
//  control and performs the following functions:
//		a.  Provides a member function to calculate its size
//		b.  Provides a tooltip.
//
// Part of:
//	Navigator.ocx
//
// Used by:
//	 CBanner
//
// History:
//	Judith Ann Powell	10-08-96		Created.
//
//
//**************************************************************************
#include "precomp.h"
#include "afxpriv.h"
#include "wbemidl.h"
#include "resource.h"
#include "AFXCONV.H"
#include "CInstanceTree.h"
#include "CContainedToolBar.h"
#include "Banner.h"
#include "InstanceSearch.h"
#include "NavigatorCtl.h"
#include "OLEMSCLient.h"

BEGIN_MESSAGE_MAP(CContainedToolBar,CToolBar)
	//{{AFX_MSG_MAP(CContainedToolBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CContainedToolBar::OnSize(UINT nType, int cx, int cy)
{
	CToolBar::OnSize(nType, cx, cy);
	if (m_pParent)
	{
		//m_pParent -> m_bBannerOnly = TRUE;
		m_pParent -> InvalidateControl();
	}

}

// ***************************************************************************
//
// CContainedToolBar::GetToolBarSize
//
// Description:
//	  This member function returns the toolbar's size..
//
// Parameters:
//	  VOID
//
// Returns:
// 	  CSize						Toolbar size.
//
// Globals accessed:
//	  NONE
//
// Globals modified:
//	  NONE
//
// ***************************************************************************
CSize CContainedToolBar::GetToolBarSize()
{
	CRect rcButtons;
	CToolBarCtrl &rToolBarCtrl = GetToolBarCtrl();
	int nButtons = rToolBarCtrl.GetButtonCount();
	if (nButtons > 0) {
		CRect rcLastButton;
		rToolBarCtrl.GetItemRect(0, &rcButtons);
		rToolBarCtrl.GetItemRect(nButtons-1, &rcLastButton);
		rcButtons.UnionRect(&rcButtons, &rcLastButton);
	}
	else {
		rcButtons.SetRectEmpty();
	}

	CSize size;
	size.cx = rcButtons.Width();
	size.cy = rcButtons.Height();
	return size;
}


// ***************************************************************************
//
// CContainedToolBar::OnCreate
//
// Description:
//	  Called by the framework after the window is being created but before
//	  the window is shown.
//
// Parameters:
//	  LPCREATESTRUCT lpCreateStruct	Pointer to the structure which contains
//									default parameters.
//
// Returns:
//	  BOOL				0 if continue; -1 if the window should be destroyed.
//
// Globals accessed:
//	  NONE
//
// Globals modified:
//	  NONE
//
// ***************************************************************************
int CContainedToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CToolBarCtrl &rToolBarCtrl = GetToolBarCtrl();

	if (m_ttip.Create(this,TTS_ALWAYSTIP))
	{
		m_ttip.Activate(TRUE);
		rToolBarCtrl.SetToolTips(&m_ttip );
	}

	EnableSearchButton(FALSE);
	return 0;
}

void CContainedToolBar::EnableSearchButton(BOOL bEnable)
{
	CToolBarCtrl &rToolBarCtrl = GetToolBarCtrl();
	rToolBarCtrl.EnableButton(ID_BROWSEFORINST, bEnable);
}