@echo Off
Rem
Rem ���̃X�N���v�g���g���� Microsoft SNA Server 3.0 �� Windows ��
Rem �^�[�~�i�� �T�[�o�[��Ő���Ɏ��s�����悤�ɍX�V���܂��B
Rem
Rem ���̃X�N���v�g�� SNA �� DLL �̂��������V�X�e���S�ʗp�ɓo�^��
Rem �āASNA ���}���`���[�U�[���Ő���ɋ@�\�ł���悤�ɂ��܂��B
Rem
Rem �d�v: ���̃X�N���v�g���R�}���h �v�����v�g������s����Ƃ��́ASNA 3.0 ��
Rem �C���X�g�[�����ꂽ��ɋN�����ꂽ�R�}���h �v�����v�g���g�p����K�v������܂��B
Rem

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    SNAROOT ���̒l���ݒ肳��Ă��Ȃ����߁A�}���`���[�U�[ �A�v
Echo    ���P�[�V�������ݒ�������ł��܂���ł����BSNA Server 3.0 ��
Echo    �C���X�g�[�������O�ɋN�������R�}���h �v�����v�g���炱�̃X�N
Echo    ���v�g�����s�����\��������܂��B
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1

Rem SNA �T�[�r�X���~���܂��B
net stop snabase

Rem SNA DLL ���V�X�e���S�ʗp�ɓo�^���܂��B
Register %SNAROOT%\SNADMOD.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAMANAG.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\WAPPC32.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\DBGTRACE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\MNGBASE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNATRC.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALM.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNANW.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAIP.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNABASE.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASERVR.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASII.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALINK.DLL /SYSTEM >Nul: 2>&1

Rem SNA �T�[�r�X���ċN�����܂��B
net start snabase

Echo SNA Server 3.0 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Cont2

