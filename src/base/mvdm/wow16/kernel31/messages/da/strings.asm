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
szDiskCap       db  'Skift disk',0
ELSE
szDiskCap       db  'Filfejl',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public  szCannotFind1,szCannotFind2
szCannotFind1   db      "Kan ikke finde ", 0
szCannotFind2   db      0

; This is the text for fatal errors at boot time
;       <szBootLoad>filename
public szBootLoad
szBootLoad      db      "Fejl ved indl�sning af ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;       <szCannotFind1>filename<szInsert>

IF 0
public  szInsert
szInsert        db  ', Inds�t i drev '
ENDIF
;public  drvlet
;drvlet         db  "X.",0

if SHERLOCK
public szGPCont         ; GP fault continuation message
szGPCont        db      "Der er opst�et en fejl i dit program.",10
	db      "Hvis du v�lger Ignorer, skal du gemme dit arbejde i en ny fil.",10
	db      "Hvis du v�lger Luk, afsluttes programmet.",0
endif

public  szDosVer
szDosVer        DB      'Forkert MS-DOS-version.  Der kr�ves MS-DOS 3.1 eller senere.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption  db      "Programfejl"
		db      0
szBlame         db      "BOOT "
		db      0
szSnoozer       db      " for�rsagede "
		db      0
szInModule      db      " i", 10, "modul <ukendt>"
		db      0
szAt            db      " ved "
		db      0
szNukeApp       db      ".", 10, 10, "V�lg luk. "
		db      0
szWillClose     db      " vil lukke."
		db      0
szGP            db      "en generel beskyttelsesfejl"
		db      0
szD0            db      "en division med nul"   ; not yet used
		db      0
szSF            db      "en stakfejl"           ; not spec'ed
		db      0
szII            db      "en ugyldig instruktion"        ; "Fejl" ???
		db      0
szPF            db      "en sidefejl"
		db      0
szNP            db      "en fejl pga. at noget mangler"
		db      0
szAF            db      "en programfejl"        ; not yet used
		db      0
szLoad          db      "segmentindl�sningsfejl"
		db      0
szOutofSelectors db     "Der er ikke flere selektorer"
		db      0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "Lukker programmet.",0

; Text for dialog box when trying to run a compressed file
			
public szBozo
szBozo          db      "Der kan ikke indl�ses komprimerede filer",0
						
; This is the caption string for system error dialog boxes

public  syserr
syserr          db      "Systemfejl",0

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

msgWriteProtect         db      "Skrivebeskyttet diskette i drev "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "Kan ikke l�se drev "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "Der kan ikke skrives til "
drvlet3                 db      "X.",0

msgShare                db      "Fildelingsfejl p� drev "
drvlet4                 db      "X.",0

msgNetError             db      "Netv�rksfejl p� drev "
drvlet5                 db      "X.",0

msgCannotReadDev        db      "Der kan ikke l�ses fra enheden "
devenam1                db      8 dup (?)
			db      0

msgCannotWriteDev       db      "Der kan ikke skrives til enheden "
devenam2                db      8 dup (?)
			db      0

msgNetErrorDev          db      "Netv�rksfejl p� enheden "
devenam3                db      8 dup (?)
			db      0

msgNoPrinter            db      "Printeren er ikke klar",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'FatalExit code = ',0
szExitStr2  DB  ' stack overflow',13,10,0
public  szUndefDyn
szUndefDyn      db      "Kald til ikke-defineret Dynalink",0
public  szFatalExit
szFatalExit     db      "Programmet forespurgte unormal afslutning",0
else
public szDebugStr
szDebugStr  DB  'KERNEL: Failed loading - ',0                    ; 0
	    DB  'KERNEL: Failed loading new instance of - ',0    ; 1
	    DB  'Error loading from resource file - ',0         ; 2
	    DB  13,10,0                                         ; 3
	    DB  7,13,10,'FatalExit code = ',0                   ; 4
	    DB  ' stack overflow',0                             ; 5
	    DB  13,10,'Stack trace:',13,10,0                    ; 6
	    DB  7,13,10,'Abort, Break, Exit or Ignore? ',0      ; 7
	    DB  'Invalid BP chain',7,13,10,0                    ; 8
	    DB  ': ',0                                          ; 9
	    DB  'Reentered FatalExit',7,13,10,0                 ; 10
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
szFailCode      db      ' Failure code is ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      'Diagnostic mode startup.  Log file is:  ', 0
endif

_DATA   ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "Win16-undersystemet kunne ikke g� i beskyttet tilstand, DOSX.EXE skal v�re i din AUTOEXEC.NT og i din PATH.",0
szMissingMod    db   "NTVDM KERNEL: Der mangler et 16-bit systemmodul",0
szPleaseDoIt    db   "Geninstaller f�lgende modul i system32-mappen:",13,10,9,9,0
szInadequate    db   "KERNEL: Mangelfuld DPMI-server",0
szNoGlobalInit  db   "KERNEL: Heapen kan ikke initialiseres",0
NoOpenFile      db   "KERNEL: Kernel.exe kan ikke �bnes",0
NoLoadHeader    db   "KERNEL: Der kan ikke indl�ses header for Kernel.exe",0
szGenBootFail   db   "KERNEL: Initialiseringsfejl i Win16-undersystem",0
else
szInadequate    db   'KERNEL: Mangelfuld DPMI-server',13,10,'$'
szNoPMode       db   'KERNEL: Der kan ikke g�s i beskyttet tilstand',13,10,'$'
szNoGlobalInit  db   "KERNEL: Heapen kan ikke initialiseres",13,10,'$'
NoOpenFile      db   "KERNEL: Kernel.exe kan ikke �bnes"
		db   13, 10, '$'
NoLoadHeader    db   "KERNEL: Der kan ikke indl�ses header for Kernel.exe"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap            db      "Kompatibilitetsadvarsel",0

msgRealModeApp1 db      "Det program, du vil k�re, er udviklet ",0
msgRealModeApp2 db      "til en tidligere version af Windows.",0Dh,0Dh
	db      "Anskaf en opdateret version, som er kompatibel med "
	db      "Windows version 3.0 og senere.",13,13
	db      "V�lger du OK og starter programmet, kan kompatibilitetsproblemer "
	db      "medf�re, at programmet eller Windows uventet lukker.",0

_NRESTEXT ENDS

end
