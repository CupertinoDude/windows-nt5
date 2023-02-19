
class FAR CNWCOMPATFileServiceEnum : public CNWCOMPATEnumVariant
{
public:
    //
    // IEnumVARIANT methods
    //
    STDMETHOD(Next)(
        ULONG cElements,
        VARIANT FAR* pvar,
        ULONG FAR* pcElementFetched
        );

    static
    HRESULT
    CNWCOMPATFileServiceEnum::Create(
        CNWCOMPATFileServiceEnum FAR* FAR* ppEnumVariant,
        BSTR ADsPath,
        BSTR bstrServerName,
        CCredentials &Credentials
        );

    CNWCOMPATFileServiceEnum();

    ~CNWCOMPATFileServiceEnum();

    HRESULT
    CNWCOMPATFileServiceEnum::GetFileShareObject(
        IDispatch ** ppDispatch
        );

    HRESULT
    CNWCOMPATFileServiceEnum::EnumFileShares(
        ULONG cElements,
        VARIANT FAR* pvar,
        ULONG FAR* pcElementFetched
        );

private:
    BSTR          _ADsPath;
    NWVOL_NUM     _bResumeVolumeID;
    NWCONN_HANDLE _hConn;
    NW_VERSION_INFO  _FileServerInfo;
    CCredentials _Credentials;
};


