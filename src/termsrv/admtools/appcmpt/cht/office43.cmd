@Echo Off

Cls

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Echo.
Echo.
Echo Administrator �ݭn�N Access �u�@�ؿ��ܧ��ϥΪ̪�
Echo Office �p�H�ؿ��A�~�����o�ӫ��O�ɡC
Echo.
Echo �����o�Ӿާ@��A�Ы����@�����~��C
Echo.
Echo �_�h�AAdministrator �ݭn����U�C�B�J:
Echo     �Ұ� MS Access�A�ñq [View] �\������ [Options]
Echo     �N [Default Database Directory] �ܧ� "%RootDrive%\OFFICE43"
Echo     ���� MS Access
Echo.
Echo �`�N�ƶ�: �z�ݭn�إߤ@�ӷs����Ʈw�A�~��Ѿ\ [View] �\���C
Echo.
Echo �����o�ǨB�J��A�Ы����N���~��...

Pause > NUL:

Echo.
Echo �p�G�z�N MS Office 4.3 �w�˦b "%SystemDrive%\MSOFFICE" �H�~���ؿ��A
Echo �N������s Ofc43ins.cmd�C
Echo.
Echo �Ы����N��}�l��s Ofc43ins.cmd ...
Echo.
Pause > NUL:
Notepad Ofc43ins.cmd
Pause

Call ofc43ins.cmd

..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.cmd ..\Logon\ofc43usr.cmd
..\acsr "#SYSTEMROOT#" "%SystemRoot%" ..\Logon\Template\ofc43usr.key ..\Logon\Template\ofc43usr.bak
..\acsr "#OFC43INST#" "%OFC43INST%" ..\Logon\Template\ofc43usr.bak ..\Logon\ofc43usr.key
Del /F /Q ..\Logon\Template\ofc43usr.bak

Rem
Rem  �`�N:  CACLS ���O�u��b NTFS �榡���ϺФ��Τ�����C
Rem

Rem �p�G�ثe���O�w�˼Ҧ��A�N�ܧ󦨦w�˼Ҧ��C
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

regini Office43.key > Nul:

Rem �p�G��ӬO����Ҧ��A�N�N�ܦ^����Ҧ��C
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

Rem
Rem ��s Office 4.3 �� INI �ɮ� 
Rem

Rem �b msoffice.ini ���]�w Excel �u�@�ؿ� (�p�G�q Office �u��C��
Rem �Ұ� Excel) Office Toolbar �зǳ]�w�|�N Excel ��b Word ��m
Rem ����C �p�G msoffice.ini ���s�b�A�|�N���إߦb%SystemRoot% �ؿ����C
Rem 

..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp1" "1,1,"
..\Aciniupd /e "Msoffice.ini" "ToolbarOrder" "MSApp2" "2,1,,%RootDrive%\office43"

..\Aciniupd /e "word6.ini" "Microsoft Word" USER-DOT-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "word6.ini" "Microsoft Word" WORKGROUP-DOT-PATH "%OFC43INST%\WINWORD\TEMPLATE"
..\Aciniupd /e "word6.ini" "Microsoft Word" INI-PATH "%RootDrive%\OFFICE43\WINWORD"
..\Aciniupd /e "word6.ini" "Microsoft Word" DOC-PATH "%RootDrive%\OFFICE43"
..\Aciniupd /e "word6.ini" "Microsoft Word" AUTOSAVE-PATH "%RootDrive%\OFFICE43"

..\Aciniupd /e "Excel5.ini" "Microsoft Excel" DefaultPath "%RootDrive%\OFFICE43"
..\Aciniupd /e "Excel5.ini" "Spell Checker" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

..\Aciniupd /k "Msacc20.ini" Libraries "WZTABLE.MDA" "%RootDrive%\OFFICE43\ACCESS\WZTABLE.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZLIB.MDA" "%RootDrive%\OFFICE43\ACCESS\WZLIB.MDA"
..\Aciniupd /k "Msacc20.ini" Libraries "WZBLDR.MDA" "%RootDrive%\OFFICE43\ACCESS\WZBLDR.MDA"
..\Aciniupd /e "Msacc20.ini" Options "SystemDB" "%RootDrive%\OFFICE43\ACCESS\System.MDA"

Rem
Rem Update the WIN.INI
Rem

..\Aciniupd /e "Win.ini" "MS Proofing Tools" "Custom Dict 1" "%RootDrive%\OFFICE43\Custom.dic"

Rem
Rem �إ� Artgalry ��Ƨ����ϥ��v���C
Rem

cacls "%SystemRoot%\Msapps\Artgalry" /E /G "Terminal Server User":F > NUL: 2>&1

Rem
Rem �ܧ� MSQuery ��Ƨ����ϥ��v���C
Rem

cacls "%SystemRoot%\Msapps\MSQUERY" /E /G "Terminal Server User":C > NUL: 2>&1

Rem
Rem �N Msacc20.ini �ƻs��t�κ޲z���� windows �ؿ��A�]���t�κ޲z�����ɮ׬O�ª����C
Rem

Copy "%SystemRoot%\Msacc20.ini" "%UserProfile%\Windows\" > NUL: 2>&1

Rem �إߤ@�ӵ�]�ɮסAInstaller �~���|��n�����X�i��Ǽ��C

Copy NUL: "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1
attrib +h "%UserProfile%\Windows\Ofc43usr.dmy" > NUL: 2>&1

FindStr /I Ofc43Usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto cont2
Echo Call Ofc43Usr.Cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:cont2

Echo.
Echo �t�κ޲z���������n�X�A�A���s�n�J�A�~�����ܧ�ͮġC
Echo   �n�J��A�t�κ޲z���]��������U�C�B�J�A�~���l��
Echo   Clip Art Gallery:
Echo       ���� Word �ÿ�� [���J����]�C
Echo       ��� Microsoft ClipArt Gallery�C
Echo       �� [�T�w] �ӶפJ��ܪ� clipart�C 
Echo       ���� ClipArt Gallery �� Word�C
Echo.
Echo   �t�κ޲z���i�H���C�ӨϥΪ̫��w�ߤ@�ʪ��w�]�ؿ��A�B�J�p�U:
Echo.
Echo    1) �ηƹ��k��� [�}�l] �\���C
Echo    2) �b����\������ [�s���Ҧ��ϥΪ�] �ӱҰ��ɮ��`�ޡC
Echo    3) �b�ɮ��`�ޥk�赡�椤�� [�{����] ��Ƨ��W����U�C
Echo    4) �ηƹ��k��A�b�ɮ��`�ޥk�赡�椤�� Microsoft Office �ϥܤW����U�C
Echo    5) �b�ɮ��`�ޥk�赡�椤�� Microsoft PowerPoint �ϥܤW���@�U�C
Echo    6) �b����\������ [���e]�C
Echo    7) ��� [���|] ���޼��ҡA�ñN [�Ұʦb:] ������ �ܧ�
Echo       %%RootDrive%%\My Documents�C
Echo    8) ��� [�T�w]�C
Echo.
Echo   �`�N: �C�ӨϥΪ̳����������D�ؿ��� %%RootDrive%%�C
Echo          %%RootDrive%%\My Documents �u�O�@�ӫ�ĳ�ȡC
Echo.
Pause
Echo Microsoft Office 4.3 �h�ϥΪ����ε{���վ�B�z�����C
Echo.
Pause

:Done
