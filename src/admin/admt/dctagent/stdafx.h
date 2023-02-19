// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__6F29585B_B641_11D2_A1DE_00A0C9AFE114__INCLUDED_)
#define AFX_STDAFX_H__6F29585B_B641_11D2_A1DE_00A0C9AFE114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#ifdef OFA
#define _WIN32_WINNT 0x0400
#else
#define _WIN32_WINNT 0x0351
#endif
#endif
#define _ATL_APARTMENT_THREADED

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
//#define _ATL_DEBUG_INTERFACES
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
};
extern CExeModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6F29585B_B641_11D2_A1DE_00A0C9AFE114__INCLUDED)
