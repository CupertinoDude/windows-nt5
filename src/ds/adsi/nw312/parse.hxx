#define MAX_TOKEN_LENGTH       80
#define MAX_KEYWORDS           13

typedef struct _kwdlist {
    DWORD   dwTokenId;
    LPWSTR  Keyword;
} KWDLIST, *PKWDLIST;

class FAR CLexer
{
public:
    CLexer(LPWSTR szBuffer);
    ~CLexer();

    BOOL
    CLexer::IsKeyword(LPWSTR szToken, LPDWORD pdwToken);

    WCHAR
    CLexer::NextChar();

    void
    CLexer::PushbackChar();

    HRESULT
    CLexer::GetNextToken(LPWSTR szToken, LPDWORD pdwToken);

    HRESULT
    CLexer::GetNextToken(LPWSTR szToken, LPWSTR szDisplayToken, LPDWORD pdwToken);

    HRESULT
    CLexer::PushBackToken();

    void
    CLexer::SetAtDisabler(BOOL bFlag);

    BOOL
    CLexer::GetAtDisabler();

private:

    LPWSTR _ptr;
    LPWSTR _Buffer;
    DWORD  _dwLastTokenLength;
    DWORD  _dwLastToken;
    DWORD  _dwEndofString;
    BOOL   _bAtDisabled;
};


HRESULT
Object(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
DsPathName(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
PathName(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
Component(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
Type(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
ProviderName(CLexer * pTokenizer, POBJECTINFO pObjectInfo);

HRESULT
AddComponent(POBJECTINFO pObjectInfo, LPWSTR szToken, LPWSTR szDisplayToken);

HRESULT
AddProviderName(POBJECTINFO pObjectInfo, LPWSTR szToken);

HRESULT
SetType(POBJECTINFO pObjectInfo, DWORD dwToken);


HRESULT
GetDisplayName(
    LPWSTR szName,
    LPWSTR *ppszDisplayName
    );

