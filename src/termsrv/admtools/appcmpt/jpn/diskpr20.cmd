@echo Off

Rem
Rem ���̃X�N���v�g�͊Ǘ��p�v���O����������ɋ@�\����悤�ɁA
Rem DiskKeeper 2.0 ���X�V���܂��B

Rem #########################################################################

Rem
Rem ���W�X�g������ DiskKeeper 2.0 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\DK20.Cmd DK20INST "HKLM\Software\Microsoft\Windows\CurrentVersion\App Paths\DKSERVE.EXE" "Path" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ DiskKeeper 2.0 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo �A�v���P�[�V�������C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\DK20.Cmd
Del %Temp%\DK20.Cmd >Nul: 2>&1

Rem #########################################################################



register %DK20INST% /system >Nul: 2>&1

Echo DiskKeeper 2.x �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
