Rem
Rem ���û�����Ŀ¼��ΪӦ�ó�����������
Rem ����Ŀ¼��
Rem

call TsMkUDir "%RootDrive%\#OFFUDIR#"
call TsMkUDir "%RootDrive%\#OFFUDIR#\Templates"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem ���Ʊ��Ŀ¼���Ա� Outlook ���Խ� Word �����༭����ʹ��
Rem

If Exist "%RootDrive%\Windows\Forms" Goto Skip2
If Not Exist "%SystemRoot%\Forms" Goto Skip2
Xcopy "%SystemRoot%\Forms" "%RootDrive%\Windows\Forms" /e /i >Nul: 2>&1
:Skip2

Rem
Rem �� Custom.dic �ļ����Ƶ��û���Ŀ¼
Rem

if "#OFFUDIR#" == "Office95" goto O95
If Exist "%RootDrive%\#OFFUDIR#\Custom.Dic" Goto Skip4
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip4
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\#OFFUDIR#\Custom.Dic" >Nul: 2>&1
goto Skip4

:O95
If Not Exist "%RootDrive%\Office95\Custom.dic" Copy Nul: "%RootDrive%\Office95\Custom.dic" > Nul: 2>&1

:Skip4

