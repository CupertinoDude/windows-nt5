
Rem
Rem �إߥؿ��A�Ψө�m�ϥΪ̥D�ؿ��������w����
Rem �{����ơC
Rem

call TsMkUDir "%RootDrive%\Office97"
call TsMkUDir "%RootDrive%\Office97\Templates"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"
call TsMkUDir "%RootDrive%\Office97\XLStart"

Rem
Rem �N�o���ɮױq�Ҧ��ϥΪ̽d�����ƻs��ثe�ϥΪ̽d���Ҧb��m�C
Rem

If Not Exist "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" copy "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" "%UserProfile%\%TEMPLATES%\" /Y >Nul: 2>&1

Rem
Rem �N ARTGALRY.CAG �ƻs��ϥΪ̪� windows �ؿ�
Rem

If Exist "%RootDrive%\Windows\ArtGalry.Cag" Goto Skip1
If Not Exist "%SystemRoot%\ArtGalry.Cag" Goto Skip1
copy "%SystemRoot%\ArtGalry.Cag" "%RootDrive%\Windows\ArtGalry.Cag" >Nul: 2>&1
:Skip1

Rem
Rem �N Custom.dic �ɮ׽ƻs��ϥΪ̥ؿ�
Rem

If Exist "%RootDrive%\Office97\Custom.Dic" Goto Skip2
If Not Exist "#INSTDIR#\Office\Custom.Dic" Goto Skip2
copy "#INSTDIR#\Office\Custom.Dic" "%RootDrive%\Office97\Custom.Dic" >Nul: 2>&1
:Skip2

