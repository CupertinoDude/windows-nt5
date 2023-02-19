//
// CertUtil.h
//
#ifndef _CERTUTIL_H
#define _CERTUTIL_H

#include <wincrypt.h>
#include <CertCli.h>
#include <xenroll.h>
#include "Certificat.h"

BOOL GetOnlineCAList(CStringList& list, const CString& certType, HRESULT * phRes);
BOOL GetRequestInfoFromPKCS10(CCryptBlob& pkcs10, 
										PCERT_REQUEST_INFO * pReqInfo,
										HRESULT * phRes);
PCCERT_CONTEXT GetPendingDummyCert(const CString& inst_name, 
											  IEnroll * pEnroll,
											  HRESULT * phRes);
HCERTSTORE OpenRequestStore(IEnroll * pEnroll, HRESULT * phResult);
HCERTSTORE OpenMyStore(IEnroll * pEnroll, HRESULT * phResult);
PCCERT_CONTEXT GetCertContextFromPKCS7File(const CString& resp_file_name, 
														CERT_PUBLIC_KEY_INFO * pKeyInfo,
														HRESULT * phResult);
PCCERT_CONTEXT GetCertContextFromPKCS7(const BYTE * pbData, DWORD cbData,
													CERT_PUBLIC_KEY_INFO * pKeyInfo,
													HRESULT * phResult);
PCCERT_CONTEXT GetRequestContext(CCryptBlob& pkcs10, HRESULT * phRes);
BOOL GetRequestInfoFromPKCS10(CCryptBlob& pkcs10, 
										PCERT_REQUEST_INFO * pReqInfo,
										HRESULT * phRes);
//BOOL GetRequestInfoFromRenewalRequest(CCryptBlob& renewal_req,
//                              PCCERT_CONTEXT * pSignerCert,
//                              HCERTSTORE hStore,
//										PCERT_REQUEST_INFO * pReqInfo,
//										HRESULT * phRes);
PCCERT_CONTEXT GetReqCertByKey(IEnroll * pEnroll, 
										 CERT_PUBLIC_KEY_INFO * pKeyInfo, 
										 HRESULT * phResult);

BOOL FormatDateString(CString& str, 
							 FILETIME ft, 
							 BOOL fIncludeTime, 
							 BOOL fLongFormat);
BOOL
GetKeyUsageProperty(PCCERT_CONTEXT pCertContext, 
						  CERT_ENHKEY_USAGE ** pKeyUsage, 
						  BOOL fPropertiesOnly, 
						  HRESULT * phRes);
BOOL
ContainsKeyUsageProperty(PCCERT_CONTEXT pCertContext, 
						 CArray<LPCSTR, LPCSTR>& uses, 
						 HRESULT * phRes);
BOOL FormatEnhancedKeyUsageString(CString& str, 
											 PCCERT_CONTEXT pCertContext, 
											 BOOL fPropertiesOnly, 
											 BOOL fMultiline,
											 HRESULT * phRes);
PCCERT_CONTEXT
GetInstalledCert(const CString& machine_name, 
					  const CString& server_name,
					  IEnroll * pEnroll,
					  HRESULT * phResult);
BOOL 
InstallCertByHash(CRYPT_HASH_BLOB * pHash,
					  const CString& machine_name, 
					  const CString& server_name,
					  IEnroll * pEnroll,
					  HRESULT * phResult);
BOOL
InstallHashToMetabase(CRYPT_HASH_BLOB * pHash,
					  const CString& machine_name, 
					  const CString& server_name,
					  HRESULT * phResult);
HRESULT CreateRequest_Base64(const BSTR bstr_dn, 
                             IEnroll * pEnroll,
                             BSTR csp_name,
                             DWORD csp_type,
                             BSTR * pOut);
BOOL AttachFriendlyName(PCCERT_CONTEXT pContext, const CString& name, HRESULT * phRes);
BOOL GetFriendlyName(PCCERT_CONTEXT pCertContext,
					 CString& name,
					 HRESULT * phRes);
BOOL GetNameString(PCCERT_CONTEXT pCertContext,
				  DWORD type,
				  DWORD flag,
				  CString& name,
				  HRESULT * phRes);
BOOL GetHashProperty(PCCERT_CONTEXT pCertContext, CCryptBlob& hash_blob, HRESULT * phRes);
BOOL GetStringProperty(PCCERT_CONTEXT pCertContext, DWORD propId, CString& str, HRESULT * phRes);
BOOL GetBlobProperty(PCCERT_CONTEXT pCertContext,
					 DWORD propId,
					 CCryptBlob& blob,
					 HRESULT * phRes);

BOOL EncodeString(CString& str, CCryptBlob& blob, HRESULT * phRes);
BOOL EncodeInteger(int number, CCryptBlob& blob, HRESULT * phRes);
BOOL EncodeBlob(CCryptBlob& in, CCryptBlob& out, HRESULT * phRes);
BOOL DecodeBlob(CCryptBlob& in, CCryptBlob& out, HRESULT * phRes);
BOOL GetServerComment(const CString& machine_name, const CString& server_name,
					  CString& comment, HRESULT * phResult);
void FormatRdnAttr(CString& str, DWORD dwValueType, CRYPT_DATA_BLOB& blob);

BOOL CreateDirectoryFromPath(LPCTSTR szPath, LPSECURITY_ATTRIBUTES lpSA);

BOOL CompactPathToWidth(CWnd * pControl, CString& strPath);

BOOL GetKeySizeLimits(IEnroll * pEnroll, 
					  DWORD * min, DWORD * max, DWORD * def, 
					  BOOL bGSC,
					  HRESULT * phRes);
HRESULT ShutdownSSL(CString& machine_name, CString& server_name);

#endif	//_CERTUTIL_H