/*++

Copyright (c) 1995  Microsoft Corporation

Module Name:

    qryparse.hxx

Abstract:

Author:

    Felix Wong [t-FelixW]    05-Nov-1996

++*/
#ifndef _QRYPARSE_HXX
#define _QRYPARSE_HXX

#define BAIL_ON_FAILURE(hr) \
        if (FAILED(hr)) {       \
                goto error;   \
        }\

#define MAXVAL 1000
#define QUERYNODE_INITIAL   2
#define QUERYNODE_INC       5

#define ATTRNODE_INITIAL    2
#define ATTRNODE_INC        2

enum snodetypes {
    SNODE_SZ,
    SNODE_NODE,
    SNODE_FILTER,
    SNODE_NULL
    };

enum query_symbols {
    QUERY_EQUAL,
    QUERY_NOT,
    QUERY_APPROX,
    QUERY_OR,
    QUERY_AND,
    QUERY_PRESENT,
    QUERY_STRING,
    QUERY_LE,
    QUERY_GE,
    QUERY_UNKNOWN
};

enum search_scope {
    NDS_SCOPE_BASE,
    NDS_SCOPE_ONELEVEL,
    NDS_SCOPE_SUBTREE
};
    

typedef struct _AttrNode {
    LPWSTR szName;
    DWORD dwType;
    BOOL fBinary;
} AttrNode;

class CQueryNode;
class CStack;
class CSyntaxNode;
class CAttrList;




class CQueryNode
{
public:

    CQueryNode();
    ~CQueryNode();

    HRESULT AddToFilterBuf(
                pFilter_Cursor_T  pCur,
                CAttrList *pAttrList
                );

    HRESULT SetToString(LPWSTR szValue);

    static HRESULT MapQueryToNDSType(
                            DWORD dwType,
                            nuint16 *pdwNDSType
                            );
    HRESULT AddChild(CQueryNode *pChild);

    static void N_FAR N_CDECL  FreeFilterTokens(
        nuint32 syntax,
        nptr pVal
        );

    DWORD _dwType;

private:
    LPWSTR _szValue;
    CQueryNode **_rgQueryNode;
    DWORD _dwQueryNode;
    DWORD _dwQueryNodeMax;
};

class CStack
{
public:

    CStack();
    ~CStack();
    HRESULT Push(CSyntaxNode*);
    HRESULT Pop(CSyntaxNode**);
    HRESULT Pop();
    HRESULT Current(CSyntaxNode**);
    void Dump();
    
private:
    DWORD _dwStackIndex;
    CSyntaxNode* _Stack[MAXVAL];
};

// Helper functions
HRESULT MakeNode(
    DWORD dwType,
    CQueryNode *pLQueryNode,
    CQueryNode *pRQueryNode,
    CQueryNode **ppQueryNodeReturn
    );

HRESULT MakeLeaf(
    LPWSTR szValue,
    CQueryNode **ppQuerynNodeReturn
    );


HRESULT Parse(
          LPWSTR szQuery,
          CQueryNode **ppNode,
          CAttrList **ppAttrList
          );

class CSyntaxNode
{
public:

    CSyntaxNode();
    ~CSyntaxNode();
    void SetNode(CQueryNode *pNode);
    void SetNode(LPWSTR szValue);
    void SetNode(DWORD dwFilterType);
    
    DWORD _dwToken;
    DWORD _dwState;
    DWORD _dwType;
    union {
        CQueryNode *_pNode;      // Put in after reduction
        LPWSTR _szValue;         // dwToken == ATTRTYPE|ATTRVAL
        DWORD _dwFilterType;     // reduction of FT     
    };
};

class CAttrList
{
public:

    CAttrList();
    ~CAttrList();
    HRESULT Add(LPWSTR szName);
    HRESULT CAttrList::SetupType(NDS_CONTEXT_HANDLE hADsContext);
    HRESULT GetType(LPWSTR szName, DWORD *pdwType);
    
private:
    DWORD _dwAttrMax;
    DWORD _dwAttrCur;
    AttrNode* _rgAttr;
};


HRESULT
AdsNdsGenerateFilterBuffer(
    NDS_CONTEXT_HANDLE hADsContext,
    LPWSTR szSearchFilter,
    NDS_BUFFER_HANDLE *phFilterBuf
);

#endif


