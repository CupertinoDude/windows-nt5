;/*++
;
;Copyright (c) 1991  Microsoft Corporation
;
;Module Name:
;
;    msg.h
;
;Abstract:
;
;    This file contains the message definitions for osloader
;
;Author:
;
;    John Vert (jvert) 12-Nov-1993
;
;Revision History:
;
;Notes:
;

;    This file is generated from msg.mc
;
;--*/
;
;#ifndef _BLDR_MSG_
;#define _BLDR_MSG_
;
;

MessageID=9000 SymbolicName=BL_MSG_FIRST
Language=English
.

MessageID=9001 SymbolicName=LOAD_SW_INT_ERR_CLASS
Language=English
Syst�m Windows se nepoda�ilo spustit z d�vodu chyby softwaru.
Oznamte uveden� pot��e:
.

MessageID=9002 SymbolicName=LOAD_SW_MISRQD_FILE_CLASS
Language=English
Syst�m Windows se nepoda�ilo spustit, nebyl nalezen uveden�
soubor, kter� pot�ebuje:
.

MessageID=9003 SymbolicName=LOAD_SW_BAD_FILE_CLASS
Language=English
Syst�m Windows nelze spustit z d�vodu po�kozen�ho souboru,
viz d�le:
.

MessageID=9004 SymbolicName=LOAD_SW_MIS_FILE_CLASS
Language=English
Syst�m Windows nelze spustit. Uveden� soubor je po�kozen 
nebo nebyl nalezen:
.

MessageID=9005 SymbolicName=LOAD_HW_MEM_CLASS
Language=English
Syst�m Windows nelze spustit z d�vodu pot��� 
s hardwarovou konfigurac� pam�ti.
.

MessageID=9006 SymbolicName=LOAD_HW_DISK_CLASS
Language=English
Syst�m Windows nelze spustit z d�vodu pot��� s hardwarovou 
konfigurac� po��ta�ov�ho disku.
.

MessageID=9007 SymbolicName=LOAD_HW_GEN_ERR_CLASS
Language=English
Syst�m Windows nelze spustit z d�vodu obecn�ch pot��� s hardwarovou
konfigurac� po��ta�e.
.

MessageID=9008 SymbolicName=LOAD_HW_FW_CFG_CLASS
Language=English
Syst�m Windows nelze spustit z d�vodu pot���
s konfigurac� startovac�ho firmwaru ARC:
.

MessageID=9009 SymbolicName=DIAG_BL_MEMORY_INIT
Language=English
Zkontrolujte hardwarovou konfiguraci pam�ti a velikost RAM.
.

MessageID=9010 SymbolicName=DIAG_BL_CONFIG_INIT
Language=English
P��li� mnoho konfigura�n�ch polo�ek
.

MessageID=9011 SymbolicName=DIAG_BL_IO_INIT
Language=English
Tabulky diskov�ch odd�l� nebyly p��stupn�
.

MessageID=9012 SymbolicName=DIAG_BL_FW_GET_BOOT_DEVICE
Language=English
Nastaven� parametru 'osloadpartition' je neplatn�.
.

MessageID=9013 SymbolicName=DIAG_BL_OPEN_BOOT_DEVICE
Language=English
Ze zadan�ho spou�t�c�ho disku nelze ��st. Zkontrolujte spou�t�c�
cestu a hardware disku.
.

MessageID=9014 SymbolicName=DIAG_BL_FW_GET_SYSTEM_DEVICE
Language=English
Nastaven� parametru 'systempartition' je neplatn�.
.

MessageID=9015 SymbolicName=DIAG_BL_FW_OPEN_SYSTEM_DEVICE
Language=English
Ze zadan�ho spou�t�c�ho disku syst�mu nelze ��st.
Zkontrolujte cestu 'systempartition'.
.

MessageID=9016 SymbolicName=DIAG_BL_GET_SYSTEM_PATH
Language=English
Parametr 'osloadfilename' se neodkazuje na platn� soubor.
.

MessageID=9017 SymbolicName=DIAG_BL_LOAD_SYSTEM_IMAGE
Language=English
<Windows root>\system32\ntoskrnl.exe.
.

MessageID=9018 SymbolicName=DIAG_BL_FIND_HAL_IMAGE
Language=English
Parametr 'osloader' neodkazuje na platn� soubor.
.

MessageID=9019 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_X86
Language=English
<Windows root>\system32\hal.dll.
.

MessageID=9020 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_ARC
Language=English
'osloader'\hal.dll
.
;#ifdef _X86_
;#define DIAG_BL_LOAD_HAL_IMAGE DIAG_BL_LOAD_HAL_IMAGE_X86
;#else
;#define DIAG_BL_LOAD_HAL_IMAGE DIAG_BL_LOAD_HAL_IMAGE_ARC
;#endif

MessageID=9021 SymbolicName=DIAG_BL_LOAD_SYSTEM_IMAGE_DATA
Language=English
Zavad؟, chyba 1.
.

MessageID=9022 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_DATA
Language=English
Zavad؟, chyba 2.
.

MessageID=9023 SymbolicName=DIAG_BL_LOAD_SYSTEM_DLLS
Language=English
Zavad؟ pot�eboval knihovny DLL pro j�dro syst�mu.
.

MessageID=9024 SymbolicName=DIAG_BL_LOAD_HAL_DLLS
Language=English
Zavad؟ pot�eboval knihovny DLL pro vrstvu HAL.
.

MessageID=9026 SymbolicName=DIAG_BL_FIND_SYSTEM_DRIVERS
Language=English
naj�t ovlada�e syst�mu.
.

MessageID=9027 SymbolicName=DIAG_BL_READ_SYSTEM_DRIVERS
Language=English
��st ovlada�e syst�mu.
.

MessageID=9028 SymbolicName=DIAG_BL_LOAD_DEVICE_DRIVER
Language=English
nezavedl ovlada� za��zen� spou�t�c�ho syst�mu.
.

MessageID=9029 SymbolicName=DIAG_BL_LOAD_SYSTEM_HIVE
Language=English
zav�d�t konfigura�n� soubor syst�mov�ho hardwaru.
.


MessageID=9030 SymbolicName=DIAG_BL_SYSTEM_PART_DEV_NAME
Language=English
naj�t n�zev za��zen� syst�mov�ho odd�lu.
.

MessageID=9031 SymbolicName=DIAG_BL_BOOT_PART_DEV_NAME
Language=English
naj�t n�zev spou�t�c�ho odd�lu.
.

MessageID=9032 SymbolicName=DIAG_BL_ARC_BOOT_DEV_NAME
Language=English
nevygeneroval spr�vn� n�zev ARC pro HAL a syst�mov� cesty.
.

MessageID=9034 SymbolicName=DIAG_BL_SETUP_FOR_NT
Language=English
Zavad؟, chyba 3.
.

MessageID=9035 SymbolicName=DIAG_BL_KERNEL_INIT_XFER
Language=English
<Windows root>\system32\ntoskrnl.exe
.

MessageID=9036 SymbolicName=LOAD_SW_INT_ERR_ACT
Language=English
Kontaktujte technickou podporu a nahlaste probl�m.
.

MessageID=9037 SymbolicName=LOAD_SW_FILE_REST_ACT
Language=English
Tento soubor m��ete zkusit opravit spu�t�n�m instalace syst�mu
Windows pomoc� origin�ln� instala�n� diskety nebo disku CD-ROM.
Opravu spust�te stisknut�m kl�vesy R na prvn� obrazovce.
.

MessageID=9038 SymbolicName=LOAD_SW_FILE_REINST_ACT
Language=English
Nainstalujte znovu uveden� soubor.
.

MessageID=9039 SymbolicName=LOAD_HW_MEM_ACT
Language=English
Hardwarov� po�adavky na pam؜ naleznete v dokumentaci k syst�mu Windows.
Dal� informace naleznete v p��ru�k�ch k hardwaru.
.

MessageID=9040 SymbolicName=LOAD_HW_DISK_ACT
Language=English
Po�adavky na konfiguraci disku naleznete v dokumentaci k syst�mu Windows.
Dal� informace naleznete v p��ru�k�ch k hardwaru.
.

MessageID=9041 SymbolicName=LOAD_HW_GEN_ACT
Language=English
Po�adavky na hardwarovou konfiguraci naleznete v dokumentaci k syst�mu Windows.
Dal� informace naleznete v p��ru�k�ch k hardwaru.
.

MessageID=9042 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
Mo�nosti konfigurace ARC naleznete v dokumentaci k syst�mu Windows.
Dal� informace naleznete v p��ru�k�ch k hardwaru.
.

MessageID=9043 SymbolicName=BL_LKG_MENU_HEADER
Language=English
     Nab�dka Profil hardwaru/Obnoven� konfigurace

Nab�dka umo��uje vybrat hardwarov� profil,
kter� se m� pou��t p�i spu�t�n� syst�mu Windows.

Pokud syst�m nen� spu�t�n spr�vn�, pak lze zvolit p�edchoz�
konfiguraci syst�mu, kter� m��e pot��e p�i spou�t�c� odstranit.
D�le�it�: Zm�ny konfigurace syst�mu, kter� byly provedeny
          po posledn�m �sp��n�m spu�t�n�, budou odstran�ny.
.

MessageID=9044 SymbolicName=BL_LKG_MENU_TRAILER
Language=English
�ipkami na kl�vesnici zv�razn�te polo�ku, kterou chcete vybrat.
Pak stiskn�te kl�vesu ENTER.
.

MessageID=9045 SymbolicName=BL_LKG_MENU_TRAILER_NO_PROFILES
Language=English
Zat�m nejsou definov�ny ��dn� hardwarov� profily.
Hardwarov� profily m��ete vytvo�it pomoc� ovl�dac�ho panelu Syst�m.
.

MessageID=9046 SymbolicName=BL_SWITCH_LKG_TRAILER
Language=English
Stisknut�m kl�vesy P p�epnete do posledn� zn�m� funk�n� konfigurace.
Stisknut�m kl�vesy F3 tuto nab�dku ukon��te a restartujete po��ta�.
.

MessageID=9047 SymbolicName=BL_SWITCH_DEFAULT_TRAILER
Language=English
Stisknut�m kl�vesy V p�epnete do v�choz� konfigurace.
Stisknut�m kl�vesy F3 tuto nab�dku ukon��te a restartujete po��ta�.
.

;//
;// The following two messages are used to provide the mnemonic keypress
;// that toggles between the Last Known Good control set and the default
;// control set. (see BL_SWITCH_LKG_TRAILER and BL_SWITCH_DEFAULT_TRAILER)
;//
MessageID=9048 SymbolicName=BL_LKG_SELECT_MNEMONIC
Language=English
P
.

MessageID=9049 SymbolicName=BL_DEFAULT_SELECT_MNEMONIC
Language=English
V
.

MessageID=9050 SymbolicName=BL_LKG_TIMEOUT
Language=English
Doba, za kterou se zv�razn�n� volba automaticky spust� (sek.): %d
.

MessageID=9051 SymbolicName=BL_LKG_MENU_PROMPT
Language=English

Nab�dku Profil hardwaru/Posledn� funk�n� konfiguraci z�sk�te 
stisknut�m mezern�ku TE�
.

MessageID=9052 SymbolicName=BL_BOOT_DEFAULT_PROMPT
Language=English
Spu�t�n� v�choz� hardwarov� konfigurace
.

;//
;// The following messages are used during hibernation restoration
;//
MessageID=9053 SymbolicName=HIBER_BEING_RESTARTED
Language=English
Syst�m je restartov�n z p�edchoz�ho um�st�n�.
Chcete-li proces p�eru�it, stiskn�te mezern�k.
.
MessageID=9054 SymbolicName=HIBER_ERROR
Language=English
Syst�m nemohl b�t restartov�n z p�edchoz�ho um�st�n�
.
MessageID=9055 SymbolicName=HIBER_ERROR_NO_MEMORY
Language=English
kv�li nedostatku pam�ti.
.
MessageID=9056 SymbolicName=HIBER_ERROR_BAD_IMAGE
Language=English
, proto�e je obraz obnoven� po�kozen.
.
MessageID=9057 SymbolicName=HIBER_IMAGE_INCOMPATIBLE
Language=English
, proto�e obraz obnoven� nen� kompatibiln� s aktu�ln� konfigurac�.
.
MessageID=9058 SymbolicName=HIBER_ERROR_OUT_OF_REMAP
Language=English
kv�li vnit�n� chyb�.
.
MessageID=9059 SymbolicName=HIBER_INTERNAL_ERROR
Language=English
kv�li vnit�n� chyb�.
.
MessageID=9060 SymbolicName=HIBER_READ_ERROR
Language=English
kv�li chyb� p�i �ten�.
.
MessageID=9061 SymbolicName=HIBER_PAUSE
Language=English
Restartov�n� syst�mu bylo pozastaveno:
.
MessageID=9062 SymbolicName=HIBER_CANCEL
Language=English
Odstra�te obnovovac� data a postupte k nab�dce spu�t�n� syst�mu
.
MessageID=9063 SymbolicName=HIBER_CONTINUE
Language=English
Pokra�ovat v restartov�n� syst�mu
.
MessageID=9064 SymbolicName=HIBER_RESTART_AGAIN
Language=English
Posledn� pokus o restartov�n� syst�mu z p�edchoz�ho um�st�n� 
se nezda�il. Chcete se o restartov�n� pokusit znovu?
.

MessageID=9065 SymbolicName=HIBER_DEBUG_BREAK_ON_WAKE
Language=English
Pokra�ovat s ladic�mi zar��kami v syst�mu
.
MessageID=9066 SymbolicName=LOAD_SW_MISMATCHED_KERNEL
Language=English
Syst�m Windows nelze spustit, proto�e ur�en� j�dro
neexistuje nebo nen� kompatibiln� s t�mto procesorem.
.
MessageID=9067 SymbolicName=BL_MSG_STARTING_WINDOWS
Language=English
Spou�t�n� syst�mu Windows...
.
MessageID=9068 SymbolicName=BL_MSG_RESUMING_WINDOWS
Language=English
Obnoven� �innosti syst�mu Windows...
.

MessageID=9069 SymbolicName=BL_EFI_OPTION_FAILURE
Language=English
Syst�m Windows nebyl spu�t�n z d�vodu chyby p�i �ten� nastaven�
spou�t�n� z pam�ti NVRAM.

Zkontrolujte nastaven� firmwaru. Pravd�podobn� budete muset obnovit
nastaven� pam�ti NVRAM ze z�lohy.
.

;
; //
; // Following messages are for the x86-specific
; // boot menu.
; //
;
MessageID=10001 SymbolicName=BL_ENABLED_KD_TITLE
Language=English
 [ladic� program byl aktivov�n]
.

MessageID=10002 SymbolicName=BL_DEFAULT_TITLE
Language=English
Windows (v�choz�)
.

MessageID=10003 SymbolicName=BL_MISSING_BOOT_INI
Language=English
NTLDR: Soubor BOOT.INI nebyl nalezen.
.

MessageID=10004 SymbolicName=BL_MISSING_OS_SECTION
Language=English
NTLDR: Chyb� odd�l [operating systems] v souboru boot.txt.
.

MessageID=10005 SymbolicName=BL_BOOTING_DEFAULT
Language=English
Spou�t�n� v�choz�ho j�dra z %s.
.

MessageID=10006 SymbolicName=BL_SELECT_OS
Language=English
Vyberte opera�n� syst�m, kter� chcete spustit:
.

MessageID=10007 SymbolicName=BL_MOVE_HIGHLIGHT
Language=English


�ipkami nahoru nebo dol� zv�razn�te po�adovanou mo�nost.
Stisknut�m kl�vesy Enter ji potvr�te.

.

MessageID=10008 SymbolicName=BL_TIMEOUT_COUNTDOWN
Language=English
Doba, za kterou se zv�razn�n� volba automaticky spust� (s):
.

MessageID=10009 SymbolicName=BL_INVALID_BOOT_INI
Language=English
Soubor BOOT.INI je neplatn�
Prob�h� spu�t�n� z %s
.

MessageID=10010 SymbolicName=BL_REBOOT_IO_ERROR
Language=English

Vstupn�-v�stupn� chyba p��stupu k souboru spou�t�c�ho sektoru
%s\BOOTSECT.DOS
.

MessageID=10011 SymbolicName=BL_DRIVE_ERROR
Language=English
NTLDR: Nelze otev��t jednotku %s
.

MessageID=10012 SymbolicName=BL_READ_ERROR
Language=English
NTLDR: Kritick� chyba %d p�i �ten� souboru BOOT.INI
.

MessageID=10013 SymbolicName=BL_NTDETECT_MSG
Language=English

NTDETECT v5.0 kontroluje hardware...

.

MessageID=10014 SymbolicName=BL_NTDETECT_FAILURE
Language=English
Do�lo k chyb� v modulu NTDETECT.
.

MessageID=10015 SymbolicName=BL_DEBUG_SELECT_OS
Language=English
Aktu�ln� v�b�r:
  N�zev...: %s
  Cesta...: %s
  Mo�nosti: %s
.

MessageID=10016 SymbolicName=BL_DEBUG_NEW_OPTIONS
Language=English
Zadejte nov� mo�nosti zav�d�n�:
.

MessageID=10017 SymbolicName=BL_HEADLESS_REDIRECT_TITLE
Language=English
 [EMS enabled]
.

MessageID=10018 SymbolicName=BL_INVALID_BOOT_INI_FATAL
Language=English
Neplatn� soubor BOOT.INI
.
;
; //
; // Following messages are for the advanced boot menu
; //
;

MessageID=11001 SymbolicName=BL_ADVANCEDBOOT_TITLE
Language=English
Up�esnit mo�nosti spu�t�n� syst�mu Windows
Zvolte jednu z mo�nost�:
.

MessageID=11002 SymbolicName=BL_SAFEBOOT_OPTION1
Language=English
Stav nouze
.

MessageID=11003 SymbolicName=BL_SAFEBOOT_OPTION2
Language=English
Stav nouze s prac� v s�ti
.

MessageID=11004 SymbolicName=BL_SAFEBOOT_OPTION3
Language=English
Potvrzovat krok za krokem
.

MessageID=11005 SymbolicName=BL_SAFEBOOT_OPTION4
Language=English
Stav nouze se syst�mem MS-DOS
.

MessageID=11006 SymbolicName=BL_SAFEBOOT_OPTION5
Language=English
Re�im VGA
.

MessageID=11007 SymbolicName=BL_SAFEBOOT_OPTION6
Language=English
Obnoven� adres��ov� slu�by (pouze pro �adi�e dom�ny Windows)
.

MessageID=11008 SymbolicName=BL_LASTKNOWNGOOD_OPTION
Language=English
Posledn� zn�m� funk�n� konfigurace
.

MessageID=11009 SymbolicName=BL_DEBUG_OPTION
Language=English
Re�im lad�n�
.

;#if defined(REMOTE_BOOT)
;MessageID=11010 SymbolicName=BL_REMOTEBOOT_OPTION1
;Language=English
;�dr�ba a odstra�ov�n� chyb vzd�len�ho spou�t�n�
;.
;
;MessageID=11011 SymbolicName=BL_REMOTEBOOT_OPTION2
;Language=English
;Vy�istit mezipam؜ offline
;.
;
;MessageID=11012 SymbolicName=BL_REMOTEBOOT_OPTION3
;Language=English
;P�i spou�t�n� pou��t pouze soubory ze serveru
;.
;#endif // defined(REMOTE_BOOT)

MessageID=11013 SymbolicName=BL_BOOTLOG
Language=English
P�i zaveden� povolit p�ihl��en�
.

MessageID=11014 SymbolicName=BL_ADVANCED_BOOT_MESSAGE
Language=English
�e�en� pot��� nebo up�esn�n� mo�nost� spu�t�n� - stiskn�te kl�vesu F8.
.

MessageID=11015 SymbolicName=BL_BASEVIDEO
Language=English
Zapnout re�im VGA
.

MessageID=11016 SymbolicName=BL_DISABLE_SAFEBOOT
Language=English

Vypnut� bezpe�n�ho zav�d�n� syst�mu a norm�ln� zaveden� syst�mu - kl�vesa ESC
.

MessageID=11017 SymbolicName=BL_MSG_BOOT_NORMALLY
Language=English
Spustit syst�m bاn�m zp�sobem
.
MessageID=11018 SymbolicName=BL_MSG_OSCHOICES_MENU
Language=English
Zp�t k v�b�ru opera�n�ho syst�mu
.
MessageID=11019 SymbolicName=BL_MSG_REBOOT

Language=English
Restartovat
.

MessageID=11020 SymbolicName=BL_ADVANCEDBOOT_AUTOLKG_TITLE
Language=English
Omlouv�me se za nep��jemnosti, ale syst�m Windows nebyl �sp��n� spu�t�n 
pravd�podobn� z d�vodu zm�ny hardwaru nebo softwaru.

Pokud po��ta� neodpov�d�, byl ne�ekan� restartov�n nebo byl automaticky
vypnut, aby chr�nil soubory a slo�ky, m��ete se vr�tit k funk�n�mu 
nastaven� volbou Posledn� zn�m� funk�n� konfigurace.

Pokud bylo p�edchoz� spu�t�n� p�eru�eno z d�vodu v�padku nap�jen� nebo
vyp�na�em nap�jen� nebo tla��tkem Reset, nebo pokud je p���ina nezn�m�,
zvolte polo�ku Spustit syst�m bاn�m zp�sobem.
.

MessageID=11021 SymbolicName=BL_ADVANCEDBOOT_AUTOSAFE_TITLE
Language=English
Syst�m nebyl �sp��n� vypnut. Jestli�e se tak stalo, proto�e syst�m
neodpov�dal nebo byl vypnut, aby chr�nil data, je mo�n� jej obnovit
jednou z konfigurac� Nouzov�ho re�imu:
.

MessageID=11022 SymbolicName=BL_ADVANCEDBOOT_TIMEOUT
Language=English
Syst�m Windows se spust� za (sekundy):
.

;
;//
;// Following messages are the symbols used
;// in the Hibernation Restore percent completed UI. 
;//
;// These symbols are here because they employ high-ASCII
;// line drawing characters, which need to be localized
;// for fonts that may not have such characters (or have
;// them in a different ASCII location). 
;//  
;// This primarily affects FE character sets. 
;//
;// Note that only the FIRST character of any of these
;// messages is ever looked at by the code.  
;//
;
; // Just a base message, contains nothing.
;
;

; //
; // NOTE : donot change the Message ID values for HIBER_UI_BAR_ELEMENT
; // and BLDR_UI_BAR_BACKGROUND from 11513 & 11514
;

;
; // The character used to draw the percent-complete bar
;
;
MessageID=11513 SymbolicName=HIBER_UI_BAR_ELEMENT
Language=English
�
.

;
; // The character used to draw the percent-complete bar
;
;

MessageID=11514 SymbolicName=BLDR_UI_BAR_BACKGROUND
Language=English
�
.


;#if defined(REMOTE_BOOT)
;;
;; //
;; // Following messages are for warning the user about
;; // an impending autoformat of the hard disk.
;; //
;;
;
;MessageID=12002 SymbolicName=BL_WARNFORMAT_TRAILER
;Language=English
;Stisknut�m kl�vesy Enter pokra�ujte ve spu�t�n� syst�mu Windows,
;nebo vypnut�m po��ta�e aktivujte vzd�len� spou�t�n�.
;.
;
;MessageID=12003 SymbolicName=BL_WARNFORMAT_CONTINUE
;Language=English
;Pokra�ovat
;.
;MessageID=12004 SymbolicName=BL_FORCELOGON_HEADER
;Language=English
;          Automatick� form�tov�n�
;
;Syst�m Windows rozpoznal v po��ta�i vzd�len�ho spou�t�n� nov�
;pevn� disk. Syst�m nem��e tento disk pou��t, dokud se nep�ihl�s�te
;a nepotvrd�te, �e m�te opr�vn�n� k disku p�istupovat.
;
;UPOZORN�N�: Syst�m Windows automaticky nastav� odd�ly a disk
;naform�tuje tak, aby byl kompatibiln� s nov�m opera�n�m syst�mem.
;Budete-li pokra�ovat, budou v�echna data na pevn�m disku ztracena.
;Jestli�e nechcete pokra�ovat, vypn�te nyn� po��ta� a obra�te se
;na spr�vce syst�mu.
;.
;#endif // defined(REMOTE_BOOT)

;
; //
; // Ramdisk related messages. DO NOT CHANGE the message numbers
; // as they are kept in sync with \base\boot\inc\ramdisk.h.
; //
; // Note that some message numbers are skipped in order to retain
; // consistency with the .NET source base.
; //
;

MessageID=15000 SymbolicName=BL_RAMDISK_GENERAL_FAILURE
Language=English
P�i spou�t�n� syst�mu z ramdisku do�lo k chyb�. Syst�m Windows nelze spustit.
.

MessageID=15003 SymbolicName=BL_RAMDISK_BOOT_FAILURE
Language=English
Otev�en� bitov� kopie ramdisku se nezda�ilo.
.

MessageID=15010 SymbolicName=BL_RAMDISK_DOWNLOAD
Language=English
Na��t�n� bitov� kopie ramdisku...
.

;#endif // _BLDR_MSG_


