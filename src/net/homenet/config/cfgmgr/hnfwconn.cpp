//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1997 - 2000
//
//  File:       H N F W C O N N . C P P
//
//  Contents:   CHNFWConn implementation
//
//  Notes:
//
//  Author:     jonburs 23 June 2000
//
//----------------------------------------------------------------------------

#include "pch.h"
#pragma hdrstop

//
// IHNetFirewalledConnection methods
//

STDMETHODIMP
CHNFWConn::Unfirewall()

{
    HRESULT hr = S_OK;
    IWbemClassObject *pwcoProperties;

    if (ProhibitedByPolicy(NCPERM_PersonalFirewallConfig))
    {
        hr = HN_E_POLICY;
    }

    if (S_OK == hr)
    {
        hr = GetConnectionPropertiesObject(&pwcoProperties);
    }

    if (WBEM_S_NO_ERROR == hr)
    {
        //
        // Change our IsFirewalled property to false
        //
        

        hr = SetBooleanValue(
                pwcoProperties,
                c_wszIsFirewalled,
                FALSE
                );

        if (WBEM_S_NO_ERROR == hr)
        {
            //
            // Write the instance to the store
            //

            hr = m_piwsHomenet->PutInstance(
                    pwcoProperties,
                    WBEM_FLAG_UPDATE_ONLY,
                    NULL,
                    NULL
                    );
        }

        if (WBEM_S_NO_ERROR == hr)
        {
            //
            // Inform netman that something changed. Error doesn't matter.
            //
            
            UpdateNetman();
        }

        pwcoProperties->Release();
    }

    if (WBEM_S_NO_ERROR == hr)
    {
        HRESULT hr2;
        
        //
        // Stop or send an update to the homenet service. We don't
        // propagate an error here, as the store correctly reflects
        // the unfirewalled state. Success is asserted on debug builds.
        //

        hr2 = UpdateOrStopService(
                m_piwsHomenet,
                m_bstrWQL,
                IPNATHLP_CONTROL_UPDATE_CONNECTION
                );
                
        RefreshNetConnectionsUI();
        _ASSERT(SUCCEEDED(hr2));
    }

    return hr;
}

