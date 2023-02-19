
/*++

Copyright (c) 2001  Microsoft Corporation 

Module Name:

	 asunld.c

Abstract:

    This application is used with the ACPI BIOS Simulator.

Author(s):

    Vincent Geglia (vincentg) 06-Apr-2001
     
Environment:

	 Console App; User mode

Notes:


Revision History:

    

--*/


//
// General includes
//

#include "windows.h"
#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"
#include "setupapi.h"
#include "devioctl.h"
#include "acpiioct.h"

//
// Specific includes
//

#include "asimictl.h"

//
// Function definitions
//

UCHAR
ConvertCharacterHexToDecimal
    (
        UCHAR   Character
    );

//
// Function code
//

void
__cdecl
main(
	 int    argc,
     char   *argv[]
	 )
{
    HDEVINFO                            deviceinfo = INVALID_HANDLE_VALUE;
    SP_DEVICE_INTERFACE_DATA            deviceinterfacedata;
    PSP_DEVICE_INTERFACE_DETAIL_DATA    deviceinterfacedetaildata = 0;
    BOOL                                success = FALSE;
    CONST GUID                          guid = ACPISIM_GUID;
    DWORD                               size = 0, count = 0;
    HANDLE                              device = INVALID_HANDLE_VALUE;
    PACPI_EVAL_INPUT_BUFFER_COMPLEX     inputbuffer = 0;
    UCHAR                               notifycode = 0;
    PACPI_METHOD_ARGUMENT               argument = 0;
    ULONG_PTR                           ddbhandle = 0;
    ULONG                               tablenumber = 0;
    
    if (argc < 2) {

        printf ("Usage:  %s <Table number (1-20) to unload>\n\n", argv[0]);
        goto Exitmain;
    }
    
    tablenumber = atoi (argv[1]);

    if (!tablenumber) {

        printf("Error parsing table number\n");
        goto Exitmain;
    }
    
    tablenumber --;
    
    deviceinterfacedata.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

    deviceinfo = SetupDiGetClassDevs (&guid,
                                      NULL,
                                      NULL,
                                      DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if (deviceinfo == INVALID_HANDLE_VALUE) {

        printf ("Error enumerating ACPISIM device instances.\n");
        goto Exitmain;
    }

    success = SetupDiEnumDeviceInterfaces (deviceinfo,
                                           NULL,
                                           &guid,
                                           0,
                                           &deviceinterfacedata);

    if (!success) {

        printf ("Error enumerating ACPISIM device interface instances.\n");
        goto Exitmain;
    }

    //
    // Find out how big our buffer needs to be
    //

    success = SetupDiGetDeviceInterfaceDetail (deviceinfo,
                                               &deviceinterfacedata,
                                               NULL,
                                               0,
                                               &size,
                                               NULL);

    if (!size) {

        printf ("Error getting device interface size.\n");
        goto Exitmain;
    }

    deviceinterfacedetaildata = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc (size);

    if (!deviceinterfacedetaildata) {

        printf ("Unable to allocate memory.\n");
        goto Exitmain;
    }

    deviceinterfacedetaildata->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
    
    success = SetupDiGetDeviceInterfaceDetail (deviceinfo,
                                               &deviceinterfacedata,
                                               deviceinterfacedetaildata,
                                               size,
                                               NULL,
                                               NULL);

    if (!success) {

        printf ("Error getting device interface detail.\n");
        goto Exitmain;
    }
    
    device = CreateFile (
                         deviceinterfacedetaildata->DevicePath,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL
                         );

    if (device == INVALID_HANDLE_VALUE) {

        printf ("Error opening %s.\n", deviceinterfacedetaildata->DevicePath);
        goto Exitmain;
    }

    //
    // Setup input structure
    //

    size = sizeof (ACPI_EVAL_INPUT_BUFFER_COMPLEX) + sizeof (ACPI_METHOD_ARGUMENT);

    inputbuffer = (PACPI_EVAL_INPUT_BUFFER_COMPLEX) malloc (size);

    if (!inputbuffer) {

        printf ("Error allocating memory.\n");
        goto Exitmain;
    }

    inputbuffer->Signature = ACPI_EVAL_INPUT_BUFFER_COMPLEX_SIGNATURE;
    CopyMemory (inputbuffer->MethodName, "ULTB", 4);
    inputbuffer->Size = size;
    inputbuffer->ArgumentCount = 1;
    
    //
    // Set up notify device (1st argument)
    //
    
    inputbuffer->Argument[0].Type = ACPI_METHOD_ARGUMENT_INTEGER;
    inputbuffer->Argument[0].DataLength = sizeof (ULONG);
    inputbuffer->Argument[0].Argument = tablenumber;
    
    success = DeviceIoControl (
                               device,
                               IOCTL_ACPISIM_METHOD_COMPLEX,
                               inputbuffer,
                               size,
                               NULL,
                               0,
                               &size,
                               NULL
                               );

    if (!success) {

        printf ("Error issuing IOCTL (%x).\n", GetLastError ());
        goto Exitmain;
    }

Exitmain:

    if (deviceinterfacedetaildata) {

        free (deviceinterfacedetaildata);
    }

    if (inputbuffer) {

        free (inputbuffer);
    }

    if (device != INVALID_HANDLE_VALUE) {

        CloseHandle (device);
    }

    return;
}

UCHAR
ConvertCharacterHexToDecimal
    (
        UCHAR   Character
    )
{
    CharUpperBuff (&Character, 1);
    
    if (Character >= '0' && Character <= '9') {

        return Character - '0';

    }

    if (Character >= 'A' && Character <= 'F') {
    
        return (Character - 'A') + 10;
    }

    return 0;
}
