@Echo Off

Rem #########################################################################

Rem
Rem �ˬd CMD Extensions  �O�_�w�ҥ�
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c eudora4.cmd
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

Rem �q�n���ɨ��o Eudora ���O�C�C

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:1" 

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �L�k�q�n���ɧ�� Eudora Pro 4.0 ���O�C�C
Echo ���ˬd Eudora Pro 4.0 �O�_�w�w�ˡA�í��s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1
set EudCmd=%EUDTMP:~0,-2%

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:2" 

If Not ErrorLevel 1 Goto Cont2
Echo.
Echo �L�k�q�n���ɧ�� Eudora Pro 4.0 ���O�C�C
Echo ���ˬd Eudora Pro 4.0 �O�_�w�w�ˡA�í��s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont2
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1

Set EudoraInstDir=%EUDTMP:~0,-13%

Rem #########################################################################

If Exist "%EudoraInstDir%\descmap.pce" Goto Cont0
Echo.
Echo �b�ϥγo�����ε{���ۮe�ʫ��O�ɤ��e�A�z���������� Eudora 4.0 �@���C
Echo ���� Eudora ����A�Цb Eudora Pro ��Ƨ�����s Eudora Pro ���|����
Echo �Ф��e�C�N %RootDrive%\eudora.ini �Ȫ��[��ؼФ��C
Echo   %RootDrive%\eudora.ini 
Echo �ؼФ��e�ȱN�|�O:
Echo  "%EudoraInstDir%\Eudora.exe" %RootDrive%\eudora.ini
Echo.
Pause
Goto Done

:Cont0

Rem
Rem �ܧ�n�����X�A�N���|���V�ϥΪ̫��w��
Rem �ؿ��C
Rem

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#INSTDIR#" "%EudoraInstDir%" Template\Eudora4.Key Eudora4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" Eudora4.tmp Eudora4.key

regini eudora4.key > Nul:
del eudora4.tmp
del eudora4.key

Rem �p�G��ӬO����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem ��s descmap.pce ���ϥ��v���C
cacls "%EudoraInstDir%\descmap.pce" /E /G "Terminal Server User":R >NUL: 2>&1

Rem #########################################################################

Echo.
Echo   ���F�T�O Eudora Pro 4.0 ��A�����A�ثe�n�J��
Echo   �ϥΪ̥������n�X�A���s�n�J��A���� Eudora Pro 4.0�C
Echo.
Echo Eudora 4.0 �h�ϥΪ����ε{���վ㧹���C
Pause

:done
