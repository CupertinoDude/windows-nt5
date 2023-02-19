//
// TXTED.CPP
// Editor for Text Objects
//
// Copyright Microsoft 1998-
//

// PRECOMP
#include "precomp.h"


/////////////////////////////////////////////////////////////////////////////
// WbTextBox


//
// This is a subclassed edit field
//

WbTextBox::WbTextBox(WbTextEditor * pEditor)
{
    OSVERSIONINFO OsData;

    m_hwnd = NULL;
    m_pfnEditPrev = NULL;

    m_MaxRect.left = 0;
    m_MaxRect.top = 0;
    m_MaxRect.right = INT_MAX;
    m_MaxRect.bottom = INT_MAX;

    ::SetRectEmpty(&m_rectErase);

    m_bInIME = FALSE;
    m_bDontEscapeThisTime = FALSE;

    // see if we need to make adjustments for NT.
    m_ptNTBooger.x = 0;
    m_ptNTBooger.y = 0;
    OsData.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if( GetVersionEx( &OsData ) )
    {
        if( OsData.dwPlatformId == VER_PLATFORM_WIN32_NT )
        {
            // NT editboxes are offset from Win95 editboxes. We
            // have to de-booger them
            m_ptNTBooger.x = 3;
        }
    }

    m_pEditor = pEditor;
}


//
// ~WbTextBox()
//
WbTextBox::~WbTextBox()
{
    if (m_hwnd != NULL)
    {
        ::DestroyWindow(m_hwnd);
        m_hwnd = NULL;
    }
}



//
// Create()
// Creates an edit field then subclasses it with our window procedure
//
BOOL WbTextBox::Create(HWND hwndParent)
{
    ASSERT(!m_hwnd);

    m_hwnd = ::CreateWindowEx(0, _T("EDIT"), NULL,
        WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN |
        ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        hwndParent, NULL, g_hInstance, NULL);

    if (!m_hwnd)
    {
        ERROR_OUT(("WbTextBox::Create failed to create edit window"));
        return(FALSE);
    }

    // Init the data
    ::SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LPARAM)this);

    // Subclass the window
    m_pfnEditPrev = (WNDPROC)::SetWindowLongPtr(m_hwnd, GWLP_WNDPROC,
        (LONG_PTR)TextWndProc);

    return(TRUE);

}



//
// TextWndProc()
// Message subclass handler for edit field
//
LRESULT CALLBACK TextWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult;
    WbTextBox * ptb;

    ptb = (WbTextBox *)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
    ASSERT(ptb != NULL);
    ASSERT(ptb->m_pfnEditPrev != NULL);

    lResult = 0;

    switch( message )
    {
        case WM_CLEAR:
        case WM_CUT:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnClearCut();
            break;

        case WM_UNDO:
        case WM_PASTE:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnUndoPaste();
            break;

        case WM_IME_STARTCOMPOSITION:
        {
            ptb->m_bInIME = TRUE;
            ptb->m_bDontEscapeThisTime = TRUE;

            // let editbox take it from here
            goto DefEditProc;
            break;
        }


        case WM_IME_CHAR:
        {
            ptb->m_bDontEscapeThisTime = FALSE;
            goto DefEditProc;
            break;
        }


        case WM_IME_ENDCOMPOSITION:
        {
            ptb->m_bInIME = FALSE;
            goto DefEditProc;
            break;
        }

        case WM_KILLFOCUS:
        {
            if (ptb->m_bInIME && g_fnImmGetContext)
            {
                HIMC hImc = g_fnImmGetContext(hwnd);
                if ((hImc != NULL) && g_fnImmNotifyIME)
                {
                    // we're loosing control, tell IME to wrap it up (bug 130)
                    g_fnImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0 );
                }
            }
            // goto DefEditProc;
            break;
        }

        case WM_CHAR:
            ptb->OnChar((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_KEYUP:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnKeyUp((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_SYSKEYDOWN:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnSysKeyDown((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_TIMER:
            ptb->OnTimer(wParam);
            break;

        case WM_MOUSEMOVE:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnMouseMove((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_LBUTTONUP:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnLButtonUp((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOVE:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            ptb->OnMove(LOWORD(lParam), HIWORD(lParam));
            break;

        default:
DefEditProc:
            lResult = ::CallWindowProc(ptb->m_pfnEditPrev, hwnd, message, wParam, lParam);
            break;
    }

    return(lResult);
}


//
// OnClearCut()
//
void WbTextBox::OnClearCut()
{
    POINT   ptCaret;
    POINT   ptPos;

    ::GetCaretPos(&ptCaret);
    m_pEditor->GetPosition(&ptPos);

    m_pEditor->m_cursorXYPos.x = ptCaret.x + ptPos.x;
    m_pEditor->m_cursorXYPos.y = ptCaret.y + ptPos.y;
    m_pEditor->m_bChanged = TRUE;

}


//
// OnUndoPaste()
//
void WbTextBox::OnUndoPaste(void)
{
    FitBox();
    AutoCaretScroll();

    m_pEditor->m_bChanged = TRUE;
}


//
// OnChar()
//
void  WbTextBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int    nFirst;
    int    nLast;
    int    nPrevNumLines;
    int    nPrevNumChars;
    LRESULT dwPosChar;
    POINT  ptTop;

    // clear ignore next escape (NM4db:456)
    m_bDontEscapeThisTime = FALSE;

    ::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&nFirst, (LPARAM)&nLast);
    dwPosChar = ::SendMessage(m_hwnd, EM_POSFROMCHAR, nFirst, 0);
    ptTop.x = (short)LOWORD(dwPosChar);
    ptTop.y = (short)HIWORD(dwPosChar);

    nPrevNumLines = (int)::SendMessage(m_hwnd, EM_GETLINECOUNT, 0, 0);
    nPrevNumChars = (int)::SendMessage(m_hwnd, EM_LINELENGTH, (WPARAM)-1, 0);

    ::CallWindowProc(m_pfnEditPrev, m_hwnd, WM_CHAR, nChar, MAKELONG(nRepCnt, nFlags));

    SetupBackgroundRepaint( ptTop,
         (nPrevNumLines != ::SendMessage(m_hwnd, EM_GETLINECOUNT, 0, 0))||
         (nPrevNumChars > ::SendMessage(m_hwnd, EM_LINELENGTH, (WPARAM)-1, 0)));

    FitBox();

    m_pEditor->m_bChanged = TRUE;
}




void WbTextBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    POINT   ptCaret;
    POINT   ptPos;

    AutoCaretScroll();

    switch( nChar )
    {
        case VK_MENU:
        case VK_SHIFT:
        case VK_CONTROL:
            break;


        case VK_DELETE:
            SelectAtLeastOne();
            ::SendMessage(m_hwnd, WM_CLEAR, 0, 0);
            break;


        case VK_ESCAPE:
            if( !m_bInIME )
            {
                if( m_bDontEscapeThisTime )
                    m_bDontEscapeThisTime = FALSE;
                else
                {
                    // End the text entry abandoning the changes
                    g_pDraw->EndTextEntry(FALSE);
                    return; // we don't exist anymore, bail out
                }
            }
            break;


        default:
            break;
    }

    ::GetCaretPos(&ptCaret);
    m_pEditor->GetPosition(&ptPos);

    m_pEditor->m_cursorXYPos.x = ptCaret.x + ptPos.x;
    m_pEditor->m_cursorXYPos.y = ptCaret.y + ptPos.y;
}





void WbTextBox::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch( nChar )
    {
        case VK_MENU:
            // ignore next escape (NM4db:456)
            m_bDontEscapeThisTime = TRUE;
            break;

        default:
            break;
    }
}




BOOL WbTextBox::FitBox( void )
{
    RECT rectErase;
    RECT crEditRect;
    RECT crEditBox;
    RECT crEditWnd;
    RECT crDrawWnd;
    int   nDeltaWidth, nDeltaHeight;
    HDC     hDC = NULL;
    HDC     hDrawDC = NULL;
    TCHAR *szBoxText;
    BOOL bNoChanges;
    POINT   ptDrawLoc;
    HFONT   hSaveFont;
    POINT  ptBitmapTopLeft;
    LPTSTR pszText;
    int    nTextLen;
    int textSize;

    bNoChanges = TRUE;

    hDC = ::GetDC(m_hwnd);
    if (hDC == NULL)
    {
        bNoChanges = TRUE;
        goto bail_out;
    }

    hDrawDC = g_pDraw->GetCachedDC();
    if (hDrawDC == NULL)
    {
        bNoChanges = TRUE;
        goto bail_out;
    }

    hSaveFont = SelectFont(hDC, m_pEditor->GetFont());

    textSize = ::GetWindowTextLength(m_hwnd);
    if (!textSize)
    {
        // Use a " " for a string, which is two characters.
        szBoxText = new TCHAR[2];
        if (!szBoxText)
        {
            ERROR_OUT(("Failed to allocate TCHAR array for empty text"));
            goto bail_out;
        }
        else
        {
            lstrcpy(szBoxText, _T(" "));
        }
    }
    else
    {
        szBoxText = new TCHAR[textSize+1];
        if (!szBoxText)
        {
            ERROR_OUT(("Failed to allocate TCHAR array for object text"));
            goto bail_out;
        }
        else
        {
            ::GetWindowText(m_hwnd, (LPTSTR)szBoxText, textSize+1);
        }
    }

    // Have to check string length for NT. crEditRect comes back from
    // DrawText    giant-positive (i.e., still equal to m_MaxRect) if szBoxText
    // is empty. This causes crEditRect to overflow to giant negative later
    // resulting in very bizare painting problems. Win95 doesn't care because
    // it returns 16bit results that look like -1 instead of giant positive.

    ::GetClientRect(g_pDraw->m_hwnd, &crDrawWnd );

    // BUG 464 -DrawText doesn't calc widths involving tabs the same way a
    //            standard edit control does so replace it with DrawTextEx
    //            using the DT_EDITCONTROL format
    crEditRect = m_MaxRect;
    pszText = szBoxText;
    nTextLen = lstrlen(szBoxText);
    ::DrawTextEx( hDC,
                  pszText, nTextLen,
                  &crEditRect,
                  DT_CALCRECT | DT_EXPANDTABS | DT_NOPREFIX | DT_EDITCONTROL,
                  NULL );

    // BUG 464 (continued) -DrawTextEx doesn't include blank-last-lines in its height
    //                        calc like DrawText does so have to add an extra line
    //                        height for blank lines to have same behavior as DrawText
    if ((nTextLen >= 2) && !IsDBCSLeadByte(pszText[nTextLen-2]) && (pszText[nTextLen-1] == _T('\n')) )
        crEditRect.bottom += m_pEditor->m_textMetrics.tmHeight;

    // NT sanity check if this still fails
    if ( ((crEditRect.right - crEditRect.left) == (m_MaxRect.right - m_MaxRect.left)) ||
         ((crEditRect.right - crEditRect.left) <= 0) )
    {
        crEditRect.right = crEditRect.left + crDrawWnd.right - crDrawWnd.left;
    }

    if ( ((crEditRect.bottom - crEditRect.top) == (m_MaxRect.bottom - m_MaxRect.top)) ||
         ((crEditRect.bottom - crEditRect.top) <= 0) )
    {
        crEditRect.bottom = crEditRect.top + crDrawWnd.bottom - crDrawWnd.top;
    }

    ::GetClientRect(m_hwnd, &crEditBox);
    ::GetWindowRect(m_hwnd, &crEditWnd);
    ::MapWindowPoints(NULL, g_pDraw->m_hwnd, (LPPOINT)&crEditWnd, 2);

    // do min size check for IME's.
    int nMinWidth;
    nMinWidth = MIN_FITBOX_CHARS*m_pEditor->m_textMetrics.tmMaxCharWidth;
    if ((crEditRect.right - crEditRect.left) < nMinWidth )
        crEditRect.right = crEditRect.left + nMinWidth;

    // must add some slop to prevent autoscroll from kicking in
    crEditRect.right += 2*m_pEditor->m_textMetrics.tmMaxCharWidth;
    crEditRect.bottom += m_pEditor->m_textMetrics.tmHeight;

    //pretend we had a right or bottom scroll
    ::OffsetRect(&crEditRect, -1, -1);

    nDeltaWidth = (crEditRect.right - crEditRect.left) - (crEditBox.right - crEditBox.left);
    if (nDeltaWidth > 0)
    {
        bNoChanges = FALSE;
        if ( crEditRect.left < 0 )
        {
            // right scroll, adjust right edge
            crEditWnd.right += nDeltaWidth;
        }
        else
        {
            // left scroll, adjust left edge
            crEditWnd.left -= nDeltaWidth;
        }
    }

    nDeltaHeight = (crEditRect.bottom - crEditRect.top) - (crEditBox.bottom - crEditBox.top);
    if (nDeltaHeight > 0)
    {
        bNoChanges = FALSE;
        if( crEditRect.left < 0 )
        {
            // bottom scroll, adjust bottom edge
            crEditWnd.bottom += nDeltaHeight;
        }
        else
        {
            // top scroll, adjust top edge
            crEditWnd.top -= nDeltaHeight;
        }
    }

    if( bNoChanges )
        goto bail_out;


    // resize
    ::MoveWindow(m_hwnd, crEditWnd.left, crEditWnd.top,
        crEditWnd.right - crEditWnd.left, crEditWnd.bottom - crEditWnd.top, TRUE );

    // update bounding box
    ::GetClientRect(m_hwnd, &crEditBox);
    ::MapWindowPoints(m_hwnd, g_pDraw->m_hwnd, (LPPOINT)&crEditBox, 2);

    ::OffsetRect(&crEditBox, g_pDraw->m_originOffset.cx+1 + m_ptNTBooger.x,
        g_pDraw->m_originOffset.cy + m_ptNTBooger.y );//+1 );
    m_pEditor->m_boundsRect = crEditBox;

bail_out:

    if (hDC != NULL )
    {
        SelectFont(hDC, hSaveFont);
        ::ReleaseDC(m_hwnd, hDC );
    }

    delete szBoxText;
    return( !bNoChanges );
}




void WbTextBox::OnTimer(UINT_PTR nIDEvent)
{
    TRACE_TIMER(("WbTextBox::OnTimer"));

    // If the text object has not been added to the page before...
    if (m_pEditor->Handle() == NULL)
    {
        // Add it now
        m_pEditor->AddToPageLast(g_pDraw->Page());
    }
    else
    {
        // Otherwise, replace the external copy
        m_pEditor->Replace();
    }
}



//
// WbTextBox::OnLButtonUp()
//
void  WbTextBox::OnLButtonUp(UINT nFlags, int x, int y)
{
    POINT   ptCaret;
    POINT   ptPos;

    ::GetCaretPos(&ptCaret);
    m_pEditor->GetPosition(&ptPos);

    m_pEditor->m_cursorXYPos.x = ptCaret.x + ptPos.x;
    m_pEditor->m_cursorXYPos.y = ptCaret.y + ptPos.y;
}




//
// WbTextBox::OnMouseMove()
//
void  WbTextBox::OnMouseMove(UINT nFlags, int x, int y)
{
    if (nFlags & MK_LBUTTON )
    {
        // we're dragging
        ::HideCaret(m_hwnd);
        AutoCaretScroll();
        ::ShowCaret(m_hwnd);
    }
}



void  WbTextBox::AutoCaretScroll( void )
{
    POINT   ptCaret;
    POINT   ptPos;

    ::GetCaretPos(&ptCaret);
    m_pEditor->GetPosition(&ptPos);

    ptCaret.x += ptPos.x;
    ptCaret.y += ptPos.y;

    g_pDraw->AutoScroll(ptCaret.x, ptCaret.y, FALSE, m_pEditor->m_cursorXYPos.x,
        m_pEditor->m_cursorXYPos.y);
    m_pEditor->m_cursorXYPos = ptCaret;
}



void WbTextBox::SetupBackgroundRepaint
(
    POINT   ptTopPaint,
    BOOL    bNumLinesChanged
)
{
    RECT    rectBox;

    ::GetClientRect(m_hwnd, &rectBox);

    if (ptTopPaint.y == -1)
    {
        ::GetCaretPos(&ptTopPaint);
    }

    rectBox.top = ptTopPaint.y;
    if( !bNumLinesChanged )
        rectBox.bottom = rectBox.top + m_pEditor->m_textMetrics.tmHeight;

    ::InvalidateRect(m_hwnd, &rectBox, TRUE );
}



//
// Selects at least one (DBCS) char if there is not a selection already.
//
void WbTextBox::SelectAtLeastOne( void )
{
    int nFirst, nLast;
    TCHAR * szBoxText;

    ::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&nFirst, (LPARAM)&nLast);
    if( nFirst == nLast )
    {
        int textSize = ::GetWindowTextLength(m_hwnd);
        szBoxText = new TCHAR[textSize + 1];
        if (!szBoxText)
        {
            ERROR_OUT(("Failed to allocate TCHAR array for object text"));
        }
        else
        {
            ::GetWindowText( m_hwnd, szBoxText, textSize+1);

            if (nFirst < textSize)
            {
                nLast++;

                if( IsDBCSLeadByte( (BYTE) szBoxText[ nFirst ] )||
                    (szBoxText[ nFirst ] == _T('\r'))
                    )
                    nLast++;

                ::SendMessage(m_hwnd, EM_SETSEL, nFirst, nLast);
            }

            delete szBoxText;
        }
    }
}



void WbTextBox::OnMove(int x, int y)
{
    if (m_pEditor->m_nLastShow == SW_SHOW)
    {
        FitBox();
        ::ShowCaret(m_hwnd);
    }
}


int WbTextBox::GetMaxCharHeight( void )
{
    return( m_pEditor->m_textMetrics.tmHeight );
}




int WbTextBox::GetMaxCharWidth( void )
{
    return( m_pEditor->m_textMetrics.tmMaxCharWidth );
}




//
// Aborts and shuts down text editor without hitting the cores.  Call this
// to shutdown editing during a lock condition (that we don't own)
//
void WbTextBox::AbortEditGently( void )
{
    RECT           boundsRect;

    // have to be carefull how we bail out of this so we
    // don't fall into an infinte exception loop (bugs 3550, 3556)
    if (g_pDraw->m_pActiveText != NULL)
    {
        delete g_pDraw->m_pActiveText;    // zap in memory original
        g_pDraw->m_pActiveText = NULL;  //  text object, loose any current
                                    //  edits
    }

    //
    //  NOTE:
    //      Can't reread the text object from cores to update
    //      boundsrect because the code that does that
    //      (DCWbGraphic::ReadExternal()) tries to lock
    //      the object and off we go into another exception
    //      loop. SO, the boundsrect we use below might
    //      not be the right size and the text might
    //      appear to be clipped when we redraw it. This
    //      will correct itself as soon as another graphic
    //      update notice comes in from DCL (the lock owner
    //      types another char). I think we have to live
    //      with that.
    //

    m_pEditor->GetBoundsRect(&boundsRect);

    // let go of core text object (see above)
    // don't unlock if handle is NULL or we will blow up the cores (bug 4621)
    if (g_pDraw->m_textEditor.Handle() != NULL )
    {
        if (g_pDraw->m_textEditor.GotLock() )
        {
            g_pDraw->m_textEditor.Unlock();
            g_pDraw->m_textEditor.ForceUpdate();
        }
    }

    g_pDraw->m_textEditor.ZapHandle();

    // shut down rejecting all edits
    g_pDraw->EndTextEntry(FALSE);

    // Redraw any altered parts of the screen
    g_pDraw->InvalidateSurfaceRect(&boundsRect);
}



//
//
// Function:    WbTextEditor
//
// Purpose:     Constructor
//
//
WbTextEditor::WbTextEditor(void)
{
    // Initialize the cursor position
    m_cursorCharPos.x = 0;
    m_cursorCharPos.y = 0;

    // set parent for editbox
    m_pEditBox = NULL;

    m_cursorXYPos.x = 0;
    m_cursorXYPos.y = 0;

    m_bFirstSetFontCall = TRUE;
    m_nLastShow = -1;
}


WbTextEditor::~WbTextEditor(void)
{
    if (m_pEditBox != NULL)
    {
        delete m_pEditBox;
        m_pEditBox = NULL;
    }
}


//
//
// Function:    SetCursorPosFromPoint
//
// Purpose:     Return the character position most closely matching a
//              given co-ordinate position in the text object.
//
//
void WbTextEditor::SetCursorPosFromPoint(POINT pointXY)
{
    int    nCharPos;

    if (::PtInRect(&m_boundsRect, pointXY))
    {
        // make point relative to editbox
        pointXY.x -= g_pDraw->m_originOffset.cx;
        pointXY.y -= g_pDraw->m_originOffset.cy;

        ::MapWindowPoints(g_pDraw->m_hwnd, m_pEditBox->m_hwnd,
            &pointXY, 1);

        ::SendMessage(m_pEditBox->m_hwnd, WM_LBUTTONDOWN, 0,
                               MAKELONG( pointXY.x, pointXY.y ) );
        ::SendMessage(m_pEditBox->m_hwnd, WM_LBUTTONUP, 0,
                               MAKELONG( pointXY.x, pointXY.y ) );


        // get char index in editbox
        nCharPos = (int)::SendMessage(m_pEditBox->m_hwnd, EM_CHARFROMPOS, 0,
            MAKELPARAM(pointXY.x, pointXY.y));
        if( nCharPos < 0 )
            return;

        // Set the new cursor char co-ordinates
        m_cursorCharPos.x = (short)LOWORD(nCharPos);
        m_cursorCharPos.y = (short)HIWORD(nCharPos);

        // Move the cursor to the new position
        GetXYPosition(m_cursorCharPos, &m_cursorXYPos);
    }
}

//
//
// Function:    GetCursorSize
//
// Purpose:     Return the cursor size for the current font
//
//
void WbTextEditor::GetCursorSize(LPSIZE lpsize)
{
    lpsize->cx = ::GetSystemMetrics(SM_CXBORDER);
    lpsize->cy = m_textMetrics.tmHeight;
}

//
//
// Function:    XYPosition
//
// Purpose:     Calculate the X,Y co-ordinates of a character position
//
//
void WbTextEditor::GetXYPosition(POINT pointChar, LPPOINT lpptXY)
{
    int     nCharIndex;
    LRESULT dwCharPos;

    nCharIndex = (int)::SendMessage(m_pEditBox->m_hwnd, EM_LINEINDEX, pointChar.y, 0)
        + pointChar.x;

    GetPosition(lpptXY);

    dwCharPos = ::SendMessage(m_pEditBox->m_hwnd, EM_POSFROMCHAR, nCharIndex, 0);

    lpptXY->x += (short)LOWORD(dwCharPos);
    lpptXY->y += (short)HIWORD(dwCharPos);
}



//
//
// Function:    Clear
//
// Purpose:     Clear the text editor
//
//
void WbTextEditor::Clear(void)
{
    RECT cEWndRect;

    // Remove all the current stored text
    strTextArray.RemoveAll();
    strTextArray.Add(_TEXT(""));

    // Reset the cursor position
    m_cursorCharPos.x = 0;
    m_cursorCharPos.y = 0;

    // clear editbox
    ::SetWindowText(m_pEditBox->m_hwnd, _TEXT(""));

    // Show that the text has not been changed
    m_bChanged = FALSE;

    // init editbox size
    m_boundsRect.right = m_boundsRect.left + 2*m_pEditBox->GetMaxCharWidth();
    m_boundsRect.bottom = m_boundsRect.top + 2*m_pEditBox->GetMaxCharHeight();

    cEWndRect = m_boundsRect;
    ::OffsetRect(&cEWndRect, -(g_pDraw->m_originOffset.cx+1 + m_pEditBox->m_ptNTBooger.x),
                          -(g_pDraw->m_originOffset.cy + m_pEditBox->m_ptNTBooger.y) );

    ::BringWindowToTop(m_pEditBox->m_hwnd);
    ::MoveWindow(m_pEditBox->m_hwnd, cEWndRect.left, cEWndRect.top,
        cEWndRect.right - cEWndRect.left, cEWndRect.bottom - cEWndRect.top,
        TRUE);
}

//
//
// Function:    New
//
// Purpose:     Clear the text editor and reset the graphic handle
//
//
BOOL WbTextEditor::New(void)
{
    // create editbox
    if (!Create())
    {
        ERROR_OUT(("Error creating drawing area window"));
        return(FALSE);
    }

    // Clear the object
    Clear();

    // Show that the text has not been changed
    m_bChanged = FALSE;

    // Reset the graphic and page handles
    m_hGraphic = NULL;
    m_hPage = WB_PAGE_HANDLE_NULL;

    return(TRUE);
}


//
//
// Function:    SetTextObject
//
// Purpose:     Attach a text object to the editor
//
//
BOOL WbTextEditor::SetTextObject(DCWbGraphicText* ptext)
{
    // create editbox
    if (!Create())
    {
        ERROR_OUT(("Error creating drawing area window"));
        return(FALSE);
    }


    // Clear and place the object
    Clear();

    // setup font
    DCWbGraphicText::SetFont( ptext->m_hFont );

    // Copy the text object's attributes to the editor
    SetText(ptext->strTextArray );
    ptext->GetBoundsRect(&m_boundsRect);

    SetColor(ptext->m_clrPenColor );

    MoveTo(m_boundsRect.left, m_boundsRect.top); // avoid kerning offset correction in Position()(bug 469)

    // Copy the page and handle of the graphic text object
    m_hPage    = ptext->m_hPage;
    m_hGraphic = ptext->m_hGraphic;

    // copy the text to the editbox
    GetText();

    // Say the text has changed so that all nodes will notice this thing
    // is locked - bug 2185
    m_bChanged = TRUE;

    return(TRUE);
}


//
//
// Function:    IsEmpty
//
// Purpose:     Return TRUE if there is no text in the object
//
//
BOOL WbTextEditor::IsEmpty(void)
{
    return(::GetWindowTextLength(m_pEditBox->m_hwnd) <= 0 );
}



void WbTextEditor::PutText(void)
{
    int nNumLines;
    int i;
    int nMaxLineLen, nLineLen;
    int nLine;
    TCHAR *cbuf;
    WbTextEditor *pThis;

    pThis = (WbTextEditor *)this; // overide const declaration

	if(m_pEditBox == NULL)
	{
		return;
	}

    nNumLines = (int)::SendMessage(m_pEditBox->m_hwnd, EM_GETLINECOUNT, 0, 0);

    // figure out buf size
    nMaxLineLen = 0;
    for (i = 0; i < nNumLines; i++)
    {
        nLine = (int)::SendMessage(m_pEditBox->m_hwnd, EM_LINEINDEX, i, 0);
        nLineLen = (int)::SendMessage(m_pEditBox->m_hwnd, EM_LINELENGTH, nLine, 0);

        if (nMaxLineLen < nLineLen)
            nMaxLineLen = nLineLen;
    }

    // make buf
    nMaxLineLen += sizeof(WORD);
    cbuf = new TCHAR[nMaxLineLen];
    if( cbuf == NULL )
    {
        ERROR_OUT(("PutText failing; couldn't allocate TCHAR array"));
        return;
    }

    //
    // copy editbox innards to textbox string
    // Again, we know in advance how many lines; use SetSize/SetAt()
    //
    strTextArray.RemoveAll();
    strTextArray.SetSize(nNumLines);

    for(i = 0; i < nNumLines; i++)
    {
        *(LPWORD)cbuf = (WORD)nMaxLineLen;
        nLineLen = (int)::SendMessage(m_pEditBox->m_hwnd, EM_GETLINE, i, (LPARAM)cbuf);
        cbuf[nLineLen] = _T('\0');
        strTextArray.SetAt(i, cbuf );
    }

    // clean up
    delete cbuf;
}




void WbTextEditor::GetText(void)
{
    int nNumLines;
    int textSize = 0;
    int i;
    TCHAR * pText = NULL;
    TCHAR * pStartText;

    nNumLines = strTextArray.GetSize();

    //
    // Calculate the buffer size we need
    //
    for (i = 0; i < nNumLines; i++ )
    {
        textSize += lstrlen(strTextArray[i]);

        if ((i + 1) < nNumLines)
            textSize += lstrlen(_T("\r\n"));
    }

    //
    // Get the lines, with \r\n separating them
    //
    pText = new TCHAR[textSize + 1];
    if (!pText)
    {
        ERROR_OUT(("GetText failing; couldn't allocate TCHAR array"));
    }
    else
    {
        // Null this out in casae textSize is 0.
        pStartText = pText;
        pStartText[0] = 0;

        for (i = 0; i < nNumLines; i++)
        {
            lstrcpy(pStartText, strTextArray[i]);
            pStartText += lstrlen(strTextArray[i]);

            if ((i + 1) < nNumLines)
            {
                lstrcpy(pStartText, _T("\r\n"));
                pStartText += lstrlen(_T("\r\n"));
            }
        }

        ::SetWindowText(m_pEditBox->m_hwnd, pText);
        delete pText;
    }
}




DWORD WbTextEditor::CalculateExternalLength( void )
{

    // stick text in underlying text object
    if( m_bChanged )
        PutText();

    // fix bounds to get rid off editbox slop
    ((WbTextEditor *)this)->DCWbGraphicText::CalculateBoundsRect();

    // tell textbox to use it in length calculation
    return( DCWbGraphicText::CalculateExternalLength() );
}




void WbTextEditor::CalculateBoundsRect( void )
{
    RECT    cEWndRect;

    ::GetClientRect(m_pEditBox->m_hwnd, &cEWndRect);
    ::MapWindowPoints(m_pEditBox->m_hwnd, g_pDraw->m_hwnd,
        (LPPOINT)&cEWndRect, 2);

    m_boundsRect = cEWndRect;
    ::OffsetRect(&m_boundsRect, g_pDraw->m_originOffset.cx+1 + m_pEditBox->m_ptNTBooger.x,
                           g_pDraw->m_originOffset.cy + m_pEditBox->m_ptNTBooger.y);//+1 );

    ::BringWindowToTop(m_pEditBox->m_hwnd);
}




// set editbox visibility
void WbTextEditor::ShowBox( int nShow )
{
    if (m_nLastShow == nShow)
    {
        if( nShow == SW_SHOW)
            ::SetFocus(m_pEditBox->m_hwnd);

        return;
    }

    m_nLastShow = nShow;

    if (nShow == SW_SHOW)
    {
        // show it
        if (m_pEditBox != NULL)
        {
            ::ShowWindow(m_pEditBox->m_hwnd, SW_SHOW);
            ::BringWindowToTop(m_pEditBox->m_hwnd);
            ::SetFocus(m_pEditBox->m_hwnd);

            ::HideCaret(m_pEditBox->m_hwnd);
            m_pEditBox->FitBox();
            ::ShowCaret(m_pEditBox->m_hwnd);
        }
    }
    else
    {
        if (m_pEditBox != NULL)
        {
            ::ShowWindow(m_pEditBox->m_hwnd, SW_HIDE);
            m_bFirstSetFontCall = TRUE;

            delete m_pEditBox;
            m_pEditBox = NULL;
        }
    }
}



//
// Create()
//
// Creates the edit box window
//
BOOL WbTextEditor::Create( void )
{
    if (m_pEditBox == NULL)
    {
        m_pEditBox = new WbTextBox(this);
        if (m_pEditBox == NULL)
        {
            ERROR_OUT(("Couldn't create edit text box"));
            return(FALSE);
        }

        if (!m_pEditBox->Create(g_pDraw->m_hwnd))
        {
            ERROR_OUT(("WbTextEditor::Create failed; can't create edit field"));
            return(FALSE);
        }
    }

    return(TRUE);
}




void WbTextEditor::MoveBy(int cx, int cy)
{
    RECT cEWndRect;

    DCWbGraphicText::MoveBy(cx, cy);

    ::GetClientRect(m_pEditBox->m_hwnd, &cEWndRect);

    if ((m_boundsRect.right - m_boundsRect.left) < (cEWndRect.right - cEWndRect.left))
        m_boundsRect.right = m_boundsRect.left + (cEWndRect.right - cEWndRect.left);

    if ((m_boundsRect.bottom - m_boundsRect.top) < (cEWndRect.bottom - cEWndRect.top))
        m_boundsRect.bottom = m_boundsRect.top + (cEWndRect.bottom - cEWndRect.top);

    cEWndRect = m_boundsRect;
    ::OffsetRect(&cEWndRect, -(g_pDraw->m_originOffset.cx+1 + m_pEditBox->m_ptNTBooger.x),
                          -(g_pDraw->m_originOffset.cy + m_pEditBox->m_ptNTBooger.y) );//+1) );

    ::BringWindowToTop(m_pEditBox->m_hwnd);
    ::MoveWindow(m_pEditBox->m_hwnd, cEWndRect.left, cEWndRect.top,
        cEWndRect.right - cEWndRect.left, cEWndRect.bottom - cEWndRect.top,
        TRUE);
}




void WbTextEditor::SetFont( LOGFONT *pLogFont, BOOL bDummy )
{
    // Ignore bDummy. Had to add that so polymorph will work correctly
    // for DCWbGraphicText::SetFont()

    // Have to delay recalc of bounding rect because editbox will have a
    // bogus (bad) font until SetFont is called. Can't call SetFont
    // before this because new font hasn't been made yet.
    DCWbGraphicText::SetFont( pLogFont, FALSE );

    ::SendMessage(m_pEditBox->m_hwnd, WM_SETFONT, (WPARAM)m_hFont, TRUE);

    // now do bounding rect
    CalculateBoundsRect();

    if( m_bFirstSetFontCall )
        m_bFirstSetFontCall = FALSE;
    else
    {
        ::HideCaret(m_pEditBox->m_hwnd);

        m_pEditBox->FitBox();
        m_pEditBox->AutoCaretScroll();

        ::ShowCaret(m_pEditBox->m_hwnd);
    }
}



void WbTextEditor::WriteExtra( PWB_GRAPHIC pHeader )
{
    PutText();

    DCWbGraphicText::WriteExtra( pHeader );
}




void WbTextEditor::SetTimer( UINT nElapse )
{
    ::SetTimer(m_pEditBox->m_hwnd, TIMER_GRAPHIC_UPDATE, nElapse, NULL);
}



void WbTextEditor::KillTimer( void )
{
    ::KillTimer(m_pEditBox->m_hwnd, TIMER_GRAPHIC_UPDATE);
}




// Resets editbox painting for a resized drawing window
void WbTextEditor::ParentResize( void )
{
    ::HideCaret(m_pEditBox->m_hwnd);
    m_pEditBox->FitBox();
    ::ShowCaret(m_pEditBox->m_hwnd);
}




void WbTextEditor::RedrawEditbox(void)
{
    ::InvalidateRect(m_pEditBox->m_hwnd, NULL, TRUE);
    m_pEditBox->FitBox();
}



