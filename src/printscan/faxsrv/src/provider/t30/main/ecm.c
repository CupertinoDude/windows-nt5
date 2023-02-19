/***************************************************************************
 Name     :     ECM.C
 Comment  :     Contains the ECM T30 routines
 Revision Log
 Date     Name  Description
 -------- ----- ---------------------------------------------------------
***************************************************************************/
#define USE_DEBUG_CONTEXT   DEBUG_CONTEXT_T30_MAIN

#include "prep.h"

#include "efaxcb.h"
#include "t30.h"
#include "hdlc.h"
#include "debug.h"

///RSL
#include "glbproto.h"

typedef enum
{
        ECMRECVOK_OK,
        ECMRECVOK_BADFR,
        ECMRECVOK_ABORT
}
ECMRECVOK;

/****************** begin prototypes from ecm.c *****************/
IFR RNR_RRLoop(PThrdGlbl pTG);
IFR CTC_RespRecvd(PThrdGlbl pTG, USHORT uBaud);
BOOL Recv_NotReadyLoop(PThrdGlbl pTG, IFR ifrFirst, IFR ifrLast);
BOOL FillInFrames(PThrdGlbl pTG, USHORT N, LONG sBufSize, USHORT uDataSize);
ECMRECVOK ECMRecvOK(PThrdGlbl pTG);
/***************** end of prototypes from ecm.c *****************/

BYTE RCP[3] = { 0xFF, 0x03, 0x86 };


ET30ACTION ECMPhaseC(PThrdGlbl pTG, BOOL fReTx)
{
    USHORT          uFrameNum, uFramesSent, uLim;
    SWORD           swRet;
    ULONG           lTotalLen=0;
    LPBYTE          lpPPRMask;
    LPBUFFER        lpbf=0;
    USHORT          uMod;

    /******** Transmitter ECM Phase C. Fig A-7/T.30 (sheet 1) ********/

    DEBUG_FUNCTION_NAME(_T("ECMPhaseC"));

    pTG->ECM.uFrameSize = ProtGetECMFrameSize(pTG);
    BG_CHK(pTG->ECM.uFrameSize==6 || pTG->ECM.uFrameSize==8);

    // already done in WhatNext
    // uSize = (1 << pTG->ECM.uFrameSize);
    // ICommSetSendMode(TRUE, uSize+ECM_EXTRA, uSize, TRUE);

    if(fReTx)
    {
        lpPPRMask = ProtGetRetransmitMask(pTG);
    }
    else
    {
        pTG->ECM.uPPRCount = 0;

        if(pTG->ECM.fEndOfPage)
        {
            pTG->ECM.SendPageCount++;
            pTG->ECM.SendBlockCount = 1;
            pTG->ECM.dwPageSize=0;

            DebugPrintEx(DEBUG_MSG,"Waiting for Startpage in ECM");

            // Callback to open file to send. Doesn't return any data
            if((swRet=GetSendBuf(pTG, 0, SEND_STARTPAGE)) != SEND_OK)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Nonzero return %d from SendProc at Start Page",
                                swRet);
                // return actionDCN;
                return actionERROR;
            }

            DebugPrintEx(DEBUG_MSG,"Got Startpage in ECM");
        }
        else
        {
            pTG->ECM.SendBlockCount++;

            DebugPrintEx(DEBUG_MSG,"Waiting for Startblock");

            // Callback to open file to send. Doesn't return any data
            if((swRet=GetSendBuf(pTG, 0, SEND_STARTBLOCK)) != SEND_OK)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Nonzero return %d from SendProc at Start Page",
                                swRet);
                // return actionDCN;
                return actionERROR;
            }

            DebugPrintEx(DEBUG_MSG,"Got Startblock in ECM");
        }
    }

    DebugPrintEx(   DEBUG_MSG,
                    "Starting ECM Partial Page SEND.......P=%d B=%d ReTx=%d",
                    pTG->ECM.SendPageCount, pTG->ECM.SendBlockCount, fReTx);
    if(fReTx)
    {
        ICommStatus(pTG, T30STATS_RESEND_ECM, pTG->ECM.SendPageCount, 0, pTG->ECM.SendBlockCount);
    }

    uMod = ProtGetSendMod(pTG);
    if(uMod >= V17_START && !pTG->ECM.fSentCTC)
        uMod |= ST_FLAG;

    pTG->ECM.fSentCTC = FALSE;

    // here we should use a small timeout (100ms?) and if it fails,
    // should go back to sending the previous V21 frame (which could be DCS
    // or MPS or whatever, which is why it gets complicated & we havn't
    // done it!). Meanwhile use a long timeout, ignore return value
    // and send anyway.

    if(!ModemRecvSilence(pTG, pTG->Params.hModem, RECV_PHASEC_PAUSE, LONG_RECVSILENCE_TIMEOUT))
    {
        DebugPrintEx(   DEBUG_ERR,
                        "ECM Pix RecvSilence(%d, %d) FAILED!!!",
                        RECV_PHASEC_PAUSE, LONG_RECVSILENCE_TIMEOUT);
    }

    if(!ModemSendMode(pTG, pTG->Params.hModem, uMod, TRUE, ifrECMPIX))
    {
        DebugPrintEx(DEBUG_ERR,"ModemSendMode failed in Tx ECM PhaseC");
        ICommFailureCode(pTG, T30FAILSE_SENDMODE_PHASEC);
        BG_CHK(FALSE);
        return actionERROR;
    }

#ifdef IFAX
    BroadcastMessage(pTG, IF_PSIFAX_DATAMODE, (PSIFAX_SEND|PSIFAX_ECM|(fReTx ? PSIFAX_RESEND : 0)), (uMod & (~ST_FLAG)));
#endif
    DebugPrintEx(DEBUG_MSG,"SENDING ECM Page Data.....");
    FComCriticalNeg(pTG, FALSE);

    uLim = (fReTx ? pTG->ECM.SendFrameCount : 256);
    BG_CHK(uLim);
    BG_CHK(lpbf == 0);

    for(uFrameNum=0, uFramesSent=0, lTotalLen=0, swRet=0; uFrameNum<uLim; uFrameNum++)
    {
        if(!fReTx || (lpPPRMask[uFrameNum/8] & (1 << (uFrameNum%8))))
        {
            BG_CHK(uFrameNum < 256 && pTG->ECM.uFrameSize <=8);  // shift below won't ovf 16 bits
            BG_CHK(lpbf == 0);
            swRet = GetSendBuf(pTG, &lpbf, (fReTx ? ((SLONG)(uFrameNum << pTG->ECM.uFrameSize)) : SEND_SEQ));

            if(swRet == SEND_ERROR)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Error return from SendProc in ECM retransmit");
                BG_CHK(lpbf == 0);
                // return actionDCN;    // goto NodeC;
                return actionERROR;
            }
            else if(swRet == SEND_EOF)
            {
                BG_CHK(lpbf == 0);
                if(!fReTx)
                    break;
                else
                {
                    BG_CHK(FALSE);
                    ICommFailureCode(pTG, T30FAILSE_PHASEC_RETX_EOF);
                    return actionDCN;
                }
            }
            BG_CHK(swRet == SEND_OK);

            BG_CHK(lpbf);
            BG_CHK(lpbf->lpbBegBuf+4 == lpbf->lpbBegData);

            lpbf->lpbBegBuf[0] = 0xFF;
            lpbf->lpbBegBuf[1] = 0x03;
            lpbf->lpbBegBuf[2] = 0x06;
            lpbf->lpbBegBuf[3] = (BYTE) uFrameNum;
            lpbf->lpbBegData -= 4;
            lpbf->wLengthData += 4;

            lTotalLen += lpbf->wLengthData;

            if(!ModemSendMem(pTG, pTG->Params.hModem, lpbf->lpbBegData, lpbf->wLengthData, SEND_ENDFRAME))
            {
                DebugPrintEx(DEBUG_ERR,"DataWrite Timeout in ECM Phase C");
                ICommFailureCode(pTG, T30FAILSE_MODEMSEND_PHASEC);
                BG_CHK(FALSE);
                return actionERROR;             // goto error;
            }

            if(!MyFreeBuf(pTG, lpbf))
            {
                DebugPrintEx(DEBUG_ERR,"FReeBuf failed in ECM Phase C");
                ICommFailureCode(pTG, T30FAILSE_FREEBUF_PHASEC);
                BG_CHK(FALSE);
                return actionERROR;             // goto error;
            }
            lpbf = 0;
            uFramesSent++;
        }
    }

    if( !ModemSendMem(pTG, pTG->Params.hModem, RCP, 3, SEND_ENDFRAME) ||
        !ModemSendMem(pTG, pTG->Params.hModem, RCP, 3, SEND_ENDFRAME) ||
        !ModemSendMem(pTG, pTG->Params.hModem, RCP, 3, SEND_ENDFRAME|SEND_FINAL))
    {
            DebugPrintEx(DEBUG_ERR,"DataWrite Timeout on RCPs");
            ICommFailureCode(pTG, T30FAILSE_MODEMSEND_ENDPHASEC);
            BG_CHK(FALSE);
            return actionERROR;             // goto error;
    }
/***
    if(!ModemDrain())
            return FALSE;
***/

    FComCriticalNeg(pTG, TRUE);

    DebugPrintEx(   DEBUG_MSG,
                    "Page Send Done.....len=(%ld, 0x%08x)",
                    lTotalLen, lTotalLen);

    pTG->ECM.FramesSent = uFramesSent;

    if(!fReTx)
    {
        BG_CHK(lTotalLen>=(ULONG)uFramesSent*4);
        pTG->ECM.dwPageSize+= (lTotalLen-uFramesSent*4); // 4-bytes of framing data
        pTG->ECM.SendFrameCount = uFrameNum;

        switch(GetSendBuf(pTG, 0, SEND_QUERYENDPAGE))
        {
        case SEND_OK:   pTG->ECM.fEndOfPage = FALSE;
                        break;
        case SEND_EOF:  pTG->ECM.fEndOfPage = TRUE;
                        break;
        default:        DebugPrintEx(   DEBUG_ERR,
                                        "Got SEND_ERROR from GetSendBuf at end of page");
                                        return actionERROR;
        }
    }
    if(!pTG->ECM.FramesSent)
    {
        DebugPrintEx(   DEBUG_ERR,
                        "Sent 0 frames--Bad PPR recvd or bad send file");
        ICommFailureCode(pTG, T30FAILSE_BADPPR);
        return actionERROR;
    }

    pTG->T30.fSendAfterSend = TRUE;      // ECM PhaseC/PIX--PPS-X
    return actionGONODE_V;
}

ET30ACTION ECMPhaseD(PThrdGlbl pTG)
{
    USHORT          uTryCount, i;
    ET30ACTION      action;
    BYTE            bPPSfif[3];
    LPBYTE          lpPPR;
    char szBuf[32]= {0};

    DEBUG_FUNCTION_NAME(_T("ECMPhaseD"));
    /******** Transmitter ECM Phase D. Fig A-8 to A-17/T.30 ********/

    if(!pTG->ECM.fEndOfPage)
    {
        pTG->T30.ifrSend = ifrPPS_NULL;
    }
    else
    {
        switch(action = pTG->Params.lpfnWhatNext(pTG, eventPOSTPAGE))
        {
        case actionSENDMPS:     pTG->T30.ifrSend = ifrPPS_MPS;
                                break;
        case actionSENDEOM:     pTG->T30.ifrSend = ifrPPS_EOM;
                                break;
        case actionSENDEOP:     pTG->T30.ifrSend = ifrPPS_EOP;
                                break;
#ifdef PRI
        case actionSENDPRIMPS:  pTG->T30.ifrSend = ifrPPS_PRI_MPS;
                                break;
        case actionSENDPRIEOM:  pTG->T30.ifrSend = ifrPPS_PRI_EOM;
                                break;
        case actionSENDPRIEOP:  pTG->T30.ifrSend = ifrPPS_PRI_EOP;
                                break;
#endif
        case actionERROR:       return action;  // goto PhaseLoop & exit
        default:                return BadAction(pTG, action);
        }
    }

    bPPSfif[0] = pTG->ECM.SendPageCount-1;
    bPPSfif[1] = pTG->ECM.SendBlockCount-1;
    BG_CHK(pTG->ECM.FramesSent && pTG->ECM.FramesSent<=256);
    // bPPSfif[2] = pTG->ECM.SendFrameCount-1;           // don't know which one..!!
    bPPSfif[2] = pTG->ECM.FramesSent-1;                  // this one! For sure

    for(uTryCount=0 ;;)
    {
        SendSingleFrame(pTG, pTG->T30.ifrSend, bPPSfif, 3, 1);

echoretry:
        pTG->T30.ifrResp = GetResponse(pTG, ifrPPSresponse);
        // if we hear our own frame, try to recv again. DONT retransmit!
        if(pTG->T30.ifrResp==pTG->T30.ifrSend)
        {
            ECHOMSG(pTG->T30.ifrResp);
            goto echoretry;
        }

        if(pTG->T30.ifrResp != ifrNULL && pTG->T30.ifrResp != ifrBAD)
                break;

        if(++uTryCount >= 3)
        {
            DebugPrintEx(DEBUG_ERR,"ECM 3 PostPages, No reply");
            ICommFailureCode(pTG, T30FAILSE_3POSTPAGE_NOREPLY);
            return actionDCN;
        }
    }

    switch(pTG->T30.ifrResp)
    {
      case ifrBAD:
      case ifrNULL: BG_CHK(FALSE);  // should never get here
                    ICommFailureCode(pTG, T30FAILSE_BUG2);
                    return actionERROR;     // in case they do :-)

      case ifrDCN:  DebugPrintEx(   DEBUG_ERR,
                                    "Got ifrDCN from GetResponse after sending"
                                    " post-page command");
                    ICommFailureCode(pTG, T30FAILSE_POSTPAGE_DCN);
                    return actionHANGUP;
      case ifrPPR:  DebugPrintEx(   DEBUG_MSG,
                                    "PPR (P=%d B=%d F=%d) Received: ",
                                    pTG->ECM.SendPageCount-1,
                                    pTG->ECM.SendBlockCount-1,
                                    pTG->ECM.FramesSent-1);
                    lpPPR = ProtGetRetransmitMask(pTG);
#ifdef DEBUG
                    for(i=0; i<32; i++)
                        _stprintf(&szBuf[i]," %02x", lpPPR[i]);
                    DebugPrintEx(DEBUG_MSG,"bytes? %s",szBuf);
#endif //DEBUG
                    if(++pTG->ECM.uPPRCount >= 4)
                        goto FourthPPR;
                    return actionGONODE_ECMRETRANSMIT;
      case ifrRNR:  if((pTG->T30.ifrResp=RNR_RRLoop(pTG)) == ifrDCN)
                    {
                        DebugPrintEx(DEBUG_ERR,"RR_RNR loop failed");
                        // ICommFailureCode already called in RR_RNRLoop()
                        return actionDCN;
                    }
                    DebugPrintEx(   DEBUG_MSG,
                                    "Got %d from RNR",
                                    pTG->T30.ifrResp);
                    break;
        }

        switch(pTG->T30.ifrResp)
        {
          case ifrPIP:
          case ifrPIN:
#ifdef PRI
                    return GONODE_E;
#else
                    DebugPrintEx(DEBUG_WRN,"Procedure interrupts not supported");
                    // return actionERROR;
                    // fallthru and treat like MCF
                    pTG->T30.ifrResp = ifrMCF;
#endif
          case ifrMCF:
                    {
                        WORD wSize = (WORD) (pTG->ECM.dwPageSize>>10); //Units are KB
                        ICommStatus(pTG, T30STATS_CONFIRM_ECM,
                                (USHORT) LOBYTE(wSize),
                                (USHORT) HIBYTE(wSize),
                                (USHORT)(pTG->ECM.SendPageCount&0xff));
                        DebugPrintEx(   DEBUG_MSG,
                                        "Sending T30STATS_CONFIRM_pTG->ECM. wSize=%u",
                                        (unsigned) wSize);

                    }
                    action=pTG->Params.lpfnWhatNext(    pTG, eventGOT_ECM_PPS_RESP,
                                                        (UWORD)pTG->T30.ifrResp,
                                                        ULongToPtr((DWORD)pTG->T30.ifrSend));
                    if(pTG->T30.ifrSend==ifrPPS_EOP && pTG->T30.ifrResp==ifrMCF && action==actionDCN)
                    {
                        ICommFailureCode(pTG, T30FAILSE_SUCCESS);
                        return actionDCN_SUCCESS;
                    }
                    else
                        return action;

          default:  DebugPrintEx(   DEBUG_ERR,
                                    "Got UNKNOWN from GetResponse after sending post-page command");
                    ICommFailureCode(pTG, T30FAILSE_POSTPAGE_UNKNOWN);
                    return actionDCN;
        }

FourthPPR:
    action = pTG->Params.lpfnWhatNext(pTG, event4THPPR, (WORD)pTG->ECM.SendFrameCount, (DWORD)pTG->ECM.FramesSent);
    switch(action)
    {
      case actionGONODE_ECMRETRANSMIT:
                    if(CTC_RespRecvd(pTG, ProtGetSendMod(pTG)) == ifrCTR)
                    {
                        pTG->ECM.uPPRCount = 0;
                        pTG->ECM.fSentCTC = TRUE;
                        return actionGONODE_ECMRETRANSMIT;
                    }
                    else
                    {
                        DebugPrintEx(DEBUG_ERR,"CTC-CTR failed");
                        // ICommFailureCode already called in CTC_RespRecvd(pTG)
                        return actionDCN;
                    }

      case actionSENDEOR_EOP:
      case actionDCN:
      case actionERROR:
                    return action;
      default:
                    return BadAction(pTG, action);
                    // none of the EOR stuff
                    // return actionDCN;
    }
}

ET30ACTION ECMSendEOR_EOP(PThrdGlbl pTG)
{
    // dont set new ICommFailure codes in this function. We have already
    // set the 'too many retries' code

    USHORT          uTryCount;

    DEBUG_FUNCTION_NAME(_T("ECMSendEOR_EOP"));

    for(uTryCount=0 ;;)
    {
        DebugPrintEx(DEBUG_WRN,"Sending EOR-EOP");

        pTG->T30.ifrSend = ifrEOR_EOP;
        SendEOR_EOP(pTG);

    echoretry:
        pTG->T30.ifrResp = GetResponse(pTG, ifrEORresponse);
        // if we hear our own frame, try to recv again. DONT retransmit!
        if(pTG->T30.ifrResp==pTG->T30.ifrSend)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Ignoring ECHO of %s(%d)",
                            (LPSTR)(rgFrameInfo[pTG->T30.ifrResp].szName),
                            pTG->T30.ifrResp);
            goto echoretry;
        }

        if(pTG->T30.ifrResp != ifrNULL && pTG->T30.ifrResp != ifrBAD)
            break;

        if(++uTryCount >= 3)
        {
            DebugPrintEx(DEBUG_ERR,"ECM 3 EORs, No reply");
            return actionDCN;
        }
    }

    switch(pTG->T30.ifrResp)
    {
      case ifrBAD:
      case ifrNULL: BG_CHK(FALSE);  // should never get here
                    return actionERROR;     // in case they do :-)

      case ifrDCN:  DebugPrintEx(   DEBUG_ERR,
                                    "Got ifrDCN from GetResponse after sending EOR");
                    return actionHANGUP;

      case ifrRNR:  DebugPrintEx(DEBUG_WRN,"Sent EOR-EOP, got RNR");
                    if((pTG->T30.ifrResp=RNR_RRLoop(pTG)) == ifrDCN)
                    {
                        DebugPrintEx(DEBUG_ERR,"RR_RNR loop failed");
                        // ICommFailureCode already called in RR_RNRLoop(pTG)
                        return actionDCN;
                    }
                    DebugPrintEx(DEBUG_MSG,"Got %d from RNR", pTG->T30.ifrResp);
                    break;
    }

    switch(pTG->T30.ifrResp)
    {
      case ifrERR:  DebugPrintEx(   DEBUG_WRN,
                                    "Sent EOR-EOP. Got ERR. Sending DCN");
                    return actionDCN;
      default:      DebugPrintEx(   DEBUG_ERR,
                                    "Got UNKNOWN from GetResponse after sending EOR");
                    return actionDCN;
    }
}

// reduce this so that when externally measured it always ends up less
// then the specified max of 65s, so we pass protocol conformance tests
#define T5_TIMEOUT              62000L                  // 60s + 5s

IFR RNR_RRLoop(PThrdGlbl pTG)
{
    /** Flowchart is:- Enter this on getting an RNR. Then start T5,
            send RR, get response (standard ResponseRecvd routine). If
            no response, send RR again. Repeat 3 times. If RNR response
            recvd, send RR again & repeat, until T5 expires, upon which
            send DCN & hangup.

            This routine returns ifrDCN, implying teh caller should
            go to NodeC, or ifrXXX, which is used as the response to be
            analysed further down the chart. Never returns ifrNULL
    **/

    UWORD   i;
    IFR     ifr;

    DEBUG_FUNCTION_NAME(_T("RNR_RRLoop"));

    TstartTimeOut(pTG, &(pTG->ECM.toT5), T5_TIMEOUT);
    do
    {
        for(i=0; i<3; i++)
        {
            if(!TcheckTimeOut(pTG, &(pTG->ECM.toT5)))
            {
                DebugPrintEx(DEBUG_ERR,"T5 timeout on Sender");
                ICommFailureCode(pTG, T30FAILSE_RR_T5);
                return ifrDCN;                                  // T5 timed out
            }

            SendRR(pTG);

        echoretry:
            ifr = GetResponse(pTG, ifrRRresponse);
            // if we hear our own frame, try to recv again. DONT retransmit!
            if(ifr==ifrRR)
            {
                DebugPrintEx(   DEBUG_WRN,
                                "Ignoring ECHO of %s(%d)",
                                (LPSTR)(rgFrameInfo[ifr].szName),
                                ifr);
                goto echoretry;
            }

            if(ifr!=ifrNULL && ifr!=ifrBAD)
                    break;
            // on ifrNULL (T4 timeout) we resend RR & try again -- 3 times
        }
    }
    while(ifr == ifrRNR);

    // BG_CHK(ifr!=ifrRNR && ifr!=ifrNULL && ifr!=ifrBAD && ifr!=ifrTIMEOUT);
    // can get BAD or NULL here when i=3
    BG_CHK(ifr!=ifrRNR && ifr!=ifrTIMEOUT);

    if(ifr == ifrDCN)
    {
        DebugPrintEx(DEBUG_ERR,"Got DCN in response to RR");
        ICommFailureCode(pTG, T30FAILSE_RR_DCN);
    }

    if(ifr==ifrBAD || ifr==ifrNULL)
    {
        BG_CHK(i==3);
        DebugPrintEx(DEBUG_ERR,"No response to RR 3 times");
        ICommFailureCode(pTG, T30FAILSE_RR_3xT4);
        ifr=ifrDCN;     // same as T5 timeout
    }
    return ifr;             // finally got a non-RNR response
                                    // return ifrDCN or ifrXXXX (not RNR)
}

IFR CTC_RespRecvd(PThrdGlbl pTG, USHORT uBaud)
{
    UWORD   i;
    IFR     ifr = ifrDCN;
    BYTE    bCTCfif[2];

    DEBUG_FUNCTION_NAME(_T("CTC_RespRecvd"));

    BG_CHK((uBaud & (~0x0F)) == 0);
    bCTCfif[0] = 0;
    bCTCfif[1] = (uBaud << 2);

    for(i=0; i<3; i++)
    {
        SendCTC (pTG, bCTCfif);

    echoretry:
        ifr = GetResponse(pTG, ifrCTCresponse);
        // if we hear our own frame, try to recv again. DONT retransmit!
        if(ifr==ifrCTC)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Ignoring ECHO of %s(%d)",
                            (LPSTR)(rgFrameInfo[ifr].szName), ifr);
            goto echoretry;
        }


        if(ifr!=ifrNULL && ifr!=ifrBAD)
                break;
        // on ifrNULL (T4 timeout) we resend RR & try again -- 3 times
    }

    if(ifr==ifrNULL || ifr==ifrBAD)
    {
        BG_CHK(i == 3);
        DebugPrintEx(DEBUG_ERR,"No response to CTC 3 times");
        ICommFailureCode(pTG, T30FAILSE_CTC_3xT4);
        ifr = ifrDCN;
    }
    else if(ifr != ifrCTR)
    {
        DebugPrintEx(DEBUG_ERR,"Bad response CTC");
        ICommFailureCode(pTG, T30FAILSE_CTC_UNKNOWN);
    }
    return ifr;             // return ifrDCN or ifrXXXX
}

ET30ACTION ECMRecvPhaseD ( PThrdGlbl pTG)
{
    DWORD           CurrPPS;
    ET30ACTION      action;

    DEBUG_FUNCTION_NAME(_T("ECMRecvPhaseD"));
    switch(pTG->T30.ifrCommand)
    {
    case ifrPRI_MPS:
    case ifrPRI_EOM:
    case ifrPRI_EOP:
#ifdef PRI
                        return actionGONODE_RECVPRIQ;
#else
                        pTG->T30.ifrCommand = pTG->T30.ifrCommand-ifrPRI_MPS+ifrMPS;
                        break;
#endif

    case ifrPPS_PRI_MPS:
    case ifrPPS_PRI_EOM:
    case ifrPPS_PRI_EOP:
#ifdef PRI
                        goto RecvPPSPRIQ;
#else
                        pTG->T30.ifrCommand = pTG->T30.ifrCommand-ifrPPS_PRI_MPS+ifrPPS_MPS;
                        break;
#endif

    case ifrEOR_PRI_MPS:
    case ifrEOR_PRI_EOM:
    case ifrEOR_PRI_EOP:
#ifdef PRI
                        goto RecvEORPRIQ;
#else
                        pTG->T30.ifrCommand = pTG->T30.ifrCommand-ifrEOR_PRI_MPS+ifrEOR_MPS;
                        break;
#endif
    }


UsePrevCommand:

    switch(pTG->T30.ifrCommand)
    {
    case ifrCTC:
            EnterPageCrit(); //start CTR--PAGE critsection
            pTG->ECM.fRecvdCTC = TRUE;
            SendCTR(pTG);
            ECHOPROTECT(ifrCTR, modeECMRETX);
            return actionGONODE_RECVECMRETRANSMIT;
    case ifrPPS_NULL:
    case ifrPPS_MPS:
    case ifrPPS_EOM:
    case ifrPPS_EOP:
            // saved for PPS--RNR--RR--MCF(missed)--RR--MCF sequences
            pTG->ECM.ifrPrevCommand = pTG->T30.ifrCommand;

            //////BugFix 396//////
            CurrPPS = ProtGetPPS(pTG);
            if(pTG->ECM.ifrPrevResponse==ifrMCF && _fmemcmp(pTG->ECM.bPrevPPS, (LPBYTE)(&CurrPPS), 4)==0)
                    goto GoSendMCF;
            _fmemcpy(pTG->ECM.bPrevPPS, (LPBYTE)(&CurrPPS), 4);
            pTG->ECM.ifrPrevResponse = 0;
            //////BugFix 396//////

            switch(ECMRecvOK(pTG))
            {
            default:
            case ECMRECVOK_ABORT:       return actionERROR;
            case ECMRECVOK_OK:          break;
            case ECMRECVOK_BADFR:
                                        EnterPageCrit(); //start PPR--PAGE critsection
                                        SendPPR(pTG, pTG->ECM.bRecvBadFrameMask);
                                        ECHOPROTECT(ifrPPR, modeECMRETX);
                                        return actionGONODE_RECVECMRETRANSMIT;
            }

            // now we can mark eop here
            // #ifdef PRI is on, this won't work (e.g. if the guy sends
            // PRI_MPS at end of page, then this needs to be called but it won't.

            if(pTG->T30.ifrCommand != ifrPPS_NULL
               && !pTG->ECM.fRecvEndOfPage)              // so we won't call this twice
            {
                // RECV_ENDDOC if PPS_EOP or PPS_EOM
                PutRecvBuf(pTG, NULL, ((pTG->T30.ifrCommand==ifrPPS_MPS) ? RECV_ENDPAGE : RECV_ENDDOC));
                // ignore error/abort. We'll catch it soon enough
                pTG->ECM.fRecvEndOfPage = TRUE;
            }

            if(!Recv_NotReadyLoop(pTG, ifrPPS_FIRST, ifrPPS_LAST))
            {
                ICommFailureCode(pTG, T30FAILRE_PPS_RNR_LOOP);
                return actionHANGUP;
            }

            if(pTG->T30.ifrCommand == ifrPPS_NULL)
            {
                action = actionSENDMCF;
            }
            else switch(action = pTG->Params.lpfnWhatNext(pTG, eventRECVPOSTPAGECMD,(WORD)pTG->T30.ifrCommand))
            {
              case actionSENDMCF:           break;
              case actionSENDRTN:
              case actionHANGUP:
              case actionERROR:
              default:                      return BadAction(pTG, action);
            }

#ifdef PRI
            if(pTG->T30.ifrCommand != ifrPPS_NULL)
            {
                    if((action = pTG->Params.lpfnWhatNext(pTG, eventQUERYLOCALINT))==actionTRUE)
                    {
                        ECHOPROTECT(ifrPIP, 0);
                        SendPIP(pTG);
                        break;
                    }
                    else if(action == actionERROR)
                        return action;
            }
#endif //PRI

GoSendMCF:
                pTG->ECM.ifrPrevResponse = ifrMCF;
                if(pTG->T30.ifrCommand == ifrPPS_NULL || pTG->T30.ifrCommand == ifrPPS_MPS)
                {
                        EnterPageCrit(); //start ECM MCF--PAGE critsection
                        ECHOPROTECT(ifrMCF, modeECM);
                        SendMCF(pTG);
                        return actionGONODE_RECVPHASEC;
                }
                ECHOPROTECT(ifrMCF, 0);
                SendMCF(pTG);
                if(pTG->T30.ifrCommand==ifrPPS_EOP)
                    return actionNODEF_SUCCESS;
                else
                    break;

    case ifrEOR_NULL:
    case ifrEOR_MPS:
    case ifrEOR_EOM:
    case ifrEOR_EOP:
                // saved for EOR--RNR--RR--ERR(missed)--RR--ERR sequences
                pTG->ECM.ifrPrevCommand = pTG->T30.ifrCommand;

                if(pTG->T30.ifrCommand!=ifrEOR_NULL
                   && !pTG->ECM.fRecvEndOfPage)              // so we won't call this twice
                {
                    // RECV_ENDDOC if EOR_EOP or EOR_EOM
                    PutRecvBuf(pTG, NULL, ((pTG->T30.ifrCommand==ifrEOR_MPS) ? RECV_ENDPAGE : RECV_ENDDOC));
                    // ignore error/abort. We'll catch it soon enough
                    pTG->ECM.fRecvEndOfPage = TRUE;
                }


                if(!Recv_NotReadyLoop(pTG, ifrEOR_FIRST, ifrEOR_LAST))
                {
                    ICommFailureCode(pTG, T30FAILRE_EOR_RNR_LOOP);
                    return actionHANGUP;
                }

#ifdef PRI
                if(pTG->T30.ifrCommand != ifrPPS_NULL)
                {
                    if((action = pTG->Params.lpfnWhatNext(pTG, eventQUERYLOCALINT))==actionTRUE)
                    {
                        ECHOPROTECT(ifrPIN, 0);
                        SendPIN(pTG);
                        break;
                    }
                    else if(action == actionERROR)
                        return action;
                }
#endif //PRI
                if(pTG->T30.ifrCommand == ifrEOR_NULL || pTG->T30.ifrCommand == ifrEOR_MPS)
                {
                    EnterPageCrit(); //start ERR--PAGE critsection
                    ECHOPROTECT(ifrERR, modeECM);
                    SendERR(pTG);
                    return actionGONODE_RECVPHASEC;
                }
                ECHOPROTECT(ifrERR, 0);
                SendERR(pTG);
                break;

    case ifrRR:
                if(pTG->ECM.ifrPrevCommand)
                {
                    pTG->T30.ifrCommand = pTG->ECM.ifrPrevCommand;
                    goto UsePrevCommand;
                }
                else
                {
                    DebugPrintEx(DEBUG_WRN,"ignoring ERR at weird time");
                    break;
                }

    default:
                DebugPrintEx(   DEBUG_WRN,
                                "Random Command frame received=%d",
                                pTG->T30.ifrCommand);
                break;  // ignore it
    }
    return actionGONODE_F;

#ifdef PRI
RecvPRIPPS:
    switch(ECMRecvOK(pTG))
    {
    default:
    ECMRECVOK_ABORT:        return actionERROR;
    ECMRECVOK_OK:           break;
    ECMRECVOK_BADFR:
            EnterPageCrit(); //start PRI PPR--PAGE critsection
            ECHOPROTECT(ifrPPR, modeECMRETX);
            SendPPR(pTG, pTG->ECM.bRecvBadFrameMask);
            return actionGONODE_RECVECMRETRANSMIT;
    }
RecvPRIEOR:
    switch(action = pTG->Params.lpfnWhatNext(pTG, eventGOTPRIQ, (WORD)pTG->T30.ifrCommand))
    {
      case actionERROR:             break;  // return to PhaseLoop
      case actionHANGUP:            break;
      case actionGONODE_F:          break;
      case actionSENDPIP:           pTG->T30.ifrSend=ifrPIP;
                                    return actionGOVOICE;
      case actionSENDPIN:           pTG->T30.ifrSend=ifrPIN;
                                    return actionGOVOICE;
      case actionGO_RECVPOSTPAGE:   if(pTG->T30.ifrCommand >= ifrPPS_PRI_FIRST && pTG->T30.ifrCommand <= ifrPPS_PRI_LAST)
                                    {
                                        pTG->T30.ifrCommand = pTG->T30.ifrCommand-ifrPPS_PRI_MPS+ifrPPS_MPS;
                                        goto NodeVIIIa;
                                    }
                                    else if(pTG->T30.ifrCommand >= ifrEOR_PRI_FIRST && pTG->T30.ifrCommand <= ifrEOR_PRI_LAST)
                                    {
                                        pTG->T30.ifrCommand = pTG->T30.ifrCommand-ifrEOR_PRI_MPS+ifrEOR_MPS;
                                        goto NodeIXa;
                                    }
                                    else
                                    {
                                        BG_CHK(FALSE);
                                    }
    }
    return action;
#endif //PRI
}

BOOL Recv_NotReadyLoop(PThrdGlbl pTG, IFR ifrFirst, IFR ifrLast)
{
    IFR ifrCommand;
    // OBSOLETE:- (??)
    // this is wrong. We should exit only on DCN I think.
    // We should definitely loop again on BAD
    // dunno about TIMEOUT and NULL
    DEBUG_FUNCTION_NAME(_T("Recv_NotReadyLoop"));
    do
    {
        if(ICommRecvBufIsEmpty(pTG))
                return TRUE;

        // sleep for a while, to give FMTRES etc some CPU cycles to catch
        // up with us. Can sleep just before sending response, since sender
        // waits upto 3 (6?) secs for it.
#ifndef FILET30
        IFProcSleep(1000);
#endif

        SendRNR(pTG);

echoretry:
        ifrCommand = GetCommand(pTG, ifrRNRcommand);
        // if we hear our own frame, try to recv again. DONT retransmit!
        if(ifrCommand==ifrRNR)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Ignoring ECHO of %s(%d)",
                            (LPSTR)(rgFrameInfo[ifrCommand].szName), ifrCommand);
            goto echoretry;
        }
    }
    while(  ifrCommand==ifrNULL || ifrCommand==ifrBAD ||
                    ifrCommand == ifrRR     ||
                    (ifrCommand >= ifrFirst && ifrCommand <= ifrLast) );

    // This means we exit on any valid frame _except_ RR and the PPS-X
    // or EOR-X we were expecting. We also exit on ifrTIMEOUT which is
    // when the GetCommand() times out without getting any CONNECT.
    // Otherwise we'd end up sending an MCF after timing out which
    // Ricoh's protocol tester doesnt like. (Ricoh's bug numbbers B3-0106)

    return FALSE;   // just hangs up after this
}

ECMRECVOK ECMRecvOK(PThrdGlbl pTG)
{
    USHORT N, i;

    DEBUG_FUNCTION_NAME(("ECMRecvOK"));

    N = ProtGetRecvECMFrameCount(pTG);
    // N==actual number of frames, i.e. PPR value+1

    DebugPrintEx(DEBUG_MSG,"ECMRecvChk %d frames", N);
    if(!N || N>256)
    {
        BG_CHK(FALSE);
        return ECMRECVOK_ABORT;
    }

    for(i=0; i<N; i++)
    {
        if(pTG->ECM.bRecvBadFrameMask[i/8] & (1 << (i%8)))
        {
            DebugPrintEx(DEBUG_ERR,"bad fr=%d", i);
            return ECMRECVOK_BADFR;
        }
    }
    return ECMRECVOK_OK;
}

ET30ACTION ECMRecvPhaseC(PThrdGlbl pTG, BOOL fRetrans)
{
    /******** Receiver ECM Phase C. Fig A-7/T.30 (sheet 1) ********/

    ULONG           lTotalLen=0;
    USHORT          uRet, uFrameNum, uRCPCount, uSize;
    USHORT          uRecvFrame, uMod;
    LPBUFFER        lpbf;
    LPBUFFER        lpbfShort;

    DEBUG_FUNCTION_NAME(_T("ECMRecvPhaseC"));
    // There is a race between sending the CFR and sending out an
    // +FRM=xx command, so we want to do it ASAP.

    pTG->ECM.uFrameSize = ProtGetRecvECMFrameSize(pTG);
    BG_CHK(pTG->ECM.uFrameSize==6 || pTG->ECM.uFrameSize==8);
    uSize = (1 << pTG->ECM.uFrameSize);

    uMod = ProtGetRecvMod(pTG);
    if(uMod >= V17_START && !pTG->ECM.fRecvdCTC) uMod |= ST_FLAG;

    pTG->T30.sRecvBufSize = MY_ECMBUF_SIZE;

    if((uRet = ModemECMRecvMode(pTG, pTG->Params.hModem, uMod, PHASEC_TIMEOUT)) != RECV_OK)
    {
        ExitPageCrit();
        DebugPrintEx(DEBUG_WRN,"RecvMode ret=%d", uRet);

        if(!fRetrans)
        {
            // in case we miss the page entirely
            // and jump into PhaseD directly

            // init bad frame bitmask to all-bad
            memset(pTG->ECM.bRecvBadFrameMask, 0xFF, 32);        // all bad
            // init the status vars for the PPS/EOR--(RR-RNR)*--PPR/MCF loop
            pTG->ECM.ifrPrevCommand = 0;
            // reset the FrameInThisBlock count. Reset only on
            // non-retransmit. Set when first PPS is recvd
            ProtResetRecvECMFrameCount(pTG);

            if(pTG->ECM.fRecvEndOfPage)  // as opposed to end-of-block
                    uRet = (USHORT)PutRecvBuf(pTG, NULL, RECV_STARTPAGE);
            else
                    uRet = (USHORT)PutRecvBuf(pTG, NULL, RECV_STARTBLOCK);

            pTG->ECM.fRecvEndOfPage = FALSE;
            if(!uRet)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Got Error from PutRecvBuf at StartPage/Block");
                return actionERROR;
            }
            pTG->EchoProtect.modePrevRecv = modeECM;
        }
        else
                pTG->EchoProtect.modePrevRecv = modeECMRETX;

        // set global flag if we got WRONGMODE
        pTG->EchoProtect.fGotWrongMode = (uRet==RECV_WRONGMODE);

        // elim flush--does no good & wastes 10ms
        // ModemFlush(pTG->Params.hModem);
        return actionGONODE_F;
        // try to get 300bps command instead
    }
    ExitPageCrit();

    // as soon as we get good carrier ZERO the pTG->EchoProtect state
    _fmemset(&pTG->EchoProtect, 0, sizeof(pTG->EchoProtect));

    // reset this flag AFTER we successfully recv LONG_TRAIN. We may get an
    // echo of our last command, go to NodeF, reject the echo and loop back
    // here. In that case we want to retry LONG train, otherwise we always
    // croak after a CTC
    pTG->ECM.fRecvdCTC = FALSE;

#ifdef IFAX
    BroadcastMessage(pTG, IF_PSIFAX_DATAMODE, (PSIFAX_RECV|PSIFAX_ECM|(fRetrans ? PSIFAX_RERECV : 0)), (uMod & (~ST_FLAG)));
#endif
    DebugPrintEx(DEBUG_MSG,"RECEIVING ECM Page.......");
    if(fRetrans)
        ICommStatus(pTG, T30STATR_RERECV_ECM, 0, 0, 0);

    // Turn yielding on *after* entering receive mode safely!
    FComCriticalNeg(pTG, FALSE);

/***
    switch(action = pTG->Params.lpfnWhatNext(eventSTARTRECV))
    {
      case actionCONTINUE:  break;
      case actionDCN:               ERRMSG((SZMOD "<<ERROR>> Got actionDCN from eventSTARTRECV\r\n"));
                                                    return actionDCN;               // goto NodeC;
      case actionHANGUP:    ERRMSG((SZMOD "<<ERROR>> Got actionHANGUP from eventSTARTRECV\r\n"));
                                                    return action;  // goto NodeB;
      case actionERROR:             return action;
      default:                              return BadAction(action);
    }
***/

    if(!fRetrans)
    {
        memset(pTG->ECM.bRecvBadFrameMask, 0xFF, 32);
        ProtResetRecvECMFrameCount(pTG);

        if(pTG->ECM.fRecvEndOfPage)  // as opposed to end-of-block
            uRet = (USHORT)PutRecvBuf(pTG, NULL, RECV_STARTPAGE);
        else
            uRet = (USHORT)PutRecvBuf(pTG, NULL, RECV_STARTBLOCK);
        if(!uRet)
        {
            DebugPrintEx(   DEBUG_ERR,
                            "Got Error from PutRecvBuf at StartPage/Block");
            return actionERROR;
        }
    }
    pTG->ECM.ifrPrevCommand = 0;
    pTG->ECM.fRecvEndOfPage = FALSE;
    pTG->ECM.ifrPrevResponse = 0;

#ifdef DEBUG
    if (fRetrans)
        D_PSIFAXCheckMask(pTG, pTG->ECM.bRecvBadFrameMask);
#endif


// make it large, in case of large buffers & slow modems
#define READ_TIMEOUT    25000

    lpbfShort=0;
    for(uFrameNum=0, lpbf=0, lTotalLen=0, uRCPCount=0, uRet=RECV_OK; uRet!=RECV_EOF;)
    {
        if(lpbf)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Freeing leftover Buf 0x%08lx inside loop",
                            lpbf);

            MyFreeBuf(pTG,  lpbf);        // need to free after bad frames etc
        }

        lpbf = 0;
        uRet=ModemRecvBuf(pTG, pTG->Params.hModem, TRUE, &lpbf, READ_TIMEOUT);

        if(uRet == RECV_BADFRAME)
        {
            DebugPrintEx(DEBUG_WRN, "ModemRecvBuf returns BADFRAME");
            continue;
        }
        else if(uRet == RECV_EOF)
        {
            BG_CHK(lpbf == 0);
            if(lpbfShort)
            {
                lpbf = lpbfShort;
                lpbfShort = 0;

                if(fRetrans && ((USHORT)(lpbf->lpbBegData[3])+1 < ProtGetRecvECMFrameCount(pTG)))
                {
                    DebugPrintEx(   DEBUG_WRN,
                                    "DISCARDING Short but not-really-terminal"
                                    " FCD frame(N=%d). len=%d. PADDING to %d!!",
                                    lpbf->lpbBegData[3],
                                    lpbf->wLengthData,
                                    uSize+4);
                    IFBufFree(lpbf);
                    break;
                }
                else
                {
                    DebugPrintEx(   DEBUG_WRN,
                                    "Short Terminal FCD frame(N=%d)."
                                    " len=%d. PADDING to %d!!",
                                    lpbf->lpbBegData[3],
                                    lpbf->wLengthData,
                                    uSize+4);
                    BG_CHK(lpbf->wLengthBuf >= uSize+4);
                    _fmemset(lpbf->lpbBegData+lpbf->wLengthData, 0, (uSize+4-lpbf->wLengthData));
                    lpbf->wLengthData = uSize+4;
                    goto skipchks;
                }
            }
            else
            {
                break;
            }
        }
        else if(uRet != RECV_OK)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Got %d from RecvBuf in ECMREcvPhaseC",
                            uRet);
            break;
        }
        else if(lpbf==0)
        {
            // sometimes we get RECV_OK with no data. Treat same as bad frame
            continue;
        }

        BG_CHK(uRet==RECV_OK && lpbf && lpbf->lpbBegData);

        if(uRCPCount >= 3)
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Got a good frame after %d RCP\r\n",
                            uRCPCount);
            continue;
        }

        if( lpbf->lpbBegData[0] != 0xFF ||
                lpbf->lpbBegData[1] != 0x03 ||
                (lpbf->lpbBegData[2] & 0x7F) != 0x06)
        {
                DebugPrintEx(   DEBUG_ERR,
                                "Bad frame (N=%d) not caught FCF=%02x!!",
                                uFrameNum,
                                lpbf->lpbBegData[2]);
                BG_CHK(FALSE);
                continue;
        }

        if(lpbf->lpbBegData[2] == 0x86)
        {
            // got RCP
            if(lpbf->wLengthData != 3)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Bad RCP frame len=%d",
                                lpbf->wLengthData);
                BG_CHK(FALSE);
                continue;
            }
            uRCPCount++;
            DebugPrintEx(DEBUG_MSG,"Got %d RCP", uRCPCount);
            continue;
        }

        if(lpbfShort)
        {
            DebugPrintEx(   DEBUG_ERR,
                            "Short FCD frame(N=%d). len=%d DISCARDING",
                            lpbfShort->lpbBegData[3],
                            lpbfShort->wLengthData);
            MyFreeBuf(pTG, lpbfShort);
            lpbfShort = NULL;
        }
        if(lpbf->wLengthData > (uSize+4))
        {
            DebugPrintEx(   DEBUG_ERR,
                            "FCD frame too long(N=%d %d). len=%d. DISCARDING",
                            uFrameNum,
                            lpbf->lpbBegData[3],
                            lpbf->wLengthData);
            continue;
        }
        else if(lpbf->wLengthData < (uSize+4))
        {
            DebugPrintEx(   DEBUG_WRN,
                            "Short FCD frame(N=%d %d). len=%d. Storing",
                            uFrameNum,
                            lpbf->lpbBegData[3],
                            lpbf->wLengthData);
            BG_CHK(lpbfShort==0);
            lpbfShort = lpbf;
            lpbf = NULL;
            continue;
        }

skipchks:
        BG_CHK(lpbf->lpbBegData[2] == 0x06);
        BG_CHK(lpbf->wLengthData == (uSize+4));

        uRecvFrame = lpbf->lpbBegData[3];
        lpbf->lpbBegData += 4;
        lpbf->wLengthData -= 4;

        if(!fRetrans)
        {
            if(uFrameNum > uRecvFrame)
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Out of order frame. Got %d Looking for %d",
                                uRecvFrame,
                                uFrameNum);
                BG_CHK(FALSE);
                // ignore this frame in non-debug mode
                continue;
            }
            else if(uFrameNum < uRecvFrame)
            {
                if(!FillInFrames(pTG, (USHORT)(uRecvFrame-uFrameNum), pTG->T30.sRecvBufSize, uSize))
                {
                    DebugPrintEx(   DEBUG_ERR,
                                    "Zero return from PutRecvBuf in FillInFrames");
                    return actionERROR;
                }
            }
            lTotalLen += lpbf->wLengthData;
        }
        uFrameNum = uRecvFrame;

        if(!fRetrans || (pTG->ECM.bRecvBadFrameMask[uFrameNum/8] & (1 << (uFrameNum%8))))
        {
            BG_CHK(uFrameNum < 256 && pTG->ECM.uFrameSize <=8); // shift below wont ovf 16 bits
            if(!PutRecvBuf(pTG, lpbf, (fRetrans ? ((SLONG)(uFrameNum << pTG->ECM.uFrameSize)) : RECV_SEQ)))
            {
                DebugPrintEx(DEBUG_ERR,"Zero return from PutRecvBuf in page");
                return actionERROR;
            }
            pTG->ECM.bRecvBadFrameMask[uFrameNum/8] &= ~(1 << (uFrameNum%8));
            uFrameNum++;
            lpbf = 0;
        }
    }


    if(lpbf)
    {
        DebugPrintEx(   DEBUG_ERR,
                        "Freeing leftover Buf 0x%08lx outside loop",
                        lpbf);
        MyFreeBuf(pTG, lpbf);        // need to free after bad frames etc
    }

    if(uRet == RECV_EOF)
    {
        FComCriticalNeg(pTG, TRUE);
        // cant mark end of page until we know if end of page or block etc.
        // PutRecvBuf(NULL, RECV_ENDPAGE);              // to mark end of Page
    }
    else
    {
        DebugPrintEx(   DEBUG_ERR,
                        "DataRead Timeout or Error=%d",
                        uRet);
        BG_CHK(FALSE);
        ICommFailureCode(pTG, T30FAILRE_MODEMRECV_PHASEC);
        return actionERROR;     // goto error;
    }

    DebugPrintEx(   DEBUG_MSG,
                    "ECM Page Recv Done.....len=(%ld, 0x%08x)",
                    lTotalLen,
                    lTotalLen);
    ECHOPROTECT(0, 0);
    return actionGONODE_F;  // goto NodeF;                  // get post-message command
}

BOOL FillInFrames(PThrdGlbl pTG, USHORT N, LONG sBufSize, USHORT uDataSize)
{
    USHORT i;
    LPBUFFER        lpbf;

    DEBUG_FUNCTION_NAME(_T("FillInFrames"));
    for(i=0; i<N; i++)
    {
#ifdef IFK
        TstartTimeOut(pTG, &pTG->T30.toBuf, WAITFORBUF_TIMEOUT);
        while(!(lpbf = MyAllocBuf(pTG, sBufSize)))
        {
            if(!TcheckTimeOut(pTG, &pTG->T30.toBuf))
            {
                DebugPrintEx(   DEBUG_ERR,
                                "Giving up on BufAlloc in T30-ECM after %ld millisecs",
                                ((ULONG)WAITFORBUF_TIMEOUT));
                BG_CHK(FALSE);
                return FALSE;
            }
            DebugPrintEx(   DEBUG_ERR,
                            "BufAlloc failed in T30-pTG->ECM. Trying again");
            IFProcSleep(100);
        }
#else
        if(!(lpbf = MyAllocBuf( pTG, sBufSize)))
            return FALSE;
#endif

        lpbf->wLengthData = uDataSize;

        if(!PutRecvBuf(pTG, lpbf, RECV_SEQBAD))
        {
            DebugPrintEx(DEBUG_ERR,"Zero return from PutRecvBuf in page");
            return FALSE;
        }
    }
    return TRUE;
}

#ifdef SWECM

USHORT ModemECMRecvMode(PThrdGlbl pTG, HMODEM h, USHORT uMod, ULONG ulTimeout)
{
    if(!SWECMRecvSetup(pTG, TRUE))
    {
        BG_CHK(FALSE);
        return RECV_ERROR;
    }
    return ModemRecvMode(pTG, h, uMod, FALSE, ulTimeout, ifrPIX_SWECM);
}

#endif //SWECM


