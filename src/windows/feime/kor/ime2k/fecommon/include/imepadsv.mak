# Microsoft Developer Studio Generated NMAKE File, Based on imepadsv.dsp
!IF "$(CFG)" == ""
CFG=imepadsv - Win32 Release
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� imepadsv - Win32 Release ��ݒ肵�܂��B
!ENDIF 

!IF "$(CFG)" != "imepadsv - Win32 Release"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "imepadsv.mak" CFG="imepadsv - Win32 Release"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "imepadsv - Win32 Release" ("Win32 (x86) Generic Project" �p)
!MESSAGE 
!ERROR �����ȍ\�����w�肳��Ă��܂��B
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\padproxy.c" ".\padguids.c" ".\imepadsv.h" ".\dlldata.c" 


CLEAN :
	-@erase 
	-@erase "dlldata.c"
	-@erase "imepadsv.h"
	-@erase "padguids.c"
	-@erase "padproxy.c"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
MTL_PROJ=

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("imepadsv.dep")
!INCLUDE "imepadsv.dep"
!ELSE 
!MESSAGE Warning: cannot find "imepadsv.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "imepadsv - Win32 Release"
SOURCE=.\imepadsv.idl
InputPath=.\imepadsv.idl

".\imepadsv.h"	".\dlldata.c"	".\padguids.c"	".\padproxy.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /h imepadsv.h /iid padguids.c /proxy padproxy.c imepadsv.idl
<< 
	

!ENDIF 

