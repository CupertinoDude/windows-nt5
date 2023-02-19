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


SignOnMsg db    13,10,'HIMEM: ��暨���� ��㚞��� ��� �� DOS XMS, 롛��� '
	db      '0' + (HimemVersion shr 8),'.'
	db      '0' + ((HimemVersion and 0ffh) / 16)
	db      '0' + ((HimemVersion and 0ffh) mod 16)
	db      ' - '
	db      DATE_String
	db      13,10,'����������� XMS 롛��� 2.0'
	db      13,10,'���������� �����飘�� 1988-1991 Microsoft Corp.'
	db      13,10,'$'

ROMDisabledMsg    db    13,10,  '� ���雞� RAM �夘� �������������⤞.$'
UnsupportedROMMsg db    13,10,'�������: � ����������垩� ��� ���雜� RAM '
		  db            '��� �������坜��� �� ���� �� �穫���.$'
ROMHookedMsg      db    13,10,'�������: � ���雞� RAM �����������嫘� ��� ��� ������ '
		  db            '�� ���������������.$'

BadDOSMsg       db      13,10,'������: ���� �� HIMEM.SYS �夘� ��� Windows NT.$'
NowInMsg        db      13,10,'������: 뤘 ��暨���� �����娠��� ��㣞� Extended ⮜� 㛞 ������������.$'
On8086Msg       db      13,10,'������: �� HIMEM.SYS ������� ��� 80x86 ������.$'
NoExtMemMsg     db      13,10,'������: ��� ��⟞�� ����⩠�� ��㣞 extended.$'
NoA20HandlerMsg db      13,10,'������: ��� �夘� ������ � ⢜���� ��� ������ A20 !$'
VDISKInMsg      db      13,10,'������: �� ��暨���� ���騞��� ��㣞� VDISK ⮜� 㛞 ������������.$'
FlushMsg        db      13,10,7,'      �� ��暨���� ��㚞��� ��� �� XMS ��� ��������៞��.',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' �� ��填�� ��������� ��� ��㣞� extended �夘� ����⩠���.$'
HMAMINMsg       db      13,10,'�� �⚜��� ��� ������� ��㣞� High (HMA) ��婟��� �� $'
KMsg            db      'K.$'
InsA20Msg       db      13,10,' ��������៞�� ��填�� ��������� A20 $'
InsA20EndMsg    db      '.$'
InsExtA20Msg    db      13,10,'��������៞�� ��૜���� ��填�� ��������� A20.$'

NoHMAMsg        db      13,10,'�������: � ������� ��㣞� High ��� �夘� ����⩠��.'
		db      13,10,'$'
A20OnMsg        db      13,10,'�������: � ������ A20 㫘� 㛞 �����������⤞.'
		db      13,10,'$'

BadArgMsg       db      13,10,'�������: ��� �� ⚡��� ���ᣜ���� ������⭟���: $'

HMAOKMsg        db      13,10,'� 64K ������� ��㣞� High �夘� ����⩠��.'
		db      13,10,13,10,'$'

		db      '���� �� ��暨���� ��㡜� ��� Microsoft Corporation.'

; end of material subject to translation


EndText         label   byte
_text   ends
	end
