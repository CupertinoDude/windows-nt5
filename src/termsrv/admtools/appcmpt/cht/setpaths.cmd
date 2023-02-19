REM ###############################################################
REM #                                                             #
REM # SetPaths                                                    #
REM #                                                             #
REM # ��X�U�C���|�é�J�����ܼƤ��A    #
REM # �����O�ɤ��ݨt�θ��|�r��N�i����C  #
REM # �o�������O�ɤ��ݨ̾a�t�λy���Ӱ���C #
REM #                                             #
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
REM # �ϥ� GETPATHS �ﶵ�ӳ]�w�Ҧ������ܼ�
REM ###############################################################
"%systemroot%\Application Compatibility Scripts\ACRegL.exe" "%TEMP%\getpaths.cmd" COMMON_PATHS "HKLM\Software" "" GETPATHS

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �L�k������ΩΨϥΪ̸��|�C
Echo.
Goto Failure

:Cont1
Call "%TEMP%\getpaths.cmd"
Del "%TEMP%\getpaths.cmd" >Nul: 2>&1

REM �p�G�U�C�ȥ��T�A�ާ@�|���\�C
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
Echo �Y�Ǥ��ΩΨϥΪ̸��|���d�ߤw����!
Echo �̿�o�ӫ��O�ɨӰ��檺���ε{���i��L�k���\�w�ˡC
Echo �иѨM�o�Ӱ��D�A�í��դ@���C
Echo.
Set _SetPaths=FAIL
REM Pause
Goto Done

:Done
