@Echo Off

Cls

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Echo.
Echo ���̃X�N���v�g�����s����O�ɁA�Ǘ��҂� Access �̍�ƃf�B���N�g����
Echo ���[�U�[��Office �l�f�B���N�g���ɕύX����K�v������܂��B
Echo.
Echo ���ɕύX���������Ă���ꍇ�A�����L�[�������đ��s���Ă��������B
Echo.
Echo �ύX����ɂ́A�ȉ��̍�Ƃ��s���Ă��������B
Echo    Microsoft Access ���N�����A[�\��] ���j���[�� [��Ə�Ԃ̐ݒ�] ��I�����܂��B
Echo    [�f�[�^�x�[�X �f�B���N�g��] �� "%RootDrive%\OFFICE43" �ɕύX���܂��B
Echo    Microsoft Access ���I�����܂��B
Echo.
Echo ����: [�\��] ���j���[��\������ɂ́A�V�����f�[�^�x�[�X���쐬���� & Echo       �K�v������܂��B
Echo.
Echo ��̎菇���I��������A�C�ӂ̃L�[�������đ��s���Ă�������...

Pause > NUL:

Echo.
Echo Microsoft Office 4.3 �� "%SystemDrive%\MSOFFICE" �ȊO�̃f�B���N�g���ɃC���X�g�[��
Echo ���Ă���ꍇ�́AOfc43ins.cmd ���X�V����K�v������܂��B
Echo.
Echo �����L�[�������� Ofc43ins.cmd �̍X�V���J�n���܂�...
Echo.
Pause > NUL:
Notepad Ofc43ins.cmd
Pause

Call ofc43ins.cmd

..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.cmd ..\Logon\ofc43usr.cmd
..\acsr "#SYSTEMROOT#" "%SystemRoot%" ..\Logon\Template\ofc43usr.key ..\Logon\Template\ofc43usr.bak
..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.bak ..\Logon\ofc43usr.key
Del /F /Q ..\Logon\Template\ofc43usr.bak

Rem
Rem ����:  ���̃X�N���v�g�̒��� CACLS �R�}���h�� NTFS
Rem �t�H�[�}�b�g�̃p�[�e�B�V�����ł̂ݗL���ł��B
Rem

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

regini Office43.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem
Rem Office 4.3 ��INI �t�@�C�����X�V���܂��B
Rem

Rem Office �c�[���o�[����N�������Ƃ��� Excel �p�� msoffice.ini �ɍ�Ɨp
Rem �f�B���N�g����ݒ肵�܂��BOffice �c�[���o�[�̕W���\���ł� Excel �� Word ��
Rem ���̈ʒu�ɒu����܂��B���݂��Ȃ��ꍇ�� msoffice.ini �� %SystemRoot% �̉���
Rem �쐬����܂��B

..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp1" "1,1,"
..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp2" "2,1,,%RootDrive%\office43"

..\Aciniupd /e "Word6.ini" "Microsoft Word" USER-DOT-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Word6.ini" "Microsoft Word" WORKGROUP-DOT-PATH "%OFC43INST%\WINWORD\TEMPLATE"
..\Aciniupd /e "Word6.ini" "Microsoft Word" INI-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Word6.ini" "Microsoft Word" DOC-PATH "%RootDrive%\OFFICE43"
..\Aciniupd /e "Word6.ini" "Microsoft Word" AUTOSAVE-PATH "%RootDrive%\OFFICE43"

..\Aciniupd /e "Excel5.ini" "Microsoft Excel" DefaultPath "%RootDrive%\OFFICE43"
..\Aciniupd /e "Excel5.ini" "Spell Checker" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

..\Aciniupd /k "Msacc20j.ini" Libraries "WZTABLE.MDA" "%RootDrive%\OFFICE43\ACCESS\WZTABLE.MDA"
..\Aciniupd /k "Msacc20j.ini" Libraries "WZLIB.MDA" "%RootDrive%\OFFICE43\ACCESS\WZLIB.MDA"
..\Aciniupd /k "Msacc20j.ini" Libraries "WZBLDR.MDA" "%RootDrive%\OFFICE43\ACCESS\WZBLDR.MDA"
..\Aciniupd /e "Msacc20j.ini" Options "SystemDB" "%RootDrive%\OFFICE43\ACCESS\System.MDA"

Rem
Rem WIN.INI ���X�V���܂��B
Rem

..\Aciniupd /e "Win.ini" "MS Proofing Tools" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

Rem
Rem Artgalry �t�H���_�̃A�N�Z�X����ύX���܂��B
Rem

cacls "%SystemRoot%\Msapps\Artgalry" /E /G "Terminal Server User":F > NUL: 2>&1

Rem
Rem MSQuery �t�H���_�̃A�N�Z�X����ύX���܂��B
Rem

cacls "%SystemRoot%\Msapps\MSQUERY" /E /G "Terminal Server User":C > NUL: 2>&1

Rem
Rem Msacc20j.ini ���Ǘ��҂� Windows �f�B���N�g���ɃR�s�[���܂��B�Ǘ��҂̃t�@�C���͌Â����߂ł��B
Rem

Copy "%SystemRoot%\Msacc20j.ini" "%UserProfile%\Windows\" > NUL: 2>&1

Rem �_�~�[ �t�@�C�����쐬���āA�C���X�g�[�������W�X�g�� �L�[��K�p���Ȃ��悤�ɂ��܂��B

Copy NUL: "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1
attrib +h "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1

FindStr /I Ofc43Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto cont2
Echo Call Ofc43Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:cont2

Echo.
Echo �ύX��L���ɂ��邽�߂ɁA�Ǘ��҂̓��O�I�t���Ă���ēx���O�I������K�v������܂��B
Echo   �ύX��L���ɂ��邽�߂ɁA�Ǘ��҂̓��O�I�t���Ă���ēx���O�I������
Echo   �N���b�v �A�[�g �M�������[������������K�v������܂�:
Echo       Word �����s���� [�I�u�W�F�N�g�̑}��] ��I�����܂��B
Echo       [Microsoft �N���b�v�A�[�g �M�������[] ��I�����܂��B
Echo       [OK] ���N���b�N���ĕ\������Ă���N���b�v�A�[�g���C���|�[�g���܂��B
Echo       �N���b�v�A�[�g �M�������[�� Word ���I�����܂��B
Echo.
Echo Microsoft Office 4.3 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B.
Echo.
Pause

:Done
