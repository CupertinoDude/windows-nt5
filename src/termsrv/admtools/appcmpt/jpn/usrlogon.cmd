@Echo Off

Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem
Rem ����� RootDrive ��K�v�Ƃ��Ȃ��X�N���v�g�p�ł��B
Rem

If Not Exist "%SystemRoot%\System32\Usrlogn1.cmd" Goto cont0
Cd /d "%SystemRoot%\Application Compatibility Scripts\Logon"
Call "%SystemRoot%\System32\Usrlogn1.cmd"

:cont0

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g�������߂܂��B�ݒ肳��Ă��Ȃ��ꍇ�A
Rem �I�����܂��B
Rem

Cd /d %SystemRoot%\"Application Compatibility Scripts"
Call RootDrv.Cmd
If "A%RootDrive%A" == "AA" End.Cmd

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g�����h���C�u�����Ƀ}�b�v���܂��B
Rem

Net Use %RootDrive% /D >NUL: 2>&1
Subst %RootDrive% "%HomeDrive%%HomePath%"
if ERRORLEVEL 1 goto SubstErr
goto AfterSubst
:SubstErr
Subst %RootDrive% /d >NUL: 2>&1
Subst %RootDrive% "%HomeDrive%%HomePath%"
:AfterSubst

Rem
Rem ���ꂼ��̃A�v���P�[�V���� �X�N���v�g�����s���܂��B�A�v���P�[�V���� �X�N���v�g�̓C���X�g�[�� �X�N���v�g�����s���ꂽ���ɁA
Rem �����I�� UsrLogn2.Cmd �ɒǉ�����܂��B
Rem

If Not Exist %SystemRoot%\System32\UsrLogn2.Cmd Goto Cont1

Cd Logon
Call %SystemRoot%\System32\UsrLogn2.Cmd

:Cont1

:Done
