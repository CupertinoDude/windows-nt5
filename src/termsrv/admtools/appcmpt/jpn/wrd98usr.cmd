
Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V����
Rem �f�[�^��p�̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Startup"
call TsMkUDir "%RootDrive%\Office97\Template"
call TsMkUDir "%RootDrive%\Office97\Template\�ЊO����"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem ���̃t�@�C�����A���ׂẴ��[�U�[�p�̃e���v���[�g���猻�݂̃��[�U�[�̃e���v���[�g�̏ꏊ�ɃR�s�[���܂��B
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" copy "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" "%UserProfile%\%Templates%\" /Y >Nul: 2>&1

Rem
Rem ARTGALRY.CAG �����[�U�[�� Windows �f�B���N�g���ɃR�s�[���܂�
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip1
Copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip1

Rem
Rem Custom.dic �����[�U�[�̃f�B���N�g���ɃR�s�[���܂�
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip2
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip2
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip2

REM
REM �T���v�� �E�B�U�[�h���C���X�g�[������Ă���ꍇ�A�X�^�C�� �e���v���[�g�����[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
REM

If Exist "#INSTDIR#\Template\�Г�����\���� ���ް��.wiz" Goto Skip3
If Exist "#INSTDIR#\Template\�ЊO����\���� ���ް��.wiz" Goto Skip3
If Not Exist "#INSTDIR#\Template\���̑��̕���\���� ���ް��.wiz" Goto Skip4
:Skip3
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 1.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 1.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 1.dot" >Nul: 2>&1
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 2.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 2.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 2.dot" >Nul: 2>&1
If Not Exist "%RootDrive%\Office97\Template\�ЊO����\��� 3.dot" Copy "#INSTDIR#\Template\�ЊO����\��� 3.dot" "%RootDrive%\Office97\Template\�ЊO����\��� 3.dot" >Nul: 2>&1
:Skip4
