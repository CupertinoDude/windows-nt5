@Echo Off

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Ofc97Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo �w�Ѱ� Microsoft Office 97 �n�J���O�ɪ��w�ˡC

