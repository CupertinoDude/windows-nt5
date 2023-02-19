
@Echo Off

Rem
Rem  �`�N: �b�o�ӫ��O�ɤ��� CACLS �R�O�u��b
Rem         NTFS �榡���ϺФ��Τ�����C
Rem

Rem #########################################################################

Rem
Rem  �q�ϥΪ̳]�w�ɤ��R�� Corel Office 7 �ؿ��C
Rem  �����A�j���ϥΪ̦^�����Ҧ��H�O�Ҹ�Ƨ�
Rem  ����ƻs�� [All Users] �����]�w�ɡC
Rem

Rem �p�G�ثe���O�b����Ҧ��A���ܧ��w�˼Ҧ��C

ChgUsr /query > Nul:
if ErrorLevel 101 Goto Begin1
Set __OrigMode=Install
Change User /Execute > Nul:
:Begin1


Rem �p�G�ϥέ�l�Ҧ��A���ܧ��w�˼Ҧ��C
If "%__OrigMode%" == "Install" Change User /Install > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rmdir "%USER_START_MENU%\Corel Office 7" /Q >Nul: 2>&1


Rem
Rem  Instruct user to modify key file.
Rem


if "%RootDrive%"=="w:" goto PostDriveChange
if "%RootDrive%"=="W:" goto PostDriveChange

Echo   �D�ؿ������]�w�b coffice7.key �ɮפ��C
Echo.
Echo   �Ш̷ӳo�ǨB�J:
Echo     1) �b�U�C��椤�j�M�D�ؿ��� ASCII �ȡC
Echo        �z���D�ؿ��� %RootDrive%
Echo.
Echo        A = 61   E = 65   I = 69   M = 6D   Q = 71   U = 75   Y = 79
Echo        B = 62   F = 66   J = 6A   N = 6E   R = 72   V = 76   Z = 7A
Echo        C = 63   G = 67   K = 6B   O = 6F   S = 73   W = 77   
Echo        D = 64   H = 68   L = 6C   P = 70   T = 74   X = 78
Echo.
Echo     2) ��O�ƥ��Ұʫ�A�H�B�J 1 ����쪺�ȨӨ��N 
Echo        �Ҧ��� 77�C
Echo        �`�N�ƶ�: �нT�w�b���N�ާ@���A�å��s�W�h�l���Ů�C
Echo     3) �x�s�ɮרõ����{���C�o�ӫ��O�ɱN�|�~�����C
Echo.
Pause

NotePad "%SystemRoot%\Application Compatibility Scripts\Install\coffice7.key"

:PostDriveChange


Rem If not currently in Install Mode, change to Install Mode.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin2
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin2

regini COffice7.key > Nul:

Rem �p�G�z�����l�Ҧ��A���ܧ�����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Rem #########################################################################

Rem
Rem Modify logon script to copy iBase database from install location.
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
Echo �L�k�q�n����� Corel Office 7 ���w�˦�m�C
Echo �нT�{�O�_�w�w�� Corel Office 7�A�í��s����o�� 
Echo ���O�ɡC
Echo.
Pause
Goto Done

:PostInstallError

Rem #########################################################################

Rem 
Rem  �N WordPerfect �d���ܧ󦨰�Ū�ɡC
Rem  �o�˷|�j���ϥΪ̦b�ܧ󤧫e���إ߽ƥ��C
Rem  �t�@�Ӥ覡�O���ѨC�ӨϥΪ̤@��
Rem  �p�H�����C
Rem

attrib +r %COffice7Loc%\Template\*wpt /s >Nul: 2>&1

Rem #########################################################################

Rem
Rem �N COfc7Usr.Cmd �[�J UsrLogn2.Cmd ���O��
Rem

FindStr /I COfc7Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call COfc7Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################


Rem
Rem  �إ� "Personal" �ؿ� (�p�G�����s�b)�C
Rem  �����إ߳o�ӥؿ��A�]�� Admin �ݭn��ʰ���H�U
Rem  ����ܪ��B�J�C
Rem  

If Not Exist "%RootDrive%\Personal" Md "%RootDrive%\Personal"

Rem #########################################################################

cls
Echo.
Echo   Quattro Pro �w�]�ؿ������̷ӳo�ǨB�J�H���
Echo   �覡�i���ܧ�:
Echo     1) �b���O���J 'change user /install'�C
Echo     2) �Ұ� Quattro Pro�C
Echo     3) ��� [�s��ߦn�]�w] �\����ءC
Echo     4) ���� [�ɮ׿ﶵ] ���޼��ҡC
Echo     5) �N�ؿ��ܧ� %RootDrive%\Personal�C
Echo     6) �������{���C
Echo     7) �b���O���J 'change user /execute'�C
Echo.
pause

Echo.
Echo   ���F���� Corel Office 7 ���T�ާ@�A
Echo   �ثe�w�n�J���ϥΪ̥������n�X�A�M�᭫�s�n�J�A
Echo   �~��������ε{���C
Echo.
Echo Corel Office 7 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
