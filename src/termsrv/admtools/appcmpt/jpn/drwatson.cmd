@echo off
Rem
Rem ���̃X�N���v�g�́ADr. Watson �̃C���X�g�[�����X�V���āA���O �t�@�C����
Rem �p�X�ƃN���b�V�� �_���v �t�@�C�������[�U�[�̃z�[�� �f�B���N�g���ɒu��
Rem ���悤�ύX���܂��B
Rem
Rem DRWTSN32 �A�v���P�[�V���������s���Ă����̏ꏊ��ύX���邱�Ƃ�
Rem �ł��܂��B
Rem

Rem #########################################################################

Rem
Rem ���ϐ� %RootDrive% �����̃X�N���v�g�p�ɐݒ肳��Ă��邩�ǂ����m�F���܂��B
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\DrWatson.Key DrWatson.Key
regini DrWatson.Key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo Dr. Watson �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
