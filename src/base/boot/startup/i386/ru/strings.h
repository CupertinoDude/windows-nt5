/*++

Copyright (c) 1992  Microsoft Corporation

Module Name:

    strings.h

Abstract:

    Contains all localizable strings for startup.com

Author:

    John Vert (jvert) 4-Jan-1994

Revision History:

    John Vert (jvert) 4-Jan-1994
        created

--*/

#define SU_NO_LOW_MEMORY \
"Windows NT �����㦨�� ⮫쪮 %d �� ���筮� �����. \n"                  \
"��� ����᪠ Windows NT �ॡ���� 512 �� ���筮� �����. \n"             \
"����� ���ॡ������� ���������� �⮣� �������� ��� ����� \n"          \
"�ணࠬ�� ����ன��, ���⠢�塞�� ����⮢�⥫��. \n"

#define SU_NO_EXTENDED_MEMORY \
"Windows NT �� �����㦨�� �����筮�� ��ꥬ� �������⥫쭮� �����. \n"  \
"��� ����᪠ Windows NT ����室��� 7 �� �������⥫쭮� �����. \n"        \
"����� ���ॡ������� ���������� �⮣� �������� ��� ����� \n"           \
"�ணࠬ�� ����ன��, ���⠢�塞�� ����⮢�⥫��. \n"                     \
"\n\n�ᯮ�짮����� �����:\n"

#define SU_NTLDR_CORRUPT \
"NTLDR �ᯮ�祭.  ���������� ����㧨�� ��⥬�."

#define PG_FAULT_MSG    " =================== ������ �������� ============================ \n\n"
#define DBL_FAULT_MSG   " ==================== ������� ������ ============================ \n\n"
#define GP_FAULT_MSG    " ================= ����� ������ ������ ========================== \n\n"
#define STK_OVERRUN_MSG " ===== ������������ ����� �������� ��� ������ ���������� ======== \n\n"
#define EX_FAULT_MSG    " ===================== ���������� =============================== \n\n"
#define DEBUG_EXCEPTION "\n����誠 DEBUG "
#define PG_FAULT_ADDRESS "** �� ��������� ����� %lx\n"
