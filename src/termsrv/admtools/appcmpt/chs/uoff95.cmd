@Echo Off

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Ofc95Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo ��ж�� Microsoft Office 95 ��¼�ű���

