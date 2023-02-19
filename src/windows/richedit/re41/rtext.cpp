/*
 *	@doc INTERNAL
 *
 *	@module RTEXT.CPP - Rich-text ptr class |
 *
 *		This text ptr consists of a plain text ptr (_rpTX), a CCharFormat
 *		run ptr (_rpCF), and a CParaFormat run ptr (_rpPF). This module
 *		contains the methods to manipulate this combination of run ptrs
 *		consistently.
 *	
 *	Authors:<nl>
 *		RichEdit 1.0 code: David R. Fulmer
 *		Main implementation: Murray Sargent <nl>
 *		Undo and notification implementations: Alex Gounares <nl>
 *
 *	Copyright (c) 1995-2000, Microsoft Corporation. All rights reserved.
 */

#include "_common.h"
#include "_edit.h"
#include "_frunptr.h"
#include "_rtext.h"
#include "_disp.h"
#include "_select.h"
#include "_m_undo.h"
#include "_antievt.h"
#include "_objmgr.h"
#include "_txtbrk.h"

ASSERTDATA

#define DEBUG_CLASSNAME CRchTxtPtr
#include "_invar.h"

#ifdef DEBUG
/*
 *	CRchTxtPtr::Invariant
 */
BOOL CRchTxtPtr::Invariant( void ) const
{
	if (m_InvariantCheckInterval < 1 || m_InvariantCheckInterval > 10)
		const_cast<CRchTxtPtr *>(this)->m_InvariantCheckInterval = 10;

	const_cast<CRchTxtPtr *>(this)->m_InvariantCheckInterval--;

	if (m_InvariantCheckInterval)
		return TRUE;

	unsigned ch;
	LONG cch;
	LONG cchLength = GetTextLength();
	LONG cp;

	_rpTX.Invariant();
	_rpCF.Invariant();
	_rpPF.Invariant();

	if(_rpCF.IsValid())
	{
		cp  = _rpCF.CalculateCp();
		cch = _rpCF.CalcTextLength();
		Assert(GetCp() == cp && cchLength == cch);
		Assert(!_rpCF._iRun || GetPed()->IsBiDi() || _rpCF.GetRun(0)->_iFormat != _rpCF.GetRun(-1)->_iFormat);
	}

	if(_rpPF.IsValid())
	{
		cp  = _rpPF.CalculateCp();
		cch = _rpPF.CalcTextLength();
		Assert(GetCp() == cp && cchLength == cch);

		CTxtPtr	tp(_rpTX);

		tp.Move(_rpPF.GetCchLeft() - 1);
		ch = tp.GetChar();
		if(!IsEOP(ch))
		{
			_rpTX.MoveGapToEndOfBlock();			// Make it easier to see
			AssertSz(FALSE,							//  what's going on
				"CRchTxtPtr::Invariant: PF run doesn't end with EOP");
		}

#ifdef EXTREME_CHECKING
		// We don't do this check normally as it is _extremely_ slow.
		// However, it's very useful for catching para-format run problems

		// Make sure each para format run ends on a paragraph mark!
		CFormatRunPtr	rpPF(_rpPF);

		rpPF.BindToCp(0);
		tp.BindToCp(0);
		do
		{
			tp.Move(rpPF.GetRun(0)->_cch);
			if(!tp.IsAfterEOP())
			{
				AssertSz(0, "ParaFormat Run not aligned along paragraphs!");
			}
		} while( rpPF.NextRun() );
#endif // EXTREME_CHECKING
	}
	return TRUE;
}

/*
 *	CRchTxtPtr::GetParaNumber ()
 *
 *	@mfunc
 *		Return number of current paragraph in a numbered list. This is
 *		0 if the current paragraph isn't part of a list. It's 1 if it's
 *		the first paragraph in a list, 2 if it's the second, etc.
 *
 *	@rdesc
 *		paragraph number active at this rich text ptr
 *
 *	@devnote
 *		When the display is calc'd from the beginning or recalc'd from
 *		a previous valid position, the list number can be determined from
 *		the display.  But if CDisplayPrinter::FormatRange() works without
 *		a display, it needs to know the number.  This routine can be so used
 *		for this purpose and for debugging the display choices.
 */
LONG CRchTxtPtr::GetParaNumber() const
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetParaNumber");

	LONG		 ch;
	LONG		 cPara = 0;
	LONG		 n;
	const CParaFormat *pPF, *pPFLast = NULL;
	CRchTxtPtr	 rtp(*this);

	while(1)
	{
		pPF = rtp.GetPF();
		if(pPF->_wEffects & PFE_TABLEROWDELIMITER)
			break;
		// CParaFormat::UpdateNumber(2, pPFLast) returns:
		//		0 -- not a numbered list
		//		1 -- new numbered list or pPFLast = NULL
		//		2 -- list number suppressed
		//		3 -- different number in same list
		n = pPF->UpdateNumber(2, pPFLast);
		if(n == 0 || n == 1 && pPFLast && cPara)
			break;
		ch = rtp.GetPrevChar();
		if((!ch || IsASCIIEOP(ch) && !IN_RANGE(VT, ch, FF) || ch == CELL) && n != 2)
			cPara++;
		if(!ch || rtp._rpTX.IsAtStartOfCell())
			break;
		rtp._rpPF.Move(rtp._rpTX.FindEOP(tomBackward));	
		pPFLast = pPF;						// Don't need to update _rpCF
	}										//  for this calculation
	return cPara;
}

#endif  // DEBUG

//======================= CRchTxtPtr constructors ========================================

CRchTxtPtr::CRchTxtPtr(CTxtEdit *ped) :
	_rpTX(ped, 0), _rpCF(NULL),	_rpPF(NULL)
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::CRchTxtPtr");

	InitRunPtrs();
}

CRchTxtPtr::CRchTxtPtr(CTxtEdit *ped, LONG cp) :
	_rpTX(ped, cp), _rpCF(NULL), _rpPF(NULL)
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::CRchTxtPtr");

	InitRunPtrs();
}

CRchTxtPtr::CRchTxtPtr (const CRchTxtPtr& rtp) :
	_rpTX(rtp._rpTX), _rpCF(rtp._rpCF), _rpPF(rtp._rpPF)
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::CRchTxtPtr");

	_rpCF.AdjustForward();		// In case rtp is adjusted backward...
	_rpPF.AdjustForward();
}

CRchTxtPtr::CRchTxtPtr (const CDisplay * pdp) :
	_rpTX(pdp->GetPed(), 0), _rpCF(NULL), _rpPF(NULL)
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::CRchTxtPtr");

	InitRunPtrs();
}

/*
 *	CRchTxtPtr::Move(cch)
 *	
 *	@mfunc
 *		Move this rich-text ptr forward <p cch> characters.  If <p cch>
 *		<lt> 0, move backward by -<p cch> characters.
 *	
 *	@rdesc
 *		cch actually moved
 */
LONG CRchTxtPtr::Move(
	LONG cch)			// @parm count of characters to move - may be <lt> 0
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::Move");

	if( cch != 0 )
	{
		cch = _rpTX.Move(cch);
		_rpCF.Move(cch);
		_rpPF.Move(cch);
		_TEST_INVARIANT_
	}
	return cch;
}

/*
 *  CRchTxtPtr::AdvanceCRLF()
 *
 *  @mfunc
 *      Advance this text ptr one char, treating CRLF as a single char.
 *
 *  @rdesc
 *      cch actually moved
 */
LONG CRchTxtPtr::AdvanceCRLF()
{
    TRACEBEGIN(TRCSUBSYSRANG, TRCSCOPEINTERN, "CRchTxtPtr::AdvanceCRLF");

    LONG cch = _rpTX.AdvanceCRLF();
    _rpPF.Move(cch);
    _rpCF.Move(cch);
    return cch;
}

/*
 *  CRchTxtPtr::SnapToCluster(iDirection)
 *
 *  @mfunc
 *      If this text ptr is not at cluster boundary, move it to the closest one.
 *
 *  @rdesc
 *      cch actually moved
 */
#ifndef NOCOMPLEXSCRIPTS
LONG CRchTxtPtr::SnapToCluster(INT iDirection)
{
    TRACEBEGIN(TRCSUBSYSRANG, TRCSCOPEINTERN, "CRchTxtPtr::SnapToCluster");

	LONG	cch = 0;
	LONG	cp;

	if (GetPed()->_pbrk)
	{
		if (iDirection >= 0)
		{
			LONG	cpEnd = GetPed()->GetAdjustedTextLength();

			while ((cp = GetCp()) < cpEnd && !GetPed()->_pbrk->CanBreakCp(BRK_CLUSTER, cp))
				cch += AdvanceCRLF();
		}
		else
		{
			while ((cp = GetCp()) > 0 && !GetPed()->_pbrk->CanBreakCp(BRK_CLUSTER, cp))
				cch += BackupCRLF();
		}
	}
    return cch;
}
#endif

/*
 *  CRchTxtPtr::BackupCRLF(fDiacriticCheck)
 *
 *  @mfunc
 *      Backup this text ptr one char, treating CRLF as a single char.
 *
 *  @rdesc
 *      cch actually moved
 */
LONG CRchTxtPtr::BackupCRLF(
	BOOL fDiacriticCheck)
{
    TRACEBEGIN(TRCSUBSYSRANG, TRCSCOPEINTERN, "CRchTxtPtr::BackupCRLF");

    LONG cch = _rpTX.BackupCRLF(fDiacriticCheck);
    _rpPF.Move(cch);
    _rpCF.Move(cch);
    return cch;
}

/*
 * CRchTxtPtr::ValidateCp(&cp)
 *
 *	@mfunc
 *		If <p cp> <lt> 0, set it to 0; if it's <gt> text length, set it to
 *		text length.
 */
void CRchTxtPtr::ValidateCp(
	LONG &cp) const			// @parm new cp for this text ptr
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::ValidateCp");

	LONG cchT = GetTextLength();

	cp = min(cp, cchT);				// Be sure cp is valid
	cp = max(cp, 0);
}

/*
 * CRchTxtPtr::SetCp(cp)
 *
 *	@mfunc
 *		Set this rich text ptr's cp to cp
 */
LONG CRchTxtPtr::SetCp(
	LONG cp)			// @parm new cp for this text ptr
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::SetCp");

	CRchTxtPtr::Move(cp - GetCp());
	return GetCp();
}

/*	CRchTxtPtr::GetIchRunXX() and CRchTxtPtr::GetCchRunXX()
 *
 *	@mfunc
 *		Text-run management to retrieve current text run cch and offset
 *
 *	@rdesc
 *		current run ich or cch
 *
 *	@devnote
 *		Use of queries like _rpCF.IsValid() instead of an inclusive fRich
 *		allows rich-text formatting to be applied per rich-text category,
 *		e.g., CHARFORMATs, but not necessarily PARAFORMATs.  If the rp isn't
 *		valid, _cp is used for ich and the document length is used for cch,
 *		i.e., the values for a document describable by a single plain-text run
 */
LONG CRchTxtPtr::GetIchRunCF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetIchRunCF");

	return _rpCF.IsValid() ? _rpCF.GetIch() : GetCp();
}

LONG CRchTxtPtr::GetIchRunPF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetIchRunPF");

	return _rpPF.IsValid() ? _rpPF.GetIch() : GetCp();
}

LONG CRchTxtPtr::GetCchRunCF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetCchRunCF");

	return _rpCF.IsValid() ? _rpCF.GetRun(0)->_cch : GetTextLength();
}

/*	CRchTxtPtr::GetCchLeftRunCF() / GetCchLeftRunPF()
 *
 *	@mfunc
 *		Return cch left in run, i.e., cchRun - ich
 *
 *	@rdesc
 *		cch left in run
 */
LONG CRchTxtPtr::GetCchLeftRunCF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetCchLeftRunCF");

	return _rpCF.IsValid()
		? _rpCF.GetCchLeft() : GetTextLength() - GetCp();
}

LONG CRchTxtPtr::GetCchLeftRunPF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetCchLeftRunPF");

	return _rpPF.IsValid()
		? _rpPF.GetCchLeft() : GetTextLength() - GetCp();
}

/*
 *	CRchTxtPtr::FindText(cpMost, dwFlags, pch, cchToFind)
 *	
 *	@mfunc
 *		Find text in a range starting at this text pointer;
 *		if found, moves this text pointer to that position.
 *	
 *	@rdesc
 *		character position of first match
 *		<lt> 0 if no match
 *
 *	@devnote
 *		Would be easy to match a single format (like Word 6) provided
 *		cchToFind is nonzero.  Else need to search runs (also pretty easy).
 *		For format-sensitive searches, might be easier to search for matching
 *		format run first and then within that run search for text.
 */
LONG CRchTxtPtr::FindText (
	LONG		cpMost,		// @parm Limit of search; <lt> 0 for end of text
	DWORD		dwFlags,	// @parm FR_MATCHCASE	case must match
							//		 FR_WHOLEWORD	match must be a whole word
	TCHAR const *pch,		// @parm Text to search for
	LONG		cchToFind)	// @parm Length of text to search for
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::FindText");

	_TEST_INVARIANT_

	LONG cpSave = GetCp();
	LONG cpMatch = _rpTX.FindText(cpMost, dwFlags, pch, cchToFind);

	if(cpMatch >= 0)					// cpMatch = -1 means "not found"
		SetRunPtrs(GetCp(), cpSave);	
	
			// possible code for format-dependent Finds
	return cpMatch;
}

/*
 *	CRchTxtPtr::GetCF()/GetPF()
 *	
 *	@mfunc
 *		Return ptr to CCharFormat/CParaFormat at this text ptr. If no CF/PF runs
 *		are allocated, then return ptr to default format
 *	
 *	@rdesc
 *		Ptr to CCharFormat/CParaFormat at this text ptr
 */
const CCharFormat* CRchTxtPtr::GetCF() const
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetCF");

	return ((CTxtArray *)_rpTX._pRuns)->GetCharFormat(_rpCF.GetFormat());
}

const CParaFormat* CRchTxtPtr::GetPF() const
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::GetPF");

	return ((CTxtArray *)_rpTX._pRuns)->GetParaFormat(_rpPF.GetFormat());
}

/*
 *	CRchTxtPtr::Get_iCF()
 *	
 *	@mfunc
 *		Get character format index at this text pointer
 *
 *	@rdesc
 *		Get iCF at this text pointer
 */
LONG CRchTxtPtr::Get_iCF ()
{
	TRACEBEGIN(TRCSUBSYSRANG, TRCSCOPEINTERN, "CRchTxtPtr::Get_iCF");

	LONG iCF = _rpCF.GetFormat();

	GetCharFormatCache()->AddRef(iCF);
	return iCF;
}

/*
 *	CRchTxtPtr::Get_iPF()
 *	
 *	@mfunc
 *		Get paragraph format index at this text pointer
 *
 *	@rdesc
 *		Get iPF at this text pointer
 */
LONG CRchTxtPtr::Get_iPF ()
{
	TRACEBEGIN(TRCSUBSYSRANG, TRCSCOPEINTERN, "CRchTxtPtr::Get_iPF");

	LONG iPF = _rpPF.GetFormat();

	GetParaFormatCache()->AddRef(iPF);
	return iPF;
}

/*
 *	CRchTxtPtr::GetPlainText(cchBuff, pch, cpMost, fTextize, fUseCRLF)
 *
 *	@mfunc
 *		Same as CTxtPtr except that hidden text isn't copied
 *
 *	@rdesc
 *		Count of characters copied
 */
LONG CRchTxtPtr::GetPlainText(
	LONG	cchBuff,		//@parm Buffer cch
	WCHAR *	pch,			//@parm Buffer to copy text into
	LONG	cpMost,			//@parm Largest cp to get
	BOOL	fTextize,		//@parm True if break on WCH_EMBEDDING
	BOOL	fUseCRLF)		//@parm If TRUE, CR or LF -> CRLF
{
	LONG	  cchTotal = 0;
	CTxtEdit *ped = GetPed();
	CTxtPtr	  tp(_rpTX);					// tp to get unhidden text

	if(!_rpCF.IsValid() || !ped->IsRich())
		cchTotal = tp.GetPlainText(cchBuff, pch, cpMost, fTextize, fUseCRLF);

	else
	{
		LONG	  cch = 0;
		LONG	  cp;
		CCFRunPtr rp(*this);				// rp to check for hidden text

		cpMost = min(cpMost, GetPed()->GetAdjustedTextLength());
		for(; cchTotal < cchBuff; pch += cch)
		{
			cch = rp.FindUnhiddenForward();
			if(tp.GetCp() + cch >= cpMost)
			{
				SetCp(cpMost);
				return cchTotal;
			}
			tp.Move(cch);					// Bypass hidden text
			for(cch = 0; !rp.IsHidden() && cch < cchBuff; )
			{
				cch += rp.GetCchLeft();
				if(tp.GetCp() + cch >= cpMost || !rp.NextRun())
					break;
			}
			if(cch)							// Copy unhidden text
			{
				cp = tp.GetCp() + cch;
				cp = min(cp, cpMost);
				cch = tp.GetPlainText(cchBuff - cchTotal, pch, cp, fTextize, fUseCRLF);
				cchTotal += cch;
			}
			if(tp.GetCp() >= cpMost || !cch)
				break;
		}
	}
	SetCp(tp.GetCp());
	return cchTotal;
}

/*
 *	CRchTxtPtr::ReplaceRange(cchOld, cchNew, *pch, pcpFirstRecalc, publdr,
 *							 iFormat, dwFlags)
 *	@mfunc
 *		Replace a range of text at this text pointer using CCharFormat iFormat
 *		and updating other text runs as needed
 *	
 *	@rdesc
 *		Count of new characters added
 *	
 *	@devnote
 *		Moves this text pointer to end of replaced text.
 *		May move text block and formatting arrays.
 */
LONG CRchTxtPtr::ReplaceRange(
	LONG		cchOld,		//@parm length of range to replace
							//		(<lt> 0 means to end of text)
	LONG		cchNew,		//@parm length of replacement text
	TCHAR const *pch,		//@parm replacement text
	IUndoBuilder *publdr,	//@parm Undo bldr to receive antievents
	LONG		iFormat,	//@parm CCharFormat iFormat to use for cchNew
	LONG *		pcchMove,	//@parm Out parm returning cch moved if paradir change
	DWORD		dwFlags)	//@parm Special flags
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::ReplaceRange");

	LONG		  cch;
	LONG		  cchEndEOP = 0;				// Default 0 final EOP fixup
	LONG		  cchAdvance = 0;
	LONG		  cchBackup = 0;
	LONG		  cchMove = 0;					// Default nothing to move
	LONG		  cchNextEOP = cchOld;			// cch to next EOP
	LONG		  cchPrevEOP = 0;				// cch back to previous EOP
	LONG		  cpFR;							//  between PF runs
	LONG 		  cpSave = GetCp();
	LONG		  cpFormatMax;
	LONG		  cpFormatMin = cpSave;			// Used for notifications
	LONG		  cpFormat = cpSave;			// Will add cchOld, maybe cchMove
	BOOL		  fParaDirChange = FALSE;
	CTxtEdit *	  ped = GetPed();
	IAntiEvent *  paeCF = NULL;
	IAntiEvent *  paePF = NULL;
	CNotifyMgr *  pnm;
	CObjectMgr *  pobjmgr;
	CFreezeDisplay fd(ped->_pdp);				// freeze until itemization is done

 	_TEST_INVARIANT_

	LONG cchEnd = GetTextLength() - GetCp();
	LONG cOldRuns = _rpTX.Count();

	if(cchOld < 0 || cchOld > cchEnd)
		cchOld = cchEnd;

  	if(IsRich() && cchOld == cchEnd)			// Attempting to delete up
	{											//  thru final EOP
		cchEndEOP = (ped->fUseCRLF())			// Calc cch of final EOP
				  ? CCH_EOD_10 : CCH_EOD_20;

		if(cchEndEOP <= cchOld)					// Don't delete it unless
			cchOld -= cchEndEOP;				//  converting from 2.0
		if(_rpPF.IsValid())
		{
			_rpPF.AdjustBackward();				// If previous para is a
			if(GetPF()->InTable())				//  table row, don't delete
				cchEndEOP = 0;					//  final para formatting
			_rpPF.AdjustForward();
		}
	}
	else if(_rpPF.IsValid())					// PARAFORMATs are enabled
	{		
		_rpPF.AdjustForward();		
		LONG iPF2 = _rpPF.GetFormat();			
		BOOL fIsTRD2 = ped->GetParaFormat(iPF2)->IsTableRowDelimiter();
		BOOL fNoTrdCheck = dwFlags & RR_NO_TRD_CHECK;

		if(cchOld)								
		{											
			CFormatRunPtr rp(_rpPF);				
			CTxtPtr 	  tp(_rpTX);			// Get tp and rp at end of	
												//  range. Need bounding para	
			tp.Move(cchOld);					//  counts to save valid PF
			if(tp.GetCp() < ped->GetAdjustedTextLength())
				cchOld += tp.AdjustCRLF(1);
			rp.Move(cchOld);					//  for undo

			LONG iPF1 = rp.GetFormat();
			BOOL fIsTRD1 = ped->GetParaFormat(iPF1)->IsTableRowDelimiter();
			cch = 0;
			if(tp.IsAfterEOP())					// Range ends with an EOP
			{										
				if ((tp.GetPrevChar() == CELL ||// Don't delete table cell
					fIsTRD2 && cchOld == 2) &&	//  at end of range
					!fNoTrdCheck)
				{
					return 0;					//  or solo row delimiter
				}
				cch = -tp.BackupCRLF();			// Get EOP length by
				tp.Move(cch);					//  backing up over it
			}									// Move past EOP
			BOOL fIsAtBOP = !GetCp() || _rpTX.IsAfterEOP();
			if(tp.IsAtTRD(0))
			{
				AssertSz(tp.IsAtTRD(STARTFIELD), "Illegal deletion attempt");
				cchNextEOP = 0;
				if(!fIsAtBOP)
				{
					cchOld--;					// Leave end CR there
					if(tp.IsAfterTRD(ENDFIELD))	// If CR of TRED, move it
					{							//  into prec PF run
						cchMove = cchNextEOP = 1;
						cpFormat++;
					}
				}
			}
			else
			{
				cchNextEOP = tp.FindEOP(tomForward);// Get cch up to next EOP
				AssertSz(rp.GetCchLeft() >= cchNextEOP,
					"CRchTxtPtr::ReplaceRange: missing EOP");
			}
			if (!fIsAtBOP && cch == cchOld &&	// Deleting EOP alone before
				!rp.GetIch())					// new PARAFORMAT run start
			{									// in para with more than EOP
				if(fIsTRD1)
					return 0;					// Don't merge with table row delim
				cchMove = cchNextEOP;			// Need to move chars up to	
				cpFormat += cchMove;			//  end of next para for	
			}										
			
			cchNextEOP += cchOld;				// Count from GetCp() to EOP
				
			tp.SetCp(GetCp());					// Back to this ptr's _cp
			if(!fIsAtBOP)
			{
				cchPrevEOP = tp.FindEOP(tomBackward);// Get cch to start of para
				AssertSz(_rpPF.GetIch() >= -cchPrevEOP,
					"CRchTxtPtr::ReplaceRange: missing EOP");
			}
			// If deleting from within one format run up to or into another, set
			// up to move last para in starting format run into the run following
			// the deleted text
			if(iPF1 != iPF2)					// Change of format during
			{									//  deleted text not starting
				if(!fIsAtBOP && !cchMove && !fIsTRD1)//  at BOP
				{
					cchMove = cchPrevEOP;		// Get cch to start of para
					cpFormatMin += cchMove;		//  in this ptr's run for
				}								//  moving into rp's run

				if (((ped->GetParaFormat(iPF1)->_wEffects ^
					  ped->GetParaFormat(iPF2)->_wEffects) & PFE_RTLPARA) &&
					!(fIsTRD1 | fIsTRD2))
				{
					fParaDirChange = TRUE;		// Note that para direction
					Assert(ped->IsBiDi());		//  changed
				}									
			}
		}
		else
		{
			UINT ch = GetPrevChar();
			if (fIsTRD2 && ch == CELL &&		// Don't paste between CELL
				(_rpTX.IsAtTRD(ENDFIELD) || !fNoTrdCheck) || //  & row terminator
				ch == NOTACHAR && !ped->IsStreaming() ||
				GetChar() == NOTACHAR)			//  or before/after NOTACHAR
			{
				return 0;							
			}
		}
	}	
	
	Assert(cchNew >= 0 && cchOld >= 0);
	if(!(cchNew + cchOld))						// Nothing to do (note: all
	{											//  these cch's are >= 0)
		if(pcchMove)
			*pcchMove = 0;
		return 0;
	}						

	// If BiDi doc, expand the range to cover the boundaries that guarantee
	// the valid state of the BiDi level so we can undo it properly. (wchao)
	cpFormatMax = cpFormat + cchOld;
	if(ped->IsBiDi())
	{
		cchBackup = ExpandRangeFormatting (cchOld + cchEndEOP,
										fParaDirChange ? cchMove : 0, cchAdvance);
		Assert (cchBackup <= 0);
		if(cchMove >= 0)					// In this case, cchBackup is minus
		{									//  the sum of the two previous
			cpFormatMin += cchBackup;		//  run counts
			Assert(cpFormatMin >= 0);
		}
		if(cchMove <= 0)					// In this case cchAdvance is sum
		{									//  of next two run counts (or less)
			cpFormatMax += cchAdvance;
			Assert(cpFormatMax <= GetTextLength());
		}
	}

	// Handle pre-replace range notifications.  This method is very
	// useful for delayed rendering of data copied to the clipboard.
	pnm = ped->GetNotifyMgr();
	if(pnm)
	{
		pnm->NotifyPreReplaceRange((ITxNotify *)this, cpSave, cchOld,
			cchNew, cpFormatMin, cpFormatMax);
	}

	if(iFormat >= 0)
		Check_rpCF();

	// Get rid of objects first.  This lets us guarantee that when we
	// insert the objects as part of an undo, the objects themselves are
	// restored _after_ their corresponding WCH_EMBEDDINGs have been
	// added to the backing store.

	if(GetObjectCount())
	{
		pobjmgr = ped->GetObjectMgr();
		Assert(pobjmgr);
		pobjmgr->ReplaceRange(cpSave, cchOld, publdr);
	}

	// The anti-events used below are a bit tricky (paeCF && paePF).
	// Essentially, this call, CRchTxtPtr::ReplaceRange generates one
	// 'combo' anti-event composed of up to two formatting AE's plus
	// the text anti-event.  These anti-events are combined together
	// to prevent ordering problems during undo/redo.
	cpFR = ReplaceRangeFormatting(cchOld + cchEndEOP, cchNew + cchEndEOP,
						iFormat, publdr, &paeCF, &paePF, cchMove, cchPrevEOP,
						cchNextEOP, cchBackup, cchAdvance);
	if(cchEndEOP)
	{
		// If we added in the EOP we need to back up by the EOP so
		// that the invariants don't get annoyed and the richtext object
		// doesn't get out of sync.
		_rpCF.Move(-cchEndEOP);
		_rpPF.Move(-cchEndEOP);
	}
			
	if(cpFR < 0)
	{
		Tracef(TRCSEVERR, "ReplaceRangeFormatting(%ld, %ld, %ld) failed", GetCp(), cchOld, cchNew);
		cch = 0;
		goto Exit;
	}

	// As noted above in the call to ReplaceRangeFormatting, the anti-events
	// paeCF and paePF, if non-NULL, were generated by ReplaceRangeFormatting.
	// In order to solve ordering problems, the anti-event generated by this
	// method is actually a combo anti-event of text && formatting AE's.
	cch = _rpTX.ReplaceRange(cchOld, cchNew, pch, publdr, paeCF, paePF);
	if(cch != cchNew)
	{
		Tracef(TRCSEVERR, "_rpTX.ReplaceRange(%ld, %ld, ...) failed", cchOld, cchNew);

#ifndef NOFULLDEBUG
		// Boy, out of memory or something bad.  Dump our formatting and hope
		// for the best.
		//
		// FUTURE: (alexgo) degrade more gracefully than losing formatting
		// info.

		// Notify every interested party that they should dump their formatting
		if(pnm)
			pnm->NotifyPreReplaceRange(NULL, CONVERT_TO_PLAIN, 0, 0, 0, 0);

		// Tell document to dump its format runs
		ped->GetTxtStory()->DeleteFormatRuns();
#endif
		goto Exit;
	}
	AssertSz(!_rpPF.IsValid() || _rpPF.GetIch() || !GetCp() || _rpTX.IsAfterEOP(),
		"CRchTxtPtr::ReplaceRange: EOP not at end of PF run");
			
	// BUGBUG!! (alexgo) doesn't handle correctly the case where things fail
	// (due to out of memory or whatever).  See also notes in CTxtPtr::HandleReplaceRange
	// Undo.  The assert below is therefore somewhat bogus, but if it fires,
	// then our floating ranges are going to be in trouble until we fix
	// up the logic here.
	Assert(cch == cchNew);

Exit:

#ifdef DEBUG
	// Test invariant again before calling out to replace range notification.
	// In this way, we can catch bugs earlier. The invariant has its own
	// scope for convenience.
	if( 1 )
	{
		_TEST_INVARIANT_
	}
#endif

	if(ped->IsBiDi() && cpSave <= ped->GetCpFirstStrong() && (cchOld | cch))
	{
		// Remember whether formatting is valid before we set context direction
		BOOL fCFValidBeforeSetContextDirection = _rpCF.IsValid();
		
		// Need to check the direction of the control if the input characters
		// control the direction.
		ped->SetContextDirection();

		// Did SetContextDirection make the formatting valid?
		if (!fCFValidBeforeSetContextDirection && _rpCF.IsValid())
		{
			// Our invariant is that cps should be equal if formatting is valid
			// so make it so!
			_rpCF.BindToCp(GetCp());
		}
	}

	if(pnm)
	{
		BOOL fTxtCellShrink = (cOldRuns > _rpTX.Count());
		NOTIFY_DATA notifyData;

		if (fTxtCellShrink)
		{
			// Setup NOTIFY_DATA
			notifyData.id = NOTIFY_DATA_TEXT_ID;
			notifyData.dwFlags = TN_TX_CELL_SHRINK;
			notifyData.pData = NULL;
		}
		pnm->NotifyPostReplaceRange((ITxNotify *)this, cpSave, cchOld, cch,
			cpFormatMin, cpFormatMax, 
			fTxtCellShrink ? &notifyData : NULL);
	}

	ped->GetCallMgr()->SetChangeEvent(CN_TEXTCHANGED);

	if(pcchMove)						// Only return non0 cchMove if para
	{									//  direction changed, i.e., it's
		*pcchMove = fParaDirChange		//  a "BOOL" with a useful value,
				  ? cchMove : 0;		//  namely the count of chars with
	}									//  changed direction

	if (ped->IsComplexScript())
	{
		if (dwFlags & RR_ITMZ_NONE || (ped->IsStreaming() && (!pch || *pch != WCH_EMBEDDING)))
			ped->_fItemizePending = TRUE;
		else
			ItemizeReplaceRange(cchNew, fParaDirChange? cchMove : 0, publdr);
	}
	return cch;
}

/*
 *	CRchTxtPtr::InitRunPtrs()
 *
 *	@mfunc
 *		Initialize Run Ptrs of this rich-text ptr to correspond to
 *		document given by ped and to cp given by cp.
 */
void CRchTxtPtr::InitRunPtrs()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::InitRunPtrs");
	AssertSz(GetPed(), "RTP::InitRunPtrs: illegal GetPed()");

	LONG		cp = GetCp();
	CTxtStory *	pStory = GetPed()->GetTxtStory();
	LONG		cchText = pStory->GetTextLength();
												// If there's RichData,
	if(pStory->_pCFRuns)						//  initialize format-run ptrs
	{
		_rpCF.SetRunArray((CRunArray *)pStory->_pCFRuns);
		_rpCF.BindToCp(cp, cchText);
	}
	if(IsRich() && pStory->_pPFRuns)
	{
		_rpPF.SetRunArray((CRunArray *)pStory->_pPFRuns);
		_rpPF.BindToCp(cp, cchText);
	}
}

/*
 *	CRchTxtPtr::SetRunPtrs(cp, cpFrom)
 *
 *	@mfunc set Run Ptrs of this rich-text ptr to correspond to cp
 *
 *	@rdesc
 *			TRUE unless cp is outside of doc (in which case RunPtrs are
 *			set to nearest document end).
 */
void CRchTxtPtr::SetRunPtrs(
	LONG cp,				// @parm character position to move RunPtrs to
	LONG cpFrom)			// @parm cp to start with
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::SetRunPtrs");

	if(cpFrom && 2*cp >= cpFrom)
	{
		_rpCF.Move(cp - cpFrom);
		_rpPF.Move(cp - cpFrom);
	}
	else
	{
		LONG cchText = GetTextLength();
		_rpCF.BindToCp(cp, cchText);
		_rpPF.BindToCp(cp, cchText);
	}
}

/*
 *	CRchTxtPtr::ExpandRangeFormatting(cchRange,	cchMove, &cchAdvance)
 *
 *	@mfunc
 *		In BiDi scenario, it's possible that updating a character affects the
 *		level of the others. Such case should only happen when number is involved.
 *
 *		Example: (AN)"11:30" changing '3' to 'x' will change the level of
 *		colon from 2 to 1. Accordingly return cch back to safe itemization
 *
 *	@rdesc
 *		cch back to safe itemization
 */
LONG CRchTxtPtr::ExpandRangeFormatting(
	LONG	cchRange,		// in: original length
	LONG	cchMove,		// in: number of chars moved after replacement
	LONG &	cchAdvance)		// out: extra chars added to range after expanding
{
	LONG	cchBackup = 0;
	
	cchAdvance = 0;

	if (_rpCF.IsValid())
	{
   		CTxtPtr		tp(_rpTX);
		if (!IsRich())
		{
			cchBackup = tp.FindEOP(tomBackward);
			tp.Move(-cchBackup + cchRange);
			cchAdvance = tp.FindEOP(tomForward);
		}
		else
		{
			CFormatRunPtr	rp(_rpCF);
			LONG			cp = GetCp();
	
			if (cchMove < 0)
			{
				// <cchMove> count of chars to be moved down to next paragraph
				cchBackup = cchMove;
			}
			else if (cchMove > 0)
			{
				// <cchMove> count of chars to be moved up to previous paragraph
				cchAdvance = cchMove;
			}
				
			// Advancing/Backing up 2 adjacent runs seems to be sufficient for now.
			if (cchBackup == 0)
			{
				rp.AdjustBackward();
				cchBackup = -rp.GetIch();
				if (rp.PrevRun())
					cchBackup -= rp.GetCchLeft();
				rp.Move(-cchBackup);		// Restore position
			}
			// Move run pointer to end of range
			rp.Move(cchRange);
			tp.SetCp(cp + cchRange);
			if (cchAdvance == 0 && !tp.IsAtEOP())
			{
				rp.AdjustForward();

				cchAdvance += rp.GetCchLeft();
				if (rp.NextRun())
					cchAdvance += rp.GetCchLeft();
			}
		}
	}
	return cchBackup;
}


/*
 *	CRchTxtPtr::ItemizeReplaceRange(cchUpdate, cchMove, publdr, fUnicodeBidi)
 *
 *	@mfunc
 *		Find out the exact range to be itemized after calling :ReplaceRange
 *
 *	@rdesc
 *		result from ItemizeRuns.
 *		Guarantee *this* pointer wont move.
 */
BOOL CRchTxtPtr::ItemizeReplaceRange(
	LONG			cchUpdate,
	LONG			cchMove,		// Count of chars moved after replacing
	IUndoBuilder*	publdr,			//  (they need reitemizing)
	BOOL			fUnicodeBidi)
{
    BOOL	fr = FALSE;

	if (GetPed()->IsComplexScript())
	{
		Assert (cchUpdate >= 0);	// the range after ReplaceRange must be degenerate

		CTxtPtr	tp(_rpTX);
		LONG 	cp = GetCp();
		LONG	cpStart, cpEnd;
        BOOL    fNonUnicodeBidiRecurse = FALSE;
		BOOL	fUseCtxLevel = FALSE;

		tp.Move(-cchUpdate);
		if (cchUpdate > 0 && GetPed()->IsRich() && fUnicodeBidi)
        {
			cpStart = cpEnd = cp;
			cpStart -= cchUpdate;

			if (GetPed()->IsBiDi())
			{
				// Fix for 7094 : Don't look at the incoming text for clues
				// fUseCtxLevel = TRUE;

				// Recurse with non-BiDi, so the run preceding/succeeding this chunk get updated
				fNonUnicodeBidiRecurse = TRUE;
			}
		}
		else
		{
			tp.FindWhiteSpaceBound(cchUpdate, cpStart, cpEnd,
								!GetPed()->IsRich() ? FWS_BOUNDTOPARA : 0);
		}

		if (cchMove < 0)
		{
			// <cchMove> number of text -before- the replaced range
			// moves down to the next paragraph.
			cpStart = max(cp - cchUpdate + cchMove, 0);
		}
		else if (cchMove > 0)
		{
			// <cchMove> number of text -after- the replaced range
			// moves up to the previous paragraph.
			cpEnd = min(cp + cchMove, GetPed()->GetTextLength());
		}

		{
			CTxtRange	rg(*this, 0);

			rg.Set(cpEnd, cpEnd - cpStart);

			fr = rg.ItemizeRuns(publdr, fUnicodeBidi, fUseCtxLevel);			

			// set pointer back to original cp
	
			// We cant use copy operator here since itemization changes format run.
			// It would cause invariant failure in _rpCF.
			cp -= rg.GetCp();
			_rpCF = rg._rpCF;
			_rpCF.Move(cp);
	
			// ItemizeRuns invalidates rg._rpPF so that the paraformat run becomes valid
			// and we need to advance it to the current cp.
			_rpPF = rg._rpPF;
			_rpPF.Move(cp);

			// Perf note: We dont want the range to be around when we recurse
			// since a range is a notification sink.
		}

		// Run itemization to the same range, this time forces it to be non-Bidi.
		if (fr && fNonUnicodeBidiRecurse)
			fr = ItemizeReplaceRange(cchUpdate, 0, publdr, FALSE);
	}
	return fr;
}


/*
 *	CRchTxtPtr::ReplaceRangeFormatting(cchOld, cchNew, iFormat, publdr,
 *									   ppaeCF, ppaePF, cchMove, cchPrevEOP,
 *									   cchNextEOP, cchSaveBefore, cchSaveAfter)
 *	@mfunc
 *		Replace character and paragraph formatting at this text pointer
 *		using CCharFormat with index iFormat
 *	
 *	@rdesc
 *		count of new characters added
 *	
 *	@devnote
 *		Moves _rpCF and _rpPF to end of replaced text and moves format arrays.
 *		CCharFormat for iFormat is fully configured, i.e., no NINCHes
 */
LONG CRchTxtPtr::ReplaceRangeFormatting(
	LONG		cchOld,		  //@parm Length of range to replace
	LONG		cchNew,		  //@parm Length of replacement text
	LONG		iFormat,	  //@parm Char format to use
	IUndoBuilder *publdr,	  //@parm UndoBuilder to receive antievents
	IAntiEvent **ppaeCF,	  //@parm Where to return 'extra' CF anti-events
	IAntiEvent **ppaePF,	  //@parm Where to return extra PF anti-events
	LONG		cchMove,	  //@parm cch to move between PF runs
	LONG		cchPrevEOP,	  //@parm cch from _cp back to prev EOP
	LONG		cchNextEOP,	  //@parm cch from _cp up to next EOP
	LONG		cchSaveBefore,//@parm cch backup for BiDi
	LONG		cchSaveAfter) //@parm cch advance for BiDi
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::ReplaceRangeFormatting");

	LONG				cp = GetCp();				
	LONG				cchText = GetTextLength() + cchNew - cchOld;
	LONG				iRunMerge	= 0;
	ICharFormatCache *	pcfc = GetCharFormatCache();
	IParaFormatCache *	ppfc = GetParaFormatCache();

	AssertSz(cchOld >= 0,
		"CRchTxtPtr::ReplaceRangeFormatting: Illegal cchOld");

	if(_rpCF.IsValid())
	{
		iRunMerge = _rpCF._iRun;
		if(iRunMerge > 0)
			iRunMerge--;

		Assert (cchSaveBefore <= 0 && cchSaveAfter >= 0);
		if(cchOld + cchSaveAfter - cchSaveBefore > 0)
		{										// add the soon-to-be deleted
			if(publdr)							// formats to the undo list
			{
				// Include previous cchSaveBefore chars
				_rpCF.Move(cchSaveBefore);
				*ppaeCF = gAEDispenser.CreateReplaceFormattingAE(GetPed(),
							cp + cchSaveBefore, _rpCF, cchSaveAfter + cchOld - cchSaveBefore,
							pcfc, CharFormat);
				// Restore _rpCF (we just want to save value not delete it)
				_rpCF.Move(-cchSaveBefore);
			}
			if(cchOld)							// Delete/modify CF runs <-->
				_rpCF.Delete(cchOld, pcfc, 0);	//  to cchOld chars
		}
		// If we deleted all of text in story, don't bother adding a new
		// run.	Else insert/modify CF runs corresponding to cchNew chars
		//
		// In a plain-text control, there is no final EOP; hence the test
		// for equality.
		if(cchNew > 1 || cchNew && cchOld <= GetTextLength())
			_rpCF.InsertFormat(cchNew, iFormat, pcfc);

		if((cchOld || cchNew) && _rpCF.IsValid())// Deleting all text
		{										//  invalidates _rpCF
			_rpCF.AdjustForward();
			_rpCF.MergeRuns(iRunMerge, pcfc);
			_rpCF.BindToCp(cp + cchNew, cchText);
		}
	}

	if(_rpPF.IsValid())
	{
		_rpPF.AdjustForward();					// Be absolutely sure that
												//  PF runs end with EOPs
		iRunMerge = _rpPF._iRun;
		if(iRunMerge > 0)
			iRunMerge--;

		if(cchOld)								// Delete cchOld from PF runs
		{										// add the soon-to-be deleted
			if(publdr)							// formats to the undo list
			{
				CFormatRunPtr rp(_rpPF);

				rp.Move(cchPrevEOP);
				*ppaePF = gAEDispenser.CreateReplaceFormattingAE(GetPed(),
								cp + cchPrevEOP, rp, cchNextEOP - cchPrevEOP,
								ppfc, ParaFormat);
			}
		    _rpPF.Delete(cchOld, ppfc, cchMove);
		}

		if(_rpPF.IsValid())						// Deleting all text
		{										//  invalidates _rpPF
			_rpPF.AdjustForward();
			_rpPF.GetRun(0)->_cch += cchNew;	// Insert cchNew into current
			_rpPF._ich	+= cchNew;				//  PF run
			if(cchOld || cchNew)
			{
				_rpPF.MergeRuns(iRunMerge, ppfc);
				_rpPF.BindToCp(cp + cchNew, cchText);
			}
		}
	}
	return cchNew;
}

/*
 *	CRchTxtPtr::ExtendFormattingCRLF()
 *	
 *	@mfunc
 *		Use the same CCharFormat and CParaFormat indices for the EOP at
 *		this text ptr as those immediately preceding it.
 *
 *	@devnote
 *		Leaves this text ptr's format ptrs at run you get from AdjustBackward
 *		since this run ends up including the new text.
 */	
void CRchTxtPtr::ExtendFormattingCRLF()
{
	LONG		cch = GetTextLength() - GetPed()->GetAdjustedTextLength();
	CNotifyMgr *pnm = GetPed()->GetNotifyMgr();

	_rpCF.AdjustFormatting(cch, GetCharFormatCache());
	if(_rpPF.IsValid())
	{
		_rpPF.AdjustBackward();
		if(!InTable())
			_rpPF.AdjustFormatting(cch, GetParaFormatCache());
		_rpPF.AdjustForward();
	}

	if(pnm)
	{
		// We assume that Cch is positive (or zero) here
		Assert(cch >= 0);
		pnm->NotifyPostReplaceRange((ITxNotify *)this, CP_INFINITE, 0, 0,
				GetCp(), GetCp() + cch);
	}
}

/*
 *	CRchTxtPtr::IsRich()
 *	
 *	@mfunc
 *		Determine whether rich-text operation is operable
 *	
 *	@rdesc
 *		TRUE if associated CTxtEdit::_fRich = 1, i.e., control is allowed
 *		to be rich.
 */
BOOL CRchTxtPtr::IsRich()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::IsRich");

	return GetPed()->IsRich();
}

/*
 *	CRchTxtPtr::Check_rpCF()
 *	
 *	@mfunc
 *		enable _rpCF if it's not already enabled
 *	
 *	@rdesc
 *		TRUE if _rpCF is enabled
 */
BOOL CRchTxtPtr::Check_rpCF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::Check_rpCF");

	if(_rpCF.IsValid())
		return TRUE;

	if(!_rpCF.InitRuns (GetCp(), GetTextLength(),
				&(GetPed()->GetTxtStory()->_pCFRuns)))
	{
		return FALSE;
	}

	CNotifyMgr *pnm = GetPed()->GetNotifyMgr();	// For notifying of changes
	if(pnm)
		pnm->NotifyPostReplaceRange(	 		// Notify interested parties
				(ITxNotify *)this, CP_INFINITE,	//  that
				0, 0, CP_INFINITE, CP_INFINITE);

	return TRUE;
}

/*
 *	CRchTxtPtr::Check_rpPF()
 *	
 *	@mfunc
 *		enable _rpPF if it's not already enabled
 *	
 *	@rdesc
 *		TRUE if _rpPF is enabled
 */
BOOL CRchTxtPtr::Check_rpPF()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::Check_rpPF");

	if(_rpPF.IsValid())
		return TRUE;

	if(!IsRich())
		return FALSE;

	if(!_rpPF.InitRuns (GetCp(), GetTextLength(),
				&(GetPed()->GetTxtStory()->_pPFRuns)))
	{
		return FALSE;
	}

	if (IsParaRTL())
		_rpPF.GetRun(0)->_level._value = 1;		// Set default paragraph base level

	CNotifyMgr *pnm = GetPed()->GetNotifyMgr();	// For notifying of changes
	if(pnm)
		pnm->NotifyPostReplaceRange(	 		// Notify interested parties
				(ITxNotify *)this, CP_INFINITE,	// of the change.
				0, 0, CP_INFINITE, CP_INFINITE);

	return TRUE;
}

/*
 * CRchTxtPtr::FindWordBreak(action, cpMost)
 *
 *	@mfunc
 *		Same as CTxtPtr::FindWordBreak(), but moves the whole rich text ptr
 *
 *	@rdesc
 *		cch this rich text ptr is moved
 */
LONG CRchTxtPtr::FindWordBreak(
	INT		action,		//@parm Kind of word break to find
	LONG	cpMost)		//@parm Limiting character position
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::FindWordBreak");

	LONG cch = _rpTX.FindWordBreak(action, cpMost);
	_rpCF.Move(cch);
	_rpPF.Move(cch);

	return cch;
}

/*
 *	CRchTxtPtr::BindToCp(dwNewCp)
 *
 *	@mfunc
 *		Set cp to new value and recalculate that new position.
 */
void CRchTxtPtr::BindToCp(
	LONG cp)			// @parm new cp for rich text
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::BindToCp");

	_rpTX.BindToCp(cp);				// Recalculate cp for plain text

	// Use the InitRunPtrs routine so that the run pointers will get
	// re-initialized and rebound with the correct run array.  The
	// run array formerly used (if any at all) is not necessarily valid
	// when this function is called.

	InitRunPtrs();

	// Do invariant testing at end because this fixes up the rich text
	// pointer in the face of backing store changes.
	_TEST_INVARIANT_
}

/*
 *	CRchTxtPtr::CheckFormatRuns ()
 *
 *	@mfunc
 *		Check the format runs against what's in CTxtStory.  If
 *		different, forces a rebind to <p cp>
 */
void CRchTxtPtr::CheckFormatRuns()
{
	CTxtStory *pStory = GetPed()->GetTxtStory();

	if (pStory->GetCFRuns() != (CFormatRuns *)_rpCF._pRuns ||
		pStory->GetPFRuns() != (CFormatRuns *)_rpPF._pRuns)
	{
		InitRunPtrs();
	}

	_TEST_INVARIANT_
}

/*
 *	CRchTxtPtr::ChangeCase(cch, Type, publdr)
 *	
 *	@mfunc
 *		Change case of cch chars starting at this text ptr according to Type,
 *		which has the possible values:
 *
 *		tomSentenceCase	= 0: capitalize first letter of each sentence
 *		tomLowerCase	= 1: change all letters to lower case
 *		tomUpperCase	= 2: change all letters to upper case
 *		tomTitleCase	= 3: capitalize the first letter of each word
 *		tomToggleCase	= 4: toggle the case of each letter
 *	
 *	@rdesc
 *		TRUE iff a change occurred
 *
 *	@devnote
 *		Since this routine only changes the case of characters, it has no
 *		effect on rich-text formatting.  However it is part of the CRchTxtPtr
 *		class in order to notify the display of changes.  CTxtRanges are also
 *		notified just in case the text blocks are modified.
 */
BOOL CRchTxtPtr::ChangeCase (
	LONG		  cch,			//@parm # chars to change case for
	LONG		  Type,			//@parm Type of change case command
	IUndoBuilder *publdr)		//@parm UndoBuilder to receive anti-event
								//  	for any replacements
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::ChangeCase");
	_TEST_INVARIANT_

#define	BUFFERLEN	256

	LONG	cchChunk, cchFirst, cchGet, cchLast, cchRep;
	LONG	cpSave = GetCp();
	BOOL	fAlpha, fToUpper, fUpper;			// Flags controling case change
	BOOL	fChange = FALSE;					// No change yet
	BOOL	fStart = TRUE;						// Start of Word/Sentence
	TCHAR *	pch;								// Ptr to walk rgCh with
	WORD *	pType;								// Ptr to walk rgType with
	WCHAR	rgCh[BUFFERLEN];					// Char buffer to work in
	WORD	rgType[BUFFERLEN];					// C1_TYPE array for rgCh

	if( GetCp() )
	{
		if( Type == tomSentenceCase )
			fStart = _rpTX.IsAtBOSentence();

		else if( Type == tomTitleCase )
		{
			// Check to see if we are at the beginning of
			// a word.  This is the case if the character preceding
			// our current position is white space.
			fStart = IsWhiteSpace(GetPrevChar());
		}
	}
	if(cpSave + cch > GetPed()->GetAdjustedTextLength())
		cch = GetPed()->GetAdjustedTextLength() - cpSave;

	// Handle pre-replace range notifications.  This method is very
	// useful for delayed rendering of data copied to the clipboard.
	CNotifyMgr *pnm = GetPed()->GetNotifyMgr();
	if(pnm)
	{
		pnm->NotifyPreReplaceRange((ITxNotify *)this, cpSave, cch,
			cch, cpSave, cpSave + cch);
	}
	while(cch > 0)								// Do 'em all (or as many as
	{											//  in story)
		cchChunk = min(BUFFERLEN, cch);			// Get next bufferful
		cch -= cchChunk;						// Decrement the count
		cchGet = _rpTX.GetText(cchChunk, rgCh);	// Manipulate chars in buffer
		if(cchGet < cchChunk)					//  (for undo, need to use
		{										//  ReplaceRange())
			cch = 0;							// No more chars in story,
			if(!cchGet)							//  so we'll be done
				break;							// We're done already
			cchChunk = cchGet;					// Something in this chunk
		}

		W32->GetStringTypeEx(0, CT_CTYPE1, rgCh,// Find out whether chars are
						cchChunk, rgType);		//  UC, LC, or neither
		cchLast = 0;							// Default nothing to replace
		cchFirst = -1;
		for(pch = rgCh, pType = rgType;			// Process buffered chars
			cchChunk;
			cchChunk--, pch++, pType++)
		{
			fAlpha = *pType & (C1_UPPER | C1_LOWER); // Nonzero if UC or LC
			fUpper = (*pType & C1_UPPER) != 0;	// TRUE if UC
			fToUpper = fStart ? TRUE : fUpper;	// capitalize first letter of a
												// sentence
			switch(Type)
			{									// Decide whether to change
			case tomLowerCase:					//  case and determine start
				fToUpper = FALSE;				//  of word/sentence for title
				break;							//  and sentence cases

			case tomUpperCase:
				fToUpper = TRUE;
				break;

			case tomToggleCase:
				fToUpper = !fUpper;
				break;

			case tomSentenceCase:
				if(*pch == TEXT('.'))			// If sentence terminator,
					fStart = TRUE;				//  capitalize next alpha
				if(fAlpha)						// If this char is alpha, next
					fStart = FALSE;				//  char can't start a
				break;							//  sentence

			case tomTitleCase:					// If this char is alpha, next
				fStart = (fAlpha == 0);			//  char can't start a word
				break;
			default:
				return FALSE;
			}

			if(fAlpha && (fToUpper ^ fUpper))	// Only change case if it
			{									//  makes a difference (saves
				if(fToUpper)					//  on system calls and undos)
					CharUpperBuff(pch, 1);
				else
					CharLowerBuff(pch, 1);

				fChange = TRUE;					// Return value: change made
				if( cchFirst == -1 )			// Save cch of unchanged
					cchFirst = cchGet-cchChunk;	//  leading string
				cchLast = cchChunk - 1;			// Save cch of unchanged
			}									//  trailing string
		}
		if( cchFirst == -1 )
		{
			Assert(cchLast == 0);
			cchFirst = cchGet;
		}
		Move(cchFirst);							// Skip unchanged leading
		cchGet -= cchFirst + cchLast;			//  string. cchGet = cch of
												//  changed span.
		cchRep = _rpTX.ReplaceRange(cchGet, cchGet, rgCh + cchFirst, publdr, NULL, NULL);
		_rpCF.Move(cchRep);
		_rpPF.Move(cchRep);
		Assert(cchRep == cchGet);
		Move(cchLast);							// Skip unchanged trailing
	}											//  string
	if(pnm)
	{
		cch = GetCp() - cpSave;
		pnm->NotifyPostReplaceRange((ITxNotify *)this, cpSave, cch,
			cch, cpSave, GetCp());
	}
	return fChange;
}

// The following defines a mask for Units implemented by UnitCounter()
#define IMPL ((1 << tomCharacter)  + (1 << tomWord) + (1 << tomSentence) + \
			  (1 << tomParagraph)  + (1 << tomLine) + (1 << tomStory) +	\
			  (1 << tomCharFormat) + (1 << tomParaFormat) + (1 << tomObject) + \
			  (1 << tomPage)	   + (1 << tomCell))
 
/*
 *	CRchTxtPtr::UnitCounter (Unit, &cUnit, cchMax, fNotAtBOL)
 *
 *	@mfunc
 *		Helper function to count chars in <p cUnit> Units defined by <p Unit>
 *		<p cUnit> is a signed count.  If it extends beyond either end of the
 *		story, count up to that end and update <p cUnit> accordingly.  If
 *		<p cchMax> is nonzero, stop counting when the count exceeds <p cchMax>
 *		in magnitude.
 *
 *	@rdesc
 *		If unit is implemented, return cch corresponding to the units counted
 *		(up to a maximum magnitude of <p cchMax>) and update cUnit;
 *		else return tomForward to signal unit not implemented and cUnit = 0.
 *		If unit is implemented but unavailable, e.g., tomObject with no
 *		embedded objects, return tomBackward.
 *
 *	@devnote
 *		This is the basic engine used by the TOM CTxtRange::Move() and Index()
 *		methods.
 */
LONG CRchTxtPtr::UnitCounter (
	LONG	Unit,				//@parm Type of unit to count
	LONG &	cUnit,				//@parm Count of units to count chars for
	LONG	cchMax,				//@parm Maximum character count
	BOOL	fNotAtBOL)			//@parm TRUE if _fSel && _fCaretNotAtBOL
{
	TRACEBEGIN(TRCSUBSYSTOM, TRCSCOPEINTERN, "CRchTxtPtr::UnitCounter");

	LONG	action;				// Gives direction and tomWord commands
	LONG	cch;				// Collects cch counted
	LONG	cchText = GetTextLength();
	LONG	cp = GetCp();
	LONG	iDir = cUnit > 0 ? 1 : -1;
	LONG	j;					// For-loop index
	CDisplay *pdp;				// Used for tomLine case

	// Valid attribute Units are high bit plus any combo of CFE_xxx. 
	// CFE_REVISED is most significant value currently defined.
	if(Unit > 0 && !((IMPL >> Unit) & 1) ||
	   Unit < 0 && (Unit & ~(2*CFM_REVISED - 1 + 0x80000000)))
	{
		return tomForward;						// Report invalid Unit
	}

	if(!cUnit)									// Nothing to count
		return 0;

	if(cchMax <= 0)
		cchMax = tomForward;					// No cch limit

	if(cUnit < 0)
		cchMax = min(cp, cchMax);				// Don't go before doc start

	else if(cchMax > cchText - cp)
		cchMax = cchText - cp;					// Don't go beyond doc end

	if(Unit < 0)
	{
		CCFRunPtr rp(*this);
		cch = rp.CountAttributes(cUnit, cchMax, cp, cchText, Unit);
		goto finish;
	}

	switch(Unit)
	{
	case tomCharacter:							// Smallest Unit
		cp += cUnit;							// Requested new cp
		ValidateCp(cp);							// Make sure it's OK
		cch = cUnit = cp - GetCp();				// How many cch, cUnits
		break;									//  actually moved

	case tomStory:								// Largest Unit
		cch = (cUnit > 0) ? cchText - cp : -cp;	// cch to start of story
		cUnit = cch ? iDir : 0;					// If already at end/start,
		break;									//  of story, no count

	case tomCharFormat:							// Constant CHARFORMAT
		cch = _rpCF.CountRuns(cUnit, cchMax, cp, cchText);
		break;

	case tomParaFormat:							// Constant PARAFORMAT
		cch = _rpPF.CountRuns(cUnit, cchMax, cp, cchText);
		break;

	case tomObject:
		if(!GetObjectCount())					// No objects: can't move, so
		{
			cUnit = 0;							//  set cUnit = 0 and
			return tomBackward;					//  signal Unit unavailable
		}
		cch = GetPed()->_pobjmgr->CountObjects(cUnit, GetCp());
		break;

	case tomCell:
		{
			CTxtRange rg(*this);
			cch = rg.CountCells(cUnit, cchMax);
		}
		break;

	case tomScreen:								// Could be supported 
		if(!GetPed()->IsInPageView())			//  in Normal View using 
			return tomBackward;					//  ITextSelection::Down()
		Unit = tomPage;							// In Page View, it's an alias

	case tomPage:
	case tomLine:
		pdp = GetPed()->_pdp;
		if(pdp)									// If this story has a display
		{										//  use a CLinePtr
			CLinePtr rp(pdp);
			//REVIEW (keithcu) This should be fixed. We can't trust the client to pass
			//down an optimal cchMax. For best results, we should have a CountRuns and CountPages
			//recalc as needed when moving down through the document.
			pdp->WaitForRecalc(min(cp + cchMax, cchText), -1);
			rp.SetCp(cp, FALSE);
			fNotAtBOL = fNotAtBOL && rp.GetLineIndex() &&
						(Unit == tomLine || rp->_fFirstOnPage);
			cch = (Unit == tomLine || !cUnit || !rp.IsValid())
				? rp.CountRuns (cUnit, cchMax, cp, cchText)
				: rp.CountPages(cUnit, cchMax, cp, cchText);

			if (cch == tomBackward)
				return tomBackward;
			
			if(fNotAtBOL && cUnit < 0)
				cUnit++;						// Keep on same line/page as selection
			break;
		}										
		if(Unit == tomPage)
		{										// No display: no pagination
			cUnit = 0;
			return tomBackward;					
		}										// For tomLine, fall thru to
												//  treat as tomPara
	default:									// tp dependent cases
	  {											// Block to contain tp() which
		CTxtPtr tp(_rpTX);						//  takes time to construct

		if (cUnit < 0)							// Counting backward
		{
			action = (Unit == tomWord)
				? WB_MOVEWORDLEFT : tomBackward;
		}
		else									// Counting forward
		{
			action = (Unit == tomWord)
				? WB_MOVEWORDRIGHT : tomForward;
		}
	
		for (cch = 0, j = cUnit; j && abs(cch) < cchMax; j -= iDir)
		{
			cp = tp.GetCp();					// Save starting cp for
			switch (Unit)						//  calculating cch for this
			{									//  Unit
			case tomWord:
				tp.FindWordBreak(action);
				break;
	
			case tomSentence:
				tp.FindBOSentence(action);
				break;
		
			case tomLine:						// Story has no line array:
			case tomParagraph:					//  treat as tomParagraph
				tp.FindEOP(action);
				break;
		
			default:
				cUnit = 0;
				return tomForward;				// Return error
			}
			if(tp.GetCp() - cp == 0)			// No count:
				break;							//  don't decrement cUnit
			cch += tp.GetCp() - cp;
		}
		cUnit -= j;								// Discount any runs not
	  }											//  counted if |cch| >= cchMax
	}

finish:
	if(abs(cch) > cchMax)						// Keep cch within requested
	{											//  limit
		cch = cch > 0 ? cchMax : -cchMax;
		if(Unit == tomCharacter)
			cUnit = cch;
	}		

	Move(cch);									// Move to new position
	return cch;									// Total cch counted
}

/*
 *	Notes on RichEdit 1.0 mode:
 *
 *	CF_UNICODETEXT should not be used in RichEdit 1.0 mode.  \uN should use
 *	the alternative.
 *
 *	CleanseAndReplaceRange() and the RTF reader need to ensure that any
 *	Unicode chars entered belong to a CharSet and stamp it accordingly.
 *	If no CharSet exists for the character, then blank should be used.
 */

/*
 *	CRchTxtPtr::GetCachFromCch(cch)
 *
 *	@mfunc
 *		Return count of A chars corresponding to cch W chars starting at
 *		this text ptr.  On first call start with this text ptr at cp = 0.
 *
 *	@rdesc
 *		Count of A chars between this text ptr and cp
 *
 *	@comm
 *		The algorithm assumes that for a DBCS charset any character
 *		above 128 has two bytes, except for the halfwidth KataKana,
 *		which are single bytes in ShiftJis.
 */
LONG CRchTxtPtr::GetCachFromCch(
	LONG cch)		//@parm Count of chars to check
{
	BYTE		 iCharRep;
	LONG		 cach = 0;				// No ach counted yet
	LONG		 cch1;
	LONG		 cchRun;				// CF run count
	LONG		 cchValid;				// Text run count
	WCHAR		 ch;
	const WCHAR *pch;					// Ptr to text run
	const CCharFormat *pCF;

	while(cch > 0)
	{
		cchRun = _rpCF.IsValid()
			   ? _rpCF.GetCchLeft()
			   : GetTextLength() - GetCp();
		if(!cchRun)
			break;						// No more text
		pCF		 = GetCF();
		iCharRep = pCF->_iCharRep;
		if (!IsFECharRep(iCharRep) ||
			(pCF->_dwEffects & CFE_RUNISDBCS))
		{
			cchRun = min(cchRun, cch);
			cach  += cchRun;			// SBCS run or DBCS stored as
			cch   -= cchRun;			//  one byte per char
			Move(cchRun);
			continue;
		}
		pch = GetPch(cchValid);
		Assert(pch);
		cchValid = min(cchValid, cchRun);
		for(cch1 = 0; cch > 0 && cchValid--; cch1++)
		{
			cch--;
			ch = *pch++;
			if(IN_RANGE(128, ch, 0xFFF0) &&
				(iCharRep != SHIFTJIS_INDEX || !IN_RANGE(0xFF61, ch, 0xFF9F)))
			{
				cach++;
			}
		}
		cach += cch1;
		Move(cch1);
	}
	return cach;
}

/*
 *	CRchTxtPtr::GetCchFromCach(cach)
 *
 *	@mfunc
 *		Return count of W chars corresponding to cach A chars starting at this
 *		text ptr. On first call start with this text ptr at cp = 0.
 *
 *	@rdesc
 *		Count of W chars corresponding to cach A chars starting at this tp.
 *
 *	@comm
 *		The algorithm assumes that for a DBCS charset any character
 *		above 128 has two bytes, except for the halfwidth KataKana,
 *		which are single bytes in ShiftJis.
 */
LONG CRchTxtPtr::GetCchFromCach(
	LONG cach)		//@parm Count of ach's starting at this text ptr
{
	BYTE		 iCharRep;
	LONG		 cch = 0;				// No ch's yet
	LONG		 cch1;
	LONG		 cchRun;				// CF run count
	LONG		 cchValid;				// Text run count
	WCHAR		 ch;
	const WCHAR *pch;					// Ptr to text run
	const CCharFormat *pCF;

	while(cach > 0)
	{
		cchRun = _rpCF.IsValid()
			   ? _rpCF.GetCchLeft()
			   : GetTextLength() - GetCp();
		if(!cchRun)
			break;						// No more text
		pCF		 = GetCF();
		iCharRep = pCF->_iCharRep;
		if (!IsFECharRep(iCharRep) ||
			(pCF->_dwEffects & CFE_RUNISDBCS))
		{
			cchRun = min(cchRun, cach);	// SBCS run or DBCS stored as
			cach -= cchRun;				//  one byte per char
			cch  += cchRun;
			Move(cchRun);
			continue;
		}
		pch = GetPch(cchValid);
		Assert(pch);
		cchValid = min(cchValid, cchRun);
		for(cch1 = 0; cach > 0 && cchValid--; cch1++)
		{
			cach--;
			ch = *pch++;
			if(IN_RANGE(128, ch, 0xFFF0) &&
				(iCharRep != SHIFTJIS_INDEX || !IN_RANGE(0xFF61, ch, 0xFF9F)))
			{
				cach--;
			}
		}
		cch += cch1;
		Move(cch1);
	}
	return cch;
}

/*
 *	CRchTxtPtr::Zombie ()
 *
 *	@mfunc
 *		Turn this object into a zombie by NULLing out its _ped member
 */
void CRchTxtPtr::Zombie ()
{
	TRACEBEGIN(TRCSUBSYSBACK, TRCSCOPEINTERN, "CRchTxtPtr::Zombie");

	_rpTX.Zombie();
	_rpCF.SetToNull();
	_rpPF.SetToNull();
}
