# Microsoft Developer Studio Project File - Name="ImageCon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 61000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ImageCon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "imagecon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "imagecon.mak" CFG="ImageCon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageCon - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "ImageCon - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ImageCon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f ImageCon.mak"
# PROP BASE Clean_Line "NMAKE /f ImageCon.mak CLEAN"
# PROP BASE Target_File "ImageCon.exe"
# PROP BASE Bsc_Name "ImageCon.bsc"
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "nmake /f "ImageCon.mak""
# PROP Clean_Line ""
# PROP Target_File "ImageCon.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""
# PROP Debug_Exe ""

!ELSEIF  "$(CFG)" == "ImageCon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f ImageCon.mak"
# PROP BASE Clean_Line "NMAKE /f ImageCon.mak CLEAN"
# PROP BASE Target_File "ImageCon.exe"
# PROP BASE Bsc_Name "ImageCon.bsc"
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "cmd.exe /k build32.cmd /ID"
# PROP Clean_Line "cmd.exe /k build32.cmd /Icz"
# PROP Target_File "ImageCon.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""
# PROP Debug_Exe ""

!ENDIF 

# Begin Target

# Name "ImageCon - Win32 Release"
# Name "ImageCon - Win32 Debug"

!IF  "$(CFG)" == "ImageCon - Win32 Release"

!ELSEIF  "$(CFG)" == "ImageCon - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ImageCon.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageCon.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\sources
# End Source File
# End Target
# End Project
