/*++



// Copyright (c) 1995-2001 Microsoft Corporation, All Rights Reserved 

Module Name:

  OPATHLEX2.CPP

Abstract:

  Object Path Lexer Map (for use with GENLEX.CPP).

History:

  24-Jun-95       Created.
  14-Apr-98       Radical update for singly quoted strings, remove
                  unused tokens, add hex constants, etc.

--*/

#include "precomp.h"
#include <stdio.h>

#include <genlex.h>
#include <opathlex2.h>             


#define ST_IDENT            24
#define ST_NUMERIC          30
#define ST_DECIMAL          32
#define ST_HEX              34
#define ST_SQ_STRING        38
#define ST_DQ_STRING        42
#define ST_DQ_STRING_ESC    45
#define ST_REFERENCE        48
#define ST_REFERENCE_ESC    51



// DFA State Table for Object Path tokens.
// =======================================

LexEl OPath_LexTable2[] =
{

// State    First   Last        New state,  Return tok,      Instructions
// =======================================================================

    // -------------------------------------------------------------
    // Identifiers
    
/* 0 */  L'A',   L'Z',       ST_IDENT,   0,               GLEX_ACCEPT,
/* 1 */  L'a',   L'z',       ST_IDENT,   0,               GLEX_ACCEPT,
/* 2 */  0x80,   0xfffd,     ST_IDENT,   0,               GLEX_ACCEPT,
/* 3 */  L'_',   GLEX_EMPTY, ST_IDENT,   0,               GLEX_ACCEPT,

    // -------------------------------------------------------------
    // Single symbols
    
/* 4 */  L'.',   GLEX_EMPTY, 0,  OPATH_TOK_DOT,              GLEX_ACCEPT,
/* 5 */  '@',    GLEX_EMPTY, 0,  OPATH_TOK_SINGLETON_SYM,    GLEX_ACCEPT,
/* 6 */  L'=',   GLEX_EMPTY, 0,  OPATH_TOK_EQ,               GLEX_ACCEPT,
/* 7 */  L':',   GLEX_EMPTY, 0,  OPATH_TOK_COLON,            GLEX_ACCEPT,

    // -------------------------------------------------------------
    // Backslashes & forward slashes are interchangeable and reported
    // as OPATH_TOK_BACKSLASH to the parser.
    
/* 8 */ L'\\',  GLEX_EMPTY, 0, OPATH_TOK_BACKSLASH,      GLEX_ACCEPT,
/* 9 */ L'/',   GLEX_EMPTY, 0, OPATH_TOK_BACKSLASH,      GLEX_ACCEPT,

    // -------------------------------------------------------------
    // Simple numeric transition.  The '-' must be followed
    // by decimal digit sequence.  The '0' may be the beginning of a hex
    // or a decimal sequence.
    
/* 10 */ L'1',   L'9',       ST_DECIMAL, 0,               GLEX_ACCEPT,
/* 11 */ L'-',   GLEX_EMPTY, ST_DECIMAL, 0,               GLEX_ACCEPT,
/* 12 */ L'0',   GLEX_EMPTY, ST_NUMERIC, 0,               GLEX_ACCEPT,


    // -------------------------------------------------------------
    // Simple string transition
    //
    // If a single quote begins the string, it must end the string.
    // Escapes are not supported:   'ab', 'a"b'  <not: 'a\'b'>
    //
    // If a double quote begins the string, it must end the string,
    // except that escape+quote is an embedded double quote:
    //
    //     "ab", "a'b", "a\"b"
    // 
        
/* 13 */ L'"',   GLEX_EMPTY, ST_DQ_STRING, 0,               GLEX_CONSUME,
/* 14 */ L'\'',  GLEX_EMPTY, ST_SQ_STRING, 0,               GLEX_CONSUME,


    // -------------------------------------------------------------
    // Whitespace, newlines, etc.  Whitespace is an error.
    
/* 15 */ L' ',   GLEX_EMPTY, 0,  OPATH_TOK_ERROR,  GLEX_ACCEPT|GLEX_RETURN,
/* 16 */ L'\t',  GLEX_EMPTY, 0,  OPATH_TOK_ERROR,  GLEX_ACCEPT|GLEX_RETURN,
/* 17 */ L'\n',  GLEX_EMPTY, 0,  OPATH_TOK_ERROR,  GLEX_ACCEPT|GLEX_RETURN,
/* 18 */ L'\r',  GLEX_EMPTY, 0,  OPATH_TOK_ERROR,  GLEX_ACCEPT|GLEX_RETURN,
/* 19 */ 0,      GLEX_EMPTY, 0,  OPATH_TOK_EOF,   GLEX_CONSUME|GLEX_RETURN, // Note forced return
/* 20 */ L',',   GLEX_EMPTY, 0,  OPATH_TOK_COMMA, GLEX_ACCEPT,
/* 21 */ L'{',   GLEX_EMPTY, ST_REFERENCE, 0,               GLEX_ACCEPT,
/* 22 */ L'{',   GLEX_EMPTY, ST_REFERENCE, 0,               GLEX_ACCEPT,    // Actually reserved for next new

    // All remaining unknown characters

/* 23 */ GLEX_ANY, GLEX_EMPTY, 0,        OPATH_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,

//TODO


    // -------------------------------------------------------------
    // ST_IDENT
    // Accepts C/C++ identifiers, plus any char >= U+0080.

/* 24 */  L'a',   L'z',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 25 */  L'A',   L'Z',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 26 */  L'_',   GLEX_EMPTY, ST_IDENT,   0,          GLEX_ACCEPT,
/* 27 */  L'0',   L'9',       ST_IDENT,   0,          GLEX_ACCEPT,
/* 28 */  0x80,  0xfffd,     ST_IDENT,   0,           GLEX_ACCEPT,
/* 29 */  GLEX_ANY, GLEX_EMPTY,  0,       OPATH_TOK_IDENT,  GLEX_PUSHBACK|GLEX_RETURN,

    // -------------------------------------------------------------
    // ST_NUMERIC
    // 
    // A zero has been recognized.
    // If the next char is 'x' or 'X', we have a valid hex sequence.
    // Otherwise, if '1' to '9' we have a decimal sequence.
    // 

/* 30 */  L'x',  GLEX_EMPTY,  ST_HEX,  0,    GLEX_ACCEPT,
/* 31 */  L'X',  GLEX_EMPTY,  ST_HEX,  0,    GLEX_ACCEPT,


    // -------------------------------------------------------------
    // ST_DECIMAL
    // 

/* 32 */  L'0',     L'9',       ST_DECIMAL, 0,              GLEX_ACCEPT,
/* 33 */  GLEX_ANY, GLEX_EMPTY, 0,          OPATH_TOK_INT,  GLEX_PUSHBACK|GLEX_RETURN,

    // -------------------------------------------------------------
    // ST_HEX
    //
    // Recognizes a valid hex sequence.

/* 34 */  L'a',     L'f',       ST_HEX,     0,                 GLEX_ACCEPT,
/* 35 */  L'A',     L'F',       ST_HEX,     0,                 GLEX_ACCEPT,
/* 36 */  L'0',     L'9',       ST_HEX,     0,                 GLEX_ACCEPT,
/* 37 */  GLEX_ANY, GLEX_EMPTY, 0,          OPATH_TOK_HEXINT,  GLEX_PUSHBACK|GLEX_RETURN,
    
    // -------------------------------------------------------------
    // ST_SQ_STRING : Single quoted strings
    //
    // If a single quote begins the string, it must end the string.
    // Escapes are not supported:   'ab', 'a"b'  <not: 'a\'b'>

/* 38 */   L'\n', GLEX_EMPTY, 0,  OPATH_TOK_ERROR,      GLEX_ACCEPT|GLEX_LINEFEED,
/* 39 */   L'\r', GLEX_EMPTY, 0,  OPATH_TOK_ERROR,      GLEX_ACCEPT|GLEX_LINEFEED,
/* 40 */   L'\'',  GLEX_EMPTY, 0,  OPATH_TOK_QSTRING,   GLEX_CONSUME,
/* 41 */   GLEX_ANY, GLEX_EMPTY, ST_SQ_STRING,  0,      GLEX_ACCEPT,

    // -------------------------------------------------------------
    // ST_DQ_STRING
    //
    // If a double quote begins the string, it must end the string,
    // except that escape+quote is an embedded double quote:
    //
    //     "ab", "a'b", "a\"b"
    // 

/* 42 */   L'\\', GLEX_EMPTY, ST_DQ_STRING_ESC, 0,                  GLEX_CONSUME,    // Escape
/* 43 */   L'"',  GLEX_EMPTY, 0,                OPATH_TOK_QSTRING,  GLEX_CONSUME,
/* 44 */   L'"',  GLEX_EMPTY, ST_DQ_STRING,     0,                  GLEX_ACCEPT|GLEX_NOT, 
                    // Accept anything but a quote

    //-------------------------------------------------------------
    // ST_DQ_STRING_ESC
    //
    // An escape occurred while in a string.
    // Either an embedded slash or embedded quote must have occurred.
    // Otherwise, an unsupported escape has occurred and we fail.

/* 45  */    L'\\', GLEX_EMPTY, ST_DQ_STRING, 0,  GLEX_ACCEPT,
/* 46  */    L'"',  GLEX_EMPTY, ST_DQ_STRING, 0,  GLEX_ACCEPT,
/* 47  */ GLEX_ANY, GLEX_EMPTY, 0,        OPATH_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,

    // -------------------------------------------------------------
    // ST_DQ_REFERENCE
    //
    // A reference begins with '{' and ends with '}'.  Embedded
    // '}' are allowed if preceeded by '\':

/* 48 */   L'\\', GLEX_EMPTY, ST_REFERENCE_ESC, 0,                     GLEX_CONSUME,    // Escape
/* 49 */   L'}',  GLEX_EMPTY, 0,                OPATH_TOK_REFERENCE,   GLEX_ACCEPT,
/* 50 */   L'}',  GLEX_EMPTY, ST_REFERENCE,     0,                     GLEX_ACCEPT|GLEX_NOT, 
                    // Accept anything but a quote

    //-------------------------------------------------------------
    // ST_DQ_REFERENCE_ESC
    //
    // An escape occurred while in a string.
    // Either an embedded slash or embedded quote must have occurred.
    // Otherwise, an unsupported escape has occurred and we fail.

/* 51  */    L'\\', GLEX_EMPTY, ST_REFERENCE, 0,  GLEX_ACCEPT,
/* 52  */    L'}',  GLEX_EMPTY, ST_REFERENCE, 0,  GLEX_ACCEPT,
/* 53  */ GLEX_ANY, GLEX_EMPTY, 0,        OPATH_TOK_ERROR, GLEX_ACCEPT|GLEX_RETURN,

};


