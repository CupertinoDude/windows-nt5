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
Rem �t�@�C�����A���ׂẴ��[�U�[�p�̃e���v���[�g���猻�݂̃��[�U�[�̃e���v���[�g�̏ꏊ�ɃR�s�[���܂��B
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem ���W�X�g������ Excel 97 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem Excel ���C���X�g�[������Ă��Ȃ��Ɖ��肵�āA�G���[ ���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0" "BinDirPath" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ Excel 97 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Excel 97 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���� Find Fast ���폜���܂��B
Rem Find Fast �̓��\�[�X���W���I�Ɏg�p���A�V�X�e���̃p�t�H�[�}���X�ɉe�����y�ڂ��܂��B
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1

Rem #########################################################################

Rem
Rem MsForms.Twd �� RefEdit.Twd �t�@�C�����쐬���܂��B�����̃t�@�C����
Rem Powerpoint �� Excel �̃A�h�C�� ([�t�@�C��]/[���O��t���ĕۑ�] ��
Rem HTML�t�H�[�}�b�g�Ȃ�) ���K�v�Ƃ���t�@�C���ł��B  �ǂ��炩�̃v���O����
Rem �����s�����ƁA�Y���̃t�@�C����K�v�ȃf�[�^�����t�@�C���Œu�������܂��B
Rem

If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul: 2>&1
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont2

If Exist %systemroot%\system32\RefEdit.Twd Goto Cont3
Copy Nul: %systemroot%\system32\RefEdit.Twd >Nul: 2>&1
Cacls %systemroot%\system32\RefEdit.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont3

Rem #########################################################################

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
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\MsExcl97.Key MsExcl97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" MsExcl97.Tmp MsExcl97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" MsExcl97.Tmp2 MsExcl97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" MsExcl97.Tmp3 MsExcl97.key
Del MsExcl97.Tmp >Nul: 2>&1
Del MsExcl97.Tmp2 >Nul: 2>&1
Del MsExcl97.Tmp3 >Nul: 2>&1

regini MsExcl97.key > Nul:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���ۂ̃C���X�g�[�� �f�B���N�g���𔽉f�����邽�� Exl97Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Exl97Usr.Cmd ..\Logon\Exl97Usr.Cmd

FindStr /I Exl97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Exl97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Microsoft Excel 97 ������ɍ쓮����ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Excel 97 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:Done
