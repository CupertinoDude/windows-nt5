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
Rem ��ע����ȡ Office 95 �İ�װλ�á�����Ҳ�����
Rem ��ٶ� Office û�а�װ����ʾ������Ϣ��
Rem

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0

..\ACRegL %Temp%\O95.Cmd O95INST "HKLM\Software\Microsoft\Microsoft Office\95\InstallRootPro" "" ""
If Not ErrorLevel 1 Goto Cont0

Echo.
Echo �޷���ע������ Office 95 ��װλ�á�
Echo ����֤ Office 95 �Ƿ��Ѿ���װ������������
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O95.Cmd
Del %Temp%\O95.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �������û��ġ��������˵�ɾ�������ٲ��ҡ���
Rem �����ٲ��ҡ�������Դ����Ӱ��ϵͳ���ܡ���Ҫ
Rem ͬʱ��鵱ǰ�û��˵��������û��˵��������ٲ��ҡ�
Rem �������ĸ��˵�ȡ����ϵͳ�Ƿ��ѷ���ִ��
Rem ģʽ��
Rem

If Not Exist "%USER_STARTUP%\Microsoft Office ��ݹ�����.lnk" Goto Cont1
Del "%USER_STARTUP%\Microsoft Office ��ݹ�����.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" Goto Cont2
Del "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem �� PowerPoint �� Binder �ļ����Ƶ������û�Ŀ¼���Ա�
Rem ���û���¼ʱ���Ը��Ƶ�ÿ���û�����Ŀ¼
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER70.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER70.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\PPT70.PPT" copy "%UserProfile%\%TEMPLATES%\PPT70.PPT" "%ALLUSERSPROFILE%\%TEMPLATES%" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem ����ע����ʹ·��ָ���û����е�Ŀ¼��
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office95.Key Office95.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office95.Tmp Office95.Tmp2
..\acsr "#INSTDIR#" "%O95INST%" Office95.Tmp2 Office95.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office95.Tmp3 Office95.Key
Del Office95.Tmp >Nul: 2>&1
Del Office95.Tmp2 >Nul: 2>&1
Del Office95.Tmp3 >Nul: 2>&1
regini Office95.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���� Ofc95Usr.Cmd ����ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#INSTDIR#" "%O95INST%" ..\Logon\Template\Ofc95Usr.Cmd ..\Logon\Ofc95Usr.Cmd

FindStr /I Ofc95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   Ҫ��֤ Office 95 ���������У��������κ�
Echo   Office 95 Ӧ�ó���֮ǰ��Ŀǰ��¼���û�����
Echo   ��ע���������µ�¼��
Echo.
Echo Microsoft Office 95 ���û�Ӧ�ó�������ѽ���
Pause

:Done