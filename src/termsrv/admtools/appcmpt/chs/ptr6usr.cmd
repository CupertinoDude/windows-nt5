@echo off

REM 
REM	PeachTree �ĵ�¼Ӧ�ó�������Խű��ѽ��� 
REM     Accounting v6.0
REM    

REM
REM ��Ӧ�ó���ʹ�� bti.ini �ļ�������ļ����� BTrieve ���ã�
REM ���� MKDE.TRN ��Ӳ����λ�á�Ҫ�ܲ���ʹ�����Ӧ�ó���
REM ���뽫���λ�øĳ�ÿ�û�Ŀ¼��
REM

REM �ĳ�ÿ�û�Ŀ¼ 
cd %userprofile%\windows > NUL:

REM replace %systemroot% by %userprofile% for entry trnfile=
"%systemroot%\Application Compatibility Scripts\acsr" "%systemroot%" "%userprofile%\windows" %systemroot%\bti.ini bti.ini
