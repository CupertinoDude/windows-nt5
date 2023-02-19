/*
 *	@doc INTERNAL
 *
 *	@module	DISPSL.CPP -- CDisplaySL class |
 *
 *		This is the Single-line display engine.  See disp.c for the base class
 *		methods and dispml.c for the Multi-line display engine.
 *	
 *	Owner:<nl>
 *		Original RichEdit code: David R. Fulmer
 *		Christian Fortini
 *		Murray Sargent
 *
 *	Copyright (c) 1995-2000, Microsoft Corporation. All rights reserved.
 */

#include "_common.h"
#include "_dispsl.h"
#include "_measure.h"
#include "_select.h"
#include "_render.h"
#include "_font.h"
#include "_dfreeze.h"

ASSERTDATA

const LONG CALC_XSCROLL_FROM_FIRST_VISIBLE = -2;

/*
 *	CDisplaySL::CDisplaySL
 *
 *	Purpose	
 *		Constructor
 */
CDisplaySL::CDisplaySL ( CTxtEdit* ped )
  : CDisplay( ped )
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::CDisplaySL");

	Assert(!_fMultiLine);
}

/*
 *	CDisplaySL::Init()
 *
 *	@mfunc	
 *		Init this display for the screen
 *
 *	@rdesc
 *		TRUE iff initialization succeeded
 */
BOOL CDisplaySL::Init()
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::Init");

	// Initialize our base class
	if(!CDisplay::Init())
		return FALSE;

    SetWordWrap(FALSE);
    return TRUE;
}

/*
 *	CDisplaySL::InitVars()
 *
 */
void CDisplaySL::InitVars()
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::InitVars");

    _upScroll = 0;
	SetCpFirstVisible(0);
}

/*
 *	CDisplaySL::RecalcView(fUpdateScrollBars, prc)
 *
 *	@mfunc
 *		Recalc all lines breaks and update first visible line
 *
 *	@rdesc
 *		TRUE if success
 */
BOOL CDisplaySL::RecalcView(
	BOOL	fUpdateScrollBars,	//@param TRUE - update scroll bars
	RECTUV* prc)
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::RecalcView");
	LONG dupOld = _dup;

    if(!RecalcLine())
        return FALSE;

    if(_fViewChanged)
    {
		if(IsActive() || _dup <= GetDupView())
		{
			_upScroll = 0;
			SetCpFirstVisible(0);
		}
		else if(CALC_XSCROLL_FROM_FIRST_VISIBLE == _upScroll)
		{
			// In this case we want to set our upScroll by a visible. The
			// only way to get here is if the active view has been cloned
			// for displaying an inactive view.
 			_upScroll = 0;				// Assume that first visible is 0
			if(GetFirstVisibleCp())		// Check first visible
			{
				CMeasurer me(this);		// Start at cp 0
				me.NewLine(*this);		// Measure from there to where we are
		
				// Scroll is length to character
				_upScroll = CLine::UpFromCch(me, GetFirstVisibleCp(), TA_TOP);
			}
		}
		if(fUpdateScrollBars)
       		UpdateScrollBar(SB_HORZ, TRUE);

		_fViewChanged = FALSE;
    }

	// We only resize if width of single line control has changed.
	if(_dup != dupOld)
	{
		if (FAILED(RequestResize()))
			_ped->GetCallMgr()->SetOutOfMemory();

		else if (prc && _ped->_fInOurHost) /*bug fix# 5830, forms3 relies on old behavior*/
			_ped->TxGetClientRect(prc);
	}
    return TRUE;
}

/*
 *	CDisplaySL::RecalcLine()
 *
 *	@mfunc
 *		Recalculate a line
 *
 *	@rdesc
 *		TRUE if success <nl>
 *		FALSE if failure <nl>
 */
BOOL CDisplaySL::RecalcLine()
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::RecalcLine");

    Assert( _ped );

	// Create a measurer starting at cp = 0
	CMeasurer me(this);

	LONG dupOld = _dup;
	BOOL fMeasured = CLine::Measure(me, MEASURE_FIRSTINPARA);

	if(!fMeasured)
	{
		Assert(FALSE);						// Should succeed
	    InitVars();
	    return FALSE;
	}

	_fNeedRecalc = FALSE;
	_fRecalcDone = TRUE;

	if(dupOld != _dup)
		_fViewChanged = TRUE;

	_fLineRecalcErr = FALSE;
	return fMeasured;
}

/*
 *	CDisplaySL::Render(rcView, rcRender)
 *
 *	@mfunc	
 *		Renders this line
 */
void CDisplaySL::Render(
	const RECTUV &rcView,	//@parm View RECT
	const RECTUV &rcRender)	//@parm RECT to render (must be contained in
							//		client rect)
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::Render");
    POINTUV	pt;

	_fRectInvalid = FALSE;

	CRenderer re(this);

    if(!re.StartRender(rcView, rcRender))
        return;

    // Set renderer at top/left of view rect
    pt.u = rcView.left - _upScroll;
    pt.v = rcView.top;
    re.SetCurPoint(pt);
    // Renderer is set at cp = 0 at the moment
	CLine::Render(re, TRUE);
	re.EndRender(this, this, 0, pt);

	if(_fOffscreenOnce)
		_fUseOffscreenDC = _fOffscreenOnce = FALSE;

    // If line metrics are not yet up to date, get them from renderer
    if(_dup == -1)
    {
        _dup			= re._li._dup;
        _dvpHeight		= re._li._dvpHeight;
        _dvpDescent		= re._li._dvpDescent;
    }
}

/*
 *	CDisplaySL::WaitForRecalcIli(ili)
 *
 *	@mfunc
 *		Wait until line array is recalculated up to line <p ili>
 *
 *	@rdesc
 *		Returns TRUE if lines were recalc'd up to ili (TRUE if ili == 0)
 */
BOOL CDisplaySL::WaitForRecalcIli (
	LONG ili)		//@parm Line index to recalculate line array up to
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::WaitForRecalcIli");

    return ili == 0;
}

/*
 *	CDisplaySL::GetScrollRange(nBar)
 *
 *	@mfunc
 *		Returns the max part of a scrollbar range for scrollbar <p nBar>
 *
 *	@rdesc
 *		LONG max part of scrollbar range
 */
LONG CDisplaySL::GetScrollRange(
	INT nBar) const		//@parm Scroll bar to interrogate (SB_VERT or SB_HORZ)
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetScrollRange");

    Assert( IsMain() );

	LONG lRange = 0; 

    if(nBar != SB_VERT && _fUScrollEnabled)
    {
	    if(_ped->TxGetScrollBars() & WS_HSCROLL)
		{
			lRange = max(0, _dup + _ped->GetCaretWidth());
			lRange = min(lRange, _UI16_MAX);
		}
    }
	return lRange;
}

/*
 *	CDisplaySL::UpdateScrollBar(nBar, fUpdateRange)
 *
 *	@mfunc
 *		Update horizontal scroll bar
 *		Also figure whether the scroll bar should be visible or not
 *
 *	@rdesc
 *		BOOL
 */
BOOL CDisplaySL::UpdateScrollBar (
	INT	 nBar,				//@parm Which scrollbar: SB_HORZ or SB_VERT
	BOOL fUpdateRange)		//@parm Should range be recomputed and updated
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::UpdateScrollBar");

	// Note: In the old days we didn't allow autosize & scroll bars, so to keep
	// forms working, we need this special logic with respect to autosize.
	if (!IsActive() || SB_VERT == nBar ||
		!_ped->fInOurHost() && _ped->TxGetAutoSize())
	{
		// Scroll bars are only updated on active views.
		return FALSE;
	}

	const DWORD dwScrollBars = _ped->TxGetScrollBars();
	const BOOL fHide = !(dwScrollBars & ES_DISABLENOSCROLL);
	BOOL fReturn = FALSE;
	BOOL fEnabled = TRUE;
	BOOL fEnabledOld = FALSE;
	LONG lScroll = 0;
	CTxtSelection *psel = _ped->GetSelNC();

	// Get scrolling position
	if(nBar == SB_HORZ)
	{
		if(!(dwScrollBars & WS_HSCROLL))
		{
			// even if we don't have scrollbars, we may allow horizontal
			// scrolling.

			if(!_fUScrollEnabled)
				_fUScrollEnabled = !!(dwScrollBars & ES_AUTOHSCROLL);
			return FALSE;
		}

		fEnabledOld = _fUScrollEnabled;
		lScroll = ConvertUPosToScrollPos(_upScroll);

        if(_dup <= _dupView)
            fEnabled = FALSE;
	}

	// !s beforehand because all true values aren't necessarily equal
	if(!fEnabled != !fEnabledOld)
	{
		if(_fDeferUpdateScrollBar)
			_fUpdateScrollBarDeferred = TRUE;
		else
		{
			if (nBar == SB_HORZ)
				_fUScrollEnabled = fEnabled;
			else
				_fVScrollEnabled = fEnabled;
		}

		if(!_fDeferUpdateScrollBar)
		{
    		if(!fHide)					// Don't hide scrollbar, just disable
    			_ped->TxEnableScrollBar(nBar, fEnabled ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
    		else 
    		{
    			fReturn = TRUE;
    			// Make sure to hide caret before showing scrollbar
    			if(psel)
    				psel->ShowCaret(FALSE);

    			// Hide or show scroll bar
    			_ped->TxShowScrollBar(nBar, fEnabled);

    			if(psel)
    				psel->ShowCaret(TRUE);
            }
		}
	}
	
	// Set scrollbar range and thumb position
	if(fEnabled)
	{
        if(fUpdateRange)
        {
			if(!_fDeferUpdateScrollBar)
				_ped->TxSetScrollRange(nBar, 0, GetScrollRange(nBar), FALSE);
        }
        
		if(_fDeferUpdateScrollBar)
			_fUpdateScrollBarDeferred = TRUE;
		else
			_ped->TxSetScrollPos(nBar, lScroll, TRUE);
	}
	else if (!_fDeferUpdateScrollBar)
	{
		// This turns off the scroll bar and only needs to happen when a change 
		// occurs so we can count on the change in state check above to set
		// _fUpdateScrollBarDeferred.
		if (!fEnabled && fEnabledOld)
			_ped->TxSetScrollRange(nBar, 0, 0, FALSE);
	}
	return fReturn;
}

BOOL CDisplaySL::IsMain() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::IsMain");

    return TRUE;
}

LONG CDisplaySL::GetDupLineMax() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetWidth");

    return CLine::_dup;
}

LONG CDisplaySL::GetHeight() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetHeight");

    return CLine::_dvpHeight;
}

LONG CDisplaySL::GetResizeHeight() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetResizeHeight");

    return CLine::_dvpHeight;
}

LONG CDisplaySL::LineCount() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::LineCount");

    return 1; 
}

/*
 *	CDisplaySL::GetCliVisible()
 *
 *	@mfunc
 *		Get count of visible lines and update GetCp()MostVisible for PageDown()
 *
 *	@rdesc
 *		count of visible lines
 */
LONG CDisplaySL::GetCliVisible (
	LONG* pcpMostVisible,				//@parm Returns cpMostVisible
	BOOL fLastCharOfLastVisible) const 	//@parm Want cp of last visible char
										// (ignored here).
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetCliVisible");

    if (pcpMostVisible)
        *pcpMostVisible = CLine::_cch;
    
    return 1;
}

LONG CDisplaySL::GetFirstVisibleLine() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetFirstVisibleLine");

    return 0;
}

/*
 *	CDisplaySL::GetLineText(ili, pchBuff, cchMost)
 *
 *	@mfunc
 *		Copy given line of this display into a character buffer
 *
 *	@rdesc
 *		number of character copied
 */
LONG CDisplaySL::GetLineText (
	LONG	ili,		//@parm Line to get text of
	TCHAR *	pchBuff,	//@parm Buffer to stuff text into
	LONG	cchMost)	//@parm Length of buffer
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetLineText");

    if(ili == 0)
    {
        cchMost = min(cchMost, _ped->GetTextLength());
        if(cchMost > 0)
        {
            CTxtPtr tp(_ped, 0);
            return tp.GetText( cchMost, pchBuff );
        }
    }
    return 0;
}

/*
 *	CDisplaySL::CpFromLine(ili, pdvp)
 *
 *	@mfunc
 *		Computes cp at start of given line 
 *		(and top of line position relative to this display)
 *
 *	@rdesc
 *		cp of given line; here always 0
 */
LONG CDisplaySL::CpFromLine (
	LONG ili,		//@parm Line we're interested in (if <lt> 0 means caret line)
	LONG *pdvp)		//@parm Returns top of line relative to display 
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::CpFromLine");
					//  	(NULL if don't want that info)
    Assert( ili == 0 );
    
    if(pdvp)
        *pdvp = 0;
    
    return 0;
}

/*
 *	CDisplaySL::LineFromCp(cp, fAtEnd)
 *
 *	@mfunc
 *		Computes line containing given cp.
 *
 *	@rdesc
 *		index of line found; here returns 0 always
 */
LONG CDisplaySL::LineFromCp(
	LONG cp,		//@parm cp to look for
	BOOL fAtEnd)	//@parm If true, return previous line for ambiguous cp
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::LineFromCp");

    return 0;
}

/*
 *	CDisplaySL::CpFromPoint(pt, prcClient, ptp, prp, fAllowEOL, pHit, pdispdim)
 *
 *	@mfunc
 *		Determine cp at given point
 *
 *	@devnote
 *      --- Use when in-place active only ---
 *
 *	@rdesc
 *		Computed cp, -1 if failed
 */
LONG CDisplaySL::CpFromPoint(
	POINTUV		pt,			//@parm Point to compute cp at (client coords)
	const RECTUV *prcClient,//@parm Client rectangle (can be NULL if active).
	CRchTxtPtr * const ptp,	//@parm Returns text pointer at cp (may be NULL)
	CLinePtr * const prp,	//@parm Returns line pointer at cp (may be NULL)
	BOOL		fAllowEOL,	//@parm Click at EOL returns cp after CRLF
	HITTEST *	phit,		//@parm Out parm for hit-test value
	CDispDim *	pdispdim,	//@parm Out parm for display dimensions
	LONG	   *pcpActual,	//@parm Out cp that pt is above
	CLine *		pliParent)	//@parm Parent pli for table row displays
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::CpFromPoint");

    RECTUV  rcView;
	LONG cp = 0;
	LONG cch = 0;

	HITTEST		hit = HT_Nothing;

    GetViewRect(rcView, prcClient);

	if (pt.v >= rcView.top && pt.v <= rcView.bottom)
	{

		// Create measurer at cp(0)
		CMeasurer me(this);

		// Get character in the line
		pt.u += _upScroll - rcView.left;		// Transform to galley coordinates
		pt.v -= rcView.top;
		cch = CLine::CchFromUp(me, pt, pdispdim, &hit, pcpActual);

		// Don't allow click at EOL to select EOL marker and take into account
		// single line edits as well
		if(!fAllowEOL && cch == CLine::_cch && CLine::_cchEOP)
			me._rpTX.BackupCRLF();

		cp = me.GetCp();
	}

	if (phit)
		*phit = hit;
    if(ptp)
        ptp->SetCp(cp);
    if(prp)
        prp->Set(0, cch);
 
 	return cp;   
}

/*
 *	CDisplaySL::PointFromTp(tp, fAtEnd, pt, prp, taMode, pdx)
 *
 *	@mfunc
 *		Determine coordinates at given tp
 *
 *	@devnote
 *      --- Use when in-place active only ---
 *
 *	@rdesc
 *		line index at cp, -1 if error
 */
LONG CDisplaySL::PointFromTp(
	const CRchTxtPtr &rtp,	//@parm Text ptr to get coordinates at
	const RECTUV *prcClient,//@parm Client rectangle (can be NULL if active).
	BOOL		fAtEnd,		//@parm Return end of previous line for ambiguous cp
	POINTUV &		pt,		//@parm Returns point at cp in client coords
	CLinePtr * const prp,	//@parm Returns line pointer at tp (may be null)
	UINT		taMode,		//@parm Text Align mode: top, baseline, bottom
	CDispDim *	pdispdim)	//@parm Out parm for display dimensions
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::PointFromTp");

	LONG	 dy = 0;
    RECTUV	 rcView;
    CLinePtr rp(this);

    if(!rp.SetCp(rtp.GetCp(), fAtEnd))
        return -1;

    AssertSz(_ped->_fInPlaceActive || prcClient, 
		"CDisplaySL::PointFromTp() called with invalid client rect");

    GetViewRect(rcView, prcClient);

    pt.u = rcView.left - _upScroll;
    pt.v = rcView.top;

    CMeasurer me(this, rtp);
    me.Move(-rp.GetIch());		// Backup to start of line      
    me.NewLine(*rp);				// Measure from there to where we are

	pt.u += CLine::UpFromCch(me, rp.GetIch(), taMode, pdispdim, &dy);
	pt.v += dy;

    if(prp)
        *prp = rp;
    return rp;
}

/*
 *	CDisplaySL::UpdateView(&tpFirst, cchOld, cchNew)
 *
 *	@mfunc
 *		Update visible part of display (the "view" on the screen).
 *
 *	@devnote
 *      --- Use when in-place active only ---
 *
 *	@rdesc
 *		TRUE if success
 */
BOOL CDisplaySL::UpdateView(
	CRchTxtPtr &tpFirst,	//@parm Text ptr where change happened
	LONG cchOld,			//@parm Count of chars deleted
	LONG cchNew)			//@parm Count of chars inserted
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::UpdateView");

    BOOL fUpdateScrollBarHorz = FALSE;
    BOOL fReturn = TRUE;
    RECTUV rcView;
	CTxtSelection *psel = _ped->GetSelNC();
	LONG dupOld = _dup;
	BOOL fScrollChanged = FALSE;
	RECTUV rcClient;
	RECTUV rc;
	LONG dvpOld = _dvpHeight;
                                                              
    if (_fNoUpdateView)
        return fReturn;

	AssertSz(_ped->_fInPlaceActive, "CDisplaySL::UpdateView(...) called when inactive");

    _ped->TxGetClientRect(&rcClient);
    GetViewRect(rcView, &rcClient);
    
    if(psel && !psel->PuttingChar())
        psel->ClearCchPending();

    if(!RecalcLine())
    {
        // We're in deep crap now, the recalc failed. Let's try to get
		// out of this with our head still mostly attached
        fReturn = FALSE;
    }

	// An update has occurred. Go offscreen to be flicker-free.
	if(!_fUseOffscreenDC)
		_fUseOffscreenDC = _fOffscreenOnce = TRUE;

	//If we tried to go offscreen, but the edit changed the first
	//character, it might be the WM_SETTEXT case called when dialogs
	//are instantiated, and so it doesn't make sense to go offscreen there.
	if (_fOffscreenOnce && tpFirst.GetCp() == 0)
		_fOffscreenOnce = _fUseOffscreenDC = FALSE;

    if(_dup <= _dupView)
    {
        // x scroll range is smaller than the view width
        // force x scrolling position = 0
        _upScroll = 0;
		SetCpFirstVisible(0);
		_fViewChanged = TRUE;
        fUpdateScrollBarHorz = TRUE;
    }

	_fRectInvalid = TRUE;

	// Only resize a Single Line edit control if the width changes
	if(_dup != dupOld)
	{
		if(FAILED(RequestResize()))
			_ped->GetCallMgr()->SetOutOfMemory();
	}

	// If view changed, update scroll bars
    if(_fViewChanged)
	{
		_fViewChanged = FALSE;
        fScrollChanged = UpdateScrollBar(SB_HORZ);
	}

	// Scroll bar state did not change so we need to update screen
	// Build an invalidation rectangle. 
	rc = rcClient;
	if(dvpOld == _dvpHeight)
	{
		// Height of control did not change so we can minimize the update 
		// rectangle to the height of the control.
		rc.bottom = rcView.top + _dvpHeight;
	}

	// Tell display to update when it gets a chance
	_ped->TxInvalidateRect(&rc);

    return fReturn;
}


/*
 *	CDisplaySL::ScrollView(upScroll, vpScroll, fTracking, fFractionalScroll)
 *
 *	@mfunc
 *		Scroll view to new x and y position
 *
 *	@devnote
 *		This method tries to adjust the y scroll pos before
 *		scrolling to display complete line at top. x scroll 
 *		pos is adjusted to avoid scrolling all text off the 
 *		view rectangle.
 *
 *		Must be able to handle vpScroll <gt> pdp->yHeight and vpScroll <lt> 0
 *
 *	@rdesc
 *		TRUE if actual scrolling occurred, 
 *		FALSE if no change
 */
BOOL CDisplaySL::ScrollView (
	LONG upScroll,		//@parm New x scroll position
	LONG vpScroll,		//@parm New y scroll position
	BOOL fTracking,		//@parm TRUE indicates we are tracking scrollbar thumb
	BOOL fFractionalScroll)
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::ScrollView");
	LONG dupMax;
	LONG dx = 0;
    RECTUV rcClient, rcClip;
	CTxtSelection *psel = _ped->GetSelNC();
	COleObject *pipo;

    AssertSz(_ped->_fInPlaceActive, "CDisplaySL::ScrollView() called when not in-place");
	
	if(upScroll == -1)
		return FALSE;

 	//For scrolling purposes, we clip to rcView's top and bottom, but rcClient's left and right
   	_ped->TxGetClientRect(&rcClient);
	GetViewRect(rcClip, &rcClient);
	rcClip.left = rcClient.left;
	rcClip.right = rcClient.right;
	
	// Determine horizontal scrolling pos.
	dupMax = _dup;
	upScroll = min(upScroll, dupMax);
	upScroll = max(0, upScroll);

	dx = _upScroll - upScroll;
	if(dx)
    {
		_upScroll = upScroll;

		// Calculate new first visible
		CMeasurer me(this);				// Create measurer at cp(0)
		POINTUV pt = {upScroll, 0};
		CDispDim dispdim;
		HITTEST Hit;
		LONG	cpActual;
		CLine::CchFromUp(me, pt, &dispdim, &Hit, &cpActual);
		
		SetCpFirstVisible(cpActual);	// Save character position
    }

	AssertSz(IsMain(), "CDisplaySL::ScrollView non-main SL control");

	// Now perform the actual scrolling
	if(dx)
	{
		if(!_fRectInvalid)
		{
			// Scroll only if scrolling < view dimensions and we are in-place
			// Note that we only scroll the active view and we can be in-place
			// active and have multiple inactive views.
			if(IsActive() && !IsTransparent() && dx < _dupView)
			{
				if(psel)
					psel->ShowCaret(FALSE);

				RECT rcxyClip;
				RectFromRectuv(rcxyClip, rcClip);

				_ped->TxScrollWindowEx(dx, 0, NULL, &rcxyClip);

				if(psel)
					psel->ShowCaret(TRUE);
			}
			else
				_ped->TxInvalidateRect(&rcClip);
		}

		if(psel)
			psel->UpdateCaret(FALSE);

		if(!fTracking && dx)
		{		
			_ped->SendScrollEvent(EN_HSCROLL);
			UpdateScrollBar(SB_HORZ);
		}
				
		_ped->TxUpdateWindow();

		// FUTURE: since we're now repositioning in place active 
		// objects every time we draw, this call seems to be 
		// superfluous (AndreiB)

		// Tell object subsystem to reposition any in place objects
		if(_ped->GetObjectCount())
		{
			pipo = _ped->GetObjectMgr()->GetInPlaceActiveObject();
			if(pipo)
				pipo->OnReposition();
		}
	}
	return dx;
}

/*
 *	CDisplaySL::InvertRange(cp, cch, selAction)
 *
 *	@mfunc
 *		Invert a given range on screen (for selection)
 *
 *	@devnote
 *      --- Use when in-place active only ---
 *
 *	@rdesc
 *		TRUE if success
 */
BOOL CDisplaySL::InvertRange (
	LONG	cp,					//@parm Active end of range to invert
	LONG	cch,				//@parm Signed length of range
	SELDISPLAYACTION selAction)	//@parm What we are doing to the selection
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::InvertRange");

	if(cch < 0)						//make cch negative, make cp the minimum
		cch = -cch;
	else
		cp -= cch;

#ifndef NOLINESERVICES
	if (g_pols)
		g_pols->DestroyLine(this);
#endif

	// If display is frozen, just update recalc region and move on.
	if(_padc)
	{
		AssertSz(cp >= 0, "CDisplayML::InvertRange: range (cp) goes below"
				"zero!!" );
		// Make sure these values are bounded.
		if(cp > _ped->GetTextLength())	// Don't bother updating region;
			return TRUE;				//  it's out of bounds

		if(cp + cch > _ped->GetTextLength())
			cch -= cp + cch - _ped->GetTextLength();

		_padc->UpdateRecalcRegion(cp, cch, cch);
		return TRUE;
	}

	// Ensure all visible lines are recalced
	if(!WaitForRecalcView())
		return FALSE;

	//REVIEW (keithcu) CDisplayML::InvertRange() has a different order WRT
	//frozen displays and OLE objects.
	
	// If an object is being inverted, and that is all that
	// is being inverted, delegate to the ObjectMgr.
	if (cch == 1 && _ped->GetObjectCount() &&
		(selAction == selSetNormal || selAction == selSetHiLite) )
	{
		CObjectMgr* pobjmgr = _ped->GetObjectMgr();
		CTxtPtr		tp(_ped, cp);

		if(tp.GetChar() == WCH_EMBEDDING && pobjmgr)
			pobjmgr->HandleSingleSelect(_ped, cp, selAction == selSetHiLite);
	}

	// Get view rectangle
    AssertSz(_ped->_fInPlaceActive, "CDisplaySL::InvertRange() called when not in-place active");

	_ped->TxInvalidate();
	return TRUE;
}

/*
 *	CDisplaySL::InitLinePtr (&lp)
 *
 *	@mfunc
 *		Initialize a CLinePtr properly
 */
void CDisplaySL::InitLinePtr (
	CLinePtr & lp )		//@parm Ptr to line to initialize
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::InitLinePtr");

    lp.Init(*this);
}

/*
 *	CDisplaySL::GetNaturalSize(hdcDraw, hicTarget, dwMode, pwidth, pheight)
 *
 *	@mfunc
 *		Recalculate display to input width & height
 *
 *	@rdesc
 *		S_OK - Call completed successfully <nl>
 */
HRESULT	CDisplaySL::GetNaturalSize(
	HDC	  hdcDraw,		//@parm DC for drawing
	HDC	  hicTarget,	//@parm DC for information
	DWORD dwMode,		//@parm Type of natural size required
	LONG *pwidth,		//@parm Input/output width parm 
	LONG *pheight)		//@parm	Input/output height parm
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetNaturalSize");

	// Assume this won't work
	HRESULT hr = E_FAIL;

	// Set height temporarily so zoom factor will work out
	LONG yOrigHeightClient = SetClientHeight(*pheight);

	// Adjust height and width by view inset
	LONG widthView  = *pwidth;
	LONG heightView = *pheight;
	GetViewDim(widthView, heightView);

	// Store adjustment so we can restore it to height & width
	LONG widthAdj  = *pwidth  - widthView;
	LONG heightAdj = *pheight - heightView;
	LONG uCaret = !_ped->fInOurHost() && !_ped->fInHost2() ? duCaret : 0;

	// Recalculate size needed
   	// Create a measurer starting at cp = 0
	CMeasurer me(this);
	CLine	  liNew;

	if(liNew.Measure(me, MEASURE_FIRSTINPARA))
	{
		*pwidth  = liNew._dup;
		if(dwMode == TXTNS_FITTOCONTENT2)
			*pwidth += liNew._upStart + me.GetRightIndent();
		*pheight = liNew._dvpHeight;
		hr = S_OK;
	}

	// Restore insets so output reflects true client rect needed
	*pwidth  += widthAdj + uCaret;
	*pheight += heightAdj;

	// Restore client height to match current cache
	SetClientHeight(yOrigHeightClient);
    return hr;
}

/*
 *	CDisplaySL::GetWordWrap()
 *
 *	@mfunc
 *		Gets the wrap flag  
 *
 *	@rdesc
 *		TRUE - Word wrap
 *		FALSE - No word Word wrap
 *
 *	@devnote
 *		Single line controls cannot word wrap.
 */
BOOL CDisplaySL::GetWordWrap() const
{
	TRACEBEGIN(TRCSUBSYSDISP, TRCSCOPEINTERN, "CDisplaySL::GetNoWrap");

	return FALSE;
}

/*
 *	CDisplaySL::Clone()
 *
 *	@mfunc
 *		Make a copy of this object
 *
 *	@rdesc
 *		NULL - failed
 *		CDisplay *
 */
CDisplay *CDisplaySL::Clone() const
{
	CDisplaySL *pdp = new CDisplaySL(_ped);
	if(pdp)
	{
		// Initialize our base class
		if(pdp->CDisplay::Init())
		{
			pdp->InitFromDisplay(this);

			// Setting scroll to 0 means use the first visible character
			pdp->_upScroll = CALC_XSCROLL_FROM_FIRST_VISIBLE;
			pdp->_fVScrollEnabled = _fVScrollEnabled;
			pdp->_fWordWrap = _fWordWrap;
			pdp->ResetDrawInfo(this);
			pdp->SetCpFirstVisible(GetFirstVisibleCp());

			// This can't be active view since it is a clone of some view
			pdp->SetActiveFlag(FALSE);
		}
	}
	return pdp;
}

/*
 *	CDisplaySL::GetMaxUScroll()
 *
 *	@mfunc
 *		Get the maximum u scroll value
 *
 *	@rdesc
 *		Maximum u scroll value
 *
 */
LONG CDisplaySL::GetMaxUScroll() const
{
	LONG uCaret = !_ped->fInOurHost() && !_ped->fInHost2() ? duCaret : 0;
	return _dup + uCaret;
}
