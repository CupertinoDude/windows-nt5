@echo off


Rem #########################################################################

Rem
Rem ����� Ȩ ���͸��� ���� ���α׷� ������ �����͸� ����
Rem ���͸��� ����ϴ�.
Rem

call TsMkUDir "%RootDrive%\Office95"

Rem #########################################################################

Rem �� ��ũ��Ʈ�� Alpha���� ����ǰ� �ִ��� Ȯ���մϴ�. �׷��ٸ� ������ ���� ������ �����մϴ�.
rem 
rem
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1


Rem #########################################################################

Rem
Rem Custom.dic ������ ����� ���͸��� ����ϴ�.
Rem

If Not Exist "%RootDrive%\Office95\Custom.Dic" Copy Nul: "%RootDrive%\Office95\Custom.Dic" >Nul: 2>&1

Rem #########################################################################

REM
REM ���͸� �̸� �� ��θ� ����ϴ�.
REM 


..\ACRegL "%Temp%\Proj95_1.Cmd" DictPath "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" "StripChar\1"
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_1.Cmd 
Del %Temp%\Proj95_1.Cmd >Nul: 2>&1

..\ACRegL "%Temp%\Proj95_3.Cmd" DictName "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" "StripPath"
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_3.Cmd 
Del %Temp%\Proj95_3.Cmd >Nul: 2>&1

Rem #########################################################################

REM
REM ��ο� ���� ������Ʈ�� ������ ����Ǿ����� ������� �����մϴ�.
REM �̰��� �ٸ� MS Office ���� ���α׷��� ��ġ�Ǿ����� �߻��� �� �ֽ��ϴ�.
REM

If "%DictPath%"=="%RootDrive%\Office95" Goto GetDictionary

..\ACRegL "%Temp%\Proj95_2.Cmd" Dictionary "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" ""
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_2.Cmd 
Del %Temp%\Proj95_2.Cmd >Nul: 2>&1

REM �� �ڵ带 ���� ���� �� ����ڿ� ���� ��ȣ�մϴ�.
REM �̰��� A ����ڰ�, DictPath�� B ����ڰ� �˻��� �Ŀ� 
REM �������� ���ϰ� �մϴ�.
..\ACRegL "%Temp%\Proj95_7.Cmd" OrigDictPath "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" "StripChar\1"
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_7.Cmd 
Del %Temp%\Proj95_7.Cmd >Nul: 2>&1
if "%OrigDictPath%"=="%RootDrive%\Office95" Goto GetDictionary

..\acsr "#DICTNAME#" "%DictName%" Template\prj95Usr.key %Temp%\Proj95_4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" %Temp%\Proj95_4.tmp  %Temp%\Proj95_5.tmp
..\acsr "#DICTIONARY#" "%Dictionary%" %Temp%\Proj95_5.tmp %Temp%\Proj95_6.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" %Temp%\Proj95_6.tmp %Temp%\Prj95Usr.Key

Rem ������Ʈ�� Ű�� �����Ͽ� ���� ��ΰ� ����� ������ ���͸��� ����Ű���� �մϴ�.
regini %Temp%\prj95Usr.key > Nul:

Del %Temp%\Proj95_4.tmp >Nul: 2>&1
Del %Temp%\Proj95_5.tmp >Nul: 2>&1
Del %Temp%\Proj95_6.tmp >Nul: 2>&1
Del %Temp%\prj95Usr.key >Nul: 2>&1

goto CopyDictionary


Rem #########################################################################

REM
REM ���� ��ΰ� ������� �ʾ����� ������Ʈ������ ���� �̸� ��
REM ��θ� �����ɴϴ�.
REM 

:GetDictionary

..\ACRegL "%Temp%\Proj95_6.Cmd" Dictionary "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "OrigDictionary" ""
If ErrorLevel 1 Goto SpellError

Call %Temp%\Proj95_6.Cmd 
Del %Temp%\Proj95_6.Cmd >Nul: 2>&1

Rem #########################################################################

REM
REM ������ ����� ���͸��� �����մϴ�.
REM 

:CopyDictionary

If Exist "%RootDrive%\Office95\%DictName%" goto Cont1
   If Not Exist "%Dictionary%"  goto Cont1
      Copy "%Dictionary%"  "%RootDrive%\Office95\%DictName%" >Nul: 2>&1

:Cont1


:Done
