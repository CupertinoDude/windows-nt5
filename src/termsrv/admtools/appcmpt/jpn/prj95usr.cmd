@echo off


Rem #########################################################################

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office95"

Rem #########################################################################

Rem ���̃X�N���v�g�� Alpha ��Ŏ��s���ł��邩�m�F���āAAlpha ��Ŏ��s����Ă���ꍇ�A�K�؂ȋ��L�c�[����ݒ肵�܂��B
rem 
rem
Set __SharedTools=Shared Tools
If Not "%PROCESSOR_ARCHITECTURE%"=="ALPHA" goto acsrCont1
If Not Exist "%ProgramFiles(x86)%" goto acsrCont1
Set __SharedTools=Shared Tools (x86)
:acsrCont1


Rem #########################################################################

Rem
Rem ���[�U�[�̃f�B���N�g���� Custom.dic �t�@�C�����쐬���܂��B
Rem

If Not Exist "%RootDrive%\Office95\Custom.Dic" Copy Nul: "%RootDrive%\Office95\Custom.Dic" >Nul: 2>&1

Rem #########################################################################

REM
REM �����̖��O�ƃp�X���擾���܂��B
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
REM �p�X�̃��W�X�g���ݒ肪�ύX����Ă���ꍇ�A���Z�b�g���܂��B
REM ����́A�ʂ� Microsoft Office �A�v���P�[�V�������C���X�g�[������Ă���ꍇ�ɔ������邱�Ƃ�����܂��B
REM

If "%DictPath%"=="%RootDrive%\Office95" Goto GetDictionary

..\ACRegL "%Temp%\Proj95_2.Cmd" Dictionary "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" ""
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_2.Cmd 
Del %Temp%\Proj95_2.Cmd >Nul: 2>&1

REM 2 �l�̃��[�U�[�����̃R�[�h��ʉ߂���̂�h���܂��B
REM ����ɂ�胆�[�U�[ A �� DictPath ���擾��ɁA���[�U�[ B ��
REM DictPath �ύX����̂�h���܂��B
..\ACRegL "%Temp%\Proj95_7.Cmd" OrigDictPath "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "Dictionary" "StripChar\1"
If ErrorLevel 1 Goto Done
Call %Temp%\Proj95_7.Cmd 
Del %Temp%\Proj95_7.Cmd >Nul: 2>&1
if "%OrigDictPath%"=="%RootDrive%\Office95" Goto GetDictionary

..\acsr "#DICTNAME#" "%DictName%" Template\prj95Usr.key %Temp%\Proj95_4.tmp
..\acsr "#ROOTDRIVE#" "%RootDrive%" %Temp%\Proj95_4.tmp  %Temp%\Proj95_5.tmp
..\acsr "#DICTIONARY#" "%Dictionary%" %Temp%\Proj95_5.tmp %Temp%\Proj95_6.tmp
..\acsr "#__SharedTools#" "%__SharedTools%" %Temp%\Proj95_6.tmp %Temp%\Prj95Usr.Key

Rem ���W�X�g�� �L�[��ύX���āA�����̃p�X�����[�U�[�ŗL�̃f�B���N�g�����w���悤�ɂ��܂��B
regini %Temp%\prj95Usr.key > Nul:

Del %Temp%\Proj95_4.tmp >Nul: 2>&1
Del %Temp%\Proj95_5.tmp >Nul: 2>&1
Del %Temp%\Proj95_6.tmp >Nul: 2>&1
Del %Temp%\prj95Usr.key >Nul: 2>&1

goto CopyDictionary


Rem #########################################################################

REM
REM �����̃p�X���ύX����Ă��Ȃ��ꍇ�A���W�X�g�����Q�Ƃ��āA����
REM ���O�ƃp�X���g�p���Ă��������B
REM 

:GetDictionary

..\ACRegL "%Temp%\Proj95_6.Cmd" Dictionary "HKLM\Software\Microsoft\%__SharedTools%\Proofing Tools\Spelling\1033\Normal" "OrigDictionary" ""
If ErrorLevel 1 Goto SpellError

Call %Temp%\Proj95_6.Cmd 
Del %Temp%\Proj95_6.Cmd >Nul: 2>&1

Rem #########################################################################

REM
REM ���������[�U�[ �f�B���N�g���ɃR�s�[���܂��B
REM 

:CopyDictionary

If Exist "%RootDrive%\Office95\%DictName%" goto Cont1
   If Not Exist "%Dictionary%"  goto Cont1
      Copy "%Dictionary%"  "%RootDrive%\Office95\%DictName%" >Nul: 2>&1

:Cont1


:Done
