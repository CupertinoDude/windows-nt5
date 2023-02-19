/*++

Copyright (C) 1996-1999 Microsoft Corporation

Module Name:

    SUBPARSE.H

History:

--*/


#ifndef PBASE_SUBPARSE_H
#define PBASE_SUBPARSE_H


extern const IID IID_ILocSubParserManager;

typedef CTypedPtrArray<CPtrArray, ILocParser *> ILocSubParserArray;

DECLARE_INTERFACE_(ILocSubParserManager, IUnknown)
{
	//
	//  IUnknown standard interface.
	//
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR*ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	//
	//  Standard Debugging interface.
	//
	STDMETHOD_(void, AssertValidInterface)(THIS) CONST_METHOD PURE;

	STDMETHOD(InitSubParsers)(THIS_ ILocSubParserArray REFERENCE)
		CONST_METHOD PURE;
};


#endif
