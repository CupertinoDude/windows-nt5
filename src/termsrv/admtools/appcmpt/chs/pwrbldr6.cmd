@Echo Off

Rem #########################################################################

Rem
Rem ȷ���Ƿ� %RootDrive% �ѱ����ã��������ڸ�����ű�
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

..\ACRegL "%Temp%\PB6.Cmd" PB6INST "HKCU\Software\ODBC\ODBC.INI\Powersoft Demo DB V6" "DataBaseFile" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �޷���ע������л�� PowerBuilder 6.0 ��װλ�á�
Echo ȷ���Ƿ� PowerBuilder 6.0 �ѱ���װ�������������и�����ű���
Echo 
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\PB6.Cmd"
Del "%Temp%\PB6.Cmd" >Nul: 2>&1

Rem
Rem ����ע����·��ָ���û��ض�
Rem Ŀ¼��
Rem

Rem �����ǰ�����ڰ�װģʽ���ı䵽��װģʽ��
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\pwrbldr6.Key pwrbldr6.key

regini pwrbldr6.key > Nul:

Rem ���ԭ����ִ��ģʽ���ı��ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ���� PBld6Usr.Cmd �Է�ӳ�����İ�װĿ¼
Rem ������ӵ� UsrLogn2.Cmd ����ű���
Rem

..\acsr "#INSTDIR#" "%PB6INST%" ..\Logon\Template\PBld6Usr.Cmd ..\Logon\PBld6Usr.cmd

FindStr /I PBld6Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call PBld6Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   Ϊ�˱�֤ PowerBuilder 6.0 ����ȷ��������ǰ��¼�û�
Echo   ����ע���Լ����µ�¼��Ȼ������ PowerBuilder 6.0 ��
Echo.
Echo Eudora 4.0 ���û�Ӧ�ó��������ϡ�
Pause

:done
