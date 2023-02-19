//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1998  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  synthprp.h
//
//  CSynthProperties

class CSynthProperties : public CBasePropertyPage {

public:

    CSynthProperties(LPUNKNOWN lpUnk, HRESULT *phr);
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();
    BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

private:

    static BOOL CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void    InitPropertiesDialog(HWND hwndParent);
    void    OnFreqSliderNotification(WPARAM wParam, WORD wPosition);
    void    OnAmpSliderNotification(WPARAM wParam, WORD wPosition);
    void    RecalcFreqSlider(void );
    void    SetDirty(void );

    HWND    m_hwndFreqSlider;           // handle of slider
    HWND    m_hwndFreqText;             // Handle of frequency text window
    HWND    m_hwndAmplitudeSlider;      // handle of slider
    HWND    m_hwndAmplitudeText;        // Handle of amplitude text window

    int     m_iWaveformOriginal;        // WAVE_SINE ...
    int     m_iFrequencyOriginal;       // if not using sweep, this is the frequency
    int     m_iBitsPerSampleOriginal;   // 8 or 16
    int     m_iChannelsOriginal;        // 1 or 2
    int     m_iSamplesPerSecOriginal;   // 8000, 11025, ...
    int     m_iAmplitudeOriginal;       // 0 to 100
    int     m_iSweepStart;              // Sweep range on freq slider
    int     m_iSweepEnd;

	BOOL	m_fWindowInActive;		// TRUE ==> dialog is in the process of being destroyed

    ISynth  *m_pSynth;                  // Interface to the synthsizer
};
