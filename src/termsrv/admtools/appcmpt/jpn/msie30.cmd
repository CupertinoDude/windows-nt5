@Echo Off
Rem
Rem ���̃X�N���v�g�ɂ�� Microsoft Internet Explorer 3.x ��
Rem �C���X�g�[�����X�V���āA�L���b�V�� �f�B���N�g���A����
Rem �f�B���N�g���Acookies�t�@�C���̃p�X�����[�U�[�̃z�[��
Rem �f�B���N�g���֕ύX���܂��B
Rem

Rem #########################################################################

Rem
Rem %RootDrive% ���\������āA���̃X�N���v�g�p�ɐݒ肳��Ă��邩
Rem �m�F���܂��B
Rem
Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msie30.Key msie30.Key
regini msie30.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Echo Microsoft Internet Explorer 3.x �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done

