/*++

Copyright (c) 1999  Microsoft Corporation

Module Name:

    wmlmacro.h

Abstract:

    This file defines macro for an easy wmi tracing.

Author:

    gorn

Revision History:

--*/
#ifndef WMLMACRO_H
#define WMLMACRO_H 1

typedef struct {char x[418957];} WMILIBTYPE_STRING;

#undef  WMILIB_TYPEDEF
#define WMILIB_TYPEDEF(_TypeName, _EquivType, _FmtStr, _Func, _MofStr, _InitType) \
    typedef _EquivType WMILIBTYPE_ ## _TypeName ;
#include "wmltypes.inc"

typedef union _WMLLOCALSTRUCT {
    UCHAR    uchar;
    USHORT   ushort;
    ULONG    uint;
    WCHAR    wchar;
    LONGLONG longlong;
} WMLLOCALSTRUCT;

#define WMLLOCAL WMLLOCALSTRUCT _wmllocal


#define WMILIB_CHECKED_ZERO(_Value, _Type) \
    (0 * (1/(int)!(sizeof(_Type) - sizeof(_Value) )))

#define WMILIB_CHECKED_SIZEOF(_Value, _Type) \
    (sizeof(_Value) + WMILIB_CHECKED_ZERO( _Value, _Type) )

#define WMILIB_LOGPAIR(_a, _b) (_a),(_b),

#define WMILIB_LOGARGVALTYPE(_value, _type) \
            WMILIB_LOGPAIR(WMILIB_CHECKED_SIZEOF(_value, _type), &(_value) )

#define LOG(_TypeName, _Value)                         \
    WMILIB_LOGARGVALTYPE( _Value, WMILIBTYPE_ ## _TypeName)

#define LOGASTR(_value) \
    WMILIB_LOGPAIR( strlen(_value) + WMILIB_CHECKED_ZERO((_value)[0],CHAR), _value )

#define LOGWSTR(_value) \
    WMILIB_LOGPAIR( wcslen(_value) * sizeof(WCHAR) + WMILIB_CHECKED_ZERO((_value)[0],WCHAR), _value)

#define LOGCSTR(_x) \
    WMILIB_LOGPAIR( sizeof((_x).Length) + WMILIB_CHECKED_ZERO(_x,STRING), &(_x).Length ) \
    WMILIB_LOGPAIR( (_x).Length, (_x).Buffer )

#define LOGUSTR(_x)                                                            \
    WMILIB_LOGPAIR( sizeof((_x).Length)                                        \
                    + WMILIB_CHECKED_ZERO((_x),UNICODE_STRING), &(_x).Length)  \
    WMILIB_LOGPAIR( (_x).Length, (_x).Buffer )

#define LOGCHARARR(_count, _x)                                 \
    WMILIB_LOGARGVALTYPE( _wmllocal.ushort , USHORT )          \
    WMILIB_LOGPAIR( (_wmllocal.ushort = _count * sizeof(CHAR)) \
                    + WMILIB_CHECKED_ZERO((_x)[0], CHAR), _x ) 

#define LOGWCHARARR(_count, _x)                                 \
    WMILIB_LOGARGVALTYPE( _wmllocal.ushort , USHORT )           \
    WMILIB_LOGPAIR( (_wmllocal.ushort = _count * sizeof(WCHAR)) \
                    + WMILIB_CHECKED_ZERO((_x)[0], WCHAR), _x ) 

#define LOGTIME(_Value)    LOG(TIMENT,  _Value)
#define LOGPTR(_Value)     LOG(PTR,     _Value)
#define LOGHANDLE(_Value)  LOG(HANDLE,  _Value)
#define LOGSTATUS(_Value)  LOG(XINT,    _Value)
#define LOGBYTE(_Value)    LOG(UBYTE,   _Value)
#define LOGULONG(_Value)   LOG(UINT,    _Value)
#define LOGULONGLONG(_Value)   LOG(ULONGLONG,    _Value)
#define LOGXLONG(_Value)   LOG(XINT,    _Value)
#define LOGXSHORT(_Value)  LOG(XSHORT,  _Value)
#define LOGUSHORT(_Value)  LOG(USHORT,  _Value)
#define LOGUCHAR(_Value)   LOG(UCHAR,   _Value)
#define LOGUBYTE(_Value)   LOG(UCHAR,   _Value)
#define LOGIRQL(_Value)    LOG(UCHAR,   _Value)
#define LOGBOOL(_Value)    LOG(BOOL,    _Value)
#define LOGBOOLEAN(_Value) LOG(BOOLEAN, _Value)
#define LOGARSTR(_Value)   LOGASTR(_Value)
#define LOGPNPMN(_Value)   LOG(UCHAR,   _Value)
#define LOGIOCTL(_Value)   LOG(ULONG,   _Value)
#define LOGGUID(_Val)      LOG(GUID, _Val)
#define LOGSLONG(_Val)     LOG(SINT, _Val)
#define LOGTIMEDELTA(_Val) LOG(TIMEDELTA, _Val)
#define LOGWAITTIME(_Val)  LOG(WAITTIME, _Val)

// These better be defined elsewhere
//#define WML_FLAGS(_Val)  ( (ULONG) (_Val) )
//#define WML_STREAM(_Val) ( (ULONG) ((_Val) >> 32) )

#define WML_CONTROL(_prefix, _flags) \
            ( _prefix ## ControlGuids[ WML_STREAM(_flags) ] )
            
#define WML_TRACEGUID(_prefix, _id) \
            ( _prefix ## TraceGuids[ WML_GUID(_id) ] )
            
#define WML_ENABLED(_prefix, _flags) \
            ( WML_CONTROL(_prefix, _flags).EnableFlags & WML_FLAGS(_flags) )
            
#define WML_LOG(_prefix, _flags, _id, _arg) \
    do { \
        if ( WML_ENABLED(_prefix, _flags) ) { \
            WmlTrace( WML_ID(_id), \
                     &WML_TRACEGUID(_prefix, _id) , \
                      WML_CONTROL(_prefix, _flags).LoggerHandle, _arg); \
        } \
    } while (0)            


#endif // WMLMACRO_H
