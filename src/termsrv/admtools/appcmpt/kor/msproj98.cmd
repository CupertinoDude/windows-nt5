
@Echo Off

Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
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
Rem ���� ���α׷� ���޾�ü �̸��� �����մϴ�.
SET VendorName=Microsoft

Rem #########################################################################
Rem ���� ���� ��θ� MS Office 2000�� ����ϴ� ��η� �����մϴ�.
SET ProofingPath=Proof

Rem #########################################################################
Rem ���� ���α׷��� �� ��ġ ��Ʈ�� �����ϴ� ������Ʈ�� Ű �� ���� �����մϴ�.

SET AppRegKey=HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot
SET AppRegValue=OfficeBin

Rem #########################################################################
Rem ���� ���α׷��� �� ���ø� ��ο� �����ϴ� ������Ʈ�� Ű �� ���� �����մϴ�.

SET AppTemplatesRegKey=HKCU\Software\Microsoft\Office\8.0\Common\FileNew\LocalTemplates
SET AppTemplatesRegValue=

Rem #########################################################################
Rem ���� ���α׷��� ����� ���� ���� ��� �� �̸��� �����ϴ� ������Ʈ�� Ű �� ���� �����մϴ�.

SET AppCustomDicRegKey=HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Custom Dictionaries
SET AppCustomDicRegValue=1


Rem #########################################################################
Rem �Ϻ� Ư���� ������� ���� ���α׷� ���� �̸� �� ��� �̸��� �����մϴ�.

SET CustomDicFile=Custom.Dic
SET AppPathName=Microsoft Project
SET AppWebPathName=Microsoft Project Web

Rem #########################################################################
Rem �⺻ ��θ� MS Office 2000�� ����ϴ� ��η� �����մϴ�.

SET AppData=%RootDrive%\%APP_DATA%
SET UserTemplatesPath=%AppData%\%VendorName%\%TEMPLATES%
SET UserCustomDicPath=%AppData%\%VendorName%\%ProofingPath%

Rem #########################################################################
Rem ������Ʈ������ Project 98 ��ġ ��ġ�� ����ϴ�. ������ ������,
Rem Project �� ��ġ�Ǿ� ���� �ʴ� ������ �����ϰ� ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL %Temp%\Proj98.Cmd PROJINST "%AppRegKey%" "%AppRegValue%" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ������Ʈ������ Project 98 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo Project 98�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1


..\ACRegL %Temp%\Proj98.Cmd PROJROOT "%AppRegKey%" "%AppRegValue%" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont01
Echo.
Echo ������Ʈ������ Project 98 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo Project 98�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont01
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

Rem #########################################################################
Rem ������Ʈ������ ���ø� ��� �̸��� ����ϴ�.
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
Rem MS Office 97�� ��ġ�Ǿ� ���� ������ �⺻ ��θ� ����մϴ�.

If Not Exist "%RootDrive%\Office97" Goto SetPathNames

Rem #########################################################################
Rem MS Office 97�� ��ġ�Ǿ� �ָ� MS Office 97 ��θ� ����մϴ�.


Rem #########################################################################
Rem ������Ʈ������ ����� ���� dic ��� �̸��� ����ϴ�.
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
Rem ������Ʈ������ ���ø� ��� �̸��� ����ϴ�.
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
Rem ����� �� ���� ��� �̸��� �����մϴ�.

SET CommonCustomDicPath=%PROJINST%
SET CommonTemplatesPath=%PROJROOT%\%TemplatesPathName%

SET UserCustomDicFileName=%UserCustomDicPath%\%CustomDicFile%
SET UserAppTemplatesPath=%UserTemplatesPath%\%AppPathName%
SET UserAppWebTemplatesPath=%UserTemplatesPath%\%AppWebPAthName%
SET CommonAppTemplatesPath=%CommonTemplatesPath%\%AppPathName%
SET CommonAppWebTemplatesPath=%CommonTemplatesPath%\%AppWebPAthName%

Rem #########################################################################

Rem
Rem Office 97�� ��ġ�Ǿ� ������ Project 98 ��ġ ��ũ��Ʈ��
Rem ���ø��� ���� ����� ���͸��� �̵��߽��ϴ�.
Rem �۷ι� ��ġ�� �����ʽÿ�. Proj98Usr.cmd�� �� �ڸ��� �̵���ų ���Դϴ�.
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
Rem Global.mpt ������ �б� �������� ����ϴ�.
Rem �׷��� ������ Project�� �����ϴ� ù ����ڰ� ACL�� ������ ���Դϴ�.
Rem

if Exist "%PROJINST%\Global.mpt" attrib +r "%PROJINST%\Global.mpt"


Rem #########################################################################

Rem
Rem Office 97�� ������Ʈ�ϴ� �ý��� DLL����
Rem ��ο� ���� �б� �׼����� ����մϴ�.
Rem
If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R > NUL: 2>&1

Rem #########################################################################

Rem
Rem Powerpoint �� Excel �߰� ��ɿ� �ʿ��� MsForms.Twd ����
Rem (����/HTML�� ����, ��)�� ����ϴ�. �� �� �ϳ��� ���α׷���
Rem ����Ǹ� �� ���α׷��� �� ������ �ʿ��� �����Ͱ� ��� �ִ�
Rem ���Ϸ� �ٲߴϴ�.
Rem
If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul:
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F > Nul: 2>&1
:Cont2

Rem #########################################################################

Rem
Rem ��� ����ڿ� ���� [����] �޴����� [���� ã��]�� �����մϴ�.
Rem [���� ã��]�� ���ҽ��� ���� �ʿ�� �ϸ� �ý��� ���ɿ� ������
Rem �ݴϴ�.
Rem


If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk"

Rem #########################################################################

Rem
Rem ������Ʈ�� Ű�� �����Ͽ� ��ΰ� ����� ������
Rem ���͸��� ����Ű���� �մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
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

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem proj97Usr.Cmd�� ������Ʈ�Ͽ� ���� ���͸��� �ݿ��ϰ� �̸�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
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
Echo   Project 98�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Microsoft Project 98 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done

