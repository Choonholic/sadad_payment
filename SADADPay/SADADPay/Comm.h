// Comm.h

#pragma once

#include "Definitions.h"
#include "PSTNComm.h"
#include "TCPComm.h"
#include "GPRSComm.h"

class CComm
{
public:
	CComm();
	CComm(CONNECTION_MODE connectionMode);
	~CComm();

	void	SetConnectionMode(CONNECTION_MODE connectionMode);

	BOOL	Open();
	BOOL	Close();

	CONNECTION_ERROR	Connect(char* pParam, UINT nParam);
	CONNECTION_STATUS	CheckConnection();
	BOOL				Disconnect();

	BOOL	Send(UCHAR* pData, UINT nSize, UINT uTimeout);
	BOOL	Receive(UCHAR* pData, UINT* pBufferSize, UINT uTimeout);

protected:
	CONNECTION_MODE	m_connectionMode;

	CPSTNComm*		m_pPSTN;
	CTCPComm*		m_pTCP;
	CGPRSComm*		m_pGPRS;
};
