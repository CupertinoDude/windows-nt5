Rem
Rem �N�ܽd��Ʈw�ƻs��ϥΪ̥ؿ��C
Rem

If Exist "%RootDrive%\psDemoDB.db" Goto Skip1
copy "#INSTDIR#\psdemodb.db" "%RootDrive%" /y >Nul: 2>&1
:Skip1


