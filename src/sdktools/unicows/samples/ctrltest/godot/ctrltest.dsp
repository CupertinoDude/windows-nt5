# Microsoft Developer Studio Project File - Name="CtrlTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CtrlTest - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ctrltest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ctrltest.mak" CFG="CtrlTest - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CtrlTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CtrlTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CtrlTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /Yu"Stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 c:\nt\sdktools\unicows\delay\obj\i386\unicows.lib kernel32.lib gdi32.lib user32.lib ole32.lib oleaut32.lib oledlg.lib shell32.lib uuid.lib comctl32.lib comdlg32.lib advapi32.lib winspool.lib uafxcw.lib libcmt.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CtrlTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /Yu"Stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 c:\nt\sdktools\unicows\delay\obj\i386\unicows.lib kernel32.lib gdi32.lib user32.lib ole32.lib oleaut32.lib oledlg.lib shell32.lib uuid.lib comctl32.lib comdlg32.lib advapi32.lib winspool.lib uafxcwd.lib libcmtd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CtrlTest - Win32 Release"
# Name "CtrlTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\bbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrlmac.r
# End Source File
# Begin Source File

SOURCE=.\ctrltest.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrltest.rc
# End Source File
# Begin Source File

SOURCE=.\custlist.cpp
# End Source File
# Begin Source File

SOURCE=.\custmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\dertest.cpp
# End Source File
# Begin Source File

SOURCE=.\paredit.cpp
# End Source File
# Begin Source File

SOURCE=.\paredit2.cpp
# End Source File
# Begin Source File

SOURCE=.\spintest.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\subtest.cpp
# End Source File
# Begin Source File

SOURCE=.\wclstest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# Begin Source File

SOURCE=.\ctrltest.h
# End Source File
# Begin Source File

SOURCE=.\paredit.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\canceld.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancelf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancelu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrltest.ico
# End Source File
# Begin Source File

SOURCE=.\res\ctrltest.rc2
# End Source File
# Begin Source File

SOURCE=.\res\image1d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image1f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image1u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image2d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image2f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image2u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\okd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\okf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\oku.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevx.bmp
# End Source File
# End Group
# End Target
# End Project
