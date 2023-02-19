//=============================================================================
// Copyright (c) 2000 Microsoft Corporation
//
// precomp.hpp
//
// Created 02/29/2000 johnstep (John Stephens)
//=============================================================================

#define UNICODE
#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
extern "C" {
#include <commctrl.h>
}
#define _CREDUI_
#include <wincrui.h>

#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif

#include <security.h>

#include <controls.hpp>
#include "utils.hpp"
#include <netlib.h>
#include <shfusion.h>

#if defined(DBG)
#if defined(CREDUIPRINTF)
#define CreduiDebugLog printf
#else
#define CreduiDebugLog DbgPrint
#endif
#else
#define CreduiDebugLog
#endif

//-----------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------

struct CREDUI_STRINGS
{
    WCHAR UserNameTipTitle[32];
    WCHAR UserNameTipText[256];
    WCHAR PasswordTipTitle[32];
    WCHAR PasswordTipText[256];
    WCHAR CapsLockTipTitle[32];
    WCHAR CapsLockTipText[256];
    WCHAR LogonTipTitle[32];
    WCHAR LogonTipText[256];
    WCHAR LogonTipCaps[256];
    WCHAR DnsCaption[64];
    WCHAR NetbiosCaption[64];
    WCHAR GenericCaption[64];
    WCHAR Welcome[64];
    WCHAR WelcomeBack[64];
    WCHAR Connecting[64];
    WCHAR PasswordStatic[32];
    WCHAR PinStatic[32];
    WCHAR UserNameStatic[32];
    WCHAR CertificateStatic[32];
    WCHAR Certificate[32];
    WCHAR LookupName[64];
    WCHAR EmptyReader[64];
    WCHAR NoCard[32];
    WCHAR UnknownCard[64];
    WCHAR BackwardsCard[64];
    WCHAR EmptyCard[64];
    WCHAR ReadingCard[64];
    WCHAR CardError[64];
    WCHAR BackwardsTipTitle[32];
    WCHAR BackwardsTipText[128];
    WCHAR SmartCardStatic[32];
    WCHAR WrongOldTipTitle[32];
    WCHAR WrongOldTipText[256];
    WCHAR NotSameTipTitle[32];
    WCHAR NotSameTipText[256];
    WCHAR TooShortTipTitle[32];
    WCHAR TooShortTipText[256];
    WCHAR Save[64];
    WCHAR PromptForUse[256];
    WCHAR NoUsernameEntered[256];
    WCHAR PassportSave[64];
    WCHAR EmailName[64];
    WCHAR DowngradeTipText[256];
    WCHAR EmailNameTipTitle[64];
    WCHAR EmailNameTipText[256];
};

// Private window message:

enum
{
    CREDUI_WM_APP_LOOKUP_COMPLETE   = WM_APP + 0,
    CREDUI_WM_APP_VIEW_COMPLETE     = WM_APP + 1
};

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern HMODULE CreduiInstance;
extern ULONG CreduiComReferenceCount;

extern BOOL CreduiIsPersonal;
extern BOOL CreduiIsDomainController;
extern BOOL CreduiIsSafeMode;

extern CREDUI_STRINGS CreduiStrings;

extern UINT CreduiScarduiWmReaderArrival;
extern UINT CreduiScarduiWmReaderRemoval;
extern UINT CreduiScarduiWmCardInsertion;
extern UINT CreduiScarduiWmCardRemoval;
extern UINT CreduiScarduiWmCardCertAvail;
extern UINT CreduiScarduiWmCardStatus;

extern BOOL CreduiHasSmartCardSupport;

extern CONST CREDUI_BALLOON_TIP_INFO CreduiCapsLockTipInfo;
