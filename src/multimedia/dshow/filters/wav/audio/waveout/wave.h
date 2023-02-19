// Copyright (c) 1996 - 1999  Microsoft Corporation.  All Rights Reserved.
//-----------------------------------------------------------------------------
// Implements the CWaveOutDevice class based on waveOut APIs
//-----------------------------------------------------------------------------

extern AMOVIESETUP_FILTER wavFilter;

class CWaveOutDevice :
    public CSoundDevice
{

public:
    // define the public functions that this class exposes. These are all
    // straight calls to the corresponding waveOut APIs. Only the APIs that are
    // used by the Quartz wave renderer are declared and defined. We may have
    // to progressively add to this list.

    /* This goes in the factory template table to create new instances */

    static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *);

    MMRESULT amsndOutClose () ;
    MMRESULT amsndOutGetDevCaps (LPWAVEOUTCAPS pwoc, UINT cbwoc) ;
    MMRESULT amsndOutGetErrorText (MMRESULT mmrE, LPTSTR pszText, UINT cchText) ;
    MMRESULT amsndOutGetPosition (LPMMTIME pmmt, UINT cbmmt, BOOL bUseAdjustedPos) ;
    MMRESULT amsndOutOpen (LPHWAVEOUT phwo, LPWAVEFORMATEX pwfx,
                           double dRate, DWORD *pnAvgBytesPerSec,
                           DWORD_PTR dwCallBack, DWORD_PTR dwCallBackInstance, DWORD fdwOpen) ;
    MMRESULT amsndOutPause () ;
    MMRESULT amsndOutPrepareHeader (LPWAVEHDR pwh, UINT cbwh) ;
    MMRESULT amsndOutReset () ;
    MMRESULT amsndOutBreak () ;
    MMRESULT amsndOutRestart () ;
    MMRESULT amsndOutUnprepareHeader (LPWAVEHDR pwh, UINT cbwh) ;
    MMRESULT amsndOutWrite (LPWAVEHDR pwh, UINT cbwh, REFERENCE_TIME const *pStart, BOOL bIsDiscontinuity) ;

    // Routines required for initialisation and volume/balance handling
    // These are not part of the Win32 waveOutXxxx api set
    HRESULT  amsndOutCheckFormat (const CMediaType *pmt, double dRate);
    LPCWSTR  amsndOutGetResourceName () ;
    HRESULT  amsndOutGetBalance (LPLONG plBalance) ;
    HRESULT  amsndOutGetVolume (LPLONG plVolume) ;
    HRESULT  amsndOutSetBalance (LONG lVolume) ;
    HRESULT  amsndOutSetVolume (LONG lVolume) ;

    HRESULT  amsndOutLoad(IPropertyBag *pPropBag) ;

    HRESULT  amsndOutWriteToStream(IStream *pStream);
    HRESULT  amsndOutReadFromStream(IStream *pStream);
    int      amsndOutSizeMax();
    
    bool     amsndOutCanDynaReconnect() { return false; } // The wave out filter does not support dynamic reconnection

    CWaveOutDevice () ;
    ~CWaveOutDevice () ;

private:
    // Get current settings from the hardware and set member variables
    HRESULT GetVolume ();

    // Put current settings to the hardware using member variables
    HRESULT PutVolume();

    // Set up right/left amp factors
    void SetBalance();

	// Get the current balance from right/left amp factors
	void GetBalance();

    // volume is in the range -10000 to 0 (100th DB units)
    // amplitude and Balance are cumulative
    LONG        m_lVolume;
    LONG        m_lBalance;

    WORD	m_wLeft;		// Left channel volume
    WORD	m_wRight;		// Right channel volume
    DWORD   m_dwWaveVolume;
    BOOL	m_fHasVolume;		// wave device can set the volume
	BOOL	m_fBalanceSet;	// remember if the balance was explicitly set at least once

    HWAVEOUT    m_hWaveDevice;		// remember the handle of the open device

    UINT        m_iWaveOutId;           // output device to open

    WCHAR       m_wszResourceName[100]; // for resource manager
    void        SetResourceName();
};



