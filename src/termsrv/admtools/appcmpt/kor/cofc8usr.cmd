Rem
Rem ����ڰ� ���α׷� ��ġ�� ��带 �����ϰ� ���� ������
Rem ���� �����Ͻʽÿ�.
Rem 

If Exist "%RootDrive%\Corel" Goto Skip1

Rem ��ȭ�� ��ġ
Rem #WPSINST#�� Corel ��Ʈ��ũ ������ �ִ� ����̺��
Rem �ٲٽʽÿ�.

If Not Exist "#WPSINST#\Suite8\Corel WordPerfect Suite 8 Setup.lnk" Goto cont2
#WPSINST#\Suite8\#WPSINSTCMD#
Goto skip0
:cont2

..\End.cmd

:Skip0

Cls
Echo Corel ��ġ ���α׷��� ��ģ ��, �ƹ� Ű�� ������ ����մϴ�...
Pause > NUL:

:Skip1

Rem
Rem ��� DOD �纻�� �����ϰ� �˴ϴ�. �׷��Ƿ� ��� ������� ���� ���� �����Ͻʽÿ�.
Rem

If Not Exist "%COMMON_START_MENU%\Corel WordPerfect Suite 8" Goto Skip2
If Exist "%USER_START_MENU%\Corel WordPerfect Suite 8" Goto Cont1
  Move "%COMMON_START_MENU%\Corel WordPerfect Suite 8" "%USER_START_MENU%\Corel WordPerfect Suite 8" > NUL: 2>&1
  Goto Skip2
:Cont1
  Rd /S /Q "%COMMON_START_MENU%\Corel WordPerfect Suite 8"
:Skip2  

If Not Exist "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" Goto Skip3
If Exist "%USER_STARTUP%\Corel Desktop Application Director 8.LNK" Goto Cont2
  Move "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" "%USER_STARTUP%\" > NUL: 2>&1
  Goto Skip3  
:Cont2
  Del /F /Q "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK"
:Skip3

If Not Exist "%RootDrive%\Windows\Twain_32\Corel" Goto Skip4
Rd /S /Q "%RootDrive%\Windows\Twain_32\Corel"

:Skip4

If Not Exist "%RootDrive%\Corel\Suite8\PhotoHse\PhotoHse.ini" Goto Skip5
..\Aciniupd /e "%RootDrive%\Corel\Suite8\PhotoHse\PhotoHse.ini" "Open File Dialog" "Initial Dir" "%RootDrive%\MyFiles"

:Skip5



