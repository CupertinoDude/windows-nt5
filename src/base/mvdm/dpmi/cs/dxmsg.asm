        PAGE    ,132
        TITLE   DXMSG.ASM  -- Dos Extender Text Messages

; Copyright (c) Microsoft Corporation 1989-1991. All Rights Reserved.

;***********************************************************************
;
;       DXMSG.ASM      -- Dos Extender Text Messages
;
;-----------------------------------------------------------------------
;
; This module contains the text messages displayed by the 80286 DOS
; Extender.  The messages are contained in this file to ease their
; conversion to other languages.
;
;-----------------------------------------------------------------------
;
;  12/06/89 jimmat  Update message text as per User Ed
;  08/03/89 jimmat  Original version
;
;***********************************************************************

        .286p

; -------------------------------------------------------
;           INCLUDE FILE DEFINITIONS
; -------------------------------------------------------

        .xlist
        .sall
include     segdefs.inc
include     gendefs.inc
        .list

; -------------------------------------------------------
;           CODE SEGMENT VARIABLES
; -------------------------------------------------------

DXCODE  segment

; Note: these DXCODE segment messages are all linked after the CodeEnd
; variable, so they will be discarded after initialization.

        public  ER_CPUTYPE, ER_PROTMODE, ER_NOHIMEM, ER_DXINIT, ER_REALMEM
        public  ER_EXTMEM, ER_NOEXE

if      VCPI
        public ER_VCPI, ER_QEMM386
endif   ;VCPI
;
; Wrong CPU type.
;
ER_CPUTYPE      db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Roz��en� syst�m DOS detekoval neshodn� procesory (CPU).',13,10
                db      13,10,'$'
;
; Can't figure out how to get into protected mode.
;
ER_PROTMODE     db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Roz��en� syst�m DOS detekoval konflikt s dal�m softwarem ',13,10
                db      '  pro chr�n�n� re�im.',13,10
                db      13,10,'$'
;
; Couldn't initialize XMS driver.
;
ER_NOHIMEM      db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Roz��en� syst�m DOS narazil na chybu p�i inicializaci spr�vce ',13,10
                db      '  roz��en� pam�ti.',13,10
                db      13,10,'$'
;
; Non-specific unable to initialize DOSX error.
;
ER_DXINIT       db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Roz��en� syst�m DOS narazil na nespecifikovanou chybu.'
                db      13,10,'$'
;
; A DOS memory allocation failed.
;
ER_REALMEM      db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Nen� dostatek konven�n� pam�ti.',13,10,13,10
                db      13,10,'$'
;
; Couldn't get enough extended memory to run.
;
ER_EXTMEM       db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Nen� dostatek roz��en� pam�ti.',13,10,13,10
                db      13,10,'$'
;
; Where is KRNL[23]86.EXE!!!
;
ER_NOEXE        db      13,10
                db      '  Tuto aplikaci pro 16-bitov� chr�n�n� re�im nelze spustit;',13,10,13,10
                db      '  Roz��en� syst�m DOS nenalezl pot�ebn� syst�mov� soubory.',13,10,13,10
                db      13,10,'$'

if      VCPI
;
; VCPI initialization failed.
;
ER_VCPI         db      13,10
                db      '  Nebylo mo�n� spustit ve standardn�m re�imu: Probl�m spr�vce pam�ti.'
                db      13,10,'$'
endif   ;VCPI

if      VCPI
;
; This message is displayed if someone fails the Windows INT 2Fh startup
; broadcast.  All of the "Windows 3.0 compatible" 3rd party memory managers
; do this.
;
ER_QEMM386      db      13,10
                db      '            Ovlada� za��zen� �i program TSR vy�aduje, aby se syst�m Windows'
                db      13,10
                db      '            ve standardn�m re�imu nespustil nyn�. Bu� tento program odstra�te,'
                db      13,10
                db      '            nebo si od dodavatele vy��dejte aktualizaci kompatibiln�'
                db      13,10
                db      '            se standardn�m re�imem syst�mu Windows.'
                db      13,10
                db      13,10
                db      '            Stiskem "y" m��ete spustit Windows ve standardn�m re�imu.'
                db      13,10
                db      13,10
                db      '            Jakoukoliv jinou kl�vesou se vr�t�te do syst�mu DOS.'
                db      13,10,'$'
endif   ;VCPI

DXCODE  ends

DXPMCODE segment
;
; Both of the next two messages probably mean a serious crash in Windows.
;
        public  szFaultMessage
;
; Displayed if a protected mode fault is caught by DOSX.
;
szFaultMessage  db      13,10
                db      '  Roz��en� DOS: Nezachycen� vyj�mka chr�n�n�ho re�imu.',13,10,'$'

        public  szRing0FaultMessage
;
; Fault in the DOSX internal fault handler.  Not recoverable.
;
; Note:  This is for a real bad one.
;
szRing0FaultMessage     db      13,10
                db      '  Roz��en� DOS: Intern� chyba.',13,10,'$'

DXPMCODE ends

        end
