@echo Off

Rem
Rem  ����ű������� DiskKeeper 2.0���Ա����
Rem  �����ܹ��������С�

Rem #########################################################################

Rem
Rem ��ע����ȡ DiskKeeper 2.0 �İ�װλ�á����
Rem �Ҳ�������ٶ�Ӧ�ó���û�а�װ����ʾ������Ϣ��
Rem

..\ACRegL %Temp%\DK20.Cmd DK20INST "HKLM\Software\Microsoft\Windows\CurrentVersion\App Paths\DKSERVE.EXE" "Path" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ DiskKeeper 2.0 ��װλ�á�
Echo ����֤��Ӧ�ó����Ƿ��Ѿ���װ�����ٴ����� 
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\DK20.Cmd
Del %Temp%\DK20.Cmd >Nul: 2>&1

Rem #########################################################################



register %DK20INST% /system >Nul: 2>&1

Echo DiskKeeper 2.x ���û�Ӧ�ó�������ѽ�����
Pause

:Done
