// Copyright (c) 1999  Microsoft Corporation.  All Rights Reserved.
// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__81E89BB6_32F9_4537_BD95_2BC45CCEE0BF__INCLUDED_)
#define AFX_STDAFX_H__81E89BB6_32F9_4537_BD95_2BC45CCEE0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef STRICT
#define STRICT
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#import "mswebdvd.tlb" raw_interfaces_only, no_namespace, named_guids
#pragma warning( disable: 4100 )  /*unreferenced formal parameter*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__81E89BB6_32F9_4537_BD95_2BC45CCEE0BF__INCLUDED)
