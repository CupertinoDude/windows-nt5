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
CHAR DntPreviousOs[]  = "P�edchoz� opera�n� syst�m na jednotce C:";

CHAR DntBootIniLine[] = "Instalace nebo inovace syst�mu Windows XP";

//
// Plain text, status msgs.
//

CHAR DntStandardHeader[]      = "\n Instalace syst�mu Windows XP \n�����������������������������";
CHAR DntPersonalHeader[]      = "\n Instalace syst�mu Windows XP Home Edition\n�������������������������������������������";
CHAR DntWorkstationHeader[]   = "\n Instalace syst�mu Windows XP Professional\n�������������������������������������������";
CHAR DntServerHeader[]        = "\n Instalace syst�mu Windows 2002 Server\n���������������������������������������";
CHAR DntParsingArgs[]         = "Anal�za parametr�...";
CHAR DntEnterEqualsExit[]     = "ENTER=Konec";
CHAR DntEnterEqualsRetry[]    = "ENTER=Zkusit znovu";
CHAR DntEscEqualsSkipFile[]   = "ESC=Vynechat soubor";
CHAR DntEnterEqualsContinue[] = "ENTER=Pokra�ovat";
CHAR DntPressEnterToExit[]    = "Instalace nem��e pokra�ovat. Ukon�ete instalaci kl�vesou ENTER.";
CHAR DntF3EqualsExit[]        = "F3=Konec";
CHAR DntReadingInf[]          = "Na��t� se soubor INF %s...";
CHAR DntCopying[]             = "� Kop�ruje se: ";
CHAR DntVerifying[]           = "�  Ov��uje se: ";
CHAR DntCheckingDiskSpace[]   = "Zji�ov�n� m�sta na disku...";
CHAR DntConfiguringFloppy[]   = "Konfigurace diskety...";
CHAR DntWritingData[]         = "Z�pis parametr� instala�n�ho programu...";
CHAR DntPreparingData[]       = "Zji�ov�n� parametr� instala�n�ho programu...";
CHAR DntFlushingData[]        = "Z�pis dat na disk...";
CHAR DntInspectingComputer[]  = "Anal�za po��ta�e...";
CHAR DntOpeningInfFile[]      = "Otev�r�n� souboru INF...";
CHAR DntRemovingFile[]        = "Odstra�ov�n� souboru %s";
CHAR DntXEqualsRemoveFiles[]  = "X=Odstranit soubory";
CHAR DntXEqualsSkipFile[]     = "X=Vynechat soubor";

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

    "Nainstaluje syst�m Windows 2002 Server nebo Windows XP Professional.",
    "",
    "",
    "WINNT [/s[:zdrojov�_cesta]] [/t[:do�asn�_jednotka]]",
    "      [/u[:soubor odpov�d�]] [/udf:id[,soubr_UDF]]",
    "      [/r:slo�ka] [/r[x]:slo�ka] [/e:p��kaz] [/a]",
    "",
    "",
    "/s[:zdrojov�_cesta]",
    "   Ur�uje um�st�n� zdrojov�ch soubor� syst�mu Windows.",
    "   Mus� se zadat �pln� cesta ve tvaru x:[cesta]",
    "   nebo \\\\server\\sd�len�[\\cesta].",
    "",
    "/t[:do�asn�_jednotka]",
    "   Ur�uje jednotku, na ni� instala�n� program um�st� do�asn� instala�n�",
    "   soubory a na ni� nainstaluje syst�m Windows.",
    "   Pokud nen� zad�na, instala�n� program se pokus� vyhledat jednotku s�m.",
    "",
    "/u[:soubor odpov�d�]",
    "   Provede bezobslu�nou instalaci pomoc� souboru odpov�d� (vy�aduje",
    "   p�ep�na� /s). Soubor odpov�d� obsahuje odpov�di na n�kter� nebo",
    "   v�echny dotazy, na nا u�ivatel obvykle odpov�d� b�hem instalace.",
    "",
    "/udf:id[,soubor_UDF]   ",
    "   Identifik�tor  (id) ur�uje, jak�m zp�sobem modifikuje soubor UDF ",
    "   (Uniqueness Database File) soubor odpov�d�  ",
    "   (viz p�ep�na� /u). Parametr /udf p�edefinuje hodnoty v souboru ",
    "   odpov�d� a tento indentifik�tor ur�uje, kter� hodnoty v souboru UDF",
    "   budou pou�ity. Pokud soubor UDF nen� zad�n, instala�n� program ",
    "   v�s vyzve ke vlo�en� diskety obsahuj�c� soubor $Unique$.udb.",
    "",
    "/r[:slo�ka]",
    "   Ur�uje volitelnou slo�ku, kter� m� b�t nainstalov�na. Slo�ka",
    "   po dokon�en� instalace z�stane v p�vodn�m um�st�n�.",
    "",
    "/rx[:slo�ka]",
    "   Ur�uje volitelnou slo�ku, kter� m� b�t zkop�rov�na. Slo�ka bude ",
    "   po dokon�en� instalace odstran�na.",
    "",
    "/e   Ur�uje p��kaz ke spu�t�n� po dokon�en� grafick� ��sti instalace.",
    "",
    "/a   Zapne mo�nosti usnadn�n�.",
    NULL
};

//
//  Inform that /D is no longer supported
//
PCHAR DntUsageNoSlashD[] = {

    "Nainstaluje syst�m Windows XP.",
    "",
    "WINNT [/S[:]zdroj_cesta] [/T[:]do�_jednotka] [/I[:]soubor_INF]",
    "      [/U[:soubor_skriptu]]",
    "      [/R[X]:adres��] [/E:p��kaz] [/A]",
    "",
    "/D[:]ko�en_winnt",
    "       Tato mo�nost ji� nen� podporov�na.",
    NULL
};

//
// out of memory screen
//

SCREEN
DnsOutOfMemory = { 4,6,
                   { "Instala�n� program nem� dost pam�ti a nem��e pokra�ovat.",
                     NULL
                   }
                 };

//
// Let user pick the accessibility utilities to install
//

SCREEN
DnsAccessibilityOptions = { 3, 5,
{   "Zvolte, zda chcete nainstalovat n�sleduj�c� funkce usnadn�n�:",
    DntEmptyString,
    "[ ] Chcete-li nainstalovat program Lupa, stiskn�te F1",
#ifdef NARRATOR
    "[ ] Chcete-li nainstalovat program P�ed��t�n� obrazovky, stiskn�te F2",
#endif
#if 0
    "[ ] Chcete-li nainstalovat program Kl�vesnice na obrazovce, stiskn�te F3",
#endif
    NULL
}
};

//
// User did not specify source on cmd line screen
//

SCREEN
DnsNoShareGiven = { 3,5,
{ "Instala�n� program mus� zn�t um�st�n� soubor� syst�mu Windows XP.",
  "Zadejte cestu k um�st�n� soubor� syst�mu Windows XP.",
  NULL
}
};


//
// User specified a bad source path
//

SCREEN
DnsBadSource = { 3,5,
                 { "Zadan� zdroj nen� platn� �i p��stupn�, nebo neobsahuje platnou",
                   "instalaci syst�mu Windows XP. Zadejte novou cestu, kde se soubory",
                   "syst�mu Windows XP nal�zaj�. Pomoc� kl�vesy BACKSPACE vyma�te znaky",
                   "a zadejte novou cestu.",
                   NULL
                 }
               };


//
// Inf file can't be read, or an error occured parsing it.
//

SCREEN
DnsBadInf = { 3,5,
              { "Instala�n� program nemohl na��st dan� informa�n� soubor, nebo",
                " je informa�n� soubor po�kozen. Obra�te se na spr�vce syst�mu.",
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
{ "Zadan� jednotka pro do�asn� instala�n� soubory nen� platn�, nebo neobsahuje",
  "alespo� %u megabajt� (%lu bajt�) voln�ho m�sta.",
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
{  "Syst�m Windows XP pot�ebuje svazek diskov� jednotky s nejm�n� %u megabajty",
   "(%lu bajty) voln�ho m�sta. Instala�n� program vyu�ije ��st tohoto",
   "prostoru k ukl�d�n� do�asn�ch soubor� b�hem instalace. Dan� jednotka",
   "mus� b�t na trvale p�ipojen�m pevn�m disku, kter� je podporov�n,",
   "syst�mem Windows XP a jednotka nesm� b�t komprimov�na.",
   DntEmptyString,
   "Instala�n� program nemohl naj�t ��dnou jednotku s po�adovan�m voln�m",
   "prostorem.",
  NULL
}
};

SCREEN
DnsNoSpaceOnSyspart = { 3,5,
{ "Na dan�m spou�t�c�m disku (obvykle C:) nen� dost prostoru pro",
  "instalaci bez disket. Instalace bez disket vy�aduje nejm�n�",
  "3,5 MB (3,641,856 bajt�) voln�ho m�sta na dan� jednotce.",
  NULL
}
};

//
// Missing info in inf file
//

SCREEN
DnsBadInfSection = { 3,5,
                     { "Sekce [%s] informa�n�ho souboru instala�n�ho programu chyb�",
                       "nebo je po�kozena. Obra�te se na spr�vce syst�mu.",
                       NULL
                     }
                   };


//
// Couldn't create directory
//

SCREEN
DnsCantCreateDir = { 3,5,
                     { "Na zadan� c�lov� jednotce se nepoda�ilo vytvo�it adres��:",
                       DntEmptyString,
                       "%s",
                       DntEmptyString,
                       "Zkontrolujte c�lovou jednotku, zda neobsahuje soubory s n�zvy, kter�",
                       "se shoduj� s c�lov�m adres��em. Zkontrolujte tak� kabely dan� jednotky.",
                       NULL
                     }
                   };

//
// Error copying a file
//

SCREEN
DnsCopyError = { 4,5,
{  "Instala�n� program nemohl zkop�rovat soubor:",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  Stisknut�m kl�vesy ENTER se pokuste kop�rovat znovu.",
   "  Stisknut�m kl�vesy ESC bude chyba ignorov�na a instalace bude pokra�ovat.",
   "  Stisknut�m kl�vesy F3 ukon��te instalaci.",
   DntEmptyString,
   "Pozn�mka: Pokud budete chybu ignorovat a pokra�ovat, m��ete se setkat",
   "          s chybami i pozd�ji.",
   NULL
}
},
DnsVerifyError = { 4,5,
{  "Kopie uveden�ho souboru vytvo�en� instala�n�m programem, nen� shodn�",
   "s origin�lem. To m��e b�t zp�sobeno chybami s�t�, disketov� jednotky,",
   "nebo jin�mi hardwarov�mi probl�my.",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  Stisknut�m kl�vesy ENTER se pokuste kop�rovat znovu.",
   "  Stisknut�m kl�vesy ESC bude chyba ignorov�na a instalace bude pokra�ovat.",
   "  Stisknut�m kl�vesy F3 ukon��te instalaci.",
   DntEmptyString,
   "Pozn�mka: Pokud budete chybu ignorovat a pokra�ovat, m��ete se setkat",
   "s chybami i pozd�ji.",
   NULL
}
};

SCREEN DnsSureSkipFile = { 4,5,
{  "Pokud budete chybu ignorovat, nebude soubor zkop�rov�n.",
   "Tato volba je ur�ena zku�en�m u�ivatel�m, kte�� rozum�",
   "n�sledk�m chyb�j�c�ch syst�mov�ch soubor�.",
   DntEmptyString,
   "  Stisknut�m kl�vesy ENTER se pokuste kop�rovat znovu.",
   "  Stisknut�m kl�vesy X tento soubor p�esko��te.",
   DntEmptyString,
   "Pozn�mka: Pokud soubor vynech�te, nem��e instala�n� program zaru�it",
   "�sp��nou instalaci nebo inovaci syst�mu Windows XP.",
  NULL
}
};

//
// Wait while setup cleans up previous local source trees.
//

SCREEN
DnsWaitCleanup =
    { 12,6,
        { "Po�kejte, ne� budou odstran�ny p�edchoz� do�asn� soubory.",
           NULL
        }
    };

//
// Wait while setup copies files
//

SCREEN
DnsWaitCopying = { 13,6,
                   { "Po�kejte, ne� budou zkop�rov�ny soubory na pevn� disk.",
                     NULL
                   }
                 },
DnsWaitCopyFlop= { 13,6,
                   { "Po�kejte, ne� budou zkop�rov�ny soubory na disketu.",
                     NULL
                   }
                 };

//
// Setup boot floppy errors/prompts.
//
SCREEN
DnsNeedFloppyDisk3_0 = { 4,4,
{  "Instalace vy�aduje �ty�i pr�zdn� naform�tovan� diskety (vysok� hustota).",
   "Tyto diskety budou instala�n�m programem ozna�ov�ny jako \"Spou�t�c� disk",
   "instalace syst�mu Windows XP\", \"Disk �.2 instalace syst�mu Windows XP\"",
   "\"Disk �.3 instalace syst�mu Windows XP\" a",
   "\"Disk �.4 instalace syst�mu Windows XP\".",
   DntEmptyString,
   "Vlo�te jednu z t�chto �ty� disket do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.4 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk3_1 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.4 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk2_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.3 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk1_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.2 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk0_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Spou�t�c� disk instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_0 = { 4,4,
{  "Instalace vy�aduje �ty�i pr�zdn� naform�tovan� diskety (vysok� hustota).",
   "Tyto diskety budou instala�n�m programem ozna�ov�ny jako \"Spu�t�c� disk",
   "instalace syst�mu Windows XP\", \"Disk �.2 instalace syst�mu Windows XP\",",
   "\"Disk �.3 instalace syst�mu Windows XP\" a \"Disk �.4 instalace",
   "syst�mu Windows XP\".",
   DntEmptyString,
   "Vlo�te jednu z t�chto �ty� disket do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.4 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_1 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.4 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk2_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.3 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk1_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Disk �.2 instalace syst�mu Windows XP\".",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk0_0 = { 4,4,
{  "Vlo�te pr�zdnou naform�tovanou disketu (vysok� hustota) do jednotky A:.",
   "Disketa bude m�t n�zev \"Spou�t�c� disk instalace syst�mu Windows XP\".",
  NULL
}
};

//
// The floppy is not formatted.
//
SCREEN
DnsFloppyNotFormatted = { 3,4,
{ "Vlo�en� disketa nen� naform�tovan� pro pou�it� v syst�mu MS-DOS.",
  "Instala�n� program nem��e disketu pou��t.",
  NULL
}
};

//
// We think the floppy is not formatted with a standard format.
//
SCREEN
DnsFloppyBadFormat = { 3,4,
{ "Vlo�en� disketa nen� naform�tovan� na standardn� form�t syst�mu MS-DOS",
  "(vysok� hustota) nebo je po�kozen�. Instala�n� program nem��e disketu pou��t.",
  NULL
}
};

//
// We can't determine the free space on the floppy.
//
SCREEN
DnsFloppyCantGetSpace = { 3,4,
{ "Instala�n� program nem��e ur�it velikost voln�ho m�sta na disket�.",
  "Instala�n� program nem��e disketu pou��t.",
  NULL
}
};

//
// The floppy is not blank.
//
SCREEN
DnsFloppyNotBlank = { 3,4,
{ "Vlo�en� disketa neumo��uje vysokou hustotu z�znamu, nebo nen� pr�zdn�.",
  "Instala�n� program nem��e disketu pou��t.",
  NULL
}
};

//
// Couldn't write the boot sector of the floppy.
//
SCREEN
DnsFloppyWriteBS = { 3,4,
{ "Instala�n� program nemohl zapisovat do syst�mov� oblasti vlo�en� diskety.",
  "Disketa je pravd�podobn� nepou�iteln�.",
  NULL
}
};

//
// Verify of boot sector on floppy failed (ie, what we read back is not the
// same as what we wrote out).

//
SCREEN
DnsFloppyVerifyBS = { 3,4,
{ "Data na�ten� ze syst�mov� oblasti diskety neodpov�daj� d��ve zapsan�m",
  "informac�m, nebo se instala�n�mu program data nepoda�ilo na��st",
  "a ov��it.",
  DntEmptyString,
  "To je zp�sobeno jednou nebo v�ce z n�sleduj�ch okolnost�:",
  DntEmptyString,
  "  Po��ta� byl infikov�n po��ta�ov�m virem.",
  "  Vlo�en� disketa je po�kozen�.",
  "  Na disketov� jednotce jsou pot��e s hardwarem nebo konfigurac�.",
  NULL
}
};


//
// We couldn't write to the floppy drive to create winnt.sif.
//

SCREEN
DnsCantWriteFloppy = { 3,5,
{ "Instala�n�mu programu se nepoda�il z�pis na disketu v jednotce A:. Vlo�en�",
  "disketa m��e b�t po�kozena. Zkuste pou��t jinou disketu.",
  NULL
}
};



//
// Exit confirmation dialog
//

SCREEN
DnsExitDialog = { 13,6,
                  { "����������������������������������������������������ͻ",
                    "�  Syst�m Windows XP nen� na po��ta�i zcela          �",
                    "�  nainstalov�n. Ukon��te-li nyn� instalaci, budete  �",
                    "�  ji muset spustit znovu, aby bylsyst�m             �",
                    "�  Windows XP nainstalov�n.                          �",
                    "�                                                    �",
                    "� Stisknut�m kl�vesy ENTER bude instalace pokra�ovat.�",
                    "� Stisknut�m kl�vesy F3 instalaci ukon��te.          �",
                    "����������������������������������������������������ĺ",
                    "�  F3=Konec  ENTER=Pokra�ovat                        �",
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
{ "Instala�n� program dokon�il ��st instalace z prost�ed� MS-DOS.",
  "Instala�n� program bude restartovat po��ta�. Po nov�m spu�t�n�",
  "bude instalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,
  "Zajist�te, aby byl \"Spou�t�c� disk instalace syst�mu Windows\" ",
  "vlo�en do jednotky A: p�ed restartov�n�m.",
  DntEmptyString,
  "Stisknut�m kl�vesy ENTER restartujte po��ta� a pokra�ujte v instalaci.",
  NULL
}
},
DnsAboutToRebootS =
{ 3,5,
{ "Instala�n� program dokon�il ��st instalace v prost�ed� syst�mu MS-DOS.",
  "Instala�n� program nyn� restartuje po��ta�. Po nov�m spu�t�n�",
  "bude instalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,                       
  "Zajist�te, aby byl \"Spou�t�c� disk instalace syst�mu Windows XP\" ",
  "vlo�en do jednotky A: je�t� p�ed restartov�n�m.",
  DntEmptyString,
  "Stisknut�m kl�vesy ENTER restartujte po��ta� a pokra�ujte v instalaci.",
  NULL
}
},
DnsAboutToRebootX =
{ 3,5,
{ "Instala�n� program dokon�il ��st instalace z prost�ed� MS-DOS.",
  "Instala�n� program nyn� restartuje po��ta�. Po nov�m spu�t�n�",
  "bude instalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,
  "Pokud je v jednotce A: vlo�ena disketa, vyjm�te ji.",
  DntEmptyString,
  "Stisknut�m kl�vesy ENTER restartujte po��ta� a pokra�ujte v instalaci.",
  NULL
}
};

//
// Need another set for '/w' switch since we can't reboot from within Windows.
//

SCREEN
DnsAboutToExitW =
{ 3,5,
{ "Instala�n� program dokon�il ��st instalace z prost�ed� MS-DOS.",
  "Nyn� budete muset restartovat po��ta�. Po nov�m spu�t�n�",
  "bude instalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,
  "Zajist�te, aby byl \"Spou�t�c� disk instalace syst�mu Windows XP\" ",
  "vlo�en do jednotky A: je�t� p�ed restartov�n�m.",
  DntEmptyString,
  "Stisknut�m ENTER se vr�t�te do syst�mu MS-DOS. Pak restartujte po��ta�,",
  "aby mohla instalace syst�mu Windows XP pokra�ovat.",
  NULL
}
},
DnsAboutToExitS =
{ 3,5,
{ "Instala�n� program dokon�il ��st instalace z prost�ed� MS-DOS.",
  "Nyn� budete muset restartovat po��ta�. Po nov�m spu�t�n�",
  "bude instalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,
  "Zajist�te, aby byl \"Spou�t�c� disk instalace syst�mu Windows XP\" ",
  "vlo�en do jednotky A: je�t� p�ed restartov�n�m.",
  DntEmptyString,
  "Stisknut�m ENTER se vr�t�te do syst�mu MS-DOS. Pak restartujte po��ta�,",
  "aby mohla instalace syst�mu Windows XP pokra�ovat.",
  NULL
}
},
DnsAboutToExitX =
{ 3,5,
{ "Instala�n� program dokon�il ��st instalace z prost�ed� MS-DOS.",
  "Nyn� budete muset restartovat po��ta�. Po nov�m spu�t�n�",
  "bude intstalace syst�mu Windows XP pokra�ovat.",
  DntEmptyString,
  "Pokud je v jednotce A: vlo�ena disketa, vyjm�te ji.",
  DntEmptyString,
  "Stisknut�m kl�vesy ENTER se vr�t�te do syst�mu MS-DOS. Pak restartujte",
  "po��ta�, aby mohla instalace syst�mu Windows XP pokra�ovat.",
  NULL
}
};

//
// Gas gauge
//

SCREEN
DnsGauge = { 7,15,
             { "����������������������������������������������������������������ͻ",
               "� Prob�h� kop�rov�n� soubor�:                                    �",
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
{ "Tento program vy�aduje ke spu�t�n� syst�m MS-DOS, verzi 5.0 nebo vy��.",
  NULL
}
},

DnsRequiresFloppy = { 3,5,
#ifdef ALLOW_525
{ "Instala�n� program zjistil, �e disketov� jednotka A: neexistuje, nebo",
  "podporuje pouze n�zkou hustotu. Ke spu�t�n� instalace je po�adov�na",
  "jednotka s kapacitou 1,2 MB nebo vy��.",
#else
{ "Instala�n� program zjistil, �e disketov� jednotka A: neexistuje, nebo",
  "se nejedn� o jednotku 3,5\" s vysokou hustotou. K proveden� instalace ",
  " s disketami je po�adov�na jednotka s kapacitou 1,44 MB nebo vy��.",
  DntEmptyString,
  "K instalaci syst�mu Windows XP bez pou�it� disket mus�te spustit tento",
  "program znovu a zadat na p��kazov� ��dce parametr /b.",
#endif
  NULL
}
},

DnsRequires486 = { 3,5,
{ "Instala�n� program zjistil, �e tento po��ta� neobsahuje procesor 80486",
  "nebo vy��. Na takov�m po��ta�i nelze syst�m Windows XP spustit.",
  NULL
}
},

DnsCantRunOnNt = { 3,5,
{ "Tento program nelze spustit v 32bitov� verzi syst�mu Windows.",
  DntEmptyString,
  "Pou�ijte program winnt32.exe.",
  NULL
}
},

DnsNotEnoughMemory = { 3,5,
{ "Instala�n� program zjistil, �e v po��ta�i nen� dost instalovan� pam�ti",
  "ke spu�t�n� syst�mu Windows XP.",
  DntEmptyString,
  "Po�adovan� pam؜: %lu%s MB",
  "Rozpoznan� pam؜: %lu%s MB",
  NULL
}
};


//
// Screens used when removing existing nt files
//
SCREEN
DnsConfirmRemoveNt = { 5,5,
{   "��d�te instala�n� program, aby odstranil soubory syst�mu Windows XP",
    "z uveden�ho adres��e. Instalace syst�mu Windows v uveden�m",
    "adres��i bude trvale zni�ena.",
    DntEmptyString,
    "%s",
    DntEmptyString,
    DntEmptyString,
    "  Stisknut�m F3 instalaci ukon��te, ani� by byl odstran�n jak�koli soubor.",
    "  Stisknut�m X soubory syst�mu Windows odstran�te z v��e uveden�ho adres��e.",
    NULL
}
},

DnsCantOpenLogFile = { 3,5,
{ "Nepoda�ilo se otev��t uveden� soubor s protokolem o instalaci.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "Ze zadan�ho adres��e nen� mo�n� odstranit soubory syst�mu Windows.",
  NULL
}
},

DnsLogFileCorrupt = { 3,5,
{ "V uveden�m souboru s protokolem o instalaci se nepoda�ilo naj�t",
  "sekci %s.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "Ze zadan�ho adres��e nen� mo�n� odstranit soubory syst�mu Windows.",
  NULL
}
},

DnsRemovingNtFiles = { 3,5,
{ "           Vy�kejte pros�m, prob�h� odstra�ov�n� soubor� syst�mu Windows.",
  NULL
}
};

SCREEN
DnsNtBootSect = { 3,5,
{ "Nepoda�ilo se nainstalovat spu�t�c� zavad؟ syst�mu Windows.",
  DntEmptyString,
  "Ujist�te se, �e jednotka C: je naform�tovan� a �e nen� po�kozen�.",
  NULL
}
};

SCREEN
DnsOpenReadScript = { 3,5,
{ "Soubor skriptu, kter� byl zad�n na p��kazov�m ��dku pomoc� p�ep�na�e /u,",
  "se nepoda�ilo otev��t.",
  DntEmptyString,
  "Bezobslu�n� instalace nem��e pokra�ovat.",
  NULL
}
};

SCREEN
DnsParseScriptFile = { 3,5,
{ "Soubor skriptu, kter� byl zad�n na p��kazov�m ��dku pomoc� p�ep�na�e /u,",
  DntEmptyString,

  "%s",
  DntEmptyString,
  "obsahuje chybu syntaxe na ��dku %u.",
  DntEmptyString,
  NULL
}
};

SCREEN
DnsBootMsgsTooLarge = { 3,5,
{ "Do�lo k vnit�n� chyb� instala�n�ho programu.",
  DntEmptyString,
  "P�elo�en� zpr�vy p�i zav�d�n� jsou p��li� dlouh�.",
  NULL
}
};

SCREEN
DnsNoSwapDrive = { 3,5,
{ "Do�lo k intern� chyb� instalace.",
  DntEmptyString,
  "Nepoda�ilo se naj�t m�sto pro ulo�en� str�nkovac�ho souboru.",
  NULL
}
};

SCREEN
DnsNoSmartdrv = { 3,5,
{ "V po��ta�i se nepoda�ilo naj�t program SmartDrive. Program SmartDrive",
  "v�razn� urychluje tuto ��st instalace syst�mu Windows.",
  DntEmptyString,
  "M�li byste ukon�it instalaci, spustit program SmartDrive a pot� instalaci",
  "znovu spustit.",
  "Podrobnosti o programu SmartDrive naleznete v dokumentaci k syst�mu MS-DOS.",
  DntEmptyString,
    "  Stisknut�m kl�vesy F3 instalaci ukon��te.",
    "  Stisknut�m kl�vesy ENTER m��ete pokra�ovat bez programu SmartDrive.",
  NULL
}
};

//
// Boot messages. These go in the fat and fat32 boot sectors.
//
CHAR BootMsgNtldrIsMissing[] = "NTLDR nenalezen";
CHAR BootMsgDiskError[] = "Chyba disku";
CHAR BootMsgPressKey[] = "Restartujte libovolnou kl�vesou";





