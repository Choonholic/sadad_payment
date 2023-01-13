// GPRSComm.h

#pragma once

#include <winsock2.h>
#include "Definitions.h"
//#include "RilApiList.h"

class CGPRSComm
{
public:
	CGPRSComm();
	virtual	~CGPRSComm();

	BOOL	Initialize();
	BOOL	CleanUp();

	CONNECTION_ERROR	Connect(char* szIPAddress, int nPort);
	CONNECTION_STATUS	CheckNetworkStatus();
	BOOL				Disconnect();

	BOOL	Send(LPBYTE lpData, UINT nLength);
	BOOL	Receive(LPBYTE lpData, UINT* nLength, int nTimeout);

private:
	SOCKET	m_socket;
};
