#include "DeviceIOCTLS.pch"
#pragma hdrstop
/*


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <crtdbg.h>


#include "nt.h"
#include "ntrtl.h"
#include "nturtl.h"
#include <winsock2.h>
#include <windows.h>
*/
#include "ntddtdi.h"
#include "tdi.h"


#include "afd.h"

#include "TCPSRVIOCTL.h"

static bool s_fVerbose = true;

void CIoctlTCPSrv::UseOutBuff(DWORD dwIOCTL, BYTE *abOutBuffer, DWORD dwOutBuff, OVERLAPPED *pOL)
{
    ;
}

HANDLE CIoctlTCPSrv::CreateDevice(CDevice *pDevice)
{
	if (INVALID_SOCKET == m_asAcceptingSocket[0])
	{
		HANDLE h = CIoctlSocketServer::CreateDevice(pDevice);
		if (INVALID_HANDLE_VALUE == h)
		{
			_ASSERT(INVALID_SOCKET == m_asAcceptingSocket[0]);
			return INVALID_HANDLE_VALUE;
		}
		_ASSERT(INVALID_SOCKET != m_asAcceptingSocket[0]);
	}

	AFD_HANDLE_INFO ahi;
    DWORD dwFlags = AFD_QUERY_ADDRESS_HANDLE|AFD_QUERY_CONNECTION_HANDLE;
	DWORD dwBytesReturned;
	OVERLAPPED Overlapped;
	if (!PrepareOverlappedStructure(&Overlapped))
	{
		return INVALID_HANDLE_VALUE;
	}

	if (! ::DeviceIoControl(
		(HANDLE)m_asAcceptingSocket[0],              // handle to a device, file, or directory 
		IOCTL_AFD_QUERY_HANDLES,       // control code of operation to perform
		&dwFlags,           // pointer to buffer to supply input data
		sizeof(dwFlags),         // size, in bytes, of input buffer
		&ahi,          // pointer to buffer to receive output data
		sizeof(ahi),        // size, in bytes, of output buffer
		&dwBytesReturned,     // pointer to variable to receive byte count
		&Overlapped    // pointer to structure for asynchronous operation
		))
	{
		DWORD dwLastError = ::GetLastError();
		if (ERROR_IO_PENDING == dwLastError)
		{
			if (!::GetOverlappedResult((HANDLE)m_asAcceptingSocket[0], &Overlapped, &dwBytesReturned, TRUE))
			{
				DPF((TEXT("CIoctlTCPSrv::CreateDevice() GetOverlappedResult(IOCTL_AFD_QUERY_HANDLES) failed with %d\n"), ::GetLastError()));
				return INVALID_HANDLE_VALUE;
			}
		}
		else
		{
			DPF((TEXT("CIoctlTCPSrv::CreateDevice() DeviceIoControl(IOCTL_AFD_QUERY_HANDLES) failed with %d\n"), ::GetLastError()));
			return INVALID_HANDLE_VALUE;
		}
	}

	//
	// either the call succeeded, or the OL operation succeeded
	//

	return ahi.TdiAddressHandle;
}

BOOL CIoctlTCPSrv::CloseDevice(CDevice *pDevice)
{
    return ::CloseHandle(pDevice->m_hDevice);;
}

BOOL CIoctlTCPSrv::DeviceWriteFile(
		HANDLE hFile,                    // handle to file to write to
		LPCVOID lpBuffer,                // pointer to data to write to file
		DWORD nNumberOfBytesToWrite,     // number of bytes to write
		LPDWORD lpNumberOfBytesWritten,  // pointer to number of bytes written
		LPOVERLAPPED lpOverlapped        // pointer to structure for overlapped I/O
		)
{
	return TRUE;
}

BOOL CIoctlTCPSrv::DeviceReadFile(
		HANDLE hFile,                // handle of file to read
		LPVOID lpBuffer,             // pointer to buffer that receives data
		DWORD nNumberOfBytesToRead,  // number of bytes to read
		LPDWORD lpNumberOfBytesRead, // pointer to number of bytes read
		LPOVERLAPPED lpOverlapped    // pointer to structure for data
		)
{
	return TRUE;
}

void CIoctlTCPSrv::CallRandomWin32API(LPOVERLAPPED pOL)
{
    return;
}

BOOL CIoctlTCPSrv::DeviceInputOutputControl(
		HANDLE hDevice,              // handle to a device, file, or directory 
		DWORD dwIoControlCode,       // control code of operation to perform
		LPVOID lpInBuffer,           // pointer to buffer to supply input data
		DWORD nInBufferSize,         // size, in bytes, of input buffer
		LPVOID lpOutBuffer,          // pointer to buffer to receive output data
		DWORD nOutBufferSize,        // size, in bytes, of output buffer
		LPDWORD lpBytesReturned,     // pointer to variable to receive byte count
		LPOVERLAPPED lpOverlapped    // pointer to structure for asynchronous operation
		)
{
	lpOverlapped->Internal = (DWORD)STATUS_PENDING;
	NTSTATUS Status = NtDeviceIoControlFile (
		hDevice, 
		lpOverlapped->hEvent, 
		NULL, 
		NULL,
		(PIO_STATUS_BLOCK)&lpOverlapped->Internal,
		dwIoControlCode,
		lpInBuffer,
		nInBufferSize,
		lpOutBuffer,
		nOutBufferSize
		);
	if (!NT_SUCCESS(Status))
	{
		::SetLastError(Status);
	}
	if (STATUS_PENDING == Status)
	{
		::SetLastError(ERROR_IO_PENDING);
	}
	_tprintf(TEXT("CIoctlTCPSrv::DeviceInputOutputControl() NtDeviceIoControlFile() return 0x%08X\n"), Status);

	return (STATUS_SUCCESS == Status);
}

void CIoctlTCPSrv::PrepareIOCTLParams(
    DWORD& dwIOCTL,
    BYTE *abInBuffer,
    DWORD &dwInBuff,
    BYTE *abOutBuffer,
    DWORD &dwOutBuff
    )
{
	switch(dwIOCTL)
	{
    case IOCTL_TDI_ACCEPT:
		break;

    case IOCTL_TDI_CONNECT:
		break;

    case IOCTL_TDI_DISCONNECT:
		break;

    case IOCTL_TDI_LISTEN:
		break;

    case IOCTL_TDI_QUERY_INFORMATION:
/*
typedef struct _TDI_REQUEST_QUERY_INFORMATION {
    TDI_REQUEST Request;
    ULONG QueryType;                          // class of information to be queried.
    PTDI_CONNECTION_INFORMATION RequestConnectionInformation;
} TDI_REQUEST_QUERY_INFORMATION, *PTDI_REQUEST_QUERY_INFORMATION;
typedef struct _TDI_REQUEST {
    union {
        HANDLE AddressHandle;
        CONNECTION_CONTEXT ConnectionContext;
        HANDLE ControlChannel;
    } Handle;

    PVOID RequestNotifyObject;
    PVOID RequestContext;
    TDI_STATUS TdiStatus;
} TDI_REQUEST, *PTDI_REQUEST;
*/
        //((PTDI_REQUEST_QUERY_INFORMATION)abInBuffer)->Request = xxx;
        ((PTDI_REQUEST_QUERY_INFORMATION)abInBuffer)->QueryType = GetRandomQueryType();
        //((PTDI_REQUEST_QUERY_INFORMATION)abInBuffer)->RequestConnectionInformation = xxx;

        SetInParam(dwInBuff, sizeof(TDI_REQUEST_QUERY_INFORMATION));

        SetOutParam(abOutBuffer, dwOutBuff, sizeof(TDI_PROVIDER_INFO));

        break;

    case IOCTL_TDI_RECEIVE:
		break;

    case IOCTL_TDI_RECEIVE_DATAGRAM:
		break;

    case IOCTL_TDI_SEND:
		break;

    case IOCTL_TDI_SEND_DATAGRAM:
		break;

    case IOCTL_TDI_SET_EVENT_HANDLER:
		break;

    case IOCTL_TDI_SET_INFORMATION:
		break;

    case IOCTL_TDI_ASSOCIATE_ADDRESS:
		break;

    case IOCTL_TDI_DISASSOCIATE_ADDRESS:
		break;

    case IOCTL_TDI_ACTION:
		break;

	default: 
		_tprintf(TEXT("CIoctlTCPSrv::PrepareIOCTLParams() 0x%08X is an unexpected IOCTL\n"), dwIOCTL);
		_ASSERTE(FALSE);
		CIoctl::PrepareIOCTLParams(dwIOCTL, abInBuffer, dwInBuff, abOutBuffer, dwOutBuff);
	}
}


BOOL CIoctlTCPSrv::FindValidIOCTLs(CDevice *pDevice)
{
	AddIOCTL(pDevice, IOCTL_TDI_ACCEPT          );
	AddIOCTL(pDevice, IOCTL_TDI_CONNECT          );
	AddIOCTL(pDevice, IOCTL_TDI_DISCONNECT          );
	AddIOCTL(pDevice, IOCTL_TDI_LISTEN          );
	AddIOCTL(pDevice, IOCTL_TDI_QUERY_INFORMATION          );
	AddIOCTL(pDevice, IOCTL_TDI_RECEIVE          );
	AddIOCTL(pDevice, IOCTL_TDI_RECEIVE_DATAGRAM          );
	AddIOCTL(pDevice, IOCTL_TDI_SEND          );
	AddIOCTL(pDevice, IOCTL_TDI_SEND_DATAGRAM          );
	AddIOCTL(pDevice, IOCTL_TDI_SET_EVENT_HANDLER          );
	AddIOCTL(pDevice, IOCTL_TDI_SET_INFORMATION          );
	AddIOCTL(pDevice, IOCTL_TDI_ASSOCIATE_ADDRESS          );
	AddIOCTL(pDevice, IOCTL_TDI_DISASSOCIATE_ADDRESS          );
	AddIOCTL(pDevice, IOCTL_TDI_ACTION          );

	return TRUE;
}

ULONG CIoctlTCPSrv::GetRandomQueryType()
{
	switch(rand()%14)
	{
	case 0:
		return TDI_QUERY_BROADCAST_ADDRESS;
	case 1:
		return TDI_QUERY_PROVIDER_INFORMATION;
	case 2:
		return TDI_QUERY_PROVIDER_INFO;
	case 3:
		return TDI_QUERY_ADDRESS_INFO;
	case 4:
		return TDI_QUERY_CONNECTION_INFO;
	case 5:
		return TDI_QUERY_PROVIDER_STATISTICS;
	case 6:
		return TDI_QUERY_DATAGRAM_INFO;
	case 7:
		return TDI_QUERY_DATA_LINK_ADDRESS;
	case 8:
		return TDI_QUERY_NETWORK_ADDRESS;
	case 9:
		return TDI_QUERY_MAX_DATAGRAM_INFO;
	case 10:
		return TDI_QUERY_ADAPTER_STATUS;
	case 11:
		return TDI_QUERY_SESSION_STATUS;
	case 12:
		return TDI_QUERY_FIND_NAME;
	case 13:
		return rand();
	}
	_ASSERTE(FALSE);
	return rand();
}

