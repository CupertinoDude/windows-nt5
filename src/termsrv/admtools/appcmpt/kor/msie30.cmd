@Echo Off
Rem
Rem �� ��ũ��Ʈ�� Microsoft Internet
Rem Explorer 3.x ��ġ�� ������Ʈ�մϴ�. ĳ�� ���͸�,
Rem ��� ��� ���͸�, ��Ű ������ ����� Ȩ ���͸���
Rem �����մϴ�.
Rem

Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ��� �� ��ũ��Ʈ�� ���� �����Ǿ�����
Rem Ȯ���մϴ�.
Rem
Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msie30.Key msie30.Key
regini msie30.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Echo Microsoft Internet Explorer 3.x ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done

