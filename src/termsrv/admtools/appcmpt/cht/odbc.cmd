@echo off
Rem
Rem  �o�ӫ��O�ɷ|��s ODBC �O���ɦ�m�C
Rem

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w���o�ӫ��O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\ODBC.Key ODBC.Key
regini ODBC.Key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo.
Echo   �]�w SQL ��ƨӷ��ɡA�����@�U [�ϥΪ� DSN] ���޼���
Echo   ���� [�ﶵ] ���s�A�M����@�U [�]�w�ɳB�z] ���s�C�N�d
Echo   �߰O���ɤβέp�O�����ܧ��x�s�b�ϥΪ̮ڥؿ��Ϻо�
Echo   (%RootDrive%) ���C
Echo.
Echo   ���~�A�t�κ޲z���i�H���Ҧ����ϥΪ̳]�w
Echo   �@�Ӹ�ƨӷ��C�����A�}�Ҥ@�ӫ��O�����A
Echo   �ÿ�J "Change User /Install"���O�C���ۡA
Echo   �]�w��ƨӷ��C�̫�A�b���O��������J
Echo   "Change User /Execute" ���O�A�^�����Ҧ��C
Echo.

Echo ODBC �h�ϥΪ����ε{���վ�B�z����
Pause

:Done

