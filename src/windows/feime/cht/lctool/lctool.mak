# Microsoft Visual C++ Generated NMAKE File, Format Version 40000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MyLCTOOL - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MyLCTOOL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MyLCTOOL - Win32 Release" && "$(CFG)" !=\
 "MyLCTOOL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyLCTOOL.mak" CFG="MyLCTOOL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyLCTOOL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MyLCTOOL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "MyLCTOOL - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir Release
# PROP BASE Intermediate_Dir Release
# PROP BASE Target_Dir
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir Release
# PROP Intermediate_Dir Release
# PROP Target_Dir
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\MyLCTOOL.exe"

CLEAN : 
	-@erase ".\Release\MyLCTOOL.exe"
	-@erase ".\Release\LCTOOL.res"
	-@erase ".\Release\LCTOOL.OBJ"
	-@erase ".\Release\LCPRINT.OBJ"
	-@erase ".\Release\LCFUNC.OBJ"
	-@erase ".\Release\LCFILE.OBJ"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/MyLCTOOL.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG"
RSC_PROJ=/l 0x404 /fo"$(INTDIR)/LCTOOL.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MyLCTOOL.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/MyLCTOOL.pdb" /machine:I386 /out:"$(OUTDIR)/MyLCTOOL.exe" 
LINK32_OBJS= \
	"$(INTDIR)/LCTOOL.OBJ" \
	"$(INTDIR)/LCPRINT.OBJ" \
	"$(INTDIR)/LCFUNC.OBJ" \
	"$(INTDIR)/LCFILE.OBJ" \
	"$(INTDIR)/LCTOOL.res"

"$(OUTDIR)\MyLCTOOL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir Debug
# PROP BASE Intermediate_Dir Debug
# PROP BASE Target_Dir
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir Debug
# PROP Intermediate_Dir Debug
# PROP Target_Dir
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\MyLCTOOL.exe" "$(OUTDIR)\MyLCTOOL.bsc"

CLEAN : 
	-@erase ".\Debug\MyLCTOOL.exe"
	-@erase ".\Debug\LCTOOL.res"
	-@erase ".\Debug\LCTOOL.OBJ"
	-@erase ".\Debug\LCPRINT.OBJ"
	-@erase ".\Debug\LCFUNC.OBJ"
	-@erase ".\Debug\LCFILE.OBJ"
	-@erase ".\Debug\MyLCTOOL.ilk"
	-@erase ".\Debug\MyLCTOOL.pdb"
	-@erase ".\Debug\MyLCTOOL.bsc"
	-@erase ".\Debug\LCTOOL.SBR"
	-@erase ".\Debug\LCPRINT.SBR"
	-@erase ".\Debug\LCFUNC.SBR"
	-@erase ".\Debug\LCFILE.SBR"
	-@erase ".\Debug\vc40.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/MyLCTOOL.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
RSC_PROJ=/l 0x404 /fo"$(INTDIR)/LCTOOL.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MyLCTOOL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/LCTOOL.SBR" \
	"$(INTDIR)/LCPRINT.SBR" \
	"$(INTDIR)/LCFUNC.SBR" \
	"$(INTDIR)/LCFILE.SBR"

"$(OUTDIR)\MyLCTOOL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/MyLCTOOL.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MyLCTOOL.exe" 
LINK32_OBJS= \
	"$(INTDIR)/LCTOOL.OBJ" \
	"$(INTDIR)/LCPRINT.OBJ" \
	"$(INTDIR)/LCFUNC.OBJ" \
	"$(INTDIR)/LCFILE.OBJ" \
	"$(INTDIR)/LCTOOL.res"

"$(OUTDIR)\MyLCTOOL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "MyLCTOOL - Win32 Release"
# Name "MyLCTOOL - Win32 Debug"

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"

!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\LCTOOL.RC
DEP_RSC_LCTOO=\
	".\LCTOOL.ICO"\
	".\rc.h"\
	

"$(INTDIR)\LCTOOL.res" : $(SOURCE) $(DEP_RSC_LCTOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LCTOOL.H

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"

!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LCTOOL.C
DEP_CPP_LCTOOL=\
	".\LCTOOL.H"\
	".\rc.h"\
	

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"


"$(INTDIR)\LCTOOL.OBJ" : $(SOURCE) $(DEP_CPP_LCTOOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"


"$(INTDIR)\LCTOOL.OBJ" : $(SOURCE) $(DEP_CPP_LCTOOL) "$(INTDIR)"

"$(INTDIR)\LCTOOL.SBR" : $(SOURCE) $(DEP_CPP_LCTOOL) "$(INTDIR)"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_LCTOOL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LCPRINT.C

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"

DEP_CPP_LCPRI=\
	".\LCTOOL.H"\
	".\rc.h"\
	
NODEP_CPP_LCPRI=\
	".\��"\
	".\WORD_POS"\
	

"$(INTDIR)\LCPRINT.OBJ" : $(SOURCE) $(DEP_CPP_LCPRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"

DEP_CPP_LCPRI=\
	".\LCTOOL.H"\
	".\rc.h"\
	

"$(INTDIR)\LCPRINT.OBJ" : $(SOURCE) $(DEP_CPP_LCPRI) "$(INTDIR)"

"$(INTDIR)\LCPRINT.SBR" : $(SOURCE) $(DEP_CPP_LCPRI) "$(INTDIR)"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_LCPRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LCFUNC.C
DEP_CPP_LCFUN=\
	".\LCTOOL.H"\
	".\rc.h"\
	

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"


"$(INTDIR)\LCFUNC.OBJ" : $(SOURCE) $(DEP_CPP_LCFUN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"


"$(INTDIR)\LCFUNC.OBJ" : $(SOURCE) $(DEP_CPP_LCFUN) "$(INTDIR)"

"$(INTDIR)\LCFUNC.SBR" : $(SOURCE) $(DEP_CPP_LCFUN) "$(INTDIR)"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_LCFUN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LCFILE.C
DEP_CPP_LCFIL=\
	".\LCTOOL.H"\
	".\rc.h"\
	

!IF  "$(CFG)" == "MyLCTOOL - Win32 Release"


"$(INTDIR)\LCFILE.OBJ" : $(SOURCE) $(DEP_CPP_LCFIL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MyLCTOOL - Win32 Debug"


"$(INTDIR)\LCFILE.OBJ" : $(SOURCE) $(DEP_CPP_LCFIL) "$(INTDIR)"

"$(INTDIR)\LCFILE.SBR" : $(SOURCE) $(DEP_CPP_LCFIL) "$(INTDIR)"

"$(INTDIR)\vc40.pdb" : $(SOURCE) $(DEP_CPP_LCFIL) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
