LP-8500 == LP-8000S(E)
LP-1600 == LP-1500 + 600dpi
RICOH SP2000 == LP-2000
RICOH SP2100 == LP-1500


(1)Please send following command to switch printer mode before STARTDOC

Fixing ESC/P mode.
ESC S ESC ESC S FS (\x1B\x53\x1B\x1B\x53\x1C)

Switch from ESC/P to ESC/Page.
ESC z 0 0 (\x1B\x7A\x00\x00)

The top of BEGINDOC \x1BS\x1B\x1BS\x1C\x1B\x7A\x00\x00.

(2)Please send following command to swith printer mode after the ENDDOC

Switch from ESC/Page to ESC/P.
GS n1 pmE   n1 == 1 ... ESC/P mode. (\x1D\x31pmE)

Release ESC/P mode.
ESC S ESC ESC S K (\x1B\x53\x1B\x1B\x53\x4B)

The end of ENDDOC    \x1D\x31pmE\x1BS\x1B\x1BSK


(3)Please send following command in BEGINDOC

Set memory mode to page memory mode
GS n1 mmE    (\x1D1mmE)

Set screen mode
GS n1 tsE  (\x1D1tsE)

Set logical draw mode                ?
GS n1 ; n1 loE (\x1D1tsE)

For saving memory on printer, pleas reduce the default setting of following items.

Screen Pattern registerable number.
GS n1 isE   -> 10     (\x1D10isE)

Font attribute registerable number.
GS n1 iaF   -> 5     (\x1D5iaF)

Download font registrable number.
GS n1 ifF   -> 10    (\x1D10ifF)

Current position registable number.
GS n1 ipP   -> 5     (\x1D5ipP)

The end of BEGINDOC \x1D\x30mmE\x1D\x31tsE\x1D\x31\x30isE\x1D\x35iaF\x1D\x35\x30lifF\x1D\x35lipP


7/31

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�V�^�Q�W�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�Ώۋ@��Ƃ��āA�֋X��ȉ��̃O���[�v�ɕ����Đ������܂��B
�O���[�v�`�FLP-8000,LP-1500,LP-8000S/SE/SX,LP-1500S,LP-1000,LP-9000,LP-8500,LP-1600

�@�h�L�������g�������E�I�����ɂ̓����[�g�R�}���h���g�p���������悢�B
�@�i���������ɂ̓����[�g�R�}���h���g�p���邱�Ƃ��ł���̂ŁA�j
�@�Ώۋ@��F�O���[�v�`
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h��ύX����
		\x1BS\x1B\x1BS\x1C(ESC/P�Эڰ��݌Œ�)		��	\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
		\x1Bz\x00\x00 (ESC/P��ESC/Pagؾ�Đ؂�ւ� )		@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
									\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
									@EJL SET EC=ON PU=1 ZO=OFF\x0A(�Ӱĺ����)
									@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
�@�@���⑫������
�@�@EC=ON �|�| ����ɂ���Ăi�h�r��`�͈͈ȊO�̕����R�[�h���X�y�[�X�ɒu�������Ĉ󎚂��܂��B
�@�@�@�@�@�@�@ Windows�h���C�o�ł́A���̐ݒ�̕��������������ł��B
�@�@PU=1�@�|�| �v�����^�̃t�����g�p�l���̐ݒ�ŋ������u�������I���Ɏw�肵�Ă���ƁA
	       ESC/Page�̋������u�I���R�}���h���L���ɋ@�\���܂���B
�@�@�@�@�@�@�@ �����ŋ������u��1�ɐݒ肷�邱�Ƃŋ������u�I���R�}���h��L���ɋ@�\����悤�ɐݒ肵�܂��B
�@�@ZO=OFF�|�| ����łW�O���k���@�\�𖳌��ɂ��܂��B
�@(2)[PC_OCD_END_DOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h��ύX����
		\x1D1pmE (ESC/Page��ESC/P�؂�ւ�)		��	\x1B\x01@EJL\x20\x0A(�����[�g�J�n�錾)
		\x1BS\x1B\x1BSK (ESC/P�Эڰ��݌Œ����)			\x1B\x01@EJL\x20\x0A(�����[�g�J�n�錾)
�@�@���⑫������
�@�@��̃����[�g�R�}���h��(1)�̏��������ɐݒ肵���ݒ�l�����ɖ߂��܂��B

�A�h�L�������g�������E�I�����ł̃p�����[�^���Z�b�g�R�}���h�͕s�v
�@�n�[�h���Z�b�g�̓p�����[�^���Z�b�g�����˂Ă��܂��̂ŁA�ȉ��̃R�}���h�͕s�v�ł��B
�@�p�t�H�[�}���X���኱�����Ă��܂��@�킪����܂��B
�@�Ώۋ@��F�SESC/Page�v�����^
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1DrpE (�p�����[�^���Z�b�g)
		\x1D0ppP(���݈ʒu�X�^�b�N������)
�@(2)[PC_OCD_END_DOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1DrpE (�p�����[�^���Z�b�g)

�B���������[�h�ݒ肪�s�v
�@�Ώۋ@��F�O���[�v�`
�@�R�}���h���s�v�ł��B
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1D1mmE (���������[�h�I��; �y�[�W���������[�h)

�C���������[�h�ݒ肪�K�v
�@�Ώۋ@��FLP-2000
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1D1mmE (���������[�h�I��; �y�[�W���������[�h)
�@���⑫������
�@LP-3000�Ɠ����R�}���h��łn�j�ł��B
�@���t�@�����X�}�j���A���̋L�q�Ɍ�肪����܂��B

�D�X�N���[���p�^�[���o�^���ݒ�̕ύX�K�v
�@���̃R�}���h�͂P�ԍŏ��̃R�}���h�݂̂��L���ł��̂ŁA���������ɂP�ɐݒ肷�鎖�͖�肪����B
�@�Ώۋ@��F�SESC/Page�v�����^
�@�m�ĂP�n
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h��ύX����
		\x1D1isE (�X�N���[���p�^�[���o�^���ݒ�; 1)	��	\x1D15isE (�X�N���[���p�^�[���o�^���ݒ�; 15)
�@(2)[VP_OCD_INIT_VECT]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1D15isE (�X�N���[���p�^�[���o�^���ݒ�; 15)

�@�m�ĂQ�n
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜����
		\x1D1isE (�X�N���[���p�^�[���o�^���ݒ�; 1)

�E�R�}���h�E�t�H�[�}�b�g�d�l�̕s��v
�@�R�}���h�E�t�H�[�}�b�g���኱�قȂ���̂�����܂��B
�@�@��ɂ���ẮA�s����ȓ�������Ă��܂��̂ŕύX���K�v�ł��B
�@�Ώۋ@��F�SESC/Page�v�����^
�@(1)�h�b�g�p�^�[���𑜓x�I���R�}���h(drE)
�@�p�����[�^�R��drE�̊Ԃ�';'(\x3B)�������Ă���B
�@(2)�r�b�g�C���[�W�`��R�}���h(bi{I)
�@�p�����[�^�S��bi{I�̊Ԃ�';'(\x3B)�������Ă���B

�F�C�^���b�N�̌X���p�x
�@Windows3.1�h���C�o�ƌX���p�x������(347�x��346�x)�����قȂ�B
�@�Ώۋ@��F�SESC/Page�v�����^
�@(1)[FS_OCD_ITALIC_ON]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h��ύX����
	\x1D347slF (�����X���ݒ�;347�x)		��	\x1D346slF (�����X���ݒ�;346�x)
	\x1D347slF (�����X���ݒ�;347�x)		��	\x1D346slF (�����X���ݒ�;346�x)
�@
�G�{�[���h�̏���
�@�{�[���h�̏����ɖ�肪����B
�@�Ώۋ@��F�SESC/Page�v�����^
�@�m�ĂP�n
�@���v���������������Ȃ��ꍇ�ɐ���Ƀ{�[���h����������ł��邩�ǂ����͋@��ɂ���Ă܂��܂��Ȗʂ�����B
�@�����ŁA�{�[���h�t�H���g�̃{�[���h�����R�}���h�h�g�p�����Ɋe���̂̃{�[���h�t�H���g�̂o�e�l��ǉ��B
�@(1)[FS_OCD_BOLD_ON]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜�F�ݒ�l�Ȃ��ɂ��遄��
	\x1D1;0mcF (̫�đ����Ăяo��;0��)
	\x1D15weF (���������I��;15)
	\x1D0;0mcF (̫�đ����L��;0��)
	\x1D1;1mcF (̫�đ����Ăяo��;1��)
	\x1D15weF (���������I��;15)
�@(2)[FS_OCD_BOLD_OFF]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h���폜�F�ݒ�l�Ȃ��ɂ��遄��
	\x1D1;0mcF (̫�đ����Ăяo��;0��)
	\x1D0weF (���������I��;0)
	\x1D0;0mcF (̫�đ����L��;0��)
	\x1D1;1mcF (̫�đ����Ăяo��;1��)
	\x1D0weF (���������I��;0)
�@(3)�e���̖��Ƀ{�[���h�o�e�l��ǉ�
�@������̈Ă��̗p����ꍇ�ɂ́A���A�����������B
�@�e���̂̃{�[���h�������������m�点���܂��B
�@�m�ĂQ�n
�@�����������P�T�Ƃ����̂́A�ɒ[������̂ŁA�R�ɂ���B
�@���A�t�H���g�����̋L���R�}���h�������Ă���̂ŏC�����K�v�B
�@(1)[FS_OCD_BOLD_ON]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h��ύX����
	\x1D1;0mcF (̫�đ����Ăяo��;0��)�@��	\x1D1;0mcF (̫�đ����Ăяo��;0��)
	\x1D15weF (���������I��;15)		\x1D3weF (���������I��;3)
	\x1D0;0mcF (̫�đ����L��;0��)		\x1D0;0mcF (̫�đ����L��;0��)
	\x1D1;1mcF (̫�đ����Ăяo��;1��)	\x1D1;1mcF (̫�đ����Ăяo��;1��)
	\x1D15weF (���������I��;15)		\x1D3weF (���������I��;3)
						\x1D0;1mcF (̫�đ����L��;1��)
�@(2)[FS_OCD_BOLD_OFF]
�@�������݂̐ݒ�l�Ɉȉ��̃R�}���h��ǉ�����
						\x1D0;1mcF (̫�đ����L��;1��)

�H�X�g���C�N�A�E�g�̃R�}���h�̏��Ԃ��s��
�@�I�t�Z�b�g�̐ݒ�͎w��R�}���h�̑O�ōs���K�v������B
�@�Ώۋ@��F�SESC/Page�v�����^
�@(1)[FS_OCD_STRIKEOUT_ON]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h�̕ύX�i���Ԃ�ς���j����
	\x1D1ulC (���ްײݎw��)				  �@��	\x1D3;0uvC (���ްײݐ����̾�Đݒ�;��ײ���Ĥ�̾��0)
	\x1D3;0uvC (���ްײݐ����̾�Đݒ�;��ײ���Ĥ�̾��0)	\x1D1ulC (���ްײݎw��)
�@(2)[FS_OCD_STRIKEOUT_OFF]
�@�������݂̐ݒ�l�̈ȉ��̃R�}���h�̕ύX�i���Ԃ�ς���j����
	\x1D0;0;uvC (���ްײݐ����̾�Đݒ�;���ްײݤ�̾��0) ��	\x1D0ulC (���ްײ݉���)
	\x1D0ulC (���ްײ݉���)					\x1D0;0;uvC (���ްײݐ����̾�Đݒ�;���ްײݤ�̾��0)

�I�s�v�ȃR�}���h�̍폜
�@�����I�ȍ���������邽�߂ɁA�s�v�ȃR�}���h��`���폜
�@�Ώۋ@��F�SESC/Page�v�����^
�@(1)[VO_OCD_C_PIE]
�@�������݂̐ݒ�l�̃R�}���h���폜�F�ݒ�l�Ȃ��ɂ��遄��
�@(2)[VO_OCD_E_PIE]
�@�������݂̐ݒ�l�̃R�}���h���폜�F�ݒ�l�Ȃ��ɂ��遄��
�@(3)[VO_OCD_E_CHORD]
�@�������݂̐ݒ�l�̃R�}���h���폜�F�ݒ�l�Ȃ��ɂ��遄��

�J�I�v�V�����t�H���ƃJ�[�h�̃T�|�[�g�������Ă���B
�@�I�v�V�����t�H���g�J�[�h���T�|�[�g���Ă���@��ŃI�v�V�����t�H���g����`����Ă��Ȃ��B
�@�i����j�I�v�V�����t�H���g�̃T�|�[�g�͂��Ȃ��̂ł��傤���B



�K�����I���A�����ݼ�
�@LP-8000,LP-8000S,LP-8000SE,LP-8000SX�̎����I���ALP-1500,LP-1500S�̵����ݼނ��@�\������ɂ́A
�����[�g�R�}���h�ł̐ݒ肪�K�v�ƂȂ�܂��B�������A�����[�g�R�}���h�͏������̂Ƃ��ɂ����g�p���鎖���ł��܂���B
�����[�g�R�}���h���[�h�ɓ���ƑS�Ă̐ݒ肪���Z�b�g����Ă��܂��܂��B
�@�����ŁA�����̑I�����͍폜�����ق��������Ǝv���܂��B

�L�������u�I���R�}���h
�@�������u�I���R�}���h�̕ύX���K�v�ł��B
		����		�ύX��		
�@�����ڲ	\x1D3;1iuE	�ύX�K�v���� for LP-9000                 LP-8000
				\x1D4;1iuE   for 	 LP-8500
                           �@�@ \x1D1;1iuE   for                                 LP-1000
�@�p������1	\x1D1;1iuE	�ύX�K�v���� for LP-9000 LP-8500         LP-8000
�@�p������2	\x1D1;2iuE	\x1D2;1iuE�@ for LP-9000 LP-8500         LP-8000
�@�p������3	\x1D1;3iuE	\x1D3;1iuE   for         LP-8500
�@�����I��	�R�}���h�Ȃ�	\x1D0;0iuE�@ for LP-9000 LP-8500
�@�����ݼށ@�@�@�R�}���h�Ȃ��@�@\x1D0;0iuE�@ for �@�@�@�@�@�@�@�@LP-1600                 
				�ύX�K�v���� for �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@         LP-2000 LP-3000
�@�p���ڲ�@�@ 	\x1D1;1iuE	�ύX�K�v���� for �@�@�@�@�@�@�@�@LP-1600                  LP-1500�@�@�@�@ LP-3000
�@۱����ā@�@ 	\x1D2;1iuE	�ύX�K�v���� for                 LP-1600                  LP-1500�@�@�@�@ LP-3000
�@�����ި�̨���	\x1D2;1iuE	�ύX�K�v���� for                                                  LP-2000
�@�p������      \x1D1;1iuE      �ύX�K�v���� for                                                  LP-2000         LP-7000
�@�荷���@�@�@�@�R�}���h�Ȃ��@�@�ύX�K�v���� for �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ LP-2000 LP-3000 LP-7000
						�@LP-8000 = LP-8000,LP-8000S,LP-8000SE,LP-8000SX
						�@LP-1500 = LP-1500,LP-1500S
						�@LP-7000 = LP-7000,LP-7000G
�@���⑫������
�@�ȉ��̐ݒ�ł́A�v�����^�̃p�l���ݒ肪�K�v�ɂȂ�܂��B
�@	(1)LP-2000,LP-3000�̵����ݼ�
�@	(2)LP-2000,LP-3000,LP-7000�̎荷��
	(3)LP-7000�̗p������



��EPSON_J.DRV��

�@�Y�t��EPSON_J.DOC���Q�Ƃ��Ă��������B
�@�Ԃ������Ă���ӏ����ύX�ӏ��ł��B


��ESCP2MSJ.DRV��

�@�Y�t��ESCP2MSJ.DOC���Q�Ƃ��Ă��������B
�@�Ԃ������Ă���ӏ����ύX�ӏ��ł��B

�ȏ�


Font Cartridge
                        1000 1500 1500S 1600 2000 3000 7000 7000G 8000 8500 9000
                          -    1    1     1    1    1    -     -    2    2    2
����13���ײݥ̫�Ķ���          O    O          O    O               O
����22���ײݥ̫�Ķ���          O    O          O    O               O
�ۃS�V�b�N�̥̫�Ķ���          O    O     O    O    O               O    0    0 
���ȏ��̥̫�Ķ���              O    O     O    O    O               O    0    0
�������̥̫�Ķ���              O    O     O    O    O               O         0
�s����(�ѕM)�̫�Ķ���          O    O     O    O    O               O    0    0
��������,���p�޼���̥̫�Ķ���             O                              0    0


Font
EPSON �������̂a              76
EPSON ���p�S�V�b�N�̂a        77

-----------------------------------------------------------------------
Windows95Mini�h���C�o�@�@�X�T�^�W�^�Q�@�Z�C�R�[�G�v�\���i���j

�i�P�jESC/Page�v�����^�̃I�v�V�����t�H���g�J�[�h
�@�e�v�����^���f�����T�|�[�g����I�v�V�����t�H���g�J�[�h�������܂��B
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@�O���[�v�`�F	����13���ײݥ̫�Ķ���
		����22���ײݥ̫�Ķ���
		�ۃS�V�b�N�̥̫�Ķ���
		���ȏ��̥̫�Ķ���
		�������̥̫�Ķ���
		�s����(�ѕM)�̫�Ķ���
�@�O���[�v�a�F	�ۃS�V�b�N�̥̫�Ķ���
		���ȏ��̥̫�Ķ���
		�������̥̫�Ķ���
		�s����(�ѕM)�̫�Ķ���
		������,���p�޼���̥̫�Ķ���
�@�O���[�v�b�F	�ۃS�V�b�N�̥̫�Ķ���
		���ȏ��̥̫�Ķ���
		�s����(�ѕM)�̫�Ķ���
		������,���p�޼���̥̫�Ķ���
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@�v�����^���f��	�t�H���g�J�[�h����	�����Ɏg�p�\�Ȑ�
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-7000,LP-7000G	�Ȃ��@�@�@�@�@�@�@	�|�|
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-3000		�O���[�v�`�@�@�@�@	����̫�Ķ��ނȂ�Q
			�@�@�@�@�@�@�@�@�@�@	����̫�Ķ��ނȂ�΂P
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-2000		�O���[�v�`�@�@�@�@�@	�P
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-8000,LP-8000S,
�@LP-8000SE,LP-8000SX	�O���[�v�`�@�@�@�@�@	�Q
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-1500,LP-1500S	�O���[�v�`�@�@�@�@�@	�P
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-1000		�Ȃ��@�@�@�@�@�@�@	�|�|
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-1600		�O���[�v�a		�P
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-9000		�O���[�v�a		�Q
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@LP-8500�@		�O���[�v�b		�Q
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|

�@�e�t�H���g�J�[�h���T�|�[�g����t�H���g�������܂��B
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@̫�Ķ��ޖ���			�t�H���g����			�t�H���g����
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@����13���ײݥ̫�Ķ���	
								Regular	Bold	Italic	Bold-Italic
				Courier				x	x	x	x
				EPSON Roman T			x	x	x	x
				EPSON Sans serif H		x	x	x	x
				Symbolic Character Set		x
�@�@�@�@�@�@�@�@�@�@�@						�v�P�R����
	�i���ӁjLP-9000�����T�|�[�g���Ă���Courier�Ƃ͑I���R�}���h�A�t�H���g�������قȂ�܂��B
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@����22���ײݥ̫�Ķ���
								Regular	Bold	Italic	Bold-Italic
				EPSON Roman P			x	x	x	x
				ITC Avant Garde Gothic		x	x	x	x
				ITC Bookman			x	x	x	x
				EPSON Sans serif HN		x	x	x	x
				EPSON Roman CS			x	x	x	x
				ITC Zapf Chancery		x
				ITC Zapf Dingbats		x
								�v�Q�Q����
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@�ۃS�V�b�N�̥̫�Ķ���		EPSON �ۃS�V�b�N�̂l
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@���ȏ��̥̫�Ķ���		EPSON ���ȏ��̂l
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@�������̥̫�Ķ���		EPSON �������̂l
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@�s����(�ѕM)�̫�Ķ���		EPSON �s���̂l
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
�@������,���p�޼���̥̫�Ķ���	EPSON �������̂a
				EPSON ���p�S�V�b�N�̂a
�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|

�i�Q�jLP-9000��LP-1600�̉𑜓x�I�������[�g�R�}���h
�@BEGINDOC�̃����[�g�R�}���h�Q�ɉ𑜓x���U�O�O�������Ɏw�肷�邽�߂ɂ̃R�}���h��ǉ�����B
�@(1)[PC_OCD_BEGINDOC]
�@�������݂̐ݒ�l�̈ȉ��̃����[�g�R�}���h��ǉ�����
	RS=FN
�@�@���������āA
		\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
		@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
		\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
		@EJL SET EC=ON PU=1 ZO=OFF RS=FN\x0A(�Ӱĺ����)�@��
		@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
�@�@�ƂȂ�B
�@�@���⑫������
�@�@RS=FN �|�| �𑜓x���U�O�Odpi�ɑI������B

�i�R�jLP-7000,LP-7000G�Ɋւ���m�F
�@����̓d�b�̊m�F�����ł��B
�@�@�x�N�^���[�h�̃T�|�[�g���͂����A���X�^�[���[�h�݂̂��T�|�[�g����B
�@�A�R�}���h�̂����������R�O�O����������Q�S�O�������ɕύX����B
�@	\x1D0;0.24muE�@���@\x1D0;0.3muE(�ŏ��P�ʐݒ�R�O�Odpi���Q�S�Odpi)

�ȏ�


Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�W�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�i�P�j���[�U�[��`�T�C�Y�̐ݒ���E�l���������Ȃ��B
�@�@�Y�t��EPAGELIM.DOC���Q�Ƃ��Ă��������B

�i�Q�j�������@�̐ݒ�ԈႢ
�@2-1.LP-1000�̃T�|�[�g�g���C�̊ԈႢ
�@�@1)LP-1000	"�p���ڲ"���T�|�[�g�Ώۂ���폜����
�@�@�@�@�@�@�@�@��LP-1000���L���Ă��鋋�����u��"�����ڲ"�̂�
�@2-2.���A�[�J�Z�b�g�̃T�|�[�g
�@�@1)LP-1500	"۱�����"���T�|�[�g�Ώۂɒǉ�����B
�@�@�@�@	���I�v�V�����������u�Ƃ��Ĕ̔�����Ă��܂��B
�@�@2)LP-1500S	����
�@�@3)LP-1600	����
�@2-3.Default�g���C�̊ԈႢ
�@�@1)LP-3000	Default��"۱�����"����"�p���ڲ"�ɕύX����
�@�@�@		��۱����Ă̓I�v�V�����ł��̂ŁADefault�ɂ͓K���܂���B
�@2-4.�����I���̃T�|�[�g�폜
�@�@1)LP-8000	"�����I��"���T�|�[�g�Ώۂ���폜����B
�@�@�@�@	��ESC/Page�R�}���h�ł͎����I�����[�h��I�����鎖���ł��܂���B
�@�@2)LP-8000S	����
�@�@3)LP-8000SE	����
�@�@4)LP-8000SX	����
�@2-5.�p���J�Z�b�g���̕ύX
�@�@1)LP-8000	"�p������"��"�p������1"�ɕύX����B
�@�@�@		��Win3.1�h���C�o�Ɠ��ꖼ�̂ɂ��Ă������ق������������Ȃ��B
�@�@2)LP-8000S	����
�@�@3)LP-8000SE	����
�@�@4)LP-8000SX	����
�@�@5)LP-9000	����

�i�R�j�𑜓x�̃f�t�H���g�l�̕ύX
�@�@LP-1600��300dpi��600dpi�̂Q�̉𑜓x���T�|�[�g���Ă��܂����A
�@�@�f�t�H���g��600dpi�ɂ��Ă��������K���ł��B
�@�@���R�́ALP-1600�͍��������d�������v�����^�ł��̂ŁA600dpi����300dpi������l�ɂ��������悢
�@�@�Ƃ������f�ł��B

�i�S�j�I�v�V�����t�H���g�J�[�h�̐ݒ����
�@�@LP-1600�͈ȉ��̂T��̫�Ķ��ނ��T�|�[�g���Ă��܂��B
�@�@�ő�T�|�[�g���́A�P�ł��B
	1)�ۃS�V�b�N�̥̫�Ķ���
	2)���ȏ��̥̫�Ķ���
	3)�������̥̫�Ķ���
	4)�s����(�ѕM)�̫�Ķ���
	5)������,���p�޼���̥̫�Ķ���

�i�T�j�v�����^�������̊ԈႢ
�@�@�@�@�@�@�@	�@�@���@�@�@�@�@	��
�@�@1)LP-1600	�@3,7,11,19,35MB	1.5,3.5MB
�@�@2)LP-8000SE	�@3,6MB�@�@�@�@�@	3,8MB
�@�@3)LP-8000SX	�@����
�@�@4)LP-8500	�@4,8MB�@�@�@�@�@	4,8,12,20,36MB

�i�U�j���W�f���g�t�H���g�̊ԈႢ
�@�@LP-1000��"Courier"�����W�f���g�t�H���g�Ƃ��Đݒ肵�Ă��܂����A����͊ԈႢ�ł��B
�@�@�ȑO�̓��Ђ���̃��|�[�g�i�@��ʃT�|�[�g�\�j�Ɍ�L�����������߂ł��B
�@�@
�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�X�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�i�P�jLP-9000,LP-8500,LP-1600��Regident fonts�ɂ���

�@�@1.Dutch801
�@�@�@Regular������PFM(26 =DUTCH.PFM)�������݂��Ă��Ȃ��B
�@�@�@���̃t�H���g�Ɠ��l��Italic�����ABold�����AItalicBold������PFM���K�v�ł��B
�@�@�@�ǉ������肢���܂��B

�@�@2.Swiss721
�@�@�@Regular������PFM(27 =SWISS.PFM)�������݂��Ă��Ȃ��B
�@�@�@���̃t�H���g�Ɠ��l��Italic�����ABold�����AItalicBold������PFM���K�v�ł��B
�@�@�@�ǉ������肢���܂��B

�@�@3.Courier
�@�@�@Regular������PFM(3 =COURIER.PFM)�������݂��Ă��Ȃ��B
�@�@�@���̃t�H���g�Ɠ��l��Italic�����ABold�����AItalicBold������PFM���K�v�ł��B
�@�@�@�ǉ������肢���܂��B
�@�@�@3 =COURIER.PFM, 4 =COUIERI.PFM, 5 =COURIERB.PFM, 6 =COURIERZ.PFM�ɂ��āA
�@�@�@�����̃t�H���g���
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ)��Courier�t�H���g
	�@�@�̏�����ɂ��Ă���̂ł���΁A
�@�@�@���͂���܂���B
�@�@�@LP-9000,LP-8500,LP-1600��Regident font�Ƃ��ď�̂S��PFM��o�^���Ă��������B
�@�@�@�����A
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ�ȊO)�ŉ���13���ײݥ̫�Ķ���
	�@�@��ǉ������Ƃ��ɃT�|�[�g����Ă���Courier�t�H���g�̏�����ɂ��Ă���̂ł���΁A
�@�@�@�t�H���g���ɈႢ������܂��̂ŕύX���K�v�ł��B

�@�@4.Symbolic
�@�@�@7 =SYMBOL.PFM�ɂ��āA
�@�@�@���̃t�H���g���
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ)��Symbolic�t�H���g
	�@�@�̏�����ɂ��Ă���̂ł���΁A
�@�@�@���͂���܂���B
�@�@�@�����A
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ�ȊO)�ŉ���13���ײݥ̫�Ķ���
	�@�@��ǉ������Ƃ��ɃT�|�[�g����Ă���Symbolic Character Set�̏�����ɂ��Ă���̂ł���΁A
�@�@�@�t�H���g���ɈႢ������܂��̂ŕύX���K�v�ł��B


�i�Q�j����13���ײݥ̫�Ķ��ނɂ���

�@�@1.EPSON Sans serif H
�@�@�@����13���ײݥ̫�Ķ��ނɑ΂��āAEPSON Sans serif HN(12,13,14,15)���ݒ肳��Ă��܂��B
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ *
�@�@�@����13���ײݥ̫�Ķ��ނɂ́AEPSON Sans serif HN�ł͂Ȃ�EPSON Sans serif H���͂����Ă��܂�
�@�@�@�̂ŁAEPSON Sans serif H(16,17,18,19)�ɕύX���Ă��������B

�@�@2.Courier
�@�@�@���̃t�H���g�ɑ΂��ẮA�V���{���Z�b�g�F80�͕s���ł��B���́i�S�j���Q�Ƃ��Ă��������B
�@�@�@3 =COURIER.PFM, 4 =COUIERI.PFM, 5 =COURIERB.PFM, 6 =COURIERZ.PFM�ɂ��āA
�@�@�@�����̃t�H���g���
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ�ȊO)�ŉ���13���ײݥ̫�Ķ���
	�@�@��ǉ������Ƃ��ɃT�|�[�g����Ă���Courier�t�H���g�̏�����ɂ��Ă���̂ł���΁A
�@�@�@���͂���܂���B
�@�@�@�����A
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ)��Courier�t�H���g
	�@�@�̏�����ɂ��Ă���̂ł���΁A
�@�@�@�t�H���g���ɈႢ������܂��̂ŕύX���K�v�ł��B

�@�@3.Symbolic Character Set
�@�@�@����13���ײݥ̫�Ķ��ނ̃V���{���t�H���g�̏��̖��́ASymbolic�ł͂Ȃ�Symbolic Character Set�ł��B
�@�@�@�܂��A�V���{���Z�b�g��127�ł��B
�@�@�@7 =SYMBOL.PFM�ɂ��āA
�@�@�@���̃t�H���g���
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ�ȊO)�ŉ���13���ײݥ̫�Ķ���
	�@�@��ǉ������Ƃ��ɃT�|�[�g����Ă���Symbolic Character Set�̏�����ɂ��Ă���̂ł���΁A
�@�@�@���͂���܂���B
�@�@�@�������A���̖��̕ύX(Symbolic��Symbolic Character Set)�ƃV���{���Z�b�g�I���R�}���h�̕ύX
�@�@�@(\x1D125;0ssF��\x1D125;0ssF)���K�v�ł��B
�@�@�@�����A
	�@�@Windows3.1�h���C�o(LP-9000,LP-8500�܂���LP-1600�̂ǂꂩ)��Symbolic�t�H���g
	�@�@�̏�����ɂ��Ă���̂ł���΁A
�@�@�@�t�H���g���ɈႢ������܂��̂ŕύX���K�v�ł��B


�i�R�j����22���ײݥ̫�Ķ��ނɂ���

�@�@1.EPSON Roman CS
�@�@�@Regular������PFM(20 =ROMANCS.PFM)�������݂��Ă��Ȃ��B
�@�@�@���̃t�H���g�Ɠ��l��Italic�����ABold�����AItalicBold������PFM���K�v�ł��B
�@�@�@�ǉ������肢���܂��B
�@�@�@
�@�@2.ITC Bookman
�@�@�@Regular������PFM(25 =BOOKMAN.PFM)�������݂��Ă��Ȃ��B
�@�@�@���̃t�H���g�Ɠ��l��Italic�����ABold�����AItalicBold������PFM���K�v�ł��B
�@�@�@�ǉ������肢���܂��B


�i�S�jEPSON Roman,EPSON Sans serif����щ���̫�Ķ��ނł̃V���{���Z�b�g�ɂ���
	8 =ROMANP.PFM
	9 =ROMANPI.PFM
	10 =ROMANPB.PFM
	11 =ROMANPZ.PFM
	12 =SANSHN.PFM
	13 =SANSHNI.PFM
	14 =SANSHNB.PFM
	15 =SANSHNZ.PFM
	16 =SANSH.PFM
	17 =SANSHI.PFM
	18 =SANSHB.PFM
	19 =SANSHZ.PFM
	20 =ROMANCS.PFM
	21 =AVANT.PFM
	22 =AVANTI.PFM
	23 =AVANTB.PFM
	24 =AVANTZ.PFM
	25 =BOOKMAN.PFM
�@�@�@�@28 =ZAPFCH.PFM
	31 =ROMANT.PFM
	32 =ROMANTI.PFM
	33 =ROMANTB.PFM
	34 =ROMANTZ.PFM
�@�@�@�ɂ��āA
�@�@�@�����̃t�H���g�ɑ΂��āA�V���{���Z�b�g�F16(EPSON Character set)��I�����Ă��܂����A
�@�@�@Windows�L�����N�^�Z�b�g�ɑ΂��Ă̓V���{���Z�b�g�F48(ECMA 94-1)�̕����}�b�`���O�������ł��B
�@�@�@�����ŁA�V���{���Z�b�g�I���R�}���h���ȉ��̂悤�ɕύX���Ă��������B
�@�@�@		\x1D16;0ssF��\x1D48;0ssF
�@�@�@�@�@�@�@�@\x1D126;0ssF��\x1D48;0ssF �E�E 28 =ZAPFCH.PFM�ɂ���

�@�@�@�i�Q�j��2.Courier�Ő������܂������A����13���ײݥ̫�Ķ��ނ�Courier�ɑ΂��ẮA�V���{��
�@�@�@�Z�b�g�F80�͕s���ł��̂ŁA��Ɠ��l�ɃV���{���Z�b�g�F48���g�p���Ă��������B���������āA
�@�@�@����13���ײݥ̫�Ķ��ނ�Courier�ɑ΂��ẮA�ȉ���PFM�����̂܂܎g�p���鎖�͂ł��܂���B
�@�@�@	3 =COURIER.PFM, 4 =COUIERI.PFM, 5 =COURIERB.PFM, 6 =COURIERZ.PFM


�i�T�j�b�s�s�ɂ���

�@�@1.ECMA 94-1(�V���{���Z�b�g�F48)
�@�@�@�V���{���Z�b�g��ECMA 94�1�ɑI�����Ă��A�L�����N�^�R�[�h��60H��80h�`9Fh�ł͉�ʂƈقȂ���
�@�@�@���ʂƂȂ�܂��B�����ŁA�Y�t��CTT�t�@�C��(ECMA94.CTT)���g�p���邱�Ƃɂ���ăO���t���\��
�@�@�@����Windows�L�����N�^�Z�b�g�Ƀ}�b�`���O�����邱�Ƃ��ł��܂��B

�@�@2.Symbolic Character Set(�V���{���Z�b�g�F127)
�@�@�@���̃V���{���Z�b�g��Windows��Symbol�Ɣ�ׂăL�����N�^�R�[�h��80h�ȍ~�ɑ��Ⴊ����܂��B
�@�@�@�����ŁA�Y�t��CTT�t�@�C��(SYMBOL.CTT)���g�p���邱�Ƃɂ���ăO���t���\�Ȍ���
�@�@�@Windows�L�����N�^�Z�b�g�Ƀ}�b�`���O�����邱�Ƃ��ł��܂��B
�@�@�@�m���Ӂn���̂b�s�s��LP-9000,LP-8500,LP-1600��Regident font�ł���Symbolic�ɑ΂��Ă�
�@�@�@�@�@�@�@�K�v����܂���B
�@�@�@

�i�U�jEPSON Roman,EPSON Sans serif�ł̃V���{���Z�b�g�ɂ���
�@�@�@EPSON Roman,EPSON Sans serif�A���̂Q���̂́A�V���{���Z�b�g�FECMA 94-1�������Ă��Ȃ��̂ŁA
�@�@�@�V���{���Z�b�g�F16���g�p���邵������܂���BPFM�̃V���{���Z�b�g�I���R�}���h�ɕύX�̕K�v��
�@�@�@����܂���B
�@�@�@Windows3.1�h���C�o�ł́A������x�����u�����������Ă��܂����A�b�s�s�ł͏����ł��܂���B
�@�@�@�g�p�p�x�͏��Ȃ��t�H���g�ł��̂ŁAWindows95�~�j�h���C�o�ł͐��������Ƃ��Ĉʒu�t���鎖��
�@�@�@���͖����Ǝv���܂��B

�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�P�O�@�Z�C�R�[�G�v�\���i���j



��EPAGEMSJ.DRV��

�i�P�jPage Control����

�@�@1.#2 Page Control
	\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
	@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
	\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
					�� @EJL SE LA=ESC/PAGE\x0A ��ǉ�
	@EJL SET EC=ON PU=1 ZO=OFF\x0A(�Ӱĺ����)
	@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)

�@�@2.#3 Page Control
	\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
	@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)
	\x1B\x01@EJL\x20\x0A(�ӰĊJ�n�錾)
					�� @EJL SE LA=ESC/PAGE\x0A ��ǉ�
	@EJL SET EC=ON PU=1 ZO=OFF RS=FN\x0A(�Ӱĺ����)
	@EJL EN LA=ESC/PAGE\x0A(ESC/PageӰ��)

���⑫������
�@�@EC=ON��L���ɂ��邽�߂ɏ�̃R�}���h���K�v�ł��B


�i�Q�j�o�e�l�̑I���R�}���h�̒���

�@�@1.PFM\ROMANPZ.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D0stF �� \x1D1stF
	�EItalic�t���O���Z�b�g

�@�@2.PFM\ROMANTZ.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D0stF �� \x1D1stF
	�EItalic�t���O���Z�b�g

�@�@3.PFM\AVANT.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D0weF �� \x1D-1weF

�@�@4.PFM\AVANTI.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D0weF �� \x1D-1weF

�@�@5.PFM\AVANTB.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D3weF �� \x1D6weF

�@�@6.PFM\AVANTZ.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D3weF �� \x1D6weF

�@�@7.PFM\DUTCH.PFM
�@�@�@�@�E�R�}���h�ύX	\1D80;16ssF �� \1D80;0ssF

�@�@8.PFM\ZAPFCH.PFM
�@�@�@�@�E�R�}���h�ǉ�	\x1D1stF

�@�@9.PFM\SHOUKAIV.PFM
�@�@�@�@�E�R�}���h�ύX	\x1D66tfF �� \x1D68tfF


�i�R�j�����o�e�l�̃t�H���g���̒���

�@�@1.PFM\KGOTHICV.PFM
�@�@�@InternalLeading:94 �� 0

�@�@2.PFM\MGOTHICV.PFM
�@�@�@InternalLeading:94 �� 0
�@�@�@Family:Roman �� Modern

�@�@3.PFM\MGOTHIC.PFM
�@�@�@Family:Roman �� Modern

�@�@5.PFM\SHOUKAI.PFM
�@�@�@Family:Roman �� Script
�@�@�@PixHeigth:1014 �� 1042

�@�@6.PFM\SHOUKAIV.PFM
�@�@�@Family:Roman �� Script
�@�@�@PixHeigth:1014 �� 1042

�@�@7.PFM\FMINB.PFM
�@�@�@Family:Script �� Roman
�@�@�@PixHeigth:1014 �� 1042

�@�@8.PFM\FMINBV.PFM
�@�@�@Family:Script �� Roman
�@�@�@PixHeigth:1014 �� 1042

�@�@9.PFM\FGOB.PFM
�@�@�@Family:Script �� Modern
�@�@�@PixHeigth:1014 �� 1042

�@�@10.PFM\FGOBV.PFM
�@�@�@Family:Script �� Modern
�@�@�@PixHeigth:1014 �� 1042

�@�@11.PFM\KYOUKA.PFM
�@�@�@PixHeigth:1014 �� 1042

�@�@12.PFM\KYOUKAV.PFM
�@�@�@PixHeigth:1014 �� 1042

�@�@13.PFM\MOUHITSU.PFM
�@�@�@PixHeigth:1014 �� 1042

�@�@14.PFM\MOUHITSV.PFM
�@�@�@PixHeigth:1014 �� 1042


�i�S�j����13���ײݥ̫�Ķ��ނ̕s���o�e�l�ɂ���
�@�@�o�e�l�ɂȂ��t�H���g�̑I���R�}���h���ȉ��Ɏ����܂��B

�@�@1.����13���ײݥ̫�Ķ��ނ�Courier
�@�@�@<Normal>		\x1D48;0ssF\x1D0spF\x2tfF\x1D0stF\x1D0weF
�@�@�@<Italic>		\x1D48;0ssF\x1D0spF\x2tfF\x1D1stF\x1D0weF
�@�@�@<Bold>		\x1D48;0ssF\x1D0spF\x2tfF\x1D0stF\x1D3weF
�@�@�@<BoldItalic>	\x1D48;0ssF\x1D0spF\x2tfF\x1D1stF\x1D3weF

�@�@2.����13���ײݥ̫�Ķ��ނ�Symbolic Character Set
�@�@�@\x1D127;0ssF\x1D1spF\x17tfF


�i�T�j<Bold>,<Italic>,<BoldItalic>�̂o�e�l�ɂ���
�@�@<Bold>,<Italic>,<BoldItalic>�̂o�e�l�ɂ̂Ȃ��t�H���g�̑I���R�}���h���ȉ��Ɏ����܂��B

�@�@1.EPSON Roman CS
�@�@�@<Normal>		\x1D48;0ssF\x1D1spF\x24tfF\x1D0stF\x1D0weF
�@�@�@<Italic>		\x1D48;0ssF\x1D1spF\x24tfF\x1D1stF\x1D0weF
�@�@�@<Bold>		\x1D48;0ssF\x1D1spF\x24tfF\x1D0stF\x1D3weF
�@�@�@<BoldItalic>	\x1D48;0ssF\x1D1spF\x24tfF\x1D1stF\x1D3weF

�@�@2.ITC Bookman
�@�@�@<Normal>		\x1D48;0ssF\x1D1spF\x20tfF\x1D0stF\x1D-3weF
�@�@�@<Italic>		\x1D48;0ssF\x1D1spF\x20tfF\x1D1stF\x1D-3weF
�@�@�@<Bold>		\x1D48;0ssF\x1D1spF\x20tfF\x1D0stF\x1D6weF
�@�@�@<BoldItalic>	\x1D48;0ssF\x1D1spF\x20tfF\x1D1stF\x1D6weF

�@�@3.Dutch801
�@�@�@<Normal>		\x1D80;0ssF\x1D1spF\x25tfF\x1D0stF\x1D0weF
�@�@�@<Italic>		\x1D80;0ssF\x1D1spF\x25tfF\x1D1stF\x1D0weF
�@�@�@<Bold>		\x1D80;0ssF\x1D1spF\x25tfF\x1D0stF\x1D3weF
�@�@�@<BoldItalic>	\x1D80;0ssF\x1D1spF\x25tfF\x1D1stF\x1D3weF

�@�@4.Swiss721
�@�@�@<Normal>		\x1D80;0ssF\x1D1spF\x26tfF\x1D0stF\x1D0weF
�@�@�@<Italic>		\x1D80;0ssF\x1D1spF\x26tfF\x1D1stF\x1D0weF
�@�@�@<Bold>		\x1D80;0ssF\x1D1spF\x26tfF\x1D0stF\x1D3weF
�@�@�@<BoldItalic>	\x1D80;0ssF\x1D1spF\x26tfF\x1D1stF\x1D3weF



��ܰ���߯�ނ̓���s���
�@�v�����^�̊����f�o�C�X�t�H���g���w�肵�Ă���ƁA��̂P�ԍŌ�̕������󎚂���Ȃ��B
�@���̂悤�ȕs����������Ă��܂��B
�@�i�Č����@�j
�@�@(1)EPAGEMSJ.DRV��EPSON_J.DRV�̃h���C�o�Ŋ����t�H���g�i�Ⴆ�΁A�����j��I������B
�@�@(2)�������P�����i�Ⴆ�΁A"�P"�j����͂��Ĉ������B
�@�@(3)�������󎚂���Ȃ��B
�@�@(4)�v�����^�R�}���h����͂��Ă������R�[�h���o�͂���Ă��Ȃ��B


�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�P�V�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��


�i�P�jPage Control�̕ύX

�@�@1.LP-1500
	#1 Page Control�@��	#2 Page Control
�@�@�@LP-1500��LP-1500S�Ɠ��l�Ƀ����[�g�R�}���h���󂯕t���܂��B

�@�@2.LP-3000
	#4 Page Control�@��	#1 Page Control
�@�@�@LP-3000��300dpi�Ȃ̂ŁA#4 Page Control�ł͂Ȃ�#1 Page Control�ł��B



�i�Q�jPage Size Limits�̒�������

�@�@1.LP-7000/LP-7000G
	Width  Max	12410	�� 12140

�@�@2.LP-1000
	Width  Min	4725	�� 4648
	       Max	12140	�� 10200
	Height Min	6995	�� 6992
	       Max	17195	�� 16800

�@�@3.LP-8000
	Height Min	660	�� 6600


�i�R�j�v�����^�������̊ԈႢ

�@�@1.LP-8500
		��@�@	�@�@��
	�@�@�@4,8MB	�� 4,8,12,20,36MB
�@�@�@RPRT0808.TXT�ł̋L�ڃ~�X�ł����B


�i�S�j�o�e�l�̏C��

�@�@�ȉ��̓_���C�����܂����B
�@�@�C���ς݂̂o�e�l�t�@�C����Y�t���܂����B
----------------------------------------------------
1   PFM\ROMAN.PFM	Pitch & Family	26 -> 16	;����`�̃r�b�g�������Ă����̂�
			InternalLeading	94 -> 0
2   PFM\SANSRF.PFM	Pitch & Family	26 -> 32	;Roman -> Swiss
			InternalLeading	94 -> 0
3   PFM\COURIER.PFM	PixWidth	782 -> 627
			Pitch & Family	26 -> 48	;Roman -> Modern
			Ascent		879 -> 796
			AveWidth	782 -> 627
			InternalLeading	94 -> 0
			MaxWidth	782 -> 627
			FontSelection	\x1D48;0ssF -> \x1D80;0ssF
			TransTab	11 -> 1
4   PFM\COURIERI.PFM	"3   PFM\COURIER.PFM"�Ɠ��l
5   PFM\COURIERB.PFM	"3   PFM\COURIER.PFM"�Ɠ��l
6   PFM\COURIERZ.PFM	"3   PFM\COURIER.PFM"�Ɠ��l
7   PFM\SYMBOL.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
			Ascent		845 -> 796
8   PFM\SYMBOLIC.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
9   PFM\ROMANP.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
10  PFM\ROMANPI.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
11  PFM\ROMANPB.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
12  PFM\ROMANPZ.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
			Italic		0 -> 1
13  PFM\SANSHN.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
			Face Name	"Sans serif HN" -> "EPSON Sans serif HN"
14  PFM\SANSHNI.PFM	"13  PFM\SANSHN.PFM"�Ɠ��l
15  PFM\SANSHNB.PFM	"13  PFM\SANSHN.PFM"�Ɠ��l
16  PFM\SANSHNZ.PFM	"13  PFM\SANSHN.PFM"�Ɠ��l
17  PFM\SANSH.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
			Face Name	"Sans serif H" -> "EPSON Sans serif H"
18  PFM\SANSHI.PFM	"17  PFM\SANSH.PFM"�Ɠ��l
19  PFM\SANSHB.PFM	"17  PFM\SANSH.PFM"�Ɠ��l
20  PFM\SANSHZ.PFM	"17  PFM\SANSH.PFM"�Ɠ��l
21  PFM\ROMANCS.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
22  PFM\ROMANCSI.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
23  PFM\ROMANCSB.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
24  PFM\ROMANCSZ.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
25  PFM\AVANT.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
			InternalLeading	796 -> 0
26  PFM\AVANTI.PFM	"25  PFM\AVANT.PFM"�Ɠ��l
27  PFM\AVANTB.PFM	"25  PFM\AVANT.PFM"�Ɠ��l
28  PFM\AVANTZ.PFM	"25  PFM\AVANT.PFM"�Ɠ��l
29  PFM\BOOKMAN.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�	
30  PFM\BOOKMANI.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
31  PFM\BOOKMANB.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
32  PFM\BOOKMANZ.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
33  PFM\DUTCH.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
34  PFM\DUTCHI.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
			FontSelection	\x1D1spF\x1D1spF -> \x1D1spF\x1D1stF
35  PFM\DUTCHB.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
36  PFM\DUTCHZ.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
			FontSelection	\x1D1spF\x1D1spF -> \x1D1spF\x1D1stF
37  PFM\SWISS.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
38  PFM\SWISSI.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
			FontSelection	\x1D1spF\x1D1spF -> \x1D1spF\x1D1stF
39  PFM\SWISSB.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
40  PFM\SWISSZ.PFM	Pitch & Family	43 -> 33	;����`�̃r�b�g�������Ă����̂�
			FontSelection	\x1D1spF\x1D1spF -> \x1D1spF\x1D1stF
41  PFM\ZAPFCH.PFM	Pitch & Family	75 -> 65	;����`�̃r�b�g�������Ă����̂�
42  PFM\ZAPFDING.PFM	Pitch & Family	91 -> 81	;����`�̃r�b�g�������Ă����̂�
43  PFM\MOREWB.PFM	Pitch & Family	59 -> 49	;����`�̃r�b�g�������Ă����̂�
44  PFM\COUR13.PFM	PixWidth	782 -> 625
			Pitch & Family	26 -> 48	;Roman -> Modern
			Ascent		879 -> 796
			AveWidth	782 -> 625
			InternalLeading	94 -> 0
			MaxWidth	782 -> 625
45  PFM\COUR13I.PFM	"44  PFM\COUR13.PFM"�Ɠ��l
46  PFM\COUR13B.PFM	"44  PFM\COUR13.PFM"�Ɠ��l
47  PFM\COUR13Z.PFM	"44  PFM\COUR13.PFM"�Ɠ��l
48  PFM\ROMANT.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
49  PFM\ROMANTI.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
50  PFM\ROMANTB.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
51  PFM\ROMANTZ.PFM	Pitch & Family	27 -> 17	;����`�̃r�b�g�������Ă����̂�
52  PFM\MINCHO.PFM	Pitch & Family	26 -> 16	;����`�̃r�b�g�������Ă����̂�
53  PFM\MINCHOV.PFM	Pitch & Family	26 -> 16	;����`�̃r�b�g�������Ă����̂�
54  PFM\KGOTHIC.PFM	�ύX�Ȃ�
55  PFM\KGOTHICV.PFM	�ύX�Ȃ�
56  PFM\MGOTHIC.PFM	�ύX�Ȃ�
57  PFM\MGOTHICV.PFM	Pitch & Family	26 -> 48	;Roman -> Modern
			InternalLeading	94 -> 0
58  PFM\KYOUKA.PFM	Pitch & Family	26 -> 16	;����`�̃r�b�g�������Ă����̂�
59  PFM\KYOUKAV.PFM	Pitch & Family	26 -> 16	;����`�̃r�b�g�������Ă����̂�
60  PFM\SHOUKAI.PFM	�ύX�Ȃ�
61  PFM\SHOUKAIV.PFM	�ύX�Ȃ�
62  PFM\MOUHITSU.PFM	�ύX�Ȃ�
63  PFM\MOUHITSV.PFM	�ύX�Ȃ�
64  PFM\FMINB.PFM	�ύX�Ȃ�
65  PFM\FMINBV.PFM	�ύX�Ȃ�
66  PFM\FGOB.PFM	�ύX�Ȃ�
67  PFM\FGOBV.PFM	�ύX�Ȃ�
----------------------------------------------------


�i�T�j�����I�t�Z�b�g�ʐݒ�̕s�

�@�R�[���o�b�N�t�@���N�V�����ŕ����I�t�Z�b�g��(�R�}���h�F\x1D%dcoP)��ݒ肵�Ă��܂����A�����ȊO��
�t�H���g�ɑ΂��Ă������I�t�Z�b�g�ʂ�ݒ肵�Ă��邽�߁A�������̈ȊO�̏��́i�������́j�̈󎚈ʒu��
���ɂ���Ă��܂��B�������̂́A�������̂ƈقȂ�A�x�[�X�|�C���g�i�󎚂��J�n����ʒu�j���x�[�X���C��
��ɂ���̂ŁA�����I�t�Z�b�g�ʂ��O�G�O�ɐݒ肷��K�v������܂��B

�@���s��Č����@��
�@(1)���[�h�p�b�h�ŁA����s�Ɋ�������(��:����)�̕����Ɖ�������(��:Courier)�̕������`���Ĉ���B
�@(2)�������̂����ɂ����̂ŁA�������̂Ɖ������̂̃x�[�X���C���������B
�@

�i�U�j�b�s�s������

�@CTT���L���ɓ����Ă��Ȃ��B�����R�[�h�ϊ�������Ă��Ȃ��B


�i�V�j�{�[���h�A�C�^���b�N�̃t�H���g�V���~���[�V����

�@�{�[���h�A�C�^���b�N�̃t�H���g�V���~���[�V�������{�[���h������C�^���b�N�����̂o�e�l��������
���̂ɑ΂��Ă������Ă��܂��B�����̑����������̂ɑ΂��ẮA���̂悤�ȃt�H���g�V���~���[�V����
�͕K�v����܂���B


�i�W�j�X�N���[���p�^�[�����̒�`

�@�m�F�ł��B
�@�ȑO�̃h���C�o�ł́A�X�N���[���p�^�[�������P�T�Œ�`���Ă��܂������A���݂̃h���C�o�ł́A�P�S��
�̒�`�ɂȂ��Ă��܂��B���݂̃h���C�o�Ŏg�p���Ă���X�N���[���p�^�[����`�͂P�S�Ȃ̂ł��傤���B


�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�Q�S�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�i�P�jPage Sizes�̏㉺���E�̔����̈�̐ݒ�ɂ���
�@�@UNITOOL�ł́APredefined�̗p���̃T�C�Y���h�b�g�iMaster Units�ł́j�ɕϊ�����ꍇ�Ɉȉ���
�@���������Ă���悤�ł��B
�@�@����P�ʁF	�i�m��� �������_��R�ʂŎl�̌ܓ��j�~ Master Units
�@�@mm�P�ʁF	�i�i�mmm �� 25.4�j�������_��R�ʂŎl�̌ܓ��j�~ Master Units
�@���̏����ł��ƁA�}0.005����̌덷�i600dpi�ōő�R�h�b�g�j���������܂��B
�@���̂��߁APage Sizes�̏㉺���E�̔����̈��0.2����ɐݒ肷��ƁA����\�̈�ƃv�����^��
�@�d�l�Ƃ��s��v���������܂��B���̂��߁A�ȉ��̕s����������܂��B
�@�@(1) ESC/Page�v�����^�ł͎w�肳�ꂽ�p���T�C�Y�̈���̈�ň���C���[�W���N���b�v����̂ŁA
�@�@�@�@�E�[�≺�[�����h�b�g����Ă��܂�
�@�@(2) Windows3.1��ESC/Page�v�����^�ƈ���̈悪��݊��ɂȂ�B

���ȉ��ɕύX���ׂ��l�������܂��BPrinting Offset��0.2����Ɉێ����邽�߂�Portrait Mode��
�@Landscape Mode�Ƃł́ATop��Bottom�̒l�����ւ���K�v������܂��B
                               Portrait Mode                   Landscape Mode
             Width  Height     Top     Bottom  Left    Right   Top     Bottom  Left    Right
A4            9924   14028      240     236     240     244     236     240     240     244
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �P�P            �P�P    �P�P                    �P�P
B4           12144   17196      240     236     240     240     236     240     240     240
                                       �P�P                    �P�P
B5            8604   12144      240     240     240     244     240     240     240     244
                                                       �P�P                            �P�P
A3           14028   19848      240     248     240     236     248     240     240     236
                                       �P�P            �P�P    �P�P                    �P�P
HAGAKI        4728    6996      240     244     240     244     244     240     240     244
                                       �P�P            �P�P    �P�P                    �P�P
A5            6996    9924      240     244     240     244     244     240     240     244
                                       �P�P            �P�P    �P�P                    �P�P
A6            4956    6996      240     246     240     236     246     240     240     236
                                       �P�P            �P�P    �P�P                    �P�P
ENV_MONARCH   4656    9000      240     240     240     248     240     240     240     248
                                       �P�P            �P�P    �P�P                    �P�P
ENV_DL        5196   10392      240     240     240     240     240     240     240     240
                                       �P�P            �P�P    �P�P                    �P�P
ENV_C5        7656   10824      240     248     240     244     248     240     240     244
                                       �P�P            �P�P    �P�P                    �P�P
���ȉ��̂Q�̗p���T�C�Y�́A��̐ݒ�̏ꍇ��LP-7000/LP-7000G�ɑ΂��āA�ۂߌ덷���������邽�߂�
�@�ȉ���Driver Defined�̗p����ǉ�����K�v������܂��B
                               Portrait Mode                   Landscape Mode
             Width  Height     Top     Bottom  Left    Right   Top     Bottom  Left    Right
B4           12140   17195      240     240     240     240     240     240     240     240
B5            8595   12140      240     240     240     240     240     240     240     240


��EPSON_J.DRV��

�i�P�jResolution�F360 x 180�̃R�}���h�ԈႢ
�@�@Send Block�R�}���h�ɊԈႢ������܂��B
			�@�@��		�@�@��
�@�@��`�R�}���h��	�@�@\x1B*{%l	���@\x1B*(%l
�@�@�i�Q�l�j�P�U�i�\��	�@�@1B2A7B�@	�@�@1B2A28
�@�@�@�@�@�@�@�@	�@�@�@�@�P	�@�@�@�@�P
���⑫������
�@�@VP-1100,VP-4000,VP-5000,VP-6000��360 x 180�ł̈���s��̌����ł����B


��ESCP2MSJ.DRV��

�i�P�jResolution�F360 x 360��180 x 180 �ł�Units�ݒ�ԈႢ
�@�@AP-1000V2��AP-400V2���T�|�[�g���Ă���Resolution(360 x 360��180 x 180)��
�@�@Send Block��[Units...]�ɐݒ�ԈႢ������܂��B
			�@�@��		�@�@��
�@�@sUnitDiv�̐ݒ�l	�@�@2		���@1

���⑫������
�@�@AP-1000V2��AP-400V2�h���C�o�ł̈���s��̌����ł����B


�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�Q�T�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�i�P�jF4�p����Paper Size��X Size(Width)��Y Size(Height)�̕ύX
�@RPRT0824.TXT�́h�i�P�jPage Sizes�̏㉺���E�̔����̈�̐ݒ�ɂ��āh�Ɠ��l�ȗ��R�ł��B
�@
�@�p������		X Size		Y Size
�@F4 210 x 330 mm	9917 �� 9920	15584 �� 15592

�ȏ�

Windows95Mini�h���C�o�̕]�����|�[�g	�X�T�^�W�^�Q�X�@�Z�C�R�[�G�v�\���i���j


��EPAGEMSJ.DRV��

�i�P�j�p���F�^�u���C�h�iTabloid  11 x 17 in�j�̑I���R�}���h
�@�@�@�p���I���R�}���h�i\x1D36psE�j��Ledger��I���ł��܂��B
�@�@�@Ledger�̗p���T�C�Y�� 11 x 17 �C���`�ł��̂ŁATabloid�Ɠ���p���T�C�Y�ł��B
�@�@�@�����ŁA�ȉ��̂悤�ɕs��`�p���ݒ�R�}���h���`�p���ݒ�R�}���h�ɕύX���Ă��������B

	\x1D-1;3300;5100psE		�� \x1D36psE
	\x1D-1;3300;5100psE\x1D1poE	�� \x1D36psE\x1D1poE

�i�Q�jModel Data��sMaxPhysWidth�̒l
�@�@�@���ׂẴ��f����sMaxPhysWidth�̒l��20400�ƂȂ��Ă���A���[�U�[��`�T�C�Y�̕��̏����
�@�@�@�P�V�C���`�Ƃ��ĕ\������Ă��܂��B
�@�@�@�����ŁA���ꂼ��̃��f���̎��ۂ̕��̏���̒l�ł���Page Size Limits��Width-Max�̒l�ɕύX
�@�@�@���Ă��������B

�i�R�j���[�U�[��`�T�C�Y�̗p���ł̑I���R�}���h
�@�@�@���[�U�[��`�T�C�Y��I������ƕ����O�Œ����݂̂����[�U�[����`�����l�Ƃ��Đݒ肳��Ă���B
�@�@�@��jLP-1600��3.87" x 5.83"�̗p�����`�����ꍇ�̃R�}���h
�@�@�@�@�@\x1D-1;1749;psE
�@�@�@�@�@5.83�~300��1749�Ȃ̂Œ����͎w��ʂ�ł��邪�A�����w�肳��Ă��܂���B
�@�@�@���Ђ�ESC/Page�v�����^�̏ꍇ�ɂ́A�p�����h�����̃Z���^�[�ɂ���Ƃ��Ĉ�������܂��̂ŁA
�@�@�@�p���̕����v�����^�ɒm�点��K�v������܂��B
�@�@�@��̗�ł́A�R�}���h��i\x1D-1;1161;1749psE�C3.87�~300��1161�j���o�͂���Ȃ��ƁA���[�U�[
�@�@�@��`�T�C�Y�̗p���ɑ΂��ẮA����Ɉ�����鎖�͂ł��܂���̂ŁA�C�������肢���܂��B

�i�S�j�����̃v�����^�t�H���g�ɑ΂��镶���I�t�Z�b�g�ʐݒ�R�}���h
�@�@�@�����I�t�Z�b�g�ʐݒ�́A�ʂ̃t�H���g���ݒ肳��Ă��ݒ�l�͗L���ƂȂ�܂��B
�@�@�@���̂��߁ATrueTyep�t�H���g�i�������Acent���j�⊿���v�����^�t�H���g�i��������Descent���j
�@�@�@�̒���ɉ����v�����^�t�H���g���g�p����ƁA�����I�t�Z�b�g�����������邽�߈ʒu������܂��B
�@�@�@�����v�����^�t�H���g��ݒ肷��ꍇ�ɂ͕����I�t�Z�b�g��(0,0)�ɐݒ肷��悤�ɂ��Ă��������B

�ȏ�
