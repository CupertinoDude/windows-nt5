@echo Off

Rem
Rem �� ��ũ��Ʈ�� DiskKeeper 2.0�� ������Ʈ�Ͽ� Administration
Rem ���α׷��� �ùٸ��� �۵��ϵ��� �մϴ�.

Rem #########################################################################

Rem
Rem ������Ʈ������ DiskKeeper 2.0�� ��ġ ��ġ�� ����ϴ�. ������ ������,
Rem ���� ���α׷��� ��ġ�Ǿ� ���� �ʴ� ������ �����ϰ� ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL %Temp%\DK20.Cmd DK20INST "HKLM\Software\Microsoft\Windows\CurrentVersion\App Paths\DKSERVE.EXE" "Path" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ������Ʈ������ DiskKeeper 2.0 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo ���� ���α׷��� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\DK20.Cmd
Del %Temp%\DK20.Cmd >Nul: 2>&1

Rem #########################################################################



register %DK20INST% /system >Nul: 2>&1

Echo DiskKeeper 2.x ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done
