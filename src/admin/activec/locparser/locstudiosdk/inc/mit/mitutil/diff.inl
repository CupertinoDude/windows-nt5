//-----------------------------------------------------------------------------
//  
//  File: diff.inl
//  Copyright (C) 1994-1997 Microsoft Corporation
//  All rights reserved.
//  
//-----------------------------------------------------------------------------
 
inline
CDifference::~CDifference()
{
}

inline
CDelta::~CDelta()
{
}

inline
CDiffAlgorithm::~CDiffAlgorithm()
{
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Calculates the delta following the given algorithm and then traverses
//	the delta and calls back the delta visitor for each difference in the delta.
//------------------------------------------------------------------------------
inline
void 
CDiffEngine::Diff(
	CDiffAlgorithm & diffalg, 
	const wchar_t * seq1, // [in] old sequence
	const wchar_t * seq2, // [in] new sequence
	const CDeltaVisitor & dv)
{
	CDelta * d;
	d = diffalg.CalculateDelta(seq1, seq2);
	d->Traverse(dv);
	delete d;
}

