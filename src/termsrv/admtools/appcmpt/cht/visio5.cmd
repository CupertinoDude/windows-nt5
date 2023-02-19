@Echo Off

Cls
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
Rem �q�n���ɨ��o Visio �w�˦�m�C
Rem Visio �h������: Standard/Technical/Professional
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
Rem ����w�˪������ѡC
Rem 

Echo.
Echo �L�k�q�n���ɧ�� Visio 5.0 �w�˦�m�C
Echo ���ˬd Visio 5.0 �O�_�w�w�ˡA�í��s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

Rem
Rem �N VSO5INST �ܼƳ]�w�� Visio �w�˥ؿ��C
Rem
:Cont0
Call %Temp%\Vso.cmd
Del %Temp%\Vso.cmd >NUL: 2>&1

Rem
Rem ��ܦw�˪����C
Rem 
Echo.
Echo ���ε{���վ�\��w������ Vision %VisioVer% ���C
Echo.

Rem
Rem ���C�ӨϥΪ̪� [�ڪ����] �]�w�x�s�ؿ��A
Rem �����w�˨�ϥΪ̪� [�ڪ����] ���C
Rem

..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "DrawingsPath" "%ROOTDRIVE%\%MY_DOCUMENTS%"

Rem
Rem [�ۭq�ؿ�] �޲z
Rem �p�G Office �w�w�� (���@�ت���)�A�N Visio.ini ���س]�w�����V Office custom.dic
Rem �_�h�N�]�w�� AppData�C
Rem

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\9.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off2000

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

Rem �p�G�����o�̡A��ܨS���w�� Office�C
Set CustomDicPath=%ROOTDRIVE%\%APP_DATA%
goto SetCusIni

:Off2000
Rem Office 2000 �w�w��
set CustomDicPath=%ROOTDRIVE%\%APP_DATA%\Microsoft\Proof
goto SetCusIni

:Off97
Rem Office97 �w�w��
set CustomDicPath=%ROOTDRIVE%\Office97
goto SetCusIni

:Off95
Rem Office95 �w�w��
Set CustomDicPath=%ROOTDRIVE%\Office95

:SetCusIni
Rem �ھڭ�h���ܧ�ϥΪ̦ۭq�ؿ����ءC
Rem 
..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "UserDictionaryPath1" "%CustomDicPath%\Custom.Dic"

Set CustomDicPath=

Rem 
Rem ���\�פ�C
Rem

Echo. 
Echo Visio 5.0 �h�ϥΪ����ε{���վ㧹���C
Echo.
Pause

:Done


