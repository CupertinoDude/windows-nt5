// Copyright (c) 1996 - 1998  Microsoft Corporation.  All Rights Reserved.

#ifndef __ASYNCRDR_H__
#define __ASYNCRDR_H__

extern const AMOVIESETUP_FILTER sudAsyncRdr;

//
// AsyncRdr
//
// Defines a file source filter.
//
// This filter (CAsyncReader) supports IBaseFilter and IFileSourceFilter interfaces from the
// filter object itself. It has a single output pin (CAsyncOutputPin)
// which supports IPin and IAsyncReader.
//
// This filter is essentially a wrapper for the CAsyncFile class that does
// all the work.
//


// the filter class (defined below)
class CAsyncReader;


// the output pin class
class CAsyncOutputPin
  : public IAsyncReader,
    public CBasePin
{
protected:
    CAsyncReader* m_pReader;
    CAsyncFile * m_pFile;

    //  This is set every time we're asked to return an IAsyncReader
    //  interface
    //  This allows us to know if the downstream pin can use
    //  this transport, otherwise we can hook up to things like the
    //  dump filter and nothing happens
    BOOL         m_bQueriedForAsyncReader;

    HRESULT InitAllocator(IMemAllocator **ppAlloc);

public:
    // constructor and destructor
    CAsyncOutputPin(
        HRESULT * phr,
	CAsyncReader *pReader,
	CAsyncFile *pFile,
        CCritSec * pLock);

    ~CAsyncOutputPin();

    // --- CUnknown ---

    // need to expose IAsyncReader
    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void**);

    // --- CBasePin methods ---

    // return the types we prefer - this will return the known
    // file type
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

    // can we support this type?
    HRESULT CheckMediaType(const CMediaType* pType);

    // Clear the flag so we see if IAsyncReader is queried for
    HRESULT CheckConnect(IPin *pPin)
    {
        m_bQueriedForAsyncReader = FALSE;
        return CBasePin::CheckConnect(pPin);
    }

    // See if it was asked for
    HRESULT CompleteConnect(IPin *pReceivePin)
    {
        if (m_bQueriedForAsyncReader) {
            return CBasePin::CompleteConnect(pReceivePin);
        } else {
            return VFW_E_NO_TRANSPORT;
        }
    }

    // --- IAsyncReader methods ---
    // pass in your preferred allocator and your preferred properties.
    // method returns the actual allocator to be used. Call GetProperties
    // on returned allocator to learn alignment and prefix etc chosen.
    // this allocator will be not be committed and decommitted by
    // the async reader, only by the consumer.
    STDMETHODIMP RequestAllocator(
                      IMemAllocator* pPreferred,
                      ALLOCATOR_PROPERTIES* pProps,
                      IMemAllocator ** ppActual);

    // queue a request for data.
    // media sample start and stop times contain the requested absolute
    // byte position (start inclusive, stop exclusive).
    // may fail if sample not obtained from agreed allocator.
    // may fail if start/stop position does not match agreed alignment.
    // samples allocated from source pin's allocator may fail
    // GetPointer until after returning from WaitForNext.
    STDMETHODIMP Request(
                     IMediaSample* pSample,
                     DWORD_PTR dwUser);	        // user context

    // block until the next sample is completed or the timeout occurs.
    // timeout (millisecs) may be 0 or INFINITE. Samples may not
    // be delivered in order. If there is a read error of any sort, a
    // notification will already have been sent by the source filter,
    // and STDMETHODIMP will be an error.
    STDMETHODIMP WaitForNext(
                      DWORD dwTimeout,
                      IMediaSample** ppSample,  // completed sample
                      DWORD_PTR * pdwUser);		// user context

    // sync read of data. Sample passed in must have been acquired from
    // the agreed allocator. Start and stop position must be aligned.
    // equivalent to a Request/WaitForNext pair, but may avoid the
    // need for a thread on the source filter.
    STDMETHODIMP SyncReadAligned(
                      IMediaSample* pSample);


    // sync read. works in stopped state as well as run state.
    // need not be aligned. Will fail if read is beyond actual total
    // length.
    STDMETHODIMP SyncRead(
                      LONGLONG llPosition,	// absolute file position
                      LONG lLength,		// nr bytes required
                      BYTE* pBuffer);		// write data here

    // return total length of stream, and currently available length.
    // reads for beyond the available length but within the total length will
    // normally succeed but may block for a long period.
    STDMETHODIMP Length(
                      LONGLONG* pTotal,
                      LONGLONG* pAvailable);

    // cause all outstanding reads to return, possibly with a failure code
    // (VFW_E_TIMEOUT) indicating they were cancelled.
    // these are defined on IAsyncReader and IPin
    STDMETHODIMP BeginFlush(void);
    STDMETHODIMP EndFlush(void);

};


//
// The filter object itself. Supports IBaseFilter through
// CBaseFilter and also IFileSourceFilter directly in this object

class CAsyncReader : public CBaseFilter, public IFileSourceFilter
{
    // filter-wide lock
    CCritSec m_csFilter;

    // all i/o done here
    CAsyncFile m_file;

    // our output pin
    CAsyncOutputPin m_OutputPin;

    LPOLESTR   	          m_pFileName; // null until loaded
    CMediaType            m_mt;        // type loaded with

public:
		
    // construction / destruction

    static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *);
    CAsyncReader(
        TCHAR *pName,
        LPUNKNOWN pUnk,
        HRESULT *phr);
    ~CAsyncReader();



    // -- CUnknown methods --

    // we export IFileSourceFilter plus whatever is in CBaseFilter
    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);

    // -- IFileSourceFilter methods ---

    STDMETHODIMP Load(LPCOLESTR pszFileName, const AM_MEDIA_TYPE *mt);
    STDMETHODIMP GetCurFile(LPOLESTR * ppszFileName, AM_MEDIA_TYPE *mt);

    // --- CBaseFilter methods ---
    int GetPinCount();
    CBasePin *GetPin(int n);

    // --- Access our media type
    const CMediaType *LoadType() const
    {
        return &m_mt;
    }
};



#endif //__ASYNCRDR_H__
