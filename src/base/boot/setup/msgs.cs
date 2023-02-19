;/*++
;
;Copyright (c) 1991  Microsoft Corporation
;
;Module Name:
;
;    msgs.h
;
;Abstract:
;
;    This file contains the message definitions for setupldr
;
;Author:
;
;    John Vert (jvert) 12-Nov-1993
;
;Revision History:
;
;Notes:
;
;    This file is generated from msgs.mc
;
;--*/
;
;#ifndef _SETUPLDR_MSG_
;#define _SETUPLDR_MSG_
;
;

MessageID=9000 SymbolicName=SL_MSG_FIRST
Language=English
.


MessageID=9001 SymbolicName=SL_SCRN_WELCOME
Language=English
V�t� v�s instala�n� program syst�mu Windows

           Pokra�ujte kl�vesou ENTER

         Instalaci ukon��te kl�vesou F3
.

MessageID=9002 SymbolicName=SL_WELCOME_HEADER
Language=English

 Instalace syst�mu Windows
�����������������������������������
.

MessageID=9003 SymbolicName=SL_TOTAL_SETUP_DEATH
Language=English
Instalace se nepoda�ila. Libovolnou kl�vesou restartujte po��ta�.
.

MessageID=9004 SymbolicName=SL_FILE_LOAD_MESSAGE
Language=English
Instala�n� program na��t� soubory (%s)...
.

MessageID=9005 SymbolicName=SL_OTHER_SELECTION
Language=English
Jin� (vy�aduje disketu s ovlada�em OEM)
.

MessageID=9006 SymbolicName=SL_SELECT_DRIVER_PROMPT
Language=English
ENTER=Vybrat  F3=Konec
.

MessageID=9007 SymbolicName=SL_NEXT_DISK_PROMPT_CANCELLABLE
Language=English
ENTER=Pokra�ovat  ESC=Storno  F3=Konec
.

MessageID=9008 SymbolicName=SL_OEM_DISK_PROMPT
Language=English
Podp�rn� disketa dodan� v�robcem hardwaru
.

MessageID=9009 SymbolicName=SL_MSG_INSERT_DISK
Language=English
  Vlo�te disketu, ozna�enou jako



           do jednotky A:

*  Potvr�te vlo�en� kl�vesou ENTER.
.

MessageID=9010 SymbolicName=SL_MSG_EXIT_DIALOG
Language=English
������������������������������������������������������ͻ
�  Syst�m Windows nen� na po��ta�i zcela               �
�  nainstalov�n. Ukon��te-li nyn� instala�n� program,  �
�  budete jej muset spustit znovu, aby se syst�m       �
�  Windows nainstaloval.                               �
�                                                      �
�     * Stisknete-li ENTER, instalace bude pokra�ovat. �
�     * Stisknut�m kl�vesy F3 instalaci ukon��te.      �
������������������������������������������������������͹
�     F3=Konec  ENTER=Pokra�ovat                       �
������������������������������������������������������ͼ
.

MessageID=9011 SymbolicName=SL_NEXT_DISK_PROMPT
Language=English
ENTER=Pokra�ovat  F3=Konec
.

MessageID=9012 SymbolicName=SL_NTDETECT_PROMPT
Language=English

Instala�n� program zji�uje hardwarovou konfiguraci po��ta�e...

.

MessageID=9013 SymbolicName=SL_KERNEL_NAME
Language=English
V�konn� j�dro syst�mu Windows
.

MessageID=9014 SymbolicName=SL_HAL_NAME
Language=English
Vrstva HAL
.

MessageID=9015 SymbolicName=SL_PAL_NAME
Language=English
Dopl�ky procesoru syst�mu Windows
.

MessageID=9016 SymbolicName=SL_HIVE_NAME
Language=English
Konfigura�n� data syst�mu Windows
.

MessageID=9017 SymbolicName=SL_NLS_NAME
Language=English
Jazykov� z�visl� data
.

MessageID=9018 SymbolicName=SL_OEM_FONT_NAME
Language=English
P�smo instala�n�ho programu
.

MessageID=9019 SymbolicName=SL_SETUP_NAME
Language=English
Instalace syst�mu Windows
.

MessageID=9020 SymbolicName=SL_FLOPPY_NAME
Language=English
Ovlada� disketov� jednotky
.

MessageID=9021 SymbolicName=SL_KBD_NAME
Language=English
Ovlada� kl�vesnice
.

MessageID=9121 SymbolicName=SL_FAT_NAME
Language=English
Syst�m soubor� FAT
.

MessageID=9022 SymbolicName=SL_SCSIPORT_NAME
Language=English
Ovlada� rozhran� SCSI
.

MessageID=9023 SymbolicName=SL_VIDEO_NAME
Language=English
Grafick� ovlada�
.

MessageID=9024 SymbolicName=SL_STATUS_REBOOT
Language=English
Restartujte po��ta� stisknut�m libovoln� kl�vesy.
.

MessageID=9025 SymbolicName=SL_WARNING_ERROR
Language=English
Nastala neo�ek�van� chyba (%d)
na ��dku %d v souboru %s.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9026 SymbolicName=SL_FLOPPY_NOT_FOUND
Language=English
Bylo nalezeno pouze %d disketov�ch jednotek,
syst�m se sna�il nal�zt jednotku %d.
.

MessageID=9027 SymbolicName=SL_NO_MEMORY
Language=English
Syst�mu do�la ve�ker� voln� pam؜
na ��dku %d v souboru %s.
.

MessageID=9028 SymbolicName=SL_IO_ERROR
Language=English
V syst�mu do�lo k V/V chyb�
p�i p��stupu k souboru %s.
.

MessageID=9029 SymbolicName=SL_BAD_INF_SECTION
Language=English
Sekce %s souboru INF je neplatn�.
.

MessageID=9030 SymbolicName=SL_BAD_INF_LINE
Language=English
��dek %d souboru INF %s je neplatn�
.

MessageID=9031 SymbolicName=SL_BAD_INF_FILE
Language=English
Soubor %s (INF) je po�kozen nebo chyb�, stav %d.
.

MessageID=9032 SymbolicName=SL_FILE_LOAD_FAILED
Language=English
Soubor %s se nepoda�ilo na��st.

K�d chyby je %d
.

MessageID=9033 SymbolicName=SL_INF_ENTRY_MISSING
Language=English
Polo�ka "%s" v sekci [%s]
souboru INF je po�kozena nebo chyb�.
.

MessageID=9034 SymbolicName=SL_PLEASE_WAIT
Language=English
Po�kejte pros�m...
.

MessageID=9035 SymbolicName=SL_CANT_CONTINUE
Language=English
Instalace nem��e pokra�ovat. Ukon�ete instalaci libovolnou kl�vesou.
.

MessageID=9036 SymbolicName=SL_PROMPT_SCSI
Language=English
Vyberte po�adovan� adapt�r SCSI ze seznamu, nebo zvolte "Jin�",
m�te-li disketu s podporou dan�ho za��zen� od v�robce adapt�ru.

.

MessageID=9037 SymbolicName=SL_PROMPT_OEM_SCSI
Language=English
Zvolili jste mo�nost konfigurovat adapt�r SCSI pro pou�it� v syst�mu 
Windows pomoc� diskety s podporou za��zen� od v�robce adapt�ru.

Vyberte po�adovan� adapt�r SCSI z n�sleduj�c�ho seznamu, nebo se
stisknut�m kl�vesy ESC vra�te na p�edchoz� obrazovku.

.
MessageID=9038 SymbolicName=SL_PROMPT_HAL
Language=English
Instala�n�mu programu se nepoda�ilo ur�it typ po��ta�e, nebo byla 
zvolena mo�nost typ po��ta�e zadat ru�n�.

Vyberte typ po��ta�e z n�sleduj�c�ho seznamu, nebo zvolte "Jin�",
m�te-li disketu s podporou za��zen� od v�robce po��ta�e.

K posouv�n� mezi polo�kami nab�dky pou�ijte kl�vesy �ipka nahoru a dol�.

.

MessageID=9039 SymbolicName=SL_PROMPT_OEM_HAL
Language=English
Zvolili jste mo�nost konfigurovat po��ta� pro syst�m Windows
pomoc� diskety s podporou za��zen� od v�robce po��ta�e.

Zadejte typ po��ta�e z n�sleduj�c�ho seznamu, nebo se stisknut�m
tla��tka ESC vra�te na p�edchoz� obrazovku.

K posouv�n� mezi polo�kami nab�dky pou�ijte kl�vesy �ipka nahoru a dol�.

.

MessageID=9040 SymbolicName=SL_PROMPT_VIDEO
Language=English
Instala�n�mu programu se nepoda�ilo ur�it typ nainstalovan�ho 
grafick�ho adapt�ru.

Vyberte grafick� adapt�r ze seznamu, nebo zvolte "Jin�",
m�te-li disketu s podporou za��zen� od v�robce adapt�ru.

.

MessageID=9041 SymbolicName=SL_PROMPT_OEM_VIDEO
Language=English
Zvolili jste mo�nost konfigurovat grafick� adapt�r pro syst�m Windows
pomoc� diskety s podporou za��zen� od v�robce adapt�ru.

Vyberte grafick� adapt�r z n�sleduj�c�ho seznamu, nebo se stisknut�m
tla��tka ESC vra�te na p�edchoz� obrazovku.

.


MessageID=9042 SymbolicName=SL_WARNING_ERROR_WFILE
Language=English
Soubor %s zp�sobil neo�ek�vanou chybu (%d)
na ��dku %d v %s.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9043 SymbolicName=SL_WARNING_IMG_CORRUPT
Language=English
Soubor %s je po�kozen.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9044 SymbolicName=SL_WARNING_IOERR
Language=English
P�i p��stupu k souboru %s do�lo k V/V chyb�.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9045 SymbolicName=SL_WARNING_NOFILE
Language=English
Soubor %s nebyl nalezen.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9046 SymbolicName=SL_WARNING_NOMEM
Language=English
Nedostatek pam�ti pro %s.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9047 SymbolicName=SL_DRIVE_ERROR
Language=English
SETUPLDR: Jednotku %s nelze otev��t
.

MessageID=9048 SymbolicName=SL_NTDETECT_MSG
Language=English

Instala�n� program zji�uje hardwarovou konfiguraci po��ta�e...

.

MessageID=9049 SymbolicName=SL_NTDETECT_FAILURE
Language=English
NTDETECT selhal
.

MessageId=9050 SymbolicName=SL_SCRN_TEXTSETUP_EXITED
Language=English
Syst�m Windows nebyl nainstalov�n.

Pokud je v jednotce A: vlo�ena disketa, vyjm�te ji.

Restartujte po��ta� stisknut�m kl�vesy ENTER.
.

MessageId=9051 SymbolicName=SL_SCRN_TEXTSETUP_EXITED_ARC
Language=English
Syst�m Windows nebyl nainstalov�n.

Restartujte po��ta� stisknut�m kl�vesy ENTER.
.

MessageID=9052 SymbolicName=SL_REBOOT_PROMPT
Language=English
ENTER=Restartovat po��ta�
.

MessageID=9053 SymbolicName=SL_WARNING_SIF_NO_DRIVERS
Language=English
Instala�n� program nenalezl ��dn� ovlada�e spojen� se zadanou volbou.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9054 SymbolicName=SL_WARNING_SIF_NO_COMPONENT
Language=English
Vlo�en� disketa neobsahuje ��dn� odpov�daj�c� soubory podpory.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9055 SymbolicName=SL_WARNING_BAD_FILESYS
Language=English
Tuto disketu nelze p�e��st, proto�e obsahuje nerozpoznan� syst�m soubor�.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.

MessageID=9056 SymbolicName=SL_BAD_UNATTENDED_SCRIPT_FILE
Language=English
Polo�ka

"%s"

souboru skriptu bezobslu�n� instalace
v sekci [%s] souboru INF %s neexistuje.
.

MessageID=9057 SymbolicName=SL_MSG_PRESS_F5_OR_F6
Language=English
Chcete-li nainstalovat jin� ovlada�e za��zen� SCSI nebo RAID, stiskn�te F6...
.

;//
;// The following three messages are used to provide the same mnemonic
;// keypress as is used in the Additional SCSI screen in setupdd.sys
;// (see setup\textmode\user\msg.mc--SP_MNEMONICS and SP_MNEMONICS_INFO)
;// The single character specified in SL_SCSI_SELECT_MNEMONIC must be
;// the same as that listed in the status text of SL_SCSI_SELECT_PROMPT
;// (and also referenced in the SL_SCSI_SELECT_MESSAGE_2).
;//
MessageID=9060 SymbolicName=SL_SCSI_SELECT_MNEMONIC
Language=English
S
.

MessageID=9061 SymbolicName=SL_SCSI_SELECT_PROMPT
Language=English
S=Specifikovat dal� za��zen�   ENTER=Pokra�ovat   F3=Konec
.

MessageID=9062 SymbolicName=SL_SCSI_SELECT_MESSAGE_2
Language=English
  * Chcete-li specifikovat dal� adapt�ry SCSI, jednotky CD-ROM, nebo 
    speci�ln� �adi�e disk� pro pou�it� v syst�mu Windows, 
    v�etn� t�ch, pro nا m�te disketu s podporou za��zen� od v�robce 
    velkokapacitn�ch ukl�dac�ch za��zen�, stiskn�te kl�vesu S.

  * Pokud nem�te disketu s podporou za��zen� od v�robce velkokapacitn�ch
    za��zen�, nebo nechcete specifikovat ��dn� dal� za��zen� pro 
    pou�it� v syst�mu Windows, stiskn�te kl�vesu ENTER.
.

MessageID=9063 SymbolicName=SL_SCSI_SELECT_MESSAGE_1
Language=English
Instala�n� program nemohl ur�it typ jednoho nebo v�ce velkokapacitn�ch
za��zen�, nainstalovan�ch v syst�mu, nebo byla zvolena mo�nost ur�it 
adapt�r ru�n�. Instala�n� program nyn� na�te podporu n�sleduj�c�ch 
velkokapacitn�ch ukl�dac�ch za��zen�:
.

MessageID=9064 SymbolicName=SL_SCSI_SELECT_MESSAGE_3
Language=English
Instala�n� program na�te podporu n�sleduj�c�ch velkokapacitn�ch 
ukl�dac�ch za��zen�:
.

MessageID=9065 SymbolicName=SL_SCSI_SELECT_ERROR
Language=English
Instala�n� program nemohl na��st podporu pro ur�en� velkokapacitn�
ukl�dac� za��zen�. Instala�n� program nyn� na�te podporu n�sleduj�c�ch 
velkokapacitn�ch ukl�dac�ch za��zen�:
.

MessageID=9066 SymbolicName=SL_TEXT_ANGLED_NONE
Language=English
<��dn�>
.

MessageID=9067 SymbolicName=SL_TEXT_SCSI_UNNAMED
Language=English
<nepojmenovan� adapt�r>
.

MessageID=9068 SymbolicName=SL_TEXT_OTHER_DRIVER
Language=English
Jin�
.

MessageID=9069 SymbolicName=SL_TEXT_REQUIRES_486
Language=English
Syst�m Windows vy�aduje procesor 80486 nebo vy��.
.

MessageID=9070 SymbolicName=SL_NTPNP_NAME
Language=English
Exportn� ovlada� Plug & Play
.

MessageID=9071 SymbolicName=SL_PCI_IDE_EXTENSIONS_NAME
Language=English
Ovlada� roz��en� PCI IDE
.

MessageID=9072 SymbolicName=SL_HW_FW_CFG_CLASS
Language=English
Syst�m Windows se nepoda�ilo spustit z d�vodu n�sleduj�c�ho
probl�mu spou�t�c� konfigurace firmwaru ARC:
.

MessageID=9073 SymbolicName=DIAG_SL_FW_GET_BOOT_DEVICE
Language=English
Nastaven� parametru 'osloadpartition' je neplatn�.
.

MessageID=9074 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
Mo�nosti konfigurace ARC naleznete v dokumentaci k syst�mu Windows
Dal� informace naleznete v p��ru�k�ch k hardwaru.
.
MessageID=9075 SymbolicName=SL_NETADAPTER_NAME
Language=English
Ovlada� s��ov�ho adapt�ru
.
MessageID=9076 SymbolicName=SL_TCPIP_NAME
Language=English
Slu�ba TCP/IP
.
MessageID=9077 SymbolicName=SL_NETBT_NAME
Language=English
Slu�ba WINS Client(TCP/IP)
.
MessageID=9078 SymbolicName=SL_MRXSMB_NAME
Language=English
Mini-p�esm�rova� MRXSMB
.
MessageID=9079 SymbolicName=SL_MUP_NAME
Language=English
Syst�m soubor� UNC
.
MessageID=9080 SymbolicName=SL_NDIS_NAME
Language=English
Ovlada� NDIS
.
MessageID=9081 SymbolicName=SL_RDBSS_NAME
Language=English
Syst�m soubor� SMB Redirector
.
MessageID=9082 SymbolicName=SL_NETBOOT_CARD_ERROR
Language=English
S��ov� karta ve va�em po��ta�i obsahuje star� verzi pam�ti ROM
a nelze ji pou��t pro vzd�lenou instalaci syst�mu Windows.
Obra�te se na spr�vce syst�mu nebo v�robce po��ta�e s ��dost�
o inovaci pam�ti ROM.
.
MessageID=9083 SymbolicName=SL_NETBOOT_SERVER_ERROR
Language=English
Vybran� obraz opera�n�ho syst�mu neobsahuje nezbytn� ovlada�e
s��ov�ho adapt�ru. Pokuste se vybrat jin� obraz opera�n�ho
syst�mu. Pokud probl�m trv�, obra�te se na spr�vce syst�mu.
.
MessageID=9084 SymbolicName=SL_IPSEC_NAME
Language=English
Zabezpe�en� protokolu IP 
.
MessageID=9085 SymbolicName=SL_CMDCONS_MSG
Language=English
Konzola pro zotaven� syst�mu Windows
.
MessageID=9086 SymbolicName=SL_KERNEL_TRANSITION
Language=English
Instala�n� program spou�t� syst�m Windows
.
;#ifdef _WANT_MACHINE_IDENTIFICATION
MessageID=9087 SymbolicName=SL_BIOSINFO_NAME
Language=English
Data pro identifikaci po��ta�e
.
;#endif
MessageID=9088 SymbolicName=SL_KSECDD_NAME
Language=English
Slu�ba Zabezpe�en� j�dra
.
MessageID=9089 SymbolicName=SL_WRONG_PROM_VERSION
Language=English
Syst�mov� pam؜ PROM (obsahuj�c� firmware) je na ni�� �rovni revize
ne� je po�adov�no. Obra�te se na technickou podporu spole�nosti SGI nebo
nav�tivte webov� server spole�nosti SGI a po��dejte o aktualizaci pam�ti PROM
a instrukce k proveden� aktualizace.

Pozn�mka: V�b�rem p��slu�n� polo�ky spou�t�c� obrazovky m�sto v�choz�
polo�ky Instalace syst�mu Windows m��ete spustit p�edchoz� instalace
syst�mu Microsoft(R) Windows(R) NT nebo Windows(R) XP.
.
MessageID=9090 SymbolicName=SIGNATURE_CHANGED
Language=English
Instala�n� program zjistil v po��ta�i v�ce disk�, kter� nelze rozli�it.
Instala�n� program probl�m opravil, ale je nutno restartovat po��ta�.
.
MessageID=9091 SymbolicName=SL_KDDLL_NAME
Language=English
Kernel Debugger DLL
.

MessageID=9092 SymbolicName=SL_OEM_DRIVERINFO
Language=English
%s

Syst�m Windows ji� m� ovlada� pro "%s".

Pokud v�robce nedoporu�uje jinak, m�li byste pou��t ovlada� syst�mu Windows.
.

MessageID=9093 SymbolicName=SL_CONFIRM_OEMDRIVER
Language=English
S=Pou��t ovlada� na disket�  ENTER=Pou��t v�choz� ovlada� syst�mu Windows
.

MessageID=9094 SymbolicName=SL_OEMDRIVER_NEW
Language=English
Poskytnut� ovlada� je pravd�podobn� nov�j� ne� v�choz� ovlada� syst�mu Windows.
.

MessageID=9095 SymbolicName=SL_INBOXDRIVER_NEW
Language=English
Poskytnut� ovlada� je pravd�podobn� star� ne� v�choz� ovlada� syst�mu Windows
.

MessageID=9096 SymbolicName=SL_CMDCONS_STARTING
Language=English
Spou�t�n� konzoly pro obnoven�...
.

MessageID=9097 SymbolicName=SL_NTDETECT_CMDCONS
Language=English
Modul NTDETECT kontroluje hardware...
.

MessageID=9098 SymbolicName=SL_MSG_PRESS_ASR
Language=English
Stisknut�m k�vesy F2 spust�te Automatick� obnoven� syst�mu (ASR)...
.

MessageID=9099 SymbolicName=SL_MSG_WARNING_ASR
Language=English
        Vlo�te disketu ozna�enou:


Disketa pro automatick� obnoven� syst�mu Windows


            do disketov� jednotky.



          Pak stiskn�te libovolnou kl�vesu.
.


MessageID=9100 SymbolicName=SL_TEXT_REQUIRED_FEATURES_MISSING
Language=English

Syst�m Windows po�aduje ur�it� funkce procesoru,
kter� nejsou v procesoru tohoto po��ta�e k dispozici.
Syst�m Windows zejm�na po�aduje instrukce:

    CPUID
    CMPXCHG8B
.

MessageID=9101 SymbolicName=SL_MSG_PREPARING_ASR
Language=English
P��prava Automatick�ho obnoven� syst�mu (ASR) (zru�it m��ete kl�vesou ESC)...
.

MessageID=9102 SymbolicName=SL_MSG_ENTERING_ASR
Language=English
Spou�t�n� Automatick�ho obnoven� syst�mu...
.

MessageID=9103 SymbolicName=SL_MOUSE_NAME
Language=English
Ovlada� my�i
.

MessageID=9104 SymbolicName=SL_SETUP_STARTING
Language=English
Spou�t�n� instalace syst�mu Windows...
.

MessageID=9105 SymbolicName=SL_MSG_INVALID_ASRPNP_FILE
Language=English
Soubor ASRPNP.SIF na disket� Automatick�ho obnoven� syst�mu je neplatn�.
.

MessageID=9106 SymbolicName=SL_SETUP_STARTING_WINPE
Language=English
Spou�t�n� p�edinstala�n�ho prost�ed� syst�mu Windows...
.

MessageID=9107 SymbolicName=SL_NTDETECT_ROLLBACK
Language=English

Odinstalace kontroluje hardware...


.

MessageID=9108 SymbolicName=SL_ROLLBACK_STARTING
Language=English
Spou�t�n� odinstalace syst�mu Windows...
.

MessageID=9109 SymbolicName=SL_NO_FLOPPY_DRIVE
Language=English
Instala�n� program nenalezl v po��ta�i disketovou jednotku k na�ten�
ovlada�� z diskety od v�robce OEM.

    * Na��t�n� ovlada�� v�robce OEM zru�te stisknut�m kl�vesy ESC 
    
    * Instalaci ukon��te stisknut�m kl�vesy F3.
.

MessageID=9110 SymbolicName=SL_UPGRADE_IN_PROGRESS
Language=English
Tento po��ta� je aktualizov�n syst�mem Microsoft Windows.
Jak chcete pokra�ovat?

    * Stisknut�m kl�vesy ENTER.bude pokra�ovat aktualizace

    * Chcete-li aktualizaci zru�it a nainstalovat novou verzi
      syst�mu Microsoft Windows, stiskn�te kl�vesu F10.

    * Chcete-li instala�n� program ukon�it bez instalace 
      syst�mu Microsoft Windows, stiskn�te kl�vesu F3.
.

MessageID=9111 SymbolicName=SL_DRVMAINSDB_NAME
Language=English
Data k identifikaci ovlada�e
.

MessageID=9112 SymbolicName=SL_OEM_FILE_LOAD_FAILED
Language=English
Na�ten� ovlada�� v�robce OEM se nezda�ilo.

Pokra�ujte stisknut�m libovoln� kl�vesy.
.


; //
; // NOTE : donot change the Message ID values for SL_CMDCONS_PROGBAR_FRONT
; // and SL_CMDCONS_PROGBAR_BACK from 11513 & 11514
; //

;
; // The character used to draw the foregound in percent-complete bar
;
;
MessageID=11513 SymbolicName=SL_CMDCONS_PROGBAR_FRONT
Language=English
�
.

;
; // The character used to draw the background in percent-complete bar
;
;
MessageID=11514 SymbolicName=SL_CMDCONS_PROGBAR_BACK
Language=English
�
.

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

;#endif // _SETUPLDR_MSG_
