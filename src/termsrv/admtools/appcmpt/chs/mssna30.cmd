@echo Off
Rem
Rem  ����ű������� Microsoft SNA Server 3.0��ʹ������ 
Rem  Windows �ն˷���������ȷ���С�
Rem
Rem  ����ű������� SNA dll ע��Ϊϵͳͨ�ã��Ա� 
Rem  SNA ���������ڶ��û���������ȷ���С�
Rem
Rem  ��Ҫ��Ϣ! �����������ʾ����������ű��������ʹ��
Rem  ��װ SNA 3.0 �Ժ�򿪵�������ʾ����
Rem

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    ����û������ SNAROOT �����������޷����
Echo    ���û�Ӧ�ó�����������������������ű�
Echo    ������������ڰ�װ SNA Server 3.0 ֮ǰ�򿪵ģ�
Echo    ���ܻᷢ����������
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1

Rem ֹͣ SNA ����
net ֹͣ snabase

Rem �� SNA DLL ע��Ϊϵͳͨ��
Register %SNAROOT%\SNADMOD.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAMANAG.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\WAPPC32.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\DBGTRACE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\MNGBASE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNATRC.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALM.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNANW.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAIP.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNABASE.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASERVR.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASII.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALINK.DLL /SYSTEM >Nul: 2>&1

Rem �������� SNA ����
net ���� snabase

Echo SNA Server 3.0 ���û�Ӧ�ó�������ѽ���
Pause

:Cont2

