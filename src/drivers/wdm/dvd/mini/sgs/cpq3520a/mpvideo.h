/*******************************************************************
*
*				 MPVIDEO.H
*
*				 Copyright (C) 1995 SGS-THOMSON Microelectronics.
*
*
*				 Prototypes for MPVIDEO.C
*
*******************************************************************/

#ifndef __MPVIDEO_H__
#define __MPVIDEO_H__

VOID VideoEnableInterrupts(IN PVOID);
VOID VideoTimerCallBack(PHW_STREAM_OBJECT pstrm);
ULONG miniPortVideoStop (PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoSetStc(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoSetAttribute(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoPlay(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
VOID miniPortVideoPacket(PHW_STREAM_REQUEST_BLOCK);
//ULONG miniPortVideoGetStc(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoGetAttribute(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoEndOfStream(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoDisable(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortVideoEnable(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortClearVideoBuffer(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
//ULONG miniPortCancelVideo(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
ULONG miniPortVideoReset(PHW_STREAM_REQUEST_BLOCK, PHW_DEVICE_EXTENSION);
VOID miniPortSetState(PHW_STREAM_REQUEST_BLOCK pSrb);
VOID mpstCtrlCommandComplete(PHW_STREAM_REQUEST_BLOCK pSrb);
void VideoPacketStub(PHW_STREAM_OBJECT pstrm);
VOID miniPortGetProperty(PHW_STREAM_REQUEST_BLOCK pSrb);
//void StubMpegEnableIRQ(PHW_STREAM_OBJECT pstrm);
void ProcessVideoFormat(PKSDATAFORMAT pfmt, PHW_DEVICE_EXTENSION pdevex);
void VideoQueryAccept(PHW_STREAM_REQUEST_BLOCK pSrb);

#endif //__MPVIDEO_H__
