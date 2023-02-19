REM ###############################################################
REM #                                                             #
REM # SetPaths                                                    #
REM #                                                             #
REM # ���� ��θ� ȯ�� ������ �����Ͽ�    #
REM # �ϵ��ڵ�� �ý��� ��� ���ڿ��� ���� ��ũ��Ʈ�� ����ǵ���   #
REM # �մϴ�. �̰��� ��ũ��Ʈ�� �ý��� �� �������   #
REM # ����ǰ� �մϴ�.                                            #
REM #                                                             #
REM # All Users:Startup   		COMMON_STARTUP            #
REM # All Users:Start Menu              COMMON_START_MENU         #
REM # All Users:Start Menu\Programs	COMMON_PROGRAMS           #
REM # Current User:Start Menu		USER_START_MENU           #
REM # Current User:Startup		USER_STARTUP              #
REM # Current User:Start Menu\Programs	USER_PROGRAMS             #
REM # Current User:My Documents         MY_DOCUMENTS              #
REM # Current User:Templates            TEMPLATES                 #
REM # Current User:Application Data     APP_DATA
REM #                                                             #
REM ###############################################################

REM ###############################################################
REM # ȯ�� ���� ��θ� �����ϴµ� GETPATHS �ɼ��� ����մϴ�.
REM ###############################################################
"%systemroot%\Application Compatibility Scripts\ACRegL.exe" "%TEMP%\getpaths.cmd" COMMON_PATHS "HKLM\Software" "" GETPATHS

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���� �Ǵ� ����� ��θ� �˻��� �� �����ϴ�.
Echo.
Goto Failure

:Cont1
Call "%TEMP%\getpaths.cmd"
Del "%TEMP%\getpaths.cmd" >Nul: 2>&1

REM �Ʒ� ���� ������ ���������� ����Ǿ����ϴ�.
REM COMMON_START_MENU = %COMMON_START_MENU%
REM COMMON_STARTUP = %COMMON_STARTUP%
REM COMMON_PROGRAMS = %COMMON_PROGRAMS%
REM USER_START_MENU = %USER_START_MENU%
REM USER_STARTUP = %USER_STARTUP%
REM USER_PROGRAMS = %USER_PROGRAMS%
REM MY_DOCUMENTS = %MY_DOCUMENTS%
REM TEMPLATES = %TEMPLATES%
REM APP_DATA= %APP_DATA%
Set _SetPaths=SUCCEED
Goto Done

:Failure
Echo.
Echo ���� �Ǵ� ����� ��ο� ���� ������ �ϳ� �̻� �����߽��ϴ�!
Echo �� ��ũ��Ʈ�� �����ϴ� ���� ���α׷��� ���������� ��ġ���� ���� �� �ֽ��ϴ�.
Echo ������ �ذ��ϰ� �ٽ� �õ��Ͻʽÿ�.
Echo.
Set _SetPaths=FAIL
REM �Ͻ� ����
Goto Done

:Done
