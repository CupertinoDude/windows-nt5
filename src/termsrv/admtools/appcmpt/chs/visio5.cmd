@Echo Off

Cls
Rem #########################################################################

Rem
Rem ȷ���Ƿ� %ROOTDRIVE% �ѱ����ã��������ڸ�����ű���
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem ��ע����л�� Visio �İ�װλ��
Rem ��� Visio �汾: Standard/Technical/Professional
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
Rem ������װ�汾ʧ��
Rem 

Echo.
Echo �޷���ע������л�� Visio 5.0 �İ�װλ�á�
Echo ȷ���Ƿ� Visio 5.0 �ѱ���װ�������������и�����ű���
Echo 
Echo.
Pause
Goto Done

Rem
Rem ���� VSO5INST ��������ָ�� Visio �İ�װĿ¼
Rem
:Cont0
Call %Temp%\Vso.cmd
Del %Temp%\Vso.cmd >NUL: 2>&1

Rem
Rem ��ʾ��װ���ĸ��汾
Rem 
Echo.
Echo Ӧ�ó�������ű���⵽�汾 Visio %VisioVer%
Echo.

Rem
Rem �����ĵ�����Ŀ¼��ÿ�û��ļ��� My Documents
Rem �����ǰ�װ�û��ļ��� My Documents
Rem

..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "DrawingsPath" "%ROOTDRIVE%\%MY_DOCUMENTS%"

Rem
Rem �û��ʵ����
Rem �����װ�� Office���뽫 Visio.ini ��ָ�� Office �� custom.dic �ļ�
Rem ����Ļ�������Ϊ APP_DATA
Rem

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\9.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off2000

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

Rem ���ִ�е����˵��û�а�װ�κ� Office �汾
Set CustomDicPath=%ROOTDRIVE%\%APP_DATA%
goto SetCusIni

:Off2000
Rem ��װ�� Office 2000
set CustomDicPath=%ROOTDRIVE%\%APP_DATA%\Microsoft\Proof
goto SetCusIni

:Off97
Rem ��װ�� Office97
set CustomDicPath=%ROOTDRIVE%\Office97
goto SetCusIni

:Off95
Rem ��װ�� Office95
Set CustomDicPath=%ROOTDRIVE%\Office95

:SetCusIni
Rem ���ݹ��򣬸��� Visio.ini �е��û��ʵ��
Rem 
..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "UserDictionaryPath1" "%CustomDicPath%\Custom.Dic"

Set CustomDicPath=

Rem 
Rem �ɹ�����
Rem

Echo. 
Echo Visio 5.0 ���û�Ӧ�ó��������ϡ�
Echo.
Pause

:Done


