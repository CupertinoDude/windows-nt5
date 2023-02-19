#include "main.h"


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// CSnapIn object implementation                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

CSnapIn::CSnapIn(CComponentData *pComponent)
{
    m_cRef = 1;
    InterlockedIncrement(&g_cRefThisDll);

    m_pcd = pComponent;

    m_pConsole = NULL;
    m_pResult = NULL;
    m_pHeader = NULL;
    m_pConsoleVerb = NULL;
    m_pDisplayHelp = NULL;
    m_nColumnSize = 180;
    m_lViewMode = LVS_REPORT;
    m_bExpand = TRUE;

    LoadString(g_hInstance, IDS_NAME, m_column1, ARRAYSIZE(m_column1));
}

CSnapIn::~CSnapIn()
{
    InterlockedDecrement(&g_cRefThisDll);
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// CSnapIn object implementation (IUnknown)                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


HRESULT CSnapIn::QueryInterface (REFIID riid, void **ppv)
{
    if (IsEqualIID(riid, IID_IComponent) || IsEqualIID(riid, IID_IUnknown))
    {
        *ppv = (LPCOMPONENT)this;
        m_cRef++;
        return S_OK;
    }
    else if (IsEqualIID(riid, IID_IExtendContextMenu))
    {
        *ppv = (LPEXTENDCONTEXTMENU)this;
        m_cRef++;
        return S_OK;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

ULONG CSnapIn::AddRef (void)
{
    return ++m_cRef;
}

ULONG CSnapIn::Release (void)
{
    if (--m_cRef == 0) {
        delete this;
        return 0;
    }

    return m_cRef;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// CSnapIn object implementation (IComponent)                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CSnapIn::Initialize(LPCONSOLE lpConsole)
{
    HRESULT hr;

    // Save the IConsole pointer
    m_pConsole = lpConsole;
    m_pConsole->AddRef();

    hr = m_pConsole->QueryInterface(IID_IHeaderCtrl,
                        reinterpret_cast<void**>(&m_pHeader));

    // Give the console the header control interface pointer
    if (SUCCEEDED(hr))
        m_pConsole->SetHeader(m_pHeader);

    m_pConsole->QueryInterface(IID_IResultData,
                        reinterpret_cast<void**>(&m_pResult));

    hr = m_pConsole->QueryConsoleVerb(&m_pConsoleVerb);

    hr = m_pConsole->QueryInterface(IID_IDisplayHelp,
                        reinterpret_cast<void**>(&m_pDisplayHelp));

    return S_OK;
}

STDMETHODIMP CSnapIn::Destroy(MMC_COOKIE cookie)
{

    if (m_pConsole != NULL)
    {
        m_pConsole->SetHeader(NULL);
        m_pConsole->Release();
        m_pConsole = NULL;
    }

    if (m_pHeader != NULL)
    {
        m_pHeader->Release();
        m_pHeader = NULL;
    }
    if (m_pResult != NULL)
    {
        m_pResult->Release();
        m_pResult = NULL;
    }

    if (m_pConsoleVerb != NULL)
    {
        m_pConsoleVerb->Release();
        m_pConsoleVerb = NULL;
    }

    if (m_pDisplayHelp != NULL)
    {
        m_pDisplayHelp->Release();
        m_pDisplayHelp = NULL;
    }

    return S_OK;
}

STDMETHODIMP CSnapIn::Notify(LPDATAOBJECT lpDataObject, MMC_NOTIFY_TYPE event, LPARAM arg, LPARAM param)
{
    HRESULT hr = S_OK;


    switch(event)
    {
    case MMCN_COLUMNS_CHANGED:
        hr = S_OK;
        break;

    case MMCN_DBLCLICK:
        hr = S_FALSE;
        break;

    case MMCN_ADD_IMAGES:
        HBITMAP hbmp16x16;
        HBITMAP hbmp32x32;

        hbmp16x16 = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_16x16));

        if (hbmp16x16)
        {
            hbmp32x32 = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_32x32));

            if (hbmp32x32)
            {
                LPIMAGELIST pImageList = (LPIMAGELIST) arg;

                // Set the images
                pImageList->ImageListSetStrip(reinterpret_cast<LONG_PTR *>(hbmp16x16),
                                                  reinterpret_cast<LONG_PTR *>(hbmp32x32),
                                                  0, RGB(255, 0, 255));

                DeleteObject(hbmp32x32);
            }

            DeleteObject(hbmp16x16);
        }
        break;

    case MMCN_SHOW:
        if (arg == TRUE)
        {
            RESULTDATAITEM resultItem;
            LPGPEDATAOBJECT pGPEDataObject;
            MMC_COOKIE cookie;
            INT i;
            LPCONSOLE2 lpConsole2;

            //
            // Get the cookie of the scope pane item
            //

            hr = lpDataObject->QueryInterface(IID_IGPEDataObject, (LPVOID *)&pGPEDataObject);

            if (FAILED(hr))
                return S_OK;

            hr = pGPEDataObject->GetCookie(&cookie);

            pGPEDataObject->Release();     // release initial ref
            if (FAILED(hr))
                return S_OK;


            //
            // Prepare the view
            //

            m_pHeader->InsertColumn(0, m_column1, LVCFMT_LEFT, m_nColumnSize);
            m_pResult->SetViewMode(m_lViewMode);


            //
            // Add result pane items for this node
            //

            for (i = 0; i < g_NameSpace[cookie].cResultItems; i++)
            {
                resultItem.mask = RDI_STR | RDI_IMAGE | RDI_PARAM;
                resultItem.str = MMC_CALLBACK;
                resultItem.nImage = g_NameSpace[cookie].pResultItems[i].iImage;
                resultItem.lParam = (LPARAM) &g_NameSpace[cookie].pResultItems[i];
                m_pResult->InsertItem(&resultItem);
            }

            m_pResult->Sort(0, 0, -1);

            if (m_bExpand)
            {
                hr = m_pConsole->QueryInterface(IID_IConsole2,
                                reinterpret_cast<void**>(&lpConsole2));

                if (SUCCEEDED(hr))
                {
                    if (m_pcd->m_hRoot)
                    {
                        lpConsole2->Expand(m_pcd->m_hRoot, TRUE);
                    }

                    if (m_pcd->m_hMachine)
                    {
                        lpConsole2->Expand(m_pcd->m_hMachine, TRUE);
                    }

                    if (m_pcd->m_hUser)
                    {
                        lpConsole2->Expand(m_pcd->m_hUser, TRUE);
                    }

                    lpConsole2->Release();
                }

                m_bExpand = FALSE;
            }
        }
        else
        {
            m_pHeader->GetColumnWidth(0, &m_nColumnSize);
            m_pResult->GetViewMode(&m_lViewMode);
        }
        break;

    case MMCN_SELECT:

        if (m_pConsoleVerb)
        {
            LPRESULTITEM pItem;
            LPGPEDATAOBJECT pGPEDataObject;
            DATA_OBJECT_TYPES type;
            MMC_COOKIE cookie;

            //
            // Set the default verb to open
            //

            m_pConsoleVerb->SetDefaultVerb(MMC_VERB_OPEN);


            //
            // See if this is one of our items.
            //

            hr = lpDataObject->QueryInterface(IID_IGPEDataObject, (LPVOID *)&pGPEDataObject);

            if (FAILED(hr))
                break;

            pGPEDataObject->GetType(&type);
            pGPEDataObject->GetCookie(&cookie);

            pGPEDataObject->Release();


            //
            // If this is a result pane item or the root of the namespace
            // nodes, enable the Properties menu item
            //

            if (m_pcd->m_pGPO)
            {
                if ((type == CCT_RESULT) || ((type == CCT_SCOPE) && (cookie == 0)))
                {
                    m_pConsoleVerb->SetVerbState(MMC_VERB_PROPERTIES, ENABLED, TRUE);


                    //
                    // If this is a result pane item, then change the default
                    // verb to Properties.
                    //

                    if (type == CCT_RESULT)
                        m_pConsoleVerb->SetDefaultVerb(MMC_VERB_PROPERTIES);
                }
            }
        }
        break;

    case MMCN_CONTEXTHELP:
        {

        if (m_pDisplayHelp)
        {
            LPOLESTR pszHelpTopic;
            LPGPEDATAOBJECT pGPEDataObject;
            MMC_COOKIE cookie;


            //
            // Get the cookie of the scope pane item
            //

            hr = lpDataObject->QueryInterface(IID_IGPEDataObject, (LPVOID *)&pGPEDataObject);

            if (FAILED(hr))
                return S_OK;

            hr = pGPEDataObject->GetCookie(&cookie);

            pGPEDataObject->Release();     // release initial ref

            if (FAILED(hr))
                return S_OK;

            pszHelpTopic = (LPOLESTR) CoTaskMemAlloc (50 * sizeof(WCHAR));

            if (pszHelpTopic)
            {
                lstrcpy (pszHelpTopic, g_NameSpace[cookie].lpHelpTopic);
                m_pDisplayHelp->ShowTopic (pszHelpTopic);
            }
        }

        }
        break;

    default:
        hr = E_UNEXPECTED;
        break;
    }

    return hr;
}

STDMETHODIMP CSnapIn::GetDisplayInfo(LPRESULTDATAITEM pResult)
{
    if (pResult)
    {
        if (pResult->bScopeItem == TRUE)
        {
            if (pResult->mask & RDI_STR)
            {
                if (pResult->nCol == 0)
                {
                    pResult->str = g_NameSpace[pResult->lParam].szDisplayName;
                }
                else
                {
                    pResult->str = L"";
                }
            }

            if (pResult->mask & RDI_IMAGE)
            {
                if (m_pcd->m_pGPO)
                    pResult->nImage = g_NameSpace[pResult->lParam].iIcon;
                else
                    pResult->nImage = 3;
            }
        }
        else
        {
            if (pResult->mask & RDI_STR)
            {
                if (pResult->nCol == 0)
                {
                    LPRESULTITEM lpResultItem = (LPRESULTITEM)pResult->lParam;

                    if (lpResultItem->szDisplayName[0] == TEXT('\0'))
                    {
                        LoadString (g_hInstance, lpResultItem->iStringID,
                                    lpResultItem->szDisplayName,
                                    MAX_DISPLAYNAME_SIZE);
                    }

                    pResult->str = lpResultItem->szDisplayName;
                }

                if (pResult->str == NULL)
                    pResult->str = (LPOLESTR)L"";
            }
        }
    }

    return S_OK;
}

STDMETHODIMP CSnapIn::QueryDataObject(MMC_COOKIE cookie, DATA_OBJECT_TYPES type, LPDATAOBJECT *ppDataObject)
{
    return m_pcd->QueryDataObject(cookie, type, ppDataObject);
}


STDMETHODIMP CSnapIn::GetResultViewType(MMC_COOKIE cookie, LPOLESTR *ppViewType,
                                        long *pViewOptions)
{
    return S_FALSE;
}

STDMETHODIMP CSnapIn::CompareObjects(LPDATAOBJECT lpDataObjectA, LPDATAOBJECT lpDataObjectB)
{
    HRESULT hr = S_FALSE;
    LPGPEDATAOBJECT pGPEDataObjectA, pGPEDataObjectB;
    MMC_COOKIE cookie1, cookie2;


    if (lpDataObjectA == NULL || lpDataObjectB == NULL)
        return E_POINTER;

    //
    // QI for the private GPODataObject interface
    //

    if (FAILED(lpDataObjectA->QueryInterface(IID_IGPEDataObject,
                                            (LPVOID *)&pGPEDataObjectA)))
    {
        return S_FALSE;
    }


    if (FAILED(lpDataObjectB->QueryInterface(IID_IGPEDataObject,
                                            (LPVOID *)&pGPEDataObjectB)))
    {
        pGPEDataObjectA->Release();
        return S_FALSE;
    }

    pGPEDataObjectA->GetCookie(&cookie1);
    pGPEDataObjectB->GetCookie(&cookie2);

    if (cookie1 == cookie2)
    {
        hr = S_OK;
    }


    pGPEDataObjectA->Release();
    pGPEDataObjectB->Release();

    return hr;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// CSnapIn object implementation (IExtendContextMenu)                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CSnapIn::AddMenuItems(LPDATAOBJECT piDataObject,
                                   LPCONTEXTMENUCALLBACK pCallback,
                                   LONG *pInsertionAllowed)
{
    return m_pcd->AddMenuItems (piDataObject, pCallback, pInsertionAllowed);
}

STDMETHODIMP CSnapIn::Command(LONG lCommandID, LPDATAOBJECT piDataObject)
{

    return m_pcd->Command (lCommandID, piDataObject);
}
