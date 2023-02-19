
@Echo Off

Rem
Rem ����:  �� ��ũ��Ʈ ���� CACLS ��ɾ�� NTFS�� ���˵� ��Ƽ�ǿ�����
Rem ���� �� �ֽ��ϴ�.
Rem

Rem #########################################################################

Rem
Rem ����� �������Ͽ��� Corel Office 7 ���͸��� �����մϴ�.
Rem ����, us ������ All Users Profile�� ����ǵ���
Rem ����ڸ� ���� ���� ���� �մϴ�.
Rem

Rem ���� ���� ��忡 ���� ������ ��ġ ���� �����մϴ�.

ChgUsr /query > Nul:
if ErrorLevel 101 Goto Begin1
Set __OrigMode=Install
Change User /Execute > Nul:
:Begin1


Rem ���� ��尡 ��ġ ����̾����� ��ġ ���� ���ϴ�.
If "%__OrigMode%" == "Install" Change User /Install > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rmdir "%USER_START_MENU%\Corel Office 7" /Q >Nul: 2>&1


Rem
Rem ����ڰ� Ű ������ �����ϴ� ���� ���� �ݴϴ�.
Rem


if "%RootDrive%"=="w:" goto PostDriveChange
if "%RootDrive%"=="W:" goto PostDriveChange

Echo   Ȩ ���͸��� coffice7.key ���Ͽ� �����Ǿ� �־�� �մϴ�.
Echo.
Echo   ���� �ܰ踦 �����ʽÿ�:
Echo     1) ���� ǥ���� Ȩ ���͸��� ���� ASCII ���� ã�� ���ʽÿ�.
Echo        ������� Ȩ ���͸��� %RootDrive%�Դϴ�.
Echo.
Echo        A = 61   E = 65   I = 69   M = 6D   Q = 71   U = 75   Y = 79
Echo        B = 62   F = 66   J = 6A   N = 6E   R = 72   V = 76   Z = 7A
Echo        C = 63   G = 67   K = 6B   O = 6F   S = 73   W = 77   
Echo        D = 64   H = 68   L = 6C   P = 70   T = 74   X = 78
Echo.
Echo     2) Notepad�� ���۵Ǹ� ��� 77��
Echo        1 �ܰ迡�� ã�� ������ �����Ͻʽÿ�.
Echo        ����: �ٲٴ� �۾� ���� ���ʿ��� �� ĭ�� �߰����� �ʴ��� Ȯ���Ͻʽÿ�.
Echo     3) ������ �����ϰ� �����ʽÿ�. �� ��ũ��Ʈ�� ��ӵ˴ϴ�.
Echo.
Pause

NotePad "%SystemRoot%\Application Compatibility Scripts\Install\coffice7.key"

:PostDriveChange


Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin2
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin2

regini COffice7.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem �α׿� ��ũ��Ʈ�� �����Ͽ� ��ġ ��ġ���� iBase �����ͺ��̽��� �����մϴ�.
Rem




..\ACRegL %Temp%\COffice7.Cmd COffice7Loc "HKLM\Software\PerfectOffice\Products\InfoCentral\7" "ExeLocation" "StripChar\2"

If ErrorLevel 1 Goto InstallError

Call %Temp%\COffice7.Cmd 
Del %Temp%\COffice7.Cmd >Nul: 2>&1


..\ACIniUpd /e "%COffice7Loc%\ICWin7\Local\Wpic.ini" Preferences Last_IBase "%RootDrive%\Personal\iBases\Personal\Personal"
If ErrorLevel 1 Goto InstallError


..\acsr "#COFFICE7INST#" "%COffice7Loc%\\" ..\Logon\Template\cofc7usr.Cmd ..\Logon\cofc7usr.Cmd
If ErrorLevel 1 Goto InstallError

goto PostInstallError
:InstallError

Echo.
Echo ������Ʈ������ Corel Office 7 ��ġ ��ġ�� �˻��� �� �����ϴ�.
Echo Corel Office 7�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:PostInstallError

Rem #########################################################################

Rem 
Rem WordPerfect ���ø��� �б� �������� �����մϴ�.
Rem �����ϱ� ���� ����ڰ� �����ϵ��� �����մϴ�.
Rem �ٸ� ����� �� ����ڰ� �������� ���ø� ���͸���
Rem �����ϵ��� �ϴ� ���Դϴ�.
Rem

attrib +r %COffice7Loc%\Template\*wpt /s >Nul: 2>&1

Rem #########################################################################

Rem
Rem COfc7Usr.Cmd�� UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

FindStr /I COfc7Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call COfc7Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################


Rem
Rem "Personal" ���͸��� ������ ����ϴ�.
Rem Admin�� �Ʒ� ǥ�õ� ��� �������� �ܰ踦 �����ؾ� �ϹǷ� ����
Rem �Ϸ�Ǿ�� �մϴ�.
Rem  

If Not Exist "%RootDrive%\Personal" Md "%RootDrive%\Personal"

Rem #########################################################################

cls
Echo.
Echo   Quattro Pro �⺻ ���͸��� ���� �ܰ迡 ����
Echo   �������� �����ؾ� �մϴ�:
Echo     1) ����ٿ��� 'change user /install'�� �Է��Ͻʽÿ�.
Echo     2) Quattro Pro�� �����Ͻʽÿ�.
Echo     3) "����-�켱 ����" �޴� �׸��� �����Ͻʽÿ�.
Echo     4) "���� �ɼ�" ������ ���ʽÿ�.
Echo     5) ���͸��� %RootDrive%\Personal�� �����Ͻʽÿ�.
Echo     6) ���α׷��� ��ġ�ʽÿ�.
Echo     7) ����ٿ��� 'change user /execute'�� �Է��Ͻʽÿ�.
Echo.
pause

Echo.
Echo   Corel Office 7�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Corel Office 7 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done
