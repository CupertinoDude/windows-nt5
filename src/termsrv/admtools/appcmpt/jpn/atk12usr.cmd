@echo off

Rem #########################################################################
Rem
Rem ATOK 12 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\atok12.Cmd ATOK12INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Atok12" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ ATOK 12 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo ATOK 12 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\atok12.Cmd
Del %Temp%\atok12.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK 12 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B
Rem

If Exist "%RootDrive%\JUST" Goto Cont1
regedit /s atk12usr2.Reg
regedit /s atk12usr.Reg
:Cont1


Rem #########################################################################
Rem
Rem ATOK 12 �̃f�[�^�p�̃f�B���N�g�������[�U�[�̃z�[�� �f�B���N�g��
Rem �ɍ쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\JUST"
call TsMkUDir "%RootDrive%\JUST\ATOK12"
call TsMkUDir "%RootDrive%\JUST\JSLIB32"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER"

Rem #########################################################################
Rem
Rem �t�@�C�������[�U�[�̃z�[�� �f�B���N�g���ɃR�s�[���܂��B
Rem

call :atk12copy ATKBNG98.STY
call :atk12copy ATKBNGDP.STY
call :atk12copy ATKMROMA.STY
call :atk12copy ATKMSIME.STY
call :atk12copy ATKOAK.STY
call :atk12copy ATKOAKV.STY
call :atk12copy ATKVJE.STY
call :atk12copy ATKWX3.STY
call :atk12copy ATKWXG.STY
call :atk12copy ATOK12.STY
call :atk12copy ATOK12IT.INI
call :atk12copy ATOK12N.STY
call :atk12copy ATOK12P.STY
call :atk12copy ATOK12U1.DIC
call :atk12copy ATOK12U2.DIC
call :atk12copy ATOK12U3.DIC
call :atk12copy ATOK12U4.DIC
call :atk12copy ATOK12U5.DIC
call :atk12copy ATOKRH.BIN

if not Exist "%RootDrive%\JUST\ATOK12\ATOK12W.INI" copy "%RootDrive%\JUST\ATOK12\ATOK12IT.INI" "%RootDrive%\JUST\ATOK12\ATOK12W.INI"

goto :Done

:atk12copy
Rem #########################################################################
Rem
Rem �t�@�C�������[�U�[�̃z�[�� �f�B���N�g���ɃR�s�[���܂��B
Rem

If Not Exist "%RootDrive%\JUST\ATOK12\%1" copy "%ATOK12INS%\%1" "%RootDrive%\JUST\ATOK12\%1"

:Done
