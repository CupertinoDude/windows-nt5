@echo off
Rem
Rem �� ��ũ��Ʈ�� ODBC �α� ���� ��ġ�� ������Ʈ�մϴ�.
Rem

Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\ODBC.Key ODBC.Key
regini ODBC.Key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo.
Echo   SQL ������ ������ ������ �� ����� DSN �ǿ��� [�ɼ�] ���߸�
Echo   Ŭ���ϰ� [������] ���߸� �����ʽÿ�.  
Echo   ���� �α� �� ��� �α׸� ����� ��Ʈ ����̺�
Echo   (%RootDrive%)�� �����ϵ��� �����Ͻʽÿ�.
Echo.
Echo   ���� administrator�� ��� ����ڿ� ���� ������ ������ ������ ��
Echo   �ֽ��ϴ�. ����, ��� â�� ���� "Change User /Install" �����
Echo   �Է��Ͻʽÿ�. �� ���� ������ ������ �����Ͻʽÿ�.
Echo   ���������� ��� â���� "Change User /Execute" ����� �Է��ϸ�
Echo   ���� ���� ���ư��ϴ�.
Echo.

Echo ODBC ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done

