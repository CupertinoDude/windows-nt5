// **************************************************************************

// Copyright (c) 1998-2001 Microsoft Corporation, All Rights Reserved
//
// File:  OPATHLEX.H
//
// Description:  
//    Object Path DFA Tokens
//
// History:
//  24-Jun-95       Created.
//  14-Apr-98       Radical update for singly quoted strings, remove
//                  unused tokens, add hex constants, etc.
//
// **************************************************************************

#ifndef _OPATHLEX_H_

#define OPATH_TOK_EOF       0
#define OPATH_TOK_ERROR     1

#define OPATH_TOK_IDENT         100
#define OPATH_TOK_QSTRING       101
#define OPATH_TOK_INT           102
#define OPATH_TOK_HEXINT        103
#define OPATH_TOK_EQ            104

#define OPATH_TOK_DOT           105
#define OPATH_TOK_OPEN_PAREN    106
#define OPATH_TOK_CLOSE_PAREN   107
#define OPATH_TOK_SINGLETON_SYM 108
#define OPATH_TOK_COMMA         109

#define OPATH_TOK_BACKSLASH     110
#define OPATH_TOK_COLON         111

#define OPATH_SINGLETON_STRING  L"@"

extern LexEl OPath_LexTable[];


#endif
