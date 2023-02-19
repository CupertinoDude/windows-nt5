// [!output IMPL_FILE] : [!output CLASS_NAME] �̎���
#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output HEADER_FILE]"


/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

// �d�v: �ȉ��ɒ�`����Ă��镶����̓��[�J���C�Y�ł��܂���B

// �N���X��
[!if CLASS_SPECIFIED]
const static WCHAR * s_pMyClassName = L"[!output WMICLASSNAME]"; 
[!if EXTRINSIC]
//�C�x���g �N���X [!output WMICLASSNAME] �̃v���p�e�B:
[!output EXTR_PROPERTY_DEFINITIONS]
[!endif]
[!else]
//TODO: �񋟂��ꂽ�N���X�����`���Ă��������B��:
const static WCHAR * s_pMyClassName = L"MyClassName"; 
[!endif]


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::Initialize()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemProviderInit::Initialize()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::Initialize(LPWSTR pszUser,	
									  LONG lFlags,
									  LPWSTR pszNamespace,
									  LPWSTR pszLocale,
									  IWbemServices *pNamespace,
									  IWbemContext *pCtx,
																																							IWbemProviderInitSink *pInitSink) 
{
	
	HRESULT hr = WBEM_E_FAILED;
	if ( NULL == pNamespace || NULL == pInitSink) 
	{
        return WBEM_E_INVALID_PARAMETER;
	}

	//IWbemServices �|�C���^���L���b�V�����܂��B
	//��: m_pNamespace �́A�X�}�[�g �|�C���^�ł��B�����I�� AddRef() ���܂��B
	m_pNamespace = pNamespace;
					
	[!if INTRINSIC]	
	//helper �I�u�W�F�N�g���擾���܂��B
	m_pHelper = new CIntrinsicEventProviderHelper(pNamespace, pCtx);

	[!endif]
	
	[!if EXTRINSIC]	
	//�C�x���g �N���X�̒�`��ۑ����܂��B				
	//��: �ȉ��̃R�[�h�́A�v���o�C�_�̎��s���̓C�x���g �N���X�̒�`���A
	//�ύX���Ȃ��Ɖ��肵�Ă��܂��B�ύX����ꍇ�́A�N���X�̏C���C�x���g�ƍ폜�C�x���g�p��
	//�R���V���[�}����������K�v������܂��B�C�x���g �R���V���[�}�ɂ��ẮA
	//MSDN ��� WMI �h�L�������g���Q�Ƃ��Ă��������B
	hr = m_pNamespace->GetObject(CComBSTR(s_pMyClassName), 
								0, 
								pCtx, //�f�b�h���b�N���Ȃ��悤�� IWbemContext �|�C���^��n���܂��B
								&m_pEventClass, 
								NULL);

    if(FAILED(hr))
	{
        return hr;
	}

	//helper �I�u�W�F�N�g���擾���܂��B
	m_pHelper = new CProviderHelper(pNamespace, pCtx);

    [!endif]
	[!if INTRINSIC]
	//�^�[�Q�b�g�̃N���X�̒�`��ۑ����܂��B
	hr = m_pNamespace->GetObject(CComBSTR(s_pMyClassName), 
								 0, 
								 pCtx, //�f�b�h���b�N���Ȃ��悤�� IWbemContext �|�C���^��n���܂��B
								 &m_pDataClass, 
								 NULL);
    if(FAILED(hr))
	{
        return hr;
	}
	[!endif]
	    
    pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
    return WBEM_S_NO_ERROR;
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::ProvideEvents()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemEventProvider::ProvideEvents()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B
	
STDMETHODIMP [!output CLASS_NAME]::ProvideEvents(
							IWbemObjectSink *pSink,
							long lFlags)
{
	//  WMI �̓C�x���g �v���o�C�_���A�N�e�B�u�ɂ��邽�߂ɂ��̃��\�b�h���Ăяo���܂��B
	//  TODO: ���̌Ăяo������Ԃ�����A�^����ꂽ�V���N �C���^�[�t�F�C�X����������̂�
	//  �C�x���g�̔z�M���J�n���Ă��������B�C�x���g�̔z�z���������邽�߂ɁA
	//  �Ɨ������X���b�h���쐬���邱�Ƃ��ł��܂��B
	[!if INTRINSIC]
	//  �g�ݍ��݃C�x���g��z�z���邽�߂ɁAm_pHelper �� FireCreationEvent()�AFireDeletionEvent()�A
	//  FireModificationEvent() ���\�b�h���Ăяo���܂��B 
	[!endif]
	//  WMI �ɏڂ����G���[�܂��͏�Ԃ�񍐂��邽�߂ɁAm_pHelper �� ConstructErrorObject() ���Ăяo�����Ƃ��ł��܂��B
	
	//  �d�v: ���̌Ăяo���𐔕b�ȏ�u���b�N���Ȃ��ł��������B

	if ( NULL == pSink )
	{
		return WBEM_E_INVALID_PARAMETER;
	}

	//�L���b�V�� �V���N �|�C���^
	//��: m_pSink �̓X�}�[�g�|�C���^�ŁA�����I�� AddRef() ���܂��B
	m_pSink = pSink;
	
	pSink->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);	
	return WBEM_S_NO_ERROR;
}

[!if EVENT_SECURITY ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::AccessCheck()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemEventProviderSecurity::AccessCheck()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B 

STDMETHODIMP [!output CLASS_NAME]::AccessCheck(
						WBEM_CWSTR wszQueryLanguage,
						WBEM_CWSTR wszQuery,
						long lSidLength,
						const BYTE* pSid)
{
	return WBEM_S_NO_ERROR;
}
[!endif]

[!if QUERY_SINK ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::CancelQuery()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemEventProviderQuerySink::CancelQuery()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::CancelQuery(
						unsigned long dwId)
{
	return WBEM_S_NO_ERROR;
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::NewQuery()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemEventProviderQuerySink::NewQuery() 
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::NewQuery(
						unsigned long dwId,
						WBEM_WSTR wszQueryLanguage,
						WBEM_WSTR wszQuery)
{
	return WBEM_S_NO_ERROR;
}
[!endif]

[!if EXTRINSIC]	
//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::FireEvent()

STDMETHODIMP [!output CLASS_NAME]::FireEvent()
{
	//�œK���̒���: �E�C�U�[�h�ɂ���Đ��������C���v�������g�͊ȒP�ł����A
	//���b 1000 �ȏ�̃C�x���g��z�M����ꍇ�́A�C�x���g �v���o�e�B��
	//���͂��邽�߂� IWbemObjectAccess �C���^�[�t�F�C�X���g�����Ƃ��ł��܂��B 
	//�܂��A�C�x���g �N���X�̃C���X�^���X���L���b�V�����čė��p���邱�Ƃ��ł��܂��B

	ATLASSERT(m_pEventClass);

	CComPtr<IWbemClassObject> pEvtInstance;
    HRESULT hr = m_pEventClass->SpawnInstance(0, &pEvtInstance);
    if(FAILED(hr))
	{
		return hr;
	}

	//�C�x���g �I�u�W�F�N�g�̃v���p�e�B�̒l�𐳂������͂��Ă��������B:
	[!if CLASS_SPECIFIED]
    [!output EXTRINSIC_PUT_BLOCK]
	[!else]
	//TODO: �C�x���g�̃I�u�W�F�N�g�̃v���p�e�B�̒l����͂��邽�߂Ɏ��̃R�}���h �R�[�h���C�����Ă��������B
	//CComVariant var;
	//var.ChangeType(<type>);	//�����ɓK�؂ȃo���A���g �^�C�v����͂��Ă��������B
	//var = <value>;			//�����ɓK�؂Ȓl����͂��Ă��������B
	//hr = pEvtInstance->Put(CComBSTR(L"EventProperty1"), 0, &var, 0);
	//var.Clear();
	[!endif]

	hr = m_pSink->Indicate(1, &pEvtInstance );

	return hr;
}
[!endif]	
