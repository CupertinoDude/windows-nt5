/*++

   Copyright    (c)    1996    Microsoft Corporation

   Module  Name :
       hashtab.hxx

   Abstract:
       Declares a stand alone multiple use hash table.

   Author:

       Murali R. Krishnan    ( MuraliK )    02-Oct-1996

   Environment:
       Win32 - User Mode
   Project:

       Internet Server DLL

   Revision History:

--*/

# ifndef _HASHTAB_HXX_
# define _HASHTAB_HXX_

/************************************************************
 *     Include Headers
 ************************************************************/

# include <windows.h>

# if !defined(dllexp)
# define dllexp __declspec( dllexport)
# endif

/************************************************************
 *   Type Definitions
 ************************************************************/

/*++
  HT_ELEMENT
  o  base class for hashable elements.
  All the hash table elements should be derived from this base object
   and should define the abstract virtual functions.

  The data that the hash table elements should maintain include:
    the key for hashing (LPCSTR), key length,
    reference count -- usually a LONG value
    + any additional data specific to the object.
--*/
class HT_ELEMENT {

public:
    dllexp
    virtual ~HT_ELEMENT( VOID) {}

    dllexp
    virtual LPCSTR QueryKey(VOID) const = 0;

    dllexp
    virtual DWORD QueryKeyLen(VOID) const = 0;

    dllexp
    virtual LONG Reference( VOID) = 0;

    dllexp
    virtual LONG Dereference( VOID) = 0;

    dllexp
    virtual BOOL IsMatch( IN LPCSTR pszKey, IN DWORD cchKey) const = 0;

    dllexp
    virtual VOID Print( VOID) const = 0;
}; // class HT_ELEMENT()


// forward declaration
class HASH_TABLE_BUCKET;

/*++
  HT_ITERATOR - is used for iterating across the elements stored
  in the hash table. The structure contains state information to
  control the iteration. The user of the iterator should not
  modify any data member here.
--*/
struct HT_ITERATOR {
    DWORD nBucketNumber;
    DWORD_PTR nChunkId;  // pointer for the bucket chunk
    DWORD nPos;
};


/*++
  HASH_TABLE
  o  reusable hash table object - which contains state information
  for each instance of the table.

  Only LPCSTR keys are supported. Complex keys are not supported now.
  The object is templatable using C++, but not done for now.

--*/
class HASH_TABLE {

public:
    dllexp
    HASH_TABLE( IN DWORD   nBuckets,
                IN LPCSTR  pszIdentifier,
                IN DWORD   dwHashTableFlags
                );

    dllexp
    virtual ~HASH_TABLE(VOID) { Cleanup(); }

    dllexp virtual
    DWORD CalculateHash( IN LPCSTR pszKey, IN DWORD cchKey) const;

    dllexp virtual
    DWORD CalculateHash( IN LPCSTR pszKey) const
    { return ( CalculateHash( pszKey, strlen( pszKey))); }

    dllexp
    VOID Cleanup( VOID);

    dllexp BOOL IsValid( VOID) const { return ( NULL != m_prgBuckets); }

    dllexp HT_ELEMENT * Lookup( IN LPCSTR pszKey, IN DWORD cchKey);
    dllexp HT_ELEMENT * Lookup( IN LPCSTR pszKey)
    { return ( Lookup( pszKey, strlen(pszKey))); }

    dllexp BOOL   Insert( HT_ELEMENT * phte,
                          IN BOOL fCheckBeforeInsert = TRUE
                          );
    dllexp BOOL   Delete( HT_ELEMENT * phte);

    dllexp DWORD  FlushElements(VOID) { return ( ERROR_CALL_NOT_IMPLEMENTED);}

    dllexp DWORD  InitializeIterator( IN HT_ITERATOR * phti);
    dllexp DWORD  FindNextElement( IN HT_ITERATOR * phti,
                                   OUT HT_ELEMENT ** pphte);
    dllexp DWORD  CloseIterator( IN HT_ITERATOR * phti);


    dllexp VOID Print( IN DWORD level = 0);

private:
    DWORD   m_nBuckets;
    HASH_TABLE_BUCKET * m_prgBuckets;

    // Statistics about Hash table usage
    DWORD   m_nLookups;
    DWORD   m_nHits;

    DWORD   m_nInserts;
    DWORD   m_nEntries;
    DWORD   m_nFlushes;

    // Identification and Control Information
    DWORD   m_dwFlags;
    CHAR    m_rgchId[8];

};

typedef HASH_TABLE * PHASH_TABLE;


inline VOID
DerefAndKillElement( IN HT_ELEMENT * phte)
{
    // if we are the last user, kill the object.
    // if not the last user who does a deref has to kill the object
    if ( phte->Dereference() == 0) {

        // we are the last user. kill the object
        delete phte;
    }

} // DerefAndKillElement()


# endif // _HASHTAB_HXX_


/************************ End of File ***********************/
