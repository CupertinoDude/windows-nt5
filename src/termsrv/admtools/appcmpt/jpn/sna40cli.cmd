@ECHO OFF
REM *** SNA40CLI.CMD - SNA Client 4.0 �o�^�o�b�` �t�@�C�� ***

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    SNAROOT ���̒l���ݒ肳��Ă��Ȃ����߁A�}���`���[�U�[ �A�v
Echo    ���P�[�V�������ݒ�������ł��܂���ł����BSNA Client 4.0 ��
Echo    �C���X�g�[�������O�ɋN�������R�}���h �v�����v�g���炱�̃X�N
Echo    ���v�g�����s�����\��������܂��B
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1
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

Echo SNA Server 4.0 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Cont2
