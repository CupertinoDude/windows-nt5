@Echo Off

Rem #########################################################################

Rem
Rem ��֤ CMD ��չ���Ƿ�������
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c netcom40.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

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
Rem ��ȡ NetScape �汾(4.5x �� 4.0x �Ĳ�һ��)
Rem

..\ACRegL "%Temp%\NS4VER.Cmd" NS4VER "HKLM\Software\Netscape\Netscape Navigator" "CurrentVersion" "STRIPCHAR(1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������ Netscape Communicator 4 �汾�� 
Echo ����֤ Communicator �Ƿ��Ѿ���װ�����ٴ� 
Echo ��������ű���
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\NS4VER.Cmd"
Del "%Temp%\NS4VER.Cmd" >Nul: 2>&1


if /i "%NS4VER%" LSS "4.5 " goto NS40x

Rem #########################################################################
Rem Netscape 4.5x

Rem
Rem ��ע����ȡ Netscape Communicator 4.5 �İ�װλ�á� 
Rem ����Ҳ�������ٶ� Communicator û�а�װ�� 
Rem ��ʾ������Ϣ��
Rem

..\ACRegL "%Temp%\NS45.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �޷���ע������ Netscape Communicator 4.5 ��װλ�á� 
Echo ����֤ Communicator �Ƿ��Ѿ���װ�����ٴ����� 
Echo ����ű���
Echo.
Pause
Goto Done

:Cont1
Call "%Temp%\NS45.Cmd"
Del "%Temp%\NS45.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���� Com40Usr.Cmd ����ӳĬ�� NetScape �û�Ŀ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#NSUSERDIR#" "%ProgramFiles%\Netscape\Users" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS40INST#" "%NS40INST%" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.tm2
..\acsr "#NS4VER#" "4.5x" ..\Logon\Com40Usr.tm2 ..\Logon\Com40Usr.Cmd

Rem #########################################################################

Rem
Rem ���� "quick launch" ͼ�굽 Netscape ��װĿ¼�Ա�
Rem �����Ǹ��Ƶ�ÿ���û��������ļ�Ŀ¼��
Rem

If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" "%NS40INST%"

goto DoUsrLogn

:NS40x
Rem #########################################################################
Rem Netscape 4.0x

Rem
Rem ��ע����ȡ Netscape Communicator 4.5 �İ�װλ�á� 
Rem ����Ҳ�������ٶ� Communicator û�а�װ�� 
Rem ��ʾ������Ϣ��
Rem

..\ACRegL "%Temp%\NS40.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" ""
If Not ErrorLevel 1 Goto Cont2
Echo.
Echo �޷���ע������ Netscape Communicator 4 ��װλ�á� 
Echo ����֤ Communicator �Ѿ���װ������������ 
Echo ����ű���
Echo.
Pause
Goto Done

:Cont2
Call "%Temp%\NS40.Cmd"
Del "%Temp%\NS40.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ��Ĭ�������ļ��ӹ���Ա����Ŀ¼���Ƶ�һ��
Rem ��֪λ�á��ڵ�¼�ڼ䣬��������ļ��ᱻ����
Rem ��ÿ���û���Ŀ¼�����ȫ��Ĭ�������ļ��Ѵ��ڣ���Ҫ��
Rem ���д�����򣬹���Ա���ܻ����Ժ���������ű�ʱ
Rem �����и�����Ϣ�Ƶ�ȫ��Ĭ�������ļ���
Rem

If Exist %RootDrive%\NS40 Goto Cont3
Echo.
Echo �� %RootDrive%\NS40 ���Ҳ���Ĭ�������ļ���������
Echo �û������ļ�������������һ����Ϊ "Default" �������ļ���
Echo �������������ļ�·������ʾʱ������������ʾ��·����
Echo ��Ҫ��д���ƺ͵����ʼ�������Ŀ��������ڱ�������ļ��� 
Echo ������ɾ���������Щ������������� 
Echo ����ű���
Echo.
Pause
Goto Done
 
:Cont3
If Exist "%NS40INST%\DfltProf" Goto Cont4
Xcopy "%RootDrive%\NS40" "%NS40INST%\DfltProf" /E /I /K >NUL: 2>&1
:Cont4

Rem #########################################################################

Rem 
Rem ���û������ļ��������ġ���ʼ���˵���ݷ�ʽ
Rem ɾ���û��Ķ�ȡȨ�����ֹ��ͨ�û�����µ��û�
Rem �����ļ���������Ա���ɿ��������û������ļ���������
Rem

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" Goto Cont5
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont5

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" Goto Cont6
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont6

Rem #########################################################################

Rem
Rem ���� Com40Usr.Cmd ����ӳʵ�ʰ�װĿ¼��
Rem ������ӵ� UsrLogn2.Cmd �ű�
Rem

..\acsr "#PROFDIR#" "%NS40INST%\DfltProf" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS4VER#" "4.0x" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.Cmd

:DoUsrLogn

del ..\Logon\Com40Usr.tmp >Nul: 2>&1
del ..\Logon\Com40Usr.tm2 >Nul: 2>&1

FindStr /I Com40Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Com40Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Ҫ��֤ Netscape Communicator ���������У��������κ�
Echo   Ӧ�ó���֮ǰ��Ŀǰ��¼���û�������ע������
Echo   ���µ�¼
Echo. 
Echo Netscape Communicator 4 ���û�Ӧ�ó�������ѽ���
Pause

:done

