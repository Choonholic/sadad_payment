// PSTNComm.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "PSTNComm.h"
#include "bbappapi.h"
#include "data_struct.h"
#include "Utilities.h"

CPSTNComm::CPSTNComm(void)
{
	m_bConnected	= FALSE;
}

CPSTNComm::~CPSTNComm(void)
{
}

BOOL CPSTNComm::Open()
{
#if PSTN_AVAILABLE
	return (BOOL)(BBPstnModemSdlcOpen(L"COM2:") == BB_SUCCESS);
#else
	return FALSE;
#endif
}

BOOL CPSTNComm::Close()
{
#if PSTN_AVAILABLE
	return (BOOL)(BBPstnModemSdlcClose() == BB_SUCCESS);
#else
	return FALSE;
#endif
}

CONNECTION_ERROR CPSTNComm::DialUp(char* szNumber)
{
#if PSTN_AVAILABLE
	if (m_bConnected)
	{
		return CONN_NOERROR;
	}

	return (BBPstnModemDial((const char*)szNumber) ? CONN_NOERROR : CONN_DIALUP_ERROR);
#else
	return CONN_NOT_COMPATIBLE;
#endif
}

CONNECTION_STATUS CPSTNComm::CheckCommStatus()
{
#if PSTN_AVAILABLE
	if (m_bConnected)
	{
		return CONN_CONNECTED;
	}

	UCHAR	uStatus	= 0;
	UCHAR	uResult	= BBPstnModemGetLineStatus(&uStatus);

	if (uResult == BB_SUCCESS)
	{
		switch (uStatus)
		{
		case BB_modem_NOCARRIER:
			{
				return CONN_PSTN_NOCARRIER;
			}
			break;
		case BB_modem_NODIALTONE:
			{
				return CONN_PSTN_NODIALTONE;
			}
			break;
		case BB_modem_LINEBUSY:
			{
				return CONN_PSTN_LINEBUSY;
			}
			break;
		case BB_modem_TIMEOUT:
			{
				return CONN_PSTN_TIMEOUT;
			}
			break;
		case BB_modem_INPROCESS:
			{
				return CONN_PSTN_CONNECTING;
			}
			break;
		case BB_modem_CNT:
			{
				m_bConnected	= TRUE;

				return CONN_CONNECTED;
			}
			break;
		}
	}

	return CONN_UNKNOWN_STATUS;
#else
	return CONN_UNKNOWN_STATUS;
#endif
}

BOOL CPSTNComm::HangUp()
{
#if PSTN_AVAILABLE
	if (m_bConnected)
	{
		UCHAR	uPackets[6]	= { 0x19, 0xBA, 0x19, 0xB1, 0x0D, 0x0A };

		m_bConnected	= FALSE;

		BBPstnModemSendRawPacket(uPackets, 6);
		Sleep(10);

		return (BOOL)(BBPstnModemDialOff());
	}

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CPSTNComm::Send(UCHAR* pData, UINT uSize, UINT uTimeout)
{
#if PSTN_AVAILABLE
	if (m_bConnected)
	{
#if LOG_SEND_PACKETS
		LOGSTART;
		LOGWRITE(L"PSTN Send Start ------\r\n");

		for (UINT i = 0; i < uSize; i++)
		{
			LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", pData[i]);
		}

		LOGWRITE(L"\r\n");
		LOGWRITE(L"PSTN Send End --------\r\n");
		LOGEND;
#endif

		if (BBPstnModemSendSdlc((const char*)pData, uSize, uTimeout * 1000) == BB_SUCCESS)
		{
			return TRUE;
		}
	}

	return FALSE;
#else
	return FALSE;
#endif
}

BOOL CPSTNComm::Receive(UCHAR* pData, UINT* pBufferSize, UINT uTimeout)
{
#if PSTN_AVAILABLE
	if (m_bConnected)
	{
		if (BBPstnModemReadSdlc(pData, pBufferSize, uTimeout * 1000) == BB_SUCCESS)
		{
#if LOG_RECV_PACKETS
			LOGSTART;
			LOGWRITE(L"PSTN Recv Start ------\r\n");

			for (UINT i = 0; i < (*pBufferSize); i++)
			{
				LOGWRITE(((i % 10) == 9) ? L"0x%02X, \r\n" : L"0x%02X, ", pData[i]);
			}

			LOGWRITE(L"\r\n");
			LOGWRITE(L"PSTN Recv End --------\r\n");
			LOGWRITE(L"PSTN Received %d Bytes\r\n", (*pBufferSize));
			LOGEND;
#endif
			return TRUE;
		}
	}

	return FALSE;
#else
	return FALSE;
#endif
}
