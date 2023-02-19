// MainFrm.h : interface of the CMainFrame class
//
//=--------------------------------------------------------------------------=
// Copyright  1997-1999  Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//=--------------------------------------------------------------------------=


/////////////////////////////////////////////////////////////////////////////


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApiCreateQueue();
	afx_msg void OnApiDeleteQueue();
	afx_msg void OnApiOpenQueue();
	afx_msg void OnApiCloseQueue();
	afx_msg void OnApiSendMessage();
	afx_msg void OnApiReceiveMessage();
	afx_msg void OnApiLocate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

    void OnUpdateFrameTitle(BOOL bAddToTitle);

protected:
	//
	// An array to hold the pathnames of the queues that were created so far.
	//

    //
    // Holds information about all "located" queues
    //
	CArray<ARRAYQ*, ARRAYQ*> m_PathNameArray;

    //
    // Holds information about all "opened" queues
    //
	CArray<ARRAYQ*, ARRAYQ*> m_OpenedQueuePathNameArray;

public:

	//
	// Utility function to display status string based on received msg Class property
	//
	void ClassToString(unsigned short MsgClass,LPTSTR pszStatus);



	//
	// functions to manipulate the path names array.
	//

    //
    // Adds a given ARRAYQ pointer to the PathName array.
    //
	void Add2PathNameArray(ARRAYQ* NewPath) {m_PathNameArray.Add(NewPath);}
    //
    // Removes an ARRAYQ pointer whose pathname matches
    // the given pathname form the PathName array.
    //
	ARRAYQ* RemoveFromPathNameArray(TCHAR szPathName[MAX_Q_PATHNAME_LEN]);
    //
    // Empties the PathName array and frees all allocated memory.
    //
	void CleanPathNameArray();
    //
    // Looks for an ARRAYQ pointer whose pathname matches the given one,
    // and returns the formatname of that ARRAYQ.
    //
    BOOL TranslatePathNameToFormatName(TCHAR szPathName[MAX_Q_PATHNAME_LEN],
                                       TCHAR szFormatName[MAX_Q_FORMATNAME_LEN]);
    //
    // Prints to screen the pathname of all ARRAYQ's in the PathName array.
    //
    void DisplayPathNameArray();

	//
	// functions to manipulate the opened queues path name array.
	//
	
    //
    // Adds a given ARRAYQ pointer to the Opened Queue PathName array.
    //
    void Add2OpenedQueuePathNameArray(ARRAYQ* NewPath)
											{m_OpenedQueuePathNameArray.Add(NewPath);}
    //
    // Removes an ARRAYQ pointer whose pathname matches
    // the given pathname form the Opened Queue PathName array.
    //
	ARRAYQ* RemoveFromOpenedQueuePathNameArray(TCHAR szPathName[MAX_Q_PATHNAME_LEN]);
    //
    // Checks if the Opened Queue PathName array is Empty.
    //
	BOOL IsOpenedQueueArrayEmpty();
    //
    // Looks for an ARRAYQ pointer whose pathname matches the given one,
    // and returns the formatname of that ARRAYQ.
    //
    BOOL TranslateOpenedQueuePathNameToFormatName(
                                           TCHAR szPathName[MAX_Q_PATHNAME_LEN],
                                           TCHAR szFormatName[MAX_Q_FORMATNAME_LEN]);
    //
    // Looks for an ARRAYQ pointer whose pathname matches the given one,
    // and returns the Queue Handle of that ARRAYQ.
    //
	BOOL GetQueueHandle(TCHAR szPathName[MAX_Q_PATHNAME_LEN],
													 QUEUEHANDLE* phClosedQueueHandle);
    //
    // Prints to screen the pathname of all ARRAYQ's in the PathName array.
    //
    void DisplayOpenedQueuePathNameArray();


	//
	// functions to manipulate both of the queues.
	//
	
    //
    // Moves an ARRAYQ pointer from the PathName array to the Opened Queue PathName array.
    //
    void MoveToOpenedQueuePathNameArray(TCHAR szPathName[MAX_Q_PATHNAME_LEN],
		                                           QUEUEHANDLE hQueue, DWORD dwAccess);
    //
    // Moves an ARRAYQ pointer from the Opened Queue PathName array array to the PathName.
    //
	void MoveToPathNameArray(TCHAR szPathName[MAX_Q_PATHNAME_LEN]);
    //
    // Looks for ARRAYQ's that are found in both arrays
    // and removes them from the PathName array.
    //
    void UpdatePathNameArrays();

};

extern "C" void  PrintToScreen(const TCHAR * Format, ...);
extern CWnd* pMainView;

/////////////////////////////////////////////////////////////////////////////
