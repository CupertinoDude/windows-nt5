/////////////////////////////////////////////////////////////////////////////
//  INTEL Corporation Proprietary Information
//  This listing is supplied under the terms of a license agreement with Intel 
//  Corporation and many not be copied nor disclosed except in accordance
//  with the terms of that agreement.
//  Copyright (c) 1995, 1996 Intel Corporation. 
//
//
//  Module Name: lhrcv.h
//	Environment: MSVC 4.0, OLE 2
/////////////////////////////////////////////////////////////////////////////////
#ifndef LHRCV_H
#define LHRCV_H

#include "ppmrcv.h"
#include "ppmclsid.h"
#include "lh.h"

#define LH_BUFFER_SIZE 5000
#define DO_NOT_PROCESS		5

class LH_ppmReceive : public ppmReceive
{
private:
	BOOL m_FirstAudioChunk;

public:

LH_ppmReceive(IUnknown* pUnkOuter, 
				IUnknown** ppUnkInner);
~LH_ppmReceive();

DECLARE_CREATEPROC()

STDMETHODIMP_( const CLSID& ) GetCLSID( void ) const {return CLSID_LHPPMReceive;}

STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR* ppvObj )
{return CUnknown::QueryInterface(riid, ppvObj);}

STDMETHODIMP GetInterface( REFIID riid, LPVOID FAR* ppvObj )
{return ppmReceive::GetInterface( riid, ppvObj );}

STDMETHODIMP_( ULONG )AddRef( void )
{return CUnknown::AddRef();}

STDMETHODIMP_( ULONG )Release( void )
{return CUnknown::Release();}

//////////////////////////////////////////////////////////////////////////////////////////
//TimeToProcessMessages:
//////////////////////////////////////////////////////////////////////////////////////////
virtual BOOL TimeToProcessMessages(FragDescriptor *pFragDescrip, MsgHeader *pMsgHdr);
 
//////////////////////////////////////////////////////////////////////////////////////////
//CheckMessageComplete: 
//////////////////////////////////////////////////////////////////////////////////////////
virtual BOOL CheckMessageComplete(MsgHeader *pMsgHdr); 

//////////////////////////////////////////////////////////////////////////////////////////
//ProcessMessages: 
//////////////////////////////////////////////////////////////////////////////////////////
virtual HRESULT ProcessMessages(void);

//////////////////////////////////////////////////////////////////////////////////////////
//PartialMessageHandler: deals with partial messages
//////////////////////////////////////////////////////////////////////////////////////////
virtual HRESULT PartialMessageHandler(MsgHeader *pMsgHdr);

};

#endif
