@ECHO OFF
REM *** SNA40SRV.CMD - ע�� SNA Server 4.0 �����ļ� ***

Rem #########################################################################

If Not "A%SNAROOT%A" == "AA" Goto Cont1
Echo.
Echo    �޷���ɶ��û�Ӧ�ó��������ԭ���ǣ�
Echo    SNAROOT ����������δ���á���������������
Echo    �ű�������������ڰ�װ SNA Server 4.0 ֮ǰ
Echo    �򿪵ģ�����������ܻᷢ����
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

Echo SNA Server 4.0 ���û�Ӧ�ó�������ѽ���
Pause

:Cont2
