@Echo Off

Rem
Rem	�w�� Application Compatibility Script for PeachTree Complete 
Rem     Accounting v6.0
Rem
Rem

Rem #########################################################################
Rem
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################

REM
REM  �N�������n�J���O�ɥ[�J UsrLogn1.cmd
REM
FindStr /I ptr6Usr %SystemRoot%\System32\UsrLogn1.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call ptr6Usr.Cmd >> %SystemRoot%\System32\UsrLogn1.Cmd
:Skip1

Rem #########################################################################
Echo.
Echo   ���F���� Peachtree 6.0 ���T�ާ@�A�ثe�w�n
Echo   �J���ϥΪ̥������n�X�A�A���s�n�J�A�~�����
Echo   Peachtree Commplate Accounting v6.0�C
Echo.
Echo PeachTree 6.0 �h�ϥΪ����ε{���վ�B�z����
Pause

:done
