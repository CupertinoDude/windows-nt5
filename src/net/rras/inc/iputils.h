/*++

Copyright (c) 1995  Microsoft Corporation

Module Name:

    net\ip\rtrmgr\defs.c

Abstract:
    IP Router Manager defines

Revision History:

    Gurdeep Singh Pall          6/16/95  Created

--*/

#ifndef __IPUTILS_H__
#define __IPUTILS_H__

//
// LONG
// Cmp(DWORD dwFirst, DWORD dwSecond, LONG lResult)
//

#define Cmp(dwFirst,dwSecond,lResult) ((LONG)((lResult) = ((dwFirst) - (dwSecond))))

//
// LONG
// PortCmp(WORD wPort1, WORD wPort2, LONG lResult)
//

#define PortCmp(dwPort1, dwPort2,lResult) ((LONG)((lResult) = ((ntohs((WORD)dwPort1)) - (ntohs((WORD)dwPort2)))))

// The addresses are in Network order

//
// LONG
// InetCmp(DWORD IpAddr1, DWORD IpAddr2, LONG lResult)
//

#define InetCmp(dwIpAddr1,dwIpAddr2,res)                                                            \
            ((LONG)(((res) = (((dwIpAddr1) & 0x000000ff) - ((dwIpAddr2) & 0x000000ff))) ? (res)   : \
                    (((res) = (((dwIpAddr1) & 0x0000ff00) - ((dwIpAddr2) & 0x0000ff00))) ? (res)  : \
                     (((res) = (((dwIpAddr1) & 0x00ff0000) - ((dwIpAddr2) & 0x00ff0000))) ? (res) : \
                      ((res) = ((((dwIpAddr1) & 0xff000000)>>8) - (((dwIpAddr2) & 0xff000000)>>8)))))))


#endif // __IPUTILS_H__

