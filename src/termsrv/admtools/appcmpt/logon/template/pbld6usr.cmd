Rem
Rem Copy the demo database to the user's directory
Rem

If Exist "%RootDrive%\psDemoDB.db" Goto Skip1
copy "#INSTDIR#\psdemodb.db" "%RootDrive%" /y >Nul: 2>&1
:Skip1


