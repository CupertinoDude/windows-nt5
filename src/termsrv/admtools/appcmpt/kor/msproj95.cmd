@Echo Off

Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem [����] �޴����� [���� ã��]�� �����մϴ�. [���� ã��]�� 
Rem ���ҽ��� ���� �ʿ��ϹǷ� �ý��� ���ɿ� ������ �ݴϴ�.
Rem


If Not Exist "%COMMON_STARTUP%\Microsoft Office Find Fast Indexer.lnk" Goto Cont1
Del "%COMMON_STARTUP%\Microsoft Office Find Fast Indexer.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%USER_STARTUP%\Microsoft Office Find Fast Indexer.lnk" Goto Cont2
Del "%USER_STARTUP%\Microsoft Office Find Fast Indexer.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem ����� ������ ����� ���͸��� �̵��մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj95.Key msproj95.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" msproj95.tmp msproj95.Key
Del msproj95.tmp >Nul: 2>&1
regini msproj95.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem �α׿��ϴ� ���� �� ������ �н��ϴ�.
Rem ����ڿ��� �׼����� �ݴϴ�.

Cacls ..\Logon\Template\prj95usr.key /E /P "Authenticated Users":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem proj95Usr.Cmd�� UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

FindStr /I prj95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call prj95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2


Rem #########################################################################

Echo.
Echo   Administrator�� ���� �ܰ迡 ���� �� ����ڿ���
Echo   ������ �⺻ ���͸��� �� �� �ֽ��ϴ�:
Echo.
Echo    1) ��ü ���콺 ���߷� [����] �޴��� Ŭ���Ͻʽÿ�.
Echo    2) �˾� �޴����� [��� ����� Ž��]�� �����Ͻʽÿ�.
Echo       Ž���Ⱑ ǥ�õ� ���Դϴ�.
Echo    3) â�� �����ʿ� �ִ� [���α׷�] ������ ���� Ŭ���Ͻʽÿ�.
Echo    4) ��ü ���콺 ���߷� â�� �����ʿ� �ִ� Microsoft Project �������� 
Echo       Ŭ���Ͻʽÿ�.
Echo    5) �˾� �޴����� [��� ����]�� �����Ͻʽÿ�.
Echo    6) [�ٷ� ����] ���� �����ϰ� [���� ��ġ:] �׸��� �����Ͻʽÿ�. [Ȯ��]�� �����ʽÿ�.
Echo.
Echo    ����: �� ����ڴ� �� Ȩ ���͸��� %RootDrive%�� ���εǾ� �ֽ��ϴ�.
Echo          [���� ��ġ:]�� ���� �����ϴ� ���� %RootDrive%\�� �����Դϴ�.
Echo.
Pause

Rem #########################################################################

Echo.
Echo   Project 95�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Microsoft Project 95 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:Done
