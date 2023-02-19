//
// dllmain.cpp
//
// DllMain module entry point.
//

#include "globals.h"
#include "snoop.h"

//+---------------------------------------------------------------------------
//
// DllMain
//
//----------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID pvReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:

            g_hInst = hInstance;

            InitializeCriticalSection(&g_cs);

            CSnoopWnd::_InitClass();

            break;

        case DLL_PROCESS_DETACH:

            CSnoopWnd::_UninitClass();

            DeleteCriticalSection(&g_cs);

            break;
    }

    return TRUE;
}
