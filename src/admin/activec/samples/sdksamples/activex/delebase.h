//==============================================================;
//
//	This source code is only intended as a supplement to 
//  existing Microsoft documentation. 
//
// 
//
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
//
//
//
//==============================================================;

#ifndef _BRANCHES_H
#define _BRANCHES_H

#include <mmc.h>
#include <crtdbg.h>
#include "globals.h"
#include "resource.h"
#include "LocalRes.h"

class CComponent;
class CComponentData;

class CDelegationBase {
public:
    CDelegationBase();
    virtual ~CDelegationBase();
    
    virtual const _TCHAR *GetDisplayName(int nCol = 0) = 0;
    virtual const GUID & getNodeType() { _ASSERT(FALSE); return IID_NULL; }
    
    virtual const LPARAM GetCookie() { return reinterpret_cast<LPARAM>(this); }
    virtual const int GetBitmapIndex() = 0;
    
    virtual HRESULT GetResultViewType(LPOLESTR *ppViewType, long *pViewOptions) { return S_FALSE; }
    
public:
    virtual HRESULT AddImages(IImageList *pImageList, HSCOPEITEM hsi);
    virtual HRESULT Expand(IConsoleNameSpace *pConsoleNameSpace, IConsole *pConsole, HSCOPEITEM parent) { return S_FALSE; }
    virtual HRESULT Select(IConsole *pConsole, BOOL bScope, BOOL bSelect) { return S_FALSE; }
    virtual HRESULT Show(IConsole *pConsole, BOOL bShow, HSCOPEITEM scopeitem) { return S_FALSE; }
    virtual HRESULT ShowContextHelp(IConsole *pConsole, IDisplayHelp *m_ipDisplayHelp, LPOLESTR helpFile) { return S_FALSE; }
    
    virtual HRESULT InitOCX(IUnknown *pUnknown) { return S_FALSE; }
    
public:
    void SetComponentData(CComponentData *pComponentData) { m_pComponentData = pComponentData; }
    
    HBITMAP m_pBMapSm;
    HBITMAP m_pBMapLg;
    
protected:
    void LoadBitmaps() {
        m_pBMapSm = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDR_SMICONS));
        m_pBMapLg = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDR_LGICONS));
    }
    
    BOOL bExpanded;
    
private:
    // {2974380B-4C4B-11d2-89D8-000021473128}
    static const GUID thisGuid;
    
public:
    CComponentData *m_pComponentData;
};

#endif // _BRANCHES_H