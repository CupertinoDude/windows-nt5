@echo off
Rem
Rem  �o�ӫ��O�ɷ|��s Dr. Watson �w�ˡC
Rem  ���|�N Log File Path �� Crash Dump File �s���m�ܧ�
Rem  ��ϥΪ̪��D�ؿ��C
Rem
Rem  �`�N�A�z�]�i�H���� drwtsn32 ���ܧ�s���m�C
Rem

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�]�w�A�ó]�w�����O�ɨϥΡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem �p�G�ثe���O�w�˼Ҧ��A�ܧ��w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\DrWatson.Key DrWatson.Key
regini DrWatson.Key > Nul:

Rem �p�G�ثe�����l�Ҧ��A�ܧ�����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=


Echo Dr. Watson �h�ϥΪ����ε{���վ�B�z����
Pause

:Done
