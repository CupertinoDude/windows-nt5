@Echo Off

Rem #########################################################################

Rem
Rem CMD �g���@�\���L���ɂȂ��Ă��邩�m�F���܂��B
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c netcom40.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

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
Rem NetScape �̃o�[�W�������擾���܂� (4.5x �� 4.0x �Ƃ͈قȂ���@�ōs���܂�)�B
Rem

..\ACRegL "%Temp%\NS4VER.Cmd" NS4VER "HKLM\Software\Netscape\Netscape Navigator" "CurrentVersion" "STRIPCHAR(1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ Netscape Communicator 4 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Communicator ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\NS4VER.Cmd"
Del "%Temp%\NS4VER.Cmd" >Nul: 2>&1

if /i "%NS4VER%" LSS "4.5 " goto NS40x

Rem #########################################################################
Rem Netscape 4.5x

Rem
Rem ���W�X�g������ Netscape Communicator 4.5 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem �G���[ ���b�Z�[�W��\�����܂��B
Rem

..\ACRegL "%Temp%\NS45.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���W�X�g������ Netscape Communicator 4.5 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Communicator ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont1
Call "%Temp%\NS45.Cmd"
Del "%Temp%\NS45.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ����� NetScape �̃��[�U�[ �f�B���N�g���𔽉f�����邽�� Com40Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#NSUSERDIR#" "%ProgramFiles%\Netscape\Users" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS40INST#" "%NS40INST%" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.tm2
..\acsr "#NS4VER#" "4.5x" ..\Logon\Com40Usr.tm2 ..\Logon\Com40Usr.Cmd

Rem #########################################################################

Rem
Rem "�N�C�b�N�N��" �̃A�C�R���� Netscape �̃C���X�g�[�� �f�B���N�g���ɃR�s�[���āA
Rem �A�C�R�������ꂼ��̃��[�U�[ �v���t�@�C���̃f�B���N�g���ɃR�s�[�ł���悤�ɂ��܂��B
Rem

If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" "%NS40INST%"

goto DoUsrLogn

:NS40x
Rem #########################################################################
Rem Netscape 4.0x

Rem
Rem ���W�X�g������ Netscape Communicator 4 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem �G���[ ���b�Z�[�W��\�����܂��B
Rem

..\ACRegL "%Temp%\NS40.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" ""
If Not ErrorLevel 1 Goto Cont2
Echo.
Echo ���W�X�g������ Netscape Communicator 4 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Communicator ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont2
Call "%Temp%\NS40.Cmd"
Del "%Temp%\NS40.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ����̃v���t�@�C�����Ǘ��҂̃z�[�� �f�B���N�g�����珊��̏ꏊ�փR�s�[��
Rem �܂��B���̃v���t�@�C���́A���O�I�����Ɋe���[�U�[�̃f�B���N�g���փR�s�[
Rem ����܂��B�S�ʗp�̊���̃v���t�@�C�������ɑ��݂���ꍇ�́A�㏑�������
Rem ����B�Ǘ��҂���ł��̃X�N���v�g�����s���āA�Ǘ��Ҍl�̏���S�ʗp��
Rem ����̃v���t�@�C���Ɉړ����邱�Ƃ��ł��܂��B
Rem

If Exist %RootDrive%\NS40 Goto Cont3
Echo.
Echo %RootDrive%\NS40 �Ɋ���̃v���t�@�C����������܂���BUser Profile Manager
Echo ���g���āA"Default" �Ƃ������O�̒P��̃v���t�@�C�����쐬���Ă��������B�v��
Echo �t�@�C���̃p�X�̓��͂����߂�ꂽ�ꍇ�A��L�̃p�X���g�p���Ă��������B���O��
Echo �d�q���[�����̓��͂͂��ׂċ󔒂ɂ��Ă��������B���Ƀv���t�@�C�������݂����
Echo ���͂��ׂč폜���Ă��������B�����̎菇������������ŁA���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done
 
:Cont3
If Exist "%NS40INST%\DfltProf" Goto Cont4
Xcopy "%RootDrive%\NS40" "%NS40INST%\DfltProf" /E /I /K >NUL: 2>&1
:Cont4

Rem #########################################################################

Rem 
Rem �X�^�[�g ���j���[�ɂ��� User Profile Manager �̃V���[�g�J�b�g�����ʃ��[�U�[
Rem �̃A�N�Z�X������菜���܂��B����ň�ʃ��[�U�[�͐V�������[�U�[ �v���t�@�C����ǉ�����
Rem ���Ƃ��ł��Ȃ��Ȃ�܂����A�Ǘ��҂� User Profile Manager �����s�ł��܂��B
Rem

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" Goto Cont5
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont5

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" Goto Cont6
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont6

Rem #########################################################################

Rem
Rem ���ۂ̃C���X�g�[�� �f�B���N�g���𔽉f�����邽�� Com40Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#PROFDIR#" "%NS40INST%\DfltProf" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS4VER#" "4.0x" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.Cmd

:DoUsrLogn

del ..\Logon\Com40Usr.tmp >Nul: 2>&1
del ..\Logon\Com40Usr.tm2 >Nul: 2>&1

FindStr /I Com40Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Com40Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Netscape Communicator ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Netscape Communicator 4 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪 & Echo �������܂����B
Pause

:done

