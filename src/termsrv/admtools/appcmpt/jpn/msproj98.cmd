
@Echo Off

Rem #########################################################################

Rem
Rem %RootDrive% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem ���W�X�g������ Project 98 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\Proj98.Cmd PROJINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "OfficeBin" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ Project 98 �̃C���X�g�[���ꏊ���擾�ł��܂���ł����B
Echo Project 98 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem Office 97 ���C���X�g�[������Ă���ꍇ�AProject 98 �̃C���X�g�[�� �X�N���v�g��
Rem �e���v���[�g�����݂̃��[�U�[ �f�B���N�g���Ɉړ����܂����B
Rem �e���v���[�g���O���[�o���ȏꏊ�ɒu���AProj98Usr.cmd ���e���v���[�g�� & Echo �e���[�U�[�̃f�B���N�g���ɖ߂��܂��B
Rem






If NOT Exist "%RootDrive%\Office97\Template\Microsoft Project"  goto skip10
If Exist  "%PROJINST%\..\Templates\Microsoft Project" goto skip10
xcopy "%RootDrive%\Office97\Template\Microsoft Project" "%PROJINST%\..\Templates\Microsoft Project" /E /I /K > Nul: 2>&1

:skip10






If NOT Exist "%RootDrive%\Office97\Template\Microsoft Project Web"  goto skip11
If Exist  "%PROJINST%\..\Templates\Microsoft Project Web" goto skip11
xcopy "%RootDrive%\Office97\Template\Microsoft Project Web" "%PROJINST%\..\Templates\Microsoft Project Web" /E /I /K > Nul: 2>&1

:skip11

Rem #########################################################################

Rem
Rem Global.mpt �t�@�C����ǂݎ���p�ɂ��܂��B
Rem �������� Project ���N������ŏ��̃��[�U�[�� ACL ��ύX���܂��B
Rem

if Exist "%PROJINST%\Global.mpt" attrib +r "%PROJINST%\Global.mpt"


Rem #########################################################################

Rem
Rem Office 97 �ɂ���čX�V���ꂽ�V�X�e�� DLL �ւ̓ǂݎ��A�N�Z�X����S����
Rem �^���܂��B
Rem
If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R > NUL: 2>&1

Rem #########################################################################

Rem
Rem MsForms.Twd �t�@�C�����쐬���܂��B���̃t�@�C����
Rem Powerpoint �� Excel �̃A�h�C�� ([�t�@�C��]/[���O��t���ĕۑ�] ��
Rem HTML�t�H�[�}�b�g�Ȃ�) ���K�v�Ƃ���t�@�C���ł��B  �ǂ��炩�̃v���O����
Rem �����s�����ƁA�Y���̃t�@�C����K�v�ȃf�[�^�����t�@�C���Œu�������܂��B
Rem
If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul:
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F > Nul: 2>&1
:Cont2

Rem #########################################################################

Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���� Find Fast ���폜���܂��B
Rem Find Fast �̓��\�[�X���W���I�Ɏg�p���A�V�X�e�� �p�t�H�[�}���X��
Rem �e�����y�ڂ��܂��B
Rem


If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk"

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
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj98.Key msproj98.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" msproj98.tmp msproj98.Key
Del msproj98.tmp >Nul: 2>&1
regini msproj98.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem ���ۂ̃C���X�g�[�� �f�B���N�g���𔽉f�����邽�� proj97Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#INSTDIR#" "%PROJINST%" ..\Logon\Template\prj98Usr.Cmd ..\Logon\prj98Usr.Cmd

FindStr /I prj98Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call prj98Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   Project 98 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Project 98 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:done

