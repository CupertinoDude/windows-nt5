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
Rem �X�^�[�g�A�b�v ���j���[���� Find Fast ���폜���܂��BFind Fast �̓��\�[�X
Rem ���W���I�Ɏg�p���A�V�X�e���̃p�t�H�[�}���X�ɉe�����y�ڂ��܂��B
Rem


If Not Exist "%COMMON_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" Goto Cont1
Del "%COMMON_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%USER_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" Goto Cont2
Del "%USER_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem ���[�U�[���������[�U�[�f�B���N�g���Ɉړ����܂��B
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj95.Key msproj95.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" msproj95.tmp msproj95.Key
Del msproj95.tmp >Nul: 2>&1
regini msproj95.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���̃t�@�C���̓��O�I�����ɓǂݎ���܂��B
Rem ���[�U�[�ɃA�N�Z�X��^���܂��B

Cacls ..\Logon\Template\prj95usr.key /E /P "Authenticated Users":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem proj95Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I prj95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call prj95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2


Rem #########################################################################

Echo.
Echo   �Ǘ��҂͈ȉ��̎菇�ŁA�X�̃��[�U�[�ɌŗL�̊���̃f�B���N�g����
Echo   �ݒ肷�邱�Ƃ��ł��܂�:
Echo.
Echo    1) [�X�^�[�g] �{�^�����}�E�X�̉E�{�^���ŃN���b�N���܂��B
Echo    2) �|�b�v�A�b�v ���j���[���� [�G�N�X�v���[�� - All Users] ���N���b�N
Echo       ����ƃG�N�X�v���[�����\������܂��B
Echo    3) �E���̃E�B���h�E�� [�v���O����] �t�H���_���_�u���N���b�N���܂��B
Echo    4) �E���̃E�B���h�E�� [Microsoft Project] �A�C�R�����}�E�X�̉E�{�^��
Echo       �ŃN���b�N���܂��B
Echo    5) �|�b�v�A�b�v ���j���[���� [�v���p�e�B] ���N���b�N���܂��B
Echo    6) [�V���[�g�J�b�g] �^�u���N���b�N���āA[��ƃt�H���_] �̃G���g���� & Echo       �ύX���A[OK] ���N���b�N���܂��B
Echo.
Echo    ����: %RootDrive% �͊e���[�U�[�� �z�[�� �f�B���N�g���Ɋ��蓖�Ă��Ă��܂��B
Echo          [�J�n�t�H���_] �ɂ� "%RootDrive%\My Documents" �𐄏����܂��B
Echo.
Pause

Rem #########################################################################

Echo.
Echo   Project 95 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Project 95 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
