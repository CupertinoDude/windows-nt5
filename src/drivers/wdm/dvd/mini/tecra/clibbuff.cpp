/*
�@�N���X���C�u���������o�b�t�@�iHAL�ɓn���j�̊Ǘ��N���X
�@�iNP2)�@Sachiko Yasukawa
*/
#include "stdafx.h"

#include "includes.h"
#include "classlib.h"

CClassLibBuffer::CClassLibBuffer( void )
{
	m_pNext = NULL;
	m_pStream = NULL;
	m_pMPEGBoardBuffer = NULL;
	m_fEnd = FALSE;
};

CClassLibBuffer::CClassLibBuffer(IMBoardListItem *pBuffer, CBaseStream *pStream, DWORD PageNum)
{
	m_pNext = NULL;
	m_PageNum = PageNum;
	m_pMPEGBoardBuffer = (IMPEGBuffer *)pBuffer;
	m_pStream = pStream;
	m_fEnd = FALSE;
};

CClassLibBuffer::CClassLibBuffer(IMBoardListItem *pBuffer, CBaseStream *pStream, DWORD PageNum, DWORD Size, DWORD Add, DWORD LinAdd, DWORD flag)
{
	m_pNext = NULL;
	m_PageNum = PageNum;
	m_Size = Size;
	m_Add = (UCHAR *)Add;
	m_LinAdd = (UCHAR *)LinAdd;
	m_flag = flag;
	m_pMPEGBoardBuffer = (IMPEGBuffer *)pBuffer;
	m_pStream = pStream;
	m_fEnd = FALSE;
};

void CClassLibBuffer::SetParam(IMBoardListItem *pBuffer, CBaseStream *pStream, DWORD PageNum, DWORD Size, DWORD Add, DWORD LinAdd, DWORD flag)
{
	m_pNext = NULL;
	m_PageNum = PageNum;
	m_Size = Size;
	m_Add = (UCHAR *)Add;
	m_LinAdd = (UCHAR *)LinAdd;
	m_flag = flag;
	m_pMPEGBoardBuffer = (IMPEGBuffer *)pBuffer;
	m_pStream = pStream;
	m_fEnd = FALSE;
};

//�T�C�Y�̎擾
DWORD CClassLibBuffer::GetSize()
{
//	ASSERT(m_pMPEGBoardBuffer);

//	return m_pMPEGBoardBuffer->GetPageSize(m_PageNum);
//	DBG_PRINTF(("CLASSLIB: ClassLibBufSize = %u\n", m_Size));
	return m_Size;
}

//�t���O�̎擾
DWORD CClassLibBuffer::Flags()
{
	//ASSERT(m_pMPEGBoardBuffer);

	//return m_pMPEGBoardBuffer->GetBufferFlag();
//	DBG_PRINTF(("CLASSLIB: ClassLibBufFlag = %u\n", m_flag));
	return m_flag;

}

//�o�b�t�@�ւ̃|�C���^
BYTE *CClassLibBuffer::GetBuffPointer()
{
	//ASSERT(m_pMPEGBoardBuffer);

	//m_pMPEGBoardBuffer->GetPagePointer(m_PageNum, &pLinear, &pPhys);
	//return (BYTE *)pPhys;

//	DBG_PRINTF(("CLASSLIB: ClassLibBufPointer = 0x%X\n", (DWORD)m_Add));
	return m_Add;
}

//�o�b�t�@�ւ̃|�C���^�����j�A�A�h���X�ł�����
BYTE *CClassLibBuffer::GetLinBuffPointer()
{
	return m_LinAdd;
}
