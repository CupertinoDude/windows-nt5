//---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:  cuas.cxx
//
//  Contents:  User Object Account Statistics FunctionalSet
//
//  History:   11-1-95     krishnag    Created.
//              8-5-96     ramv        Modified to be consistent with spec
//
//
//    PROPERTY_RO(AccountExpiration, DATE, 1)       Implemented
//    PROPERTY_RO(BadLoginAddress, BSTR, 2)         NI
//    PROPERTY_RO(BadLoginCount, long, 3)           NI
//    PROPERTY_RO(BadPasswordAttempts, long, 4)     Implemented
//    PROPERTY_RO(LastLogin, DATE, 5)               Implemented
//    PROPERTY_RO(LastLogoff, DATE, 6)              Implemented
//    PROPERTY_RO(LastFailedLogin, DATE, 7)         NI
//    PROPERTY_RO(PasswordLastChanged, DATE, 8)     Implemented
//
//----------------------------------------------------------------------------
#include "nds.hxx"
#pragma hdrstop


//  Class CNDSUser

STDMETHODIMP CNDSUser::get_BadLoginAddress(THIS_ BSTR FAR* retval)
{
    GET_PROPERTY_BSTR((IADsUser *)this, BadLoginAddress);
}

STDMETHODIMP CNDSUser::get_BadLoginCount(THIS_ long FAR* retval)
{
    GET_PROPERTY_LONG((IADsUser *)this, BadLoginCount);
}

STDMETHODIMP CNDSUser::get_LastLogin(THIS_ DATE FAR* retval)
{
    GET_PROPERTY_DATE((IADsUser *)this, LastLogin);
}

STDMETHODIMP CNDSUser::get_LastLogoff(THIS_ DATE FAR* retval)
{
    GET_PROPERTY_DATE((IADsUser *)this, LastLogoff);
}

STDMETHODIMP CNDSUser::get_LastFailedLogin(THIS_ DATE FAR* retval)
{
    GET_PROPERTY_DATE((IADsUser *)this, LastFailedLogin);
}

STDMETHODIMP CNDSUser::get_PasswordLastChanged(THIS_ DATE FAR* retval)
{
    GET_PROPERTY_DATE((IADsUser *)this, PasswordLastChanged);
}

