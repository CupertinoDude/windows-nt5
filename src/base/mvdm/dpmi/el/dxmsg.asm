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
                db      ' ��� �夘� ������ � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' �� ��暨���� ��⡫���� ��� DOS ���槠�� ������� CPU.',13,10
                db      13,10,'$'
;
; Can't figure out how to get into protected mode.
;
ER_PROTMODE     db      13,10
                db      ' ��� �夘� ������ � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' �� ��暨���� ��⡫���� ��� DOS ���槠�� ��� ��⤜�� �� ᢢ� ��������� ',13,10
                db      ' ����������⤞� ��������嘪.',13,10
                db      13,10,'$'
;
; Couldn't initialize XMS driver.
;
ER_NOHIMEM      db      13,10
                db      ' ��� �夘� ������ � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' �� ��暨���� ��⡫���� ��� DOS �������駠�� ⤘ ��ᢣ� ���� ��� ����������� ',13,10
                db      ' ��� �����娠��� ��� ��㣞� extended.',13,10
                db      13,10,'$'
;
; Non-specific unable to initialize DOSX error.
;
ER_DXINIT       db      13,10
                db      ' ��� �夘� ������ � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' �� ��暨���� ��⡫���� ��� DOS �������駠�� ⤘ ���������⤦ ��ᢣ�.'
                db      13,10,'$'
;
; A DOS memory allocation failed.
;
ER_REALMEM      db      13,10
                db      ' ��� �夘� � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' ��� ��ᨮ�� ������ ��������� ��㣞.',13,10,13,10
                db      13,10,'$'
;
; Couldn't get enough extended memory to run.
;
ER_EXTMEM       db      13,10
                db      ' ��� �夘� � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' ��� ��ᨮ�� ������ ��㣞 extended.',13,10,13,10
                db      13,10,'$'
;
; Where is KRNL[23]86.EXE!!!
;
ER_NOEXE        db      13,10
                db      ' ��� �夘� ������ � ���⢜�� ��� 16-bit �������� �� ����������⤞ ���������',13,10,13,10
                db      ' �� ��暨���� ��⡫���� ��� DOS ��� ������ �� ���� �� ����� ��� ����㣘���',13,10
                db      ' ��� �����ov��� ��� ��� ���⢜��.',13,10,13,10
                db      13,10,'$'

if      VCPI
;
; VCPI initialization failed.
;
ER_VCPI         db      13,10
                db      ' ��� �夘� ������ � ���⢜�� �� ������ ���ᩫ��� ��������嘪 ���.',13,10
                db      ' ��� �����㣘��� ��� �����娠��� ��㣞�.'
                db      13,10,'$'
endif   ;VCPI

if      VCPI
;
; This message is displayed if someone fails the Windows INT 2Fh startup
; broadcast.  All of the "Windows 3.0 compatible" 3rd party memory managers
; do this.
;
ER_QEMM386      db      13,10
                db      '            뤘 ��暨���� ��㚞��� ������� � TSR ����᝜��� ',13,10
                db      '            ������ ���ᩫ��� ��������嘪'
                db      13,10
                db      '            �� Windows ��� ������ �� �������� �騘.  ������㩫� ����,'
                db      13,10
                db      '            �� ��暨���� � ����������嫜 ��� ����������⤞ ⡛��� ��� � ����'
                db      13,10
                db      '            �夘� ������� �� ��� ������ ���ᩫ��� ��������嘪 �� Windows.'
                db      13,10
                db      13,10
                db      '            ���㩫� "y" ��� �� �������� � ������ ���ᩫ��� ��������嘪.'
                db      13,10
                db      13,10
                db      '            ��⩫� ������㧦�� ᢢ� ��㡫�� ��� �� ���嫜 ��� DOS.'
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
                db      '  ��暨���� ��⡫���� ��� DOS: �� �������棜�� ���娜��',13,10
                db      '  ����������⤞� ���ᩫ���� ��������嘪.',13,10,'$'
                                                                  
        public  szRing0FaultMessage
;
; Fault in the DOSX internal fault handler.  Not recoverable.
;
; Note:  This is for a real bad one.
;
szRing0FaultMessage     db      13,10
                db      '  ��暨���� ��⡫���� ��� DOS: �������ᩫ��� ��૜���� ��ᢣ�.',13,10,'$'

DXPMCODE ends

        end
