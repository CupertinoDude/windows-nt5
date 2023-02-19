/********************************************************************

Copyright (c) 1999 Microsoft Corporation

Module Name:
    stdafx.h

Abstract:
    common include dumping ground

Revision History:
    DerekM  created  05/01/99

********************************************************************/

#if !defined(AFX_STDAFX_H__F6D93C5D_DF3F_11D2_80AF_00C04F688C0B__INCLUDED_)
#define AFX_STDAFX_H__F6D93C5D_DF3F_11D2_80AF_00C04F688C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
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
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_STDAFX_H__F6D93C5D_DF3F_11D2_80AF_00C04F688C0B__INCLUDED)
