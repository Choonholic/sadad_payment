// GPRSComm.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "GPRSComm.h"
#include "data_struct.h"
#include "Device.h"
#include "RilApis.h"

CGPRSComm::CGPRSComm()
{
	Initialize();
}

CGPRSComm::~CGPRSComm()
{
	CleanUp();
}

BOOL CGPRSComm::Initialize()
{
#if GPRS_AVAILABLE
	WSADATA	wsaData;
	int		nResult	= WSAStartup(MAKEWORD(2, 2), &wsaData);

	RilApi_Initialize();
	CheckNetworkStatus();

	return (BOOL)(nResult == NO_ERROR);
#else
	return FALSE;
#endif
}

BOOL CGPRSComm::CleanUp()
{
#if GPRS_AVAILABLE
	RilApi_Cleanup();
	WSACleanup();

	return TRUE;
#else
	return FALSE;
#endif
}

#define GPRS_MAX_RETRY	10
#define GPRS_WAIT_MSEC	3000

CONNECTION_ERROR CGPRSComm::Connect(char* szIPAddress, int nPort)
{
#if GPRS_AVAILABLE
	struct	sockaddr_in	clientService;
	int					i;

	for (i = 0; i < GPRS_MAX_RETRY; i++)
	{
		if (CheckNetworkStatus() == CONN_CONNECTED)
		{
			break;
		}

		Sleep(GPRS_WAIT_MSEC);
	}

	if (i >= GPRS_MAX_RETRY)
	{
		return CONN_GPRS_ERROR;
	}

	m_socket	= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		return CONN_SOCKET_ERROR;
	}

	clientService.sin_family		= AF_INET;
	clientService.sin_addr.s_addr	= inet_addr(szIPAddress);
	clientService.sin_port			= htons(nPort);

	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		closesocket(m_socket);

		return CONN_CONNECT_ERROR;
    }

	return CONN_NOERROR;
#else
	return CONN_NOT_COMPATIBLE;
#endif
}

BOOL CGPRSComm::Disconnect()
{
#if GPRS_AVAILABLE
    if (m_socket)
	{
		closesocket(m_socket);
	}

	return TRUE;
#else
	return FALSE;
#endif
}

CONNECTION_STATUS CGPRSComm::CheckNetworkStatus()
{
#if GPRS_AVAILABLE
	DWORD	dwStatus	= DATA_DISCONNECTED;

	if (RilApi_GetDataConnectionStatusStandAlone(dwStatus))
	{
		return ((dwStatus == DATA_CONNECTED) ? CONN_CONNECTED : CONN_GPRS_NOT_CONNECTED);
	}

	return CONN_GPRS_NOT_CONNECTED;
#else
	return CONN_UNKNOWN_STATUS;
#endif
}

BOOL CGPRSComm::Send(LPBYTE lpData, UINT nLength)
{
#if GPRS_AVAILABLE
#if LOG_SEND_PACKETS
	LOGSTART;
	LOGWRITE(L"GPRS Send Start ------\r\n");

	for (UINT i = 0; i < nLength; i++)
	{
		LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", lpData[i]);
	}

	LOGWRITE(L"\r\n");
	LOGWRITE(L"GPRS Send End --------\r\n");
	LOGEND;
#endif

    if (send(m_socket, (PCHAR)lpData, nLength, 0) == SOCKET_ERROR)
	{
		closesocket(m_socket);

		return FALSE;
    }

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CGPRSComm::Receive(LPBYTE lpData, UINT* nLength, int nTimeout)
{
#if GPRS_AVAILABLE
	timeval	timeout;
	fd_set	fsRead;

	memset(lpData, 0, sizeof(BYTE) * (*nLength));

	timeout.tv_sec		= ((nTimeout < 15) ? 15 : nTimeout);
	timeout.tv_usec		= 0;
	fsRead.fd_count		= 1;
	fsRead.fd_array[0]	= m_socket;

	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeval));

	switch (select(0, &fsRead, NULL, NULL, &timeout))
	{
	case SOCKET_ERROR:
	case 0:
		{
			closesocket(m_socket);

			return FALSE;
		}
		break;
	}

	int	iResult	= recv(m_socket, (char*)lpData, *nLength, 0);

	if (iResult > 0)
	{
		(*nLength)	= (UINT)iResult;

#if LOG_RECV_PACKETS
		LOGSTART;
		LOGWRITE(L"GPRS Recv Start ------\r\n");

		for (UINT i = 0; i < *nLength; i++)
		{
			LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", lpData[i]);
		}

		LOGWRITE(L"\r\n");
		LOGWRITE(L"GPRS Recv End --------\r\n");
		LOGWRITE(L"GPRS Received %d Bytes\r\n", iResult);
		LOGEND;
#endif

		return TRUE;
	}
	else if (iResult == 0)
	{
	}
	else
	{
	}

	closesocket(m_socket);

	return FALSE;
#else
	return FALSE;
#endif
}
