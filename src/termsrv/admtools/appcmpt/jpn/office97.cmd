
@Echo Off

Rem
Rem ����:  ���̃X�N���v�g�̒��� CACLS �R�}���h�� NTFS
Rem �t�H�[�}�b�g�̃p�[�e�B�V�����ł̂ݗL���ł��B
Rem

Rem #########################################################################


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

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1



Rem
Rem ���W�X�g������ Office 97 ���C���X�g�[������Ă���f�B���N�g�����擾���܂��B������Ȃ��ꍇ�́A
Rem ���̃A�v���P�[�V�����̓C���X�g�[������Ă��Ȃ��Ɖ��肵�ăG���[���b�Z�[�W��\�����܂��B
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ Office 97 �̃C���X�g�[���ꏊ���擾�ł��܂���B
Echo Office 97 ���C���X�g�[������Ă��邩�ǂ����m�F���Ă���A���̃X�N���v�g��
Echo �Ď��s���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem Access 97 �̃V�X�e�� �f�[�^�x�[�X��ǂݎ���p�ɕύX���܂��B
Rem ����ɂ�蕡���̃��[�U�[���f�[�^�x�[�X�𓯎��Ɏ��s�ł���悤
Rem �ɂ��܂��B�������A[�c�[��] ���j���[�� [�Z�L�����e�B] ���g�p���āA
Rem �V�X�e�� �f�[�^�x�[�X���X�V���邱�Ƃ��ł��Ȃ��Ȃ�܂��B���[�U�[
Rem ��ǉ�����ɂ́A�V�X�e�� �f�[�^�x�[�X�̃A�N�Z�X�����������݉\
Rem �ɖ߂��K�v������܂��B
Rem

If Not Exist %SystemRoot%\System32\System.Mdw Goto Cont1
cacls %SystemRoot%\System32\System.Mdw /E /P "Authenticated Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P "Power Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P Administrators:R >NUL: 2>&1

:Cont1

Rem #########################################################################

Rem
Rem Office 97 �ɂ���čX�V���ꂽ�V�X�e�� DLL �ւ̓ǂݎ��A�N�Z�X��
Rem �S���ɗ^���܂��B
Rem

REM If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R >NUL: 2>&1

Rem #########################################################################

Rem #########################################################################

Rem
Rem �^�[�~�i�� �T�[�r�X�̃��[�U�[�� Outlook �p�� frmcache.dat �ύX�A�N�Z�X�������܂��B
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################


Rem
Rem PowerPoint �E�B�U�[�h��ǂݎ���p�ɕύX���āA�E�B�U�[�h�𓯎�
Rem �� 2 �ȏ���s�ł���悤�ɂ��܂��B
Rem


If Exist "%O97INST%\Template\��ھ��ð���\�ݽ��� ���ް��.Pwz" Attrib +R "%O97INST%\Template\��ھ��ð���\�ݽ��� ���ް��.Pwz" >Nul: 2>&1

If Exist "%O97INST%\Office\Ppt2html.ppa" Attrib +R "%O97INST%\Office\Ppt2html.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\bshppt97.ppa" Attrib +R "%O97INST%\Office\bshppt97.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\geniwiz.ppa" Attrib +R "%O97INST%\Office\geniwiz.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\ppttools.ppa" Attrib +R "%O97INST%\Office\ppttools.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\graphdrs.ppa" Attrib +R "%O97INST%\Office\graphdrs.ppa" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �Ǘ��҂łȂ����[�U�[�� Access �̃E�B�U�[�h�� Excel �� Access �A�h�C����
Rem ���s�ł���悤�ɂ���ɂ́A���[�U�[�� Wizard �t�@�C���ւ̕ύX�A�N�Z�X��
Rem �^���邽�߂ɁA���� 3 �s���� "Rem" �̕�������菜���K�v������܂��B
Rem 
Rem

Rem If Exist "%O97INST%\Office\WZLIB80.MDE" cacls "%O97INST%\Office\WZLIB80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZMAIN80.MDE" cacls "%O97INST%\Office\WZMAIN80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZTOOL80.MDE" cacls "%O97INST%\Office\WZTOOL80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 

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
Rem SystemRoot �̉��� msremote.sfs �f�B���N�g�����쐬���܂��B����ɂ��A���[�U�[���R���g���[��
Rem �p�l���� [���[���� FAX] �̃A�C�R�����g���ăv���t�@�C�����쐬�ł���悤�ɂȂ�܂��B
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1

Rem #########################################################################

Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���� Find Fast ���폜���܂��B
Rem Find Fast �̓��\�[�X���W���I�Ɏg�p���A�V�X�e�� �p�t�H�[�}���X��
Rem �e�����y�ڂ��܂��B
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1


Rem #########################################################################

Rem
Rem �S���[�U�[�p�̃X�^�[�g �A�b�v ���j���[���� "Microsoft Office ���Ķ�� �ް.lnk" �̃t�@�C������菜���܂��B
Rem

If Exist "%COMMON_STARTUP%\Microsoft Office ���Ķ�� �ް.lnk" Del "%COMMON_STARTUP%\Microsoft Office ���Ķ�� �ް.lnk" >Nul: 2>&1

Rem #########################################################################
Rem
Rem SystemRoot �̉��� msfslog.txt ���쐬���āA�^�[�~�i�� �T�[�r�X�̃��[�U�[�ɁA
Rem ���̃t�@�C���̃t�� �A�N�Z�X����^���܂��B
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office97.Key Office97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office97.Tmp Office97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" Office97.Tmp2 Office97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office97.Tmp3 Office97.Key
Del Office97.Tmp >Nul: 2>&1
Del Office97.Tmp2 >Nul: 2>&1
Del Office97.Tmp3 >Nul: 2>&1

regini Office97.key > Nul:

Rem Office 97 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���ۂ̃C���X�g�[�� �f�B���N�g���𔽉f�����邽�� Ofc97Usr.Cmd ���X�V���āA
Rem UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Ofc97Usr.Cmd ..\Logon\Ofc97Usr.Cmd

FindStr /I Ofc97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Office 97 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Office 97 �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Pause

:done
