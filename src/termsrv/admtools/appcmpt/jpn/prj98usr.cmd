
@echo off


Rem #########################################################################

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office97"



call TsMkUDir "%RootDrive%\Office97\Template"

call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem #########################################################################

Rem
Rem Custom.dic �t�@�C�������[�U�[�̃f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip1
If Not Exist "#INSTDIR#\Custom.Dic" Goto Skip1
Copy "#INSTDIR#\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip1

Rem #########################################################################

Rem
Rem �e���v���[�g�����[�U�[�̃e���v���[�g�̈�ɃR�s�[���܂��B
Rem








If Exist "%RootDrive%\Office97\Template\Microsoft Project"  Goto Skip2
If Not Exist "#INSTDIR#\..\Templates\Microsoft Project"  Goto Skip2
xcopy "#INSTDIR#\..\Templates\Microsoft Project" "%RootDrive%\Office97\Template\Microsoft Project" /E /I >Nul: 2>&1

:Skip2







if Exist "%RootDrive%\Office97\Template\Microsoft Project Web" Goto Skip3
if Not Exist "#INSTDIR#\..\Templates\Microsoft Project Web" Goto Skip3
xcopy "#INSTDIR#\..\Templates\Microsoft Project Web" "%RootDrive%\Office97\Template\Microsoft Project Web"  /E /I >Nul: 2>&1

:Skip3
