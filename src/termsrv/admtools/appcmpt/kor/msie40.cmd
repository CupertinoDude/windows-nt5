@Echo off

Rem
Rem ��ġ �� Administrator�� �� ��ũ��Ʈ�� �����ؾ� �մϴ�.
Rem

CD /D "%SystemRoot%\Application Compatibility Scripts\Install" > NUL: 2>&1

If Not "A%1A" == "AA" Goto cont0

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

:cont0

If "A%1A" == "AA" ..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msie40.key msie40.key
..\acsr "#SYSTEMROOT#" "%SystemRoot%" Template\msie40s.key msie40s.key

If Exist "%SystemRoot%\Setup.ini" Goto Cont2
Copy NUL: "%SystemRoot%\Setup.ini" > NUL: 2>&1

:Cont2
Cacls "%SystemRoot%\Setup.ini" /E /T /G "Authenticated Users":F >Nul: 2>&1

If Exist "%SystemRoot%\Setup.old" Goto Cont3
Copy NUL: "%SystemRoot%\Setup.old" > NUL: 2>&1

:Cont3
Attrib +r "%SystemRoot%\Setup.old" > NUL: 2>&1

FindStr /I Msie4Usr %SystemRoot%\System32\UsrLogn1.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Cont4
Echo Call Msie4Usr.Cmd >> %SystemRoot%\System32\UsrLogn1.Cmd

:Cont4

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

If "A%1A" == "AA" regini Msie40.key > Nul:
regini Msie40s.key > NUL:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

If Not "A%1A" == "AA" Goto Done

Echo.
Echo   MS IE 4�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Microsoft Internet Explorer 4.x ���� ����� ���� ���α׷� ���� �Ϸ�

Rem IE AutoInstall ���� �Ͻ� �������� ���ʽÿ�.
Pause

:Done
