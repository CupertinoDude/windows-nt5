@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w���o�ӫ��O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �ܧ�n�����X�A�N���|���V�ϥΪ̩ҫ��w���ؿ��C
Rem

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\NetNav30.Key NetNav30.Key
regini netnav30.key > Nul:

Rem �p�G�쥻�O����Ҧ��A�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem �N Nav30Usr.Cmd �[�J UsrLogn2.Cmd ���O��
Rem

FindStr /I Nav30Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Nav30Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   ���F���� Netscape Navigator ���T�ާ@�A�ثe�w
Echo   �n�J���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   Netscape Navigator.
Echo.
Echo Netscape Navigator 3.x �h�ϥΪ����ε{���վ�B�z����
Pause

:Done

