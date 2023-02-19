//-----------------------------------------------------------------------------

//  

//  File: counter.h

// Copyright (c) 1994-2001 Microsoft Corporation, All Rights Reserved 
//  All rights reserved.
//  
//  
//  
//-----------------------------------------------------------------------------
 
#ifndef ESPUTIL_COUNTER_H
#define ESPUTIL_COUNTER_H


class LTAPIENTRY CCounter
{
public:
	CCounter(const TCHAR *);
	
	UINT operator++(void);
	UINT operator--(void);
	void operator+=(UINT);
	void operator-=(UINT);
	
	void Dump(void) const;
	
	~CCounter();
	
private:
	UINT m_uiCurCount;
	UINT m_uiMaxCount;
	UINT m_uiTotal;
	
	const TCHAR *m_szDescription;
};


//------------------------------------------------------------------------------
class LTAPIENTRY CSmartCheck
{
public:
	CSmartCheck(DWORD dwFreqMilli = 2000);

	void Reset();
	BOOL Check();

protected:
	DWORD	m_dwFreqMilli;
	DWORD	m_dwCancelTickMin;	// prevents calling fCancel() too often
	DWORD	m_dwCancelTickMax;
};


#endif
