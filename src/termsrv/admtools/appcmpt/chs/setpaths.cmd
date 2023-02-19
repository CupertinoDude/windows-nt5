REM ###############################################################
REM #                                                             #
REM # SetPaths                                                    #
REM #                                                             #
REM # ������·����ȡ�����������У�                                #
REM # ����ű�����Ӳ����ϵͳ·���ַ��������С�                    #
REM # ������ű�����ϵͳ��������                                  #
REM # �������С�                                                  #
REM #                                                             #
REM # �����û�:����     		COMMON_STARTUP            #
REM # �����û�:����ʼ���˵�             COMMON_START_MENU         #
REM # �����û�:����ʼ���˵�\����	COMMON_PROGRAMS           #
REM # ��ǰ�û�:����ʼ���˵�		USER_START_MENU           #
REM # ��ǰ�û�:����      		USER_STARTUP              #
REM # ��ǰ�û�:����ʼ���˵�\����	USER_PROGRAMS             #
REM # ��ǰ�û�:�ҵ��ĵ�                 MY_DOCUMENTS              #
REM # ��ǰ�û�:ģ��                     TEMPLATES                 #
REM # ��ǰ�û�:Ӧ�ó�������     APP_DATA
REM #                                                             #
REM ###############################################################

REM ###############################################################
REM # �� GETPATHS ѡ�����������л�������
REM ###############################################################
"%systemroot%\Application Compatibility Scripts\ACRegL.exe" "%TEMP%\getpaths.cmd" COMMON_PATHS "HKLM\Software" "" GETPATHS

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �޷�����ͨ�û��û�·����
Echo.
Goto Failure

:Cont1
Call "%TEMP%\getpaths.cmd"
Del "%TEMP%\getpaths.cmd" >Nul: 2>&1

REM ��������ֵ��ȷ����˵��ִ�гɹ�
REM 	COMMON_START_MENU = %COMMON_START_MENU%
REM 	COMMON_STARTUP = %COMMON_STARTUP%
REM 	COMMON_PROGRAMS = %COMMON_PROGRAMS%
REM 	USER_START_MENU = %USER_START_MENU%
REM 	USER_STARTUP = %USER_STARTUP%
REM 	USER_PROGRAMS = %USER_PROGRAMS%
REM     MY_DOCUMENTS = %MY_DOCUMENTS%
REM     TEMPLATES = %TEMPLATES%
REM     APP_DATA= %APP_DATA%
Set _SetPaths=SUCCEED
Goto Done

:Failure
Echo.
Echo ������һ��ͨ�û��û�·���Ĳ�ѯû�гɹ�!
Echo ��������ű���Ӧ�ó�������޷���װ�ɹ���
Echo ����������⣬����һ�Ρ�
Echo.
Set _SetPaths=FAIL
REM ��ͣ
Goto Done

:Done
