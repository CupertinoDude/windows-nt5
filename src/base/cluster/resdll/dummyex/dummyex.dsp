# Microsoft Developer Studio Project File - Name="DummyEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=DummyEx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DummyEx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DummyEx.mak" CFG="DummyEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DummyEx - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DummyEx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DummyEx - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "DummyEx - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "DummyEx - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "."
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /Gf /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\AlphaRel"
# PROP BASE Intermediate_Dir ".\AlphaRel"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\AlphaRel"
# PROP Intermediate_Dir ".\AlphaRel"
# PROP Target_Dir "."
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 /nologo /subsystem:windows /dll /machine:ALPHA

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\AlphaDbg"
# PROP BASE Intermediate_Dir ".\AlphaDbg"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\AlphaDbg"
# PROP Intermediate_Dir ".\AlphaDbg"
# PROP Target_Dir "."
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /alpha
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:ALPHA

!ENDIF 

# Begin Target

# Name "DummyEx - Win32 Release"
# Name "DummyEx - Win32 Debug"
# Name "DummyEx - Win32 (ALPHA) Release"
# Name "DummyEx - Win32 (ALPHA) Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasePage.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DDxDDv.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DummyEx.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DummyEx.def
# End Source File
# Begin Source File

SOURCE=.\DummyEx.rc

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExtObj.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExtObjID.idl

!IF  "$(CFG)" == "DummyEx - Win32 Release"

# Begin Custom Build - Running MIDL
InputDir=.
InputPath=.\ExtObjID.idl
InputName=ExtObjID

BuildCmds= \
	midl $(InputPath) -DMIDL_PASS /header $(InputDir)\$(InputName).h /iid\
 $(InputDir)\$(InputName)_i.c /tlb $(InputDir)\$(InputName).tlb

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

# Begin Custom Build - Running MIDL
InputDir=.
InputPath=.\ExtObjID.idl
InputName=ExtObjID

BuildCmds= \
	midl $(InputPath) -DMIDL_PASS /header $(InputDir)\$(InputName).h /iid\
 $(InputDir)\$(InputName)_i.c /tlb $(InputDir)\$(InputName).tlb

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

# Begin Custom Build - Running MIDL
InputDir=.
InputPath=.\ExtObjID.idl
InputName=ExtObjID

BuildCmds= \
	midl $(InputPath) -DMIDL_PASS /header $(InputDir)\$(InputName).h /iid\
 $(InputDir)\$(InputName)_i.c /tlb $(InputDir)\$(InputName).tlb

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

# Begin Custom Build - Running MIDL
InputDir=.
InputPath=.\ExtObjID.idl
InputName=ExtObjID

BuildCmds= \
	midl $(InputPath) -DMIDL_PASS /header $(InputDir)\$(InputName).h /iid\
 $(InputDir)\$(InputName)_i.c /tlb $(InputDir)\$(InputName).tlb

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PropList.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RegExt.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ResProp.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "DummyEx - Win32 Release"

# ADD BASE CPP /Yc
# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "DummyEx - Win32 Debug"

# ADD BASE CPP /Yc
# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Release"

# ADD BASE CPP /Gt0 /Yc
# ADD CPP /Gt0 /Yc

!ELSEIF  "$(CFG)" == "DummyEx - Win32 (ALPHA) Debug"

# ADD BASE CPP /Gt0 /Yc
# ADD CPP /Gt0 /Yc

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasePage.h
# End Source File
# Begin Source File

SOURCE=.\BasePage.inl
# End Source File
# Begin Source File

SOURCE=.\DDxDDv.h
# End Source File
# Begin Source File

SOURCE=.\DummyEx.h
# End Source File
# Begin Source File

SOURCE=.\ExtObj.h
# End Source File
# Begin Source File

SOURCE=.\PropList.h
# End Source File
# Begin Source File

SOURCE=.\RegExt.h
# End Source File
# Begin Source File

SOURCE=.\ResProp.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DummyEx.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
