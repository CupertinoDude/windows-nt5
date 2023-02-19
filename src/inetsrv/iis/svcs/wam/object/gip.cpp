/*===================================================================
Microsoft Denali

Microsoft Confidential.
Copyright 1997 Microsoft Corporation. All Rights Reserved.

Component: Global Interface Pointer API support

File: Gip.cpp

Owner: DmitryR

This is the GIP source file.
===================================================================*/

#include "isapip.hxx"
#include "gip.h"

/*===================================================================
  Globals
===================================================================*/

CGlobalInterfaceAPI g_GIPAPI;

/*===================================================================
  C  G l o b a l  I n t e r f a c e  A P I
===================================================================*/

/*===================================================================
CGlobalInterfaceAPI::CGlobalInterfaceAPI

CGlobalInterfaceAPI constructor

Parameters:

Returns:
===================================================================*/	
CGlobalInterfaceAPI::CGlobalInterfaceAPI() 
    : m_fInited(FALSE), m_pGIT(NULL)
    {
    }
    
/*===================================================================
CGlobalInterfaceAPI::~CGlobalInterfaceAPI

CGlobalInterfaceAPI destructor

Parameters:

Returns:
===================================================================*/	
CGlobalInterfaceAPI::~CGlobalInterfaceAPI()
    {
    UnInit();
    }

/*===================================================================
CGlobalInterfaceAPI::Init

Creates instance of GlobalInterfaceTable

Parameters:

Returns:
    HRESULT
===================================================================*/	
HRESULT CGlobalInterfaceAPI::Init()
    {
    DBG_ASSERT(!m_fInited); // don't init twice
    
    HRESULT hr = CoCreateInstance
        (
        CLSID_StdGlobalInterfaceTable,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IGlobalInterfaceTable,
        (void **)&m_pGIT
        );
        
    if (SUCCEEDED(hr))
        m_fInited = TRUE;
    else
        m_pGIT = NULL;

    return hr;
    }

/*===================================================================
CGlobalInterfaceAPI::UnInit

Releases instance of GlobalInterfaceTable

Parameters:

Returns:
    HRESULT (NOERROR)
===================================================================*/	
HRESULT CGlobalInterfaceAPI::UnInit()
    {
    if (m_pGIT)
        {
        m_pGIT->Release();
        m_pGIT = NULL;
        }

    m_fInited = FALSE;
    return NOERROR;
    }
