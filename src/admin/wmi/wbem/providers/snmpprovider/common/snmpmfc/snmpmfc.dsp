# Microsoft Developer Studio Project File - Name="snmpmfc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=snmpmfc - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "snmpmfc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "snmpmfc.mak" CFG="snmpmfc - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "snmpmfc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "snmpmfc - Win32 Profile" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "snmpmfc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "output"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /Zi /Od /I ".\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_MT" /D "_DLL" /FR /YX /FD /c
# SUBTRACT CPP /X
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "snmpmfc - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "snmpmfc_"
# PROP BASE Intermediate_Dir "snmpmfc_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "output"
# PROP Intermediate_Dir "Profile"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GR /GX /Zi /Od /I ".\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_MT" /D "_DLL" /FR /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /Zi /Od /I ".\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_MT" /D "_DLL" /FR /YX /FD /Gh /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "snmpmfc - Win32 Release"
# Name "snmpmfc - Win32 Profile"
# Begin Source File

SOURCE=.\ARRAY_O.CPP
# End Source File
# Begin Source File

SOURCE=.\LIST_O.CPP
# End Source File
# Begin Source File

SOURCE=.\MTCORE.CPP
# End Source File
# Begin Source File

SOURCE=.\MTEX.CPP
# End Source File
# Begin Source File

SOURCE=.\plex.cpp
# End Source File
# Begin Source File

SOURCE=.\STRCORE.CPP
# End Source File
# Begin Source File

SOURCE=.\STREX.CPP
# End Source File
# End Target
# End Project
