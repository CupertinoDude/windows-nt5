
@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w���o�ӫ��O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Set __SharedTools=Shared Tools

If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto Start0
If Not Exist "%ProgramFiles(x86)%" goto Start0
Set __SharedTools=Shared Tools (x86)

:Start0

Rem #########################################################################
Rem �]�w app VendorName
SET VendorName=Microsoft

Rem #########################################################################
Rem �N�չ�u����|�]�w�� MS Office 2000 �ϥΪ����|
SET ProofingPath=Proof

Rem #########################################################################
Rem �]�w�ΨӦs�����ε{���w�ˮڥؿ����n�����X�Ψ��

SET AppRegKey=HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot
SET AppRegValue=OfficeBin

Rem #########################################################################
Rem �]�w�ΨӦs�����ε{���d���ؿ����n�����X�Ψ��

SET AppTemplatesRegKey=HKCU\Software\Microsoft\Office\8.0\Common\FileNew\LocalTemplates
SET AppTemplatesRegValue=

Rem #########################################################################
Rem �]�w�ΨӦs�����ε{���ۭq�ؿ����|�W�٪��n�����X�Ψ��

SET AppCustomDicRegKey=HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Custom Dictionaries
SET AppCustomDicRegValue=1


Rem #########################################################################
Rem �]�w���w���������ε{���ɮצW�٤θ��|�W��

SET CustomDicFile=Custom.Dic
SET AppPathName=Microsoft Project
SET AppWebPathName=Microsoft Project Web

Rem #########################################################################
Rem �]�w MS Office 2000 �ҨϥΪ��w�]���|

SET AppData=%RootDrive%\%APP_DATA%
SET UserTemplatesPath=%AppData%\%VendorName%\%TEMPLATES%
SET UserCustomDicPath=%AppData%\%VendorName%\%ProofingPath%

Rem #########################################################################
Rem �q�n�������o Project 98 �w�˦�m�C
Rem �p�G�䤣��A���]�o�����ε{���å��w�ˡA����ܿ��~�T���C
Rem

..\ACRegL %Temp%\Proj98.Cmd PROJINST "%AppRegKey%" "%AppRegValue%" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� Project 98 ���w�˦�m�C
Echo �нT�{ Project 98 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1


..\ACRegL %Temp%\Proj98.Cmd PROJROOT "%AppRegKey%" "%AppRegValue%" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont01
Echo.
Echo �L�k�q�n������� Project 98 ���w�˦�m�C
Echo �нT�{ Project 98 �O�_�w�w�ˡA�M�᭫�s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont01
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

Rem #########################################################################
Rem �q�n�������o�d�����|�W��
Rem
..\ACRegL %Temp%\Proj98.Cmd TemplatesPathName "%AppTemplatesRegKey%" "%AppTemplatesRegValue%" "STRIPPATH"
If Not ErrorLevel 1 Goto Cont02
SET TemplatesPathName=%TEMPLATES%
Goto Cont03
:Cont02
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

:Cont03

Rem #########################################################################
Rem �p�G�å��w�� MS Office 97�A�N�ϥιw�]���|

If Not Exist "%RootDrive%\Office97" Goto SetPathNames

Rem #########################################################################
Rem �p�G�w�w�� MS Office 97�A�N�ϥ� MS Office 97 ���|�C


Rem #########################################################################
Rem �q�n�������o�ۭq�r�媺���|�W��
Rem
..\ACRegL %Temp%\Proj98.Cmd AppData "%AppCustomDicRegKey%" "%AppCustomDicRegValue%" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont04
SET AppData=%RootDrive%\Office97
Goto Cont05
:Cont04
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1
:Cont05

Rem #########################################################################
Rem �q�n�������o�d�����|�W��
Rem
..\ACRegL %Temp%\Proj98.Cmd UserTemplatesPath "%AppTemplatesRegKey%" "%AppTemplatesRegValue%" ""
If Not ErrorLevel 1 Goto Cont06
SET UserTemplatesPath=%AppData%\%TemplatesPathName%
Goto Cont07
:Cont06
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

:Cont07
SET UserCustomDicPath=%AppData%

:SetPathNames

Rem #########################################################################
Rem �]�w�ϥΪ̤Τ��θ��|�W��

SET CommonCustomDicPath=%PROJINST%
SET CommonTemplatesPath=%PROJROOT%\%TemplatesPathName%

SET UserCustomDicFileName=%UserCustomDicPath%\%CustomDicFile%
SET UserAppTemplatesPath=%UserTemplatesPath%\%AppPathName%
SET UserAppWebTemplatesPath=%UserTemplatesPath%\%AppWebPAthName%
SET CommonAppTemplatesPath=%CommonTemplatesPath%\%AppPathName%
SET CommonAppWebTemplatesPath=%CommonTemplatesPath%\%AppWebPAthName%

Rem #########################################################################

Rem
Rem �p�G Office 97 �w�w�ˡAProject 98 �w�˫��O��
Rem �|�N�d�����ʥثe���ϥΪ̥ؿ��C
Rem �N���̩�b�q�ΰϰ�CProj98Usr.cmd �i�H�N�d�����^��B�C
Rem

If NOT Exist "%UserAppTemplatesPath%"  goto skip10
If Exist  "%CommonAppTemplatesPath%" goto skip10
xcopy "%UserAppTemplatesPath%" "%CommonAppTemplatesPath%" /E /I /K > Nul: 2>&1
:skip10

If NOT Exist "%UserAppWebTemplatesPath%"  goto skip11
If Exist  "%CommonAppWebTemplatesPath%" goto skip11
xcopy "%UserAppWebTemplatesPath%" "%CommonAppWebTemplatesPath%" /E /I /K > Nul: 2>&1
:skip11

Rem #########################################################################

Rem
Rem �N Global.mpt �ɮ׳]����Ū�C
Rem �_�h�Ĥ@�ӱҰ� Project ���ϥΪ̷|�ܧ� ACLs�C
Rem

if Exist "%PROJINST%\Global.mpt" attrib +r "%PROJINST%\Global.mpt"


Rem #########################################################################

Rem
Rem ���\����HŪ���Q  Office 97 ��s�L���t�� DLL�C
Rem
If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R > NUL: 2>&1

Rem #########################################################################

Rem
Rem �إ� MsForms.Twd �ɮסA���O 
Rem Powerpoint �� Excel Add-ins ([�ɮ�] [�x�s�� HTML] ����) �������ɮסC 
Rem ����o������ε{���ɡA���̱N�|�ξA���ɮרӨ��N�t�����n��ƪ��ɮסC
Rem
If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul:
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F > Nul: 2>&1
:Cont2

Rem #########################################################################

Rem
Rem  �q [�Ұ�] �\������� [�ֳt�M��]�C
Rem  [�ֳt�M��] �|�ϥΤj�q�귽�í��C�t�ήį�C
Rem


If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk"

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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj98.Key msproj98.tm1
..\acsr "#__SharedTools#" "%__SharedTools%" msproj98.tm1 msproj98.tm2
Del msproj98.tm1 >Nul: 2>&1
..\acsr "#USERCUSTOMDICFILE#" "%UserCustomDicFileName%" msproj98.tm2 msproj98.Key
Del msproj98.tm2 >Nul: 2>&1

regini msproj98.key > Nul:

Rem If original mode was execute, change back to Execute Mode.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem ��s proj97Usr.Cmd �ӤϬM��ڪ��w�˥ؿ��A
Rem �ñN��[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#USERTEMPLATESPATH#" "%UserTemplatesPath%" ..\Logon\Template\prj98Usr.Cmd prj98Usr.tm1
..\acsr "#USERCUSTOMDICPATH#" "%UserCustomDicPath%" prj98Usr.tm1 prj98Usr.tm2
Del prj98Usr.tm1 >Nul: 2>&1
..\acsr "#COMMONTEMPLATESPATH#" "%CommonTemplatesPath%" prj98Usr.tm2 prj98Usr.tm1
Del prj98Usr.tm2 >Nul: 2>&1
..\acsr "#COMMONCUSTOMDICPATH#" "%CommonCustomDicPath%" prj98Usr.tm1 prj98Usr.tm2
Del prj98Usr.tm1 >Nul: 2>&1
..\acsr "#CUSTOMDICNAME#" "%CustomDicFile%" prj98Usr.tm2 prj98Usr.tm1
Del prj98Usr.tm2 >Nul: 2>&1
..\acsr "#APPPATHNAME#" "%AppPathName%" prj98Usr.tm1 prj98Usr.tm2
Del prj98Usr.tm1 >Nul: 2>&1
..\acsr "#APPWEBPATHNAME#" "%AppWebPathName%" prj98Usr.tm2 ..\Logon\prj98Usr.Cmd
Del prj98Usr.tm2 >Nul: 2>&1

FindStr /I prj98Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call prj98Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   ���F���� Project 98 ���T�ާ@�A�ثe�w�n
Echo   �J���ϥΪ̥������n�X�A�A���s�n�J�A�~��
Echo   �������ε{���C
Echo.
Echo Microsoft Project 98 �h�ϥΪ����ε{���վ�B�z����
Pause

:done

