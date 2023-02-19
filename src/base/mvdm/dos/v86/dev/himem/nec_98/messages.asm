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

ifndef	NEC_98

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


SignOnMsg db	13,10,'HIMEM: DOS XMS��ײ��, �ް�ޮ� '
	db	'0' + (HimemVersion shr 8),'.'
	db	'0' + ((HimemVersion and 0ffh) / 16)
	db	'0' + ((HimemVersion and 0ffh) mod 16)
	db	' - '
	db	DATE_String
	db	13,10,'XMS�K�i �ް�ޮ� 2.0'
	db	13,10,'Copyright 1988-1991,1993 Microsoft Corp.'
	db	13,10,'$'

ROMDisabledMsg	  db	13,10,	'�V���h�[RAM �͖����ł�.$'
UnsupportedROMMsg db	13,10,'����: ���̃V�X�e���ł�, �V���h�[RAM �̖��������T�|�[�g���Ă��܂���.$'
ROMHookedMsg	  db	13,10,'����: �V���h�[RAM �͎g�p����, �����ɂł��܂���.$'

BadDOSMsg	db	13,10,'�G���[: HIMEM.SYS ��, 3.00�ȏ�� DOS��ł������삵�܂���.$'
NowInMsg	db	13,10,'�G���[: �G�N�X�e���h�������}�l�[�W�������łɑg�ݍ��܂�Ă��܂�.$'
On8086Msg	db	13,10,'�G���[: HIMEM.SYS ��, 80x86 �x�[�X�̋@�B��K�v�Ƃ��܂�.$'
NoExtMemMsg	db	13,10,'�G���[: �g�p�\�ȃG�N�X�e���h��������������܂���.$'
NoA20HandlerMsg db	13,10,'�G���[: A20���C���̐��䂪�ł��܂���I$'
VDISKInMsg	db	13,10,'�G���[: VDISK ����������t�������łɑg�ݍ��܂�Ă��܂�.$'
FlushMsg	db	13,10,7,'       XMS�h���C�o�͑g�ݍ��܂�܂���.',13,10,13,10,'$'

StartMsg	db	13,10,'$'
HandlesMsg	db	' �G�N�X�e���h�������n���h�����L���ɂȂ�܂���.$'
HMAMINMsg	db	13,10,'�ŏ� HMA �T�C�Y��ݒ肵�܂� $'
KMsg		db	'K.$'
InsA20Msg	db	13,10,'A20�n���h������ݒ肵�܂� $'
InsA20EndMsg	db	'.$'
InsExtA20Msg	db	13,10,'�O�� A20�n���h����g�ݍ��݂܂�.$'

NoHMAMsg	db	13,10,'����: �n�C�������̈�͖����ł�.'
		db	13,10,'$'
A20OnMsg	db	13,10,'����: A20���C���͂��łɗL���ɂȂ��Ă��܂�.'
		db	13,10,'$'

BadArgMsg       db      13,10,'����: �����ȃp�����[�^������̂Ŗ������܂�: $'

HMAOKMsg	db	13,10,'64K �n�C�������̈�͗L���ł�.'
		db	13,10,13,10,'$'

		db	'���̃v���O�����Ɋւ����؂̌�����, �}�C�N���\�t�g�Ђ��ۗL���Ă��܂�.'

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

else    ; NEC_98

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
        public  cant_ena20_msg
        public  FlushMsg
        public  StartMsg
        public  HandlesMsg
        public  HMAMINMsg
        public  KMsg
        public  InHMAMsg
        public  NoHMAMsg
        public  A20OnMsg
        public  cant_dia20_msg
        public  HMAOKMsg
        public  InsA20Msg
        public  InsA20EndMsg
        public  EndText


SignOnMsg db    13,10,'HIMEM: XMS�h���C�o Ver.'
        db      '0' + (HimemVersion shr 8),'.'
        db      '0' + ((HimemVersion and 0ffh) / 16)
        db      '0' + ((HimemVersion and 0ffh) mod 16)
        db      ' - '
        db      DATE_String
        db      13,10,'XMS�d�l�o�[�W���� 2.0'
        db      13,10,'Copyright 1988-1990 Microsoft Corp.'
        db      '/ NEC Corporation'             ; ins NEC <90.07.24>
        db      13,10,'$'

ROMDisabledMsg    db    13,10,  'Shadow RAM disabled.$'
UnsupportedROMMsg db    13,10,'WARNING: Shadow RAM disable not supported '
                  db            'on this system.$'
ROMHookedMsg      db    13,10,'WARNING: Shadow RAM is in use and can''t '
                  db            'be disabled.$'

BadDOSMsg       db      13,10,'�G���[: HIMEM.SYS ��DOS 3.00�ȏオ�K�v�ł�$'
NowInMsg        db      13,10,'�G���[: �g���������}�l�[�W���͂��łɃC���X�g�[������Ă��܂�$'
On8086Msg       db      13,10,'�G���[: HIMEM.SYS �� 80x86 �ȏオ�K�v�ł��@�@�@�@$'
NoExtMemMsg     db      13,10,'�G���[: �g�p�\�Ȋg��������������܂���.$'
cant_ena20_msg  db      13,10,'�G���[: A20���C�l�[�u���ɂł��܂���!$'
FlushMsg        db      13,10,9,'      XMS�h���C�o�͎g�p�ł��܂���',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' �g���������n���h���͗L���ł�$'
HMAMINMsg       db      13,10,'�ŏ���HMA�T�C�Y��$'
KMsg            db      'K�ɐݒ肵�܂�$'
InsA20Msg       db      13,10,'Installed A20 handler number $'
InsA20EndMsg    db      '.$'

InHMAMsg        db      'HIMEM�̈ꕔ��HMA�Ɉړ����܂���'
                db      13,10,'$'
NoHMAMsg        db      13,10,'�x��: �n�C�������̈�͖����ł�'
                db      13,10,'$'
A20OnMsg        db      13,10,'�x��: A20�͂��łɃC�l�[�u���ł�'
                db      13,10,'$'
cant_dia20_msg  db      13,10,'�x��: A20���f�B�Z�[�u���ɂł��܂���'

HMAOKMsg        db      13,10,'64K �n�C�������̈悪�L���ł�'
                db      13,10,13,10,'$'

                db      'This program is the property of Microsoft Corporation.'

endif   ; NEC_98

EndText		label	byte
_text	ends
	end
