cl -P -nologo -Ii386\ -I. -I..\include -I.\help -I..\..\vb98\inc -I..\..\devbin\htmlhelp\v6\idl\include -ID:\nt\public\oak\inc -ID:\nt\public\sdk\inc -ID:\nt\private\inc -ID:\nt\public\sdk\inc\crt -D_X86_=1 -Di386=1   -DCONDITION_HANDLING=1 -DNT_UP=1  -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DWINNT=1 -D_WIN32_WINNT=0x0400 -DWINVER=0x0400 -D_WIN32_IE=0x0400    -DWIN32_LEAN_AND_MEAN=1 -DDBG=1 -DDEVL=1 -DFPO=0    -D_DEBUG -D_DLL=1 -D_MT=1 -D_MT     /c /Zel /Zp8 /Gy  -W3 -WX /Gd  /QIfdiv- /QIf /QI0f /GB /Gi- /Gm- /GX  /GR- /GF /Zi -Od /Oy- /FdD:\mmc.vb\vb98ctls\mssnapd\obj\i386\  -FI..\include\vc5warn.h -FID:\nt\public\sdk\inc\warning.h /Yupch.h /Fpobj\i386\pch.pch .\dll.cpp