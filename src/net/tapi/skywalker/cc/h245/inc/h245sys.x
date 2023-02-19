/******************************************************************************
 *
 *   INTEL Corporation Proprietary Information				   
 *   Copyright (c) 1994, 1995, 1996 Intel Corporation.				   
 *									   
 *   This listing is supplied under the terms of a license agreement	   
 *   with INTEL Corporation and may not be used, copied, nor disclosed	   
 *   except in accordance with the terms of that agreement.		   
 *
 *****************************************************************************/

/******************************************************************************
 *									   
 *  $Workfile:   h245sys.x  $						
 *  $Revision:   1.2  $							
 *  $Modtime:   17 Jan 1997 14:39:22  $					
 *  $Log:   S:\sturgeon\src\h245\include\vcs\h245sys.x_v  $	
   
      Rev 1.2   24 Jan 1997 19:37:26   SBELL1
   upgraded to oss 4.2: added OSS defs return types of memory allocation funcs.
   
      Rev 1.1   07 Jun 1996 18:15:46   EHOWARDX
   Change H245_malloc, H245_realloc, and H245_free to macros.
   
      Rev 1.0   09 May 1996 21:05:06   EHOWARDX
   Initial revision.
 * 
 *    Rev 1.11.1.0   09 May 1996 19:38:26   EHOWARDX
 * Redesigned locking logic and added new functionality.
   
      Rev 1.11   26 Mar 1996 09:48:40   cjutzi
   
   - ok.. Added Enter&Leave&Init&Delete Critical Sections for Ring 0
   
      Rev 1.10   25 Mar 1996 18:06:48   cjutzi
   
   - wow.. this is emparassing.. undid what I undid.. 
   
      Rev 1.9   25 Mar 1996 17:54:02   cjutzi
   
   - backstep broke build
   
      Rev 1.8   25 Mar 1996 17:21:18   cjutzi
   
   - added oil level EnterCritical Section.. 
   
      Rev 1.7   19 Mar 1996 21:00:58   cjutzi
   - change realloc to take proper parameter size_t
   
      Rev 1.6   19 Mar 1996 17:41:58   helgebax
   
   added defintion for timer callbacks
   
      Rev 1.5   18 Mar 1996 12:41:52   cjutzi
   
   - added multiple timer queue
   
      Rev 1.4   02 Mar 1996 22:17:08   DABROWN1
   
   removed prototypes for h245_bcopy/h245_bzero
   changed prototype for h245_free to 1 parameter (ptr)
   
      Rev 1.3   26 Feb 1996 12:41:56   cjutzi
   
   - added bcopy
   
      Rev 1.2   09 Feb 1996 16:46:04   cjutzi
   
   - added Dollar Log.. 
 *  $Ident$
 *
 *****************************************************************************/
#ifndef _H245SYS_X_
#define _H245SYS_X_

#include "av_asn1.h"

/* MACROS */
#define H245_min(x,y) ( ( (x)>(y) )?(y):(x) )

#if defined(_IA_SPOX_)
# define H245_malloc(dwSize)        _H245_malloc(dwSize)
# define H245_realloc(pBuf, dwSize) _H245_realloc(pBuf, dwSize)
# define H245_free(pBuf)            _H245_free(pBuf)
# define DLL_ENTRY_FDEF				
#else
# if defined(_MALLOC_DBG)
#  include <crtdbg.h>
#  define H245_malloc(dwSize)        _malloc_dbg(dwSize, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define H245_realloc(pBuf, dwSize) _realloc_dbg(pBuf, dwSize, _NORMAL_BLOCK, __FILE__, __LINE__)
#  define H245_free(pBuf)            _free_dbg(pBuf, _NORMAL_BLOCK)
# else
#  define H245_malloc(dwSize)        malloc(dwSize)
#  define H245_realloc(pBuf, dwSize) realloc(pBuf, dwSize)
#  define H245_free(pBuf)            free(pBuf)
# endif
#endif

void * DLL_ENTRY_FDEF _H245_malloc  ( size_t dwSize);
void * DLL_ENTRY_FDEF _H245_realloc ( void *pBuf, size_t dwSize ); 
void   DLL_ENTRY_FDEF _H245_free    ( void *pBuf );

typedef  (*H245TIMERCALLBACK)(struct InstanceStruct *pInstance, DWORD, void*);

DWORD H245StartTimer (struct InstanceStruct *pInstance, void *context, H245TIMERCALLBACK cbTimer, DWORD dwTimeout);
DWORD H245StopTimer  (struct InstanceStruct *pInstance, DWORD id);
//void  H245InitTimer  (struct InstanceStruct *pInstance);
//void  H245DeInitTimer(struct InstanceStruct *pInstance);


#ifdef _IA_SPOX_

#include <oil.x>			/* need to define LockHandle */

typedef LockHandle CRITICAL_SECTION;
PUBLIC RESULT InitializeCriticalSection(CRITICAL_SECTION * phLock);
PUBLIC RESULT EnterCriticalSection(CRITICAL_SECTION * phLock);
PUBLIC RESULT LeaveCriticalSection(CRITICAL_SECTION * phLock);
PUBLIC RESULT DeleteCriticalSection(CRITICAL_SECTION * phLock);

#endif /* _IA_SPOX_ */

#endif /* _H245SYS_X_ */
