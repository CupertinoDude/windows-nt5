@echo off

REM 
REM PeachTree Accounting v6.0
REM ���O�I�� �A�v���P�[�V�����݊����X�N���v�g�̊���
REM    

REM
REM �A�v���P�[�V������ bti.ini �t�@�C�����g�p���܂��B���̃t�@�C���ɂ� BTrieve �̐ݒ肪����A
REM ����ɂ̓n�[�h�R�[�h���ꂽ MKDE.TRN �̏ꏊ���܂܂�Ă��܂��B���̏ꏊ�����[�U�[���Ƃ�
REM �f�B���N�g���ɕύX���āA�A�v���P�[�V�����𓯎��� 2 �ȏ�g�p�ł���悤�ɂ��܂��B
REM

REM ���[�U�[���Ƃ̃f�B���N�g���ֈړ����܂��B
cd %userprofile%\windows > NUL:

REM %systemroot% �� �G���g�� trnfile= �p�� %userprofile% �Œu�������܂��B
"%systemroot%\Application Compatibility Scripts\acsr" "%systemroot%" "%userprofile%\windows" %systemroot%\bti.ini bti.ini
