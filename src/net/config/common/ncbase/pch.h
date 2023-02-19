#pragma once

#define COM_NO_WINDOWS_H
#define RPC_NO_WINDOWS_H
#define NOCOMM
#define NOCRYPT
//#define NOGDI
#define NOICONS
#define NOIME
#define NOMCX
#define NOMDI
#define NOMENUS
#define NOMETAFILE
#define NOSOUND
#define NOSYSPARAMSINFO
#define NOWH
#define NOWINABLE
#define NOWINRES

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>

// This avoids duplicate definitions with Shell PIDL functions
// and MUST BE DEFINED!
#define AVOID_NET_CONFIG_DUPLICATES

#include <windows.h>
#include <objbase.h>

#include <cfgmgr32.h>
#include <devguid.h>
#include <infstr.h>
#include <setupapi.h>
#include <shellapi.h>
#include <wchar.h>

#include "stllist.h"
#include "stlvec.h"
using namespace std;

#include "ncmem.h"
#include "ncbase.h"
#include "ncdebug.h"
#include "ncdefine.h"
