//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 1999-2000 Microsoft Corporation
//
//  Module Name:
//      CClusterNetInterface.h
//
//  Implementation File:
//      CClusterNetInterface.cpp
//
//  Description:
//      Definition of the CClusterNetInterface class.
//
//  Author:
//      Henry Wang (HenryWa)    24-AUG-1999
//
//  Notes:
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////
//  Include Files
//////////////////////////////////////////////////////////////////////////////

#include "ProvBase.h"

//////////////////////////////////////////////////////////////////////////////
//  Forward Declarations
//////////////////////////////////////////////////////////////////////////////

class CClusterNetInterface;

//////////////////////////////////////////////////////////////////////////////
//++
//
//  class CClusterNetInterface
//
//  Description:
//      Provider Implement for cluster Node
//
//--
//////////////////////////////////////////////////////////////////////////////
class CClusterNetInterface : public CProvBase
{
//
// constructor
//
public:
    CClusterNetInterface::CClusterNetInterface(
        LPCWSTR         pwszNameIn,
        CWbemServices * pNamespaceIn
        );

//
// methods
//
public:

    virtual SCODE EnumInstance(
        long                 lFlagsIn,
        IWbemContext *       pCtxIn,
        IWbemObjectSink *    pHandlerIn
        );

    virtual SCODE GetObject(
        CObjPath &           rObjPathIn,
        long                 lFlagsIn,
        IWbemContext *       pCtxIn,
        IWbemObjectSink *    pHandlerIn 
        );

    virtual SCODE ExecuteMethod(
        CObjPath &           rObjPathIn,
        WCHAR *              pwszMethodNameIn,
        long                 lFlagIn,
        IWbemClassObject *   pParamsIn,
        IWbemObjectSink *    pHandlerIn
        );

    virtual SCODE PutInstance(
        CWbemClassObject &   rInstToPutIn,
        long                 lFlagIn,
        IWbemContext *       pCtxIn,
        IWbemObjectSink *    pHandlerIn
        );

    virtual SCODE DeleteInstance(
        CObjPath &           rObjPathIn,
        long                 lFlagIn,
        IWbemContext *       pCtxIn,
        IWbemObjectSink *    pHandlerIn
        );

    static CProvBase * S_CreateThis(
        LPCWSTR         pwszNameIn,
        CWbemServices * pNamespaceIn,
        DWORD           // dwEnumTypeIn
        );

protected:
    
    static const SPropMapEntryArray * RgGetPropMap( void );

    void ClusterToWMI(
        HNETINTERFACE       hNetInterfaceIn,
        IWbemObjectSink *   pHandlerIn
        );

}; //*** class CClusterNetInterface
