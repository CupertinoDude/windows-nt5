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
Rem ATOK 11 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\atok11.Cmd ATOK11INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Atok11" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ ATOK 11 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo ATOK 11 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\atok11.Cmd
Del %Temp%\atok11.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK 11 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKLM�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\atok11.Reg atok11.Reg


Rem #########################################################################
Rem
Rem ATOK 11 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B�iHKLM�ύX���j
Rem

regedit /s atok11.Reg


Rem #########################################################################
Rem
Rem ATOK 11 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKCU�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" ..\Logon\Template\atk11usr.Reg ..\Logon\atk11usr.Reg


Rem #########################################################################
Rem
Rem ATOK11PV.INI ���쐬���܂��B
Rem

copy "%SystemRoot%\ATOK11W.INI" "%ATOK11INS%\ATOK11PV.INI" > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK11PV.INI ���}���`���[�U�[�d�l�ɕύX���܂��B
Rem

..\acsr "%ATOK11INS%" "%RootDrive%\JUST\ATOK11" "%ATOK11INS%\ATOK11PV.INI" "%ATOK11INS%\ATOK11PV.tmp1"
Del "%ATOK11INS%\ATOK11PV.INI" > Nul: 2>&1
..\acsr "�V�X�e������=%RootDrive%\JUST\ATOK11" "�V�X�e������=%ATOK11INS%" "%ATOK11INS%\ATOK11PV.tmp1" "%ATOK11INS%\ATOK11PV.tmp2"
Del "%ATOK11INS%\ATOK11PV.tmp1" > Nul: 2>&1
..\acsr "�⏕�����P=%RootDrive%\JUST\ATOK11" "�⏕�����P=%ATOK11INS%" "%ATOK11INS%\ATOK11PV.tmp2" "%ATOK11INS%\ATOK11PV.INI"
Del "%ATOK11INS%\ATOK11PV.tmp2" > Nul: 2>&1


Rem #########################################################################
Rem
Rem Atk11Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I Atk11Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Atk11Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd

:Skip1


Rem #########################################################################
Echo.
Echo   ATOK 11 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă��郆�[�U�[��
Echo   �A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t���Ă���ēx���O�I��
Echo   ����K�v������܂��B
Echo.
Echo ATOK 11 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Echo.
Pause

:Done
