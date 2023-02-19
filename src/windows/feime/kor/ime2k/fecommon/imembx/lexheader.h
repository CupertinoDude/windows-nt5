#if !defined(_LEXHEADER_H__INCLUDED_)
#define _LEXHEADER_H__INCLUDED_

#define DICT_HEADER_SIZE 512
#define COPYRIGHT_STR "(C) 1997 Hangul Engineering Team. Microsoft Corp. All rights reserved.\n"
#define LEX_VERSION					0x1100
#define LEX_COMPATIBLE_VERSION_LIMIT	0x2000
#define LEX_FILE_NAME TEXT("IMEKR.LEX")

#define TOTAL_NUMBER_OF_HANGUL_MAPPING	(484 + 18) // K0+K1 + # of symbols(�� - ��)
#define TOTAL_NUMBER_OF_HANJA			7744
#define MAX_NUMBER_OF_HANJA_SAME_PRONUNC 103
#define MAX_SENSE_LENGTH	50

struct  _DictHeader {
	char	COPYRIGHT_HEADER[150];
	WORD	Version;
	UINT	NumOfHangulEntry;
	UINT	MaxNumOfHanja;
	DWORD	Headersize;
	DWORD	iBufferStart;	      // seek point of HnagulToHanja Index
	UINT	uiNumofHanja;
	DWORD   iHanjaToHangulIndex;  // seek point of HanjaToHangul Index
	DWORD	reserved[10];
	_DictHeader() { 
		Version = 0;
		iBufferStart = Headersize= 0;

		memset(reserved, '\0', sizeof(reserved));
		memset(COPYRIGHT_HEADER, '\0', sizeof(COPYRIGHT_HEADER));
		strcpy(COPYRIGHT_HEADER, COPYRIGHT_STR);
		COPYRIGHT_HEADER[strlen(COPYRIGHT_HEADER)+1] = '\032';
	}
};

struct _LexIndex {
	WCHAR	wcHangul;
	WORD	wNumOfK0, wNumOfK1;
	UINT	iOffset;

	_LexIndex() {
		wcHangul = 0;
		wNumOfK0 = wNumOfK1 = 0;
		iOffset = 0;
	}
};

struct HanjaToHangulIndex 
{
	WCHAR	wchHanja;
	WCHAR	wchHangul;
	UINT	iOffset;

	HanjaToHangulIndex() 
	{
		wchHanja = wchHangul = 0;
		iOffset = 0;
	}
};


#endif // !defined(_LEXHEADER_H__INCLUDED_)
