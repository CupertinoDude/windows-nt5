

rem
rem __ProgramFiles ������ ��ǻ�� ������ ���� �����մϴ�.
rem 
Set __ProgramFiles=%ProgramFiles%
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __ProgramFiles=%Program Files(x86)%
:acsrCont1
Rem
Rem ����� Ȩ ���͸��� ���� ���α׷� ������ �����͸� ����
Rem ���͸��� ����ϴ�.
Rem

call TsMkUDir "%RootDrive%\Office95"
call TsMkuDir "%RootDrive%\Office95\Startup"





call TsMkUDir "%RootDrive%\Office95\Templates"


call TsMkuDir "%RootDrive%\Office95\XLStart"

Rem
Rem ����� Ȩ ���͸��� �ý��� ���� ������ ������
Rem �����մϴ�.
Rem

If Exist "%RootDrive%\Office95\ShortCut Bar" Goto Skip1
If Not Exist "#INSTDIR#\Office\ShortCut Bar" Goto Skip1
Xcopy "#INSTDIR#\Office\ShortCut Bar" "%RootDrive%\Office95\ShortCut Bar" /E /I >Nul: 2>&1
:Skip1

Rem
Rem ClipArt Gallery ������ User�� ���͸��� �����մϴ�.
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip2
If Not Exist "%__ProgramFiles%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" Goto Skip2
Copy "%__ProgramFiles%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip2

Rem
Rem Custom.dic ������ ����� ���͸��� ����ϴ�.
Rem

If Not Exist "%RootDrive%\Office95\Custom.Dic" Copy Nul: "%RootDrive%\Office95\Custom.Dic" >Nul: 2>&1

Rem
Rem PowerPoint �� Binder ������ ����� ���͸��� �����մϴ�.
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\BINDER70.OBD" copy "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\PPT70.PPT" copy "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
