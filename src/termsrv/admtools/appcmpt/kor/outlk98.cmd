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
Rem ������Ʈ������ Outlook 98 ��ġ ��ġ�� ����ϴ�. ������ ������,
Rem Outlook�� ��ġ�Ǿ� ���� �ʴ� ������ �����ϰ� ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL %Temp%\O98.Cmd O98INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "OfficeBin" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ������Ʈ������ Outlook 98 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo Outlook 98�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O98.Cmd
Del %Temp%\O98.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem ������Ʈ�� Ű�� �����Ͽ� ��ΰ� ����� ������
Rem ���͸��� ����Ű���� �մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin


REM
REM Office97�� ��ġ�Ǿ� �ְų� Office�� ��ġ�Ǿ� ���� ������ Office97 per-user dir�� ����մϴ�.
REM Office95�� ��ġ�Ǿ� ������ Office95 per-user dir�� ����մϴ�.
REM
Set OffUDir=Office97

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto OffChk

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Off95

..\ACRegL %Temp%\Off.Cmd OFFINST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Off95

set OFFINST=%O98INST%
goto Cont1

:Off95
Set OffUDir=Office95

:OffChk

Call %Temp%\Off.Cmd
Del %Temp%\Off.Cmd >Nul: 2>&1

:Cont1

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Outlk98.Key Outlk98.Tmp
..\acsr "#INSTDIR#" "%OFFINST%" Outlk98.Tmp Outlk98.Tmp2
..\acsr "#OFFUDIR#" "%OffUDir%" Outlk98.Tmp2 Outlk98.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Outlk98.Tmp3 Outlk98.Key


Del Outlk98.Tmp >Nul: 2>&1
Del Outlk98.Tmp2 >Nul: 2>&1
Del Outlk98.Tmp3 >Nul: 2>&1

regini Outlk98.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem Olk98Usr.Cmd�� ������Ʈ�Ͽ� ���� ��ġ ���͸��� �ݿ��ϰ� �̸�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

..\acsr "#INSTDIR#" "%OFFINST%" ..\Logon\Template\Olk98Usr.Cmd Olk98Usr.Tmp
..\acsr "#OFFUDIR#" "%OffUDir%" Olk98Usr.Tmp ..\Logon\Olk98Usr.Cmd
Del Olk98Usr.Tmp

FindStr /I Olk98Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Olk98Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Rem
Rem SystemRoot �Ʒ��� msremote.sfs�� ����ϴ�. �̷��� �ϸ�
Rem �������� ���� �� �������� "���� �� �ѽ�"�� ����� �� �ְ� �մϴ�.
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1



Rem #########################################################################

Rem
Rem outlook�� ���� frmcache.dat ���Ͽ��� �͹̳� ���� ����ڿ� ���� ���� �׼����� ����մϴ�.
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################


Rem #########################################################################

Rem
Rem SystemRoot �Ʒ��� msfslog.txt ������ ����� Terminal Server Users����
Rem �� ���Ͽ� ���� ��� ������ �ݴϴ�.
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


Echo.
Echo   Outlook 98�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   Outlook 98�� �����Ͻʽÿ�.
Echo.
Echo Microsoft Outlook 98 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done

