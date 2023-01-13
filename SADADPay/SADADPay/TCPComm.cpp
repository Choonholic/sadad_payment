// TCPComm.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "TCPComm.h"
#include "data_struct.h"

CTCPComm::CTCPComm()
{
	Initialize();
}

CTCPComm::~CTCPComm()
{
	CleanUp();
}

BOOL CTCPComm::Initialize()
{
#if LAN_AVAILABLE
	WSADATA	wsaData;
	int		nResult	= WSAStartup(MAKEWORD(2, 2), &wsaData);

	return (BOOL)(nResult == NO_ERROR);
#else
	return FALSE;
#endif
}

BOOL CTCPComm::CleanUp()
{
#if LAN_AVAILABLE
	WSACleanup();

	return TRUE;
#else
	return FALSE;
#endif
}

CONNECTION_ERROR CTCPComm::Connect(char* szIPAddress, int nPort)
{
#if LAN_AVAILABLE
	struct	sockaddr_in	clientService;

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

BOOL CTCPComm::Disconnect()
{
#if LAN_AVAILABLE
	if (m_socket)
	{
		closesocket(m_socket);
	}

	return TRUE;
#else
	return FALSE;
#endif
}

#define HOSTNAME	"google.com"

CONNECTION_STATUS CTCPComm::CheckNetworkStatus()
{
#if LAN_AVAILABLE
	return (gethostbyname(HOSTNAME) ? CONN_CONNECTED : CONN_HOST_NOT_RESOLVED);
#else
	return CONN_UNKNOWN_ERROR;
#endif
}

BOOL CTCPComm::Send(LPBYTE lpData, UINT nLength)
{
#if LAN_AVAILABLE
#if LOG_SEND_PACKETS
	LOGSTART;
	LOGWRITE(L"TCP Send Start ------\r\n");

	for (UINT i = 0; i < nLength; i++)
	{
		LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", lpData[i]);
	}

	LOGWRITE(L"\r\n");
	LOGWRITE(L"TCP Send End --------\r\n");
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

BOOL CTCPComm::Receive(LPBYTE lpData, UINT* nLength, int nTimeout)
{
#if LAN_AVAILABLE
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
		LOGWRITE(L"TCP Recv Start ------\r\n");

		for (UINT i = 0; i < *nLength; i++)
		{
			LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", lpData[i]);
		}

		LOGWRITE(L"\r\n");
		LOGWRITE(L"TCP Recv End --------\r\n");
		LOGWRITE(L"TCP Received %d Bytes\r\n", iResult);
		LOGEND;
#endif

		return TRUE;
	}
	else if (iResult == 0)
	{
		// TCP Connection Closed
	}
	else
	{
		// TCP Receive Error = WSAGetLastError()
	}

	closesocket(m_socket);

	return FALSE;
#else
	return FALSE;
#endif
}
