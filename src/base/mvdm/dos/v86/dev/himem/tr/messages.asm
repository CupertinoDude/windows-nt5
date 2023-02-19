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
	public	BadArgMsg
	public	EndText

; Start of text subject to translation
;  Material appearing in single quotation marks should be translated.


SignOnMsg db	13,10,'HIMEM: DOS XMS Driver, Version '
	db	'0' + (HimemVersion shr 8),'.'
	db	'0' + ((HimemVersion and 0ffh) / 16)
	db	'0' + ((HimemVersion and 0ffh) mod 16)
	db	' - '
	db	DATE_String
	db	13,10,'XMS Belirtimi S�r�m 2.0'
	db	13,10,'Telif Hakk� 1988-1991 Microsoft Corp.'
	db	13,10,'$'

ROMDisabledMsg	  db	13,10,	'G�lge RAM devre d���.$'
UnsupportedROMMsg db	13,10,'UYARI: G�lge RAM''� devreden ��kartmak bu '
		  db		' sistemde desteklenmiyor.$'
ROMHookedMsg	  db	13,10,'UYARI: G�lge RAM kullan�mda ve devre d��� '
		  db		' b�rak�lam�yor.$'

BadDOSMsg	db	13,10,'HATA: Bu HIMEM.SYS Windows NT i�in.$'
NowInMsg	db	13,10,'HATA: Bir Uzat�lm�� Bellek Y�neticisi zaten y�kl�.$'
On8086Msg	db	13,10,'HATA: HIMEM.SYS bir 80x86-tabanl� makine gerektirir.$'
NoExtMemMsg	db	13,10,'HATA: Kullan�labilir uzat�lm�� bellek bulunamad�.$'
NoA20HandlerMsg db	13,10,'HATA: A20 sat�r� denetlenemiyor!$'
VDISKInMsg	db	13,10,'HATA: VDISK bellek ay�r�c�s� zaten y�kl�.$'
FlushMsg	db	13,10,7,'       XMS S�r�c�s� y�kl� de�il.',13,10,13,10,'$'

StartMsg	db	13,10,'$'
HandlesMsg	db	' uzat�lm�� bellek i�leyicileri kullan�labilir.$'
HMAMINMsg	db	13,10,'En k���k HMA boyutu ayar� $'
KMsg		db	'K.$'
InsA20Msg	db	13,10,'Y�kl� A20 i�leyicisi numaras� $'
InsA20EndMsg	db	'.$'
InsExtA20Msg	db	13,10,'Y�kl� d�� A20 i�leyicisi.$'

NoHMAMsg	db	13,10,'UYARI: �st Bellek Alan� kullan�lam�yor.'
		db	13,10,'$'
A20OnMsg	db	13,10,'UYARI: A20 sat�r� zaten etkin durumda.'
		db	13,10,'$'

BadArgMsg	db	13,10,'UYARI: Ge�ersiz parametre yoksay�ld�: $'

HMAOKMsg	db	13,10,'64K �st bellek alan� kullan�labilir.'
		db	13,10,13,10,'$'

		db	'Bu program Microsoft Corporation''�n mal�d�r.'

; end of material subject to translation


EndText		label	byte
_text	ends
	end
