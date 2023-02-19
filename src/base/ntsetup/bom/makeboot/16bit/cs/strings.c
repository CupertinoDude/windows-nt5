//----------------------------------------------------------------------------
//
// Copyright (c) 1999  Microsoft Corporation
// All rights reserved.
//
// File Name:
//      strings.c
//
// Description:
//      Contains all of the strings constants for DOS based MAKEBOOT program.
//
//      To localize this file for a new language do the following:
//           - change the unsigned int CODEPAGE variable to the code page
//             of the language you are translating to
//           - translate the strings in the EngStrings array into the
//             LocStrings array.  Be very careful that the 1st string in the
//             EngStrings array corresponds to the 1st string in the LocStrings
//             array, the 2nd corresponds to the 2nd, etc...
//
//----------------------------------------------------------------------------

//
//  NOTE: To add more strings to this file, you need to:
//          - add the new #define descriptive constant to the makeboot.h file
//          - add the new string to the English language array and then make
//            sure localizers add the string to the Localized arrays
//          - the #define constant must match the string's index in the array
//

#include <stdlib.h>

unsigned int CODEPAGE = 852;

const char *EngStrings[] = {

"Windows XP SP1",
"Spou�t�c� instala�n� disketa syst�mu Windows XP SP1",
"Instala�n� disketa �. 2 syst�mu Windows XP SP1",
"Instala�n� disketa �. 3 syst�mu Windows XP SP1",
"Instala�n� disketa �. 4 syst�mu Windows XP SP1",

"Nepoda�ilo se nal�zt soubor %s.\n",
"Pro dokon�en� po�adavku nen� dostatek pam�ti.\n",
"%s nen� spustiteln� soubor.\n",
"****************************************************",

"Tento program vytvo�� spou�t�c� instala�n� diskety",
"pro syst�m Microsoft %s.",
"K vytvo�en� t�chto disket budete pot�ebovat �est pr�zdn�ch,",
"naform�tovan�ch disket s vysokou hustotou z�znamu.",

"Vlo�te jednu z disket do jednotky %c:. Pojmenujte disketu",
"%s.",

"Vlo�te dal� disketu do jednotky %c:. Pojmenujte disketu",
"%s.",

"Pot� stiskn�te libovolnou kl�vesu.",

"Spou�t�c� instala�n� diskety byly �sp��n� vytvo�eny.",
"Dokon�eno",

"P�i pokusu spustit %s do�lo k nezn�m� chyb�.",
"Zadejte c�lovou disketovou jednotku pro zkop�rov�n� bitov�ch kopi�: ",
"P�smeno jednotky je neplatn�.\n",
"Jednotka %c: nen� disketov� jednotka.\n",

"Chcete se pokusit vytvo�it disketu znovu?",
"Pokra�ujte stisknut�m kl�vesy Enter, nebo kl�vesou Esc program ukon�ete.",

"Chyba: Disk je chr�n�n proti z�pisu.\n",
"Chyba: Nezn�m� diskov� jednotka.\n",
"Chyba: Jednotka nen� p�ipravena.\n",
"Chyba: Nezn�m� p��kaz.\n",
"Chyba: Chyba dat (chybn� kontroln� sou�et CRC).\n",
"Chyba: Chybn� d�lka ��dosti.\n",
"Chyba: Chyba vystaven�.\n",
"Chyba: Typ m�dia nebyl nalezen.\n",
"Chyba: Sektor nebyl nalezen.\n",
"Chyba: Chyba z�pisu.\n",
"Chyba: Obecn� chyba.\n",
"Chyba: Neplatn� ��dost nebo chybn� p��kaz.\n",
"Chyba: Adresn� zna�ka nebyla nalezena.\n",
"Chyba: Chyba z�pisu na disk.\n",
"Chyba: Do�lo k p�eb�hu DMA (Direct Memory Access).\n",
"Chyba: Chyba �ten� dat (chybn� CRC nebo ECC).\n",
"Chyba: Chyba �adi�e.\n",
"Chyba: �asov� limit diskov� operace vypr�el nebo disk neodpov�d�l.\n",
"Instala�n� disketa �. 5 syst�mu Windows XP SP1",
"Instala�n� disketa �. 6 syst�mu Windows XP SP1"

};
const char *LocStrings[] = {"\0"};
