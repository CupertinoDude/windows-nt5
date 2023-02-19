class CNWCOMPATGroupCollection;

class CNWCOMPATGroupCollection : INHERIT_TRACKING,
                     public CCoreADsObject,
                     public ISupportErrorInfo,           
                     public IADsMembers
{
public:

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) ;

    DECLARE_STD_REFCOUNTING

    DECLARE_IDispatch_METHODS

    NW_DECLARE_ISupportErrorInfo_METHODS

    DECLARE_IADsMembers_METHODS

    CNWCOMPATGroupCollection::CNWCOMPATGroupCollection();

    CNWCOMPATGroupCollection::~CNWCOMPATGroupCollection();

    static
    HRESULT
    CNWCOMPATGroupCollection::CreateGroupCollection(
        BSTR Parent,
        ULONG ParentType,
        BSTR ServerName,
        BSTR GroupName,
        CCredentials &Credentials,
        REFIID riid,
        void **ppvObj
        );

    static
    HRESULT
    CNWCOMPATGroupCollection::AllocateGroupCollectionObject(
        CNWCOMPATGroupCollection ** ppGroup
        );

protected:

    ULONG         _ParentType;
    BSTR          _ServerName;
    BSTR          _lpServerName;
    CCredentials  _Credentials;


    CDispatchMgr FAR * _pDispMgr;
    VARIANT            _vFilter;
};
