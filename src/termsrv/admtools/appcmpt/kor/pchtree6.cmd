@Echo Off

Rem
Rem PeachTree�� ���� ���� ���α׷� ȣȯ ��ũ��Ʈ ��ġ �Ϸ�
Rem Accounting v6.0
Rem
Rem

Rem #########################################################################
Rem
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

REM
REM �ش� �α׿� ��ũ��Ʈ�� UsrLogn1.cmd�� �߰��մϴ�.
REM
FindStr /I ptr6Usr %SystemRoot%\System32\UsrLogn1.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call ptr6Usr.Cmd >> %SystemRoot%\System32\UsrLogn1.Cmd
:Skip1

Rem #########################################################################
Echo.
Echo   Peachtree 6.0�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   Peachtree Complete Accounting v6.0�� �����ؾ� �մϴ�.
Echo.
Echo PeachTree 6.0 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done
