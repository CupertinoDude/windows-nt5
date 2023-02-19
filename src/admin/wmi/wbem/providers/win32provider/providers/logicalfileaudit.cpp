/*****************************************************************************/

/*  Copyright (c) 1999-2001 Microsoft Corporation, All Rights Reserved            /
/*****************************************************************************/

#include "precomp.h"
#include "AccessEntry.h"			// CAccessEntry class
#include "AccessEntryList.h"
#include "DACL.h"					// CDACL class
#include "SACL.h"
#include "securitydescriptor.h"
#include "securefile.h"
#include "logicalfileaudit.h"


typedef std::vector<_bstr_t> BSTRTVEC;


CWin32LogicalFileAudit LogicalFileAudit( LOGICAL_FILE_AUDIT_NAME, IDS_CimWin32Namespace );

/*
    [Dynamic, Association: ToInstance]
class Win32_LogicalFileAccess : Win32_SecuritySettingAccess
{
    Win32_LogicalFileSecuritySetting ref SecuritySetting;

    Win32_SID ref Trustee;
};
*/

CWin32LogicalFileAudit::CWin32LogicalFileAudit( const CHString& setName, LPCTSTR pszNameSpace /*=NULL*/)
:	Provider( setName, pszNameSpace )
{
}

CWin32LogicalFileAudit::~CWin32LogicalFileAudit()
{
}

HRESULT CWin32LogicalFileAudit::GetObject( CInstance* pInstance, long lFlags /*= 0L*/ )
{
	HRESULT hr = WBEM_E_NOT_FOUND;

#ifdef NTONLY

	if(pInstance != NULL)
	{
		CInstancePtr pLogicalFileInstance;

		// get instance by path on Win32_LogicalFileSecuritySetting part
		CHString chsLogicalFileSecurityPath;
		pInstance->GetCHString(_T("SecuritySetting"), chsLogicalFileSecurityPath);
		MethodContext* pMethodContext = pInstance->GetMethodContext();

		if(SUCCEEDED(CWbemProviderGlue::GetInstanceByPath(chsLogicalFileSecurityPath, &pLogicalFileInstance, pMethodContext)) &&
           pLogicalFileInstance != NULL)
		{
			CHString chsFilePath;

			pLogicalFileInstance->GetCHString(IDS_Path, chsFilePath);

			CSecureFile secFile(chsFilePath, TRUE); // Need SACL
			CSACL sacl;
			secFile.GetSACL(sacl);

			// walk SACL looking for the sid path passed in....
			ACLPOSITION aclPos;
            // Need merged list...
            CAccessEntryList t_ael;
            if(sacl.GetMergedACL(t_ael))
            {
			    if(t_ael.BeginEnum(aclPos))
                {
			        CAccessEntry ACE;
			        CSid sidTrustee;
                    bool fFoundIt = false;
			        while(t_ael.GetNext(aclPos, ACE ) && !fFoundIt)
			        {
				        ACE.GetSID(sidTrustee);
				        CHString chsTrustee = sidTrustee.GetSidString();

				        CInstancePtr pSIDInstance;
				        CHString chstrSIDPath;
				        pInstance->GetCHString(_T("Trustee"), chstrSIDPath);
		                //CHString chstrFullSIDPath;
                        //chstrFullSIDPath.Format("\\\\%s\\%s:Win32_SID.SID=\"%s\"",
                        //             GetLocalComputerName(),
                        //             IDS_CimWin32Namespace,
                        //             (LPCTSTR)chstrSIDPath);
			  	        if(SUCCEEDED(CWbemProviderGlue::GetInstanceByPath(chstrSIDPath, &pSIDInstance, pInstance->GetMethodContext())))
				        {
					        if(pSIDInstance != NULL)
					        {
						        CHString chsInstanceSID;
						        pSIDInstance->GetCHString(_T("SID"), chsInstanceSID);
						        if(chsTrustee.CompareNoCase(chsInstanceSID) == 0)
						        {
							        hr = WBEM_S_NO_ERROR;
                                    fFoundIt = true;
						        }
					        }
				        }
			        }
			        t_ael.EndEnum(aclPos);
                }
            }
		}
	}
#endif

	return(hr);
}


HRESULT CWin32LogicalFileAudit::ExecQuery(MethodContext *pMethodContext, CFrameworkQuery& pQuery, long lFlags /*= 0L*/)
{
    HRESULT hr = WBEM_S_NO_ERROR;

#ifdef NTONLY

    // We optimize for one scenario only:  the query specified one or more SecuritySettings, requesting associations with the Trustees of
    // each; if the query specified one or more Trustees, we would have to enumerate all instances of cim_logicalfile and
    // determine the Trustees of each, so we don't support that "optimization".
    BSTRTVEC vectorSecuritySettings;
    BSTRTVEC vectorTrustees;
    pQuery.GetValuesForProp(IDS_SecuritySetting, vectorSecuritySettings);
    pQuery.GetValuesForProp(IDS_Trustee, vectorTrustees);
    DWORD dwSettings = vectorSecuritySettings.size();
    DWORD dwTrustees = vectorTrustees.size();
    if(dwSettings >= 1 && dwTrustees == 0)
    {
        CInstancePtr pSecSetting;
        for(LONG m = 0; m < dwSettings && SUCCEEDED(hr); m++)
        {
            CHString chstrLFSSPath;  // LogicalFileSecuritySetting path
            pSecSetting = NULL;
            chstrLFSSPath.Format(L"\\\\%s\\%s:%s",
                                 (LPCTSTR)GetLocalComputerName(),
                                 IDS_CimWin32Namespace,
                                 (LPCTSTR)CHString((WCHAR*)vectorSecuritySettings[m]));

            if(SUCCEEDED(hr = CWbemProviderGlue::GetInstanceKeysByPath(chstrLFSSPath, &pSecSetting, pMethodContext)))
            {
                if(pSecSetting != NULL)
                {
                    CHString chstrSSPath; // SecuritySetting path
                    pSecSetting->GetCHString(IDS_Path, chstrSSPath);
                    if(!chstrSSPath.IsEmpty())
                    {
                        CSecureFile secFile(chstrSSPath, TRUE);  // need SACL
                        // Get the SACL and for each entry create an instance of this association...
			            CSACL sacl;
                        CSid sidTrustee;
                        ACLPOSITION aclPos;
                        CAccessEntry ACE;
			            secFile.GetSACL(sacl);
                        // Need merged list...
                        CAccessEntryList t_ael;
                        if(sacl.GetMergedACL(t_ael))
                        {
                            if(t_ael.BeginEnum(aclPos))
                            {
                                while(t_ael.GetNext(aclPos, ACE) && SUCCEEDED(hr))
			                    {
                                    ACE.GetSID(sidTrustee);
                                    if(sidTrustee.IsValid())
                                    {
                                        CInstancePtr pNewAssocInst;
                                        pNewAssocInst.Attach(CreateNewInstance(pMethodContext));
                                        if(pNewAssocInst != NULL)
                                        {
                                            // Set the SecuritySetting property of the association instance.
                                            pNewAssocInst->SetCHString(IDS_SecuritySetting, chstrLFSSPath);
                                            // Set the Trustee property of the association instance.
                                            CHString chstrFullWin32SIDPath;
                                            chstrFullWin32SIDPath.Format(L"\\\\%s\\%s:Win32_SID.SID=\"%s\"",
                                                                         (LPCTSTR)GetLocalComputerName(),
                                                                         IDS_CimWin32Namespace,
                                                                         (LPCTSTR)sidTrustee.GetSidString());
                                            pNewAssocInst->SetCHString(IDS_Trustee, chstrFullWin32SIDPath);
                                            hr = pNewAssocInst->Commit();
                                        }
                                    }
                                }
                                t_ael.EndEnum(aclPos);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        // hr = EnumerateInstances(pMethodContext, lFlags);
        // commented out since some other classes may support exec queries of this type, and returning provider not
        // capable from this class results in some instances (from other classes) being returned, followed by an abort
        // of the enumeration due to this class's returning provider not capable.
    }

#endif

    return hr;
}


HRESULT CWin32LogicalFileAudit::EnumerateInstances( MethodContext*  pMethodContext, long lFlags /*= 0L*/ )
{
/*	HRESULT hr = WBEM_S_NO_ERROR;

    if(m_dwPlatformID != VER_PLATFORM_WIN32_NT)
    {
		return(hr);
	}

    TRefPointerCollection<CInstance> LCIMLogicalFiles;

    CHString chstrAllFilesQuery;
    chstrAllFilesQuery = L"SELECT __PATH FROM CIM_LogicalFile";
    if(SUCCEEDED(CWbemProviderGlue::GetInstancesByQuery(chstrAllFilesQuery,
                                                        &LCIMLogicalFiles,
                                                        pMethodContext,
                                                        IDS_CimWin32Namespace)))

    {
        REFPTRCOLLECTION_POSITION pos;
        if(LCIMLogicalFiles.BeginEnum(pos))
        {
            CInstance* pinstCIMLogicalFile = NULL;
            CInstance* pSecSetting = NULL;
            while((SUCCEEDED(hr)) && (pinstCIMLogicalFile = LCIMLogicalFiles.GetNext(pos)))
            {
                if(pinstCIMLogicalFile != NULL)
                {
                    // For each logicalfile instance, need to create an association instance.
                    // For each association instance, need to fill in two properties: Trustee and SecuritySetting.

                    // Get and set the SecuritySetting property:
                    // First get the name property of the CIM_LogicalFile instance:
                    CHString chstrCLFName;
                    pinstCIMLogicalFile->GetCHString(IDS_Name, chstrCLFName);
                    CHString chstrDblEscCLFName;
                    EscapeBackslashes(chstrCLFName, chstrDblEscCLFName);
                    CHString chstrLFSSPath;  // LogicalFileSecuritySetting path
                    pSecSetting = NULL;
                    chstrLFSSPath.Format(L"\\\\%s\\%s:Win32_LogicalFileSecuritySetting.Path=\"%s\"",
                                         (LPCTSTR)GetLocalComputerName(),
                                         IDS_CimWin32Namespace,
                                         (LPCTSTR)chstrDblEscCLFName);

                    if(SUCCEEDED(hr = CWbemProviderGlue::GetInstanceByPath(chstrLFSSPath, &pSecSetting)))
                    {
                        if(pSecSetting != NULL)
                        {
                            CHString chstrSSPath; // SecuritySetting path
                            pSecSetting->GetCHString(IDS_Path, chstrSSPath);
                            if(!chstrSSPath.IsEmpty())
                            {
                                CSecureFile secFile(chstrSSPath, TRUE);  // need SACL
                                // Get the SACL and for each entry create an instance of this association...
			                    CSACL sacl;
                                CSid sidTrustee;
                                ACLPOSITION aclPos;
                                CAccessEntry ACE;
			                    secFile.GetSACL(sacl);
                                if(sacl.BeginEnum(aclPos))
                                {
                                    while(sacl.GetNext(aclPos, ACE))
			                        {
                                        ACE.GetSID(sidTrustee);
                                        if(sidTrustee.IsValid())
                                        {
                                            CInstance* pNewAssocInst = CreateNewInstance(pMethodContext);
                                            if(pNewAssocInst != NULL)
                                            {
                                                // Set the SecuritySetting property of the association instance.
                                                pNewAssocInst->SetCHString(IDS_SecuritySetting, chstrLFSSPath);
                                                // Set the Trustee property of the association instance.
                                                CHString chstrFullWin32SIDPath;
                                                chstrFullWin32SIDPath.Format(L"\\\\%s\\%s:Win32_SID.SID=\"%s\"",
                                                                             (LPCTSTR)GetLocalComputerName(),
                                                                             IDS_CimWin32Namespace,
                                                                             (LPCTSTR)sidTrustee.GetSidString());
                                                pNewAssocInst->SetCHString(IDS_Trustee, chstrFullWin32SIDPath);
                                                hr = Commit(pNewAssocInst);
                                            }
                                        }
                                    }
                                    sacl.EndEnum(aclPos);
                                }
                            }
                            pSecSetting->Release();
                            pSecSetting = NULL;
                        }
                    }
                    pinstCIMLogicalFile->Release();
                    pinstCIMLogicalFile = NULL;
                }
            }
            LCIMLogicalFiles.EndEnum();
        }
    }

	return hr;
*/
    return WBEM_E_PROVIDER_NOT_CAPABLE;
}
