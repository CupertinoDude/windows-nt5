@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN��]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

..\ACRegL "%Temp%\PB6.Cmd" PB6INST "HKCU\Software\ODBC\ODBC.INI\Powersoft Demo DB V6" "DataBaseFile" "STRIPCHAR\1"
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n���ɧ�� PowerBuilder 6.0  ���O�C�C
Echo ���ˬd PowerBuilder 6.0  �O�_�w�w�ˡA�í��s
Echo ����o�ӫ��O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call "%Temp%\PB6.Cmd"
Del "%Temp%\PB6.Cmd" >Nul: 2>&1

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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\pwrbldr6.Key pwrbldr6.key

regini pwrbldr6.key > Nul:

Rem �p�G��ӬO����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem ��s PBld6Usr.Cmd �H�ϬM��ڦw�˥ؿ��A�ñN��
Rem �[�J UsrLogn2.Cmd ���O�ɡC
Rem

..\acsr "#INSTDIR#" "%PB6INST%" ..\Logon\Template\PBld6Usr.Cmd ..\Logon\PBld6Usr.cmd

FindStr /I PBld6Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call PBld6Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Rem #########################################################################

Echo.
Echo   ���F�T�O PowerBuilder 6.0 ��A�����A�ثe�n�J��
Echo   �ϥΪ̥������n�X�A���s�n�J��A���� PowerBuilder 6.0�C
Echo.
Echo PowerBuilder 6.0 �h�ϥΪ����ε{���վ㧹���C
Pause

:done
