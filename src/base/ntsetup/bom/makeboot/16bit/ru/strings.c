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

unsigned int CODEPAGE = 866;

const char *EngStrings[] = {

"Windows XP",
"��⠭����� ��� 1 Windows XP SP1",
"��⠭����� ��� 2 Windows XP SP1",
"��⠭����� ��� 3 Windows XP SP1",
"��⠭����� ��� 4 Windows XP SP1",

"�� ������ 䠩� %s\n",
"�������筮 ����� ��� �믮������ �����\n",
"%s �� ���� �ᯮ��塞� 䠩���\n",
"****************************************************",

"�� �ணࠬ�� ᮧ���� ����㧮�� ��᪨ ��⠭����",
"��� Microsoft %s.",
"�⮡� ᮧ���� �� ��᪨, �ॡ���� 6 �����",
"���ଠ�஢����� ��᪮� ��᮪�� ���⭮��.",

"��⠢�� ���� �� ��� � ��᪮��� %c:.  ��� ���",
"�㤥� ���뢠���� %s.",

"��⠢�� ��㣮� ��� � ��᪮��� %c:.  ��� ���",
"�㤥� ���뢠���� %s.",

"������ ���� ������� ��� �த�������.",

"����㧮�� ��᪨ ��⠭���� �ᯥ譮 ᮧ����.",
"�����襭�",

"�ந��諠 �訡�� �� ����⪥ �믮������ %s.",
"������ ��᪮���, �� ����� ᫥��� ����஢��� ��ࠧ�: ",
"�������⨬�� �㪢� ��᪠\n",
"���ன�⢮ %c: �� ���� ��᪮����� ������ ��᪮�\n",

"���� ������� ������ ᮧ����� �⮣� ��᪠?",
"������ <����> ��� ����७�� ��� <Esc> ��� ��室� �� �ணࠬ��.",

"�訡��: ��� ���饭 �� �����\n",
"�訡��: �������⭮� ��᪮��� ���ன�⢮\n",
"�訡��: ���ன�⢮ �� ��⮢�\n",
"�訡��: �������⭠� �������\n",
"�訡��: �訡�� � ������ (���ࠢ���� CRC)\n",
"�訡��: ���ࠢ��쭠� ����� �������� �����\n",
"�訡��: �訡�� ���᪠\n",
"�訡��: ⨯ ���⥫� �� ������\n",
"�訡��: ᥪ�� �� ������\n",
"�訡��: �訡�� �����\n",
"�訡��: ���� �訡��\n",
"�訡��: ���ࠢ���� ����� ��� �������\n",
"�訡��: �� ������ ����� ��થ�\n",
"�訡��: �訡�� ����� �� ���\n",
"�訡��: ��९������� DMA\n",
"�訡��: �訡�� �⥭�� ������ (CRC ��� ECC)\n",
"�訡��: ᡮ� ����஫���\n",
"�訡��: �ॢ�襭� �६� �������� ��� ��� �⢥� �� ��᪠\n",

"��⠭����� ��� 5 Windows XP SP1",
"��⠭����� ��� 6 Windows XP SP1"
};

const char *LocStrings[] = {"\0"};



