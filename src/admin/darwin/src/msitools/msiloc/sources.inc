
TARGETNAME=MsiLoc
!INCLUDE ..\..\..\MsiMake.inc

TARGETTYPE=PROGRAM
SUBSYSTEM_VERSION=4.0
SUBSYSTEM_WINVER=4.0

UMTYPE=console
UMENTRY=$(MSI_ENTRY)

USE_LIBCMT=1
USE_NATIVE_EH=1

TARGETPATH=$(MSI_BUILD_TARGET)
TARGETLIBS=\
	$(SDK_LIB_PATH)\msi.lib \
	$(SDK_LIB_PATH)\ole32.lib \
	$(SDK_LIB_PATH)\user32.lib

C_DEFINES=$(C_DEFINES) -D_EXE

INCLUDES=$(INCLUDES);$(INC_DIR);$(RES_OBJDIR);$(BUILD_COMMONDIR);

SOURCES=..\msiloc.cpp \
		..\msiloc.rc