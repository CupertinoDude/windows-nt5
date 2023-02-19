	TITLE	STRINGS - OEM dependent strings used by KERNEL
include gpcont.inc		; SHERLOCK

_DATA	SEGMENT PARA PUBLIC 'DATA'
_DATA	ENDS

DGROUP	GROUP	_DATA

_INITTEXT SEGMENT WORD PUBLIC 'CODE'
_INITTEXT ENDS

_NRESTEXT SEGMENT WORD PUBLIC 'CODE'
_NRESTEXT ENDS

ASSUME	DS:DGROUP


_DATA	SEGMENT PARA PUBLIC 'DATA'

; This is the caption string for the dialog box.

public	szDiskCap
IF 0
szDiskCap	db  '������ �婡��',0
ELSE
szDiskCap	db  '�������ᩫ��� ��ᢣ� ����妬',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public	szCannotFind1,szCannotFind2
szCannotFind1	db	"��� �夘� ������ �� ������ ", 0
szCannotFind2	db	0

; This is the text for fatal errors at boot time
;	<szBootLoad>filename
public szBootLoad
szBootLoad	db	"�������ᩫ��� ��ᢣ� ���� �� �樫ਫ਼ ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;	<szCannotFind1>filename<szInsert>

IF 0
public	szInsert
szInsert	db  ', �������㩫� �� ����⫘ ��� ���ᛘ '
ENDIF
;public  drvlet
;drvlet		db  "X.",0

if SHERLOCK
public szGPCont		; GP fault continuation message
szGPCont	db	"�������ᩫ��� ��ᢣ� ���� �������� ���.",10
        db      "�� ����⥜�� '���ᙢ���', �������穫� ��� ������ ��� �� ⤘ �� �����.",10
        db      "�� ����⥜�� '���婠��', � �������� ��� �� �����������.",0
endif

public	szDosVer
szDosVer	DB	'������⤞ ⡛��� ��� MS-DOS.  MS-DOS 3.1 � ��櫜�� ⡛��� ������嫘�.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption	db	"��ᢣ� ��������"
		db	0
szBlame		db	"BOOT "
		db	0
szSnoozer	db	" ����ᢜ�� "
		db	0
szInModule	db	" ����", 10, "��櫞�� <ᚤ੫�>"
		db	0
szAt		db	" ��� "
		db	0
szNukeApp       db      ".", 10, 10, "����⥫� '���婠��'. "
		db	0
szWillClose	db	"�� ���婜�."
		db	0
szGP		db	"⤘ ������ ��ᢣ� �������嘪"
		db	0
szD0		db	"��� ���娜�� ��� ��� ������"	; not yet used
		db	0
szSF		db	"⤘ ��ᢣ� ���噘�"		; not spec'ed
		db	0
szII		db	"��� �� ⚡��� ������"	; "�៦�" ???
		db	0
szPF		db	"⤘ ��ᢣ� ���囘�"
		db	0
szNP		db	"⤘ ��ᢣ� �� ���椫�� ��㣘���"
		db	0
szAF		db	"� ���ᢣ� ��������"	; not yet used
		db	0
szLoad		db	"������� �樫ਫ਼� ��㣘���"
		db	0
szOutofSelectors db	"�� �������� ������㟞���"
		db	0

; Text for dialog box when terminating an application

public szAbort
szAbort 	db	"���婠�� ��� ��⮦���� ��������.",0

; Text for dialog box when trying to run a compressed file
			   
public szBozo
szBozo  	db	"��� �夘� ������ � �樫ਫ਼ ���������� ������",0
			 	     		     
; This is the caption string for system error dialog boxes

public	syserr
syserr		db	"��ᢣ� ����㣘���",0

; The following group of messages forms all of the messages used
; in the INT 24 dialog box.
;
; There are 7 messages which can be translated individually. The
; location of drvlet? and devenam? can be moved to any location
; within the string.

public	msgWriteProtect,drvlet1
public	msgCannotReadDrv,drvlet2
public	msgCannotWriteDrv,drvlet3
public	msgShare,drvlet4
public	msgNetError,drvlet5
public	msgCannotReadDev,devenam1
public	msgCannotWriteDev,devenam2
public	msgNoPrinter
public	msgNetErrorDev,devenam3

msgWriteProtect         db      "�婡�� �� �������� ������� ��� ���ᛘ "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "��� �夘� ������ � ��ᚤਫ਼ ��� ���ᛘ� �婡�� "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "��� �夘� ������ � ������� ��� ���ᛘ �婡�� "
drvlet3                 db      "X.",0

msgShare		db	"�����嘩� ����� ��㩞� ��� ���ᛘ �婡�� "
drvlet4                 db      "X.",0

msgNetError		db	"�������ᩫ��� ��ᢣ� ����禬 ��� ���ᛘ �婡�� "
drvlet5 		db	"X.",0

msgCannotReadDev        db      "��� �夘� ������ � ��ᚤਫ਼ ��� �� ������� "
devenam1                db      8 dup (?)
                        db      0

msgCannotWriteDev       db      "��� �夘� ������ � ������� ��� ������� "
devenam2                db      8 dup (?)
			db	0

msgNetErrorDev		db	"�������ᩫ��� ��ᢣ� ����禬 ��� ������� "
devenam3		db	8 dup (?)
			db	0

msgNoPrinter            db      "� ������� ��� �夘� ⫦����",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'�雠��� ��� ������樟૞ ⥦�� = ',0
szExitStr2  DB  ' ������墠�� ���噘�',13,10,0
public  szUndefDyn
szUndefDyn      db      "��㩞 ����樠���� �������� �礛����",0
public  szFatalExit
szFatalExit	db	"� �������� �㫞�� ���������� �� �� ����������� ��槦",0
else
public szDebugStr
szDebugStr  DB  '�������: ������� �樫ਫ਼� - ',0                   ; 0
            DB  '�������: ������� �樫ਫ਼� �☪ ������嘪 ��� - ',0   ; 1
            DB  '�������ᩫ��� ��ᢣ� ���� �� �樫ਫ਼ ��� ����� �樦� - ',0         ; 2
            DB  13,10,0                                         ; 3
            DB  7,13,10,'�雠��� ��� ������樟૞ ⥦��= ',0    ; 4
            DB  ' ������墠�� ���噘�',0                        ; 5
            DB  13,10,'��������矞�� ���噘�:',13,10,0          ; 6
	    DB  7,13,10,'�����ਫ਼, �������, 륦��� � ���ᙢ���; ',0;7
            DB  '� ����囘 BP ��� �夘� ⚡���',7,13,10,0       ; 8
	    DB	': ',0						; 9
	    DB	'Reentered FatalExit',7,13,10,0 		; 10
	    DB  0
public szFKE
szFKE	DB '*** ������樟૦ ��ᢣ� ���㤘 ***',0
endif

;** Diagnostic mode messages
        public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
        public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      '� �雠��� ������嘪 �夘� ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      '���夞�� �� �����੫��� ���ᩫ��� ��������嘪.  �� ����� ��������� �夘�:  ', 0
endif

_DATA	ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "�� ����穫��� Win16 ��� �夘� ������ �� ����������㩜� ����������⤞ ���������, �� DOSX.EXE ��⧜� �� ��婡���� ��� AUTOEXEC.NT ��� �� ��ᨮ�� ��� ��������� PATH.",0
szMissingMod    db   "������� NTVDM: ��姜� 16-bit ����������� ���ᛘ ����㣘���",0
szPleaseDoIt    db   "��������㩫� �ᢠ ��� ������� ����������� ���ᛘ ���� ���ᢦ�� system32:",13,10,9,9,0
szInadequate	db   "������� NTVDM: �������� ���������� DPMI",0
szNoGlobalInit	db   "������� NTVDM: ��� �夘� ������ � ����������� ��� heap",0
NoOpenFile	    db   "������� NTVDM: ��� �夘� ������ �� ᤦ���� ��� �����⩠��� ��� ������",0
NoLoadHeader	db   "������� NTVDM: ��� �夘� ������ � �樫ਫ਼ ��� ��������囘� ��� KERNEL EXE ",0
szGenBootFail   db   "������� NTVDM: ��������⤞ ����������� ��� �������㣘��� Win16",0
else
szInadequate	db   '�������: �������� ���������� DPMI',13,10,'$'
szNoPMode	db   '�������: ��� �夘� ������ �� �������������� ����������⤞ ���������',13,10,'$'
szNoGlobalInit	db   "�������: ��� �夘� ������ � ����������� ��� heap",13,10,'$'
NoOpenFile      db   "�������: ��� �夘� ������ �� ᤦ���� ��� �����⩠��� ��� ������"
                db   13, 10, '$'
NoLoadHeader    db   "�������: ��� �夘� ������ � �樫ਫ਼ ��� ��������囘� ��� KERNEL EXE"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap		db	"���������垩� ������櫞��� ��������",0

msgRealModeApp1 db	"� �������� ��� ��桜���� �� �����⩜��, ",0
msgRealModeApp2 db	"⮜� ���������� ��� ������磜�� ⡛��� �� Windows.",0Dh,0Dh
	db	"����᝜��� ��� �������⤞ ⡛��� ��� �������� � ���� �夘� ������� "
	db	"�� �� Windows 3.0 ��� ��� ��櫜��� ���橜�� ����.",13,13
	db	"�� ����⥜�� 'OK' ��� �����㩜�� ��� ��������, �� �����㣘�� ������櫞��� "
	db	"������ �� ������⩦�� ��� ������桞�� ���������� ��� �������� � �� Windows.",0

_NRESTEXT ENDS

end
