// PageIni.h : Declaration of the CPageIni

#ifndef __PAGEREGISTRY_H_
#define __PAGEREGISTRY_H_

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "msconfigstate.h"

/////////////////////////////////////////////////////////////////////////////
// CPageIni
class CPageRegistry : 
	public CAxDialogImpl<CPageRegistry>
{
public:
	CPageRegistry()
	{
	}

	~CPageRegistry()
	{
	}

	enum { IDD = IDD_PAGEREGISTRY };

BEGIN_MSG_MAP(CPageIni)
MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

private:
	CString			m_strCaption;		// contains the localized name of this page

public:
	//-------------------------------------------------------------------------
	// Functions used by the parent dialog.
	//-------------------------------------------------------------------------

	BOOL IsValid(CMSConfigState * state)
	{
		return TRUE;
	}

	LPCTSTR GetCaption()
	{
		if (m_strCaption.IsEmpty())
		{
			::AfxSetResourceHandle(_Module.GetResourceInstance());
			m_strCaption.LoadString(IDS_REGISTRY_CAPTION);
		}
		return m_strCaption;
	}
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::EnableWindow(GetDlgItem(IDC_LISTBOXREGISTRY), FALSE);
		::EnableWindow(GetDlgItem(IDC_BUTTONRUNREGCLEAN), FALSE);
		::EnableWindow(GetDlgItem(IDC_BUTTONCHECKREGISTRY), FALSE);
		return 0;
	}
};

#endif //__PAGEREGISTRY_H_
