@Echo Off

Rem #########################################################################

Rem
Rem ����֤ %RootDrive% �Ѿ������ã���Ϊ����ű����øñ�����
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �� WMsgUsr.Cmd ��ӵ� UsrLogn2.Cmd �ű�
Rem

FindStr /I WMsgUsr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call WMsgUsr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1


Echo.
Echo   Ҫ��֤ Windows Messaging ���������У�������
Echo   �κ�Ӧ�ó���֮ǰ��Ŀǰ��¼���û�������ע����
Echo   �����µ�¼��
Echo.
Echo Windows Messaging ���û�Ӧ�ó�������ѽ���
Pause

:Done

