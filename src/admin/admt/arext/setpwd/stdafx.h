// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__660C4751_349D_11D3_8C93_0090270D48D1__INCLUDED_)
#define AFX_STDAFX_H__660C4751_349D_11D3_8C93_0090270D48D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#define _ATL_DEBUG_INTERFACES
//#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

//#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <comdef.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__660C4751_349D_11D3_8C93_0090270D48D1__INCLUDED)
