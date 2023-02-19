//
//	%%Title: IMEAPP
//	%%Unit: COM
//	%%Contact: TakeshiF/SeijiA
//	%%Date: 97/06/20
//	%%File: imeapp.h
//
//	Private API / Message service
//

#ifndef __IMEUP_H__
#define __IMEUP_H__

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif /* __cplusplus */

#ifndef RC_INVOKED
#pragma pack(1)         /* Assume byte packing throughout */
#endif /* !RC_INVOKED */

#include "actdict.h"

//// START

/***********************************************************************
    IME Version IDs
 ***********************************************************************/
#define VERSION_ID_JAPANESE                 0x01000000
#define VERSION_ID_KOREAN                   0x02000000
#define VERSION_ID_CHINESE_TRADITIONAL      0x04000000
#define VERSION_ID_CHINESE_SIMPLIFIED       0x08000000

#define VERSION_ID_IMEJP98  (VERSION_ID_JAPANESE | 0x980)
#define VERSION_ID_IMEJP98A (VERSION_ID_JAPANESE | 0x98a)

/***********************************************************************
    Msg:    WM_MSIME_SERVICE
    Desc:   service functions
    Dir:    Apps to IME
    wParam: reserved
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_SERVICE     TEXT("MSIMEService")

//getting version number (wParam)
#define FID_MSIME_VERSION       0

/***********************************************************************
    Msg:    WM_MSIME_UIREADY
    Desc:   service functions
    Dir:    IME to Apps
    wParam: Version ID
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_UIREADY     TEXT("MSIMEUIReady")


//getting class id
#define FID_MSIME_GETCLSID		2

typedef struct _IMECLSID
{
	int			cbSize;		//size of this structure
	CLSID		clsid;		//class id
} IMECLSID;


/***********************************************************************
	Msg:	WM_MSIME_AUTOCOMPLETE
	Desc: 	use IImeActiveDict to support auto-complete
	Owner: 	seijia				
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_AUTOCOMPLETE 		TEXT("MSIMEAutoComplete")

// AutoComplete Version
#define VERSION_AUTOCOMPETE		1

// Dictionary Structure
typedef struct _IMEDICT
{
	int				cbSize;			//size of this structure
	HIMC			hIMC;			//IME context
	IImeActiveDict	*pdict;			//program dictionary
} IMEDICT;

//WParam definition
#define FID_AUTO_VERSION		1
#define FID_AUTO_ACTIVATE		2
#define FID_AUTO_DEACTIVATE		3


/***********************************************************************
	Msg:	WM_MSIME_WORDSTAT
	Desc: 	use IImeActiveDict to get word statistics
	Owner: 	seijia				
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_WORDSTAT 		TEXT("MSIMEWordStatistics")

// WordStat Version
#define VERSION_WORDSTAT		1

// WParam definition
#define FID_STAT_VERSION		1
#define FID_STAT_GIVESTAT		2


/***********************************************************************
	Msg:	WM_MSIME_DOCKDICT
	Desc: 	use IImeActiveDict to dock dictionary
	Owner: 	seijia				
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_DOCKDICT 		TEXT("MSIMEDockActiveDict")

// Dictionary Docking Version
#define VERSION_DOCKDICT		1

// WParam definition
#define FID_DOCK_VERSION		1
#define FID_DOCK_ACTIVATE		2
#define FID_DOCK_DEACTIVATE		3


/***********************************************************************
	Msg:	WM_MSIME_MOUSE
	Desc: 	mouse operation definition                
	Owner: 	kentu				
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_MOUSE 		TEXT("MSIMEMouseOperation")

// Mouse Operation Version (return value of IMEMOUSE_VERSION)
#define VERSION_MOUSE_OPERATION		1

// Mouse operation result
#define IMEMOUSERET_NOTHANDLED		(-1)

//WParam definition for WM_IME_MOUSE.
#define IMEMOUSE_VERSION	0xff	// mouse supported?

#define IMEMOUSE_NONE		0x00	// no mouse button was pushed
#define IMEMOUSE_LDOWN		0x01
#define IMEMOUSE_RDOWN		0x02
#define IMEMOUSE_MDOWN		0x04
#define IMEMOUSE_WUP		0x10	// wheel up
#define IMEMOUSE_WDOWN		0x20	// wheel down


/***********************************************************************
	Msg:	WM_MSIME_RECONVERT/WM_MSIME_RECONVERTREQUEST
	Desc: 	reconversion
	Owner: 	takeshif				
 ***********************************************************************/

// wParam of WM_MSIME_RECONVERTREQUEST
#define FID_RECONVERT_VERSION	0x10000000

// Private reconversion Version
#define VERSION_RECONVERSION		1

// Label for RegisterWindowMessage
#define	RWM_RECONVERT			TEXT("MSIMEReconvert")
#define	RWM_RECONVERTREQUEST	TEXT("MSIMEReconvertRequest")

/***********************************************************************
	Msg:	WM_MSIME_DOCUMENTFEED
	Desc: 	reconversion
	Owner: 	takeshif				
	Usage: SendMessage( hwndApp, WM_MSIME_DOCUMENTFEED, VERSION_DOCUMENTFEED,
				(RECONVERTSTRING*)pReconv );
	wParam: VERSION_DOCUMENTFEED
	lParam: Pointer of RECONVERTSTRING structure
	return: size of RECONVERTSTRING structure
 ***********************************************************************/

// wParam of WM_MSIME_DOCUMENTFEED (set current docfeed version)
#define VERSION_DOCUMENTFEED		1

// lParam is pointer of RECONVERTSTRING structure

// Label for RegisterWindowMessage
#define	RWM_DOCUMENTFEED	TEXT("MSIMEDocumentFeed")

/***********************************************************************
	Msg:	WM_MSIME_QUERYHIDECARET
	Desc: 	composition UI
	Owner: 	takeshif				
	Usage: SendMessage( hwndDefUI, WM_MSIME_QUERYHIDECARET, 0, 0 );
	wParam: reserved
	lParam: reserved
	return: Non-zero = shows caret. Zero = hide caret.
 ***********************************************************************/

// wParam of WM_MSIME_QUERYHIDECARET
#define VERSION_QUERYHIDECARET		1

// Label for RegisterWindowMessage
#define	RWM_QUERYHIDECARET	TEXT("MSIMEQueryHideCaret")

/***********************************************************************
	Msg:	WM_MSIME_QUERYPOSITION
	Desc: 	composition UI
	Owner: 	takeshif				
	Usage: SendMessage( hwndApp, WM_MSIME_QUERYPOSITION, VERSION_QUERYPOSITION, (IMEPOSITION*)pPs );
	wParam: reserved. must be 0.
	lParam: pointer of IMEPOSITION structure
	return: Non-zero = success. Zero = error.
 ***********************************************************************/

// wParam of WM_MSIME_QUERYPOSITION
#define VERSION_QUERYPOSITION		1

// Label for RegisterWindowMessage
#define	RWM_QUERYPOSITION	TEXT("MSIMEQueryPosition")


/***********************************************************************
	Msg:	WM_MSIME_MODEBIAS
	Desc: 	input mode bias
	Owner: 	takeshif
	Usage: SendMessage( hwndDefUI, WM_MSIME_MODEBIAS, MODEBIAS_xxxx, MODEBIASMODE_xxxx );
	wParam: operation of bias
	lParam: bias mode
	return: If wParam is MODEBIAS_GETVERSION,returns version number of interface.
			If wParam is MODEBIAS_SETVALUE : return non-zero value if succeeded. Returns 0 if fail.
			If wParam is MODEBIAS_GETVALUE : returns current bias mode.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define	RWM_MODEBIAS			TEXT("MSIMEModeBias")

// Current version
#define VERSION_MODEBIAS		1

// Set or Get (wParam)
#define MODEBIAS_GETVERSION		0
#define MODEBIAS_SETVALUE		1
#define MODEBIAS_GETVALUE		2

// Bias (lParam)
#define MODEBIASMODE_DEFAULT				0x00000000	// reset all of bias setting
#define MODEBIASMODE_FILENAME				0x00000001	// filename
#define MODEBIASMODE_READING				0x00000002	// reading recommended
#define MODEBIASMODE_DIGIT					0x00000004	// ANSI-Digit Recommended Mode


/***********************************************************************
	Msg:	WM_MSIME_SHOWIMEPAD
	Desc: 	show ImePad
	Owner: 	toshiaK
	Usage: SendMessage( hwndDefUI, WM_MSIME_SHOWIMEPAD, 0, 0 );
	wParam: reserved
	lParam: reserved
	return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define	RWM_SHOWIMEPAD			TEXT("MSIMEShowImePad")


/***********************************************************************
	Msg:	WM_MSIME_KEYMAP
	Desc: 	key map sharing with apps
	Owner: 	HiroakiK
 ***********************************************************************/

// Label for RegisterWindowMessage
#define	RWM_KEYMAP				TEXT("MSIMEKeyMap")
#define	RWM_CHGKEYMAP			TEXT("MSIMEChangeKeyMap")
#define	RWM_NTFYKEYMAP			TEXT("MSIMENotifyKeyMap")


#define FID_MSIME_KMS_VERSION		1
#define FID_MSIME_KMS_INIT			2
#define FID_MSIME_KMS_TERM			3
#define FID_MSIME_KMS_DEL_KEYLIST	4
#define FID_MSIME_KMS_NOTIFY		5
#define FID_MSIME_KMS_GETMAP		6
#define FID_MSIME_KMS_INVOKE		7
#define FID_MSIME_KMS_SETMAP		8
#define FID_MSIME_KMS_FUNCDESC		9


#define IMEKMS_NOCOMPOSITION        0
#define IMEKMS_COMPOSITION          1
#define IMEKMS_SELECTION            2
#define IMEKMS_IMEOFF               3
#define IMEKMS_2NDLEVEL             4
#define IMEKMS_INPTGL               5



typedef struct tagIMEKMSINIT {
	INT			cbSize;
	HWND		hWnd;	// Window which receives notification from IME.
						// If hWnd is NULL, no notification is posted
						// to Input context.
} IMEKMSINIT;


typedef struct tagIMEKMSKEY {
	DWORD dwStatus;		//Shift-Control combination status.
						//Any combination of constants below
						//(defined in IMM.H)
						// 0x0000 (default)
						// MOD_CONTROL     0x0002
						// MOD_SHIFT       0x0004
						// Alt key and Win key is not processed by IME. 
	
	DWORD dwCompStatus;	//Composition string status
						//One of the constants below
						// IMEKMS_NOCOMPOSITION  No composition string      
						// IMEKMS_COMPOSITION    Some composition string
						// IMEKMS_SELECTION      Selection exists in apps
						// IMEKMS_IMEOFF         IME Off state
						// IMEKMS_2NDLEVEL       2nd stage on 2 stroke key operation
						
	
	DWORD dwVKEY;		// VKEY code defined in IMM.H
	union {
		DWORD dwControl;// IME Functionality ID
		DWORD dwNotUsed;
	};
	union {
		WCHAR pwszDscr[31];// The pointer to string of description of this functionalify
		WCHAR pwszNoUse[31];
	};
} IMEKMSKEY;

typedef struct tagIMEKMS {
	INT			cbSize;
	HIMC		hIMC;
	DWORD		cKeyList;
	IMEKMSKEY	*pKeyList;
} IMEKMS;

typedef struct tagIMEKMSNTFY {
	INT			cbSize;
	HIMC		hIMC;
	BOOL		fSelect;
} IMEKMSNTFY;

typedef struct tagIMEKMSKMP {
	INT			cbSize;			//[in] size of this structure
	HIMC		hIMC;			//[in] Input context
	LANGID		idLang;			//[in] Language ID
	WORD		wVKStart;		//[in] VKEY start
	WORD		wVKEnd;			//[in] VKEY end
	INT			cKeyList;		//[out] number of IMEKMSKEY
	IMEKMSKEY	*pKeyList;		//[out] retrieve buffer of IMEKMSKEY
								//      Must be GlobalMemFree by clients
} IMEKMSKMP;

typedef struct tagIMEKMSINVK {
	INT			cbSize;
	HIMC		hIMC;
	DWORD		dwControl;
} IMEKMSINVK;

typedef struct tagIMEKMSFUNCDESC {
	INT			cbSize;			//[in] size of this structure
	LANGID		idLang;			//[in] Language ID
	DWORD		dwControl;		//[in] IME Functionality ID
	WCHAR		pwszDescription[128]; //[out] Description of the functionality
} IMEKMSFUNCDESC;

/***********************************************************************
    Msg:    WM_MSIME_RECONVERTOPTIONS
    Desc:   Set reconversion options
    Usage: SendMessage( hwndDefUI, WM_MSIME_RECONVERTOPTIONS, dwOpt, (LPARAM)(HIMC)hIMC );
    wParam: options
    lParam: Input context handle
    return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_RECONVERTOPTIONS          TEXT("MSIMEReconvertOptions")

//WParam definition for WM_IME_RECONVERTOPTIONS.
#define RECONVOPT_NONE              0x00000000  // default
#define RECONVOPT_USECANCELNOTIFY   0x00000001  // cancel notify

// parameter of ImmGetCompositionString
#define GCSEX_CANCELRECONVERT       0x10000000

//// END

#ifndef RC_INVOKED
#pragma pack()
#endif  /* !RC_INVOKED */

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif	// __cplusplus


#endif // __IMEUP_H__
