# Microsoft Developer Studio Project File - Name="All" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=All - Win32 Win64 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "All.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "All.mak" CFG="All - Win32 Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "All - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "All - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "All - Win32 Win64 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "All - Win32 Win64 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "All - Win32 Release"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f All.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "All.exe"
# PROP BASE Bsc_Name "All.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "common\bin\spgrazzle.cmd free exec build -Z -F -I"
# PROP Rebuild_Opt "-c"
# PROP Target_File "All.exe"
# PROP Bsc_Name "All.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "All - Win32 Debug"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f All.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "All.exe"
# PROP BASE Bsc_Name "All.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "common\bin\spgrazzle.cmd exec build -Z -F -I"
# PROP Rebuild_Opt "-c"
# PROP Target_File "All.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "All - Win32 Win64 Debug"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win64 Debug"
# PROP BASE Intermediate_Dir "Win64 Debug"
# PROP BASE Cmd_Line "common\bin\spgrazzle.cmd exec build -Z -F -I"
# PROP BASE Rebuild_Opt "-c"
# PROP BASE Target_File "All.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win64 Debug"
# PROP Intermediate_Dir "Win64 Debug"
# PROP Cmd_Line "common\bin\spgrazzle.cmd win64 exec build -Z -F -I"
# PROP Rebuild_Opt "-c"
# PROP Target_File "All.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "All - Win32 Win64 Release"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win64 Release"
# PROP BASE Intermediate_Dir "Win64 Release"
# PROP BASE Cmd_Line "common\bin\spgrazzle.cmd free exec build -Z -F -I"
# PROP BASE Rebuild_Opt "-c"
# PROP BASE Target_File "All.exe"
# PROP BASE Bsc_Name "All.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win64 Release"
# PROP Intermediate_Dir "Win64 Release"
# PROP Cmd_Line "common\bin\spgrazzle.cmd win64 free exec build -Z -F -I"
# PROP Rebuild_Opt "-c"
# PROP Target_File "All.exe"
# PROP Bsc_Name "All.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "All - Win32 Release"
# Name "All - Win32 Debug"
# Name "All - Win32 Win64 Debug"
# Name "All - Win32 Win64 Release"

!IF  "$(CFG)" == "All - Win32 Release"

!ELSEIF  "$(CFG)" == "All - Win32 Debug"

!ELSEIF  "$(CFG)" == "All - Win32 Win64 Debug"

!ELSEIF  "$(CFG)" == "All - Win32 Win64 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
