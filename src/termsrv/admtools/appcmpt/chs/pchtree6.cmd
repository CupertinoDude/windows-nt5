@Echo Off

Rem
Rem	PeachTree ��װӦ�ó�������Խű��ѽ��� 
Rem     Accounting v6.0
Rem
Rem

Rem #########################################################################
Rem
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

REM
REM  ����Ӧ�ĵ�¼�ű���ӵ� UsrLogn1.cmd
REM
FindStr /I ptr6Usr %SystemRoot%\System32\UsrLogn1.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call ptr6Usr.Cmd >> %SystemRoot%\System32\UsrLogn1.Cmd
:Skip1

Rem #########################################################################
Echo.
Echo   Ҫ��֤ Peachtree 6.0 ���������У�������
Echo   Peachtree Commplate Accounting v6.0 ֮ǰ��Ŀǰ��¼
Echo   ���û�������ע���������µ�¼��
Echo.
Echo PeachTree 6.0 ���û�Ӧ�ó�������ѽ���
Pause

:done
