//***************************************************************************
//
//  Copyright (c) 2000-2001 Microsoft Corporation
//
//  OPGETIN.CPP
//
//  alanbos  07-Nov-00   Created.
//
//  WMI Get Classes operation implementation.  
//
//***************************************************************************

#include "precomp.h"

HRESULT WMIGetInstancesOperation::BeginRequest (
	CComPtr<IWbemServices> & pIWbemServices
)
{
	HRESULT hr = WBEM_E_FAILED;
	LONG lFlags = WBEM_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY;

	if (m_bUseAmendedQualifiers)
		lFlags |= WBEM_FLAG_USE_AMENDED_QUALIFIERS;

	if (m_bDirectRead)
		lFlags |= WBEM_FLAG_DIRECT_READ;

	if (!m_bDeep)
		lFlags |= WBEM_FLAG_SHALLOW;

	// Call the enumerator
	if (SUCCEEDED(hr = pIWbemServices->CreateInstanceEnum (
						m_bsClassBasis, 
						lFlags, 
						GetContext(), 
						&m_pIEnumWbemClassObject)))
	{
		/*
		 * Get the first object - we do this as we are operating in semi-sync and some
		 * errors are only flagged on Next, not on CreateInstanceEnum.
		 */
		ULONG lReturned = 0;
		hr = m_pIEnumWbemClassObject->Next (WBEM_INFINITE, 1, &m_pFirstIWbemClassObject, &lReturned);

		if ((S_OK == hr) && (1 == lReturned))
			m_bIsEmptyResultsSet = false;
	}

	return hr;
}

HRESULT WMIGetInstancesOperation::ProcessRequest (void)
{
	HRESULT hr = S_OK;
	
	// Start by checking if we have an initial object
	if (m_pFirstIWbemClassObject)
	{
		hr = EncodeAndSendInstance (m_pFirstIWbemClassObject);
		m_pFirstIWbemClassObject.Release ();
	}

	// Now send the remainder
	if (SUCCEEDED(hr))
	{
		CComPtr<IWbemClassObject> pIWbemClassObject;
		ULONG lReturned = 0;

		while ((S_OK == (hr = m_pIEnumWbemClassObject->Next (WBEM_INFINITE, 1, &pIWbemClassObject, &lReturned)))
				&& (1 == lReturned))
		{
			if (FAILED(hr = EncodeAndSendInstance (pIWbemClassObject)))
				break;

			lReturned = 0;
		}
	}

	return hr;
}

bool WMIGetInstancesOperation::ProcessElement( 
      const wchar_t __RPC_FAR *pwchNamespaceUri,
      int cchNamespaceUri,
      const wchar_t __RPC_FAR *pwchLocalName,
      int cchLocalName,
      const wchar_t __RPC_FAR *pwchRawName,
      int cchRawName,
      ISAXAttributes __RPC_FAR *pAttributes)
{
	bool result = false;

	if (0 == wcscmp(WMI_GETCLASSES_PARAMETER_BASIS, pwchLocalName))
	{
		// following content will be the value of the object name
		SetParseState (ClassBasis);
		result = true;
	}
	else if (0 == wcscmp(WMI_GETOBJECT_PARAMETER_USEAQ, pwchLocalName))
	{
		// following content will be the value of UseAmendedQualifiers
		SetParseState (UseAmendedQualifiers);
		result = true;
	}
	else if (0 == wcscmp(WMI_GETINSTANCES_PARAMETER_DIRECTREAD, pwchLocalName))
	{
		// following content will be the value of DirectRead
		SetParseState (DirectRead);
		result = true;
	}
	else if (0 == wcscmp(WMI_GETCLASSES_PARAMETER_DEEP, pwchLocalName))
	{
		// following content will be the value of DirectRead
		SetParseState (Deep);
		result = true;
	}

	return result;
}

bool WMIGetInstancesOperation::ProcessContent (
        const unsigned short * pwchChars,
        int cchChars )
{
	bool result = true;

	switch (GetParseState ())
	{
		case ClassBasis:
			m_bsClassBasis = SysAllocStringLen (pwchChars, cchChars);
			break;

		case UseAmendedQualifiers:
			{
				if (0 == wcsncmp (pwchChars, L"true", cchChars))
					m_bUseAmendedQualifiers = true;
				else if (0 == wcsncmp (pwchChars, L"false", cchChars))
					m_bUseAmendedQualifiers = false;
				else
					result = false;
			}
			break;

		case DirectRead:
			{
				if (0 == wcsncmp (pwchChars, L"true", cchChars))
					m_bDirectRead = true;
				else if (0 == wcsncmp (pwchChars, L"false", cchChars))
					m_bDirectRead = false;
				else
					result = false;
			}
			break;

		case Deep:
			{
				if (0 == wcsncmp (pwchChars, L"true", cchChars))
					m_bDeep = true;
				else if (0 == wcsncmp (pwchChars, L"false", cchChars))
					m_bDeep = false;
				else
					result = false;
			}
			break;
	}

	return result;
}
