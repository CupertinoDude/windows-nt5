
@Echo Off

Rem
Rem  ע��:  ����ű��е� CACLS ����ֻ�� NTFS 
Rem         ��ʽ���Ĵ��̷�������Ч��
Rem

Rem #########################################################################

Rem
Rem  ���û������ļ���ɾ�� Corel Office 7 Ŀ¼��
Rem  ���ȣ�ǿ���û��ص�ִ��ģʽ���Ա�֤�ļ���
Rem  us ���Ը��Ƶ��������û������ļ�����
Rem

Rem  ���Ŀǰ����ִ��ģʽ�У���ĳɰ�װģʽ��

ChgUsr /query > Nul:
if ErrorLevel 101 Goto Begin1
Set __OrigMode=Install
Change User /Execute > Nul:
:Begin1


Rem ���ԭʼģʽ�ǰ�װ����Ļذ�װģʽ��
If "%__OrigMode%" == "Install" Change User /Install > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem ����֤ %RootDrive% �Ѿ������ã���Ϊ����ű����øñ�����
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rmdir "%USER_START_MENU%\Corel Office 7" /Q >Nul: 2>&1


Rem
Rem  ָ���û��޸� key �ļ���
Rem


if "%RootDrive%"=="w:" goto PostDriveChange
if "%RootDrive%"=="W:" goto PostDriveChange

Echo   ������ coffice7.key �ļ���������Ŀ¼��
Echo.
Echo   ��ִ�����в���:
Echo     1) �����±���в�����Ŀ¼�� ASCII ֵ��
Echo        ������Ŀ¼�� %RootDrive%
Echo.
Echo        A = 61   E = 65   I = 69   M = 6D   Q = 71   U = 75   Y = 79
Echo        B = 62   F = 66   J = 6A   N = 6E   R = 72   V = 76   Z = 7A
Echo        C = 63   G = 67   K = 6B   O = 6F   S = 73   W = 77   
Echo        D = 64   H = 68   L = 6C   P = 70   T = 74   X = 78
Echo.
Echo     2) ���á����±��������ڲ��� 1 ���ҵ���ֵ 
Echo        �滻���е� 77��
Echo        ע��: ��ȷ�����滻������û����Ӷ���Ŀո�
Echo     3) �����ļ����˳�������ű���������
Echo.
Pause

NotePad "%SystemRoot%\Application Compatibility Scripts\Install\coffice7.key"

:PostDriveChange


Rem ���Ŀǰ���ڰ�װģʽ�У���ĳɰ�װģʽ��
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin2
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin2

regini COffice7.key > Nul:

Rem ���ԭʼģʽ��ִ�У���Ļ�ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem �޸ĵ�¼�ű����Ա�Ӱ�װλ�ø��� iBase ���ݿ⡣
Rem




..\ACRegL %Temp%\COffice7.Cmd COffice7Loc "HKLM\Software\PerfectOffice\Products\InfoCentral\7" "ExeLocation" "StripChar\2"

If ErrorLevel 1 Goto InstallError

Call %Temp%\COffice7.Cmd 
Del %Temp%\COffice7.Cmd >Nul: 2>&1


..\ACIniUpd /e "%COffice7Loc%\ICWin7\Local\Wpic.ini" Preferences Last_IBase "%RootDrive%\Personal\iBases\Personal\Personal"
If ErrorLevel 1 Goto InstallError


..\acsr "#COFFICE7INST#" "%COffice7Loc%\\" ..\Logon\Template\cofc7usr.Cmd ..\Logon\cofc7usr.Cmd
If ErrorLevel 1 Goto InstallError

goto PostInstallError
:InstallError

Echo.
Echo �޷���ע������ Corel Office 7 ��װλ�á�
Echo ����֤ Corel Office 7 �Ƿ��Ѿ���װ���ٴ�����
Echo ����ű���
Echo.
Pause
Goto Done

:PostInstallError

Rem #########################################################################

Rem 
Rem  �� WordPerfect ģ���Ϊֻ����
Rem  �⽫ǿ���û��ڽ��и���֮ǰ�ȸ��ơ�
Rem  ��һ���취�ǣ���ÿ���û�һ��
Rem  ר��ģ��Ŀ¼��
Rem

attrib +r %COffice7Loc%\Template\*wpt /s >Nul: 2>&1

Rem #########################################################################

Rem
Rem �� COfc7Usr.Cmd ��ӵ� UsrLogn2.Cmd �ű�
Rem

FindStr /I COfc7Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call COfc7Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################


Rem
Rem  ��� "Personal" Ŀ¼�������ڣ����䴴�� ����� 
Rem  ������������ִ�У�ԭ���ǹ���Ա��Ҫ�ֶ�ִ������
Rem  ��ʾ��һ�����衣
Rem  

If Not Exist "%RootDrive%\Personal" Md "%RootDrive%\Personal"

Rem #########################################################################

cls
Echo.
Echo   �������������Щ�����ֶ����� Quattro Pro
Echo   Ĭ��Ŀ¼:
Echo     1) �������д����� 'change user /install'��
Echo     2) ���� Quattro Pro��
Echo     3) ѡ�񡰱༭��ѡ��˵���Ŀ��
Echo     4) �����ļ�ѡ�ѡ���
Echo     5) ��Ŀ¼�ĳ� %RootDrive%\Personal��
Echo     6) �˳�����
Echo     7) �������д����� 'change user /execute'��
Echo.
pause

Echo.
Echo   Ҫ��֤ Corel Office 7 ���������У��������κ�
Echo   Ӧ�ó���֮ǰ��Ŀǰ��¼���û�������ע����
Echo   �����µ�¼��
Echo.
Echo Corel Office 7 ���û�Ӧ�ó�������ѽ���
Pause

:Done
