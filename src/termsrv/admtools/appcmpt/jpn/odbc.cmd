@echo off
Rem
Rem ���̃X�N���v�g�� ODBC ���O �t�@�C���̏ꏊ���X�V���܂��B
Rem

Rem #########################################################################

Rem
Rem %RootDrive% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\ODBC.Key ODBC.Key
regini ODBC.Key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo.
Echo   SQL �f�[�^ �\�[�X���\������Ƃ��A[���[�U�[ DSN] �^�u�� [�\��] �{�^�����N���b
Echo   �N���āA[�I�v�V����] �{�^�����N���b�N������A[�v���t�@�C�����O] ���N���b�N��
Echo   �Ă��������B������ [�N�G�� ���O] �� [���v�p���O] �{�^�����g���ă��O �t�@�C��
Echo   �̕ۑ�������[�U�[�̃��[�g �h���C�u (%RootDrive%) �ɐݒ肵�Ă��������B
Echo.
Echo   ����ɁA�Ǘ��҂͑S���[�U�[�p�̃f�[�^ �\�[�X���\�����邱�Ƃ��ł��܂��B
Echo   �܂��A�R�}���h �v�����v�g�� "Change User /Install" �R�}���h����͂���
Echo   ����A�f�[�^ �\�[�X���\�����Ă��������B
Echo   �Ō�ɂ܂��R�}���h �v�����v�g�� "Change User /Execute" ����͂��Č���
Echo   ���s���[�h�ɖ߂��Ă��������B
Echo.

Echo ODBC �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done

