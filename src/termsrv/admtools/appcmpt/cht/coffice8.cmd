@Echo off
Cls
Rem
Echo �� Administrator �w�� Corel WordPerfect Suite 8 �����O�ɡC
Echo.
Echo Net Setup �O�q netadmin ��Ƨ��Ұ� netsetup.exe �ɩҨϥΪ����O�C
Echo Node Setup �O�b�����w�˫�A�z�Ĥ@���n�J�ɩҨϥΪ����O�C
Echo **�����w�˫�A�z��������o�ӫ��O�ɡA�b�`�I�w�˫�A�z **
Echo **�����A������o�ӫ��O�ɡC**  (�D�`���n! �u�A�� Admin) 
Echo.
Echo �Ы����N���~��...
Pause > Nul:
cls
Echo.
Echo Corel WordPerfect Suite 8 �����ϥ� Netsetup.exe �Ӧw�ˡC
Echo.
Echo �p�G�z�S������ Netsetup.exe�A�в{�b�����{�� (�� Ctrl-c)
Echo �ð���U�C�B�J�A�ӭ��Ʀw�ˡC
Echo   �q [����x] �Ѱ� WordPerfect Suite 8 ���w��
Echo   �Ш� [����x]�A�� [�s�W/����] �{��
Echo   ��� Corel 8 CD-ROM �� NetAdmin �ؿ��U�� Netsetup.exe 
Echo   ���� Corel WordPerfect Suite 8 �� Net Setup 
Echo.
Echo �_�h�A�Ы����N���~��...

Pause > NUL:

Echo.
Echo �p�G Corel WordPerfect Suite �����ɮץ��Q�w�˨�
Echo "D:\Corel"�AAdministrator �����s�� Cofc8ins.cmd �ɮסA
Echo �åH�ɮצw�˩Ҧb���ؿ� 
Echo �Ө��N "D:\Corel"�C
Echo.
Echo �Ы����N��}�l��s Cofc8ins.cmd ...

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

Echo ���n: �z�w�Ұ� Node �w�˵{���F��?
Echo ���n: �p�G�z�觹�� Net �w�˵{���A�z�{�b����
Echo ���n: �Ұ� Node �w�˵{���C 
Echo ���n: �Ш̷ӳo�ǨB�J�ӱҰ� Node �w�˵{��:
Echo ���n:  1. �� Control-C �ӵ����o�ӫ��O��
Echo ���n:  2. �n�X�õn�J�� Admin
Echo ���n:  3. �ϥ� [�s�W\����] �{�������s���\�� (�� 
Echo ���n:     �b�ܧ�ϥΪ� /�w�˼Ҧ�) �Ұ� 
Echo ���n:     \corel\Suite 8\Corel WordPerfect Suite 8 �w�˵{�����|
Echo ���n:  4. �b�w�˹L�{���A�п�� %RootDrive% �� Choose Destination
Echo ���n:  5. �b Node Setup ������A�Э��s����o�ӫ��O��

Rem �b�t�κ޲z������ Corel WordPerfect Suite 8 Node
Rem �w�˵{�������Ӱ���o�ӫ��O�ɡC
Rem.
Rem �b�t�κ޲z���|������ Node �w�˵{���A�Ы� Ctrl-C
Rem �ӵ������O�ɨõn�X�A�M��H Administrator �����n�J�A����
Rem Corel WordPerfect Suite Node �w�˵{���A�ñN���ε{��
Rem �w�˨�ϥΪ̪��u�D�ؿ�: %RootDrive%�C
Rem.
Echo �_�h�A�Ы����N���~��...

Pause > NUL:


If Not Exist "%COMMON_START_MENU%\Corel WordPerfect Suite 8" Goto skip1
Move "%COMMON_START_MENU%\Corel WordPerfect Suite 8" "%USER_START_MENU%\" > NUL: 2>&1


:skip1


If Not Exist "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" Goto skip2
Move "%COMMON_STARTUP%\Corel Desktop Application Director 8.LNK" "%USER_STARTUP%\" > NUL: 2>&1

:skip2

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C

Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:

:Begin
regini COffice8.key > Nul:

Rem �p�G��ӬO����Ҧ��A�N�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem �b IDAPI ��Ƨ����R���ɮסC
Rem CACLS ���O�u��b NTFS �榡���ϺФ��Τ�����C

Cacls "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" /E /T /G "Authenticated Users":C > Nul: 2>&1
Move "%UserProfile%\Corel\Suite8\Shared\Idapi\idapi.cfg" "%WPSINST%\Suite8\Shared\Idapi\" > NUL: 2>&1
Del /F /Q "%UserProfile%\Corel\Suite8\Shared\Idapi"

Echo Corel WordPerfect Suite 8 �h�ϥΪ����ε{���վ�B�z�w�����C
Echo.
Echo Administrator �i�H���� Node Setup Response �ɮרӱ���w�˵{����
Echo �]�w�C�����ԲӸ�T�A��Ū���u�W���άd��
Echo Corel �����C
Echo   http://www.corel.com/support/netadmin/admin8/Installing_to_a_client.htm
Echo.

Pause

:Done


