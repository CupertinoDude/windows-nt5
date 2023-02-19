#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <dnssdk.h>

_cdecl
main(int argc, char **argv)
{
    WCHAR             usName[MAX_PATH];
    LONG              cch;

    cch = MultiByteToWideChar(
              CP_ACP,
              0L,
              argv[1],
              -1,
              usName,
              MAX_PATH
              );

    if ( argc == 2 )
    {
        DnsEnableAdapterDomainNameRegistration( usName );
    }

    return(0);
}


