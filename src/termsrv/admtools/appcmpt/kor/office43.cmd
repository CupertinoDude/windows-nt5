@Echo Off

Cls

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Echo.
Echo �� ��ũ��Ʈ�� �����ϱ� ����, Administrator�� Access �۾� ���͸���
Echo ������� Office private ���͸��� �����ؾ� �մϴ�.
Echo.
Echo ������ �� �ƹ� Ű�� ������ ��ӵ˴ϴ�.
Echo.
Echo �׷��� ������ Administrator�� ���� �ܰ踦 �����ؾ� �մϴ�:
Echo     MS Access�� �����Ͽ� [����] �޴����� [�ɼ�]�� �����մϴ�.
Echo     "�⺻ �����ͺ��̽� ���͸�"�� "%RootDrive%\OFFICE43"���� �ٲߴϴ�.
Echo     MS Access�� �����մϴ�.
Echo.
Echo ����: [����] �޴��� ������ �� �����ͺ��̽��� ������ �մϴ�.
Echo.
Echo �� �ܰ踦 �Ϸ��� �� �ƹ� Ű�� ������ ��ӵ˴ϴ�...

Pause > NUL:

Echo.
Echo MS Office 4.3�� "%SystemDrive%\MSOFFICE"�� �ƴ� �ٸ� ���͸��� ��ġ�� ���,
Echo Ofc43ins.cmd�� ������Ʈ�ؾ� �մϴ�.
Echo.
Echo Ofc43ins.cmd ������Ʈ�� �����Ϸ��� �ƹ� Ű�� �����ʽÿ�...
Echo.
Pause > NUL:
Notepad Ofc43ins.cmd
Pause

Call ofc43ins.cmd

..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.cmd ..\Logon\ofc43usr.cmd
..\acsr "#SYSTEMROOT#" "%SystemRoot%" ..\Logon\Template\ofc43usr.key ..\Logon\Template\ofc43usr.bak
..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.bak ..\Logon\ofc43usr.key
Del /F /Q ..\Logon\Template\ofc43usr.bak

Rem
Rem ����:  �� ��ũ��Ʈ ���� CACLS ��ɾ�� NTFS�� ���˵� ��Ƽ�ǿ�����
Rem ���� �� �ֽ��ϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

regini Office43.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem
Rem Office 4.3�� INI ���� ������Ʈ
Rem

Rem Excel�� Office Toolbar���� ������ �� msoffice.ini�� �ִ� �۾� ���͸���
Rem �����մϴ�. Office Toolbar�� ǥ�� ������ Excel�� Word �ڿ� �� ��°��
Rem ǥ���մϴ�. msoffice.ini�� ������, %SystemRoot%�� �������
Rem ���Դϴ�.

..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp1" "1,1,"
..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp2" "2,1,,%RootDrive%\office43"

..\Aciniupd /e "Winword6.ini" "Microsoft Word" USER-DOT-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Winword6.ini" "Microsoft Word" WORKGROUP-DOT-PATH "%OFC43INST%\WINWORD\TEMPLATE"
..\Aciniupd /e "Winword6.ini" "Microsoft Word" INI-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Winword6.ini" "Microsoft Word" DOC-PATH "%RootDrive%\OFFICE43"
..\Aciniupd /e "Winword6.ini" "Microsoft Word" AUTOSAVE-PATH "%RootDrive%\OFFICE43"

..\Aciniupd /e "Excel5.ini" "Microsoft Excel" DefaultPath "%RootDrive%\OFFICE43"
..\Aciniupd /e "Excel5.ini" "Spell Checker" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

..\Aciniupd /k "Msacc20.ini" Libraries "WZTABLE.MDA" "%RootDrive%\OFFICE43\ACCESS\WZTABLE.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZLIB.MDA" "%RootDrive%\OFFICE43\ACCESS\WZLIB.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZBLDR.MDA" "%RootDrive%\OFFICE43\ACCESS\WZBLDR.MDA"
..\Aciniupd /e "Msacc20.ini" Options "SystemDB" "%RootDrive%\OFFICE43\ACCESS\System.MDA"

Rem
Rem WIN.INI ������Ʈ
Rem

..\Aciniupd /e "Win.ini" "MS Proofing Tools" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

Rem
Rem Artgalry ������ ��� ���� �ٲٱ�
Rem

cacls "%SystemRoot%\Msapps\Artgalry" /E /G "Terminal Server User":F > NUL: 2>&1

Rem
Rem MSQuery ������ ��� ���� �ٲٱ�
Rem

cacls "%SystemRoot%\Msapps\MSQUERY" /E /G "Terminal Server User":C > NUL: 2>&1

Rem
Rem Admin�� ���� ������ ������ �����Ƿ�, Msacc20.ini�� Admin�� windows ���͸��� �����մϴ�.
Rem

Copy "%SystemRoot%\Msacc20.ini" "%UserProfile%\Windows\" > NUL: 2>&1

Rem ��ġ ���α׷��� ������Ʈ�� Ű�� �������� �ʵ��� ���� ������ ����ϴ�.

Copy NUL: "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1
attrib +h "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1

FindStr /I Ofc43Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto cont2
Echo Call Ofc43Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:cont2

Echo.
Echo Administrator�� �α׿����ߴٰ� �ٽ� �α׿��ؾ� ���� ������ ����˴ϴ�.
Echo   �α׿��� ��, Administrator�� ClipArt Gallery�� �ʱ�ȭ�ϱ� ���� ���� �ܰ踦
Echo   �����ؾ� �մϴ� :
Echo       Word�� �����Ͽ� [��ü ����]�� �����մϴ�.
Echo       Microsoft ClipArt Gallery�� �����մϴ�.
Echo       ��Ÿ�� Ŭ����Ʈ�� �������� ���� [Ȯ��]�� �����ϴ�.
Echo       ClipArt Gallery �� Word�� �����ϴ�.
Echo.
Echo Microsoft Office 4.3 ���� ����� ���� ���α׷� Ʃ���� �Ϸ�Ǿ����ϴ�.
Echo.
Pause

:Done
