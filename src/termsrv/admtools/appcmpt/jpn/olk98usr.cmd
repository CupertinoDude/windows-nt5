Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\#OFFUDIR#"
call TsMkUDir "%RootDrive%\#OFFUDIR#\Template"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem �t�H�[�� �f�B���N�g�����R�s�[���� Outlook ��� Word ���G�f�B�^�Ƃ��Ďg����悤�ɂ��܂��B
Rem

If Exist "%RootDrive%\Windows\Forms" Goto Skip2
If Not Exist "%SystemRoot%\Forms" Goto Skip2
Xcopy "%SystemRoot%\Forms" "%RootDrive%\Windows\Forms" /e /i >Nul: 2>&1
:Skip2

Rem
Rem Custom.dic �t�@�C�������[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
Rem

if "#OFFUDIR#" == "Office95" goto O95
If Exist "%RootDrive%\#OFFUDIR#\Custom.Dic" Goto Skip4
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip4
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\#OFFUDIR#\Custom.Dic" >Nul: 2>&1
goto Skip4

:O95
If Not Exist "%RootDrive%\Office95\Custom.dic" Copy Nul: "%RootDrive%\Office95\Custom.dic" > Nul: 2>&1

:Skip4

