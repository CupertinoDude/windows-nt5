@Echo Off

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Com40Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo �w�Ѱ� Netscape Communicator 4 �n�J���O�ɪ��w�ˡC

