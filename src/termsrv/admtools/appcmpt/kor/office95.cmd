@Echo Off

Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem ������Ʈ������ Office 95 ��ġ ��ġ�� ����ϴ�. ������ ������,
Rem Office�� ��ġ�Ǿ� ���� �ʴ� ������ �����ϰ� ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Cont0

Echo.
Echo ������Ʈ������ Office 95 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo Office 95�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O95.Cmd
Del %Temp%\O95.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem ��� ����ڿ� ���� [����] �޴����� [���� ã��]�� �����մϴ�. [���� ã��]��
Rem ���ҽ��� ���� �ʿ��ϹǷ� �ý��� ���ɿ� ������ �ݴϴ�.
Rem ���� ����� �޴� �� ��� ����� �޴��� �� Ȯ���� �ʿ䰡 �ֽ��ϴ�.
Rem �ý����� ���� ���� ���ƿԴ��� �ƴ����� ���� ��Ÿ���� �޴���
Rem �ٸ� �� �ֽ��ϴ�.
Rem

If Not Exist "%USER_STARTUP%\Microsoft Office Find Fast Indexer.lnk" Goto Cont1
Del "%USER_STARTUP%\Microsoft Office Find Fast Indexer.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%COMMON_STARTUP%\Microsoft Office Find Fast Indexer.lnk" Goto Cont2
Del "%COMMON_STARTUP%\Microsoft Office Find Fast Indexer.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem PowerPoint �� Binder ������ ��� ����� ���͸��� �����Ͽ�
Rem ����ڰ� �α����� �� �� ������� Ȩ ���͸��� ����� �� �ְ� �մϴ�.
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER70.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" copy "%UserProfile%\%TEMPLATES%\PPT70.PPT" "%ALLUSERSPROFILE%\%TEMPLATES%" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem ������Ʈ�� Ű�� �����Ͽ� ��ΰ� ����� ������ ���͸��� ����Ű���� �մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office95.Key Office95.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office95.Tmp Office95.Tmp2
..\acsr "#INSTDIR#" "%O95INST%" Office95.Tmp2 Office95.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office95.Tmp3 Office95.Key
Del Office95.Tmp >Nul: 2>&1
Del Office95.Tmp2 >Nul: 2>&1
Del Office95.Tmp3 >Nul: 2>&1
regini Office95.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem Ofc95Usr.Cmd�� ������Ʈ�Ͽ� ���� ��ġ ���͸��� �ݿ��ϰ� �̸�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

..\acsr "#INSTDIR#" "%O95INST%" ..\Logon\Template\Ofc95Usr.Cmd ..\Logon\Ofc95Usr.Cmd

FindStr /I Ofc95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

If Not Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto EchoCont
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto CAGx86
If Not Exist "%ProgramFiles(x86)%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" copy "%RootDrive%\Windows\ArtGalry.Cag" "%ProgramFiles(x86)%\Common Files\Microsoft Shared\Artgalry" /y
goto EchoCont3

:CAGx86
If Not Exist "%ProgramFiles%\Common Files\Microsoft Shared\Artgalry\ArtGalry.cag" copy "%RootDrive%\Windows\ArtGalry.Cag" "%ProgramFiles%\Common Files\Microsoft Shared\Artgalry" /y
goto EchoCont3

:EchoCont
Echo.
Echo   administrator�� ���� ������ Clip Art Gallery ������Ʈ ������ ��ġ�ؾ� �մϴ�.
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto EchoCont1
If Not Exist "%ProgramFiles(x86)%" goto EchoCont1
Echo   %systemdrive%\Program Files (x86)\Common Files\Microsoft Shared\Artgalry.
goto EchoCont2
:EchoCont1
Echo   %systemdrive%\Program Files\Common Files\Microsoft Shared\Artgalry.
:EchoCont2
Echo   Clip Art Gallery 2.0a�� ������ �ֽ��ϴ�.
Echo.
Echo      http://support.microsoft.com/support/downloads/dp2115.asp
Echo.
Echo   Clip Art Gallery�� ������Ʈ�� ��, administrator�� ����
Echo   ���� �ܰ踦 �����Ͽ� Clip Art Gallery�� �ʱ�ȭ�ؾ� �մϴ�:
Echo     - �α׿����ϰ� �ٽ� �α׿��Ͻʽÿ�.
Echo     - Word�� �����ϰ� [��ü ����]�� �����Ͻʽÿ�.
Echo     - Microsoft ClipArt Gallery�� �����Ͻʽÿ�.
Echo     - Ŭ�� ��Ʈ�� ���������� [Ȯ��]�� �����ʽÿ�.
Echo     - ClipArt Gallery �� Word�� �����ʽÿ�.
Echo     - %systemdir%\Application Compatibility Scripts\Install\Office95.cmd�� �ٽ� �����Ͻʽÿ�.
Echo     - �α׿����Ͻʽÿ�.
Echo.
Pause
:EchoCont3

Echo.
Echo   Office 95�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   Office 95 ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Microsoft Office 95 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done


