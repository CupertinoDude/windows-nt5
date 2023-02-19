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
"System Windows NT wykry� tylko %d KB pami�ci niskiej. Do uruchomienia\n"  \
"systemu Windows NT wymagane jest 512 KB. Musisz rozbudowa� komputer\n"      \
"lub uruchomi� program konfiguracyjny dostarczony przez producenta.\n"

#define SU_NO_EXTENDED_MEMORY \
"System Windows NT nie znalaz� wystarczaj�cej ilo�ci pami�ci typu Extended.\n"       \
"Do uruchomienia systemu Windows NT wymagane jest 7 MB pami�ci Extended.\n"     \
"Musisz rozbudowa� komputer lub uruchomi� program konfiguracyjny dostarczony\n"   \
"przez producenta.\n\nMapa pami�ci:\n"

#define SU_NTLDR_CORRUPT \
"Plik NTLDR jest uszkodzony. System nie zostanie uruchomiony."

#define PG_FAULT_MSG    " =============== B��D STRONY == (PAGE FAULT) ==================== \n\n"
#define DBL_FAULT_MSG   " =========== B��D PODW�JNY == (DOUBLE FAULT) ==================== \n\n"
#define GP_FAULT_MSG    " ======== OG�LNY B��D OCHRONY == (GENERAL PROTECTION FAULT) ===== \n\n"
#define STK_OVERRUN_MSG " ====== B��D PRZEPE�NIENIA SEGMENTU STOSU lub NIEOBECNO�CI ====== \n\n"
#define EX_FAULT_MSG    " =================== WYJ�TEK == (EXCEPTION) ===================== \n\n"
#define DEBUG_EXCEPTION "\nPU�APKA DEBUGOWANIA "
#define PG_FAULT_ADDRESS "** Pod adresem liniowym %lx\n"
