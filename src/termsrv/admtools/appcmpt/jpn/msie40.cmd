@Echo off

Rem
Rem �C���X�g�[���̒���ɊǗ��҂����̃X�N���v�g�����s����K�v������܂��B
Rem

CD /D "%SystemRoot%\Application Compatibility Scripts\Install" > NUL: 2>&1

If Not "A%1A" == "AA" Goto cont0

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

:cont0

If "A%1A" == "AA" ..\acsr "#ROOTDRIVE#" "%RootDrive%" Template\msie40.key msie40.key
..\acsr "#SYSTEMROOT#" "%SystemRoot%" Template\msie40s.key msie40s.key

If Exist "%SystemRoot%\Setup.ini" Goto Cont2
Copy NUL: "%SystemRoot%\Setup.ini" > NUL: 2>&1

:Cont2
Cacls "%SystemRoot%\Setup.ini" /E /T /G "Authenticated Users":F >Nul: 2>&1

If Exist "%SystemRoot%\Setup.old" Goto Cont3
Copy NUL: "%SystemRoot%\Setup.old" > NUL: 2>&1

:Cont3
Attrib +r "%SystemRoot%\Setup.old" > NUL: 2>&1

FindStr /I Msie4Usr %SystemRoot%\System32\UsrLogn1.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Cont4
Echo Call Msie4Usr.Cmd >> %SystemRoot%\System32\UsrLogn1.Cmd

:Cont4

Rem ���݁A�C���X�g�[�� ���[�h�łȂ��ꍇ�A�C���X�g�[�� ���[�h�ɕύX���܂��B
Set __OrigMode=Install
ChgUsr /query > Nul:
if Not ErrorLevel 101 Goto Begin
Set __OrigMode=Exec
Change User /Install > Nul:
:Begin

If "A%1A" == "AA" regini Msie40.key > Nul:
regini Msie40s.key > NUL:

Rem ���̃��[�h�����s���[�h�������ꍇ�A���s���[�h�ɖ߂��܂��B
If "%__OrigMode%" == "Exec" Change User /Execute > Nul:
Set __OrigMode=

If Not "A%1A" == "AA" Goto Done

Echo.
Echo   Microsoft Internet Explorer 4 ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă���
Echo   ���[�U�[�̓A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t����
Echo   ����ēx���O�I������K�v������܂��B
Echo.
Echo Microsoft Internet Explorer 4.x �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B

Rem Internet Explorer �̎����C���X�g�[�����͈ꎞ��~���Ȃ��ł��������B
Pause

:Done
