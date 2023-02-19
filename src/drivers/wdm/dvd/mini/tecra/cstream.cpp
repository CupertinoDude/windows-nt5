/*
�X�g���[���Ǘ��N���X�N���X
�iNP2)�@Sachiko Yasukawa
*/
//     Date    |   Author     |  Description
//  -----------+--------------+--------------------------------------------
//  1998.03.27 |  Hideki Yagi | Change the specification about
//             |              | Get***Property().
//  1998.03.31 |  Hideki Yagi | Change the specification about
//             |              | GetChlgKey() and GetDVDKey2().
//

#include "stdafx.h"

#include "includes.h"
#include "classlib.h"

#define ISENDSTREAMOBJECTINIT ((m_pMPEGBoardStateObject == NULL || m_pIHAL == NULL || m_pTransfer == NULL || m_pIStreamHAL == NULL ) ? TRUE : FALSE)
//�R���X�g���N�^
CBaseStream::CBaseStream()
{
	m_pNextStreamObject=NULL;
	m_pPrevStreamObject=NULL;
	m_pMPEGBoardStateObject=NULL;
	m_pIHAL=NULL;
	m_pIStreamHAL = NULL;
	m_pTransfer = NULL;
}

//�ǂ��������ɂ�΂�邩���l���ăR�[�f�B���O���Ȃ���
//������
inline BOOL CBaseStream::Init()
{
	m_pNextStreamObject=NULL;
	m_pPrevStreamObject=NULL;
	m_pMPEGBoardStateObject=NULL;
	m_pIHAL=NULL;
	m_pIStreamHAL = NULL;
	m_pTransfer = NULL;

	return TRUE; 
}

//�Đ�
BOOL CBaseStream::Play()
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::GetMPEGBuffer:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return GetIMPEGBoardState()->ChangePlay(m_pIHAL, m_pIStreamHAL);

}

//��~
BOOL CBaseStream::Stop()
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::Stop:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return GetIMPEGBoardState()->ChangeStop(m_pIHAL, m_pIStreamHAL, this);

}

//�ꎞ��~
BOOL CBaseStream::Pause()
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::Pause:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}

	return GetIMPEGBoardState()->ChangePause(m_pIHAL, m_pIStreamHAL);

}

//�R�}����
//VxD�̂Ƃ������g���Ȃ��悤�ɂ���̂́H
BOOL CBaseStream::SingleStep()
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		DBG_PRINTF(("CLASSLIB:CBaseStream::SingleStep:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	//VxD�̂Ƃ������g���Ȃ�
	if(!ISMIXSTREAMTYPE(m_StreamType)){
		_RPTF0(_CRT_WARN, "NOTIMPLEMENTED \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SingleStep:CALL ONLY FOR MIXSTREAM\n"));
		DBG_BREAK();
		return FALSE;
	}

	return GetIMPEGBoardState()->ChangePauseViaSingleStep(m_pIHAL, m_pIStreamHAL);
}

//�X���[�Đ�
//VxD�̂Ƃ������g���Ȃ��悤�ɂ���ɂ́H
BOOL CBaseStream::Slow(DWORD speed)
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::Slow:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}

	//VxD�̂Ƃ������g���Ȃ�
	if(!ISMIXSTREAMTYPE(m_StreamType)){
		_RPTF0(_CRT_WARN, "NOTIMPLEMENTED \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::Slow:CALL ONLY FOR MIXSTREAM\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return GetIMPEGBoardState()->ChangeSlow(speed, m_pIHAL, m_pIStreamHAL);
}

//�X�L����
//VxD�̂Ƃ������g���Ȃ��悤�ɂ���ɂ́H
BOOL CBaseStream::Scan(DWORD speed)
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		DBG_PRINTF(("CLASSLIB:CBaseStream::Scan:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	//VxD�̂Ƃ������g���Ȃ�
	if(!ISMIXSTREAMTYPE(m_StreamType)){
		_RPTF0(_CRT_WARN, "NOTIMPLEMENTED \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::Slow:CALL ONLY FOR MIXSTREAM\n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return GetIMPEGBoardState()->ChangeScan(speed, m_pIHAL, m_pIStreamHAL);
}

//���݂̃X�g���[���̃X�e�[�^�X��Ԃ�
LIBSTATE CBaseStream::GetState()
{
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::GetState:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		return PowerOff;
	}
	return ((CMPEGBoardState *)m_pMPEGBoardStateObject)->GetState();
}
	
//�f�[�^�]���̗v��
//���^�[���l���l����
BOOL CBaseStream::SendData(IMPEGBuffer *pBuffer)
{
	ASSERT(pBuffer);
	DBG_PRINTF(("CLASSLIB: CALLED CBaseStream::SendData\n"));

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		DBG_PRINTF(("CLASSLIB:CBaseStream::SendData:NOT INITILIZE CMPEGBoard COLLECTLY\n"));
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	// �󂯎�����o�b�t�@��Next�|�C���^��������
	pBuffer->SetNext( NULL );

	//�g�����X�t�@�[�ɃL���[�ɓ���Ă��炦��悤�v��
	if(!((CTransfer *)m_pTransfer)->EnQueue( pBuffer )){
		_RPTF0(_CRT_WARN, "CAN'T ENQUEUE \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SendData:CAN'T ENQUEUE \n"));
		DBG_BREAK();
		return FALSE;
	}

	//�g�����X�t�@�[�ɓ]���v��
	if(!((CTransfer *)m_pTransfer)->DeQueue()){
		_RPTF0(_CRT_WARN, "CAN'T DEQUEUE \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SendData:CAN'T DEQUEUE \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�X�g���[���̃X�e�[�g���Ǘ�����I�u�W�F�N�g���Z�b�g����
BOOL CBaseStream::SetStateObject(IMPEGBoardState *pMPEGBoardStateObject)
{
	ASSERT(pMPEGBoardStateObject);

	//�X�e�[�g�R���g���[���I�u�W�F�N�g
	m_pMPEGBoardStateObject = pMPEGBoardStateObject;

	return TRUE;
}

//�]�����Ǘ�����I�u�W�F�N�g�̐ݒ�
BOOL CBaseStream::SetTransferObject(ITransfer *pTransfer)
{
	ASSERT(pTransfer);
	
	//�g�����X�t�@�[�R���g���[���I�u�W�F�N�g�͂Q�x�ݒ�ł��Ȃ�
	if(m_pTransfer){
		_RPTF0(_CRT_WARN, "ALREADY SET TRANSFER OBJECT \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SetTransferObject:ALREADY SET TRANSFER OBJECT \n"));
		DBG_BREAK();
		return FALSE;
	}

	//�g�����X�t�@�[�R���g���[���I�u�W�F�N�g�ݒ�
	m_pTransfer = pTransfer;

	//�t�Ƀg�����X�t�@�[�I�u�W�F�N�g�ɑ΂��ăX�g���[����ݒ�
	((CTransfer *)m_pTransfer)->AddStreamObject((IBaseStream *)/*(CBuffer *)*/this);

	return TRUE;
}

//�]�����[�h�̐ݒ�
BOOL CBaseStream::SetTransferMode(HALSTREAMMODE StreamMode)
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->SetTransferMode(StreamMode)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TRANSFERMODE \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SetTransferMode:CAN'T SET TRANSFERMODE\n"));
		DBG_BREAK();
		return FALSE;
	}

	return TRUE;
}

// Set transfer typr & direction                98.03.31 H.Yagi
BOOL CBaseStream::SetDataDirection(DirectionType type)
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->SetDataDirection(type)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DATADIRECTION \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::SetDataDirection:CAN'T SET DATADIRECTION\n"));
		DBG_BREAK();
		return FALSE;
	}

	return TRUE;
}

// Get transfer typr & direction                98.03.31 H.Yagi
BOOL CBaseStream::GetDataDirection(DirectionType *ptype)
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
    if((st = m_pIStreamHAL->GetDataDirection(ptype)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DATADIRECTION \n");
		DBG_PRINTF(("CLASSLIB:CBaseStream::GetDataDirection:CAN'T GET DATADIRECTION\n"));
		DBG_BREAK();
		return FALSE;
	}

	return TRUE;
}


//�R�s�[�v���e�N�g�����̏�����
BOOL CVideoStream::CppInit()
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//�`�������W�L�[�̐ݒ�
BOOL CVideoStream::SetChlgKey(UCHAR *pDecoderChallenge)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDecoderChallenge(pDecoderChallenge)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�`�������W�L�[�̎擾
//UCHAR *CVideoStream::GetChlgKey()
BOOL CVideoStream::GetChlgKey( UCHAR *ptr )                 // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->GetDriveChallenge(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DRIVERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̐ݒ�
BOOL CVideoStream::SetDVDKey1(UCHAR *pDriveReponse)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDriveResponse(pDriveReponse)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DRIVERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̎擾
//UCHAR *CVideoStream::GetDVDKey2()
BOOL CVideoStream::GetDVDKey2(UCHAR *ptr)                   // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->GetDecoderResponse(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�^�C�g���L�[�̐ݒ�
BOOL CVideoStream::SetTitleKey(UCHAR *pTitleKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetTitleKey(pTitleKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TITLEKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}	

//�f�B�X�N�L�[�̐ݒ�
BOOL CVideoStream::SetDiscKey(UCHAR *pDiscKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDiskKey(pDiscKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DISCKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B���擾
BOOL CVideoStream::GetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CVideoStream::SetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�R�s�[�v���e�N�g�����̏�����
BOOL CAudioStream::CppInit()
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//�`�������W�L�[�̐ݒ�
BOOL CAudioStream::SetChlgKey(UCHAR *pDecoderChallenge)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDecoderChallenge(pDecoderChallenge)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�`�������W�L�[�̎擾
//UCHAR *CAudioStream::GetChlgKey()
BOOL CAudioStream::GetChlgKey(UCHAR *ptr)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->GetDriveChallenge(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DRIVERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̐ݒ�
BOOL CAudioStream::SetDVDKey1(UCHAR *pDriveResponse)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDriveResponse(pDriveResponse)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DRIVERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̎擾
//UCHAR *CAudioStream::GetDVDKey2()
BOOL CAudioStream::GetDVDKey2(UCHAR *ptr)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK(); 
		return FALSE;
	}

	if((st = m_pIStreamHAL->GetDecoderResponse(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�^�C�g���L�[�̐ݒ�
BOOL CAudioStream::SetTitleKey(UCHAR *pTitleKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetTitleKey(pTitleKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TITLEKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}	

//�f�B�X�N�L�[�̐ݒ�
BOOL CAudioStream::SetDiscKey(UCHAR *pDiscKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDiskKey(pDiscKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DISCKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�I�[�f�B�I�ւ���v���p�e�B���擾
BOOL CAudioStream::GetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�I�[�f�B�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CAudioStream::SetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�R�s�[�v���e�N�g�����̏�����
BOOL CSubpicStream::CppInit()
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//�`�������W�L�[�̐ݒ�
BOOL CSubpicStream::SetChlgKey(UCHAR *pDecoderChallenge)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDecoderChallenge(pDecoderChallenge)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�`�������W�L�[�̎擾
//UCHAR *CSubpicStream::GetChlgKey()
BOOL CSubpicStream::GetChlgKey(UCHAR *ptr)              // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->GetDriveChallenge(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DRIVERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̐ݒ�
BOOL CSubpicStream::SetDVDKey1(UCHAR *pDriveResponse)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDriveResponse(pDriveResponse)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DRIVERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̎擾
//UCHAR *CSubpicStream::GetDVDKey2()
BOOL CSubpicStream::GetDVDKey2(UCHAR *ptr)              // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->GetDecoderResponse(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�^�C�g���L�[�̐ݒ�
BOOL CSubpicStream::SetTitleKey(UCHAR *pTitleKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetTitleKey(pTitleKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TITLEKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}	

//�f�B�X�N�L�[�̐ݒ�
BOOL CSubpicStream::SetDiscKey(UCHAR *pDiscKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDiskKey(pDiscKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DISCKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�T�u�s�N�`���ւ���v���p�e�B���擾
BOOL CSubpicStream::GetSubpicProperty(SUBPICPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetSubpicProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�T�u�s�N�`���Ɋւ���v���p�e�B�̐ݒ�
BOOL CSubpicStream::SetSubpicProperty(SUBPICPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetSubpicProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�R�s�[�v���e�N�g�����̏�����
BOOL CVideoCDStream::CppInit()
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//�`�������W�L�[�̐ݒ�
BOOL CVideoCDStream::SetChlgKey(UCHAR *pDecoderChallenge)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDecoderChallenge(pDecoderChallenge)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�`�������W�L�[�̎擾
//UCHAR *CVideoCDStream::GetChlgKey()
BOOL CVideoCDStream::GetChlgKey(UCHAR *ptr)             // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->GetDriveChallenge(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DRIVERCHALLENGE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̐ݒ�
BOOL CVideoCDStream::SetDVDKey1(UCHAR *pDriveResponse)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDriveResponse(pDriveResponse)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DRIVERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//DVD�L�[�̎擾
//UCHAR *CVideoCDStream::GetDVDKey2()
BOOL CVideoCDStream::GetDVDKey2(UCHAR *ptr)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->GetDecoderResponse(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERRESPONSE \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�^�C�g���L�[�̐ݒ�
BOOL CVideoCDStream::SetTitleKey(UCHAR *pTitleKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetTitleKey(pTitleKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TITLEKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}	

//�f�B�X�N�L�[�̐ݒ�
BOOL CVideoCDStream::SetDiscKey(UCHAR *pDiscKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDiskKey(pDiscKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DISCKEY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B���擾
BOOL CVideoCDStream::GetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CVideoCDStream::SetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�I�[�f�B�I�ւ���v���p�e�B���擾
BOOL CVideoCDStream::GetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�I�[�f�B�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CVideoCDStream::SetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_BREAK();
		return FALSE;
	}
	
	return TRUE;
}

//�R�s�[�v���e�N�g�����̏�����
BOOL CDVDStream::CppInit()
{
	HALRESULT st;

	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->CPPInit()) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T Initialize CPP\n");
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//�`�������W�L�[�̐ݒ�
BOOL CDVDStream::SetChlgKey(UCHAR *pDecoderChallenge)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetChlgKey:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDecoderChallenge(pDecoderChallenge)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DECORDERCHALLENGE \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetChlgKey:CAN'T SET DECORDERCHALLENGE \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::SetChlgKey:SUCCESS SetDecoderChallenge\n"));
	return TRUE;
}

//�`�������W�L�[�̎擾
//UCHAR CDVDStream::GetChlgKey()
BOOL CDVDStream::GetChlgKey(UCHAR *ptr)                 // 98.03.31 H.Yagi
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetChlgKey:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}
	if((st = m_pIStreamHAL->GetDriveChallenge(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DRIVERCHALLENGE \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetChlgKey:CAN'T GET DRIVERCHALLENGE \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::GetChlgKey:SUCCESS GetDriveChallenge \n"));
	return TRUE;
}

//DVD�L�[�̐ݒ�
BOOL CDVDStream::SetDVDKey1(UCHAR *pDriveResponse)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetDVDKey1:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDriveResponse(pDriveResponse)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DRIVERRESPONSE \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetDVDKey1:CAN'T SET DRIVERRESPONSE \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::GetChlgKey:SUCCESS SetDriveResponse \n"));
	return TRUE;
}

//DVD�L�[�̎擾
//UCHAR *CDVDStream::GetDVDKey2()
BOOL CDVDStream::GetDVDKey2(UCHAR *ptr)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetDVDKey2:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->GetDecoderResponse(ptr)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET DECORDERRESPONSE \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetDVDKey2:CAN'T GET DECORDERRESPONSE \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::GetChlgKey:SUCCESS GetDecoderResponse \n"));
	return TRUE;
}

//�^�C�g���L�[�̐ݒ�
BOOL CDVDStream::SetTitleKey(UCHAR *pTitleKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetTitleKey:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetTitleKey(pTitleKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET TITLEKEY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetTitleKey:CAN'T SET TITLEKEY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::SetTitleKey:SUCCESS SetTitleKey\n"));
	return TRUE;
}	

//�f�B�X�N�L�[�̐ݒ�
BOOL CDVDStream::SetDiscKey(UCHAR *pDiscKey)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetDiscKey:NOT INITILIZE CMPEGBoard \n"));
		DBG_BREAK();
		return FALSE;
	}

	if((st = m_pIStreamHAL->SetDiskKey(pDiscKey)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SET DISCKEY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetDiskKey:CAN'T SET DISCKEY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:CDVDStream::SetDiscKey:SUCCESS SetDiskKey\n"));
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B���擾
BOOL CDVDStream::GetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetVideoProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetVideoProperty:CAN'T GET VIDEOPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS GETVIDEOPROPERTY\n"));
	return TRUE;
}

//�r�f�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CDVDStream::SetVideoProperty(VIDEOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetVideoProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetVideoProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetVideoProperty:CAN'T SET VIDEOPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS SETVIDEOPROPERTY\n"));
	return TRUE;
}

//�I�[�f�B�I�ւ���v���p�e�B���擾
BOOL CDVDStream::GetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//    PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetAudioProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetAudioProperty:CAN'T GET AUDIOPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS GETAUDIOPROPERTY\n"));
	return TRUE;
}

//�I�[�f�B�I�Ɋւ���v���p�e�B�̐ݒ�
BOOL CDVDStream::SetAudioProperty(AUDIOPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetAudioProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetAudioProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetAudioProperty:CAN'T SET AUDIOPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS SETAUDIOPROPERTY\n"));
	return TRUE;
}

//�T�u�s�N�`���ւ���v���p�e�B���擾
BOOL CDVDStream::GetSubpicProperty(SUBPICPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
//	PVOID pProperty;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetSubpicProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetSubpicProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetSubpicProperty:CAN'T GET SUBPICPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS GETSUBPICPROPERTY\n"));
	return TRUE;
}

//�T�u�s�N�`���Ɋւ���v���p�e�B�̐ݒ�
BOOL CDVDStream::SetSubpicProperty(SUBPICPROPTYPE PropertyType, PVOID pProperty)
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetSubpicProperty:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->SetSubpicProperty(PropertyType, pProperty)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetSubpicProperty:CAN'T SET SUBPICPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS SETSUBPICPROPERTY\n"));
	return TRUE;
}

//Get Property capability                                 98.04.03 H.Yagi
BOOL CDVDStream::GetCapability( CAPSTYPE PropType, DWORD *pPropType )
{
	HALRESULT st;
	
	//�����ݒ肪�I����ĂȂ�
	if(ISENDSTREAMOBJECTINIT){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::GetCapability:NOT INITIALIZE CDVDSTREAM \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	if((st = m_pIHAL->GetCapability( PropType, pPropType)) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GET PROPERTY \n");
		DBG_PRINTF(("CLASSLIB:CDVDStream::SetSubpicProperty:CAN'T SET SUBPICPROPERTY \n"));
		DBG_BREAK();
		return FALSE;
	}
	
	DBG_PRINTF(("CLASSLIB:SUCCESS GetCapability\n"));
	return TRUE;
}


void CBaseStream::FlushTransferBuffer()
{
	((CTransfer *)m_pTransfer)->Flush();
}

