@Echo Off

Rem #########################################################################

Rem
Rem ȷ���Ƿ� CMD ��չ������
Rem

if "A%cmdextversion%A" == "AA" (
  call cmd /e:on /c eudora4.cmd
) else (
  goto ExtOK
)
goto Done

:ExtOK

Rem #########################################################################

Rem
Rem ȷ���Ƿ� %RootDrive% �ѱ����ã��������ڸ�����ű�
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################

Rem ��ע������л�� Eudora �����й���

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:1" 

If Not ErrorLevel 1 Goto Cont1
Echo.
Echo �޷���ע������л�� Eudora Pro 4.0 �����й��ߡ�
Echo ȷ���Ƿ� Eudora Pro 4.0 �ѱ���װ�������������и�����ű���
Echo 
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1
set EudCmd=%EUDTMP:~0,-2%

..\ACRegL "%Temp%\EPro4.Cmd" EUDTMP "HKCU\Software\Qualcomm\Eudora\CommandLine" "Current" "STRIPCHAR:2" 

If Not ErrorLevel 1 Goto Cont2
Echo.
Echo �޷���ע������л�� Eudora Pro 4.0 �İ�װĿ¼��
Echo ȷ���Ƿ� Eudora Pro 4.0 �ѱ���װ�������������и�����ű���
Echo 
Echo.
Pause
Goto Done

:Cont2
Call %Temp%\EPro4.Cmd
Del %Temp%\EPro4.Cmd >Nul: 2>&1

Set EudoraInstDir=%EUDTMP:~0,-13%

Rem #########################################################################

If Exist "%EudoraInstDir%\descmap.pce" Goto Cont0
Echo.
Echo �ڼ���ִ�����Ӧ�ó������������ű�֮ǰ������������һ�� Eudora 4.0��
Echo ������ Eudora ֮�󣬸��� Eudora Pro �ļ��� Eudora Pro ��ݷ�ʽ��Ŀ�����ԡ�
Echo �� %RootDrive%\eudora.ini ���ӵ�Ŀ��֮��
Echo ��Ϊ:
Echo  "%EudoraInstDir%\Eudora.exe" %RootDrive%\eudora.ini
Echo.
Pause
Goto Done

:Cont0

Rem
Rem ����ע����·��ָ���û��ض�
Rem Ŀ¼��
Rem

Rem �����ǰ�����ڰ�װģʽ���ı䵽��װģʽ��
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

..\acsr "#INSTDIR#" "%EudoraInstDir%" Template\Eudora4.Key Eudora4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" Eudora4.tmp Eudora4.key

regini eudora4.key > Nul:
del eudora4.tmp
del eudora4.key

Rem ���ԭ����ִ��ģʽ���ı��ִ��ģʽ��
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem ���� descmap.pce �Ϸ���Ȩ�ޡ�
cacls "%EudoraInstDir%\descmap.pce" /E /G "Terminal Server User":R >NUL: 2>&1

Rem #########################################################################

Echo.
Echo   Ϊ�˱�֤ Eudora Pro 4.0 ����ȷ��������ǰ��¼�û�
Echo   ����ע���Լ����µ�¼��Ȼ������ Eudora Pro 4.0 ��
Echo.
Echo Eudora 4.0 ���û�Ӧ�ó��������ϡ�
Pause

:done
