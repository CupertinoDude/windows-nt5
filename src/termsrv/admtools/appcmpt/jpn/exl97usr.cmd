
Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Templates"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"
call TsMkUDir "%RootDrive%\Office97\XLStart"

Rem
Rem ���̃t�@�C�����A���ׂẴ��[�U�[�p�̃e���v���[�g���猻�݂̃��[�U�[�̃e���v���[�g�̏ꏊ�ɃR�s�[���܂��B
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" copy "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem
Rem ARTGALRY.CAG �����[�U�[�� Windows �f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip1
If Not Exist "%SystemRoot%\ArtGalry.Cag" Goto Skip1
copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip1

Rem
Rem Custom.dic �t�@�C�������[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip2
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip2
copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip2

