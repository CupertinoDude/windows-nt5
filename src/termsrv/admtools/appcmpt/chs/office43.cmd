@Echo Off

Cls

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Echo.
Echo ����������ű�֮ǰ������ԱӦ�ý� Access ����Ŀ¼
Echo �ĳ��û��� Office ר��Ŀ¼��
Echo.
Echo һ���������������밴�����������
Echo.
Echo ���򣬹���ԱӦ��ִ�����в���s:
Echo     ���� MS Access ���ӡ��鿴���˵�ѡ��ѡ�
Echo     ����Ĭ�����ݿ�Ŀ¼���ĳ� "%RootDrive%\OFFICE43"
Echo     �˳� MS Access
Echo.
Echo ע��: ��������Ҫ�����µ����ݿ���ܿ������鿴���˵���
Echo.
Echo �����Щ������밴���������...

Pause > NUL:

Echo.
Echo ����� MS Office 4.3 ��װ����Ŀ¼���� "%SystemDrive%\MSOFFICE"�� 
Echo ����Ҫ���� Ofc43ins.cmd��
Echo.
Echo �밴���������ʼ���� Ofc43ins.cmd ...
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
Rem  ע��:  ����ű��е� CACLS ����ֻ�� NTFS 
Rem         ��ʽ���Ĵ��̷�������Ч��
Rem

Rem ���Ŀǰ���ڰ�װģʽ�У���ĳɰ�װģʽ��
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

regini Office43.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem
Rem ���� Office 4.3 �� INI �ļ�
Rem

Rem �� Office ���������� Excel ʱ���� msoffice.ini �� 
Rem Ϊ�����ù���Ŀ¼��Office �������ı�׼���ý� Excel 
Rem ���ڵڶ�λ���� Word ���档��� msoffice.ini �в����ڣ� 
Rem ���� %SystemRoot% �´���

..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp1" "1,1,"
..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp2" "2,1,,%RootDrive%\office43"

..\Aciniupd /e "Word6.ini" "Microsoft Word" USER-DOT-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Word6.ini" "Microsoft Word" WORKGROUP-DOT-PATH "%OFC43INST%\WINWORD\TEMPLATE"
..\Aciniupd /e "Word6.ini" "Microsoft Word" INI-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "Word6.ini" "Microsoft Word" DOC-PATH "%RootDrive%\OFFICE43"
..\Aciniupd /e "Word6.ini" "Microsoft Word" AUTOSAVE-PATH "%RootDrive%\OFFICE43"

..\Aciniupd /e "Excel5.ini" "Microsoft Excel" DefaultPath "%RootDrive%\OFFICE43"
..\Aciniupd /e "Excel5.ini" "Spell Checker" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

..\Aciniupd /k "Msacc20.ini" Libraries "WZTABLE.MDA" "%RootDrive%\OFFICE43\ACCESS\WZTABLE.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZLIB.MDA" "%RootDrive%\OFFICE43\ACCESS\WZLIB.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZBLDR.MDA" "%RootDrive%\OFFICE43\ACCESS\WZBLDR.MDA"
..\Aciniupd /e "Msacc20.ini" Options "SystemDB" "%RootDrive%\OFFICE43\ACCESS\System.MDA"

Rem
Rem ���� WIN.INI
Rem

..\Aciniupd /e "Win.ini" "MS Proofing Tools" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

Rem
Rem ���� Artgalry �ļ��е�Ȩ��
Rem

cacls "%SystemRoot%\Msapps\Artgalry" /E /G "Terminal Server User":F > NUL: 2>&1

Rem
Rem ���� MSQuery �ļ��е�Ȩ��
Rem

cacls "%SystemRoot%\Msapps\MSQUERY" /E /G "Terminal Server User":C > NUL: 2>&1

Rem
Rem ��Ϊ����Ա�� Msacc20.ini �Ǿɵģ����临�Ƶ�����Ա�� Windows Ŀ¼��
Rem

Copy "%SystemRoot%\Msacc20.ini" "%UserProfile%\Windows\" > NUL: 2>&1

Rem ����һ��α�ļ����Ա㰲װ�����޷�����ע����

Copy NUL: "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1
attrib +h "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1

FindStr /I Ofc43Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto cont2
Echo Call Ofc43Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:cont2

Echo.
Echo Ҫʹ�Ķ���Ч������ԱӦ����ע���������µ�¼��
Echo   ��¼�󣬹���Ա��Ӧ��ִ�����в�����
Echo   ��ʼ�� ClipArt Gallery:
Echo       ���� Word ��ѡ�񡰲�����󡱡�
Echo       ѡ�� Microsoft ClipArt Gallery��
Echo       ����ȷ������������ʾ�ļ�������
Echo       �˳� ClipArt Gallery �� Word.
Echo.
Echo Microsoft Office 4.3 ���û�Ӧ�ó�������ѽ�����
Echo.
Pause

:Done
