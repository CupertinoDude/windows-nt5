@Echo Off

Rem #########################################################################

Rem
Rem CMD Ȯ���� ����� �� �ִ��� Ȯ���մϴ�.
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c netcom40.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

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
Rem NetScape(4.5x�� 4.0x�� �ٸ�) ������ ����ϴ�.
Rem

..\ACRegL "%Temp%\NS4VER.Cmd" NS4VER "HKLM\Software\Netscape\Netscape Navigator" "CurrentVersion" "STRIPCHAR(1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ������Ʈ������ Netscape Communicator 4 ������ �˻��� �� �����ϴ�.
Echo Communicator�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ�
Echo �� ��ũ��Ʈ�� �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\NS4VER.Cmd"
Del "%Temp%\NS4VER.Cmd" >Nul: 2>&1

if /i "%NS4VER%" LSS "4.5 " goto NS40x

Rem #########################################################################
Rem Netscape 4.5x

Rem
Rem ������Ʈ������ Netscape Communicator 4.5 ��ġ ��ġ�� ����ϴ�.
Rem ������ ������, Communicator�� ��ġ�Ǿ� ���� ���� ������ �����ϰ�
Rem ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL "%Temp%\NS45.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ������Ʈ������ Netscape Communicator 4.5 ������ �˻��� �� �����ϴ�.
Echo Communicator�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ�
Echo �� ��ũ��Ʈ�� �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont1
Call "%Temp%\NS45.Cmd"
Del "%Temp%\NS45.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem Com40Usr.Cmd�� ������Ʈ�Ͽ� �⺻ NetScape Users ���͸��� �ݿ��ϰ�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

..\acsr "#NSUSERDIR#" "%ProgramFiles%\Netscape\Users" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS40INST#" "%NS40INST%" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.tm2
..\acsr "#NS4VER#" "4.5x" ..\Logon\Com40Usr.tm2 ..\Logon\Com40Usr.Cmd

Rem #########################################################################

Rem
Rem "quick launch" �������� netscape ���͸��� �����Ͽ�
Rem ����ڰ� �� ����� ������ ���͸��� ������ �� �ְ� �մϴ�.
Rem

If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" "%NS40INST%"

goto DoUsrLogn

:NS40x
Rem #########################################################################
Rem Netscape 4.0x

Rem
Rem ������Ʈ������ Netscape Communicator 4 ��ġ ��ġ�� ����ϴ�.
Rem ������ ������, Communicator�� ��ġ�Ǿ� ���� ���� ������ �����ϰ�
Rem ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL "%Temp%\NS40.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" ""
If Not ErrorLevel 1 Goto Cont2
Echo.
Echo ������Ʈ������ Netscape Communicator 4 ������ �˻��� �� �����ϴ�.
Echo Communicator�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ�
Echo �� ��ũ��Ʈ�� �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont2
Call "%Temp%\NS40.Cmd"
Del "%Temp%\NS40.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �⺻ ���������� administrator�� Ȩ ���͸�����
Rem �˷��� ��ġ�� �����մϴ�. �� �������� �α׿��ϴ� ���� �� ����� ���͸���
Rem ����˴ϴ�. �۷ι� �⺻ �������� �̹� ������ ����� �ʽ��ϴ�.
Rem �׷��� ������ Admin�� �� ��ũ��Ʈ�� �ξ� ���߿� �����Ͽ�
Rem ��� ���� ������ �۷ι� �⺻ �����ʷ� �̵��մϴ�.
Rem

If Exist %RootDrive%\NS40 Goto Cont3
Echo.
Echo %RootDrive%\NS40�� �⺻ �������� �����ϴ�.
Echo ����� ������ �����ڸ� �����ϰ�  "Default"��� �̸��� �������� �ϳ� ����ʽÿ�.
Echo ������ ��θ� �Է��϶�� ������ ���� ǥ�õ� ��θ� ����Ͻʽÿ�.
Echo ��� �̸� �� ���� ���� �̸� �׸��� ��� �νʽÿ�. �ٸ� �������� ������
Echo ����ʽÿ�. �� �ܰ踦 �Ϸ��ϸ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done
 
:Cont3
If Exist "%NS40INST%\DfltProf" Goto Cont4
Xcopy "%RootDrive%\NS40" "%NS40INST%\DfltProf" /E /I /K >NUL: 2>&1
:Cont4

Rem #########################################################################

Rem 
Rem ����� ������ ������ ���� �޴� �ٷ� ���⿡�� ����ڿ� ���� �б� ������ �����Ͻʽÿ�.
Rem �׷��� �ϸ� �Ϲ� ����ڰ� �� ����� �������� �߰��� �� �����ϴ�.
Rem �׷��� administrator�� ������ ����� ������ �����ڸ� ������ �� �ֽ��ϴ�.
Rem

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" Goto Cont5
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont5

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" Goto Cont6
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont6

Rem #########################################################################

Rem
Rem Com40Usr.Cmd�� ������Ʈ�Ͽ� ���� ��ġ ���͸��� �ݿ��ϰ� �̸�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

..\acsr "#PROFDIR#" "%NS40INST%\DfltProf" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS4VER#" "4.0x" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.Cmd

:DoUsrLogn

del ..\Logon\Com40Usr.tmp >Nul: 2>&1
del ..\Logon\Com40Usr.tm2 >Nul: 2>&1

FindStr /I Com40Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Com40Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Netscape Communicator�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Netscape Communicator 4 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done

