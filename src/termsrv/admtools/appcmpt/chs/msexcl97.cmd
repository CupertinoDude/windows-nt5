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
Rem ���ļ��ӵ�ǰ�û�ģ�帴�Ƶ������û�ģ��λ��
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1


Rem #########################################################################

Rem
Rem ��ע����ȡ Excel 97 �İ�װλ�á�����Ҳ�����
Rem ��ٶ�û�а�װ Office ����ʾ������Ϣ��
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0" "BinDirPath" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ Excel 97 ��װλ�á�
Echo ����֤ Excel 97 �Ƿ��Ѿ���װ�����ٴ�����
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �������û��ġ��������˵�ɾ�������ٲ��ҡ��������ٲ��ҡ�
Rem ������Դ����Ӱ��ϵͳ���ܡ�
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���� MsForms.Twd �� RefEdit.Twd �ļ����������ļ��� 
Rem Powerpoint �� Excel ������(�ļ�/���Ϊ HTML���ȵ�)������ġ� 
Rem ���������κ�һ���������Ƕ����ú����������ݵ��ļ� 
Rem �滻��Ӧ�ļ���
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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\MsExcl97.Key MsExcl97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" MsExcl97.Tmp MsExcl97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" MsExcl97.Tmp2 MsExcl97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" MsExcl97.Tmp3 MsExcl97.key
Del MsExcl97.Tmp >Nul: 2>&1
Del MsExcl97.Tmp2 >Nul: 2>&1
Del MsExcl97.Tmp3 >Nul: 2>&1

regini MsExcl97.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���� Exl97Usr.Cmd �Է�ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Exl97Usr.Cmd ..\Logon\Exl97Usr.Cmd

FindStr /I Exl97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Exl97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Ҫ��֤ MS Excel 97 ���������У��������κ�Ӧ��
Echo   ����֮ǰ��Ŀǰ��¼���û�������ע����������
Echo   ��¼��
Echo.
Echo Microsoft Excel 97 ���û�Ӧ�ó�������ѽ���
Pause

:Done
