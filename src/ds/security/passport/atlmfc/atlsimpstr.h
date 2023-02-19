// This is a part of the Active Template Library.
// Copyright (C) 1996-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef __ATLSIMPSTR_H__
#define __ATLSIMPSTR_H__

#pragma once

#include <atldef.h>
#include <atlbase.h>
#include <atlexcept.h>
#include <atlmem.h>

namespace ATL
{

struct CStringData;

__interface IAtlStringMgr
{
public:
	// Allocate a new CStringData
	CStringData* Allocate( int nAllocLength, int nCharSize ) throw();
	// Free an existing CStringData
	void Free( CStringData* pData ) throw();
	// Change the size of an existing CStringData
	CStringData* Reallocate( CStringData* pData, int nAllocLength, int nCharSize ) throw();
	// Get the CStringData for a Nil string
	CStringData* GetNilString() throw();
	IAtlStringMgr* Clone() throw();
};

#ifdef _M_IX86
#ifndef _M_CEE
extern "C"
{
	LONG _InterlockedIncrement( LONG* pn );
	LONG _InterlockedDecrement( LONG* pn );
};

#pragma intrinsic( _InterlockedIncrement )
#pragma intrinsic( _InterlockedDecrement )
#else
#define _InterlockedIncrement InterlockedIncrement
#define _InterlockedDecrement InterlockedDecrement
#endif  // !_M_CEE
#endif  // _M_IX86_

struct CStringData
{
	IAtlStringMgr* pStringMgr;  // String manager for this CStringData
	int nDataLength;  // Length of currently used data in XCHARs (not including terminating null)
	int nAllocLength;  // Length of allocated data in XCHARs (not including terminating null)
	long nRefs;     // Reference count: negative == locked
	// XCHAR data[nAllocLength+1]  // A CStringData is always followed in memory by the actual array of character data

	void* data() throw()
	{
		return (this+1);
	}

	void AddRef() throw()
	{
		ATLASSERT(nRefs > 0);
		_InterlockedIncrement(&nRefs);
	}
	bool IsLocked() const throw()
	{
		return nRefs < 0;
	}
	bool IsShared() const throw()
	{
		return( nRefs > 1 ); 
	}
	void Lock() throw()
	{
		ATLASSERT( nRefs <= 1 );
		nRefs--;  // Locked buffers can't be shared, so no interlocked operation necessary
		if( nRefs == 0 )
		{
			nRefs = -1;
		}
	}
	void Release() throw()
	{
		ATLASSERT( nRefs != 0 );

		if( _InterlockedDecrement( &nRefs ) <= 0 )
		{
			pStringMgr->Free( this );
		}
	}
	void Unlock() throw()
	{
		ATLASSERT( IsLocked() );
		nRefs++;  // Locked buffers can't be shared, so no interlocked operation necessary
		if( nRefs == 0 )
		{
			nRefs = 1;
		}
	}
};

class CNilStringData :
	public CStringData
{
public:
	CNilStringData() throw()
	{
		pStringMgr = NULL;
		nRefs = 2;  // Never gets freed by IAtlStringMgr
		nDataLength = 0;
		nAllocLength = 0;
		achNil[0] = 0;
		achNil[1] = 0;
	}

	void SetManager( IAtlStringMgr* pMgr ) throw()
	{
		ATLASSERT( pStringMgr == NULL );
		pStringMgr = pMgr;
	}

public:
	wchar_t achNil[2];
};

class CAtlStringMgr :
	public IAtlStringMgr
{
public:
	CAtlStringMgr( IAtlMemMgr* pMemMgr = NULL ) throw() :
		m_pMemMgr( pMemMgr )
	{
		m_nil.SetManager( this );
	}
	~CAtlStringMgr() throw()
	{
	}

	void SetMemoryManager( IAtlMemMgr* pMemMgr ) throw()
	{
		ATLASSERT( m_pMemMgr == NULL );
		m_pMemMgr = pMemMgr;
	}

// IAtlStringMgr
public:
	virtual CStringData* Allocate( int nChars, int nCharSize ) throw()
	{
		size_t nTotalSize;
		CStringData* pData;
		size_t nDataBytes;

		nChars = AtlAlignUp( nChars + 1, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.

		nDataBytes = nChars*nCharSize;
		nTotalSize = sizeof( CStringData )+nDataBytes;
		pData = static_cast< CStringData* >( m_pMemMgr->Allocate( nTotalSize ) );
		if( pData == NULL )
		{
			return( NULL );
		}
		pData->pStringMgr = this;
		pData->nRefs = 1;
		pData->nAllocLength = nChars - 1;
		pData->nDataLength = 0;

		return( pData );
	}
	virtual void Free( CStringData* pData ) throw()
	{
		ATLASSERT( pData->pStringMgr == this );
		m_pMemMgr->Free( pData );
	}
	virtual CStringData* Reallocate( CStringData* pData, int nChars, int nCharSize ) throw()
	{
		CStringData* pNewData;
		ULONG nTotalSize;
		ULONG nDataBytes;

		ATLASSERT( pData->pStringMgr == this );
		nChars = AtlAlignUp( nChars+1, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.

		nDataBytes = nChars*nCharSize;
		nTotalSize = sizeof( CStringData )+nDataBytes;
		pNewData = static_cast< CStringData* >( m_pMemMgr->Reallocate( pData, nTotalSize ) );
		if( pNewData == NULL )
		{
			return NULL;
		}
		pNewData->nAllocLength = nChars - 1;

		return pNewData;
	}
	virtual CStringData* GetNilString() throw()
	{
		m_nil.AddRef();
		return &m_nil;
	}
	virtual IAtlStringMgr* Clone() throw()
	{
		return this;
	}

protected:
	IAtlMemMgr* m_pMemMgr;
	CNilStringData m_nil;
};

template< typename BaseType, const int t_nSize >
class CStaticString
{
public:
	CStaticString( const BaseType* psz ) :
		m_psz( psz )
	{
	}

	operator const BaseType*() const
	{
		return m_psz;
	}

	static int GetLength() 
	{
		return (t_nSize/sizeof( BaseType ))-1;
	}

private:
	const BaseType* m_psz;

private:
	CStaticString( const CStaticString& str ) throw();
	CStaticString& operator=( const CStaticString& str ) throw();
};

#define _ST( psz ) ATL::CStaticString< TCHAR, sizeof( _T( psz ) ) >( _T( psz ) )
#define _SA( psz ) ATL::CStaticString< char, sizeof( psz ) >( psz )
#define _SW( psz ) ATL::CStaticString< wchar_t, sizeof( L##psz ) >( L##psz )
#define _SO( psz ) _SW( psz )

template< typename BaseType = char >
class ChTraitsBase
{
public:
	typedef char XCHAR;
	typedef LPSTR PXSTR;
	typedef LPCSTR PCXSTR;
	typedef wchar_t YCHAR;
	typedef LPWSTR PYSTR;
	typedef LPCWSTR PCYSTR;
};

template<>
class ChTraitsBase< wchar_t >
{
public:
	typedef wchar_t XCHAR;
	typedef LPWSTR PXSTR;
	typedef LPCWSTR PCXSTR;
	typedef char YCHAR;
	typedef LPSTR PYSTR;
	typedef LPCSTR PCYSTR;
};

template< typename BaseType >
class CSimpleStringT
{
public:
	typedef ChTraitsBase< BaseType >::XCHAR XCHAR;
	typedef ChTraitsBase< BaseType >::PXSTR PXSTR;
	typedef ChTraitsBase< BaseType >::PCXSTR PCXSTR;
	typedef ChTraitsBase< BaseType >::YCHAR YCHAR;
	typedef ChTraitsBase< BaseType >::PYSTR PYSTR;
	typedef ChTraitsBase< BaseType >::PCYSTR PCYSTR;

public:
	explicit CSimpleStringT( IAtlStringMgr* pStringMgr ) throw()
	{
		ATLASSERT( pStringMgr != NULL );
		CStringData* pData = pStringMgr->GetNilString();
		Attach( pData );
	}
	CSimpleStringT( const CSimpleStringT& strSrc )
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pNewData = CloneData( pSrcData );

		Attach( pNewData );
	}
	CSimpleStringT( PCXSTR pszSrc, IAtlStringMgr* pStringMgr )
	{
		ATLASSERT( pStringMgr != NULL );

		int nLength = StringLength( pszSrc );
		CStringData* pData = pStringMgr->Allocate( nLength, sizeof( XCHAR ) );
		if( pData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, pszSrc, nLength );
	}
	CSimpleStringT( const XCHAR* pchSrc, int nLength, IAtlStringMgr* pStringMgr )
	{
		ATLASSERT( pStringMgr != NULL );

		CStringData* pData = pStringMgr->Allocate( nLength, sizeof( XCHAR ) );
		if( pData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, pchSrc, nLength );
	}
	~CSimpleStringT() throw()
	{
		CStringData* pData = GetData();
		pData->Release();
	}

	CSimpleStringT& operator=( const CSimpleStringT& strSrc )
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pOldData = GetData();
		if( pSrcData != pOldData )
		{
			if( pOldData->IsLocked() )
			{
				SetString( strSrc.GetString(), strSrc.GetLength() );
			}
			else
			{
				CStringData* pNewData = CloneData( pSrcData );
				pOldData->Release();
				Attach( pNewData );
			}
		}

		return( *this );
	}
	CSimpleStringT& operator=( PCXSTR pszSrc )
	{
		SetString( pszSrc );

		return( *this );
	}

	CSimpleStringT& operator+=( const CSimpleStringT& strSrc )
	{
		Append( strSrc );

		return( *this );
	}
	CSimpleStringT& operator+=( PCXSTR pszSrc )
	{
		Append( pszSrc );

		return( *this );
	}
	template< int t_nSize >
	CSimpleStringT& operator+=( const CStaticString< XCHAR, t_nSize >& strSrc )
	{
		Append( strSrc.m_psz, strSrc.GetLength() );

		return( *this );
	}
	CSimpleStringT& operator+=( char ch )
	{
		XCHAR chTemp = XCHAR( ch );
		Append( &chTemp, 1 );

		return( *this );
	}
	CSimpleStringT& operator+=( unsigned char ch )
	{
		XCHAR chTemp = XCHAR( ch );
		Append( &chTemp, 1 );
		
		return( *this );
	}
	CSimpleStringT& operator+=( wchar_t ch )
	{
		XCHAR chTemp = XCHAR( ch );
		Append( &chTemp, 1 );

		return( *this );
	}

	XCHAR operator[]( int iChar ) const throw()
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK
		return( m_pszData[iChar] );
	}

	operator PCXSTR() const throw()
	{
		return( m_pszData );
	}

	void Append( PCXSTR pszSrc )
	{
		Append( pszSrc, StringLength( pszSrc ) );
	}
	void Append( PCXSTR pszSrc, int nLength )
	{
		// See comment in SetString() about why we do this
		UINT_PTR nOffset = pszSrc-GetString();

		UINT nOldLength = GetLength();
		int nNewLength = nOldLength+nLength;
		PXSTR pszBuffer = GetBuffer( nNewLength );
		if( nOffset <= nOldLength )
		{
			pszSrc = pszBuffer+nOffset;
			// No need to call CopyCharsOverlapped, since the destination is
			// beyond the end of the original buffer
		}
		CopyChars( pszBuffer+nOldLength, pszSrc, nLength );
		ReleaseBuffer( nNewLength );
	}
	void Append( const CSimpleStringT& strSrc )
	{
		Append( strSrc.GetString(), strSrc.GetLength() );
	}
	void Empty() throw()
	{
		CStringData* pOldData = GetData();
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nDataLength == 0 )
		{
			return;
		}

		if( pOldData->IsLocked() )
		{
			// Don't reallocate a locked buffer that's shrinking
			SetLength( 0 );
		}
		else
		{
			pOldData->Release();
			CStringData* pNewData = pStringMgr->GetNilString();
			Attach( pNewData );
		}
	}
	void FreeExtra() throw()
	{
		CStringData* pOldData = GetData();
		int nLength = pOldData->nDataLength;
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nAllocLength == nLength )
		{
			return;
		}

		if( !pOldData->IsLocked() )  // Don't reallocate a locked buffer that's shrinking
		{
			CStringData* pNewData = pStringMgr->Allocate( nLength, sizeof( XCHAR ) );
			if( pNewData == NULL )
			{
				SetLength( nLength );
				return;
			}
			CopyChars( PXSTR( pNewData->data() ), PCXSTR( pOldData->data() ), nLength );
			pOldData->Release();
			Attach( pNewData );
			SetLength( nLength );
		}
	}

	int GetAllocLength() const throw()
	{
		return( GetData()->nAllocLength );
	}
	XCHAR GetAt( int iChar ) const throw()
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK
		return( m_pszData[iChar] );
	}
	PXSTR GetBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
		}

		return( m_pszData );
	}
	PXSTR GetBuffer( int nMinBufferLength )
	{
		return( PrepareWrite( nMinBufferLength ) );
	}
	PXSTR GetBufferSetLength( int nLength )
	{
		PXSTR pszBuffer = GetBuffer( nLength );
		SetLength( nLength );

		return( pszBuffer );
	}
	int GetLength() const throw()
	{
		return( GetData()->nDataLength );
	}
	IAtlStringMgr* GetManager() const throw()
	{
		return( GetData()->pStringMgr->Clone() );
	}

	PCXSTR GetString() const throw()
	{
		return( m_pszData );
	}
	bool IsEmpty() const throw()
	{
		return( GetLength() == 0 );
	}
	PXSTR LockBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
			pData = GetData();  // Do it again, because the fork might have changed it
		}
		pData->Lock();

		return( m_pszData );
	}
	void UnlockBuffer() throw()
	{
		CStringData* pData = GetData();
		pData->Unlock();
	}
	void Preallocate( int nLength )
	{
		PrepareWrite( nLength );
	}
	void ReleaseBuffer( int nNewLength = -1 ) throw()
	{
		if( nNewLength == -1 )
		{
			nNewLength = StringLength( m_pszData );
		}
		SetLength( nNewLength );
	}
	void Truncate( int nNewLength )
	{
		ATLASSERT( nNewLength <= GetLength() );
		GetBuffer( nNewLength );
		ReleaseBuffer( nNewLength );
	}
	void SetAt( int iChar, XCHAR ch )
	{
		ATLASSERT( (iChar >= 0) && (iChar < GetLength()) );
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer();
		pszBuffer[iChar] = ch;
		ReleaseBuffer( nLength );
	}
	void SetManager( IAtlStringMgr* pStringMgr )
	{
		ATLASSERT( IsEmpty() );

		CStringData* pData = GetData();
		pData->Release();
		pData = pStringMgr->GetNilString();
		Attach( pData );
	}
	void SetString( PCXSTR pszSrc )
	{
		SetString( pszSrc, StringLength( pszSrc ) );
	}
	void SetString( PCXSTR pszSrc, int nLength )
	{
		if( nLength == 0 )
		{
			Empty();
		}
		else
		{
			// It is possible that pszSrc points to a location inside of our 
			// buffer.  GetBuffer() might change m_pszData if (1) the buffer 
			// is shared or (2) the buffer is too small to hold the new 
			// string.  We detect this aliasing, and modify pszSrc to point
			// into the newly allocated buffer instead.

			UINT nOldLength = GetLength();
			UINT_PTR nOffset = pszSrc-GetString();
			// If 0 <= nOffset <= nOldLength, then pszSrc points into our 
			// buffer

			PXSTR pszBuffer = GetBuffer( nLength );
			if( nOffset <= nOldLength )
			{
				CopyCharsOverlapped( pszBuffer, pszBuffer+nOffset, nLength );
			}
			else
			{
				CopyChars( pszBuffer, pszSrc, nLength );
			}
			ReleaseBuffer( nLength );
		}
	}

public:
	friend CSimpleStringT operator+(
		const CSimpleStringT& str1,
		const CSimpleStringT& str2 )
	{
		CSimpleStringT s( str1.GetManager() );

		Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

		return( s );
	}

	friend CSimpleStringT operator+(
		const CSimpleStringT& str1,
		PCXSTR psz2 )
	{
		CSimpleStringT s( str1.GetManager() );

		Concatenate( s, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

		return( s );
	}

	friend CSimpleStringT operator+(
		PCXSTR psz1,
		const CSimpleStringT& str2 )
	{
		CSimpleStringT s( str2.GetManager() );

		Concatenate( s, psz1, StringLength( psz1 ), str2, str2.GetLength() );

		return( s );
	}

	static void CopyChars( XCHAR* pchDest, const XCHAR* pchSrc, int nChars ) throw()
	{
		memcpy( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
	}
	static void CopyCharsOverlapped( XCHAR* pchDest, const XCHAR* pchSrc, int nChars ) throw()
	{
		memmove( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
	}
#ifdef _ATL_MIN_CRT
	ATL_NOINLINE static int StringLength( PCXSTR psz ) throw()
	{
		int nLength = 0;
		if( psz != NULL )
		{
			const XCHAR* pch = psz;
			while( *pch != 0 )
			{
				nLength++;
				pch++;
			}
		}

		return( nLength );
	}
#else
	static int StringLength( const char* psz ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strlen( psz ) ) );
	}
	template<>
	static int StringLength( const wchar_t* psz ) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcslen( psz ) ) );
	}
#endif

protected:
	static void Concatenate( CSimpleStringT& strResult, PCXSTR psz1, int nLength1, PCXSTR psz2, int nLength2 )
	{
		int nNewLength = nLength1+nLength2;
		PXSTR pszBuffer = strResult.GetBuffer( nNewLength );
		CopyChars( pszBuffer, psz1, nLength1 );
		CopyChars( pszBuffer+nLength1, psz2, nLength2 );
		strResult.ReleaseBuffer( nNewLength );
	}
	ATL_NOINLINE static void ThrowMemoryException()
	{
		AtlThrow( E_OUTOFMEMORY );
	}

// Implementation
private:
	void Attach( CStringData* pData ) throw()
	{
		m_pszData = static_cast< PXSTR >( pData->data() );
	}
	ATL_NOINLINE void Fork( int nLength )
	{
		CStringData* pOldData = GetData();
		int nOldLength = pOldData->nDataLength;
		CStringData* pNewData = pOldData->pStringMgr->Clone()->Allocate( nLength, sizeof( XCHAR ) );
		if( pNewData == NULL )
		{
			ThrowMemoryException();
		}
		int nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;  // Copy '\0'
		CopyChars( PXSTR( pNewData->data() ), PCXSTR( pOldData->data() ), nCharsToCopy );
		pNewData->nDataLength = nOldLength;
		pOldData->Release();
		Attach( pNewData );
	}
	CStringData* GetData() const throw()
	{
		return( reinterpret_cast< CStringData* >( m_pszData )-1 );
	}
	PXSTR PrepareWrite( int nLength )
	{
		CStringData* pOldData = GetData();
		int nShared = 1-pOldData->nRefs;  // nShared < 0 means true, >= 0 means false
		int nTooShort = pOldData->nAllocLength-nLength;  // nTooShort < 0 means true, >= 0 means false
		if( (nShared|nTooShort) < 0 )  // If either sign bit is set (i.e. either is less than zero), we need to copy data
		{
			PrepareWrite2( nLength );
		}

		return( m_pszData );
	}
	ATL_NOINLINE void PrepareWrite2( int nLength )
	{
		CStringData* pOldData = GetData();
		if( pOldData->nDataLength > nLength )
		{
			nLength = pOldData->nDataLength;
		}
		if( pOldData->IsShared() )
		{
			Fork( nLength );
		}
		else if( pOldData->nAllocLength < nLength )
		{
			// Grow exponentially, until we hit 1K.
			int nNewLength = pOldData->nAllocLength;
			if( nNewLength > 1024 )
			{
				nNewLength += 1024;
			}
			else
			{
				nNewLength *= 2;
			}
			if( nNewLength < nLength )
			{
				nNewLength = nLength;
			}
			Reallocate( nNewLength );
		}
	}
	ATL_NOINLINE void Reallocate( int nLength )
	{
		CStringData* pOldData = GetData();
		ATLASSERT( pOldData->nAllocLength < nLength );
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		CStringData* pNewData = pStringMgr->Reallocate( pOldData, nLength, sizeof( XCHAR ) );
		if( pNewData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pNewData );
	}

	void SetLength( int nLength ) throw()
	{
		ATLASSERT( nLength >= 0 );
		ATLASSERT( nLength <= GetData()->nAllocLength );

		GetData()->nDataLength = nLength;
		m_pszData[nLength] = 0;
	}

	static CStringData* CloneData( CStringData* pData )
	{
		CStringData* pNewData = NULL;

		IAtlStringMgr* pNewStringMgr = pData->pStringMgr->Clone();
		if( !pData->IsLocked() && (pNewStringMgr == pData->pStringMgr) )
		{
			pNewData = pData;
			pNewData->AddRef();
		}
		else
		{
			pNewData = pNewStringMgr->Allocate( pData->nDataLength, sizeof( XCHAR ) );
			if( pNewData == NULL )
			{
				ThrowMemoryException();
			}
			pNewData->nDataLength = pData->nDataLength;
			CopyChars( PXSTR( pNewData->data() ), PCXSTR( pData->data() ), pData->nDataLength+1 );  // Copy '\0'
		}

		return( pNewData );
	}

private:
	PXSTR m_pszData;
};

template< typename TCharType >
class CStrBufT
{
public:
	typedef CSimpleStringT< TCharType > StringType;
	typedef StringType::XCHAR XCHAR;
	typedef StringType::PXSTR PXSTR;
	typedef StringType::PCXSTR PCXSTR;

	static const DWORD AUTO_LENGTH = 0x01;  // Automatically determine the new length of the string at release.  The string must be null-terminated.
	static const DWORD SET_LENGTH = 0x02;  // Set the length of the string object at GetBuffer time

public:
	explicit CStrBufT( StringType& str ) throw( ... ) :
		m_str( str ),
		m_pszBuffer( NULL ),
#ifdef _DEBUG
		m_nBufferLength( str.GetLength() ),
#endif
		m_nLength( str.GetLength() )
	{
		m_pszBuffer = m_str.GetBuffer();
	}
	CStrBufT( StringType& str, int nMinLength, DWORD dwFlags = AUTO_LENGTH ) throw( ... ) :
		m_str( str ),
		m_pszBuffer( NULL ),
#ifdef _DEBUG
		m_nBufferLength( nMinLength ),
#endif
		m_nLength( (dwFlags&AUTO_LENGTH) ? -1 : nMinLength )
	{
		if( dwFlags&SET_LENGTH )
		{
			m_pszBuffer = m_str.GetBufferSetLength( nMinLength );
		}
		else
		{
			m_pszBuffer = m_str.GetBuffer( nMinLength );
		}
	}
	~CStrBufT() throw()
	{
		m_str.ReleaseBuffer( m_nLength );
	}

	operator PXSTR() throw()
	{
		return( m_pszBuffer );
	}
	operator PCXSTR() const throw()
	{
		return( m_pszBuffer );
	}

	void SetLength( int nLength ) throw()
	{
		ATLASSERT( nLength <= m_nBufferLength );
		m_nLength = nLength;
	}

// Implementation
private:
	StringType& m_str;
	PXSTR m_pszBuffer;
	int m_nLength;
#ifdef _DEBUG
	int m_nBufferLength;
#endif

// Private copy constructor and copy assignment operator to prevent accidental use
private:
	CStrBufT( const CStrBufT& ) throw();
	CStrBufT& operator=( const CStrBufT& ) throw();
};

typedef CSimpleStringT< TCHAR > CSimpleString;
typedef CSimpleStringT< char > CSimpleStringA;
typedef CSimpleStringT< wchar_t > CSimpleStringW;
typedef CStrBufT< TCHAR > CStrBuf;
typedef CStrBufT< char > CStrBufA;
typedef CStrBufT< wchar_t > CStrBufW;

};  // namespace ATL

#endif  // __ATLSIMPSTR_H__
