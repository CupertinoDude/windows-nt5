// [!output IMPL_FILE] :  [!output CLASS_NAME] �̎��s
#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output HEADER_FILE]"


/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

//////////////////////////////////////////////////////////////////////////////
// �N���X���A�v���p�e�B���A���\�b�h��

// �d�v: �ȉ��ɒ�`����Ă��镶����̓��[�J���C�Y�ł��܂���B

[!if CLASS_SPECIFIED]
const static WCHAR * s_pMyClassName = L"[!output WMICLASSNAME]"; //�N���X��

//�v���p�e�B:
[!output PROPERTY_DEFINITIONS]

//���\�b�h:
[!output METHOD_DEFINITIONS]

[!else]
//TODO: �^����ꂽ�N���X�����`���Ă��������B��:
const static WCHAR * s_pMyClassName = L"MyClassName"; 

//TODO: �^����ꂽ�N���X�̃v���o�e�B�����`���Ă��������B��:
//const static WCHAR * pMyProperty = L"MyProperty";

//TODO: �^����ꂽ�N���X�̃��\�b�h�����`���Ă��������B��:
//const static WCHAR * pMyMethod = L"MyMethod";
[!endif]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::Initialize
//���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemProviderInit::Initialize()
//�� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::Initialize(LPWSTR pszUser,	
									  LONG lFlags,
									  LPWSTR pszNamespace,
									  LPWSTR pszLocale,
									  IWbemServices *pNamespace,
							 		  IWbemContext *pCtx,
									  IWbemProviderInitSink *pInitSink) 
{

	if ( NULL == pNamespace || NULL == pInitSink) 
	{
        return WBEM_E_INVALID_PARAMETER;
	}

  	//IWbemServices �|�C���^���L���b�V�����܂��B
	//��: m_pNamespace �̓X�}�[�g �|�C���^�ł��B�����I�� AddRef() ���܂��B
	m_pNamespace = pNamespace;
				
	//�C�x���g �N���X�̒�`��ۑ����܂��B				
	//��: �ȉ��̃R�[�h�́A�v���o�C�_�̎��s���̓C�x���g �N���X�̒�`���A
	//�ύX���Ȃ��Ɖ��肵�Ă��܂��B�ύX����ꍇ�́A�N���X�̏C���C�x���g�ƍ폜�C�x���g�p��
	//�R���V���[�}����������K�v������܂��B�C�x���g �R���V���[�}�ɂ��ẮA
	//MSDN ��� WMI �h�L�������g���Q�Ƃ��Ă��������B
	
    HRESULT hr = m_pNamespace->GetObject(CComBSTR(s_pMyClassName), 
											0, 
											pCtx, 	//�f�b�h���b�N���Ȃ��悤�� IWbemContext �|�C���^��n���܂��B
											&m_pClass, 
											NULL);
    if(FAILED(hr))
	{
        return WBEM_E_FAILED;
	}    
	
	//�p�X �p�[�T�[ �N���X �t�@�N�g�����L���b�V�����܂��B
	hr = CoGetClassObject(CLSID_WbemDefPath, 
							CLSCTX_INPROC_SERVER, 
							NULL,
							IID_IClassFactory,
							(void **) &m_pPathFactory);
	if (FAILED(hr))
	{
		return WBEM_E_FAILED;
	}

	//helper �I�u�W�F�N�g���쐬���܂��B
	m_pHelper = new CInstanceProviderHelper(pNamespace, pCtx);

	//��: WMI�ɏڂ����G���[�܂��͏�Ԃ�񍐂��邽�߂ɁA
	//�v���o�C�_���� m_pHelper �̂ǂ��łł� ConstructErrorObject() ���Ăяo�����Ƃ��ł��܂��B 

    pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
    return WBEM_S_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetObjectAsync
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́ARIWbemServices::GetObjectAsync()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::GetObjectAsync( 
							 const BSTR bstrObjectPath,
							 long lFlags,
							 IWbemContext  *pCtx,
							 IWbemObjectSink  *pResponseHandler)
{
    
	//bugbug: �e�v���p�e�B�̎擾?
    [!if SUPPORT_GET ]
	if (NULL == pResponseHandler)
	{
	     return WBEM_E_INVALID_PARAMETER;
	}

	CComPtr<IWbemClassObject> pInstance;
	HRESULT hr = GetInstanceByPath(bstrObjectPath, &pInstance);
	if (FAILED (hr))
	{
		return hr;
	}
			   
   	//���ׂĂ��܂��������ꍇ�́A�I�u�W�F�N�g�� WMI �ɖ߂�������\�����܂��B:
    pResponseHandler->Indicate (1, &(pInstance.p)); 
	pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);

    return WBEM_S_NO_ERROR;
	[!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::PutInstanceAsync()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemServices::PutInstanceAsync()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::PutInstanceAsync( 
							IWbemClassObject  *pInst,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
   
	[!if SUPPORT_PUT ]
	if ( NULL == pResponseHandler || NULL == pInst)
	{
        return WBEM_E_INVALID_PARAMETER;
	}
	//TODO: �\�ȃt���O�l�𒲂ׂĂ�������: WBEM_FLAG_UPDATE_ONLY�A
	//WBEM_FLAG_CREATE_ONLY�AWBEM_FLAG_CREATE_OR_UPDATE
	//�K�v�ȃT�|�[�g�̃��x����I�����āA�T�|�[�g���Ă��Ȃ��t���O�l�̂��߂� WBEM_E_PROVIDER_NOT_CAPABLE
	//��Ԃ��Ă��������B

	//TODO: �����I�ȍX�V���T�|�[�g����\�肪����ꍇ�́A"__PUT_EXTENSIONS"  ��
	// pCtx �Ƃق��̊֘A����V�X�e�� �R���e�L�X�g�l�𒲂ׂāA
	//�C���X�^���X �f�[�^��K�؂ɍX�V���Ă��������B
	
	//TODO: �����ŃC���X�^���X�̍X�V�܂��͍쐬���������܂��B			
	
	pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
	return WBEM_S_NO_ERROR;
	[!else]
	return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::DeleteInstanceAsync()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́A IWbemServices::DeleteInstanceAsync()
// �� MSDN �w���v���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::DeleteInstanceAsync( 
							const BSTR ObjectPath,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	//���̃��\�b�h���C���v�������g���邽�߂ɁA�C���X�^���X �v���o�C�_�� strObjectPath �p�����[�^��
	//�Ɏw�肳�ꂽ�I�u�W�F�N�g �p�X���������͂��A�Ή�����C���X�^���X��������
	//�폜���܂��B
	[!if SUPPORT_DELETE ]

	if (NULL == pResponseHandler)
	{
		return WBEM_E_INVALID_PARAMETER;
	}
    
	[!if IS_COMPOUND_KEY]
	ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_IS_COMPOUND;
	[!else]
	[!if IS_SINGLETON]
	ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_CONTAINS_SINGLETON;
	[!else]
	ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF;
	[!endif]
	[!endif]
		
	if (FAILED(m_pHelper->CheckInstancePath(m_pPathFactory,
											ObjectPath,
											CComBSTR(s_pMyClassName),
											ulPathTest)) )
	{
		//�p�X���̍\���G���[���A�^����ꂽ�N���X�̃p�X������������܂���B
		return WBEM_E_INVALID_PARAMETER; 
	}			
	
	[!if IS_SINGLETON]
	//��: [!output WMICLASSNAME] �̓V���O���g�� �I�u�W�F�N�g�ł��B �C���X�^���X�����ʂ���K�v�͂���܂���B
	//��: �C���X�^���X�����݂��Ȃ��ꍇ�́A�����WMI�ɒʐM���Ԃ����߂Ɏ��̃R�����g���Ȃ����Ă�������:
	// WBEM_E_NOT_FOUND ��Ԃ��܂�;
	[!else]
	//�p�X �p�[�T�[ �I�u�W�F�N�g���쐬���܂��B
	CComPtr<IWbemPath>pPath;
	HRESULT hr = m_pPathFactory->CreateInstance(NULL,
												IID_IWbemPath,
												(void **) &pPath);
	if (FAILED(hr))
	{
		return WBEM_E_FAILED;
	}

	hr = pPath->SetText(WBEMPATH_CREATE_ACCEPT_ALL,	ObjectPath);
	
	CComPtr<IWbemPathKeyList> pIKeyList;
	hr = pPath->GetKeyList(&pIKeyList);
	if (FAILED(hr))
	{
		return hr;
	}
	
	unsigned long ulNumKeys;
	hr = pIKeyList->GetCount(&ulNumKeys);

	//�L�[ �v���p�e�B�̒l���擾���܂��B
	unsigned long uKeyNameBufferSize = CComBSTR(ObjectPath).Length() + 1;
	WCHAR * wKeyName = new WCHAR[uKeyNameBufferSize];
	if (NULL == wKeyName)
	{
		return WBEM_E_OUT_OF_MEMORY;
	}
	CComVariant vValue;
	unsigned long ulApparentCimType;
	for (unsigned long i = 0; i < ulNumKeys; i++)
	{			
		hr = pIKeyList->GetKey2(i, 0L, &uKeyNameBufferSize, 
								wKeyName, &vValue, &ulApparentCimType);			
		//TODO: ��Ŏg�����߂� vValue ��ۑ����܂��B
	}
	delete[] wKeyName;

	//TODO: ��v����I�u�W�F�N�g�����o���č폜���邽�߂Ƀf�[�^ �\�[�X�ŌJ��Ԃ��܂��B
	//��: �p�X���Ɏ����ꂽ�L�[�l�Ɉ�v����I�u�W�F�N�g�����o�ł��Ȃ��ꍇ�A
	//����� WMI �ɒʐM���邽�߂Ɏ��̍s���R�����g�ɂ��Ă�������:
	//return WBEM_E_NOT_FOUND;
	[!endif]	

	pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);	
    return WBEM_S_NO_ERROR;
	
	[!else]
	return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::CreateInstanceEnumAsync()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemServices::CreateInstanceEnumAsync()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::CreateInstanceEnumAsync( 
							const BSTR Class,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	

	//��: �C���X�^���X �v���o�C�_�������̑�������s���邽�߂� WMI ����X���b�h���擾���Ă��邱�Ƃ�
	//���ɒ��ӂ��Ă��������B�V���N AddRef() ���\�b�h���Ăяo���Č��ʃZ�b�g���ɃI�u�W�F�N�g��z�z���邽�߂�
	//�ʂ̃X���b�h���쐬���Ă��������B 
	//����ɂ��A�X���b�h �v�[�������Ղ��Ȃ��Ō��݂̃X���b�h�� WMI �Ԃ����Ƃ��ł��܂��B
	//�v���o�C�_���f���A�� �X���b�h �f�U�C�����V���O�� �X���b�h �f�U�C����I�����邩�ǂ����́A
	//�v���o�C�_�� WMI �X���b�h���g�p����\��̊��ԂɈˑ����܂��B

	[!if SUPPORT_ENUMERATE ]
	if (NULL == pResponseHandler)
	{
	    return WBEM_E_INVALID_PARAMETER;
	}

	HRESULT hr = WBEM_S_NO_ERROR;
	
	[!if IS_SINGLETON]
	// �C���X�^���X �f�[�^���󂯎�邽�߂ɋ�̃I�u�W�F�N�g��p�ӂ��܂��B
    CComPtr<IWbemClassObject> pNewInst;
    hr = m_pClass->SpawnInstance(0, &pNewInst);
		
	CComVariant var;
	[!if CLASS_SPECIFIED]
	[!output POPULATE_INSTANCE]
	[!else]
	//TODO: �C���X�^���X�Ƀv���p�e�B�������Ă��������B��:
	//CComVariant var;
	//var.ChangeType(VT_BSTR);
	//var = <value>;  //�����ɓK�؂Ȓl����͂��Ă��������B
	//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
	//var.Clear();
	[!endif]
		
	// �N���X�� WMI �ɔz�z���܂��B
    pResponseHandler->Indicate(1, &(pNewInst.p));
	[!else]
    // �v���C�x�[�g �\�[�X�����[�v�ɂ��A�e�C���X�^���X���쐬���Ă��������B
	//while (<�C���X�^���X>)
    {
		// �C���X�^���X �f�[�^���󂯎�邽�߂ɋ�̃I�u�W�F�N�g��p�ӂ��܂��B
        CComPtr<IWbemClassObject> pNewInst;
        hr = m_pClass->SpawnInstance(0, &pNewInst);
		if (FAILED(hr))
		{	
			//TODO: ���[�v��������͂�����A���̍s���R�����g�ɂ��Ă��������B
			//break;
		}
		
		CComVariant var;
		[!if CLASS_SPECIFIED]
		[!output POPULATE_INSTANCE]
		[!else]
		//TODO: �C���X�^���X�Ƀv���p�e�B�������܂��B��:
		//CComVariant var;
		//var.ChangeType(VT_BSTR);
		//var = <value>;  //�����ɓK�؂Ȓl����͂��Ă�������
		//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
		//var.Clear();
		[!endif]
		
		// �N���X�� WMI �ɔz�z���܂��B
        pResponseHandler->Indicate(1, &(pNewInst.p));
    }  	
	[!endif]
	pResponseHandler->SetStatus(0, hr, NULL, NULL);
   	return WBEM_S_NO_ERROR;
	[!else]
	return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}


//////////////////////////////////////////////////////////////////////////////
//[!output CLASS_NAME]::ExecQueryAsync()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemServices::ExecQueryAsync()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::ExecQueryAsync( 
							const BSTR QueryLanguage,
							const BSTR Query,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	
	
   	// �C���X�^���X �v���o�C�_�̓T�[�r�X�ɑ΂��āA�N�G�������̃T�|�[�g�܂��͂��̃T�|�[�g�� WMI �Ɉˑ�����
	// �I�v�V����������܂��B�N�G�����T�|�[�g���邽�߂ɂ́A�C���X�^���X �v���o�C�_��
	// �ȒP�� SQL (Structured Query Language) �X�e�[�g�����g�̉�́A�v�����ꂽ�N�G���̎��s�A
	// ���ʃZ�b�g �I�u�W�F�N�g�̗v���҂̃V���N�ւ̔z�M���s����K�v������܂��B 
	
	//TODO: �N�G�������R�[�h�������ɓ��͂��Ă��������B
	
	//pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
	// return WBEM_S_NO_ERROR;

	return WBEM_E_PROVIDER_NOT_CAPABLE;
	
}

[!if PROVIDE_METHODS ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::ExecMethodAsync()
// ���̃��\�b�h�̎����ɂ��Ă̏ڍׂ́AIWbemServices::ExecMethodAsync()
// �� MSDN �h�L�������g���Q�Ƃ��Ă��������B

STDMETHODIMP [!output CLASS_NAME]::ExecMethodAsync( 
							const BSTR strObjectPath,
							const BSTR strMethodName,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemClassObject  *pInParams,
							IWbemObjectSink  *pResponseHandler)
{
	

	[!if CLASS_SPECIFIED]
	[!if  !HAS_IMPL_METHODS]	
	//[!output WMICLASSNAME] �ɂ͎������ꂽ���\�b�h������܂���B
	return WBEM_E_NOT_SUPPORTED;
	[!else]
	HRESULT hr = WBEM_E_FAILED;	
    if (NULL == pResponseHandler)
	{
        return WBEM_E_INVALID_PARAMETER;
	}	
	[!output EXEC_METHOD_BODY]
	[!endif]
	[!else]
	
	//�p�X�̃N���X�������������Ƃ��m�F���Ă��������B
	//�p�X �p�[�T�[ �I�u�W�F�N�g���擾���܂�:
	CComPtr<IWbemPath>pPath;
	HRESULT hr = m_pPathFactory->CreateInstance(NULL,
												IID_IWbemPath,
												(void **) &pPath);
	if (FAILED(hr))
	{
		return WBEM_E_FAILED;
	}

	HRESULT hr = pPath->SetText(WBEMPATH_CREATE_ACCEPT_ALL, strObjectPath);
	long nPathLen = CComBSTR(strObjectPath).Length();
	unsigned long ulBufLen = nPathLen + 1;
	WCHAR * wClass = new WCHAR[nPathLen];
	if (NULL == wClass)
	{
		return WBEM_E_OUT_OF_MEMORY;
	}
	pPath->GetClassName(&ulBufLen, wClass);
	if ( _wcsicmp(s_pMyClassName, wClass))
	{
		delete[] wClass;
		return WBEM_E_INVALID_PARAMETER;
	}
	delete[] wClass;
	//TODO: �v�����ꂽ���\�b�h���ƃN���X�̃��\�b�h���r���Ă��������B
	//if (!_wcsicmp (strMethodName, pMyNonStaticMethod))
	{
		CComVariant var;

		//���͈������擾���܂�:
		//TODO: ���\�b�h�ɓ��͈���������ꍇ�́A���͈����� pInParams �I�u�W�F�N�g�̃v���o�e�B�Ƃ��ēn����܂��B
		//���͈����̎��o�����́A�ȉ��̃R�����g�s���Q�Ƃ��Ă�������:
		//hr = pInParams->Get(CComBSTR("InputArgument1"), 0, &var, NULL, NULL);
		//TODO: ���̓p�����[�^�̒l��ۑ����܂��B
		var.Clear();		
		
		//���\�b�h�̎��s�̃C���X�^���X�����o���邽�߂Ƀp�X����͂��܂�: �ÓI�łȂ����\�b�h�p
		CComPtr<IWbemPathKeyList> pIKeyList;
		hr = pPath->GetKeyList(&pIKeyList);
		if (FAILED(hr))
		{
			return WBEM_E_INVALID_PARAMETER;
		}
		unsigned long ulNumKeys;
		hr = pIKeyList->GetCount(&ulNumKeys);
		//�L�[�v���p�e�B�̒l���擾���܂�:
		unsigned long uKeyNameBufferSize = nPathLen + 1;
		WCHAR  * wKeyName = new WCHAR[uKeyNameBufferSize];
		if (NULL == wKeyName)
		{
			return WBEM_E_OUT_OF_MEMORY;
		}
		CComVariant vValue;
		unsigned long ulApparentCimType;
		for (unsigned long i = 0; i < ulNumKeys; i++)
		{
			hr = pIKeyList->GetKey2(i, 0L, &uKeyNameBufferSize,
									wKeyName, &vValue, &ulApparentCimType);
			//TODO: ��Ŏg�����߂� vValue ��ۑ����܂��B
		}
		delete[] wKeyName;
	
		//TODO: �������I�u�W�F�N�g�������邽�߂Ƀf�[�^ �\�[�X���g���ČJ��Ԃ��B
		
		//TODO: ���\�b�h�����s���邽�߂ɂ����ɃR�[�h�𑫂��Ă��������B
		//�o�̓p�����[�^�̃N���X���󂯎��܂��B
		CComPtr<IWbemClassObject> pOutClass;
		hr = m_pClass->GetMethod(CComBSTR("Method1"), 0, NULL, &pOutClass);
		CComPtr<IWbemClassObject> pOutParams;
		pOutClass->SpawnInstance(0, &pOutParams);
		//TODO: pOutParams �̃N���X�̃v���p�e�B���t�@�C�����āA
		//�o�̓p�����[�^���쐬���܂��B��:

		//var.ChangeType(VT_BSTR);
		//var �ɓK�؂Ȓl����͂��Ă��������B
		//hr = pOutParams->Put(CComBSTR("MyOutputParameter"), 0, &var, 0);
		//var.Clear();
		//var.ChangeType(VT_I2);
		//var �ɓK�؂Ȓl����͂��Ă��������B
		//hr = pOutParams->Put(CComBSTR("ReturnValue"), 0, &var, 0);
		//var.Clear();
		// �o�̓I�u�W�F�N�g���V���N��ʂ��ăN���C�A���g�ɑ���Ԃ��Ă��������B
		hr = pResponseHandler->Indicate(1, &(pOutParams.p));
		
		pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
		return WBEM_S_NO_ERROR;
	}
	
	//�F������Ȃ����\�b�h���ł��B
	return WBEM_E_INVALID_PARAMETER;
	
	
	[!endif]	
}
[!endif]



//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetInstanceByPath() �͗v�����ꂽ�L�[�̒l�����o���邽�߂Ƀp�X����͂��A
// �L�[�̒l�Ɉ�v����I�u�W�F�N�g������X�g�A���������܂��B
// ���̂悤�ȃI�u�W�F�N�g�����o�����ꍇ�́A���\�b�h�͐V�����C���X�^���X�𐶐����A���ׂẴv���p�e�B����͂��āA
// ppInstance �ɕԂ��܂��B ���o����Ȃ������ꍇ�́A���\�b�h�� WBEM_E_NOT_FOUND ��Ԃ��܂��B
// bugbug: �����I�ȃC���X�^���X�����́H�H�H  �ق��ɉ\�ȃt���O�́H

STDMETHODIMP [!output CLASS_NAME]::GetInstanceByPath (
													/*in*/CComBSTR bstrPath,
													/*out*/IWbemClassObject ** ppInstance )
													
{
		HRESULT hr = WBEM_E_FAILED;				  
		
		[!if IS_COMPOUND_KEY]
		ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_IS_COMPOUND;
		[!else]
		[!if IS_SINGLETON]
		ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_CONTAINS_SINGLETON;
		[!else]
		ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF;
		[!endif]
		[!endif]
		
		if (FAILED(m_pHelper->CheckInstancePath(m_pPathFactory,
												bstrPath,
												CComBSTR(s_pMyClassName),
												ulPathTest)))
		{
			//�p�X���ɍ\���G���[�����邩�A�^����ꂽ�N���X�̃p�X������������܂���B
			return WBEM_E_INVALID_PARAMETER; 
		}			
		
		[!if IS_SINGLETON]
		//[!output WMICLASSNAME] �̓V���O���g�� �I�u�W�F�N�g�ł��B�C���X�^���X�����ʂ���K�v�͂���܂���B
	//��: �C���X�^���X�����݂��Ȃ��ꍇ�́A����� WMI �ɒʐM���邽�߂Ɏ��̍s���R�����g�Ƃ��Ă�������:
		//return WBEM_E_NOT_FOUND;
		[!else]
		//�p�X �p�[�T�[ �I�u�W�F�N�g���擾���܂�:
		CComPtr<IWbemPath>pPath;
		hr = m_pPathFactory->CreateInstance(NULL,
												IID_IWbemPath,
												(void **) &pPath);
		if (FAILED(hr))
		{
			return WBEM_E_FAILED;
		}
		
		hr = pPath->SetText(WBEMPATH_CREATE_ACCEPT_ALL,
									bstrPath);

		CComPtr<IWbemPathKeyList> pIKeyList;
		hr = pPath->GetKeyList(&pIKeyList);
		if (FAILED(hr))
		{
			return hr;
		}
		unsigned long ulNumKeys;
		hr = pIKeyList->GetCount(&ulNumKeys);

		//�L�[�v���p�e�B�̒l���擾���܂��B
		unsigned long uKeyNameBufferSize = bstrPath.Length() + 1;
		WCHAR  * wKeyName = new WCHAR[uKeyNameBufferSize];
		if (NULL == wKeyName)
		{
			return WBEM_E_OUT_OF_MEMORY;
		}
		CComVariant vValue;		
		unsigned long ulApparentCimType;
		for (unsigned long i = 0; i < ulNumKeys; i++)
		{			
			hr = pIKeyList->GetKey2(i, 0L, &uKeyNameBufferSize, 
									wKeyName, &vValue, &ulApparentCimType);			
			//TODO:��Ŏg�����߂� vValue ��ۑ����܂��B
		}

		delete[] wKeyName;
		
		//TODO: ��v�����I�u�W�F�N�g�����o���邽�߂ɓ����̃f�[�^ �\�[�X���������Ă��������B
		//�v�����ꂽ�L�[�̒l���Ȃ��ꍇ�́A 
		//WBEM_E_NOT_FOUND ��Ԃ��܂��B
		[!endif]
	   
		//�V�����C���X�^���X�𐶐����܂��B
		CComPtr<IWbemClassObject> pNewInst;
		hr = m_pClass->SpawnInstance(0, &pNewInst);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant var;

		//TODO: �V�����C���X�^���X�̃v���p�e�B����v��������I�u�W�F�N�g�œ��͂��Ă��������B�B
		[!if CLASS_SPECIFIED]
		[!output POPULATE_INSTANCE]			
		[!else]
		//��:
		//var.ChangeType(VT_BSTR);
		//var = <value>; //�����ɓK�؂Ȓl����͂��Ă��������B
		//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
		//var.Clear();		
		[!endif]

		pNewInst.CopyTo(ppInstance);

		return hr;
}

