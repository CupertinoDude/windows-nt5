/////////////////////////////////////////////////////////////////////////////
//  INTEL Corporation Proprietary Information
//  This listing is supplied under the terms of a license agreement with Intel 
//  Corporation and many not be copied nor disclosed except in accordance
//  with the terms of that agreement.
//  Copyright (c) 1995, 1996 Intel Corporation. 
//
//
//  Module Name: g723snd.cpp
//	Environment: MSVC 4.0, OLE 2
/////////////////////////////////////////////////////////////////////////////////
#include "g723snd.h"

G723_ppmSend::G723_ppmSend(IUnknown* pUnkOuter, 
						   IUnknown** ppUnkInner) : 
		ppmSend(G723_PT, 0, 
				8000, pUnkOuter, ppUnkInner),
        
        m_dwLastTimeStamp(0)
{
}

G723_ppmSend::~G723_ppmSend()
{
}

IMPLEMENT_CREATEPROC(G723_ppmSend);

//////////////////////////////////////////////////////////////////////////////////////////
//SetMarkerBit: Determines whether to set the marker bit or not.  lastPacket is TRUE if
//				this is the last packet of the frame; FALSE if not.	 With audio, we don't
//				don't care about fragmentation, just the start of a talkspurt.
//////////////////////////////////////////////////////////////////////////////////////////
BOOL G723_ppmSend::SetMarkerBit(BOOL lastPacket)
{
	return m_markTalkSpurt;
}

//////////////////////////////////////////////////////////////////////////////////////////
//MakeTimeStamp: Generate a time stamp based on the frequency specified in the Profile Spec.
//////////////////////////////////////////////////////////////////////////////////////////
DWORD G723_ppmSend::MakeTimeStamp(MsgDescriptor* pMsgDescrip, 
								BOOL bStartStream,
								BOOL bUseInputTime)
{

#ifndef TIMESTAMP_OFF 

       DWORD ThisTimeStamp;
       DWORD CurTime = timeGetTime();
	   DWORD delta;
       DWORD epsilon;

    
    if (bUseInputTime) CurTime = pMsgDescrip->m_TimeStamp;

    // calculate the time span encoded in this packet
    delta = 240 * CountFrames((char *) pMsgDescrip->m_pBuffer, pMsgDescrip->m_Size) / (m_Frequency/1000);
    epsilon = delta/2;

    // init, do it here so it is set when we get the first packet
    // not at init time, they may be significantly different
	// Generate our first time stamp based on the current time.
    if (m_dwStartTime == 0)
    {
		// if the first packet we receive is a drop or silence then the delta will
		// be zero.  We just won't do anything until we receive valid data.
		if (delta != 0)
		{
			m_dwStartTime = CurTime;
			m_dwLastTime = m_dwStartTime -  delta;
			ThisTimeStamp = (((CurTime - m_dwStartTime) + (epsilon)) / delta) * delta * (m_Frequency/1000);
		}
	}
    else
	if (bStartStream)
	{
		// bStartStream will be set if this is the first packet after a break in a 
		// data stream.  We need to get our time stamps back on track, so we'll generate a time
		// based on the current time.  This case can happen if for some reason the capture device
		// gets starved or we are in half duplex and we are switching to talk mode.
		if (delta != 0)
		{
			ThisTimeStamp = (((CurTime - m_dwStartTime) + (epsilon)) / delta) * delta * (m_Frequency/1000);
		}
		else
		{
			// we don't have a valid delta, so we'll make do with the previous delta
			ThisTimeStamp = (((CurTime - m_dwStartTime) + (epsilon)) / m_dwLastDelta) * m_dwLastDelta * (m_Frequency/1000);
			ThisTimeStamp -= m_dwLastDelta * (m_Frequency/1000);
		}
    }
	else
	{
	    // if we are in a continuous audio data stream, then we just want to increment our timestamp
		// for this data packet.  We don't want to use the current time because we don't know how long
		// it took from the time the data was acutally captured to the time we got it.  We have to rely
		// on the person feeding us data to let us know more information about the data stream.
		ThisTimeStamp = m_dwLastTimeStamp + 240 * CountFrames((char *) pMsgDescrip->m_pBuffer, pMsgDescrip->m_Size);
	}

	m_dwLastTimeStamp = ThisTimeStamp;
	m_dwLastTime = CurTime;
	if (delta != 0)
	{
		m_dwLastDelta = delta;
	}
    return ThisTimeStamp;
#else
//I changed this because the times I was getting were widely spaced.  When I was in debugging
//mode.
static DWORD CurTime = 0;
CurTime++;
#endif 

return CurTime;
}


int G723_ppmSend::CountFrames(char *ipBuffer, int len)
{
    int Count = 0;
    int Delta = 0;
    int Length = len;
    LPBYTE pBuffer = (LPBYTE) ipBuffer;

    while(Length >= 4)
    {
        if (pBuffer[0] & 2)  // silence frame
        {
            if (Length < 4)  // invalid frame
            {
                break;
            }
            Delta = 4;
			//TTDBG(ghISRInst, TT_NOTIFY, "Dejitter::CountFrames - found silence frame");

        } 

        else if (pBuffer[0] & 1)  // 20 byte frame (5.3k)
        {
            if (Length < 20) // invalid frame
            {
                break;
            }
            Delta = 20;
        } 
        
        else               // 24 byte frame (6.3k)
        { 
            if (Length < 24) // invalid frame
            {
                break;
            }
            Delta = 24;
        }

        Count++;
        pBuffer += Delta;
        Length -= Delta;
   }
   return Count;
}


