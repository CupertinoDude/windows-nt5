@Echo off

If Exist "%RootDrive%\Lotus" Goto Done

Rem #########################################################################
Rem
Rem ���[�^�X �X�[�p�[�I�t�B�X �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\lotso.Cmd" LOTSOINS "HKCU\Software\Lotus\Components" "User Path" STRIPCHAR\2

If Exist "%Temp%\lotso.Cmd" Call "%Temp%\lotso.Cmd"
Del "%Temp%\lotso.Cmd" > Nul: 2>&1

Rem #########################################################################

Rem
Rem ���[�^�X ���[�h�v�� �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\wordpro.Cmd" WP "HKLM\Software\Lotus\Wordpro\98.0" "Path" ""

If Exist "%Temp%\wordpro.Cmd" Call "%Temp%\wordpro.Cmd"
Del "%Temp%\wordpro.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���[�^�X �X�}�[�g�Z���^�[ �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\smart.Cmd" SC "HKLM\Software\Lotus\SmartCenter\98.0" "Path" ""

If Exist "%Temp%\smart.Cmd" Call "%Temp%\smart.Cmd"
Del "%Temp%\smart.Cmd" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ���[�^�X �t���[�����X �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\flance.Cmd" FL "HKLM\Software\Lotus\FreeLance\98.0" "Path" ""

If Exist "%Temp%\flance.Cmd" Call "%Temp%\flance.Cmd"
Del "%Temp%\flance.Cmd" >Nul: 2>&1

Rem
Rem #########################################################################

Rem
Rem ���[�^�X �t�@�X�g�T�C�g �̃C���X�g�[������Ă���p�X���擾���܂��B  
Rem

..\ACRegL "%Temp%\fsite.Cmd" FS "HKLM\Software\Lotus\FastSite\1.0" "Path" ""

If Exist "%Temp%\fsite.Cmd" Call "%Temp%\fsite.Cmd"
Del "%Temp%\fsite.Cmd" >Nul: 2>&1

call TsMkUDir "%RootDrive%\Lotus\Work\123\Auto"
call TsMkUDir "%RootDrive%\Lotus\work\approach"
call TsMkUDir "%RootDrive%\Lotus\work\flg"
call TsMkUDir "%RootDrive%\Lotus\backup\flg"
call TsMkUDir "%RootDrive%\Lotus\work\organize"
call TsMkUDir "%RootDrive%\Lotus\backup\organize"
call TsMkUDir "%RootDrive%\Lotus\work\smartctr"
call TsMkUDir "%RootDrive%\Lotus\work\wordpro"
call TsMkUDir "%RootDrive%\Lotus\backup\wordpro"
call TsMkUDir "%RootDrive%\Lotus\compnent\spell"
call TsMkUDir "%RootDrive%\Lotus\work\fastsite"

Rem ���[�^�X ���[�h�v�� ���C���X�g�[������Ă��Ȃ��ꍇ�͎��̏������X�L�b�v���܂��B
If "%WP%A" == "A" Goto Skip1

Rem ���̃t�@�C���Œ�`����Ă��� ���W�X�g�� �L�[��o�^���܂��B���[�h�v�����o�^��
Rem �X�L�b�v���邱�Ƃ�����܂��B
set List1="%WP%\expcntx.reg" "%WP%\ltscorrt.reg" "%WP%\ltsfills.reg"
set List2="%WP%\lwphtml.reg" "%WP%\lwpimage.reg" "%WP%\lwptools.reg" "%WP%\lwpuser.reg" "%WP%\wpinst.reg"
set List3="%SC%\cntr.reg" "%SC%\tray.reg" "%FL%\flg.reg" "%FS%\fst.reg"

regedit /s %List1% %List2% %List3%

:Skip1

regedit /s LotsoUsr.reg

Rem #########################################################################
Rem
Rem ���ׂẴ��[�U�[�p�̃X�^�[�g�A�b�v ���j���[���烆�[�U�[��
Rem �z�[�� �f�B���N�g���ɃV���[�g�J�b�g �t�@�C�����ړ����܂��B
Rem

If Not Exist "%LOTSOINS%\���Ľ��� 97.lnk" Goto Skip2
Copy "%LOTSOINS%\���Ľ��� 97.lnk" "%USER_STARTUP%" >Nul: 2>&1

:Skip2
If Not Exist "%LOTSOINS%\�ϰľ��� 97.lnk" Goto Skip3
Copy "%LOTSOINS%\�ϰľ��� 97.lnk" "%USER_STARTUP%" >Nul: 2>&1

:Skip3
If Not Exist "%LOTSOINS%\�X�C�[�g�X�^�[�g 2000.lnk" Goto Skip4
Copy "%LOTSOINS%\�X�C�[�g�X�^�[�g 2000.lnk" "%USER_STARTUP%" >Nul: 2>&1

:Skip4
If Not Exist "%LOTSOINS%\�X�}�[�g�Z���^�[ 2000.lnk" Goto Done
Copy "%LOTSOINS%\�X�}�[�g�Z���^�[ 2000.lnk" "%USER_STARTUP%" >Nul: 2>&1

:Done