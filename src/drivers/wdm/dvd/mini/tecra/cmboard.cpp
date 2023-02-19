/*
�@�{�[�h�Ǘ��N���X
�@�iNP2)�@Sachiko Yasukawa
*/
//     Date    |   Author     |  Description
//  -----------+--------------+--------------------------------------------
//  1998.03.27 |  Hideki Yagi | Change the specification about GetSTC().
//

#include "stdafx.h"

#include "includes.h"
#include "classlib.h"


//������
BOOL CMPEGBoard::Init()
{
	m_pBaseStreamObject=NULL;
	m_pIHAL=NULL;
	
	return TRUE;
}

//�Ǘ����ׂ��V�����X�g���[���̒ǉ�
//�p�����[�^
//IBaseStream�@���b�p�[�����X�g���[���N���X�̊�{�N���X�ւ̃|�C���^
BOOL CMPEGBoard::AddStreamObjectInterface(IBaseStream *INewStreamObject)
{
	CBaseStream *pBSt1, *pBSt2;
	
	ASSERT(INewStreamObject);

	//HAL��ݒ肵�Ă��Ȃ��̂ŃG���[
	//if(m_pIHAL == NULL){
	//	_RPTF0(_CRT_ERROR, "m_pIHAL = NULL\n");
	//	return FALSE;
	//}

	//�܂�����X�g���[�����ǉ�����Ă��Ȃ��B
	//m_pBaseStreamObject ... �Ǘ����ׂ��X�g���[���̐擪������
	if(m_pBaseStreamObject == NULL){
		m_pBaseStreamObject = (CBaseStream *) INewStreamObject;
		pBSt1 = m_pBaseStreamObject;
	
	}
	//�Ǘ����ׂ��V�����X�g���[����ǉ�
	else{
		pBSt1 = (CBaseStream *)INewStreamObject;
		//MIX�^�C�v�̃X�g���[���ɒǉ��ł��Ȃ�
		if(ISMIXSTREAM(pBSt1->m_StreamType) == TRUE || ISMIXSTREAM(m_pBaseStreamObject->m_StreamType) == TRUE){
			_RPTF0(_CRT_ERROR, "INVALID STREAM\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::AddStreamObjectInterface:SET INVALID TYPE STREAM\n"));
			DBG_BREAK();
			return FALSE;
		}
		//�Ǘ����ׂ��X�g���[���̃��X�g��T��
		for(pBSt2 = m_pBaseStreamObject; pBSt2->m_pNextStreamObject != NULL; pBSt2 = pBSt2->m_pNextStreamObject);
		pBSt2->m_pNextStreamObject= pBSt1;//���܂܂Ń��X�g���������̂�Next�Ƀp�����[�^�Ŏw�肳�ꂽ�X�g���[����
		pBSt1->m_pPrevStreamObject = pBSt2;//�p�����[�^�Ŏw�肳�ꂽ�X�g���[����Prev�ɍ��܂Ń��X�g���������̂�
	}

	//�ǉ����悤�Ƃ��Ă���X�g���[���ɂg�`�k�C���^�t�F�[�X�ւ̃|�C���^��������B
	pBSt1->m_pIHAL=m_pIHAL;
	
	//�ǉ����悤�Ƃ��Ă���X�g���[����StreamControlHAL�C���^�t�F�[�X�ւ̃|�C���^���擾������B
	if(SetHALStream(pBSt1, m_pIHAL) == FALSE){
		_RPTF0(_CRT_ERROR, "CAN'T GET STREAMCONTROLHAL\n");
		DBG_PRINTF(("CLASSLIB:CMPEGBOARD::AddStreamObjectInterface:CAN'T GET STREAMCONTROLHAL\n"));
		DBG_BREAK();
		return FALSE;
	}

	return TRUE;
}

//�Ǘ����ׂ��X�g���[������̉��
//�p�����[�^
//IBaseStream�@���b�p�[�����X�g���[���N���X�̊�{�N���X�ւ̃|�C���^
BOOL CMPEGBoard::ReleaseStreamObjectInterface(IBaseStream *IReleaseStreamObject)
{
	CBaseStream *pBSt1;
	
	ASSERT(IReleaseStreamObject);

	//������ׂ��X�g���[�����Ȃ��̂ŃG���[
	if(m_pBaseStreamObject == NULL){
		_RPTF0(_CRT_WARN, "NO ADDED STREAM\n");
		DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ReleaseStreamObjectInterface:NO ADDED STREAM\n"));
		DBG_BREAK();
		return FALSE;
	}
	else{
		pBSt1 = (CBaseStream *)IReleaseStreamObject;
		
		//�����[�X���Ă͂����Ȃ��΂���������͂��H
		
		//�Ǘ����ׂ��X�g���[���̃`�F�[��������
		pBSt1->m_pPrevStreamObject->m_pNextStreamObject = pBSt1->m_pNextStreamObject;
		pBSt1->m_pNextStreamObject->m_pPrevStreamObject = pBSt1->m_pPrevStreamObject;

		//Transfer�I�u�W�F�N�g�ł��Ǘ����Ă���͂��Ȃ̂Ń����[�X
		ASSERT(((CBaseStream *)IReleaseStreamObject)->m_pTransfer);
		if(((CTransfer *)((CBaseStream *)IReleaseStreamObject)->m_pTransfer)->ReleaseStreamObject(IReleaseStreamObject) == FALSE){
			_RPTF0(_CRT_WARN, "NO RELEASE STREAM FROM TRANSFER OBJECT\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ReleaseStreamObjectInterface:CAN'T RELEASE STREAM FROM TRANSFER OBJECT\n"));
			DBG_BREAK();
			return FALSE;
		}
	}

	return TRUE;
}

//�d�����I���ɂ���
BOOL CMPEGBoard::PowerOn()
{
	 return ChangePower(POWERSTATE_ON);	
}

//�d�����I�t�ɂ���
BOOL CMPEGBoard::PowerOff()
{
	return ChangePower(POWERSTATE_OFF);
}

//STC�̒l���Z�b�g����
BOOL CMPEGBoard::SetSTC(DWORD time)
{
	//�܂����������I����ĂȂ��B
	if(m_pIHAL == NULL || m_pBaseStreamObject == NULL){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CMPEGBOARD::SetSTC:NOT BE INITILIZE CMPEGBoard CORRECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}
	if(m_pIHAL->SetSTC(time) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T SETSTC \n");
		DBG_PRINTF(("CLASSLIB:CMPEGBOARD::SetSTC:CAN'T SETSTC \n"));
		DBG_BREAK();
		return FALSE;
	}
	return TRUE;
}

//STC�̒l���Q�b�g����
//���^�[���l���l�������B
BOOL CMPEGBoard::GetSTC(DWORD *foo)                    // 98.03.27 H.Yagi
{
	DWORD Time;
	
	//�܂����������I����ĂȂ��B
	if(m_pIHAL == NULL || m_pBaseStreamObject == NULL){
		_RPTF0(_CRT_ERROR, "NOT INITILIZE CMPEGBoard \n");
		*foo = 0xffffffff;
		return FALSE;
	}

	//STC�̎擾
	if(m_pIHAL->GetSTC(&Time) != HAL_SUCCESS){
		_RPTF0(_CRT_WARN, "CAN'T GETSTC \n");
        *foo = 0xffffffff;
		return FALSE;                           // 98.04.21 H.Yagi
	}
	*foo = Time;
	return TRUE;
}

//HAL�I�u�W�F�N�g�ւ̃C���^�t�F�[�X�̐ݒ�
BOOL CMPEGBoard::SetHALObjectInterface(IClassLibHAL *pILibHAL)
{
	CBaseStream *pStream;

	ASSERT(pILibHAL);
	
	m_pIHAL = pILibHAL;

	//10/14����
	for(pStream = m_pBaseStreamObject; pStream; pStream = (CBaseStream *)pStream->GetNextStream()){
		if(SetHALStream(pStream, m_pIHAL) == FALSE){
			_RPTF0(_CRT_ERROR, "CAN'T GET STREAMCONTROLHAL\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::SetHALInterface:CAN'T GET STREAMCONTROLHAL\n"));
			DBG_BREAK();
			return FALSE;
		}
	}
	return TRUE;
}

//�d���̃I���I�t
BOOL CMPEGBoard::ChangePower(POWERSTATE fOnOff)
{
	CBaseStream *pBSt;
	IMPEGBoardBaseState *pState;
	
	//�܂����������I����ĂȂ��B
	if(m_pIHAL == NULL || m_pBaseStreamObject == NULL ){
		_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
		DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ChangePower:NOT INITILIZE CMPEGBoard CRRECTLY\n"));
		DBG_BREAK();
		return FALSE;
	}
	else{
		for(pBSt = m_pBaseStreamObject; pBSt; pBSt = pBSt->m_pNextStreamObject){
			
			ASSERT(pBSt->m_pMPEGBoardStateObject);
			
			//BaseState(CPowerOffState, CStopState etc.�̊�{�N���X�j�ւ̃|�C���^���Ƃ��Ă���
			pState = pBSt->GetIMPEGBoardState();
			
			//���������������I����ĂȂ��B
			if(pState == NULL){
				_RPTF0(_CRT_WARN, "NOT INITILIZE CMPEGBoard \n");
				DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ChangePower:NOT INITILIZE CMPEGBoard \n"));
				DBG_BREAK();
				return FALSE;
			}

			//�d���̃X�e�[�^�X�ύX
			//�I�t�Ȃ�
			if(fOnOff == POWERSTATE_ON){
				if(!ISPOWEROFF()){
					//_RPTF0(_CRT_WARN, "STATE CHANGE ERROR From PowerON To PowerON\n");
					return TRUE;//FALSE�H�H�H
				}
				if(pState->ChangeStop(m_pIHAL, pBSt->GetHALStreamControl(), NULL) == FALSE){
					_RPTF0(_CRT_WARN, "CANT CHANGE POWERSTATE\n");
					DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ChangePower:CANT CHANGE POWERSTATE\n"));
					DBG_BREAK();
					return FALSE;
				}
			}
			else{
////                if(ISPOWEROFF()){
////                    //_RPTF0(_CRT_WARN, "STATE CHANGE ERROR From PowerOff To PowerOff\n");
////                    return TRUE;//FALSE???
////                }
				if(pState->ChangePowerOff(m_pIHAL, pBSt->GetHALStreamControl()) == FALSE){
					_RPTF0(_CRT_WARN, "CANT CHANGE POWERSTATE");
					DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ChangePower:CANT CHANGE POWERSTATE\n"));
					DBG_BREAK();
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

BOOL CMPEGBoard::ISPOWEROFF()
{
	POWERSTATE PowerState;
	ASSERT(m_pIHAL);
	m_pIHAL->GetPowerState(&PowerState);
	if(PowerState == POWERSTATE_ON)
		return POWERSTATE_ON;
	else
		return POWERSTATE_OFF;
}

BOOL CMPEGBoard::SetHALStream(CBaseStream *pStream, IClassLibHAL *pIHAL)
{
	HALRESULT st;

	ASSERT(pStream);

	//MIX�X�g���[���p
	if(ISMIXSTREAM(pStream->m_StreamType) == TRUE){
		if((st = pIHAL->GetMixHALStream(&pStream->m_pIStreamHAL)) != HAL_SUCCESS){
			if(st != HAL_NOT_IMPLEMENT){
				_RPTF0(_CRT_WARN, "NOT IMPLEMENT");
			}
			//���ۂɃC���v�������g����Ȃ��̂͋������H�H�H
			_RPTF0(_CRT_WARN, "CAN'T GET MIXHALSTREAM INTERFACE\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::ChangePower:CAN'T GET MIXHALSTREAM INTERFACE\n"));
			DBG_BREAK();
			return FALSE;
		}
	}
	//�P�ƃX�g���[���p
	else{
		switch(pStream->m_StreamType){
		case Video:
			st = pIHAL->GetVideoHALStream(&pStream->m_pIStreamHAL);
			break;
		case Audio:
			st = pIHAL->GetAudioHALStream(&pStream->m_pIStreamHAL);
			break;
		case Subpicture:
			st = pIHAL->GetSubpicHALStream(&pStream->m_pIStreamHAL);
			break;
		default:
			_RPTF0(_CRT_WARN, "INVALID STREAMTYPE\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::SetHALStream:INVALID STREAMTYPE\n"));
			DBG_BREAK();
			return FALSE;
		}
		if(st != HAL_SUCCESS){
			_RPTF0(_CRT_WARN, "INVALID STREAMTYPE\n");
			DBG_PRINTF(("CLASSLIB:CMPEGBOARD::SetHALStream:INVALID STREAMTYPE\n"));
			DBG_BREAK();
			return FALSE;
		}
	}

	return TRUE;
}
