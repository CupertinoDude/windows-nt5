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

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem #########################################################################



Rem #########################################################################

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0" "BinDirPath" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ Word 97 ��װλ�á�
Echo ����֤ Word 97 �Ƿ��Ѿ���װ�����ٴ�����
Echo ����ű���
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem �������û��ġ��������˵�ɾ�������ٲ��ҡ���
Rem �����ٲ��ҡ�������Դ����Ӱ��ϵͳ���ܡ�
Rem

If Exist "%COMMON_STARTUP%\Microsoft �ļ�����.lnk" Del "%COMMON_STARTUP%\Microsoft �ļ�����.lnk" >Nul: 2>&1


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
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\MsWord97.Key MsWord97.Tmp 
..\acsr "#__SharedTools#" "%__SharedTools%" MsWord97.Tmp MsWord97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" MsWord97.Tmp2 MsWord97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" MsWord97.Tmp3 MsWord97.key
Del MsWord97.Tmp >Nul: 2>&1
Del MsWord97.Tmp2 >Nul: 2>&1
Del MsWord97.Tmp3 >Nul: 2>&1
regini MsWord97.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���� Wrd97Usr.Cmd ����ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Wrd97Usr.Cmd ..\Logon\Wrd97Usr.Cmd

FindStr /I Wrd97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Wrd97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Ҫ��֤ MS Word 97 ���������У��������κ�Ӧ��
Echo   ����֮ǰ��Ŀǰ��¼���û�������ע������
Echo   ���µ�¼��
Echo.
Echo Microsoft Word 97 ���û�Ӧ�ó�������ѽ���
Pause

:Done





