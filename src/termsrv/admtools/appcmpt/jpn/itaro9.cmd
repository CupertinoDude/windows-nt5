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
Rem �ꑾ�Y 9 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\itaro9.Cmd ITARO9INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Just" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ �ꑾ�Y 9 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo �ꑾ�Y 9 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\itaro9.Cmd
Del %Temp%\itaro9.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem ATOK 12 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\atok12.Cmd ATOK12INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Atok12" ""
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���W�X�g������ ATOK 12 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo ATOK 12 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\atok12.Cmd
Del %Temp%\atok12.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem �X�N���v�g���s�O�� �ꑾ�Y 9 ���N�������ꍇ�ɍ쐬����郌�W�X�g����
Rem �폜���܂��B
Rem

regini itaro9.key > Nul:


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �p�̃��W�X�g���L�[�t�@�C�����쐬���܂��B�iHKLM�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\itaro9.Reg itaro9.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B�iHKLM�ύX���j
Rem

regedit /s itaro9.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �p�̃��W�X�g���L�[�t�@�C�����쐬���܂��B�iHKCU�ύX���j
Rem

..\acsr "#ROOTDRIVE#" "%RootDrive%" ..\Logon\Template\itr9usr.Reg ..\Logon\itr9usr.Reg


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �p�̃��W�X�g���L�[�t�@�C�����쐬���܂��B�iHKLM->HKCU�j
Rem

regedit /a ..\Logon\itr9usr2.tmp HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem
..\acsr "HKEY_LOCAL_MACHINE\SOFTWARE\Justsystem" "HKEY_CURRENT_USER\Software\Justsystem" ..\Logon\itr9usr2.tmp ..\Logon\itr9usr2.Reg
Del ..\Logon\itr9usr2.tmp > Nul: 2>&1


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
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[����풓����v���O�������폜
Rem ���܂��B�����̓��\�[�X���W���g�p����̂ŁA�V�X�e���̃p�t�H�[�}��
Rem �X�ɑ傫���e�����A�A�v���P�[�V���� �G���[���������܂��B
Rem

If Exist "%COMMON_STARTUP%\JS�������̧�� �����X�V.LNK" Del "%COMMON_STARTUP%\JS�������̧�� �����X�V.LNK"
Rem If Exist "%COMMON_STARTUP%\�ꑾ�Y�X.LNK" Del "%COMMON_STARTUP%\�ꑾ�Y�X.LNK"
Rem If Exist "%COMMON_STARTUP%\JS�������.LNK" Del "%COMMON_STARTUP%\JS�������.LNK"
Rem If Exist "%COMMON_STARTUP%\Office�����ް.LNK" Del "%COMMON_STARTUP%\Office�����ް.LNK"
Rem If Exist "%COMMON_STARTUP%\����ޯ���X�V���ޭ��.LNK" Del "%COMMON_STARTUP%\����ޯ���X�V���ޭ��.LNK"


Rem #########################################################################
Rem
Rem �N�C�b�N�T�[�`�t�@�C��(QUF)�X�V�Ď� �𒆎~���邽�߂ɁA
Rem �t�@�C������ύX���܂��B
Rem

Ren "%ITARO9INS%\JSLIB32\JSQSF32.EXE" JSQSF32.E_E > Nul: 2>&1


Rem #########################################################################
Rem
Rem Itr9Usr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I Itr9Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Itr9Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd

:Skip1


Rem #########################################################################
Echo.
Echo   �ꑾ�Y 9 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă��郆�[�U�[��
Echo   �A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t���Ă���ēx���O�I��
Echo   ����K�v������܂��B
Echo.
Echo �ꑾ�Y 9 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
