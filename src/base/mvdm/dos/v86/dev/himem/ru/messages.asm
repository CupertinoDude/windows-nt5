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


SignOnMsg db    13,10,'HIMEM: DOS �ࠩ��� XMS, ����� '
	db      '0' + (HimemVersion shr 8),'.'
	db      '0' + ((HimemVersion and 0ffh) / 16)
	db      '0' + ((HimemVersion and 0ffh) mod 16)
	db      ' - '
	db      DATE_String
	db      13,10,'����䨪��� XMS, ����� 2.0'
	db      13,10,'Copyright 1988-1991 Microsoft Corp.'
	db      13,10,'$'

ROMDisabledMsg  db      13,10,'����஢���� �����⥩ ��� � ��� �⪫�祭�.$'
UnsupportedROMMsg       db      13,10,'��������: ���⥬� �� �����ন���� �⬥�� ����஢���� '
			db              '�����⥩ ��� � ���.$'
ROMHookedMsg    db      13,10,'��������: ����஢���� �����⥩ ��� � ��� �ᯮ������ '
		db                      '� �� ����� ���� �⪫�祭�.$'

BadDOSMsg       db      13,10,'������: ��� �ࠩ��� HIMEM.SYS �।�����祭 ��� Windows NT.$'
NowInMsg        db      13,10,'������: ��ᯥ��� �������⥫쭮� ����� 㦥 ��⠭�����.$'
On8086Msg       db      13,10,'������: HIMEM.SYS ����� ࠡ���� ⮫쪮 �� ��������� � �����஬ 80x86.$'
NoExtMemMsg     db      13,10,'������: ����㯭�� �������⥫쭠� ������ �� �����㦥��.$'
NoA20HandlerMsg db      13,10,'������: �� 㤠���� ��⠭����� ����஫� ��� ���᭮� ������ A20!$'
VDISKInMsg      db      13,10,'������: �ணࠬ�� VDISK 㦥 ��⠭������.$'
FlushMsg        db      13,10,7,'        �ࠩ��� XMS �� ��⠭�����.',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' ����㯭�� ���ਯ�஢ �����.$'
HMAMINMsg       db      13,10,'��������� ࠧ��� HMA ��⠭����� ࠢ�� $'
KMsg            db      'K.$'
InsA20Msg       db      13,10,'��⠭����� ��ࠡ��稪 ���뢠��� ��� A20: $'
InsA20EndMsg    db      '.$'
InsExtA20Msg    db      13,10,'��⠭����� ���譨� ��ࠡ��稪 ���뢠��� ��� A20.$'

NoHMAMsg        db      13,10,'��������: ������� HMA ������㯥�.'
		db      13,10,'$'
A20OnMsg        db      13,10,'��������: ���᭠� ����� A20 㦥 ������⢮����.'
		db      13,10,'$'

BadArgMsg       db      13,10,'��������: ������ ��ࠬ��� �ந����஢��: $'

HMAOKMsg        db      13,10,'����㯥� ᥣ���� HMA ��ꥬ�� 64K.'
		db      13,10,13,10,'$'

		db      '������ �ணࠬ�� ���� ᮡ�⢥������� Microsoft Corporation.'

; end of material subject to translation


EndText         label   byte
_text   ends
	end
