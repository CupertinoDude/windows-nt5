@echo Off
Rem
Rem �� ��ũ��Ʈ�� Microsoft SNA Server 3.0�� ������Ʈ�Ͽ�
Rem Windows Terminal Server���� �ùٸ��� �۵��ϵ��� �մϴ�.
Rem
Rem �� ��ũ��Ʈ�� ���� SNA dll�� �ý��� �۷ι��� ����Ͽ�
Rem SNA ������ ���� ����� ȯ�濡�� �ùٸ��� �۵��� �� �ְ� �մϴ�.
Rem
Rem �߿��� ����! �� ��ũ��Ʈ�� ��� ������Ʈ���� �����ϸ�
Rem SNA 3.0�� ��ġ�� ���Ŀ� ���� ��� ������Ʈ�� ����ϴ��� Ȯ���Ͻʽÿ�.
Rem

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    SNAROOT ȯ�� ������ �����Ǿ� ���� �����Ƿ� ���� �����
Echo    ���� ���α׷� ������ �Ϸ��� �� �����ϴ�. �� ��ũ��Ʈ��
Echo    �����ϴ� �� ���Ǵ� ��� ���� SNA Server 3.0�� 
Echo    ��ġ�ϱ� ���� �������� �̷� ����� �߻��� �� �ֽ��ϴ�.
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1

Rem SNA ���񽺸� �����մϴ�.
net stop snabase

Rem SNA DLL�� �ý��� �۷ι��� ����մϴ�.
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

Rem SNA ���񽺸� �ٽ� �����մϴ�.
net start snabase

Echo SNA Server 3.0 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Cont2

