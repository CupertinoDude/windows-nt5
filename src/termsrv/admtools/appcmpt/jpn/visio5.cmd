@Echo Off

Cls
Rem #########################################################################

Rem
Rem %ROOTDRIVE% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem ���W�X�g������ Visio ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B
Rem Visio �̕����̃o�[�W���� : Standard/Technical/Professional
Rem

Regedit /a Visio.tmp HKEY_LOCAL_MACHINE\SOFTWARE\Visio
For /f "tokens=*" %%i in ('findstr /i \[HKEY_LOCAL_MACHINE\\SOFTWARE\\Visio\\.*\\5.0\] Visio.tmp') do set VisioVer=%%i
Set VisioVer=%VisioVer:[HKEY_LOCAL_MACHINE\SOFTWARE\Visio\Visio =%
Set VisioVer=%VisioVer:\5.0]=%
Del Visio.tmp

..\ACRegL %Temp%\Vso.cmd VSO5INST "HKLM\SOFTWARE\Visio\Visio %VisioVer%\5.0" "InstallDir" ""
If Not ErrorLevel 1 Goto Cont0

Rem
Rem �C���X�g�[�����ꂽ�o�[�W�����̎擾���ł��Ȃ��ꍇ
Rem 

Echo.
Echo ���W�X�g������ Visio 5.0 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Visio 5.0 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g���Ď��s���Ă��������B
Echo.
Pause
Goto Done

Rem
Rem VSO5INST �̕ϐ��� Visio �̃C���X�g�[�� �f�B���N�g���ɐݒ肵�܂��B
Rem
:Cont0
Call %Temp%\Vso.cmd
Del %Temp%\Vso.cmd >NUL: 2>&1

Rem
Rem �ǂ̃o�[�W�������C���X�g�[������Ă��邩�\�����܂��B
Rem 
Echo.
Echo �A�v���P�[�V�������ݒ�� Visio %VisioVer% �o�[�W�����ł��邱�Ƃ����o����܂����B
Echo.

Rem
Rem �ۑ�����f�B���N�g�����C���X�g�[�����郆�[�U�[�� [�}�C �h�L�������g] �ł͂Ȃ��A
Rem ���[�U�[���Ƃ� [�}�C �h�L�������g] �ɐݒ肵�܂��B
Rem

..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "DrawingsPath" "%ROOTDRIVE%\%MY_DOCUMENTS%"

Rem
Rem �J�X�^�������̊Ǘ�
Rem ����o�[�W������ Office ���C���X�g�[������Ă���ꍇ�A Visio.ini �̃G���g���� Office �� custom.dic ���w�������悤�ɐݒ肵�܂��B
Rem ���̂ق��̏ꍇ�AAPP_DATA �ɐݒ肵�܂��B
Rem

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\9.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off2000

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

Rem �����ɂ��ǂ蒅�����ꍇ�A�ǂ̃o�[�W������ Office ���C���X�g�[������Ă��܂���B
Set CustomDicPath=%ROOTDRIVE%\%APP_DATA%
goto SetCusIni

:Off2000
Rem Office 2000 ���C���X�g�[������Ă��܂��B
set CustomDicPath=%ROOTDRIVE%\%APP_DATA%\Microsoft\Proof
goto SetCusIni

:Off97
Rem Office97 ���C���X�g�[������Ă��܂��B
set CustomDicPath=%ROOTDRIVE%\Office97
goto SetCusIni

:Off95
Rem Office95 ���C���X�g�[������Ă��܂��B
Set CustomDicPath=%ROOTDRIVE%\Office95

:SetCusIni
Rem �|���V�[�ɏ]���AVisio.ini �ɂ��郆�[�U�[�̃J�X�^������
Rem �G���g����ύX���܂��B
..\Aciniupd /e "%VSO5INST%\System\Visio.ini" "Application" "UserDictionaryPath1" "%CustomDicPath%\Custom.Dic"

Set CustomDicPath=

Rem 
Rem �������b�Z�[�W�ł��B
Rem

Echo. 
Echo Visio 5.0 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Echo.
Pause

:Done
