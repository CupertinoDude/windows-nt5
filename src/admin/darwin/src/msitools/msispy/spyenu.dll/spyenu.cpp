#if 0
MODULENAME = spyenu
DESCRIPTION = MsiSpy English UI
FILEVERSION = MSI
SUBSYSTEM = RESOURCE
DEPEND = resource.h
!include "..\MsiTool.mak"
!if 0  #nmake skips the rest of this file
#endif // end of makefile definitions

//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1997 - 1998
//
//--------------------------------------------------------------------------


#ifndef RC_INVOKED    // start of source code
#else // RC_INVOKED, end of source code, start of resources
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#define APSTUDIO_HIDDEN_SYMBOLS
#include "windows.h"
#undef APSTUDIO_HIDDEN_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_MSISPY              ICON    DISCARDABLE     "msispy.ico"
IDI_ABSENTFE            ICON    DISCARDABLE     "absentfe.ico"
IDI_BROKENCO            ICON    DISCARDABLE     "brokenco.ico"
IDI_BROKENFE            ICON    DISCARDABLE     "brokenfe.ico"
IDI_BROKENPR            ICON    DISCARDABLE     "brokenpr.ico"
IDI_COMPONENT           ICON    DISCARDABLE     "componen.ico"
IDI_COMPUTER            ICON    DISCARDABLE     "computer.ico"
IDI_FEATURE             ICON    DISCARDABLE     "feature.ico"
IDI_PRODUCT             ICON    DISCARDABLE     "product.ico"
IDI_BROKENFL            ICON    DISCARDABLE     "brokenfl.ico"
IDI_FILE                ICON    DISCARDABLE     "file.ico"
IDI_ABSENTCO            ICON    DISCARDABLE     "absentco.ico"


/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//


IDD_ABOUT DIALOGEX 0, 0, 254, 170
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "About Msispy"
FONT 6, "MS Sans Serif"
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,196,126,49,14
    LTEXT           "",IDT_ABT_APPNAME,57,20,178,11
    LTEXT           "Copyright 2000 Microsoft Corporation",-1,57,43,120,8
    LTEXT           "",IDT_ABT_USERINFO,57,79,188,34,0,WS_EX_CLIENTEDGE
    LTEXT           "This product is licensed to",-2,57,68,107,8
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,7,119,238,1
    CONTROL         IDB_ABOUT,-1,"Static",SS_BITMAP,7,20,42,92
    CONTROL         IDB_COPYRIGHT,-1,"Static",SS_BITMAP,7,122,177,38
END


IDD_PREFERENCES1 DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Preferences"
FONT 8, "MS Sans Serif"
BEGIN
    CONTROL         "Normal",IDC_MODENORMAL,"Button",BS_AUTORADIOBUTTON,15,35,59,
                    8
    CONTROL         "Diagnostic",IDC_MODEDIAGNOSTIC,"Button",BS_AUTORADIOBUTTON,15,
                    50,59,8
    CONTROL         "Restricted",IDC_MODERESTRICTED,"Button",BS_AUTORADIOBUTTON,15,
                    65,59,8
    LTEXT           "",IDC_MODEHELP,15,85,190,60
    GROUPBOX        "Mode",-1,10,20,200,130
//    CONTROL         "Save Settings on Exit",IDC_SAVESETTINGS,"Button",
//                    BS_AUTOCHECKBOX | WS_TABSTOP,13,175,198,16
END

IDD_PREFERENCES2 DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Msispy Language Preferences"
FONT 8, "MS Sans Serif"
BEGIN
    CONTROL         "",IDC_LANGUAGELIST,"SysListView32",WS_BORDER | WS_TABSTOP,10,
                    55,205,75,WS_EX_CLIENTEDGE
    LTEXT           "Msispy can be configured to run in any of these \
languages on this machine. If you would like to change the current \
language, click on the desired language and click OK:",-1,10,26,206,25
    LTEXT           "Current Selection:",-1,10,142,56,8
    EDITTEXT        IDC_CURRLANGUAGE,70,142,140,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP
END

/*

IDD_MISSINGCOMP DIALOGEX 0, 0, 221, 177
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Msispy: Missing Features"
FONT 8, "MS Sans Serif"
BEGIN
    CONTROL         302,-1,"Static",SS_BITMAP,13,13,42,92
    DEFPUSHBUTTON   "OK",IDOK,160,131,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,160,150,50,14
    LTEXT           "The following Msispy feature(s) are missing or corrupted on this machine:",
                    -1,67,13,137,16
    CONTROL         "",IDC_COMPLIST,"SysListView32",WS_BORDER | 
                    WS_TABSTOP,67,30,137,20,WS_EX_CLIENTEDGE
    LTEXT           "Would you like Msispy to attempt to fix itself?",-1,67,
                    58,144,8
    CONTROL         "Yes",IDC_YUP,"Button",BS_AUTORADIOBUTTON,67,71,92,11
    CONTROL         "No",IDC_NOPE,"Button",BS_AUTORADIOBUTTON,67,84,42,8
    GROUPBOX        "",-1,7,109,147,57
    LTEXT           "",IDC_HELPLINE1,13,116,134,18
    EDITTEXT        IDC_INSTSRC,13,133,134,8,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    LTEXT           "",IDC_HELPLINE2,13,146,134,18
END
*/

IDD_PF_REINSTALL DIALOG DISCARDABLE  0, 0, 246, 194
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Re-install Product or Feature"
FONT 8, "MS Sans Serif"
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,178,6,54,14
    PUSHBUTTON      "Cancel",IDCANCEL,178,23,54,14
    PUSHBUTTON      "Advanced >>",IDC_PF_RI_ADVANCED,178,40,54,14

    LTEXT           "Select Re-install Mode:",-1,7,13,73,14
    LTEXT           "Additonal Re-install Modes:",-1,7,80,120,14

    CONTROL         "Repair all detected reinstall problems",IDC_PF_RI_FILEVERIFY,
                    "Button",BS_AUTORADIOBUTTON,19,28,150,13
    CONTROL         "Reinstall only if file is missing",IDC_PF_RI_FILEMISSING,
                    "Button",BS_AUTORADIOBUTTON,19,41,150,13
    CONTROL         "Force all files to be reinstalled",IDC_PF_RI_FILEREPLACE,
                    "Button",BS_AUTORADIOBUTTON,19,54,150,13

    CONTROL         "Reinstall if file is missing, or an older version exists",
                    IDC_PF_RI_FILEOLDER,"Button",BS_AUTORADIOBUTTON,19,95,208,13
    CONTROL         "Reinstall if file is missing, or an older or equal version exists",
                    IDC_PF_RI_FILEEQUAL,"Button",BS_AUTORADIOBUTTON,19,108,208,13
    CONTROL         "Reinstall if existing file has different version",
                    IDC_PF_RI_FILEEXACT,"Button",BS_AUTORADIOBUTTON,19,121,208,13
    CONTROL         "Verify that required user registry entries are present",
                    IDC_PF_RI_USERDATA,"Button",BS_AUTORADIOBUTTON,19,134,208,13
    CONTROL         "Verify that required local machine registry entries are present",
                    IDC_PF_RI_MACHINEDATA,"Button",BS_AUTORADIOBUTTON,19,147,208,13
    CONTROL         "Recreate all shortcuts",IDC_PF_RI_SHORTCUT,"Button",
                    BS_AUTORADIOBUTTON,19,160,208,13
    CONTROL         "Validate advertised shortcuts and icons",IDC_PF_RI_ADVERTISE,
                    "Button",BS_AUTORADIOBUTTON,19,173,208,13
END


IDD_P_CONFIGURE DIALOG DISCARDABLE  0, 0, 225, 230
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Install or Configure Product"
FONT 8, "MS Sans Serif"
BEGIN
    DEFPUSHBUTTON   "Proceed",IDOK,107,206,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,165,206,50,14
    LTEXT           "This will attempt to install or configure the selected product to the settings you specify. \
To complete this operation, you may need the source from which the selected product was installed.",
                    -1,9,18,190,34
    GROUPBOX        "Product Settings",-1,5,59,210,140
    LTEXT           "Please select the new installation state:",-1,13,74,177,11
    CONTROL         "Default (Files will be installed to their default location)",
                    IDC_P_RC_DEFAULT,"Button",BS_AUTORADIOBUTTON,20,86,181,10
    CONTROL         "Local (Files will be installed on the local machine)",
                    IDC_P_RC_LOCAL,"Button",BS_AUTORADIOBUTTON,20,98,181,10
    CONTROL         "Source (Files will be run from the installation source)",
                    IDC_P_RC_SOURCE,"Button",BS_AUTORADIOBUTTON,20,110,181,10
    CONTROL         "On Demand (Files will be installed when needed)",
                    IDC_P_RC_ADVERTISE,"Button",BS_AUTORADIOBUTTON,20,122,181,10
    LTEXT           "Please select the new installation level:",-1,13,144,177,11
    CONTROL         "Minimum (Only the essential features will be installed)",
                    IDC_P_RC_MINIMUM,"Button",BS_AUTORADIOBUTTON,20,156,193,10
    CONTROL         "Typical (Most commonly used features will be installed)",
                    IDC_P_RC_TYPICAL,"Button",BS_AUTORADIOBUTTON,20,168,193,10
    CONTROL         "Complete (All of the program's features will be installed)",
                    IDC_P_RC_COMPLETE,"Button",BS_AUTORADIOBUTTON,20,180,193,10
END

IDD_F_CONFIGURE DIALOG DISCARDABLE  0, 0, 225, 172
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Install or Configure Feature"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "This will attempt to install or configure the selected feature to the settings you specify. \
To complete this operation, you may need the source from which the selected feature was installed.",
                    -1,9,18,190,34
    GROUPBOX        "Feature Settings",-1,5,59,210,82
    LTEXT           "Please select the new installation state:",-1,13,74,155,11
    CONTROL         "Default (Files will be installed to their default location)",
                    IDC_F_RC_DEFAULT,"Button",BS_AUTORADIOBUTTON,20,86,181,10
    CONTROL         "Local (Files will be installed on the local machine)",
                    IDC_F_RC_LOCAL,"Button",BS_AUTORADIOBUTTON,20,98,181,10
    CONTROL         "Source (Files will be run from the installation source)",
                    IDC_F_RC_SOURCE,"Button",BS_AUTORADIOBUTTON,20,110,181,10
    CONTROL         "On Demand (Files will be installed when needed)",
                    IDC_F_RC_ADVERTISE,"Button",BS_AUTORADIOBUTTON,20,122,181,10
    DEFPUSHBUTTON   "Proceed",IDOK,107,148,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,165,148,50,14
END


IDD_C_PROPERTIES DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Component Properties"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Name:",-1,10,26,30,12
    LTEXT           "GUID:",-1,10,61,30,12
    LTEXT           "Status:",-1,10,74,30,12
    LTEXT           "Location:",-1,10,87,30,12

    EDITTEXT        IDT_C_NAME,45,26,162,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_C_GUID,45,61,162,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_C_STATUS,45,74,162,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_C_LOCATION,45,87,162,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,51,206,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,130,206,1
END

IDD_PFC_FILELIST DIALOGEX 0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "File List"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Name:",-1,10,21,30,12
    EDITTEXT        IDT_FLIST_PFC_NAME,45,21,162,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,35,205,1
    LTEXT           "Files:",-1,11,43,59,10
    CONTROL         "",IDC_FILELIST,"SysListView32",WS_BORDER | WS_TABSTOP,
                    10,57,205,143
    PUSHBUTTON      "Verify Files ...",IDC_VERIFYFILELIST,10,205,65,14
    CONTROL         "Include sub-features",IDC_SUBFEATURES,"Button",
                    BS_AUTOCHECKBOX | NOT WS_VISIBLE | WS_TABSTOP,135,42,80,9
    EDITTEXT        IDT_FLIST_TOTALFILES,103,205,112,8,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT | WS_EX_RIGHT
    EDITTEXT        IDT_FLIST_TOTALSIZE,103,214,112,8,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT | WS_EX_RIGHT
END

IDD_F_PROPERTIES DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Feature Properties"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Name:",-1,10,26,40,8
    LTEXT           "Title:",-1,10,61,40,8
    LTEXT           "Parent:",-1,10,74,40,8
    LTEXT           "Description:",-1,10,87,40,8
    LTEXT           "Usage Count:",-1,10,140,45,8
    LTEXT           "Last Used:",-1,10,153,45,8
    LTEXT           "Status:",-1,10,166,45,8

    EDITTEXT        IDT_F_NAME,54,26,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_TITLE,54,61,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_PARENT,54,74,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_DESCRIPTION,54,87,158,40,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP|ES_MULTILINE,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_USAGE_COUNT,58,140,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_LASTUSED_DATE,58,153,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_F_STATUS,58,166,158,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT

    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,51,206,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,130,206,1
END


IDD_P_PROPERTIES1 DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Product Properties 1"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Name:",-1,10,26,50,8
    LTEXT           "Version:",-1,10,61,50,8
    LTEXT           "Publisher:",-1,10,74,50,8
    LTEXT           "Product Code:",-1,10,87,50,8
    LTEXT           "Local Package:",-1,10,100,50,8
    LTEXT           "Registered To:",-1,10,135,50,8
    LTEXT           "Product ID:",-1,10,160,50,8
    LTEXT           "Status:",-1,10,174,50,8

    EDITTEXT        IDT_P1_NAME,63,26,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_VERSION,63,61,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_PUBLISHER,63,74,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_PRODUCTCODE,63,87,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_LOCALPACKAGE,63,100,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_USERNAME,63,135,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_USERORG,63,145,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_PRODUCT_ID,63,160,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P1_STATUS,63,174,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT

    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,51,206,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,125,206,1
END

IDD_P_PROPERTIES2 DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Product Properties 2"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Name:",-1,10,26,50,8
    LTEXT           "Help Link:",-1,10,61,50,8
    LTEXT           "Information:",-1,10,74,50,8
    LTEXT           "Updates:",-1,10,87,50,8
    LTEXT           "Help Line:",-1,10,100,50,8
    LTEXT           "Installed on:",-1,10,135,50,8
    LTEXT           "Installed from:",-1,10,148,50,8
    LTEXT           "Installed to:",-1,10,161,50,8
    LTEXT           "Language:",-1,10,174,50,8

    EDITTEXT        IDT_P2_NAME,63,26,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_HELPLINK_URL,63,61,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_INFO_URL,63,74,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_UPDATES_URL,68,87,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_HELPPHONE,63,100,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_INSTALL_DATE,63,135,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_INSTALL_SOURCE,63,148,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_INSTALL_TO,63,161,150,12,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_P2_LANGUAGE,63,174,150,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,51,206,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,125,206,1
END
//    LTEXT           "Company:",-1,10,74,50,8
//    LTEXT           "",IDT_USERCOMP,63,74,150,8


IDD_MSI_PROPERTIES1 DIALOGEX 0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Windows installer Package Properties Page1"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Title:",-1,10,26,50,8
    LTEXT           "Subject:",-1,10,51,50,8
    LTEXT           "Author:",-1,10,64,50,8
    LTEXT           "Keywords:",-1,10,77,50,8
    LTEXT           "Comments:",-1,10,90,50,8
    LTEXT           "Template:",-1,10,114,50,8
    LTEXT           "Codepage:",-1,10,127,50,8
    LTEXT           "Dictionary:",-1,10,140,50,8
    LTEXT           "Application:",-1,10,167,50,8
    LTEXT           "Security:",-1,10,180,50,8
    LTEXT           "Thumbnail:",-1,10,193,50,8

    EDITTEXT        IDT_MSI1_TITLE,63,26,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_SUBJECT,63,51,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_AUTHOR,63,64,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_KEYWORDS,63,77,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_COMMENTS,63,90,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_TEMPLATE,63,114,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_CODEPAGE,63,127,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_DICTIONARY,63,140,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_APPLICATION,63,167,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_SECURITY,63,180,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI1_THUMBNAIL,63,193,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,42,205,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,103,205,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,156,204,1
END

IDD_MSI_PROPERTIES2 DIALOGEX 0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Database Properties 2"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Title:",-1,10,26,50,8
    LTEXT           "Location:",-1,10,51,50,8
    LTEXT           "Last Saved:",-1,10,64,50,8
    LTEXT           "Revision:",-1,10,77,50,8
    LTEXT           "Saved By:",-1,10,90,50,8
    LTEXT           "Created:",-1,10,114,50,8
    LTEXT           "Edit Time:",-1,10,127,50,8
    LTEXT           "Printed:",-1,10,140,50,8
    LTEXT           "MSI Version Used:",-1,10,167,60,8
    LTEXT           "MSI Source Type:",-1,10,180,59,8
    LTEXT           "MSI Transform:",-1,10,193,59,8
    EDITTEXT        IDT_MSI1_TITLE,63,26,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_LOCATION,63,51,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_LASTSAVED_ON,63,64,136,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_REVISION_NUM,63,77,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_LASTSAVED_BY,63,90,150,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_CREATED_ON,63,114,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_EDITTIME,63,127,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_LASTPRINTED,63,140,150,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_MSI_VERSION,77,167,136,12,ES_AUTOHSCROLL | ES_READONLY | 
                    NOT WS_BORDER | NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_MSI_SOURCETYPE,77,180,136,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    EDITTEXT        IDT_MSI2_MSI_TRANSFORM,77,193,136,12,ES_AUTOHSCROLL | 
                    ES_READONLY | NOT WS_BORDER | NOT WS_TABSTOP,
                    WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,42,205,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,103,205,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,156,204,1
END



IDD_SS_PROPERTIES DIALOG DISCARDABLE  0, 0, 226, 226
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Saved State File Properties"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Location:",-1,10,26,40,8
    LTEXT           "Saved By:",-1,10,61,40,8
    LTEXT           "Saved On:",-1,10,74,40,8
    LTEXT           "OS Name:",-1,10,135,50,8
    LTEXT           "OS Version:",-1,10,148,50,8
    LTEXT           "OS Build:",-1,10,161,50,8
    LTEXT           "OS Info:",-1,10,174,50,8

    EDITTEXT        IDT_SS_LOCATION,55,26,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_SS_SAVED_BY,55,61,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_SS_SAVED_ON,55,74,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT

    EDITTEXT        IDT_SS_OS_NAME,55,135,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_SS_OS_VERSION,55,148,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_SS_OS_BUILD,55,161,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    EDITTEXT        IDT_SS_OS_INFO,55,174,162,8,ES_AUTOHSCROLL|ES_READONLY|NOT WS_BORDER|NOT WS_TABSTOP,WS_EX_TRANSPARENT
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,51,206,1
    CONTROL         "",-1,"Static",SS_ETCHEDHORZ | SS_SUNKEN,10,130,206,1
END


/////////////////////////////////////////////////////////////////////////////
//
// Menu
//

IDR_MSISPYMENU MENU DISCARDABLE 
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "View &Windows installer Package...\tCtrl+W",      IDM_FL_OPEN_MSI_PACKAGE
        MENUITEM "View &Installed Products \tCtrl+I",				IDM_FL_OPEN_LOCAL_SYS
        MENUITEM SEPARATOR
        MENUITEM "View &Msispy Saved State...\tCtrl+M",				IDM_FL_LOAD_SAVED_STATE
        MENUITEM "&Save Current State...\tCtrl+S",					IDM_FL_SAVE_CURRENT_STATE
        MENUITEM "&Restore Saved State\tCtrl+R", 		            IDM_FL_RESTORE_STATE, GRAYED
        MENUITEM "&Log Differences...\tCtrl+L",        				IDM_FL_CHECK_DIFFERENCES, GRAYED
        MENUITEM SEPARATOR
        MENUITEM "E&xit",                          					IDM_FL_EXIT
    END
    POPUP "&View"
    BEGIN
        MENUITEM "&Refresh\tF5",                      IDM_VW_REFRESH
        MENUITEM SEPARATOR
        MENUITEM "&Preferences...",                   IDM_VW_PREFERENCES
    END
//  POPUP "&Options"
//  BEGIN
//        MENUITEM "&Diagnostic Mode",                IDM_DIAGNOSTIC
//    END
    POPUP "&Help"
    BEGIN
		MENUITEM "&Index\tF1"						  IDM_HLP_INDEX
        MENUITEM "&About Msispy...",                  IDM_HLP_ABOUT
    END
END

IDR_LVPOPMENU MENU DISCARDABLE 
BEGIN
    POPUP "&Options"
    BEGIN
        MENUITEM "&Properties...",                        IDM_C_SHOW_PROPERTIES
    END
END

IDR_TVPOPMENU MENU DISCARDABLE 
BEGIN
    POPUP "&Options"
    BEGIN
        MENUITEM "&Properties...",                        IDM_PF_SHOW_PROPERTIES
    END
END


/////////////////////////////////////////////////////////////////////////////
//
// Accelerator
//

IDR_ACCEL ACCELERATORS DISCARDABLE 
BEGIN
    VK_F5,          IDM_VW_REFRESH,						VIRTKEY, NOINVERT
    VK_F1,          IDM_HLP_INDEX,						VIRTKEY, NOINVERT
    "W",            IDM_FL_OPEN_MSI_PACKAGE,			VIRTKEY, CONTROL, NOINVERT
    "I",            IDM_FL_OPEN_LOCAL_SYS,				VIRTKEY, CONTROL, NOINVERT
    "M",            IDM_FL_LOAD_SAVED_STATE,			VIRTKEY, CONTROL, NOINVERT
    "S",            IDM_FL_SAVE_CURRENT_STATE,			VIRTKEY, CONTROL, NOINVERT
    "R",            IDM_FL_RESTORE_STATE,				VIRTKEY, CONTROL, NOINVERT
    "L",            IDM_FL_CHECK_DIFFERENCES,			VIRTKEY, CONTROL, NOINVERT
END


/////////////////////////////////////////////////////////////////////////////
//
// Cursor
//

IDC_SPLIT               CURSOR  DISCARDABLE     "split.cur"



/////////////////////////////////////////////////////////////////////////////
//
// Bitmap
//
IDB_ABOUT               BITMAP  DISCARDABLE     "spyabout.bmp"
IDB_COPYRIGHT           BITMAP  DISCARDABLE     "copyrit.bmp"


/////////////////////////////////////////////////////////////////////////////
//
// String Table
//



STRINGTABLE DISCARDABLE 
BEGIN
    IDS_CLASSNAME          "Msispy"
    IDS_APPNAME            "Msispy"
    IDS_FATALINITERRMSG    "Msispy could not be initialized\nPlease click on OK to quit"
    IDS_FATALERRCAPTION    "Fatal Error"
    IDS_NULLSTRING         ""
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_TV_ROOT						"My Computer"
    IDS_SB_NOTHING_SELECTED			"No product or feature selected"
    IDS_SB_NO_COMPONENTS			"No components to display"
    IDS_SB_ONE_COMPONENT			"1 component"
    IDS_SB_PF_SELECTED				"%s selected"
    IDS_SB_MANY_COMPONENTS			"%d components"
	//changed
    IDS_MSI_PACKAGE					"MSI Package: '%s'"
    IDS_SAVED_STATE					"Saved State: '%s'"
	IDS_RESTORE_SS					"Restore Saved State"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_P_PROPERTIES_DEFAULT		""
    IDS_P_PROPERTIES_HEADER1		"General"
    IDS_P_PROPERTIES_HEADER2		"More Info"
    IDS_P_PROPERTIES_CAPTION		"Product Properties"

    IDS_SS_PROPERTIES_HEADER		"General"
    IDS_SS_PROPERTIES_CAPTION		"Saved State File Properties"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_F_PROPERTIES_DEFAULT		""
    IDS_F_PROPERTIES_HEADER			"General"
    IDS_F_PROPERTIES_CAPTION		"Feature Properties"
    IDS_PF_REINSTALL				"Re-install '%s'..."
    IDS_PF_CONFIGURE				"Configure '%s'..."
    IDS_PF_UNINSTALL				"Uninstall '%s'"
    IDS_PF_INSTALL					"Install '%s'..."
    IDS_PF_ADVERTISE				"Advertise '%s'"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_C_PROPERTIES_DEFAULT		""
    IDS_C_PROPERTIES_HEADER1		"General"
    IDS_C_PROPERTIES_CAPTION		"Component Properties"
    IDS_C_NAME_HEADER				"Component Name"
    IDS_C_STATUS_HEADER				"Component Status"
    IDS_C_PATH_HEADER				"Component Location"
    IDS_C_GUID_HEADER				"Component Id"
    IDS_C_REINSTALL					"Re-install '%s'..."
    IDS_C_PROPERTIES_HEADER2		"File List"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_MSI_OPEN_FILTER				"Windows installer Packages (*.msi)\0*.msi\0All Files (*.*)\0*.*\0\0"
    IDS_MSI_OPEN_CAPTION			"View Windows installer Package"
    IDS_MSI_OPEN_DEFAULT_EXTN		"msi"
    IDS_MSI_OPEN_ERROR_MESSAGE		"The file '%s' is not a valid Windows installer package. \
Please check the file name and path and try again."
    IDS_MSI_OPEN_ERROR_CAPTION		"Error Opening Windows installer Package"
END


STRINGTABLE DISCARDABLE 
BEGIN
    IDS_MSI_PROPERTIES_DEFAULT		""
    IDS_MSI_PROPERTIES_HEADER1		"General"
    IDS_MSI_PROPERTIES_HEADER2		"More Info"
    IDS_MSI_PROPERTIES_CAPTION		"Windows installer Package Properties"

END


STRINGTABLE DISCARDABLE 
BEGIN
    IDS_SS_OPEN_FILTER				"Msispy Saved States (*.mss)\0*.mss\0All files (*.*)\0*.*\0\0"
    IDS_SS_OPEN_CAPTION				"View Msispy Saved State"
    IDS_SS_OPEN_DEFAULT_EXTN		"mss"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_SS_SAVE_FILTER				"Msispy Saved States (*.mss)\0*.mss\0All files (*.*)\0*.*\0\0"
    IDS_SS_SAVE_CAPTION				"Save Current State"
    IDS_SS_SAVE_DEFAULT_EXTN		"mss"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_CHKDIFF_SAVE_FILTER			"Msispy Difference Logs (*.log)\0*.log\0All files (*.*)\0*.*\0\0"
    IDS_CHKDIFF_SAVE_CAPTION		"Log Differences"
    IDS_CHKDIFF_SAVE_DEFAULT_EXTN	"log"
END


STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FILELISTHDR					"File List"
    IDS_FILELISTHDR_COMPONENT_NAME	"Component"
    IDS_FILELISTHDR_FILE_TITLE		"File Title"
    IDS_FILELISTHDR_FILE_PATH		"Location"
    IDS_FILELISTHDR_EXPECTED_SIZE	"Install Size"
    IDS_FILELISTHDR_CURRENT_SIZE	"Current Size"
    IDS_FILELISTHDR_CREATED			"File Created"
    IDS_FILELISTHDR_LASTWRITE		"Last Modified"
    IDS_FILELISTHDR_LASTACCESS		"Last Accessed"

    IDS_FILELIST_ERROR_MESSAGE		"Could not create window\n Out of Memory?"
    IDS_FILELIST_ERROR_CAPTION		"Error"
	IDS_FILEVERIFY_CAPTION			"File Verify Results ..."
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FILE						"%d File"
    IDS_FILES						"%d Files"
    IDS_BYTE						"%d Byte"
    IDS_BYTES						"%d Bytes"
    IDS_MBYTE						"%d MB"
    IDS_KBYTE						"%d KB"
END


STRINGTABLE DISCARDABLE 
BEGIN
	IDS_IS_NOTUSED			"Disabled"
	IDS_IS_BADCONFIG		"Configuration data corrupt"
	IDS_IS_INCOMPLETE		"Installation suspended or in progress"
	IDS_IS_SOURCEABSENT		"Source at remote location is absent"
	IDS_IS_MOREDATA			"Return buffer overflow"
	IDS_IS_INVALIDARG		"Invalid function argument"
	IDS_IS_UNKNOWN			"Unrecognized"
	IDS_IS_BROKEN			"Broken"
	IDS_IS_ADVERTISED		"Advertised on local system"
	IDS_IS_ABSENT			"Not installed"
	IDS_IS_LOCAL			"Installed on local drive"
	IDS_IS_SOURCE			"Installed to run from source"
	IDS_IS_DEFAULT			"Installed at default location"
	IDS_IS_UNDETERMINED		"Could not be determined"
// LOCALISE::
	IDS_IS_UNKNOWNPRODUCT	"Unrecognized product"
	IDS_IS_UNKNOWNFEATURE	"Unrecognized feature"
	IDS_IS_UNKNOWNCOMPONENT	"Unrecognized component"
	IDS_IS_SOMEFILESMISSING	"Some files modified or missing"
// endLOCALISE
END
			
STRINGTABLE DISCARDABLE 
BEGIN
	IDS_PF_RI_ADVANCED				"Advanced <<"
	IDS_PF_RI_NOADVANCED			"Advanced >>"
	IDS_DIAGNOSTICMODE_CAPTION		"Diagnostic Mode"
	IDS_DIAGNOSTICMODE_MESSAGE		"Msispy will now switch to the diagnostic mode.\n\n\
In this mode, Msispy will perform a complete check of all files to determine\n\
component status, instead of relying on the component key file. This mode\n\
will give a more accurate picture of the component status, but will make the\n\
program significantly slower. Msispy should not normally be used in this mode.\n\n\
Are you sure you want to switch to the diagnostic mode?\n\n"
END



STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ITEMTYPE_NONE				" item"
    IDS_ITEMTYPE_COMPONENT			" component"
    IDS_ITEMTYPE_FEATURE			" feature"
    IDS_ITEMTYPE_PRODUCT			" product"
    IDS_ITEMTYPE_ROOT				" item"
END


STRINGTABLE DISCARDABLE 
BEGIN
    IDS_COMMANDLINEHELP_MESSAGE		"/d<srcprof.mss> <destfile.log> :\n\
\tSaves the differences between current system and\n\
\tprofile <srcprof.mss> into <destfile.log>\n\n\
/s<destprof.mss> :\n\
\tSaves current system profile to <destprof.mss>\n\n\
/r<srcprof.mss> :\n\
\tRestores current system to state saved in <srcprof.mss>\n\n\
<srcprof.mss> :\n\
\tLaunches GUI version and loads <srcprof.mss>\n\n\
without any command-line option :\n\
\tLaunches GUI version\n\n"

    IDS_COMMANDLINEHELP_CAPTION		"Msispy Command Line Options"

    IDS_ERRORRESTORING_MESSAGE1		"Msispy could not open the saved state '%s'\n\
Please check the file name and path and try again.\n\n\
For help with the Msispy command line options type in 'msispy /?'"

    IDS_ERRORRESTORING_CAPTION		"Msispy: Error Restoring Saved State"

    IDS_ERRORCHKDIFF_MESSAGE1		"Msispy could not open the saved state '%s'\n\
Please check the file name and path and try again.\n\n\
For help with the Msispy command line options type in 'msispy /?'"

    IDS_ERRORCHKDIFF_CAPTION		"Msispy: Error Checking Differences"

END

// not used
STRINGTABLE DISCARDABLE 
BEGIN
    IDS_YESHELPLINE1				"Msispy will attempt to reinstall the missing feature(s) from the original source"
    IDS_YESHELPLINE2				"If this is on a CD or a floppy disk, you will need to insert it for the reinstall to proceed"
    IDS_NOHELPLINE1					"Msispy will attempt to continue without reinstalling the broken feature(s)."
    IDS_NOHELPLINE2					"The functionality of Msispy will be limited and some features will not be available."
END
// end not used

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_HELP_FAILED_MESSAGE					"Msispy could not load the help file. Please ensure that the help file is present."
    IDS_HELP_FAILED_CAPTION					"Error Loading Help"
	IDS_SWITCH_TO_RESTRICTED_MESSAGE		"\
Msispy has switched to the restricted mode, since a required \n\
system-interface DLL could not be loaded. The functionality \n\
of Msispy will be limited and some features will not be \n\
available in this mode."

	IDS_SWITCH_TO_RESTRICTED_CAPTION		"Msispy Information"
	IDS_STARTUP_RESTRICTED_MESSAGE		"\
Msispy started up in the restricted mode, since a required \n\
system-interface DLL could not be loaded. The functionality \n\
of Msispy will be limited and some features will not be \n\
available in this mode."

	IDS_STARTUP_RESTRICTED_CAPTION		"Msispy Information"

	IDS_UNABLE_TO_FIND_SS_MESSAGE			"Msispy could not load the saved state '%s'\n\
Please check the file name and path and try again.\n\n\
For help with the Msispy command line options type in 'msispy /?'"

	IDS_UNABLE_TO_FIND_SS_CAPTION			"Msispy: Error Loading Saved State"
END

STRINGTABLE DISCARDABLE
BEGIN

	IDS_F_ERROR_RESTORING_LN1		"Msispy could not restore the feature '%s' to its original state." 
	IDS_P_ERROR_RESTORING_LN1		"Msispy could not restore the product '%s' to its original state." 
	IDS_PFC_ERROR_RESTORING_LN2		"Status in saved state: '%s'\n\
Current status: '%s'\n\n\
Please select from one of the options below: \n"
END

STRINGTABLE DISCARDABLE
BEGIN
	IDS_PREFERENCES_HEADER1				"General"
	IDS_PREFERENCES_HEADER2				"Language"
	IDS_PREFERENCES_CAPTION				"Msispy Preferences"
	IDS_MODENORMAL_HELP				"This is the default Msispy mode. In this mode, \
the status of a component is determined solely by checking the key-file. \
All the features of Msispy are available in this mode."
	IDS_MODEDIAGNOSTIC_HELP			"In this mode, Msispy will perform a complete check of all files to determine \
component status, instead of relying on component key files. This mode \
will give a more accurate picture of the component status, but will make the \
program significantly slower. Msispy should not normally be used in this mode."
	IDS_MODERESTRICTED_HELP			"In this mode, the functionality of Msispy is limited to \
the Local Database only. You will not be able to access Windows installer packages \
or Msispy saved states. Leads to a slight performance gain."
	IDS_MODEDEGRADED_HELP			"Msispy can only run in the restricted mode, since a required \
system-interface DLL could not be loaded. The functionality \
of Msispy is limited and you will not be able to access Windows installer packages \
or Msispy saved states. To restore the broken features, you may need to \
re-install Msispy."
	IDS_COPYRIGHT			"Warning: This program is protected by \
copyright law and international treaties. Unauthorized reproduction or distribution of \
this program, or any portion of it, may result in severe civil and criminal penalties, \
and will be prosecuted under the law."
END

#endif // RC_INVOKED

#if 0 
!endif // makefile terminator
#endif
