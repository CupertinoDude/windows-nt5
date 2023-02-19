;/*
; *                      Microsoft Confidential
; *                      Copyright (C) Microsoft Corporation 1988-1991,1993
; *                      All Rights Reserved.
; */
	page	95,160
	title	himem3 - Initialization messages

	.xlist
	include	himem.inc
	.list

;*----------------------------------------------------------------------*
;*	DRIVER MESSAGES							*
;*----------------------------------------------------------------------*

	public	SignOnMsg
	public	ROMDisabledMsg
	public	UnsupportedROMMsg
	public	ROMHookedMsg
	public	BadDOSMsg
	public	NowInMsg
	public	On8086Msg
	public	NoExtMemMsg
	public	FlushMsg
	public	StartMsg
	public	HandlesMsg
	public	HMAMINMsg
	public	KMsg
	public	NoHMAMsg
	public	A20OnMsg
	public	HMAOKMsg
	public	InsA20Msg
	public	InsA20EndMsg
	public	InsExtA20msg
	public	NoA20HandlerMsg
	public	VDISKInMsg
        public  BadArgMsg
	public	EndText

ifdef BILINGUAL
	public	SignOnMsg2
	public	ROMDisabledMsg2
	public	UnsupportedROMMsg2
	public	ROMHookedMsg2
	public	BadDOSMsg2
	public	NowInMsg2
	public	On8086Msg2
	public	NoExtMemMsg2
	public	FlushMsg2
	public	StartMsg2
	public	HandlesMsg2
	public	HMAMINMsg2
	public	KMsg2
	public	NoHMAMsg2
	public	A20OnMsg2
	public	HMAOKMsg2
	public	InsA20Msg2
	public	InsA20EndMsg2
	public	InsExtA20msg2
	public	NoA20HandlerMsg2
	public	VDISKInMsg2
	public	BadArgMsg2
endif


; Start of text subject to translation
;  Material appearing in single quotation marks should be translated.


SignOnMsg db	13,10,'HIMEM: DOS XMS ����̹�, ���� '
	db	'0' + (HimemVersion shr 8),'.'
	db	'0' + ((HimemVersion and 0ffh) / 16)
	db	'0' + ((HimemVersion and 0ffh) mod 16)
	db	' - '
	db	DATE_String
	db	13,10,'XMS ��� ���� 2.0'
	db	13,10,'Copyright 1988-1991,1993 Microsoft Corp.'
	db	13,10,'$'

ROMDisabledMsg	  db	13,10,	'������ RAM ��� ����.$'
UnsupportedROMMsg db	13,10,'���: �� �ý��ۿ����� ������ RAM�� ��� ���ϰ� ����� ���� �������� �ʽ��ϴ�.$'
ROMHookedMsg	  db	13,10,'���: ������ RAM�� ��� ���̶� ������� �ʰ� ���� �� �����ϴ�.$'

BadDOSMsg	db	13,10,'����: HIMEM.SYS�� ���� 3.00 �̻��� DOS�� �ʿ�� �մϴ�.$'
NowInMsg	db	13,10,'����: ���� Ȯ�� �޸� �����ڰ� �̹� ��ġ�Ǿ� �ֽ��ϴ�.$'
On8086Msg	db	13,10,'����: HIMEM.SYS�� 80x86 ����� �ý����� �ʿ�� �մϴ�.$'
NoExtMemMsg	db	13,10,'����: ����� �� �ִ� ���� Ȯ�� �޸𸮰� �����ϴ�.$'
NoA20HandlerMsg db	13,10,'����: A20 ������ ������ �� �����ϴ�!$'
VDISKInMsg	db	13,10,'����: VDISK �޸� �Ҵ�Ⱑ �̹� ��ġ�Ǿ� �ֽ��ϴ�.$'
FlushMsg	db	13,10,7,'       XMS ����̹��� ��ġ���� �ʾҽ��ϴ�.',13,10,13,10,'$'

StartMsg	db	13,10,'$'
HandlesMsg	db	' ���� Ȯ�� �޸� �ڵ� ��� ����.$'
HMAMINMsg	db	13,10,'�ּ� HMA ũ�� ����: $'
KMsg		db	'K.$'
InsA20Msg	db	13,10,'��ġ�� A20 �ڵ鷯 ��ȣ $'
InsA20EndMsg	db	'.$'
InsExtA20Msg	db	13,10,'�ܺ� A20 �ڵ鷯 ��ġ.$'

NoHMAMsg	db	13,10,'���: ���� �޸� ������ ����� �� �����ϴ�.'
		db	13,10,'$'
A20OnMsg	db	13,10,'���: A20 ������ �̹� ����� �� �ְ� �Ǿ� �ֽ��ϴ�.'
		db	13,10,'$'

BadArgMsg       db      13,10,'���: �ùٸ��� ���� �Ű� ������ �����մϴ�: $'

HMAOKMsg	db	13,10,'64K�� ���� �޸� ������ ����� �� �ֽ��ϴ�.'
		db	13,10,13,10,'$'

		db	'�� ���α׷��� Microsoft Corporation �����Դϴ�.'

ifdef BILINGUAL

SignOnMsg2 db	13,10,'HIMEM: DOS XMS Driver, Version '
	db	'0' + (HimemVersion shr 8),'.'
	db	'0' + ((HimemVersion and 0ffh) / 16)
	db	'0' + ((HimemVersion and 0ffh) mod 16)
	db	' - '
	db	DATE_String
	db	13,10,'XMS Specification Version 2.0'
	db	13,10,'Copyright 1988-1991,1993 Microsoft Corp.'
	db	13,10,'$'

ROMDisabledMsg2	  db	13,10,	'Shadow RAM disabled.$'
UnsupportedROMMsg2 db	13,10,'WARNING: Shadow RAM disable not supported '
		  db		'on this system.$'
ROMHookedMsg2	  db	13,10,'WARNING: Shadow RAM is in use and can''t '
		  db		'be disabled.$'

BadDOSMsg2	db	13,10,'ERROR: HIMEM.SYS requires DOS 3.00 or higher.$'
NowInMsg2	db	13,10,'ERROR: An Extended Memory Manager is already installed.$'
On8086Msg2	db	13,10,'ERROR: HIMEM.SYS requires an 80x86-based machine.$'
NoExtMemMsg2	db	13,10,'ERROR: No available extended memory was found.$'
NoA20HandlerMsg2 db	13,10,'ERROR: Unable to control A20 line!$'
VDISKInMsg2	db	13,10,'ERROR: VDISK memory allocator already installed.$'
FlushMsg2	db	13,10,7,'       XMS Driver not installed.',13,10,13,10,'$'

StartMsg2	db	13,10,'$'
HandlesMsg2	db	' extended memory handles available.$'
HMAMINMsg2	db	13,10,'Minimum HMA size set to $'
KMsg2		db	'K.$'
InsA20Msg2	db	13,10,'Installed A20 handler number $'
InsA20EndMsg2	db	'.$'
InsExtA20Msg2	db	13,10,'Installed external A20 handler.$'

NoHMAMsg2	db	13,10,'WARNING: The High Memory Area is unavailable.'
		db	13,10,'$'
A20OnMsg2	db	13,10,'WARNING: The A20 Line was already enabled.'
		db	13,10,'$'

BadArgMsg2	db	13,10,'WARNING: Invalid parameter ignored: $'

HMAOKMsg2	db	13,10,'64K High Memory Area is available.'
		db	13,10,13,10,'$'

		db	'This program is the property of Microsoft Corporation.'
endif

; end of material subject to translation


EndText		label	byte
_text	ends
	end
