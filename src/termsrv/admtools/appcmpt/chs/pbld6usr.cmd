Rem
Rem ������ʾ���ݿ⵽�û�Ŀ¼
Rem

If Exist "%RootDrive%\psDemoDB.db" Goto Skip1
copy "#INSTDIR#\psdemodb.db" "%RootDrive%" /y >Nul: 2>&1
:Skip1


