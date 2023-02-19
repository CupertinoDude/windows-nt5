//-----------------------------------------------------------------------------
// Microsoft OLE DB Sample Consumer
// (C) Copyright 1995 - 1998 Microsoft Corporation. All Rights Reserved.
//
// @doc
//
// @module OLEDBVER.H | version include file
//
// @rev 1 | 05-09-95 | Peterbu | Created
// @rev 2 | 08-15-95 | Briants | changed from kageravr.h
//

// Constants -----------------------------------------------------------------

#define VER_FILEVERSION  02,00,2905,0
#define VER_FILEVERSION_STR  "02.00.2905.0\0"
#define VER_PRODUCTVERSION  02,00,2905,0
#define VER_PRODUCTVERSION_STR  "02.00.2905.0\0"

#define VER_FILEFLAGSMASK		(VS_FF_DEBUG | VS_FF_PRERELEASE)
#ifdef DEBUG
#define VER_FILEFLAGS			(VS_FF_DEBUG)
#else
#define VER_FILEFLAGS			(0)
#endif

#define VER_FILEOS				VOS_NT_WINDOWS32

#define VER_COMPANYNAME_STR		"Microsoft Corporation\0"
#define VER_PRODUCTNAME_STR		"Microsoft OLE DB\0"
#define VER_LEGALCOPYRIGHT_STR	"Copyright \251 Microsoft Corporation 1995-1998\0"
