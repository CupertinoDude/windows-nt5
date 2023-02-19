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
��� 㫘� ������ � ���夞�� �� Windows ��� ��� ��ᢣ���� ��� ���������.
����⨜�� ���� �� ��晢��� � :
.

MessageID=9002 SymbolicName=LOAD_SW_MISRQD_FILE_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ������ ��� ��⟞�� �� �������
�����嫞�� �����:
.

MessageID=9003 SymbolicName=LOAD_SW_BAD_FILE_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ��� ������⤞� ���������
��� ������� ����妬 : 
.

MessageID=9004 SymbolicName=LOAD_SW_MIS_FILE_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ������ �� ������� ����� ��姜� �
�夘� ����������⤦ :
.

MessageID=9005 SymbolicName=LOAD_HW_MEM_CLASS
Language=English
�� Windows ��� 㫘� ������ �� �����㩦�� ��� ��� �����㣘��� �矣����
��� ��㣞� ������.
.

MessageID=9006 SymbolicName=LOAD_HW_DISK_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ��� ��� �����㣘��� �矣����
��� �婡�� ��� ����������.
.

MessageID=9007 SymbolicName=LOAD_HW_GEN_ERR_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ��� ��� ������� �����㣘���
�矣���� ��� ������ ��� ����������.
.

MessageID=9008 SymbolicName=LOAD_HW_FW_CFG_CLASS
Language=English
��� 㫘� ������ � ���夞�� �� Windows ��� ��� ������� �����㣘���
�矣���� ���夞��� ��� ��������������� ARC :
.

MessageID=9009 SymbolicName=DIAG_BL_MEMORY_INIT
Language=English
뢜���� �����⫨� ��� ��㣞� ������ ��� ���櫞�� ��㣞� RAM.
.

MessageID=9010 SymbolicName=DIAG_BL_CONFIG_INIT
Language=English
���������� ������ ������㩜� �����⫨�.
.

MessageID=9011 SymbolicName=DIAG_BL_IO_INIT
Language=English
��� 㫘� ������ � ��橙��� ����� �夘��� ����������� ��� �婡�� ���
.

MessageID=9012 SymbolicName=DIAG_BL_FW_GET_BOOT_DEVICE
Language=English
� �矣��� ��� �����⫨�� 'osloadpartition' ��� �夘� ⚡���.
.

MessageID=9013 SymbolicName=DIAG_BL_OPEN_BOOT_DEVICE
Language=English
��� 㫘� ������ � ��ᚤਫ਼ ��� ��� �������⤞ ���ᛘ �婡�� ���夞���.
��⚥�� �� �������� ���夞��� ��� �� ���ᛘ �婡��.
.

MessageID=9014 SymbolicName=DIAG_BL_FW_GET_SYSTEM_DEVICE
Language=English
� �矣��� ��� �����⫨�� 'systempartition' ��� �夘� ⚡���.
.

MessageID=9015 SymbolicName=DIAG_BL_FW_OPEN_SYSTEM_DEVICE
Language=English
��� 㫘� ������ � ��ᚤਫ਼ ��� ��� �������⤞ ���ᛘ �婡�� ���夞���.
��⚥�� �� �������� ��� �����⫨�� 'systempartition'.
.

MessageID=9016 SymbolicName=DIAG_BL_GET_SYSTEM_PATH
Language=English
� ���ᣜ���� 'osloadfilename' ��� ���������眠 ⤘ ⚡��� �����.
.

MessageID=9017 SymbolicName=DIAG_BL_LOAD_SYSTEM_IMAGE
Language=English
<Windows root>\system32\ntoskrnl.exe.
.

MessageID=9018 SymbolicName=DIAG_BL_FIND_HAL_IMAGE
Language=English
� ���ᣜ���� 'osloader' ��� ���������眠 ⤘ ⚡��� �����.
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
�������ᩫ��� ��ᢣ� ��� ��暨���� �樫ਫ਼� 1.
.

MessageID=9022 SymbolicName=DIAG_BL_LOAD_HAL_IMAGE_DATA
Language=English
�������ᩫ��� ��ᢣ� ��� ��暨���� �樫ਫ਼� 2.
.

MessageID=9023 SymbolicName=DIAG_BL_LOAD_SYSTEM_DLLS
Language=English
�樫ਫ਼ ���������� DLL ��� ��� ���㤘.
.

MessageID=9024 SymbolicName=DIAG_BL_LOAD_HAL_DLLS
Language=English
�樫ਫ਼ ���������� DLL ��� �� HAL.
.

MessageID=9026 SymbolicName=DIAG_BL_FIND_SYSTEM_DRIVERS
Language=English
�稜�� ���������� ��㚞��� ��� ����㣘���.
.

MessageID=9027 SymbolicName=DIAG_BL_READ_SYSTEM_DRIVERS
Language=English
��ᚤਫ਼ ���������� ��㚞��� ��� ����㣘���.
.

MessageID=9028 SymbolicName=DIAG_BL_LOAD_DEVICE_DRIVER
Language=English
��� �樫ਫ਼ �����ᣣ���� ��㚞��� ������� ���夞��� ����㣘���.
.

MessageID=9029 SymbolicName=DIAG_BL_LOAD_SYSTEM_HIVE
Language=English
�樫ਫ਼ ��� ����妬 �矣���� ��� ������ ��� ����㣘���.
.

MessageID=9030 SymbolicName=DIAG_BL_SYSTEM_PART_DEV_NAME
Language=English
�稜�� ��� ����⨠��� ����㣘��� ��� ��棘��� ��� �������.
.

MessageID=9031 SymbolicName=DIAG_BL_BOOT_PART_DEV_NAME
Language=English
�稜�� ��� ��棘��� ��� ������婣���� ���夞���.
.

MessageID=9032 SymbolicName=DIAG_BL_ARC_BOOT_DEV_NAME
Language=English
��� ��������㟞�� �������� �� ARC 椦�� ��� �� HAL ��� ��� �������� ����㣘���.
.

MessageID=9034 SymbolicName=DIAG_BL_SETUP_FOR_NT
Language=English
�������ᩫ��� ��ᢣ� ��� ��暨���� �樫ਫ਼� 3.
.

MessageID=9035 SymbolicName=DIAG_BL_KERNEL_INIT_XFER
Language=English
<Windows root>\system32\ntoskrnl.exe
.

MessageID=9036 SymbolicName=LOAD_SW_INT_ERR_ACT
Language=English
��������㩫� �� ���� ��� ��� ���⮜� �����㨠��, ����⨦��� ��� �� ��晢���.
.

MessageID=9037 SymbolicName=LOAD_SW_FILE_REST_ACT
Language=English
�����嫜 �� �������㩜�� �� ��������驜�� �� ����� �����餫�� ��� �����ᩫ���
�� Windows ����������餫�� �� ������ CD-ROM ��� �����ᩫ����.
����⥫� 'r' ���� ��髞 ��椞 ��� �� �����㩜�� ��� �����樟ਫ਼.
.

MessageID=9038 SymbolicName=LOAD_SW_FILE_REINST_ACT
Language=English
��������㩫� �ᢠ ⤘ ���嚨��� ��� ������� ����妬.
.

MessageID=9039 SymbolicName=LOAD_HW_MEM_ACT
Language=English
��⚥�� ��� �������ਫ਼ �� Windows ��� ��� �����㩜�� ��㣞�
��� ������ ��� �� ������因� ������� ��� ������ ���
��� ������櫜��� ��������圪.
.

MessageID=9040 SymbolicName=LOAD_HW_DISK_ACT
Language=English
��⚥�� ��� �������ਫ਼ �� Windows ��� �� �矣��� ��� �婡��
��� �� ������因� ������� ��� ������ ��� ��� ������櫜���
��������圪.
.

MessageID=9041 SymbolicName=LOAD_HW_GEN_ACT
Language=English
��⚥�� ��� �������ਫ਼ �� Windows ��� �� �矣��� ��� ������
��� �� ������因� ������� ��� ������ ��� ��� ������櫜���
��������圪.
.

MessageID=9042 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
��⚥�� ��� �������ਫ਼ �� Windows ��� ��� ������� �矣����
��� ARC ��� �� ������因� ������� ��� ������ ��� ��� ������櫜���
��������圪.
.

MessageID=9043 SymbolicName=BL_LKG_MENU_HEADER
Language=English
     ����� ������/����� ������ᩫ���� ����婜�

�� ���� �� ����� �����嫜 �� ����⥜�� �� ����� ������
��� �� �����������嫘� 櫘� ������� �� Windows.

�� �� �穫��� ��� ��� ������ �੫�, �����嫜 �� ���ᥜ�� ��� ��⮦��� �矣���
��� ����㣘��� �� �� ������磜��, ��� �� ����穜� �� �����㣘�� ���夞���.
�������: �� ������ �� ����婜� ��� ����㣘��� ��� ⚠��� ���� ��� ��������
��������⤞ ���夞�� , �� ����������.
.

MessageID=9044 SymbolicName=BL_LKG_MENU_TRAILER
Language=English
����������㩫� �� ��� ��� ��� �⢞ ��� �� �������㩜�� ��� ����㣘���
���� ������� ��� �⢜��. ���� ���㩫� ENTER.
.

MessageID=9045 SymbolicName=BL_LKG_MENU_TRAILER_NO_PROFILES
Language=English
��� ⮦�� ������� ����� ������. �� ����� ������ ������ �� ������������
��� ��� �������� �穫��� ��� �夘�� ��⚮��.
.

MessageID=9046 SymbolicName=BL_SWITCH_LKG_TRAILER
Language=English
��� �� ������嫜 ���� �������� �੫� �矣��� , ���㩫� 'L'.
��� �� ���婜�� �� ����� ��� �� �����㩜�� �ᢠ ��� ���������� ���, ���㩫� F3.
.

MessageID=9047 SymbolicName=BL_SWITCH_DEFAULT_TRAILER
Language=English
��� �� ������嫜 ���� ����������⤞ �矣��� , ���㩫� 'D'.
��� �� ���婜�� �� ����� ��� �� �����㩜�� �ᢠ ��� ���������� ���, ���㩫� F3.
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
������梜��� ���� ��� ���棘�� ���夞�� ��� �ૠ��⤞� �������: %d
.

MessageID=9051 SymbolicName=BL_LKG_MENU_PROMPT
Language=English

���㩫� �� ��㡫�� �����㣘��� ���� ��� �� ���⩜�� �� ����� 
����� ������/�������� ��������⤦
.

MessageID=9052 SymbolicName=BL_BOOT_DEFAULT_PROMPT
Language=English
���夞�� �� ������������ ����婜� ������
.

;//
;// The following messages are used during hibernation restoration
;//
MessageID=9053 SymbolicName=HIBER_BEING_RESTARTED
Language=English
�夜��� �������夞�� ��� ����㣘��� ��� ��� ������磜�� �������� ���.
���㩫� �� ��㡫�� �����㣘��� �� �� ����毜��.
.
MessageID=9054 SymbolicName=HIBER_ERROR
Language=English
��� 㫘� ������ � �������夞�� ��� ����㣘��� ��� ��� ������磜�� �������� ���
.
MessageID=9055 SymbolicName=HIBER_ERROR_NO_MEMORY
Language=English
�����嘪 �� ����⩠��� ��㣞�.
.
MessageID=9056 SymbolicName=HIBER_ERROR_BAD_IMAGE
Language=English
��櫠 � ���椘 ��������� �夘� ����������⤞.
.
MessageID=9057 SymbolicName=HIBER_IMAGE_INCOMPATIBLE
Language=English
������ � ���椘 ��������� ��� �夘� ������� �� ��� ��⮦���
�矣���.
.
MessageID=9058 SymbolicName=HIBER_ERROR_OUT_OF_REMAP
Language=English
��� ��� ��૜����� ��ᢣ����.
.
MessageID=9059 SymbolicName=HIBER_INTERNAL_ERROR
Language=English
��� ��� ��૜����� ��ᢣ����.
.
MessageID=9060 SymbolicName=HIBER_READ_ERROR
Language=English
��� ���� ������嘪 ��ᚤਫ਼�.
.
MessageID=9061 SymbolicName=HIBER_PAUSE
Language=English
� �������夞�� ��� ����㣘��� ����槞��:
.
MessageID=9062 SymbolicName=HIBER_CANCEL
Language=English
�������� �� ������� ��������� ��� ���ᤠ�� ��� ����� ���夞���
.
MessageID=9063 SymbolicName=HIBER_CONTINUE
Language=English
���⮜�� �� �������夞�� ��� ����㣘���
.
MessageID=9064 SymbolicName=HIBER_RESTART_AGAIN
Language=English
� �������� �����ៜ�� �������夞��� ��� ����㣘��� ��� ��� ������磜�� 
�������� ��� ��⫬��.  �⢜�� �� �夜� �����ៜ�� �������夞��� �ᢠ;
.
MessageID=9065 SymbolicName=HIBER_DEBUG_BREAK_ON_WAKE
Language=English
���⮜�� ��� ����� ������� ���������� ������� ���� ��������垩�
��� ����㣘���
.
MessageID=9066 SymbolicName=LOAD_SW_MISMATCHED_KERNEL
Language=English
��⫬�� � ���夞�� �� Windows: � ���㤘� ��� ��ᨮ��
� ��� �夘� ������� �� ���� ��� �����������.
.
MessageID=9067 SymbolicName=BL_MSG_STARTING_WINDOWS
Language=English
���夞�� �� Windows...
.
MessageID=9068 SymbolicName=BL_MSG_RESUMING_WINDOWS
Language=English
��������� �� Windows...
.

MessageID=9069 SymbolicName=BL_EFI_OPTION_FAILURE
Language=English
��� 㫘� ������ � ���夞�� �� Windows ������ �������ᩫ��� ��ᢣ�
���� ��� ��ᚤਫ਼ �� ����婜� ���夞��� ��� �� NURAM.

��⚥�� ��� ����婜�� ��� ��������������� ���. �� ��������� ��
������⨜�� ��� ����婜�� NURAM ��� ⤘ ���嚨��� ������嘪.
.

;
; //
; // Following messages are for the x86-specific
; // boot menu.
; //
;
MessageID=10001 SymbolicName=BL_ENABLED_KD_TITLE
Language=English
 [��������垩� debugger]
.

MessageID=10002 SymbolicName=BL_DEFAULT_TITLE
Language=English
Windows (����������)
.

MessageID=10003 SymbolicName=BL_MISSING_BOOT_INI
Language=English
NTLDR: �� ����� BOOT.INI ��� ��⟞��.
.

MessageID=10004 SymbolicName=BL_MISSING_OS_SECTION
Language=English
NTLDR: � ��櫞�� [operating systems] ��� ��⟞�� ��� ����� boot.txt.
.

MessageID=10005 SymbolicName=BL_BOOTING_DEFAULT
Language=English
���夞�� ����������⤦� ���㤘 ��� %s.
.

MessageID=10006 SymbolicName=BL_SELECT_OS
Language=English
����⥫� �� ����������� �穫��� ��� �⢜�� �� �����㩜�:
.

MessageID=10007 SymbolicName=BL_MOVE_HIGHLIGHT
Language=English


���㩫� ���� ��� ��� �⢦� ��� �� �������㩜�� ��� ������� ���.
���㩫� Enter ��� �� ����⥜��.
.

MessageID=10008 SymbolicName=BL_TIMEOUT_COUNTDOWN
Language=English
������梜��� ���� ��� ���棘�� ���夞�� ��� �ૠ��⤞� �������:
.

MessageID=10009 SymbolicName=BL_INVALID_BOOT_INI
Language=English
�� ����� BOOT.INI ��� �夘� ⚡���
���夞�� ��������嘪 ��� %s
.

MessageID=10010 SymbolicName=BL_REBOOT_IO_ERROR
Language=English
��ᢣ� ���曦�/��曦� ���� ��� �����⢘�� ��� ����妬 ��� ���� ���夞���
%s\BOOTSECT.DOS
.

MessageID=10011 SymbolicName=BL_DRIVE_ERROR
Language=English
NTLDR: ��� �夘� ������ �� ᤦ���� ��� ���ᛘ� �婡�� %s
.

MessageID=10012 SymbolicName=BL_READ_ERROR
Language=English
NTLDR: ������樟૦ ��ᢣ� %d ���� ��� ��ᚤਫ਼ ��� BOOT.INI
.

MessageID=10013 SymbolicName=BL_NTDETECT_MSG
Language=English

NTDETECT V5.0 뢜���� ������ ...

.

MessageID=10014 SymbolicName=BL_NTDETECT_FAILURE
Language=English
NTDETECT ��⫬��
.

MessageID=10015 SymbolicName=BL_DEBUG_SELECT_OS
Language=English
��⮦��� �������:
  �嫢��..   : %s
  ��������...: %s
  �������   : %s
.

MessageID=10016 SymbolicName=BL_DEBUG_NEW_OPTIONS
Language=English
�✪ ������� �樫ਫ਼�:
.

MessageID=10017 SymbolicName=BL_HEADLESS_REDIRECT_TITLE
Language=English
 [EMS enabled]
.

MessageID=10018 SymbolicName=BL_INVALID_BOOT_INI_FATAL
Language=English
�� ⚡��� ����� BOOT.INI
.


;
; //
; // Following messages are for the advanced boot menu
; //
;

MessageID=11001 SymbolicName=BL_ADVANCEDBOOT_TITLE
Language=English
����� ������� ��� ����ਞ�⤦�� �� Windows XP
����⥫� ��� �矣���:
.

MessageID=11002 SymbolicName=BL_SAFEBOOT_OPTION1
Language=English
������ ���������
.

MessageID=11003 SymbolicName=BL_SAFEBOOT_OPTION2
Language=English
������ ��������� �� �填��
.

MessageID=11004 SymbolicName=BL_SAFEBOOT_OPTION3
Language=English
��������� ��������ਫ਼� �㣘 ���� �㣘
.

MessageID=11005 SymbolicName=BL_SAFEBOOT_OPTION4
Language=English
������ ��������� �� ������ ������
.

MessageID=11006 SymbolicName=BL_SAFEBOOT_OPTION5
Language=English
���ᩫ��� ��������嘪 VGA
.

MessageID=11007 SymbolicName=BL_SAFEBOOT_OPTION6
Language=English
��������� ��������� �������� �����暦� (�椦 ������� ���� �� Windows)
.

MessageID=11008 SymbolicName=BL_LASTKNOWNGOOD_OPTION
Language=English
�������圪 ��੫� �੫� ����婜�� (�� ��� ��橭���� ��� ���������穘�)
.

MessageID=11009 SymbolicName=BL_DEBUG_OPTION
Language=English
���ᩫ��� ��������嘪 ���������� �������
.

;#if defined(REMOTE_BOOT)
;MessageID=11010 SymbolicName=BL_REMOTEBOOT_OPTION1
;Language=English
;����㨞�� ����������⤞� ���夞��� ��� �������駠�� ���������
;.
;
;MessageID=11011 SymbolicName=BL_REMOTEBOOT_OPTION2
;Language=English
;�����ᨠ�� Cache ��� ��� ��婡���� �� ���������
;.
;
;MessageID=11012 SymbolicName=BL_REMOTEBOOT_OPTION3
;Language=English
;�樫ਫ਼ ����������餫�� ����� �椦 ��� �� ����������
;.
;#endif // defined(REMOTE_BOOT)

MessageID=11013 SymbolicName=BL_BOOTLOG
Language=English
��������垩� ��������� ���夞���
.

MessageID=11014 SymbolicName=BL_ADVANCED_BOOT_MESSAGE
Language=English
��� ��墬�� ��������� ��� ������� ���夞��� ��� ����ਞ�⤦��, ���㩫� F8.
.

MessageID=11015 SymbolicName=BL_BASEVIDEO
Language=English
��������垩� ���ᩫ���� ��������嘪 VGA
.

MessageID=11016 SymbolicName=BL_DISABLE_SAFEBOOT
Language=English

���㩫� ESCAPE ��� ����������垩� ��� ������ ��������嘪 ��� ���夞�� ��������.
.

MessageID=11017 SymbolicName=BL_MSG_BOOT_NORMALLY
Language=English
�������� ���夞�� �� Windows
.
MessageID=11018 SymbolicName=BL_MSG_OSCHOICES_MENU
Language=English
��������� �� ������� ����� OS
.

MessageID=11019 SymbolicName=BL_MSG_REBOOT
Language=English
�������夞��
.

MessageID=11020 SymbolicName=BL_ADVANCEDBOOT_AUTOLKG_TITLE
Language=English
����磘���, ���� �� Windows ��� ���夞��� ��������⤘. ���� �� ���墜���
�� ��� ��橭��� ������ ������ � ����������.

�� � ���������� ��� ����᫞�� �� �����夜���, ���������㟞�� ���橣���
� � ��������� ��� ������婫��� ���棘�� ��� �� ������������ �� ����� ���
�� �ᡜ��� ���, ����⥫� ��� �������圪 ��੫� ��� �੫� ����婜��
��� �� ������⯜�� ���� ��� ��橭���� ����婜�� ��� ���������穘�.

�� ��� ������磜�� �����ៜ�� ���夞��� ����槞�� ��� ������� ��磘���
� �圩�� ��� �������� ���禪 � ��������� � �� ��� �婫� �♘��� ��
����ᢜ�� �� ��晢���, ����⥫� �������� ���夞�� �� Windows.
.

MessageID=11021 SymbolicName=BL_ADVANCEDBOOT_AUTOSAFE_TITLE
Language=English
�� Windows ��� ������婫���� ��������⤘. �� ���� ���墜��� ��� 櫠 ��
�穫��� ��� �����夜��� � �� �� �穫��� ������婫��� ��� �� ��������穜�
�� �����⤘, �� �����嫜 �� �ᤜ�� ������ᩫ��� ����⚦���� �� ��� ���
����婜�� ������� ��������嘪 ��� �� ������� �����:
.

MessageID=11022 SymbolicName=BL_ADVANCEDBOOT_TIMEOUT
Language=English
������梜��� �⮨� ��� ���夞�� �� Windows:
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
;��⩫� ENTER ��� �� ���������� � ���夞�� ��������嘪 �� Windows,
;����������� ���婫� ��� ����������⤞ ��� ������ ���夞���.
;.
;
;MessageID=12003 SymbolicName=BL_WARNFORMAT_CONTINUE
;Language=English
;���⮜��
;.
;MessageID=12004 SymbolicName=BL_FORCELOGON_HEADER
;Language=English
;          ���棘�� ����樭ਫ਼

;�� Windows ���槠��� ⤘ �� ������ �婡� ���� ����������⤞ ��� ������
;���夞���. ���� �� Windows ������ �� ����������㩦�� ���� �� �婡�, ��⧜�
;�� �������嫜 ��� �� ������驜�� 櫠 ⮜�� ��橙��� �� ���� �� �婡�.

;�������:  �� Windows ���棘�� �� �����ᤦ�� ���������� ��� �� 
;�������驦�� ���� �� �婡� 驫� �� ������������ �� �� ����������� �穫���.
; �� �����⤘ ��� ������� �婡�� �� ����� �� ����⥜�� �� �����婜��.
;�� ��� �⢜�� �� �����婜�� ���婫� ��� ���������� �騘 ��� ��������㩫�
;�� �� ����������� ���.
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
�� Windows ��� 㫘� ������ �� ��������� ��� ��ᢣ���� ���� ��� ���夞�� ��� ⤘ RAMDISK.
.

MessageID=15003 SymbolicName=BL_RAMDISK_BOOT_FAILURE
Language=English
�� Windows ��� 㫘� ������ �� ���奦�� �� ��ࢦ ��� RAMDISK.
.

MessageID=15010 SymbolicName=BL_RAMDISK_DOWNLOAD
Language=English
�樫ਫ਼ ��� ���颦� ��� RAMDISK...
.

;#endif // _BLDR_MSG_


