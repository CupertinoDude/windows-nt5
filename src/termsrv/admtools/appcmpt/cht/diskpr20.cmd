@echo Off

Rem
Rem  �o�ӫ��O�ɱN�|��s DiskKeeper 2.0 �A�� Administration
Rem  �{�����`�B�@�C

Rem #########################################################################

Rem
Rem �q�n�������o DiskKeeper 2.0 �w�˦�m�C
Rem �p�G�䤣��A���]�o�����ε{���å��w�ˡA����ܿ��~�T���C
Rem

..\ACRegL %Temp%\DK20.Cmd DK20INST "HKLM\Software\Microsoft\Windows\CurrentVersion\App Paths\DKSERVE.EXE" "Path" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo �L�k�q�n������� DiskKeeper 2.0 ���w�˦�m�C
Echo �нT�{�O�_�w�w�����ε{���A�í��s����o��
Echo ���O�ɡC
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\DK20.Cmd
Del %Temp%\DK20.Cmd >Nul: 2>&1

Rem #########################################################################



register %DK20INST% /system >Nul: 2>&1

Echo DiskKeeper 2.x Multi-user Application Tuning Complete
Pause

:Done
