@Echo off
Cls
Rem
Echo �����ڸ� ���� Corel WordPerfect Suite 8�� ��ũ��Ʈ�� ��ġ�մϴ�.
Echo.
Echo Net Setup�� netadmin �������� netsetup.exe�� �ҷ��� ���Դϴ�.
Echo Node Setup�� net setup �Ŀ� ó������ �α׿��� ���Դϴ�.
Echo **�� ��ũ��Ʈ�� net setup ��  **
Echo **�� node setup �� �ٽ� ����Ǿ�� �մϴ�.**  (�ſ� �߿��մϴ�. Admin���� ���� ���Դϴ�) 
Echo.
Echo �ƹ� Ű�� ������ ��ӵ˴ϴ�...
Pause > Nul:
cls
Echo.
Echo Corel WordPerfect Suite 8�� Netsetup.exe�� ����Ͽ� ��ġ�ؾ� �մϴ�.
Echo.
Echo Netsetup.exe�� �������� �ʾ����� ���� �����ʽÿ�(Ctrl-c�� �����ʽÿ�.)
Echo �׸��� ���� �ܰ踦 �����Ͽ� ��ġ�� �ݺ��Ͻʽÿ�.
Echo   [������]���� WordPerfect Suite 8�� �����Ͻʽÿ�.
Echo   ���������� ���� [���α׷� �߰�/����]�� Ŭ���Ͻʽÿ�.
Echo   Corel 8 CD-ROM�� NetAdmin ���͸����� Netsetup.exe�� �����Ͻʽÿ�.
Echo   Corel WordPerfect Suite 8�� ���� Net Setup�� ��ġ�ʽÿ�.
Echo.
Echo �׷��� ������ �ƹ� Ű�� ������ ��ӵ˴ϴ�...

Pause > NUL:

Echo.
Echo Corel WordPerfect Suite ��Ʈ��ũ ������
Echo "D:\Corel"�� ��ġ���� �ʾ�����, Administrator�� Cofc8ins.cmd�� �����ϰ�
Echo "D:\Corel"�� �ش� ������ ��ġ�� ���͸���
Echo �ٲپ�� �մϴ�.
Echo.
Echo Cofc8ins.cmd ������Ʈ�� �����Ϸ��� �ƹ� Ű�� �����ʽÿ�...

Pause > NUL:

Notepad Cofc8ins.cmd

Echo.
Pause

Call Cofc8ins.cmd

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


..\acsr "#WPSINST#" "%WPSINST%" Template\Coffice8.key Coffice8.key
..\acsr "#WPSINST#" "%WPSINST%" ..\Logon\Template\Cofc8usr.cmd %temp%\Cofc8usr.tmp
..\acsr "#WPSINSTCMD#" "%WPSINST%\Suite8\Corel WordPerfect Suite 8 Setup.lnk" %temp%\Cofc8usr.tmp ..\Logon\Cofc8usr.cmd
Del %temp%\Cofc8usr.tmp >Nul: 2>&1

FindStr /I Cofc8Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip0
Echo Call Cofc8Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip0

Echo ġ����: Node ��ġ�� �����ϼ̽��ϱ�?
Echo ġ����: ���� �� Net setup�� �Ϸ������� 
Echo ġ����: ���� Node setup�� �����ؾ� �մϴ�. 
Echo ġ����: ���� �ܰ迡 ���� Node setup�� �����Ͻʽÿ�:
Echo ġ����:  1. Control-C�� ���� �� ��ũ��Ʈ�� �����ʽÿ�.
Echo ġ����:  2. �α׿����ϰ� Admin���� �α����Ͻʽÿ�.
Echo ġ����:  3. [���α׷� �߰�/����]���� [ã�ƺ���] ����� ����Ͽ�
Echo ġ����:     (�Ǵ� �����/��ġ ��� ���濡��]
Echo ġ����:     \corel\Suite 8\Corel WordPerfect Suite 8 ��ġ �ٷ� ���⸦ �����Ͻʽÿ�.
Echo ġ����:  4. ��ġ�ϴ� ���� ������� %RootDrive%�� �����Ͻʽÿ�.
Echo ġ����:  5. Node Setup�� ��ģ �� �� ��ũ��Ʈ�� �ٽ� �����Ͻʽÿ�.

Rem Administrator�� 
Rem Corel WordPerfect Suite 8 Node Setup�� ��ģ �� �� ��ũ��Ʈ�� �ٽ� �����ؾ� �մϴ�.
Rem.
Rem Administrator�� Node Setup�� �Ϸ����� �ʾ����� Ctrl-C�� ����
Rem ��ũ��Ʈ�� ������ �α׿����Ͻʽÿ�. Administrator�� �ٽ� �α׿��ϰ�
Rem Corel WordPerfect Suite Node setup�� �����ϰ� ���� ���α׷���
Rem ����� Ȩ ���͸�: %RootDrive%�� ��ġ�Ͻʽÿ�.
Rem.
Echo �׷��� ������ �ƹ� Ű�� ������ ��ӵ˴ϴ�...

Pause > NUL:


If Not Exist "%COMMON_START_MENU%\Corel WordPerfect Suite 8" Goto skip1
Move "%COMMON_START_MENU%\Corel WordPerfect Suite 8" "%USER_START_MENU%\" > NUL: 2>&1


:skip1


If Not Exist "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" Goto skip2
Move "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" "%USER_STARTUP%\" > NUL: 2>&1

:skip2

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.

Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:

:Begin
regini COffice8.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem IDAPI ������ �ִ� ������ �����մϴ�.
Rem �� ��ũ��Ʈ ���� CACLS ��ɾ�� NTFS�� ���˵� ��Ƽ�ǿ����� ���� �� �ֽ��ϴ�.

Cacls "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" /E /T /G "Authenticated Users":C > Nul: 2>&1
Move "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" "%WPSINST%\Suite8\Shared\Idapi\" > NUL: 2>&1
Del /F /Q "%UserProfile%\Corel\Suite8\Shared\Idapi"

Echo Corel WordPerfect Suite 8 ���� ����� ���� ���α׷� ������ �Ϸ��߽��ϴ�.
Echo.
Echo Administrator�� Node Setup ���� ������ ����� ��ġ
Echo ������ ������ �� �ֽ��ϴ�. �ڼ��� ������ �¶��� ������ �аų�
Echo Corel �� ����Ʈ�� �����Ͻʽÿ�.
Echo   http://www.corel.com/support/netadmin/admin8/Installing_to_a_client.htm
Echo.

Pause

:Done


