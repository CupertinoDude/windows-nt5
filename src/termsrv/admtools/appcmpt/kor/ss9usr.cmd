@echo off

Rem
Rem #########################################################################

Rem
Rem ������Ʈ������ Lotus Wordpro 9 ��ġ ��ġ�� ����ϴ�.
Rem

..\ACRegL "%Temp%\wordpro.Cmd" WP "HKLM\Software\Lotus\Wordpro\98.0" "Path" ""

Call "%Temp%\wordpro.Cmd"
Del "%Temp%\wordpro.Cmd" >Nul: 2>&1

Call "%SystemRoot%\Application Compatibility Scripts\RootDrv.cmd"

if Exist "%RootDrive%\Lotus" Goto Done

cd "%SystemRoot%\Application Compatibility Scripts\Logon"
call TsMkUDir "%RootDrive%\Lotus\Work\123\Auto"
call TsMkUDir "%RootDrive%\Lotus\work\approach"
call TsMkUDir "%RootDrive%\Lotus\work\flg"
call TsMkUDir "%RootDrive%\Lotus\backup\flg"
call TsMkUDir "%RootDrive%\Lotus\work\organize"
call TsMkUDir "%RootDrive%\Lotus\backup\organize"
call TsMkUDir "%RootDrive%\Lotus\work\smartctr"
call TsMkUDir "%RootDrive%\Lotus\work\wordpro"
call TsMkUDir "%RootDrive%\Lotus\backup\wordpro"

Rem Lotus Wordpro�� ��ġ���� �ʾ����� �� �ܰ踦 �ǳ� �ݴϴ�.
If "%WP%A" == "A" Goto Skip1

Rem � ���� ������ ���� ���Ͽ� ���ǵ� ������Ʈ�� Ű�� �����մϴ�. wordpro�� �� Ű�� �������� �ʽ��ϴ�.
set List1="%WP%\expcntx.reg" "%WP%\ltsfills.reg" "%WP%\ltscorrt.reg" "%WP%\lwp4wp.reg" "%WP%\lwp4wpex.reg"
set List2="%WP%\lwpdca.reg" "%WP%\lwphtml.reg" "%WP%\lwpimage.reg" "%WP%\lwptools.reg"

regedit /s %List1% %List2%

:Skip1

regedit /s ss9usr.reg

:Done
