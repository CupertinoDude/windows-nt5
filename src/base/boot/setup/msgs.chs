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
Welcome to Windows Setup

  Press ENTER to continue

     Press F3 to Exit
.

MessageID=9002 SymbolicName=SL_WELCOME_HEADER
Language=English

Windows Setup
=============
.

MessageID=9003 SymbolicName=SL_TOTAL_SETUP_DEATH
Language=English
Setup failed. Press any key to restart your computer.
.

MessageID=9004 SymbolicName=SL_FILE_LOAD_MESSAGE
Language=English
Setup is loading files (%s)...
.

MessageID=9005 SymbolicName=SL_OTHER_SELECTION
Language=English
Other (requires an OEM driver diskette)
.

MessageID=9006 SymbolicName=SL_SELECT_DRIVER_PROMPT
Language=English
ENTER=Select  F3=Exit
.

MessageID=9007 SymbolicName=SL_NEXT_DISK_PROMPT_CANCELLABLE
Language=English
ENTER=Continue  ESC=Cancel  F3=Exit
.

MessageID=9008 SymbolicName=SL_OEM_DISK_PROMPT
Language=English
Manufacturer-supplied hardware support disk
.

MessageID=9009 SymbolicName=SL_MSG_INSERT_DISK
Language=English
Please insert the disk labeled



         into Drive A:

 *  Press ENTER when ready.
.

MessageID=9010 SymbolicName=SL_MSG_EXIT_DIALOG
Language=English
+====================================================+
|  Windows is not completely set up on your          |
|  system. If you quit Setup now, you will need      |
|  to run Setup again to set up Windows.             |
|                                                    |
|     * Press ENTER to continue Setup.               |
|     * Press F3 to quit Setup.                      |
|----------------------------------------------------|
|  F3=Exit  ENTER=Continue                           |
+====================================================+
.

MessageID=9011 SymbolicName=SL_NEXT_DISK_PROMPT
Language=English
ENTER=Continue  F3=Exit
.

MessageID=9012 SymbolicName=SL_NTDETECT_PROMPT
Language=English

Setup is inspecting your computer's hardware configuration...

.

MessageID=9013 SymbolicName=SL_KERNEL_NAME
Language=English
Windows Executive
.

MessageID=9014 SymbolicName=SL_HAL_NAME
Language=English
Hardware Abstraction Layer
.

MessageID=9015 SymbolicName=SL_PAL_NAME
Language=English
Windows Processor Extensions
.

MessageID=9016 SymbolicName=SL_HIVE_NAME
Language=English
Windows Configuration Data
.

MessageID=9017 SymbolicName=SL_NLS_NAME
Language=English
Locale-Specific Data
.

MessageID=9018 SymbolicName=SL_OEM_FONT_NAME
Language=English
Setup Font
.

MessageID=9019 SymbolicName=SL_SETUP_NAME
Language=English
Windows Setup
.

MessageID=9020 SymbolicName=SL_FLOPPY_NAME
Language=English
Floppy Disk Driver
.

MessageID=9021 SymbolicName=SL_KBD_NAME
Language=English
Keyboard Driver
.

MessageID=9121 SymbolicName=SL_FAT_NAME
Language=English
FAT File System
.

MessageID=9022 SymbolicName=SL_SCSIPORT_NAME
Language=English
SCSI Port Driver
.

MessageID=9023 SymbolicName=SL_VIDEO_NAME
Language=English
Video Driver
.

MessageID=9024 SymbolicName=SL_STATUS_REBOOT
Language=English
Press any key to restart your computer.
.

MessageID=9025 SymbolicName=SL_WARNING_ERROR
Language=English
An unexpected error (%d) occurred at
line %d in %s.

Press any key to continue.
.

MessageID=9026 SymbolicName=SL_FLOPPY_NOT_FOUND
Language=English
Only %d floppy drives were found,
the system was trying to find drive %d.
.

MessageID=9027 SymbolicName=SL_NO_MEMORY
Language=English
The system has run out of memory at
line %d in file %s
.

MessageID=9028 SymbolicName=SL_IO_ERROR
Language=English
The system encountered an I/O error
accessing %s.
.

MessageID=9029 SymbolicName=SL_BAD_INF_SECTION
Language=English
Section %s of the INF file is invalid
.

MessageID=9030 SymbolicName=SL_BAD_INF_LINE
Language=English
Line %d of the INF file %s is invalid
.

MessageID=9031 SymbolicName=SL_BAD_INF_FILE
Language=English
INF file %s is corrupt or missing, status %d.
.

MessageID=9032 SymbolicName=SL_FILE_LOAD_FAILED
Language=English
File %s could not be loaded.
The error code is %d
.

MessageID=9033 SymbolicName=SL_INF_ENTRY_MISSING
Language=English
The entry "%s" in the [%s] section
of the INF file is corrupt or missing.
.

MessageID=9034 SymbolicName=SL_PLEASE_WAIT
Language=English
Please wait...
.

MessageID=9035 SymbolicName=SL_CANT_CONTINUE
Language=English
Setup cannot continue. Press any key to exit.
.

MessageID=9036 SymbolicName=SL_PROMPT_SCSI
Language=English
Select the SCSI Adapter you want from the following list, or select "Other"
if you have a device support disk provided by an adapter manufacturer.

.

MessageID=9037 SymbolicName=SL_PROMPT_OEM_SCSI
Language=English
You have chosen to configure a SCSI Adapter for use with Windows,
using a device support disk provided by an adapter manufacturer.

Select the SCSI Adapter you want from the following list, or press ESC
to return to the previous screen.

.
MessageID=9038 SymbolicName=SL_PROMPT_HAL
Language=English
Setup could not determine the type of computer you have, or you have
chosen to manually specify the computer type.

Select the computer type from the following list, or select "Other"
if you have a device support disk provided by your computer manufacturer.

To scroll through the menu items press up arrow or down arrow.

.

MessageID=9039 SymbolicName=SL_PROMPT_OEM_HAL
Language=English
You have chosen to configure a computer for use with Windows,
using a device support disk provided by the computer's manufacturer.

Select the computer type from the following list, or press ESC
to return to the previous screen.

To scroll through the menu items press up arrow or down arrow.

.

MessageID=9040 SymbolicName=SL_PROMPT_VIDEO
Language=English
Setup could not determine the type of video adapter installed in the system.

Select the video Adapter you want from the following list, or select "Other"
if you have a device support disk provided by an adapter manufacturer.

.

MessageID=9041 SymbolicName=SL_PROMPT_OEM_VIDEO
Language=English
You have chosen to configure a video Adapter for use with Windows,
using a device support disk provided by an adapter manufacturer.

Select the video Adapter you want from the following list, or press ESC
to return to the previous screen.

.

MessageID=9042 SymbolicName=SL_WARNING_ERROR_WFILE
Language=English
File %s caused an unexpected error (%d) at
line %d in %s.

Press any key to continue.
.

MessageID=9043 SymbolicName=SL_WARNING_IMG_CORRUPT
Language=English
The file %s is corrupted.

Press any key to continue.
.

MessageID=9044 SymbolicName=SL_WARNING_IOERR
Language=English
An I/O error occurred on file %s.

Press any key to continue.
.

MessageID=9045 SymbolicName=SL_WARNING_NOFILE
Language=English
The file %s could not be found.

Press any key to continue.
.

MessageID=9046 SymbolicName=SL_WARNING_NOMEM
Language=English
Insufficient memory for %s.

Press any key to continue.
.

MessageID=9047 SymbolicName=SL_DRIVE_ERROR
Language=English
SETUPLDR: Couldn't open drive %s
.

MessageID=9048 SymbolicName=SL_NTDETECT_MSG
Language=English

Setup is inspecting your computer's hardware configuration...

.

MessageID=9049 SymbolicName=SL_NTDETECT_FAILURE
Language=English
NTDETECT failed
.

MessageId=9050 SymbolicName=SL_SCRN_TEXTSETUP_EXITED
Language=English
Windows has not been installed.

If there is a floppy disk inserted in drive A:, remove it.

Press ENTER to restart your computer.
.

MessageId=9051 SymbolicName=SL_SCRN_TEXTSETUP_EXITED_ARC
Language=English
Windows has not been installed.

Press ENTER to restart your computer.
.

MessageID=9052 SymbolicName=SL_REBOOT_PROMPT
Language=English
ENTER=Restart Computer
.

MessageID=9053 SymbolicName=SL_WARNING_SIF_NO_DRIVERS
Language=English
Setup could not find any drivers associated with your selection.

Press any key to continue.
.

MessageID=9054 SymbolicName=SL_WARNING_SIF_NO_COMPONENT
Language=English
The disk you have supplied does not provide any relevant support files.

Press any key to continue.
.

MessageID=9055 SymbolicName=SL_WARNING_BAD_FILESYS
Language=English
This disk cannot be read because it contains an unrecognized file system.

Press any key to continue.
.

MessageID=9056 SymbolicName=SL_BAD_UNATTENDED_SCRIPT_FILE
Language=English
The entry

"%s"

in the unattended script file doesn't exist
in the [%s] section of the INF file %s.
.

MessageID=9057 SymbolicName=SL_MSG_PRESS_F5_OR_F6
Language=English
Press F6 if you need to install a third party SCSI or RAID driver...
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
S=Specify Additional Device   ENTER=Continue   F3=Exit
.

MessageID=9062 SymbolicName=SL_SCSI_SELECT_MESSAGE_2
Language=English
  * To specify additional SCSI adapters, CD-ROM drives, or special
    disk controllers for use with Windows, including those for
    which you have a device support disk from a mass storage device
    manufacturer, press S.

  * If you do not have any device support disks from a mass storage
    device manufacturer, or do not want to specify additional
    mass storage devices for use with Windows, press ENTER.
.

MessageID=9063 SymbolicName=SL_SCSI_SELECT_MESSAGE_1
Language=English
Setup could not determine the type of one or more mass storage devices
installed in your system, or you have chosen to manually specify an adapter.
Currently, Setup will load support for the following mass storage devices(s):
.

MessageID=9064 SymbolicName=SL_SCSI_SELECT_MESSAGE_3
Language=English
Setup will load support for the following mass storage device(s):
.

MessageID=9065 SymbolicName=SL_SCSI_SELECT_ERROR
Language=English
Setup was unable to load support for the mass storage device you specified.
Currently, Setup will load support for the following mass storage devices(s):
.

MessageID=9066 SymbolicName=SL_TEXT_ANGLED_NONE
Language=English
<none>
.

MessageID=9067 SymbolicName=SL_TEXT_SCSI_UNNAMED
Language=English
<unnamed adapter>
.

MessageID=9068 SymbolicName=SL_TEXT_OTHER_DRIVER
Language=English
Other
.

MessageID=9069 SymbolicName=SL_TEXT_REQUIRES_486
Language=English
Windows requires an 80486 or later processor.
.

MessageID=9070 SymbolicName=SL_NTPNP_NAME
Language=English
Plug & Play Export Driver
.

MessageID=9071 SymbolicName=SL_PCI_IDE_EXTENSIONS_NAME
Language=English
PCI IDE Extensions Driver
.

MessageID=9072 SymbolicName=SL_HW_FW_CFG_CLASS
Language=English
Windows could not start because of the following ARC
firmware boot configuration problem :
.

MessageID=9073 SymbolicName=DIAG_SL_FW_GET_BOOT_DEVICE
Language=English
The 'osloadpartition' parameter setting is invalid.
.

MessageID=9074 SymbolicName=LOAD_HW_FW_CFG_ACT
Language=English
Please check the Windows documentation about ARC configuration
options and your hardware reference manuals for additional
information.
.
MessageID=9075 SymbolicName=SL_NETADAPTER_NAME
Language=English
Network Adapter Driver
.
MessageID=9076 SymbolicName=SL_TCPIP_NAME
Language=English
TCP/IP Service
.
MessageID=9077 SymbolicName=SL_NETBT_NAME
Language=English
WINS Client(TCP/IP) Service
.
MessageID=9078 SymbolicName=SL_MRXSMB_NAME
Language=English
MRXSMB mini-redirector
.
MessageID=9079 SymbolicName=SL_MUP_NAME
Language=English
UNC Filesystem
.
MessageID=9080 SymbolicName=SL_NDIS_NAME
Language=English
NDIS Driver
.
MessageID=9081 SymbolicName=SL_RDBSS_NAME
Language=English
SMB Redirector Filesystem
.
MessageID=9082 SymbolicName=SL_NETBOOT_CARD_ERROR
Language=English
The network boot card in your computer has an older version
of ROM and cannot be used to remotely install Windows.
Contact your system administrator or computer manufacturer
for information about upgrading the ROM.
.
MessageID=9083 SymbolicName=SL_NETBOOT_SERVER_ERROR
Language=English
The operating system image you selected does not contain the
necessary drivers for your network adapter. Try selecting a
different operating system image. If the problem persists,
contact your system administrator.
.
MessageID=9084 SymbolicName=SL_IPSEC_NAME
Language=English
IP Security Service
.
MessageID=9085 SymbolicName=SL_CMDCONS_MSG
Language=English
Windows Recovery Console
.
MessageID=9086 SymbolicName=SL_KERNEL_TRANSITION
Language=English
Setup is starting Windows
.
;#ifdef _WANT_MACHINE_IDENTIFICATION
MessageID=9087 SymbolicName=SL_BIOSINFO_NAME
Language=English
Machine Identification Data
.
;#endif
MessageID=9088 SymbolicName=SL_KSECDD_NAME
Language=English
Kernel Security Service
.
MessageID=9089 SymbolicName=SL_WRONG_PROM_VERSION
Language=English
Your system's PROM (firmware) is below the required revision level.
Please contact SGI customer support or visit the SGI web site to obtain
the PROM update and upgrade instructions.

Note: you can boot previous installations of Microsoft(R) Windows(R) 
by selecting the appropriate boot entry in the startup menu rather 
than the current default "Windows Setup" boot entry.
.
MessageID=9090 SymbolicName=SIGNATURE_CHANGED
Language=English
Setup either detected multiple disks in your machine that are
indistinguishable or detected raw disk(s). Setup has corrected the problem,
but a reboot is required.
.
MessageID=9091 SymbolicName=SL_KDDLL_NAME
Language=English
Kernel Debugger DLL
.

MessageID=9092 SymbolicName=SL_OEM_DRIVERINFO
Language=English
%s

Windows already has a driver that you can use for
"%s".

Unless the device manufacturer prefers that you use the driver on the
floppy disk, you should use the driver in Windows.
.

MessageID=9093 SymbolicName=SL_CONFIRM_OEMDRIVER
Language=English
S=Use the driver on floppy ENTER=Use the default Windows driver
.

MessageID=9094 SymbolicName=SL_OEMDRIVER_NEW
Language=English
The driver you provided seems to be newer than the Windows
default driver.
.

MessageID=9095 SymbolicName=SL_INBOXDRIVER_NEW
Language=English
The driver you provided seems to be older than the Windows
default driver.
.

MessageID=9096 SymbolicName=SL_CMDCONS_STARTING
Language=English
Starting Windows Recovery Console...
.

MessageID=9097 SymbolicName=SL_NTDETECT_CMDCONS
Language=English
NTDETECT Checking Hardware ...
.

MessageID=9098 SymbolicName=SL_MSG_PRESS_ASR
Language=English
Press F2 to run Automated System Recovery (ASR)...
.

MessageID=9099 SymbolicName=SL_MSG_WARNING_ASR
Language=English
        Please insert the disk labeled:


Windows Automated System Recovery Disk


            into the floppy drive.



          Press any key when ready.
.


MessageID=9100 SymbolicName=SL_TEXT_REQUIRED_FEATURES_MISSING
Language=English
Windows requires certain processor features that are not available
on the processor in this computer.   Specifically, Windows requires
the following instructions.

    CPUID
    CMPXCHG8B
.

MessageID=9101 SymbolicName=SL_MSG_PREPARING_ASR
Language=English
Preparing for Automated System Recovery, press ESC to cancel...
.

MessageID=9102 SymbolicName=SL_MSG_ENTERING_ASR
Language=English
Starting Windows Automated System Recovery...
.

MessageID=9103 SymbolicName=SL_MOUSE_NAME
Language=English
Mouse Driver
.

MessageID=9104 SymbolicName=SL_SETUP_STARTING
Language=English
Starting Windows Setup...
.

MessageID=9105 SymbolicName=SL_MSG_INVALID_ASRPNP_FILE
Language=English
The file ASRPNP.SIF on the Automated System Recovery disk is invalid
.

MessageID=9106 SymbolicName=SL_SETUP_STARTING_WINPE
Language=English
Starting Windows Preinstall Environment...
.

MessageID=9107 SymbolicName=SL_NTDETECT_ROLLBACK
Language=English

Uninstall is checking hardware...

.

MessageID=9108 SymbolicName=SL_ROLLBACK_STARTING
Language=English
Starting Windows Uninstallation...
.


MessageID=9109 SymbolicName=SL_NO_FLOPPY_DRIVE
Language=English
Setup could not find a floppy drive on your machine to load
OEM drivers from floppy disk.

    * Press ESC to cancel loading OEM drivers
    
    * Press F3 to quit setup.
.

MessageID=9110 SymbolicName=SL_UPGRADE_IN_PROGRESS
Language=English
This computer is already in the process of being upgraded to Microsoft 
Windows. What do you want to do ?

    * To continue the current upgrade, press ENTER.

    * To cancel the current upgrade and install new version of
      Microsoft Windows, press F10.

    * To exit Setup without installing Microsoft Windows, 
      press F3.
.

MessageID=9111 SymbolicName=SL_DRVMAINSDB_NAME
Language=English
Driver Identification Data
.

MessageID=9112 SymbolicName=SL_OEM_FILE_LOAD_FAILED
Language=English
Setup failed to load the OEM drivers.

Press any key to continue.
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
>
.

;
; // The character used to draw the background in percent-complete bar
;
;
MessageID=11514 SymbolicName=SL_CMDCONS_PROGBAR_BACK
Language=English
=
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
Windows could not start due to an error while booting from a RAMDISK.
.

MessageID=15003 SymbolicName=BL_RAMDISK_BOOT_FAILURE
Language=English
Windows failed to open the RAMDISK image.
.

MessageID=15010 SymbolicName=BL_RAMDISK_DOWNLOAD
Language=English
Loading RAMDISK image...
.

;#endif // _SETUPLDR_MSG_
