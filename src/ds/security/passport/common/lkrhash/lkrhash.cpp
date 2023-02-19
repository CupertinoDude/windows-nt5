/*++

   Copyright    (c) 1998-2000    Microsoft Corporation

   Module  Name :
       LKRhash.cpp

   Abstract:
       Implements LKRhash: a fast, scalable, cache- and MP-friendly hash table

   Author:
       Paul (Per-Ake) Larson, palarson@microsoft.com, July 1997
       Murali R. Krishnan    (MuraliK)
       George V. Reilly      (GeorgeRe)     06-Jan-1998

   Environment:
       Win32 - User Mode

   Project:
       Internet Information Server RunTime Library

   Revision History:
       Jan 1998   - Massive cleanup and rewrite.  Templatized.
       10/01/1998 - Change name from LKhash to LKRhash

--*/

#include "precomp.hxx"


#define DLL_IMPLEMENTATION
#define IMPLEMENTATION_EXPORT
#include <lkrhash.h>

#ifdef __LKRHASH_NAMESPACE__
namespace LKRHash {
#endif // __LKRHASH_NAMESPACE__

#ifdef LKR_COUNTDOWN
const int NODE_BEGIN = CLKRHashTable::NODES_PER_CLUMP-1;
const int NODE_END   = -1;
const int NODE_STEP  = -1;
#define FOR_EACH_NODE(x)    for (x = NODES_PER_CLUMP;  --x >= 0;  )
#else // !LKR_COUNTDOWN
const int NODE_BEGIN = 0;
const int NODE_END   = CLKRHashTable::NODES_PER_CLUMP;
const int NODE_STEP  = +1;
#define FOR_EACH_NODE(x)    for (x = 0;  x < NODES_PER_CLUMP;  ++x)
#endif // !LKR_COUNTDOWN


#ifdef LKRHASH_ALLOCATOR_NEW

# define DECLARE_ALLOCATOR(CLASS)                        \
  CLKRhashAllocator* CLASS::sm_palloc = NULL;            \

# define DECLARE_ALLOCATOR_LHTSUBCLASS(CLASS)            \
  CLKRhashAllocator* CLKRLinearHashTable::CLASS::sm_palloc = NULL; \


  // DECLARE_ALLOCATOR(CLKRLinearHashTable);
  // DECLARE_ALLOCATOR(CLKRHashTable);
  DECLARE_ALLOCATOR_LHTSUBCLASS(CNodeClump);
  DECLARE_ALLOCATOR_LHTSUBCLASS(CSmallSegment);
  DECLARE_ALLOCATOR_LHTSUBCLASS(CMediumSegment);
  DECLARE_ALLOCATOR_LHTSUBCLASS(CLargeSegment);

#endif // LKRHASH_ALLOCATOR_NEW


static bool s_fInitialized = false;


// -------------------------------------------------------------------------
// Initialize per-class allocators
// -------------------------------------------------------------------------

bool
LKRHashTableInit()
{
    bool f = true;

    IRTLTRACE(_T("LKRHashTableInit\n"));

#define INIT_ALLOCATOR(CLASS, N)                                \
    LKRHASH_ALLOCATOR_INIT(CLASS, N, f);                         \

#define INIT_ALLOCATOR_LHTSUBCLASS(CLASS, N)                    \
    LKRHASH_ALLOCATOR_INIT(CLKRLinearHashTable::CLASS, N, f);     \


    // INIT_ALLOCATOR(CLKRLinearHashTable,        20);
    // INIT_ALLOCATOR(CLKRHashTable,               4);
    INIT_ALLOCATOR_LHTSUBCLASS(CNodeClump,    200);
    INIT_ALLOCATOR_LHTSUBCLASS(CSmallSegment,   5);
    INIT_ALLOCATOR_LHTSUBCLASS(CMediumSegment,  5);
    INIT_ALLOCATOR_LHTSUBCLASS(CLargeSegment,   5);

    s_fInitialized = f;

    return f;
} // LKRHashTableInit



// -------------------------------------------------------------------------
// Destroy per-class allocators
// -------------------------------------------------------------------------

void
LKRHashTableUninit()
{
#define UNINIT_ALLOCATOR(CLASS)                        \
    LKRHASH_ALLOCATOR_UNINIT(CLASS);                    \

#define UNINIT_ALLOCATOR_LHTSUBCLASS(CLASS)            \
    LKRHASH_ALLOCATOR_UNINIT(CLKRLinearHashTable::CLASS);\

    // UNINIT_ALLOCATOR(CLKRLinearHashTable);
    // UNINIT_ALLOCATOR(CLKRHashTable);
    UNINIT_ALLOCATOR_LHTSUBCLASS(CNodeClump);
    UNINIT_ALLOCATOR_LHTSUBCLASS(CSmallSegment);
    UNINIT_ALLOCATOR_LHTSUBCLASS(CMediumSegment);
    UNINIT_ALLOCATOR_LHTSUBCLASS(CLargeSegment);

    s_fInitialized = false;

    IRTLTRACE(_T("LKRHashTableUninit done\n"));
} // LKRHashTableUninit



// -------------------------------------------------------------------------
// class static member variables
// -------------------------------------------------------------------------

#ifdef LOCK_INSTRUMENTATION
LONG CLKRLinearHashTable::CBucket::sm_cBuckets    = 0;

LONG CLKRLinearHashTable::sm_cTables              = 0;
#endif // LOCK_INSTRUMENTATION


#ifndef LKR_NO_GLOBAL_LIST
CLockedDoubleList CLKRLinearHashTable::sm_llGlobalList;
CLockedDoubleList CLKRHashTable::sm_llGlobalList;
#endif // LKR_NO_GLOBAL_LIST



// CLKRLinearHashTable --------------------------------------------------------
// Public Constructor for class CLKRLinearHashTable.
// -------------------------------------------------------------------------

CLKRLinearHashTable::CLKRLinearHashTable(
    LPCSTR          pszName,        // An identifier for debugging
    PFnExtractKey   pfnExtractKey,  // Extract key from record
    PFnCalcKeyHash  pfnCalcKeyHash, // Calculate hash signature of key
    PFnEqualKeys    pfnEqualKeys,   // Compare two keys
    PFnAddRefRecord pfnAddRefRecord,// AddRef in FindKey, etc
    double          maxload,        // Upperbound on the average chain length
    DWORD           initsize,       // Initial size of hash table.
    DWORD         /*num_subtbls*/   // for compatiblity with CLKRHashTable
    )
    :
#ifdef LOCK_INSTRUMENTATION
      m_Lock(_LockName()),
#endif // LOCK_INSTRUMENTATION
      m_nTableLockType(TableLock::LockType()),
      m_nBucketLockType(BucketLock::LockType()),
      m_phtParent(NULL)
{
    STATIC_ASSERT(1 <= LK_DFLT_MAXLOAD  && LK_DFLT_MAXLOAD <= NODES_PER_CLUMP);
    STATIC_ASSERT(0 <= NODE_BEGIN  &&  NODE_BEGIN < NODES_PER_CLUMP);
    STATIC_ASSERT(!(0 <= NODE_END  &&  NODE_END < NODES_PER_CLUMP));

    IRTLVERIFY(LK_SUCCESS
               == _Initialize(pfnExtractKey, pfnCalcKeyHash, pfnEqualKeys,
                              pfnAddRefRecord, pszName, maxload, initsize));

    _InsertThisIntoGlobalList();
} // CLKRLinearHashTable::CLKRLinearHashTable



// CLKRLinearHashTable --------------------------------------------------------
// Private Constructor for class CLKRLinearHashTable, used by CLKRHashTable.
// -------------------------------------------------------------------------

CLKRLinearHashTable::CLKRLinearHashTable(
    LPCSTR          pszName,        // An identifier for debugging
    PFnExtractKey   pfnExtractKey,  // Extract key from record
    PFnCalcKeyHash  pfnCalcKeyHash, // Calculate hash signature of key
    PFnEqualKeys    pfnEqualKeys,   // Compare two keys
    PFnAddRefRecord pfnAddRefRecord,// AddRef in FindKey, etc
    double          maxload,        // Upperbound on the average chain length
    DWORD           initsize,       // Initial size of hash table.
    CLKRHashTable*  phtParent       // Owning table.
    )
    :
#ifdef LOCK_INSTRUMENTATION
      m_Lock(_LockName()),
#endif // LOCK_INSTRUMENTATION
      m_nTableLockType(TableLock::LockType()),
      m_nBucketLockType(BucketLock::LockType()),
      m_phtParent(phtParent)
{
    IRTLASSERT(m_phtParent != NULL);
    IRTLVERIFY(LK_SUCCESS
               == _Initialize(pfnExtractKey, pfnCalcKeyHash, pfnEqualKeys,
                              pfnAddRefRecord, pszName, maxload, initsize));

    _InsertThisIntoGlobalList();
} // CLKRLinearHashTable::CLKRLinearHashTable



// _Initialize -------------------------------------------------------------
// Do all the real work of constructing a CLKRLinearHashTable
// -------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_Initialize(
    PFnExtractKey   pfnExtractKey,
    PFnCalcKeyHash  pfnCalcKeyHash,
    PFnEqualKeys    pfnEqualKeys,
    PFnAddRefRecord pfnAddRefRecord,
    LPCSTR          pszName,
    double          maxload,
    DWORD           initsize)
{
    m_dwSignature =     SIGNATURE;
    m_dwBktAddrMask0 =  0;
    m_dwBktAddrMask1 =  0;
    m_iExpansionIdx =   0;
    m_paDirSegs =       NULL;
    m_lkts =            LK_MEDIUM_TABLESIZE;
    m_dwSegBits =       0;
    m_dwSegSize =       0;
    m_dwSegMask =       0;
    m_lkrcState =       LK_UNUSABLE;
    m_MaxLoad =         LK_DFLT_MAXLOAD;
    m_nLevel =          0;
    m_cDirSegs =        0;
    m_cRecords =        0;
    m_cActiveBuckets =  0;
    m_wBucketLockSpins= LOCK_USE_DEFAULT_SPINS;
    m_pfnExtractKey =   pfnExtractKey;
    m_pfnCalcKeyHash =  pfnCalcKeyHash;
    m_pfnEqualKeys =    pfnEqualKeys;
    m_pfnAddRefRecord = pfnAddRefRecord;

    strncpy(m_szName, pszName, NAME_SIZE-1);
    m_szName[NAME_SIZE-1] = '\0';

    IRTLASSERT(m_pfnExtractKey != NULL
               && m_pfnCalcKeyHash != NULL
               && m_pfnEqualKeys != NULL
               && m_pfnAddRefRecord != NULL);

    IRTLASSERT(s_fInitialized);

    if (!s_fInitialized)
        return (m_lkrcState = LK_NOT_INITIALIZED);

    if (m_pfnExtractKey == NULL
            || m_pfnCalcKeyHash == NULL
            || m_pfnEqualKeys == NULL
            || m_pfnAddRefRecord == NULL)
        return (m_lkrcState = LK_BAD_PARAMETERS);

    // Choose the size of the segments according to the desired "size" of
    // the table, small, medium, or large.
    if (initsize == LK_SMALL_TABLESIZE)
    {
        _SetSegVars(LK_SMALL_TABLESIZE);
        initsize = CSmallSegment::INITSIZE;
    }
    else if (initsize == LK_MEDIUM_TABLESIZE)
    {
        _SetSegVars(LK_MEDIUM_TABLESIZE);
        initsize = CMediumSegment::INITSIZE;
    }
    else if (initsize == LK_LARGE_TABLESIZE)
    {
        _SetSegVars(LK_LARGE_TABLESIZE);
        initsize = CLargeSegment::INITSIZE;
    }

    // specified an explicit initial size
    else
    {
        // force Small::INITSIZE <= initsize <= MAX_DIRSIZE * Large::INITSIZE
        initsize = min(max(initsize, CSmallSegment::INITSIZE),
                       (MAX_DIRSIZE >> CLargeSegment::SEGBITS)
                            * CLargeSegment::INITSIZE);

        // Guess a table size
        if (initsize <= 8 * CSmallSegment::INITSIZE)
            _SetSegVars(LK_SMALL_TABLESIZE);
        else if (initsize >= CLargeSegment::INITSIZE)
            _SetSegVars(LK_LARGE_TABLESIZE);
        else
            _SetSegVars(LK_MEDIUM_TABLESIZE);
    }

    m_cActiveBuckets = initsize;

    // TODO: better sanity check for ridiculous values?
    m_MaxLoad = (maxload <= 1.0)  ?  LK_DFLT_MAXLOAD  :  maxload;
    m_MaxLoad = min(m_MaxLoad, 10 * NODES_PER_CLUMP);

    // adjust m_dwBktAddrMask0 (== m_dwSegMask) to make it large
    // enough to distribute the buckets across the address space
    for (DWORD tmp = m_cActiveBuckets >> m_dwSegBits;  tmp > 1;  tmp >>= 1)
    {
        ++m_nLevel;
        m_dwBktAddrMask0 = (m_dwBktAddrMask0 << 1) | 1;
    }

    m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;

    IRTLASSERT(_H1(m_cActiveBuckets) == m_cActiveBuckets);
    m_iExpansionIdx = m_cActiveBuckets & m_dwBktAddrMask0;

    // create and clear directory of segments
    DWORD dirsize = MIN_DIRSIZE;
    while (dirsize < (m_cActiveBuckets >> m_dwSegBits))
        dirsize <<= 1;

    dirsize = min(dirsize, MAX_DIRSIZE);
    IRTLASSERT(dirsize * m_dwSegSize >= m_cActiveBuckets);

    m_lkrcState = LK_ALLOC_FAIL;
    m_paDirSegs = _AllocateSegmentDirectory(dirsize);

    if (m_paDirSegs != NULL)
    {
        m_cDirSegs = dirsize;
        IRTLASSERT(m_cDirSegs >= MIN_DIRSIZE
                   &&  (m_cDirSegs & (m_cDirSegs-1)) == 0);  // == (1 << N)

        // create and initialize only the required segments
        DWORD dwMaxSegs = (m_cActiveBuckets + m_dwSegSize - 1) >> m_dwSegBits;

        IRTLTRACE(_T("m_lkts = %d, m_cActiveBuckets = %lu, ")
                  _T("m_dwSegSize = %lu, bits = %lu\n")
                  _T("m_cDirSegs = %lu, dwMaxSegs = %lu, ")
                  _T("segment total size = %lu bytes\n"),
                  m_lkts, m_cActiveBuckets,
                  m_dwSegSize, m_dwSegBits,
                  m_cDirSegs, dwMaxSegs,
                  m_dwSegSize * sizeof(CBucket));

        m_lkrcState = LK_SUCCESS; // so IsValid/IsUsable won't fail

        for (DWORD i = 0;  i < dwMaxSegs;  i++)
        {
            CSegment* pSeg = _AllocateSegment();
            if (pSeg != NULL)
                m_paDirSegs[i].m_pseg = pSeg;
            else
            {
                // problem: deallocate everything
                m_lkrcState = LK_ALLOC_FAIL;
                for (DWORD j = i;  j-- > 0;  )
                {
                    _FreeSegment(m_paDirSegs[j].m_pseg);
                    m_paDirSegs[j].m_pseg = NULL;
                }
                _FreeSegmentDirectory(m_paDirSegs);
                break;
            }
        }
    }

    if (m_lkrcState != LK_SUCCESS)
    {
        m_paDirSegs      = NULL;
        m_cDirSegs       = 0;
        m_cActiveBuckets = 0;
        m_iExpansionIdx  = 0;
    }

    return m_lkrcState;
} // CLKRLinearHashTable::_Initialize



// CLKRHashTable ----------------------------------------------------------
// Constructor for class CLKRHashTable.
// ---------------------------------------------------------------------

CLKRHashTable::CLKRHashTable(
    LPCSTR          pszName,        // An identifier for debugging
    PFnExtractKey   pfnExtractKey,  // Extract key from record
    PFnCalcKeyHash  pfnCalcKeyHash, // Calculate hash signature of key
    PFnEqualKeys    pfnEqualKeys,   // Compare two keys
    PFnAddRefRecord pfnAddRefRecord,// AddRef in FindKey, etc
    double          maxload,        // Bound on the average chain length
    DWORD           initsize,       // Initial size of hash table.
    DWORD           num_subtbls     // Number of subordinate hash tables.
    )
    : m_dwSignature(SIGNATURE),
      m_cSubTables(0),
      m_palhtDir(NULL),
      m_pfnExtractKey(pfnExtractKey),
      m_pfnCalcKeyHash(pfnCalcKeyHash),
      m_lkrcState(LK_BAD_PARAMETERS)
{
    strncpy(m_szName, pszName, NAME_SIZE-1);
    m_szName[NAME_SIZE-1] = '\0';

    _InsertThisIntoGlobalList();

    IRTLASSERT(pfnExtractKey != NULL
               && pfnCalcKeyHash != NULL
               && pfnEqualKeys != NULL
               && pfnAddRefRecord != NULL);

    if (pfnExtractKey == NULL
            || pfnCalcKeyHash == NULL
            || pfnEqualKeys == NULL
            || pfnAddRefRecord == NULL)
        return;

    if (!s_fInitialized)
    {
        m_lkrcState = LK_NOT_INITIALIZED;
        return;
    }

    LK_TABLESIZE lkts = NumSubTables(initsize, num_subtbls);

#ifdef _DEBUG
    int cBuckets = initsize;
    if (initsize == LK_SMALL_TABLESIZE)
        cBuckets = SubTable::CSmallSegment::INITSIZE;
    else if (initsize == LK_MEDIUM_TABLESIZE)
        cBuckets = SubTable::CMediumSegment::INITSIZE;
    else if (initsize == LK_LARGE_TABLESIZE)
        cBuckets = SubTable::CLargeSegment::INITSIZE;

    IRTLTRACE(_T("CLKRHashTable: %s, %d subtables, initsize = %d, ")
              _T("total #buckets = %d\n" ),
              ((lkts == LK_SMALL_TABLESIZE) ? "small" : 
               (lkts == LK_MEDIUM_TABLESIZE) ? "medium" : "large"),
              num_subtbls, initsize, cBuckets * num_subtbls);
#endif

    m_lkrcState = LK_ALLOC_FAIL;
    m_palhtDir  = _AllocateSubTableArray(num_subtbls);

    if (m_palhtDir == NULL)
        return;
    else
    {
        m_cSubTables = num_subtbls;
        for (DWORD i = 0;  i < m_cSubTables;  i++)
            m_palhtDir[i] = NULL;
    }

    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        m_palhtDir[i] = _AllocateSubTable(pszName, pfnExtractKey,
                                          pfnCalcKeyHash, pfnEqualKeys,
                                          pfnAddRefRecord, maxload,
                                          initsize, this);

        // Failed to allocate a subtable.  Destroy everything allocated so far.
        if (m_palhtDir[i] == NULL  ||  !m_palhtDir[i]->IsValid())
        {
            for (DWORD j = i;  j-- > 0;  )
                _FreeSubTable(m_palhtDir[j]);
            _FreeSubTableArray(m_palhtDir);
            m_cSubTables = 0;
            m_palhtDir   = NULL;

            return;
        }
    }

#ifdef LKR_SUBTABLE_MASK
    m_nSubTableMask = m_cSubTables - 1;
    // power of 2?
    if ((m_nSubTableMask & m_cSubTables) != 0)
        m_nSubTableMask = -1;
#endif // LKR_SUBTABLE_MASK

    m_lkrcState = LK_SUCCESS; // so IsValid/IsUsable won't fail
} // CLKRHashTable::CLKRHashTable



// ~CLKRLinearHashTable ------------------------------------------------------
// Destructor for class CLKRLinearHashTable
//-------------------------------------------------------------------------

CLKRLinearHashTable::~CLKRLinearHashTable()
{
    // must acquire all locks before deleting to make sure
    // that no other threads are using the table
    WriteLock();
    _Clear(false);
    WriteUnlock();

    _RemoveThisFromGlobalList();

    m_dwSignature = SIGNATURE_FREE;
    m_lkrcState = LK_UNUSABLE; // so IsUsable will fail
} // CLKRLinearHashTable::~CLKRLinearHashTable



// ~CLKRHashTable ------------------------------------------------------------
// Destructor for class CLKRHashTable
//-------------------------------------------------------------------------
CLKRHashTable::~CLKRHashTable()
{
    // delete in reverse order, just like delete[].
    for (DWORD i = m_cSubTables;  i-- > 0;  )
        _FreeSubTable(m_palhtDir[i]);

    _FreeSubTableArray(m_palhtDir);

    _RemoveThisFromGlobalList();

    m_dwSignature = SIGNATURE_FREE;
    m_lkrcState = LK_UNUSABLE; // so IsUsable will fail
} // CLKRHashTable::~CLKRHashTable



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::NumSubTables
// Synopsis: 
//------------------------------------------------------------------------

LK_TABLESIZE
CLKRLinearHashTable::NumSubTables(
    DWORD& rinitsize,
    DWORD& rnum_subtbls)
{
    LK_TABLESIZE lkts = LK_MEDIUM_TABLESIZE;

    return lkts;
} // CLKRLinearHashTable::NumSubTables



//------------------------------------------------------------------------
// Function: CLKRHashTable::NumSubTables
// Synopsis: 
//------------------------------------------------------------------------

LK_TABLESIZE
CLKRHashTable::NumSubTables(
    DWORD& rinitsize,
    DWORD& rnum_subtbls)
{
    LK_TABLESIZE lkts;
    
    // Establish the table size
    if (rinitsize == LK_SMALL_TABLESIZE
        ||  rinitsize == LK_MEDIUM_TABLESIZE
        ||  rinitsize == LK_LARGE_TABLESIZE)
    {
        lkts = static_cast<LK_TABLESIZE>(rinitsize);
    }
    else
    {
        if (rnum_subtbls != LK_DFLT_NUM_SUBTBLS)
        {
            rinitsize = (rinitsize - 1) / rnum_subtbls + 1;

            if (rinitsize <= SubTable::CSmallSegment::SEGSIZE)
                lkts = LK_SMALL_TABLESIZE;
            else if (rinitsize >= SubTable::CLargeSegment::SEGSIZE)
                lkts = LK_LARGE_TABLESIZE;
            else
                lkts = LK_MEDIUM_TABLESIZE;
        }
        else
        {
            lkts = LK_MEDIUM_TABLESIZE;
        }
    }

    // Choose a suitable number of subtables
    if (rnum_subtbls == LK_DFLT_NUM_SUBTBLS)
    {
        int nCPUs = NumProcessors();
        switch (lkts)
        {
        case LK_SMALL_TABLESIZE:
            rnum_subtbls = min(1, nCPUs);
            break;
        
        case LK_MEDIUM_TABLESIZE:
            rnum_subtbls = 2 * nCPUs;
            break;
        
        case LK_LARGE_TABLESIZE:
            rnum_subtbls = 4 * nCPUs;
            break;
        }
    }

    rnum_subtbls = min(64, rnum_subtbls);

    return lkts;
} // CLKRHashTable::NumSubTables



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FindBucket
// Synopsis: Find a bucket, given its signature. The bucket is locked
//           before returning. Assumes table is already locked, to avoid
//           race conditions.
//------------------------------------------------------------------------

LOCK_FORCEINLINE
CLKRLinearHashTable::CBucket*
CLKRLinearHashTable::_FindBucket(
    DWORD dwSignature,
    bool  fLockForWrite) const
{
    IRTLASSERT(IsValid());
    IRTLASSERT(m_dwBktAddrMask0 > 0);
    IRTLASSERT((m_dwBktAddrMask0 & (m_dwBktAddrMask0+1)) == 0); // 00011..111
    IRTLASSERT(m_dwBktAddrMask0 == (1U << m_nLevel) - 1);
    IRTLASSERT(m_dwBktAddrMask1 == ((m_dwBktAddrMask0 << 1) | 1));
    IRTLASSERT((m_dwBktAddrMask1 & (m_dwBktAddrMask1+1)) == 0);
    IRTLASSERT(0 <= m_iExpansionIdx && m_iExpansionIdx <= m_dwBktAddrMask0);
    IRTLASSERT(0 < m_dwSegBits  &&  m_dwSegBits < 20
               &&  m_dwSegSize == (1U << m_dwSegBits)
               &&  m_dwSegMask == (m_dwSegSize - 1));
    IRTLASSERT(IsReadLocked()  ||  IsWriteLocked());
    
    const DWORD dwBktAddr = _BucketAddress(dwSignature);
    IRTLASSERT(dwBktAddr < m_cActiveBuckets);
    
    CBucket* const pbkt = _Bucket(dwBktAddr);
    IRTLASSERT(pbkt != NULL);
    
    if (fLockForWrite)
        pbkt->WriteLock();
    else
        pbkt->ReadLock();
    
    return pbkt;
} // CLKRLinearHashTable::_FindBucket



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_IsNodeCompact
// Synopsis: validates that a node is correctly compacted
//------------------------------------------------------------------------

int
CLKRLinearHashTable::_IsNodeCompact(
    CBucket* const pbkt) const
{
    CNodeClump* pncCurr;
    CNodeClump* pncPrev;
    bool fEmpty  = pbkt->m_ncFirst.InvalidSignature(NODE_BEGIN);
    int  cErrors = fEmpty ? !pbkt->m_ncFirst.IsLastClump() : 0;

    for (pncCurr = &pbkt->m_ncFirst, pncPrev = NULL;
         pncCurr != NULL;
         pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (fEmpty)
            {
                cErrors += (!pncCurr->InvalidSignature(i));
                cErrors += (!pncCurr->IsEmptyNode(i));
            }
            else if (pncCurr->InvalidSignature(i))
            {
                fEmpty = true;
                cErrors += (!pncCurr->IsEmptyNode(i));
                cErrors += (!pncCurr->IsLastClump());
            }
            else // still in non-empty portion
            {
                cErrors += (pncCurr->InvalidSignature(i));
                cErrors += (pncCurr->IsEmptyNode(i));
            }
        }
    }

    return cErrors;
} // CLKRLinearHashTable::_IsNodeCompact



LOCK_FORCEINLINE
CLKRHashTable::SubTable*
CLKRHashTable::_SubTable(
    DWORD dwSignature) const
{
    IRTLASSERT(m_lkrcState == LK_SUCCESS
               &&  m_palhtDir != NULL  &&  m_cSubTables > 0);
    
    const DWORD PRIME = 1048583UL;  // used to scramble the hash sig
    DWORD       index = dwSignature;
    
#ifdef LKR_BITSCRAMBLE_SUBTABLE_INDEX
    index = (((index * PRIME + 12345) >> 16)
             | ((index * 69069 + 1) & 0xffff0000));
#else // !LKR_BITSCRAMBLE_SUBTABLE_INDEX
    index %= PRIME;
#endif // !LKR_BITSCRAMBLE_SUBTABLE_INDEX
    
#ifdef LKR_SUBTABLE_BITMASK
# ifdef LKR_SUBTABLE_MASK
    if (m_nSubTableMask >= 0)
        index &= m_nSubTableMask;
# else // !LKR_SUBTABLE_MASK
    if ((m_cSubTables & (m_cSubTables-1)) == 0)
        index &= (m_cSubTables-1);
# endif // !LKR_SUBTABLE_MASK
    else
#endif // LKR_SUBTABLE_BITMASK
        index %= m_cSubTables;

    return m_palhtDir[index];
} // CLKRHashTable::_SubTable



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_InsertRecord
// Synopsis: Inserts a new record into the hash table. If this causes the
//           average chain length to exceed the upper bound, the table is
//           expanded by one bucket.
// Output:   LK_SUCCESS,    if the record was inserted.
//           LK_KEY_EXISTS, if the record was not inserted (because a record
//               with the same key value already exists in the table, unless
//               fOverwrite==true).
//           LK_ALLOC_FAIL, if failed to allocate the required space
//           LK_UNUSABLE,   if hash table not in usable state
//           LK_BAD_RECORD, if record is bad.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_InsertRecord(
    const void* pvRecord,   // Pointer to the record to add to table
    DWORD       dwSignature,// hash signature
    bool        fOverwrite  // overwrite record if key already present
    )
{
    IRTLASSERT(IsUsable()
               &&  pvRecord != NULL
               &&  dwSignature != HASH_INVALID_SIGNATURE);

    // find the beginning of the correct bucket chain
    WriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    CBucket* const pbkt = _FindBucket(dwSignature, true);
    IRTLASSERT(pbkt != NULL);
    IRTLASSERT(pbkt->IsWriteLocked());
    WriteUnlock();

    // check that no record with the same key value exists
    // and save a pointer to the last element on the chain
    LK_RETCODE lkrc = LK_SUCCESS;
    CNodeClump* pncFree = NULL;
    int         iFreePos = NODE_BEGIN - NODE_STEP;
    CNodeClump* pncPrev;
    CNodeClump* pncCurr;
    bool        fUpdate = false;
    const DWORD_PTR pnKey = _ExtractKey(pvRecord);

    // walk down the entire bucket chain, looking for matching hash
    // signatures and keys
    for (pncCurr = &pbkt->m_ncFirst, pncPrev = NULL;
         pncCurr != NULL;
         pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pncCurr->IsEmptySlot(i))
            {
                IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                IRTLASSERT(0 == _IsNodeCompact(pbkt));
                IRTLASSERT(pncCurr->IsLastClump());

                pncFree  = pncCurr;
                iFreePos = i;
                goto insert;
            }

            if (dwSignature == pncCurr->m_dwKeySigs[i]
                &&  (
#ifdef LKR_EQUALKEYS
                    pvRecord == pncCurr->m_pvNode[i]  ||
#endif// LKR_EQUALKEYS
                    _EqualKeys(pnKey,  _ExtractKey(pncCurr->m_pvNode[i]))))
            {
                if (fOverwrite)
                {
                    // If we allow overwrites, this is the slot to do it to
                    fUpdate  = true;
                    pncFree  = pncCurr;
                    iFreePos = i;
                    goto insert;
                }
                else
                {
                    // overwrites forbidden: return an error
                    lkrc = LK_KEY_EXISTS;
                    goto exit;
                }
            }
        }
    }

  insert:
    if (pncFree != NULL)
    {
        pncCurr = pncFree;
        IRTLASSERT(0 <= iFreePos  &&  iFreePos < NODES_PER_CLUMP);
    }
    else
    {
        // No free slots.  Attach the new node to the end of the chain
        IRTLASSERT(iFreePos == NODE_BEGIN - NODE_STEP);
        pncCurr = _AllocateNodeClump();

        if (pncCurr == NULL)
        {
            lkrc = LK_ALLOC_FAIL;
            goto exit;
        }

        IRTLASSERT(pncPrev != NULL  &&  pncPrev->IsLastClump());
        pncPrev->m_pncNext = pncCurr;
        iFreePos = NODE_BEGIN;
    }

    // Bump the new record's reference count upwards
    _AddRefRecord(pvRecord, +1);

    if (fUpdate)
    {
        // We're overwriting an existing record.  Adjust the old record's
        // refcount downwards.  (Doing ++new, --old in this order ensures
        // that the refcount won't briefly go to zero if new and old are
        // the same record.)
        IRTLASSERT(!pncCurr->IsEmptyAndInvalid(iFreePos));
        _AddRefRecord(pncCurr->m_pvNode[iFreePos], -1);
    }
    else
    {
        IRTLASSERT(pncCurr->IsEmptyAndInvalid(iFreePos));
        InterlockedIncrement(reinterpret_cast<LONG*>(&m_cRecords));
    }

    pncCurr->m_dwKeySigs[iFreePos] = dwSignature;
    pncCurr->m_pvNode[iFreePos]    = pvRecord;

  exit:
    pbkt->WriteUnlock();

    if (lkrc == LK_SUCCESS)
    {
        // If the average load factor has grown too high, we grow the
        // table one bucket at a time.
        while (m_cRecords > m_MaxLoad * m_cActiveBuckets)
        {
            // If _Expand returns an error code (viz. LK_ALLOC_FAIL), it
            // just means that there isn't enough spare memory to expand
            // the table by one bucket. This is likely to cause problems
            // elsewhere soon, but this hashtable has not been corrupted.
            // If the call to _AllocateNodeClump above failed, then we do
            // have a real error that must be propagated back to the caller
            // because we were unable to insert the element at all.
            if (_Expand() != LK_SUCCESS)
                break;  // expansion failed
        }
    }

    return lkrc;
} // CLKRLinearHashTable::_InsertRecord



// Thin wrapper for the corresponding method in CLKRLinearHashTable

LK_RETCODE
CLKRHashTable::InsertRecord(
    const void* pvRecord,
    bool fOverwrite /*=false*/)
{
    if (!IsUsable())
        return m_lkrcState;
    
    if (pvRecord == NULL)
        return LK_BAD_RECORD;
    
    LKRHASH_GLOBAL_WRITE_LOCK();    // usu. no-op
    DWORD     hash_val  = _CalcKeyHash(_ExtractKey(pvRecord));
    SubTable* const pst = _SubTable(hash_val);
    LK_RETCODE lk = pst->_InsertRecord(pvRecord, hash_val, fOverwrite);
    LKRHASH_GLOBAL_WRITE_UNLOCK();    // usu. no-op
    return lk;
} // CLKRLinearHashTable::InsertRecord



//-------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_DeleteKey
// Synopsis: Deletes the record with the given key value from the hash
//           table (if it exists).
// Returns:  LK_SUCCESS, if record found and deleted.
//           LK_NO_SUCH_KEY, if no record with the given key value was found.
//           LK_UNUSABLE, if hash table not in usable state
//-------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_DeleteKey(
    const DWORD_PTR pnKey,      // Key value of the record, depends on key type
    DWORD           dwSignature
    )
{
    IRTLASSERT(IsUsable());

    LK_RETCODE lkrc = LK_NO_SUCH_KEY;

    // locate the beginning of the correct bucket chain
    WriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    CBucket* const pbkt = _FindBucket(dwSignature, true);
    IRTLASSERT(pbkt != NULL);
    IRTLASSERT(pbkt->IsWriteLocked());
    WriteUnlock();

    // scan down the bucket chain, looking for the victim
    for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
         pncCurr != NULL;
         pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pncCurr->IsEmptySlot(i))
            {
                IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                IRTLASSERT(0 == _IsNodeCompact(pbkt));
                IRTLASSERT(pncCurr->IsLastClump());
                goto exit;
            }

            if (dwSignature != pncCurr->m_dwKeySigs[i])
                continue;

            const DWORD_PTR pnKey2 = _ExtractKey(pncCurr->m_pvNode[i]);

            if (
#ifdef LKR_EQUALKEYS
                pnKey == pnKey2  ||
#endif// LKR_EQUALKEYS
                _EqualKeys(pnKey,  pnKey2))
            {
                IRTLVERIFY(_DeleteNode(pbkt, pncCurr, pncPrev, i));

                lkrc = LK_SUCCESS;
                goto exit;
            }
        }
    }

  exit:
    pbkt->WriteUnlock();

    if (lkrc == LK_SUCCESS)
    {
        // contract the table if necessary
        double maxcontract = 1.0 / static_cast<double>(m_MaxLoad);

        for (int contractions = 0;
             m_cRecords < m_MaxLoad * m_cActiveBuckets
                 &&  m_cActiveBuckets > m_dwSegSize * MIN_DIRSIZE
                 &&  contractions < maxcontract;
             ++contractions)
        {
            // If _Contract returns an error code (viz. LK_ALLOC_FAIL), it
            // just means that there isn't enough spare memory to contract
            // the table by one bucket. This is likely to cause problems
            // elsewhere soon, but this hashtable has not been corrupted.
            if (_Contract() != LK_SUCCESS)
                break;
        }
    }

    return lkrc;
} // CLKRLinearHashTable::_DeleteKey



// Thin wrapper for the corresponding method in CLKRLinearHashTable

LK_RETCODE
CLKRHashTable::DeleteKey(
    const DWORD_PTR pnKey)
{
    if (!IsUsable())
        return m_lkrcState;
    
    LKRHASH_GLOBAL_WRITE_LOCK();    // usu. no-op
    DWORD     hash_val  = _CalcKeyHash(pnKey);
    SubTable* const pst = _SubTable(hash_val);
    LK_RETCODE lk       = pst->_DeleteKey(pnKey, hash_val);
    LKRHASH_GLOBAL_WRITE_UNLOCK();    // usu. no-op
    return lk;
} // CLKRLinearHashTable::DeleteKey



//-------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_DeleteRecord
// Synopsis: Deletes the specified record from the hash table (if it
//           exists).  This is not the same thing as calling
//           DeleteKey(_ExtractKey(pvRecord)).  If _DeleteKey were called for
//           a record that doesn't exist in the table, it could delete some
//           completely unrelated record that happened to have the same key.
// Returns:  LK_SUCCESS, if record found and deleted.
//           LK_NO_SUCH_KEY, if the record is not found in the table.
//           LK_UNUSABLE, if hash table not in usable state.
//-------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_DeleteRecord(
    const void* pvRecord,   // Pointer to the record to delete from the table
    DWORD       dwSignature
    )
{
    IRTLASSERT(IsUsable()  &&  pvRecord != NULL);

    LK_RETCODE lkrc = LK_NO_SUCH_KEY;

    // locate the beginning of the correct bucket chain
    WriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    CBucket* const pbkt = _FindBucket(dwSignature, true);
    IRTLASSERT(pbkt != NULL);
    IRTLASSERT(pbkt->IsWriteLocked());
    WriteUnlock();

    const DWORD_PTR pnKey = _ExtractKey(pvRecord);
    IRTLASSERT(dwSignature == _CalcKeyHash(pnKey));

    // scan down the bucket chain, looking for the victim
    for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
         pncCurr != NULL;
         pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pncCurr->IsEmptySlot(i))
            {
                IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                IRTLASSERT(0 == _IsNodeCompact(pbkt));
                IRTLASSERT(pncCurr->IsLastClump());
                goto exit;
            }

            if (pncCurr->m_pvNode[i] == pvRecord)
            {
                IRTLASSERT(_EqualKeys(pnKey,
                                      _ExtractKey(pncCurr->m_pvNode[i])));
                IRTLASSERT(dwSignature == pncCurr->m_dwKeySigs[i]);

                IRTLVERIFY(_DeleteNode(pbkt, pncCurr, pncPrev, i));

                lkrc = LK_SUCCESS;
                goto exit;
            }
        }
    }

  exit:
    pbkt->WriteUnlock();

    if (lkrc == LK_SUCCESS)
    {
        // contract the table if necessary
        double maxcontract = 1.0 / static_cast<double>(m_MaxLoad);

        for (int contractions = 0;
             m_cRecords < m_MaxLoad * m_cActiveBuckets
                 &&  m_cActiveBuckets > m_dwSegSize * MIN_DIRSIZE
                 &&  contractions < maxcontract;
             ++contractions)
        {
            // If _Contract returns an error code (viz. LK_ALLOC_FAIL), it
            // just means that there isn't enough spare memory to contract
            // the table by one bucket. This is likely to cause problems
            // elsewhere soon, but this hashtable has not been corrupted.
            if (_Contract() != LK_SUCCESS)
                break;
        }
    }

    return lkrc;
} // CLKRLinearHashTable::_DeleteRecord



// Thin wrapper for the corresponding method in CLKRLinearHashTable

LK_RETCODE
CLKRHashTable::DeleteRecord(
    const void* pvRecord)
{
    if (!IsUsable())
        return m_lkrcState;
    
    if (pvRecord == NULL)
        return LK_BAD_RECORD;
    
    LKRHASH_GLOBAL_WRITE_LOCK();    // usu. no-op
    DWORD     hash_val  = _CalcKeyHash(_ExtractKey(pvRecord));
    SubTable* const pst = _SubTable(hash_val);
    LK_RETCODE lk       = pst->_DeleteRecord(pvRecord, hash_val);
    LKRHASH_GLOBAL_WRITE_UNLOCK();    // usu. no-op
    return lk;
} // CLKRLinearHashTable::DeleteRecord



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_DeleteNode
// Synopsis: Deletes a node; removes the node clump if empty
// Returns:  true if successful
//------------------------------------------------------------------------

bool
CLKRLinearHashTable::_DeleteNode(
    CBucket*     pbkt,      // bucket chain containing node
    CNodeClump*& rpnc,      // actual node
    CNodeClump*& rpncPrev,  // predecessor of actual node, or NULL
    int&         riNode)    // index within node
{
    IRTLASSERT(pbkt != NULL  &&  pbkt->IsWriteLocked());
    IRTLASSERT(rpnc != NULL);
    IRTLASSERT(rpncPrev == NULL  ||  rpncPrev->m_pncNext == rpnc);
    IRTLASSERT(0 <= riNode  &&  riNode < NODES_PER_CLUMP);
    IRTLASSERT(!rpnc->IsEmptyAndInvalid(riNode));

#ifdef _DEBUG
    // Check that the node clump really does belong to the bucket
    CNodeClump* pnc1 = &pbkt->m_ncFirst;

    while (pnc1 != NULL  &&  pnc1 != rpnc)
         pnc1 = pnc1->m_pncNext;

    IRTLASSERT(pnc1 == rpnc);
#endif // _DEBUG

    // Release the reference to the record
    _AddRefRecord(rpnc->m_pvNode[riNode], -1);

    IRTLASSERT(0 == _IsNodeCompact(pbkt));

    // Compact the nodeclump by moving the very last node back to the
    // newly freed slot
    CNodeClump* pnc2   = rpnc;
    int         iNode2 = riNode;

    // Find the last nodeclump in the chain
    while (!pnc2->IsLastClump())
    {
         pnc2 = pnc2->m_pncNext;
         iNode2 = NODE_BEGIN;
    }

    IRTLASSERT(0 <= iNode2  &&  iNode2 < NODES_PER_CLUMP);
    IRTLASSERT(!pnc2->IsEmptyAndInvalid(iNode2));

    // Find the first empty slot in the nodeclump
    while (iNode2 != NODE_END  &&  !pnc2->IsEmptySlot(iNode2))
    {
        iNode2 += NODE_STEP;
    }

    // Back up to last non-empty slot
    iNode2 -= NODE_STEP;
    IRTLASSERT(0 <= iNode2  &&  iNode2 < NODES_PER_CLUMP
               &&  !pnc2->IsEmptyAndInvalid(iNode2));
    IRTLASSERT(iNode2+NODE_STEP == NODE_END
               ||  pnc2->IsEmptyAndInvalid(iNode2+NODE_STEP));

#ifdef _DEBUG
    // Check that all the remaining nodes are empty
    IRTLASSERT(pnc2->IsLastClump());
    for (int iNode3 = iNode2 + NODE_STEP;
         iNode3 != NODE_END;
         iNode3 += NODE_STEP)
    {
        IRTLASSERT(pnc2->IsEmptyAndInvalid(iNode3));
    }
#endif // _DEBUG

    // Move the last node's data back to the current node
    rpnc->m_pvNode[riNode]    = pnc2->m_pvNode[iNode2];
    rpnc->m_dwKeySigs[riNode] = pnc2->m_dwKeySigs[iNode2];

    // Blank the old last node.
    // Correct even if (rpnc, riNode) == (pnc2, iNode2).
    pnc2->m_pvNode[iNode2]    = NULL;
    pnc2->m_dwKeySigs[iNode2] = HASH_INVALID_SIGNATURE;

    IRTLASSERT(0 == _IsNodeCompact(pbkt));

    // Back up riNode by one, so that the next iteration of the loop
    // calling _DeleteNode will end up pointing to the same spot.
    if (riNode != NODE_BEGIN)
    {
        riNode -= NODE_STEP;
    }
    else
    {
        // rewind rpnc and rpncPrev to previous node
        if (rpnc == &pbkt->m_ncFirst)
        {
            riNode = NODE_BEGIN - NODE_STEP;
        }
        else
        {
            riNode = NODE_END;
            rpnc = rpncPrev;
            if (rpnc == &pbkt->m_ncFirst)
            {
                rpncPrev = NULL;
            }
            else
            {
                for (rpncPrev = &pbkt->m_ncFirst;
                     rpncPrev->m_pncNext != rpnc;
                     rpncPrev = rpncPrev->m_pncNext)
                {}
            }
        }
    }

    // Is the last node clump now completely empty?  Delete, if possible
    if (iNode2 == NODE_BEGIN  &&  pnc2 != &pbkt->m_ncFirst)
    {
        // Find preceding nodeclump
        CNodeClump* pnc3 = &pbkt->m_ncFirst;
        while (pnc3->m_pncNext != pnc2)
        {
            pnc3 = pnc3->m_pncNext;
            IRTLASSERT(pnc3 != NULL);
        }

        pnc3->m_pncNext = NULL;
#ifdef _DEBUG
        pnc2->m_pncNext = NULL; // or dtor will ASSERT
#endif // _DEBUG
        _FreeNodeClump(pnc2);
    }

    IRTLASSERT(rpncPrev == NULL  ||  rpncPrev->m_pncNext == rpnc);

    InterlockedDecrement(reinterpret_cast<LONG*>(&m_cRecords));

    return true;
} // CLKRLinearHashTable::_DeleteNode



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FindKey
// Synopsis: Locate the record associated with the given key value.
// Returns:  Pointer to the record, if it is found.
//           NULL, if the record is not found.
// Returns:  LK_SUCCESS, if record found (record is returned in *ppvRecord)
//           LK_BAD_RECORD, if ppvRecord is invalid
//           LK_NO_SUCH_KEY, if no record with the given key value was found.
//           LK_UNUSABLE, if hash table not in usable state
// Note:     the record is AddRef'd.  You must decrement the reference count
//           when you are finished with the record (if you're implementing
//           refcounting semantics).
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_FindKey(
    const DWORD_PTR  pnKey,  // Key value of the record, depends on key type
    DWORD        dwSignature,// hash signature
    const void** ppvRecord   // resultant record
    ) const
{
    IRTLASSERT(IsUsable()  &&  ppvRecord != NULL);

    *ppvRecord = NULL;
    LK_RETCODE lkrc = LK_NO_SUCH_KEY;

    // locate the beginning of the correct bucket chain
    bool fReadLocked = _ReadOrWriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    CBucket* const pbkt = _FindBucket(dwSignature, false);
    IRTLASSERT(pbkt != NULL);
    IRTLASSERT(pbkt->IsReadLocked());
    _ReadOrWriteUnlock(fReadLocked);

    // walk down the bucket chain
    for (CNodeClump* pncCurr = &pbkt->m_ncFirst;
         pncCurr != NULL;
         pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pncCurr->IsEmptySlot(i))
            {
                IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                IRTLASSERT(0 == _IsNodeCompact(pbkt));
                IRTLASSERT(pncCurr->IsLastClump());
                goto exit;
            }

            if (dwSignature != pncCurr->m_dwKeySigs[i])
                continue;

            const DWORD_PTR pnKey2 = _ExtractKey(pncCurr->m_pvNode[i]);

            if (
#ifdef LKR_EQUALKEYS
                pnKey == pnKey2  ||
#endif// LKR_EQUALKEYS
                _EqualKeys(pnKey,  pnKey2))
            {
                    *ppvRecord = pncCurr->m_pvNode[i];
                    lkrc = LK_SUCCESS;

                    // bump the reference count before handing the record
                    // back to the user.  The user should decrement the
                    // reference count when finished with this record.
                    _AddRefRecord(*ppvRecord, +1);
                    goto exit;
            }
        }
    }

  exit:
    pbkt->ReadUnlock();
    return lkrc;
} // CLKRLinearHashTable::_FindKey



// Thin wrapper for the corresponding method in CLKRLinearHashTable

LK_RETCODE
CLKRHashTable::FindKey(
    const DWORD_PTR pnKey,
    const void** ppvRecord) const
{
    if (!IsUsable())
        return m_lkrcState;
    
    if (ppvRecord == NULL)
        return LK_BAD_RECORD;
    
    LKRHASH_GLOBAL_READ_LOCK();    // usu. no-op
    DWORD     hash_val   = _CalcKeyHash(pnKey);
    SubTable* const pst  = _SubTable(hash_val);
    LK_RETCODE lkrc      = pst->_FindKey(pnKey, hash_val, ppvRecord);
    LKRHASH_GLOBAL_READ_UNLOCK();    // usu. no-op
    return lkrc;
} // CLKRLinearHashTable::FindKey



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FindRecord
// Synopsis: Sees if the record is contained in the table
// Returns:  Pointer to the record, if it is found.
//           NULL, if the record is not found.
// Returns:  LK_SUCCESS, if record found
//           LK_BAD_RECORD, if pvRecord is invalid
//           LK_NO_SUCH_KEY, if the record was not found in the table
//           LK_UNUSABLE, if hash table not in usable state
// Note:     The record is *not* AddRef'd.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_FindRecord(
    const void* pvRecord,    // Pointer to the record to find in the table
    DWORD       dwSignature  // hash signature
    ) const
{
    IRTLASSERT(IsUsable()  &&  pvRecord != NULL);

    LK_RETCODE lkrc = LK_NO_SUCH_KEY;

    // locate the beginning of the correct bucket chain
    bool fReadLocked = _ReadOrWriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    CBucket* const pbkt = _FindBucket(dwSignature, false);
    IRTLASSERT(pbkt != NULL);
    IRTLASSERT(pbkt->IsReadLocked());
    _ReadOrWriteUnlock(fReadLocked);

    const DWORD_PTR pnKey = _ExtractKey(pvRecord);
    IRTLASSERT(dwSignature == _CalcKeyHash(pnKey));

    // walk down the bucket chain
    for (CNodeClump* pncCurr = &pbkt->m_ncFirst;
         pncCurr != NULL;
         pncCurr = pncCurr->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pncCurr->IsEmptySlot(i))
            {
                IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                IRTLASSERT(0 == _IsNodeCompact(pbkt));
                IRTLASSERT(pncCurr->IsLastClump());
                goto exit;
            }

            if (pncCurr->m_pvNode[i] == pvRecord)
            {
                IRTLASSERT(dwSignature == pncCurr->m_dwKeySigs[i]);
                IRTLASSERT(_EqualKeys(pnKey,
                                      _ExtractKey(pncCurr->m_pvNode[i])));
                lkrc = LK_SUCCESS;

                goto exit;
            }
        }
    }

  exit:
    pbkt->ReadUnlock();
    return lkrc;
} // CLKRLinearHashTable::_FindRecord



// Thin wrapper for the corresponding method in CLKRLinearHashTable

LK_RETCODE
CLKRHashTable::FindRecord(
    const void* pvRecord) const
{
    if (!IsUsable())
        return m_lkrcState;
    
    if (pvRecord == NULL)
        return LK_BAD_RECORD;
    
    LKRHASH_GLOBAL_READ_LOCK();    // usu. no-op
    DWORD     hash_val   = _CalcKeyHash(_ExtractKey(pvRecord));
    SubTable* const pst  = _SubTable(hash_val);
    LK_RETCODE lkrc      = pst->_FindRecord(pvRecord, hash_val);
    LKRHASH_GLOBAL_READ_UNLOCK();    // usu. no-op
    return lkrc;
} // CLKRLinearHashTable::FindRecord



#ifdef LKR_DEPRECATED_ITERATORS

//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::Apply
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::Apply(
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    LK_PREDICATE lkp = LKP_PERFORM;
    if (lkl == LKL_WRITELOCK)
        WriteLock();
    else
        ReadLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    DWORD dw = _Apply(pfnAction, pvState, lkl, lkp);
    if (lkl == LKL_WRITELOCK)
        WriteUnlock();
    else
        ReadUnlock();

    return dw;
} // CLKRLinearHashTable::Apply



//------------------------------------------------------------------------
// Function: CLKRHashTable::Apply
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRHashTable::Apply(
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    DWORD dw = 0;
    LK_PREDICATE lkp = LKP_PERFORM;

    if (lkl == LKL_WRITELOCK)
        WriteLock();
    else
        ReadLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    if (IsValid())
    {
        for (DWORD i = 0;  i < m_cSubTables;  i++)
        {
            dw += m_palhtDir[i]->_Apply(pfnAction, pvState, lkl, lkp);
            if (lkp == LKP_ABORT  ||  lkp == LKP_PERFORM_STOP
                ||  lkp == LKP_DELETE_STOP)
                break;
        }
    }

    if (lkl == LKL_WRITELOCK)
        WriteUnlock();
    else
        ReadUnlock();

    return dw;
} // CLKRHashTable::Apply



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::ApplyIf
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::ApplyIf(
    PFnRecordPred   pfnPredicate,
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    DWORD dw = 0;
    LK_PREDICATE lkp = LKP_PERFORM;

    if (lkl == LKL_WRITELOCK)
        WriteLock();
    else
        ReadLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    if (IsValid())
    {
        dw = _ApplyIf(pfnPredicate, pfnAction, pvState, lkl, lkp);
    }

    if (lkl == LKL_WRITELOCK)
        WriteUnlock();
    else
        ReadUnlock();
    return dw;
} // CLKRLinearHashTable::ApplyIf



//------------------------------------------------------------------------
// Function: CLKRHashTable::ApplyIf
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRHashTable::ApplyIf(
    PFnRecordPred   pfnPredicate,
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    DWORD dw = 0;
    LK_PREDICATE lkp = LKP_PERFORM;

    if (lkl == LKL_WRITELOCK)
        WriteLock();
    else
        ReadLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    if (IsValid())
    {
        for (DWORD i = 0;  i < m_cSubTables;  i++)
        {
            dw += m_palhtDir[i]->_ApplyIf(pfnPredicate, pfnAction,
                                          pvState, lkl, lkp);
            if (lkp == LKP_ABORT  ||  lkp == LKP_PERFORM_STOP
                ||  lkp == LKP_DELETE_STOP)
                break;
        }
    }

    if (lkl == LKL_WRITELOCK)
        WriteUnlock();
    else
        ReadUnlock();

    return dw;
} // CLKRHashTable::ApplyIf



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::DeleteIf
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::DeleteIf(
    PFnRecordPred pfnPredicate,
    void*         pvState)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    DWORD dw = 0;
    LK_PREDICATE lkp = LKP_PERFORM;

    WriteLock();
    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());
    if (IsValid())
        dw = _DeleteIf(pfnPredicate, pvState, lkp);
    WriteUnlock();

    return dw;
} // CLKRLinearHashTable::DeleteIf



//------------------------------------------------------------------------
// Function: CLKRHashTable::DeleteIf
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRHashTable::DeleteIf(
    PFnRecordPred pfnPredicate,
    void*         pvState)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    DWORD dw = 0;
    LK_PREDICATE lkp = LKP_PERFORM;

    WriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    if (IsValid())
    {
        for (DWORD i = 0;  i < m_cSubTables;  i++)
        {
            dw += m_palhtDir[i]->_DeleteIf(pfnPredicate, pvState, lkp);
            if (lkp == LKP_ABORT  ||  lkp == LKP_PERFORM_STOP
                ||  lkp == LKP_DELETE_STOP)
                break;
        }
    }

    WriteUnlock();

    return dw;
} // CLKRHashTable::DeleteIf



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_Apply
// Synopsis:
// Returns:
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::_Apply(
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl,
    LK_PREDICATE&  rlkp)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(lkl == LKL_WRITELOCK  ?  IsWriteLocked()  :  IsReadLocked());
    return _ApplyIf(_PredTrue, pfnAction, pvState, lkl, rlkp);
} // CLKRLinearHashTable::_Apply



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_ApplyIf
// Synopsis:
// Returns:  Number of successful actions
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::_ApplyIf(
    PFnRecordPred   pfnPredicate,
    PFnRecordAction pfnAction,
    void*           pvState,
    LK_LOCKTYPE    lkl,
    LK_PREDICATE&  rlkp)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(lkl == LKL_WRITELOCK  ?  IsWriteLocked()  :  IsReadLocked());
    IRTLASSERT(pfnPredicate != NULL  &&  pfnAction != NULL);

    if ((lkl == LKL_WRITELOCK  ?  !IsWriteLocked()  :  !IsReadLocked())
            ||  pfnPredicate == NULL  ||  pfnAction == NULL)
        return 0;

    DWORD cActions = 0;

    for (DWORD iBkt = 0;  iBkt < m_cActiveBuckets;  ++iBkt)
    {
        CBucket* const pbkt = _Bucket(iBkt);
        IRTLASSERT(pbkt != NULL);

        if (lkl == LKL_WRITELOCK)
            pbkt->WriteLock();
        else
            pbkt->ReadLock();

        for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
             pncCurr != NULL;
             pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
        {
            int i;

            FOR_EACH_NODE(i)
            {
                if (pncCurr->IsEmptySlot(i))
                {
                    IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                    IRTLASSERT(0 == _IsNodeCompact(pbkt));
                    IRTLASSERT(pncCurr->IsLastClump());
                    goto unlock;
                }
                else
                {
                    rlkp = (*pfnPredicate)(pncCurr->m_pvNode[i], pvState);

                    switch (rlkp)
                    {
                    case LKP_ABORT:
                        if (lkl == LKL_WRITELOCK)
                            pbkt->WriteUnlock();
                        else
                            pbkt->ReadUnlock();
                        return cActions;
                        break;

                    case LKP_NO_ACTION:
                        // nothing to do
                        break;

                    case LKP_DELETE:
                    case LKP_DELETE_STOP:
                        if (lkl != LKL_WRITELOCK)
                        {
                            pbkt->ReadUnlock();
                            return cActions;
                        }

                        // fall through

                    case LKP_PERFORM:
                    case LKP_PERFORM_STOP:
                    {
                        LK_ACTION lka;

                        if (rlkp == LKP_DELETE  ||  rlkp == LKP_DELETE_STOP)
                        {
                            IRTLVERIFY(_DeleteNode(pbkt, pncCurr, pncPrev, i));

                            ++cActions;
                            lka = LKA_SUCCEEDED;
                        }
                        else
                        {
                            lka = (*pfnAction)(pncCurr->m_pvNode[i], pvState);

                            switch (lka)
                            {
                            case LKA_ABORT:
                                if (lkl == LKL_WRITELOCK)
                                    pbkt->WriteUnlock();
                                else
                                    pbkt->ReadUnlock();
                                return cActions;
                                
                            case LKA_FAILED:
                                // nothing to do
                                break;
                                
                            case LKA_SUCCEEDED:
                                ++cActions;
                                break;
                                
                            default:
                                IRTLASSERT(FALSE);
                                break;
                            }
                        }

                        if (rlkp == LKP_PERFORM_STOP
                            ||  rlkp == LKP_DELETE_STOP)
                        {
                            if (lkl == LKL_WRITELOCK)
                                pbkt->WriteUnlock();
                            else
                                pbkt->ReadUnlock();
                            return cActions;
                        }

                        break;
                    }

                    default:
                        IRTLASSERT(FALSE);
                        break;
                    }
                }
            }
        }

      unlock:
        if (lkl == LKL_WRITELOCK)
            pbkt->WriteUnlock();
        else
            pbkt->ReadUnlock();
    }

    return cActions;
} // CLKRLinearHashTable::_ApplyIf



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_DeleteIf
// Synopsis: Deletes all records that match the predicate
// Returns:  Count of successful deletions
//------------------------------------------------------------------------

DWORD
CLKRLinearHashTable::_DeleteIf(
    PFnRecordPred  pfnPredicate,
    void*          pvState,
    LK_PREDICATE& rlkp)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(IsWriteLocked());
    IRTLASSERT(pfnPredicate != NULL);

    if (!IsWriteLocked()  ||  pfnPredicate == NULL)
        return 0;

    DWORD cActions = 0;

    for (DWORD iBkt = 0;  iBkt < m_cActiveBuckets;  ++iBkt)
    {
        CBucket* const pbkt = _Bucket(iBkt);
        IRTLASSERT(pbkt != NULL);
        pbkt->WriteLock();

        for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
             pncCurr != NULL;
             pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
        {
            int i;

            FOR_EACH_NODE(i)
            {
                if (pncCurr->IsEmptySlot(i))
                {
                    IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                    IRTLASSERT(0 == _IsNodeCompact(pbkt));
                    IRTLASSERT(pncCurr->IsLastClump());
                    goto unlock;
                }
                else
                {
                    rlkp = (*pfnPredicate)(pncCurr->m_pvNode[i], pvState);

                    switch (rlkp)
                    {
                    case LKP_ABORT:
                        pbkt->WriteUnlock();
                        return cActions;
                        break;

                    case LKP_NO_ACTION:
                        // nothing to do
                        break;

                    case LKP_PERFORM:
                    case LKP_PERFORM_STOP:
                    case LKP_DELETE:
                    case LKP_DELETE_STOP:
                    {
                        IRTLVERIFY(_DeleteNode(pbkt, pncCurr, pncPrev, i));

                        ++cActions;

                        if (rlkp == LKP_PERFORM_STOP
                            ||  rlkp == LKP_DELETE_STOP)
                        {
                            pbkt->WriteUnlock();
                            return cActions;
                        }

                        break;
                    }

                    default:
                        IRTLASSERT(FALSE);
                        break;
                    }
                }
            }
        }

      unlock:
        pbkt->WriteUnlock();
    }

    return cActions;
} // CLKRLinearHashTable::_DeleteIf

#endif // LKR_DEPRECATED_ITERATORS



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::CheckTable
// Synopsis: Verify that all records are in the right place and can be located.
// Returns:   0 => hash table is consistent
//           >0 => that many misplaced records
//           <0 => otherwise invalid
//------------------------------------------------------------------------

int
CLKRLinearHashTable::CheckTable() const
{
    if (!IsUsable())
        return LK_UNUSABLE;

    bool fReadLocked = _ReadOrWriteLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());

    if (!IsValid())
    {
        _ReadOrWriteUnlock(fReadLocked);
        return LK_UNUSABLE;
    }

    int       cMisplaced = 0;
    DWORD     cRecords = 0;
    int       retcode = 0;

    // Check every bucket
    for (DWORD i = 0;  i < m_cActiveBuckets;  i++)
    {
        CBucket* const pbkt = _Bucket(i);

        IRTLASSERT(pbkt != NULL);
        retcode += !(pbkt != NULL);

        pbkt->ReadLock();

        IRTLASSERT(0 == _IsNodeCompact(pbkt));

        // Walk the bucket chain
        for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
             pncCurr != NULL;
             pncPrev = pncCurr, pncCurr = pncCurr->m_pncNext)
        {
            int j;

            FOR_EACH_NODE(j)
            {
                if (pncCurr->IsEmptySlot(j))
                {
                    IRTLASSERT(pncCurr->IsLastClump());
                    retcode += !(pncCurr->IsLastClump());

                    for (int k = j;  k != NODE_END;  k += NODE_STEP)
                    {
                        IRTLASSERT(pncCurr->IsEmptyNode(k));
                        retcode += !pncCurr->IsEmptyNode(k);
                        IRTLASSERT(pncCurr->InvalidSignature(k));
                        retcode += !pncCurr->InvalidSignature(k);
                    }
                    break;
                }

                if (!pncCurr->IsEmptySlot(j))
                {
                    ++cRecords;

                    const DWORD_PTR pnKey = _ExtractKey(pncCurr->m_pvNode[j]);

                    DWORD dwSignature = _CalcKeyHash(pnKey);
                    IRTLASSERT(dwSignature != HASH_INVALID_SIGNATURE);
                    retcode += !(dwSignature != HASH_INVALID_SIGNATURE);
                    IRTLASSERT(dwSignature == pncCurr->m_dwKeySigs[j]);
                    retcode += !(dwSignature == pncCurr->m_dwKeySigs[j]);

                    DWORD address = _BucketAddress(dwSignature);
                    IRTLASSERT(address == i);
                    retcode += !(address == i);

                    if (address != i || dwSignature != pncCurr->m_dwKeySigs[j])
                        cMisplaced++;
                }
                else // pncCurr->IsEmptySlot(j)
                {
                    IRTLASSERT(pncCurr->IsEmptyAndInvalid(j));
                    retcode += !pncCurr->IsEmptyAndInvalid(j);
                }
            }
            if (pncPrev != NULL)
            {
                IRTLASSERT(pncPrev->m_pncNext == pncCurr);
                retcode += !(pncPrev->m_pncNext == pncCurr);
            }
        }
        pbkt->ReadUnlock();
    }

    if (cRecords != m_cRecords)
        ++retcode;

    IRTLASSERT(cRecords == m_cRecords);
    retcode += !(cRecords == m_cRecords);

    if (cMisplaced > 0)
        retcode = cMisplaced;

    IRTLASSERT(cMisplaced == 0);
    retcode += !(cMisplaced == 0);

    _ReadOrWriteUnlock(fReadLocked);

    return retcode;
} // CheckTable



//------------------------------------------------------------------------
// Function: CLKRHashTable::CheckTable
// Synopsis: Verify that all records are in the right place and can be located.
// Returns:   0 => hash table is consistent
//           >0 => that many misplaced records
//           <0 => otherwise invalid
//------------------------------------------------------------------------
int
CLKRHashTable::CheckTable() const
{
    if (!IsUsable())
        return LK_UNUSABLE;

    int retcode = 0;

    for (DWORD i = 0;  i < m_cSubTables;  i++)
        retcode += m_palhtDir[i]->CheckTable();

    return retcode;
} // CLKRHashTable::CheckTable



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_Clear
// Synopsis: Remove all data from the table
//------------------------------------------------------------------------

void
CLKRLinearHashTable::_Clear(
    bool fShrinkDirectory)  // Shrink to min size but don't destroy entirely?
{
    if (!IsUsable())
        return;

    IRTLASSERT(IsWriteLocked());

#ifdef _DEBUG
    DWORD cDeleted = 0;
    DWORD cOldRecords = m_cRecords;
#endif // _DEBUG

    for (DWORD iBkt = 0;  iBkt < m_cActiveBuckets;  ++iBkt)
    {
        CBucket* const pbkt = _Bucket(iBkt);
        IRTLASSERT(pbkt != NULL);
        pbkt->WriteLock();

        IRTLASSERT(0 == _IsNodeCompact(pbkt));

        for (CNodeClump* pncCurr = &pbkt->m_ncFirst, *pncPrev = NULL;
             pncCurr != NULL;
             )
        {
            int i;

            FOR_EACH_NODE(i)
            {

                if (pncCurr->IsEmptySlot(i))
                {
                    IRTLASSERT(pncCurr->IsEmptyAndInvalid(i));
                    IRTLASSERT(pncCurr->IsLastClump());
                    break;
                }
                else
                {
                    _AddRefRecord(pncCurr->m_pvNode[i], -1);
                    pncCurr->m_pvNode[i]    = NULL;
                    pncCurr->m_dwKeySigs[i] = HASH_INVALID_SIGNATURE;
                    m_cRecords--;

#ifdef _DEBUG
                    ++cDeleted;
#endif // _DEBUG
                }
            } // for (i ...

            pncPrev = pncCurr;
            pncCurr = pncCurr->m_pncNext;
            pncPrev->m_pncNext = NULL;

            if (pncPrev != &pbkt->m_ncFirst)
                _FreeNodeClump(pncPrev);
        } // for (pncCurr ...

        pbkt->WriteUnlock();
    } // for (iBkt ...

    IRTLASSERT(m_cRecords == 0  &&  cDeleted == cOldRecords);

    // delete all (or all but the first MIN_DIRSIZE) segments
    for (DWORD iSeg = fShrinkDirectory  ?  MIN_DIRSIZE * m_dwSegSize  :  0;
         iSeg < m_cActiveBuckets;
         iSeg += m_dwSegSize)
    {
        _FreeSegment(_Segment(iSeg));
        _Segment(iSeg) = NULL;
    }

    // reduce directory of segments to minimum size
    if (fShrinkDirectory)
    {
        if (m_cDirSegs > MIN_DIRSIZE)
        {
            CDirEntry* paDirSegsNew = _AllocateSegmentDirectory(MIN_DIRSIZE);
            
            if (paDirSegsNew != NULL)
            {
                for (DWORD j = 0;  j < MIN_DIRSIZE;  j++)
                    paDirSegsNew[j] = m_paDirSegs[j];
                for (j = 0;  j < m_cDirSegs;  j++)
                    m_paDirSegs[j].m_pseg = NULL;
                
                _FreeSegmentDirectory(m_paDirSegs);
                
                m_paDirSegs = paDirSegsNew;
                m_cDirSegs  = MIN_DIRSIZE;
            }

            m_nLevel         = m_dwSegBits;
            m_cActiveBuckets = MIN_DIRSIZE * m_dwSegSize;
            m_dwBktAddrMask0 = m_dwSegMask;
            m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;

            for (DWORD tmp = m_cActiveBuckets >> m_dwSegBits;
                 tmp > 1;
                 tmp >>= 1)
            {
                ++m_nLevel;
                m_dwBktAddrMask0 =  m_dwBktAddrMask1;
                m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;
            }
            
            IRTLASSERT(_H1(m_cActiveBuckets) == m_cActiveBuckets);
            m_iExpansionIdx = m_cActiveBuckets & m_dwBktAddrMask0;
        }

#ifdef _DEBUG
        for (DWORD iSeg = 0;  iSeg < m_cActiveBuckets;  iSeg += m_dwSegSize)
        {
            IRTLASSERT(_Segment(iSeg) != NULL);
        }
#endif // _DEBUG
    }

    else // !fShrinkDirectory
    {
        _FreeSegmentDirectory(m_paDirSegs);
        m_paDirSegs = NULL;
        m_cDirSegs = m_nLevel = m_cActiveBuckets = m_iExpansionIdx = 0;
        m_dwBktAddrMask0 = 1;
        m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;
    }
} // CLKRLinearHashTable::_Clear



//------------------------------------------------------------------------
// Function: CLKRHashTable::Clear
// Synopsis: Remove all data from the table
//------------------------------------------------------------------------

void
CLKRHashTable::Clear()
{
    WriteLock();
    for (DWORD i = 0;  i < m_cSubTables;  i++)
        m_palhtDir[i]->_Clear(true);
    WriteUnlock();
} // CLKRHashTable::Clear



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::GetStatistics
// Synopsis: Gather statistics about the table
//------------------------------------------------------------------------

CLKRHashTableStats
CLKRLinearHashTable::GetStatistics() const
{
    CLKRHashTableStats stats;

    if (!IsUsable())
        return stats;

    if (m_paDirSegs != NULL)
    {
        stats.RecordCount   = m_cRecords;
        stats.TableSize     = m_cActiveBuckets;
        stats.SplitFactor   = static_cast<double>(m_iExpansionIdx)
                              / (1 << m_nLevel);
        stats.DirectorySize = m_cDirSegs;
        stats.NodeClumpSize = NODES_PER_CLUMP;
        stats.CBucketSize   = sizeof(CBucket);

#ifdef LOCK_INSTRUMENTATION
        stats.m_alsBucketsAvg.m_nContentions     = 0;
        stats.m_alsBucketsAvg.m_nSleeps          = 0;
        stats.m_alsBucketsAvg.m_nContentionSpins = 0;
        stats.m_alsBucketsAvg.m_nAverageSpins    = 0;
        stats.m_alsBucketsAvg.m_nReadLocks       = 0;
        stats.m_alsBucketsAvg.m_nWriteLocks      = 0;
        stats.m_alsBucketsAvg.m_nItems           = 0;
#endif // LOCK_INSTRUMENTATION

        int empty = 0;
        int totacc = 0;
        int low_count = 0;
        int high_count = 0;
        int max_length = 0;

        for (DWORD i = 0;  i < m_cActiveBuckets;  i++)
        {
            int acc = 0;

            for (CNodeClump* pncCurr = &_Bucket(i)->m_ncFirst;
                 pncCurr != NULL;
                 pncCurr = pncCurr->m_pncNext)
            {
                int j;

                FOR_EACH_NODE(j)
                {
                    if (!pncCurr->IsEmptySlot(j))
                    {
                        acc++;
                        totacc += acc;
                        int iBucketIndex = stats.BucketIndex(acc);
                        ++stats.m_aBucketLenHistogram[iBucketIndex];
                    }
                }
            }

#ifdef LOCK_INSTRUMENTATION
            CLockStatistics ls = _Bucket(i)->LockStats();

            stats.m_alsBucketsAvg.m_nContentions     += ls.m_nContentions;
            stats.m_alsBucketsAvg.m_nSleeps          += ls.m_nSleeps;
            stats.m_alsBucketsAvg.m_nContentionSpins += ls.m_nContentionSpins;
            stats.m_alsBucketsAvg.m_nAverageSpins    += ls.m_nAverageSpins;
            stats.m_alsBucketsAvg.m_nReadLocks       += ls.m_nReadLocks;
            stats.m_alsBucketsAvg.m_nWriteLocks      += ls.m_nWriteLocks;
            stats.m_alsBucketsAvg.m_nItems           ++;
#endif // LOCK_INSTRUMENTATION

            max_length = max(max_length, acc);
            if (acc == 0)
                empty++;

            if (_H0(i) < m_iExpansionIdx)
            {
                low_count += acc;
            }
            else
            {
                high_count += acc;
            }
        }

        stats.LongestChain = max_length;
        stats.EmptySlots   = empty;

        if (m_cActiveBuckets > 0)
        {
            if (m_cRecords > 0)
            {
                double x=static_cast<double>(m_iExpansionIdx) /(1 << m_nLevel);
                double alpha= static_cast<double>(m_cRecords)/m_cActiveBuckets;
                double low_sl = 0.0;
                double high_sl = 0.0;
                
                stats.AvgSearchLength= static_cast<double>(totacc) /m_cRecords;
                stats.ExpSearchLength  = 1 + alpha * 0.25 * (2 + x - x*x);
                
                if (m_iExpansionIdx > 0)
                    low_sl  = static_cast<double>(low_count)
                        / (2.0 * m_iExpansionIdx);
                if (m_cActiveBuckets - 2 * m_iExpansionIdx > 0)
                    high_sl = static_cast<double>(high_count)
                        / (m_cActiveBuckets - 2.0 * m_iExpansionIdx);
                stats.AvgUSearchLength = low_sl * x + high_sl * (1.0 - x);
                stats.ExpUSearchLength = alpha * 0.5 * (2 + x - x*x);
            }

#ifdef LOCK_INSTRUMENTATION
            stats.m_alsBucketsAvg.m_nContentions     /= m_cActiveBuckets;
            stats.m_alsBucketsAvg.m_nSleeps          /= m_cActiveBuckets;
            stats.m_alsBucketsAvg.m_nContentionSpins /= m_cActiveBuckets;
            stats.m_alsBucketsAvg.m_nAverageSpins    /= m_cActiveBuckets;
            stats.m_alsBucketsAvg.m_nReadLocks       /= m_cActiveBuckets;
            stats.m_alsBucketsAvg.m_nWriteLocks      /= m_cActiveBuckets;
#endif // LOCK_INSTRUMENTATION

        }
        else
        {
            stats.AvgSearchLength  = 0.0;
            stats.ExpSearchLength  = 0.0;
            stats.AvgUSearchLength = 0.0;
            stats.ExpUSearchLength = 0.0;
        }
    }

#ifdef LOCK_INSTRUMENTATION
    stats.m_gls     = TableLock::GlobalStatistics();
    CLockStatistics ls = _LockStats();

    stats.m_alsTable.m_nContentions     = ls.m_nContentions;
    stats.m_alsTable.m_nSleeps          = ls.m_nSleeps;
    stats.m_alsTable.m_nContentionSpins = ls.m_nContentionSpins;
    stats.m_alsTable.m_nAverageSpins    = ls.m_nAverageSpins;
    stats.m_alsTable.m_nReadLocks       = ls.m_nReadLocks;
    stats.m_alsTable.m_nWriteLocks      = ls.m_nWriteLocks;
    stats.m_alsTable.m_nItems           = 1;
#endif // LOCK_INSTRUMENTATION

    return stats;
} // CLKRLinearHashTable::GetStatistics



//------------------------------------------------------------------------
// Function: CLKRHashTable::GetStatistics
// Synopsis: Gather statistics about the table
//------------------------------------------------------------------------

CLKRHashTableStats
CLKRHashTable::GetStatistics() const
{
    CLKRHashTableStats hts;

    if (!IsUsable())
        return hts;

    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        CLKRHashTableStats stats = m_palhtDir[i]->GetStatistics();

        hts.RecordCount +=      stats.RecordCount;
        hts.TableSize +=        stats.TableSize;
        hts.DirectorySize +=    stats.DirectorySize;
        hts.LongestChain =      max(hts.LongestChain, stats.LongestChain);
        hts.EmptySlots +=       stats.EmptySlots;
        hts.SplitFactor +=      stats.SplitFactor;
        hts.AvgSearchLength +=  stats.AvgSearchLength;
        hts.ExpSearchLength +=  stats.ExpSearchLength;
        hts.AvgUSearchLength += stats.AvgUSearchLength;
        hts.ExpUSearchLength += stats.ExpUSearchLength;
        hts.NodeClumpSize =     stats.NodeClumpSize;
        hts.CBucketSize =       stats.CBucketSize;

        for (int j = 0;  j < CLKRHashTableStats::MAX_BUCKETS;  ++j)
            hts.m_aBucketLenHistogram[j] += stats.m_aBucketLenHistogram[j];

#ifdef LOCK_INSTRUMENTATION
        hts.m_alsTable.m_nContentions     += stats.m_alsTable.m_nContentions;
        hts.m_alsTable.m_nSleeps          += stats.m_alsTable.m_nSleeps;
        hts.m_alsTable.m_nContentionSpins
            += stats.m_alsTable.m_nContentionSpins;
        hts.m_alsTable.m_nAverageSpins    += stats.m_alsTable.m_nAverageSpins;
        hts.m_alsTable.m_nReadLocks       += stats.m_alsTable.m_nReadLocks;
        hts.m_alsTable.m_nWriteLocks      += stats.m_alsTable.m_nWriteLocks;
        
        hts.m_alsBucketsAvg.m_nContentions
            += stats.m_alsBucketsAvg.m_nContentions;
        hts.m_alsBucketsAvg.m_nSleeps
            += stats.m_alsBucketsAvg.m_nSleeps;
        hts.m_alsBucketsAvg.m_nContentionSpins
            += stats.m_alsBucketsAvg.m_nContentionSpins;
        hts.m_alsBucketsAvg.m_nAverageSpins
            += stats.m_alsBucketsAvg.m_nAverageSpins;
        hts.m_alsBucketsAvg.m_nReadLocks
            += stats.m_alsBucketsAvg.m_nReadLocks;
        hts.m_alsBucketsAvg.m_nWriteLocks
            += stats.m_alsBucketsAvg.m_nWriteLocks;
        hts.m_alsBucketsAvg.m_nItems
            += stats.m_alsBucketsAvg.m_nItems;
        
        hts.m_gls = stats.m_gls;
#endif // LOCK_INSTRUMENTATION
    }

    // Average out the subtables statistics.  (Does this make sense
    // for all of these fields?)
    hts.DirectorySize /=    m_cSubTables;
    hts.SplitFactor /=      m_cSubTables;
    hts.AvgSearchLength /=  m_cSubTables;
    hts.ExpSearchLength /=  m_cSubTables;
    hts.AvgUSearchLength /= m_cSubTables;
    hts.ExpUSearchLength /= m_cSubTables;

#ifdef LOCK_INSTRUMENTATION
    hts.m_alsTable.m_nContentions     /= m_cSubTables;
    hts.m_alsTable.m_nSleeps          /= m_cSubTables;
    hts.m_alsTable.m_nContentionSpins /= m_cSubTables;
    hts.m_alsTable.m_nAverageSpins    /= m_cSubTables;
    hts.m_alsTable.m_nReadLocks       /= m_cSubTables;
    hts.m_alsTable.m_nWriteLocks      /= m_cSubTables;
    hts.m_alsTable.m_nItems            = m_cSubTables;

    hts.m_alsBucketsAvg.m_nContentions     /= m_cSubTables;
    hts.m_alsBucketsAvg.m_nSleeps          /= m_cSubTables;
    hts.m_alsBucketsAvg.m_nContentionSpins /= m_cSubTables;
    hts.m_alsBucketsAvg.m_nAverageSpins    /= m_cSubTables;
    hts.m_alsBucketsAvg.m_nReadLocks       /= m_cSubTables;
    hts.m_alsBucketsAvg.m_nWriteLocks      /= m_cSubTables;
#endif // LOCK_INSTRUMENTATION

    return hts;
} // CLKRHashTable::GetStatistics



//-----------------------------------------------------------------------
// Function: CLKRLinearHashTable::_SetSegVars
// Synopsis: sets the size-specific segment variables
//-----------------------------------------------------------------------

void
CLKRLinearHashTable::_SetSegVars(
    LK_TABLESIZE lkts)
{
    switch (lkts)
    {
    case LK_SMALL_TABLESIZE:
        m_lkts      = LK_SMALL_TABLESIZE;
        m_dwSegBits = CSmallSegment::SEGBITS;
        m_dwSegSize = CSmallSegment::SEGSIZE;
        m_dwSegMask = CSmallSegment::SEGMASK;
        break;
        
    default:
        IRTLASSERT(FALSE);
        // fall-through
        
    case LK_MEDIUM_TABLESIZE:
        m_lkts      = LK_MEDIUM_TABLESIZE;
        m_dwSegBits = CMediumSegment::SEGBITS;
        m_dwSegSize = CMediumSegment::SEGSIZE;
        m_dwSegMask = CMediumSegment::SEGMASK;
        break;
        
    case LK_LARGE_TABLESIZE:
        m_lkts      = LK_LARGE_TABLESIZE;
        m_dwSegBits = CLargeSegment::SEGBITS;
        m_dwSegSize = CLargeSegment::SEGSIZE;
        m_dwSegMask = CLargeSegment::SEGMASK;
        break;
    }

    m_dwBktAddrMask0 = m_dwSegMask;
    m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;
    m_nLevel         = m_dwSegBits;
} // CLKRLinearHashTable::_SetSegVars




#include <stdlib.h>

LONG g_cAllocDirEntry = 0;
LONG g_cAllocNodeClump = 0;
LONG g_cAllocSmallSegment = 0;
LONG g_cAllocMediumSegment = 0;
LONG g_cAllocLargeSegment = 0;

extern "C"
__declspec(dllexport)
bool
GetAllocCounters()
{
return true;
}

// #define LKR_RANDOM_MEMORY_FAILURES 1000  // 1..RAND_MAX (32767)

// Memory allocation wrappers to allow us to simulate allocation
// failures during testing

//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_AllocateSegmentDirectory
// Synopsis: 
//------------------------------------------------------------------------

CLKRLinearHashTable::CDirEntry* const
CLKRLinearHashTable::_AllocateSegmentDirectory(
    size_t n)
{
#ifdef LKR_RANDOM_MEMORY_FAILURES
    if (rand() < LKR_RANDOM_MEMORY_FAILURES)
        return NULL;
#endif
    // InterlockedIncrement(&g_cAllocDirEntry);
    return new CDirEntry [n];
}



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FreeSegmentDirectory
// Synopsis: 
//------------------------------------------------------------------------

bool
CLKRLinearHashTable::_FreeSegmentDirectory(
    CLKRLinearHashTable::CDirEntry* paDirSegs)
{
    delete [] paDirSegs;
    return true;
}



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_AllocateNodeClump
// Synopsis: 
//------------------------------------------------------------------------

CLKRLinearHashTable::CNodeClump* const
CLKRLinearHashTable::_AllocateNodeClump()
{
#ifdef LKR_RANDOM_MEMORY_FAILURES
    if (rand() < LKR_RANDOM_MEMORY_FAILURES)
        return NULL;
#endif
    // InterlockedIncrement(&g_cAllocNodeClump);
    return new CNodeClump;
}



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FreeNodeClump
// Synopsis: 
//------------------------------------------------------------------------

bool
CLKRLinearHashTable::_FreeNodeClump(
    CLKRLinearHashTable::CNodeClump* pnc)
{
    delete pnc;
    return true;
}



//-----------------------------------------------------------------------
// Function: CLKRLinearHashTable::_AllocateSegment
// Synopsis: creates a new segment of the approriate size
// Output:   pointer to the new segment; NULL => failure
//-----------------------------------------------------------------------

CLKRLinearHashTable::CSegment* const
CLKRLinearHashTable::_AllocateSegment(
    ) const
{
#ifdef LKR_RANDOM_MEMORY_FAILURES
    if (rand() < LKR_RANDOM_MEMORY_FAILURES)
        return NULL;
#endif
    CSegment* pseg = NULL;

    switch (m_lkts)
    {
    case LK_SMALL_TABLESIZE:
#ifdef LKRHASH_ALLOCATOR_NEW
        IRTLASSERT(CLKRLinearHashTable::CSmallSegment::sm_palloc != NULL);
#endif
        // InterlockedIncrement(&g_cAllocSmallSegment);
        pseg = new CSmallSegment;
        break;
        
    default:
        IRTLASSERT(FALSE);
        // fall-through
        
    case LK_MEDIUM_TABLESIZE:
#ifdef LKRHASH_ALLOCATOR_NEW
        IRTLASSERT(CLKRLinearHashTable::CMediumSegment::sm_palloc != NULL);
#endif
        // InterlockedIncrement(&g_cAllocMediumSegment);
        pseg = new CMediumSegment;
        break;
        
    case LK_LARGE_TABLESIZE:
#ifdef LKRHASH_ALLOCATOR_NEW
        IRTLASSERT(CLKRLinearHashTable::CLargeSegment::sm_palloc != NULL);
#endif
        // InterlockedIncrement(&g_cAllocLargeSegment);
        pseg = new CLargeSegment;
        break;
    }

    IRTLASSERT(pseg != NULL);

    if (pseg != NULL  &&  BucketLock::PerLockSpin() == LOCK_INDIVIDUAL_SPIN)
    {
        for (DWORD i = 0;  i < m_dwSegSize;  ++i)
            pseg->Slot(i).SetSpinCount(m_wBucketLockSpins);
    }

    return pseg;
} // CLKRLinearHashTable::_AllocateSegment



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_FreeSegment
// Synopsis: 
//------------------------------------------------------------------------

bool
CLKRLinearHashTable::_FreeSegment(
    CLKRLinearHashTable::CSegment* pseg) const
{
#ifdef LKR_OLD_SEGMENT
    delete pseg;

#else // !LKR_OLD_SEGMENT
    switch (m_lkts)
    {
    case LK_SMALL_TABLESIZE:
        delete static_cast<CSmallSegment*>(pseg);
        break;
        
    default:
        IRTLASSERT(FALSE);
        // fall-through
        
    case LK_MEDIUM_TABLESIZE:
        delete static_cast<CMediumSegment*>(pseg);
        break;
        
    case LK_LARGE_TABLESIZE:
        delete static_cast<CLargeSegment*>(pseg);
        break;
    }
#endif // !LKR_OLD_SEGMENT

    return true;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::_AllocateSubTableArray
// Synopsis: 
//------------------------------------------------------------------------

CLKRHashTable::SubTable** const
CLKRHashTable::_AllocateSubTableArray(
    size_t n)
{
#ifdef LKR_RANDOM_MEMORY_FAILURES
    if (rand() < LKR_RANDOM_MEMORY_FAILURES)
        return NULL;
#endif
    return new SubTable* [n];
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::_FreeSubTableArray
// Synopsis: 
//------------------------------------------------------------------------

bool
CLKRHashTable::_FreeSubTableArray(
    CLKRHashTable::SubTable** palht)
{
    delete [] palht;
    return true;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::_AllocateSubTable
// Synopsis: 
//------------------------------------------------------------------------

CLKRHashTable::SubTable* const
CLKRHashTable::_AllocateSubTable(
    LPCSTR          pszName,        // An identifier for debugging
    PFnExtractKey   pfnExtractKey,  // Extract key from record
    PFnCalcKeyHash  pfnCalcKeyHash, // Calculate hash signature of key
    PFnEqualKeys    pfnEqualKeys,   // Compare two keys
    PFnAddRefRecord pfnAddRefRecord,// AddRef in FindKey, etc
    double          maxload,        // Upperbound on average chain length
    DWORD           initsize,       // Initial size of hash table.
    CLKRHashTable*  phtParent       // Owning table.
    )
{
#ifdef LKR_RANDOM_MEMORY_FAILURES
    if (rand() < LKR_RANDOM_MEMORY_FAILURES)
        return NULL;
#endif
    return new SubTable(pszName, pfnExtractKey, pfnCalcKeyHash,
                        pfnEqualKeys,  pfnAddRefRecord,
                        maxload, initsize, phtParent);
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::_FreeSubTable
// Synopsis: 
//------------------------------------------------------------------------

bool
CLKRHashTable::_FreeSubTable(
    CLKRHashTable::SubTable* plht)
{
    delete plht;
    return true;
}




//-----------------------------------------------------------------------
// Function: CLKRLinearHashTable::_Expand
// Synopsis: Expands the table by one bucket. Done by splitting the
//           bucket pointed to by m_iExpansionIdx.
// Output:   LK_SUCCESS, if expansion was successful.
//           LK_ALLOC_FAIL, if expansion failed due to lack of memory.
//-----------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_Expand()
{
    if (m_cActiveBuckets >= MAX_DIRSIZE * m_dwSegSize - 1)
        return LK_ALLOC_FAIL;  // table is not allowed to grow any more

    WriteLock();

    // double segment directory size if necessary
    if (m_cActiveBuckets >= m_cDirSegs * m_dwSegSize)
    {
        IRTLASSERT(m_cDirSegs < MAX_DIRSIZE);
        DWORD cDirSegsNew = (m_cDirSegs == 0) ? MIN_DIRSIZE : m_cDirSegs << 1;
        CDirEntry* paDirSegsNew = _AllocateSegmentDirectory(cDirSegsNew);

        if (paDirSegsNew != NULL)
        {
            for (DWORD j = 0;  j < m_cDirSegs;  j++)
            {
                paDirSegsNew[j] = m_paDirSegs[j];
                m_paDirSegs[j].m_pseg = NULL;
            }

            _FreeSegmentDirectory(m_paDirSegs);
            m_paDirSegs = paDirSegsNew;
            m_cDirSegs  = cDirSegsNew;
        }
        else
        {
            WriteUnlock();
            return LK_ALLOC_FAIL;  // expansion failed
        }
    }

    // locate the new bucket, creating a new segment if necessary
    ++m_cActiveBuckets;

    DWORD     dwOldBkt = m_iExpansionIdx;
    DWORD     dwNewBkt = (1 << m_nLevel) | dwOldBkt;

    IRTLASSERT(dwOldBkt < m_cActiveBuckets);
    IRTLASSERT(dwNewBkt < m_cActiveBuckets);

    IRTLASSERT(_Segment(dwOldBkt) != NULL);
    CSegment* psegNew  = _Segment(dwNewBkt);

    if (psegNew == NULL)
    {
        psegNew = _AllocateSegment();
        if (psegNew == NULL)
        {
            --m_cActiveBuckets;
            WriteUnlock();
            return LK_ALLOC_FAIL;  // expansion failed
        }
        _Segment(dwNewBkt) = psegNew;
    }

    // prepare to relocate records to the new bucket
    CBucket* pbktOld = _Bucket(dwOldBkt);
    CBucket* pbktNew = _Bucket(dwNewBkt);

    // get locks on the two buckets involved
    pbktOld->WriteLock();
    pbktNew->WriteLock();

    // Now work out if we need to allocate any extra CNodeClumps.  We do
    // this up front, before calling _SplitRecordSet, as it's hard to
    // gracefully recover from the depths of that routine should we run
    // out of memory.

    CNodeClump* pncFreeList = NULL;
    LK_RETCODE  lkrc        = LK_SUCCESS;

    // If the old bucket has more than one CNodeClump, there's a chance that
    // we'll need extra CNodeClumps in the new bucket too.  If it doesn't,
    // we definitely won't. One CNodeClump is enough to prime the freelist.
    if (!pbktOld->m_ncFirst.IsLastClump())
    {
        pncFreeList = _AllocateNodeClump();
        if (pncFreeList == NULL)
        {
            lkrc = LK_ALLOC_FAIL;
            --m_cActiveBuckets;
        }
    }

    // adjust expansion pointer, level, and mask
    if (lkrc == LK_SUCCESS)
    {
        if (++m_iExpansionIdx == (1U << m_nLevel))
        {
            ++m_nLevel;
            m_iExpansionIdx = 0;
            m_dwBktAddrMask0 = (m_dwBktAddrMask0 << 1) | 1;
            // m_dwBktAddrMask0 = 00011..111
            IRTLASSERT((m_dwBktAddrMask0 & (m_dwBktAddrMask0+1)) == 0);
            m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;
            IRTLASSERT((m_dwBktAddrMask1 & (m_dwBktAddrMask1+1)) == 0);
        }
    }

    DWORD iExpansionIdx = m_iExpansionIdx;  // save to avoid race conditions
    DWORD dwBktAddrMask = m_dwBktAddrMask0; // ditto

    // Release the table lock before doing the actual relocation
    WriteUnlock();

    if (lkrc == LK_SUCCESS)
    {
        lkrc = _SplitRecordSet(&pbktOld->m_ncFirst, &pbktNew->m_ncFirst,
                               iExpansionIdx, dwBktAddrMask,
                               dwNewBkt, pncFreeList);
    }

    pbktNew->WriteUnlock();
    pbktOld->WriteUnlock();

    return lkrc;
} // CLKRLinearHashTable::_Expand



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_SplitRecordSet
// Synopsis: Split records between the old and new buckets.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_SplitRecordSet(
    CNodeClump* pncOldTarget,
    CNodeClump* pncNewTarget,
    DWORD       iExpansionIdx,
    DWORD       dwBktAddrMask,
    DWORD       dwNewBkt,
    CNodeClump* pncFreeList     // list of free nodes available for reuse
    )
{
    CNodeClump  ncFirst = *pncOldTarget;    // save head of old target chain
    CNodeClump* pncOldList = &ncFirst;
    CNodeClump* pncTmp;
    int         iOldSlot = NODE_BEGIN;
    int         iNewSlot = NODE_BEGIN;

    // clear target buckets
    pncOldTarget->Clear();
    pncNewTarget->Clear();

    // scan through the old bucket chain and decide where to move each record
    while (pncOldList != NULL)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            // node already empty?
            if (pncOldList->IsEmptySlot(i))
            {
                IRTLASSERT(pncOldList->IsEmptyAndInvalid(i));
                continue;
            }

            // calculate bucket address of this node
            DWORD dwBkt = _H0(pncOldList->m_dwKeySigs[i], dwBktAddrMask);
            if (dwBkt < iExpansionIdx)
                dwBkt = _H1(pncOldList->m_dwKeySigs[i], dwBktAddrMask);

            // record to be moved to the new address?
            if (dwBkt == dwNewBkt)
            {
                // node in new bucket chain full?
                if (iNewSlot == NODE_END)
                {
                    // the calling routine has passed in a FreeList adequate
                    // for all needs
                    IRTLASSERT(pncFreeList != NULL);
                    pncTmp = pncFreeList;
                    pncFreeList = pncFreeList->m_pncNext;
                    pncTmp->Clear();
                    pncNewTarget->m_pncNext = pncTmp;
                    pncNewTarget = pncTmp;
                    iNewSlot = NODE_BEGIN;
                }

                pncNewTarget->m_dwKeySigs[iNewSlot]
                    = pncOldList->m_dwKeySigs[i];
                pncNewTarget->m_pvNode[iNewSlot]
                    = pncOldList->m_pvNode[i];
                iNewSlot += NODE_STEP;
            }

            // no, record stays in its current bucket chain
            else
            {
                // node in old bucket chain full?
                if (iOldSlot == NODE_END)
                {
                    // the calling routine has passed in a FreeList adequate
                    // for all needs
                    IRTLASSERT(pncFreeList != NULL);
                    pncTmp = pncFreeList;
                    pncFreeList = pncFreeList->m_pncNext;
                    pncTmp->Clear();
                    pncOldTarget->m_pncNext = pncTmp;
                    pncOldTarget = pncTmp;
                    iOldSlot = NODE_BEGIN;
                }

                pncOldTarget->m_dwKeySigs[iOldSlot]
                    = pncOldList->m_dwKeySigs[i];
                pncOldTarget->m_pvNode[iOldSlot]
                    = pncOldList->m_pvNode[i];
                iOldSlot += NODE_STEP;
            }

            // clear old slot
            pncOldList->m_dwKeySigs[i] = HASH_INVALID_SIGNATURE;
            pncOldList->m_pvNode[i]    = NULL;
        }

        // keep walking down the original bucket chain
        pncTmp     = pncOldList;
        pncOldList = pncOldList->m_pncNext;

        // ncFirst is a stack variable, not allocated on the heap
        if (pncTmp != &ncFirst)
        {
            pncTmp->m_pncNext = pncFreeList;
            pncFreeList = pncTmp;
        }
    }

    // delete any leftover nodes
    while (pncFreeList != NULL)
    {
        pncTmp = pncFreeList;
        pncFreeList = pncFreeList->m_pncNext;
#ifdef _DEBUG
        pncTmp->m_pncNext = NULL; // or ~CNodeClump will ASSERT
#endif // _DEBUG
        _FreeNodeClump(pncTmp);
    }

#ifdef _DEBUG
    ncFirst.m_pncNext = NULL; // or ~CNodeClump will ASSERT
#endif // _DEBUG

    return LK_SUCCESS;
} // CLKRLinearHashTable::_SplitRecordSet



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_Contract
// Synopsis: Contract the table by deleting the last bucket in the active
//           address space. Return the records to the "buddy" of the
//           deleted bucket.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_Contract()
{
    WriteLock();

    // update the state variables (expansion ptr, level and mask)
    if (m_iExpansionIdx > 0)
        --m_iExpansionIdx;
    else
    {
        --m_nLevel;
        m_iExpansionIdx = (1 << m_nLevel) - 1;
        IRTLASSERT(m_nLevel > 0  &&  m_iExpansionIdx > 0);
        m_dwBktAddrMask0 >>= 1;
        IRTLASSERT((m_dwBktAddrMask0 & (m_dwBktAddrMask0+1)) == 0); // 00011..111
        m_dwBktAddrMask1 >>= 1;
        IRTLASSERT(m_dwBktAddrMask1 == ((m_dwBktAddrMask0 << 1) | 1));
        IRTLASSERT((m_dwBktAddrMask1 & (m_dwBktAddrMask1+1)) == 0);
    }

    // The last bucket is the one that will be emptied
    CBucket* pbktLast = _Bucket(m_cActiveBuckets - 1);
    pbktLast->WriteLock();

    // Decrement after calculating pbktLast, or _Bucket() will assert.
    --m_cActiveBuckets;

    // Where the nodes from pbktLast will end up
    CBucket* pbktNew = _Bucket(m_iExpansionIdx);
    pbktNew->WriteLock();

    // Now we work out if we need to allocate any extra CNodeClumps.  We do
    // this up front, before calling _MergeRecordSets, as it's hard to
    // gracefully recover from the depths of that routine should we run
    // out of memory.
    
    CNodeClump* pnc;
    int         c = 0;

    // First, count the number of items in the old bucket
    for (pnc = &pbktLast->m_ncFirst;  pnc != NULL;  pnc = pnc->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (!pnc->IsEmptySlot(i))
            {
                IRTLASSERT(!pnc->IsEmptyAndInvalid(i));
                c++;
            }
        }
    }

    // Then, subtract off the number of empty slots in the new bucket
    for (pnc = &pbktNew->m_ncFirst;  pnc != NULL;  pnc = pnc->m_pncNext)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (pnc->IsEmptySlot(i))
            {
                IRTLASSERT(pnc->IsEmptyAndInvalid(i));
                c--;
            }
        }
    }

    CNodeClump* pncFreeList = NULL;  // list of nodes available for reuse
    LK_RETCODE  lkrc        = LK_SUCCESS;

    // Do we need to allocate CNodeClumps to accommodate the surplus items?
    if (c > 0)
    {
        pncFreeList = _AllocateNodeClump();
        if (pncFreeList == NULL)
            lkrc = LK_ALLOC_FAIL;
        else if (c > NODES_PER_CLUMP)
        {
            // In the worst case, we need a 2-element freelist for
            // _MergeRecordSets. Two CNodeClumps always suffice since the
            // freelist will be augmented by the CNodeClumps from the old
            // bucket as they are processed.
            pnc = _AllocateNodeClump();
            if (pnc == NULL)
            {
                _FreeNodeClump(pncFreeList);
                lkrc = LK_ALLOC_FAIL;
            }
            else
                pncFreeList->m_pncNext = pnc;
        }
    }

    // Abort if we couldn't allocate enough CNodeClumps
    if (lkrc != LK_SUCCESS)
    {
        // undo the changes to the state variables
        if (++m_iExpansionIdx == (1U << m_nLevel))
        {
            ++m_nLevel;
            m_iExpansionIdx  = 0;
            m_dwBktAddrMask0 = (m_dwBktAddrMask0 << 1) | 1;
            m_dwBktAddrMask1 = (m_dwBktAddrMask0 << 1) | 1;
        }
        ++m_cActiveBuckets;

        // Unlock the buckets and the table
        pbktLast->WriteUnlock();
        pbktNew->WriteUnlock();
        WriteUnlock();

        return lkrc;
    }

    // Copy the chain of records from pbktLast
    CNodeClump ncOldFirst = pbktLast->m_ncFirst;

    // destroy pbktLast
    pbktLast->m_ncFirst.Clear();
    pbktLast->WriteUnlock();

    // remove segment, if empty
    if (_SegIndex(m_cActiveBuckets) == 0)
    {
#ifdef _DEBUG
        // double-check that the supposedly empty segment is really empty
        IRTLASSERT(_Segment(m_cActiveBuckets) != NULL);
        for (DWORD i = 0;  i < m_dwSegSize;  ++i)
        {
            CBucket* pbkt = &_Segment(m_cActiveBuckets)->Slot(i);
            IRTLASSERT(pbkt->IsWriteUnlocked()  &&  pbkt->IsReadUnlocked());
            IRTLASSERT(pbkt->m_ncFirst.IsLastClump());

            int j;

            FOR_EACH_NODE(j)
            {
                IRTLASSERT(pbkt->m_ncFirst.IsEmptyAndInvalid(j));
            }
        }
#endif // _DEBUG
        _FreeSegment(_Segment(m_cActiveBuckets));
        _Segment(m_cActiveBuckets) = NULL;
    }

    // reduce directory of segments if possible
    if (m_cActiveBuckets <= (m_cDirSegs * m_dwSegSize) >> 1
        &&  m_cDirSegs > MIN_DIRSIZE)
    {
        DWORD cDirSegsNew = m_cDirSegs >> 1;
        CDirEntry* paDirSegsNew = _AllocateSegmentDirectory(cDirSegsNew);

        // Memory allocation failure here does not require us to abort; it
        // just means that the directory of segments is larger than we'd like.
        if (paDirSegsNew != NULL)
        {
            for (DWORD j = 0;  j < cDirSegsNew;  j++)
                paDirSegsNew[j] = m_paDirSegs[j];
            for (j = 0;  j < m_cDirSegs;  j++)
                m_paDirSegs[j].m_pseg = NULL;

            _FreeSegmentDirectory(m_paDirSegs);
            m_paDirSegs = paDirSegsNew;
            m_cDirSegs  = cDirSegsNew;
        }
    }

    // release the table lock before doing the reorg
    WriteUnlock();

    lkrc = _MergeRecordSets(pbktNew, &ncOldFirst, pncFreeList);

    pbktNew->WriteUnlock();

#ifdef _DEBUG
    ncOldFirst.m_pncNext = NULL; // or ~CNodeClump will ASSERT
#endif // _DEBUG

    return lkrc;
} // CLKRLinearHashTable::_Contract



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_MergeRecordSets
// Synopsis: Merge two record sets.  Copy the contents of pncOldList
//           into pbktNewTarget.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_MergeRecordSets(
    CBucket*    pbktNewTarget,
    CNodeClump* pncOldList,
    CNodeClump* pncFreeList
    )
{
    IRTLASSERT(pbktNewTarget != NULL  &&  pncOldList != NULL);

    CNodeClump*   pncTmp = NULL;
    CNodeClump* const pncOldFirst = pncOldList;
    CNodeClump*   pncNewTarget = &pbktNewTarget->m_ncFirst;
    int           iNewSlot;

    // find the first nodeclump in the new target bucket with an empty slot
    while (!pncNewTarget->IsLastClump())
    {
        FOR_EACH_NODE(iNewSlot)
        {
            if (pncNewTarget->IsEmptySlot(iNewSlot))
                break;
        }

        if (iNewSlot == NODE_END)
            pncNewTarget = pncNewTarget->m_pncNext;
        else
            break;
    }

    IRTLASSERT(pncNewTarget != NULL);

    // find the first empty slot in pncNewTarget;
    // if none, iNewSlot == NODE_END
    FOR_EACH_NODE(iNewSlot)
    {
        if (pncNewTarget->IsEmptySlot(iNewSlot))
        {
            break;
        }
    }
    
    while (pncOldList != NULL)
    {
        int i;

        FOR_EACH_NODE(i)
        {
            if (!pncOldList->IsEmptySlot(i))
            {
                // any empty slots left in pncNewTarget?
                if (iNewSlot == NODE_END)
                {
                    // no, so walk down pncNewTarget until we find another
                    // empty slot
                    while (!pncNewTarget->IsLastClump())
                    {
                        pncNewTarget = pncNewTarget->m_pncNext;

                        FOR_EACH_NODE(iNewSlot)
                        {
                            if (pncNewTarget->IsEmptySlot(iNewSlot))
                                goto found_slot;
                        }
                    }

                    // Oops, reached the last nodeclump in pncNewTarget
                    // and it's full.  Get a new nodeclump off the free
                    // list, which is big enough to handle all needs.
                    IRTLASSERT(pncNewTarget != NULL);
                    IRTLASSERT(pncFreeList != NULL);
                    pncTmp = pncFreeList;
                    pncFreeList = pncFreeList->m_pncNext;
                    pncTmp->Clear();
                    pncNewTarget->m_pncNext = pncTmp;
                    pncNewTarget = pncTmp;
                    iNewSlot = NODE_BEGIN;
                }

              found_slot:
                // We have an empty slot in pncNewTarget
                IRTLASSERT(0 <= iNewSlot  &&  iNewSlot < NODES_PER_CLUMP
                       &&  pncNewTarget != NULL
                       &&  pncNewTarget->IsEmptyAndInvalid(iNewSlot));

                // Let's copy the node from pncOldList
                pncNewTarget->m_dwKeySigs[iNewSlot]
                    = pncOldList->m_dwKeySigs[i];
                pncNewTarget->m_pvNode[iNewSlot]
                    = pncOldList->m_pvNode[i];

                // Clear old slot
                pncOldList->m_dwKeySigs[i] = HASH_INVALID_SIGNATURE;
                pncOldList->m_pvNode[i]    = NULL;

                // find the next free slot in pncNewTarget
                while ((iNewSlot += NODE_STEP) != NODE_END)
                {
                    if (pncNewTarget->IsEmptySlot(iNewSlot))
                    {
                        break;
                    }
                }
            }
            else // iNewSlot != NODE_END
            {
                IRTLASSERT(pncOldList->IsEmptyAndInvalid(i));
            }
        }

        // Move into the next nodeclump in pncOldList
        pncTmp = pncOldList;
        pncOldList = pncOldList->m_pncNext;

        // Append to the free list.  Don't put the first node of
        // pncOldList on the free list, as it's a stack variable.
        if (pncTmp != pncOldFirst)
        {
            pncTmp->m_pncNext = pncFreeList;
            pncFreeList = pncTmp;
        }
    }

    // delete any leftover nodes
    while (pncFreeList != NULL)
    {
        pncTmp = pncFreeList;
        pncFreeList = pncFreeList->m_pncNext;
#ifdef _DEBUG
        pncTmp->m_pncNext = NULL; // or ~CNodeClump will ASSERT
#endif // _DEBUG
        _FreeNodeClump(pncTmp);
    }

    return LK_SUCCESS;
} // CLKRLinearHashTable::_MergeRecordSets



#ifdef LKR_DEPRECATED_ITERATORS

//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_InitializeIterator
// Synopsis: Make the iterator point to the first record in the hash table.
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_InitializeIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL);
    IRTLASSERT(piter->m_lkl == LKL_WRITELOCK
               ?  IsWriteLocked()
               :  IsReadLocked());
    if (piter == NULL  ||  piter->m_plht != NULL)
        return LK_BAD_ITERATOR;

    piter->m_plht = this;
    piter->m_dwBucketAddr = 0;

    CBucket* pbkt = _Bucket(piter->m_dwBucketAddr);
    IRTLASSERT(pbkt != NULL);
    if (piter->m_lkl == LKL_WRITELOCK)
        pbkt->WriteLock();
    else
        pbkt->ReadLock();

    piter->m_pnc = &pbkt->m_ncFirst;
    piter->m_iNode = NODE_BEGIN - NODE_STEP;

    // Let IncrementIterator do the hard work of finding the first
    // slot in use.
    return IncrementIterator(piter);
} // CLKRLinearHashTable::_InitializeIterator



//------------------------------------------------------------------------
// Function: CLKRHashTable::InitializeIterator
// Synopsis: make the iterator point to the first record in the hash table
//------------------------------------------------------------------------

LK_RETCODE
CLKRHashTable::InitializeIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL  &&  piter->m_pht == NULL);
    if (piter == NULL  ||  piter->m_pht != NULL)
        return LK_BAD_ITERATOR;

    // First, lock all the subtables
    if (piter->m_lkl == LKL_WRITELOCK)
        WriteLock();
    else
        ReadLock();

    // Must call IsValid inside a lock to ensure that none of the state
    // variables change while it's being evaluated
    IRTLASSERT(IsValid());
    if (!IsValid())
        return LK_UNUSABLE;

    piter->m_pht  = this;
    piter->m_ist  = -1;
    piter->m_plht = NULL;

    // Let IncrementIterator do the hard work of finding the first
    // valid node in the subtables.
    return IncrementIterator(piter);
} // CLKRHashTable::InitializeIterator



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::IncrementIterator
// Synopsis: move the iterator on to the next record in the hash table
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::IncrementIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL);
    IRTLASSERT(piter->m_plht == this);
    IRTLASSERT(piter->m_lkl == LKL_WRITELOCK
               ?  IsWriteLocked()
               :  IsReadLocked());
    IRTLASSERT(piter->m_dwBucketAddr < m_cActiveBuckets);
    IRTLASSERT(piter->m_pnc != NULL);
    IRTLASSERT((0 <= piter->m_iNode  &&  piter->m_iNode < NODES_PER_CLUMP)
               || (NODE_BEGIN - NODE_STEP == piter->m_iNode));

    if (piter == NULL  ||  piter->m_plht != this)
        return LK_BAD_ITERATOR;

    const void* pvRecord = NULL;

    if (piter->m_iNode != NODE_BEGIN - NODE_STEP)
    {
        // Release the reference acquired in the previous call to
        // IncrementIterator
        pvRecord = piter->m_pnc->m_pvNode[piter->m_iNode];
        _AddRefRecord(pvRecord, -1);
    }

    do
    {
        do
        {
            // find the next slot in the nodeclump that's in use
            while ((piter->m_iNode += NODE_STEP) != NODE_END)
            {
                pvRecord = piter->m_pnc->m_pvNode[piter->m_iNode];
                if (pvRecord != NULL)
                {
                    // Add a new reference
                    _AddRefRecord(pvRecord, +1);
                    return LK_SUCCESS;
                }
            }

            // try the next nodeclump in the bucket chain
            piter->m_iNode = NODE_BEGIN - NODE_STEP;
            piter->m_pnc = piter->m_pnc->m_pncNext;
        } while (piter->m_pnc != NULL);

        // Exhausted this bucket chain.  Unlock it.
        CBucket* pbkt = _Bucket(piter->m_dwBucketAddr);
        IRTLASSERT(pbkt != NULL);
        IRTLASSERT(piter->m_lkl == LKL_WRITELOCK
                   ?  pbkt->IsWriteLocked()
                   :  pbkt->IsReadLocked());
        if (piter->m_lkl == LKL_WRITELOCK)
            pbkt->WriteUnlock();
        else
            pbkt->ReadUnlock();

        // Try the next bucket, if there is one
        if (++piter->m_dwBucketAddr < m_cActiveBuckets)
        {
            pbkt = _Bucket(piter->m_dwBucketAddr);
            IRTLASSERT(pbkt != NULL);
            if (piter->m_lkl == LKL_WRITELOCK)
                pbkt->WriteLock();
            else
                pbkt->ReadLock();
            piter->m_pnc = &pbkt->m_ncFirst;
        }
    } while (piter->m_dwBucketAddr < m_cActiveBuckets);

    // We have fallen off the end of the hashtable
    piter->m_iNode = NODE_BEGIN - NODE_STEP;
    piter->m_pnc = NULL;

    return LK_NO_MORE_ELEMENTS;
} // CLKRLinearHashTable::IncrementIterator



//------------------------------------------------------------------------
// Function: CLKRHashTable::IncrementIterator
// Synopsis: move the iterator on to the next record in the hash table
//------------------------------------------------------------------------

LK_RETCODE
CLKRHashTable::IncrementIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL);
    IRTLASSERT(piter->m_pht == this);
    IRTLASSERT(-1 <= piter->m_ist
               &&  piter->m_ist < static_cast<int>(m_cSubTables));

    if (piter == NULL  ||  piter->m_pht != this)
        return LK_BAD_ITERATOR;

    // Table is already locked
    if (!IsValid())
        return LK_UNUSABLE;

    LK_RETCODE lkrc;
    CLHTIterator* pBaseIter = static_cast<CLHTIterator*>(piter);

    for (;;)
    {
        // Do we have a valid iterator into a subtable?  If not, get one.
        while (piter->m_plht == NULL)
        {
            while (++piter->m_ist < static_cast<int>(m_cSubTables))
            {
                lkrc = m_palhtDir[piter->m_ist]->_InitializeIterator(piter);
                if (lkrc == LK_SUCCESS)
                {
                    IRTLASSERT(m_palhtDir[piter->m_ist] == piter->m_plht);
                    return lkrc;
                }
                else if (lkrc == LK_NO_MORE_ELEMENTS)
                    lkrc = piter->m_plht->_CloseIterator(pBaseIter);

                if (lkrc != LK_SUCCESS)
                    return lkrc;
            }

            // There are no more subtables left.
            return LK_NO_MORE_ELEMENTS;
        }

        // We already have a valid iterator into a subtable.  Increment it.
        lkrc = piter->m_plht->IncrementIterator(pBaseIter);
        if (lkrc == LK_SUCCESS)
            return lkrc;

        // We've exhausted that subtable.  Move on.
        if (lkrc == LK_NO_MORE_ELEMENTS)
            lkrc = piter->m_plht->_CloseIterator(pBaseIter);

        if (lkrc != LK_SUCCESS)
            return lkrc;
    }
} // CLKRHashTable::IncrementIterator



//------------------------------------------------------------------------
// Function: CLKRLinearHashTable::_CloseIterator
// Synopsis: release the resources held by the iterator
//------------------------------------------------------------------------

LK_RETCODE
CLKRLinearHashTable::_CloseIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL);
    IRTLASSERT(piter->m_plht == this);
    IRTLASSERT(piter->m_lkl == LKL_WRITELOCK
               ?  IsWriteLocked()
               :  IsReadLocked());
    IRTLASSERT(piter->m_dwBucketAddr <= m_cActiveBuckets);
    IRTLASSERT((0 <= piter->m_iNode  &&  piter->m_iNode < NODES_PER_CLUMP)
               || (NODE_BEGIN - NODE_STEP == piter->m_iNode));

    if (piter == NULL  ||  piter->m_plht != this)
        return LK_BAD_ITERATOR;

    // Are we abandoning the iterator before the end of the table?
    // If so, need to unlock the bucket.
    if (piter->m_dwBucketAddr < m_cActiveBuckets)
    {
        CBucket* pbkt = _Bucket(piter->m_dwBucketAddr);
        IRTLASSERT(pbkt != NULL);
        IRTLASSERT(piter->m_lkl == LKL_WRITELOCK
                   ?  pbkt->IsWriteLocked()
                   :  pbkt->IsReadLocked());
        if (0 <= piter->m_iNode  &&  piter->m_iNode < NODES_PER_CLUMP)
        {
            IRTLASSERT(piter->m_pnc != NULL);
            const void* pvRecord = piter->m_pnc->m_pvNode[piter->m_iNode];
            _AddRefRecord(pvRecord, -1);
        }
        if (piter->m_lkl == LKL_WRITELOCK)
            pbkt->WriteUnlock();
        else
            pbkt->ReadUnlock();
    }

    piter->m_plht = NULL;
    piter->m_pnc  = NULL;

    return LK_SUCCESS;
} // CLKRLinearHashTable::_CloseIterator



//------------------------------------------------------------------------
// Function: CLKRHashTable::CloseIterator
// Synopsis: release the resources held by the iterator
//------------------------------------------------------------------------

LK_RETCODE
CLKRHashTable::CloseIterator(
    CIterator* piter)
{
    if (!IsUsable())
        return LK_UNUSABLE;

    IRTLASSERT(piter != NULL);
    IRTLASSERT(piter->m_pht == this);
    IRTLASSERT(-1 <= piter->m_ist
               &&  piter->m_ist <= static_cast<int>(m_cSubTables));

    if (piter == NULL  ||  piter->m_pht != this)
        return LK_BAD_ITERATOR;

    LK_RETCODE lkrc = LK_SUCCESS;

    if (!IsValid())
        lkrc = LK_UNUSABLE;
    else
    {
        // Are we abandoning the iterator before we've reached the end?
        // If so, close the subtable iterator.
        if (piter->m_plht != NULL)
        {
            IRTLASSERT(piter->m_ist < static_cast<int>(m_cSubTables));
            CLHTIterator* pBaseIter = static_cast<CLHTIterator*>(piter);
            piter->m_plht->_CloseIterator(pBaseIter);
        }
    }

    // Unlock all the subtables
    if (piter->m_lkl == LKL_WRITELOCK)
        WriteUnlock();
    else
        ReadUnlock();

    piter->m_plht = NULL;
    piter->m_pht  = NULL;
    piter->m_ist  = -1;

    return lkrc;
} // CLKRHashTable::CloseIterator

#endif // LKR_DEPRECATED_ITERATORS



//------------------------------------------------------------------------
// Function: CLKRHashTable::WriteLock
// Synopsis: Lock all subtables for writing
//------------------------------------------------------------------------

void
CLKRHashTable::WriteLock()
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        m_palhtDir[i]->WriteLock();
        IRTLASSERT(m_palhtDir[i]->IsWriteLocked());
    }
} // CLKRHashTable::WriteLock



//------------------------------------------------------------------------
// Function: CLKRHashTable::ReadLock
// Synopsis: Lock all subtables for reading
//------------------------------------------------------------------------

void
CLKRHashTable::ReadLock() const
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        m_palhtDir[i]->ReadLock();
        IRTLASSERT(m_palhtDir[i]->IsReadLocked());
    }
} // CLKRHashTable::ReadLock



//------------------------------------------------------------------------
// Function: CLKRHashTable::WriteUnlock
// Synopsis: Unlock all subtables
//------------------------------------------------------------------------

void
CLKRHashTable::WriteUnlock() const
{
    // unlock in reverse order: LIFO
    for (DWORD i = m_cSubTables;  i-- > 0;  )
    {
        IRTLASSERT(m_palhtDir[i]->IsWriteLocked());
        m_palhtDir[i]->WriteUnlock();
        IRTLASSERT(m_palhtDir[i]->IsWriteUnlocked());
    }
} // CLKRHashTable::WriteUnlock



//------------------------------------------------------------------------
// Function: CLKRHashTable::ReadUnlock
// Synopsis: Unlock all subtables
//------------------------------------------------------------------------

void
CLKRHashTable::ReadUnlock() const
{
    // unlock in reverse order: LIFO
    for (DWORD i = m_cSubTables;  i-- > 0;  )
    {
        IRTLASSERT(m_palhtDir[i]->IsReadLocked());
        m_palhtDir[i]->ReadUnlock();
        IRTLASSERT(m_palhtDir[i]->IsReadUnlocked());
    }
} // CLKRHashTable::ReadUnlock



//------------------------------------------------------------------------
// Function: CLKRHashTable::IsWriteLocked
// Synopsis: Are all subtables write-locked?
//------------------------------------------------------------------------

bool
CLKRHashTable::IsWriteLocked() const
{
    bool fLocked = (m_cSubTables > 0);
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        fLocked = fLocked && m_palhtDir[i]->IsWriteLocked();
    }
    return fLocked;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::IsReadLocked
// Synopsis: Are all subtables read-locked?
//------------------------------------------------------------------------

bool
CLKRHashTable::IsReadLocked() const
{
    bool fLocked = (m_cSubTables > 0);
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        fLocked = fLocked && m_palhtDir[i]->IsReadLocked();
    }
    return fLocked;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::IsWriteUnlocked
// Synopsis: Are all subtables write-unlocked?
//------------------------------------------------------------------------

bool
CLKRHashTable::IsWriteUnlocked() const
{
    bool fUnlocked = (m_cSubTables > 0);
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        fUnlocked = fUnlocked && m_palhtDir[i]->IsWriteUnlocked();
    }
    return fUnlocked;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::IsReadUnlocked
// Synopsis: Are all subtables read-unlocked?
//------------------------------------------------------------------------

bool
CLKRHashTable::IsReadUnlocked() const
{
    bool fUnlocked = (m_cSubTables > 0);
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        fUnlocked = fUnlocked && m_palhtDir[i]->IsReadUnlocked();
    }
    return fUnlocked;
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::ConvertSharedToExclusive
// Synopsis: Convert the read lock to a write lock
//------------------------------------------------------------------------

void
CLKRHashTable::ConvertSharedToExclusive() const
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        m_palhtDir[i]->ConvertSharedToExclusive();
        IRTLASSERT(m_palhtDir[i]->IsWriteLocked());
    }
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::ConvertExclusiveToShared
// Synopsis: Convert the write lock to a read lock
//------------------------------------------------------------------------

void
CLKRHashTable::ConvertExclusiveToShared() const
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
    {
        m_palhtDir[i]->ConvertExclusiveToShared();
        IRTLASSERT(m_palhtDir[i]->IsReadLocked());
    }
}



//------------------------------------------------------------------------
// Function: CLKRHashTable::Size
// Synopsis: Number of elements in the table
//------------------------------------------------------------------------

DWORD
CLKRHashTable::Size() const
{
    DWORD cSize = 0;

    for (DWORD i = 0;  i < m_cSubTables;  i++)
        cSize += m_palhtDir[i]->Size();

    return cSize;
} // CLKRHashTable::Size



//------------------------------------------------------------------------
// Function: CLKRHashTable::MaxSize
// Synopsis: Maximum possible number of elements in the table
//------------------------------------------------------------------------

DWORD
CLKRHashTable::MaxSize() const
{
    return (m_cSubTables == 0)  ? 0  : m_cSubTables * m_palhtDir[0]->MaxSize();
} // CLKRHashTable::MaxSize



//------------------------------------------------------------------------
// Function: CLKRHashTable::IsValid
// Synopsis: is the table valid?
//------------------------------------------------------------------------

bool
CLKRHashTable::IsValid() const
{
    bool f = (m_lkrcState == LK_SUCCESS     // serious internal failure?
              &&  (m_palhtDir != NULL  &&  m_cSubTables > 0)
              &&  ValidSignature());

    for (DWORD i = 0;  f  &&  i < m_cSubTables;  i++)
        f = f && m_palhtDir[i]->IsValid();

    return f;
} // CLKRHashTable::IsValid



//------------------------------------------------------------------------
// Function: CLKRHashTable::SetBucketLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

void
CLKRLinearHashTable::SetBucketLockSpinCount(
    WORD wSpins)
{
    m_wBucketLockSpins = wSpins;

    if (BucketLock::PerLockSpin() != LOCK_INDIVIDUAL_SPIN)
        return;
    
    for (DWORD i = 0;  i < m_cDirSegs;  i++)
    {
        CSegment* pseg = m_paDirSegs[i].m_pseg;

        if (pseg != NULL)
        {
            for (DWORD j = 0;  j < m_dwSegSize;  ++j)
            {
                pseg->Slot(j).SetSpinCount(wSpins);
            }
        }
    }
} // CLKRLinearHashTable::SetBucketLockSpinCount



//------------------------------------------------------------------------
// Function: CLKRHashTable::SetBucketLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

WORD
CLKRLinearHashTable::GetBucketLockSpinCount()
{
    return m_wBucketLockSpins;
} // CLKRLinearHashTable::GetBucketLockSpinCount



//------------------------------------------------------------------------
// Function: CLKRHashTable::SetTableLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

void
CLKRHashTable::SetTableLockSpinCount(
    WORD wSpins)
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
        m_palhtDir[i]->SetTableLockSpinCount(wSpins);
} // CLKRHashTable::SetTableLockSpinCount



//------------------------------------------------------------------------
// Function: CLKRHashTable::GetTableLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

WORD
CLKRHashTable::GetTableLockSpinCount()
{
    return ((m_cSubTables == 0)
            ?  LOCK_DEFAULT_SPINS
            :  m_palhtDir[0]->GetTableLockSpinCount());
} // CLKRHashTable::GetTableLockSpinCount



//------------------------------------------------------------------------
// Function: CLKRHashTable::SetBucketLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

void
CLKRHashTable::SetBucketLockSpinCount(
    WORD wSpins)
{
    for (DWORD i = 0;  i < m_cSubTables;  i++)
        m_palhtDir[i]->SetBucketLockSpinCount(wSpins);
} // CLKRHashTable::SetBucketLockSpinCount



//------------------------------------------------------------------------
// Function: CLKRHashTable::GetBucketLockSpinCount
// Synopsis: 
//------------------------------------------------------------------------

WORD
CLKRHashTable::GetBucketLockSpinCount()
{
    return ((m_cSubTables == 0)
            ?  LOCK_DEFAULT_SPINS
            :  m_palhtDir[0]->GetBucketLockSpinCount());
} // CLKRHashTable::GetBucketLockSpinCount



#ifdef __LKRHASH_NAMESPACE__
}
#endif // __LKRHASH_NAMESPACE__
