//-----------------------------------------------------------------------------

//

//  File: WMIparse.h

//

//  Author: Kjell Swedin

// Copyright (c) 1995-2001 Microsoft Corporation, All Rights Reserved 
//
//-----------------------------------------------------------------------------

#ifndef WMIPARSE_H
#define WMIPARSE_H

#define OPT_DISABLE_WATERMARKING "DisableWatermarking"

class CWMILocParser : public CPULocParser
{
public:
	CWMILocParser();
	~CWMILocParser();

// Overrides
public:
	// ILocParser
	virtual HRESULT OnInit(IUnknown *);
	virtual HRESULT OnCreateFileInstance(ILocFile * &, FileType);
	virtual void OnGetParserInfo(ParserInfo &) const;
	virtual void OnGetFileDescriptions(CEnumCallback &) const;

	// ILocVersion
	virtual void OnGetParserVersion(DWORD &dwMajor,	DWORD &dwMinor, BOOL &fDebug) const;

	// ILocStringValidation
	virtual CVC::ValidationCode OnValidateString(const CLocTypeId &ltiType,
			const CLocTranslation &trTrans, CReporter *pReporter,
			const CContext &context);

// Implementation
protected:
	void GetDefaultExtensionList(CLocExtensionList & elExtList) const;

private:
	void RegisterOptions();
	void UnRegisterOptions();

	BOOL m_fOptionInit;
};

#endif
