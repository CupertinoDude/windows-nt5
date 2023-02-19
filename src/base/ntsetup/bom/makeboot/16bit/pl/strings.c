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
"Dysk rozruchowy Instalatora dodatku SP1 systemu Windows XP",
"Dodatek SP1 systemu Windows XP - dysk instalacyjny nr 2",
"Dodatek SP1 systemu Windows XP - dysk instalacyjny nr 3",
"Dodatek SP1 systemu Windows XP - dysk instalacyjny nr 4",

"Nie mo�na znale�� pliku %s\n",
"Za ma�o wolnej pami�ci do wykonania ��dania\n",
"%s nie ma formatu pliku wykonywalnego\n",
"****************************************************",

"Ten program tworzy dyskietki rozruchowe Instalatora",
"dla systemu Microsoft %s.",
"Aby utworzy� te dyskietki, potrzebnych jest 6 pustych,",
"sformatowanych dyskietek du�ej g�sto�ci.",

"W��� jedn� z tych dyskietek do stacji dysk�w %c:. B�dzie to",
"%s.",

"W��� kolejn� dyskietk� do stacji dysk�w %c:. B�dzie to",
"%s.",

"Naci�nij dowolny klawisz, gdy zechcesz kontynuowa�.",

"Dyskietki rozruchowe Instalatora zosta�y utworzone pomy�lnie.",
"zako�czono",

"Podczas pr�by wykonania %s wyst�pi� nieznany b��d.",
"Okre�l stacj� dyskietek, do kt�rej maj� by� skopiowane obrazy: ",
"Nieprawid�owa litera stacji dysk�w\n",
"Stacja dysk�w %c: nie jest stacj� dyskietek\n",

"Czy chcesz ponownie spr�bowa� utworzy� t� dyskietk�?",
"Naci�nij klawisz Enter, aby ponowi� pr�b�, lub klawisz Esc, aby zako�czy�.",

"B��d: dysk jest zabezpieczony przed zapisem\n",
"B��d: nieznana jednostka dyskowa\n",
"B��d: stacja dysk�w nie jest gotowa\n",
"B��d: nieznane polecenie\n",
"B��d: b��d danych (z�a suma kontrolna CRC)\n",
"B��d: z�a d�ugo�� struktury ��dania\n",
"B��d: b��d wyszukiwania\n",
"B��d: nie znaleziono typu no�nika\n",
"B��d: nie znaleziono sektora\n",
"B��d: niepowodzenie zapisu\n",
"B��d: b��d og�lny\n",
"B��d: nieprawid�owe ��danie lub z�e polecenie\n",
"B��d: nie znaleziono znacznika adresu\n",
"B��d: niepowodzenie zapisu na dysku\n",
"B��d: przepe�nienie podczas bezpo�redniego dost�pu do pami�ci (DMA)\n",
"B��d: b��d odczytu danych (suma kontrolna CRC lub ECC)\n",
"B��d: b��d kontrolera\n",
"B��d: up�yn�� limit czasu dysku lub dysk nie odpowiada\n",

"Dodatek SP1 systemu Windows XP - dysk instalacyjny nr 5",
"Dodatek SP1 systemu Windows XP - dysk instalacyjny nr 6"
};

const char *LocStrings[] = {"\0"};



