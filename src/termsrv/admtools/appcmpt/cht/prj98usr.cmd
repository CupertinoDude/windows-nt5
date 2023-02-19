
@echo off

Rem #########################################################################
Rem �]�w�ϥΪ̽d���Φۭq�ؿ����|
Rem 
SET UserTemplatesPath=#USERTEMPLATESPATH#
SET UserCustomDicPath=#USERCUSTOMDICPATH#
SET UserCustomDicFile=%UserCustomDicPath%\#CUSTOMDICNAME#
SET UserAppTemplatesPath=%UserTemplatesPath%\#APPPATHNAME#
SET UserAppWebTemplatesPath=%UserTemplatesPath%\#APPWEBPATHNAME#

Rem #########################################################################
Rem �]�w���νd���Φۭq�ؿ����|
Rem 
SET CommonTemplatesPath=#COMMONTEMPLATESPATH#
SET CommonCustomDicPath=#COMMONCUSTOMDICPATH#
SET CommonCustomDicFile=%CommonCustomDicPath%\#CUSTOMDICNAME#
SET CommonAppTemplatesPath=%CommonTemplatesPath%\#APPPATHNAME#
SET CommonAppWebTemplatesPath=%CommonTemplatesPath%\#APPWEBPATHNAME#

Rem #########################################################################
Rem �b�ϥΪ̥D�ؿ����إ����ε{�����w��ƥؿ��C
Rem

call TsMkUDir "%UserTemplatesPath%"
call TsMkUDir "%UserCustomDicPath%"
call TsMkUDir "%RootDrive%\%MY_DOCUMENTS%"

Rem #########################################################################

Rem
Rem �N Custom.dic �ɮ׽ƻs��ϥΪ̥ؿ��C
Rem

If Exist "%UserCustomDicFile%" Goto Skip1
If Not Exist "%CommonCustomDicFile%" Goto Skip1
Copy "%CommonCustomDicFile%" "%UserCustomDicFile%" >Nul: 2>&1
:Skip1


Rem #########################################################################

Rem
Rem �N�d���ƻs��ϥΪ̽d���ϰ�C
Rem

If Exist "%UserAppTemplatesPath%"  Goto Skip2
If Not Exist "%CommonAppTemplatesPath%"  Goto Skip2
xcopy "%CommonAppTemplatesPath%" "%UserAppTemplatesPath%" /E /I >Nul: 2>&1
:Skip2

if Exist "%UserAppWebTemplatesPath%" Goto Skip3
if Not Exist "%CommonAppWebTemplatesPath%" Goto Skip3
xcopy "%CommonAppWebTemplatesPath%" "%UserAppWebTemplatesPath%"  /E /I >Nul: 2>&1
:Skip3
