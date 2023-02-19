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
szDiskCap       db  '�f�B�X�N�̕ύX',0
ELSE
szDiskCap       db  '�t�@�C�� �G���[',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public  szCannotFind1,szCannotFind2
szCannotFind1   db      "�t�@�C��: ", 0
szCannotFind2   db      " ��������܂��� "0

; This is the text for fatal errors at boot time
;       <szBootLoad>filename
public szBootLoad
szBootLoad      db      "���[�h �G���[: ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;       <szCannotFind1>filename<szInsert>

IF 0
public  szInsert
szInsert        db  '��������܂���B���̃t�@�C���̂���f�B�X�N�����̃h���C�u�ɑ}�����Ă�������: '
ENDIF
;public  drvlet
;drvlet         db  "X",0

if SHERLOCK
public szGPCont         ; GP fault continuation message
szGPCont        db      "�A�v���P�[�V�����ŃG���[���������܂����B",10
        db      "[����] ��I�񂾂Ƃ��́A��Ɠ��e��V�����t�@�C���ɕۑ����Ă��������B",10
        db      "[����] ��I�ԂƁA�A�v���P�[�V�����������I�����܂��B",0
endif

public  szDosVer
szDosVer        DB      '���̃o�[�W������MS-DOS�ł�Windows�����s�ł��܂���B�o�[�W����3.1�ȍ~��MS-DOS���K�v�ł��B',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
ifndef PM386
ifdef JAPAN
; RISC x86 emulation for JAPAN 
public szRISC386
endif ; for JAPAN
endif ; not PM386
szAbortCaption  db      "�A�v���P�[�V���� �G���["
                db      0
szBlame         db      "�u�[�g"
                db      0
szSnoozer       db      " �� "
                db      0
szInModule      db      " ���������܂����B", 10, "���������ꏊ�́A���W���[�� <�s��>"
                db      0
szAt            db      " ���� "
                db      0
szNukeApp       db      " �Ԓn�ł��B", 10, 10, "[����] ��I��ł��������B"
                db      0
szWillClose     db      " ���I�����܂��B"
                db      0
szGP            db      "��ʕی�ᔽ"
                db      0
szD0            db      "0�ɂ�鏜�Z"        ; not yet used
                db      0
szSF            db      "�X�^�b�N�ᔽ"                ; not spec'ed
                db      0
szII            db      "�s���Ȗ���" ; "Fault" ???
                db      0
ifndef PM386
ifdef JAPAN
; RISC x86 emulation for JAPAN 
szRISC386       db      "RISC �V�X�e���̃T�|�[�g���Ă��Ȃ� x86 ����" ; "Fault" ???
                db      0
endif ; for JAPAN
endif ; not PM386
szPF            db      "�y�[�W�ᔽ"
                db      0
szNP            db      "�s�݈ᔽ"
                db      0
szAF            db      "�A�v���P�[�V�������ł̈ᔽ" ; not yet used
                db      0
szLoad          db      "�Z�O�����g�̃��[�h�̎��s"
                db      0
szOutofSelectors db     "�Z���N�^�s��"
                db      0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "���ݎ��s���̃A�v���P�[�V�������I�����Ă��܂��B",0

; Text for dialog box when trying to run a compressed file

public szBozo
szBozo          db      "���k�t�@�C�������[�h�ł��܂���",0

; This is the caption string for system error dialog boxes

public  syserr
syserr          db      "�V�X�e�� �G���[",0

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
IFDEF   JAPAN                           ; For FontAssoc 92/09/29 yasuho
public  SIZE_INT24_MESSAGES
;
;       ATTENTION: msgWriteProtect is must be first label of int24 messages.
;
ENDIF   ; was DB_FA DNT

msgWriteProtect         db      "�������݋֎~�f�B�X�N: �h���C�u "
drvlet1                 db      "X",0

msgCannotReadDrv        db      "�ǂݏo���܂���: �h���C�u "
drvlet2                 db      "X",0

msgCannotWriteDrv       db      "�������߂܂���: �h���C�u "
drvlet3                 db      "X",0

msgShare                db      "���L�ᔽ: �h���C�u "
drvlet4                 db      "X",0

msgNetError             db      "�l�b�g���[�N �G���[: �h���C�u "
drvlet5                 db      "X",0

msgCannotReadDev        db      "�ǂݏo���܂���: �f�o�C�X "
devenam1                db      8 dup (0)
                        db      0

msgCannotWriteDev       db      "�������߂܂���: �f�o�C�X "
devenam2                db      8 dup (0)
                        db      0

msgNetErrorDev          db      "�l�b�g���[�N �G���[: �f�o�C�X "
devenam3                db      8 dup (0)
                        db      0

msgNoPrinter            db      "�v�����^�������ł��Ă��܂���",0

IFDEF   JAPAN                           ; For FontAssoc 92/09/29 yasuho
SIZE_INT24_MESSAGES     equ     $ - offset msgWriteProtect
ENDIF   ; was DB_FA - now JAPAN DNT

ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'FatalExit�R�[�h = ',0
szExitStr2  DB  '�X�^�b�N �I�[�o�[�t���[',13,10,0
public  szUndefDyn
szUndefDyn      db      "����`�̃_�C�i�~�b�N �����N�ւ̌Ăяo��",0
public  szFatalExit
szFatalExit     db      "�A�v���P�[�V�����ɑ΂���ُ�I���̗v��",0
else
public szDebugStr
szDebugStr  DB  '�J�[�l��: ���[�h�Ɏ��s - ',0                   ; 0
            DB  '�J�[�l��: �V�����C���X�^���X�̃��[�h�Ɏ��s - ',0   ; 1
            DB  '���\�[�X �t�@�C������̃��[�h�Ɏ��s - ',0      ; 2
            DB  13,10,0                                         ; 3
            DB  7,13,10,'FatalExit�R�[�h = ',0                  ; 4
            DB  '�X�^�b�N �I�[�o�[�t���[',0                     ; 5
            DB  13,10,'�X�^�b�N �g���[�X:',13,10,0              ; 6
            DB  7,13,10,'Abort, Break, Exit or Ignore? ',0      ; 7
            DB  '������BP�`�F�[��',7,13,10,0                    ; 8
            DB  ': ',0                                          ; 9
            DB  '�ē��͂��ꂽFatalExit',7,13,10,0               ; 10
            DB  0
public szFKE
szFKE   DB '*** Fatal Kernel Error ***',0
endif

;** Diagnostic mode messages
        public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
        public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      ' �G���[ �R�[�h�� ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
        public szInitSpew
szInitSpew      DB      '�f�f���[�h�ŋN�����܂��B���O �t�@�C����:  ', 0
endif

_DATA   ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt
	
szMissingMod    db   "NTVDM KERNEL: Missing 16-bit system module",0
szPleaseDoIt    db   "Please re-install the following module to your system32 directory:",13,10,9,9,0
szInadequate    db   '�J�[�l��: DPMI�T�[�o�[���s�K��',13,10,'$'
szNoPMode       db   '�J�[�l��: �v���e�N�g ���[�h�ɂł��܂���',13,10,'$'
szNoGlobalInit  db   "�J�[�l��: �q�[�v���������ł��܂���",13,10,'$'

NoOpenFile      db   "�J�[�l��: KERNEL�̎��s�\�t�@�C�����I�[�v���ł��܂���"
                db   13, 10, '$'
NoLoadHeader    db   "�J�[�l��: KERNEL��EXE�w�b�_�[�����[�h�ł��܂���"
                db   13, 10, '$'

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap            db      "�A�v���P�[�V�����̌݊����ɂ��Ă̌x��",0

msgRealModeApp1 db      "���s���悤�Ƃ��Ă���A�v���P�[�V���� ",0
msgRealModeApp2 db      " �́A�ȑO�̃o�[�W������Windows�p�ɊJ�����ꂽ���̂ł��B",0Dh,0Dh
        db      "�o�[�W����3.0�ȍ~��Windows�ƌ݊����̂���A�ŐV�̃A�v���P�[�V��������肵��"
        db      "���������B",13,13
        db      "���̂܂� [OK] ��I��ŃA�v���P�[�V�������N�������ꍇ�A�݊����̖��ɂ��"
        db      "�A�v���P�[�V������Windows���ˑR�N���[�Y����\��������܂��B",0

_NRESTEXT ENDS

end
