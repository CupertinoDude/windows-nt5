/*****************************************************************************
 *
 *  DIDEnum.c
 *
 *  Copyright (c) 1996 Microsoft Corporation.  All Rights Reserved.
 *
 *  Abstract:
 *
 *      The IDirectInput device enumerator.
 *
 *      We don't bother making this an honest OLE enumerator because
 *      there's no point.  There's no way to access it from the outside.
 *
 *  Contents:
 *
 *      CDIDEnum_New
 *      CDIDEnum_Next
 *      CDIDEnum_Release
 *
 *****************************************************************************/

#include "dinputpr.h"

/*****************************************************************************
 *
 *      The sqiffle for this file.
 *
 *****************************************************************************/

#define sqfl sqflDEnum


/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @struct _CDIDEnum |
 *
 *          Records the state of a device enumeration.  Note that this
 *          is not free-threaded.
 *
 *  @field  PDIW | pdiW |
 *
 *          The <i IDirectInputW> object that owns the enumeration.
 *
 *  @field  DWORD | dwDevType |
 *
 *          Device type filter.
 *
 *  @field  DWORD | edfl |
 *
 *          Enumeration flags.
 *
 *  @field  int | idosdStatic |
 *
 *          The next static device to enumerate.  Static devices live
 *          in <c c_rgdosdStatic>.
 *
 *  @field  DWORD | dwVer |
 *
 *          Version of DirectX we are emulating.
 *
 *          If we are emulating DirectX 3.0 or less, then don't
 *          reveal joysticks.
 *
 *  @field  int | idosdDynamic |
 *
 *          The next dynamic device to enumerate.  Dyanmic devices
 *          are kept in the <e CDIDEnum.rgdosdDynamic> array.  They
 *          are snapshotted into the enumeration structure to avoid
 *          race conditions if a device comes or goes while we are
 *          in the middle of an enumeration.
 *
 *  @field  PHIDDEVICELIST | phdl |
 *
 *          List of HID devices to be returned by the enumeration.
 *
 *****************************************************************************/

    typedef struct _CDIDEnum
    {
    
        D(DWORD dwSig;)
        PDIW pdiW;
        DWORD dwDevType;
        DWORD edfl;
        int idosdStatic;
        DWORD dwVer;
        int idosdDynamic;
        PHIDDEVICELIST phdl;
        PDIDW pdidW;
    } DENUM, *PDENUM, **PPDENUM;

    #define CDIDENUM_SIGNATURE  0x4D554E45          /* "ENUM" */

    #define AssertPde(pde)          AssertF((pde)->dwSig == CDIDENUM_SIGNATURE)

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @global DIOBJECTSTATICDATA | c_rgdosdStatic[] |
 *
 *          Right now, the list of device is static and hard-coded.
 *          Eventually, we'll
 *          use plug and play to enumerate devices of class "input" and
 *          get information from their config/software keys.
 *
 *****************************************************************************/

    #pragma BEGIN_CONST_DATA

/*
 *  Our array of static joystick instance guids.
 *
 */
GUID rgGUID_Joystick[cJoyMax] = {
    {   0x6F1D2B70,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B71,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B72,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B73,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B74,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B75,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B76,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B77,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B78,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B79,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7A,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7B,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7C,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7D,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7E,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
    {   0x6F1D2B7F,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00},
};

    #if cJoyMax != 16
        #error rgGUID_Joystick supports only 16 joysticks.
    #endif

/*
 *  Note that we recycle the SysMouse GUID as the instance GUID too,
 *  since there will never be more than one system mouse installed in
 *  the system.  Similarly for SysKeyboard.
 */

DIOBJECTSTATICDATA c_rgdosdStatic[] = {
    {   &GUID_SysMouse,     DI8DEVTYPE_MOUSE,    CMouse_New,},
    {   &GUID_SysMouseEm,   DI8DEVTYPE_MOUSE,    CMouse_New,},
    {   &GUID_SysMouseEm2,  DI8DEVTYPE_MOUSE,    CMouse_New,},
    {   &GUID_SysKeyboard,  DI8DEVTYPE_KEYBOARD, CKbd_New,},
    {   &GUID_SysKeyboardEm,   DI8DEVTYPE_KEYBOARD, CKbd_New,},
    {   &GUID_SysKeyboardEm2,  DI8DEVTYPE_KEYBOARD, CKbd_New,},

    #ifndef WINNT
    /*
     * On WINNT all joysticks are HID devices.
     * So it is pointless to include predefined
     * Joystick GUIDs
     */
        #define MAKEJOY(n)                                                  \
    {   &rgGUID_Joystick[n],DI8DEVCLASS_GAMECTRL, CJoy_New,           }
        MAKEJOY( 0),
    MAKEJOY( 1),
    MAKEJOY( 2),
    MAKEJOY( 3),
    MAKEJOY( 4),
    MAKEJOY( 5),
    MAKEJOY( 6),
    MAKEJOY( 7),
    MAKEJOY( 8),
    MAKEJOY( 9),
    MAKEJOY(10),
    MAKEJOY(11),
    MAKEJOY(12),
    MAKEJOY(13),
    MAKEJOY(14),
    MAKEJOY(15),

        #undef MAKEJOY
    #endif

};

    #pragma END_CONST_DATA

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | hresFindInstanceGUID |
 *
 *          Locates information given an instance GUID.
 *
 *  @parm   IN PCGUID | pguid |
 *
 *          The instance GUID to be located.
 *
 *  @parm   OUT CREATEDCB * | pcdcb |
 *
 *          Receives pointer to the <f CreateDcb> function for the object.
 *
 *****************************************************************************/

HRESULT EXTERNAL
    hresFindInstanceGUID_(PCGUID pguid, CREATEDCB *pcdcb,
                          LPCSTR s_szProc, int iarg)
{
    HRESULT hres;
    EnterProcS(hresFindInstance, (_ "G", pguid));

    if(SUCCEEDED(hres = hresFullValidGuid(pguid, iarg)))
    {
        int idosd;

        /*
         *  First try the list of static devices.  Since this
         *  list never changes, we don't need to protect it
         *  with a critical section.
         */
        for(idosd = 0; idosd < cA(c_rgdosdStatic); idosd++)
        {
            if(IsEqualGUID(pguid, c_rgdosdStatic[idosd].rguidInstance))
            {
                *pcdcb = c_rgdosdStatic[idosd].CreateDcb;
                goto done;
            }
        }

        /*
         *  So it wasn't one of the static devices.  See if it's
         *  one of the dynamic HID devices we've already found.
         */
        hres = hresFindHIDInstanceGUID(pguid, pcdcb);
        if(FAILED(hres))
        {

            /*
             *  Not on our list of dynamic HID devices.
             *  Re-enumerate them and try again.  Maybe it was
             *  for a device that we recently added.
             */
            DIHid_BuildHidList(TRUE);
            hres = hresFindHIDInstanceGUID(pguid, pcdcb);
        }

        if(FAILED(hres))
        {
        #ifdef WINNT
            /*
             *  NT Bug#351951.
             *  If they are directly asking for one of the predefined joystick 
             *  IDs then see if we have a device mapped to that ID.  If so,
             *  pretend they asked for that GUID instead.
             */

            /*
             *  Weakly Assert the range of predefined static joystick instance GUIDs
             */
            AssertF( ( rgGUID_Joystick[0].Data1 & 0x0f ) == 0 );
            AssertF( ( rgGUID_Joystick[0x0f].Data1 & 0x0f ) == 0x0f );

            /*
             *  Check the GUID is the same as the first static one ignoring LS 4 bits
             */
            if( ( (pguid->Data1 & 0xf0) == (rgGUID_Joystick[0].Data1 & 0xf0) )
              && !memcmp( ((PBYTE)&rgGUID_Joystick)+1, ((PBYTE)pguid)+1, sizeof(*pguid) - 1 ) )
            {
                RPF("%s: Using predefined instance GUIDs is bad and should not work!", s_szProc);
                if( phdiFindJoyId( pguid->Data1 & 0x0f ) )
                {
                    *pcdcb = CHid_New;
                    hres = S_OK;
                }
                else
                {
                    *pcdcb = 0;
                    hres = DIERR_DEVICENOTREG;
                }
            }
            else
            {
                RPF("%s: Warning: GUID is not installed in this system", s_szProc);
                *pcdcb = 0;
                hres = DIERR_DEVICENOTREG;
            }
        #else
            RPF("%s: Warning: GUID is not installed in this system", s_szProc);
            *pcdcb = 0;
            hres = DIERR_DEVICENOTREG;
        #endif
        }
    }

    done:;
    ExitOleProcPpv(pcdcb);
    return hres;
}

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   void | CDIDEnum_Release |
 *
 *          Free the enumeration object and its associated resources.
 *
 *  @parm   CDIDEnum * | pde |
 *
 *          The enumeration state to be released.
 *
 *****************************************************************************/

void EXTERNAL
    CDIDEnum_Release(PDENUM pde)
{
    EnterProcI(CDIDEnum_Release, (_ "p", pde));

    AssertPde(pde);

    /*
     *  First, release any last enumerated device
     */
    if( pde->pdidW )
    {
        OLE_Release(pde->pdidW);
    }
    OLE_Release(pde->pdiW);
    FreePpv(&pde->phdl);
    FreePv(pde);

}



    #define S_SKIP      hresUs(2)

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | hresIsDeviceTypeMatch |
 *
 *          Test if a device type matches the filter.
 *
 *  @parm   BYTE | bFilter |
 *
 *          The device type or class to filter by.
 *
 *  @parm   BYTE | bDevType |
 *
 *          The device type to be tested.
 *
 *  @returns
 *
 *          Returns <c S_OK> if the device matches the filter 
 *          or <c S_SKIP> if it does not.
 *
 *****************************************************************************/

HRESULT hresIsDeviceTypeMatch
( 
    BYTE bFilter,
    BYTE bDevType
)
{
    HRESULT hres;

    AssertF( bDevType >= DI8DEVTYPE_MIN );
    AssertF( bDevType < DI8DEVTYPE_MAX );

    if( ( bFilter == 0 )
     || ( bFilter == bDevType ) )
    
    {
        hres = S_OK;
    }
    else 
    {
        hres = S_SKIP;
        switch( bFilter )
        {
        case DI8DEVCLASS_DEVICE:
            if( bDevType == DI8DEVTYPE_DEVICE )
            {
                hres = S_OK;
            }
            break;
        case DI8DEVCLASS_POINTER:
            if( ( bDevType == DI8DEVTYPE_MOUSE )
             || ( bDevType == DI8DEVTYPE_SCREENPOINTER ) )
            {
                hres = S_OK;
            }
            break;
        case DI8DEVCLASS_KEYBOARD:
            if( bDevType == DI8DEVTYPE_KEYBOARD )
            {
                hres = S_OK;
            }
            break;
        case DI8DEVCLASS_GAMECTRL:
            if( (( bDevType >= DI8DEVTYPE_GAMEMIN )
              && ( bDevType < DI8DEVTYPE_GAMEMAX )) ||
                ( bDevType == DI8DEVTYPE_SCREENPOINTER ) ||  /* Windows bug 385284 (jacklin) - DI8DEVCLASS_GAMECTRL should   */
                ( bDevType == DI8DEVTYPE_SUPPLEMENTAL ) )    /* include DI8DEVTYPE_SCREENPOINTER and DI8DEVTYPE_SUPPLEMENTAL */
            {
                hres = S_OK;
            }
            break;
        }
    }

    return hres;
}

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | CDIDEnum_Next |
 *
 *          Return the next device.
 *
 *          Note that this is not the same as the OLE <mf IEnumXxx::Next>
 *          function.  Not that it'd be hard to convert over; it's just
 *          not needed yet.
 *
 *  @parm   CDIDEnum * | pde |
 *
 *          Maintains enumeration state.
 *
 *  @parm   LPGUID | pguid |
 *
 *          Receives the enumerated GUID.
 *
 *  @parm   LPDIDEVICEINSTANCEW | pddiW |
 *
 *          Receives device attributes.
 *
 *  @returns
 *
 *          Returns <c S_OK> if the object was successfully obtained,
 *          or <c S_FALSE> if there aren't any more objects.
 *
 *          Warning! <f CDIObj_EnumDevicesW> assumes that this function
 *          cannot fail.
 *
 *****************************************************************************/

STDMETHODIMP
    CDIDEnum_Next(PDENUM pde, LPDIDEVICEINSTANCEW pddiW)
{
    HRESULT hres;
    EnterProcI(CDIDEnum_Next, (_ "p", pde));

    AssertPde(pde);

    AssertF(pddiW->dwSize == cbX(*pddiW));


    /*
     *  Keep going until something works.
     */
    do
    {
        PDIOBJECTSTATICDATA pdosd;

        /*
         *  Release any previously enumerated or looked at device
         */
        if( pde->pdidW )
        {
            OLE_Release(pde->pdidW);
            pde->pdidW = NULL;
        }

        /*
         *  Pull one from the static list first.
         *  If that is empty, then pull from the dynamic list.
         *  If that is empty, then we're done.
         */
        if(pde->idosdStatic < cA(c_rgdosdStatic))
        {
            pdosd = &c_rgdosdStatic[pde->idosdStatic++];
        } else if(pde->phdl && pde->idosdDynamic < pde->phdl->chdi)
        {
            pdosd = &pde->phdl->rghdi[pde->idosdDynamic].osd;
            pdosd->rguidInstance = &pde->phdl->rghdi[pde->idosdDynamic].guid;
            pde->idosdDynamic++;
        } else
        {
            hres = S_FALSE;
            goto done;
        }

        /*
         *  ISSUE-2001/03/03-MarcAnd Filtered device enumerations are slow
         *  Since DI8DEVTYPEs can be generated on the fly and can be 
         *  overridden we can no longer filter before creating the device.
         *  This is badness.  Ideally, we need to add the WinMM and system 
         *  devices to our dynamic device list, tidy that info up and add 
         *  all the info we need to that list.
         */
        if( 1 ) 
        {

            hres = IDirectInput_CreateDevice(pde->pdiW, pdosd->rguidInstance,
                                             (PV)&pde->pdidW, 0);
            if(SUCCEEDED(hres))
            {
                if(CDIObj_TestDeviceFlags(pde->pdidW, pde->edfl) == S_OK)
                {
                    pddiW->dwSize = cbX(*pddiW);
                    hres = IDirectInputDevice_GetDeviceInfo(pde->pdidW, pddiW);

                    if( SUCCEEDED( hres ) )
                    {
                        AssertF( IsEqualGUID(pdosd->rguidInstance, &pddiW->guidInstance) );

                        hres = hresIsDeviceTypeMatch( GET_DIDEVICE_TYPE(pde->dwDevType), GET_DIDEVICE_TYPE(pddiW->dwDevType) );
                    }

                } else
                {
                    hres = S_SKIP;
                }
            } else
            {
                hres = S_SKIP;
            }
        } else
        {
            hres = S_SKIP;
        }
    } while(hres == S_SKIP);

    done:;

    AssertF(hres == S_OK || hres == S_FALSE);

    ScrambleBit(&pddiW->dwDevType, DIDEVTYPE_RANDOM);

    return hres;
}


STDMETHODIMP
    CDIDEnum_InternalNext(PDENUM pde, LPDIDEVICEINSTANCEW pddiW, LPDIRECTINPUTDEVICE8W *ppdid8W)
{
    HRESULT hres;
    EnterProcI(CDIDEnum_Next, (_ "p", pde));

    AssertPde(pde);

    AssertF(pddiW->dwSize == cbX(*pddiW));


    /*
     *  Keep going until something works.
     */
    do
    {
        PDIOBJECTSTATICDATA pdosd;

        /*
         *  Release any previously enumerated or looked at device
         */
        if( pde->pdidW )
        {
            OLE_Release(pde->pdidW);
            pde->pdidW = NULL;
        }

        /*
         *  Pull one from the static list first.
         *  If that is empty, then pull from the dynamic list.
         *  If that is empty, then we're done.
         */
        if(pde->idosdStatic < cA(c_rgdosdStatic))
        {
            pdosd = &c_rgdosdStatic[pde->idosdStatic++];
        } else if(pde->phdl && pde->idosdDynamic < pde->phdl->chdi)
        {
            pdosd = &pde->phdl->rghdi[pde->idosdDynamic].osd;
            pdosd->rguidInstance = &pde->phdl->rghdi[pde->idosdDynamic].guid;
            pde->idosdDynamic++;
        } else
        {
            hres = S_FALSE;
            goto done;
        }

        hres = IDirectInput_CreateDevice(pde->pdiW, pdosd->rguidInstance,
                                         (PV)&pde->pdidW, 0);
        if(SUCCEEDED(hres))
        {
            if(CDIObj_TestDeviceFlags(pde->pdidW, pde->edfl) == S_OK)
            {

                pddiW->dwSize = cbX(*pddiW);
                hres = IDirectInputDevice_GetDeviceInfo(pde->pdidW, pddiW);
                *ppdid8W = (LPDIRECTINPUTDEVICE8W)pde->pdidW;

                AssertF(fLimpFF(SUCCEEDED(hres),
                                IsEqualGUID(pdosd->rguidInstance,
                                            &pddiW->guidInstance)));

                /*
                 *  Do filtering here (see ISSUE in CDIDEnum_Next for why)
                 */
                hres = hresIsDeviceTypeMatch( GET_DIDEVICE_TYPE(pde->dwDevType), GET_DIDEVICE_TYPE(pddiW->dwDevType) );

            } else
            {
                hres = S_SKIP;
            }
        } else
        {
            hres = S_SKIP;
        }
    } while(hres == S_SKIP);

    done:;

    AssertF(hres == S_OK || hres == S_FALSE);

    ScrambleBit(&pddiW->dwDevType, DIDEVTYPE_RANDOM);

    return hres;
}

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | CDIDEnum_New |
 *
 *          Create an enumeration object.
 *
 *          The enumeration object snapshots the system device state
 *          and farms them out one at a time.
 *
 *  @parm   PDIW | pdiW |
 *
 *          Parent <i IDirectInputW> we piggyback off of for device
 *          creation.
 *
 *  @field  DWORD | dwDevType |
 *
 *          Device type filter.
 *
 *  @field  DWORD | edfl |
 *
 *          Enumeration flags.
 *
 *  @field  DWORD | dwVer |
 *
 *          Version of DirectX we are emulating.
 *
 *          This should always be DirectX 8.0.
 *
 *  @parm   CDIDEnum ** | ppde |
 *
 *          Receives the enumeration object.
 *
 *  @returns
 *
 *          Returns <c S_OK> on success or an error code on failure.
 *
 *****************************************************************************/

STDMETHODIMP
    CDIDEnum_New(PDIW pdiW, DWORD dwDevType, DWORD edfl, DWORD dwVer, PPDENUM ppde)
{
    HRESULT hres;
    EnterProcI(CDIDEnum_New, (_ "pxx", pdiW, dwDevType, edfl));

    /*
     *  Refresh the HID device list so the enumeration is fresh.
     */
    DIHid_BuildHidList(TRUE);

    hres = AllocCbPpv(cbX(CDIDEnum), ppde);
    if(SUCCEEDED(hres))
    {
        PDENUM pde = *ppde;

        D(pde->dwSig = CDIDENUM_SIGNATURE);
        pde->pdiW = pdiW;
        pde->dwDevType = dwDevType;
        pde->edfl = edfl;
        pde->dwVer = dwVer;
        /*
         *  Make sure last enumerated device pointer is cleared
         */
        pde->pdidW = NULL;
        AssertF(pde->idosdStatic == 0);

        /*
         *  If enumerating only HID devices, then skip over all
         *  the static (non-HID) devices.  This is important so
         *  we don't go into infinite recursion death with WINMM.DLL,
         *  which does an enumeration to find HID joysticks
         *  in the first place.
         */
        if(pde->dwDevType & DIDEVTYPE_HID)
        {
            pde->idosdStatic = cA(c_rgdosdStatic);
        }

        AssertF(pde->idosdDynamic == 0);

        /*
         *  Clone the device list.  This must be done under the
         *  critical section to avoid races.
         */
        DllEnterCrit();
        if(g_phdl)
        {
            hres = AllocCbPpv(cbHdlChdi(g_phdl->chdi), &pde->phdl);
            if(SUCCEEDED(hres))
            {
                CopyMemory(pde->phdl, g_phdl, cbHdlChdi(g_phdl->chdi));
                SquirtSqflPtszV(sqfl, TEXT("%S: Have %d HID devices"),
                                s_szProc, pde->phdl->chdi);
                hres = S_OK;
            }
        } else
        {
            hres = S_OK;
        }
        DllLeaveCrit();

        if(SUCCEEDED(hres))
        {
            OLE_AddRef(pde->pdiW);
            hres = S_OK;
        }

    }

    ExitOleProcPpv(ppde);
    return hres;

}

