@Echo off

Rem
Rem Corel Office 8 �Ѱ��w�˫��O��

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Cofc8Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo �w�Ѱ� Corel WordPerfect Suite 8 �n�J���O�ɪ��w�ˡC
