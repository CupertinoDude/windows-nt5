@Echo Off

Rem #########################################################################

Rem
Rem ����֤ %RootDrive% �Ѿ������ã���Ϊ����ű����øñ�����
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem �ӡ��������˵�ɾ�������ٲ��ҡ��������ٲ��ҡ�
Rem ������Դ����Ӱ��ϵͳ���ܡ�
Rem


If Not Exist "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" Goto Cont1
Del "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%USER_STARTUP%\Microsoft Office ��ݹ�����.lnk" Goto Cont2
Del "%USER_STARTUP%\Microsoft Office ��ݹ�����.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem ���û��ʵ��Ƶ��û�Ŀ¼��
Rem

Rem ���Ŀǰ���ڰ�װģʽ�У���ĳɰ�װģʽ��
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

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem  �ڵ�¼ʱ����ȡ����ļ���
Rem  �����û�����Ȩ��

Cacls ..\Logon\Template\prj95usr.key /E /P "Authenticated Users":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem �� proj95Usr.Cmd ��ӵ� UsrLogn2.Cmd �ű�
Rem

FindStr /I prj95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call prj95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2


Rem #########################################################################

Echo.
Echo   ����Ա���԰���Щ�����ÿ���û�һ��Ψһ��
Echo   Ĭ��Ŀ¼:
Echo.
Echo    1) �ñ�����갴ť��������ʼ���˵���
Echo    2) �ӵ����˵�ѡ����Դ���������û�����
Echo       ����Դ�������������֡�
Echo    3) ˫���������ֱߵġ������ļ��С�
Echo    4) �ñ�����갴ť�����������ֱߵ� Microsoft 
Echo       Project ͼ�ꡣ
Echo    5) �ӵ����˵�ѡ�����ԡ���
Echo    6) ѡ�񡰿�ݷ�ʽ��ѡ������� Start in: �ѡ��ȷ������
Echo.
Echo    ע��: ÿ���û�����ӳ�䵽�Լ���Ŀ¼�� %RootDrive%��
Echo          Start In: ���Ƽ�ֵΪ %RootDrive%\My Documents��
Echo.
Pause

Rem #########################################################################

Echo.
Echo   Ҫ��֤ Project 95 ���������У��������κ�Ӧ��
Echo   ����֮ǰ��Ŀǰ��¼���û�������ע������
Echo   ���µ�¼��
Echo.
Echo Microsoft Project 95 ���û�Ӧ�ó�������ѽ�����
Pause

:Done
