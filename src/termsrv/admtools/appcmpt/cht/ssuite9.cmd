
@Echo Off


Rem #########################################################################

Rem
Rem �q�n�����o Lotus Wordpro 9 �w�˦�m�C
Rem

..\ACRegL "%Temp%\wordpro.Cmd" WP "HKLM\Software\Lotus\Wordpro\98.0" "Path" ""

Call "%Temp%\wordpro.Cmd"
Del "%Temp%\wordpro.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN��]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

If Exist ..\Logon\SS9Usr.Cmd Goto Skip1

Echo.
Echo     �䤣�� SS9Usr.Cmd �n�J���O��
Echo        %Systemroot%\Application Compatibility Scripts\Logon.
Echo.
Echo     Lotus SmartSuite 9 �h�ϥΪ����ε{���վ�B�z�w�פ�C
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

Rem �վ�w�]�n���ɦs���v���A���ϥΪ̯������ Smart Suite�C
regini ssuite9.key > Nul:

Rem �p�G Lotus Wordpro �å��w�ˡA�N���L�o�ӨB�J
If "%WP%A" == "A" Goto Skip3  
Rem �]�w�b�U�C�ɮפ��w�q���n�����X�A�]�����ɭ� wordpro ���|�]�w�o�Ǿ��X�C
set List="%WP%\lwp.reg" "%WP%\lwpdcaft.reg" "%WP%\lwplabel.reg" "%WP%\lwptls.reg"

regedit /s %List% 

:Skip3

Rem �إߨϥΪ̵n���ɨӧ�s�C�ӨϥΪ̪��n�����X�C
Echo Windows Registry Editor Version 5.00 >..\Logon\ss9usr.reg
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

Echo Lotus SmartSuite 9 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
