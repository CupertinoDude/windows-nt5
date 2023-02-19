//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1998 - 1999
//
//  File:       query.cpp
//
//--------------------------------------------------------------------------

#include "pch.h"
#include <SnapBase.h>

#include "query.h"
#include "editor.h"

#ifdef DEBUG_ALLOCATOR
	#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif
#endif

	///////////////////////////////////////////////////////////////////////////////
CADSIQueryObject::CADSIQueryObject()
{
  m_bInitialized = FALSE;
  m_pwszFilter = NULL;
  m_pObj = NULL;
  m_SearchHandle = NULL;
  aSearchPref = NULL;
}

CADSIQueryObject::~CADSIQueryObject()
{
  if (m_SearchHandle) 
  {
    m_pObj->CloseSearchHandle (m_SearchHandle);
  }
  if (aSearchPref != NULL)
  {
    delete aSearchPref;
    aSearchPref = NULL;
  }
}


HRESULT CADSIQueryObject::Init(IDirectorySearch * pObj)
{
  HRESULT hr = S_OK;
  
  m_pObj = pObj;
  m_bInitialized = TRUE;
  
  return hr;
}

HRESULT CADSIQueryObject::Init(CString ObjPath, CCredentialObject* pCredObject)
{
  HRESULT hr, hCredResult;

	hr = OpenObjectWithCredentials(
																 pCredObject, 
																 (LPWSTR)(LPCWSTR)ObjPath,
																 IID_IDirectorySearch, 
																 (LPVOID*) &m_pObj
																 );
  if (SUCCEEDED(hr)) 
	{
    m_bInitialized = TRUE;
  } 
	else 
	{
    m_bInitialized = FALSE;
    m_pObj = NULL;
  }
  return hr;
}

HRESULT CADSIQueryObject::SetAttributeList (LPTSTR *pszAttribs, INT cAttrs)
{

  m_pszAttribs = pszAttribs;
  m_nAttrs = cAttrs;
  return S_OK;
}

const int nSearchPrefs = 4;
HRESULT CADSIQueryObject::SetSearchPrefs (ADS_SCOPEENUM scope, ULONG nMaxObjectCount)
{
  HRESULT hr;
	int nNumPrefs = nSearchPrefs;
	if (nMaxObjectCount == 0)
	{
		nNumPrefs--;
	}
  aSearchPref = new ADS_SEARCHPREF_INFO[nNumPrefs];

  if (m_bInitialized) 
	{
    aSearchPref[0].dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
    aSearchPref[0].vValue.dwType = ADSTYPE_INTEGER;
    aSearchPref[0].vValue.Integer = scope;
    aSearchPref[1].dwSearchPref = ADS_SEARCHPREF_ASYNCHRONOUS;
    aSearchPref[1].vValue.dwType = ADSTYPE_BOOLEAN;
    aSearchPref[1].vValue.Boolean = TRUE;
    aSearchPref[2].dwSearchPref = ADS_SEARCHPREF_PAGESIZE;
    aSearchPref[2].vValue.dwType = ADSTYPE_INTEGER;
    aSearchPref[2].vValue.Integer = QUERY_PAGESIZE;
		
		if (nMaxObjectCount > 0)
		{
			aSearchPref[3].dwSearchPref = ADS_SEARCHPREF_SIZE_LIMIT;
			aSearchPref[3].vValue.dwType = ADSTYPE_INTEGER;
			aSearchPref[3].vValue.Integer = nMaxObjectCount;
		}
		hr = m_pObj->SetSearchPreference (aSearchPref, nNumPrefs);
    delete aSearchPref;
    aSearchPref = NULL;
  } 
	else 
	{
    hr = E_ADS_BAD_PATHNAME;
  }
  return hr;
}


const int NUM_PREFS=3;
HRESULT CADSIQueryObject::DoQuery()
{
  HRESULT hr;
  if (m_bInitialized) 
	{
     hr = m_pObj->ExecuteSearch (m_pwszFilter,
                                 m_pszAttribs,
                                 m_nAttrs,
                                 &m_SearchHandle);
  } 
	else 
	{
    hr = E_ADS_BAD_PATHNAME;
  }
  return hr;
}

HRESULT CADSIQueryObject::GetNextRow()
{
  if (m_bInitialized) 
	{
    return m_pObj->GetNextRow (m_SearchHandle);
  }
  return E_ADS_BAD_PATHNAME;
}

HRESULT CADSIQueryObject::GetColumn(LPWSTR Attribute, PADS_SEARCH_COLUMN pColumnData)
{
  if (m_bInitialized) 
	{
    return m_pObj->GetColumn (m_SearchHandle,
                              Attribute,
                              pColumnData);
  }
  return E_ADS_BAD_PATHNAME;
}

