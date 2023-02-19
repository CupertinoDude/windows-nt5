/*-----------------------------------------------------------------------------
Microsoft SXAPW

Microsoft Confidential
Copyright 1995-2000 Microsoft Corporation. All Rights Reserved.

@doc external
@module PreprocessorCharize.h

@owner JayKrell
-----------------------------------------------------------------------------*/
#if !defined(VS_COMMON_INC_SXAPW_PREPROCESSORCHARIZE_H_INCLUDED_) // {
#define VS_COMMON_INC_SXAPW_PREPROCESSORCHARIZE_H_INCLUDED_
/*#pragma once ends up in .rgi, which is bad, so do not do it*/

#include "SxApwPreprocessorPaste.h"

#define SxApwPrivatePreprocessorCharize(x) #@ x

/*-----------------------------------------------------------------------------
Name: SxApwPreprocessorCharize, SxApwPreprocessorCharizeW
@macro
These macros simply charize their parameter, after evaluating it;
it is evaluated so that
define A B
SxApwPreprocessorCharize(A) -> 'B' instead of 'A'
SxApwPreprocessorCharizeW(A) -> 'B' instead of L'A'
@owner JayKrell
-----------------------------------------------------------------------------*/
#define SxApwPreprocessorCharize(x) SxApwPrivatePreprocessorCharize(x)
#define SxApwPreprocessorCharizeW(x) SxApwPreprocessorPaste(L, SxApwPrivatePreprocessorCharize(x))

#endif // }
