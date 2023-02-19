// Copyright (c) 1999-2001 Microsoft Corporation, All Rights Reserved
#ifndef __SNMPGET_H
#define __SNMPGET_H

class GetOperation : public SnmpGetOperation
{
private:

	WbemSnmpProperty **m_PropertyContainer ;
	ULONG m_PropertyContainerLength ;

	SnmpGetResponseEventObject *eventObject ;

	BOOL virtuals ;
	BOOL virtualsInitialised ;
	ULONG varBindsReceived ;
	ULONG erroredVarBindsReceived ;

	SnmpSession *session ;

protected:

	void ReceiveResponse () ;

	void ReceiveVarBindResponse (

		IN const ULONG &var_bind_index,
		IN const SnmpVarBind &requestVarBind ,
		IN const SnmpVarBind &replyVarBind ,
		IN const SnmpErrorReport &error
	) ;

	void ReceiveErroredVarBindResponse(

		IN const ULONG &var_bind_index,
		IN const SnmpVarBind &requestVarBind  ,
		IN const SnmpErrorReport &error
	) ;

	void FrameTooBig() ;

	void FrameOverRun() ;

public:

	GetOperation (IN SnmpSession &session , IN SnmpGetResponseEventObject *eventObject ) ;
	~GetOperation () ;

	void Send () ;

	void DestroyOperation () { SnmpGetOperation :: DestroyOperation () ; }
};

#endif // __SNMPGET_H