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
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  A DOS v�dett m�d� b�v�t�je CPU hib�t �szlelt.',13,10
                db      13,10,'$'
;
; Can't figure out how to get into protected mode.
;
ER_PROTMODE     db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  Az alkalmaz�s �tk�zik egy m�sik v�dett m�d�  ',13,10
                db      '  programmal.',13,10
                db      13,10,'$'
;
; Couldn't initialize XMS driver.
;
ER_NOHIMEM      db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  A DOS v�dett m�d� b�v�t� nem tudta alaphelyzetbe hozni a ',13,10
                db      '  kiterjesztettmem�ria-kezel�t.',13,10
                db      13,10,'$'
;
; Non-specific unable to initialize DOSX error.
;
ER_DXINIT       db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  A DOS v�dett m�d� b�v�t� meghat�rozatlan hib�t �szlelt.'
                db      13,10,'$'
;
; A DOS memory allocation failed.
;
ER_REALMEM      db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  Nincs elegend� hagyom�nyos mem�ria.',13,10,13,10
                db      13,10,'$'
;
; Couldn't get enough extended memory to run.
;
ER_EXTMEM       db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  Nincs elegend� kiterjesztett mem�ria.',13,10,13,10
                db      13,10,'$'
;
; Where is KRNL[23]86.EXE!!!
;
ER_NOEXE        db      13,10
                db      '  Ez a 16 bites v�dett m�d� alkalmaz�s nem futtathat�;',13,10,13,10
                db      '  DOS v�dett m�d� b�v�t� nem tal�lja a futtat�shoz sz�ks�ges rendszerf�jlokat.',13,10,13,10
                db      13,10,'$'

if      VCPI
;
; VCPI initialization failed.
;
ER_VCPI         db      13,10
                db      '  A mem�riakezel� hib�ja miatt a programot nem lehet Standard �zemm�dban futtatni.'
                db      13,10,'$'
endif   ;VCPI

if      VCPI
;
; This message is displayed if someone fails the Windows INT 2Fh startup
; broadcast.  All of the "Windows 3.0 compatible" 3rd party memory managers
; do this.
;
ER_QEMM386      db      13,10
                db      '            Az illeszt�program vagy TSR nem engedi, hogy a Windows '
                db      13,10
                db      '            standard �zemm�dban t�lt�dj�n be. T�vol�tsa el a programot, vagy '
                db      13,10
                db      '            szerezze be egy �jabb v�ltozat�t, amely kompat�bilis a '
                db      13,10
                db      '            Windows standard �zemm�dj�val.'
                db      13,10
                db      13,10
                db      '            Az "y" billenty�t le�tve a Windows standard m�dban indul.'
                db      13,10
                db      13,10
                db      '            B�rmely m�s billenty�t le�tve visszajut a DOS-hoz.'
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
                db      '  DOS v�dett m�d� b�v�t�: nem kezelt v�dett m�d� kiv�tel.',13,10,'$'

        public  szRing0FaultMessage
;
; Fault in the DOSX internal fault handler.  Not recoverable.
;
; Note:  This is for a real bad one.
;
szRing0FaultMessage     db      13,10
                db      ' DOS v�dett m�d� b�v�t�: bels� hiba.',13,10,'$'

DXPMCODE ends

        end
