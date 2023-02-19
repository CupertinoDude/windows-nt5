/**************************************************************************
*
*  HWR_FILE.H                             Created: 21.01.92
*
*    This file contains the function prototypes needed for some basic
*  functions, data definitions  and  function  prototypes needed for
*  file handling functions,
*
**************************************************************************/

#ifndef FILE_DEFINED        /*  See #endif in the end of file.  */
#define FILE_DEFINED

#include "bastypes.h"
#include "hwr_sys.h"

#if !defined(HWR_SYSTEM_NO_LIBC) && HWR_SYSTEM == HWR_ANSI
	#include <stdio.h>
	#define HWR_FILENAME_MAX FILENAME_MAX
#endif

#ifndef HWR_FILENAME_MAX
	#define HWR_FILENAME_MAX 1024
#endif

#ifndef  HFILE_TO_VALUE
typedef  p_VOID  _HFILE;
#define  VALUE_TO_HFILE VALUE_TO_PTR
#define  HFILE_TO_VALUE PTR_TO_VALUE
#endif
typedef  _HFILE _PTR           p_HFILE;

#ifndef  HTEXT_TO_VALUE
typedef  p_VOID  _HTEXT;
#define  VALUE_TO_HTEXT VALUE_TO_PTR
#define  HTEXT_TO_VALUE PTR_TO_VALUE
#endif
typedef  _HTEXT _PTR           p_HTEXT;

#ifndef  HFIND_TO_VALUE
typedef  _HMEM _HFIND;
#define  VALUE_TO_HFIND VALUE_TO_HMEM
#define  HFIND_TO_VALUE HMEM_TO_VALUE
#endif
typedef  _HFIND _PTR           p_HFIND;

#ifndef  HSEEK_TO_VALUE
typedef  _LONG  _HSEEK;
#define  VALUE_TO_HSEEK VALUE_TO_LONG
#define  HSEEK_TO_VALUE LONG_TO_VALUE
#endif
typedef  _HSEEK _PTR           p_HSEEK;

/* Values available for Open (parameter wRdWrAccess HWRFileOpend and
   HWRTextOpen) */
#define HWR_FILE_NORMAL 0
#define HWR_FILE_RDWR   0
#define HWR_FILE_RDONLY 1
#define HWR_FILE_WRONLY 2

#define HWR_TEXT_NORMAL 0
#define HWR_TEXT_RDWR   0
#define HWR_TEXT_RDONLY 1
#define HWR_TEXT_WRONLY 2

/* Origin constants for seek (parameter wOrigin in function HWRFileSeek) */
#define HWR_FILE_SEEK_SET 0
#define HWR_FILE_SEEK_CUR 1
#define HWR_FILE_SEEK_END 2

/* Constants for seek (parameter hSeek in function HWRTextSeek) */
#define HWR_TEXT_SEEK_BEGIN   ((_LONG)(-2L))
#define HWR_TEXT_SEEK_END     ((_LONG)(-3L))

/* EOF constants */
#define HWR_FILE_EOF   ((_WORD)(_INT)-1)
#define HWR_TEXT_EOF   ((_WORD)(_INT)-1)

/* Open mode for file functions (parameter wOpenMode in function HWRFileOpen) */
#define HWR_FILE_EXIST_OPEN      0x01
#define HWR_FILE_EXIST_CREATE    0x02
#define HWR_FILE_EXIST_ERROR     0x03
#define HWR_FILE_EXIST_UNIQUE    0x04
#define HWR_FILE_EXIST_APPEND    0x05
#define HWR_FILE_NOTEXIST_CREATE 0x10
#define HWR_FILE_NOTEXIST_ERROR  0x20
#define HWR_FILE_EXIST_MASK      0x0F
#define HWR_FILE_NOTEXIST_MASK   0xF0

#define HWR_FILE_EXCL      (HWR_FILE_EXIST_OPEN|HWR_FILE_NOTEXIST_ERROR)
#define HWR_FILE_OPEN      (HWR_FILE_EXIST_OPEN|HWR_FILE_NOTEXIST_CREATE)
#define HWR_FILE_CREAT     HWR_FILE_OPEN
#define HWR_FILE_TRUNC     (HWR_FILE_EXIST_CREATE|HWR_FILE_NOTEXIST_CREATE)
#define HWR_FILE_APPEND    (HWR_FILE_EXIST_APPEND|HWR_FILE_NOTEXIST_CREATE)

/* Open mode for text functions (parameter wOpenMode in function HWRTextOpen) */
#define HWR_TEXT_EXIST_OPEN      0x01
#define HWR_TEXT_EXIST_CREATE    0x02
#define HWR_TEXT_EXIST_ERROR     0x03
#define HWR_TEXT_EXIST_UNIQUE    0x04
#define HWR_TEXT_EXIST_APPEND    0x05
#define HWR_TEXT_NOTEXIST_CREATE 0x10
#define HWR_TEXT_NOTEXIST_ERROR  0x20
#define HWR_TEXT_EXIST_MASK      0x0F
#define HWR_TEXT_NOTEXIST_MASK   0xF0

#define HWR_TEXT_EXCL      (HWR_TEXT_EXIST_OPEN|HWR_TEXT_NOTEXIST_ERROR)
#define HWR_TEXT_OPEN      (HWR_TEXT_EXIST_OPEN|HWR_TEXT_NOTEXIST_CREATE)
#define HWR_TEXT_CREAT     HWR_TEXT_OPEN
#define HWR_TEXT_TRUNC     (HWR_TEXT_EXIST_CREATE|HWR_TEXT_NOTEXIST_CREATE)
#define HWR_TEXT_APPEND    (HWR_TEXT_EXIST_APPEND|HWR_TEXT_NOTEXIST_CREATE)

   /* Binary file functions */

_HFILE  HWRFileOpen (_STR zPathName, _WORD wRdWrAccess, _WORD wOpenMode);
/* ���a�����, �Ǽ���� ���2. wRdWrAccess - ��ǧǒ �ǧ¹ȇ - �� ���E,
   ������ �2� ǒ�Ǘ2���E. wOpenMode - � ���a���° ���2. ����4�����§U
   ����2��Ǘ�° ��4����aǗ����� ��a���¶:
   HWR_FILE_EXCL - ���a����� ��2��� ������E��� ���2, ��2� ��� ���, �� ����.
   HWR_FILE_OPEN - ��2� ���2� ���, �� �� �Ǽ���§U. ��2� ��° - ���a����§U.
   HWR_FILE_CREAT - �� ��, ��� � HWR_FILE_OPEN.
   HWR_FILE_TRUNC - ���2 �Ǽ���§U ���Ǘ� ��������4� �� �ǜ� ��° �� �2� ���.
   HWR_FILE_APPEND - ��» � � HWR_FILE_OPEN, �� �ǧ2� ���a���U ���2�
      ���U/������ ������� �� ����� ���2�.
   �Ǽ�a�� - Handle �2� _NULL �a� ����
*/

_WORD  HWRFileRead (_HFILE hFile, p_VOID pReadBuffer,
                              _WORD wNumberOfBytes);
/* ���� �� ���2� ����. �Ǽ�a����� ���2� ������o ���� (0 ��2� ������ ��
   ���2ǧ� �a� ���� �2� ����� ���2�.
*/

_WORD HWRFileWrite (_HFILE hFile, p_VOID pWriteBuffer,
                              _WORD wNumberOfBytes);
/* ������ � ���2 ����. �Ǽ�a����� ���2� ���������o ���� (0 ��2� ������ ��
   ������2ǧ� �a� ���� �2� ����� 4�� �� �ǧ�2�.
*/

/* _WORD HWRFileXRead (_HFILE hFile, p_VOID pReadBuffer,
                              _WORD wNumberOfBytes); */

/* _WORD HWRFileXWrite (_HFILE hFile, p_VOID pReadBuffer,
                              _WORD wNumberOfBytes); */

_BOOL HWRFileSeek (_HFILE hFile, _LONG lOffset, _WORD wOrigin);
/* ���Ǘ ���2U ���U/������ � ��2ǻ���� lOffset �� 4��ǖ� wOrigin.
   �Ǽ�a����� _TRUE ��2� ��� �a��2� ��a4�2��� � _FALSE ��2� ����.
*/

_LONG HWRFileTell (_HFILE hFile);
/* �Ǽ�a����� ¹��E �Ǽ���E ���2� �2� -1L ��2� ���� �2� �Ǽ���U ��
   ��a���2���.
*/

_BOOL  HWRFileEOF (_HFILE hFile);
/* �Ǽ�a����� �� _NULL ��2� ���2� ���U/������ �� ����� ���2� � _NULL
   � �a������4 �2����.
*/

_BOOL HWRFileClose (_HFILE hFile);
/* ���a����� ���2. �Ǽ�a����� _NULL �a� ����, _TRUE �a� ��Ȅo�.
*/

   /* Text file functions */

_HTEXT HWRTextOpen (_STR zPathName, _WORD wRdWrAccess, _WORD wOpenMode);
/* ���a�����, �Ǽ���� ���2. wRdWrAccess - ��ǧǒ �ǧ¹ȇ - �� ���E,
   ������ �2� ǒ�Ǘ2���E. wOpenMode - � ���a���° ���2. ����4�����§U
   ����2��Ǘ�° ��4����aǗ����� ��a���¶:
   HWR_TEXT_EXCL - ���a����� ��2��� ������E��� ���2, ��2� ��� ���, �� ����.
   HWR_TEXT_OPEN - ��2� ���2� ���, �� �� �Ǽ���§U. ��2� ��° - ���a����§U.
   HWR_TEXT_CREAT - �� ��, ��� � HWR_TEXT_OPEN.
   HWR_TEXT_TRUNC - ���2 �Ǽ���§U ���Ǘ� ��������4� �� �ǜ� ��° �� �2� ���.
   HWR_TEXT_APPEND - ��» � � HWR_TEXT_OPEN, �� �ǧ2� ���a���U ���2�
      ���U/������ ������� �� ����� ���2�.
   �Ǽ�a�� - Handle �2� _NULL �a� ����
*/

#if HWR_SYSTEM == HWR_MACINTOSH || HWR_SYSTEM == HWR_ANSI

#define hwr_stdin  hwr_getstdin()
#define hwr_stdout hwr_getstdout()
#define hwr_stderr hwr_getstderr()

_HTEXT hwr_getstdin(_VOID);
_HTEXT hwr_getstdout(_VOID);
_HTEXT hwr_getstderr(_VOID);

_WORD  _FVPREFIX HWRTextPrintf (_HTEXT hText, _STR pFormatString, ... );
/* ������� printf. �Ǽ�a����� ���2� ���������o ��4��2Ǘ, � �a� ���� -
   HWR_TEXT_EOF.
*/

_WORD _FVPREFIX HWRTextSPrintf (p_CHAR pcBuffer, _STR pFormatString, ... );
/* ������� sprintf. �Ǽ�a����� ���2� ���������o ��4��2Ǘ, � �a� ���� -
   HWR_TEXT_EOF.
*/
#else 
#define HWRTextSPrintf sprintf
#define HWRTextPrintf  fprintf
#endif /* HWR_SYSTEM */
/* _WORD    _FVPREFIX HWRTextScanf (_HTEXT hText, _STR pFormatString, ... ); */
/* �� �� a�����.
*/

/* _WORD HWRTextGetLine (_HTEXT hText, p_VOID pBuffer); */
/* ���µ�U ������ ��ǒ�� - ����2���� HWRTextGetS.
*/

_WORD HWRTextGetC (_HTEXT hText);
/* ��ǖ ��4��2�. �a����ǖ�§U �a���2U��U \r\n. �Ǽ�a����� ��������� ��4��2,
   � �a� ���� HWR_TEXT_EOF.
*/

_WORD HWRTextPutC (_WORD wChar, _HTEXT hText);
/* ���ǖ ��4��2�. �a����ǖ�§U �a���2U��U \r\n. �Ǽ�a����� ���������� ��4��2,
   � �a� ���� HWR_TEXT_EOF.
*/

_STR HWRTextGetS (_STR zStr, _WORD wMaxSize, _HTEXT hText);
/* ��ǖ ��a���. �a����ǖ�§U �a���2U��U \r\n. wMaxSize - a��4�a ����a� �2U
   �a��4� ��a��� (�� ������ � \0, ����a�� ������ �ǒ��2U�§U � ����� -
   �2U ���� �ǻ� ���� 4����). �Ǽ�a����§U _NULL �a� ���� � zStr �a�
   �����. ����� ��4!
*/

_BOOL HWRTextPutS (_STR zStr, _HTEXT hText);
/* ���ǖ ��a���. �a����ǖ�§U �a���2U��U \r\n. �Ǽ�a����§U _NULL �a�
   ���� � _TRUE �a� ��Ȅo�.
*/

_BOOL HWRTextEOF (_HTEXT hText);
/* �Ǽ�a����� �� _NULL ��2� ���2� ���U/������ �� ����� ���2� � _NULL
   � �a������4 �2����.
*/

_HSEEK HWRTextTell (_HTEXT hText);
/* ��a������� ¹��E �Ǽ���E ���2� �2U �ǜ�, ��ǒ� � ������4 ��ȇ�°
   ¹�� �� (��2��� ¹�� ��!). �Ǽ�a����� _NULL �a� ���� � ����� handle
   �a� ��Ȅo�. ���� handle 4ǻ�� ����2��Ǘ�° ��2��� � ���µ�� HWRTextSeek.
*/

_BOOL HWRTextSeek (_HTEXT hText, _HSEEK hSeek);
/* �����2����� ���2 � �Ǽ���E ��a���2U�4�E hSeek. hSeek - 2��� a���2��
   ����2����U HWRTextTell, 2� �� ǖ�� �� ������ HWR_TEXT_SEEK_BEGIN
   �2� HWR_TEXT_SEEK_END. ��a��� ���4����, ��� � ������
   HWR_FILE_SEEK_END � HWR_TEXT_SEEK_END ��oǻ�� �4���, �� �Ǘ�aۄ���
   a����� �4��2 �, ����§����, a���Ǆ ����a����� �������� � ���.
*/

_BOOL HWRTextClose (_HTEXT hText);
/* ���a����� ���2. �Ǽ�a����� _NULL �a� ����, _TRUE �a� ��Ȅo�.
*/


typedef struct {
    _STR    zFileName;
    _LONG   lFileSize;
    } _FileFind, _PTR p_FileFind;

_HFIND HWRFileFindOpen (_STR zPathName, _WORD wAttr);
p_FileFind  HWRFileFind (_HFIND hFind);
_BOOL  HWRFileFindClose (_HFIND hFind);

#endif  /*  FILE_DEFINED  */


