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
Rem �ꑾ�Y Office 8 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\ioffice8.Cmd ITARO8INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Just" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ �ꑾ�Y Office 8 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo �ꑾ�Y Office 8 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\ioffice8.Cmd
Del %Temp%\ioffice8.Cmd > Nul: 2>&1

Rem #########################################################################
Rem
Rem ATOK 11 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\atok11.Cmd ATOK11INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Atok11" ""
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���W�X�g������ ATOK 11 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo ATOK 11 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\atok11.Cmd
Del %Temp%\atok11.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem �X�N���v�g���s�O�� �ꑾ�Y Office 8 ���N�������ꍇ�ɍ쐬����郌�W�X�g����
Rem �폜���܂��B
Rem

regini ioffice8.key > Nul:


Rem #########################################################################
Rem
Rem �ꑾ�Y Office 8 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKLM�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\ioffice8.Reg ioffice8.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y Office 8 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B�iHKLM�ύX���j
Rem

regedit /s ioffice8.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y Office 8 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKCU�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" ..\Logon\Template\Itr8Usr.Reg ..\Logon\Itr8Usr.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y Office 8 �p�̃��W�X�g�� �L�[ �t�@�C�����쐬���܂��B�iHKLM->HKCU�j
Rem

regedit /a ..\Logon\Itr8Usr2.tmp HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem
..\acsr "HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem" "HKEY_CURRENT_USER\Software\Justsystem" ..\Logon\Itr8Usr2.tmp ..\Logon\Itr8Usr2.Reg
Del ..\Logon\Itr8Usr2.tmp > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK11PV.INI ���쐬���܂��B
Rem

copy "%SystemRoot%\ATOK11W.INI" "%ITARO8INS%\ATOK11\ATOK11PV.INI" > Nul: 2>&1

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
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[����풓����v���O�������폜
Rem ���܂��B�����̓��\�[�X���W���g�p����̂ŁA�V�X�e���̃p�t�H�[�}���X��
Rem �傫���e�����A�A�v���P�[�V���� �G���[�𔭐������܂��B
Rem

If Exist "%COMMON_STARTUP%\JS�������̧�� �����X�V.LNK" Del "%COMMON_STARTUP%\JS�������̧�� �����X�V.LNK"
Rem If Exist "%COMMON_STARTUP%\Office8�����ް.LNK" Del "%COMMON_STARTUP%\Office8�����ް.LNK"
Rem If Exist "%COMMON_STARTUP%\�ꑾ�Y�W.LNK" Del "%COMMON_STARTUP%\�ꑾ�Y�W.LNK"
Rem If Exist "%COMMON_STARTUP%\�O�l�Y�W.LNK" Del "%COMMON_STARTUP%\�O�l�Y�W.LNK"
Rem If Exist "%COMMON_STARTUP%\FullBand.LNK" Del "%COMMON_STARTUP%\FullBand.LNK"

Rem #########################################################################
Rem
Rem �N�C�b�N�T�[�`�t�@�C��(QUF)�X�V�Ď� �𒆎~���邽�߂ɁA
Rem �t�@�C������ύX���܂��B
Rem

Ren "%ITARO8INS%\JSLIB32\JSQSF32.EXE" JSQSF32.E_E > Nul: 2>&1


Rem #########################################################################
Rem
Rem Itr8Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I Itr8Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Itr8Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd

:Skip1


Rem #########################################################################
Echo.
Echo   �ꑾ�Y Office 8 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă��郆�[�U�[��
Echo   �A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t���Ă���ēx���O�I��
Echo   ����K�v������܂��B
Echo.
Echo �ꑾ�Y Office 8 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
