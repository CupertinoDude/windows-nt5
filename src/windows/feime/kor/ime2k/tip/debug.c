//
//

// This file cannot be compiled as a C++ file, otherwise the linker
// will bail on unresolved externals (even with extern "C" wrapping 
// this).

#include "private.h"

// Define some things for debug.h
//
#define SZ_DEBUGINI     "cicero.ini"
#define SZ_DEBUGSECTION "IMEKRCIC"
#define SZ_MODULE       "IMEKRCIC"
#define DECLARE_DEBUG
#include <debug.h>
