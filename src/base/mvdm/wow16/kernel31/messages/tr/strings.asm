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
szDiskCap       db  'Diski De�i�tir',0
ELSE
szDiskCap       db  'Dosya Hatas�',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public	szCannotFind1,szCannotFind2
szCannotFind1   db      "Bulunam�yor ", 0
szCannotFind2	db	0

; This is the text for fatal errors at boot time
;	<szBootLoad>filename
public szBootLoad
szBootLoad      db      "Y�kleme hatas� ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;	<szCannotFind1>filename<szInsert>

IF 0
public	szInsert
szInsert        db  ', S�r�c�ye yerle�tirin '
ENDIF
;public  drvlet
;drvlet		db  "X.",0

if SHERLOCK
public szGPCont		; GP fault continuation message
szGPCont        db      "Uygulaman�zda bir hata olu�tu.",10
        db      "Yoksay''� se�erseniz �al��malar�n�z� yeni bir dosyaya kaydetmelisiniz.",10
        db      "Kapat''� se�erseniz uygulaman�z sona erecek.",0
endif

public	szDosVer
szDosVer        DB      'Yanl�� MS-DOS s�r�m�.  MS-DOS 3.1 veya yukar�s� gerekli.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption  db      "Uygulama Hatas�"
		db	0
szBlame         db      "�NY�KLEME YAP "
		db	0
szSnoozer	db	" neden oldu "
		db	0
szInModule	db	" ", 10, "birim <bilinmiyor>"
		db	0
szAt		db	" konum "
		db	0
szNukeApp       db      ".", 10, 10, "Kapat''� se�in. "
		db	0
szWillClose	db	" kapanacak."
		db	0
szGP            db      "Genel Koruma Hatas�"
		db	0
szD0            db      "S�f�ra B�l�nme"        ; not yet used
		db	0
szSF            db      "Y���n Hatas�"          ; not spec'ed
		db	0
szII            db      "Ge�ersiz Komut"        ; "Hatas�" ???
		db	0
szPF            db      "Sayfa Hatas�"
		db	0
szNP            db      "Yok Hatas�"
		db	0
szAF            db      "Uygulama Hatas�"       ; not yet used
		db	0
szLoad          db      "B�l�t Y�kleme Hatas�"
		db	0
szOutofSelectors db     "Se�iciler Bitti"
		db	0

; Text for dialog box when terminating an application

public szAbort
szAbort         db      "Ge�erli uygulama kapat�l�yor.",0

; Text for dialog box when trying to run a compressed file
			   
public szBozo
szBozo          db      "S�k��t�r�lm�� dosyalar y�klenemez",0
			 	     		     
; This is the caption string for system error dialog boxes

public	syserr
syserr          db      "Sistem Hatas�",0

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

msgWriteProtect         db      "S�r�c�de yazma korumal� disk "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "S�r�c�den okunam�yor "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "S�r�c�ye yaz�lamIyor "
drvlet3                 db      "X.",0

msgShare                db      "S�r�c�de payla��m ihlali "
drvlet4                 db      "X.",0

msgNetError             db      "S�r�c�de a� hatas� "
drvlet5 		db	"X.",0

msgCannotReadDev        db      "Ayg�ttan okunam�yor "
devenam1                db      8 dup (?)
                        db      0

msgCannotWriteDev       db      "Ayg�ta yaz�lam�yor "
devenam2                db      8 dup (?)
			db	0

msgNetErrorDev          db      "Ayg�tta a� hatas� "
devenam3		db	8 dup (?)
			db	0

msgNoPrinter            db      "Yaz�c� haz�r de�il",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'FatalExit code = ',0
szExitStr2  DB  ' stack overflow',13,10,0
public  szUndefDyn
szUndefDyn      db      "Tan�ms�z Dynalink �a�r�s�",0
public  szFatalExit
szFatalExit     db      "Uygulama anormal sonland�rma istedi",0
else
public szDebugStr
szDebugStr  DB  'KERNEL: Y�kleme ba�ar�s�z - ',0                   ; 0
            DB  'KERNEL: Yeni kopya y�kleme ba�ar�s�z - ',0   ; 1
            DB  'Kaynak dosyadan y�klemede hata - ',0         ; 2
            DB  13,10,0                                         ; 3
            DB  7,13,10,'FatalExit kodu = ',0                   ; 4
            DB  ' y���n ta�mas�',0                             ; 5
            DB  13,10,'Y���n izleme:',13,10,0                    ; 6
            DB  7,13,10,'�ptal et, Kes, �Ik veya Yoksay? ',0      ; 7
            DB  'Ge�ersiz BP zinciri',7,13,10,0                    ; 8
	    DB	': ',0						; 9
	    DB	'Yeniden girildi FatalExit',7,13,10,0 		; 10
	    DB  0
public szFKE
szFKE   DB '*** �nemli �ekirdek Hatas� ***',0
endif

;** Diagnostic mode messages
        public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
        public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'LoadStart = ',0
szLoadSuccess   db      'LoadSuccess = ', 0
szLoadFail      db      'LoadFail = ', 0
szFailCode      db      ' Hata kodu ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      'Tan� modu ba�lang�c�.  G�nl�k dosyas�:  ', 0
endif

_DATA	ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "Win16 Alt sistemi KorumalI Kipe giremedi, DOSX.EXE "
          db "AUTOEXEC.NT''nizde ve YOL''unuzda olmal�.",0
szMissingMod    db   "NTVDM �EK�RDEK: Eksik 16-bit sistem mod�l�",0
szPleaseDoIt    db   "A�a��daki mod�l� system32 dizininize yeniden y�kleyin:",13,10,9,9,0
szInadequate    db   "NTVDM �EK�RDEK: Yetersiz DPMI Sunucusu",0
szNoGlobalInit  db   "NTVDM �EK�RDEK: K�me alan� haz�rlanamad�",0
NoOpenFile          db   "NTVDM �EK�RDEK: �EK�RDEK �al��t�r�labilir a��lam�yor",0
NoLoadHeader    db   "NTVDM �EK�RDEK: �EK�RDEK EXE ba�l��� y�klenemiyor",0
szGenBootFail   db   "NTVDM �EK�RDEK: Win16 Alt sistemi Ba�latma Hatas�",0
else
szInadequate    db   '�EK�RDEK: Yetersiz DPMI Sunucusu',13,10,'$'
szNoPMode       db   '�EK�RDEK: Korumal� Kipe girilemedi',13,10,'$'
szNoGlobalInit  db   "�EK�RDEK: K�me haz�rlanamad�",13,10,'$'
NoOpenFile      db   "�EK�RDEK: �EK�RDEK �al��t�r�labilir a��lam�yor"
                db   13, 10, '$'
NoLoadHeader    db   "�EK�RDEK: �EK�RDEK EXE ba�l��� y�klenemiyor"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap            db      "Uygulama Uyumluk Uyar�s�",0

msgRealModeApp1 db      "�al��t�rmak �zere oldu�unuz uygulama, ",0
msgRealModeApp2 db      ", Windows''un �nceki bir s�r�m� i�in tasarlanm��.",0Dh,0Dh
        db      "Uygulaman�n g�ncelle�tirilmi�, Windows s�r�m 3.0 ve sonras� ile "
        db      "uyumlu bir s�r�m�n� edinin.",13,13
        db      "Tamam d��mesini se�ip uygulamay� ba�lat�rsan�z uyumluluk sorunlar� uygulaman�n "
        db      "veya Windows''un beklenmedik bir �ekilde kapanmas�na neden olabilir.",0

_NRESTEXT ENDS

end
