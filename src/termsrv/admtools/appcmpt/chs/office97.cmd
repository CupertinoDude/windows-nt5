
@Echo Off

Rem
Rem  ע��:  ����ű��е� CACLS ����ֻ�� NTFS 
Rem         ��ʽ���Ĵ��̷�������Ч��
Rem

Rem #########################################################################

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
Rem ���ļ��ӵ�ǰ�û�ģ�帴�Ƶ������û�ģ��λ��
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem
Rem ��ע����ȡ Office 97 �İ�װλ�á����
Rem �Ҳ�������ٶ� Office û�а�װ����ʾ������Ϣ��
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ Office 97 ��װλ�á�
Echo ����֤ Office 97 �Ƿ��Ѱ�װ�����ٴ�����
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �� Access 97 ϵͳ���ݿ�ĳ�ֻ����������
Rem ����û�����ͬʱ�������ݿ⡣���ǣ�������
Rem ����ø���ϵͳ���ݿ�Ĺ��ܡ�����ϵͳ���ݿ�
Rem ͨ����ͨ��������/��ȫ���á��˵�ѡ����еġ�
Rem �����Ҫ����û������������Ƚ�д��Ȩ�Ż�
Rem ϵͳ���ݿ��ϡ�
Rem

If Not Exist %SystemRoot%\System32\System.Mdw Goto Cont1
cacls %SystemRoot%\System32\System.Mdw /E /P "Authenticated Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P "Power Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P Administrators:R >NUL: 2>&1

:Cont1

Rem #########################################################################

Rem
Rem ���� Office 97 ���µ�ϵͳ DLL �ϵ�ÿ����
Rem ��ȡȨ��
Rem

REM If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R >NUL: 2>&1

Rem #########################################################################

Rem #########################################################################

Rem
Rem ����ı� Outlook �� frmcache.dat �ļ��ϵ� TS �û��ķ���Ȩ�ޡ�
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################


Rem
Rem �� Powerpoint �򵼸ĳ�ֻ�����Ա����ͬʱ����
Rem һ�����ϵ��򵼡�
Rem




If Exist "%O97INST%\Templates\��ʾ�ĸ�\������ʾ��.pwz" Attrib +R "%O97INST%\Templates\��ʾ�ĸ�\������ʾ��.pwz" >Nul: 2>&1

If Exist "%O97INST%\Office\Ppt2html.ppa" Attrib +R "%O97INST%\Office\Ppt2html.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\bshppt97.ppa" Attrib +R "%O97INST%\Office\bshppt97.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\geniwiz.ppa" Attrib +R "%O97INST%\Office\geniwiz.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\ppttools.ppa" Attrib +R "%O97INST%\Office\ppttools.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\graphdrs.ppa" Attrib +R "%O97INST%\Office\graphdrs.ppa" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���Ҫʹ�ǹ���Ա�û��� Excel ������ Access Wizards 
Rem �� Access ���������Ҫɾ���������е� "Rem"�� 
Rem �����û��������ļ���Ȩ�ޡ�
Rem 
Rem

Rem If Exist "%O97INST%\Office\WZLIB80.MDE" cacls "%O97INST%\Office\WZLIB80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZMAIN80.MDE" cacls "%O97INST%\Office\WZMAIN80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZTOOL80.MDE" cacls "%O97INST%\Office\WZTOOL80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 

Rem #########################################################################

Rem
Rem ���� MsForms.Twd �� RefEdit.Twd �ļ����������ļ��� 
Rem Powerpoint �� Excel �����(�ļ�/���Ϊ HTML��etc)����ġ� 
Rem ������Щ����ʱ�����ǻ��ú����������ݵ��ļ� 
Rem �滻��Ӧ���ļ���
Rem

If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul: 2>&1
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont2

If Exist %systemroot%\system32\RefEdit.Twd Goto Cont3
Copy Nul: %systemroot%\system32\RefEdit.Twd >Nul: 2>&1
Cacls %systemroot%\system32\RefEdit.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont3

Rem #########################################################################

Rem
Rem �� SystemRoot �´��� msremote.sfs Ŀ¼���������û� 
Rem ʹ�á�������塱�еġ��ʼ��ʹ��桱ͼ�������������ļ���
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1

Rem #########################################################################

Rem
Rem �������û��ġ��������˵�ɾ�������ٲ��ҡ���
Rem �����ٲ��ҡ�������Դ����Ӱ��ϵͳ
Rem ���ܡ�
Rem

If Exist "%COMMON_STARTUP%\Microsoft �ļ�����.lnk" Del "%COMMON_STARTUP%\Microsoft �ļ�����.lnk" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �������û��ġ���ʼ���˵���ɾ�� "Microsoft Office ��ݹ�����.lnk" �ļ���
Rem

If Exist "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" Del "%COMMON_STARTUP%\Microsoft Office ��ݹ�����.lnk" >Nul: 2>&1

Rem #########################################################################
Rem
Rem �� SystemRoot �´��� msfslog.txt �ļ����������ն˷����� 
Rem �û�����ļ�����ȫ����Ȩ�� 
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem ����ע����ʹ·��ָ���û�����
Rem ��Ŀ¼��
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office97.Key Office97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office97.Tmp Office97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" Office97.Tmp2 Office97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office97.Tmp3 Office97.Key
Del Office97.Tmp >Nul: 2>&1
Del Office97.Tmp2 >Nul: 2>&1
Del Office97.Tmp3 >Nul: 2>&1

regini Office97.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���� Ofc97Usr.Cmd ����ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Ofc97Usr.Cmd ..\Logon\Ofc97Usr.Cmd

FindStr /I Ofc97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Ҫ��֤ Office 97 ���������У��������κ�
Echo   Office 97 Ӧ�ó���֮ǰ��Ŀǰ��¼�������û�
Echo   ������ע���������µ�¼��
Echo.
Echo Microsoft Office 97 ���û�Ӧ�ó�������ѽ���
Pause

:done
