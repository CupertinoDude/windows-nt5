@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�g�]�w�A�ñN���]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �q�n�������o Outlook 98  �w�˦�m�C
Rem �p�G�䤣��A�N���] Outlook 98  �å��w�˨���ܿ��~�T���C
Rem

..\ACRegL %Temp%\O98.Cmd O98INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "OfficeBin" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� Outlook 98 ���w�˦�m�C
Echo �нT�{ Outlook 98 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O98.Cmd
Del %Temp%\O98.Cmd >Nul: 2>&1

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


REM
REM �p�G�w�w�� Office97 �Ψå��w�� Office �A�N�ϥ� Office97 per-user dir
REM �p�G�w�w�� Office95�A�N�ϥ� Office95 per-user dir
REM
Set OffUDir=Office97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto OffChk

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

set OFFINST=%O98INST%
goto Cont1

:Off95
Set OffUDir=Office95

:OffChk

Call %Temp%\Off.Cmd
Del %Temp%\Off.Cmd >Nul: 2>&1

:Cont1

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Outlk98.Key Outlk98.Tmp
..\acsr "#INSTDIR#" "%OFFINST%" Outlk98.Tmp Outlk98.Tmp2
..\acsr "#OFFUDIR#" "%OffUDir%" Outlk98.Tmp2 Outlk98.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Outlk98.Tmp3 Outlk98.Key


Del Outlk98.Tmp >Nul: 2>&1
Del Outlk98.Tmp2 >Nul: 2>&1
Del Outlk98.Tmp3 >Nul: 2>&1

regini Outlk98.key > Nul:

Rem �p�G��ӬO����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s Olk98Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%OFFINST%" ..\Logon\Template\Olk98Usr.Cmd Olk98Usr.Tmp
..\acsr "#OFFUDIR#" "%OffUDir%" Olk98Usr.Tmp ..\Logon\Olk98Usr.Cmd
Del Olk98Usr.Tmp

FindStr /I Olk98Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Olk98Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Rem
Rem �b SystemRoot �U�إ� msremote.sfs �ؿ��C�o�|���\�ϥΪ�
Rem �ϥ� [����x] [�l��ζǯu] �ϥܨӫإ߳]�w�ɡC
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1



Rem #########################################################################

Rem
Rem ���\ TS Users �b frmcache.dat �ɮפ��ܧ� outlook �s���v�C
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################


Rem #########################################################################

Rem
Rem �b SystemRoot �U�إ� msfslog.txt �ɮסA�ñ»P Terminal Server �ϥΪ�
Rem ��o���ɮת�����s���v�C
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


Echo.
Echo   ���F���� Outlook 98 ���T�ާ@�A�ثe�w�n�J
Echo   ���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   Outlook 98�C
Echo.
Echo Microsoft Outlook 98 �h�ϥΪ����ε{���վ�B�z����
Pause

:done

