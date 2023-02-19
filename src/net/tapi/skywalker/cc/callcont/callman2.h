/****************************************************************************
 *
 *	$Archive:   S:/STURGEON/SRC/CALLCONT/VCS/callman2.h_v  $
 *
 *  INTEL Corporation Prorietary Information
 *
 *  This listing is supplied under the terms of a license agreement
 *  with INTEL Corporation and may not be copied nor disclosed except
 *  in accordance with the terms of that agreement.
 *
 *	Copyright (c) 1996 Intel Corporation.
 *
 *	$Revision:   1.7  $
 *	$Date:   03 Jan 1997 16:28:04  $
 *	$Author:   EHOWARDX  $
 *
 *	Deliverable:
 *
 *	Abstract:
 *
 *	Notes:
 *
 ***************************************************************************/

#ifdef GATEKEEPER

#ifndef CALLMAN2_H
#define CALLMAN2_H

#include "gkiman.h"

typedef HRESULT (*PGKICALLFUN)(PGKICALL pGkiCall, void *pConference);

// Additions to CALLMAN.C functionality
HRESULT LockGkiCallAndConference(	HANDLE					hGkiCall,
									PPGKICALL 				ppGkiCall,
									void * *				ppConference,
									DWORD *					phCall,
									DWORD *					phConference);
HRESULT UnlockGkiCallAndConference(	PGKICALL				pGkiCall,
									void *					pConference,
									DWORD					hCall,
									DWORD					hConference);
HRESULT LockGkiCall(HANDLE hGkiCall, PPGKICALL ppGkiCall);
HRESULT UnlockGkiCall(PGKICALL pGkiCall);

HRESULT ApplyToAllCalls(PGKICALLFUN pGkiCallFun);

// Complete CC_xxx Operations
HRESULT ListenReject        (DWORD hListen, HRESULT Reason);
HRESULT PlaceCallConfirm    (void *pCallVoid, void *pConferenceVoid);
HRESULT PlaceCallReject     (void *pCallVoid, void *pConferenceVoid, HRESULT Reason);
HRESULT AcceptCallConfirm   (void *pCallVoid, void *pConferenceVoid);
HRESULT AcceptCallReject    (void *pCallVoid, void *pConferenceVoid, HRESULT Reason);

HRESULT OpenChannelConfirm  (DWORD hChannel);
HRESULT OpenChannelReject   (DWORD hChannel, HRESULT Reason);
HRESULT AcceptChannelConfirm(DWORD hChannel);
HRESULT AcceptChannelReject (DWORD hChannel, HRESULT Reason);

// Handle gratuitous messages from Gatekeeper
HRESULT Disengage           (void *pCallVoid);
HRESULT BandwidthShrunk     (void *pCallVoid,
                             void *pConferenceVoid,
                             unsigned uBandwidthAllocated,
                             long lBandwidthChange);

#endif // CALLMAN2_H

#endif // GATEKEEPER
