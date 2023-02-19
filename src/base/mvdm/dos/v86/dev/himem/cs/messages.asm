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


SignOnMsg db    13,10,'HIMEM: Ovlada� DOS XMS, verze '
        db      '0' + (HimemVersion shr 8),'.'
        db      '0' + ((HimemVersion and 0ffh) / 16)
        db      '0' + ((HimemVersion and 0ffh) mod 16)
        db      ' - '
        db      DATE_String
        db      13,10,'Specifikace XMS, verze 2.0'
        db      13,10,'Copyright 1988-1991 Microsoft Corp.'
        db      13,10,'$'

ROMDisabledMsg    db    13,10,  'St�nov� pam؜ RAM je vypnuta.$'
UnsupportedROMMsg db    13,10,'UPOZORN�N�: Vypnut� st�nov� pam�ti RAM nen� '
                  db            'na tomto syst�mu podporov�no.$'
ROMHookedMsg      db    13,10,'UPOZORN�N�: St�nov� pam؜ RAM je pou��v�na '
                  db            'a nelze ji vypnout.$'

BadDOSMsg       db      13,10,'CHYBA: Tato verye HIMEM.SYS je ur�ena pro syst�m Windows NT.$'
NowInMsg        db      13,10,'CHYBA: Spr�vce roz��en� pam�ti je ji� nainstalov�n.$'
On8086Msg       db      13,10,'CHYBA: HIMEM.SYS vy�aduje po��ta� na b�zi 80x86.$'
NoExtMemMsg     db      13,10,'CHYBA: Nebyla nalezena voln� roz��en� pam؜.$'
NoA20HandlerMsg db      13,10,'CHYBA: Nelze ovl�dnout linku A20!$'
VDISKInMsg      db      13,10,'CHYBA: Alok�tor pam�ti VDISK je ji� nainstalov�n.$'
FlushMsg        db      13,10,7,'       Nen� nainstalov�n ovlada� XMS.',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' popisova�� roz��en� pam�ti k dispozici.$'
HMAMINMsg       db      13,10,'Minim�ln� velikost HMA nastavena na $'
KMsg            db      'kB.$'
InsA20Msg       db      13,10,'Nainstalov�n ovlada� linky A20 ��slo $'
InsA20EndMsg    db      '.$'
InsExtA20Msg    db      13,10,'Nainstalov�n extern� ovlada� linky A20.$'

NoHMAMsg        db      13,10,'UPOZORN�N�: Horn� oblast pam�ti (HMA) je nedostupn�.'
                db      13,10,'$'
A20OnMsg        db      13,10,'UPOZORN�N�: Linka A20 ji� byla aktivov�na.'
                db      13,10,'$'

BadArgMsg       db      13,10,'UPOZORN�N�: Neplatn� parametr bude ignorov�n: $'

HMAOKMsg        db      13,10,'Je voln�ch 64 kB horn� oblasti pam�ti (HMA).'
                db      13,10,13,10,'$'

                db      'Tento program je majetkem spole�nosti Microsoft Corporation.'

; end of material subject to translation


EndText         label   byte
_text   ends
        end
