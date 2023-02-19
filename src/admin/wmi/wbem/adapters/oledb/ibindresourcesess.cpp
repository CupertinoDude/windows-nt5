///////////////////////////////////////////////////////////////////////////
//
// Microsoft WMIOLE DB Provider
//
// (C) Copyright 1999 Microsoft Corporation. All Rights Reserved.
//
//  IBindResource.CPP CImplIBindRsrc interface implementation for 
//	Session object
//
///////////////////////////////////////////////////////////////////////////

#include "headers.h"

///////////////////////////////////////////////////////////////////////////
// Method of the IBindResource which binds the requested URL
//			S_OK						Bind succeeded
//			DB_S_ERRORSOCCURRED			Bind succeeded, but some bind flags 
//										or properties were not satisfied
//			DB_E_NOAGGREGATION			Aggregation not supported by the 
//										object requested
//			DB_E_NOTFOUND				Object requested as from URL not found
//			DB_E_OBJECTMISMATCH			The object requested and the URL passed
//										does not match
//			DB_SEC_E_PERMISSIONDENIED	User does not have permission for the
//										object requested
//			E_FAIL						Other error ( WMI specifice errors)
//			E_INVALIDARG				one or more arguments are not valid
//			E_NOINTERFACE				The interface requested is not supported
//			E_UNEXPECTED				unexpected error
///////////////////////////////////////////////////////////////////////////
STDMETHODIMP CImplIBindRsrc::Bind( IUnknown *            pUnkOuter,
									   LPCOLESTR             pwszURL,
									   DBBINDURLFLAG         dwBindURLFlags,
									   REFGUID               rguid,
									   REFIID                riid,
									   IAuthenticate *       pAuthenticate,
									   DBIMPLICITSESSION *   pImplSession,
									   DBBINDURLSTATUS *     pdwBindStatus,
									   IUnknown **           ppUnk)
{ 
	HRESULT hr = S_OK;
	BSTR strUrl;
    CSetStructuredExceptionHandler seh;

	TRY_BLOCK;

	//=========================================================================================
	// Serialize the object
	//=========================================================================================
	CAutoBlock	cab(m_pObj->GetCriticalSection());

	// Clear ErrorInfo
	g_pCError->ClearErrorInfo();

	//=========================================================================================
	// If URL is NULL return Invalid Argument
	//=========================================================================================
	if(pwszURL == NULL)
	{
		hr = E_INVALIDARG;
	}
	else
	{

		//=========================================================================================
		// Allocate the string
		//=========================================================================================
		strUrl = Wmioledb_SysAllocString(pwszURL);

		//=========================================================================================
		// Check if the flags passed are valid for the object requested
		//=========================================================================================
		if(!CheckBindURLFlags(dwBindURLFlags,rguid))
		{
			hr =  E_INVALIDARG;
		}
		else
		if(!CheckIfProperURL(strUrl,rguid,pdwBindStatus))
		{
			hr =  DB_E_OBJECTMISMATCH;
		}
		else
		if( pUnkOuter != NULL && riid != IID_IUnknown)
		{
			hr = DB_E_NOAGGREGATION;
		}
		if(SUCCEEDED(hr))
		{
			//=========================================================================================
			// Calling this to bind the URL to the appropriate object
			//=========================================================================================
			hr = BindURL(pUnkOuter,strUrl,dwBindURLFlags,rguid,riid,pImplSession,pdwBindStatus,ppUnk);
		}

		// Free the string
		SysFreeString(strUrl);
	}
	hr = hr == S_OK ? hr :g_pCError->PostHResult(hr,&IID_IBindResource);

	CATCH_BLOCK_HRESULT(hr,L"IBindResource::Bind on Session object");
	return hr;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Function which checks if the URL flags matches the requested object
//  This is as per the OLEDB specs
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CImplIBindRsrc::CheckBindURLFlags(DBBINDURLFLAG dwBindURLFlags , REFGUID rguid)
{
	BOOL bFlag = FALSE;
	LONG lTemp = 0;

	if( DBGUID_DSO == rguid)
	{
		lTemp = DBBINDURLFLAG_ASYNCHRONOUS | DBBINDURLFLAG_READ | DBBINDURLFLAG_WAITFORINIT;
		//=========================================================================================
		// Flags can have only these values
		//=========================================================================================
		if((dwBindURLFlags & ~lTemp) == 0)
			bFlag = TRUE;

	}

	if( DBGUID_SESSION == rguid)
	{
		if( dwBindURLFlags == DBBINDURLFLAG_READ) // Flags can have only these values
			bFlag = TRUE;
	}

	if( DBGUID_COMMAND == rguid)
	{
		lTemp = DBBINDURLFLAG_READ | DBBINDURLFLAG_WAITFORINIT;
		if((dwBindURLFlags & ~lTemp) == 0)
			bFlag = TRUE;
	}

	if( DBGUID_ROW == rguid)
	{
		bFlag = TRUE;
	}

	if( DBGUID_ROWSET == rguid)
	{
		if(!((dwBindURLFlags & DBBINDURLFLAG_DELAYFETCHCOLUMNS) ||			// Flags cannot have any of these two values
			(dwBindURLFlags & DBBINDURLFLAG_DELAYFETCHSTREAM)))
			bFlag = TRUE;

	}

	if( DBGUID_STREAM == rguid)
	{
	}


	return bFlag;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Function which checks if the URL is valid for the requested object
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CImplIBindRsrc::CheckIfProperURL(BSTR & strUrl,REFGUID rguid,DBBINDURLSTATUS * pdwBindStatus)
{
	BOOL bRet = TRUE;
	LONG lUrlType = -1;
	CURLParser urlParser;
	
	//=========================================================================================
	// Set the URL string of the URL parser utility class
	//=========================================================================================
	if(SUCCEEDED(urlParser.SetURL(strUrl)))
	{
		// If the url is a valid URL
		if((lUrlType = urlParser.GetURLType()) != -1)
		{
			switch(lUrlType)
			{
				case URL_ROW:
				case URL_EMBEDEDCLASS:
					bRet = TRUE;

					//=========================================================================================
					// if the url is of type row or rowset and if the requested object is
					// datasource or session or command , then set the status 
					//=========================================================================================
					if(rguid == DBGUID_DSO || rguid == DBGUID_SESSION || rguid == DBGUID_COMMAND)
						*pdwBindStatus = DBBINDURLSTATUS_S_REDIRECTED;					

					break;

				case URL_DATASOURCE:
					//=========================================================================================
					// check if the url is of type datasource and the requested object is
					// row or rowset or stream 
					//=========================================================================================
					if(!(rguid == DBGUID_ROW || rguid == DBGUID_ROWSET || rguid == DBGUID_STREAM))
						bRet = TRUE;

					break;

				case URL_ROWSET:

					//=========================================================================================
					// if the url is of type rowset and if the requested object is
					// datasource or session or command , then set the status 
					//=========================================================================================
					if(rguid == DBGUID_DSO || rguid == DBGUID_SESSION || rguid == DBGUID_COMMAND)
						*pdwBindStatus = DBBINDURLSTATUS_S_REDIRECTED;
	
					bRet = TRUE;
					
					if(rguid == DBGUID_ROW)
						bRet = FALSE;

					break;
			};
		}
	}

	return bRet;
	
}



///////////////////////////////////////////////////////////////////////////
// Function to bind the requested URL
// NTRaid:136545
// 07/05/00
///////////////////////////////////////////////////////////////////////////
HRESULT CImplIBindRsrc::BindURL(IUnknown *            pUnkOuter,
									LPCOLESTR             pwszURL,
									DBBINDURLFLAG         dwBindURLFlags,
									REFGUID               rguid,
									REFIID                riid,
									DBIMPLICITSESSION *   pImplSession,
									DBBINDURLSTATUS *     pdwBindStatus,
									IUnknown **           ppUnk)
{

	HRESULT hr		= E_FAIL;
	BSTR	strURL	= NULL;
	
	strURL	= Wmioledb_SysAllocString(pwszURL);

	
	//=========================================================================================
	// If the requested object is DSO then create the object aggregating with the outer object
	//=========================================================================================
	if( rguid == DBGUID_DSO)
	{

		hr = m_pObj->m_pCDataSource->QueryInterface(riid,(void **)ppUnk);

	}
	else
	//=========================================================================================
	// If the requested object is Session object then QI for the requiested object
	//=========================================================================================
	if( rguid == DBGUID_SESSION)
	{
		hr = m_pObj->QueryInterface(riid,(void **)ppUnk);
	}
	else
	//=========================================================================================
	// If requested object is command then call function to 
	// to create a command
	//=========================================================================================
	if(rguid == DBGUID_COMMAND)
	{
		hr = m_pObj->CreateCommand(pUnkOuter,riid,ppUnk);
	}
	//=========================================================================================
	// If requested object is row then call function to 
	// to create a row
	//=========================================================================================
	if( rguid == DBGUID_ROW)
	{
		hr = m_pObj->CreateRow(pUnkOuter,strURL,riid,ppUnk);
	}
	//=========================================================================================
	// If requested object is rowset then call function to 
	// to create a rowset
	//=========================================================================================
	if( rguid == DBGUID_ROWSET)
	{
		hr = m_pObj->CreateRowset(pUnkOuter,strURL,riid,ppUnk);
	}


	if(! SUCCEEDED(hr))
	{
		*ppUnk = NULL;
	}

	SysFreeString(strURL);

	return hr ;
}


