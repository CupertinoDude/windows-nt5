

@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN��]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

If Exist ..\Logon\SS97Usr.Cmd Goto Skip1

Echo.
Echo     �䤣�� SS97Usr.Cmd �n�J���O��
Echo        %Systemroot%\Application Compatibility Scripts\Logon.
Echo.
Echo     Lotus SmartSuite 97 �h�ϥΪ����ε{���վ�B�z�w�פ�C
Echo.
Pause
Goto Done

:Skip1

Rem #########################################################################
FindStr /I SS97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call SS97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2

Rem #########################################################################

Rem �վ�w�]�n���ɦs���v���A���ϥΪ̯������ Smart Suite�C
regini ssuite97.key > Nul:

Rem �إߨϥΪ̵n���ɨӧ�s�C�ӨϥΪ̪��n�����X�C
Echo Windows Registry Editor Version 5.00 >..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\123\97.0\Paths\Work]>>..\Logon\SS97Usr.reg
Echo "EN"="%RootDrive%\\Lotus\\Work\\123\\" >>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Approach\97.0\Paths\Work]>>..\Logon\SS97Usr.reg
Echo "EN"="%RootDrive%\\Lotus\\work\\approach\\">>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Freelance\97.0\Freelance Graphics]>>..\Logon\SS97Usr.reg
Echo "Working Directory"="%RootDrive%\\Lotus\\work\\flg\\">>..\Logon\SS97Usr.reg
Echo "Backup Directory"="%RootDrive%\\Lotus\\backup\\flg\\">>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Organizer\97.0\Paths]>>..\Logon\SS97Usr.reg
Echo "OrganizerFiles"="%RootDrive%\\Lotus\\work\\organize">>..\Logon\SS97Usr.reg
Echo "Backup"="%RootDrive%\\Lotus\\backup\\organize">>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\WordPro\97.0\Paths\Backup]>>..\Logon\SS97Usr.reg
Echo "EN"="%RootDrive%\\Lotus\\backup\\wordpro\\">>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\WordPro\97.0\Paths\Work]>>..\Logon\SS97Usr.reg
Echo "EN"="%RootDrive%\\Lotus\\work\\wordpro\\">>..\Logon\SS97Usr.reg
Echo. >>..\Logon\SS97Usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\Components\Spell\4.0]>> ..\Logon\ss97usr.reg
Echo "Multi User Path"="%RootDrive%\\Lotus" >> ..\Logon\ss97usr.reg
Echo. >> ..\Logon\ss97usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\SmartCenter\97.0]>>..\Logon\ss97usr.reg
Echo "Configure"=dword:00000001>>..\Logon\ss97usr.reg
Echo [HKEY_CURRENT_USER\Software\Lotus\SmartCenter\97.0\Paths\Work]>>..\Logon\ss97usr.reg
Echo "EN"="%RootDrive%\\Lotus\\Work\\SmartCtr">>..\Logon\ss97usr.reg
Echo. >>..\Logon\ss97usr.reg

Rem #########################################################################

Echo Lotus SmartSuite 97 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
