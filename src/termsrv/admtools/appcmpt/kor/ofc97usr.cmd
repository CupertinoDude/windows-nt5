
Rem #########################################################################

Rem
Rem "Microsoft Office �ٷ� ���� ����.lnk" ������ Office ��ġ ��Ʈ����
Rem ���� ������� ���� �޴��� �����մϴ�.
Rem

If Exist "%RootDrive%\Office97" Goto Skip0
If Exist "%USER_STARTUP%\Microsoft Office Shortcut Bar.lnk" Goto Skip0
If Not Exist "#INSTDIR#\Microsoft Office Shortcut Bar.lnk" Goto Skip0
Copy "#INSTDIR#\Microsoft Office Shortcut Bar.lnk" "%USER_STARTUP%\Microsoft Office Shortcut Bar.lnk" >Nul: 2>&1
:Skip0

Rem
Rem ����� Ȩ ���͸��� ���� ���α׷� ������ �����͸� ����
Rem ���͸��� ����ϴ�.
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Startup"



call TsMkUDir "%RootDrive%\Office97\Templates"

call TsMkuDir "%RootDrive%\Office97\XLStart"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem ��� ����� ���ø����� ���� ����� ���ø� ��ġ�� �� ������ �����մϴ�.
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" copy "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" copy "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%UserProfile%\%TEMPLATES%\BINDER.OBD" copy "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem
Rem ����� Ȩ ���͸��� �ý��� ���� ������ ������
Rem �����մϴ�.
Rem

If Exist "%RootDrive%\Office97\ShortCut Bar" Goto Skip1
If Not Exist "#INSTDIR#\Office\ShortCut Bar" Goto Skip1
Xcopy "#INSTDIR#\Office\ShortCut Bar" "%RootDrive%\Office97\ShortCut Bar" /E /I >Nul: 2>&1
:Skip1

Rem
Rem ��� ���͸��� �����Ͽ� Outlook�� ������� Word�� ����� �� �ֽ��ϴ�.
Rem

If Exist "%RootDrive%\Windows\Forms" Goto Skip2
If Not Exist "%SystemRoot%\Forms" Goto Skip2
Xcopy "%SystemRoot%\Forms" "%RootDrive%\Windows\Forms" /e /i >Nul: 2>&1
:Skip2

Rem
Rem ARTGALRY.CAG�� ������� Windows ���͸��� �����մϴ�.
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip3
If Not Exist "%SystemRoot%\ArtGalry.Cag" Goto Skip3
Copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip3

Rem
Rem Custom.dic ������ ����� ���͸��� �����մϴ�.
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip4
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip4
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip4

Rem
Rem Graph ������ ����� ���͸��� �����մϴ�.
Rem

If Exist "%RootDrive%\Office97\GR8GALRY.GRA" Goto Skip5
If Not Exist "#INSTDIR#\Office\GR8GALRY.GRA" Goto Skip5
Copy "#INSTDIR#\Office\GR8GALRY.GRA" "%RootDrive%\Office97\GR8GALRY.GRA" >Nul: 2>&1
:Skip5

If Exist "%RootDrive%\Office97\XL8GALRY.XLS" Goto Skip6
If Not Exist "#INSTDIR#\Office\XL8GALRY.XLS" Goto Skip6
Copy "#INSTDIR#\Office\XL8GALRY.XLS" "%RootDrive%\Office97\XL8GALRY.XLS" >Nul: 2>&1
:Skip6



