#if !defined(AFX_REMOVE_H__E0F2E944_8915_11D1_984D_00C04FB9603F__INCLUDED_)
#define AFX_REMOVE_H__E0F2E944_8915_11D1_984D_00C04FB9603F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Remove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemove dialog

class CRemove : public CDialog
{
// Construction
public:
        CRemove(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
        //{{AFX_DATA(CRemove)
        enum { IDD = IDD_REMOVE };
        int             m_iState;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRemove)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CRemove)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOVE_H__E0F2E944_8915_11D1_984D_00C04FB9603F__INCLUDED_)
