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
Rem ���W�X�g������ Office 95 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Cont0

Echo.
Echo ���W�X�g������ Office 95 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Office 95 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O95.Cmd
Del %Temp%\O95.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���� Find Fast ���폜���܂��B
Rem Find Fast �̓��\�[�X���W���I�Ɏg�p���A�V�X�e���̃p�t�H�[�}���X�ɉe�����y�ڂ��܂��B
Rem ���݂̃��[�U�[�p�̃��j���[�ƑS���[�U�[�p�̃��j���[�̗������m�F���܂��B
Rem �ǂ���̃��j���[���\������邩�ɂ��ẮA�V�X�e�������s���[�h�ɖ߂���
Rem ���邩�ǂ����Ɉˑ����܂��B
Rem

If Not Exist "%USER_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" Goto Cont1
Del "%USER_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%COMMON_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" Goto Cont2
Del "%COMMON_STARTUP%\Find Fast �p���ޯ���쐬°�.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem PowerPoint �ƃo�C���_�̃t�@�C���� All Users �f�B���N�g���ɃR�s�[���āA���ꂼ���
Rem ���[�U�[�̃��O�C�����Ƀ��[�U�[�̃z�[���f�B���N�g���ɃR�s�[�����悤�ɂ��܂��B
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER70.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" copy "%UserProfile%\%TEMPLATES%\PPT70.PPT" "%ALLUSERSPROFILE%\%TEMPLATES%" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem ���W�X�g�� �L�[��ύX���āA�p�X�����[�U�[�ŗL�̃f�B���N�g�����w�������悤�ɂ��܂��B
Rem

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office95.Key Office95.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office95.Tmp Office95.Tmp2
..\acsr "#INSTDIR#" "%O95INST%" Office95.Tmp2 Office95.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office95.Tmp3 Office95.Key
Del Office95.Tmp >Nul: 2>&1
Del Office95.Tmp2 >Nul: 2>&1
Del Office95.Tmp3 >Nul: 2>&1
regini Office95.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���ۂ̃C���X�g�[�� �f�B���N�g���𔽉f�����邽�� Ofc95Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#INSTDIR#" "%O95INST%" ..\Logon\Template\Ofc95Usr.Cmd ..\Logon\Ofc95Usr.Cmd

FindStr /I Ofc95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   Office 95 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Office 95 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done


