
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
Rem ��ע����ȡ Project 98 �İ�װλ�á�����Ҳ�����
Rem ��ٶ� Project û�а�װ����ʾ������Ϣ��
Rem

..\ACRegL %Temp%\Proj98.Cmd PROJINST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "OfficeBin" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ Project 98 �İ�װλ�á�
Echo ����֤ Project 98 �Ƿ��Ѿ���װ�����ٴ�����
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\Proj98.Cmd 
Del %Temp%\Proj98.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �Ѱ�װ Office 97������ Project 98 ��װ�ű�
Rem �ѽ�ģ���Ƶ��˵�ǰ�û���Ŀ¼��
Rem �����Ƿ���һ��ͨ�õĵط���Proj98Usr.cmd �Ὣ�����ƻ�ȥ��
Rem






If NOT Exist "%RootDrive%\Office97\Templates\Microsoft Project"  goto skip10
If Exist  "%PROJINST%\..\Templates\Microsoft Project" goto skip10
xcopy "%RootDrive%\Office97\Templates\Microsoft Project" "%PROJINST%\..\Templates\Microsoft Project" /E /I /K > Nul: 2>&1

:skip10






If NOT Exist "%RootDrive%\Office97\Templates\Microsoft Project Web"  goto skip11
If Exist  "%PROJINST%\..\Templates\Microsoft Project Web" goto skip11
xcopy "%RootDrive%\Office97\Templates\Microsoft Project Web" "%PROJINST%\..\Templates\Microsoft Project Web" /E /I /K > Nul: 2>&1

:skip11

Rem #########################################################################

Rem
Rem ʹ Global.mpt �ļ���Ϊֻ���� 
Rem ���򣬵�һ������ Project ���û���ı� ACL
Rem

if Exist "%PROJINST%\Global.mpt" attrib +r "%PROJINST%\Global.mpt"


Rem #########################################################################

Rem
Rem ���� Office 97 ���µ�ϵͳ DLL �ϵ�ÿ����
Rem �ж�ȡȨ��
Rem
If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R > NUL: 2>&1

Rem #########################################################################

Rem
Rem ���� Powerpoint �� Excel ������(�ļ�/���Ϊ HTML����)
Rem ����� MsForms.Twd �ļ���������Щ����ʱ������ 
Rem ���ú����������ݵ��ļ��滻 
Rem ��Ӧ���ļ���
Rem
If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul:
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F > Nul: 2>&1
:Cont2

Rem #########################################################################

Rem
Rem �������û��ġ��������˵�ɾ�������ٲ��ҡ���
Rem �����ٲ��ҡ�������Դ����Ӱ��ϵͳ
Rem ���ܡ�
Rem


If Exist "%COMMON_STARTUP%\Microsoft �ļ�����.lnk" Del "%COMMON_STARTUP%\Microsoft �ļ�����.lnk"

Rem #########################################################################

Rem
Rem ����ע����ʹ·��ָ���û����е�
Rem Ŀ¼��
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj98.Key msproj98.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" msproj98.tmp msproj98.Key
Del msproj98.tmp >Nul: 2>&1
regini msproj98.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem ���� proj97Usr.Cmd ����ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#INSTDIR#" "%PROJINST%" ..\Logon\Template\prj98Usr.Cmd ..\Logon\prj98Usr.Cmd

FindStr /I prj98Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call prj98Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   Ҫ��֤ Project 98 ���������У��������κ�
Echo   Ӧ�ó���֮ǰ��Ŀǰ��¼���û�������ע����
Echo   �����µ�¼��
Echo.
Echo Microsoft Project 98 ���û�Ӧ�ó�������ѽ���
Pause

:done

