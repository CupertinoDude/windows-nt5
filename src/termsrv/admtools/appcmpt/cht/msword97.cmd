@Echo Off

Rem #########################################################################
Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w���o�ӫ��O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �N�o���ɮױq�ثe�ϥΪ̽d�����ƻs��Ҧ��ϥΪ̽d���Ҧb��m�C
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem #########################################################################



Rem #########################################################################

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0" "BinDirPath" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� Word 97 ���w�˦�m�C
Echo �нT�{ Word 97 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem Remove Find Fast from the Startup menu for all users.  Find Fast
Rem is resource intensive and will impact system performance.
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1


Rem #########################################################################

Rem
Rem �ܧ�n�����X�A���V�ϥΪ̫��w�����|�C
Rem

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\MsWord97.Key MsWord97.Tmp 
..\acsr "#__SharedTools#" "%__SharedTools%" MsWord97.Tmp MsWord97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" MsWord97.Tmp2 MsWord97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" MsWord97.Tmp3 MsWord97.key
Del MsWord97.Tmp >Nul: 2>&1
Del MsWord97.Tmp2 >Nul: 2>&1
Del MsWord97.Tmp3 >Nul: 2>&1
regini MsWord97.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s Wrd97Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Wrd97Usr.Cmd ..\Logon\Wrd97Usr.Cmd

FindStr /I Wrd97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Wrd97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   ���F���� MS Word 97 ���T�ާ@�A�ثe�w�n�J
Echo   ���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   ���ε{���C
Echo.
Echo Microsoft Word 97 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done





