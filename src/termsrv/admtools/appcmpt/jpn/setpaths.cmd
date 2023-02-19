REM ###############################################################
REM #                                                             #
REM # SetPaths                                                    #
REM #                                                             #
REM # ���̃p�X�����ϐ��ɓW�J���āA�n�[�h �R�[�h���ꂽ�V�X�e��   #
REM # �p�X�̕�������g�킸�ɃX�N���v�g�����s�ł���悤�ɂ��܂��B  #
REM # ����ɂ��V�X�e���̌���Ɉˑ����邱�ƂȂ��X�N���v�g�����s  #
REM # �ł���悤�ɂȂ�܂��B                                      #
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
REM # GETPATHS �I�v�V�������g���āA���ׂĂ̊��ϐ���ݒ肵�܂��B
REM ###############################################################
"%systemroot%\Application Compatibility Scripts\ACRegL.exe" "%TEMP%\getpaths.cmd" COMMON_PATHS "HKLM\Software" "" GETPATHS

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo ���ʃp�X�܂��̓��[�U�[�p�X���擾�ł��܂���B
Echo.
Goto Failure

:Cont1
Call "%TEMP%\getpaths.cmd"
Del "%TEMP%\getpaths.cmd" >Nul: 2>&1

REM �ȉ��̒l����������΁A���s�͐����������ƂɂȂ�܂��B
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
Echo 1 �ȏ�̋��ʃp�X�܂��̓��[�U�[ �p�X�̃N�G�������s���܂����B
Echo ���̃X�N���v�g�Ɉˑ����Ă���A�v���P�[�V�����͐������C���X�g�[��
Echo ����Ȃ��ꍇ������܂��B�����������Ă���Ď��s���Ă��������B
Echo.
Set _SetPaths=FAIL
REM Pause
Goto Done

:Done
