/*
 *	@doc INTERNAL
 *
 *	@module _RTFCONV.H -- RichEdit RTF Converter Base Class Definition |
 *
 *	Description:
 *		This file contains the type declarations used by both the RTF reader
 *		and writer for the RICHEDIT control
 *
 *	Authors: <nl>
 *		Original RichEdit 1.0 RTF converter: Anthony Francisco <nl>
 *		Conversion to C++ and RichEdit 2.0:  Murray Sargent
 *
 *	@devnote
 *		sz's in the RTF*.? files usually refer to LPSTRs, not LPWSTRs
 *
 *	Copyright (c) 1995-2002, Microsoft Corporation. All rights reserved.
 */
#ifndef __RTFCONV_H
#define __RTFCONV_H

#include "_edit.h"
#include "_array.h"
#include "_range.h"
#include "_rtext.h"
#include "tokens.h"

extern const KEYWORD rgKeyword[];
extern const KEYWORD rgShapeKeyword[];

#define	LBRACE	TEXT('{')
#define	BSLASH	TEXT('\\')
#define	RBRACE	TEXT('}')
#define ZERO	TEXT('0')

// Character property bits like an ASCII-only ANSI C LC_CTYPE types
#define fUC			0x01		// A-Z
#define	fLC			0x02		// a-z
#define fDG			0x04		// 0-9
#define	fSP			0x08		// Space chars
#define	fPN			0x10		// Punctuation chars
#define fCT			0x20		// Control chars
#define	fBL			0x40		// Blank chars
#define fHX			0x80		// 0-9, a-f, or A-F

#define	fAlpha		(fUC + fLC)
#define	fAlphaNum	(fAlpha + fDG)

extern const BYTE rgbCharClass[256];

#define Classify(_ch)		(rgbCharClass[_ch])
#define IsLC(_ch)			((Classify(_ch) & fLC))
#define IsASCIIAlpha(_ch)	((Classify(_ch) & fAlpha))
#define IsDigit(_ch)		((Classify(_ch) & fDG))
#define IsXDigit(_ch)		((Classify(_ch) & fHX))
#define IsAlphaNum(_ch) 	((Classify(_ch) & fAlphaNum))
#define IsAlphaNumBlank(_ch) ((Classify(_ch) & (fAlphaNum + fBL)))

template <class T> unsigned int DiffPtrs(T *pA, T *pB)
{
	return pA - pB;
}

//#define DiffPtrs(_pA, _pB, _type) ((UINT) (((_type *) (_pA)) - ((_type *) (_pB))))

extern INT  cKeywords;
extern INT  cShapeKeywords;
extern const COLORREF g_Colors[];
extern const char szEndGroupCRLF[];

#define szaCRLF		(BYTE *)&szEndGroupCRLF[1]

/*
 *		Converter Error Codes
 */
enum
{
	ecNoError = 0,						// Success
	ecCantUnicode,
	ecColorTableOverflow,
	ecExpectingRtfKeyword,
	ecExpectingStartGroup,
	ecFontTableOverflow,
	ecGeneralFailure,
	ecKeywordTooLong,
	ecLexInitFailed,
	ecNoMemory,
	ecParserBusy,
	ecPutCharFailed,
	ecStackOverflow,
	ecStackUnderflow,
	ecUnexpectedChar,
	ecUnexpectedEOF,
	ecUnexpectedToken,
	ecUnGetCharFailed,
	ecTextMax,
	ecStreamOutObj,
	ecStreamInObj,
	ecTruncateAtCRLF,
	ecFormatCache,
	ecUTF8NotUsed,
	ecAbort,
	ecLastError							// Total error messages
};

typedef	INT	EC;

/*
 *	@struct RTFOBJECT |
 *		Object data transfer structure
 */
typedef struct _rtfobject
{
	SHORT	sType;				// @field object type (ROT_*)
	SHORT	sPictureType;		// @field specific type of sPicture
	SHORT	cBitsPerPixel;		// @field # bits per pixel, if bitmap
	SHORT	cColorPlanes;		// @field # color planes, if bitmap
	SHORT	cBytesPerLine;		// @field # bytes per raster line, if bitmap
	BOOL	fSetSize;			// @field Let client tell server the size
	LONG	xExt, yExt;			// @field dimensions in pixels for pictures, twips for
								//	for objects
	LONG	xScale, yScale;		// @field scaling percentage along axes
	SHORT	xExtGoal, yExtGoal;	// @field desired dimensions in twips for pictures
	RECT	rectCrop;			// @field cropping information in twips
	WCHAR *	szClass;			// @field object class
	WCHAR *	szName;				// @field object name

	// On RTF generation
	LONG	xExtPict, yExtPict;	// @field metafile dimensions
	LPBYTE	pbResult;			// metafile depiction of the object
	ULONG	cbResult;
} RTFOBJECT;

/*
 *	@enum ROTYPE | The values for OBJECT.sType
 *
 *		Keep this in sync with rgszROT in rtfwrit.cpp
 */
enum ROTYPE
{
	ROT_Bitmap,					// @emem Bitmap
	ROT_Metafile,				// @emem Metafile
	ROT_DIB,					// @emem Device-Independent Bitmap
	ROT_JPEG,					// @emem JPEG blip
	ROT_PNG,					// @emem PNG blip

	ROT_Embedded,				// @emem Embedded Object
	ROT_Link,					// @emem Linked Object
	ROT_AutoLink,				// @emem Autolink
	ROT_MacEdition,				// @emem Mac object

	ROT_EBookImage,				// @emem Ebook Image object
};

/*
 *		DEFINE's
 */
#define cachBufferMost		4096
#define	cachTextMax			( 512 + 1 )
#define	cachKeywordMax		( 32 + 1 )
#define	cachParamMax		( 11 + 1 )
#define cFooChunk			8
#define MAXTABLENEST		16

// Characters to give to RichEdit

#if	( cachTextMax - 1 ) % 2 == 1
#error "cachTextMax  - 1 MUST be even"
#endif

#if ( cachParamMax - 1 ) < 11
#error "cachParamMax MUST be >= 11"
#endif

/*
 * Some RTF defaults
 */
#ifdef NEVER
// we don't care about margins, just indents
#define dxDefaultLeftMargin		1800
#define dxDefaultRightMargin	1800

#else
#define dxDefaultLeftMargin		0
#define dxDefaultRightMargin	0
#endif

// next two in half points
#define	yDefaultFontSize		( 12 * 2 )
#define dyDefaultSuperscript	6

#define RESERVED_FONT_HANDLES	0x800

/*
 *	@struct TEXTFONT |
 *		text font structure
 */
typedef struct _textfont
{
	SHORT		sHandle;				// @field RTF input font handle
	BYTE		iCharRep;				// @field Font character repertoire
	BYTE		bPitchAndFamily;		// @field Font family
	SHORT		iFont;					// @field Font name index
	WCHAR		szName[LF_FACESIZE+1];	// @field Font name
	SHORT		sCodePage;				// @field Code page for font 
										// 			(INVALID_CODEPAGE == not set)
	BYTE		fNameIsDBCS;			// @field Indicates if szName is DBCS stuffed into Unicode buffer
	BYTE		fCpgFromSystem;			// @field Indicates is cpg was 
										// 			retrieved from system based
										//			on font name.
} TEXTFONT;

/*
 *		Global variables for the scope of the entire parser/reader
 */
#ifdef DEBUG
extern CHAR *		rgszParseError[];
extern CHAR *		szDest[];
#endif

#define cchMaxNumText 16

// tagged font info
typedef struct _tfi
{
	WCHAR *szNormalName;
	WCHAR *szTaggedName;
	BYTE iCharRep;
} TFI;

typedef CArray<TEXTFONT> TEXTFONTS;
typedef CArray<COLORREF> COLORREFS;

const short INVALID_CODEPAGE = -1;
const short INVALID_LANGUAGE = -1;

// default value for \ucN tag
const int iUnicodeCChDefault = 1;

/*
 *	CRTFConverter
 *
 *	@class	RTF converter base class used by CRTFRead and CRTFWrite
 */
class CRTFConverter
{
//@access Protected Data Members
protected:
	TEXTFONTS	_fonts;				// @cmember Font table
	COLORREFS	_colors;			// @cmember Color table
	EC			_ecParseError;		// @cmember Error code
	CTxtEdit *	_ped;				// @cmember CTxtEdit
	CTxtRange *	_prg;				// @cmember CTxtRange to replace/write from
	EDITSTREAM *_pes;				// @cmember EDITSTREAM to use
	DWORD		_dwFlags;			// @cmember See #defines below
	CCharFormat	_CF;				// @cmember Character formatting info
	BYTE		_iCharRep;			// @cmember Converter char set (ANSI, UTF7, UTF8)
	char		_bTableLevel;		// @cmember Table level (0 for no table)
	char		_bTableLevelIP;		// @cmember Table level at insertion point

	static TFI *_rgtfi;				// @cmember Pointer to the first font substitute record
	static INT _ctfi;				// @cmember Number of the font substitute records
	static WCHAR *_pchFontSubInfo;	// @cmember Font strings for substitutions

//@access Protected Functions
	void ReadFontSubInfo(void);

	enum PARSEFONTNAME { PFN_SUCCESS, PFN_FAIL, PFN_EOF };
	PARSEFONTNAME ParseFontName(WCHAR *pchBuf,
							WCHAR *pchBufMax,
							WCHAR chDelimiter,
							WCHAR **pszName,
							BYTE &iCharRep,
							BOOL &fSetCharSet,
							WCHAR **ppchBufNew) const;
	BOOL FontSubstitute(WCHAR *szTaggedName, 
						WCHAR *szNormalName, 
						BYTE *piCharRep);
	BOOL FindTaggedFont(const WCHAR *szNormalName, BYTE iCharRep, WCHAR **ppchTaggedName);	 
									// @cmember	Find font name with additional special tag 
									// corresponding to szNormalName & iCharRep
	BOOL IsTaggedFont(const WCHAR *szName, BYTE *piCharRep, WCHAR **ppchNormalName);
								   	// @cmember Figure out is szName font name with additional tag
								   	// corresponding to piCharRep

//@access Public Functions

public:
	CRTFConverter(CTxtRange *prg, EDITSTREAM *pes, DWORD dwFlags, BOOL fRead);
	inline ~CRTFConverter();
	static void FreeFontSubInfo();

protected:
#if defined(DEBUG)
	// for capturing RTF as its read from or written to a file
	HANDLE _hfileCapture;
#endif
};

#define	fRTFNoObjs	1
#define	fRTFFE	8					// Check this

#define IsUTF8	((_dwFlags & (0xFFFF0000 | SF_USECODEPAGE)) \
					 == ((CP_UTF8 << 16) | SF_USECODEPAGE))

/*
 *	CRTFConverter::~CRTFConverter()
 *
 *	@mfunc
 *		RTF Converter destructor
 */
inline CRTFConverter::~CRTFConverter()
{
#if defined(DEBUG)
	if(_hfileCapture)
	{
		CloseHandle(_hfileCapture);
		_hfileCapture = NULL;
	}
#endif
}

#endif // __RTFCONV_H
