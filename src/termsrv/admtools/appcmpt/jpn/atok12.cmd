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
Rem ATOK 12 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKLM�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\atok12.Reg atok12.Reg


Rem #########################################################################
Rem
Rem ATOK 12 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B�iHKLM�ύX���j
Rem

regedit /s atok12.Reg


Rem #########################################################################
Rem
Rem ATOK 12 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKCU�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" ..\Logon\Template\atk12usr.Reg ..\Logon\atk12usr.Reg


Rem #########################################################################
Rem
Rem ATOK 12 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKLM->HKCU�j
Rem

regedit /a ..\Logon\atk12usr2.tmp HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem
..\acsr "HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem" "HKEY_CURRENT_USER\Software\Justsystem" ..\Logon\atk12usr2.tmp ..\Logon\atk12usr2.Reg
Del ..\Logon\atk12usr2.tmp > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK12IT.INI ���}���`���[�U�[�d�l�ɕύX���܂��B
Rem

..\acsr "%ATOK12INS%" "%RootDrive%\JUST\ATOK12" "%ATOK12INS%\ATOK12IT.INI" "%ATOK12INS%\ATOK12IT.tmp1"
Del "%ATOK12INS%\ATOK12IT.INI" > Nul: 2>&1
..\acsr "�V�X�e������=%RootDrive%\JUST\ATOK12" "�V�X�e������=%ATOK12INS%" "%ATOK12INS%\ATOK12IT.tmp1" "%ATOK12INS%\ATOK12IT.tmp2"
Del "%ATOK12INS%\ATOK12IT.tmp1" > Nul: 2>&1
..\acsr "�⏕�����P=%RootDrive%\JUST\ATOK12" "�⏕�����P=%ATOK12INS%" "%ATOK12INS%\ATOK12IT.tmp2" "%ATOK12INS%\ATOK12IT.INI"
Del "%ATOK12INS%\ATOK12IT.tmp2" > Nul: 2>&1


Rem #########################################################################
Rem
Rem Atk12Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I Atk12Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Atk12Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd

:Skip1


Rem #########################################################################
Echo.
Echo   ATOK 12 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă��郆�[�U�[��
Echo   �A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t���Ă���ēx���O�I��
Echo   ����K�v������܂��B
Echo.
Echo ATOK 12 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
