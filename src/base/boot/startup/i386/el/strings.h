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
"�� Windows NT ���槠��� �椦 %dK ��� ��㣞� low.  512k ��� ��㣞� low\n"  \
"������礫�� ��� ��� ���⢜�� �� Windows NT.  ��� ����᝜��� ��\n"       \
"�������婜�� ��� ���������� ��� � �� ���⢜���� ⤘ ��暨���� �矣����\n" \
"�����⫨� ��� ��� �柞�� ��� ��� ������������\n"

#define SU_NO_EXTENDED_MEMORY \
"�� Windows NT ��� ���槠��� ������ ��㣞 extended.  7Mb ��� ��㣞� extended\n"       \
"������礫�� ��� ��� ���⢜�� �� Windows NT.  ��� ����᝜��� �� \n"                \
"�������婜�� ��� ���������� ��� � �� ���⢜���� ⤘ ��暨���� �矣����\n"           \
"�����⫨� ��� ��� �柞�� ��� ��� ������������\n"                                   \
"\n\n�����椠�� ��㣞�:\n"

#define SU_NTLDR_CORRUPT \
"�� NTLDR �夘� ����������⤦.  �� �穫��� ��� ������ �� �����㩜�."

#define PG_FAULT_MSG    " =================== ������ ������� ============================= \n\n"
#define DBL_FAULT_MSG   " ==================== ����� ������ ============================== \n\n"
#define GP_FAULT_MSG    " ============== ������ ������ ���������� ======================== \n\n"
#define STK_OVERRUN_MSG " ===== �������� �������� ������� � ������ �� ���������=========== \n\n"
#define EX_FAULT_MSG    " ===================== �������� ================================= \n\n"
#define DEBUG_EXCEPTION "\N������� ��� �� ��������� ������������� ��������� "
#define PG_FAULT_ADDRESS "** ��� �������� ���矬��� %lx\n"
