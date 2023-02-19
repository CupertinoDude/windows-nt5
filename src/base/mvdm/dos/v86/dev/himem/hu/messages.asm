;/* messages.asm
; *
; * Microsoft Confidential
; * Copyright (C) Microsoft Corporation 1988-1991
; * All Rights Reserved.
; *
; * Modification History
; *
; * Sudeepb 14-May-1991 Ported for NT XMS support
; */


        page    95,160
        title   himem3 - Initialization messages

        .xlist
        include himem.inc
        .list

;*----------------------------------------------------------------------*
;*      DRIVER MESSAGES                                                 *
;*----------------------------------------------------------------------*

        public  SignOnMsg
        public  ROMDisabledMsg
        public  UnsupportedROMMsg
        public  ROMHookedMsg
        public  BadDOSMsg
        public  NowInMsg
        public  On8086Msg
        public  NoExtMemMsg
        public  FlushMsg
        public  StartMsg
        public  HandlesMsg
        public  HMAMINMsg
        public  KMsg
        public  NoHMAMsg
        public  A20OnMsg
        public  HMAOKMsg
        public  InsA20Msg
        public  InsA20EndMsg
        public  InsExtA20msg
        public  NoA20HandlerMsg
        public  VDISKInMsg
        public  BadArgMsg
        public  EndText

; Start of text subject to translation
;  Material appearing in single quotation marks should be translated.


SignOnMsg db    13,10,'HIMEM: DOS XMS illeszt�program. Verzi�sz�m: '
        db      '0' + (HimemVersion shr 8),'.'
        db      '0' + ((HimemVersion and 0ffh) / 16)
        db      '0' + ((HimemVersion and 0ffh) mod 16)
        db      ' - '
        db      DATE_String
        db      13,10,'XMS Specification Version 2.0'
        db      13,10,'Copyright 1988-1991 Microsoft Corp.'
        db      13,10,'$'

ROMDisabledMsg    db    13,10,  'Shadow RAM letiltva.$'
UnsupportedROMMsg db    13,10,'FIGYELMEZTET�S: A shadow RAM letilt�sa nem t�mogatott '
                  db            'ezen a sz�m�t�g�pen.$'
ROMHookedMsg      db    13,10,'FIGYELMEZTET�S: A shadow RAM haszn�latban van, ez�rt '
                  db            'nem lehet letiltani.$'

BadDOSMsg       db      13,10,'HIBA: ez a HIMEM.SYS Windows NT-hez haszn�lhat�.$'
NowInMsg        db      13,10,'HIBA: a kiterjesztett mem�riakezel� m�r telep�tve van.$'
On8086Msg       db      13,10,'HIBA: a HIMEM.SYS haszn�lat�hoz 80x86 alap� g�p sz�ks�ges.$'
NoExtMemMsg     db      13,10,'HIBA: nem tal�lhat� szabad kiterjesztett mem�ria.$'
NoA20HandlerMsg db      13,10,'HIBA: az A20 c�mvezet�k nem vez�relhet�!$'
VDISKInMsg      db      13,10,'HIBA: a VDISK mem�riafoglal� m�r telep�tve van.$'
FlushMsg        db      13,10,7,'      az XMS illeszt�program telep�t�se megszak�tva.',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' szabad kiterjesztettmem�ria-le�r�.$'
HMAMINMsg       db      13,10,'A HMA minim�lis m�rete: $'
KMsg            db      'K.$'
InsA20Msg       db      13,10,'A telep�tett A20 kezel� sz�ma $'
InsA20EndMsg    db      '.$'
InsExtA20Msg    db      13,10,'Telep�tett k�ls� A20 kezel�.$'

NoHMAMsg        db      13,10,'FIGYELEM: a Fels� mem�riater�let nem �rhet� el.'
                db      13,10,'$'
A20OnMsg        db      13,10,'FIGYELEM: az A20 c�mvezet�k m�r enged�lyezve van.'
                db      13,10,'$'

BadArgMsg       db      13,10,'FIGYELEM: Hib�s param�ter (figyelmen k�v�l hagyva): $'

HMAOKMsg        db      13,10,'64K szabad fels� mem�ria.'
                db      13,10,13,10,'$'

                db      'A program a Microsoft Corporation tulajdona.'

; end of material subject to translation


EndText         label   byte
_text   ends
        end
