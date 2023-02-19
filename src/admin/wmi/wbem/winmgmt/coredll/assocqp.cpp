
/*++

Copyright (C) 1999-2001 Microsoft Corporation

Module Name:

    ASSOCQP.CPP

Abstract:

    Association query parser

History:

    raymcc   04-Jul-99   Created.
    raymcc   14-Aug-99   Resubmit due to VSS problem.

--*/

#include "precomp.h"
#include <stdio.h>
#include <stdlib.h>

#include <wbemcore.h>


// ==========================================================================
//                        ASSOCIATION QUERY PARSER.
// ==========================================================================


#define QASSOC_TOK_STRING       101
#define QASSOC_TOK_IDENT        102
#define QASSOC_TOK_DOT          103
#define QASSOC_TOK_EQU          104
#define QASSOC_TOK_COLON        105

#define QASSOC_TOK_ERROR        1
#define QASSOC_TOK_EOF          0

#define ST_IDENT                13
#define ST_STRING               19
#define ST_QSTRING              26
#define ST_QSTRING_ESC          30

// DFA State Table for Assoc query tokens.
// =======================================

LexEl AssocQuery_LexTable[] =
{

// State    First   Last        New state,  Return tok,      Instructions
// =======================================================================
/* 0 */  L'A',   L'Z',       ST_IDENT,   0,               GLEX_ACCEPT,
/* 1 */  L'a',   L'z',       ST_IDENT,   0,               GLEX_ACCEPT,
/* 2 */  L'_',   GLEX_EMPTY, ST_IDENT,   0,               GLEX_ACCEPT,
/* 3 */  L'{',   GLEX_EMPTY, ST_STRING,  0,               GLEX_CONSUME,

/* 4 */  L'=',   GLEX_EMPTY, 0,  QASSOC_TOK_EQU, GLEX_ACCEPT|GLEX_RETURN,
/* 5 */  L'.',   GLEX_EMPTY, 0,  QASSOC_TOK_DOT, GLEX_ACCEPT|GLEX_RETURN,
/* 6 */  L':',   GLEX_EMPTY, 0,  QASSOC_TOK_COLON, GLEX_ACCEPT|GLEX_RETURN,

/* 7 */  L' ',  GLEX_EMPTY, 0,  0,               GLEX_CONSUME,
/* 8 */  L'\t',  GLEX_EMPTY, 0,  0,               GLEX_CONSUME,
/* 9 */  L'\n',  GLEX_EMPTY, 0,  0,               GLEX_CONSUME|GLEX_LINEFEED,
/* 10 */  L'\r',  GLEX_EMPTY, 0,  0,               GLEX_CONSUME,
/* 11 */  0,      GLEX_EMPTY, 0,  QASSOC_TOK_EOF,   GLEX_CONSUME|GLEX_RETURN, // Note forced return
/* 12 */  GLEX_ANY, GLEX_EMPTY, 0,        QASSOC_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,


/* ST_IDENT */

/* 13 */  L'a',   L'z',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 14 */  L'A',   L'Z',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 15 */  L'_',   GLEX_EMPTY, ST_IDENT,   0,          GLEX_ACCEPT,
/* 16 */  L'0',   L'9',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 17 */  GLEX_ANY, GLEX_EMPTY,  0,  QASSOC_TOK_IDENT,  GLEX_PUSHBACK|GLEX_RETURN,

/* ST_STRING */
/* 18 */  0, GLEX_EMPTY, 0,        QASSOC_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,
/* 19 */  L'"', GLEX_EMPTY, ST_QSTRING, 0, GLEX_ACCEPT,
/* 20 */  L'}',  GLEX_EMPTY, 0, QASSOC_TOK_STRING, GLEX_RETURN,
/* 21 */  L' ',  GLEX_EMPTY, ST_STRING, 0, GLEX_ACCEPT,
/* 22 */  L'\r',  GLEX_EMPTY, ST_STRING, 0, GLEX_ACCEPT,
/* 23 */  L'\n',  GLEX_EMPTY, ST_STRING, 0, GLEX_ACCEPT,
/* 24 */  L'\t',  GLEX_EMPTY, ST_STRING, 0, GLEX_ACCEPT,
/* 25 */  GLEX_ANY, GLEX_EMPTY, ST_STRING, 0, GLEX_ACCEPT,

/* ST_QSTRING */
/* 26 */   0,    GLEX_EMPTY,   0, QASSOC_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,
/* 27 */   L'"', GLEX_EMPTY,   ST_STRING, 0, GLEX_ACCEPT,
/* 28 */   L'\\', GLEX_EMPTY,   ST_QSTRING_ESC, 0, GLEX_ACCEPT,
/* 29 */   GLEX_ANY, GLEX_EMPTY, ST_QSTRING, 0, GLEX_ACCEPT,

/* ST_QSTRING_ESC */
/* 30 */   GLEX_ANY, GLEX_EMPTY, ST_QSTRING, 0, GLEX_ACCEPT,
};

/*----------------------------------------------------

References of {objpath} where
    ResultClass=XXX
    Role=YYY
    RequiredQualifier=QualifierName
    ClassDefsOnly

Associators of {objpath} where
    ResultClass=XXX
    AssocClass=YYY
    Role=PPP
    RequiredQualifier=QualifierName
    RequiredAssocQualifier=QualifierName
    ClassDefsOnly

------------------------------------------------------*/

static BOOL ParseAssocQuery(
    IN  LPWSTR  Query,
    OUT LPWSTR *pTargetObj,
    OUT LPWSTR *pResultClass,
    OUT LPWSTR *pAssocClass,
    OUT LPWSTR *pRole,
    OUT LPWSTR *pResultRole,
    OUT LPWSTR *pRequiredQualifier,
    OUT LPWSTR *pRequiredAssocQualifier,
    OUT DWORD  *pdwQueryType
    )
{
    *pTargetObj = 0;
    *pResultClass = 0;
    *pAssocClass = 0;
    *pRole = 0;
    *pResultRole = 0;
    *pRequiredQualifier = 0;
    *pRequiredAssocQualifier = 0;
    *pdwQueryType = 0;

    // TBD: Check for out-of-memory here
    CTextLexSource src(Query);
    CGenLexer Lexer(AssocQuery_LexTable, &src);

    int nTok = 0;
    BOOL bHadTokens = FALSE;

    // Get first token.
    // TBD: Check for out-of-memory
    // =============================
    nTok = Lexer.NextToken();
    if (nTok != QASSOC_TOK_IDENT)
        goto Error;

    // REFERENCES or ASSOCIATORS
    // =========================
    if (wbem_wcsicmp(L"References", Lexer.GetTokenText()) == 0)
        *pdwQueryType |= QUERY_TYPE_GETREFS;
    else if (wbem_wcsicmp(L"Associators", Lexer.GetTokenText()) == 0)
        *pdwQueryType |= QUERY_TYPE_GETASSOCS;
    else
        goto Error;

    // OF
    // ==
    nTok = Lexer.NextToken();
    if (nTok != QASSOC_TOK_IDENT)
        goto Error;
    if (wbem_wcsicmp(L"of", Lexer.GetTokenText()) != 0)
        goto Error;

    // {OBJECTPATH}
    // ============
    nTok = Lexer.NextToken();
    if (nTok != QASSOC_TOK_STRING)
        goto Error;

    *pTargetObj = Macro_CloneLPWSTR(Lexer.GetTokenText());

    // WHERE
    // =====
    nTok = Lexer.NextToken();
    if (nTok == QASSOC_TOK_EOF)
        goto Completed;

    if (nTok != QASSOC_TOK_IDENT)
        goto Error;

    if (wbem_wcsicmp(L"where", Lexer.GetTokenText()) != 0)
        goto Error;

    // Check for RESULTCLASS, ROLE, ASSOCCLASS, CLASSDEFSONLY,
    // REQUIREDQUALIFIER, REQUIREDASSOCQUALIFIER
    // ======================================================

    for (;;)
    {
        nTok = Lexer.NextToken();
        if (nTok == QASSOC_TOK_ERROR)
            goto Error;

        if (nTok == QASSOC_TOK_EOF)
        {
            if(!bHadTokens)
                goto Error;
            else
                goto Completed;
        }

        if (nTok != QASSOC_TOK_IDENT)
            goto Error;

        bHadTokens = TRUE;

        if (wbem_wcsicmp(L"RESULTCLASS", Lexer.GetTokenText()) == 0)
        {
            if(*pResultClass)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pResultClass = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"ROLE", Lexer.GetTokenText()) == 0)
        {
            if(*pRole)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pRole = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"RESULTROLE", Lexer.GetTokenText()) == 0)
        {
            if(*pResultRole)
                goto Error;
            if(*pdwQueryType & QUERY_TYPE_GETREFS)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pResultRole = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"ASSOCCLASS", Lexer.GetTokenText()) == 0)
        {
            if(*pAssocClass)
                goto Error;
            if(*pdwQueryType & QUERY_TYPE_GETREFS)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pAssocClass = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"REQUIREDQUALIFIER", Lexer.GetTokenText()) == 0)
        {
            if(*pRequiredQualifier)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pRequiredQualifier = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"REQUIREDASSOCQUALIFIER", Lexer.GetTokenText()) == 0)
        {
            if(*pRequiredAssocQualifier)
                goto Error;
            if(*pdwQueryType & QUERY_TYPE_GETREFS)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_EQU)
                goto Error;
            nTok = Lexer.NextToken();
            if (nTok != QASSOC_TOK_IDENT)
                goto Error;
            *pRequiredAssocQualifier = Macro_CloneLPWSTR(Lexer.GetTokenText());
        }
        else if (wbem_wcsicmp(L"CLASSDEFSONLY", Lexer.GetTokenText()) == 0)
        {
            *pdwQueryType |= QUERY_TYPE_CLASSDEFS_ONLY;
        }
        else if (wbem_wcsicmp(L"KEYSONLY", Lexer.GetTokenText()) == 0)
        {
            *pdwQueryType |= QUERY_TYPE_KEYSONLY;
        }
        else if (wbem_wcsicmp(L"SCHEMAONLY", Lexer.GetTokenText()) == 0)
        {
            *pdwQueryType |= QUERY_TYPE_SCHEMA_ONLY;
        }
        else
        {
            goto Error;
        }
    }


Completed:
    if( (*pdwQueryType & QUERY_TYPE_SCHEMA_ONLY) &&
        (*pdwQueryType & QUERY_TYPE_CLASSDEFS_ONLY))
    {
        goto Error;
    }
    return TRUE;

Error:
    delete *pTargetObj;
    delete *pResultClass;
    delete *pAssocClass;
    delete *pRole;
    delete *pResultRole;
    delete *pRequiredQualifier;
    delete *pRequiredAssocQualifier;

    *pTargetObj = 0;
    *pResultClass = 0;
    *pAssocClass = 0;
    *pRole = 0;
    *pResultRole = 0;
    *pdwQueryType = 0;
    *pRequiredQualifier = 0;
    *pRequiredAssocQualifier = 0;

    return FALSE;
}

//***************************************************************************
//
//***************************************************************************

CAssocQueryParser::CAssocQueryParser()
{
    m_pszQueryText = 0;
    m_pszTargetObjPath = 0;
    m_pszResultClass = 0;
    m_pszAssocClass= 0;
    m_pszRole = 0;
    m_pszResultRole = 0;
    m_pszRequiredQual = 0;
    m_pszRequiredAssocQual = 0;
    m_dwType = 0;
    m_pPathParser = 0;
    m_pPath = 0;
}

//***************************************************************************
//
//***************************************************************************

CAssocQueryParser::~CAssocQueryParser()
{
    delete m_pszQueryText;
    delete m_pszTargetObjPath;
    delete m_pszResultClass;
    delete m_pszAssocClass;
    delete m_pszRole;
    delete m_pszResultRole;
    delete m_pszRequiredQual;
    delete m_pszRequiredAssocQual;

    if (m_pPathParser)
    {
        if (m_pPath)
            m_pPathParser->Free(m_pPath);
        delete m_pPathParser;
    }
}

//***************************************************************************
//
//***************************************************************************
HRESULT CAssocQueryParser::Parse(LPWSTR pszQuery)
{
    if (pszQuery == NULL)
        return WBEM_E_INVALID_QUERY;

    // Clone the query text for debugging.
    // ===================================
    int nLen = wcslen(pszQuery) + 1;
    m_pszQueryText = new wchar_t[nLen];
    if (m_pszQueryText == NULL)
        return WBEM_E_OUT_OF_MEMORY;
    wcscpy(m_pszQueryText, pszQuery);

    // Parse it.
    // =========

    BOOL bRes = ParseAssocQuery(
        m_pszQueryText,
        &m_pszTargetObjPath,
        &m_pszResultClass,
        &m_pszAssocClass,
        &m_pszRole,
        &m_pszResultRole,
        &m_pszRequiredQual,
        &m_pszRequiredAssocQual,
        &m_dwType
        );

    if (bRes == FALSE)
        return WBEM_E_INVALID_QUERY;

    // Parse the object path.
    // ======================

    if (m_pszTargetObjPath)
    {
        m_pPathParser = new CObjectPathParser;
        if (!m_pPathParser)
            return WBEM_E_OUT_OF_MEMORY;
        int nStatus = m_pPathParser->Parse(m_pszTargetObjPath,  &m_pPath);
        if (nStatus != 0)
            return WBEM_E_INVALID_OBJECT_PATH;
    }

    return WBEM_S_NO_ERROR;
}

