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
"Syst�m Windows NT na�el pouze %d kB doln� pam�ti. Ke spu�t�n� syst�mu\n"   \
"je pot�eba 512 kB �i v�ce doln� pam�ti. Asi budete muset inovovat dan�\n"  \
"po��ta�, nebo spustit konfigura�n� program, kter� byl dod�n v�robcem.\n"

#define SU_NO_EXTENDED_MEMORY \
"Syst�m Windows NT nena�el dost roz��en� pam�ti. Syst�m Windows NT\n"     \
"pot�ebuje ke spu�t�n� 7 MB roz��en� pam�ti. Asi budete muset inovovat\n"  \
"po��ta�, nebo spustit konfigura�n� program, kter� byl dod�n v�robcem.\n"   \
"\n\nMapa pam�ti:\n"

#define SU_NTLDR_CORRUPT \
"NTLDR je po�kozen. Syst�m nelze nastartovat."

#define PG_FAULT_MSG    " ================== CHYBA STR�NKY =============================== \n\n"
#define DBL_FAULT_MSG   " ==================== DVOJCHYBA ================================= \n\n"
#define GP_FAULT_MSG    " ============== V�EOBECN� CHYBA OCHRANY ========================= \n\n"
#define STK_OVERRUN_MSG " ====== CHYBA: SEGMENT Z�SOBN�KU P�EKRYT �i CHYB� =============== \n\n"
#define EX_FAULT_MSG    " ====================== VYJ�MKA ================================= \n\n"
#define DEBUG_EXCEPTION  "\nPAST DEBUG "
#define PG_FAULT_ADDRESS "** Na line�rn� adrese %lx\n"

