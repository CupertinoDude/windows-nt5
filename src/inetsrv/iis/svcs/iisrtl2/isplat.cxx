/*++

    Copyright (c) 1996  Microsoft Corporation

    Module  Name :
        isplat.cxx

    Abstract:

        This module defines functions for determining platform types

    Author:

        Johnson Apacible    (johnsona)      19-Nov-1996

        Murali Krishnan     (MuraliK)       17-Apr-1997
                   Added CriticalSectionWith SpinCount stuff
--*/

#include "precomp.hxx"

#include <inetsvcs.h>


typedef
BOOLEAN
(NTAPI *GET_PRODUCT_TYPE)(
            PNT_PRODUCT_TYPE
            );

extern "C"
PLATFORM_TYPE
IISGetPlatformType(
        VOID
        )
/*++

  This function consults the registry and determines the platform type
   for this machine.

  Arguments:

    None

  Returns:
    Platform type

--*/
{
    PLATFORM_TYPE pt;
    LONG result;
    HKEY keyHandle;
    WCHAR productType[30];
    DWORD type;
    BOOL isNt = TRUE;

    OSVERSIONINFO osInfo;

    //
    // See if the platform type has already been discovered.
    //

    if ( g_PlatformType != PtInvalid ) {
        return(g_PlatformType);
    }

    //
    // see if this is win95
    //

    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if ( GetVersionEx( &osInfo ) ) {
        isNt = (osInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
    } else {
        IIS_PRINTF((buff,"GetVersionEx failed with %d\n",
                    GetLastError()));
    }

    if ( isNt ) {

        HINSTANCE hNtdll;
        NT_PRODUCT_TYPE ntType;
        GET_PRODUCT_TYPE pfnGetProductType;

        //
        // Get the product type from the system
        //

        pt = PtNtWorkstation;
        hNtdll = LoadLibrary("ntdll.dll");
        if ( hNtdll != NULL ) {

            pfnGetProductType = (GET_PRODUCT_TYPE)
                GetProcAddress(hNtdll, "RtlGetNtProductType");

            if ( (pfnGetProductType != NULL) &&
                  pfnGetProductType( &ntType ) ) {

                if ( (ntType == NtProductLanManNt) ||
                     (ntType == NtProductServer) ) {

                    pt = PtNtServer;
                }
            }

            FreeLibrary( hNtdll );
        }

    } else {
        pt = PtWindows95;
    }

    g_PlatformType = pt;
    return(pt);

} // IISGetPlatformType




//------------------------------------------------------------------------
// SetCriticalSectionSpinCount hackery


typedef
DWORD
(WINAPI * PFN_SET_CRITICAL_SECTION_SPIN_COUNT)(
    LPCRITICAL_SECTION lpCriticalSection,
    DWORD dwSpinCount
   );

static PFN_SET_CRITICAL_SECTION_SPIN_COUNT  g_pfnSetCSSpinCount = NULL;


//------------------------------------------------------------------------
// Function: FakeSetCriticalSectionSpinCount
// Synopsis: This function fakes setting critical section spin count.
//           See CCritSec::SetSpinCount() for details
// Returns:  0 - since we are faking the set of cs with spin count
//------------------------------------------------------------------------

static DWORD
FakeSetCriticalSectionSpinCount(
    LPCRITICAL_SECTION /*lpCriticalSection*/,
    DWORD              /*dwSpinCount*/)
{
    // For faked critical sections, the previous spin count is just ZERO!
    return 0;
}


//------------------------------------------------------------------------
// Function: CCritSec::SetSpinCount
// Synopsis: This function is used to call the appropriate underlying
//           functions to set the spin count for the supplied critical
//           section. The original function is supposed to be exported out
//           of kernel32.dll from NT 4.0 SP3. If the func is not available
//           from the dll, we will use a fake function.
//
// Arguments:
//   lpCriticalSection
//      Points to the critical section object.
//
//   dwSpinCount
//      Supplies the spin count for the critical section object. For UP
//      systems, the spin count is ignored and the critical section spin
//      count is set to 0. For MP systems, if contention occurs, instead of
//      waiting on a semaphore associated with the critical section, the
//      calling thread will spin for spin count iterations before doing the
//      hard wait. If the critical section becomes free during the spin, a
//      wait is avoided.
//
// Returns:
//      The previous spin count for the critical section is returned.
//------------------------------------------------------------------------

DWORD
IISSetCriticalSectionSpinCount(
    LPCRITICAL_SECTION lpCriticalSection,
    DWORD dwSpinCount)
{
    if ( g_pfnSetCSSpinCount == NULL )
    {
        HINSTANCE tmpInstance;

        //
        // load kernel32 and get NT specific entry points
        //

        tmpInstance = LoadLibrary("kernel32.dll");
        if ( tmpInstance != NULL )
        {
            g_pfnSetCSSpinCount = (PFN_SET_CRITICAL_SECTION_SPIN_COUNT )
                GetProcAddress( tmpInstance, "SetCriticalSectionSpinCount");

            if ( g_pfnSetCSSpinCount == NULL ) {
                // the set CS Spincount function is not availble.
                //  Just thunk it.
                g_pfnSetCSSpinCount = FakeSetCriticalSectionSpinCount;
            }

            //
            // We can free this because we are statically linked to it
            //

            FreeLibrary(tmpInstance);
        }
    }

    // Pass the inputs to the global function pointer which is already setup.
    return ( g_pfnSetCSSpinCount( lpCriticalSection, dwSpinCount));
}


VOID
IISInitializeCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
    InitializeCriticalSection(lpCriticalSection);
    IISSetCriticalSectionSpinCount(lpCriticalSection, IIS_DEFAULT_CS_SPIN_COUNT);
}
