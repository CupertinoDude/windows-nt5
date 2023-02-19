#if !defined(AFX_PKGDTL_H__BB970E11_9CA4_11D0_8D3F_00A0C90DCAE7__INCLUDED_)
#define AFX_PKGDTL_H__BB970E11_9CA4_11D0_8D3F_00A0C90DCAE7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// pkgdtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPackageDetails dialog

class CPackageDetails : public CPropertyPage
{
        DECLARE_DYNCREATE(CPackageDetails)

// Construction
public:
        CPackageDetails();
        ~CPackageDetails();

        CPackageDetails ** m_ppThis;

// Dialog Data
        //{{AFX_DATA(CPackageDetails)
        enum { IDD = IDD_PACKAGE_DETAILS };
        CListBox        m_cList;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CPackageDetails)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        //}}AFX_VIRTUAL

        void RefreshData(void);

// Implementation
protected:
        // Generated message map functions
        //{{AFX_MSG(CPackageDetails)
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
        void DumpDetails(FILE *);
        void DumpClassDetail(FILE *, CLASSDETAIL *);

        public:
            long    m_hConsoleHandle; // Handle given to the snap-in by the console
            APP_DATA * m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PKGDTL_H__BB970E11_9CA4_11D0_8D3F_00A0C90DCAE7__INCLUDED_)
