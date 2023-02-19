/* *************************************************************************
**    INTEL Corporation Proprietary Information
**
**    This listing is supplied under the terms of a license
**    agreement with INTEL Corporation and may not be copied
**    nor disclosed except in accordance with the terms of
**    that agreement.
**
**    Copyright (c) 1995 Intel Corporation.
**    Copyright (c) 1996 Intel Corporation.
**    All Rights Reserved.
**
** *************************************************************************
*/
;// $Header:   S:\h26x\src\dec\d3rtp.h_v   1.4   06 Nov 1996 16:27:40   gmlim  $
;// $Log:   S:\h26x\src\dec\d3rtp.h_v  $
;// 
;//    Rev 1.4   06 Nov 1996 16:27:40   gmlim
;// Removed H263ModeC.
;// 
;//    Rev 1.3   06 Nov 1996 15:17:16   CZHU
;// changed FindNextPacket interface to return MVs.
;// 
;//    Rev 1.2   03 Nov 1996 18:39:08   gmlim
;// Added NEXT_MODE_C.
;// 
;//    Rev 1.1   23 Jul 1996 11:22:42   CZHU
;// 
;// Added a MV recovery. Hursitic will be added in later.
;// 
;//    Rev 1.0   22 Apr 1996 16:44:06   BECHOLS
;// Initial revision.
;// 
;//    Rev 1.7   10 Apr 1996 13:35:30   CZHU
;// 
;// Added subroutine to recover picture header information from extended bitstr
;// 
;//    Rev 1.6   29 Mar 1996 14:40:00   CZHU
;// 
;// cleaning 
;// 
;//    Rev 1.5   29 Mar 1996 13:39:00   CZHU
;// 
;// Moved bs verification to c3rtp.cpp
;// 
;//    Rev 1.4   28 Mar 1996 18:40:18   CZHU
;// Support packet loss recovery
;// 
;//    Rev 1.3   23 Feb 1996 16:21:26   CZHU
;// No change.
;// 
;//    Rev 1.2   15 Feb 1996 12:01:56   CZHU
;// 
;// More clean up
;// 
;//    Rev 1.1   14 Feb 1996 15:00:20   CZHU
;// Added support Mode A and Mode B
;// 
;//    Rev 1.0   12 Feb 1996 17:05:58   CZHU
;// Initial revision.
;// 
;//    Rev 1.0   11 Dec 1995 14:54:26   CZHU
;// Initial revision.

#ifndef _H263_D3RTP_H_
#define _H263_D3RTP_H_

const long PACKET_FAULT     = ICERR_CUSTOM -1;
const long NEXT_MODE_A      = ICERR_CUSTOM -2;
const long NEXT_MODE_B      = ICERR_CUSTOM -3;
const long NEXT_MODE_C      = ICERR_CUSTOM -4;
const long NEXT_MODE_LAST   = ICERR_CUSTOM -5;

extern  I32 RtpH263FindNextPacket( 
          T_H263DecoderCatalog FAR * , BITSTREAM_STATE FAR * , U32 **, U32 *, int *, int *,I8 *
	    );                      
extern I32 RtpGetPicHeaderFromBsExt(T_H263DecoderCatalog FAR * );
void MVAdjustment(T_BlkAction *,int ,int,  int ,int , int , const int );
#endif
