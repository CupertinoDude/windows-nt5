
Set _CHKROOT=PASS

Cd "%SystemRoot%\Application Compatibility Scripts"

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont2


Echo REM > RootDrv2.Cmd
Echo Rem �� ���� ���α׷� ȣȯ ��ũ��Ʈ�� �����ϱ� ����, >> RootDrv2.Cmd
Echo Rem ���� �͹̳� ������ ���ǰ� ���� �ʴ� ����̺� ���ڸ� �����Ͽ� �� �������   >> RootDrv2.Cmd
Echo Rem Ȩ ���͸��� �����ؾ� �մϴ�. >> RootDrv2.Cmd
Echo Rem ���ϴ� ����̺� ���ڸ� �����Ϸ��� "Set RootDrive" ���� �� ���� >> RootDrv2.Cmd
Echo Rem ������Ʈ�Ͻʽÿ�. ��ȣ�ϴ� ���ڰ� ������ >> RootDrv2.Cmd
Echo Rem W: ����̺긦 �����մϴ�. ���� ���: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM            Set RootDrive=W: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo Rem ����:  ����̺� ���ڿ� �ݷ� ���̿� �� ĭ�� ������ Ȯ���Ͻʽÿ�. >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo Rem �� �۾��� �Ϸ�������, �� ������ �����ϰ� >> RootDrv2.Cmd
Echo Rem NotePad�� ������ ����ؼ� ���� ���α׷� ȣȯ ��ũ��Ʈ�� �����Ͻʽÿ�. >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd
Echo Set RootDrive=>> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd

NotePad RootDrv2.Cmd

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont1

Echo.
Echo     �� ���� ���α׷� ȣȯ ��ũ��Ʈ�� �����ϱ� ����,
Echo     �� ������� Ȩ ���͸��� ������ ����̺� ���ڸ�
Echo     �����Ͻʽÿ�.
Echo.
Echo     ��ũ��Ʈ�� �����߽��ϴ�.
Echo.
Pause

Set _CHKROOT=FAIL
Goto Cont3


:Cont1

Rem
Rem ����� �α׿� ��ũ��Ʈ�� ȣ���Ͽ� ��Ʈ ����̺긦 �����մϴ�. ����
Rem ��ġ ���� ���α׷��� ���� �� �ֽ��ϴ�.
Rem 

Call "%SystemRoot%\System32\UsrLogon.Cmd


:Cont2

Rem ������Ʈ���� RootDrive Ű�� �����մϴ�.

echo HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Terminal Server > chkroot.key
echo     RootDrive = REG_SZ %ROOTDRIVE%>> chkroot.key

regini chkroot.key > Nul: 


:Cont3

Cd "%SystemRoot%\Application Compatibility Scripts\Install"
