@Echo Off

Rem #########################################################################

Rem
Rem CMD �g���@�\���L���ɂȂ��Ă��邩�m�F���܂��B
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c eudora4.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

Rem #########################################################################

Rem
Rem %RootDrive% ���\������āA���̃X�N���v�g�p�ɐݒ肳��Ă��邩�m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem ���W�X�g������ Eudora �̃R�}���h ���C�����擾���܂��B

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:1" 

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���W�X�g������ Eudora �̃R�}���h ���C�����擾�ł��܂���B
Echo Eudora Pro 4.0 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1
set EudCmd=%EUDTMP:~0,-2%

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:2" 

If Not ErrorLevel 1 Goto Cont2
Echo.
Echo ���W�X�g������ Eudora Pro 4.0 �C���X�g�[�� �f�B���N�g�����擾�ł��܂���B
Echo Eudora Pro 4.0 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont2
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1

Set EudoraInstDir=%EUDTMP:~0,-13%

Rem #########################################################################

If Exist "%EudoraInstDir%\descmap.pce" Goto Cont0
Echo.
Echo ���̃A�v���P�[�V�����݊����X�N���v�g�𑱍s����O�� Eudora 4.0 �����s����
Echo ����΂Ȃ�܂���BEudora �̎��s��A�X�^�[�g ���j���[�� Eudora Pro �t�H���_
Echo �ɂ��� Eudora Pro �̃V���[�g �J�b�g�̃v���p�e�B���J���A�����N���
Echo   %RootDrive%\eudora.ini 
Echo ��ǉ����Ă��������B�����N��͎��̂悤�ɂȂ�܂�:
Echo  "%EudoraInstDir%\Eudora.exe" %RootDrive%\eudora.ini
Echo.
Pause
Goto Done

:Cont0

Rem
Rem ���W�X�g�� �L�[��ύX���āA�p�X�����[�U�[�ŗL�̃f�B���N�g��
Rem ���w�������悤�ɂ��܂��B
Rem

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#INSTDIR#" "%EudoraInstDir%" Template\Eudora4.Key Eudora4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" Eudora4.tmp Eudora4.key

regini eudora4.key > Nul:
del eudora4.tmp
del eudora4.key

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem descmap.pce �̃A�N�Z�X�����X�V���܂��B
cacls "%EudoraInstDir%\descmap.pce" /E /G "Terminal Server User":R >NUL: 2>&1

Rem #########################################################################

Echo.
Echo   Eudora Pro 4.0 ������ɍ쓮����ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Eudora 4.0 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:done
