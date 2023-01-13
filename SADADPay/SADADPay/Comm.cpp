// Comm.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "Comm.h"
#include "data_struct.h"

CComm::CComm()
{
	m_connectionMode	= MODE_DIALUP;
	m_pPSTN				= new CPSTNComm();
	m_pTCP				= new CTCPComm();
	m_pGPRS				= new CGPRSComm();
}

CComm::CComm(CONNECTION_MODE connectionMode)
{
	m_connectionMode	= connectionMode;
	m_pPSTN				= new CPSTNComm();
	m_pTCP				= new CTCPComm();
	m_pGPRS				= new CGPRSComm();
}

CComm::~CComm()
{
	if (m_pPSTN != NULL)
	{
		delete m_pPSTN;

		m_pPSTN	= NULL;
	}

	if (m_pTCP != NULL)
	{
		delete m_pTCP;

		m_pTCP	= NULL;
	}

	if (m_pGPRS != NULL)
	{
		delete m_pGPRS;

		m_pGPRS	= NULL;
	}
}

void CComm::SetConnectionMode(CONNECTION_MODE connectionMode)
{
	m_connectionMode	= connectionMode;
}

BOOL CComm::Open()
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->Open();
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->Initialize();
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->Initialize();
		}
		break;
	}

	return FALSE;
}

BOOL CComm::Close()
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->Close();
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->CleanUp();
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->CleanUp();
		}
		break;
	}

	return FALSE;
}

CONNECTION_ERROR CComm::Connect(char* pParam, UINT nParam)
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->DialUp(pParam);
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->Connect(pParam, nParam);
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->Connect(pParam, nParam);
		}
		break;
	}

	return CONN_NOERROR;
}

CONNECTION_STATUS CComm::CheckConnection()
{
	CONNECTION_STATUS	status;

	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			status	= m_pPSTN->CheckCommStatus();
		}
		break;
	case MODE_LAN:
		{
			status	= m_pTCP->CheckNetworkStatus();
		}
		break;
	case MODE_GPRS:
		{
			status	= m_pGPRS->CheckNetworkStatus();
		}
		break;
	}

	return status;
}

BOOL CComm::Disconnect()
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->HangUp();
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->Disconnect();
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->Disconnect();
		}
		break;
	}

	return FALSE;
}

BOOL CComm::Send(UCHAR* pData, UINT nSize, UINT uTimeout)
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->Send(pData, nSize, uTimeout);
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->Send(pData, nSize);
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->Send(pData, nSize);
		}
		break;
	}

	return FALSE;
}

BOOL CComm::Receive(UCHAR* pData, UINT* pBufferSize, UINT uTimeout)
{
	switch (m_connectionMode)
	{
	case MODE_DIALUP:
		{
			return m_pPSTN->Receive(pData, pBufferSize, uTimeout);
		}
		break;
	case MODE_LAN:
		{
			return m_pTCP->Receive(pData, pBufferSize, uTimeout);
		}
		break;
	case MODE_GPRS:
		{
			return m_pGPRS->Receive(pData, pBufferSize, uTimeout);
		}
		break;
	}

	return FALSE;
}
