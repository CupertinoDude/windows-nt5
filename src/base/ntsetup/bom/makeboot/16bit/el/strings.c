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

unsigned int CODEPAGE = 737;

const char *EngStrings[] = {

"Windows XP SP1",
"����⫘ ���夞��� �����ᩫ���� �� Windows XP SP1",
"����⫘ �����ᩫ���� #2 �� Windows XP SP1",
"����⫘ �����ᩫ���� #3 �� Windows XP SP1",
"����⫘ �����ᩫ���� #4 �� Windows XP SP1",

"��� �夘� ������ � �稜�� ��� ����妬 %s\n",
"� ���矜�� ��㣞 ��� ������� ��� ��� ������ਫ਼ ��� �嫞���\n",
"�� %s ��� �夘� �� ����� �����⩠��� ����妬\n",
"****************************************************",

"���� �� ��暨���� ���������� ��� ����⫝̸� ���夞��� ��� �����ᩫ����",
"��� �� Microsoft %s.",
"��� �� ��������㩜�� ���� ��� ����⫝̸�, ��⧜� �� ⮜�� 6 ����,",
"��������⤜�, ����� ����櫞��� ����⫝̸�.",

"�������㩫� �� ��� ���� ��� ����⫝̸� ��� ���ᛘ %c:.  ���� � ����⫘",
"�� �夜� � %s.",

"�������㩫� ��� ᢢ� ����⫘ ��� ���ᛘ %c:.  ���� � ����⫘",
"�� �夜� � %s.",

"��⩫� ⤘ ��㡫�� 櫘� �����驜��.",

"�� ����⫝̸� ���夞��� �����ᩫ���� ��������㟞��� �� �������.",
"������ਫ਼",

"�������ᩫ��� ⤘ ᚤ੫� ��ᢣ� ���� ��� ���⢜�� ��� %s.",
"�����婫� �� ���ᛘ ����⫘� ��� ��� ��������� �� �����: ",
"�� ⚡��� ��ᣣ� ���ᛘ�\n",
"� ���ᛘ %c: ��� �夘� ���ᛘ ����⫘�\n",

"�⢜�� �� �������㩜�� �� ��������㩜�� ���� �� ����⫘ �ᢠ;",
"���㩫� Enter ��� �� �������㩜�� �ᢠ � Esc ��� ⥦��.",

"��ᢣ�: � ����⫘ ⮜� �������� �������\n",
"��ᢣ�: ꚤ੫� ���ᛘ ����⫘�\n",
"��ᢣ�: � ���ᛘ ��� �夘� ⫦���\n",
"��ᢣ�: ꚤ੫� ������\n",
"��ᢣ�: ��ᢣ� ������� (������⤦� CRC)\n",
"��ᢣ�: Bad request structure length\n",
"��ᢣ�: ��ᢣ� ����㫞���\n",
"��ᢣ�: ��� ��⟞�� � �秦� ������������� �⩦�\n",
"��ᢣ�: ��� ��⟞�� ���☪\n",
"��ᢣ�: ��ᢣ� �������\n",
"��ᢣ�: ������ �������\n",
"��ᢣ�: �� ⚡��� �嫞�� � ������⤞ ������\n",
"��ᢣ�: ��� ��⟞�� ⤛���� ���矬����\n",
"��ᢣ�: ��ᢣ� ������� ����⫘�\n",
"��ᢣ�: ��⨙��� ꣜��� �����⢘��� ��㣞�(DMA)\n",
"��ᢣ�: ��ᢣ� ��ᚤਫ਼� �������(CRC � ECC)\n",
"��ᢣ�: ������� �������\n",
"��ᢣ�: �� ������� 樠� ��� �婡�� ⢞�� � ��⫬�� �� ���������\n",

"����⫘ �����ᩫ���� #5 �� Windows XP SP1",
"����⫘ �����ᩫ���� #6 �� Windows XP SP1"
};

const char *LocStrings[] = {"\0"};



