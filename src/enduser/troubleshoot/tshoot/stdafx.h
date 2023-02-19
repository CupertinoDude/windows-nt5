// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__A8F3A149_99E9_11D2_8C7E_00C04F949D33__INCLUDED_)
#define AFX_STDAFX_H__A8F3A149_99E9_11D2_8C7E_00C04F949D33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
//#define _ATL_FREE_THREADED
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

///////////////////////////////////////////////////////////////////////////////
// Threading model of the Local Troubleshooter
#ifdef LOCAL_TROUBLESHOOTER
	#ifdef _ATL_FREE_THREADED
	#define RUNNING_FREE_THREADED()			true
	#define RUNNING_APARTMENT_THREADED()	false
	#endif
	#ifdef _ATL_APARTMENT_THREADED
	#define RUNNING_FREE_THREADED()			false
	#define RUNNING_APARTMENT_THREADED()	true
	#endif
#else
	#define RUNNING_FREE_THREADED()			false
	#define RUNNING_APARTMENT_THREADED()	false
#endif
///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8F3A149_99E9_11D2_8C7E_00C04F949D33__INCLUDED)
