# Microsoft Developer Studio Project File - Name="fileDist" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fileDist - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fileDist.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fileDist.mak" CFG="fileDist - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fileDist - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fileDist - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "fileDist"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "fileDist - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fileDist.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "fileDist.exe"
# PROP BASE Bsc_Name "fileDist.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line ""set BROWSER_INFO=1 && ..\..\..\common\bin\spgrazzle.cmd free exec build -z -F -I""
# PROP Rebuild_Opt "-c"
# PROP Target_File "obj\i386\fileDist.exe"
# PROP Bsc_Name "obj\i386\fileDist.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fileDist - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fileDist.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "fileDist.exe"
# PROP BASE Bsc_Name "fileDist.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line ""set BROWSER_INFO=1 && ..\..\..\common\bin\spgrazzle.cmd exec build -z -F -I""
# PROP Rebuild_Opt "-c"
# PROP Target_File "objd\i386\fileDist.exe"
# PROP Bsc_Name "objd\i386\fileDist.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "fileDist - Win32 Release"
# Name "fileDist - Win32 Debug"

!IF  "$(CFG)" == "fileDist - Win32 Release"

!ELSEIF  "$(CFG)" == "fileDist - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Viterbi.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Viterbi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
