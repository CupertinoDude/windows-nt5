@echo off

REM 
REM PeachTree�� ���� ���� ���α׷� ȣȯ ��ũ��Ʈ �α׿� �Ϸ�
REM Accounting v6.0
REM    

REM
REM ���� ���α׷��� bti.ini ������ ����մϴ�. �� ���Ͽ��� MKDE.TRN�� 
REM �ϵ� �ڵ�� ��ġ�� ������ BTrieve ������ ��� �ֽ��ϴ�.  �� ��ġ�� ���� ���α׷��� 
REM ���� ����� �����ϰ� �ϱ� ���� ����� ���� ���͸��� ����Ǿ�� �մϴ�.
REM

REM ����� ���� ���͸��� �����մϴ�.
cd %userprofile%\windows > NUL:

REM %userprofile%��(��) trnfile= �׸� ���� %systemroot%�� �ٲߴϴ�.
"%systemroot%\Application Compatibility Scripts\acsr" "%systemroot%" "%userprofile%\windows" %systemroot%\bti.ini bti.ini
