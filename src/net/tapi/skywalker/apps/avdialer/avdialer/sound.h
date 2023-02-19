/////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1997 Active Voice Corporation. All Rights Reserved. 
//
// Active Agent(r) and Unified Communications(tm) are trademarks of Active Voice Corporation.
//
// Other brand and product names used herein are trademarks of their respective owners.
//
// The entire program and user interface including the structure, sequence, selection, 
// and arrangement of the dialog, the exclusively "yes" and "no" choices represented 
// by "1" and "2," and each dialog message are protected by copyrights registered in 
// the United States and by international treaties.
//
// Protected by one or more of the following United States patents: 5,070,526, 5,488,650, 
// 5,434,906, 5,581,604, 5,533,102, 5,568,540, 5,625,676, 5,651,054.
//
// Active Voice Corporation
// Seattle, Washington
// USA
//
/////////////////////////////////////////////////////////////////////////////////////////

//sound.h

#ifndef _SOUND_H_
#define _SOUND_H_

#include <mmsystem.h>

// --- BUG416970 ---
static LPCTSTR    szSoundDialer    = _T("Dialer");

///////////////////////////////////////////////////////////////////////////////
//Prototypes

BOOL        ActivePlaySound(LPCTSTR szSound,LPCTSTR szSection=NULL,UINT uSound=SND_ASYNC);
BOOL        ActivePlaySound(LPCTSTR szSound,LPCTSTR szSection,CString& sFullPath);
void        ActiveClearSound();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif //_SOUND_H_