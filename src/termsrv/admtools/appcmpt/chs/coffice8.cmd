@Echo off
Cls
Rem
Echo Ϊ����Ա��װ Corel WordPerfect Suite 8 �Ľű���
Echo.
Echo �� netadmin �ļ��е��� netsetup.exe���������簲װ��
Echo �����簲װ���һ�ε�¼ʱ�����ǽڵ㰲װ��
Echo **��Ҫ�����簲װ��ͽڵ㰲װ������ **
Echo **����ű���**  (�ǳ��ؼ�! ֻ������Աʹ��) 
Echo.
Echo �밴���������...
Pause > Nul:
cls
Echo.
Echo Ӧ���� Netsetup.exe ��װ Corel WordPerfect Suite 8��
Echo.
Echo ���û������ Netsetup.exe���������˳�(�� Ctrl-c)
Echo ��ִ�����в������ظ���װ������
Echo   �ӡ�������塱ж�� WordPerfect Suite 8
Echo   ����������塱�������������/ɾ������
Echo   �� Corel 8 CD-ROM �е� NetAdmin Ŀ¼��ѡ�� Netsetup.exe
Echo   ��� Corel WordPerfect Suite 8 �����簲װ 
Echo.
Echo ���򣬰����������...

Pause > NUL:

Echo.
Echo ��� Corel WordPerfect Suite �����ļ�û�а�װ��
Echo "D:\Corel" �У�����Ա��Ҫ�༭�ļ� Cofc8ins.cmd��
Echo ���ð�װ���ļ����ڵ�Ŀ¼�滻
Echo "D:\Corel"��
Echo.
Echo �밴���������ʼ���� Cofc8ins.cmd ...

Pause > NUL:

Notepad Cofc8ins.cmd

Echo.
Pause

Call Cofc8ins.cmd

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


..\acsr "#WPSINST#" "%WPSINST%" Template\Coffice8.key Coffice8.key
..\acsr "#WPSINST#" "%WPSINST%" ..\Logon\Template\Cofc8usr.cmd %temp%\Cofc8usr.tmp
..\acsr "#WPSINSTCMD#" "%WPSINST%\Suite8\Corel WordPerfect Suite 8 Setup.lnk" %temp%\Cofc8usr.tmp ..\Logon\Cofc8usr.cmd
Del %temp%\Cofc8usr.tmp >Nul: 2>&1

FindStr /I Cofc8Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip0
Echo Call Cofc8Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip0

Echo �ؼ�: ��ʼ�ڵ㰲װ����?
Echo �ؼ�: �����������簲װ����Ӧ��
Echo �ؼ�: ������ʼ�ڵ㰲װ�� 
Echo �ؼ�: �����в�������ʼ�ڵ㰲װ:
Echo �ؼ�:  1. �� Control-C ����������ű�
Echo �ؼ�:  2. ��ע�������Թ���Ա��ݵ�¼
Echo �ؼ�:  3. �ӡ����/ɾ�����򡱣����������(�� 
Echo �ؼ�:     �ڡ������û�/��װ��ģʽ��)���� 
Echo �ؼ�:     \corel\Suite 8\Corel WordPerfect Suite 8 ��װ�����ݷ�ʽ
Echo �ؼ�:  4. ��װ�ڼ䣬Ϊ��ѡ��Ŀ�ꡱѡ�� %RootDrive%
Echo �ؼ�:  5. �ڽڵ㰲װ���ٴ���������ű�

Rem Ӧ���ڹ���Ա��� Corel WordPerfect Suite 8 �ڵ�
Rem ��װ����������ű���
Rem.
Rem �������Ա��δ��ɽڵ㰲װ���밴 Ctrl-C��
Rem �˳��ű���ע�����Թ���Ա������ٴε�¼������
Rem Corel WordPerfect Suite �ڵ㰲װ������Ӧ�ó���װ
Rem ���û�����Ŀ¼: %RootDrive%��
Rem.
Echo �����밴���������...

Pause > NUL:


If Not Exist "%COMMON_START_MENU%\Corel WordPerfect Suite 8" Goto skip1
Move "%COMMON_START_MENU%\Corel WordPerfect Suite 8" "%USER_START_MENU%\" > NUL: 2>&1


:skip1


If Not Exist "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" Goto skip2
Move "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" "%USER_STARTUP%\" > NUL: 2>&1

:skip2

Rem ���Ŀǰ���ڰ�װģʽ�У���ĳɰ�װģʽ��

Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:

:Begin
regini COffice8.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem ɾ�� IDAPI �ļ����е��ļ���
Rem ����ű��е� CACLS ����ֻ�� NTFS ��ʽ�Ĵ��̷�������Ч��

Cacls "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" /E /T /G "Authenticated Users":C > Nul: 2>&1
Move "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" "%WPSINST%\Suite8\Shared\Idapi\" > NUL: 2>&1
Del /F /Q "%UserProfile%\Corel\Suite8\Shared\Idapi"

Echo Corel WordPerfect Suite 8 ���û�Ӧ�ó�������ѽ�����
Echo.
Echo ����Ա�������ɽڵ㰲װ��Ӧ�ļ������ư�װ
Echo ���á��й���ϸ��Ϣ������������ĵ��������
Echo Corel Web վ�㡣
Echo   http://www.corel.com/support/netadmin/admin8/Installing_to_a_client.htm
Echo.

Pause

:Done


