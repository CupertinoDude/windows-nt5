/*
�]���Ǘ��N���X
�iNP2)�@Sachiko Yasukawa
*/
#include "stdafx.h"

#include "includes.h"
#include "classlib.h"

#ifdef TEST
IMBoardListItem *pbuffers1[20];
IMBoardListItem *pbuffers2[20];
#endif 

#define ISSETDMABUFFER (m_LinerAdd == NULL) ? FALSE : ((m_PhysAdd == NULL) ? FALSE : ((m_DMABufferSize == 0) ? FALSE : TRUE))
#define ISENDTRANSFERINIT (m_pStreamObject == NULL) ? FALSE : TRUE

BOOL CTransfer::Init( void )
{
	m_pStreamObject = NULL; 
	m_DMABufferSize = 0;
	m_pTopEventList = NULL; 
	m_pLastEventList = NULL; 

	m_pTopQueuedMPEGBuffer = NULL;
	m_pLastQueuedMPEGBuffer = NULL;
	m_pNextTransferMPEGBuffer = NULL;
	m_TopPagePoint = 1;
	m_LastPagePoint = 1;

	//�f�o�b�O�p
	BufCount = 0;
	StreamCount = 0;
	return TRUE;
};

//�f�X�g���N�^�@����������o�b�t�@���c���Ă�����������
CTransfer::~CTransfer()
{
	CTransferStreamList *pDeleteStreamList;

	for( ;m_pStreamObject; ){
		pDeleteStreamList = m_pStreamObject;
		m_pStreamObject = (CTransferStreamList *)m_pStreamObject->GetNext();
		delete pDeleteStreamList;
		//�f�o�b�O�p
		StreamCount--;
		ASSERT(StreamCount>=0);
	}
	ASSERT(StreamCount == 0);
//	DBG_PRINTF(("CLASSLIB:StreamCount = %d\n", StreamCount));
}

//�C�x���g�I�u�W�F�N�g���Z�b�g����B
BOOL CTransfer::SetSink(IMPEGBoardEvent *pEvent)
{
	ASSERT(pEvent);
	
	// �󂯎�����C�x���g�I�u�W�F�N�g��Next��������
	pEvent->SetNext( NULL );

	//�����ƈ�����Ȃ���B
	//�C�x���g���X�g�ɒǉ�����
	if(m_pTopEventList == NULL)
		m_pTopEventList = m_pLastEventList = (IMBoardListItem *)pEvent;//�����̓L���X�g������
	else{
		m_pLastEventList->SetNext((IMBoardListItem *)pEvent);//�����̓L���X�g������
		m_pLastEventList = (IMBoardListItem *)pEvent;//�����̓L���X�g������
	}
	
	return TRUE;
}

//�C�x���g�I�u�W�F�N�g���폜����BAdd by Nakamura
BOOL CTransfer::UnSetSink(IMPEGBoardEvent *pEvent)
{
	ASSERT(pEvent);
	//�C�x���g���X�g����폜����B

	// �C�x���g���X�g�����݂��Ȃ��ꍇ�́A�G���[
	if(m_pTopEventList == NULL || m_pLastEventList == NULL )
		return FALSE;

	// �P�����o�^����Ă��Ȃ��ꍇ
	if( m_pTopEventList == pEvent && m_pLastEventList == pEvent )
	{
		m_pTopEventList = m_pLastEventList = NULL;
		return TRUE;
	};

	// �Q�ȏ�o�^����Ă��āA�擪�̃I�u�W�F�N�g���폜�g�p�Ƃ���ꍇ
	if( m_pTopEventList == pEvent && m_pLastEventList != pEvent )
	{
		m_pTopEventList = m_pTopEventList->GetNext();
		return TRUE;
	};

	// �Q�ȏ�o�^����Ă��āA�Q�ڈȍ~���폜���悤�Ƃ��Ă���ꍇ
	IMBoardListItem *pTmpEvent;
	for( pTmpEvent = m_pTopEventList; pTmpEvent != NULL; pTmpEvent = pTmpEvent->GetNext() )
	{
		// ���X�g��̍폜�ΏۃC�x���g�I�u�W�F�N�g�Ɠ������H
		if( pTmpEvent->GetNext() == pEvent )
		{
			// �폜���悤�Ƃ��Ă���I�u�W�F�N�g���A�Ō�ł���ꍇ
			if( pTmpEvent->GetNext() == m_pLastEventList )
				m_pLastEventList = pTmpEvent;
			
			// ���X�g�̒���Ȃ���
			pTmpEvent->SetNext( pTmpEvent->GetNext()->GetNext() );

			return TRUE;
		};
	};

	DBG_PRINTF(("CLASSLIB: CALLED CTransfer::UnSetSinkError!!!\n"));
	DBG_BREAK();
	return FALSE;
}

//DMABuffer��ݒ肷��B
BOOL CTransfer::SetDMABuffer(DWORD size, BYTE* LinerAdd, BYTE *PhysAdd)
{
	ASSERT(LinerAdd);
	ASSERT(PhysAdd);

/*	
	if(!ISENDTRANSFERINIT)
		return MBC_NOTINITIALIZE;
	
	m_LinerAdd = LinerAdd;
	m_PhysAdd = PhysAdd;
	m_DMABufferSize = size;
*/
	return TRUE;
}

//Queue�ɓ����ĂȂ����̂������B
BOOL CTransfer::EnQueue( IMPEGBuffer *pBuffer )
{
	ASSERT( pBuffer != NULL );

	pBuffer->SetNext( NULL );

	//�]�����ׂ��o�b�t�@�ɒǉ�
	if(m_pTopQueuedMPEGBuffer == NULL){
		m_pTopQueuedMPEGBuffer = m_pLastQueuedMPEGBuffer = m_pNextTransferMPEGBuffer = pBuffer;
		m_TopPagePoint = 1;
		m_LastPagePoint = 1;
//		DBG_PRINTF(("CLASSLIB: m_pTopQueuedTOPMPEGBuffer = 0x%X\n", (DWORD)m_pTopQueuedMPEGBuffer));
//		DBG_PRINTF(("CLASSLIB: m_pLastQueuedTOPMPEGBuffer = 0x%X\n", (DWORD)m_pLastQueuedMPEGBuffer));
//		DBG_PRINTF(("CLASSLIB: PageSize = %d\n", pBuffer->GetPageNum()));
	}
	else{
		m_pLastQueuedMPEGBuffer->SetNext(pBuffer);
		m_pLastQueuedMPEGBuffer = pBuffer;
		if( m_pNextTransferMPEGBuffer == NULL )
		{
			m_pNextTransferMPEGBuffer = m_pLastQueuedMPEGBuffer;
			m_LastPagePoint = 1;
		};
//		DBG_PRINTF(("CLASSLIB: m_pTopQueuedTOPMPEGBuffer = 0x%X\n", (DWORD)m_pTopQueuedMPEGBuffer));
//		DBG_PRINTF(("CLASSLIB: m_pLastQueuedTOPMPEGBuffer = 0x%X\n", (DWORD)m_pLastQueuedMPEGBuffer));
//		DBG_PRINTF(("CLASSLIB: PageSize = %d\n", pBuffer->GetPageNum()));
	}

//	DBG_PRINTF(("CLASSLIB: CALLED CTransfer::EnQueue\n"));
	
	if(!ISENDTRANSFERINIT){
		DBG_PRINTF(("CLASSLIB:CTRANSFER::ENQUEUE:NOT INTIALIZE CTRANSFER\n"));
		DBG_BREAK();
		return FALSE;
	}

	return TRUE;
}

//Queue����Ƃ肾���ē]������
BOOL CTransfer::DeQueue()
{
	DWORD PageNum;
	CBaseStream *pStream;
	CTransferStreamList *pStreamList;
	IHALStreamControl *pHALStream;
	DWORD pLinear, pPhys, Flag;
	int BufSize;

//	DBG_PRINTF(("CLASSLIB: CALLED CTransfer::DeQueue\n"));
	
	if(!ISENDTRANSFERINIT){
		DBG_PRINTF(("CLASSLIB:CTRANSFER::DEQUEUE:NOT INTIALIZE CTRANSFER\n"));
		DBG_BREAK();
		return FALSE;
	}

	//pStream = m_pStreamObject->GetBaseStrem();
	//DMA�o�b�t�@�������ꍇ�̏���
	if(ISSETDMABUFFER){
		
	}
	//DMA�o�b�t�@���g��Ȃ��ꍇ�̏���
	else{
		for(pStreamList = m_pStreamObject; pStreamList; pStreamList = (CTransferStreamList *)pStreamList->GetNext()){
			pStream = pStreamList->GetBaseStream();
			ASSERT(pStream);
			pHALStream = pStream->GetHALStreamControl();
			ASSERT(pHALStream);
			
			DWORD QueueNum;
			HALRESULT st;
			DWORD Count;
			
			//�ϊ�����ă��X�g�ɓ����Ă�o�b�t�@�����ۂɃf�[�^�]��
			if((st = pHALStream->GetAvailableQueue( &QueueNum )) != HAL_SUCCESS){
				_RPT0(_CRT_WARN, "CAN'T GETAVAILABLEQUEUE\n");
				DBG_PRINTF(("CLASSLIB:CTransfer::DeQueue:CAN'T GETAVAILABLEQUEUE\n"));
				DBG_BREAK();
				return FALSE;
			}
			for(Count = 0; Count < QueueNum ; Count++){
				IMPEGBuffer *pBuffer = NULL;
				if( GetNextTransferPoint( &pBuffer, &PageNum ) == FALSE )
					return TRUE;

				if(!pBuffer->GetPagePointer(PageNum, &pLinear, &pPhys)){
					return FALSE;
				}
				//CClassLibBuffer����邽�߂ɃT�C�Y���擾
				BufSize = pBuffer->GetPageSize(PageNum);
				//CClassLibBuffer����邽�߂�flag���擾
				if(PageNum == 1)
					Flag = pBuffer->GetBufferFlag();
				else
					Flag = 0; //�����̓R�[�f�B���O������
				//�V�����o�b�t�@�Ǘ��N���X�����
				CClassLibBuffer *pBuf = m_Mem.Alloc(pBuffer, pStream, PageNum, BufSize, pPhys, pLinear, Flag);//�f�[�^��ݒ�
				
				if(!pBuf){
					_RPTF0(_CRT_WARN, "CAN'T New");
					DBG_PRINTF(("CLASSLIB:CTRANSFER::ENQUEUE:CAN'T NEW\n"));
					DBG_BREAK();
					return FALSE;
				}

		//�f�o�b�O�p
		BufCount++;

//				DBG_PRINTF(("CLASSLIB: Dequeue: Senddata(%d) pBuffer =  0x%X  IMPEGBuff=0x%x\n", BufCount, (DWORD)pBuf,pBuffer ));
				if((st = pHALStream->SendData(pBuf)) != HAL_SUCCESS){
					_RPT0(_CRT_WARN, "CAN'T SENDDATA TO HAL\n");
					DBG_PRINTF(("CLASSLIB:CTransfer::DeQueue:CAN'T SENDDATA TO HAL\n"));
					DBG_BREAK();
					return FALSE;
				}

			}
		}
	}
	return TRUE;
}
//Transfer�I�u�W�F�N�g�ɃX�g���[����ǉ�����B
BOOL CTransfer::AddStreamObject(IBaseStream *INewStreamObject)
{
	//CBaseStream *pBSt1, *pBSt2;
	CTransferStreamList *pNewTransferStreamList, *pStreamList;

	ASSERT(INewStreamObject);
	
	pNewTransferStreamList = new CTransferStreamList((CBaseStream *)INewStreamObject);
	
	if(!pNewTransferStreamList){
		DBG_PRINTF(("CLASSLIB:CTRANSFER::AddStreamObject:CAN'T NEW\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	//�f�o�b�O�p
	StreamCount++;

	//�܂�����X�g���[�����ǉ�����Ă��Ȃ��B
	if(m_pStreamObject == NULL){
		m_pStreamObject = pNewTransferStreamList;
		//pBSt1 = m_pStreamObject->GetBaseStream();
		//pHALStream = m_pBSt1->GetHALStreamControl();
		m_pIHAL = m_pStreamObject->GetBaseStream()->GetClassLibHAL();
		m_EndOfTransferEvent.SetEventType(ClassLibEvent_SendData);
		m_EndOfTransferEvent.SetTransfer(this);
		m_pIHAL->SetSinkClassLib((IMPEGBoardEvent *)/*(CClassLibEvent *)*/&m_EndOfTransferEvent);
	}
	//�V�����X�g���[����ǉ�
	else{
		//pBSt1 = pNewTransferStreamList;
		for(pStreamList = m_pStreamObject; pStreamList->GetNext() != NULL; pStreamList = (CTransferStreamList *)pStreamList->GetNext());
		pStreamList->SetNext((IMBoardListItem *)pNewTransferStreamList);
	}
	
	return TRUE;
}
//Transfer�I�u�W�F�N�g����X�g���[�����������B
BOOL CTransfer::ReleaseStreamObject(IBaseStream *IStreamObject)
{
	CTransferStreamList *pStreamList, *pNextStreamList;
	CBaseStream *pBaseStream;
	
	ASSERT(IStreamObject);

	//������ׂ��X�g���[�����Ȃ��̂ŃG���[
	if(m_pStreamObject == NULL){
		_RPTF0(_CRT_WARN, "CTransfer:THERE IS NO STREAM WHICH SHOULD BE RELEASED\n");
		DBG_PRINTF(("CLASSLIB:CTRANSFER::ReleaseStreamObject:THERE IS NO STREAM WHICH SHOULD BE RELEASED\n"));
		DBG_BREAK();
		return FALSE;
	}
	else{
		for(pStreamList = m_pStreamObject; ; pStreamList = (CTransferStreamList *)pStreamList->GetNext()){
			if((pNextStreamList = (CTransferStreamList *)pStreamList->GetNext()) == NULL){
				_RPTF0(_CRT_WARN, "CTransfer:THERE IS SUCH A STREAM WHICH SHOULD BE RELEASED\n");
				DBG_PRINTF(("CLASSLIB:CTRANSFER::ReleaseStreamObject:THERE IS SUCH A STREAM WHICH SHOULD BE RELEASED\n"));
				DBG_BREAK();
				return FALSE;
			}
			if((pBaseStream = (CBaseStream *)pNextStreamList->GetBaseStream()) == (CBaseStream *)IStreamObject){
				break;
			}
			ASSERT(pBaseStream);
		}
		pStreamList->SetNext(pNextStreamList->GetNext());
		delete pNextStreamList;
		//�f�o�b�O�p
		StreamCount--;
		ASSERT(StreamCount>=0);
	}
	return TRUE;
}


//Queue���t���b�V������B
void CTransfer::Flush()
{
	CTransferStreamList *pStreamList;
	CBaseStream *pBaseStream;
	IMPEGBoardEvent *pEvent;

	DBG_PRINTF(("CLASSLIB:Before flash BufCount = %d\n", BufCount));

	m_Mem.Flush();
	BufCount=0;
	
	for(pStreamList = m_pStreamObject; pStreamList; pStreamList  = (CTransferStreamList *)pStreamList->GetNext()){
		pBaseStream = pStreamList->GetBaseStream();
		DWORD PageNum;
		ASSERT(pBaseStream);

		IMPEGBuffer *pBuffer = NULL;
		BOOL NeedAdvice = FALSE;

		// Stream��Queue�Ɏ����Ă���o�b�t�@�����ׂāA�]�����Ƀ}�[�N����B
		while( TRUE )
		{
			if( GetNextTransferPoint( &pBuffer, &PageNum ) == FALSE )
				break;
		}

		while( HasQueuedBuffer() == TRUE )
		{
			NeedAdvice = FreeTopTransferPoint(&pBuffer);
			if( NeedAdvice == TRUE )
			{
				for(pEvent = (IMPEGBoardEvent *)m_pTopEventList; pEvent != NULL;pEvent = (IMPEGBoardEvent *)((IMBoardListItem *)pEvent)->GetNext())
				{
					if((/*(CClassLibEvent *)*/pEvent)->GetEventType() == ClassLibEvent_SendData){
						//MPEGBuffer�̐擪���ЂƂ�����ɂ��炷
						//Wrapper�ɂ����g���I��������Ƃ�m�点��
						pEvent->Advice(pBuffer);
					}
				}
			};
		}
	}
	DBG_PRINTF(("CLASSLIB:After flush BufCount = %d\n", BufCount));
	ASSERT( BufCount == 0 );
}

//�f�[�^�]���I���̏���
BOOL CTransfer::EndOfTransfer(CClassLibBuffer *pBuffer)
{
	IMPEGBoardEvent *pEvent;
	CBaseStream *pStream;
	CTransferStreamList *pStreamList;
		
//	DBG_PRINTF(("CLASSLIB: EndOfTransfer(%d) pBuffer =  0x%X\n", BufCount, (DWORD)pBuffer));
//	DBG_PRINTF(("CLASSLIB: pBuffer->GetIMPEGBuffer() = 0x%X\n", (DWORD)pBuffer->GetIMPEGBuffer()));

	ASSERT(pBuffer);
	pStream = pBuffer->GetStream();
	ASSERT(pStream);
	
	IMPEGBuffer *pFree = NULL;
	if( FreeTopTransferPoint(&pFree) == TRUE )
	{
//		DBG_PRINTF(("CLASSLIB: Free IMPEGBuffer() = 0x%X\n", (DWORD)pFree ));
		ASSERT( pFree == pBuffer->GetIMPEGBuffer()  );
		//Wrapper����n���ꂽ�o�b�t�@�̍Ō�̃y�[�W�Ȃ�o�b�t�@�[�J��
		for(pEvent = (IMPEGBoardEvent *)m_pTopEventList; pEvent != NULL;pEvent = (IMPEGBoardEvent *)((IMBoardListItem *)pEvent)->GetNext()){
			if(pEvent->GetEventType() == ClassLibEvent_SendData){
				//Wrapper�ɂ����g���I��������Ƃ�m�点��
//				DBG_PRINTF(("CLASSLIB: EndOfTransfer Advice Buffer =  0x%X\n", pBuffer->GetIMPEGBuffer() ));
				pEvent->Advice(pBuffer->GetIMPEGBuffer());
			}
		}
	}
	//���X�g����͂�����CClassLibBuffer�����
	m_Mem.Free(pBuffer);
	//�f�o�b�O�p
	BufCount--;
	ASSERT(BufCount>=0);

	//STOP��Ԃ̂Ƃ���HAL�ɑ΂���SendData���Ȃ�
	for(pStreamList = m_pStreamObject; pStreamList; pStreamList = (CTransferStreamList *)pStreamList->GetNext()){
			pStream = pStreamList->GetBaseStream();
			ASSERT(pStream);
			if(pStream->GetState() == Stop)
				return TRUE;
	}

	if(DeQueue() == FALSE){
		DBG_PRINTF(("CLASSLIB:CTRANSFER::EndOfTransfer:CAN'T DEQUEUE\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}


BOOL	CTransfer::HasQueuedBuffer( void )
{
	if( m_pTopQueuedMPEGBuffer == NULL )
		return FALSE;
	return TRUE;
};


BOOL	CTransfer::GetNextTransferPoint( IMPEGBuffer **Point, DWORD *Page )
{
	if( m_pNextTransferMPEGBuffer == NULL )
	{
		*Point = NULL;
		*Page = 0;
		return FALSE;
	};

	*Point = m_pNextTransferMPEGBuffer;
	*Page = m_LastPagePoint;

	if( m_pNextTransferMPEGBuffer->GetPageNum() == m_LastPagePoint )
	{
		m_pNextTransferMPEGBuffer = (IMPEGBuffer *)m_pNextTransferMPEGBuffer->GetNext();
		m_LastPagePoint = 1;
		return TRUE;
	};

	m_LastPagePoint ++;
	return TRUE;
};

BOOL	CTransfer::FreeTopTransferPoint( IMPEGBuffer **Free )
{
	ASSERT( m_pTopQueuedMPEGBuffer != NULL );

	if( m_pTopQueuedMPEGBuffer == NULL 
		|| ( m_pTopQueuedMPEGBuffer == m_pNextTransferMPEGBuffer && m_LastPagePoint == m_TopPagePoint ) )
	{
		*Free = NULL;
		return FALSE;
	};

	if( m_TopPagePoint == m_pTopQueuedMPEGBuffer->GetPageNum() )
	{
		*Free = m_pTopQueuedMPEGBuffer;
		m_pTopQueuedMPEGBuffer = (IMPEGBuffer *)m_pTopQueuedMPEGBuffer->GetNext();
		m_TopPagePoint = 1;
		return TRUE;		// Need Advice
	};

	*Free = NULL;
	m_TopPagePoint ++;

	return FALSE;		// Not Need Advice
	
};


//HAL�ɂ��ł��炢����
void CClassLibEvent::Advice(PVOID pBuffer)
{
	ASSERT(m_pTransfer);
	m_pTransfer->EndOfTransfer((CClassLibBuffer *)pBuffer);
}
