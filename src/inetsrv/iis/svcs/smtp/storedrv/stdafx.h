// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__2DB263A4_499F_11D1_B78F_0080C731C893__INCLUDED_)
#define AFX_STDAFX_H__2DB263A4_499F_11D1_B78F_0080C731C893__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#ifdef _ASSERT
#undef _ASSERT
#endif //_ASSERT
#include <transmem.h>
#include <dbgtrace.h>

#endif // !defined(AFX_STDAFX_H__2DB263A4_499F_11D1_B78F_0080C731C893__INCLUDED)
