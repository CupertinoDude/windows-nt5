@Echo Off

Copy %SystemRoot%\System32\UsrLogn1.Cmd %SystemRoot%\System32\UsrLogn1.Bak >Nul: 2>&1
FindStr /VI Msie4usr %SystemRoot%\System32\UsrLogn1.Bak > %SystemRoot%\System32\UsrLogn1.Cmd

Echo Microsoft Internet Explorer �α׿� ��ũ��Ʈ ��ġ�� �����߽��ϴ�.

