// stdafx.h : include file for standard system include files,

//  or project specific include files that are used frequently, but

//      are changed infrequently

//

// Copyright (c) 1997-2001 Microsoft Corporation, All Rights Reserved
//

#if !defined(AFX_STDAFX_H__BD8F08F5_77F0_11D1_A9D5_0060081EBBAD__INCLUDED_)
#define AFX_STDAFX_H__BD8F08F5_77F0_11D1_A9D5_0060081EBBAD__INCLUDED_

#if !defined(_WIN32_DCOM)
#define _WIN32_DCOM
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <winsvc.h>

#include <wbemcli_i.c>
#include <wbemprov_i.c>
//#include <cominit.cpp>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BD8F08F5_77F0_11D1_A9D5_0060081EBBAD__INCLUDED_)
