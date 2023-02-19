//-----------------------------------------------------------------------------
// This files contains the module name for this mini driver.  Each mini driver
// must have a unique module name.  The module name is used to obtain the
// module handle of this Mini Driver.  The module handle is used by the
// generic library to load in tables from the Mini Driver.
// 
// Copyright (C) 1994-1995 Microsoft Corporation
// Copyright (C) 1995      Advanced Peripherals Technologies, Inc.
//-----------------------------------------------------------------------------

char *rgchModuleName = "PAGESMS";

#define PRINTDRIVER
#include <print.h>
#include "mdevice.h"
#include "gdidefs.inc"
#include "unidrv.h"
#include <memory.h>

#ifndef _INC_WINDOWSX
#include <windowsx.h>
#endif

#define CCHMAXCMDLEN    128
#define MAXIMGSIZE      0x7FED   /* GDI����CBFilterGraphics�ɑ����Ă���    */
                                 /* �ް���1ײݕ������A�Ƃ肠����ESX86�ő��M  */
                                 /* �\�ȍő�IMAGE���ނ�p�ӂ��Ă����B      */
                                 /* 0x7FFF - 18 =  7FED byte                 */

/*_ �޲����ݸ޽ ���kٰ�� */
extern WORD FAR PASCAL RL_ECmd(LPBYTE, LPBYTE, WORD);
/*_ ���ݸ޽4 ���kٰ�� */
extern WORD FAR PASCAL RL4_ECmd (LPBYTE, LPBYTE, WORD, WORD, WORD);

typedef struct
{
    BYTE  fGeneral;       // General purpose bitfield
    BYTE  bCmdCbId;       // Callback ID; 0 iff no callback
    WORD  wCount;         // # of EXTCD structures following
    WORD  wLength;        // length of the command
} CD, *PCD, FAR * LPCD;

typedef struct tagPAGES {
    short sHorzRes;
    short sVertRes;
    LPSTR  lpCompBuf;     // ���k�ް���ޯ̧
} PAGES, FAR * LPPAGES;

static BYTE ShiftJisPAGES[256] = {
//     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //00
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //20
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //30
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //40
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //50
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //60
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //70
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //80
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //90
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //A0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //B0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //C0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //D0
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //E0
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0   //F0
};

static BYTE ESC_VERT_ON[]  = "\x1B\x7E\x0E\x00\x01\x0B";
static BYTE ESC_VERT_OFF[] = "\x1B\x7E\x0E\x00\x01\x0C";

short usHorzRes;
short usVertRes;

#ifndef WINNT
BYTE NEAR __loadds IsDBCSLeadBytePAGES(BYTE Ch)
#else
BYTE NEAR IsDBCSLeadBytePAGES(BYTE Ch)
#endif
{
return ShiftJisPAGES[Ch];
}

#ifdef WINNT
LPWRITESPOOLBUF WriteSpoolBuf;
LPALLOCMEM UniDrvAllocMem;
LPFREEMEM UniDrvFreeMem;
#endif // WINNT

//---------------------------*OEMSendScalableFontCmd*--------------------------
// Action:  send Pages-style font selection command.
//-----------------------------------------------------------------------------
VOID FAR PASCAL OEMSendScalableFontCmd(lpdv, lpcd, lpFont)
LPDV    lpdv;
LPCD    lpcd;     // offset to the command heap
LPFONTINFO lpFont;
{
    LPSTR   lpcmd;
    short   ocmd;
    WORD    i;
    BYTE    rgcmd[CCHMAXCMDLEN];    // build command here
    LPPAGES lpPages = lpdv->lpMdv;

    if (!lpcd || !lpFont)
        return;

    // be careful about integer overflow.
    lpcmd = (LPSTR)(lpcd+1);
    ocmd = 0;

    for (i = 0; i < lpcd->wLength && ocmd < CCHMAXCMDLEN; )
        if (lpcmd[i] == '#' && lpcmd[i+1] == 'V')      // height
        {
            WORD    height;

            height = (lpFont->dfPixHeight - lpFont->dfInternalLeading)
                    * (1440 / lpFont->dfVertRes);
            rgcmd[ocmd++] = HIBYTE(height);
            rgcmd[ocmd++] = LOBYTE(height);
            i += 2;
        }
        else if (lpcmd[i] == '#' && lpcmd[i+1] == 'L')     // pitch
        {
            WORD    height;

            height = lpFont->dfPixHeight * (1440 / lpFont->dfVertRes);
            rgcmd[ocmd++] = HIBYTE(height);
            rgcmd[ocmd++] = LOBYTE(height);
            i += 2;
        }
        else if (lpcmd[i] == '#' && lpcmd[i+1] == 'H')     // width
        {
            if (lpFont->dfPixWidth > 0)
            {
                short tmpWidth;

                tmpWidth = lpFont->dfMaxWidth * (1440 / lpFont->dfVertRes);

                rgcmd[ocmd++] = HIBYTE(tmpWidth);
                rgcmd[ocmd++] = LOBYTE(tmpWidth);
            }
            i += 2;
            
        }
        else if (lpcmd[i] == '#' && lpcmd[i+1] == 'P')     // pitch
        {
            if (lpFont->dfPixWidth > 0)
            {
                short sWidth = (lpFont->dfMaxWidth * (1440/lpPages->sHorzRes));

                rgcmd[ocmd++] = HIBYTE(sWidth);
                rgcmd[ocmd++] = LOBYTE(sWidth);
            }
            i += 2;
        }
        else
            rgcmd[ocmd++] = lpcmd[i++];

    WriteSpoolBuf(lpdv, (LPSTR) rgcmd, ocmd);
}

//----------------------------*OEMScaleWidth*--------------------------------
// Action: return the scaled width which is calcualted based on the
//      assumption that ESC\Page assumes 72 points in one 1 inch.
//
// Formulas:
//  <extent> : <font units> = <base Width> : <hRes>
//  <base width> : <etmMasterHeight> = <newWidth> : <newHeight>
//  <etmMasterUnits> : <etmMasterHeight> = <font units> : <vRes>
// therefore,
//   <newWidth> = (<extent> * <hRes> * <newHeight>) / 
//                  (<etmMasterUnits> * <vRes>)
//---------------------------------------------------------------------------
short FAR PASCAL OEMScaleWidth(width, masterUnits, newHeight, vRes, hRes)
short width;        // in units specified by 'masterUnits'.
short masterUnits;
short newHeight;    // in units specified by 'vRes'.
short vRes, hRes;   // height and width device units.
{
    DWORD newWidth10;
    short newWidth;

    // assert that hRes == vRes to avoid overflow problem.
    if (vRes != hRes)
        return 0;

    newWidth10 = (DWORD)width * (DWORD)newHeight * 10;
    newWidth10 /= (DWORD)masterUnits;

    // we multiplied 10 first in order to maintain the precision of
    // the width calcution. Now convert it back and round to the
    // nearest integer.
    newWidth = (short)((newWidth10 + 5) / 10);

    return newWidth;
}

#ifndef WINNT
short FAR PASCAL __loadds OEMOutputChar( lpdv, lpstr, len, rcID)
#else
short FAR PASCAL OEMOutputChar( lpdv, lpstr, len, rcID)
#endif
LPDV	lpdv;
LPSTR	lpstr;
short	len;
short	rcID;
{

    short rSize = 0;

    if (rcID == 6 || rcID == 8)
    {
        LPSTR lpChar = lpstr, 
              lpStrTmp = lpstr;
        WORD  fLeadByteFlag = TRUE;
        int   i, j;

        for (i = 0,j = 0; i < len; j ++, i++, lpChar++)
        { 
            if (!IsDBCSLeadBytePAGES((BYTE)*lpChar)) // SBCS
            {
                if (fLeadByteFlag)
                    {
                    WriteSpoolBuf(lpdv, lpStrTmp, j);
                    WriteSpoolBuf(lpdv, ESC_VERT_OFF, sizeof(ESC_VERT_OFF));
                    lpStrTmp += j;
                    j = 0;
                    fLeadByteFlag = FALSE;
                    rSize += sizeof(ESC_VERT_OFF);
                    }
            }
            else                                 // DBCS
            {
                if (!fLeadByteFlag)
                    {
                    WriteSpoolBuf(lpdv,  lpStrTmp, j);
                    WriteSpoolBuf(lpdv, ESC_VERT_ON, sizeof(ESC_VERT_ON));
                    lpStrTmp += j;
                    j = 0;
                    fLeadByteFlag = TRUE;
                    rSize += sizeof(ESC_VERT_ON);
                    }
                j ++; i++; lpChar++;
            }
        } 
        WriteSpoolBuf(lpdv, lpStrTmp, j);
    }
    else
        WriteSpoolBuf(lpdv, lpstr, len);

    return len+rSize;

}

#ifndef WINNT
short FAR PASCAL Enable( lpdv, style, lpModel, lpPort, lpStuff)
LPDV    lpdv;
WORD    style;
LPSTR   lpModel;
LPSTR   lpPort;
LPDM    lpStuff;
{
    CUSTOMDATA      cd;
    short           sRet;
    LPPAGES         lpPages;


    cd.cbSize               = sizeof( CUSTOMDATA );
    cd.hMd                  = GetModuleHandle( (LPSTR)rgchModuleName );
    cd.fnOEMDump            = NULL;
    cd.fnOEMOutputChar      = (LPFNOEMOUTPUTCHAR)OEMOutputChar;

    // In order to the Style vlalue, following process is performed.
    //      0x0000 Initialize device block.
    //      0x0001 Inquire Device GDIINFO.
    //      0x8000 Initialize device block without output. CreateIC()
    //      0x8001 Inquire Device GDIINFO without output.  CreateIC()

    sRet = UniEnable( lpdv, style, lpModel, lpPort, lpStuff, &cd );

    if (style == 0x0000)
    {
        lpdv->fMdv = FALSE;
        if (!(lpPages = lpdv->lpMdv = GlobalAllocPtr(GHND,sizeof(PAGES))))
        {
            UniDisable( lpdv );
            return FALSE;
        }
        lpdv->fMdv = TRUE;

        lpPages->sHorzRes = usHorzRes;
        lpPages->sVertRes = usVertRes;
        
    } else 
    if( style == 0x0001)
    {
        //INQUIREINFO
        usHorzRes = ((LPGDIINFO)lpdv)->dpAspectX;
        usVertRes = ((LPGDIINFO)lpdv)->dpAspectY;

    }

   return sRet;
}

//-------------------------------------------------------------------
// Function: Disable()
// Action  : free Mdv and call Mdv
//-------------------------------------------------------------------
void FAR PASCAL Disable(lpdv)
LPDV lpdv;
{
    if (lpdv->fMdv)
    {
        GlobalFreePtr (lpdv->lpMdv);
        lpdv->fMdv = FALSE;
    }

    UniDisable(lpdv);
}
#else //WINNT
/*************************** Function Header *******************************
 *  MiniDrvEnablePDEV
 *
 * HISTORY:
 *  30 Apl 1996    -by-    Sueya Sugihara    [sueyas]
 *      Created it,  from NT/DDI spec.
 *
 ***************************************************************************/
BOOL
MiniDrvEnablePDEV(
LPDV      lpdv,
ULONG    *pdevcaps)
{
    LPPAGES   lpPages;


        usHorzRes = (short)((PGDIINFO)pdevcaps)->ulAspectX;
        usVertRes = (short)((PGDIINFO)pdevcaps)->ulAspectY;

        lpdv->fMdv = FALSE;
        if (!(lpPages = lpdv->lpMdv = UniDrvAllocMem(sizeof(PAGES))))
        {
            return FALSE;
        }
        if (!(lpPages->lpCompBuf = UniDrvAllocMem(MAXIMGSIZE)))
        {
            return FALSE;
        }
        lpdv->fMdv = TRUE;

        lpPages->sHorzRes = usHorzRes;
        lpPages->sVertRes = usVertRes;


    return TRUE;


}
/*************************** Function Header *******************************
 *  MiniDrvDisablePDEV
 *
 * HISTORY:
 *  30 Apl 1996    -by-    Sueya Sugihara    [sueyas]
 *      Created it,  from NT/DDI spec.
 *
 ***************************************************************************/
VOID
MiniDrvDisablePDEV(
LPDV lpdv)
{

    if (lpdv->fMdv)
    {
        UniDrvFreeMem(((LPPAGES)(lpdv->lpMdv))->lpCompBuf);
        UniDrvFreeMem(lpdv->lpMdv);
        lpdv->fMdv = FALSE;
    }

}

#endif //WINNT


/*f***************************************************************************/
/*  PAGES PRINTER DRIVER for MS-Windows95                                    */
/*                                                                           */
/*  ���́F  CBFilterGraphics                                                 */
/*                                                                           */
/*  �@�\�F  �Ұ���ް���ESX86����ނ��g�p���ďo�͂���B                        */
/*                                                                           */
/*  �����F  WORD FAR PASCAL CBFilterGraphics(lpdv, lpBuf, wLen)              */
/*                                                                           */
/*  ���́F  LPDV     lpdv      UNIDRV.DLL���g�p����PDEVICE�\����             */
/*          LPSTR    lpBuf     ׽�����̨����ް����ޯ̧���߲��                */
/*          WORD     wLen      lpBuf�̻���(�޲Đ�)                           */
/*                                                                           */
/*                                                                           */
/*  �o�́F  return             �o�͂����޲Đ�  �@                            */
/*                                                                           */
/*  ���L�F                                                                   */
/*                                                                           */
/*  �����F  1995.11.xx  Ver 1.00                                             */
/*****************************************************************************/
WORD FAR PASCAL CBFilterGraphics (lpdv, lpBuf, wLen)
LPDV      lpdv;     // Points to private data required by the Unidriver.dll
LPSTR     lpBuf;    // points to buffer of graphics data
WORD      wLen;     // length of buffer in bytes

{  
    LPSTR   lpCompImage;
    WORD    wCompLen;
    LONG    lHorzPixel;
    WORD    wLength;      // Let's use a temporary LEN
    LPPAGES lpPages = lpdv->lpMdv;

//#define MAXIMGSIZE  0x7FED          // 32K-18 bytes
static BYTE params[] = {(0x1B), (0x7E), (0x86), 00,00, 01, 00, 00,00,00,00, 00,00,00,00, 00,00,00,00, 00,00,00,01};


    /*_ LPDV�ɕۊǂ����Ұ���ޯ̧���߲����� */
    lpCompImage = lpPages->lpCompBuf;

    /*_ �Ұ�ނ�i�������̻���ISIZ���v�Z����B */
    lHorzPixel = (LONG)(wLen * 8);

    /*_ �C���[�W�f�[�^��Btye Run Length Algorithm�ň��k�B */
    wCompLen = RL_ECmd((LPBYTE)lpBuf, (LPBYTE)lpCompImage, wLen);

    /*_ ���k����ް��̻��ނ�ESX86��LEN�ȊO�����Ұ����̻��ނ�������B */
    wLength = wCompLen + 18;

    /*_ ESX86�R�}���h��LEN���Z�b�g�B */
    params[3] = (BYTE) (wLength >>8 & 0x00ff);    // get higher byte
    params[4] = (BYTE) (wLength & 0x00ff);        // get lower byte

    /*_ ���k���@�̃p�����[�^��Byte Run Length�ɃZ�b�g�B */
    params[6] = 0x02;

    /*_ ESX86�R�}���h��ISIZ���Z�b�g�B */
    params[17] = (BYTE) (lHorzPixel >> 8 & 0x000000ffL);   // get ISZ higher byte
    params[18] = (BYTE) (lHorzPixel & 0x000000ffL);        // get ISZ lower byte

    /*_ ESX86�R�}���h���X�v�[���o�͂���B */
    WriteSpoolBuf((LPDV)lpdv, (LPSTR)params, 23);

    /*_ ���k���ꂽ�f�[�^���X�v�[���o�͂���B */
    WriteSpoolBuf((LPDV)lpdv, lpCompImage, wCompLen);

    return wLen;
}

// The following is implemented in MiniDrvEnablePDEV/DisablePDEV
// on NT-J.  We do not simulate Control DDI call, and it is not
// guranteed that STARTOC, etc. always corresponds to minidriver
// enable/disable.

#ifndef WINNT

/*f***************************************************************************/
/*  PAGES PRINTER DRIVER for MS-Windows95                                    */
/*                                                                           */
/*  ���́F  Control                                                          */
/*                                                                           */
/*  �@�\�F  Calls Escape function from applications		                     */
/*                                                                           */
/*  �����F  short FAR PASCAL Control(lpdv, nFunction,                        */
/*                                             lpInData, lpOutData)          */
/*                                                                           */
/*  ���́F  LPDV        lpdv        PDEVICE structure                        */
/*          WORD        function    Subfunction ID                           */
/*          LPSTR       lpInData    Input data                               */
/*          LPSTR       lpOutData   Output data                              */
/*                                                                           */
/*  �o�́F  short ret   Positive :    Normal exit                            */
/*                      Negative :    Error exit                             */
/*                      FALSE  :      No escape subfunction                  */
/*                                                                           */
/*  ���L�F  nFunction and Escape numbers are the same                        */
/*                                                                           */
/*  �����F  1995.12.xx  Ver 1.00                                             */
/*                                                                           */
/*****************************************************************************/
short FAR PASCAL Control(LPDV  lpdv,
                         WORD  function,
                         LPSTR lpInData,
                         LPSTR lpOutData)
{
    LPPAGES lpPages = lpdv->lpMdv;

    switch (function)
    {
        /*_ STARTDOC�̂Ƃ��́A���k�ް��p���ޯ̧���m�ۂ���B */
        case STARTDOC :
            lpPages->lpCompBuf = GlobalAllocPtr(GHND,MAXIMGSIZE);
            break;

        /*_ ABORTDOC,ENDDOC�̂Ƃ��́A���k�ް��p���ޯ̧���������B */
        case ABORTDOC :
        case ENDDOC :
            GlobalFreePtr (lpPages->lpCompBuf);
            break;
    }

    /*_ UNIDRV��Control DDI��� */
    return UniControl(lpdv, function, lpInData, lpOutData);
}
#endif //!WINNT


#ifdef WINNT
DRVFN  MiniDrvFnTab[] =
{
    {  INDEX_MiniDrvEnablePDEV,       (PFN)MiniDrvEnablePDEV  },
    {  INDEX_MiniDrvDisablePDEV,      (PFN)MiniDrvDisablePDEV  },
    {  INDEX_OEMWriteSpoolBuf,        (PFN)CBFilterGraphics  },
    {  INDEX_OEMSendScalableFontCmd,  (PFN)OEMSendScalableFontCmd  },
    {  INDEX_OEMScaleWidth1,          (PFN)OEMScaleWidth  },
    {  INDEX_OEMOutputChar,           (PFN)OEMOutputChar  }
};

BOOL
MiniDrvEnableDriver(
    MINIDRVENABLEDATA  *pEnableData
    )
{
    if (pEnableData == NULL)
        return FALSE;

    if (pEnableData->cbSize == 0)
    {
        pEnableData->cbSize = sizeof (MINIDRVENABLEDATA);
        return TRUE;
    }

    if (pEnableData->cbSize < sizeof (MINIDRVENABLEDATA)
            || HIBYTE(pEnableData->DriverVersion)
            < HIBYTE(MDI_DRIVER_VERSION))
    {
        // Wrong size and/or mismatched version
        return FALSE;
    }

    // Load callbacks provided by the Unidriver

    if (!bLoadUniDrvCallBack(pEnableData,
            INDEX_UniDrvWriteSpoolBuf, (PFN *) &WriteSpoolBuf)
        ||!bLoadUniDrvCallBack(pEnableData,
            INDEX_UniDrvAllocMem, (PFN *) &UniDrvAllocMem)
        ||!bLoadUniDrvCallBack(pEnableData,
            INDEX_UniDrvFreeMem, (PFN *) &UniDrvFreeMem))
    {
        return FALSE;
    }

    pEnableData->cMiniDrvFn
        = sizeof (MiniDrvFnTab) / sizeof(MiniDrvFnTab[0]);
    pEnableData->pMiniDrvFn = MiniDrvFnTab;

    return TRUE;
}

#endif //WINNT

