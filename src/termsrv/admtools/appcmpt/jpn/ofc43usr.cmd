@Echo off

Rem
Rem ���[�U�[�̃z�[�� �f�B���N�g���ɃA�v���P�[�V�����ŗL�̃f�[�^
Rem �̃f�B���N�g�����쐬���܂��B
Rem

call TsMkUDir "%RootDrive%\Office43"
call TsMkUDir "%RootDrive%\Office43\Access"
call TsMkUDir "%RootDrive%\Office43\Winword"

Rem ������ Normal.dot �� Winword.opt �͍폜���܂��B

If Not Exist "#OFC43INST#\Winword\Normal.dot" Goto cont0
Del /F /Q "#OFC43INST#\Winword\Normal.dot" > NUL: 2>&1
:cont0

If Not Exist "#OFC43INST#\Winword\Winword.opt" Goto cont1
Del /F /Q "#OFC43INST#\Winword\Winword.opt" > NUL: 2>&1
:cont1

Rem
Rem Access �E�B�U�[�h�p�̃t�@�C�����R�s�[���܂��B
Rem

If Exist "%RootDrive%\Office43\Access\wztable.mda" Goto skip1
If Not Exist "#OFC43INST#\Access\wztable.mda" Goto skip1
copy "#OFC43INST#\Access\wztable.mda" "%RootDrive%\Office43\Access\" > NUL: 2>&1

:skip1

If Exist "%RootDrive%\Office43\Access\wzlib.mda" Goto skip2
If Not Exist "#OFC43INST#\Access\wzlib.mda" Goto skip2
copy "#OFC43INST#\Access\wzlib.mda" "%RootDrive%\Office43\Access\" > NUL: 2>&1

:skip2

If Exist "%RootDrive%\Office43\Access\wzbldr.mda" Goto skip3
If Not Exist "#OFC43INST#\Access\wzbldr.mda" Goto skip3
copy "#OFC43INST#\Access\wzbldr.mda" "%RootDrive%\Office43\Access\" > NUL: 2>&1

:skip3

Rem
Rem �e���[�U�[�̃J�X�^�������t�@�C�����R�s�[���܂��B
Rem

If Exist "%RootDrive%\Office43\Custom.dic" Goto skip4
If Not Exist "%SystemRoot%\Msapps\Proof\Custom.dic" Goto skip4
copy "%SystemRoot%\Msapps\Proof\Custom.dic" "%RootDrive%\Office43" > NUL: 2>&1

:skip4

Rem
Rem Access �̃I�v�V���� �t�@�C�������[�U�[�� Office �f�B���N�g���փR�s�[���܂��B
Rem

If Exist "%RootDrive%\Office43\Access\System.mda" Goto skip5
If Not Exist "#OFC43INST#\Access\System.mda" Goto skip5
copy "#OFC43INST#\Access\System.mda" "%RootDrive%\Office43\Access\" > NUL: 2>&1

:skip5

Rem
Rem �g���@�\�̃��W�X�g�� �L�[��K�p�����܂��B���W�X�g�� �}�b�v�͂����̃��W�X�g�� �L�[�ɑ΂��ē��삵�܂���B

If Exist "%RootDrive%\Windows\Ofc43usr.dmy" Goto skip6
regini ofc43usr.key > NUL:
copy NUL: "%RootDrive%\Windows\Ofc43usr.dmy" > NUL: 2>&1
attrib +h "%RootDrive%\Windows\Ofc43usr.dmy" > NUL: 2>&1

:skip6

