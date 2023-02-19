//+---------------------------------------------------------------------------
//
//  Copyright (c) 1997, Microsoft Corporation.
//
//  File:       mmistrm.hxx
//
//  Contents:   Memory Mapped IStream
//
//  Classes:    CMmIStream
//
//  History:    11-Feb-97 KyleP     Created
//
//----------------------------------------------------------------------------

#pragma once

#include <pmmstrm.hxx>

//+---------------------------------------------------------------------------
//
//  Class:      CMmIStream
//
//  Purpose:    Memory Mapped Stream, based on IStream
//
//  History:    11-Feb-97       KyleP         Created
//
//----------------------------------------------------------------------------

class CMmIStream: INHERIT_VIRTUAL_UNWIND, public PMmStream
{
    friend class CMmStreamBuf;

    INLINE_UNWIND(CMmIStream)

public:

    CMmIStream();

    BOOL Ok() { return _pStream != 0; }

    virtual ~CMmIStream();

    void Open( IStream * pStream );

    void Close();

    void SetSize( PStorage& storage, ULONG newSizeLow, ULONG newSizeHigh ) {}

    BOOL   isEmpty() { return _statstg.cbSize.HighPart == 0 && _statstg.cbSize.LowPart == 0; }

    ULONG  SizeLow() { return _statstg.cbSize.LowPart; }

    ULONG  SizeHigh() { return _statstg.cbSize.HighPart; }

    LONGLONG Size() { return _statstg.cbSize.QuadPart; }

    BOOL   isWritable() { return FALSE; }

    void   MapAll ( CMmStreamBuf& sbuf );

    void   Map ( CMmStreamBuf& sbuf,
                 ULONG cb = 0,
                 ULONG offLow = 0,
                 ULONG offHigh = 0,
                 BOOL  fMapForWrite = FALSE );

    void   Unmap ( CMmStreamBuf& sbuf );

    void   Flush ( CMmStreamBuf& sbuf, ULONG cb, BOOL fThrowOnFailure = TRUE );

    void   FlushMetaData ( BOOL fThrowOnFailure = TRUE );

protected:

    IStream * _pStream;        // Stream
    BYTE *    _pBuf;           // Stream is 'mapped' (buffered) here

    STATSTG   _statstg;        // Stream size, etc.
};

