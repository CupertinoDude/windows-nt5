@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN��]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem �q�n�������o Office 95 �w�˦�m�C
Rem �p�G�䤣��A�N���] Office 95 �å��w�˨���ܿ��~�T���C
Rem

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Cont0

Echo.
Echo �L�k�q�n������� Office 95 ���w�˦�m�C
Echo �нT�{ Office 95 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O95.Cmd
Del %Temp%\O95.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem  �q [�Ұ�] �\������� [�ֳt�M��]�C
Rem  [�ֳt�M��] �|�ϥΤj�q�귽�í��C�t�ήį�C
Rem  �����ˬd�ثe���ϥΪ̥\���ΩҦ��ϥΪ̥\���C Which menu
Rem  �\�����ܤ覡�N���t�Ϊ�^����Ҧ����覡�M�w�C
Rem

If Not Exist "%USER_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" Goto Cont1
Del "%USER_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%COMMON_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" Goto Cont2
Del "%COMMON_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem �N PowerPoint �Τ���ɮ׽ƻs�� [All Users] �ؿ��A
Rem ��ϥΪ̵n�J�A���̥i�H�Q�ƻs��ϥΪ̥D�ؿ����C
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER70.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" copy "%UserProfile%\%TEMPLATES%\PPT70.PPT" "%ALLUSERSPROFILE%\%TEMPLATES%" /Y >Nul: 2>&1


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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office95.Key Office95.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office95.Tmp Office95.Tmp2
..\acsr "#INSTDIR#" "%O95INST%" Office95.Tmp2 Office95.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office95.Tmp3 Office95.Key
Del Office95.Tmp >Nul: 2>&1
Del Office95.Tmp2 >Nul: 2>&1
Del Office95.Tmp3 >Nul: 2>&1
regini Office95.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s Ofc95Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%O95INST%" ..\Logon\Template\Ofc95Usr.Cmd ..\Logon\Ofc95Usr.Cmd

FindStr /I Ofc95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   ���F�T�O Office 95 �A�����A�ثe�n�J���ϥΪ�
Echo   �������n�X��A�n�J�A�~����� Office 95 ���ε{���C
Echo.
Echo Microsoft Office 95 �h�ϥΪ����ε{���վ㧹���C
Pause

:Done

