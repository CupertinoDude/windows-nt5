@ECHO OFF
REM *** SNA40SRV.CMD - Batch file to register SNA Server 4.0 ***

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    ���]�w SNAROOT �����ܼơA�]���L�k�����h�ϥΪ�
Echo    ���ε{���վ�B�z�C�p�G�b�w�� SNA Server 4.0
Echo    ���e���}�ҥΨӰ���o�ӫ��O�ɪ����O�߼h�A�N�|
Echo    �o�ͳo�ر��p�C
Echo.
Pause
Goto Cont2

Rem #########################################################################

:Cont1
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

Echo SNA Server 4.0 �h�ϥΪ����ε{���վ�B�z����
Pause

:Cont2
