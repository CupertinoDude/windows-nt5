
rem
rem �R���s���[�^�̎�ނɉ����� __ProgramFiles �̒l��ݒ肵�܂��B
rem 
Set __ProgramFiles=%ProgramFiles%
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __ProgramFiles=%Program Files(x86)%
:acsrCont1
Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office95"
call TsMkuDir "%RootDrive%\Office95\Startup"
call TsMkUDir "%RootDrive%\Office95\Template"

call TsMkuDir "%RootDrive%\Office95\XLStart"

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���Ƀc�[���o�[���R�s�[���܂��B
Rem ���ɃR�s�[����Ă���ꍇ�̓R�s�[���܂���B
Rem

If Exist "%RootDrive%\Office95\ShortCut Bar" Goto Skip1
If Not Exist "#INSTDIR#\Office\ShortCut Bar" Goto Skip1
Xcopy "#INSTDIR#\Office\ShortCut Bar" "%RootDrive%\Office95\ShortCut Bar" /E /I >Nul: 2>&1
:Skip1

Rem
Rem ���[�U�[�̃f�B���N�g���փN���b�v �A�[�g �M�������[�̃t�@�C�����R�s�[���܂��B
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip2
If Not Exist "%__ProgramFiles%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" Goto Skip2
Copy "%__ProgramFiles%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip2

Rem
Rem ���[�U�[�̃f�B���N�g���� Custom.dic �t�@�C�����쐬���܂��B
Rem

If Not Exist "%RootDrive%\Office95\Custom.Dic" Copy Nul: "%RootDrive%\Office95\Custom.Dic" >Nul: 2>&1

Rem
Rem ���[�U�[�̃f�B���N�g���� PowerPoint �ƁA�o�C���_ �t�@�C�����R�s�[���܂��B
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\BINDER70.OBD" copy "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\PPT70.PPT" copy "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
