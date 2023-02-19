// HashSHA1.cpp -- definition of CHashSHA1

// (c) Copyright Schlumberger Technology Corp., unpublished work, created
// 1998. This computer program includes Confidential, Proprietary
// Information and is a Trade Secret of Schlumberger Technology Corp. All
// use, disclosure, and/or reproduction is prohibited unless authorized
// in writing.  All Rights Reserved.

#include "stdafx.h"  // because handles.h uses the ASSERT macro

#include <memory>                                 // for auto_ptr

#include "HashSHA1.h"

using namespace std;

/////////////////////////// LOCAL/HELPER  /////////////////////////////////

///////////////////////////    PUBLIC     /////////////////////////////////

                                                  // Types
                                                  // C'tors/D'tors

CHashSHA1::CHashSHA1(CryptContext const &rcryptctx)
    : CHashContext(rcryptctx, CALG_SHA)
{}

CHashSHA1::~CHashSHA1() throw()
{}

                                                  // Operators
                                                  // Operations

auto_ptr<CHashContext>
CHashSHA1::Clone(DWORD const *pdwReserved,
                 DWORD dwFlags) const
{
    return auto_ptr<CHashContext>(new CHashSHA1(*this, pdwReserved,
                                                dwFlags));
}

                                                  // Access
                                                  // Predicates
                                                  // Static Variables

///////////////////////////   PROTECTED   /////////////////////////////////

                                                  // C'tors/D'tors

// Duplicate the hash and its state
CHashSHA1::CHashSHA1(CHashSHA1 const &rhs,
                     DWORD const *pdwReserved,
                     DWORD dwFlags)
    : CHashContext(rhs, pdwReserved, dwFlags)
{}

                                                  // Operators
                                                  // Operations
                                                  // Access

Blob
CHashSHA1::EncodedAlgorithmOid()
{
    // SHA1 Algorithm Object IDentifier (OID) in ASN.1 format (little endian)
    static unsigned char const sSha1AlgOid[] =
    {
        '\x30', '\x21', '\x30', '\x09', '\x06',
        '\x05', '\x2b', '\x0e', '\x03', '\x02',
        '\x1a', '\x05', '\x00', '\x04', '\x14'
    };

    return Blob(sSha1AlgOid, sizeof sSha1AlgOid / sizeof *sSha1AlgOid);
}


                                                  // Predicates
                                                  // Static Variables


///////////////////////////    PRIVATE    /////////////////////////////////

                                                  // C'tors/D'tors
                                                  // Operators
                                                  // Operations
                                                  // Access
                                                  // Predicates
                                                  // Static Variables
