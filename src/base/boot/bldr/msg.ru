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
�� 㤠���� �������� Windows ��-�� �訡��
� �ணࠬ���� ���ᯥ祭��.
������ �� �⮬ ��� �� �訡��:
.

MessageID=9002 SymbolicName=LOAD_SW_MISRQD_FILE_CLASS
Language=English
�� 㤠���� �������� Windows, ��᪮��� �� ������
����室��� 䠩�.
��� ����室����� 䠩��:
.

MessageID=9003 SymbolicName=LOAD_SW_BAD_FILE_CLASS
Language=English
�� 㤠���� �������� Windows ��᪮��� �ᯮ�祭
᫥���騩 䠩�:
.

MessageID=9004 SymbolicName=LOAD_SW_MIS_FILE_CLASS
Language=English
�� 㤠���� �������� Windows ��-�� �ᯮ�祭���� ���
���������饣� 䠩��:
.

MessageID=9005 SymbolicName=LOAD_HW_MEM_CLASS
Language=English
�� 㤠���� �������� Windows ��-�� �������� �訡��
����ன�� �����.
.

MessageID=9006 SymbolicName=LOAD_HW_DISK_CLASS
Language=English
�� 㤠���� �������� Windows ��-�� �������� �訡��
����ன�� ��᪠.
.

MessageID=9007 SymbolicName=LOAD_HW_GEN_ERR_CLASS
Language=English
�� 㤠���� �������� Windows ��-�� ���� �������� �訡��
����ன��.
.

MessageID=9008 SymbolicName=LOAD_HW_FW_CFG_CLASS
Language=English
�� 㤠���� �������� Windows ��-�� ᫥���饩 �訡��
����ன�� ����㧪� ����㤮����� ARC:
.

MessageID=9009 SymbolicName=DIAG_BL_MEMORY_INIT
Language=English
�஢���� �������� ����ன�� ����� � ��騩 ��ꥬ RAM.
.

MessageID=9010 SymbolicName=DIAG_BL_CONFIG_INIT
Language=English
���誮� ����� ����⮢ ᯨ᪠ ����ன��.
.

MessageID=9011 SymbolicName=DIAG_BL_IO_INIT
Language=English
��� ����㯠 � ⠡��栬 ࠧ����� ���⪮�� ��᪠
.

MessageID=9012 SymbolicName=DIAG_BL_FW_GET_BOOT_DEVICE
Language=English
���ࠢ��쭮� ���祭�� ��ࠬ��� 'osloadpartition'.
.

MessageID=9013 SymbolicName=DIAG_BL_OPEN_BOOT_DEVICE
Language=English
�� 㤠���� �믮����� �⥭�� � ��࠭���� ����㧮筮�� ��᪠.
�஢���� 㪠����� ���� � ��ࠢ����� ����㤮����� ��᪠.
.

MessageID=9014 SymbolicName=DIAG_BL_FW_GET_SYSTEM_DEVICE
Language=English
���ࠢ��쭮� ���祭�� ��ࠬ��� 'systempartition'.
.

MessageID=9015 SymbolicName=DIAG_BL_FW_OPEN_SYSTEM_DEVICE
Language=English
�� 㤠���� �믮����� �⥭�� � ��࠭���� ��⥬���� ����㧮筮�� ��᪠.
�஢���� ���� � 'systempartition'.
.

MessageID=9016 SymbolicName=DIAG_BL_GET_SYSTEM_PATH
Language=English
��ࠬ��� 'osloadfilename' �� 㪠�뢠�� �� �ࠢ���� 䠩�.
.

MessageID=9017 SymbolicName=DIAG_BL_LOAD_SYSTEM_IMAGE
Language=English
<Windows root>\system32\ntoskrnl.exe.
.

MessageID=9018 SymbolicName=DIAG_BL_FIND_HAL_IMAGE
Language=English
��ࠬ��� 'osloader' �� 㪠�뢠�� �� �ࠢ���� 䠩�.
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
�訡�� �����稪� 1.
.

MessageID=9022 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_DATA
Language=English
�訡�� �����稪� 2.
.

MessageID=9023 SymbolicName=DIAG_BL_LOAD_SYSTEM_DLLS
Language=English
����室��� ������⥪� DLL ��� ��.
.

MessageID=9024 SymbolicName=DIAG_BL_LOAD_HAL_DLLS
Language=English
����室��� ������⥪� DLL ��� HAL.
.

MessageID=9026 SymbolicName=DIAG_BL_FIND_SYSTEM_DRIVERS
Language=English
���� ��⥬��� �ࠩ��஢.
.

MessageID=9027 SymbolicName=DIAG_BL_READ_SYSTEM_DRIVERS
Language=English
�⥭�� ��⥬��� �ࠩ��஢.
.

MessageID=9028 SymbolicName=DIAG_BL_LOAD_DEVICE_DRIVER
Language=English
�� ����㦥� �ࠩ��� ��⥬���� ����㧮筮�� ���ன�⢠.
.

MessageID=9029 SymbolicName=DIAG_BL_LOAD_SYSTEM_HIVE
Language=English
����㧪� 䠩�� ����ன�� ��⥬���� ����㤮�����.
.

MessageID=9030 SymbolicName=DIAG_BL_SYSTEM_PART_DEV_NAME
Language=English
���� ����� ���ன�⢠ ����� ��⥬���� ࠧ����.
.

MessageID=9031 SymbolicName=DIAG_BL_BOOT_PART_DEV_NAME
Language=English
���� ����� ����㧮筮�� ࠧ����.
.

MessageID=9032 SymbolicName=DIAG_BL_ARC_BOOT_DEV_NAME
Language=English
���ࠢ��쭮 ᣥ���஢��� ARC-��� ��� HAL � ��⥬�� ����.
.

MessageID=9034 SymbolicName=DIAG_BL_SETUP_FOR_NT
Language=English
�訡�� �����稪� 3.
.

MessageID=9035 SymbolicName=DIAG_BL_KERNEL_INIT_XFER
Language=English
<Windows root>\system32\ntoskrnl.exe
.

MessageID=9036 SymbolicName=LOAD_SW_INT_ERR_ACT
Language=English
������ �� �⮩ �訡�� � �㦡� �����প�.
.

MessageID=9037 SymbolicName=LOAD_SW_FILE_REST_ACT
Language=English
����� ���஡����� ����⠭����� ��� 䠩�, �����⨢ �ணࠬ��
��⠭���� Windows � �ਣ����쭮�� ��⠭���筮�� CD-ROM.
�롥�� 'r' � ��ࢮ� ���������� �࠭� ��� ����᪠
��楤��� ����⠭�������.
.

MessageID=9038 SymbolicName=LOAD_SW_FILE_REINST_ACT
Language=English
��⠭���� ������ ����� 㪠������� ��� 䠩��.
.

MessageID=9039 SymbolicName=LOAD_HW_MEM_ACT
Language=English
��� ����祭�� �������⥫쭮� ���ଠ樨 � �ॡ������� � ����㤮�����
�� ������ ����� ����� ���㬥���� �� Windows
� ���㬥���� �� ����饬��� ����㤮�����.
.

MessageID=9040 SymbolicName=LOAD_HW_DISK_ACT
Language=English
��� ����祭�� �������⥫쭮� ���ଠ樨 � �ॡ������� � ����㤮�����
�� ����ன�� ���⪮�� ��᪠ ����� ���㬥���� �� Windows
� ���㬥���� �� ����饬��� ����㤮�����.
.

MessageID=9041 SymbolicName=LOAD_HW_GEN_ACT
Language=English
��� ����祭�� �������⥫쭮� ���ଠ樨 � �ॡ������� � ����㤮�����
�� ��� ����ன�� ����� ���㬥���� �� Windows
� ���㬥���� �� ����饬��� ����㤮�����.
.

MessageID=9042 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
��� ����祭�� �������⥫쭮� ���ଠ樨 � �ॡ������� � ����㤮�����
�� ��ࠬ��ࠬ ����ன�� ARC ����� ���㬥���� �� Windows
� ���㬥���� �� ����饬��� ����㤮�����.
.

MessageID=9043 SymbolicName=BL_LKG_MENU_HEADER
Language=English
     ���� �롮� ���䨣��樨 ����㤮�����

������ ���� �������� ����� ���䨣���� ����㤮�����,
����� �㤥� �ᯮ�짮������ �� ����᪥ Windows.

�᫨ ��⥬� �� ����᪠����, � ����� ��४������� �� �ᯮ�짮�����
�।��饩 ���䨣��樨 ��⥬�, � ⥬ ᠬ� ����� �஡���� ����᪠.
��������: ��������� ���䨣��樨 ��⥬�, ���ᥭ�� ��᫥ ��᫥�����
㤠筮�� ����᪠, ���� ������.
.

MessageID=9044 SymbolicName=BL_LKG_MENU_TRAILER
Language=English
�ᯮ���� ������ � ��५��� ��� ��६�饭�� �뤥������ ��ப�
� �롮� �㦭��� �����, � ��⥬ ������ ������� ����.
.

MessageID=9045 SymbolicName=BL_LKG_MENU_TRAILER_NO_PROFILES
Language=English
� �����饥 �६� �� ��।����� �� ����� ���䨣��樨 ����㤮�����.
���䨣��樨 ����㤮����� ����� ᮧ���� � ������� �ਫ������ ���⥬�,
�ᯮ�����饣��� � ���� ������ �ࠢ�����.
.

MessageID=9046 SymbolicName=BL_SWITCH_LKG_TRAILER
Language=English
��� ��४��祭�� �� ��᫥���� 㤠��� ���䨣���� ������ ������� 'L'.
��� ��室� �� �⮣� ���� � ��१���㧪� �������� ������ ������� F3.
.

MessageID=9047 SymbolicName=BL_SWITCH_DEFAULT_TRAILER
Language=English
��� ��४��祭�� �� ���䨣���� '�� 㬮�砭��' ������ ������� 'D'.
��� ��室� �� �⮣� ���� � ��१���㧪� �������� ������ ������� F3.
.

;//
;// The following two messages are used to provide the mnemonic keypress
;// that toggles between the Last Known Good control set and the default
;// control set. (see BL_SWITCH_LKG_TRAILER and BL_SWITCH_DEFAULT_TRAILER)
;//
MessageID=9048 SymbolicName=BL_LKG_SELECT_MNEMONIC
Language=English
L
.

MessageID=9049 SymbolicName=BL_DEFAULT_SELECT_MNEMONIC
Language=English
D
.

MessageID=9050 SymbolicName=BL_LKG_TIMEOUT
Language=English
�뤥������ ���䨣���� �㤥� ��⮬���᪨ ����饭� �१: %d ᥪ.
.

MessageID=9051 SymbolicName=BL_LKG_MENU_PROMPT
Language=English

������ ������ ��� �맮�� ���� �롮� ���䨣��樨 ����㤮����� ���
��� �ᯮ�짮����� ��᫥���� 㤠筮� ���䨣��樨 ����㤮�����
.

MessageID=9052 SymbolicName=BL_BOOT_DEFAULT_PROMPT
Language=English
����㧪� ���䨣��樨 ����㤮�����, �ᯮ��㥬�� �� 㬮�砭��.
.

;//
;// The following messages are used during hibernation restoration
;//
MessageID=9053 SymbolicName=HIBER_BEING_RESTARTED
Language=English
���⥬� �㤥� ��१���饭� �� �०���� ���筨��.
������ ������� '�஡��' ��� ���뢠���.
.
MessageID=9054 SymbolicName=HIBER_ERROR
Language=English
�� 㤠���� ��१������� ��⥬� �� �०���� ���筨��
.
MessageID=9055 SymbolicName=HIBER_ERROR_NO_MEMORY
Language=English
��-�� ��墠⪨ �����.
.
MessageID=9056 SymbolicName=HIBER_ERROR_BAD_IMAGE
Language=English
��᪮��� ��ࠧ ����⠭������� ���०���.
.
MessageID=9057 SymbolicName=HIBER_IMAGE_INCOMPATIBLE
Language=English
��᪮��� ��ࠧ ����⠭������� �� ᮢ���⨬ � ⥪�饩 ���䨣��樥�.
.
MessageID=9058 SymbolicName=HIBER_ERROR_OUT_OF_REMAP
Language=English
��-�� ����७��� �訡��.
.
MessageID=9059 SymbolicName=HIBER_INTERNAL_ERROR
Language=English
��-�� ����७��� �訡��.
.
MessageID=9060 SymbolicName=HIBER_READ_ERROR
Language=English
��-�� �訡�� �⥭��.
.
MessageID=9061 SymbolicName=HIBER_PAUSE
Language=English
��१���� ��⥬� �ਮ�⠭�����:
.
MessageID=9062 SymbolicName=HIBER_CANCEL
Language=English
������ ����� ����⠭������� � �ᯮ���� ��⥬��� ���� ����㧪�
.
MessageID=9063 SymbolicName=HIBER_CONTINUE
Language=English
�த������� ��१���᪠ ��⥬�
.
MessageID=9064 SymbolicName=HIBER_RESTART_AGAIN
Language=English
�।���� ����⪠ ��१���᪠ ��⥬� �� �०���� ���筨�� �� 㤠����.
���� ������� ������?
.
MessageID=9065 SymbolicName=HIBER_DEBUG_BREAK_ON_WAKE
Language=English
�த������� � �⫠��筮� �窮� ���뢠��� �� �஡㦤���� ��⥬�
.
MessageID=9066 SymbolicName=LOAD_SW_MISMATCHED_KERNEL
Language=English
���������� �������� Windows, ��᪮��� 㪠������ ��
�� ������� ��� �� ᮢ���⨬� � ����騬�� �����஬.
.
MessageID=9067 SymbolicName=BL_MSG_STARTING_WINDOWS
Language=English
����� Windows...
.
MessageID=9068 SymbolicName=BL_MSG_RESUMING_WINDOWS
Language=English
������������� Windows...
.

MessageID=9069 SymbolicName=BL_EFI_OPTION_FAILURE
Language=English
���������� �������� Windows, ��-�� �訡�� �⥭��
��ࠬ��஢ ����㧪� �� NVRAM.

�஢���� ��ࠬ���� ���யணࠬ�. ��������, ���ॡ���� �믮�����
����⠭������� ��ࠬ��஢ NVRAM �� ��娢��� �����.
.

;
; //
; // Following messages are for the x86-specific
; // boot menu.
; //
;
MessageID=10001 SymbolicName=BL_ENABLED_KD_TITLE
Language=English
 [� �⫠�稪��]
.

MessageID=10002 SymbolicName=BL_DEFAULT_TITLE
Language=English
Windows (�� 㬮�砭��)
.

MessageID=10003 SymbolicName=BL_MISSING_BOOT_INI
Language=English
NTLDR: �� ������ 䠩� BOOT.INI
.

MessageID=10004 SymbolicName=BL_MISSING_OS_SECTION
Language=English
NTLDR: ��������� ᥪ�� [operating systems] � 䠩�� BOOT.TXT
.

MessageID=10005 SymbolicName=BL_BOOTING_DEFAULT
Language=English
����㦠���� �ᯮ��㥬�� �� 㬮�砭�� �� � %s.
.

MessageID=10006 SymbolicName=BL_SELECT_OS
Language=English
�롥�� ����樮���� ��⥬� ��� ����᪠:
.

MessageID=10007 SymbolicName=BL_MOVE_HIGHLIGHT
Language=English


�ᯮ���� ������ ��५�� <�����> � <����> ��� �뤥����� �㦭�� ��ப�.
������ ������� <����> ��� ���⢥ত���� �롮�.

.

MessageID=10008 SymbolicName=BL_TIMEOUT_COUNTDOWN
Language=English
�뤥������ ��⥬� �㤥� ��⮬���᪨ ����饭� �१:
.

MessageID=10009 SymbolicName=BL_INVALID_BOOT_INI
Language=English
���ࠢ���� 䠩� BOOT.INI
����㧪� � %s
.

MessageID=10010 SymbolicName=BL_REBOOT_IO_ERROR
Language=English
�訡�� �����/�뢮�� - 䠩� ����㧮筮�� ᥪ��
%s\BOOTSECT.DOS
(I/O Error accessing boot sector file)
.

MessageID=10011 SymbolicName=BL_DRIVE_ERROR
Language=English
NTLDR: �� 㤠���� ������ ��� %s
.

MessageID=10012 SymbolicName=BL_READ_ERROR
Language=English
NTLDR: ����᪠� �訡�� %d �� �⥭�� BOOT.INI
.

MessageID=10013 SymbolicName=BL_NTDETECT_MSG
Language=English

NTDETECT V5.0 - �஢�ઠ ����㤮�����...

.

MessageID=10014 SymbolicName=BL_NTDETECT_FAILURE
Language=English
���� NTDETECT
.

MessageID=10015 SymbolicName=BL_DEBUG_SELECT_OS
Language=English
����騩 �롮�:
  ��������.: %s
  ����.....: %s
  ��ࠬ����: %s
.

MessageID=10016 SymbolicName=BL_DEBUG_NEW_OPTIONS
Language=English
������ ���� ��ࠬ���� ����㧪�:
.

MessageID=10017 SymbolicName=BL_HEADLESS_REDIRECT_TITLE
Language=English
 [����祭 ०�� EMS]
.

MessageID=10018 SymbolicName=BL_INVALID_BOOT_INI_FATAL
Language=English
�������⨬� 䠩� BOOT.INI
.


;
; //
; // Following messages are for the advanced boot menu
; //
;

MessageID=11001 SymbolicName=BL_ADVANCEDBOOT_TITLE
Language=English
���� �������⥫��� ��ਠ�⮢ ����㧪� Windows
�롥�� ���� �� ᫥����� ���������⥩:
.

MessageID=11002 SymbolicName=BL_SAFEBOOT_OPTION1
Language=English
�������� ०��
.

MessageID=11003 SymbolicName=BL_SAFEBOOT_OPTION2
Language=English
�������� ०�� � ����㧪�� �⥢�� �ࠩ��஢
.

MessageID=11004 SymbolicName=BL_SAFEBOOT_OPTION3
Language=English
����㧪� � ��蠣��� ���⢥ত�����
.

MessageID=11005 SymbolicName=BL_SAFEBOOT_OPTION4
Language=English
�������� ०�� � �����প�� ��������� ��ப�
.

MessageID=11006 SymbolicName=BL_SAFEBOOT_OPTION5
Language=English
����㧪� � ०��� VGA
.

MessageID=11007 SymbolicName=BL_SAFEBOOT_OPTION6
Language=English
����⠭������� �㦡� ��⠫���� (⮫쪮 �� ����஫��� ������ Windows)
.

MessageID=11008 SymbolicName=BL_LASTKNOWNGOOD_OPTION
Language=English
����㧪� ��᫥���� 㤠筮� ���䨣��樨 (� ࠡ��ᯮᮡ�묨 ��ࠬ��ࠬ�)
.

MessageID=11009 SymbolicName=BL_DEBUG_OPTION
Language=English
����� �⫠���
.

;#if defined(REMOTE_BOOT)
;MessageID=11010 SymbolicName=BL_REMOTEBOOT_OPTION1
;Language=English
;�����প� 㤠������ ����㧪� � ���࠭���� ���������
;.
;
;MessageID=11011 SymbolicName=BL_REMOTEBOOT_OPTION2
;Language=English
;��������� ����㧪� � ���⪮� ��⮭������ ���
;.
;
;MessageID=11012 SymbolicName=BL_REMOTEBOOT_OPTION3
;Language=English
;����㧪� � �ᯮ�짮������ ⮫쪮 䠩��� �ࢥ�
;.
;#endif // defined(REMOTE_BOOT)

MessageID=11013 SymbolicName=BL_BOOTLOG
Language=English
������� ��⮪���஢���� ����㧪�
.

MessageID=11014 SymbolicName=BL_ADVANCED_BOOT_MESSAGE
Language=English
��� �롮� �ᮡ�� ��ਠ�⮢ ����㧪� Windows ������ F8.
.

MessageID=11015 SymbolicName=BL_BASEVIDEO
Language=English
������� ०�� VGA
.

MessageID=11016 SymbolicName=BL_DISABLE_SAFEBOOT
Language=English

������ <ESCAPE> ��� �⪫�祭�� ������᭮� � �믮������ ���筮� ����㧪�.
.

MessageID=11017 SymbolicName=BL_MSG_BOOT_NORMALLY
Language=English
���筠� ����㧪� Windows
.
MessageID=11018 SymbolicName=BL_MSG_OSCHOICES_MENU
Language=English
������ � �롮�� ����樮���� ��⥬�
.

MessageID=11019 SymbolicName=BL_MSG_REBOOT
Language=English
��१���㧪�
.

MessageID=11020 SymbolicName=BL_ADVANCEDBOOT_AUTOLKG_TITLE
Language=English
�� 㤠���� �믮����� ����㧪� � ����� ����樮���� ��⥬� Windows.
��������, �� ���� ᫥��⢨�� ��������� � ��ࠬ���� ����ன��
����㤮����� ��� �ணࠬ����� ���ᯥ祭��.

�᫨ �� ���� ᫥��⢨�� ����ᠭ�� ��������, ����������� ��१���㧪�
��� ���਩���� �����襭�� ࠡ���, �롥�� ��᫥���� 㤠��� ���䨣����
��� ������ � ࠡ��ᯮᮡ���� ������ ��ࠬ��஢ ����ன�� ��⥬�.

�᫨ �।���� ����⪠ ����㧪� �뫠 ��ࢠ�� ��-�� �⪫�祭�� 
���ய�⠭��, ��砩���� ������ ������ ��� ��� �⪫�祭�� ��������,
��� �᫨ ��稭� ᡮ� ��� �������⭠, �롥�� ������ ����㧪� Windows.
.

MessageID=11021 SymbolicName=BL_ADVANCEDBOOT_AUTOSAFE_TITLE
Language=English
�� 㤠���� ��ଠ�쭮 �������� ࠡ��� Windows. �᫨ �� �맢��� ����ᠭ���
��� ���਩�� �����襭��� ࠡ��� ��⥬� ��� ����� ������, ����� �믮�����
����⠭�������, ��ࠢ ��᫥���� 㤠��� ���䨣���� � ᫥���饬 ����:
.

MessageID=11022 SymbolicName=BL_ADVANCEDBOOT_TIMEOUT
Language=English
���㭤 �� ����᪠ Windows:
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
;������ ������� ���� ��� �த������� ����㧪� Windows
;��� �몫��� �������� 㤠������ ����㧪�.
;.
;
;MessageID=12003 SymbolicName=BL_WARNFORMAT_CONTINUE
;Language=English
;�த������
;.
;MessageID=12004 SymbolicName=BL_FORCELOGON_HEADER
;Language=English
;          ����ଠ�
;
;�����㦥� ���� ���⪨� ��� �� �������� 㤠������ ����㧪�.
;�०��, 祬 Windows ᬮ��� �ᯮ�짮���� ��� ���, ����室���
;�믮����� �室 � ��⥬� � 㡥������, �� �� ����� �ࠢ�
;����㯠 � �⮬� ����.
;
;��������: Windows ��⮬���᪨ ����ନ��� ࠧ����
;�� �⮬ ��᪥ � �믮���� ��� �ଠ�஢���� ��� ࠡ��� � �����
;����樮���� ��⥬��.
;�᫨ �� �த����� ����㧪�, �� ����� �� �⮬ ��᪥ ���� �����!
;�᫨ �� �� ��� �த������, �몫��� �������� � ������� �
;��⥬���� ������������.
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
�� 㤠���� �������� Windows ��-�� �訡�� �� ����㧪� � RAMDISK.
.

MessageID=15003 SymbolicName=BL_RAMDISK_BOOT_FAILURE
Language=English
�� 㤠���� ������ ��ࠧ RAMDISK.
.

MessageID=15010 SymbolicName=BL_RAMDISK_DOWNLOAD
Language=English
����㧪� ��ࠧ� RAMDISK...
.

;#endif // _BLDR_MSG_

