
Rem
Rem �b�ϥΪ̥D�ؿ����إ����ε{�����w��ƪ��ؿ��C
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Startup"
call TsMkUDir "%RootDrive%\Office97\Templates"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem
Rem �N�U�C�ɮױq�Ҧ��ϥΪ̽d�����ƻs��ثe�ϥΪ̽d���Ҧb��m�C
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" copy "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" "%UserProfile%\%Templates%\" /Y >Nul: 2>&1

Rem
Rem �N ARTGALRY.CAG �ƻs��ϥΪ� windows �ؿ��C
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip1
Copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip1

Rem
Rem �N Custom.dic �ɮ׽ƻs��ϥΪ̥ؿ��C
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip2
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip2
Copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip2

