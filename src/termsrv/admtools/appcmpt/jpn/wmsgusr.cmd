
Rem
Rem �t�H�[���̃f�B���N�g�����R�s�[���āAWindows Messaging �����삷��悤�ɂ��܂��B
Rem

If Exist "%RootDrive%\Windows\Forms" Goto Skip1
If Not Exist "%SystemRoot%\Forms" Goto Skip1
Xcopy "%SystemRoot%\Forms" "%RootDrive%\Windows\Forms" /e /i >Nul: 2>&1
:Skip1

