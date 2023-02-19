/*++

Copyright (c) 1994-1998 Microsoft Corporation

Module Name:

    dbgutil.h

Abstract:

    This module declares the macros to wrap around DEBUG_PRINTS class
    defined in pudebug.h    
  
    This is the exported header file, that the users are allowed to modify.
    If there are no custom definitions, please use the templated version
    in the root iisrearc\inc\dbgutil.h
 
Author:

    Murali R. Krishnan (MuraliK)    22-Sept-1994

--*/


# ifndef _DBGUTIL_H_
# define _DBGUTIL_H_



// begin_user_modifiable

//
//  Modify the following flags if necessary
//

# define DEFAULT_OUTPUT_FLAGS   ( DbgOutputStderr )

// end_user_modifiable


//
// include standard debug support
//
// note: define DEFAULT_OUTPUT_FLAGS before including pudebug.h
//

# include <pudebug.h>


// begin_user_modifiable

// Use the default constants from pudebug.h: 0x00000001 to 0x00000100

//
//  Define the debugging constants as bit-flag values
//  Example: # define DEBUG_FOOBAR 0x00010000
//  Note: All debugging bit-values below 0x00001000 are reserved!
// 
//  If you add or change a flag, please update FlagTableWp
//  in core\common\dtflags\dtflags.c
//

# define    DEBUG_UL_CALLS             0x00010000

# define    DEBUG_NREQ                 0x00040000
# define    DEBUG_WPIPM                0x00080000

# define    DEBUG_TRACE                0x10000000
# define    DEBUG_DUMPS                0x20000000

#if DBG

//
// handy trace macro
//
#define WpTrace(a, _b_)     \
do {                        \
    IF_DEBUG(##a)           \
    {                       \
        PuDbgPrint _b_ ;      \
    }                       \
}while (0)

#else // !DBG

#define WpTrace(a, _b_)        ((void) 0)  /* do nothing */

#endif



// end_user_modifiable


# endif  /* _DBGUTIL_H_ */

