//***************************************************************************
//
//   Title      : CCData.h
//
//   Date       : 1998.03.26       1st making
//
//   Author     : Toshiba [NPC](NP2) Hiroyuki Oka
//
//   Copyright 1998  Toshiba Corporation.  All Rights Reserved.
//
// -------------------------------------------------------------------------
//
//   Change Log :
//
//      Date      Revision                 Description
//  ------------ ---------- -----------------------------------------------
//
//***************************************************************************
// $Header: /DVD Drivers/Sources/ZiVAHAL/userdata.h 1     98/04/20 7:30p Hero $
// $Modtime: 98/03/30 7:13p $
// $Nokeywords: $
//***************************************************************************

#ifndef _CCDATA_H_
#define _CCDATA_H_

// 1GOP����line21�f�[�^�̈�̊m��(header + 36field * data_size + alpha)
// data_size�ɉ����ăf�[�^�̈���m�ۂ����ق�����������
#define USER_DATA_SIZE (5+36*3+10)

// �N���[�Y�h�L���v�V�����p�����O�o�b�t�@�̃T�C�Y(DWORD)
#define	CC_DATA_SIZE	90

class CUserData
{
public:
	CUserData(void);
	~CUserData(void);

	BOOL	Init();
	BOOL	Set(DWORD data);
	BYTE	Get(DWORD number);
	BOOL	DataCopy(PVOID pDestPoint, DWORD dwSize);
	DWORD	GetDataSize();
private:
	BYTE data[USER_DATA_SIZE];
	DWORD	m_DataSize;
	DWORD	m_DataPoint;

//friend CMPEGBoardHAL;
};

#endif // _CCDATA_H_