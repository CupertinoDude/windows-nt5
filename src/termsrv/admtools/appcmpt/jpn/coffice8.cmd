@Echo off
Cls
Rem
Echo �Ǘ��җp Corel WordPerfect Suite 8 �C���X�g�[�� �X�N���v�g
Echo.
Echo �l�b�g �Z�b�g�A�b�v�Ƃ� netadmin �t�H���_���� netsetup.exe ���s�����̂ł��B
Echo �m�[�h �Z�b�g�A�b�v�Ƃ́A�l�b�g �Z�b�g�A�b�v��A�ŏ��̃��O�C���̎��ɍs�����̂ł��B
Echo **���̃X�N���v�g�̓l�b�g �Z�b�g�A�b�v�̌�Ŏ��s���āA�m�[�h **
Echo ** �Z�b�g�A�b�v�̌�ōēx���s����K�v������܂��B ** & Echo (���ɏd�v�ł��B�Ǘ��҂������s���Ă��������B) 
Echo.
Echo �����L�[�������Ƒ��s���܂�...
Pause > Nul:
cls
Echo.
Echo Corel WordPerfect Suite 8 �̃C���X�g�[���́A�K�� Netsetup.exe �g���� & Echo �s���Ă��������B
Echo.
Echo Netsetup.exe ���g�킸�ɃC���X�g�[�������Ȃ�΁A�����ɃX�N���v�g���I�����Ă��� & Echo ����(Ctrl+C �������Ă�������)�B
Echo �����Ĉȉ��̎菇�ɏ]���Ă�����x�C���X�g�[�����s���Ă��������B
Echo   �R���g���[���p�l�����g���� WordPerfect Suite 8 ���A���C���X�g�[�����܂��B
Echo   �R���g���[���p�l�����J���āA[�A�v���P�[�V�����̒ǉ��ƍ폜] ���N���b�N���܂��B
Echo   Corel WordPerfect Suite 8 �� CD-ROM ��NetAdmin �f�B���N�g���̉��ɂ��� & Echo   Netsetup.exe ��I�����Ď��s���܂��B
Echo   Corel WordPerfect Suite 8 �� Net Setup ���������܂��B
Echo.
Echo ���� Netsetup.exe ���g���ăC���X�g�[�����Ă���ꍇ�́A& Echo �����L�[�������đ��s���Ă�������...

Pause > NUL:

Echo.
Echo Corel WordPerfect Suite �̃l�b�g���[�N �t�@�C���� "D:\Corel" 
Echo �ȊO�̏ꏊ�ɃC���X�g�[�������ꍇ�A�Ǘ��҂� Cofc8ins.cmd ��ҏW����
Echo "D:\Corel" ���A�t�@�C�����C���X�g�[�������f�B���N�g���ɕύX����K�v��
Echo ����܂��B
Echo.
Echo �����L�[�������� Cofc8ins.cmd �̕ҏW���J�n���܂�...

Pause > NUL:

Notepad Cofc8ins.cmd

Echo.
Pause

Call Cofc8ins.cmd

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


..\acsr "#WPSINST#" "%WPSINST%" Template\Coffice8.key Coffice8.key
..\acsr "#WPSINST#" "%WPSINST%" ..\Logon\Template\Cofc8usr.cmd %temp%\Cofc8usr.tmp
..\acsr "#WPSINSTCMD#" "%WPSINST%\Suite8\Corel WordPerfect Suite 8 Setup.lnk" %temp%\Cofc8usr.tmp ..\Logon\Cofc8usr.cmd
Del %temp%\Cofc8usr.tmp >Nul: 2>&1

FindStr /I Cofc8Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip0
Echo Call Cofc8Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip0

Echo �d�v: �m�[�h �Z�b�g�A�b�v���J�n���܂�����?
Echo �d�v: �l�b�g �Z�b�g�A�b�v��������������ł���ꍇ�A�������m�[�h �Z�b�g�A�b�v��
Echo �d�v: �J�n���Ă��������B
Echo �d�v: ���̎菇�ɏ]���ăm�[�h �Z�b�g�A�b�v���J�n���Ă�������:
Echo �d�v:   1. Ctrl+C �������Ă��̃X�N���v�g���I�����܂��B
Echo �d�v:   2. ���O�I�t���Ă���Ǘ��҂Ƃ��ă��O�I�����܂��B
Echo �d�v:  3. [�A�v���P�[�V�����̒ǉ��ƍ폜] ����A�v���O�����̒ǉ��̋@�\���g���āA
Echo �d�v:     (�܂��� "change user /install" �ŃC���X�g�[�� ���[�h�ɐ؂�ւ���)
Echo �d�v:     "\corel\Suite 8\Corel" �ɂ��� WordPerfect Suite 8 �Z�b�g�A�b�v�� & Echo �d�v:     �V���[�g�J�b�g���N�����܂��B
Echo �d�v:  4. �Z�b�g�A�b�v���A�C���X�g�[����̑I�����A%RootDrive% ��I�����܂��B
Echo �d�v:  5. �m�[�h �Z�b�g�A�b�v�I����A���̃X�N���v�g���Ď��s���܂��B

Rem �Ǘ��҂ɂ�� Corel WordPerfect Suite 8 �̃m�[�h �Z�b�g�A�b�v��
Rem ������ɁA���̃X�N���v�g�����s����K�v������܂��B
Rem.
Rem �Ǘ��҂��m�[�h �Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�ACtrl+C �������āA�X�N���v�g
Rem ���I�����Ă��烍�O�I�t���Ă��������B�Ǘ��҂Ƃ��ă��O�I���� Corel WordPerfect
Rem Suite �̃m�[�h �Z�b�g�A�b�v�����s���āA�A�v���P�[�V���������[�U�[�̃z�[��
Rem �f�B���N�g���ł��� %RootDrive% �ɃC���X�g�[�����Ă��������B
Rem.
Echo ���Ƀm�[�h �Z�b�g�A�b�v���������Ă���ꍇ�A�����L�[�������đ��s���Ă�������...

Pause > NUL:


If Not Exist "%COMMON_START_MENU%\Corel WordPerfect Suite 8" Goto skip1
Move "%COMMON_START_MENU%\Corel WordPerfect Suite 8" "%USER_START_MENU%\" > NUL: 2>&1


:skip1


If Not Exist "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" Goto skip2
Move "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" "%USER_STARTUP%\" > NUL: 2>&1

:skip2

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B

Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:

:Begin
regini COffice8.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem IDAPI �t�H���_�̃t�@�C�����폜���܂��B
Rem ���̃X�N���v�g�ɂ��� CACLS �R�}���h�� NTFS �t�H�[�}�b�g�̃p�[�e�B�V������ł̂ݗL���ł��B

Cacls "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" /E /T /G "Authenticated Users":C > Nul: 2>&1
Move "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" "%WPSINST%\Suite8\Shared\Idapi\" > NUL: 2>&1
Del /F /Q "%UserProfile%\Corel\Suite8\Shared\Idapi"

Echo Corel WordPerfect Suite 8 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪 & Echo �������܂����B
Echo.
Echo �Ǘ��҂̓m�[�h �Z�b�g�A�b�v�����t�@�C�����쐬���ăZ�b�g�A�b�v�̍\����
Echo �Ǘ����邱�Ƃ��ł��܂��B�ڍׂȏ��̓I�����C�� �h�L�������g��ǂނ�
Echo Corel �� Web �T�C�g���Q�Ƃ��Ă��������B
Echo   http://www.corel.com/support/netadmin/admin8/Installing_to_a_client.htm
Echo.

Pause

:Done


