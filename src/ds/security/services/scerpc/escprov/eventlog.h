// eventlog.h: interface for the CEventLogSettings class.
//
// Copyright (c)1997-1999 Microsoft Corporation
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTLOG_H__BD7570F7_9F0E_4C6B_B525_E078691B6D0E__INCLUDED_)
#define AFX_EVENTLOG_H__BD7570F7_9F0E_4C6B_B525_E078691B6D0E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GenericClass.h"

/*

Class description
    
    Naming: 

        CEventLogSettings.
    
    Base class: 

        CGenericClass, because it is a class representing a WMI  
        object - its WMI class name is Sce_EventLog
        
    Purpose of class:
    
        (1) Implement Sce_EventLog WMI class.
    
    Design:
         
        (1) Almost trivial other than implementing necessary method as a concrete class
    
    Use:
        (1) Almost never used directly. Always through the common interface defined by
            CGenericClass.
    
*/

class CEventLogSettings : public CGenericClass
{
public:
        CEventLogSettings (
                           ISceKeyChain *pKeyChain, 
                           IWbemServices *pNamespace, 
                           IWbemContext *pCtx = NULL
                           );

        virtual ~CEventLogSettings ();

        virtual HRESULT PutInst (
                                 IWbemClassObject *pInst, 
                                 IWbemObjectSink *pHandler, 
                                 IWbemContext *pCtx
                                 );

        virtual HRESULT CreateObject (
                                      IWbemObjectSink *pHandler, 
                                      ACTIONTYPE atAction
                                      );

private:

        HRESULT ConstructInstance (
                                   IWbemObjectSink *pHandler, 
                                   CSceStore* pSceStore, 
                                   LPCWSTR wszLogStorePath, 
                                   LPCWSTR wszLogType, 
                                   BOOL bPostFilter 
                                   );

        HRESULT DeleteInstance (
                                IWbemObjectSink *pHandler, 
                                CSceStore* pSceStore, 
                                LPCWSTR wszLogType 
                                );

        HRESULT ValidateEventlogType (
                                      LPCWSTR wszLogType, 
                                      DWORD *pIndex
                                      );

        HRESULT SaveSettingsToStore (
                                     CSceStore* pScePersist, 
                                     LPCWSTR Section,
                                     DWORD dwSize, 
                                     DWORD dwRet, 
                                     DWORD dwDays, 
                                     DWORD dwRestrict
                                     );

};

PCWSTR GetEventLogType (DWORD idx);

#endif // !defined(AFX_EVENTLOG_H__BD7570F7_9F0E_4C6B_B525_E078691B6D0E__INCLUDED_)
