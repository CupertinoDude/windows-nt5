// APriKey.cpp -- CAbstractPrivateKey implementation

// (c) Copyright Schlumberger Technology Corp., unpublished work, created
// 1999. This computer program includes Confidential, Proprietary
// Information and is a Trade Secret of Schlumberger Technology Corp. All
// use, disclosure, and/or reproduction is prohibited unless authorized
// in writing.  All Rights Reserved.

#include <algorithm>

#include "cciPriKey.h"
#include "cciCont.h"
#include "AKeyPair.h"
#include "TransactionWrap.h"
#include "AContHelp.h"

using namespace std;
using namespace cci;

/////////////////////////// LOCAL/HELPER  /////////////////////////////////


///////////////////////////    PUBLIC     /////////////////////////////////

                                                  // Types
                                                  // C'tors/D'tors
CAbstractPrivateKey::~CAbstractPrivateKey()
{}

                                                  // Operators
bool
CAbstractPrivateKey::operator==(CAbstractPrivateKey const &rhs) const
{
    CTransactionWrap wrap(m_hcard);
    CTransactionWrap rhswrap(rhs.m_hcard);

    return CProtectableCrypt::operator==(rhs) &&
        DoEquals(rhs);
}

bool
CAbstractPrivateKey::operator!=(CAbstractPrivateKey const &rhs) const
{
    return !(*this == rhs);
}

                                                 // Operations
void
CAbstractPrivateKey::Delete()
{
    CTransactionWrap wrap(m_hcard);

    // Remove any references to this object from the container
    vector<CContainer> vCont(m_hcard->EnumContainers());
    for_each(vCont.begin(), vCont.end(),
             EraseFromContainer<CPrivateKey, CAbstractKeyPair>(CPrivateKey(this),
                                                               CAbstractKeyPair::PrivateKey,
                                                               CAbstractKeyPair::PrivateKey));

    DoDelete();
}

void
CAbstractPrivateKey::Value(CPrivateKeyBlob const &rblob)
{
    CTransactionWrap wrap(m_hcard);

    DoWriteKey(rblob);
}

                                                  // Access
                                                  // Predicates
                                                  // Static Variables

///////////////////////////   PROTECTED   /////////////////////////////////

                                                  // C'tors/D'tors
CAbstractPrivateKey::CAbstractPrivateKey(CAbstractCard const &racard,
                                         ObjectAccess oa)
    : slbRefCnt::RCObject(),
      CProtectableCrypt(racard, oa)
{}
                                                  // Operators
                                                  // Operations
                                                  // Access
                                                  // Predicates
                                                  // Static Variables


///////////////////////////    PRIVATE    /////////////////////////////////

                                                  // C'tors/D'tors
                                                  // Operators
                                                  // Operations
                                                  // Access
                                                  // Predicates
                                                  // Static Variables
