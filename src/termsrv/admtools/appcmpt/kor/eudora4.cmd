@Echo Off

Rem #########################################################################

Rem
Rem CMD Ȯ���� ����� �� �ִ��� Ȯ���մϴ�.
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c eudora4.cmd
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

Rem ������Ʈ������ Eudora ������� ����ϴ�.

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:1" 

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ������Ʈ������ Eudora Pro 4.0 ������� �˻����� ���߽��ϴ�.
Echo Eudora Pro 4.0�� ��ġ�Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1
set EudCmd=%EUDTMP:~0,-2%

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:2" 

If Not ErrorLevel 1 Goto Cont2
Echo.
Echo ������Ʈ������ Eudora Pro 4.0 ��ġ ���͸��� �˻����� ���߽��ϴ�.
Echo Eudora Pro 4.0�� ��ġ�Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont2
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1

Set EudoraInstDir=%EUDTMP:~0,-13%

Rem #########################################################################

If Exist "%EudoraInstDir%\descmap.pce" Goto Cont0
Echo.
Echo �� ���� ���α׷� ȣȯ ��ũ��Ʈ�� ����ϱ� ���� Eudora 4.0�� �����ؾ� �մϴ�.
Echo Eudora�� ������ �� , Eudora Pro�� �ִ� Eudora Pro �ٷ� ���⿡ ����
Echo ����� �Ӽ��� ������Ʈ�մϴ�.
Echo   %RootDrive%\eudora.ini ����
Echo ��� �߰��մϴ�. �׷��� �����:
Echo  "%EudoraInstDir%\Eudora.exe" %RootDrive%\eudora.ini�� �˴ϴ�.
Echo.
Pause
Goto Done

:Cont0

Rem
Rem ������Ʈ�� Ű�� �����Ͽ� ��ΰ� ����� ������
Rem ���͸��� ����Ű���� �մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#INSTDIR#" "%EudoraInstDir%" Template\Eudora4.Key Eudora4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" Eudora4.tmp Eudora4.key

regini eudora4.key > Nul:
del eudora4.tmp
del eudora4.key

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem descmap.pce�� ��� ������ ������Ʈ�մϴ�.
cacls "%EudoraInstDir%\descmap.pce" /E /G "Terminal Server User":R >NUL: 2>&1

Rem #########################################################################

Echo.
Echo   Eudora Pro 4.0�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   Eudora Pro 4.0�� �����ؾ� �մϴ�.
Echo.
Echo Eudora 4.0 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done
