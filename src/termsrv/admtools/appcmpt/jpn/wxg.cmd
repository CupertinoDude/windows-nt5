@echo off

Rem #########################################################################
Rem
Rem %RootDrive% ���\������Ă��̃X�N���v�g�p�ɐݒ肳��Ă��邱�Ƃ��m�F���܂��B
Rem

Call "%SystemRoot%\Application Compatibility Scripts\ChkRoot.Cmd"
If "%_CHKROOT%" == "FAIL" Goto Done

Rem #########################################################################
Rem
Rem WXG30.Cmd �����łɎ��s����Ă���ꍇ�͏����𒆒f���܂��B
Rem

FindStr /I wxg30usr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If ErrorLevel 1 Goto Cont0

Echo.
Echo   ���ł� WXG30.Cmd �����s����Ă��܂��B
Echo.   
Echo WXG �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪
Echo ���f����܂����B
Echo.
Pause
Goto Done

:Cont0

Rem #########################################################################
Rem
Rem �����t�@�C���̃C���X�g�[������Ă���p�X���擾���܂��B
Rem

..\ACRegL %Temp%\wxg.Cmd DIC_Path "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\WXGIME.IME" "dicdir" ""
If Not ErrorLevel 1 Goto Cont1

Echo.
Echo ���W�X�g������ WXG �̃C���X�g�[������Ă���p�X���擾�ł��܂���ł����B
Echo WXG ���C���X�g�[������Ă��邱�Ƃ��m�F���Ă��������B
Echo.
Pause
Goto Done

:Cont1
Call %Temp%\wxg.Cmd
Del %Temp%\wxg.Cmd >Nul: 2>&1

Rem #########################################################################
Rem 
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

if not exist %RootDrive%\WXG mkdir %RootDrive%\WXG

Rem #########################################################################
Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���Ƀ��[�U�[�����t�@�C�����R�s�[���܂��B
Rem

if exist "%DIC_Path%\wxgu.gdj" copy "%DIC_Path%\wxgu.gdj" %RootDrive%\WXG\wxgu.gdj> Nul:
if exist "%DIC_Path%\wxgurev.gdj" copy "%DIC_Path%\wxgurev.gdj" %RootDrive%\WXG\wxgurev.gdj> Nul:

Rem #########################################################################
Rem
Rem WxgUsr.Cmd �� UsrLogn2.Cmd �X�N���v�g�ɒǉ����܂��B
Rem

FindStr /I wxgusr %SystemRoot%\System32\UsrLogn2.Cmd >Nul: 2>&1
If Not ErrorLevel 1 Goto Cont2
Echo Call wxgusr.cmd >> %SystemRoot%\System32\UsrLogn2.Cmd
:Cont2

Rem #########################################################################
cls
Echo.
Echo �����ŁA�ȉ��̋L�q�ɂ��������āA�ݒ�𑱂��Ă��������B
Echo ���ׂĂ̐ݒ肪�I���܂ŁA���̃��[�U�����O�I�����Ȃ��悤�ɂ��Ă��������B
Echo.
Echo WXG �̃v���p�e�B�ݒ�̃_�C�A���O �{�b�N�X��\�����Ă��������B
Echo (�R���g���[�� �p�l���� [�L�[�{�[�h] ���_�u���N���b�N���A
Echo  [���̓��P�[��] �^�u���N���b�N���܂��B���ɁA�C���X�g�[�������
Echo  ���錾��̈ꗗ���� [WXG Ver X.XX] ��I�����A[IME �̐ݒ�]
Echo  ���N���b�N����ƕ\������܂�)
Echo.
pause

cls
Echo.
Echo ���ɁA[WXG �̃v���p�e�B] �_�C�A���O �{�b�N�X�� [����] �^�u
Echo ���N���b�N���A�����Z�b�g�̕ҏW���s���܂��B�W���ł́A����
Echo 5 �̎����Z�b�g���ݒ肳��Ă��܂��B
Echo.
Echo �@�E Wxg.gds
Echo �@�E Wxgsei.gds
Echo �@�E Wxgurl.gds
Echo �@�E Wxg.gdj
Echo �@�E Wxgrev.gds
Echo. 
Echo ���̒��� Wxg.gdj ������ 4 �̎����Z�b�g�ɂ��āA
Echo ���̎菇�ɏ]���đ��삵�Ă��������B
Echo (4 �̎����Z�b�g�ɑ΂��āA���ׂĂ̍�Ƃ��s���܂�)
Echo.
pause

cls
Echo.
Echo �@1. [WXG �̃v���p�e�B] �_�C�A���O �{�b�N�X�� [����] �^�u�ŁA
Echo �@�@ [�ҏW] ���N���b�N���܂��B
Echo �@2. [WXG �����Z�b�g�ҏW] �_�C�A���O �{�b�N�X�ŁA��΃p�X��
Echo �@�@ �̕ύX���s���܂��B[�����Z�b�g�ꗗ] ���� �����Z�b�g ��
Echo �@�@ �I�����A[�����Z�b�g] ���j���[���� [�v���p�e�B]
Echo �@�@ ��I�����܂��B[�����̈ʒu�̊] �� [�����Z�b�g] ���� 
Echo �@�@ [�ʎw�� (��΃p�X)] �ɕύX���A[OK] ���N���b�N���܂��B
Echo �@�@ ����� 4 �̎����Z�b�g�ɑ΂��čs���܂��B
Echo �@3. ���[�U�[�����̈ꎞ�폜���s���܂��B[WXG �����Z�b�g�ҏW]
Echo �@�@ �_�C�A���O �{�b�N�X�̎����ꗗ�� "1. WXG ���[�U�[����" 
Echo �@�@ ��I�����A[�����폜] ��I�����č폜���܂��B
Echo �@�@ ����� 4 �̎����Z�b�g�ɑ΂��čs���܂��B
Echo �@�@ (�����Z�b�g Wxgrev.gds �ł́A���[�U�[������ 
Echo �@�@  "WXG �t�������[�U�[����" �Ƃ������O�ɂȂ��Ă���̂ŁA
Echo �@�@  ������폜���܂�)
Echo.
pause
Echo.
Echo �@4. ���ׂẴ��[�U�[�������폜���I�������A���Ɋe�����Z�b�g��
Echo �@�@ %RootDrive%�h���C�u�̃��[�U�[������o�^���܂��B[�����ǉ�] ��
Echo �@�@ �N���b�N���āA%RootDrive%\wxg\wxgu.gdj ���w�肵�܂� ([���� 1] �Ƃ���
Echo �@�@ �o�^���܂�)�B�܂��A������ [�w�K�ݒ�] �� [�w�K����] ��I��
Echo �@�@ ����悤�ɂ��܂��B
Echo �@�@ ����� 4 �̎����Z�b�g�ɑ΂��čs���܂��B
Echo �@�@ (�����Z�b�g Wxgrev.gds �ɂ��ẮA%RootDrive%\wxg\wxgurev.gdj ��
Echo �@�@  �w�肵�܂�)
Echo �@5. �ȏ�̍�Ƃ��I��������A[OK] ���N���b�N���ă_�C�A���O 
Echo �@�@ �{�b�N�X����܂��B
Echo.
pause

cls
Echo.
Echo   WXG ������ɍ쓮���邽�߂ɂ́A���݃��O�I�����Ă��郆�[�U�[��
Echo   �A�v���P�[�V���������s����O�ɁA�������񃍃O�I�t���Ă���ēx���O�I��
Echo   ����K�v������܂��B
Echo.
Echo WXG �̃}���`���[�U�[ �A�v���P�[�V�������ݒ肪�������܂����B
Echo.
pause

:Done