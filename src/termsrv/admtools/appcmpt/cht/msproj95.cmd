@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w���o�ӫ��O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done


Rem #########################################################################

Rem
Rem  �q [�Ұ�] �\������� [�ֳt�M��]�C
Rem  [�ֳt�M��] �|�ϥΤj�q�귽�í��C�t�ήį�C
Rem


If Not Exist "%COMMON_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" Goto Cont1
Del "%COMMON_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" >Nul: 2>&1
:Cont1

If Not Exist "%USER_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" Goto Cont2
Del "%USER_STARTUP%\Microsoft Office �ֳt�d�\����.lnk" >Nul: 2>&1
:Cont2


Rem #########################################################################

Rem
Rem �N�ϥΪ̥ؿ�����ϥΪ̥ؿ��C
Rem

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ��w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msproj95.Key msproj95.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" msproj95.tmp msproj95.Key
Del msproj95.tmp >Nul: 2>&1
regini msproj95.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem  �n�J��Ū���o���ɮ�
Rem  �¤��ϥΪ̦s���v�C

Cacls ..\Logon\Template\prj95usr.key /E /P "Authenticated Users":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem Add proj95Usr.Cmd to the UsrLogn2.Cmd script
Rem

FindStr /I prj95Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip2
Echo Call prj95Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip2


Rem #########################################################################

Echo.
Echo   Administrator �|�̷ӤU�C�B�J�����C�ӨϥΪ�
Echo   �ߤ@���w�]�ؿ�:
Echo.
Echo    1) �ϥαz�������ƹ����s���@�U [�}�l] �\���C
Echo    2) ��ܧ���\����� [�s�������ϥΪ�]�C
Echo       �ɮ��`�ޱN�|�X�{�C
Echo    3) �b�����k�䪺 [�{����] ��Ƨ��W����U�C
Echo    4) �ϥαz�������ƹ����s���@�U�����k�䪺
Echo       Microsoft Project �ϥܡC
Echo    5) ��ܧ���\����� [���e]�C
Echo    6) ��� [���|] ���޼��Ҩ��ܧ�}�l��m: ���ءC��� [�T�w]�C
Echo.
Echo    �`�N�ƶ�: �C�ӨϥΪ̱N %RootDrive% ������L�̪��D�ؿ��C
Echo          �}�l��m����ĳ��: �� %RootDrive%\My Documents�C
Echo.
Pause

Rem #########################################################################

Echo.
Echo   ���F���� Project 95 ���`�ާ@�A�ثe�w�n
Echo   �J���ϥΪ̥������n�X�A�A���s�n�J�A�~��
Echo   �������ε{���C
Echo.
Echo Microsoft Project 95 �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
