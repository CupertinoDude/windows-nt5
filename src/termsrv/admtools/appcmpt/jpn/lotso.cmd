@Echo Off

Rem #########################################################################
Rem
Rem %RootDrive% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################
Rem
Rem SSuite97.Cmd �����łɎ��s����Ă���ꍇ�͏����𒆒f���܂��B
Rem
 
If Not Exist "%COMMON_STARTUP%\SS97Usr.Cmd" Goto Cont0

Echo.
Echo   ���ł� SSuite97.Cmd �����s����Ă��܂��B
Echo.   
Echo ���[�^�X �X�[�p�[�I�t�B�X �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪
Echo ���f����܂����B
Echo.
Pause
Goto Done

:Cont0

Rem #########################################################################
Rem
Rem ���[�^�X �X�[�p�[�I�t�B�X �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\lotso.Cmd" LOTSOINS "HKCU\Software\Lotus\Components" "User Path" STRIPCHAR\2
If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���W�X�g������ ���[�^�X �X�[�p�[�I�t�B�X �̃C���X�g�[������Ă���
Echo �p�X���擾�ł��܂���ł����B
Echo ���[�^�X �X�[�p�[�I�t�B�X ���C���X�g�[������Ă��邱�Ƃ��m�F����
Echo ���������B
Echo.
Pause
Goto Done

:Cont1
Call "%Temp%\lotso.Cmd"
Del "%Temp%\lotso.Cmd" > Nul: 2>&1


Rem #########################################################################
Rem
Rem LotsoUsr.Cmd ��������Ȃ��ꍇ�͏����𒆒f���܂��B  
Rem

If Exist ..\Logon\LotsoUsr.Cmd Goto Cont2

Echo.
Echo     LotsoUsr.Cmd ���ȉ��̃t�H���_�Ɍ�����܂���B
Echo        %Systemroot%\Application Compatibility Scripts\Logon.
Echo.
Echo ���[�^�X �X�[�p�[�I�t�B�X �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪
Echo ���f����܂����B
Echo.
Pause
Goto Done

:Cont2

Rem #########################################################################
Rem
Rem ���[�^�X ���[�h�v�� �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\wordpro.Cmd" WP "HKLM\Software\Lotus\Wordpro\98.0" "Path" ""
If ErrorLevel 1 Goto Cont3
Call "%Temp%\wordpro.Cmd"
Del "%Temp%\wordpro.Cmd" >Nul: 2>&1

:Cont3

Rem #########################################################################
Rem
Rem LotsoUsr.cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I LotsoUsr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Cont4
Echo Call LotsoUsr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Cont4

Rem #########################################################################
Rem
Rem �X�[�p�[�I�t�B�X �����s�ł���悤�� ���W�X�g���̃A�N�Z�X����ύX���܂��B
Rem

regini lotso.key > Nul:

Rem #########################################################################
Rem
Rem ���[�^�X ���[�h�v�� ���C���X�g�[������Ă��Ȃ��ꍇ�͎��̏������X�L�b�v���܂��B
Rem

If "%WP%A" == "A" Goto Cont5
  
Rem #########################################################################
Rem
Rem ���̃t�@�C���Œ�`����Ă��� ���W�X�g�� �L�[��o�^���܂��B���[�h�v�����o�^��
Rem �X�L�b�v���邱�Ƃ�����܂��B

set List="%WP%\lwp.reg" "%WP%\lwplabel.reg" "%WP%\lwptls.reg"

regedit /s %List% 

:Cont5

Rem #########################################################################
Rem
Rem ���[�U�[�̃��W�X�g�� �L�[���X�V���邽�߂̃��W�X�g�� �t�@�C�����쐬���܂��B
Rem

Rem #########################################################################
Rem
Rem ���[�^�X �X�[�p�[�I�t�B�X�̃��W�X�g�����t�@�C���ɏ����o���܂��B
Rem

Regedit /a lotso.tmp HKEY_CURRENT_USER\Software\Lotus

Rem #########################################################################
Rem
Rem �����o�����t�@�C������A�p�X��ύX����K�v�̂��镔���𒊏o���܂��B
Rem

Echo Windows Registry Editor Version 5.00 >lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\123\\.*\\Paths\\Work\] lotso.tmp >>lotso.tmp2
findstr /i \"JA\".*\\\\Work\\\\123\\\\ lotso.tmp >>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\123\\.*\\Paths\\AutoOpen\] lotso.tmp >>lotso.tmp2
Echo @="%RootDrive%\\Lotus\\Work\\123\\Auto\\" >>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\Approach\\.*\\Paths\\Work\] lotso.tmp >>lotso.tmp2
findstr /i \"JA\".*\\\\work\\\\approach\\\\ lotso.tmp >>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i /r /c:"\[HKEY_CURRENT_USER\\Software\\Lotus\\Freelance\\.*\\Freelance Graphics\]" lotso.tmp >>lotso.tmp2
findstr /i /r /c:"\"Working Directory\".*\\\\work\\\\flg\\\\" lotso.tmp >>lotso.tmp2
findstr /i /r /c:"\"Backup Directory\".*\\\\backup\\\\flg\\\\" lotso.tmp >>lotso.tmp2
Echo "User Dictionary"="%RootDrive%\\Lotus\\compnent\\spell\\ltsuser1.udc">>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\Organizer\\.*\\Paths\] lotso.tmp >>lotso.tmp2
findstr /i \"OrganizerFiles\".*\\\\work\\\\organize lotso.tmp >>lotso.tmp2
findstr /i \"Backup\".*\\\\backup\\\\organize lotso.tmp >>lotso.tmp2
Echo.>>lotso.tmp2 
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\WordPro\\.*\\Paths\\Backup\] lotso.tmp >>lotso.tmp2
findstr /i \"JA\".*\\\\backup\\\\wordpro\\\\ lotso.tmp >>lotso.tmp2
Echo. >>lotso.tmp2 
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\WordPro\\.*\\Paths\\Work\] lotso.tmp >>lotso.tmp2
findstr /i \"JA\".*\\\\work\\\\wordpro\\\\ lotso.tmp >>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\Components\\Spell\\.*\] lotso.tmp >>lotso.tmp2
Echo "Multi User Path"="%RootDrive%\\Lotus\\compnent\\spell\\">>lotso.tmp2
Echo "UserDictionaryFiles"="ltsuser1.udc">>lotso.tmp2
Echo.>>lotso.tmp2
Echo [HKEY_CURRENT_USER\Software\Lotus\Components\Spell\4.0]>>lotso.tmp2
Echo "Multi User Path"="%RootDrive%\\Lotus\\compnent\\spell\\">>lotso.tmp2
Echo "UserDictionaryFiles"="ltsuser1.udc">>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\SuiteStart\\[0-9][0-9]\.[0-9]\] lotso.tmp >>lotso.tmp2
Echo "Configure"=dword:00000001>>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\SmartCenter\\[0-9][0-9]\.[0-9]\] lotso.tmp >>lotso.tmp2
Echo "Configure"=dword:00000001>>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i \[HKEY_CURRENT_USER\\Software\\Lotus\\SmartCenter\\.*\\Paths\\Work\] lotso.tmp >>lotso.tmp2
findstr /i JA\".*\\\\Work\\\\SmartCtr\" lotso.tmp >>lotso.tmp2
Echo.>>lotso.tmp2
findstr /i /r /c:"\[HKEY_CURRENT_USER\\Software\\Lotus\\FastSite\\.*\\Paths\]" lotso.tmp >>lotso.tmp2
findstr /i /r /c:"\"Work Directory\".*\\\\work\\\\fastsite\\\\" lotso.tmp >>lotso.tmp2

Rem #########################################################################
Rem
Rem �p�X��ύX���܂��B  
Rem

echo %LOTSOINS%> lotso.tmp3
..\acsr "\\" "\\\\" lotso.tmp3 lotso.tmp4

for /f "tokens=*" %%i in ( 'type lotso.tmp4' ) do set LOTSOINST=%%i

..\acsr "%LOTSOINST%" "%RootDrive%\\lotus" lotso.tmp2 ..\Logon\LotsoUsr.reg

Del lotso.tmp >Nul: 2>&1
Del lotso.tmp2 >Nul: 2>&1
Del lotso.tmp3 >Nul: 2>&1
Del lotso.tmp4 >Nul: 2>&1

Rem #########################################################################
Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���烆�[�U�[��
Rem �z�[�� �f�B���N�g���ɃV���[�g�J�b�g �t�@�C�����ړ����܂��B
Rem

If Exist "%COMMON_STARTUP%\���Ľ��� 97.lnk" Move "%COMMON_STARTUP%\���Ľ��� 97.lnk" "%LOTSOINS%" >Nul: 2>&1
If Exist "%COMMON_STARTUP%\�ϰľ��� 97.lnk" Move "%COMMON_STARTUP%\�ϰľ��� 97.lnk" "%LOTSOINS%" >Nul: 2>&1
If Exist "%COMMON_STARTUP%\�X�C�[�g�X�^�[�g 2000.lnk" Move "%COMMON_STARTUP%\�X�C�[�g�X�^�[�g 2000.lnk" "%LOTSOINS%" >Nul: 2>&1
If Exist "%COMMON_STARTUP%\�X�}�[�g�Z���^�[ 2000.lnk" Move "%COMMON_STARTUP%\�X�}�[�g�Z���^�[ 2000.lnk" "%LOTSOINS%" >Nul: 2>&1

Rem #########################################################################
Echo.
Echo   ���[�^�X �X�[�p�[�I�t�B�X ������ɍ쓮���邽�߂ɂ́A���݃��O�I��
Echo   ���Ă��郆�[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t
Echo   ���Ă���ēx���O�I������K�v������܂��B
Echo.
Echo ���[�^�X �X�[�p�[�I�t�B�X �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪
Echo �������܂����B
Pause

:Done