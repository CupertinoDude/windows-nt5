@echo Off
Rem
Rem  �o�ӫ��O�ɱN�|��s Microsoft SNA Server 3.0�A�����b
Rem  Windows Terminal Server �U���T����C
Rem
Rem  �o�ӫ��O�ɱN�|�N�ƭ� SNA dll �n�����t�γq���ɡA 
Rem  �� SNA ���A���b�h�ϥΪ̼Ҧ������T����C
Rem
Rem  ���n!  �p�G�Q�b�R�O���ܦr���U����o�ӫ��O�ɡA
Rem  �нT�w�z�w�w�� SNA 3.0�C
Rem

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    ���]�w SNAROOT �����ܼơA�]���L�k�����h�ϥΪ�
Echo    ���ε{���վ�B�z�C�p�G�b�w�� SNA Server 3.0
Echo    ���e���}�ҥΨӰ���o�ӫ��O�ɪ����O�߼h�A�N�|
Echo    �o�ͳo�ر��p�C
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1

Rem ���� SNA �A��
net stop snabase

Rem �N SNA DLL �n�����t�γq���ɡC
Register %SNAROOT%\SNADMOD.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAMANAG.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\WAPPC32.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\DBGTRACE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\MNGBASE.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNATRC.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALM.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNANW.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNAIP.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNABASE.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASERVR.EXE /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNASII.DLL /SYSTEM >Nul: 2>&1
Register %SNAROOT%\SNALINK.DLL /SYSTEM >Nul: 2>&1

Rem ���s�Ұ� SNA �A��
net start snabase

Echo SNA Server 3.0 �h�ϥΪ����ε{���վ�B�z����
Pause

:Cont2

