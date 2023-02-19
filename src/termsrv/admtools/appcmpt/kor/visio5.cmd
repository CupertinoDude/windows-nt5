@Echo Off

Cls
Rem #########################################################################

Rem
Rem %ROOTDRIVE%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem ������Ʈ������ Visio ��ġ ��ġ�� ����ϴ�.
Rem Visio�� ���� ����: Standard/Technical/Professional
Rem

Set VisioVer=Standard
..\ACRegL %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Standard\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Set VisioVer=Technical
..\ACRegl %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Technical\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Set VisioVer=Professional
..\ACRegl %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Professional\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Set VisioVer=Enterprise
..\ACRegl %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Enterprise\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Set VisioVer=TechnicalPlus
..\ACRegl %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Technical 5.0 Plus\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Set VisioVer=ProfessionalAndTechnical
..\ACRegl %Temp%\Vso.cmd VSO5INST "HKLM\Software\Visio\Visio Professional and Technical\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Rem
Rem ��ġ�� ������ �˻����� ���߽��ϴ�.
Rem 

Echo.
Echo ������Ʈ������ Visio 5.0 ��ġ ��ġ�� �˻��� �� �����ϴ�.
Echo Visio 5.0�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ�� �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

Rem
Rem VSO5INST ������ Visio ��ġ ���͸��� �����մϴ�.
Rem
:Cont0
Call %Temp%\Vso.cmd
Del %Temp%\Vso.cmd >NUL: 2>&1

Rem
Rem ��� ������ ��ġ�Ǿ� �ִ��� ǥ���մϴ�.
Rem 
Echo.
Echo Vision %VisioVer% ������ ã�ҽ��ϴ�.
Echo.

Rem
Rem ���� ���͸��� ����� ���� My Documents�� �����մϴ�.
Rem ����� My Documents�� ��ġ���� �ʽ��ϴ�.
Rem

..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "DrawingsPath" "%ROOTDRIVE%\%MY_DOCUMENTS%"

Rem
Rem ����� ���� ���� ����
Rem Office ������ ��ġ�Ǿ� ������ Visio.ini �׸��� Office custom.dic�� ����Ű���� �����մϴ�.
Rem �׷��� ������ APP_DATA�� �����մϴ�.
Rem

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\9.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off2000

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

Rem ������� ������, Office ������ ��ġ�Ǿ� ���� �ʽ��ϴ�.
Set CustomDicPath=%ROOTDRIVE%\%APP_DATA%
goto SetCusIni

:Off2000
Rem Office 2000�� ��ġ�Ǿ����ϴ�.
set CustomDicPath=%ROOTDRIVE%\%APP_DATA%\Microsoft\Proof
goto SetCusIni

:Off97
Rem Office97�� ��ġ�Ǿ����ϴ�.
set CustomDicPath=%ROOTDRIVE%\Office97
goto SetCusIni

:Off95
Rem Office95�� ��ġ�Ǿ����ϴ�.
Set CustomDicPath=%ROOTDRIVE%\Office95

:SetCusIni
Rem Visio.ini ���� ����� ���� ���� �׸���
Rem MS ��å�� ���� �����Ͻʽÿ�.
..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "UserDictionaryPath1" "%CustomDicPath%\Custom.Dic"

Set CustomDicPath=

Rem 
Rem ���������� ����˴ϴ�.
Rem

Echo. 
Echo Visio 5.0 ���� ����� ���� ���α׷� ���� �Ϸ�
Echo.
Pause

:Done


