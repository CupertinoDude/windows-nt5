
@Echo Off

Rem
Rem ����:  ���̃X�N���v�g�̒��� CACLS �R�}���h�� NTFS
Rem �t�H�[�}�b�g�̃p�[�e�B�V�����ł̂ݗL���ł��B
Rem

Rem #########################################################################

Rem
Rem Corel Office 7 �f�B���N�g�������[�U�[�� Profile ����폜���܂��B
Rem �ŏ��ɁACorel Office 7 �̃t�H���_���m���� All Users Profile �ɃR�s�[�����悤�ɁA
Rem Execute ���[�h�ɖ߂��܂��B
Rem

Rem ���ݎ��s���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B

ChgUsr /query > Nul:
if ErrorLevel 101 Goto Begin1
Set __OrigMode=Install
Change User /Execute > Nul:
:Begin1


Rem ���̃��[�h���C���X�g�[�� ���[�h�������ꍇ�A�C���X�g�[�� ���[�h�֖߂��܂��B
If "%__OrigMode%" == "Install" Change User /Install > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem %RootDrive% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rmdir "%USER_START_MENU%\Corel Office 7" /Q >Nul: 2>&1


Rem
Rem key �t�@�C����ҏW������@�����[�U�[�ɐ������܂��B
Rem


if "%RootDrive%"=="w:" goto PostDriveChange
if "%RootDrive%"=="W:" goto PostDriveChange

Echo   �z�[���f�B���N�g���� coffice7.key �t�@�C���ɐݒ肵�Ă��������B
Echo.
Echo   �ȉ��̎菇�ɏ]���Ă�������:
Echo     1) �z�[���f�B���N�g���Ɋ��蓖�Ă��h���C�u������ ASCII �R�[�h�����̕\��
Echo        �T���Ă��������B�z�[���f�B���N�g���� %RootDrive% �ł��B
Echo.
Echo        A = 61   E = 65   I = 69   M = 6D   Q = 71   U = 75   Y = 79
Echo        B = 62   F = 66   J = 6A   N = 6E   R = 72   V = 76   Z = 7A
Echo        C = 63   G = 67   K = 6B   O = 6F   S = 73   W = 77   
Echo        D = 64   H = 68   L = 6C   P = 70   T = 74   X = 78
Echo.
Echo     2) ���������N�����ꂽ��A���ׂĂ� �R�[�h 77 ���菇 1 ��
Echo        �m�F�����R�[�h�ɒu�������Ă��������B
Echo        ����: �u��������Ƃ��ɗ]���ȃX�y�[�X�����Ȃ��悤�ɒ��ӂ��Ă��������B
Echo     3) �t�@�C����ۑ����ă��������I�����܂��B���̃X�N���v�g�͑��s����܂��B
Echo.
Pause

NotePad "%SystemRoot%\Application Compatibility Scripts\Install\coffice7.key"

:PostDriveChange


Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin2
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin2

regini COffice7.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem �C���X�g�[���ꏊ���� iBase �f�[�^�x�[�X���R�s�[���郍�O�I�� �X�N���v�g��ύX���܂��B
Rem




..\ACRegL %Temp%\COffice7.Cmd COffice7Loc "HKLM\Software\PerfectOffice\Products\WordPerfect\7" "ExeLocation" "StripChar\2"

If ErrorLevel 1 Goto InstallError

Call %Temp%\COffice7.Cmd 
Del %Temp%\COffice7.Cmd >Nul: 2>&1


..\acsr "#COFFICE7INST#" "%COffice7Loc%\\" ..\Logon\Template\cofc7usr.Cmd ..\Logon\cofc7usr.Cmd
If ErrorLevel 1 Goto InstallError

goto PostInstallError
:InstallError

Echo.
Echo ���W�X�g������ Corel Office 7 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Corel Office 7 ���C���X�g�[��������ɂ��̃X�N���v�g�����s���Ă��邩
Echo ������x�m�F���Ă��������B
Echo.
Pause
Goto Done

:PostInstallError

Rem #########################################################################

Rem 
Rem WordPerfect �̃e���v���[�g��ǂݎ���p�ɕύX���܂��B
Rem ����ɂ��A���[�U�[�͕ύX����O�ɃR�s�[���Ȃ���΂Ȃ�܂���B
Rem ����ɁA���ꂼ��̃��[�U�[�Ɍl�p�̃e���v���[�g
Rem �f�B���N�g����^������@������܂��B
Rem

attrib +r %COffice7Loc%\Template\*wpt /s >Nul: 2>&1

Rem #########################################################################

Rem
Rem COfc7Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I COfc7Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call COfc7Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################


Rem
Rem ���݂��Ȃ���� "Personal" �f�B���N�g�����쐬���܂��B
Rem �Ǘ��҂��ȉ��̏������蓮�ōs�����߁A�����Ńf�B���N�g�����쐬���Ă���
Rem �K�v������܂��B
Rem  

If Not Exist "%RootDrive%\Personal" Md "%RootDrive%\Personal"

Rem #########################################################################

cls
Echo.
Echo    ���̎菇�ɏ]���� Quattro Pro �̊���̃f�B���N�g����
Echo    �蓮�ŕύX����K�v������܂�:
Echo      1) �R�}���h�s�� 'change user /install' ����͂��܂��B
Echo      2) Quattro Pro ���N�����܂��B
Echo      3) [�ҏW] ���j���[���� [�����ݒ�] ��I�����܂��B
Echo      4) [�t�@�C���I�v�V����] �^�u�ֈړ����܂��B
Echo      5) �t�H���_�� %RootDrive%\Personal �ɕύX���܂��B
Echo      6) �v���O�������I�����܂��B
Echo      7) �R�}���h�s�� 'change user /execute' ����͂��܂��B
Echo.
pause

Echo.
Echo   Corel Office 7 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Corel Office 7 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
