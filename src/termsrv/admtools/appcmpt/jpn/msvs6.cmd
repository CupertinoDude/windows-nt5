@Echo Off

Rem
Rem ����:  ���̃X�N���v�g�̒��� CACLS �R�}���h�� NTFS
Rem �t�H�[�}�b�g�̃p�[�e�B�V�����ł̂ݗL���ł��B
Rem

Rem #########################################################################
Rem
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem ���W�X�g������ Visual Studio 6.0 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\0VC98.Cmd 0VC98 "HKLM\Software\Microsoft\VisualStudio\6.0\Setup\Microsoft Visual C++" "ProductDir" ""
If Not ErrorLevel 1 Goto Cont0

Echo.
Echo ���W�X�g������ Visual Studio 6.0 �̃C���X�g�[���ꏊ���擾�ł��܂���ł����B
Echo Visual Studio 6.0 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done
:Cont0
Call %Temp%\0VC98.Cmd
Del %Temp%\0VC98.Cmd >Nul: 2>&1

Rem #########################################################################
Rem
Rem ���W�X�g�� �L�[��ύX���āA�p�X�����[�U�[�ŗL�̃f�B���N�g��
Rem ���w�������悤�ɂ��܂��B
Rem
Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msvs6.Key %temp%\msvs6.tmp
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" %temp%\msvs6.tmp %temp%\msvs6.tmp2
..\acsr "#APP_DATA#" "%APP_DATA%" %temp%\msvs6.tmp2 msvs6.key
Del %temp%\msvs6.tmp >Nul: 2>&1
Del %temp%\msvs6.tmp2 >Nul: 2>&1
regini msvs6.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################
Rem Visual Studio �A�v���P�[�V�����p�̃��[�U�[ ���O�I�� �t�@�C�����쐬���܂��B

Echo Rem >..\logon\VS6USR.Cmd

Rem #########################################################################
Rem ���[�U�[���Ƃ� Visual Studio �v���W�F�N�g �f�B���N�g�����쐬���܂��B

Echo Rem >>..\logon\VS6USR.Cmd
Echo Rem ���[�U�[���Ƃ� Visual Studio �v���W�F�N�g �f�B���N�g�����쐬���܂��B>>..\logon\VS6USR.Cmd
Echo call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%\Visual Studio Projects">>..\logon\VS6USR.Cmd
Echo Rem >>..\logon\VS6USR.Cmd


Rem #########################################################################

Rem
Rem ���W�X�g������ Visual Studio 6.0 Entreprise Edition Tools ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem ������ꍇ�A�č��o�[�W�����ł́A<VStudioPath>\Common\Tools �Ɋ܂܂�܂��B
Rem

..\ACRegL %Temp%\VSEET.Cmd VSEET "HKLM\Software\Microsoft\VisualStudio\6.0\Setup\Microsoft VSEE Client" "ProductDir" ""
If Not ErrorLevel 1 Goto VSEET0

Goto VSEETDone
:VSEET0
Call %Temp%\VSEET.Cmd
Del %Temp%\VSEET.Cmd >Nul: 2>&1

If Not Exist "%VSEET%\APE\AEMANAGR.INI" Goto VSEETDone
..\acsr "=AE.LOG" "=%RootDrive%\AE.LOG" "%VSEET%\APE\AEMANAGR.INI" "%VSEET%\APE\AEMANAGR.TMP"
If Exist "%VSEET%\APE\AEMANAGRINI.SAV" Del /F /Q "%VSEET%\APE\AEMANAGRINI.SAV"
ren "%VSEET%\APE\AEMANAGR.INI" "AEMANAGRINI.SAV"
ren "%VSEET%\APE\AEMANAGR.TMP" "AEMANAGR.INI"

Echo Rem Copy APE ini file to the user windows directory >>..\logon\VS6USR.Cmd
Echo Rem >>..\logon\VS6USR.Cmd
Echo If Exist "%RootDrive%\Windows\AEMANAGR.INI" Goto UVSEET0 >>..\logon\VS6USR.Cmd
Echo If Exist "%VSEET%\APE\AEMANAGR.INI" Copy "%VSEET%\APE\AEMANAGR.INI" "%RootDrive%\Windows\AEMANAGR.INI" >Nul: 2>&1 >>..\logon\VS6USR.Cmd
Echo Rem >>..\logon\VS6USR.Cmd
Echo :UVSEET0>>..\logon\VS6USR.Cmd

Echo Rem Copy Visual Modeler ini file to the user windows directory >>..\logon\VS6USR.Cmd
Echo Rem >>..\logon\VS6USR.Cmd
Echo If Exist "%RootDrive%\Windows\ROSE.INI" Goto UVSEET1 >>..\logon\VS6USR.Cmd
Echo If Exist "%VSEET%\VS-Ent98\Vmodeler\ROSE.INI" Copy "%VSEET%\VS-Ent98\Vmodeler\ROSE.INI" "%RootDrive%\Windows\ROSE.INI" >Nul: 2>&1 >>..\logon\VS6USR.Cmd
Echo Rem >>..\logon\VS6USR.Cmd
Echo :UVSEET1>>..\logon\VS6USR.Cmd

:VSEETDone


Rem #########################################################################

Rem
Rem VS6USR.Cmd �� UsrLogn2.Cmd �ɒǉ����܂��B
Rem

FindStr /I VS6USR %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call VS6USR.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1


Rem #########################################################################
Rem Visual foxPro �v���_�N�g�̃C���X�g�[�� �f�B���N�g�����擾���܂��B

..\ACRegL %Temp%\VFP98TMP.Cmd VFP98DIR "HKLM\Software\Microsoft\VisualStudio\6.0\Setup\Microsoft Visual FoxPro" "ProductDir" ""

Rem Visual FoxPro ���C���X�g�[������Ă��Ȃ��ꍇ�A�N���[���A�b�v �R�[�h�܂ŃX�L�b�v���܂��B
If ErrorLevel 1 goto Skip2

Rem #########################################################################

Rem
Rem ���W�X�g������J�X�^�������L�[���擾���܂��B
Rem

Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto VFP98L2
If Not Exist "%ProgramFiles(x86)%" goto VFP98L2
Set __SharedTools=Shared Tools (x86)
:VFP98L2


..\ACRegL %Temp%\VFP98TMP.Cmd VFP98DIC "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Custom Dictionaries" "1" ""
If Not ErrorLevel 1 Goto VFP98L3

Echo.
Rem ���W�X�g������l���擾�ł��܂���B�������쐬���Ă��������B
Echo.

Rem VFP98TMP.key �t�@�C�����쐬���܂��B

Echo HKEY_LOCAL_MACHINE\Software\Microsoft\%__SharedTools%\Proofing Tools\Custom Dictionaries> %Temp%\VFP98TMP.key
Echo     1 = REG_SZ "%RootDrive%\%MY_DOCUMENTS%\Custom.Dic">> %Temp%\VFP98TMP.key

Rem �l���쐬���܂��B
regini %Temp%\VFP98TMP.key > Nul:

Del %Temp%\VFP98TMP.key >Nul: 2>&1

Echo set VFP98DIC=%RootDrive%\%MY_DOCUMENTS%\Custom.Dic>%Temp%\VFP98TMP.Cmd
:VFP98L3

Call %Temp%\VFP98TMP.Cmd
Del %Temp%\VFP98TMP.Cmd >Nul: 2>&1


Rem #########################################################################
Rem Visual FoxPro �A�v���P�[�V�����p�̃��[�U�[ ���O�I�� �t�@�C�����쐬���܂��B

Echo Rem >..\logon\VFP98USR.Cmd

Rem #########################################################################
Rem ���[�U�[���Ƃ� Visual FoxPro �f�B���N�g�����쐬���܂��B

Echo Rem >>..\logon\VFP98USR.Cmd
Echo Rem ���[�U�[���Ƃ� Visual FoxPro �f�B���N�g�����쐬���܂� (VFP98)�B>>..\logon\VFP98USR.Cmd
Echo call TsMkUDir %RootDrive%\VFP98>>..\logon\VFP98USR.Cmd
Echo Rem >>..\logon\VFP98USR.Cmd

Echo Rem ���[�U�[���Ƃ� Visual FoxPro �z�z�f�B���N�g�����쐬���܂��B >>..\logon\VFP98USR.Cmd
Echo call TsMkUDir %RootDrive%\VFP98\DISTRIB>>..\logon\VFP98USR.Cmd
Echo Rem >>..\logon\VFP98USR.Cmd

Echo Rem #########################################################################>>..\logon\VFP98USR.Cmd
Echo Rem ���݂��Ȃ��ꍇ�A�J�X�^���������쐬���܂��B>>..\logon\VFP98USR.Cmd
Echo Rem >>..\logon\VFP98USR.Cmd

Echo If Exist "%VFP98DIC%" Goto VFP98L2 >>..\logon\VFP98USR.Cmd
Echo Copy Nul: "%VFP98DIC%" >Nul: 2>&1 >>..\logon\VFP98USR.Cmd

Echo :VFP98L2 >>..\logon\VFP98USR.Cmd

Rem #########################################################################
Rem Visual foxPro ���i�̃C���X�g�[�� �f�B���N�g�����擾���܂��B

..\ACRegL %Temp%\VFP98TMP.Cmd VFP98DIR "HKLM\Software\Microsoft\VisualStudio\6.0\Setup\Microsoft Visual FoxPro" "ProductDir" ""
If Not ErrorLevel 1 Goto VFP98L4

Del ..\logon\VFP98USR.Cmd >Nul: 2>&1

Echo.
Echo ���W�X�g������ Visual FoxPro �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo ���̃A�v���P�[�V�������C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Skip2

:VFP98L4
Call "%Temp%\VFP98TMP.Cmd"
Del "%Temp%\VFP98TMP.Cmd"

Rem #########################################################################
Rem ���̃L�[�� WZSETUP.INI �t�@�C���ɐݒ肵�܂��B
Rem 
If Exist "%VFP98DIR%\WZSETUP.INI" Goto VFP98L5
Echo [Preferences] >"%VFP98DIR%\WZSETUP.INI" 
Echo DistributionDirectory=%RootDrive%\VFP98\DISTRIB >>"%VFP98DIR%\WZSETUP.INI" 

:VFP98L5


Rem #########################################################################
Rem
Rem ���W�X�g�� �L�[��ύX���āA�p�X�����[�U�[�ŗL�̃f�B���N�g��
Rem ���w�������悤�ɂ��܂��B
Rem


Rem �ŏ��� VFP98TMP.key �t�@�C�����쐬���܂��B

Echo HKEY_CURRENT_USER\Software\Microsoft\VisualFoxPro\6.0\Options> %Temp%\VFP98TMP.key
Echo     DEFAULT = REG_SZ "%RootDrive%\VFP98">> %Temp%\VFP98TMP.key
Echo     SetDefault = REG_SZ "1">> %Temp%\VFP98TMP.key
Echo     ResourceTo = REG_SZ "%RootDrive%\VFP98\FOXUSER.DBF">> %Temp%\VFP98TMP.key
Echo     ResourceOn = REG_SZ "1">> %Temp%\VFP98TMP.key

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto VFP98L6
Set __OrigMode=Exec
Change User /Install > Nul:
:VFP98L6

regini %Temp%\VFP98TMP.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Del %Temp%\VFP98TMP.key >Nul: 2>&1


Rem #########################################################################

Rem
Rem VFP98USR.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I VFP98USR %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call VFP98USR.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2

If Exist "%Temp%\VFP98TMP.Cmd" Del "%Temp%\VFP98TMP.Cmd"

Rem #########################################################################

Rem
Rem �^�[�~�i�� �T�[�r�X�̃��[�U�[�Ɋi�[�f�B���N�g���̕ύX�A�N�Z�X����^���āA
Rem ���[�U�[�� Visual Component Manager ���g�p�ł���悤�ɂ��܂��B
Rem

If Exist "%SystemRoot%\msapps\repostry" cacls "%SystemRoot%\msapps\repostry" /E /G "Terminal Server User":C >NUL: 2>&1


Rem #########################################################################
Echo.
Echo   Visual Studio 6.0 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Visual Studio 6.0 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:done
