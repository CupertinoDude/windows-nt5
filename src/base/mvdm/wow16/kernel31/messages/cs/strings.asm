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
szDiskCap       db  'V�m�na diskety',0
ELSE
szDiskCap       db  'Chyba souboru',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public  szCannotFind1,szCannotFind2
szCannotFind1   db      "Nelze nal�zt soubor ", 0
szCannotFind2   db      0

; This is the text for fatal errors at boot time
;       <szBootLoad>filename
public szBootLoad
szBootLoad      db      "Chyba p�i na��t�n� souboru ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;       <szCannotFind1>filename<szInsert>

IF 0
public  szInsert
szInsert        db  ', vlo�te disk do jednotky '
ENDIF
;public  drvlet
;drvlet         db  "X.",0

if SHERLOCK
public szGPCont         ; GP fault continuation message
szGPCont        db      "V aplikaci se vyskytla chyba.",10
	db      "Pokud zvol�te Ignorovat, ulo�te va�e data do nov�ho souboru.",10
	db      "Pokud zvol�te Ukon�it, bude dan� aplikace ukon�ena.",0
endif

public  szDosVer
szDosVer        DB      'Chybn� verze syst�mu MS-DOS. Vy�aduje se verze 3.1 nebo vy��.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption  db      "Chyba aplikace"
		db      0
szBlame         db      "START "
		db      0
szSnoozer       db      " zp�sobil "
		db      0
szInModule      db      " v", 10, "modulu <nezn�m�>"
		db      0
szAt            db      " na adrese "
		db      0
szNukeApp       db      ".", 10, 10, "Zvolte ukon�en� aplikace. "
		db      0
szWillClose     db      " bude ukon�en."
		db      0
szGP            db      "v�eobecnou chybu ochrany"
		db      0
szD0            db      "d�len� nulou"  ; not yet used
		db      0
szSF            db      "chybu z�sobn�ku"               ; not spec'ed
		db      0
szII            db      "ileg�ln� instrukci"    ; "Fault" ???
		db      0
szPF            db      "chybu str�nky"
		db      0
szNP            db      "chybu nep��tomnosti"
		db      0
szAF            db      "chybu aplikace"        ; not yet used
		db      0
szLoad          db      "chybu na�ten� segmentu"
		db      0
szOutofSelectors db     "nedostatek selektor�"
		db      0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "Aplikace bude ukon�ena.",0

; Text for dialog box when trying to run a compressed file

public szBozo
szBozo          db      "Nelze na��st komprimovan� soubory",0

; This is the caption string for system error dialog boxes

public  syserr
syserr          db      "Chyba syst�mu",0

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

msgWriteProtect         db      "Disk chr�n�n� proti z�pisu v jednotce "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "Nelze ��st z jednotky "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "Nelze zapisovat na jednotku "
drvlet3                 db      "X.",0

msgShare                db      "P�estupek sd�len� na jednotce "
drvlet4                 db      "X.",0

msgNetError             db      "Chyba s�t� na jednotce "
drvlet5                 db      "X.",0

msgCannotReadDev        db      "Nelze ��st ze za��zen� "
devenam1                db      8 dup (?)
			db      0

msgCannotWriteDev       db      "Nelze zapisovat na za��zen� "
devenam2                db      8 dup (?)
			db      0

msgNetErrorDev          db      "Chyba s�t� na za��zen� "
devenam3                db      8 dup (?)
			db      0

msgNoPrinter            db      "Tisk�rna nen� p�ipravena",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'k�d FatalExit = ',0
szExitStr2  DB  ' p�ete�en� z�sobn�ku',13,10,0
public  szUndefDyn
szUndefDyn      db      "vol�n� nedefinovan�ho Dynalinku",0
public  szFatalExit
szFatalExit     db      "aplikace si vy��dala abnorm�ln� ukon�en�",0
else
public szDebugStr
szDebugStr  DB  'J�DRO: Selhalo na�ten� - ',0                   ; 0
	    DB  'J�DRO: Selhalo na�ten� nov� instance - ',0     ; 1
	    DB  'Chyba p�i na��t�n� souboru zdroje - ',0        ; 2
	    DB  13,10,0                                         ; 3
	    DB  7,13,10,'FatalExit, k�d = ',0                   ; 4
	    DB  ' p�ete�en� z�sobn�ku',0                        ; 5
	    DB  13,10,'Stav z�sobn�ku:',13,10,0                 ; 6
	    DB  7,13,10,'P�eru�it, Ukon�it, N�vrat nebo Ignorovat? ',0      ; 7
	    DB  'Neplatn� �et�zec BP',7,13,10,0                 ; 8
	    DB  ': ',0                                          ; 9
	    DB  'Nov� v�skyt chyby FatalExit',7,13,10,0         ; 10
	    DB  0
public szFKE
szFKE   DB '*** Fat�ln� chyba j�dra ***',0
endif

;** Diagnostic mode messages
	public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
	public szFailCode, szCodeString
szDiagStart     db      '[start]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      ' K�d selh�n� je ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      'Start diagnostick�ho re�imu. Soubor protokolu:  ', 0
endif

_DATA   ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "Podsyst�m Win16 nemohl p�ej�t do chr�n�n�ho re�imu; DOSX.EXE mus� b�t uveden v souboru AUTOEXEC.NT a le�et v cest� (prom�nn� PATH).",0
szMissingMod    db   "J�DRO NTVDM: Nebyl nalezen 16bitov� syst�mov� modul",0
szPleaseDoIt    db   "P�einstalujte n�sleduj�c� modul do adres��e system32:",13,10,9,9,0
szInadequate    db   "J�DRO NTVDM: Neadekv�tn� server DPMI",0
szNoGlobalInit  db   "J�DRO NTVDM: Nelze inicializovat haldu",0
NoOpenFile      db   "J�DRO NTVDM: Nelze otev��t spustiteln� program J�DRA",0
NoLoadHeader    db   "J�DRO NTVDM: Nelze na��st z�hlav� EXE J�DRA",0
szGenBootFail   db   "J�DRO NTVDM: Inicializace podsyst�mu Win16 se nezda�ila",0
else
szInadequate    db   'J�DRO: Neadekv�tn� server DPMI',13,10,'$'
szNoPMode       db   'J�DRO: Nelze p�ej�t do chr�n�n�ho re�imu',13,10,'$'
szNoGlobalInit  db   "J�DRO: Nelze inicializovat haldu",13,10,'$'
NoOpenFile      db   "J�DRO: Nelze otev��t spustiteln� program J�DRA"
		db   13, 10, '$'
NoLoadHeader    db   "J�DRO: Nelze na��st z�hlav� EXE J�DRA"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap            db      "Varov�n� - kompatibilita aplikac�",0

msgRealModeApp1 db      "Aplikace, kter� m� b�t spu�t�na,  ",0
msgRealModeApp2 db      "byla navr�ena pro p�edchoz� verzi syst�mu Windows.",0Dh,0Dh
        db      "Opat�ete si aktualizovanou verzi aplikace, kter� je kompatibiln� "
        db      "se syst�mem Windows 3.0 nebo vy��m.",13,13
        db      "Pokud klepnete na tla��tko OK a aplikaci spust�te, m��e doj�t k pot���m "
        db      "kompatibility ve form� neo�ek�van�ho ukon�en� aplikace nebo syst�mu Windows. ",0

_NRESTEXT ENDS

end
