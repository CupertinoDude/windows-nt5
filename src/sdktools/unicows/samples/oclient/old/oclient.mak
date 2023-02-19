# Microsoft Developer Studio Generated NMAKE File, Based on oclient.dsp
!IF "$(CFG)" == ""
CFG=OClient - Win32 Release
!MESSAGE No configuration specified. Defaulting to OClient - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "OClient - Win32 Release" && "$(CFG)" != "OClient - Win32 Debug" && "$(CFG)" != "OClient - Win32 Unicode Debug" && "$(CFG)" != "OClient - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "oclient.mak" CFG="OClient - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "OClient - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "OClient - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OClient - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

ALL : ".\.\Release\oclient.HLP" "$(OUTDIR)\oclient.exe"


CLEAN :
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\maindoc.obj"
	-@erase "$(INTDIR)\mainview.obj"
	-@erase "$(INTDIR)\oclient.obj"
	-@erase "$(INTDIR)\oclient.pch"
	-@erase "$(INTDIR)\oclient.res"
	-@erase "$(INTDIR)\rectitem.obj"
	-@erase "$(INTDIR)\splitfra.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\oclient.exe"
	-@erase ".\.\Release\oclient.HLP"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\oclient.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\oclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\oclient.pdb" /machine:I386 /out:"$(OUTDIR)\oclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\maindoc.obj" \
	"$(INTDIR)\mainview.obj" \
	"$(INTDIR)\oclient.obj" \
	"$(INTDIR)\rectitem.obj" \
	"$(INTDIR)\splitfra.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\oclient.res"

"$(OUTDIR)\oclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "OClient - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

ALL : ".\.\Debug\oclient.HLP" "$(OUTDIR)\oclient.exe"


CLEAN :
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\maindoc.obj"
	-@erase "$(INTDIR)\mainview.obj"
	-@erase "$(INTDIR)\oclient.obj"
	-@erase "$(INTDIR)\oclient.pch"
	-@erase "$(INTDIR)\oclient.res"
	-@erase "$(INTDIR)\rectitem.obj"
	-@erase "$(INTDIR)\splitfra.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\oclient.exe"
	-@erase "$(OUTDIR)\oclient.ilk"
	-@erase "$(OUTDIR)\oclient.pdb"
	-@erase ".\.\Debug\oclient.HLP"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\oclient.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\oclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\oclient.pdb" /debug /machine:I386 /out:"$(OUTDIR)\oclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\maindoc.obj" \
	"$(INTDIR)\mainview.obj" \
	"$(INTDIR)\oclient.obj" \
	"$(INTDIR)\rectitem.obj" \
	"$(INTDIR)\splitfra.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\oclient.res"

"$(OUTDIR)\oclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Debug"

OUTDIR=.\UniDebug
INTDIR=.\UniDebug
# Begin Custom Macros
OutDir=.\.\UniDebug
# End Custom Macros

ALL : ".\.\UniDebug\oclient.HLP" "$(OUTDIR)\oclient.exe"


CLEAN :
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\maindoc.obj"
	-@erase "$(INTDIR)\mainview.obj"
	-@erase "$(INTDIR)\oclient.obj"
	-@erase "$(INTDIR)\oclient.pch"
	-@erase "$(INTDIR)\oclient.res"
	-@erase "$(INTDIR)\rectitem.obj"
	-@erase "$(INTDIR)\splitfra.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\oclient.exe"
	-@erase "$(OUTDIR)\oclient.ilk"
	-@erase "$(OUTDIR)\oclient.pdb"
	-@erase ".\.\UniDebug\oclient.HLP"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\oclient.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\oclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\oclient.pdb" /debug /machine:I386 /out:"$(OUTDIR)\oclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\maindoc.obj" \
	"$(INTDIR)\mainview.obj" \
	"$(INTDIR)\oclient.obj" \
	"$(INTDIR)\rectitem.obj" \
	"$(INTDIR)\splitfra.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\oclient.res"

"$(OUTDIR)\oclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Release"

OUTDIR=.\UniRel
INTDIR=.\UniRel
# Begin Custom Macros
OutDir=.\.\UniRel
# End Custom Macros

ALL : ".\.\UniRel\oclient.HLP" "$(OUTDIR)\oclient.exe"


CLEAN :
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\maindoc.obj"
	-@erase "$(INTDIR)\mainview.obj"
	-@erase "$(INTDIR)\oclient.obj"
	-@erase "$(INTDIR)\oclient.pch"
	-@erase "$(INTDIR)\oclient.res"
	-@erase "$(INTDIR)\rectitem.obj"
	-@erase "$(INTDIR)\splitfra.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\oclient.exe"
	-@erase ".\.\UniRel\oclient.HLP"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\oclient.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\oclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\oclient.pdb" /machine:I386 /out:"$(OUTDIR)\oclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\maindoc.obj" \
	"$(INTDIR)\mainview.obj" \
	"$(INTDIR)\oclient.obj" \
	"$(INTDIR)\rectitem.obj" \
	"$(INTDIR)\splitfra.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\oclient.res"

"$(OUTDIR)\oclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("oclient.dep")
!INCLUDE "oclient.dep"
!ELSE 
!MESSAGE Warning: cannot find "oclient.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "OClient - Win32 Release" || "$(CFG)" == "OClient - Win32 Debug" || "$(CFG)" == "OClient - Win32 Unicode Debug" || "$(CFG)" == "OClient - Win32 Unicode Release"
SOURCE=.\frame.cpp

"$(INTDIR)\frame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\maindoc.cpp

"$(INTDIR)\maindoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\mainview.cpp

"$(INTDIR)\mainview.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\oclient.cpp

"$(INTDIR)\oclient.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\hlp\oclient.hpj

!IF  "$(CFG)" == "OClient - Win32 Release"

OutDir=.\.\Release
ProjDir=.
TargetName=oclient
InputPath=.\hlp\oclient.hpj

"$(OutDir)\$(TargetName).HLP"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ELSEIF  "$(CFG)" == "OClient - Win32 Debug"

OutDir=.\.\Debug
ProjDir=.
TargetName=oclient
InputPath=.\hlp\oclient.hpj

"$(OutDir)\$(TargetName).HLP"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Debug"

OutDir=.\.\UniDebug
ProjDir=.
TargetName=oclient
InputPath=.\hlp\oclient.hpj

"$(OutDir)\$(TargetName).HLP"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Release"

OutDir=.\.\UniRel
ProjDir=.
TargetName=oclient
InputPath=.\hlp\oclient.hpj

"$(OutDir)\$(TargetName).HLP"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ENDIF 

SOURCE=.\oclient.rc

"$(INTDIR)\oclient.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\rectitem.cpp

"$(INTDIR)\rectitem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\splitfra.cpp

"$(INTDIR)\splitfra.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\oclient.pch"


SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "OClient - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\oclient.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OClient - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\oclient.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\oclient.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OClient - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\oclient.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\oclient.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

