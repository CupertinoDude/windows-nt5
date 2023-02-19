//+----------------------------------------------------------------------------
//
//  Windows NT Directory Service Property Pages
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 2001
//
//  File:       user.cxx
//
//  Contents:   AD user shell property pages
//
//  History:    05-May-97 EricB created
//
//-----------------------------------------------------------------------------

#include "pch.h"
#include "proppage.h"
#include "objlist.h"
#include "user.h"
//#include "group.h"
#ifndef UNICODE
#   include <stdio.h>
#endif

//+----------------------------------------------------------------------------
//
//  Function:   CountryCode
//
//  Synopsis:   Handles the Country combo box to get/set the Country-Code
//              (LDAP display name: countryCode) numeric ISO-3166 code.
//
//  Notes:      This attr function MUST be called after CountryName. It
//              relies on CountryName populating the combobox and setting its
//              item data values.
//
//-----------------------------------------------------------------------------
HRESULT
CountryCode(CDsPropPageBase * pPage, PATTR_MAP pAttrMap,
            PADS_ATTR_INFO pAttrInfo, LPARAM lParam, PATTR_DATA pAttrData,
            DLG_OP DlgOp)
{
    switch(DlgOp)
    {
    case fOnCommand:
        if (CBN_SELCHANGE == lParam)
        {
            pPage->SetDirty();
            PATTR_DATA_SET_DIRTY(pAttrData);
        }
        break;

    case fApply:
        DBG_OUT("CountryCode: fApply");

        if (!PATTR_DATA_IS_WRITABLE(pAttrData) || !PATTR_DATA_IS_DIRTY(pAttrData))
        {
            return ADM_S_SKIP;
        }

        int iSel = (int)SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                           CB_GETCURSEL, 0, 0);
        if (iSel < 0)
        {
            pAttrInfo->dwControlCode = ADS_ATTR_CLEAR;
        }
        else
        {
            INT_PTR pCur = SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                              CB_GETITEMDATA, iSel, 0);
            if (pCur == LB_ERR)
            {
                DWORD dwErr = GetLastError();
                CHECK_WIN32_REPORT(dwErr, pPage->GetHWnd(), return HRESULT_FROM_WIN32(dwErr););
            }

            PDsCountryCode pCountryCode = (PDsCountryCode)pCur;

            PADSVALUE pADsValue;
            pADsValue = new ADSVALUE;
            CHECK_NULL_REPORT(pADsValue, pPage->GetHWnd(), return E_OUTOFMEMORY);

            pAttrInfo->pADsValues = pADsValue;
            pAttrInfo->dwNumValues = 1;
            pAttrInfo->dwControlCode = ADS_ATTR_UPDATE;
            pADsValue->dwType = pAttrInfo->dwADsType;
            pADsValue->Integer = pCountryCode->wCode;
        }
        break;
    }

    return S_OK;
}

//+----------------------------------------------------------------------------
//
//  Function:   CountryName
//
//  Synopsis:   Handles the Country combo box/static control to get/set the
//              Country-Name (LDAP display name: c) 2 character ISO-3166 code.
//
//  Notes:      If the control is read-only, then assume it is a static text
//              control (or a read-only edit control) rather than a combobox.
//              Also, if read-only, then only the fInit should be called.
//
//-----------------------------------------------------------------------------
HRESULT
CountryName(CDsPropPageBase * pPage, PATTR_MAP pAttrMap,
            PADS_ATTR_INFO pAttrInfo, LPARAM lParam, PATTR_DATA pAttrData,
            DLG_OP DlgOp)
{
    PWSTR pwsz = NULL;
    DWORD dwErr = 0;
    INT_PTR pCur = NULL;
    PDsCountryCode pCountryCode = NULL;
    int iSel = 0, iCur = -1, cxExtent = 0;
#ifdef UNICODE
    CStrW strFirstCode, strLastCode, strCodeLine, strCurName;
#else
    CStr strFirstCode, strLastCode, strCodeLine, strCurName;
#endif
    CStrW str2CharAbrev;

    switch (DlgOp)
    {
    case fInit:
        dspAssert(pAttrData);
        if (!pAttrMap->fIsReadOnly && !PATTR_DATA_IS_WRITABLE(pAttrData))
        {
            EnableWindow(GetDlgItem(pPage->GetHWnd(), pAttrMap->nCtrlID),
                         FALSE);
            //
            // Remove the accelerator from the label since a disabled control
            // is not reachable.
            //
            CStr strLabel;
            strLabel.LoadString(g_hInstance, IDS_COUNTRY_LABEL_NO_ACCEL);
            SetDlgItemText(pPage->GetHWnd(), IDC_COUNTRY_LABEL, strLabel.GetBuffer(0));
        }
		// fall through...
    case fObjChanged:
      {
        DBG_OUT("CountryName: fInit");
        PTSTR ptzFullName = NULL;
        WORD wCode = 0;
        HDC hDC = NULL;
        unsigned long ulFirstCode, ulLastCode, i;

        strFirstCode.LoadString(g_hInstance, IDS_FIRST_COUNTRY_CODE);
        strLastCode.LoadString(g_hInstance, IDS_LAST_COUNTRY_CODE);

        if (strFirstCode.IsEmpty() || strLastCode.IsEmpty())
        {
            ERR_MSG(IDS_ERR_COUNTRY_DATA_BAD, pPage->GetHWnd());
            return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
        }

        ulFirstCode = _tcstoul(strFirstCode, NULL, 10);
        ulLastCode = _tcstoul(strLastCode, NULL, 10);

        if (!pAttrMap->fIsReadOnly)
        {
            hDC = GetDC(GetDlgItem(pPage->GetHWnd(), pAttrMap->nCtrlID));
        }

        PWSTR pwzCurCode;

        if (pAttrInfo && pAttrInfo->dwNumValues)
        {
            pwzCurCode = pAttrInfo->pADsValues->CaseIgnoreString;
        }
        else
        {
            pwzCurCode = NULL;
        }

        for (i = ulFirstCode; i <= ulLastCode; i++)
        {
            strCodeLine.LoadString(g_hInstance, i);

            if (strCodeLine.IsEmpty())
            {
                ERR_MSG(IDS_ERR_COUNTRY_DATA_BAD, pPage->GetHWnd());
                return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            }

            if (!GetALineOfCodes(strCodeLine.GetBuffer(1), &ptzFullName, str2CharAbrev, &wCode))
            {
                ERR_MSG(IDS_ERR_COUNTRY_DATA_BAD, pPage->GetHWnd());
                return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            }

            if (!pAttrMap->fIsReadOnly)
            {
                // If not in read-only mode, then we use a combobox from which
                // the user selects the country.
                // Insert the full name into the combobox list.
                //
                SIZE s;
                if (hDC != NULL)
                {
                  GetTextExtentPoint32(hDC, ptzFullName, static_cast<int>(_tcslen(ptzFullName)), &s);

                  if (s.cx > cxExtent)
                  {
                      cxExtent = s.cx;
                  }

                  iSel = (int)SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                                 CB_ADDSTRING, 0, (LPARAM)ptzFullName);

                  if (iSel < 0)
                  {
                      CHECK_HRESULT_REPORT(E_OUTOFMEMORY, pPage->GetHWnd(), return E_OUTOFMEMORY);
                  }

                  //
                  // Add the name codes as item data.
                  //
                  pCountryCode = new DsCountryCode;

                  CHECK_NULL_REPORT(pCountryCode, pPage->GetHWnd(), return E_OUTOFMEMORY);

                  wcscpy(pCountryCode->pwz2CharAbrev, str2CharAbrev);

                  pCountryCode->wCode = wCode;

                  if (SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                         CB_SETITEMDATA, iSel,
                                         (LPARAM)pCountryCode) == CB_ERR)
                  {
                      CHECK_HRESULT_REPORT(E_OUTOFMEMORY, pPage->GetHWnd(), return E_OUTOFMEMORY);
                  }
                }
            }
            //
            // See if the current country matches that saved on the DS object
            // (if one has been saved).
            //
            if (pwzCurCode)
            {
                if (_wcsicmp(pwzCurCode, str2CharAbrev) == 0)
                {
                    iCur = iSel;
                    strCurName = ptzFullName;
                }
            }

            if ((iCur == iSel) && pAttrMap->fIsReadOnly)
            {
                // Read-only mode means that we are using a static text
                // control. Insert the full name into the control.
                //
                SetDlgItemText(pPage->GetHWnd(), pAttrMap->nCtrlID, ptzFullName);
                break;
            }

            if (pAttrMap->fIsReadOnly)
            {
                iSel++;
            }
        }

        if (!pAttrMap->fIsReadOnly)
        {
          if (hDC != NULL)
          {
            ReleaseDC(GetDlgItem(pPage->GetHWnd(), pAttrMap->nCtrlID), hDC);
          }
          SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                             CB_SETHORIZONTALEXTENT, (WPARAM)cxExtent, 0);
          if (iCur >= 0)
          {
            iCur = (int) SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                      CB_FINDSTRINGEXACT, 0,
                                      (WPARAM)(LPCTSTR)strCurName);
            dspAssert(iCur != CB_ERR);
            SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                               CB_SETCURSEL, iCur, 0);
          }
        }
        else
        {
          if (iCur < 0)
          {
            // If iCur is still -1, then country code hasn't been set.
            //
            SetDlgItemText(pPage->GetHWnd(), pAttrMap->nCtrlID, TEXT(""));
          }
        }
        break;
      }
    case fApply:
        DBG_OUT("CountryName: fApply");

        if (!PATTR_DATA_IS_WRITABLE(pAttrData) || !PATTR_DATA_IS_DIRTY(pAttrData))
        {
            return ADM_S_SKIP;
        }

        iSel = (int)SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                       CB_GETCURSEL, 0, 0);
        if (iSel < 0)
        {
            pAttrInfo->dwControlCode = ADS_ATTR_CLEAR;
        }
        else
        {
            pCur = SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                      CB_GETITEMDATA, iSel, 0);
            if (pCur == LB_ERR)
            {
                dwErr = GetLastError();
                CHECK_WIN32_REPORT(dwErr, pPage->GetHWnd(), return HRESULT_FROM_WIN32(dwErr););
            }

            pCountryCode = (PDsCountryCode)pCur;

            if (!AllocWStr(pCountryCode->pwz2CharAbrev, &pwsz))
            {
                CHECK_HRESULT_REPORT(E_OUTOFMEMORY, pPage->GetHWnd(), return E_OUTOFMEMORY);
            }

            PADSVALUE pADsValue;
            pADsValue = new ADSVALUE;
            CHECK_NULL_REPORT(pADsValue, pPage->GetHWnd(), return E_OUTOFMEMORY);

            pAttrInfo->pADsValues = pADsValue;
            pAttrInfo->dwNumValues = 1;
            pAttrInfo->dwControlCode = ADS_ATTR_UPDATE;
            pADsValue->dwType = pAttrInfo->dwADsType;
            pADsValue->CaseIgnoreString = pwsz;
        }
        break;

    case fOnCommand:
        if (CBN_SELCHANGE == lParam)
        {
            pPage->SetDirty();
            PATTR_DATA_SET_DIRTY(pAttrData);
        }
        break;

    case fOnDestroy:
        DBG_OUT("CountryName: fOnDestroy");
        iSel = 0;
        do
        {
            pCur = SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                      CB_GETITEMDATA, iSel, 0);
            if (pCur != CB_ERR)
            {
                // Delete the itemdata string.
                //
                delete (PDsCountryCode)pCur;

                iSel++;
            }
        } while (pCur != CB_ERR);
        break;
    }

    return S_OK;
}

//+----------------------------------------------------------------------------
//
//  Function:   TextCountry
//
//  Synopsis:   Handles the Country combo box to get/set the Text-Country
//              (LDAP display name: co) 
//
//  Notes:      This attr function MUST be called after CountryName. It
//              relies on CountryName populating the combobox
//
//-----------------------------------------------------------------------------
HRESULT
TextCountry(CDsPropPageBase * pPage, PATTR_MAP pAttrMap,
            PADS_ATTR_INFO pAttrInfo, LPARAM lParam, PATTR_DATA pAttrData,
            DLG_OP DlgOp)
{
    switch(DlgOp)
    {
    case fOnCommand:
        if (CBN_SELCHANGE == lParam)
        {
            pPage->SetDirty();
            PATTR_DATA_SET_DIRTY(pAttrData);
        }
        break;

    case fApply:
        DBG_OUT("TextCountry: fApply");

        if (!PATTR_DATA_IS_WRITABLE(pAttrData) || !PATTR_DATA_IS_DIRTY(pAttrData))
        {
            return ADM_S_SKIP;
        }

        int iSel = (int)SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                           CB_GETCURSEL, 0, 0);
        if (iSel < 0)
        {
            pAttrInfo->dwControlCode = ADS_ATTR_CLEAR;
        }
        else
        {   
            LPTSTR ptz = new TCHAR[pAttrMap->nSizeLimit + 1];
            CHECK_NULL_REPORT(ptz, pPage->GetHWnd(), return E_OUTOFMEMORY);

            INT_PTR pCur = SendDlgItemMessage(pPage->GetHWnd(), pAttrMap->nCtrlID,
                                              CB_GETLBTEXT, iSel, (LPARAM) (LPCSTR) ptz );
            if (pCur == LB_ERR)
            {
                delete ptz;
                DWORD dwErr = GetLastError();
                CHECK_WIN32_REPORT(dwErr, pPage->GetHWnd(), return HRESULT_FROM_WIN32(dwErr););
            }
            
            PADSVALUE pADsValue;
            pADsValue = new ADSVALUE;
            if( pADsValue == NULL )
            {
              ReportError(E_OUTOFMEMORY,0, pPage->GetHWnd()); 
              delete ptz;
              return E_OUTOFMEMORY;
            }

            
            if (!TcharToUnicode(ptz, &pADsValue->CaseIgnoreString))
            {
              delete ptz;
              delete pADsValue;
              REPORT_ERROR(E_OUTOFMEMORY, pPage->GetHWnd());
              return E_OUTOFMEMORY;
            }

            delete ptz;
    
            pAttrInfo->pADsValues = pADsValue;
            pAttrInfo->dwNumValues = 1;
            pAttrInfo->dwControlCode = ADS_ATTR_UPDATE;
            pADsValue->dwType = pAttrInfo->dwADsType;
            
        }
        break;
    }

    return S_OK;
}

// CountryName helpers:

//+----------------------------------------------------------------------------
//
//  Function:   GetALineOfCodes
//
//  Synopsis:   Parse a line of country codes.
//
//-----------------------------------------------------------------------------
BOOL GetALineOfCodes(PTSTR ptzLine, PTSTR * pptzFullName,
                     CStrW & str2CharAbrev, LPWORD pwCode)
{
    //
    // The line is parsed from end to beginning. That way we don't need to
    // dependend on the column widths being fixed.
    //
    // The last token is the numeric code. Read it.
    //
    RemoveTrailingWhitespace(ptzLine);

    PTSTR ptzCode = _tcsrchr(ptzLine, TEXT(' '));

    if (!ptzCode)
    {
        // try tab char.
        //
        ptzCode = _tcsrchr(ptzLine, TEXT('\t'));
    }

    if (!ptzCode || (ptzCode <= ptzLine) || (_tcslen(ptzCode) < 2))
    {
        return FALSE;
    }

    *ptzCode = TEXT('\0');

    ptzCode++;

    int iScanned = _stscanf(ptzCode, TEXT("%u"), pwCode);
    dspAssert(iScanned == 1);

    //
    // The next to last token is the 3 character code. Skip it.
    //
    RemoveTrailingWhitespace(ptzLine);

    size_t nLen = _tcslen(ptzLine);

    if (3 >= nLen)
    {
        return FALSE;
    }

    ptzLine[nLen - 3] = TEXT('\0');

    //
    // The next token (moving toward the front) is the 2 character code.
    //
    RemoveTrailingWhitespace(ptzLine);

    PTSTR ptz2CharAbrev = _tcsrchr(ptzLine, TEXT(' '));

    if (!ptz2CharAbrev)
    {
        // try tab char.
        //
        ptz2CharAbrev = _tcsrchr(ptzLine, TEXT('\t'));
    }

    if (!ptz2CharAbrev || (ptz2CharAbrev <= ptzLine))
    {
        return FALSE;
    }

    *ptz2CharAbrev = TEXT('\0');

    ptz2CharAbrev++;

    if (_tcslen(ptz2CharAbrev) != 2)
    {
        return FALSE;
    }

    str2CharAbrev = ptz2CharAbrev;

    //
    // The first token is the full country name.
    //
    RemoveTrailingWhitespace(ptzLine);

    if (!_tcslen(ptzLine))
    {
        return FALSE;
    }

    *pptzFullName = ptzLine;

    return TRUE;
}

//+----------------------------------------------------------------------------
//
//  Function:   RemoveTrailingWhitespace
//
//  Synopsis:   Trailing white space is replaced by NULLs.
//
//-----------------------------------------------------------------------------
void RemoveTrailingWhitespace(PTSTR ptz)
{
    size_t nLen = _tcslen(ptz);

    while (nLen)
    {
        if (!iswspace(ptz[nLen - 1]))
        {
            return;
        }
        ptz[nLen - 1] = L'\0';
        nLen--;
    }
}

//+----------------------------------------------------------------------------
//
//  Function:   ManagerEdit
//
//  Synopsis:   Handles the manager edit control.
//
//  Notes:      The page member m_pData stores the pAttrData value whose pVoid
//              element is set to the DN of the manager. The other manager
//              attr functions can then access the manager value and can also
//              read the fWritable element.
//
//-----------------------------------------------------------------------------
HRESULT
ManagerEdit(CDsPropPageBase * pPage, PATTR_MAP pAttrMap,
            PADS_ATTR_INFO pAttrInfo, LPARAM lParam, PATTR_DATA pAttrData,
            DLG_OP DlgOp)
{
    PWSTR pwz = NULL;
    PWSTR canonical = NULL;
    HRESULT hr = S_OK;

    switch (DlgOp)
    {
    case fInit:
        dspAssert(pAttrData);
        if (pAttrInfo && pAttrInfo->dwNumValues > 0)
        {
            if (!AllocWStr(pAttrInfo->pADsValues[0].DNString, &pwz))
            {
                REPORT_ERROR(E_OUTOFMEMORY, pPage->GetHWnd());
                return E_OUTOFMEMORY;
            }
            hr = CrackName (pwz, &canonical, GET_OBJ_CAN_NAME_EX, pPage->GetHWnd());
            if (FAILED(hr))
            {
                delete pwz;
                return S_FALSE;
            }
            PTSTR ptz, ptzName;
            if (!UnicodeToTchar(canonical, &ptz))
            {
                REPORT_ERROR(E_OUTOFMEMORY, pPage->GetHWnd());
                delete pwz;
                return E_OUTOFMEMORY;
            }
            LocalFreeStringW(&canonical);
            ptzName = _tcschr(ptz, TEXT('\n'));
            dspAssert(ptzName);
            ptzName++;
            SetDlgItemText(pPage->GetHWnd(), pAttrMap->nCtrlID, ptzName);
            delete ptz;
        }
        pAttrData->pVoid = reinterpret_cast<LPARAM>(pwz);
        ((CDsTableDrivenPage *)pPage)->m_pData = reinterpret_cast<LPARAM>(pAttrData);
        break;

    case fOnCommand:
        if (EN_CHANGE == lParam)
        {
            pPage->SetDirty();
            PATTR_DATA_SET_DIRTY(pAttrData);
        }
        break;

    case fApply:
        if (!PATTR_DATA_IS_WRITABLE(pAttrData) || !PATTR_DATA_IS_DIRTY(pAttrData))
        {
            return ADM_S_SKIP;
        }

        pwz = (PWSTR)reinterpret_cast<PATTR_DATA>(((CDsTableDrivenPage *)pPage)->m_pData)->pVoid;

        if (pwz)
        {
            // make a copy cause CDsTableDrivenPage::OnApply deletes it.
            PWSTR pwzTmp;
            if (!AllocWStr(pwz, &pwzTmp))
            {
                REPORT_ERROR(E_OUTOFMEMORY, pPage->GetHWnd());
                return E_OUTOFMEMORY;
            }

            PADSVALUE pADsValue;
            pADsValue = new ADSVALUE;
            CHECK_NULL(pADsValue, return E_OUTOFMEMORY);
      
            pAttrInfo->pADsValues = pADsValue;
            pAttrInfo->dwNumValues = 1;
            pAttrInfo->dwControlCode = ADS_ATTR_UPDATE;
            pADsValue->dwType = pAttrInfo->dwADsType;
            pADsValue->CaseIgnoreString = pwzTmp;
        }
        else
        {
            pAttrInfo->pADsValues = NULL;
            pAttrInfo->dwNumValues = 0;
            pAttrInfo->dwControlCode = ADS_ATTR_CLEAR;
        }
        break;

    case fOnDestroy:
        if (reinterpret_cast<CDsTableDrivenPage *>(pPage)->m_pData)
        {
            PATTR_DATA pData = reinterpret_cast<PATTR_DATA>(((CDsTableDrivenPage *)pPage)->m_pData);
            PVOID pVoid = reinterpret_cast<PVOID>(pData->pVoid);
            DO_DEL(pVoid);
        }
        break;
    }

    return S_OK;
}

//+----------------------------------------------------------------------------
//
//  Function:   DirectReportsList
//
//  Synopsis:   Handles the User Organisation Direct Reports list.
//
//-----------------------------------------------------------------------------
HRESULT
DirectReportsList(CDsPropPageBase * pPage, PATTR_MAP pAttrMap,
                  PADS_ATTR_INFO pAttrInfo, LPARAM lParam, PATTR_DATA,
                  DLG_OP DlgOp)
{
  //
  // Multi-select will result in a return at this point
  //
  if (pPage->GetObjPathName() == NULL)
  {
    return S_OK;
  }

  switch (DlgOp)
  {
    case fInit:
      {
        HRESULT hr;
        Smart_PADS_ATTR_INFO spAttrs;
        DWORD cAttrs = 0;
        PWSTR rgpwzAttrNames[] = {pAttrMap->AttrInfo.pszAttrName};
        CComPtr <IDirectoryObject> spGcObj;

        hr = BindToGCcopyOfObj(pPage, pPage->GetObjPathName(), &spGcObj);

        if (SUCCEEDED(hr))
        {
          hr = spGcObj->GetObjectAttributes(rgpwzAttrNames, 1, &spAttrs, &cAttrs);

          if (SUCCEEDED(hr))
          {
            //
            // If the bind to the GC was successful, use those results.
            // Otherwise, use the results of the local object read.
            //
            if (!cAttrs)
            {
              return S_OK;
            }
            pAttrInfo = spAttrs;
          }
        }

        if (!pAttrInfo)
        {
          return S_OK;
        }

        for (DWORD i = 0; i < pAttrInfo->dwNumValues; i++)
        {
          PWSTR pwzDns;
          hr = CrackName(pAttrInfo->pADsValues[i].DNString, &pwzDns,
                         GET_OBJ_CAN_NAME_EX, pPage->GetHWnd());

          CHECK_HRESULT(hr, return hr);

          PTSTR ptz, ptzName;
          if (!UnicodeToTchar(pwzDns, &ptz))
          {
            LocalFreeStringW(&pwzDns);
            REPORT_ERROR(E_OUTOFMEMORY, pPage->GetHWnd());
            return E_OUTOFMEMORY;
          }
          LocalFreeStringW(&pwzDns);
          ptzName = _tcschr(ptz, TEXT('\n'));
          dspAssert(ptzName);
          ptzName++;
          LRESULT lresult = SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST, LB_ADDSTRING,
                                               0, (LPARAM)ptzName);
          if (lresult != LB_ERR)
          {
            PWSTR pwzDN = new WCHAR[wcslen(pAttrInfo->pADsValues[i].DNString) + 1];
            if (pwzDN != NULL)
            {
              wcscpy(pwzDN, pAttrInfo->pADsValues[i].DNString);
              SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST,
                                 LB_SETITEMDATA, lresult, (LPARAM)pwzDN);
            }
          }
          delete ptz;
        }
      }
      break;
    
    case fOnCommand:
      {
        if (lParam == LBN_DBLCLK)
        {
          //
          // Retrieve the current selection
          //
          PWSTR pwzDN = NULL;
          LRESULT lresult = SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST,
                                               LB_GETCURSEL, 0, 0);

          if (lresult != LB_ERR)
          {
            //
            // Get the DN associated with the item
            //
            lresult = SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST,
                                         LB_GETITEMDATA, lresult, 0);
            if (lresult != LB_ERR)
            {
              pwzDN = (PWSTR)lresult;
              if (pwzDN != NULL)
              {
                //
                // Launch the secondary proppages
                //
                PostPropSheet(pwzDN, pPage);
              }
            }
          }
        }
      }
      break;
    
    case fOnDestroy:
      {
        //
        // Must free the memory associated with the list box
        //
        LRESULT lresult = SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST,
                                             LB_GETCOUNT, 0, 0);
        if (lresult != LB_ERR)
        {
          for (LRESULT idx = lresult - 1 ; idx >= 0; idx--)
          {
            lresult = SendDlgItemMessage(pPage->GetHWnd(), IDC_REPORTS_LIST,
                                         LB_GETITEMDATA, idx, 0);
            if (lresult != LB_ERR)
            {
              PWSTR pwzDN = (PWSTR)lresult;
              if (pwzDN != NULL)
              {
                delete[] pwzDN;
              }
            }
          }
        }
      }
      break;

    default:
      break;
  }

  return S_OK;
}
