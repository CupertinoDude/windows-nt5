@echo off
Rem
Rem  ����ű����� Dr. Watson �İ�װ���ýű�
Rem  ������־�ļ�·���͹���ת���ļ����������ļ�
Rem  �������û�����Ŀ¼�С�
Rem
Rem  ע�⣬��Ҳ����ͨ������ drwtsn32 Ӧ�ó�����
Rem  �ı���Щλ�á�
Rem

Rem #########################################################################

Rem
Rem ����֤ %RootDrive% �Ѿ������ã���Ϊ����ű����øñ�����
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem ���Ŀǰ���ڰ�װģʽ�У���ĳɰ�װģʽ��
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\DrWatson.Key DrWatson.Key
regini DrWatson.Key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo Dr. Watson ���û�Ӧ�ó�������ѽ���
Pause

:Done
