/******************************************************************
   SNetScal.cpp -- WBEM provider class implementation

   MODULE:
        DhcpProv.dll

   DESCRIPTION:
        Contains: the definition of the DHCP_Subnet class,
        the static table of manageable objects.

   REVISION:
        08/03/98 - created

******************************************************************/
#include <stdafx.h>

#include "SNetFn.h"     // needed for the declarations of all the functions.
#include "SNetScal.h"   // own header

// static table of CDHCP_Property objects containing the DHCP Server
// scalar parameters (properties) which are WBEM manageable. Each object associates
// the name of the property with their SET and GET functions.
// *** NOTE ***
// The name of each property has to be in sync with the ones specified in the DhcpSchema.mof.
// The indices specified in SrvScal.h should also be in sync with the actual row from this table (they are used
// in the property's action functions.
static const CDHCP_Property DHCP_Subnet_Property[]=
{
 CDHCP_Property(L"Address",                 fnSNetGetAddress,                   NULL),
 CDHCP_Property(L"Mask",                    fnSNetGetMask,                      NULL),
 CDHCP_Property(L"Name",                    fnSNetGetName,                      fnSNetSetName),
 CDHCP_Property(L"Comment",                 fnSNetGetComment,                   fnSNetSetComment),
 CDHCP_Property(L"State",                   fnSNetGetState,                     fnSNetSetState),
 CDHCP_Property(L"NumberOfAddressesInUse",  fnSNetGetNumberOfAddressesInUse,    NULL),
 CDHCP_Property(L"NumberOfAddressesFree",   fnSNetGetNumberOfAddressesFree,     NULL),
 CDHCP_Property(L"NumberOfPendingOffers",   fnSNetGetNumberOfPendingOffers,     NULL)
};

// the name of the WBEM class
#define PROVIDER_NAME_DHCP_SUBNET       "DHCP_Subnet"

// main class instantiation.
CDHCP_Subnet MyDHCP_Subnet_Scalars (PROVIDER_NAME_DHCP_SUBNET, PROVIDER_NAMESPACE_DHCP) ;

/*****************************************************************************
 *
 *  FUNCTION    :	CDHCP_Subnet::CDHCP_Subnet
 *
 *  DESCRIPTION :	Constructor
 *
 *  INPUTS      :	none
 *
 *  RETURNS     :	nothing
 *
 *  COMMENTS    :	Calls the Provider constructor.
 *
 *****************************************************************************/
CDHCP_Subnet::CDHCP_Subnet (const CHString& strName, LPCSTR pszNameSpace ) :
	Provider(strName, pszNameSpace)
{
}

/*****************************************************************************
 *
 *  FUNCTION    :	CDHCP_Subnet::~CDHCP_Subnet
 *
 *  DESCRIPTION :	Destructor
 *
 *  INPUTS      :	none
 *
 *  RETURNS     :	nothing
 *
 *  COMMENTS    : 
 *
 *****************************************************************************/
CDHCP_Subnet::~CDHCP_Subnet ()
{
}

/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::EnumerateInstances
*
*  DESCRIPTION :	Returns all the instances of this class.
*
*  INPUTS      :	none
*
*  RETURNS     :	WBEM_S_NO_ERROR if successful
*
*  COMMENTS    :    Enumerates all this instances of this class. As there is only one
*                   DHCP Server per system, there is only one instance for this class
*
*****************************************************************************/
HRESULT CDHCP_Subnet::EnumerateInstances ( MethodContext* pMethodContext, long lFlags )
{
	CInstance*              pInstance;
    CDHCP_Subnet_Parameters subnetParams(0); // don't know here the IP address
    LPDHCP_MIB_INFO         pMibInfo;
    LPSCOPE_MIB_INFO        pScopeMibInfo;

    HRESULT                 hRes = WBEM_E_FAILED;


    if (subnetParams.GetMibInfo(pMibInfo, pScopeMibInfo, TRUE) && pMibInfo != NULL)
    {
        hRes = WBEM_S_NO_ERROR;

        for (int i=0; hRes == WBEM_S_NO_ERROR && i < pMibInfo->Scopes; i++)
        {
            LPSCOPE_MIB_INFO    pScope = &(pMibInfo->ScopeInfo[i]);             // this is the info from the server,
            CInstance*          pInstance = CreateNewInstance(pMethodContext);  // create now the instance

            if (pScope != NULL && 
                pInstance != NULL)
            {
                WCHAR szSubnetAddr[16]; // should be enough for holding 'xxx.yyy.zzz.uuu\0'

                // don't want to link to ws2_32.lib just for using inet_ntoa only
                swprintf(szSubnetAddr, L"%u.%u.%u.%u",(pScope->Subnet & 0xff000000) >> 24,
                                                      (pScope->Subnet & 0x00ff0000) >> 16,
                                                      (pScope->Subnet & 0x0000ff00) >> 8,
                                                      (pScope->Subnet & 0x000000ff));

                if (pInstance->SetCHString(DHCP_Subnet_Property[IDX_SNET_Address].m_wsPropName, szSubnetAddr) &&
                    LoadInstanceProperties(pInstance))
                {
                    hRes = Commit(pInstance);
                    // now everything relys on the err returned above.
                    continue;
                }
            }

            // at this point something should have definitively gone wrong
            hRes = WBEM_E_FAILED;
         }
    }

    return hRes ;
}

/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::GetObject
*
*  DESCRIPTION :	Find a single instance based on the key properties for the
*					class. 
*
*  INPUTS      :	A pointer to a CInstance object containing the key properties. 
*
*  RETURNS     :	WBEM_S_NO_ERROR if the instance can be found
*			WBEM_E_NOT_FOUND if the instance described by the key properties 
* 				could not be found
*			WBEM_E_FAILED if the instance could be found but another error 
*				occurred. 
*
*  COMMENTS    :
*
*****************************************************************************/
HRESULT CDHCP_Subnet::GetObject ( CInstance* pInstance, long lFlags )
{
    return LoadInstanceProperties(pInstance)? WBEM_S_NO_ERROR : WBEM_E_NOT_FOUND;
}

/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::ExecQuery
*
*  DESCRIPTION :	You are passed a method context to use in the creation of 
*			instances that satisfy the query, and a CFrameworkQuery 
*			which describes the query.  Create and populate all 
*			instances which satisfy the query.  CIMOM will post - 
*			filter the query for you, you may return more instances 
*			or more properties than are requested and CIMOM 
*			will filter out any that do not apply.
*
*
*  INPUTS      : 
*
*  RETURNS     :	WBEM_E_PROVIDER_NOT_CAPABLE if not supported for this class
*			WBEM_E_FAILED if the query failed
*			WBEM_S_NO_ERROR if query was successful 
*
*  COMMENTS    : TO DO: Most providers will not need to implement this method.  If you don't, cimom 
*			will call your enumerate function to get all the instances and perform the 
*			filtering for you.  Unless you expect SIGNIFICANT savings from implementing 
*			queries, you should remove this entire method.
*
*****************************************************************************/
HRESULT CDHCP_Subnet::ExecQuery (MethodContext *pMethodContext, CFrameworkQuery& Query, long lFlags)
{
	 return (WBEM_E_PROVIDER_NOT_CAPABLE);
}

/*****************************************************************************
*
*  FUNCTION    : CDHCP_Subnet::PutInstance
*
*  DESCRIPTION :	PutInstance should be used in provider classes that can write
*			instance information back to the hardware or software.
*			For example: Win32_Environment will allow a PutInstance of a new
*			environment variable, because environment variables are "software"
*			related.   However, a class like Win32_MotherboardDevice will not
*			allow editing of the bus speed.   Since by default PutInstance 
*			returns WBEM_E_PROVIDER_NOT_CAPABLE, this function is placed here as a
*			skeleton, but can be removed if not used.
*
*  INPUTS      : 
*
*  RETURNS     :	WBEM_E_PROVIDER_NOT_CAPABLE if PutInstance is not available
*			WBEM_E_FAILED if there is an error delivering the instance
*			WBEM_E_INVALID_PARAMETER if any of the instance properties 
*				are incorrect.
*			WBEM_S_NO_ERROR if instance is properly delivered
*
*  COMMENTS    : TO DO: If you don't intend to support writing to your provider, remove this method.
*
*****************************************************************************/
HRESULT CDHCP_Subnet::PutInstance ( const CInstance &Instance, long lFlags)
{
    int                 i;
    DWORD               returnCode;
    CHString            strName, strMask;
    DHCP_IP_ADDRESS     dwAddress, dwMask;
    LPDHCP_SUBNET_INFO  pSubnetInfo;
    BOOL                newInstance;

    if (!Instance.GetCHString(DHCP_Subnet_Property[IDX_SNET_Address].m_wsPropName, strName) ||
        !Instance.GetCHString(DHCP_Subnet_Property[IDX_SNET_Mask].m_wsPropName, strMask) ||
        !inet_wstodw(strName, dwAddress) ||
        !inet_wstodw(strMask, dwMask))
        return FALSE;

    // the object below is used as a "repository" for all the properties' values.
    // once it is filled up with data, it is commited explicitely to the DHCP Server.
    CDHCP_Subnet_Parameters SubnetParameters(dwAddress, dwMask);

    // first get the subnet information, just to get the info that doesn't change
    // if fails => a new instance has to be created
    newInstance = !SubnetParameters.GetSubnetInfo(pSubnetInfo,TRUE);

    for (i = 0; i < NUM_SUBNET_PROPERTIES; i++)
    {
        if (DHCP_Subnet_Property[i].m_pfnActionSet != NULL)
        {
            // execute the SET property action function
            // because of the last NULL, no data is written to DHCP Server at this moment, 
            // only SubnetParameters object is filled up with the values taken from the Instance.
            // don't care much here about the error codes. Failure means some properties will
            // not be written. At least we are giving a chance to all the writable properties.
            (*(DHCP_Subnet_Property[i].m_pfnActionSet))(&SubnetParameters, (CInstance *)&Instance, NULL);
        }
    }

    if (newInstance)
        return SubnetParameters.CommitNew(returnCode) ? WBEM_S_NO_ERROR : WBEM_E_FAILED;
    else
        // commit the values of all the writable properties to DHCP Server
        return SubnetParameters.CommitSet(returnCode) ? WBEM_S_NO_ERROR : WBEM_E_FAILED;
}

/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::DeleteInstance
*
*  DESCRIPTION :	DeleteInstance, like PutInstance, actually writes information
*			to the software or hardware.   For most hardware devices, 
*			DeleteInstance should not be implemented, but for software
*			configuration, DeleteInstance implementation is plausible.
*			Like PutInstance, DeleteInstance returns WBEM_E_PROVIDER_NOT_CAPABLE from
*			inside Provider::DeleteInstance (defined in Provider.h).  So, if
*			you choose not to implement DeleteInstance, remove this function
*			definition and the declaration from DHCP_Server_Scalars.h 
*
*  INPUTS      : 
*
*  RETURNS     :	WBEM_E_PROVIDER_NOT_CAPABLE if DeleteInstance is not available.
*			WBEM_E_FAILED if there is an error deleting the instance.
*			WBEM_E_INVALID_PARAMETER if any of the instance properties 
*				are incorrect.
*			WBEM_S_NO_ERROR if instance is properly deleted.
*
*  COMMENTS    : TO DO: If you don't intend to support deleting instances, remove this method.
*
*****************************************************************************/
HRESULT CDHCP_Subnet::DeleteInstance ( const CInstance &Instance, long lFlags )
{
    DWORD           returnCode;
    CHString        str;
    DHCP_IP_ADDRESS dwAddress;

    if (!Instance.GetCHString(DHCP_Subnet_Property[IDX_SNET_Address].m_wsPropName, str) ||
        !inet_wstodw(str, dwAddress))
        return (WBEM_E_FAILED);

    // the object below is used as a "repository" for all the properties' values.
    // once it is filled up with data, it is commited explicitely to the DHCP Server.
    CDHCP_Subnet_Parameters SubnetParameters(dwAddress);

    return SubnetParameters.DeleteSubnet() ? WBEM_S_NO_ERROR : WBEM_E_FAILED;
}

/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::ExecMethod
*
*  DESCRIPTION : 	Override this function to provide support for methods.  
*			A method is an entry point for the user of your provider 
*			to request your class perform some function above and 
*			beyond a change of state.  (A change of state should be 
*			handled by PutInstance() )
*
*  INPUTS      :	A pointer to a CInstance containing the instance the method was executed against.
*			A string containing the method name
*			A pointer to the CInstance which contains the IN parameters.
*			A pointer to the CInstance to contain the OUT parameters.
*			A set of specialized method flags
*
*  RETURNS     :	WBEM_E_PROVIDER_NOT_CAPABLE if not implemented for this class
*			WBEM_S_NO_ERROR if method executes successfully
*			WBEM_E_FAILED if error occurs executing method 
*
*  COMMENTS    : TO DO: If you don't intend to support Methods, remove this method.
*
*****************************************************************************/
HRESULT CDHCP_Subnet::ExecMethod ( const CInstance& Instance,
						const BSTR bstrMethodName,
						CInstance *pInParams,
						CInstance *pOutParams,
						long lFlags)
{
    int                 i;
    DWORD               returnCode;
    CHString            str;
    DHCP_IP_ADDRESS     dwAddress;
    LPDHCP_SUBNET_INFO  pSubnetInfo;
    int     nSetPrefixLen = wcslen(_SNET_SET_PREFIX);

    if (!Instance.GetCHString(DHCP_Subnet_Property[IDX_SNET_Address].m_wsPropName, str) ||
        !inet_wstodw(str, dwAddress))
        return FALSE;

    // build the object identifying the subnet against which the method is executed
    CDHCP_Subnet_Parameters SubnetParameters(dwAddress);

    // get the subnet information, just to preserve the info that doesn't change
    if (!SubnetParameters.GetSubnetInfo(pSubnetInfo,TRUE))
        return WBEM_E_FAILED;

    // is it a "SET" operation?
    if (!_wcsnicmp (bstrMethodName, _SNET_SET_PREFIX, nSetPrefixLen))
    {
        int i;

        WCHAR *wcsPropertyName = bstrMethodName + nSetPrefixLen;    // pointer to the property name

        // scan the DHCP_Subnet_Property table looking for the property's row
        for (i = 0; i < NUM_SUBNET_PROPERTIES; i++)
        {
            // if the property row is found
            if (!_wcsicmp(wcsPropertyName, DHCP_Subnet_Property[i].m_wsPropName))
            {
                // see if the property is writable 
                if (DHCP_Subnet_Property[i].m_pfnActionSet != NULL)
                {
                    // execute the SET property action function
                    // because of the pOutParams, the method will be commited inside the call
                    if ((*(DHCP_Subnet_Property[i].m_pfnActionSet))(&SubnetParameters, pInParams, pOutParams))
                        // everything worked fine.
                        return WBEM_S_NO_ERROR;
                    else
                        // an error occured during "SET"
                        return WBEM_E_FAILED;
                }
                else
                    // no, the property cannot be written. (shouldn't really happen, as the methods from the
                    // repository should match the writable properties only)
                    return WBEM_E_READ_ONLY;
            }
        }
    }

    // if this point was reached, no method was found => provider not capable
    return WBEM_E_PROVIDER_NOT_CAPABLE;
}


/*****************************************************************************
*
*  FUNCTION    :	CDHCP_Subnet::LoadInstanceProperties
*
*  RETURNS     :	TRUE if the values for all the properties was loaded successfully,
*           FALSE otherwise.
*
*  COMMENTS    :    It loops through the Server_Property table, calling the GET functions.
*
*****************************************************************************/
BOOL CDHCP_Subnet::LoadInstanceProperties(CInstance* pInstance)
{
    CHString        str;
    DHCP_IP_ADDRESS dwAddress;

    if (!pInstance->GetCHString(DHCP_Subnet_Property[IDX_SNET_Address].m_wsPropName, str) ||
        !inet_wstodw(str, dwAddress))
        return FALSE;

    // there should be used this object, in order to not call several times the DHCP Server API.
    CDHCP_Subnet_Parameters SubnetParameters(dwAddress);

    for (int i = 0; i < NUM_SUBNET_PROPERTIES; i++)
    {
        // if there is an invisible property (does not support GET) just skip it.
        if (DHCP_Subnet_Property[i].m_pfnActionGet == NULL)
            continue;

        // call the appropriate GET function, fail if the call fails (there should be no reason for failure)
        if (!(*(DHCP_Subnet_Property[i].m_pfnActionGet))(&SubnetParameters, NULL, pInstance))
            return FALSE;
    }

    return TRUE;
}
