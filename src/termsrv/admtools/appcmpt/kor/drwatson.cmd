@echo off
Rem
Rem �� ��ũ��Ʈ�� Dr. Watson ��ġ�� ������Ʈ�մϴ�.
Rem �̰��� �α� ���� ��� �� �ջ� ���� ������ ����� Ȩ ���͸���
Rem ��ġ��Ű���� �����մϴ�.
Rem
Rem drwtsn32 ���� ���α׷��� �����Ͽ� �� ��ġ��
Rem ������ ���� �ֽ��ϴ�.
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\DrWatson.Key DrWatson.Key
regini DrWatson.Key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo Dr. Watson ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done
