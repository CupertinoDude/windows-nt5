@Echo Off

Rem #########################################################################

Rem
Rem �ˬd CMD Extensions �O�_�w�ҥΡC
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c netcom40.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN��]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem ���o NetScape ���� (4.5x �M 4.0x ����覡���P)
Rem

..\ACRegL "%Temp%\NS4VER.Cmd" NS4VER "HKLM\Software\Netscape\Netscape Navigator" "CurrentVersion" "STRIPCHAR(1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� Netscape Communicator 4 �����C 
Echo �нT�{ Communicator �O�_�w�w�ˡA�M�᭫�s���� 
Echo �o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\NS4VER.Cmd"
Del "%Temp%\NS4VER.Cmd" >Nul: 2>&1

if /i "%NS4VER%" LSS "4.5 " goto NS40x

Rem #########################################################################
Rem Netscape 4.5x

Rem
Rem �q�n�������o Netscape Communicator 4.5 �w�˦�m�C
Rem �p�G�䤣��A�N���] Communicator 4.5 �å��w�˨���ܿ��~�T���C
Rem

..\ACRegL "%Temp%\NS45.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" "Stripchar\1"
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �L�k�q�n������� Netscape Communicator 4.5 ���w�˦� 
Echo �m�C�нT�{ Communicator �O�_�w�w�ˡA�M�᭫�s����o
Echo �ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont1
Call "%Temp%\NS45.Cmd"
Del "%Temp%\NS45.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem Update Com40Usr.Cmd to reflect the default NetScape Users directory and
Rem add it to the UsrLogn2.Cmd script
Rem

..\acsr "#NSUSERDIR#" "%ProgramFiles%\Netscape\Users" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS40INST#" "%NS40INST%" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.tm2
..\acsr "#NS4VER#" "4.5x" ..\Logon\Com40Usr.tm2 ..\Logon\Com40Usr.Cmd

Rem #########################################################################

Rem
Rem �N [�ֳt�Ұ�] �ϥܽƻs�� netscape �w�˥ؿ��A
Rem ���{������N���̽ƻs��ϥΪ̳]�w�ɥؿ��C
Rem

If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Composer.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Messenger.lnk" "%NS40INST%"
If Exist "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" copy "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch\Netscape Navigator.lnk" "%NS40INST%"

goto DoUsrLogn

:NS40x
Rem #########################################################################
Rem Netscape 4.0x

Rem
Rem �q�n�������o Netscape Communicator 4 �w�˦�m�C
Rem �p�G�䤣��A�N���]  Communicator 4.5 �å��w�˨���ܿ��~�T���C
Rem

..\ACRegL "%Temp%\NS40.Cmd" NS40INST "HKCU\Software\Netscape\Netscape Navigator\Main" "Install Directory" ""
If Not ErrorLevel 1 Goto Cont2
Echo.
Echo �L�k�q�n������� Netscape Communicator 4 �w�˦�m�C
Echo ���ˬd Communicator �O�_�w�g�w�ˡA�í��s����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont2
Call "%Temp%\NS40.Cmd"
Del "%Temp%\NS40.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �N�w�]���]�w�ɽƻs��t�κ޲z�����D�ؿ��C
Rem �o�ӳ]�w�ɷ|�b�ϥΪ̵n�J�ɳQ�ƻs��ϥΪ̥D�ؿ����C
Rem �p�G�q�Ϊ��w�]�]�w�ɤw�g�s�b�A�Ф��n�мg���C
Rem �_�h Admin �i�H�b�y�԰���o�ӫ��O�ɡA�ñN�Ҧ�
Rem �L���ӤH��T����q�ιw�]�]�w�ɡC
Rem

If Exist %RootDrive%\NS40 Goto Cont3
Echo.
Echo �b %RootDrive%\NS40 ���䤣��w�]�]�w�ɡC�а���
Echo [�ϥΪ̳]�w�ɺ޲z��]�A�ëإߤ@�ӦW�٬� "Default"
Echo ����@�]�w�ɡC���ܿ�J�]�w�ɸ��|�ɡA�ШϥΤW��
Echo ����ܪ����|�C�бN�Ҧ����W�٤ιq�l�l��W�ٶ��ثO�d
Echo �ťաC�p�G����L�]�w�ɦs�b�A�ЧR�����̡C�b�z�����o
Echo �ǨB�J��A�Э��s����o�ӫ��O�ɡC
Echo.
Pause
Goto Done
 
:Cont3
If Exist "%NS40INST%\DfltProf" Goto Cont4
Xcopy "%RootDrive%\NS40" "%NS40INST%\DfltProf" /E /I /K >NUL: 2>&1
:Cont4

Rem #########################################################################

Rem 
Rem �q [�}�l] �\����|�� [�ϥΪ̳]�w�ɺ޲z��] �������ϥΪ̪�
Rem Ū���v���A�o�|����@��ϥΪ̷s�W�ϥΪ̳]�w�ɡC
Rem �t�κ޲z�����M�i�H����ϥΪ̳]�w�ɺ޲z���C
Rem

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" Goto Cont5
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont5

If Not Exist "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" Goto Cont6
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Authenticated Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Users" >Nul: 2>&1
Cacls "%COMMON_PROGRAMS%\Netscape Communicator Professional Edition\Utilities\User Profile Manager.Lnk" /E /R "Everyone" >Nul: 2>&1

:Cont6

Rem #########################################################################

Rem
Rem ��s Com40Usr.Cmd �ӤϬM��ڪ��w�˥ؿ���
Rem �N��s�W�� UsrLogn2.Cmd ���O�ɤ��C
Rem

..\acsr "#PROFDIR#" "%NS40INST%\DfltProf" ..\Logon\Template\Com40Usr.Cmd ..\Logon\Com40Usr.tmp
..\acsr "#NS4VER#" "4.0x" ..\Logon\Com40Usr.tmp ..\Logon\Com40Usr.Cmd

:DoUsrLogn

del ..\Logon\Com40Usr.tmp >Nul: 2>&1
del ..\Logon\Com40Usr.tm2 >Nul: 2>&1

FindStr /I Com40Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Com40Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   ���F���� Netscape Communicator ���T�ާ@�A�ثe
Echo   �w�n�J���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   ���ε{���C
Echo.
Echo Netscape Communicator 4 �h�ϥΪ����ε{���վ�B�z����
Pause

:done

