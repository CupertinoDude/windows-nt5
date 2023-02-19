
@Echo Off

Rem
Rem ����:  �� ��ũ��Ʈ ���� CACLS ��ɾ�� NTFS�� ���˵� ��Ƽ�ǿ�����
Rem ���� �� �ֽ��ϴ�.
Rem

Rem #########################################################################


Rem #########################################################################

Rem
Rem %RootDrive%�� �����Ǿ����� Ȯ���ϰ� �� ��ũ��Ʈ�� ���� �����մϴ�.
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done
Call "%SystemRoot%\Application Compatibility Scripts\SetPaths.Cmd"
If "%_SETPATHS%" == "FAIL" Goto Done

Rem #########################################################################



Rem
Rem ���� ����� ���ø����� ��� ����� ���ø� ��ġ�� ������ �����մϴ�.
Rem

If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\WINWORD8.DOC" copy "%UserProfile%\%TEMPLATES%\WINWORD8.DOC" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\EXCEL8.XLS" copy "%UserProfile%\%TEMPLATES%\EXCEL8.XLS" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1
If Not Exist "%ALLUSERSPROFILE%\%TEMPLATES%\BINDER.OBD" copy "%UserProfile%\%TEMPLATES%\BINDER.OBD" "%ALLUSERSPROFILE%\%TEMPLATES%\" /Y >Nul: 2>&1



Rem
Rem ������Ʈ������ Office 97 ��ġ ��ġ�� ����ϴ�. ������ ������,
Rem Office�� ��ġ�Ǿ� ���� �ʴ� ������ �����ϰ� ���� �޽����� ǥ���մϴ�.
Rem

..\ACRegL %Temp%\O97.Cmd O97INST "HKLM\Software\Microsoft\Office\8.0\Common\InstallRoot" "" ""
If Not ErrorLevel 1 Goto Cont0
Echo.
Echo ������Ʈ������ Office 97 ��ġ ��ġ�� �˻����� ���߽��ϴ�.
Echo Office 97�� ��ġ�Ǿ� �ִ��� Ȯ���ϰ� �� ��ũ��Ʈ��
Echo �ٽ� �����Ͻʽÿ�.
Echo.
Pause
Goto Done

:Cont0
Call %Temp%\O97.Cmd
Del %Temp%\O97.Cmd >Nul: 2>&1

Rem #########################################################################

Rem
Rem Access 97 �ý��� �����ͺ��̽��� �б� �������� �����մϴ�. �̷��� �ϸ�
Rem ���� ����� ���ÿ� �����ͺ��̽��� ������ �� ������ [����/����] �޴�
Rem �ɼ��� ����Ͽ� �Ϲ������� �� �� �ִ� �ý��� �����ͺ��̽� ������Ʈ��
Rem �� �� ���� �˴ϴ�. ����ڸ� �߰��� �ʿ䰡 ������ ���� �ý��� ������
Rem ���̽��� ���� �׼����� �ο��ؾ� 
Rem �մϴ�.
Rem


















If Not Exist %SystemRoot%\System32\System.Mdw Goto Cont1
cacls %SystemRoot%\System32\System.Mdw /E /P "Authenticated Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P "Power Users":R >NUL: 2>&1
cacls %SystemRoot%\System32\System.Mdw /E /P Administrators:R >NUL: 2>&1

:Cont1

Rem #########################################################################

Rem
Rem Office 97�� ������Ʈ�ϴ� �ý��� DLL����
Rem ��ο� ���� �б� �׼����� ����մϴ�.
Rem

REM If Exist %SystemRoot%\System32\OleAut32.Dll cacls %SystemRoot%\System32\OleAut32.Dll /E /T /G "Authenticated Users":R >NUL: 2>&1

Rem #########################################################################

Rem #########################################################################

Rem
Rem outlook�� ���� frmcache.dat ���Ͽ��� �͹̳� ���� ����ڿ� ���� ���� �׼����� ����մϴ�.
Rem

If Exist %SystemRoot%\Forms\frmcache.dat cacls %SystemRoot%\forms\frmcache.dat /E /G "Terminal Server User":C >NUL: 2>&1

Rem #########################################################################


Rem
Rem Powerpoint �����縦 �б� �������� �����Ͽ� ���ÿ� �ϳ� �̻���
Rem �����縦 �ҷ� �� �� �ְ� �մϴ�.
Rem




If Exist "%O97INST%\Templates\Presentations\AutoContent Wizard.Pwz" Attrib +R "%O97INST%\Templates\Presentations\AutoContent Wizard.Pwz" >Nul: 2>&1

If Exist "%O97INST%\Office\Ppt2html.ppa" Attrib +R "%O97INST%\Office\Ppt2html.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\bshppt97.ppa" Attrib +R "%O97INST%\Office\bshppt97.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\geniwiz.ppa" Attrib +R "%O97INST%\Office\geniwiz.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\ppttools.ppa" Attrib +R "%O97INST%\Office\ppttools.ppa" >Nul: 2>&1
If Exist "%O97INST%\Office\graphdrs.ppa" Attrib +R "%O97INST%\Office\graphdrs.ppa" >Nul: 2>&1

Rem #########################################################################

Rem
Rem �����ڰ� �ƴ� ����ڰ� Access ������ �Ǵ� Excel�� Access �߰� �����
Rem ������ �� �ְ� �Ϸ��� ���� �� �ٿ��� "Rem"�� �����Ͽ�
Rem ����ڿ��� ������ ���Ͽ� ���� ���� �׼����� �ο��Ͻʽÿ�.
Rem 
Rem

Rem If Exist "%O97INST%\Office\WZLIB80.MDE" cacls "%O97INST%\Office\WZLIB80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZMAIN80.MDE" cacls "%O97INST%\Office\WZMAIN80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 
Rem If Exist "%O97INST%\Office\WZTOOL80.MDE" cacls "%O97INST%\Office\WZTOOL80.MDE" /E /P "Authenticated Users":C >NUL: 2>&1 

Rem #########################################################################

Rem
Rem Powerpoint �� Excel �߰� ��ɿ� �ʿ��� MsForms.Twd �� RefEdit.Twd ����
Rem (����/HTML�� ����, ��)�� ����ϴ�. �� �� �ϳ��� ���α׷���
Rem ����Ǹ� �� ���α׷��� �� ������ �ʿ��� �����Ͱ� ��� �ִ�
Rem ���Ϸ� �ٲߴϴ�.
Rem

If Exist %systemroot%\system32\MsForms.Twd Goto Cont2
Copy Nul: %systemroot%\system32\MsForms.Twd >Nul: 2>&1
Cacls %systemroot%\system32\MsForms.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont2

If Exist %systemroot%\system32\RefEdit.Twd Goto Cont3
Copy Nul: %systemroot%\system32\RefEdit.Twd >Nul: 2>&1
Cacls %systemroot%\system32\RefEdit.Twd /E /P "Authenticated Users":F >Nul: 2>&1
:Cont3

Rem #########################################################################

Rem
Rem SystemRoot �Ʒ��� msremote.sfs�� ����ϴ�. �̷��� �ϸ�
Rem �������� ���� �� �������� "���� �� �ѽ�"�� ����� �� �ְ� �մϴ�.
Rem

md %systemroot%\msremote.sfs > Nul: 2>&1

Rem #########################################################################

Rem
Rem ��� ����ڿ� ���� [����] �޴����� [���� ã��]�� �����մϴ�.
Rem [���� ã��]�� ���ҽ��� ���� �ʿ�� �ϸ� �ý��� ���ɿ� ������
Rem �ݴϴ�.
Rem

If Exist "%COMMON_STARTUP%\Microsoft Find Fast.lnk" Del "%COMMON_STARTUP%\Microsoft Find Fast.lnk" >Nul: 2>&1

Rem #########################################################################

Rem
Rem ��� ����ڿ� ���� [����] �޴����� "Microsoft Office �ٷ� ���� ����.lnk" ������ �����մϴ�.
Rem

If Exist "%COMMON_STARTUP%\Microsoft Office Shortcut Bar.lnk" Del "%COMMON_STARTUP%\Microsoft Office Shortcut Bar.lnk" >Nul: 2>&1

Rem #########################################################################
Rem
Rem SystemRoot �Ʒ��� msfslog.txt ������ ����� Terminal Server Users����
Rem �� ���Ͽ� ���� ��� ������ �ݴϴ�.
Rem

If Exist %systemroot%\MSFSLOG.TXT Goto MsfsACLS
Copy Nul: %systemroot%\MSFSLOG.TXT >Nul: 2>&1
:MsfsACLS
Cacls %systemroot%\MSFSLOG.TXT /E /P "Terminal Server User":F >Nul: 2>&1


Rem #########################################################################

Rem
Rem ������Ʈ�� Ű�� �����Ͽ� ��ΰ� ����� ������
Rem ���͸��� ����Ű���� �մϴ�.
Rem

Rem ���� ��ġ ��忡 ���� ������ ��ġ ���� �����մϴ�.
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1
..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\Office97.Key Office97.Tmp
..\acsr "#__SharedTools#" "%__SharedTools%" Office97.Tmp Office97.Tmp2
..\acsr "#INSTDIR#" "%O97INST%" Office97.Tmp2 Office97.Tmp3
..\acsr "#MY_DOCUMENTS#" "%MY_DOCUMENTS%" Office97.Tmp3 Office97.Key
Del Office97.Tmp >Nul: 2>&1
Del Office97.Tmp2 >Nul: 2>&1
Del Office97.Tmp3 >Nul: 2>&1

regini Office97.key > Nul:

Rem ���� ��尡 ���� ��忴����, ���� ���� �ٽ� �����մϴ�.
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem #########################################################################

Rem
Rem Ofc97Usr.Cmd�� ������Ʈ�Ͽ� ���� ��ġ ���͸��� �ݿ��ϰ� �̸�
Rem UsrLogn2.Cmd ��ũ��Ʈ�� �߰��մϴ�.
Rem

..\acsr "#INSTDIR#" "%O97INST%" ..\Logon\Template\Ofc97Usr.Cmd ..\Logon\Ofc97Usr.Cmd

FindStr /I Ofc97Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Skip1
Echo Call Ofc97Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Skip1

Echo.
Echo   Office 97�� �ùٸ��� �۾��ϱ� ����
Echo   ���� �α׿µǾ� �ִ� ����ڰ� �α׿����ϰ� �ٽ� �α׿��� ��
Echo   Office 97 ���� ���α׷��� �����ؾ� �մϴ�.
Echo.
Echo Microsoft Office 97 ���� ����� ���� ���α׷� ���� �Ϸ�
Pause

:done

















































































































































































































































