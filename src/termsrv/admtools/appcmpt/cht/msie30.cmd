@Echo Off
Rem
Rem  �o�ӫ��O�ɷ|��s Microsoft Internet Explorer 3.x �w�ˡC
Rem  ���|�N�֨��ؿ��Ahistory �ؿ��� cookies �ɮת����|
Rem  �ܧ��ϥΪ̪��D�ؿ��C
Rem

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w�����O�ɡC
Rem 
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

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msie30.Key msie30.Key
regini msie30.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Echo Microsoft Internet Explorer 3.x �h�ϥΪ����ε{���վ�B�z����
Pause

:Done

