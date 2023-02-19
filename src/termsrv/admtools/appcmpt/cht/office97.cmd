
@Echo Off

Rem
Rem  �`�N: �b�o�ӫ��O�ɤ��� CACLS �R�O�u��b
Rem         NTFS �榡���ϺФ��Τ�����C
Rem

Rem #########################################################################


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

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1



Rem
Rem �q�n�������o Office 97 �w�˦�m�C
Rem �p�G�䤣��A�N���] Office �å��w�ˡA����ܿ��~�T���C
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� Office 97 ���w�˦�m�C
Echo �нT�{ Office 97 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �N Access 97 �t�θ�Ʈw�ܧ󦨰�Ū�C
Rem �o�˥i�H���\�h�H�P�ɰ����Ʈw�C
Rem ���O�N�L�k�ϥ� [�u��] �\��� [�O��] �ӧ�s�t�θ�Ʈw�C
Rem �p�G�z�ݭn�s�W�ϥΪ̡A�z�����N�t�θ�Ʈw�]�w��
Rem �i�g�J�C
Rem

If Not Exist %SystemRoot%\System32\System.Mdw Goto Cont1
cacls %SystemRoot%\System32\System.Mdw /E /P "Authenticated Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P "Power Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P Administrators:R >NUL: 2>&1

:Cont1

Rem #########################################################################

Rem
Rem ���\����HŪ���Q  Office 97 ��s�L���t�� DLL�C
Rem

REM If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R >NUL: 2>&1

Rem #########################################################################

Rem #########################################################################

Rem
Rem ���\ TS Users �ܧ� outlook �� frmcache.dat �ɮ�
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################

Rem
Rem �N Powerpoint ���F�ܧ󦨰�Ū�A���\�h�H�i�H
Rem �P�ɩI�s���F�C
Rem

If Exist "%O97INST%\Templates\²��\���e�j�����F.pwz" Attrib +R "%O97INST%\Templates\²��\���e�j�����F.pwz" >Nul: 2>&1

If Exist "%O97INST%\Office\Ppt2html.ppa" Attrib +R "%O97INST%\Office\Ppt2html.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\bshppt97.ppa" Attrib +R "%O97INST%\Office\bshppt97.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\geniwiz.ppa" Attrib +R "%O97INST%\Office\geniwiz.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\ppttools.ppa" Attrib +R "%O97INST%\Office\ppttools.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\graphdrs.ppa" Attrib +R "%O97INST%\Office\graphdrs.ppa" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �p�G�z�n���D�t�κ޲z�����ϥΪ̦b Excel ������ Access ���F��
Rem Access Add-Ins�A�z���������H�U 3 ��{���X��  "Rem"�A 
Rem ���ϥΪ��ܧ���F�ɮת��s���覡�C
Rem 
Rem

Rem If Exist "%O97INST%\Office\WZLIB80.MDE" cacls "%O97INST%\Office\WZLIB80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZMAIN80.MDE" cacls "%O97INST%\Office\WZMAIN80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZTOOL80.MDE" cacls "%O97INST%\Office\WZTOOL80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 

Rem #########################################################################

Rem
Rem �إ� MsForms.Twd �� RefEdit.Twd �ɮסA���̬O 
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
Rem �b SystemRoot �U�إ� msremote.sfs �ؿ��C�o�˷|���\�ϥΪ�
Rem �ϥ� [����x] [�l��ζǯu] �ϥܨӫإ߳]�w�ɡC
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1

Rem #########################################################################

Rem
Rem  �q [�Ұ�] �\������� [�ֳt�M��]�C
Rem  [�ֳt�M��] �|�ϥΤj�q�귽�í��C�t�ήį�C
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1


Rem #########################################################################
Rem
Rem �q�Ҧ��ϥΪ̪� [�Ұ�] �\���W���� [Microsoft Office �ֱ��C.lnk] �ɮסC
Rem

If Exist "%COMMON_STARTUP%\Microsoft Office �ֱ��C.lnk" Del "%COMMON_STARTUP%\Microsoft Office �ֱ��C.lnk" >Nul: 2>&1

Rem #########################################################################
Rem
Rem �b SystemRoot �U�إ� msfslog.txt �ɮסA���� Terminal Server
Rem ���ϥΪ̦��ɮת�����s���v�C
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office97.Key Office97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office97.Tmp Office97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" Office97.Tmp2 Office97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office97.Tmp3 Office97.Key
Del Office97.Tmp >Nul: 2>&1
Del Office97.Tmp2 >Nul: 2>&1
Del Office97.Tmp3 >Nul: 2>&1

regini Office97.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s Ofc97Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Ofc97Usr.Cmd ..\Logon\Ofc97Usr.Cmd

FindStr /I Ofc97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   ���F���� Office 97 ���T�ާ@�A�ثe�w�n�J
Echo   ���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   Office 97 ���ε{���C
Echo.
Echo Microsoft Office 97 �h�ϥΪ����ε{���վ�B�z����
Pause

:done
