
@Echo Off

Rem
Rem ���û�����Ŀ¼��ΪӦ�ó������������
Rem ����Ŀ¼��
Rem


#ifndef JAPAN
If Exist "%RootDrive%\Personal\iBases\Personal" goto cont1
call TsMkUDir "%RootDrive%\Personal\iBases\Personal"
copy "#COFFICE7INST#ICWin7\Local\iBases\Personal.*" "%RootDrive%\Personal\iBases\Personal\."   >Nul: 2>&1
:cont1
#endif


call TsMkUDir "%RootDrive%\COffice7\Backup"
call TsMkUDir "%RootDrive%\Personal"




