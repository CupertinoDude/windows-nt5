// [!output IMPL_FILE] : Implementierung von [!output CLASS_NAME]
#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output HEADER_FILE]"


/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

//////////////////////////////////////////////////////////////////////////////
// Klassen-, Eigenschaften- und Methodennamen

// Wichtig: Die folgenden Strings sollten nicht lokalisiert werden.

[!if CLASS_SPECIFIED]
const static WCHAR * s_pMyClassName = L"[!output WMICLASSNAME]"; //Klassenname

//Eigenschaften:
[!output PROPERTY_DEFINITIONS]

//Methoden:
[!output METHOD_DEFINITIONS]

[!else]
//Aktion: Definieren Sie den angegebenen Klassenname, z.B.:
const static WCHAR * s_pMyClassName = L"MyClassName"; 

//Aktion: Definieren Sie die Eigenschaftennamen der angegebenen Klasse, z.B.:
//const static WCHAR * pMyProperty = L"MyProperty";

//Aktion: Definieren Sie die Methodennamen der angegebenen Klasse, z.B.:
//const static WCHAR * pMyMethod = L"MyMethod";
[!endif]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::Initialize
//In der MSDN-Dokumentation f�r IWbemProviderInit::Initialize()
//finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

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

  	//IWbemServices-Zeiger zwischenspeichern
	//Hinweis: m_pNamespace ist ein dynamischer Zeiger: AddRef() funktionieren automatisch
	m_pNamespace = pNamespace;
				
	//Definition der angegebenen Klasse zwischenspeichern
	//Hinweis: Der Code unten geht davon aus, dass die Klassendefinition w�hrend der
	//Anbieterausf�hrung unver�ndert bleibt. Andernfalls m�ssen Sie einen Benutzer
	//f�r Klassen�nderungen und Klassenl�schereignisse implementieren. Weitere
	//Informationen �ber Ereignisbenutzer finden Sie auf MSDN in der WMI-Dokumentenation.
	
    HRESULT hr = m_pNamespace->GetObject(CComBSTR(s_pMyClassName), 
											0, 
											pCtx, 	//IWbemContext-Zeiger werden umgangen, um Sperrungen zu verhindern
											&m_pClass, 
											NULL);
    if(FAILED(hr))
	{
        return WBEM_E_FAILED;
	}    
	
	//Pfadverarbeitungs-Klassenherstellung zwischenspeichern
	hr = CoGetClassObject(CLSID_WbemDefPath, 
							CLSCTX_INPROC_SERVER, 
							NULL,
							IID_IClassFactory,
							(void **) &m_pPathFactory);
	if (FAILED(hr))
	{
		return WBEM_E_FAILED;
	}

	//Hilfsobjekt erstellen
	m_pHelper = new CInstanceProviderHelper(pNamespace, pCtx);

	//Hinweis: Rufen Sie ConstructErrorObject() auf m_pHelper irgendwo auf 
	//dem Anbieter auf, um einen Fehler oder Status an WMI zu melden.

    pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
    return WBEM_S_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetObjectAsync
// In der MSDN-Dokumentation f�r IWbemServices::GetObjectAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

STDMETHODIMP [!output CLASS_NAME]::GetObjectAsync( 
							 const BSTR bstrObjectPath,
							 long lFlags,
							 IWbemContext  *pCtx,
							 IWbemObjectSink  *pResponseHandler)
{
    
	//Fehler: Abrufen pro Eigenschaft?
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
			   
   	//Falls der Vorgang richtig ausgef�hrt wurde, sollte das Objekt an WMI zur�ckgegeben werden und der erfolgreiche Vorgang angezeigt werden:
    pResponseHandler->Indicate (1, &(pInstance.p)); 
	pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);

    return WBEM_S_NO_ERROR;
	[!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::PutInstanceAsync()
// In der MSDN-Dokumentation f�r IWbemServices::PutInstanceAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

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
	//Aktion: �berpr�fen Sie die Flagwerte: WBEM_FLAG_UPDATE_ONLY, 
	//WBEM_FLAG_CREATE_ONLY und WBEM_FLAG_CREATE_OR_UPDATE.
	//W�hlen Sie eine Unterst�tzungsebene, und geben Sie
	//WBEM_E_PROVIDER_NOT_CAPABLE f�r nicht unterst�tzte Flagwerte zur�ck.

	//Aktion: Falls Updates auch teilweise unterst�tzt werden sollen, �berpr�fen 
	//Sie pCtx f�r "__PUT_EXTENSIONS" und andere relevante Systemkontextwerte,
	//und aktualisieren Sie die Instanzdaten dementsprechend.
	
	//Aktion: Verarbeiten Sie das Instanzupdate oder die -erstellung an dieser Stelle.			
	
	pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
	return WBEM_S_NO_ERROR;
	[!else]
	return WBEM_E_PROVIDER_NOT_CAPABLE;
	[!endif]
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::DeleteInstanceAsync()
// In der MSDN-Dokumentation f�r IWbemServices::DeleteInstanceAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

STDMETHODIMP [!output CLASS_NAME]::DeleteInstanceAsync( 
							const BSTR ObjectPath,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	//F�r die Methodenimplementierung verarbeitet ein Instanzanbieter die 
	//Objektpfadzeichenfolge, die im Parameter strObjectPath angegeben wird,
	//und versucht die passenden Instanz zu finden und zu l�schen.
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
		//Syntaxfehler im Pfad oder falscher Pfad f�r angegebene Klasse
		return WBEM_E_INVALID_PARAMETER; 
	}			
	
	[!if IS_SINGLETON]
	//Hinweis: [!output WMICLASSNAME] ist ein einzelnes Objekt. Instanz muss nicht identifiziert werden.
	//Hinweis: Falls die Instanz nicht vorhanden ist, muss die folgende Zeile auskommentiert werden, damit die Information an WMI zur�ckgegeben werden kann:
	//return WBEM_E_NOT_FOUND;
	[!else]
	//Pfadverarbeitungsobjekt erstellen
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

	//Werte der wichtigen Eigenschaften abrufen
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
		//Aktion: vValue f�r sp�tere Verwendung speichern
	}
	delete[] wKeyName;

	//Aktion: Suchen Sie in den Daten nach dem �bereinstimmenden Objekt, und l�schen Sie es.
	//Hinweis: Falls Sie kein Objekt, das mit den im Pfad angezeigten Schl�sselwerten �bereinstimmt, 
	//finden, muss die folgende Zeile auskommentiert werden, so dass diese Information an WMI �bermittelt wird:
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
// In der MSDN-Dokumentation f�r IWbemServices::CreateInstanceEnumAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

STDMETHODIMP [!output CLASS_NAME]::CreateInstanceEnumAsync( 
							const BSTR Class,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	

	//Wichtiger Hinweis: Der Instanzanbieter hat einen Thread von WMI �bernommen, um diese 
	//Vorg�nge auszuf�hren. Sie sollten die Datensenkmethode AddRef() aufrufen und einen
	//anderen Thread zum Zustellen der Objekte im Ergebnissatz erstellen. 
	//Somit k�nnen Sie den aktuellen Thread zum Zur�ckgeben an WMI verwenden, ohne dass der 
	//Threadpool gel�scht wird. Abh�ngig davon wie lange der Anbieter den WMI-Thread 
	//verwenden m�chte, wird ein einfacher oder ein doppelter Thread verwendet.

	[!if SUPPORT_ENUMERATE ]
	if (NULL == pResponseHandler)
	{
	    return WBEM_E_INVALID_PARAMETER;
	}

	HRESULT hr = WBEM_S_NO_ERROR;
	
	[!if IS_SINGLETON]
	// Ein leeres Objekt zum Abrufen der Instanzdaten verwenden
    CComPtr<IWbemClassObject> pNewInst;
    hr = m_pClass->SpawnInstance(0, &pNewInst);
		
	CComVariant var;
	[!if CLASS_SPECIFIED]
	[!output POPULATE_INSTANCE]
	[!else]
	//Aktion: F�llen Sie die Instanz mit Eigenschaften auf, z.B.:
	//CComVariant var;
	//var.ChangeType(VT_BSTR);
	//var = <value>;  //richtigen Wert hier setzen
	//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
	//var.Clear();
	[!endif]
		
	// Klasse an WMI �bermitteln
    pResponseHandler->Indicate(1, &(pNewInst.p));
	[!else]
    // Durch die pers�nliche Quelle schleifen und jede Instanz erstellen,
	//solange (while): (<weitere Instanzen vorhanden>)
    {
		// Ein leeres Objekt zum Empfangen der Klassendefinition vorbereiten
        CComPtr<IWbemClassObject> pNewInst;
        hr = m_pClass->SpawnInstance(0, &pNewInst);
		if (FAILED(hr))
		{	
			//Aktion: Kommentieren Sie die Zeile unten aus, sobald die Schleifenbedingung erstellt wurde
			//break;
		}
		
		CComVariant var;
		[!if CLASS_SPECIFIED]
		[!output POPULATE_INSTANCE]
		[!else]
		//Aktion: F�llen Sie die Instanz mit Eigenschaften auf, z.B.:
		//CComVariant var;
		//var.ChangeType(VT_BSTR);
		//var = <value>;  //richtigen Wert hier angegeben
		//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
		//var.Clear();
		[!endif]
		
		// Klasse an WMI �bermitteln
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
// In der MSDN-Dokumentation f�r IWbemServices::ExecQueryAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.

STDMETHODIMP [!output CLASS_NAME]::ExecQueryAsync( 
							const BSTR QueryLanguage,
							const BSTR Query,
							long lFlags,
							IWbemContext  *pCtx,
							IWbemObjectSink  *pResponseHandler)
{
	
	
   	// Instanzanbieter k�nnen Abfragen entweder selbst verarbeiten oder Abfragen von WMI 
	// verarbeiten lassen. Ein Instanzanbieter muss SQL-Anweisungen (Structured Query Language)  
	// verarbeiten, die angeforderte Anfrage ausf�hren und die Ergebnissatzobjekte der 
	// richtigen Datensenke zustellen, um Abfragen unterst�tzen zu k�nnen. 
	
	//Aktion: Verwenden Sie den Abfrageverarbeitungscode an dieser Stelle.
	
	//pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
	// return WBEM_S_NO_ERROR;

	return WBEM_E_PROVIDER_NOT_CAPABLE;
	
}

[!if PROVIDE_METHODS ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::ExecMethodAsync()
// In der MSDN-Dokumentation f�r IWbemServices::ExecMethodAsync()
// finden Sie weitere Einzelheiten �ber die Implementierung dieser Methode.
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
	//[!output WMICLASSNAME] enth�lt keine implementierten Methoden
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
	
	//Vergewissen Sich, dass der Klassenname im Pfad richtig ist.
	//Pfadverarbeitungsobjekt abrufen:
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
	//Aktion: Vergleichen Sie den angeforderten Methodenname mit den Methoden der Klassen.
	//if (!_wcsicmp (strMethodName, pMyNonStaticMethod))
	{
		CComVariant var;

		//Eingabeargumente abrufen:
		//Aktion: Falls die Methode Eingabeargumente enth�lt, werden diese wie Eigenschaften vom Objekt 
		//pInParams weitergeleitet. Die kommentierte Zeile unten gibt an, wie diese Argumente abgerufen werden:
		//hr = pInParams->Get(CComBSTR("InputArgument1"), 0, &var, NULL, NULL);
		//Aktion: Eingabeparameterwert speichern
		var.Clear();		
		
		//Verarbeitungspfad zum Suchen der Instanz f�r die Methodenausf�hrung: f�r nicht statische Methode
		CComPtr<IWbemPathKeyList> pIKeyList;
		hr = pPath->GetKeyList(&pIKeyList);
		if (FAILED(hr))
		{
			return WBEM_E_INVALID_PARAMETER;
		}
		unsigned long ulNumKeys;
		hr = pIKeyList->GetCount(&ulNumKeys);
		//Get values of key properties:
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
			//Aktion: vValue f�r sp�tere Verwendung speichern
		}
		delete[] wKeyName;
	
		//Aktion: Suchen Sie in der Datenquelle nach dem �bereinstimmenden Objekt.
		
		//Aktion: F�gen Sie Code hinzu, um die Methode an dieser Stelle auszuf�hren.
		//Ausgabeparameterklasse abrufen
		CComPtr<IWbemClassObject> pOutClass;
		hr = m_pClass->GetMethod(CComBSTR("Method1"), 0, NULL, &pOutClass);
		CComPtr<IWbemClassObject> pOutParams;
		pOutClass->SpawnInstance(0, &pOutParams);
		//Aktion: Erstellen Sie Ausgabeparameter, indem Sie Eigenschaften
		//von der pOutParams-Klasse auff�llen. Zum Beispiel:

		//var.ChangeType(VT_BSTR);
		//Variable mit richtigen Wert auff�llen
		//hr = pOutParams->Put(CComBSTR("MyOutputParameter"), 0, &var, 0);
		//var.Clear();
		//var.ChangeType(VT_I2);
		//Variable mit richtigen Wert auff�llen
		//hr = pOutParams->Put(CComBSTR("ReturnValue"), 0, &var, 0);
		//var.Clear();
		//Ausgabeobjekt zur�ck an den Client �ber die Datensenke senden
		hr = pResponseHandler->Indicate(1, &(pOutParams.p));
		
		pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
		return WBEM_S_NO_ERROR;
	}
	
	//Methodenname wurde nicht erkannt
	return WBEM_E_INVALID_PARAMETER;
	
	
	[!endif]	
}
[!endif]



//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetInstanceByPath() verarbeitet den Pfad, um erforderliche Schl�sselwerte
// zu finden. Dann wird der interne Speicher nach einem Objekt mit den passenden Schl�sselwerten durchsucht.
// Falls das Objekt gefunden wird, erstellt die Methode eine neue Instanz, f�llt die Eigenschaften auf
// und gibt sie in ppInstance zur�ck. Falls das Objekt nicht gefunden wird, wird WBEM_E_NOT_FOUND zur�ckgegeben.
// Fehler: Instanzteil einer Provision???  Andere m�gliche Flags?

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
			//Syntaxfehler im Pfad oder falscher Pfad f�r angegebene Klasse
			return WBEM_E_INVALID_PARAMETER; 
		}			
		
		[!if IS_SINGLETON]
		//[!output WMICLASSNAME] ist ein einzelnes Objekt. Instanz muss nicht identifiziert werden.
		//Hinweis: Falls die Instanz nicht vorhanden ist, sollte die folgende Zeile auskommentiert werden, 		//so dass diese Informationen an WMI �bermittelt wird:
		//return WBEM_E_NOT_FOUND;
		[!else]
		//Pfadverarbeitungsobjekt abrufen:
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

		//Werte der Schl�sseleigenschaften abrufen
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
			//Aktion: vValue f�r sp�tere Verwendung speichern
		}

		delete[] wKeyName;
		
		//Aktion: Suchen Sie in der Datenquelle nach dem �bereinstimmenden Objekt.
		//Falls keine Objekte mit den erforderlichen Schl�sselwerten gefunden werden, 
		//WBEM_E_NOT_FOUND zur�ckgeben.
		[!endif]
	   
		//Neue Instanz erstellen
		CComPtr<IWbemClassObject> pNewInst;
		hr = m_pClass->SpawnInstance(0, &pNewInst);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant var;

		//Aktion: F�llen Sie die Eigenschaften der neuen Instanz mit denen des �bereinstimmenden internen Objekts auf.
		[!if CLASS_SPECIFIED]
		[!output POPULATE_INSTANCE]			
		[!else]
		//Beispiel:
		//var.ChangeType(VT_BSTR);
		//var = <value>; //put appropriate value here
		//hr = pNewInst->Put(CComBSTR(pMyProperty), 0, &var, 0);
		//var.Clear();		
		[!endif]

		pNewInst.CopyTo(ppInstance);

		return hr;
}

