@echo off

REM 
REM	���ε{���n�J�ۮe���O�� (�� PeachTree Complete 
REM     Accounting v6.0)
REM    

REM
REM ���ε{���ϥ� bti.ini �ɮסC�o���ɮקt�� BTrieve �]�w�A
REM �]�t MKDE.TRN ���T�w��m�C�o�Ӧ�m�����ܧ�
REM �ϥΪ̯S�w���ؿ��A���ϥΪ̥i�H�ϥ����ε{���C
REM

REM �ܧ󦨨ϥΪ̯S�w���ؿ��C
cd %userprofile%\windows > NUL:

REM �� %userprofile% ���N %systemroot%  ��J trnfile=
"%systemroot%\Application Compatibility Scripts\acsr" "%systemroot%" "%userprofile%\windows" %systemroot%\bti.ini bti.ini
