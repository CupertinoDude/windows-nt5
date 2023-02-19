/*++

Copyright (c) 1992  Microsoft Corporation

Module Name:

    dntext.c

Abstract:

    Translatable text for DOS based NT installation program.

Author:

    Ted Miller (tedm) 30-March-1992

Revision History:

--*/


#include "winnt.h"


//
// Name of sections in inf file.  If these are translated, the section
// names in dosnet.inf must be kept in sync.
//

CHAR DnfDirectories[]       = "Directories";
CHAR DnfFiles[]             = "Files";
CHAR DnfFloppyFiles0[]      = "FloppyFiles.0";
CHAR DnfFloppyFiles1[]      = "FloppyFiles.1";
CHAR DnfFloppyFiles2[]      = "FloppyFiles.2";
CHAR DnfFloppyFiles3[]      = "FloppyFiles.3";
CHAR DnfFloppyFilesX[]      = "FloppyFiles.x";
CHAR DnfSpaceRequirements[] = "DiskSpaceRequirements";
CHAR DnfMiscellaneous[]     = "Miscellaneous";
CHAR DnfRootBootFiles[]     = "RootBootFiles";
CHAR DnfAssemblyDirectories[] = SXS_INF_ASSEMBLY_DIRECTORIES_SECTION_NAME_A;


//
// Names of keys in inf file.  Same caveat for translation.
//

CHAR DnkBootDrive[]     = "BootDrive";      // in [SpaceRequirements]
CHAR DnkNtDrive[]       = "NtDrive";        // in [SpaceRequirements]
CHAR DnkMinimumMemory[] = "MinimumMemory";  // in [Miscellaneous]

CHAR DntMsWindows[]   = "Microsoft Windows";
CHAR DntMsDos[]       = "MS-DOS";
CHAR DntPcDos[]       = "PC-DOS";
CHAR DntOs2[]         = "OS/2";
CHAR DntPreviousOs[]  = "������磜�� ����������� �穫��� ��� C:";

CHAR DntBootIniLine[] = "�����ᩫ���/����៣��� �� Windows XP";

//
// Plain text, status msgs.
//

CHAR DntStandardHeader[]      = "\n �����ᩫ��� �� Windows XP\n����������������";
CHAR DntPersonalHeader[]      = "\n �����ᩫ��� �� Windows XP Personal\n����������������";
CHAR DntWorkstationHeader[]   = "\n �����ᩫ��� �� Windows XP Professional\n����������������";
CHAR DntServerHeader[]        = "\n �����ᩫ��� �� Windows 2002 Server\n����������������";
CHAR DntParsingArgs[]         = "��᢬�� �����⫨�...";
CHAR DntEnterEqualsExit[]     = "ENTER=륦���";
CHAR DntEnterEqualsRetry[]    = "ENTER=����ᢞ��";
CHAR DntEscEqualsSkipFile[]   = "ESC=���ᢜ��� ����妬";
CHAR DntEnterEqualsContinue[] = "ENTER=���⮜��";
CHAR DntPressEnterToExit[]    = "� �����ᩫ��� ��� �夘� ������ �� �����婜�. ��⩫� ENTER ��� ⥦��.";
CHAR DntF3EqualsExit[]        = "F3=륦���";
CHAR DntReadingInf[]          = "��ᚤਫ਼ ��� ����妬 INF %s...";
CHAR DntCopying[]             = "����������: ";
CHAR DntVerifying[]           = "�����㟜���: ";
CHAR DntCheckingDiskSpace[]   = "뢜���� �騦� ��� �婡�...";
CHAR DntConfiguringFloppy[]   = "����樭ਫ਼ ����⫘�...";
CHAR DntWritingData[]         = "������� �����⫨� �������垩��...";
CHAR DntPreparingData[]       = "��������� �����⫨� �������垩��...";
CHAR DntFlushingData[]        = "����ਫ਼ ������� �� �婡�...";
CHAR DntInspectingComputer[]  = "��᢬�� ����������...";
CHAR DntOpeningInfFile[]      = "ꤦ���� ����妬 INF...";
CHAR DntRemovingFile[]        = "���ᨚ��� ����妬 %s";
CHAR DntXEqualsRemoveFiles[]  = "X=���ᨚ��� ����妬";
CHAR DntXEqualsSkipFile[]     = "X=���ᢜ��� ����妬";

//
// confirmation keystroke for DnsConfirmRemoveNt screen.
// Kepp in sync with DnsConfirmRemoveNt and DntXEqualsRemoveFiles.
//
ULONG DniAccelRemove1 = (ULONG)'x',
      DniAccelRemove2 = (ULONG)'X';

//
// confirmation keystroke for DnsSureSkipFile screen.
// Kepp in sync with DnsSureSkipFile and DntXEqualsSkipFile.
//
ULONG DniAccelSkip1 = (ULONG)'x',
      DniAccelSkip2 = (ULONG)'X';

CHAR DntEmptyString[] = "";

//
// Usage text.
//

PCHAR DntUsage[] = {

    "��������� �� Windows 2002 Server � �� Windows XP Professional.",
    "",
    "",
    "WINNT [/s[:�������� ���⢜����]] [/t[:���ᛘ]]",
    "	   [/u[:����� ��桨����]] [/udf:id[,UDF_�����]]",
    "	   [/r:�ᡜ���] [/r[x]:�ᡜ���] [/e:������] [/a]",
    "",
    "",
    "/s[:�������� ���⢜����]",
    "   �����坜� �� �⩞ ���⢜���� �� ������ �� Windows.",
    "   � �⩞ ��⧜� �� �夘� ��� ��㨞� �������� �� �礫��� x:\\[��������] � ",
    "	\\\\����������\\����殨����[\\��������].",
    "",
    "/t[:���ᛘ]",
    "   ��������� �� ��暨���� �����ᩫ���� ��� �����⫞�� �� ����ਠ�� ������ ���� ��������⤞",
    "   ���ᛘ ��� �����ᩫ��� �� Windows XP �� ���� �� ���ᛘ. �� ��� ���������� ��� �⩞",
    "   �� ��暨���� �����ᩫ���� �� �������㩜� ��� ��������� ���� ���ᛘ�.",
    "",
    "/u[:����� ��桨����]",
    "   ������� �� ��暨���� �����ᩫ���� ��� ��������矞�� �� ��㩞 ��� ����妬 ��桨����",
    "   (������嫘� /s). �� ����� ��桨���� ���⮜� �����㩜�� �� �᧦��� � 梜� ��� ���㩜��",
    "   �� ���圪 ����ᢢ���������� ���� ������ ��㩫� ���� �� ��ᨡ��� ��� �����ᩫ����.",
    "",
    "/udf:id[,UDF_�����]	",
    "   ��������眠 ⤘ �����ਠ����� (id) �� ���� ������������ �� ��暨����",
    "   �����ᩫ���� ��� �� �����婜� �� ⤘ ����� UDF (Uniqueness Database",
    "   File) ���������� ⤘ ����� ��桨���� (��嫜 /u). � ���ᣜ���� /udf",
    "	����������� ��� ���� ��� ����� ��桨���� ��� �� �����ਠ�����",
    "   �����坜� ���� ���� ��� ����妬 UDF �����������礫��. �� ���",
    "   ���������� ����� UDF, �� ��暨���� �����ᩫ���� �� ���� �� ",
    "   �������㩜�� ��� ����⫘ � ���� �� ����⮜� �� ����� $Unique$.udb.",   
    "",
    "/r[:�ᡜ���]",
    "	�����坜� ⤘ ����������� �ᡜ�� ���� �����ᩫ���. �",
    "	�ᡜ��� �����⤜� ���� �� �⢦� ��� �����ᣣ���� �����ᩫ����.",
    "",
    "/rx[:�ᡜ���]",
    "	�����坜� ⤘ ����������� �ᡜ�� ���� �����ᩫ���. � �ᡜ���",
    "	�������嫘� ���� �� �⢦� ��� �����ᣣ���� �����ᩫ����.",
    "",
    "/e �����坜� ��� ������ ���� ���⢜�� ���� �� ������� ��㣘 ��� ",
    "   �����ᣣ���� �����ᩫ����.",
    "",
    "/a	����������� ��� ������� ��� ᫦�� �� ������ ��ᚡ��.",
     NULL
};

//
//  Inform that /D is no longer supported
//
PCHAR DntUsageNoSlashD[] = {

    "��������� �� Windows XP.",
    "",
    "WINNT [/S[:]�������㧨�⢜����] [/T[:]���ᛘ] [/I[:]�����INF]",
    "      [/O[X]] [/X | [/F] [/C]] [/B] [/U[:����妛⩣����������]]",
    "      [/R[X]:���ᢦ���] [/E:������] [/A]",
    "",
    "/D[:]winntroot",
    "       ���� � ������� ��� �������坜��� ���.",
    NULL
};

//
// out of memory screen
//

SCREEN
DnsOutOfMemory = { 4,6,
                   { "� ��㣞 ��� ������� ��� �� ��暨���� �����ᩫ���� ��� �夘� ������ �� �����婜�.",
                     NULL
                   }
                 };

//
// Let user pick the accessibility utilities to install
//

SCREEN
DnsAccessibilityOptions = { 3, 5,
{   "������� �� ��������� ���������� ���� �����ᩫ���:",
    DntEmptyString,
    "[ ] ��⩫� F1 ��� �� ����������� ���� ��� Microsoft",
#ifdef NARRATOR
    "[ ] ��⩫� F2 ��� ��� ������� ��� Microsoft",
#endif
#if 0
    "[ ] ��⩫� F3 ��� �� ��������暠� ��椞� ��� Microsoft",
#endif
    NULL
}
};

//
// User did not specify source on cmd line screen
//

SCREEN
DnsNoShareGiven = { 3,5,
{ "�� ��暨���� �����ᩫ���� ����᝜��� �� �⩞ �� ������ �� Windows XP.",
  "�����᚜�� �� �������� ��� �⩞� 槦� ��婡����� �� ����� �� Windows XP.",
  NULL
}
};


//
// User specified a bad source path
//

SCREEN
DnsBadSource = { 3,5,
                 { "� ��������⤞ ���⢜��� ��� �夘� ⚡���, ��� �夘� �������᩠�� � ��� ����⮜�",
                   "��� ⚡��� �����ᩫ��� �� Windows XP. �����᚜�� ��� �� �������� �⩞� ���� ����",
                   "��⧜� �� ��婡����� �� ����� �� Windows XP. ����������㩫� �� ��㡫�� BACKSPACE",
                   "��� �� �����ᯜ�� ���� ������㨜� ��� ����������㩫� ��� ���⮜�� �� ��������.",
                   NULL
                 }
               };


//
// Inf file can't be read, or an error occured parsing it.
//

SCREEN
DnsBadInf = { 3,5,
              { "_��� 㫘� ������ � ��ᚤਫ਼ ��� �� ��暨���� �����ᩫ���� ��� ����妬 ���������� (INF)",
                "� �� ����� ⮜� �����������. ��������㩫� �� �� ����������� ��� ����㣘�� ���.",
                NULL
              }
            };

//
// The specified local source drive is invalid.
//
// Remember that the first %u will expand to 2 or 3 characters and
// the second one will expand to 8 or 9 characters!
//
SCREEN
DnsBadLocalSrcDrive = { 3,4,
{ "� ��������⤞ ���ᛘ ��� �� ����ਠ�� ����� ��� �����ᩫ���� ��� �夘�",
  "��� ⚡��� ���ᛘ � ��� ����⮜� ����ᮠ���� %u �� (%lu byte)",
  "����⩠��� �騦�.",
  NULL
}
};

//
// No drives exist that are suitable for the local source.
//
// Remeber that the %u's will expand!
//
SCREEN
DnsNoLocalSrcDrives = { 3,4,
{  "�� Windows XP ������� ⤘ �棦 ��� ������ �婡� ����ᮠ���� %u ��",
   "(%lu byte) ����⩠��� �騦�. � �����ᩫ��� �� ����������㩜�",
   "�⨦� ��� �騦� ��� ����ਠ�� ����㡜��� �� ������ ���� ��� �����ᩫ���.",
   "� ���ᛘ �婡�� ��⧜� �� ��婡���� �� ⤘ ������ ������ �婡� ������� ",
   "���������棜�� ��� �� Windows XP ��� ��� ��⧜� �� �夘� ��������⤞.",
   DntEmptyString,
   "�� ��暨���� �����ᩫ���� ��� 㫘� ������ �� �����婜� �⫦�� �婡� �",
   "���妪 �� ����⫝̸� ���矜�� ��� ������磜�� �騦.",
  NULL
}
};

SCREEN
DnsNoSpaceOnSyspart = { 3,5,
{ "� ���矜��� �騦� ��� ���ᛘ ��� ���夞��� (����� C:) ��� �������",
  "��� ��������� ��� ����⫘. ��� ��������� ��� ����⫘ �������",
  "����ᮠ���� 3,5 MB (3.641.856 byte) ����⩠��� �騦� �� ���夞 �� ���ᛘ.",
  NULL
}
};

//
// Missing info in inf file
//

SCREEN
DnsBadInfSection = { 3,5,
                     { "� ��櫞�� [%s] ��� ����妬 ���������� ��� �����ᣣ���� �����ᩫ���� ���",
                       "��ᨮ�� � ⮜� �����������. ��������㩫� �� �� ����������� ��� ����㣘�� ���.",
                       NULL
                     }
                   };


//
// Couldn't create directory
//

SCREEN
DnsCantCreateDir = { 3,5,
                     { "� �����ᩫ��� ��� 㫘� ������ �� ��������㩜� ���� ������� �����暦�� ��� ���ᛘ ����������.",
                       DntEmptyString,
                       "%s",
                       DntEmptyString,
                       "��⚥�� �� ���ᛘ ���������� ��� ��������嫜 櫠 ��� ��ᨮ��� ����� �� �� 因� 椦��",
                       "��� �����暦� ����������. ��⚥�� ��婞� �� � �礛��� �� �������",
                       "�夘� �੫�.",
                       NULL
                     }
                   };

//
// Error copying a file
//

SCREEN
DnsCopyError = { 4,5,
{  "� �����ᩫ��� ��� 㫘� ������ �� ������ᯜ� �� ������� �����:",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   " ��⩫� ENTER ��� ����ᢞ�� ��� ��������嘪 ���������.",
   " ��⩫� ESC ��� ���枩� ��� ��ᢣ���� ��� ���⮠�� ��� �����ᩫ����.",
   " ��⩫� F3 ��� ⥦�� ��� �� ��暨���� �����ᩫ����.",
   DntEmptyString,
   "�����ਫ਼: M� ���枩� ��� ��ᢣ���� ��� ���⮠�� ��� �����ᩫ����, ��",
   "��������婜�� ᢢ� �����㣘�� ���� �� ��ᨡ��� ��� �����ᩫ����.",
   NULL
}
},
DnsVerifyError = { 4,5,
{  "�� ���嚨��� ��� ����妬 ��� �����稚��� �� ��暨���� �����ᩫ���� ��� �夘�",
   "因� �� �� ������ �����. �� ���� �� ���墜��� �� ��ᢣ��� ��� ����禬,",
   "�����㣘�� ��� ����⫘� � ᢢ� �����㣘�� ������� �� �� �����.",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   " ��⩫� ENTER ��� ����ᢞ�� ��� ��������嘪 ���������.",
   " ��⩫� ESC ��� ���枩� ��� ��ᢣ���� ��� ���⮠�� ��� �����ᩫ����.",
   " ��⩫� F3 ��� ⥦�� ��� �� ��暨���� �����ᩫ����.",
   DntEmptyString,
   "�����ਫ਼: M� ���枩� ��� ��ᢣ���� ��� ���⮠�� ��� �����ᩫ����, ��",
   "��������婜�� ᢢ� �����㣘�� ���� �� ��ᨡ��� ��� �����ᩫ����.",
   NULL
}
};

SCREEN DnsSureSkipFile = { 4,5,
{  "�� ����㩜�� �� ��ᢣ� ��� �� ������������ ���嚨��� ��� ����妬.",
   "���� � ������� �����坜��� ��� ����ਞ�⤦�� ��㩫�� �� ���妠 ��������",
   "��� ���⧜��� ��� ��� ⢢���� ������ ��� ����㣘���.",
   DntEmptyString,
   " ��⩫� ENTER ��� ����ᢞ�� ��� ��������嘪 ���������.",
   " ��⩫� I ��� ���枩� ��� ����妬.",
   DntEmptyString,
   "�����ਫ਼: M� ���枩� ��� ��ᢣ���� ��� ���⮠�� ��� �����ᩫ����, ��",
   "�� ��� �夜� � �����ᩫ��� � ����៣��� �� Windows XP �� �������.",
  NULL
}
};

//
// Wait while setup cleans up previous local source trees.
//

SCREEN
DnsWaitCleanup =
    { 12,6,
        { "�����⤜�� 橦 � �����ᩫ��� �������� �� ������磜�� ����ਠ�� �����.",
           NULL
        }
    };

//
// Wait while setup copies files
//

SCREEN
DnsWaitCopying = { 13,6,
                   { "�����⤜�� 橦 � �����ᩫ��� ������᭜� �� ����� ��� �婡�.",
                     NULL
                   }
                 },
DnsWaitCopyFlop= { 13,6,
                   { "�����⤜�� 橦 � �����ᩫ��� ������᭜� �� ����� ��� ����⫘.",
                     NULL
                   }
                 };

//
// Setup boot floppy errors/prompts.
//

SCREEN
DnsNeedFloppyDisk3_0 = { 4,4,
{  "�� ��暨���� �����ᩫ���� ������� ����� ���� ����⫝̸� ����� ����櫞���",
   "����������⤜�. �� �夜��� ������� �� ���� �\"�����ᩫ��� �� Windows XP",
   "����⫘ ���夞���,\" \"�����ᩫ��� �� Windows XP ����⫘ #2,\" \"�����ᩫ��� ��",
   " Windows XP ����⫘ #3\" and \"�����ᩫ��� �� Windows XP ����⫘ #4.\"",
   DntEmptyString,
   "�������㩫� �� ��� ���� ��� �⩩���� ����⫝̸� ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 4\"",
   NULL
}
};

SCREEN
DnsNeedFloppyDisk3_1 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 4\"",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk2_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 3\"",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk1_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 2\"",
   NULL
}
};

SCREEN
DnsNeedFloppyDisk0_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ ���夞���\"",  
   NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_0 = { 4,4,
{  "�� ��暨���� �����ᩫ���� ������� ����� ���� ����⫝̸� ����� ����櫞���",
   "����������⤜�. �� �夜��� ������� �� ���� �\"�����ᩫ��� �� Windows XP",
   "����⫘ ���夞���,\" \"�����ᩫ��� �� Windows XP ����⫘ #2,\" \"�����ᩫ���",
   "�� Windows XP ����⫘ #3\" ��� \"�����ᩫ��� �� Windows XP ����⫘ #4.\"",
   DntEmptyString,
   "�������㩫� �� ��� ���� ��� �⩩���� ����⫝̸� ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 4\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_1 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 4\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk2_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 3\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk1_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ 2\"",
   NULL
}
};

SCREEN
DnsNeedSFloppyDsk0_0 = { 4,4,
{  "�������㩫� ��� ���� ����⫘ ����� ����櫞���, ����������⤞ ��� ���ᛘ A:.",
   "���� � ����⫘ �� �夜� \"�����ᩫ��� �� Windows XP ����⫘ ���夞���\"",
  NULL
}
};

//
// The floppy is not formatted.
//
SCREEN
DnsFloppyNotFormatted = { 3,4,
{ "���� � ����⫘ ��� �夘� ����� ����櫞���, ����������⤞ ��� ��㩞 �� MS-DOS",
  "� �夘� ����������⤞. ��� �夘� ������ � ��㩞 ��� ��� ��� �����ᩫ���.",
  NULL
}
};

//
// We think the floppy is not formatted with a standard format.
//
SCREEN
DnsFloppyBadFormat = { 3,4,
{ "���� � ����⫘ ��� �夘� ����� ����櫞���, ����������⤞ ��� ��㩞 �� MS-DOS",
  "� �夘� ����������⤞. ��� �夘� ������ � ��㩞 ��� ��� ��� �����ᩫ���.",
  NULL
}
};

//
// We can't determine the free space on the floppy.
//
SCREEN
DnsFloppyCantGetSpace = { 3,4,
{ "�� ��暨���� �����ᩫ���� ��� �夘� ������ �� ��������婜� ��� ���矜�� �騦",
  "��� ����⫘ ��� �������㩘��. ��� �夘� ������ � ��㩞 ��� ��� ��� �����ᩫ���.",
  NULL
}
};

//
// The floppy is not blank.
//
SCREEN
DnsFloppyNotBlank = { 3,4,
{ "� ����⫘ ��� �������㩘�� ��� �夘� ����� ����櫞��� � ��� �夘� ����.",
  "��� �夘� ������ � ��㩞 ���� ��� ����⫘� ��� ��� �����ᩫ���.",
  NULL
}
};

//
// Couldn't write the boot sector of the floppy.
//
SCREEN
DnsFloppyWriteBS = { 3,4,
{ "��� 㫘� ������ � ������� ��� ��� �����ᩫ��� ���� ������� ����㣘��� ���",
  "����⫘�. ��� �夘� ������ � ��㩞 ���� ��� ����⫘� ��� ��� �����ᩫ���.",
  NULL
}
};

//
// Verify of boot sector on floppy failed (ie, what we read back is not the
// same as what we wrote out).
//
SCREEN
DnsFloppyVerifyBS = { 3,4,
{ "� ��ᚤਫ਼ ������� ��� ��� �����ᩫ��� ��� ������� ����㣘��� ��� ����⫘�",
  "��� �����᝜� �� ��� ������� ������� � ��� 㫘� ������ � ��ᚤਫ਼ ��� ���",
  "�����ᩫ��� ��� ������� ����㣘��� ��� ����⫘� ��� ��� ����㟜��� ����.",
  DntEmptyString,
  "���� ���墜��� �� �� � ������櫜��� ��� ��� ������� ����㡜�:",
  DntEmptyString,
  "  � ���������� ⮜� ���������� ��� ⤘ ��.",
  "  � ���������⤞ ����⫘ �夘� ����������⤞.",
  "  � ���ᛘ ����⫘� ⮜� ⤘ ��晢��� ������ � �矣���� �����⫨�.",
  NULL
}
};


//
// We couldn't write to the floppy drive to create winnt.sif.
//

SCREEN
DnsCantWriteFloppy = { 3,5,
{ "��� 㫘� ������ � ������� ��� ����⫘ ��� ���ᛘ A:. �� � ����⫘",
  "�� �夘� ����������⤞. �����ᩫ� ��� ����������� ����⫘.",
  NULL
}
};


//
// Exit confirmation dialog
//

SCREEN
DnsExitDialog = { 13,6,
                  { "����������������������������������������������������ͻ",
                    "�  �� Windows XP ��� ⮦�� ������������ ����       �",
                    "�  ��� �穫��� ���. �� ���婜�� �� ��暨����         �",
                    "�  �����ᩫ���� �騘 �� ��⧜� �� ���������� �ᢠ    �",
                    "�  ��� �� ��������㩜�� �� Windows XP.               �",
                    "�     * ���㩫� ENTER �� �����婜�� ��� �����ᩫ���. �",
                    "�     * ���㩫� F3 �� ���婜�� ��� �����ᩫ���.      �",
                    "����������������������������������������������������Ķ",
                    "�  F3=륦���  ENTER=���⮜��                         �",
                    "����������������������������������������������������ͼ",
                    NULL
                  }
                };


//
// About to reboot machine and continue setup
//

SCREEN
DnsAboutToRebootW =
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �� �����婜� ���� ��� �������夞��.",
  DntEmptyString,
  "��������嫜 櫠 � ����⫘ ��� \"�����ᩫ��� �� Windows XP", 
  "����⫘ ���夞���\" ��婡���� ��� ���ᛘ A: ������ �����婜��.",
  DntEmptyString,
  "��⩫� ENTER ��� �������夞�� ��� ���������� ��� ���⮠�� ��� �����ᩫ����",
  "�� Windows XP.",
  NULL
}
},

DnsAboutToRebootS =  
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �� �����婜� ���� ��� �������夞��.",
  DntEmptyString,
  "��������嫜 櫠 � ����⫘ ��� \"�����ᩫ��� �� Windows XP", 
  "����⫘ ���夞���\" ��婡���� ��� ���ᛘ A: ������ �����婜��.",
  DntEmptyString,
  "��⩫� ENTER ��� �������夞�� ��� ���������� ��� ���⮠�� ��� �����ᩫ����",
  "�� Windows XP.",
 NULL
}
},
DnsAboutToRebootX =
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �����坜��� ���� ��� �������夞��.",
  DntEmptyString,
  "�� ��ᨮ�� ��� ����⫘ ��� ���ᛘ A:, �����⩫� ��� �騘.",
  DntEmptyString,
  "��⩫� ENTER ��� �������夞�� ��� ���������� ��� ���⮠�� ��� �����ᩫ����",
  "�� Windows XP.",
  NULL
}
};


//
// Need another set for '/w' switch since we can't reboot from within Windows.
//

SCREEN
DnsAboutToExitW =
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �����坜��� ���� ��� �������夞��.",
  DntEmptyString,
  "��������嫜 櫠 � ����⫘ ��� \"�����ᩫ��� �� Windows XP", 
  "����⫘ ���夞���\" ��婡���� ��� ���ᛘ A: ������ �����婜��.",
  DntEmptyString,
  "��⩫� ENTER ��� ��������� �� MS-DOS, ���������㩫� ��� ���������� ���",
  "�����婫� �� ��暨���� �����ᩫ���� �� Windows XP.",
  NULL
}

},
DnsAboutToExitS =
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �����坜��� ���� ��� �������夞��.",
  DntEmptyString,
  "��������嫜 櫠 � ����⫘ ��� \"�����ᩫ��� �� Windows XP", 
  "����⫘ ���夞���\" ��婡���� ��� ���ᛘ A: ������ �����婜��.",
  DntEmptyString,
  "��⩫� ENTER ��� ��������� �� MS-DOS, ���������㩫� ��� ���������� ���",
  "�����婫� �� ��暨���� �����ᩫ���� �� Windows XP.",
  NULL
}
},
DnsAboutToExitX =
{ 3,5,
{ "�� ��㣘 ��� �����ᩫ���� ��� ���坜��� �� MS-DOS �������韞��.",
  "�� ��暨���� �����ᩫ���� �� �����㩜� �ᢠ ��� ���������� ���.",
  "� ��������� �����ᩫ���� �� Windows XP �����坜��� ���� ��� �������夞��.",
  DntEmptyString,
  "�� ��ᨮ�� ��� ����⫘ ��� ���ᛘ A:, �����⩫� ��� �騘.",
  DntEmptyString,
  "��⩫� ENTER ��� ��������� �� MS-DOS, ���������㩫� ��� ���������� ���",
  "�����婫� �� ��暨���� �����ᩫ���� �� Windows XP.",
  NULL
}
};


//
// Gas gauge
//

SCREEN
DnsGauge = { 7,15,
             { "����������������������������������������������������������������ͻ",
               "� �� ��暨���� �����ᩫ���� ������᭜� �����...                 �",
               "�                                                                �",
               "�      ��������������������������������������������������Ŀ      �",
               "�      �                                                  �      �",
               "�      ����������������������������������������������������      �",
               "����������������������������������������������������������������ͼ",
               NULL
             }
           };


//
// Error screens for initial checks on the machine environment
//

SCREEN
DnsBadDosVersion = { 3,5,
{ "���� �� ��暨���� ������� MS-DOS ⡛���� 5.0 � ��櫜���.",
  NULL
}
},

DnsRequiresFloppy = { 3,5,
#ifdef ALLOW_525
{ "�� ��暨���� �����ᩫ���� ���槠�� 櫠 � ���ᛘ A: ��� ��ᨮ�� � �夘�",
  "���ᛘ ������ ����櫞���. ��� ��� ���⢜�� ��� �����ᣣ���� �����ᩫ����",
  "������嫘� ��� ���ᛘ ����⫘� 1,2 �� � ����櫜��.",
#else
{"�� ��暨���� �����ᩫ���� ���槠�� 櫠 � ���ᛘ A: ��� ��ᨮ�� � �夘�",
 "3,5 ����� ����� ����櫞���. ������嫘� ��� ���ᛘ A:  �ਞ���櫞��� 1,44 ��",
 "� ��髜��� ��� ��� �����ᩫ��� ��� ����⫝̸�.",
 DntEmptyString,
"��� ��� �����ᩫ��� �� Windows XP ��� �� ��㩞 �������, �����㩫� �ᢠ ��",
"��暨���� ��� �����婫� ��� ������� /b ��� ������ ������.",
#endif
  NULL
}
},

DnsRequires486 = { 3,5,
{ "�� ��暨���� �����ᩫ���� ���槠�� 櫠 ���� � ���������� ��� ����⫝̸�",
  "����������� 80486 � ��櫜��. ��� �夘� ������ � ���⢜�� �� Windows XP.",
  NULL
}
},

DnsCantRunOnNt = { 3,5,
{ "��� �夘� ������ � ���⢜�� ��� �����ᣣ���� �� ���� ⡛��� 32 bit �� Windows.",
  DntEmptyString,
  "����������㩫� winnt32.exe.",
  NULL
}
},

DnsNotEnoughMemory = { 3,5,
{ "�� ��暨���� �����ᩫ���� ���槠�� 櫠 � ����������⤞ ��㣞 ��� ����������",
  "��� ������� ��� ��� ���⢜�� �� Windows XP.",
  DntEmptyString,
  "��㣞 ��� ������嫘�: %lu%s Mb",
  "��㣞 ��� �����婫���: %lu%s Mb",
  NULL
}
};

//
// Screens used when removing existing nt files
//
SCREEN
DnsConfirmRemoveNt = { 5,5,
{   "�� ��暨���� �����ᩫ���� �� ������㩜� �� ����� �� Windows XP ���",
    "��� �������棜�� ������� ���ᢦ��. � �����ᩫ��� �� Windows XP ��",
    "���� ��� ���ᢦ�� �� ����������� ��������.",
    DntEmptyString,
    "%s",
    DntEmptyString,
    DntEmptyString,
    " ��⩫� F3 ��� ⥦�� ��� �� ��暨���� �����ᩫ���� ��� ���ᨚ��� ������.",
    " ��⩫� � ��� ���ᨚ��� �� ������ �� Windows ��� ��� ������� ���ᢦ��.",
    NULL
}
},

DnsCantOpenLogFile = { 3,5,
{ "��� 㫘� ������ �� �������� �� ������� ����� ��������� ��� ��� �����ᩫ���.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "�� ��暨���� �����ᩫ���� ��� �夘� ������ �� ������㩜� �� ����� ��", 
  "Windows XP ��� ��� ��������⤦ ���ᢦ��.",
  NULL
}
},

DnsLogFileCorrupt = { 3,5,
{ "��� �夘� ������ �稜�� ��� �� ��暨���� �����ᩫ���� ��� ��櫞��� %s",
  "��� �������棜�� ������� ����� ��������� ��� �����ᩫ����.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "�� ��暨���� �����ᩫ���� ��� �夘� ������ �� ������㩜� �� ����� ��", 
  "Windows XP ��� ��� ��������⤦ ���ᢦ��.",
  NULL
}
},

DnsRemovingNtFiles = { 3,5,
{ "           �����⤜�� 橦 �������礫�� �� ����� �� Windows XP.",
  NULL
}
};

SCREEN
DnsNtBootSect = { 3,5,
{ "��� �夘� ������ � �����ᩫ��� ��� �����ᣣ���� ���夞��� �� Windows XP.",
  DntEmptyString,
  "��������嫜 櫠 � ���ᛘ ��� C: �夘� ��������⤞ ��� ��� �夘� ����������⤞.",
 NULL
}
};

SCREEN
DnsOpenReadScript = { 3,5,
{ "��� 㫘� ������ � ��橙��� ��� ����� �⩣�� �������� ��� �����婫��� �� ��",
  "����槫� /u ��� ������ ������.",
  DntEmptyString,
  "� ��������� ��� ��������矞�� ��� �夘� ������ �� ����������.",
  NULL
}
};

SCREEN
DnsParseScriptFile = { 3,5,
{ "�� ����� �⩣�� �������� ��� �����婫��� �� �� ����槫� /u ��� ������",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "������, ����⮜� ⤘ ��ᢣ� �礫���� ��� ����� %u.",
  DntEmptyString,
  NULL
}
};

SCREEN
DnsBootMsgsTooLarge = { 3,5,
{ "�������ᩫ��� ⤘ ��૜���� ��ᢣ� ��� �����ᩫ����.",
  DntEmptyString,
  "�� ������ᩜ�� �� ������� ���夞��� �夘� ���� ���ᢜ�.",
  NULL
}
};

SCREEN
DnsNoSwapDrive = { 3,5,
{ "�������ᩫ��� ⤘ ��૜���� ��ᢣ� ��� �����ᩫ����.",
  DntEmptyString,
  "��� 㫘� ������ � �稜�� �⩞� ��� �� ����� ���������.",
  NULL
}
};

SCREEN
DnsNoSmartdrv = { 3,5,
{ "�� ��暨���� �����ᩫ���� ��� ���槠�� SmartDrive ���� ���������� ���. ��",
  "SmartDrive �� �����驜� ��������� ��� ����橜�� ���� ��� �ᩞ� �����ᩫ����",
  "�� Windows.",
  DntEmptyString,
  "������嫘� ᣜ�� ⥦���, ���夞�� SmartDrive ��� �������夞�� ���",
  "�����ᩫ����. �����⥫� ���� �������ਫ਼ DOS ��� ������櫜��� ��������圪.",
  DntEmptyString,
    "  ��⩫� F3 ��� ⥦�� ��� �� ��暨���� �����ᩫ����.",
    "  ��⩫� ENTER ��� �� �����婜�� ��� �� SmartDrive.",
  NULL
}
};

//
// Boot messages. These go in the fat and fat32 boot sectors.
//
CHAR BootMsgNtldrIsMissing[] = "NTLDR is missing";
CHAR BootMsgDiskError[] = "Disk error";
CHAR BootMsgPressKey[] = "Press any key to restart";
