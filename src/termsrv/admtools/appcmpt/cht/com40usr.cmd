Rem �ھ� Netscape ��������A����O�C
if "#NS4VER#" == "4.5x" goto NS45

Rem
Rem Netscape 4.0x
Rem �p�G�w�]���]�w�ɩ|���ƻs��ϥΪ̥D�ؿ��A
Rem �N�N�w�]���]�w�ɽƻs��ϥΪ̥D�ؿ��C
Rem

If Exist "%RootDrive%\NS40" Goto Done
If Not Exist "#PROFDIR#" Goto Done
Xcopy "#PROFDIR#" "%RootDrive%\NS40" /E /I /K >Nul: 2>&1
goto Done

:NS45
Rem
Rem Netscape 4.5x
Rem �b�ϥΪ̪� Netscape �]�w�ɥؿ����]�w ACL�A
Rem �o�˥u���ϥΪ̤~��s�����C
Rem

..\ACRegL "%Temp%\NS45_1.Cmd" NSHomeDir "HKCU\Software\Netscape\Netscape Navigator\biff" "CurrentUser" ""
If ErrorLevel 1 Goto Done
Call %Temp%\NS45_1.Cmd 
Del %Temp%\NS45_1.Cmd >Nul: 2>&1

If Not Exist "#NSUSERDIR#\%NSHomeDir%" Goto Done
If Exist "#NSUSERDIR#\%NSHomeDir%\com45usr.dat" Goto Done

Rem
Rem �ƻs Netscape �ֳt�Ұ� .lnk �ɮ�
Rem
If Exist "#NS40INST#\Netscape Composer.lnk" copy "#NS40INST#\Netscape Composer.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y
If Exist "#NS40INST#\Netscape Messenger.lnk" copy "#NS40INST#\Netscape Messenger.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y
If Exist "#NS40INST#\Netscape Navigator.lnk" copy "#NS40INST#\Netscape Navigator.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y

cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /g %username%:F
cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /r "Terminal Server User"
cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /r "Users"
echo done > "#NSUSERDIR#\%NSHomeDir%\com45usr.dat"
:Done
