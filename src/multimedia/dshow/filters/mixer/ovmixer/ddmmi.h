/*==========================================================================
 *
 *  Copyright (c) 1995 - 1999  Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       ddmm.cpp
 *  Content:    Routines for using DirectDraw on a multimonitor system
 *
 ***************************************************************************/

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


HMONITOR DeviceFromWindow(HWND hwnd, LPSTR szDevice, RECT*prc);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
