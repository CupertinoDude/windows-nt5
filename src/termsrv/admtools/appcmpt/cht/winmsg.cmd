@Echo Off

Rem #########################################################################

Rem
Rem �ˬd %RootDrive% �O�_�w�]�w�A�ñN���]�w�����O�ɡC
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

Rem
Rem �N WMsgUsr.Cmd �[�J UsrLogn2.Cmd ���O�ɡC
Rem

FindStr /I WMsgUsr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call WMsgUsr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1


Echo.
Echo   ���F���� Windows Messaging ���T�ާ@�A�ثe�w
Echo   �n�J���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   ���ε{���C
Echo.
Echo Windows Messaging �h�ϥΪ����ε{���վ�B�z����
Pause

:Done

