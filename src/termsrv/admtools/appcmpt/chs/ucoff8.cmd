@Echo off

Rem
Rem Corel Office 8 ж�ؽű�

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Cofc8Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo ��ж�� Corel WordPerfect Suite 8 ��¼�ű���
