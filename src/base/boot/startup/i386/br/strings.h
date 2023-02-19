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
"O Windows NT encontrou apenas %dKB de mem�ria convencional. S�o necess�rios\n"  \
"512KB de mem�ria convencional para executar o Windows NT. Voc� talvez tenha\n"  \
"que atualizar seu computador ou executar o programa de configura��o\n"          \
"fornecido pelo fabricante.\n"

#define SU_NO_EXTENDED_MEMORY \
"O Windows NT n�o encontrou mem�ria estendida suficiente. S�o necess�rios\n"     \
"7MB de mem�ria estendida para executar o Windows NT. Voc� talvez tenha\n"       \
"que atualizar seu computador ou executar o programa de configura��o\n"          \
"fornecido pelo fabricante.\n"                                                   \
"\n\nMapa da mem�ria:\n"

#define SU_NTLDR_CORRUPT \
"NTLDR est� corrompido. O sistema n�o pode ser reiniciado."


#define PG_FAULT_MSG    " ======================== ERRO DE P�GINA ======================== \n\n"
#define DBL_FAULT_MSG   " ========================= FALHA DUPLA ========================== \n\n"
#define GP_FAULT_MSG    " ==================== FALHA DE PROTE��O GERAL =================== \n\n"
#define STK_OVERRUN_MSG " === SEGMENTO DA PILHA AUSENTE OU FALHA DE SATURA��O DO MESMO === \n\n"
#define EX_FAULT_MSG    " =========================== EXCE��O ============================ \n\n"
#define DEBUG_EXCEPTION "\nDEBUG TRAP "
#define PG_FAULT_ADDRESS "** No endere�o linear %lx\n"
