//-----------------------------------------------------------------------------

//  

//  File: gotohelp.h

// Copyright (c) 1994-2001 Microsoft Corporation, All Rights Reserved 
//  All rights reserved.
//  
//  
//  
//-----------------------------------------------------------------------------
 
class LTAPIENTRY CGotoHelp : public CRefCount
{
public:
	virtual void Edit() = 0;
	
	virtual BOOL GotoHelp() = 0;
	
};



class LTAPIENTRY CEspGotoHelp : public CGotoHelp
{
public:
	explicit CEspGotoHelp(UINT uiHelpId);

	virtual void Edit();
	virtual BOOL GotoHelp();

private:
	UINT m_uiHelpId;
};


class LTAPIENTRY CExternalGotoHelp : public CGotoHelp
{
public:
	CExternalGotoHelp(const TCHAR *szFileName, UINT uiHelpId);

	virtual void Edit();
	virtual BOOL GotoHelp();

private:
	CLString m_strFileName;
	UINT m_uiHelpId;
};
