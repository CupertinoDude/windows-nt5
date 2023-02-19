# Copyright (c) 1997-2001 Microsoft Corporation, All Rights Reserved
#---------------------------------------------------------------------
#
# This makefile is for use with the SMSBUILD utility.  It builds the
# snmpthrd.dll
#
# created 01-24-97  stevm
#
#---------------------------------------------------------------------

TARGET=snmpcl.dll

NO_OPTIM=1

ICECAP=1

CFLAGS=$(CFLAGS) /GX /GR

CDEFS = $(CDEFS) \
		/D_UNICODE /DUNICODE /DSNMPCLINIT

CINC= \
	$(CINC) \
	-I.\include \
	-I$(DEFDRIVE)$(DEFDIR)\snmpprovider\common\thrdlog\include \
	-I$(DEFDRIVE)$(DEFDIR)\snmpprovider\common\snmpmfc\include \
	-I$(DEFDRIVE)$(DEFDIR)\snmpprovider\winsnmp\nt5.0\inc \
	-I$(DEFDRIVE)$(DEFDIR)\idl

release=core\$(RELDIR)

CPPFILES=\
	address.cpp \
	encdec.cpp \
	dummy.cpp \
	flow.cpp \
	frame.cpp \
	fs_reg.cpp \
	idmap.cpp \
	message.cpp \
	op.cpp \
	ophelp.cpp \
	opreg.cpp \
	pdu.cpp \
	pseudo.cpp \
	reg.cpp \
	sec.cpp \
	session.cpp \
	ssent.cpp \
	startup.cpp \
	timer.cpp \
	transp.cpp \
	tsent.cpp \
	tsess.cpp \
	value.cpp \
	vbl.cpp \
	vblist.cpp \
	window.cpp \
	wsess.cpp \
	trap.cpp \
	trapsess.cpp \
	snmpcont.cpp \
	snmpauto.cpp \
	snmptype.cpp \
	tree.cpp


DEFFILE=snmpcl.def

LIBS=\
	$(DEFDRIVE)$(DEFDIR)\snmpprovider\common\snmpmfc\OBJ$(PLAT)N$(BLDT)D\snmpmfc.lib \
	$(DEFDRIVE)$(DEFDIR)\snmpprovider\common\thrdlog\OBJ$(PLAT)N$(BLDT)D\snmpthrd.lib \
	$(DEFDRIVE)$(DEFDIR)\snmpprovider\winsnmp\nt5.0\dll\OBJ$(PLAT)N$(BLDT)D\wsnmp32.lib \
	$(IDL)\OBJ$(PLAT)$(OPST)$(BLDT)D\wbemuuid.lib \
	$(CLIB)\ole32.lib \
	$(CLIB)\oleaut32.lib \
	$(CLIB)\msvcirt.lib \
	$(CLIB)\kernel32.lib \
	$(CLIB)\msvcrt.lib \
	$(CLIB)\advapi32.lib \
	$(CLIB)\user32.lib \
	$(CLIB)\wsock32.lib
