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
Det gick inte att starta Windows p� grund av ett fel i
programvaran. Rapportera felet som:
.

MessageID=9002 SymbolicName=LOAD_SW_MISRQD_FILE_CLASS
Language=English
Det gick inte att starta Windows p� grund av att f�ljande
fil inte kunde hittas:
.

MessageID=9003 SymbolicName=LOAD_SW_BAD_FILE_CLASS
Language=English
Det gick inte att starta Windows p� grund av en felaktig kopia
av f�ljande fil:
.

MessageID=9004 SymbolicName=LOAD_SW_MIS_FILE_CLASS
Language=English
Det gick inte att starta Windows eftersom f�ljande fil saknas
eller �r skadad:
.

MessageID=9005 SymbolicName=LOAD_HW_MEM_CLASS
Language=English
Det gick inte att starta Windows p� grund av ett konfigurationsfel
i maskinvaruminnet.
.

MessageID=9006 SymbolicName=LOAD_HW_DISK_CLASS
Language=English
Det gick inte att starta Windows p� grund av ett konfigurationsfel p�
h�rddisken.
.

MessageID=9007 SymbolicName=LOAD_HW_GEN_ERR_CLASS
Language=English
Det gick inte att starta Windows p� grund av ett konfigurationsfel i
maskinvaran.
.

MessageID=9008 SymbolicName=LOAD_HW_FW_CFG_CLASS
Language=English
Det gick inte att starta Windows p� grund av f�ljande konfigurationsfel
i inbyggd ARC:
.

MessageID=9009 SymbolicName=DIAG_BL_MEMORY_INIT
Language=English
Kontrollera maskinvaruminneskonfigurationen och storleken p� RAM.
.

MessageID=9010 SymbolicName=DIAG_BL_CONFIG_INIT
Language=English
F�r m�nga konfigurationsposter.
.

MessageID=9011 SymbolicName=DIAG_BL_IO_INIT
Language=English
Det gick inte att komma �t partitionstabellerna.
.

MessageID=9012 SymbolicName=DIAG_BL_FW_GET_BOOT_DEVICE
Language=English
Inst�llningen f�r parametern osloadpartition �r felaktig.
.

MessageID=9013 SymbolicName=DIAG_BL_OPEN_BOOT_DEVICE
Language=English
Det gick inte att l�sa fr�n angiven startdiskett. Kontrollera starts�kv�gen
och disken.
.

MessageID=9014 SymbolicName=DIAG_BL_FW_GET_SYSTEM_DEVICE
Language=English
Parameterinst�llningen 'systempartition' �r felaktig.
.

MessageID=9015 SymbolicName=DIAG_BL_FW_OPEN_SYSTEM_DEVICE
Language=English
Det gick inte att l�sa fr�n angiven systemstartdiskett.
Kontrollera 'systempartition'-s�kv�g.
.

MessageID=9016 SymbolicName=DIAG_BL_GET_SYSTEM_PATH
Language=English
Parametern 'osloadfilename' pekar inte p� en giltig fil.
.

MessageID=9017 SymbolicName=DIAG_BL_LOAD_SYSTEM_IMAGE
Language=English
<Windows-rot>\system32\ntoskrnl.exe.
.

MessageID=9018 SymbolicName=DIAG_BL_FIND_HAL_IMAGE
Language=English
Parametern 'osloader' pekar inte p� en giltig fil.
.

MessageID=9019 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_X86
Language=English
<Windows-rot>\system32\hal.dll.
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
Inl�sningsfel 1.
.

MessageID=9022 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_DATA
Language=English
Inl�sningsfel 2.
.

MessageID=9023 SymbolicName=DIAG_BL_LOAD_SYSTEM_DLLS
Language=English
inl�sningsprogram beh�ver DLL f�r kernel.
.

MessageID=9024 SymbolicName=DIAG_BL_LOAD_HAL_DLLS
Language=English
inl�sningsprogram beh�ver DLL f�r HAL.
.

MessageID=9026 SymbolicName=DIAG_BL_FIND_SYSTEM_DRIVERS
Language=English
hitta systemdrivrutiner.
.

MessageID=9027 SymbolicName=DIAG_BL_READ_SYSTEM_DRIVERS
Language=English
l�s systemdrivrutiner.
.

MessageID=9028 SymbolicName=DIAG_BL_LOAD_DEVICE_DRIVER
Language=English
l�ste inte in drivrutin f�r systemstartenhet.
.

MessageID=9029 SymbolicName=DIAG_BL_LOAD_SYSTEM_HIVE
Language=English
l�s in konfigurationsfil f�r systemmaskinvara.
.

MessageID=9030 SymbolicName=DIAG_BL_SYSTEM_PART_DEV_NAME
Language=English
hitta namnet p� systempartitionen.
.

MessageID=9031 SymbolicName=DIAG_BL_BOOT_PART_DEV_NAME
Language=English
hitta namnet p� startpartitionen.
.

MessageID=9032 SymbolicName=DIAG_BL_ARC_BOOT_DEV_NAME
Language=English
skapade inte ARC-namn f�r HAL och systems�kv�gar.
.

MessageID=9034 SymbolicName=DIAG_BL_SETUP_FOR_NT
Language=English
Inl�sningsfel 3.
.

MessageID=9035 SymbolicName=DIAG_BL_KERNEL_INIT_XFER
Language=English
<Windows-rot>\system32\ntoskrnl.exe
.

MessageID=9036 SymbolicName=LOAD_SW_INT_ERR_ACT
Language=English
Kontakta din supportperson och rapportera felet.
.

MessageID=9037 SymbolicName=LOAD_SW_FILE_REST_ACT
Language=English
F�rs�k att reparera filen genom att starta installationsprogrammet
f�r Windows med hj�lp av installations-CD:n.
V�lj R vid f�rsta sk�rmbilden f�r reparation.
.

MessageID=9038 SymbolicName=LOAD_SW_FILE_REINST_ACT
Language=English
Installera en kopia av filen ovanf�r p� nytt.
.

MessageID=9039 SymbolicName=LOAD_HW_MEM_ACT
Language=English
Kontrollera i dokumentationen f�r Windows om krav p�
maskinvaruminnet och referensmanualen till maskinvaran f�r
till�ggsinformation.
.

MessageID=9040 SymbolicName=LOAD_HW_DISK_ACT
Language=English
Kontrollera i dokumentationen f�r Windows om konfiguration av
h�rddisken och referensmanualen till maskinvaran f�r
till�ggsinformation.
.

MessageID=9041 SymbolicName=LOAD_HW_GEN_ACT
Language=English
Kontrollera i dokumentationen f�r Windows om konfiguration av
maskinvaran och referensmanualen till maskinvaran f�r
till�ggsinformation.
.

MessageID=9042 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
Kontrollera i dokumentationen f�r Windows om alternativ f�r
konfigurering av ARC och referensmanualen till maskinvaran f�r
till�ggsinformation.
.

MessageID=9043 SymbolicName=BL_LKG_MENU_HEADER
Language=English
     Maskinvaruprofiler/�terskapa konfigurationen

Denna meny l�ter dig v�lja vilken maskinvaruprofil som ska anv�ndas
n�r Windows startas.

Om datorn inte startar korrekt kan du byta till en tidigare
systemkonfiguration, vilket kan l�sa startproblem.
VIKTIGT! �ndringar i systemkonfigurationen som gjorts efter den
senaste lyckade starten kommer att g� f�rlorade.
.

MessageID=9044 SymbolicName=BL_LKG_MENU_TRAILER
Language=English
Anv�nd Uppil och Nedpil om du vill flytta markeringen.
Tryck d�refter p� Retur.
.

MessageID=9045 SymbolicName=BL_LKG_MENU_TRAILER_NO_PROFILES
Language=English
Inga maskinvaruprofiler har definierats. Du kan skapa maskinvaruprofiler
med hj�lp av System i Kontrollpanelen.
.

MessageID=9046 SymbolicName=BL_SWITCH_LKG_TRAILER
Language=English
Tryck p� F om du vill byta till den senaste fungerande konfigurationen.
Tryck p� F3 om du vill avsluta den h�r menyn och starta om datorn.
.

MessageID=9047 SymbolicName=BL_SWITCH_DEFAULT_TRAILER
Language=English
Tryck p� S om du vill byta till standardkonfigurationen.
Tryck p� F3 om du vill avsluta den h�r menyn och starta om datorn.
.

;//
;// The following two messages are used to provide the mnemonic keypress
;// that toggles between the Last Known Good control set and the default
;// control set. (see BL_SWITCH_LKG_TRAILER and BL_SWITCH_DEFAULT_TRAILER)
;//
MessageID=9048 SymbolicName=BL_LKG_SELECT_MNEMONIC
Language=English
F
.

MessageID=9049 SymbolicName=BL_DEFAULT_SELECT_MNEMONIC
Language=English
S
.

MessageID=9050 SymbolicName=BL_LKG_TIMEOUT
Language=English
Tid i sekunder till det markerade valet startas automatiskt: %d
.

MessageID=9051 SymbolicName=BL_LKG_MENU_PROMPT
Language=English

Tryck ned blanksteg nu om du vill se menyn f�r maskinvaruprofiler
och senaste fungerande konfiguration
.

MessageID=9052 SymbolicName=BL_BOOT_DEFAULT_PROMPT
Language=English
Starta med standardmaskinvarukonfigurationen
.

;//
;// The following messages are used during hibernation restoration
;//
MessageID=9053 SymbolicName=HIBER_BEING_RESTARTED
Language=English
Datorn startar om fr�n den senaste platsen.
Tryck ned blanksteg nu om du vill avbryta.
.
MessageID=9054 SymbolicName=HIBER_ERROR
Language=English
Datorn kunde inte startas om fr�n den senaste platsen
.
MessageID=9055 SymbolicName=HIBER_ERROR_NO_MEMORY
Language=English
pga att minnet �r slut.
.
MessageID=9056 SymbolicName=HIBER_ERROR_BAD_IMAGE
Language=English
pga att �terst�llningsfilen �r skadad.
.
MessageID=9057 SymbolicName=HIBER_IMAGE_INCOMPATIBLE
Language=English
pga att �terst�llningsfilen inte �r kompatibel med den nuvarande
konfigurationen.
.
MessageID=9058 SymbolicName=HIBER_ERROR_OUT_OF_REMAP
Language=English
pga ett internt fel.
.
MessageID=9059 SymbolicName=HIBER_INTERNAL_ERROR
Language=English
pga ett internt fel.
.
MessageID=9060 SymbolicName=HIBER_READ_ERROR
Language=English
pga ett l�sfel.
.
MessageID=9061 SymbolicName=HIBER_PAUSE
Language=English
Omstarten har pausats:
.
MessageID=9062 SymbolicName=HIBER_CANCEL
Language=English
Ta bort �terst�llningsdata och forts�tt till startmenyn
.
MessageID=9063 SymbolicName=HIBER_CONTINUE
Language=English
Fors�tt omstarten
.
MessageID=9064 SymbolicName=HIBER_RESTART_AGAIN
Language=English
Det senaste f�rs�ket att starta fr�n den f�rra platsen misslyckades.
Vill du f�rs�ka igen?
.
MessageID=9065 SymbolicName=HIBER_DEBUG_BREAK_ON_WAKE
Language=English
Forts�tt med fels�kningsbrytpunkt p� systemstarten
.
MessageID=9066 SymbolicName=LOAD_SW_MISMATCHED_KERNEL
Language=English
Windows kunde inte startas eftersom angiven kernel inte finns
eller inte �r kompatibel med den aktuella processorn.
.
MessageID=9067 SymbolicName=BL_MSG_STARTING_WINDOWS
Language=English
Windows startas...
.
MessageID=9068 SymbolicName=BL_MSG_RESUMING_WINDOWS
Language=English
Windows forts�tter...
.

MessageID=9069 SymbolicName=BL_EFI_OPTION_FAILURE
Language=English
Windows kunde inte startas eftersom det inte gick att l�sa
startinst�llningar fr�n NVRAM.

Kontrollera inst�llningarna i den inbyggda programvaran. Kanske
m�ste du �terst�lla NVRAM-inst�llningarna fr�n en s�kerhetskopia.
.

;
; //
; // Following messages are for the x86-specific
; // boot menu.
; //
;
MessageID=10001 SymbolicName=BL_ENABLED_KD_TITLE
Language=English
 [fels�kning aktiverat]
.

MessageID=10002 SymbolicName=BL_DEFAULT_TITLE
Language=English
Windows (standard)
.

MessageID=10003 SymbolicName=BL_MISSING_BOOT_INI
Language=English
NTLDR: Det gick inte att hitta filen BOOT.INI.
.

MessageID=10004 SymbolicName=BL_MISSING_OS_SECTION
Language=English
NTLDR: Avsnittet [operating systems] saknas i BOOT.TXT.
.

MessageID=10005 SymbolicName=BL_BOOTING_DEFAULT
Language=English
Startar standardkernel fr�n %s.
.

MessageID=10006 SymbolicName=BL_SELECT_OS
Language=English
V�lj vilket operativsystem som ska startas:
.

MessageID=10007 SymbolicName=BL_MOVE_HIGHLIGHT
Language=English


Anv�nd upp- och nedpilarna om du vill flytta markeringen.
Tryck d�refter p� Retur.

.

MessageID=10008 SymbolicName=BL_TIMEOUT_COUNTDOWN
Language=English
Tid i sekunder till det markerade valet startas automatiskt:
.

MessageID=10009 SymbolicName=BL_INVALID_BOOT_INI
Language=English
Felaktig BOOT.INI-fil
Startar fr�n %s
.

MessageID=10010 SymbolicName=BL_REBOOT_IO_ERROR
Language=English
I/O-fel vid anv�ndning av startsektorfilen %s\BOOTSECT.DOS
.

MessageID=10011 SymbolicName=BL_DRIVE_ERROR
Language=English
NTLDR: Det gick inte att �ppna enheten %s
.

MessageID=10012 SymbolicName=BL_READ_ERROR
Language=English
NTLDR: Allvarligt fel %d vid l�sning av BOOT.INI
.

MessageID=10013 SymbolicName=BL_NTDETECT_MSG
Language=English

NTDETECT V5.0 kontrollerar maskinvaran...

.

MessageID=10014 SymbolicName=BL_NTDETECT_FAILURE
Language=English
NTDETECT misslyckades
.

MessageID=10015 SymbolicName=BL_DEBUG_SELECT_OS
Language=English
Nuvarande val:
  Titel.....: %s
  S�kv�g....: %s
  Alternativ: %s
.

MessageID=10016 SymbolicName=BL_DEBUG_NEW_OPTIONS
Language=English
Ange nya startaltenativ:
.

MessageID=10017 SymbolicName=BL_HEADLESS_REDIRECT_TITLE
Language=English
 [Tj�nster f�r n�dadministration �r aktiverat]
.

MessageID=10018 SymbolicName=BL_INVALID_BOOT_INI_FATAL
Language=English
Invalid BOOT.INI file
.

;
; //
; // Following messages are for the advanced boot menu
; //
;

MessageID=11001 SymbolicName=BL_ADVANCEDBOOT_TITLE
Language=English
Avancerade alternativ f�r Windows
V�lj ett alternativ:
.

MessageID=11002 SymbolicName=BL_SAFEBOOT_OPTION1
Language=English
Fels�kert l�ge
.

MessageID=11003 SymbolicName=BL_SAFEBOOT_OPTION2
Language=English
Fels�kert l�ge med n�tverk
.

MessageID=11004 SymbolicName=BL_SAFEBOOT_OPTION3
Language=English
Bekr�fta varje rad
.

MessageID=11005 SymbolicName=BL_SAFEBOOT_OPTION4
Language=English
Fels�kert l�ge med kommandotolk
.

MessageID=11006 SymbolicName=BL_SAFEBOOT_OPTION5
Language=English
VGA-l�ge
.

MessageID=11007 SymbolicName=BL_SAFEBOOT_OPTION6
Language=English
�terst�llning av katalogtj�nst (endast Window-dom�nkontrollanter)
.

MessageID=11008 SymbolicName=BL_LASTKNOWNGOOD_OPTION
Language=English
Senast fungerande konfiguration (de senaste inst�llningarna som fungerade)
.

MessageID=11009 SymbolicName=BL_DEBUG_OPTION
Language=English
Fels�kningsl�ge
.

;#if defined(REMOTE_BOOT)
;MessageID=11010 SymbolicName=BL_REMOTEBOOT_OPTION1
;Language=English
;Underh�ll och fels�kning f�r fj�rrstart                  
;.
;
;MessageID=11011 SymbolicName=BL_REMOTEBOOT_OPTION2
;Language=English
;Rensa offline-cacheminnet
;.
;
;MessageID=11012 SymbolicName=BL_REMOTEBOOT_OPTION3
;Language=English
;Ladda endast med filer fr�n servern
;.
;#endif // defined(REMOTE_BOOT)

MessageID=11013 SymbolicName=BL_BOOTLOG
Language=English
Aktivera startloggning
.

MessageID=11014 SymbolicName=BL_ADVANCED_BOOT_MESSAGE
Language=English
Tryck ned F8 om du vill se alternativ f�r fels�kning och avancerad start.
.

MessageID=11015 SymbolicName=BL_BASEVIDEO
Language=English
Aktivera VGA-l�ge
.

MessageID=11016 SymbolicName=BL_DISABLE_SAFEBOOT
Language=English

Tryck p� ESCAPE f�r att inaktivera safeboot och starta systemet normalt.
.

MessageID=11017 SymbolicName=BL_MSG_BOOT_NORMALLY
Language=English
Starta Windows normalt
.
MessageID=11018 SymbolicName=BL_MSG_OSCHOICES_MENU
Language=English
G� tillbaka till listan �ver operativsystem
.

MessageID=11019 SymbolicName=BL_MSG_REBOOT
Language=English
Starta om datorn
.

MessageID=11020 SymbolicName=BL_ADVANCEDBOOT_AUTOLKG_TITLE
Language=English
Windows kunde inte startas. Detta kanske beror p� en �ndring i program-
eller maskinvara.

Om detta beror p� att datorn inte svarade, startade om ov�ntat eller
st�ngdes av automatiskt f�r att skydda dina filer och mappar kan du
v�lja Senast fungerande konfiguration f�r att �terst�lla de inst�llningar
som anv�ndes f�rra g�ngen som datorn kunde startas.

Om ett tidigare startf�rs�k avbr�ts p� grund av ett str�mavbrott eller att
str�mknappen trycktes in eller om du �r os�ker p� varf�r datorn inte kan
startas kan du v�lja Starta Windows normalt.
.

MessageID=11021 SymbolicName=BL_ADVANCEDBOOT_AUTOSAFE_TITLE
Language=English
Windows avslutades inte korrekt. Om det beror p� att datorn inte svarade
kan du f�rs�ka med n�gon konfiguration f�r fels�kert l�ge. V�lj ett
alternativ i listan nedan:
.

MessageID=11022 SymbolicName=BL_ADVANCEDBOOT_TIMEOUT
Language=English
Tid i sekunder tills Windows startar:
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
;Tryck ned RETUR om du vill forts�tta starta Windows.
;St�ng annars av fj�rrstartsdatorn.
;.
;
;MessageID=12003 SymbolicName=BL_WARNFORMAT_CONTINUE
;Language=English
;Forts�tt
;.
;MessageID=12004 SymbolicName=BL_FORCELOGON_HEADER
;Language=English
;      Automatisk formatering
;
;Windows har uppt�ckt en ny h�rddisk i fj�rrstartsdatorn.
;Innan h�rddisken kan anv�ndas m�ste du logga in och
;verifiera att du har tillg�ng till denna h�rddisk.
;
;VARNING! Windows kommer att automatiskt partitionera och 
;formatera om h�rddisken s� att det nya operativsystemet kan 
;anv�ndas. Alla data p� h�rddisken kommer att f�rsvinna om du
;v�ljer att forts�tta. Om du inte vill forts�tta m�ste du st�nga
;av datorn nu och kontakta administrat�ren.
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
Det gick inte att starta Windows p g a ett fel vid start med RAMDISK.
.

MessageID=15003 SymbolicName=BL_RAMDISK_BOOT_FAILURE
Language=English
Det gick inte att �ppna RAMDISK-avbildningen.
.

MessageID=15010 SymbolicName=BL_RAMDISK_DOWNLOAD
Language=English
L�ser in RAMDISK-diskavbildningen...
.

;#endif // _BLDR_MSG_
