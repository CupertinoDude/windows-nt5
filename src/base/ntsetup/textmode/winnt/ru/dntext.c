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
CHAR DntPreviousOs[]  = "Previous Operating System on C:";

CHAR DntBootIniLine[] = "Windows XP Installation/Upgrade";

//
// Plain text, status msgs.
//

CHAR DntStandardHeader[]      = "\n ��⠭���� Windows XP\n������������������";
CHAR DntPersonalHeader[]      = "\n ��⠭���� Windows XP Personal\n���������������������������";
CHAR DntWorkstationHeader[]   = "\n ��⠭���� Windows XP Professional\n�������������������������������";
CHAR DntServerHeader[]        = "\n ��⠭���� Windows XP Server\n�������������������������";
CHAR DntParsingArgs[]         = "������ ��㬥�⮢...";
CHAR DntEnterEqualsExit[]     = "����=��室";
CHAR DntEnterEqualsRetry[]    = "����=�����";
CHAR DntEscEqualsSkipFile[]   = "ESC=�ய�� 䠩��";
CHAR DntEnterEqualsContinue[] = "����=�த�������";
CHAR DntPressEnterToExit[]    = "���������� �த������ ��⠭����. ������ ���� ��� ��室�.";
CHAR DntF3EqualsExit[]        = "F3=��室";
CHAR DntReadingInf[]          = "�⥭�� INF-䠩�� %s...";
CHAR DntCopying[]             = "� ����஢����: ";
CHAR DntVerifying[]           = "�    �஢�ઠ: ";
CHAR DntCheckingDiskSpace[]   = "�஢�ઠ ���� �� ��᪥...";
CHAR DntConfiguringFloppy[]   = "����ன�� ������� ��᪠...";
CHAR DntWritingData[]         = "������ ��ࠬ��஢ ��⠭����...";
CHAR DntPreparingData[]       = "�롮� ��ࠬ��஢ ��⠭����...";
CHAR DntFlushingData[]        = "������ ������ �� ���...";
CHAR DntInspectingComputer[]  = "�஢�ઠ ��������...";
CHAR DntOpeningInfFile[]      = "����⨥ INF-䠩��...";
CHAR DntRemovingFile[]        = "�������� 䠩�� %s";
CHAR DntXEqualsRemoveFiles[]  = "X=������� 䠩��";
CHAR DntXEqualsSkipFile[]     = "X=�ய����� 䠩�";

//
// confirmation keystroke for DnsConfirmRemoveNt screen.
// Keep in sync with DnsConfirmRemoveNt and DntXEqualsRemoveFiles.
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

    "��⠭���� Windows 2002 Server ��� Windows XP Professional",
    "",
    "WINNT [/s[:��室��_����]] [/t[:ࠡ�稩_���]]",
    "      [/u[:䠩�_�⢥⮢]] [udf:id[,UDF_䠩�]]",
    "      [/r:�����] [/rx:�����] [/e:�������] [/a]",
    "",
    "/s[:]��室��_����",
    "   ������ ��室��� �ᯮ������� 䠩��� Windows.",
    "   ������ ���� 㪠��� ����� ���� � ���� x:[����] ���",
    "   \\\\�ࢥ�\\�����[����].",
    "",
    "/t[:ࠡ�稩_���]",
    "   ������ ��� ��� ࠧ��饭�� �६����� 䠩��� ��⠭���� ",
    "   � ��� ࠧ��饭�� ��⠭���������� ��⥬� Windows XP.",
    "   �᫨ ��� �� 㪠���, �ணࠬ�� ��⠭���� ����⠥��� ",
    "   ᠬ����⥫쭮 �������� ࠡ�稩 ���.",
    "",
    "/u[:䠩�_�⢥⮢]",
    "   ������ ��⬠����� ��⠭���� � �ᯮ�짮������ 䠩�� �⢥⮢",
    "   (�ॡ���� 㪠���� ��ࠬ��� /s). ���� �⢥⮢ ᮤ�ন� �⢥��",
    "   �� �� ��� ������� ������ �ணࠬ�� ��⠭����, ���筮 ��",
    "   �⢥�� ���� ������ ���짮��⥫�.",
    "udf:id[,UDF_䠩�]",
    "   ����뢠�� �����䨪��� (ID), �ᯮ��㥬� �ணࠬ��� ��⠭����",
    "   ��� ⮣�, �⮡� ��।�����, ��� UDF-䠩� �㤥� �������� ��ࠬ����",
    "   䠩�� �⢥⮢ (�. ��ࠬ��� /u). ��ࠬ��� /udf ��४�뢠�� ���祭��",
    "   䠩�� �⢥⮢, � �����䨪��� 㪠�뢠��, ����� ���祭�� UDF-䠩��",
    "   ���� �ᯮ�짮����. �᫨ UDF-䠩� �� 㪠���, �ணࠬ�� ��⠭����",
    "   ������ ��᪥�� � 䠩��� $Unique$.udb",
    "",
    "/r[:�����]",
    "   ������ ����易⥫��� ����� ��� ��⠭����. �� ����� ��࠭����",
    "   ��᫥ �����襭�� ��⠭����.",
    "",
    "/rx[:�����]",
    "   ������ ����易⥫��� ����� ��� ����஢����. �� ����� 㤠�����",
    "   ��᫥ �����襭�� ��⠭����.",
    "",
    "/e:�������",
    "   ������ �믮������ 㪠������ ������� �� ����砭�� ����᪮� ��� ",
    "   ��⠭����.",
    "",
    "/a ����砥� ᯥ樠��� ���������� ��� �� � ���娬 �७���",
    "   ��� ��࠭�祭��� ������������.",
    NULL
};

//
//  Inform that /D is no longer supported
//
PCHAR DntUsageNoSlashD[] = {

    "�믮���� ��⠭���� Windows XP.",
    "",
    "WINNT [/S[:]��室��_����] [/T[:]ࠡ�稩_���] [/I[:]inf_䠩�]",
    "      [/U[:䠩�_�業���]]",
    "      [/R[x]:�����] [/E:�������] [/A]",
    "",
    "/D[:]winntroot",
    "       ��� ��ࠬ��� ����� �� �����ন������.",
    NULL
};

//
// out of memory screen
//

SCREEN
DnsOutOfMemory = { 4,6,
                   { "�ணࠬ�� ��⠭���� �� 墠⠥� ����� - ���������� �த������ ࠡ���.",
                     NULL
                   }
                 };

//
// Let user pick the accessibility utilities to install
//

SCREEN
DnsAccessibilityOptions = { 3, 5,
{   "������, ����� ᯥ樠��� ���������� ᫥��� ��⠭�����:",
    DntEmptyString,
    "[ ] ������ ������� F1 ��� ��⠭���� �࠭��� ���",
#ifdef NARRATOR
    "[ ] ������ ������� F2 ��� ��⠭���� �࠭���� �����",
#endif
#if 0
    "[ ] ������ ������� F3 ��� ��⠭���� �࠭��� ����������",
#endif
    NULL
}
};

//
// User did not specify source on cmd line screen
//

SCREEN
DnsNoShareGiven = { 3,5,
{ "�ணࠬ�� ��⠭���� ����室��� �����, ��� ��室���� 䠩�� Windows XP.",
  "������ ����, ��� ����� ���� 䠩�� Windows XP.",
  NULL
}
};


//
// User specified a bad source path
//

SCREEN
DnsBadSource = { 3,5,
                 { "�������� ��室�� ���� �� ��७, �� ����㯥� ��� �� ᮤ�ন� �ࠢ��쭮��",
                   "�������� ��⠭������ 䠩��� Windows XP. ������ ���� ����, ���",
                   "����� ���� 䠩�� Windows XP. �ᯮ���� ������� BACKSPACE ���",
                   "㤠����� ��������� ᨬ�����, � ��⥬ 㪠��� �ࠢ���� ����.",
                   NULL
                 }
               };


//
// Inf file can't be read, or an error occured parsing it.
//

SCREEN
DnsBadInf = { 3,5,
              { "�ணࠬ�� ��⠭���� �� ����� ���� ᢮� inf-䠩� ��� ��� 䠩� �ᯮ�祭.",
                "������� � ��⥬���� ������������.",
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
{ "���, ����� �� ��࠭ ��� ࠧ��饭�� �६����� 䠩��� ��⠭����, ��",
  "���� �ࠢ���� ��᪮� ��� �� ᮤ�ন� �� �ࠩ��� ��� %u ��",
  "(��� %lu ����) ᢮������� ��᪮���� ����࠭�⢠.",
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
{  "Windows XP ����室�� ⮬ �� ���⪮� ��᪥, �� ���஬ ᢮�����",
   "�� ����� %u �� (%lu ����). �ணࠬ�� ��⠭���� �ᯮ����",
   "���� �⮣� ����࠭�⢠ ��� �࠭���� �६����� 䠩���, ����室���� ��",
   "�६� ��⠭����. ��� ⮬ ������ ��室����� �� ����ﭭ� ������祭���",
   "�����쭮� ���⪮� ��᪥, �����ন������ Windows XP, � �� ������",
   "���� ᦠ�� ��᪮�.",
   DntEmptyString,
   "�ணࠬ�� ��⠭���� �� ᬮ��� �����㦨�� ⠪�� ��� � ������� ��ꥬ��",
   "᢮������� ��᪮���� ����࠭�⢠.",
  NULL
}
};

SCREEN
DnsNoSpaceOnSyspart = { 3,5,
{ "�������筮 ᢮������� ���� �� ���⮢�� ��᪥ (���筮 C:)",
  "��� �믮������ ��⠭���� ��� �ᯮ�짮����� ��᪥�. ��⠭���� ���",
  "�ᯮ�짮����� ��᪥� �ॡ�� ������ �������⥫��� 3.5 ��",
  "(3,641,856 ����) ᢮������� ����࠭�⢠ �� �⮬ ��᪥.",
  NULL
}
};

//
// Missing info in inf file
//

SCREEN
DnsBadInfSection = { 3,5,
                     { "����� [%s] ���ଠ樮����� 䠩�� �ணࠬ�� ��⠭����",
                       "��������� ��� ���०����.  ������� � ��⥬���� ������������.",
                       NULL
                     }
                   };


//
// Couldn't create directory
//

SCREEN
DnsCantCreateDir = { 3,5,
                     { "�ணࠬ�� ��⠭���� �� 㤠���� ᮧ���� 㪠������ ����� �� ����筮� ",
                       "���⪮� ��᪥:",
                       DntEmptyString,
                       "%s",
                       DntEmptyString,
                       "�஢���� ������ ��� � 㡥����� � ⮬, �� �� ��� ���������� 䠩��",
                       "� �������, ᮢ�����騬� � ᮧ�������� ������. �஢���� ⠪��",
                       "������� ᮥ������� ��� �⮣� ��᪠.",
                       NULL
                     }
                   };

//
// Error copying a file
//

SCREEN
DnsCopyError = { 4,5,
{  "�ணࠬ�� ��⠭���� �� 㤠���� ᪮��஢��� ᫥���騩 䠩�:",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  ������ ���� ��� ����७�� ����樨 ����஢����.",
   "  ������ ESC ��� �����஢���� �訡�� � �த������� ��⠭����.",
   "  ������ F3 ��� �४�饭�� ��⠭����.",
   DntEmptyString,
   "��������: �����஢���� �⮩ �訡�� � �த������� ��⠭���� �����",
   "�ਢ��� � ���쭥�襬 � ��㣨� �訡��� ��⠭����.",
   NULL
}
},
DnsVerifyError = { 4,5,
{  "��������� �ணࠬ��� ��⠭���� ����� 㪠������� ���� 䠩�� �� ᮢ������",
   "� �ਣ������. �� ����� ���� १���⮬ �⥢�� �訡��, ᡮ�� ��᪮����,",
   "��㣨� ��������� ����㤮�����.",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  ������ ���� ��� ����७�� ����樨 ����஢����.",
   "  ������ ESC ��� �����஢���� �訡�� � �த������� ��⠭����.",
   "  ������ F3 ��� �४�饭�� ��⠭����.",
   DntEmptyString,
   "��������: �����஢���� �⮩ �訡�� � �த������� ��⠭���� �����",
   "�ਢ��� � ���쭥�襬 � ��㣨� �訡��� ��⠭����.",
   NULL
}
};

SCREEN DnsSureSkipFile = { 4,5,
{  "�����஢���� �⮩ �訡�� ����砥�, �� ����� 䠩� �� �㤥� ᪮��஢��.",
   "��� ��ࠬ��� �।�����祭 ��� ������ ���짮��⥫��, ����� ���",
   "��������, � ����� �᫮������ ����� ������ ������⢨� ��⥬��� 䠩���.",
   DntEmptyString,
   "  ������ ���� ��� ����७�� ����樨 ����஢����.",
   "  ������ X ��� ⮣�, �⮡� �ய����� ��� 䠩�.",
   DntEmptyString,
   "��������: �᫨ ��� 䠩� �㤥� �ய�饭, �ணࠬ�� ��⠭���� �� �����",
   "��࠭�஢��� �ᯥ譮� ��⠭���� ��� ���������� Windows XP.",
  NULL
}
};

//
// Wait while setup cleans up previous local source trees.
//

SCREEN
DnsWaitCleanup =
    { 4,6,
        { "��������, ���� �ணࠬ�� ��⠭���� 㤠��� ����騥�� �६���� 䠩��.",
           NULL
        }
    };

//
// Wait while setup copies files
//

SCREEN
DnsWaitCopying = { 4,6,
                   { "��������, ���� �ணࠬ�� ��⠭���� ᪮����� 䠩�� �� ���⪨� ���.",
                     NULL
                   }
                 },
DnsWaitCopyFlop= { 4,6,
                   { "��������, ���� �ணࠬ�� ��⠭���� ᪮����� 䠩�� �� ��᪥��.",
                     NULL
                   }
                 };

//
// Setup boot floppy errors/prompts.
//
SCREEN
DnsNeedFloppyDisk3_0 = { 4,4,
{  "�ணࠬ�� ��⠭���� �ॡ����� ���� ���ଠ�஢�����, ����� ��᪥��",
   "��᮪�� ���⭮��. � ���쭥�襬 �ணࠬ�� ��⠭���� �㤥� ���������",
   "�� ��᪥�� \"��⠭����� ��� 1 Windows XP\", ",
   "\"��⠭����� ��� 2 Windows XP\", ",
   "\"��⠭����� ��� 3 Windows XP\" � ",
   "\"��⠭����� ��� 4 Windows XP\".",
   DntEmptyString,
   "��⠢�� ���� �� ��� ����� ��᪥� � ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 4 Windows XP.\"",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk3_1 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 4 Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk2_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 3 Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk1_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 2 Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk0_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 1 Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_0 = { 4,4,
{  "�ணࠬ�� ��⠭���� �ॡ����� ���� ���ଠ�஢�����, ����� ��᪥��",
   "��᮪�� ���⭮��. � ���쭥�襬 �ணࠬ�� ��⠭���� �㤥� ���������",
   "�� ��᪥�� \"��⠭����� ��� 1 Windows 2002\", ",
   "\"��⠭����� ��� 2 Windows 2002\", ",
   "\"��⠭����� ��� 3 Windows 2002\" � ",
   "\"��⠭����� ��� 4 Windows 2002\".",
   DntEmptyString,
   "��⠢�� ���� �� ��� ����� ��᪥� � ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 4 Windows 2002\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_1 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 4 Windows 2002\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk2_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 3 Windows 2002\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk1_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 2 Windows 2002\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk0_0 = { 4,4,
{  "��⠢�� ���ଠ�஢�����, ����� ��᪥�� ��᮪�� ���⭮��",
   "� ��᪮��� A:.",
   "�㤥� ᮧ��� \"��⠭����� ��� 1 Windows 2002\".",
  NULL
}
};

//
// The floppy is not formatted.
//
SCREEN
DnsFloppyNotFormatted = { 3,4,
{ "�।��⠢������ ��᪥� �� ���ଠ�஢��� ��� �ᯮ�짮����� � MS-DOS.",
  "�ணࠬ�� ��⠭���� �� ����� �ᯮ�짮���� ��� ��᪥��.",
  NULL
}
};

//
// We think the floppy is not formatted with a standard format.
//
SCREEN
DnsFloppyBadFormat = { 3,4,
{ "�� ��᪥� ���� �� ���� ���ଠ�஢����� ��᪥⮩ ��᮪�� ���⭮��,",
  "���� ���ଠ�஢��� �� �⠭����� ��� MS-DOS ᯮᮡ��, ���� ���०����.",
  "�ணࠬ�� ��⠭���� �� ����� �ᯮ�짮���� ��� ��᪥��.",
  NULL
}
};

//
// We can't determine the free space on the floppy.
//
SCREEN
DnsFloppyCantGetSpace = { 3,4,
{ "�ணࠬ�� ��⠭���� �� 㤠���� ��।����� ��ꥬ ᢮������� ���� �� �⮩",
  "��᪥�. �ணࠬ�� ��⠭���� �� ����� �ᯮ�짮���� ��� ��᪥��.",
  NULL
}
};

//
// The floppy is not blank.
//
SCREEN
DnsFloppyNotBlank = { 3,4,
{ "�� ��᪥� ���� �� ���� ��᪥⮩ ��᮪�� ���⭮��, ���� 㦥 ᮤ�ন�",
  "������� 䠩��. �ணࠬ�� ��⠭���� �� ����� �ᯮ�짮���� ��� ��᪥��.",
  NULL
}
};

//
// Couldn't write the boot sector of the floppy.
//
SCREEN
DnsFloppyWriteBS = { 3,4,
{ "�ணࠬ�� ��⠭���� �� 㤠���� �믮����� ������ � ��⥬��� ������� ",
  "�⮩ ��᪥��. ����⭮, �� ��᪥� �� �ਣ���� ��� �ᯮ�짮�����.",
  NULL
}
};

//
// Verify of boot sector on floppy failed (ie, what we read back is not the
// same as what we wrote out).
//
SCREEN
DnsFloppyVerifyBS = { 3,4,
{ "�����, ���⠭�� �ணࠬ��� ��⠭���� �� ��⥬��� ������ �⮩ ��᪥��",
  "���� �� ᮢ������ � ����ᠭ�묨, ���� �ணࠬ�� ��⠭���� �� 㤠����",
  "������ ��⥬��� ������� ��᪥�� ��� �஢�ન.",
  DntEmptyString,
  "�� ����� ���� �맢��� ᫥���騬� ��稭���:",
  DntEmptyString,
  "  ����� �������� ��ࠦ�� ����ᮬ.",
  "  �।��⠢������ ��᪥� ���०����.",
  "  ����㤮����� ��᪮���� ����ࠢ�� ��� ����୮ ����஥��.",
  NULL
}
};


//
// We couldn't write to the floppy drive to create winnt.sif.
//

SCREEN
DnsCantWriteFloppy = { 3,5,
{ "�ணࠬ�� ��⠭���� �� 㤠���� ������� ����� �� ��᪥��, ��⠭��������",
  "� ��᪮��� A:. ��������, �� ��᪥� ���०����. ���஡�� �ᯮ�짮����",
  "����� ��᪥��.",
  NULL
}
};


//
// Exit confirmation dialog
//

SCREEN
DnsExitDialog = { 10,6,
                  { "�����������������������������������������������������������ͻ",
                    "�  Windows XP �� �뫠 ��������� ��⠭������ �� �⮬         �",
                    "�  ��������. �᫨ �������� ��⠭���� ᥩ��, �          �",
                    "�  ����������� ������ �믮����� ��⠭���� Windows XP.       �",
                    "�                                                           �",
                    "�      ������ <����> ��� �த������� ��⠭����.           �",
                    "�      ������ <F3> ��� �४�饭�� ��⠭����              �",
                    "�����������������������������������������������������������Ķ",
                    "�  F3=��室  ����=�த������                                �",
                    "�����������������������������������������������������������ͼ",
                    NULL
                  }
                };


//
// About to reboot machine and continue setup
//

SCREEN
DnsAboutToRebootW =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "����� �ணࠬ�� ��⠭���� �ந������ ��१���㧪� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows XP �㤥� �த������.",
  DntEmptyString,
  "��। ⥬, ��� �த������ ࠡ���, 㡥����� � ⮬, �� �����⮢������",
  "࠭�� ��᪥� \"��⠭����� ��� 1 Windows XP\"",
  "��⠢���� � ��᪮��� A:.",
  DntEmptyString,
  "������ ���� ��� ��१���㧪� �������� � �த������� ��⠭����",
  "Windows XP.",
  NULL
}
},
DnsAboutToRebootS =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "����� �ணࠬ�� ��⠭���� �ந������ ��१���㧪� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows 2002 �㤥� �த������.",
  DntEmptyString,
  "��। ⥬, ��� �த������ ࠡ���, 㡥����� � ⮬, �� �����⮢������",
  "࠭�� ��᪥� \"��⠭����� ��� 1 Windows 2002\"",
  "��⠢���� � ��᪮��� A:.",
  DntEmptyString,
  "������ ���� ��� ��१���㧪� �������� � �த������� ��⠭����",
  "Windows 2002.",
  NULL
}
},
DnsAboutToRebootX =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "����� �ணࠬ�� ��⠭���� �ந������ ��१���㧪� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows XP �㤥� �த������.",
  DntEmptyString,
  "�᫨ � ��᪮��� A: ��⠢���� ��᪥�, ����� ��.",
  DntEmptyString,
  "������ ���� ��� ��१���㧪� �������� � �த������� ��⠭����",
  "Windows XP.",
  NULL
}
};

//
// Need another set for '/w' switch since we can't reboot from within Windows.
//

SCREEN
DnsAboutToExitW =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "������ ��� ����室��� ��१���㧨�� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows XP �㤥� �த������.",
  DntEmptyString,
  "��। ⥬, ��� �த������ ࠡ���, 㡥����� � ⮬, �� �����⮢������",
  "࠭�� ��᪥� \"��⠭����� ��� 1 Windows XP\" ",
  "��⠢���� � ��᪮��� A:.",
  DntEmptyString,
  "������ ���� ��� ��室� � MS-DOS, � ��⥬ ��१���㧨� ��������",
  "��� �த������� ��⠭���� Windows XP.",
  NULL
}
},
DnsAboutToExitS =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "������ ��� ����室��� ��१���㧨�� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows 2002 �㤥� �த������.",
  DntEmptyString,
  "��। ⥬, ��� �த������ ࠡ���, 㡥����� � ⮬, �� �����⮢������",
  "࠭�� ��᪥� \"��⠭����� ��� 1 Windows 2002\" ",
  "��⠢���� � ��᪮��� A:.",
  DntEmptyString,
  "������ ���� ��� ��室� � MS-DOS, � ��⥬ ��१���㧨� ��������",
  "��� �த������� ��⠭���� Windows 2002.",
  NULL
}
},
DnsAboutToExitX =
{ 3,5,
{ "���� �⠯ ��⠭���� ��� �ࠢ������ MS-DOS �����襭.",
  "������ ��� ����室��� ��१���㧨�� ��������.",
  "��᫥ ��१���㧪� ��⠭���� Windows XP �㤥� �த������.",
  DntEmptyString,
  "�᫨ � ��᪮��� A: ��⠢���� ��᪥�, ����� ��.",
  DntEmptyString,
  "������ ���� ��� ��室� � MS-DOS, � ��⥬ ��१���㧨� ��������",
  "��� �த������� ��⠭���� Windows XP.",
  NULL
}
};

//
// Gas gauge
//

SCREEN
DnsGauge = { 7,15,
             { "����������������������������������������������������������������ͻ",
               "�      �ணࠬ�� ��⠭���� �믮���� ����஢���� 䠩���...       �",
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
{ "��� �믮������ �⮩ �ணࠬ�� ����室��� MS-DOS ���ᨨ 5.0 ��� �����.",
  NULL
}
},

DnsRequiresFloppy = { 3,5,
#ifdef ALLOW_525
{ "�ணࠬ�� ��⠭���� ��।�����, �� ��᪮��� A: ��������� ��� �����",
  "������ ���⭮���. ��� �믮������ ��⠭���� ����室�� ��᪮���, ᯮᮡ��",
  "ࠡ���� � ��᪥⠬� ��ꥬ�� 1,2 �������� ��� �����.",
#else
{ "�ணࠬ�� ��⠭���� ��।�����, �� ��᪮��� A: ��������� ���",
  "�� ���� 3,5\" ��᪮����� ��᮪�� ���⭮��. ����室�� ��᪮���,",
  "ᯮᮡ�� ࠡ���� � ��᪥⠬� ��ꥬ�� 1,44 �������� ��� ����� ���",
  "��⠭���� � �ᯮ�짮������ ��᪥�",
  DntEmptyString,
  "�⮡� �믮����� ��⠭���� Windows XP ��� �ᯮ�짮����� ��᪥�,",
  "��१������ ��� �ணࠬ�� � �ᯮ���� ��ࠬ���  /b  � ���������",
  "��ப�.",
#endif
  NULL
}
},

DnsRequires486 = { 3,5,
{ "�ணࠬ�� ��⠭���� ��।�����, �� �� �⮬ �������� ��� 80486",
  "��� ����� ������ ������. Windows XP �� ����� ࠡ����",
  "�� �⮬ ��������.",
  NULL
}
},

DnsCantRunOnNt = { 3,5,
{ "�� �ணࠬ�� �� ����� ࠡ���� ��� �ࠢ������ 32-��⭮� ���ᨨ Windows.",
  DntEmptyString,
  "����� ��� ᫥��� �ᯮ�짮���� �ணࠬ�� WINNT32.EXE.",
  NULL
}
},

DnsNotEnoughMemory = { 3,5,
{ "�ணࠬ�� ��⠭���� ��।�����, �� �� �⮬ �������� �� ��⠭������",
  "�����筮�� ������⢠ ����⨢��� ����� ��� ࠡ��� Windows XP.",
  DntEmptyString,
  "�ॡ㥬� ��ꥬ �����: %lu%s ��",
  "����騩�� ��ꥬ �����: %lu%s ��",
  NULL
}
};


//
// Screens used when removing existing nt files
//
SCREEN
DnsConfirmRemoveNt = { 3,5,
{   "�� ����ᨫ� �ணࠬ�� ��⠭���� 㤠���� 䠩�� Windows XP ��",
    "㪠������ ���� �����. ��⠭�������� � �⮩ ����� �����",
    "Windows �㤥� 㭨�⮦���.",
    DntEmptyString,
    "%s",
    DntEmptyString,
    DntEmptyString,
    "  ������ F3 ��� ��室� �� ��⠭���� ��� 㤠����� �����-���� 䠩���.",
    "  ������ X ��� 㤠����� 䠩��� Windows �� 㪠������ ��� �����.",
    NULL
}
},

DnsCantOpenLogFile = { 3,5,
{ "�ணࠬ�� ��⠭���� �� 㤠���� ������ 㪠����� ���� LOG-䠩� ��⠭����.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "�ணࠬ�� ��⠭���� �� ����� 㤠���� 䠩�� Windows �� 㪠������",
  "�����.",
  NULL
}
},

DnsLogFileCorrupt = { 3,5,
{ "�ணࠬ�� ��⠭���� �� 㤠���� �����㦨�� ࠧ��� %s",
  "� 㪠������ ���� LOG-䠩�� ��⠭����.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "�ணࠬ�� ��⠭���� �� ����� 㤠���� 䠩�� Windows �� 㪠������",
  "�����.",
  NULL
}
},

DnsRemovingNtFiles = { 3,5,
{ "        ��������, ���� �ணࠬ�� ��⠭���� 㤠��� 䠩�� Windows.",
  NULL
}
};

SCREEN
DnsNtBootSect = { 3,5,
{ "�ணࠬ�� ��⠭���� �� 㤠���� ��⠭����� Windows Boot Loader.",
  DntEmptyString,
  "��������, �� ��� C: ���ଠ�஢�� � ���ன�⢮ ��ࠢ��.",
  NULL
}
};

SCREEN
DnsOpenReadScript = { 3,5,
{ "��� ����㯠 � 䠩�� �業���, 㪠������� � ������� ��ࠬ���",
  "��������� ��ப� /u.",
  DntEmptyString,
  "���������� �த������ �믮������ ��⮬���᪮� ��⠭����",
  "��� ����⥫��⢠ ������.",
  NULL
}
};

SCREEN
DnsParseScriptFile = { 3,5,
{ "���� �業���, 㪠����� � ������� ��ࠬ��� ��������� ��ப� /u",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "ᮤ�ন� ᨭ⠪����� �訡�� � ��ப� %u.",
  DntEmptyString,
  NULL
}
};

SCREEN
DnsBootMsgsTooLarge = { 3,5,
{ "�ந��諠 ����७��� �訡�� �ணࠬ�� ��⠭����.",
  DntEmptyString,
  "��ॢ������ ᮮ�饭�� ����㧪� ᫨誮� ������.",
  NULL
}
};

SCREEN
DnsNoSwapDrive = { 3,5,
{ "�ந��諠 ����७��� �訡�� �ணࠬ�� ��⠭����.",
  DntEmptyString,
  "�� 㤠���� ���� ���� ��� 䠩�� �����窨.",
  NULL
}
};

SCREEN
DnsNoSmartdrv = { 3,5,
{ "�� 㤠���� ���� SmartDrive �� �⮬ ��������. SmartDrive ����⢥���",
  "����蠥� ᪮���� �믮������ �⮩ 䠧� ��⠭���� Windows.",
  DntEmptyString,
  "�멤�� �� �ணࠬ�� ��⠭����, ������� SmartDrive, � ��⥬",
  "᭮�� ������� �ணࠬ�� ��⠭����. �������⥫�� ᢥ����� �",
  "�ணࠬ�� SmartDrive ����� ���� � ���㬥��樨 �� DOS.",
  DntEmptyString,
    "  ������ F3 ��� ��室� �� �ணࠬ�� ��⠭����.",
    "  ������ ���� ��� �த������� ��⠭���� ��� SmartDrive.",
  NULL
}
};

//
// Boot messages. These go in the fat and fat32 boot sectors.
//
CHAR BootMsgNtldrIsMissing[] = "NTLDR is missing";
CHAR BootMsgDiskError[] = "Disk error";
CHAR BootMsgPressKey[] = "Press any key to restart";

