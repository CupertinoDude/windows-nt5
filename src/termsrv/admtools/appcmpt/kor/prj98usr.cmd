
@echo off

Rem #########################################################################
Rem ����� ���ø� �� ����� ���� ���� ��θ� �����մϴ�.
Rem 
SET UserTemplatesPath=#USERTEMPLATESPATH#
SET UserCustomDicPath=#USERCUSTOMDICPATH#
SET UserCustomDicFile=%UserCustomDicPath%\#CUSTOMDICNAME#
SET UserAppTemplatesPath=%UserTemplatesPath%\#APPPATHNAME#
SET UserAppWebTemplatesPath=%UserTemplatesPath%\#APPWEBPATHNAME#

Rem #########################################################################
Rem ���� ���ø� �� ����� ���� ���� ��θ� �����մϴ�.
Rem 
SET CommonTemplatesPath=#COMMONTEMPLATESPATH#
SET CommonCustomDicPath=#COMMONCUSTOMDICPATH#
SET CommonCustomDicFile=%CommonCustomDicPath%\#CUSTOMDICNAME#
SET CommonAppTemplatesPath=%CommonTemplatesPath%\#APPPATHNAME#
SET CommonAppWebTemplatesPath=%CommonTemplatesPath%\#APPWEBPATHNAME#

Rem #########################################################################
Rem ����� Ȩ ���͸��� ���� ���α׷� ������ �����͸� ����
Rem ���͸��� ����ϴ�.
Rem

call TsMkUDir "%UserTemplatesPath%"
call TsMkUDir "%UserCustomDicPath%"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem #########################################################################

Rem
Rem Custom.dic ������ ����� ���͸��� �����մϴ�.
Rem

If Exist "%UserCustomDicFile%" Goto Skip1
If Not Exist "%CommonCustomDicFile%" Goto Skip1
Copy "%CommonCustomDicFile%" "%UserCustomDicFile%" >Nul: 2>&1
:Skip1


Rem #########################################################################

Rem
Rem ���ø��� ����� ���ø� �������� �����մϴ�.
Rem

If Exist "%UserAppTemplatesPath%"  Goto Skip2
If Not Exist "%CommonAppTemplatesPath%"  Goto Skip2
xcopy "%CommonAppTemplatesPath%" "%UserAppTemplatesPath%" /E /I >Nul: 2>&1
:Skip2

if Exist "%UserAppWebTemplatesPath%" Goto Skip3
if Not Exist "%CommonAppWebTemplatesPath%" Goto Skip3
xcopy "%CommonAppWebTemplatesPath%" "%UserAppWebTemplatesPath%"  /E /I >Nul: 2>&1
:Skip3
