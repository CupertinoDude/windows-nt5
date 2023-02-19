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
szDiskCap       db  'Lemezcsere',0
ELSE
szDiskCap       db  'F�jlhiba',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public  szCannotFind1,szCannotFind2
szCannotFind1   db      "A(z) ", 0
szCannotFind2   db      " nem tal�lhat�.", 0

; This is the text for fatal errors at boot time
;       <szBootLoad>filename
public szBootLoad
szBootLoad      db      "Hiba a k�vetkez� komponens bet�lt�se k�zben: ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;       <szCannotFind1>filename<szInsert>

IF 0
public  szInsert
szInsert        db  ' nem tal�lhat�. Helyezze be a k�vetkez� meghajt�ba: '
ENDIF
;public  drvlet
;drvlet         db  "X.",0

if SHERLOCK
public szGPCont         ; GP fault continuation message
szGPCont        db      "Hiba t�rt�nt az alkalmaz�sban.",10
        db      "Ha a Tov�bb gombra kattint, azonnal mentse el adatait egy �j f�jlba.",10
        db      "Ha a Bez�r�st v�lasztja, az alkalmaz�s befejez�dik.",0
endif

public  szDosVer
szDosVer        DB      'Nem megfelel� MS-DOS verzi�.  MS-DOS 3.1 vagy �jabb sz�ks�ges.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption  db      "Alkalmaz�shiba"
                db      0
szBlame         db      "BOOT "
                db      0
szSnoozer       db      "  "
                db      0
szInModule      db      " -", 10, "modul: <unknown>"
                db      0
szAt            db      " - "
                db      0
szNukeApp       db      ".", 10, 10, "V�lassza a Bez�r�s gombot. "
                db      0
szWillClose     db      " be lesz z�rva."
                db      0
szGP            db      "�ltal�nos v�delmi hiba (GPF)"
                db      0
szD0            db      "null�val val� oszt�s " ; not yet used
                db      0
szSF            db      "veremhiba"             ; not spec'ed
                db      0
szII            db      "illeg�lis utas�t�s"    ; "Fault" ???
                db      0
szPF            db      "laphiba "
                db      0
szNP            db      "'objektum nem tal�lhat�' hiba"
                db      0
szAF            db      "alkalmaz�shiba "    ; not yet used
                db      0
szLoad          db      "szegmensbet�lt�si hiba"
                db      0
szOutofSelectors db     "a szelektorok elfogytak "
                db      0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "Az aktu�lis alkalmaz�s bez�r�sa.",0

; Text for dialog box when trying to run a compressed file
                           
public szBozo
szBozo          db      "T�m�r�tett f�jlokat nem lehet bet�lteni.",0
                                                     
; This is the caption string for system error dialog boxes

public  syserr
syserr          db      "Rendszerhiba",0

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

msgWriteProtect         db      "A k�vetkez� meghajt�ban l�v� lemez �r�sv�dett: "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "A k�vetkez� meghajt�r�l nem lehet olvasni: "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "A k�vetkez� meghajt�ra nem lehet �rni: "
drvlet3                 db      "X.",0

msgShare                db      "Megoszt�smegs�rt�si hiba a k�vetkez� meghajt�n: "
drvlet4                 db      "X.",0

msgNetError             db      "H�l�zati hiba a k�vetkez� meghajt�n: "
drvlet5                 db      "X.",0

msgCannotReadDev        db      "A k�vetkez� eszk�zr�l nem lehet olvasni: "
devenam1                db      8 dup (?)
                        db      0

msgCannotWriteDev       db      "A k�vetkez� eszk�zre nem lehet �rni: "
devenam2                db      8 dup (?)
                        db      0

msgNetErrorDev          db      "H�l�zati hiba a k�vetkez� eszk�z�n: "
devenam3                db      8 dup (?)
                        db      0

msgNoPrinter            db      "A nyomtat� nem �zemk�sz.",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'FatalExit code = ',0
szExitStr2  DB  ' stack overflow',13,10,0
public  szUndefDyn
szUndefDyn      db      "Nem defini�lt Dynalink-h�v�s.",0
public  szFatalExit
szFatalExit     db      "Az alkalmaz�s nem a szab�lyos m�don fejez�d�tt be",0
else
public szDebugStr
szDebugStr  DB  'KERNEL: nem t�lthet� be - ',0                                     ; 0
            DB  'KERNEL: az objektum �j p�ld�nya nem t�lthet� be - ',0             ; 1
            DB  'Az objektum nem t�lthet� be az er�forr�sf�jlb�l  - ',0    ; 2
            DB  13,10,0                                                            ; 3
            DB  7,13,10,'FatalExit k�d = ',0                                       ; 4
            DB  ' veremt�r t�lcsordul�s',0                                         ; 5
            DB  13,10,'A verem tartalma:',13,10,0                                  ; 6
            DB  7,13,10,'Megszak�t�s, T�r�spont, Kil�p�s vagy Folytat�s? ',0         ; 7
            DB  '�rv�nytelen BP l�nc.',7,13,10,0                                   ; 8
            DB  ': ',0                                          ; 9
            DB  '�jra be�rt FatalExit',7,13,10,0                 ; 10
            DB  0
public szFKE
szFKE   DB '*** S�lyos kernelhiba ***',0
endif

;** Diagnostic mode messages
        public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
        public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      ' Hibak�d: ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
        public szInitSpew
szInitSpew      DB      'Ind�t�s diagnosztikai �zemm�dban. Napl�f�jl:  ', 0
endif

_DATA   ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "A Win16 alrendszer nem tudott v�dett m�dba kapcsolni. A DOSX.EXE programnak szerepelnie kell az AUTOEXEC.NT f�jlban, vagy PATH v�ltoz�val megadott el�r�si �ton.",0
szMissingMod    db   "NTVDM KERNEL: Hi�nyz� 16 bites rendszermodul.",0
szPleaseDoIt    db   "Telep�tse �jra a k�vetkez� modult a system32 k�nyvt�rba:",13,10,9,9,0
szInadequate    db   "KERNEL: Nem megfelel� DPMI-kiszolg�l�.",0
szNoGlobalInit  db   "KERNEL: A heap nem inicializ�lhat�.",0
NoOpenFile      db   "KERNEL: A kernel futtathat� f�jlj�t nem siker�lt megnyitni.",0
NoLoadHeader    db   "KERNEL: A KERNEL EXE-fejl�ce nem t�lthet� be.",0
szGenBootFail   db   "KERNEL: Win16 alrendszer - inicializ�ci�s hiba.",0
else
szInadequate    db   'KERNEL: Nem megfelel� DPMI-kiszolg�l�.',13,10,'$'
szNoPMode       db   'KERNEL: Nem siker�lt v�dett m�dba kapcsolni.',13,10,'$'
szNoGlobalInit  db   "KERNEL: Nem siker�lt inicializ�lni a heapet.",13,10,'$'
NoOpenFile      db   "KERNEL: A kernel futtathat� f�jlj�t nem siker�lt megnyitni."
                db   13, 10, '$'
NoLoadHeader    db   "KERNEL: a KERNEL EXE-fejl�ce nem t�lthet� be."
                db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap            db      "Alkalmaz�s-kompatibilit�si �zenet",0

msgRealModeApp1 db      "A futtatni k�v�nt alkalmaz�st a Windows kor�bbi ",0
msgRealModeApp2 db      ", verzi�j�hoz tervezt�k.",0Dh,0Dh
        db      "Szerezze be az alkalmaz�s �jabb, legal�bb Windows 3.0-val kompatibilis "
        db      "v�ltozat�t.",13,13
        db      "Ha az OK gombra kattint �s elind�tja az alkalmaz�st, akkor nem v�rt "
        db      "probl�m�k l�phetnek fel, illetve a Windows bez�rhatja az alkalmaz�st.",0

_NRESTEXT ENDS

end
