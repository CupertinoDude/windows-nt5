Rem Netscape �̃o�[�W�����ɉ����ēK�؂ȃR�}���h�����s���܂��B
if "#NS4VER#" == "4.5x" goto NS45

Rem
Rem Netscape 4.0x
Rem �܂��R�s�[����Ă��Ȃ��ꍇ�A����̃v���t�@�C�������[�U�[�̃z�[��
Rem �f�B���N�g���ɃR�s�[���܂��B
Rem

If Exist "%RootDrive%\NS40" Goto Done
If Not Exist "#PROFDIR#" Goto Done
Xcopy "#PROFDIR#" "%RootDrive%\NS40" /E /I /K >Nul: 2>&1
goto Done

:NS45
Rem
Rem Netscape 4.5x
Rem ���[�U�[�� Netscape �̃v���t�@�C�� �f�B���N�g���� ACL ��ݒ肵�āA���[�U�[������
Rem �A�N�Z�X�ł���悤�ɂ��܂��B
Rem

..\ACRegL "%Temp%\NS45_1.Cmd" NSHomeDir "HKCU\Software\Netscape\Netscape Navigator\biff" "CurrentUser" ""
If ErrorLevel 1 Goto Done
Call %Temp%\NS45_1.Cmd 
Del %Temp%\NS45_1.Cmd >Nul: 2>&1

If Not Exist "#NSUSERDIR#\%NSHomeDir%" Goto Done
If Exist "#NSUSERDIR#\%NSHomeDir%\com45usr.dat" Goto Done

Rem
Rem Netscape quick launch .lnk �t�@�C�����R�s�[���܂��B
Rem
If Exist "#NS40INST#\Netscape Composer.lnk" copy "#NS40INST#\Netscape Composer.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y
If Exist "#NS40INST#\Netscape Messenger.lnk" copy "#NS40INST#\Netscape Messenger.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y
If Exist "#NS40INST#\Netscape Navigator.lnk" copy "#NS40INST#\Netscape Navigator.lnk" "%UserProfile%\%App_Data%\Microsoft\Internet Explorer\Quick Launch" /y

cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /g %username%:F
cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /r "Terminal Server User"
cacls "#NSUSERDIR#\%NSHomeDir%" /e /t /r "Users"
echo done > "#NSUSERDIR#\%NSHomeDir%\com45usr.dat"
:Done
