
Set _CHKROOT=PASS

Cd "%SystemRoot%\Application Compatibility Scripts"

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont2


Echo REM > RootDrv2.Cmd
Echo REM   ���������Ӧ�ó�������Խű�֮ǰ�������� >> RootDrv2.Cmd
Echo REM   ָ��һ���ն˷�����û��ʹ�õ��������ţ�  >> RootDrv2.Cmd
Echo REM   �Ա�ӳ�䵽ÿ���û�����Ŀ¼�� >> RootDrv2.Cmd
Echo REM   ��������ļ���β�� "Set RootDrive" ��� >> RootDrv2.Cmd
Echo REM   ��ָ����Ҫ���������š������û��ѡ���� >> RootDrv2.Cmd
Echo REM   ����ʹ�������� W:������: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM            Set RootDrive=W: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM   ע��:  ��ȷ�����������ź�ð�ź�û�пո� >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM   �������������뱣������ļ����˳� >> RootDrv2.Cmd
Echo REM   �����±�������������Ӧ�ó�������Խű��� >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd
Echo Set RootDrive=>> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd

NotePad RootDrv2.Cmd

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont1

Echo.
Echo     ���������Ӧ�ó�������Խű�֮ǰ��������
Echo     ָ��һ��Ҫӳ�䵽ÿ���û�����Ŀ¼��
Echo     �������š�
Echo.
Echo     ����ֹ�ű���
Echo.
Pause

Set _CHKROOT=FAIL
Goto Cont3


:Cont1

Rem
Rem  ���������û���¼�ű���ӳ�����������
Rem  �Ա�������װӦ�ó���
Rem 

Call "%SystemRoot%\System32\UsrLogon.Cmd


:Cont2

Rem  ����ע����е� RootDrive ��

echo HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Terminal Server > chkroot.key
echo     RootDrive = REG_SZ %ROOTDRIVE%>> chkroot.key

regini chkroot.key > Nul: 


:Cont3

Cd "%SystemRoot%\Application Compatibility Scripts\Install"
