@Echo Off

Copy %SystemRoot%\System32\UsrLogn2.Cmd %SystemRoot%\System32\UsrLogn2.Bak >Nul: 2>&1
FindStr /VI Exl97Usr %SystemRoot%\System32\UsrLogn2.Bak > %SystemRoot%\System32\UsrLogn2.Cmd

Echo Microsoft Excel 97 �α׿� ��ũ��Ʈ ��ġ�� �����߽��ϴ�.

