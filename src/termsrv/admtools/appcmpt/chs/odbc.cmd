@echo off
Rem
Rem  ����ű����� ODBC ��־�ļ���λ�á�
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\ODBC.Key ODBC.Key
regini ODBC.Key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo.
Echo   ���� SQL ������Դʱ���뵥�����û� DSN��ѡ��ϵġ�ѡ�
Echo   ��ť��Ȼ���ٵ����������ļ�����ť�����ġ���ѯ��־����
Echo   ��ͳ����־���ļ��������Ǳ������û��ĸ�
Echo   ������(%RootDrive%)�ϡ�
Echo.
Echo   ���⣬ϵͳ����Ա����Ϊ�����û���������
Echo   ��Դ�����ȣ��򿪡�������ڡ�����������
Echo   "Change User /Install"��Ȼ������������Դ��
Echo   �������������������� "Change User /Execute"��
Echo   ����ִ��ģʽ��
Echo.

Echo ODBC ���û�Ӧ�ó�������ѽ���
Pause

:Done

