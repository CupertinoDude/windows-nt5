#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <winsock2.h>
#include <wsipx.h>
#include <svcguid.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc.h>
#include <rpcdce.h>

#define BUFFSIZE 3000

GUID ServiceClassId = { /* 5b50962a-e5a5-11cf-a555-00c04fd8d4ac */
    0x5b50962a,
    0xe5a5,
    0x11cf,
    {0xa5, 0x55, 0x00, 0xc0, 0x4f, 0xd8, 0xd4, 0xac}
  };

_cdecl
main(int argc, char **argv)
{
    HANDLE hLib;
    WCHAR Buffer[BUFFSIZE];
    PWSAQUERYSETW Query = (PWSAQUERYSETW)Buffer;
    HANDLE hRnr;
    DWORD dwIp;
    DWORD dwQuerySize = BUFFSIZE;
    WSADATA wsaData;
    ANSI_STRING         asServiceInstanceName;
    UNICODE_STRING      usServiceInstanceName;
    WCHAR               UnicodeStringBuf[1024];
    ANSI_STRING         asContext;
    UNICODE_STRING      usContext;
    WCHAR               UnicodeStringBuf2[1024];

    usServiceInstanceName.Length = 0;
    usServiceInstanceName.MaximumLength = 1024;
    usServiceInstanceName.Buffer = UnicodeStringBuf;

    usContext.Length = 0;
    usContext.MaximumLength = 1024;
    usContext.Buffer = UnicodeStringBuf2;

    if ( argc < 2 || argc > 4 )
    {
        printf( "\nUsage: lookup <Name> <Context> Deep\n" );

        return( -1 );
    }

    RtlInitAnsiString( &asServiceInstanceName, argv[1] );
    RtlAnsiStringToUnicodeString( &usServiceInstanceName,
                                  &asServiceInstanceName,
                                  FALSE );
    RtlInitAnsiString( &asContext, argv[2] );
    RtlAnsiStringToUnicodeString( &usContext,
                                  &asContext,
                                  FALSE );

    WSAStartup(MAKEWORD(1, 1), &wsaData);

    memset(Query, 0, sizeof(*Query));

    Query->dwSize = sizeof(*Query);
    Query->lpszServiceInstanceName = usServiceInstanceName.Buffer;
    Query->lpVersion = 0;
    Query->dwNameSpace = NS_NTDS;
    Query->lpNSProviderId = 0;
    Query->lpszContext = usContext.Buffer;
    Query->dwNumberOfProtocols = 0;
    Query->lpServiceClassId = &ServiceClassId;

    if( WSALookupServiceBegin( Query,
                               ( argv[3][0] == 'D' ? LUP_DEEP : 0 ) |
                               LUP_RETURN_NAME |
                               // LUP_RETURN_TYPE |
                               // LUP_RETURN_VERSION |
                               LUP_RETURN_COMMENT |
                               // LUP_RETURN_ADDR |
                               // LUP_RETURN_BLOB |
                               // LUP_RETURN_ALIASES |
                               // LUP_RETURN_QUERY_STRING |
                               LUP_CONTAINERS,
                               // LUP_RETURN_ALL,
                               &hRnr ) == SOCKET_ERROR )
    {
        printf( "LookupBegin failed  %d\n", GetLastError() );
    }

    while ( WSALookupServiceNext( hRnr,
                                  0,
                                  &dwQuerySize,
                                  Query ) == NO_ERROR )
    {
        printf( "Next got: \n" );
        printf( "   dwSize = %d\n",
                Query->dwSize );
        printf( "   dwOutputFlags = %d\n",
                Query->dwOutputFlags );
        printf( "   lpszServiceInstanceName = %ws\n",
                Query->lpszServiceInstanceName );
        if ( Query->lpVersion )
        {
            printf( "   lpVersion->dwVersion = %d\n",
                    Query->lpVersion->dwVersion );
            printf( "   lpVersion->ecHow = %d\n",
                    Query->lpVersion->ecHow );
        }
        if ( Query->lpszComment )
        {
            printf( "   lpszComment = %ws\n",
                    Query->lpszComment );
        }
        printf( "   dwNameSpace = %d\n",
                Query->dwNameSpace );
        if ( Query->lpszContext )
        {
            printf( "   lpszContext = %ws\n",
                    Query->lpszContext );
        }
        printf( "   dwNumberOfCsAddrs = %d\n",
                Query->dwNumberOfCsAddrs );
    }

    printf( "Next finished with %d\n", GetLastError() );

    if( WSALookupServiceEnd( hRnr ) )
    {
        printf( "ServiceEnd failed %d\n", GetLastError() );
    }

    WSACleanup();

    return(0);
}


