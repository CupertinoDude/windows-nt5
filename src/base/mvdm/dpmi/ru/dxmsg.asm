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
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ����७��� DOS �����㦥� �� ᮮ⢥�����騩 ⨯ CPU.',13,10
                db      13,10,'$'
;
; Can't figure out how to get into protected mode.
;
ER_PROTMODE     db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ����७��� DOS �����㦥� ���䫨�� � ��㣨� �ணࠬ��� ',13,10
                db      '  ���ᯥ祭���, ࠡ���饬 � ���饭��� ०���.',13,10
                db      13,10,'$'
;
; Couldn't initialize XMS driver.
;
ER_NOHIMEM      db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ����७�� DOS �����㦨�� �訡�� �� ���樠����樨',13,10
                db      '  ��ᯥ��� �������⥫쭮� ����� (XMS).',13,10
                db      13,10,'$'
;
; Non-specific unable to initialize DOSX error.
;
ER_DXINIT       db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ����७�� DOS �����㦨�� ������������ �訡��.'
                db      13,10,'$'
;
; A DOS memory allocation failed.
;
ER_REALMEM      db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ��������� ��ꥬ ���筮� �����.',13,10,13,10
                db      13,10,'$'
;
; Couldn't get enough extended memory to run.
;
ER_EXTMEM       db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ��������� ��ꥬ �������⥫쭮� ����� (XMS).',13,10,13,10
                db      13,10,'$'
;
; Where is KRNL[23]86.EXE!!!
;
ER_NOEXE        db      13,10
                db      '  ����� �������� �� 16-ࠧ�來�� �ਫ������ � ���饭��� ०���;',13,10,13,10
                db      '  ����७�� DOS �� �����㦨�� ����室���� ��� ����᪠ ��⥬��� 䠩���.',13,10,13,10
                db      13,10,'$'

if      VCPI
;
; VCPI initialization failed.
;
ER_VCPI         db      13,10
                db      '  ���������� �������� �⠭����� ०�� ��-�� �訡�� ��ᯥ��� �����.'
                db      13,10,'$'
endif   ;VCPI

if      VCPI
;
; This message is displayed if someone fails the Windows INT 2Fh startup
; broadcast.  All of the "Windows 3.0 compatible" 3rd party memory managers
; do this.
;
ER_QEMM386      db      13,10
                db      '            �ࠩ��� ���ன�⢠ ��� १����⭠� �ணࠬ�� (TSR) �ॡ��,'
                db      13,10
                db      '            �⮡� �⠭����� ०�� Windows ᥩ�� �� ����㦠���.'
                db      13,10
                db      '            ������ ��� �ணࠬ�� ��� ������� �� �� ᮢ���⨬��'
                db      13,10
                db      '            � �⠭����� ०���� Windows.'
                db      13,10
                db      13,10
                db      '            ������ ������� "Y", �⮡� �� ࠢ�� ����㧨��'
                db      13,10
                db      '            �⠭����� ०�� Windows.'
                db      13,10
                db      '            ������ ���� ����� �������, �⮡� �������� � DOS.'
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
                db      '  ����७�� DOS: ������������ �訡�� ���饭���� ०���.',13,10,'$'

        public  szRing0FaultMessage
;
; Fault in the DOSX internal fault handler.  Not recoverable.
;
; Note:  This is for a real bad one.
;
szRing0FaultMessage     db      13,10
                db      '  ����७�� DOS: ����७��� �訡��.',13,10,'$'

DXPMCODE ends

        end
