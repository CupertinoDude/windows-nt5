
Set _CHKROOT=PASS

Cd "%SystemRoot%\Application Compatibility Scripts"

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont2


Echo REM > RootDrv2.Cmd
Echo REM   �b����o�����ε{���ۮe�ʫ��O�ɫe�A�z������ >> RootDrv2.Cmd
Echo REM   ���w�@�ө|���Φb�׺ݾ����A�����Ϻо�   >> RootDrv2.Cmd
Echo REM   �N���A�Υ��ӹ����C�ӨϥΪ̪��D�ؿ��C >> RootDrv2.Cmd
Echo REM   �Ч�s�o���ɮ׵����� "Set RootDrive" �ԭz���A�� >> RootDrv2.Cmd
Echo REM   ���X�һݪ��Ϻо��N���C�p�G�z�S���ߦn�]�w�A >> RootDrv2.Cmd
Echo REM   ��ĳ�z�ϥκϺо� W:�C�Ҧp: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM            Set RootDrive=W: >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM   �`�N�ƶ�:  �нT�w�b�Ϻо��N���Ϋ_���᭱�S���Ů�C >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo REM   �b�z�����o�Ӥu�@��A���x�s�o���ɮרõ��� >> RootDrv2.Cmd
Echo REM   �O�ƥ��A�H�~��������ε{���ۮe�ʫ��O�ɡC >> RootDrv2.Cmd
Echo REM >> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd
Echo Set RootDrive=>> RootDrv2.Cmd
Echo. >> RootDrv2.Cmd

NotePad RootDrv2.Cmd

Call RootDrv.Cmd
If Not "A%ROOTDRIVE%A" == "AA" Goto Cont1

Echo.
Echo     �b����o�����ε{���ۮe�ʫ��O�ɫe�A�z������
Echo     ���w�@�ӺϺо��N���A�Υ��ӹ����C�ӨϥΪ̪�
Echo     �D�ؿ��C
Echo.
Echo     ���O�ɲפ�C
Echo.
Pause

Set _CHKROOT=FAIL
Goto Cont3


:Cont1

Rem
Rem  �I�s�ϥΪ̵n�J���O�ɨӹ����{�b���ںϺо��A
Rem  �H�K�i�����ε{���w�ˡC
Rem 

Call "%SystemRoot%\System32\UsrLogon.Cmd


:Cont2

Rem  �]�w RootDrive �n�����X

echo HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Terminal Server > chkroot.key
echo     RootDrive = REG_SZ %ROOTDRIVE%>> chkroot.key

regini chkroot.key > Nul: 


:Cont3

Cd "%SystemRoot%\Application Compatibility Scripts\Install"
