@Echo Off

Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem
Rem �̰��� RootDrive�� �ʿ���� ��ũ��Ʈ�� ���� ���Դϴ�.
Rem

If Not Exist "%SystemRoot%\System32\Usrlogn1.cmd" Goto cont0
Cd /d "%SystemRoot%\Application Compatibility Scripts\Logon"
Call "%SystemRoot%\System32\Usrlogn1.cmd"

:cont0

Rem
Rem ����� Ȩ ���͸� ����̺� ���ڸ� �����մϴ�. �����Ǿ� ����
Rem ������ �����ϴ�.
Rem

Cd /d %SystemRoot%\"Application Compatibility Scripts"
Call RootDrv.Cmd
If "A%RootDrive%A" == "AA" End.Cmd

Rem
Rem ����� Ȩ ���͸��� ����̺� ���ڷ� �����մϴ�.
Rem

Net Use %RootDrive% /D >NUL: 2>&1
Subst %RootDrive% "%HomeDrive%%HomePath%"
if ERRORLEVEL 1 goto SubstErr
goto AfterSubst
:SubstErr
Subst %RootDrive% /d >NUL: 2>&1
Subst %RootDrive% "%HomeDrive%%HomePath%"
:AfterSubst

Rem
Rem �� ���� ���α׷� ��ũ��Ʈ�� �ҷ��ɴϴ�. ��ġ ��ũ��Ʈ�� ����Ǹ�
Rem ���� ���α׷� ��ũ��Ʈ�� �ڵ����� UsrLogn2.Cmd�� �߰��˴ϴ�.
Rem

If Not Exist %SystemRoot%\System32\UsrLogn2.Cmd Goto Cont1

Cd Logon
Call %SystemRoot%\System32\UsrLogn2.Cmd

:Cont1

:Done
