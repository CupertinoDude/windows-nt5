	TITLE   STRINGS - OEM dependent strings used by KERNEL
include gpcont.inc              ; SHERLOCK

_DATA   SEGMENT PARA PUBLIC 'DATA'
_DATA   ENDS

DGROUP  GROUP   _DATA

_INITTEXT SEGMENT WORD PUBLIC 'CODE'
_INITTEXT ENDS

_NRESTEXT SEGMENT WORD PUBLIC 'CODE'
_NRESTEXT ENDS

ASSUME  DS:DGROUP


_DATA   SEGMENT PARA PUBLIC 'DATA'

; This is the caption string for the dialog box.

public  szDiskCap
IF 0
szDiskCap       db  '����� �����',0
ELSE
szDiskCap       db  '������ ��� ������ � ������',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public  szCannotFind1,szCannotFind2
szCannotFind1   db      "�� ������� ����� ", 0
szCannotFind2   db      0

; This is the text for fatal errors at boot time
;       <szBootLoad>filename
public szBootLoad
szBootLoad      db      "������ ��� �������� ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;       <szCannotFind1>filename<szInsert>

IF 0
public  szInsert
szInsert        db  ', �������� � �������� '
ENDIF
;public  drvlet
;drvlet         db  "X.",0

if SHERLOCK
public szGPCont         ; GP fault continuation message
szGPCont        db      "������ � ����������.",10
	db      "����� ���������� ������, ������� ������ ���������� � ��������� ������ � ������ �����.",10
	db      "����� ��������� ������, ������� ������ �������. ��� ������������� ������ ����� �������.",0
endif

public  szDosVer
szDosVer        DB      '������������ ������ MS-DOS. ��������� MS-DOS 3.1 ��� ����� ������� ������.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption  db      "������ � ����������"
		db      0
szBlame         db      "BOOT "
		db      0
szSnoozer       db      " ������ "
		db      0
szInModule      db      " �", 10, "������ <unknown>"
		db      0
szAt            db      " � "
		db      0
szNukeApp       db      ".", 10, 10, "������� ������ �������. "
		db      0
szWillClose     db      " ����� �������."
		db      0
szGP            db      "����� ������ ������"
		db      0
szD0            db      "������� �� ����"       ; not yet used
		db      0
szSF            db      "������ ��� ������������� �����"                ; not spec'ed
		db      0
szII            db      "������������ �������"  ; "Fault" ???
		db      0
szPF            db      "������ �������� ������"
		db      0
szNP            db      "������ ����������"
		db      0
szAF            db      "������ ����������"     ; not yet used
		db      0
szLoad          db      "������ �������� ��������"
		db      0
szOutofSelectors db     "�������� ����������"
		db      0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "�������� �������� ����������.",0

; Text for dialog box when trying to run a compressed file
			   
public szBozo
szBozo          db      "������ ��������� ������ ����",0
						     
; This is the caption string for system error dialog boxes

public  syserr
syserr  db      "��������� ������",0

; The following group of messages forms all of the messages used
; in the INT 24 dialog box.
;
; There are 7 messages which can be translated individually. The
; location of drvlet? and devenam? can be moved to any location
; within the string.

public  msgWriteProtect,drvlet1
public  msgCannotReadDrv,drvlet2
public  msgCannotWriteDrv,drvlet3
public  msgShare,drvlet4
public  msgNetError,drvlet5
public  msgCannotReadDev,devenam1
public  msgCannotWriteDev,devenam2
public  msgNoPrinter
public  msgNetErrorDev,devenam3

msgWriteProtect         db      "���������� �� ������ ���� � ����������"
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "�� ������� ��������� ������ � ����� "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "�� ������� �������� ������ �� ���� "
drvlet3                 db      "X.",0

msgShare                db      "������ ����������� ������� �� ����� "
drvlet4                 db      "X.",0

msgNetError             db      "������� ������ �� ����� "
drvlet5                 db      "X.",0

msgCannotReadDev        db      "�� ������� ��������� ������ � ���������� "
devenam1                db      8 dup (?)
			db      0

msgCannotWriteDev       db      "�� ������� �������� ������ �� ���������� "
devenam2                db      8 dup (?)
				db      0

msgNetErrorDev          db      "������� ������ �� ���������� "
devenam3                db      8 dup (?)
			db      0

msgNoPrinter            db      "������� �� �����",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'��� ���������� ���������� = ',0
szExitStr2  DB  ' ������������ �����',13,10,0
public  szUndefDyn
szUndefDyn              db      "����� �� ������������ ������������ �����",0
public  szFatalExit
szFatalExit             db      "���������� ������� ��������� ����������",0
else
public szDebugStr
szDebugStr  DB  '����: ���� ��� �������� - ',0                   ; 0
	    DB  '����: ���� ��� �������� ������ ����� - ',0   ; 1
	    DB  '������ ��� �������� ����� �������� - ',0         ; 2
	    DB  13,10,0                                         ; 3
	    DB  7,13,10,'��� ���������� ���������� = ',0        ; 4
	    DB  ' ������������ �����',0                         ; 5
	    DB  13,10,'����������� �����:',13,10,0                    ; 6
	    DB  7,13,10,'Abort, Break, Exit ��� Ignore? ',0      ; 7
	    DB  '������������ ������� BP',7,13,10,0                    ; 8
	    DB  ': ',0                                          ; 9
	    DB  '��������� ����� ���������� ����������',7,13,10,0               ; 10
	    DB  0
public szFKE
szFKE   DB '*** ��������� ������ ���� ***',0
endif

;** Diagnostic mode messages
	public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
	public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      ' ��� ������ - ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      '������ ���������������� ������.  ���� ���������:  ', 0
endif

_DATA   ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode       db "���������� Win16 �� ����� ����� � ���������� �����, DOSX.EXE ������ �������������� � AUTOEXEC.NT, � ���� � ���� - � PATH.",0
szMissingMod    db   "���� NTVDM: ����������� 16-��������� ��������� ������",0
szPleaseDoIt    db   "�������������� ��������� ������ � ����� SYSTEM32:",13,10,9,9,0
szInadequate    db   "���� NTVDM: �� ��������������� ������ DPMI",0
szNoGlobalInit  db   "���� NTVDM: ���������� ���������������� heap-������",0
NoOpenFile      db   "���� NTVDM: ���������� ������� ���� KERNEL.EXE",0
NoLoadHeader    db   "���� NTVDM: ���������� ��������� ��������� KERNEL.EXE",0
szGenBootFail   db   "���� NTVDM: ���� ���������� ������������� Win16",0
else
szInadequate    db   '����: �� ��������������� ������ DPMI',13,10,'$'
szNoPMode       db   '����: ���������� ����� � ���������� �����',13,10,'$'
szNoGlobalInit  db   "����: ���������� ���������������� heap-������",13,10,'$'
NoOpenFile      db   "����: ���������� ������� ���� KERNEL.EXE"
		db   13, 10, '$'
NoLoadHeader    db   "����: ���������� ��������� ��������� KERNEL.EXE"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap    db      "�������������� � ��������������� ����������",0

msgRealModeApp1 db      "����������, ������� �� ������ ���������, ",0
msgRealModeApp2 db      ", ���� ����������� ��� ���������� ������ Windows.",0Dh,0Dh
	db      "�������� ����������� ������ ����������, ����������� "
	db      "� Windows ������ 3.0 ��� ����� �������.",13,13
	db      "���� �� ������� ������ OK � ��������� ����������, ����� ���������� �������� "
	db      "������������� � ��������� ����������������� ���������� ���������� ��� Windows.",0

_NRESTEXT ENDS

end

