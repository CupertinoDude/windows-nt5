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
szDiskCap	db  'Troque o disco',0
ELSE
szDiskCap	db  'Erro de arquivo',0
ENDIF


; This is the text for the "Cannot find xxxxxx" dialog box.
; It is printed:
;
;       <szCannotFind1>filename<szCannotFind2>

public	szCannotFind1,szCannotFind2
szCannotFind1	db	"N�o foi poss�vel encontrar ", 0
szCannotFind2	db	0

; This is the text for fatal errors at boot time
;	<szBootLoad>filename
public szBootLoad
szBootLoad	db	"Erro de carregamento ",0

; The following group of strings is used for the "Please insert disk with XXXX
;   in drive X:" dialog box.
;
; These two strings form the dialog message it is:
;
;	<szCannotFind1>filename<szInsert>

IF 0
public	szInsert
szInsert	db  ', insira na unidade '
ENDIF
;public  drvlet
;drvlet		db  "X.",0

if SHERLOCK
public szGPCont		; GP fault continuation message
szGPCont	db	"Ocorreu um erro no aplicativo.",10
        db      "Se escolher 'Ignorar', ter� que salvar os dados em um novo arquivo.",10
        db      "Se escolher 'Fechar', o aplicativo ser� finalizado.",0
endif

public	szDosVer
szDosVer	DB	'Vers�o incorreta do MS-DOS. � necess�rio o MS-DOS 3.1 ou uma vers�o posterior.',13,10,'$'
; Text for exceptions and faults lead to app termination.

public szAbortCaption,szInModule,szAt
public szNukeApp,szSnoozer,szGP,szSF,szII,szPF,szNP,szBlame,szLoad,szWillClose
public szOutofSelectors
szAbortCaption	db	"Erro de aplicativo"
		db	0
szBlame		db	"BOOT "
		db	0
szSnoozer	db	" foi causado "
		db	0
szInModule	db	" em", 10, "m�dulo <descon.>"
		db	0
szAt		db	" em "
		db	0
szNukeApp       db      ".", 10, 10, "Escolha fechar. "
		db	0
szWillClose	db	" ser� fechado."
		db	0
szGP		db	"uma falha de prote��o geral"
		db	0
szD0		db	"uma divis�o por zero"	; not yet used
		db	0
szSF		db	"uma falha na pilha"		; not spec'ed
		db	0
szII		db	"uma instru��o inv�lida"	; "Fault" ???
		db	0
szPF		db	"um erro de p�gina"
		db	0
szNP		db	"uma falha de presen�a"
		db	0
szAF		db	"uma falha de aplicativo"	; not yet used
		db	0
szLoad		db	"falha no carregamento de segmento"
		db	0
szOutofSelectors db	"sem seletores"
		db	0

; Text for dialog box when terminating an application

public szAbort
szAbort 	db	"O aplicativo est� sendo fechado.",0

; Text for dialog box when trying to run a compressed file
			   
public szBozo
szBozo  	db	"N�o foi poss�vel carregar arquivos compactados",0
			 	     		     
; This is the caption string for system error dialog boxes

public	syserr
syserr		db	"Erro de sistema",0

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

msgWriteProtect         db      "Disco protegido contra grava��o na unidade "
drvlet1                 db      "X.",0

msgCannotReadDrv        db      "N�o foi poss�vel ler a unidade "
drvlet2                 db      "X.",0

msgCannotWriteDrv       db      "N�o foi poss�vel gravar na unidade "
drvlet3                 db      "X.",0

msgShare		db	"Viola��o de compartilhamento na unidade "
drvlet4                 db      "X.",0

msgNetError		db	"Ocorreu um erro de rede na unidade "
drvlet5 		db	"X.",0

msgCannotReadDev        db      "N�o foi poss�vel ler o dispositivo "
devenam1                db      8 dup (?)
                        db      0

msgCannotWriteDev       db      "N�o foi poss�vel gravar no dispositivo "
devenam2                db      8 dup (?)
			db	0

msgNetErrorDev		db	"Ocorreu um erro de rede no dispositivo "
devenam3		db	8 dup (?)
			db	0

msgNoPrinter            db      "A impressora n�o est� pronta",0


ifndef WINDEBUG
public szExitStr1,szExitStr2
szExitStr1  DB  7,13,10,'FatalExit code = ',0
szExitStr2  DB  ' stack overflow',13,10,0
public  szUndefDyn
szUndefDyn      db      "Chamada a um v�nculo din�mico indefinido",0
public  szFatalExit
szFatalExit	db	"O aplicativo foi encerrado de forma anormal",0
else
public szDebugStr
szDebugStr  DB  'KERNEL: Erro ao carregar - ',0                   	 ; 0
            DB  'KERNEL: Erro ao carregar uma nova c�pia de - ',0 ; 1
            DB  'Erro ao carregar o arquivo de recursos - ',0        ; 2
            DB  13,10,0                                         ; 3
            DB  7,13,10,'C�digo de sa�da fatal = ',0                   ; 4
            DB  ' estouro de pilha',0                             ; 5
            DB  13,10,'Segmento da pilha:',13,10,0                    ; 6
	    DB  7,13,10,'Anular, Interromper, Sair ou Ignorar? ',0      ; 7
            DB  'Cadeia BP inv�lida',7,13,10,0                    ; 8
	    DB	': ',0						; 9
	    DB	'Sa�da fatal fornecida outra vez',7,13,10,0 		; 10
	    DB  0
public szFKE
szFKE	DB '*** Erro fatal de n�cleo ***',0
endif

;** Diagnostic mode messages
        public szDiagStart, szCRLF, szLoadStart, szLoadSuccess, szLoadFail
        public szFailCode, szCodeString
szDiagStart     db      '[boot]'      ;lpCRLF must follow
szCRLF          db      0dh, 0ah, 0
szLoadStart     db      'In�cio de carregamento = ',0
szLoadSuccess   db      'Carregamento correto = ', 0
szLoadFail      db      'Carregamento incorreto = ', 0
szFailCode      db      ' O c�digo de erro � ' ;szCodeString must follow
szCodeString    db      '00', 0dh, 0ah, 0
ifdef WINDEBUG
	public szInitSpew
szInitSpew      DB      'Inicializa��o do modo de diagn�stico. O arquivo de log �: ', 0
endif

_DATA	ENDS


_INITTEXT SEGMENT WORD PUBLIC 'CODE'
public szInadequate, szNoPMode, szNoGlobalInit
public NoOpenFile, NoLoadHeader, szMissingMod, szPleaseDoIt

ifdef WOW
public szGenBootFail
szNoPMode db "O Subsistema Win16 n�o conseguiu entrar no modo protegido. DOSX.EXE deve estar no arquivo AUTOEXEC.NT e em seu PATH.",0
szMissingMod    db   "NTVDM KERNEL: O m�dulo do sistema de 16 bits est� faltando",0
szPleaseDoIt    db   "Reinstale o seguinte m�dulo na pasta system32:",13,10,9,9,0
szInadequate	db   "NTVDM KERNEL: Servidor DPMI inadequado",0
szNoGlobalInit	db   "NTVDM KERNEL: N�o foi poss�vel inicializar a pilha",0
NoOpenFile	db   "NTVDM KERNEL: N�o foi poss�vel abrir o execut�vel KERNEL",0
NoLoadHeader	db   "NTVDM KERNEL: N�o foi poss�vel carregar o cabe�alho do execut�vel KERNEL",0
szGenBootFail   db   "NTVDM KERNEL: Falha na inicializa��o do subsistema Win16",0
else
szInadequate	db   'KERNEL: O servidor DPMI n�o � adequado',13,10,'$'
szNoPMode	db   'KERNEL: N�o foi poss�vel entrar no modo protegido',13,10,'$'
szNoGlobalInit	db   "KERNEL: N�o foi poss�vel inicializar a pilha",13,10,'$'
NoOpenFile      db   "KERNEL: N�o foi poss�vel abrir o execut�vel KERNEL"
                db   13, 10, '$'
NoLoadHeader    db   "KERNEL: N�o foi poss�vel carregar o cabe�alho do execut�vel KERNEL"
		db   13, 10, '$'
endif

_INITTEXT ENDS




_NRESTEXT SEGMENT WORD PUBLIC 'CODE'

; This is the caption string for the protect mode dialog box.
;
; DO NOT CHANGE THE LENGTH OF THIS MESSAGE
;

public szProtectCap,msgRealModeApp1,msgRealModeApp2

szProtectCap		db	"Aviso sobre a compatibilidade do aplicativo",0

msgRealModeApp1 db	"O aplicativo que voc� vai executar foi ",0
msgRealModeApp2 db	"desenvolvido para uma vers�o anterior do Windows.",0Dh,0Dh
	db	"Obtenha uma vers�o atualizada do aplicativo que seja compat�vel "
	db	"com o Windows 3.0 ou posterior.    ",13,13
	db	"Se for escolhido OK e o aplicativo for inicializado, problemas de compatibilidade "
	db	"poder�o fazer com que o aplicativo ou o Windows sejam encerrados inesperadamente.",0

_NRESTEXT ENDS

end
