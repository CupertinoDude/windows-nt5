// V2PriKey.h: interface for the CV2PrivateKey class.
//
// (c) Copyright Schlumberger Technology Corp., unpublished work, created
// 1999. This computer program includes Confidential, Proprietary
// Information and is a Trade Secret of Schlumberger Technology Corp. All
// use, disclosure, and/or reproduction is prohibited unless authorized
// in writing.  All Rights Reserved.
//////////////////////////////////////////////////////////////////////

// Note:  This file should only be included by the CCI, not directly
// by the client.

#if !defined(SLBCCI_V2PRIKEY_H)
#define SLBCCI_V2PRIKEY_H

#include <string>
#include <vector>
#include <memory>                                 // for auto_ptr

#include <slbRCObj.h>

#include "APriKey.h"

namespace cci
{

class CV2Card;
class CPriKeyInfoRecord;

class CV2PrivateKey
    : public CAbstractPrivateKey
{
public:
                                                  // Types
                                                  // C'tors/D'tors

    CV2PrivateKey(CV2Card const &rv2card,
                  ObjectAccess oa);

    CV2PrivateKey(CV2Card const &rv2card,
                  SymbolID sidHandle,
                  ObjectAccess oa);

    CV2PrivateKey(CV2Card const &rv2card,
                  BYTE bKeyType,
                  BYTE bKeyNumber,
                  ObjectAccess oa);

    virtual
    ~CV2PrivateKey() throw();

                                                  // Operators
                                                  // Operations

    virtual void
    CredentialID(std::string const &rstrID);

    virtual void
    Decrypt(bool flag);

    virtual void
    Derive(bool flag);

    virtual void
    EndDate(Date const &rEndDate);

    virtual void
    Exportable(bool flag);

    virtual void
    ID(std::string const &rstrID);

    virtual std::string
    InternalAuth(std::string const &rstrOld);

    virtual void
    Label(std::string const &rstrLabel);

    virtual void
    Local(bool flag);

    static CV2PrivateKey *
    Make(CV2Card const &rv2card,
         SymbolID sidHandle,
         ObjectAccess oa);

    virtual void
    Modifiable(bool flag);

    virtual void
    Modulus(std::string const &rstrModulus);

    virtual void
    NeverExportable(bool flag);

    virtual void
    NeverRead(bool flag);

    virtual void
    PublicExponent(std::string const &rstrExponent);

    virtual void
    Read(bool flag);

    virtual void
    Sign(bool flag);

    virtual void
    SignRecover(bool flag);

    virtual void
    StartDate(Date &rdtStart);

    virtual void
    Subject(std::string const &rstrSubject);

    virtual void
    Unwrap(bool flag);

                                                  // Access

    virtual std::string
    CredentialID();

    virtual bool
    Decrypt();

    virtual bool
    Derive();

    virtual Date
    EndDate();

    virtual bool
    Exportable();

    SymbolID
    Handle() const;

    virtual std::string
    ID();

    virtual std::string
    Label();

    virtual bool
    Local();

    virtual bool
    Modifiable();

    virtual std::string
    Modulus();

    virtual bool
    NeverExportable();

    virtual bool
    NeverRead();

    virtual bool
    Private();

    virtual std::string
    PublicExponent();

    virtual bool
    Read();

    virtual bool
    Sign();

    virtual bool
    SignRecover();

    virtual Date
    StartDate();

    virtual std::string
    Subject();

    virtual bool
    Unwrap();


                                                  // Predicates

protected:
                                                  // Types
                                                  // C'tors/D'tors
                                                  // Operators
                                                  // Operations

    virtual void
    DoDelete();

    virtual void
    DoWriteKey(CPrivateKeyBlob const &rblob);

                                                  // Access
                                                  // Predicates

    virtual bool
    DoEquals(CAbstractPrivateKey const &rhs) const;

                                                  // Variables

private:
                                                  // Types
                                                  // C'tors/D'tors

    CV2PrivateKey(CAbstractPrivateKey const &rhs);
        // not defined, copying not allowed.

                                                  // Operators

    CAbstractPrivateKey &
    operator=(CAbstractPrivateKey const &rhs);
        // not defined, initialization not allowed.

                                                  // Operations

    void
    Setup(CV2Card const &rv2card);

    void
    Setup(CV2Card const &rv2card,
          SymbolID sidHandle);

                                                  // Access
                                                  // Predicates
                                                  // Variables
    SymbolID m_sidHandle;
    std::auto_ptr<CPriKeyInfoRecord> m_apcir;
};

} // namespace

#endif // !defined(SLBCCI_V2PRIKEY_H)
