///*M*
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a licence agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 1997 Intel Corporation. All Rights Reserved.
//
// Filename : SPHProp.h
// Purpose  : Define the class that implements the RTP SPH
//            filter property page.
// Contents : 
//      class CSPHPropertyPage
//*M*/

#ifndef _SPHPROP_H_
#define _SPHPROP_H_

class 
CSPHPropertyPage 
: public CBasePropertyPage
{
	DWORD m_dwIDD_Base;

    static CUnknown * WINAPI
	CreateInstance2( LPUNKNOWN punk, HRESULT *phr, DWORD IDD_Base );
	
public:
    static CUnknown * WINAPI
	CreateInstance_aud( LPUNKNOWN punk, HRESULT *phr );

    static CUnknown * WINAPI
	CreateInstance_gena( LPUNKNOWN punk, HRESULT *phr );

    static CUnknown * WINAPI
	CreateInstance_genv( LPUNKNOWN punk, HRESULT *phr );

    static CUnknown * WINAPI
	CreateInstance_h26x( LPUNKNOWN punk, HRESULT *phr );

protected:
    INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate () ;
    HRESULT OnApplyChanges();

    CSPHPropertyPage( LPUNKNOWN punk, HRESULT *phr, DWORD IDD_Base);

    BOOL OnInitDialog( void );
    BOOL OnCommand( int iButton, int iNotify, LPARAM lParam );

    void SetDirty();

protected:
	IRTPSPHFilter	*m_pIRTPSPHFilter;
    BOOL m_bIsInitialized;  // Will be false while we set init values in Dlg

	BOOL m_bPayloadScanned;
	BOOL m_bMTUScanned;
	int m_dwPayloadType;
	DWORD m_dwMTUsize;
};

#endif _SPHPROP_H_
