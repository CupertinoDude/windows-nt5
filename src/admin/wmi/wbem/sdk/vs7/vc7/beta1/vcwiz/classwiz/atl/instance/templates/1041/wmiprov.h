[!if  !INSTANCE]
//�G���[: �C�x���g �v���o�C�_ �w�b�_�[ �e���v���[�g�͊Ԉ������ނ̃v���o�C�_���Ăяo���܂����B
[!else]
// [!output HEADER_FILE] :  [!output CLASS_NAME] �̐錾�B

#pragma once
#include "resource.h"       // ���C�� �V���|���ł��B

#include <wbemidl.h>
#include <atlwmiprov.h>
#include <wmiutils.h>	//�p�X �p�[�T�[�ł��B

//////////////////////////////////////////////////////////////////////////////
// �N���X���A�v���p�e�B���A���\�b�h���ł�: [!output IMPL_FILE] �Œ�`����܂��B

extern const WCHAR * s_pMyClassName;	//�N���X���ł��B

[!if CLASS_SPECIFIED]
//�v���p�e�B:
[!output PROPERTY_DECLARATIONS]

//���\�b�h:
[!output METHOD_DECLARATIONS]
[!else]
//TODO: �^����ꂽ�N���X�̃v���p�e�B����錾���Ă��������B��:
//extern const WCHAR * pMyProperty;

//TODO: �^����ꂽ�N���X�̃��\�b�h����錾���Ă��������B��:
//extern const WCHAR * pMyMethod;
[!endif]

[!if ATTRIBUTED]
[
	coclass,
[!if THREADING_APARTMENT]
	threading("apartment"),
[!endif]
[!if THREADING_BOTH]
	threading("both"),
[!endif]
[!if THREADING_FREE]
	threading("free"),
[!endif]
	aggregatable("never"),
	vi_progid("[!output VERSION_INDEPENDENT_PROGID]"),
	progid("[!output PROGID]"),
	version(1.0),
	uuid("[!output CLSID_REGISTRY_FORMAT]"),
	helpstring("[!output TYPE_NAME]")
]
[!endif]
/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

class ATL_NO_VTABLE [!output CLASS_NAME] : 
					[!if !ATTRIBUTED]
					[!if THREADING_APARTMENT]
						public CComObjectRootEx<CComSingleThreadModel>,
					[!endif]
					[!if THREADING_BOTH]
						public CComObjectRootEx<CComMultiThreadModel>,
					[!endif]
					[!if THREADING_FREE]
						public CComObjectRootEx<CComMultiThreadModel>,
					[!endif]
						public CComCoClass<[!output CLASS_NAME], &CLSID_[!output COCLASS]>,
					[!endif]
						public IWbemInstProviderImpl
{
	
	CComPtr<IWbemServices>  m_pNamespace; 	//�L���b�V�����ꂽ IWbemServices �|�C���^�ł��B
	CComPtr<IWbemClassObject> m_pClass;		//�L���b�V�����ꂽ�^����ꂽ�N���X�̒�`�ł��B  	
	CComPtr<IWbemClassObject> m_pErrorObject;//�L���b�V�����ꂽ�G���[ �I�u�W�F�N�g �N���X�̒�`�̃|�C���^�ł��B
	CComPtr<IClassFactory> m_pPathFactory;	 //�p�[�T�[�̃N���X �t�@�N�g�����p�X���邽�߂̃L���b�V�����ꂽ�|�C���^�ł��B

	CInstanceProviderHelper * m_pHelper;
	
	STDMETHODIMP GetInstanceByPath (CComBSTR bstrPath,
									IWbemClassObject ** ppInstance );

  public:
	[!output CLASS_NAME]()
	{
		m_pHelper = NULL;					
	}

	~[!output CLASS_NAME]()
	{
	    if (NULL != m_pHelper)
		{
			delete m_pHelper;
		}
		//�C���^�[�t�F�C�X �|�C���^ �f�[�^ �����o�̓X�}�[�g�Ŏ����I�ɉ������܂��B
	}
	
	[!if !ATTRIBUTED]
	DECLARE_REGISTRY_RESOURCEID(IDR_[!output UPPER_SHORT_NAME])

	DECLARE_NOT_AGGREGATABLE([!output CLASS_NAME])

	BEGIN_COM_MAP([!output CLASS_NAME])
		COM_INTERFACE_ENTRY(IWbemServices)
		COM_INTERFACE_ENTRY(IWbemProviderInit)
	END_COM_MAP()
	[!endif]

	//IWbemProviderInit
	HRESULT STDMETHODCALLTYPE Initialize( 
										LPWSTR pszUser,
										LONG lFlags,
										LPWSTR pszNamespace,
										LPWSTR pszLocale,
										IWbemServices *pNamespace,
										IWbemContext *pCtx,
										IWbemProviderInitSink *pInitSink);

	//IWbemServices
	HRESULT STDMETHODCALLTYPE GetObjectAsync(
										const BSTR ObjectPath,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemObjectSink __RPC_FAR *pResponseHandler);


	HRESULT STDMETHODCALLTYPE DeleteInstanceAsync(
										const BSTR ObjectPath,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemObjectSink __RPC_FAR *pResponseHandler);


	HRESULT STDMETHODCALLTYPE PutInstanceAsync(
										IWbemClassObject __RPC_FAR *pInst,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemObjectSink __RPC_FAR *pResponseHandler);


	HRESULT STDMETHODCALLTYPE CreateInstanceEnumAsync( 
										const BSTR Class,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemObjectSink __RPC_FAR *pResponseHandler);

	HRESULT STDMETHODCALLTYPE ExecQueryAsync( 
										const BSTR QueryLanguage,
										const BSTR Query,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemObjectSink __RPC_FAR *pResponseHandler);

	[!if PROVIDE_METHODS]
	HRESULT STDMETHODCALLTYPE ExecMethodAsync( 
										const BSTR strObjectPath,
										const BSTR strMethodName,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemClassObject __RPC_FAR *pInParams,
										IWbemObjectSink __RPC_FAR *pResponseHandler);
	[!else]
	HRESULT STDMETHODCALLTYPE ExecMethodAsync( 
										const BSTR strObjectPath,
										const BSTR strMethodName,
										long lFlags,
										IWbemContext __RPC_FAR *pCtx,
										IWbemClassObject __RPC_FAR *pInParams,
										IWbemObjectSink __RPC_FAR *pResponseHandler) 
	{	
		//��: ���\�b�h��^����ꍇ�́A
		//ExecMethodAsync() �̃C���v�������g���쐬����K�v������܂��B
		return WBEM_E_NOT_SUPPORTED;
	}
	[!endif]

};

[!if !ATTRIBUTED]
OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output CLASS_NAME])
[!endif]

[!endif]