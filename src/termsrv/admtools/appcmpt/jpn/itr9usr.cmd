@echo off

Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\itaro9.Cmd ITARO9INS "HKLM\SOFTWARE\Justsystem\Common\SETUP\Folder" "Just" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ���W�X�g������ �ꑾ�Y 9 �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo �ꑾ�Y 9 ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\itaro9.Cmd
Del %Temp%\itaro9.Cmd > Nul: 2>&1


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 ���}���`���[�U�[�Ŏg�p�ł���悤��
Rem ���W�X�g����ύX���܂��B
Rem

If Exist "%RootDrive%\JUST" Goto Cont1
regedit /s itr9usr2.Reg
regedit /s itr9usr.Reg
:Cont1


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �̃f�[�^�p�̃f�B���N�g�������[�U�[�̃z�[�� �f�B���N�g��
Rem �ɍ쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\JUST"
call TsMkUDir "%RootDrive%\JUST\ATOK12"
call TsMkUDir "%RootDrive%\JUST\INTERNET"
call TsMkUDir "%RootDrive%\JUST\INTERNET\CACHE"
call TsMkUDir "%RootDrive%\JUST\JSDIC"
call TsMkUDir "%RootDrive%\JUST\JSDIC\MULTIDIC"
call TsMkUDir "%RootDrive%\JUST\JSLIB32"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\ADR"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\JSTART"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\JSTART\���ع����"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\JSTART\����ȯ�°�"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\Memo"
call TsMkUDir "%RootDrive%\JUST\JSLIB32\USER\TARO9"
call TsMkUDir "%RootDrive%\JUST\TAROWIN9"
call TsMkUDir "%RootDrive%\JUST\TAROWIN9\HTMLCNVT"
Call TsMkUDir "%RootDrive%\JUST\TAROWIN9\MACRO"
call TsMkUDir "%RootDrive%\JUST\�޷����"
call TsMkUDir "%RootDrive%\JUST\�޷����\MYHOME"


Rem #########################################################################
Rem
Rem �ꑾ�Y 9 �̃t�@�C�����N�C�b�N�N���ɓo�^���܂��B
Rem

If Not Exist "%RootDrive%\Application Data\Microsoft\Internet Explorer\Quick Launch\�ꑾ�Y�X.LNK" copy "%ITARO9INS%\JSLIB32\USER\JSTART\���ع����\�ꑾ�Y�X.LNK" "%RootDrive%\Application Data\Microsoft\Internet Explorer\Quick Launch\�ꑾ�Y�X.LNK"
If Not Exist "%RootDrive%\Application Data\Microsoft\Internet Explorer\Quick Launch\Shuriken.LNK" copy "%ITARO9INS%\JSLIB32\USER\JSTART\����ȯ�°�\Shuriken.LNK" "%RootDrive%\Application Data\Microsoft\Internet Explorer\Quick Launch\Shuriken.LNK"


Rem #########################################################################
Rem
Rem �t�@�C�������[�U�[�̃z�[�� �f�B���N�g���ɃR�s�[���܂��B
Rem

call :itr9copy ATOK12\ATKBNG98.STY
call :itr9copy ATOK12\ATKBNGDP.STY
call :itr9copy ATOK12\ATKMROMA.STY
call :itr9copy ATOK12\ATKMSIME.STY
call :itr9copy ATOK12\ATKOAK.STY
call :itr9copy ATOK12\ATKOAKV.STY
call :itr9copy ATOK12\ATKVJE.STY
call :itr9copy ATOK12\ATKWX3.STY
call :itr9copy ATOK12\ATKWXG.STY
call :itr9copy ATOK12\ATOK12.STY
call :itr9copy ATOK12\ATOK12IT.INI
call :itr9copy ATOK12\ATOK12N.STY
call :itr9copy ATOK12\ATOK12P.STY
call :itr9copy ATOK12\ATOK12U1.DIC
call :itr9copy ATOK12\ATOK12U2.DIC
call :itr9copy ATOK12\ATOK12U3.DIC
call :itr9copy ATOK12\ATOK12U4.DIC
call :itr9copy ATOK12\ATOK12U5.DIC
call :itr9copy ATOK12\ATOKRH.BIN
call :itr9copy INTERNET\DEFAULT.JVM
call :itr9copy JSDIC\JDSAME3.DSD
call :itr9copy JSDIC\JDSAME3.DSI
call :itr9copy JSDIC\JSENG2U.DSS
call :itr9copy JSDIC\JSYUTA2U.DIC
call :itr9copy JSLIB32\USER\JSCLPSYS.ALB
call :itr9copy JSLIB32\USER\JSEQ_STK.FML
call :itr9copy JSLIB32\USER\JSEQ_STK.MRK
call :itr9copy JSLIB32\USER\JSFNTY3D.FPR
call :itr9copy JSLIB32\USER\JSSLPSYS.SLP
call :itr9copy JSLIB32\USER\ADR\��Ж�.TXT
call :itr9copy JSLIB32\USER\ADR\��E��.TXT
call :itr9copy JSLIB32\USER\ADR\�h��.TXT
call :itr9copy JSLIB32\USER\ADR\������.TXT
call :itr9copy JSLIB32\USER\ADR\�֌W.TXT
call :itr9copy JSLIB32\USER\ADR\���ږ�.TXT
call :itr9copy JSLIB32\USER\JSTART\���ع����\JS�A�����2.LNK
call :itr9copy JSLIB32\USER\JSTART\���ع����\�ꑾ�Y�X.LNK
call :itr9copy JSLIB32\USER\JSTART\����ȯ�°�\Just InfoRunner 2.0.LNK
call :itr9copy JSLIB32\USER\JSTART\����ȯ�°�\JustDiver 2.0.LNK
call :itr9copy JSLIB32\USER\JSTART\����ȯ�°�\JustView 3.0.LNK
call :itr9copy JSLIB32\USER\JSTART\����ȯ�°�\Shuriken.LNK
call :itr9copy JSLIB32\USER\JSTART\����ȯ�°�\Shuriken���[�����M.LNK
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.ALB
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.CAL
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.KEY
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.MCR
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.MNU
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.PBS
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.PFS
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.PVS
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.TBM
call :itr9copy JSLIB32\USER\TARO9\TAR9_STK.TLX
call :itr9copy TAROWIN9\MACRO\T9ESP.JTD
call :itr9copy TAROWIN9\MACRO\T9MEMORY.JTD
call :itr9copy TAROWIN9\MACRO\T9TYPING.JTD
call :itr9copy TAROWIN9\MACRO\T9WORLD.JTD
call :itr9copy TAROWIN9\MACRO\TAR9_SMP.MCR

if not Exist "%RootDrive%\JUST\ATOK12\ATOK12W.INI" copy "%RootDrive%\JUST\ATOK12\ATOK12IT.INI" "%RootDrive%\JUST\ATOK12\ATOK12W.INI"

goto :Done

:itr9copy
Rem #########################################################################
Rem
Rem �t�@�C�������[�U�[�̃z�[�� �f�B���N�g���ɃR�s�[���܂��B
Rem

If Not Exist "%RootDrive%\JUST\%1 %2 %3" copy "%ITARO9INS%\%1 %2 %3" "%RootDrive%\JUST\%1 %2 %3"

:Done
