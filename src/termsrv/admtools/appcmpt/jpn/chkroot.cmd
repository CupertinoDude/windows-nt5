
Set _CHKROOT=PASS

Cd "%SystemRoot%\Application Compatibility Scripts"

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont2


Echo REM > RootDrv2.Cmd
Echo Rem ���̃A�v���P�[�V�����݊����X�N���v�g�����s����O�ɁA >> RootDrv2.Cmd
Echo Rem �^�[�~�i�� �T�[�r�X�ɂ���Ďg�p����Ă��Ȃ��A�e���[�U�[�̃z�[��   >> RootDrv2.Cmd
Echo Rem �f�B���N�g���Ƀ}�b�v����h���C�u���������߂�K�v������܂��B >> RootDrv2.Cmd
Echo Rem �}�b�v����h���C�u�����́A���̃t�@�C���̉��̕��ɂ��� >> RootDrv2.Cmd
Echo Rem "Set RootDrive" �Ŏn�܂�X�e�[�g�����g�Ɏw�肵�܂��B >> RootDrv2.Cmd
Echo Rem ���ɍD�݂��Ȃ���� W: �𐄏����܂��B���ɂ��̗�������܂��B >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM            Set RootDrive=W: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo Rem ����: �h���C�u�����ƃR�����̌�ɂ̓X�y�[�X�����Ȃ��悤�ɒ��ӂ��Ă��������B >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo Rem ���̍�Ƃ��I��������A���̃t�@�C����ۑ����ă��������I�����A >> RootDrv2.Cmd
Echo Rem �A�v���P�[�V�����݊����X�N���v�g�̎��s�𑱂��Ă��������B >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd
Echo Set RootDrive=>> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd

NotePad RootDrv2.Cmd

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont1

Echo.
Echo     ���̃A�v���P�[�V�����݊����X�N���v�g�����s����O�ɁA�e���[�U�[
Echo     �̃z�[�� �f�B���N�g���Ƀ}�b�v����h���C�u���������߂�K�v����
Echo     ��܂��B
Echo.
Echo     �X�N���v�g�͏I�����܂����B
Echo.
Pause

Set _CHKROOT=FAIL
Goto Cont3


:Cont1

Rem
Rem ���������[�U�[ ���O�I�� �X�N���v�g�����s���ă��[�g���}�b�v���āA
Rem �A�v���P�[�V�����̃C���X�g�[���ɔ����Ă��������B
Rem 

Call "%SystemRoot%\System32\UsrLogon.Cmd


:Cont2

Rem ���W�X�g���� RootDrive �L�[��ݒ肵�܂��B

echo HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Terminal Server > chkroot.key
echo     RootDrive = REG_SZ %ROOTDRIVE%>> chkroot.key

regini chkroot.key > Nul: 


:Cont3

Cd "%SystemRoot%\Application Compatibility Scripts\Install"
