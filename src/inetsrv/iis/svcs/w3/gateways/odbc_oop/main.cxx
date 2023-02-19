/*++

Copyright (c) 1994  Microsoft Corporation

Module Name:

    main.cxx

Abstract:

    This is the HTTP ODBC gateway

Author:

    John Ludeman (johnl)   20-Feb-1995

Revision History:
	Tamas Nemeth (t-tamasn)  12-Jun-1998

--*/


#include "dbgutil.h"

#include <iis64.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif

//
//  System include files.
//

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>


#ifdef __cplusplus
} // extern "C"

#include <buffer.hxx>
#include <string.hxx>
#include <refb.hxx>

#include <lmcons.h>
//#include <datetime.hxx>
#include <parse.hxx>


#endif // __cplusplus


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


extern "C" {

#include <tchar.h>

} // extern "C"


//
//  Local include files.
//

#include "parmlist.hxx"

#include <odbcconn.hxx>
#include <festrcnv.h>


DECLARE_DEBUG_PRINTS_OBJECT();

#ifndef _NO_TRACING_
#include <initguid.h>
DEFINE_GUID(IisOdbcPoolGuid, 
0x784d8937, 0xaa8c, 0x11d2, 0x92, 0x5e, 0x00, 0xc0, 0x4f, 0x72, 0xd9, 0x0e);
#else
DECLARE_DEBUG_VARIABLE();
#endif


extern "C" {
#include <httpext.h>


BOOL
WINAPI
DLLEntry(
    HINSTANCE hDll,
    DWORD     dwReason,
    LPVOID    lpvReserved
    );
}
#include <odbcmsg.h>
#include <odbcreq.hxx>

//
//  Globals
//

BOOL           g_fIsSystemDBCS;        // Is this system DBCS?

//
//  Prototypes
//


BOOL
DoQuery(
    EXTENSION_CONTROL_BLOCK * pecb,
    const CHAR *              pszQueryFile,
    const CHAR *              pszParamList,
    STR *                     pstrError,
    int                       nCharset,
    BOOL *                    pfAccessDenied
    );


DWORD
OdbcExtensionOutput(
    EXTENSION_CONTROL_BLOCK * pecb,
    const CHAR *              pchOutput,
    DWORD                     cbOutput
    );

BOOL
OdbcExtensionHeader(
    EXTENSION_CONTROL_BLOCK * pecb,
    const CHAR *              pchStatus,
    const CHAR *              pchHeaders
    );

BOOL LookupHttpSymbols(  // eliminated, check its functionality
    const CHAR *   pszSymbolName,
    STR *          pstrSymbolValue
    );


BOOL
GetIDCCharset(
    CONST CHAR *   pszPath,
    int *          pnCharset,
    STR *          pstrError
    );

BOOL
ConvertUrlEncodedStringToSJIS(
    int            nCharset,
    STR *          pstrParams
    );

BOOL
IsSystemDBCS(
    VOID
    );

TCHAR * 
FlipSlashes( 
	TCHAR * pszPath 
	);

DWORD
HttpExtensionProc(
    EXTENSION_CONTROL_BLOCK * pecb
    )
{
    STACK_STR(     strPath, MAX_PATH);
    STACK_STR(     strParams, MAX_PATH);
    STR            strError;
    CHAR *         pch;
    DWORD          cch;
    int            nCharset;

	
	//
    //  Make sure ODBC is loaded
    //

    if ( !LoadODBC() )
    {
        STR str;

        str.FormatString( ODBCMSG_CANT_LOAD_ODBC,
                          NULL,
                          HTTP_ODBC_DLL );

        pecb->ServerSupportFunction( pecb->ConnID,
                            HSE_REQ_SEND_RESPONSE_HEADER,
                            "500 Unable to load ODBC",
                            NULL,
                            (LPDWORD) str.QueryStr() );

        return HSE_STATUS_ERROR;
    }

    //
    //  We currently only support the GET and POST methods
    //

    if ( !strcmp( pecb->lpszMethod, "POST" ))
    {
        if ( _stricmp( pecb->lpszContentType,
                      "application/x-www-form-urlencoded" ))
        {
            goto BadRequest;
        }

        //
        //  The query params are in the extra data, add a few bytes in case
        //  we need to double "'"
        //

        if ( !strParams.Resize( pecb->cbAvailable + sizeof(TCHAR) + 3))
            return HSE_STATUS_ERROR;

        strParams.Copy( (const char * ) pecb->lpbData, pecb->cbAvailable);
    }
    else if ( !strcmp( pecb->lpszMethod, "GET" ))
    {
        if ( !strParams.Copy( pecb->lpszQueryString  ))
            return HSE_STATUS_ERROR;
    }
    else
    {
BadRequest:

        STR str;

        str.FormatString( ODBCMSG_UNSUPPORTED_METHOD,
                          NULL,
                          HTTP_ODBC_DLL );

        pecb->ServerSupportFunction(
                            pecb->ConnID,
                            HSE_REQ_SEND_RESPONSE_HEADER,
                            "400 Unsupported method",
                            NULL,
                            (LPDWORD) str.QueryStr() );

        return HSE_STATUS_ERROR;
    }

    //
    //  "Charset" field is enabled for CP932 (Japanese) only in this version.
    //

    if ( 932 != GetACP() )
    {
        nCharset = CODE_ONLY_SBCS;
    }
    else
    {
        //
        //  Get the charset from .idc file
        //

        if ( !GetIDCCharset( pecb->lpszPathTranslated, &nCharset, &strError ) )
        {
            STR str;
            LPCSTR apsz[1];

            apsz[0] = strError.QueryStr();

            str.FormatString( ODBCMSG_ERROR_PERFORMING_QUERY,
                              apsz,
                              HTTP_ODBC_DLL,
                              1024 + strError.QueryCB() );

            pecb->ServerSupportFunction( pecb->ConnID,
                                         HSE_REQ_SEND_RESPONSE_HEADER,
                                         (LPDWORD) "500 Error performing query",
                                         NULL,
                                         (LPDWORD) str.QueryStr() );

            return HSE_STATUS_ERROR;
        }

        if ( strParams.QueryCB() )
        {
            if ( CODE_ONLY_SBCS != nCharset )
            {
                //
                //  Convert the charset of Parameters to SJIS
                //

                if ( !ConvertUrlEncodedStringToSJIS( nCharset, &strParams ) )
                {
                    STR strError;
                    STR str;

                    strError.LoadString( GetLastError(), (LPCTSTR) NULL );

                    str.FormatString( ODBCMSG_ERROR_PERFORMING_QUERY,
                                      NULL,
                                      HTTP_ODBC_DLL );

                    pecb->ServerSupportFunction( pecb->ConnID,
                                                 HSE_REQ_SEND_RESPONSE_HEADER,
                                                 (LPDWORD) "500 Error performing Query",
                                                 NULL,
                                                 (LPDWORD) str.QueryStr() );

                    return HSE_STATUS_ERROR;
                }
            }
        }
    }

    //
    //  Walk the parameter string to do three things:
    //
    //    1) Double all single quotes to prevent SQL quoting problem
    //    2) Remove escaped '\n's so we don't break parameter parsing later on
    //    3) Replace all '&' parameter delimiters with real '\n' so escaped
    //       '&'s won't get confused later on
    //

    pch = strParams.QueryStr();
    cch = strParams.QueryCCH();

    while ( *pch )
    {
        switch ( *pch )
        {
        case '%':
            if ( pch[1] == '0' && toupper(pch[2]) == 'A' )
            {
                pch[1] = '2';
                pch[2] = '0';
            }
            else if ( pch[1] == '2' && pch[2] == '7' )
            {
                //
                //  This is an escaped single quote
                //

                if ( strParams.QuerySize() < (cch + 4) )  // Include null
                {
                    DWORD Pos = DIFF(pch - strParams.QueryStr());

                    if ( !strParams.Resize( cch + 4 ) )
                        return HSE_STATUS_ERROR;

                    //
                    //  Adjust for possible pointer shift
                    //

                    pch = strParams.QueryStr() + Pos;
                }

                //
                //  Note the memory copy just doubles the existing quote
                //

                memmove( pch+3,
                         pch,
                         (cch + 1) - DIFF(pch - strParams.QueryStr()) );

                cch += 3;          // Adjust for the additional '%27'
                pch += 3;
            }

            pch += 3;
            break;

        case '\'':
            if ( strParams.QuerySize() < (cch + 2) )
            {
                DWORD Pos = DIFF(pch - strParams.QueryStr());

                if ( !strParams.Resize( cch + 2 ) )
                    return HSE_STATUS_ERROR;

                //
                //  Adjust for possible pointer shift
                //

                pch = strParams.QueryStr() + Pos;
            }

            //
            //  Note the memory copy just doubles the existing quote
            //

            memmove( pch+1,
                     pch,
                     (cch + 1) - DIFF(pch - strParams.QueryStr()) );

            pch += 2;
            cch++;          // Adjust for the additional '''
            break;

        case '&':
            *pch = '\n';
            pch++;
            break;

        default:
            pch++;
        }
    }

    //
    //  The path info contains the location of the query file
    //

    if ( !strPath.Copy( pecb->lpszPathTranslated ))
        return HSE_STATUS_ERROR;

    FlipSlashes( strPath.QueryStr() );

    //
    //  Attempt the query
    //

    BOOL fAccessDenied = FALSE;

    if ( !DoQuery( pecb,
                   strPath.QueryStr(),
                   strParams.QueryStr(),
                   &strError,
                   nCharset,
                   &fAccessDenied ))
    {
        if ( fAccessDenied )
        {
            pecb->ServerSupportFunction( pecb->ConnID,
                     HSE_REQ_SEND_RESPONSE_HEADER,
                     (LPDWORD) "401 Authentication Required",
                     NULL,
                     NULL );

            return HSE_STATUS_ERROR;
        }
        else
        {
            STR str;
            LPCSTR apsz[1];

            apsz[0] = strError.QueryStr();

            //
            //  Note we terminate the error message (ODBC sometimes generates
            //  22k errors) *and* we double the buffer size we pass to FormatString()
            //  because the win32 API FormatMessage() has a bug that doesn't
            //  account for Unicode conversion
            //

            if ( strlen( apsz[0] ) > 1024 ) {
                apsz[1024] = '\0';
            }

            str.FormatString( ODBCMSG_ERROR_PERFORMING_QUERY,
                              apsz,
                              HTTP_ODBC_DLL,
                              1024 + strError.QueryCB() );

            pecb->ServerSupportFunction( pecb->ConnID,
                     HSE_REQ_SEND_RESPONSE_HEADER,
                     (LPDWORD) "500 Error performing query",
                     NULL,
                     (LPDWORD) str.QueryStr() );

            return HSE_STATUS_ERROR;
        }
    }

    return HSE_STATUS_SUCCESS;
}

BOOL
DoQuery(
    EXTENSION_CONTROL_BLOCK * pecb,    
    const CHAR *              pszQueryFile,
    const CHAR *              pszParamList,
    STR *                     pstrError,
    int                       nCharset,
    BOOL *                    pfAccessDenied
    )
/*++

Routine Description:

    Performs the actual query or retrieves the same query from the query
    cache

Arguments:

    pecb - Extension context
    pTsvcInfo - Server info class
    pszQueryFile - .wdg file to use for query
    pszParamList - Client supplied param list
//    pReq - Http request this query is for
    pstrError - Error text to return errors in
    pfAccessDenied - Set to TRUE if the user was denied access

Return Value:

    TRUE if successful, FALSE on error

--*/
{
    ODBC_REQ *              podbcreq;
    ODBC_REQ *              podbcreqCached;
    VOID *                  pvCacheCookie = NULL;
    BOOL                    fRet = TRUE;
    STACK_STR(              strHeaders, MAX_PATH);
    CHAR                    achPragmas[250];
    DWORD                   cbPragmas = sizeof(achPragmas);
    BOOL                    fRetrieveFromCache = TRUE;
    PSECURITY_DESCRIPTOR    pSecDesc = NULL;

    //
    //  Create an odbc request as we will either use it for the query or
    //  as the key for the cache lookup
    //	
	
	podbcreq = new ODBC_REQ(  pszQueryFile,
                             pszParamList,
//                             hToken,
                             0, // hard-coded - pReq->QueryMetaData()->QueryPoolIDCTimeout(),
                             (ODBC_REQ_FIND_SYMBOL) NULL, // (ODBC_REQ_FIND_SYMBOL) LookupHttpSymbols,
                             (VOID *) NULL, // pReq,
                             nCharset );

    if ( !podbcreq ||
         !podbcreq->IsValid() )
    {
        pstrError->LoadString( GetLastError() );

        delete podbcreq;
        return FALSE;
    }

    //
    //  Check to see if user already authentified
    //

    CHAR  achLoggedOnUser[UNLEN + 1];
    DWORD dwLoggedOnUser = sizeof( achLoggedOnUser );
    BOOL  fIsAuth = pecb->GetServerVariable( (HCONN)pecb->ConnID,
                                             "LOGON_USER",
                                             achLoggedOnUser,
                                             &dwLoggedOnUser ) &&
                                             achLoggedOnUser[0] != '\0';


    //
    //  Check to see if the client specified "Pragma: no-cache"
    //

    if ( pecb->GetServerVariable( pecb->ConnID,
                                  "HTTP_PRAGMA",
                                  achPragmas,
                                  &cbPragmas ))
    {
        CHAR * pch;

        //
        //  Look for "no-cache"
        //

        pch = _strupr( achPragmas );

        while ( pch = strchr( pch, 'N' ))
        {
            if ( !memcmp( pch, "NO-CACHE", 8 ))
            {
                fRetrieveFromCache = FALSE;
                goto Found;
            }

            pch++;
        }
    }

Found:



        //
        //  Open the query file and do the query
        //

        if ( !podbcreq->OpenQueryFile( pfAccessDenied )       ||
             !podbcreq->ParseAndQuery( achLoggedOnUser )      ||
             !podbcreq->AppendHeaders( &strHeaders )          ||
             !podbcreq->OutputResults( (ODBC_REQ_CALLBACK) OdbcExtensionOutput,
                                       pecb, &strHeaders,
                                       (ODBC_REQ_HEADER) OdbcExtensionHeader,
                                       fIsAuth,
                                       pfAccessDenied ) )
        {
            fRet = FALSE;
            goto Exit;
        }


Exit:

    if ( !fRet )
    {
        podbcreq->GetLastErrorText( pstrError );
	}
            delete podbcreq;
    



    if ( pSecDesc )
    {
        LocalFree( pSecDesc );
    }

    return fRet;
}


DWORD OdbcExtensionOutput( EXTENSION_CONTROL_BLOCK * pecb,
                           const CHAR *              pchOutput,
                           DWORD                     cbOutput )
{
    if ( !pecb->WriteClient( pecb->ConnID,
                             (VOID *) pchOutput,
                             &cbOutput,
                             0 ))

    {
        return GetLastError();
    }

    return NO_ERROR;
}


BOOL OdbcExtensionHeader( EXTENSION_CONTROL_BLOCK * pecb,
                           const CHAR *              pchStatus,
                           const CHAR *              pchHeaders )
{
    return pecb->ServerSupportFunction(
                pecb->ConnID,
                HSE_REQ_SEND_RESPONSE_HEADER,
                (LPDWORD) "200 OK",
                NULL,
                (LPDWORD) pchHeaders );
}


BOOL
WINAPI
DLLEntry(
    HINSTANCE hDll,
    DWORD     dwReason,
    LPVOID    lpvReserved
    )
{
    DWORD  err;

    switch ( dwReason )
    {
    case DLL_PROCESS_ATTACH:

#ifdef _NO_TRACING_
        CREATE_DEBUG_PRINT_OBJECT( "httpodbc.dll");
        SET_DEBUG_FLAGS( 0);
#else
        CREATE_DEBUG_PRINT_OBJECT( "httpodbc.dll", IisOdbcPoolGuid);
#endif

        if ( !InitializeOdbcPool() )
        {
            return FALSE;
        }

        DisableThreadLibraryCalls( hDll );

        g_fIsSystemDBCS = IsSystemDBCS();
        break;

    case DLL_PROCESS_DETACH:

        TerminateOdbcPool();
        DELETE_DEBUG_PRINT_OBJECT();
        break;

    default:
        break;
    }

    return TRUE;
}

BOOL
GetExtensionVersion(
    HSE_VERSION_INFO * pver
    )
{
    pver->dwExtensionVersion = MAKELONG( 0, 2 );
    strcpy( pver->lpszExtensionDesc,
            "Microsoft HTTP ODBC Gateway, v2.0" );

    return TRUE;
}


CHAR * skipwhite( CHAR * pch )
{
    CHAR ch;

    while ( 0 != (ch = *pch) )
    {
        if ( ' ' != ch && '\t' != ch )
            break;
        ++pch;
    }

    return pch;
}


CHAR * nextline( CHAR * pch )
{
    CHAR ch;

    while ( 0 != (ch = *pch) )
    {
        ++pch;

        if ( '\n' == ch )
        {
            break;
        }
    }

    return pch;
}


BOOL
GetIDCCharset(
    CONST CHAR *   pszPath,
    int *          pnCharset,
    STR *          pstrError
    )
{
    BUFFER           buff;
    HANDLE           hFile;
    DWORD            dwSize;

#define QUERYFILE_READSIZE  4096

    if ( !pnCharset )
        return FALSE;
    *pnCharset = CODE_ONLY_SBCS;

    if ( !buff.Resize( QUERYFILE_READSIZE + sizeof(TCHAR) ) )
    {
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        pstrError->LoadString( GetLastError() );
        return FALSE;
    }

    hFile = CreateFile(
                    pszPath,
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                    NULL);

    if ( INVALID_HANDLE_VALUE == hFile )
    {
        LPCSTR apsz[1];

        apsz[0] = pszPath;
        pstrError->FormatString( ODBCMSG_QUERY_FILE_NOT_FOUND,
                               apsz,
                               HTTP_ODBC_DLL );
        return FALSE;
    }

    if ( !ReadFile( hFile, buff.QueryPtr(), QUERYFILE_READSIZE, &dwSize, NULL ) )
    {
        pstrError->LoadString( GetLastError() );
        return FALSE;
    }

    CloseHandle( hFile );

    *((CHAR *) buff.QueryPtr() + dwSize) = '\0';

    CHAR * pch = (CHAR *)buff.QueryPtr();  // we know that this is null terminated!

    while ( *pch )
    {
        pch = skipwhite( pch );

        if ( 'C' == toupper( *pch ) && !_strnicmp( IDC_FIELDNAME_CHARSET, pch, sizeof(IDC_FIELDNAME_CHARSET)-1 ) )
        {
            pch += sizeof(IDC_FIELDNAME_CHARSET) - 1;
            pch = skipwhite( pch );
            if ( 932 == GetACP() )
            {
                if ( !_strnicmp( IDC_CHARSET_JIS1, pch, sizeof(IDC_CHARSET_JIS1)-1 ) ||
                     !_strnicmp( IDC_CHARSET_JIS2, pch, sizeof(IDC_CHARSET_JIS2)-1 ) )
                {
                    *pnCharset = CODE_JPN_JIS;
                    break;
                }
                else if ( !_strnicmp( IDC_CHARSET_EUCJP, pch, sizeof(IDC_CHARSET_EUCJP)-1 ))
                {
                    *pnCharset = CODE_JPN_EUC;
                    break;
                }
                else if ( !_strnicmp( IDC_CHARSET_SJIS, pch, sizeof(IDC_CHARSET_SJIS)-1 ))
                {
                    *pnCharset = CODE_ONLY_SBCS;
                    break;
                }
                else
                {
                    LPCSTR apsz[1];
                    //
                    //  illegal value for Charset: field
                    //
                    apsz[0] = pszPath;
                    pstrError->FormatString( ODBCMSG_UNREC_FIELD,
                                           apsz,
                                           HTTP_ODBC_DLL );
                    return FALSE;
                }
            }

//
//          please add code here to support other FE character encoding(FEFEFE)
//
//          else if ( 949 == GetACP() )
//          ...

        }
        pch = nextline( pch );
    }

    return TRUE;
}


BOOL
ConvertUrlEncodedStringToSJIS(
    int            nCharset,
    STR *          pstrParams
    )
{
    STACK_STR( strTemp, MAX_PATH);
    int cbSJISSize;
    int nResult;

    //
    //  Pre-process the URL encoded parameters
    //

    for ( char * pch = pstrParams->QueryStr(); *pch; ++pch )
    {
        if ( *pch == '&' )
            *pch = '\n';
        else if ( *pch == '+' )
            *pch = ' ';
    }

    //
    //  URL decoding (decode %nn only)
    //

    pstrParams->Unescape();

    //
    //  charset conversion
    //

    if ( !pstrParams->Clone( &strTemp ) )
    if ( !strTemp.Copy( pstrParams->QueryStr() ) )
        return FALSE;

    cbSJISSize = UNIX_to_PC(
                            GetACP(),
                            nCharset,
                            (UCHAR *)strTemp.QueryStr(),
                            strTemp.QueryCB(),
                            NULL,
                            0 );

    if ( !pstrParams->Resize( cbSJISSize + sizeof(TCHAR) ) )
        return FALSE;

    nResult = UNIX_to_PC(
                            GetACP(),
                            nCharset,
                            (UCHAR *)strTemp.QueryStr(),
                            strTemp.QueryCB(),
                            (UCHAR *)pstrParams->QueryStr(),
                            cbSJISSize );
    if ( -1 == nResult || nResult != cbSJISSize )
        return FALSE;

    DBG_REQUIRE ( pstrParams->SetLen( cbSJISSize));

    //
    //  URL encoding
    //

    if ( !pstrParams->Escape() )
        return FALSE;

    return TRUE;
}

BOOL
IsSystemDBCS(
    VOID )
{
    WORD wPrimaryLangID = PRIMARYLANGID( GetSystemDefaultLangID() );

    return ( wPrimaryLangID == LANG_JAPANESE ||
             wPrimaryLangID == LANG_CHINESE ||
             wPrimaryLangID == LANG_KOREAN );
}

TCHAR * FlipSlashes( TCHAR * pszPath )
{
    TCHAR   ch;
    TCHAR * pszScan = pszPath;

    while( ( ch = *pszScan ) != TEXT('\0') )
    {
        if( ch == TEXT('/') )
        {
            *pszScan = TEXT('\\');
        }

        pszScan++;
    }

    return pszPath;

}   // FlipSlashes
