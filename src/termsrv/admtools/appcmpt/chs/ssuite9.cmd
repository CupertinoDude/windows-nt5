@Echo Off



Rem #########################################################################

Rem
Rem ��ע������л�� Lotus Wordpro 9 �İ�װλ��
Rem

..\ACRegL "%Temp%\wordpro.Cmd" WP "HKLM\Software\Lotus\Wordpro\98.0" "Path" ""

Call "%Temp%\wordpro.Cmd"
Del "%Temp%\wordpro.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ����֤ %RootDrive% �Ѿ������ã���Ϊ����ű����øñ�����
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

If Exist ..\Logon\SS9Usr.Cmd Goto Skip1

Echo.
Echo     �Ҳ��� SS9Usr.Cmd ��¼�ű�
Echo        %Systemroot%\Application Compatibility Scripts\Logon.
Echo.
Echo     Lotus SmartSuite 9 ���û�Ӧ�ó����������ֹ��
Echo.
Pause
Goto Done

:Skip1

Rem #########################################################################
FindStr /I SS9Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call SS9Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2


Rem #########################################################################


Rem ����Ĭ��ע���Ȩ�ޣ��Ա������û����� Smart Suite
regini ssuite9.key > Nul:

Rem ���û�а�װ Lotus Wordpro��������һ����
If "%WP%A" == "A" Goto Skip3  
Rem ����ĳ��ԭ�� wordpro û�����ö����������ļ���ע�����������������ǡ�
set List="%WP%\lwp.reg" "%WP%\lwpdcaft.reg" "%WP%\lwplabel.reg" "%WP%\lwptls.reg"

regedit /s %List% 

:Skip3

Rem �����û�ע����ļ��Ա����ÿ�û�ע�����
Echo Windows ע���༭���汾 5.00 >..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\123\98.0\Paths\Work]>>..\Logon\ss9usr.reg
Echo "EN"="%RootDrive%\\Lotus\\Work\\123\\" >>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\123\98.0\Paths\AutoOpen]>>..\Logon\ss9usr.reg
Echo @="%RootDrive%\\Lotus\\Work\\123\\Auto\\" >>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Approach\97.0\Paths\Work]>>..\Logon\ss9usr.reg
Echo "EN"="%RootDrive%\\Lotus\\work\\approach\\">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Freelance\98.0\Freelance Graphics]>>..\Logon\ss9usr.reg
Echo "Working Directory"="%RootDrive%\\Lotus\\work\\flg\\">>..\Logon\ss9usr.reg
Echo "Backup Directory"="%RootDrive%\\Lotus\\backup\\flg\\">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Organizer\98.0\Paths]>>..\Logon\ss9usr.reg
Echo "OrganizerFiles"="%RootDrive%\\Lotus\\work\\organize">>..\Logon\ss9usr.reg
Echo "Backup"="%RootDrive%\\Lotus\\backup\\organize">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\WordPro\98.0\Paths\Backup]>>..\Logon\ss9usr.reg
Echo "EN"="%RootDrive%\\Lotus\\backup\\wordpro\\">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\WordPro\98.0\Paths\Work]>>..\Logon\ss9usr.reg
Echo "EN"="%RootDrive%\\Lotus\\work\\wordpro\\">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Components\Spell\4.1]>> ..\Logon\ss9usr.reg
Echo "Multi User Path"="%RootDrive%\\Lotus" >> ..\Logon\ss9usr.reg
Echo. >> ..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\SmartCenter\98.0]>>..\Logon\ss9usr.reg
Echo "Configure"=dword:00000001>>..\Logon\ss9usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\SmartCenter\98.0\Paths\Work]>>..\Logon\ss9usr.reg
Echo "EN"="%RootDrive%\\Lotus\\Work\\SmartCtr">>..\Logon\ss9usr.reg
Echo. >>..\Logon\ss9usr.reg

Rem #########################################################################


Echo Lotus SmartSuite 9 ���û�Ӧ�ó�������ѽ���
Pause

:Done
