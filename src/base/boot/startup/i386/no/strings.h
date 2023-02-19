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
"Windows NT fant bare %d kB lavminne.  Windows NT krever\n"     \
"512 kB lavminne for � kj�re.  Du m� kanskje oppgradere\n"          \
"datamaskinen eller kj�re et konfigurasjonsprogram fra leverand�ren.\n"

#define SU_NO_EXTENDED_MEMORY \
"Ikke nok utvidet minne til � starte Windows NT.  Windows NT krever 7 MB\n"        \
"utvidet minne for � starte.  Du m� kanskje oppgradere datamaskinen eller kj�re\n"       \
"et konfigurasjonsprogram fra en maskinvareleverand�r.\n"   \
"\n\nMinnelisting:\n"

#define SU_NTLDR_CORRUPT \
"NTLDR er skadet.  Datamaskinen kan ikke startes opp."

#define PG_FAULT_MSG    " ========================= SIDEFEIL ======================== \n\n"
#define DBL_FAULT_MSG   " ======================== DOBBELTFEIL ====================== \n\n"
#define GP_FAULT_MSG    " ================ GENERELL BESKYTTELSESFEIL ================ \n\n"
#define STK_OVERRUN_MSG " ======= OVERFYLT I STAKKSEGMENT eller IKKE TILSTEDE ======= \n\n"
#define EX_FAULT_MSG    " ========================== UNNTAK ========================= \n\n"
#define DEBUG_EXCEPTION "\nFEILS�KINGSUNNTAK "
#define PG_FAULT_ADDRESS "** ved line�r addresse %lx\n"

