
Rem #########################################################################

Rem
Rem "Microsoft Office ���Ķ�� �ް.lnk" �t�@�C���� Office �C���X�g�[�� ���[�g����
Rem ���݂̃��[�U�[�̃X�^�[�g�A�b�v ���j���[�ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office97" Goto Skip0
If Exist "%USER_STARTUP%\Microsoft Office ���Ķ�� �ް.lnk" Goto Skip0
If Not Exist "#INSTDIR#\Microsoft Office ���Ķ�� �ް.lnk" Goto Skip0
Copy "#INSTDIR#\Microsoft Office ���Ķ�� �ް.lnk" "%USER_STARTUP%\Microsoft Office ���Ķ�� �ް.lnk" >Nul: 2>&1
:Skip0

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Startup"

call TsMkUDir "%RootDrive%\Office97\Template"
call TsMkUDir "%RootDrive%\Office97\Template\�ЊO����"

call TsMkuDir "%RootDrive%\Office97\XLStart"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem �S���[�U�[�̃e���v���[�g�ɂ��邱���̃t�@�C�������݂̃��[�U�[�̃e���v���[�g�ɃR�s�[���܂��B
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" copy "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" copy "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\BINDER.OBD" copy "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem
Rem �V�X�e�� �c�[���o�[�����[�U�[�̃z�[�� �f�B���N�g���ɃR�s�[���܂��B
Rem ���ɃR�s�[����Ă���ꍇ�̓R�s�[���܂���B
Rem

If Exist "%RootDrive%\Office97\ShortCut Bar" Goto Skip1
If Not Exist "#INSTDIR#\Office\ShortCut Bar" Goto Skip1
Xcopy "#INSTDIR#\Office\ShortCut Bar" "%RootDrive%\Office97\ShortCut Bar" /E /I >Nul: 2>&1
:Skip1

Rem
Rem �t�H�[�� �f�B���N�g�����R�s�[���� Outlook ��� Word ���G�f�B�^�Ƃ��Ďg����悤�ɂ��܂��B
Rem

If Exist "%RootDrive%\Windows\Forms" Goto Skip2
If Not Exist "%SystemRoot%\Forms" Goto Skip2
Xcopy "%SystemRoot%\Forms" "%RootDrive%\Windows\Forms" /e /i >Nul: 2>&1
:Skip2

Rem
Rem ARTGALRY.CAG �����[�U�[�� Windows �f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip3
If Not Exist "%SystemRoot%\ArtGalry.Cag" Goto Skip3
Copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip3

Rem
Rem Custom.dic �t�@�C�������[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip4
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip4
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip4

Rem
Rem �O���t �t�@�C�������[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office97\GR8GALRY.GRA" Goto Skip5
If Not Exist "#INSTDIR#\Office\GR8GALRY.GRA" Goto Skip5
Copy "#INSTDIR#\Office\GR8GALRY.GRA" "%RootDrive%\Office97\GR8GALRY.GRA" >Nul: 2>&1
:Skip5

If Exist "%RootDrive%\Office97\XL8GALRY.XLS" Goto Skip6
If Not Exist "#INSTDIR#\Office\XL8GALRY.XLS" Goto Skip6
Copy "#INSTDIR#\Office\XL8GALRY.XLS" "%RootDrive%\Office97\XL8GALRY.XLS" >Nul: 2>&1
:Skip6

REM
REM �T���v�� �E�B�U�[�h���C���X�g�[������Ă���ꍇ�A�X�^�C�� �e���v���[�g�����[�U�[ �f�B���N�g���ɃR�s�[���܂��B
REM

If Exist "#INSTDIR#\Template\�Г�����\���� ���ް��.wiz" Goto Skip7
If Exist "#INSTDIR#\Template\�ЊO����\���� ���ް��.wiz" Goto Skip7
If Not Exist "#INSTDIR#\Template\���̑��̕���\���� ���ް��.wiz" Goto Skip8
:Skip7
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 1.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 1.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 1.dot" >Nul: 2>&1
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 2.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 2.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 2.dot" >Nul: 2>&1
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 3.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 3.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 3.dot" >Nul: 2>&1
:Skip8

