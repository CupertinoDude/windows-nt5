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
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      ' DOS uzat�c�s� bir CPU uyu�mazl��� alg�lad�.',13,10
                db      13,10,'$'
;
; Can't figure out how to get into protected mode.
;
ER_PROTMODE     db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      ' DOS uzat�c�s� di�er korumal� yaz�l�mla ',13,10
                db      ' bir �ak��ma alg�lad�.',13,10
                db      13,10,'$'
;
; Couldn't initialize XMS driver.
;
ER_NOHIMEM      db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      ' DOS uzat�c�s� uzat�lm�� bellek y�neticisini ba�lat�rken bir',13,10
                db      '  hatayla kar��la�t�.',13,10
                db      13,10,'$'
;
; Non-specific unable to initialize DOSX error.
;
ER_DXINIT       db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      '  DOS uzat�c�s� belirli olmayan bir hatayla kar��la�t�.'
                db      13,10,'$'
;
; A DOS memory allocation failed.
;
ER_REALMEM      db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      '  Yeterli geleneksel bellek yok.',13,10,13,10
                db      13,10,'$'
;
; Couldn't get enough extended memory to run.
;
ER_EXTMEM       db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      '  Yeterli uzat�lm�� bellek yok.',13,10,13,10
                db      13,10,'$'
;
; Where is KRNL[23]86.EXE!!!
;
ER_NOEXE        db      13,10
                db      '  Bu 16-bit korumal� kip uygulamas� �al��t�r�lam�yor;',13,10,13,10
                db      '  DOS uzat�c�s� �al��t�rmak i�in gerekli sistem dosyalar�n� bulamad�.',13,10,13,10
                db      13,10,'$'

if      VCPI
;
; VCPI initialization failed.
;
ER_VCPI         db      13,10
                db      '  Bellek y�neticisi sorunu nedeniyle Standart Kip''te �al��t�r�lam�yor.'
                db      13,10,'$'
endif   ;VCPI

if      VCPI
;
; This message is displayed if someone fails the Windows INT 2Fh startup
; broadcast.  All of the "Windows 3.0 compatible" 3rd party memory managers
; do this.
;
ER_QEMM386      db      13,10
                db      '            Bir ayg�t s�r�c�s� veya TSR �imdi Standart Kipte'
                db      13,10
                db      '            Windows y�klenmemesini istedi.  Bu program� kald�r�n veya'
                db      13,10
                db      '            sa�lay�c�n�zdan Standart Kip Windows ile uyumlu bir'
                db      13,10
                db      '            g�ncelle�tirme edinin.'
                db      13,10
                db      13,10
                db      '            Yine de Standart Kip Windows''u y�klemek i�in "e"ye bas�n.'
                db      13,10
                db      13,10
                db      '            DOS''a d�nmek i�in herhangi bir tu�a bas�n.'
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
                db      '  DOS Uzat�c�s�: Yakalanmam�� korumal� kip �zel durumu.',13,10,'$'

        public  szRing0FaultMessage
;
; Fault in the DOSX internal fault handler.  Not recoverable.
;
; Note:  This is for a real bad one.
;
szRing0FaultMessage     db      13,10
                db      '  DOS Uzat�c�s�: �� hata.',13,10,'$'

DXPMCODE ends

        end
