@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �N�ɮױq�ثe�ϥΪ̽d���ƻs��Ҧ��ϥΪ̽d���Ҧb��m�C
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem �q�n�������o Excel 97 �w�˦�m�C
Rem �p�G�䤣��A�N���] Excel �å��w�ˡA����ܿ��~�T���C
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0" "BinDirPath" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n����� Excel 97 ���w�˦�m�C
Echo �нT�{�O�_�w�w�� Excel 97�A�í��s����o��
Echo ���O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem  �q [�Ұ�] �\������� [�ֳt�M��]�C
Rem  [�ֳt�M��] �|�ϥΤj�q�귽�í��C�t�ήį�C
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �إ� MsForms.Twd �ɮסA���O 
Rem Powerpoint �� Excel Add-ins ([�ɮ�] [�x�s�� HTML] ����) �������ɮסC 
Rem ����o������ε{���ɡA���̱N�|�ξA���ɮרӨ��N�t�����n��ƪ��ɮסC
Rem

If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul: 2>&1
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont2

If Exist %systemroot%\system32\RefEdit.Twd Goto Cont3
Copy Nul: %systemroot%\system32\RefEdit.Twd >Nul: 2>&1
Cacls %systemroot%\system32\RefEdit.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont3

Rem #########################################################################

Rem
Rem �ܧ�n�����X�A�N���|���V�ϥΪ̩ҫ��w��
Rem �ؿ��C
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\MsExcl97.Key MsExcl97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" MsExcl97.Tmp MsExcl97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" MsExcl97.Tmp2 MsExcl97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" MsExcl97.Tmp3 MsExcl97.key
Del MsExcl97.Tmp >Nul: 2>&1
Del MsExcl97.Tmp2 >Nul: 2>&1
Del MsExcl97.Tmp3 >Nul: 2>&1

regini MsExcl97.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s Exl97Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Exl97Usr.Cmd ..\Logon\Exl97Usr.Cmd

FindStr /I Exl97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Exl97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   ���F���� MS Excel 97 ���T�ާ@�A�ثe�w�n�J���ϥΪ�
Echo   �������n�X�A�M�᭫�s�n�J�A
Echo   �~��������ε{���C
Echo.
Echo Microsoft Excel 97 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
