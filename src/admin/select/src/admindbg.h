//+---------------------------------------------------------------------------
//  Copyright (C) 1996, Microsoft Corporation.
//
//  File:       AdminDbg.h
//
//  Contents:   Debugging macros. Stolen from old Cairo debnot.h with the
//              following history...
//
//  History:    23-Jul-91   KyleP       Created.
//              15-Oct-91   KevinRo     Major changes and comments added
//              18-Oct-91   vich        Consolidated win4p.hxx
//              29-Apr-92   BartoszM    Moved from win4p.h
//              18-Jun-94   AlexT       Make Assert a better statement
//               7-Oct-94   BruceFo     Stole and ripped out everything except
//                                      debug prints and asserts.
//              20-Oct-95   EricB       Set component debug level in the
//                                      registry.
//              26-Feb-96   EricB       Renamed Win4xxx exported fcns to not
//                                      conflict with ole32.lib.
//
//
//  NOTE: you must call the InitializeDebugging() API before calling any other
//  APIs!
//
//  To set a non-default debug info level outside of the debugger, create the
//  below registry key:
//
//   "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\AdminDebug"
//
//  and in it create a value whose name is the component's debugging tag name
//  (the "comp" parameter to the DECLARE_INFOLEVEL macro) and whose data is
//  the desired infolevel in REG_DWORD format.
//  e.g. Sched = REG_DWORD 0x707
//
//----------------------------------------------------------------------------

#ifndef __DEBUG_H__
#define __DEBUG_H__

//+----------------------------------------------------------------------
//
// DECLARE_DEBUG(comp)
// DECLARE_INFOLEVEL(comp)
//
// This macro defines xxDebugOut where xx is the component prefix
// to be defined. This declares a static variable 'xxInfoLevel', which
// can be used to control the type of xxDebugOut messages printed to
// the terminal. For example, xxInfoLevel may be set at the debug terminal.
// This will enable the user to turn debugging messages on or off, based
// on the type desired. The predefined types are defined below. Component
// specific values should use the upper 24 bits
//
// To Use:
//
// 1)   In your components main include file, include the line
//              DECLARE_DEBUG(comp)
//      where comp is your component prefix
//
// 2)   In one of your components source files, include the line
//              DECLARE_INFOLEVEL(comp)
//      where comp is your component prefix. This will define the
//      global variable that will control output.
//
// It is suggested that any component define bits be combined with
// existing bits. For example, if you had a specific error path that you
// wanted, you might define DEB_<comp>_ERRORxxx as being
//
// (0x100 | DEB_ERROR)
//
// This way, we can turn on DEB_ERROR and get the error, or just 0x100
// and get only your error.
//
//-----------------------------------------------------------------------



#if DBG==1

class CDbg
{
public:
    CDbg(LPTSTR str);
   ~CDbg();

    void __cdecl Trace(PWSTR pszfmt, ...);
    void __cdecl Trace(LPSTR pszfmt, ...);
    void __cdecl DebugOut(unsigned long fDebugMask, LPSTR pszfmt, ...);
    void __cdecl DebugOut(unsigned long fDebugMask, PWSTR pszfmt, ...);
    void DebugMsg(LPSTR file, unsigned long line, PWSTR  msg);
    void DebugMsg(LPSTR file, unsigned long line, LPSTR  msg);
    void DebugErrorL(LPSTR file, ULONG line, LONG err);
    void DebugErrorX(LPSTR file, ULONG line, LONG err);
    void IncIndent();
    void DecIndent();

    static void AssertEx(LPSTR pszFile, int iLine, LPTSTR pszMsg);

    static ULONG s_idxTls;

private:

    ULONG
    _GetIndent();

    unsigned long   m_flInfoLevel; // must be the first data member
    unsigned long   m_flOutputOptions;
    LPTSTR          m_InfoLevelString;
};  // class CDbg

class CIndenter
{
public:

    CIndenter(CDbg *pdbg): m_pDbg(pdbg) { m_pDbg->IncIndent(); }
    ~CIndenter() { m_pDbg->DecIndent(); m_pDbg = NULL; }

private:

    CDbg *m_pDbg;
};

#define DECLARE_DEBUG(comp)     extern "C" CDbg comp##InfoLevel;
#define DECLARE_INFOLEVEL(comp) CDbg comp##InfoLevel(_T(#comp));
#define Win4Assert(x) (void)((x) || (CDbg::AssertEx(THIS_FILE,__LINE__, _T(#x)),0))

#else  // ! DBG==1

#define DECLARE_DEBUG(comp)
#define DECLARE_INFOLEVEL(comp)
#define Win4Assert(x)      NULL

#endif // ! DBG==1





////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  Debug info levels
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define DEB_ERROR           0x00000001      // exported error paths
#define DEB_WARN            0x00000002      // exported warnings
#define DEB_TRACE           0x00000004      // exported trace messages

#define DEB_DBGOUT          0x00000010      // Output to debugger
#define DEB_STDOUT          0x00000020      // Output to stdout

#define DEB_IERROR          0x00000100      // internal error paths
#define DEB_IWARN           0x00000200      // internal warnings
#define DEB_ITRACE          0x00000400      // internal trace messages

#define DEB_USER1           0x00010000      // User defined
#define DEB_USER2           0x00020000      // User defined
#define DEB_USER3           0x00040000      // User defined
#define DEB_USER4           0x00080000      // User defined
#define DEB_USER5           0x00100000      // User defined
#define DEB_USER6           0x00200000      // User defined
#define DEB_USER7           0x00400000      // User defined

#define DEB_FUNCTION        0x00800000
#define DEB_RESOURCE        0x01000000
#define DEB_METHOD          0x02000000
#define DEB_DSOBJECT        0x04000000
#define DEB_DATAOBJECT      0x08000000
#define DEB_RICHEDIT        0x10000000      // text processing
#define DEB_ELAPSEDTIME     0x20000000      // output elapsed time
#define DEB_NOCOMPNAME      0x40000000      // suppress component name

#define DEB_FORCE           0x1fffffff      // force message

#define ASSRT_MESSAGE       0x00000001      // Output a message
#define ASSRT_BREAK         0x00000002      // Int 3 on assertion
#define ASSRT_POPUP         0x00000004      // And popup message

#ifndef DEF_INFOLEVEL
#define DEF_INFOLEVEL (DEB_ERROR | DEB_WARN)
#endif

#endif // __DEBUG_H__
