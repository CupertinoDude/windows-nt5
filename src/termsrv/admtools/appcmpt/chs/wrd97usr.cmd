
Rem
Rem ���û�����Ŀ¼��ΪӦ�ó�����������
Rem ����Ŀ¼��
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Startup"
call TsMkUDir "%RootDrive%\Office97\Templates"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem ������ļ��������û�ģ�帴�Ƶ���ǰ�û�ģ��λ��
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" copy "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" "%UserProfile%\%Templates%\" /Y >Nul: 2>&1

Rem
Rem �� ARTGALRY.CAG ���Ƶ��û��� Windows Ŀ¼
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip1
Copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip1

Rem
Rem �� Custom.dic �ļ����Ƶ��û���Ŀ¼
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip2
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip2
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip2

