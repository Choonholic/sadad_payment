// Transaction.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "Transaction.h"
#include "ISO8583_Legacy.h"
#include "Device.h"
#include "Utilities.h"
#include "PciPad_t.h"
#include "data_struct.h"
#include "TransactionDatabase.h"
#include "TMS.h"
#include "bbappapi.h"
#include "Notify.h"
#include "HexPair.h"

BYTE	cHDLC_TPDU[5]	= { 0x60, 0x00, 0x01, 0x80, 0xB5 };	// HDLC Transport Protocol Data Unit

DWORD DoTransaction(LPVOID lp)
{
	CChildView*			pView							= (CChildView*)lp;
	UCHAR				cRecvBuffer[PSTN_BUFFER_SIZE]	= { 0, };
	UINT				uRecvSize						= PSTN_BUFFER_SIZE;
	DWORD				dwLastConnectError				= 0;
	TRANSACTION_INFO	info;

	pView->SetThreadStatus(TSTATUS_READY);
	pView->SetThreadMode(TMODE_READY);

	while (!pView->m_bThreadForceClose)
	{
		while (pView->m_tThreadFirstRun == TS_INITIATED)
		{
			Sleep(2000);
		}

		if (pView->m_tThreadFirstRun == TS_SET)
		{
			pView->CleanUpGarbage();
		}

		if (pView->GetThreadMode() == TMODE_TMS)
		{
			Sleep(1000);
			continue;
		}

		if (pView->BatteryIsLow())
		{
			if (pView->GetThreadMode() == TMODE_READY)
			{
				pView->SetThreadMode(TMODE_LOWBATTERY);
				pView->ForcePaintFlag();
				Sleep(1);
				continue;
			}
		}
		else
		{
			if (pView->GetThreadMode() == TMODE_LOWBATTERY)
			{
				pView->SetThreadMode(TMODE_READY);
				pView->ForcePaintFlag();
				Sleep(1);
				continue;
			}
		}

		if (pView->TMSReady())
		{
			pView->m_pComm->Close();
			pView->SetThreadMode(TMODE_TMS);
			pView->ForcePaintFlag();

			g_bAppActivated	= FALSE;

			LOGFILE(L"%S[%d] - MSG_RES_RESOURCE\r\n", __FUNCTION__, __LINE__);
			::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_RES_RESOURCE, pView->m_Parameters.nCurrentConnectionType);
			continue;
		}

#if !IGNORE_LOGON
		if (pView->LogonReady())
		{
			pView->Logon();
		}
#endif

		if (pView->ConnectRequested())
		{
			char	cParam[20];
			int		nParam;

			switch (pView->m_Parameters.nCurrentConnectionType)
			{
			case MODE_DIALUP:
				{
					WCHAR wServer[20];

					pView->SetThreadStatus(TSTATUS_DIALING);
					pView->SetThreadCount(pView->GetNextServerPhone(pView->GetThreadCount()));

					if (pView->GetThreadCount() == THREAD_RETRY_INITIAL)
					{
						pView->RequestTransaction(FALSE);
						pView->SetThreadError(dwLastConnectError);
						pView->ForcePaintFlag();
						continue;
					}

					swprintf(wServer, L"%s%s", pView->m_Settings.szCityCode, pView->m_Parameters.szPhonebook[pView->GetThreadCount()]);
					WA2BA((LPBYTE)cParam, 20, wServer, wcslen(wServer));

					nParam	= 0;
				}
				break;
			case MODE_LAN:
				{
					pView->SetThreadStatus(TSTATUS_CONNECTING);

					if (pView->GetThreadCount() >= THREAD_RETRY_EXCEEDED)
					{
						pView->SetThreadCount(THREAD_RETRY_INITIAL);
						pView->RequestTransaction(FALSE);
						pView->SetThreadError(ERR_CONNECT_FAILED);
						pView->ForcePaintFlag();
						continue;
					}
					else
					{
						pView->SetThreadCount(pView->GetThreadCount() + 1);
					}

					WA2BA((LPBYTE)cParam, 20, pView->m_Parameters.szServerIP, wcslen(pView->m_Parameters.szServerIP));

					nParam	= _wtoi(pView->m_Parameters.szServerPort);
				}
				break;
			case MODE_GPRS:
				{
					pView->SetThreadStatus(TSTATUS_CONNECTING);

					if (pView->GetThreadCount() >= THREAD_RETRY_EXCEEDED)
					{
						pView->SetThreadCount(THREAD_RETRY_INITIAL);
						pView->RequestTransaction(FALSE);
						pView->SetThreadError(ERR_CONNECT_FAILED);
						pView->ForcePaintFlag();
						continue;
					}
					else
					{
						pView->SetThreadCount(pView->GetThreadCount() + 1);
					}

					WA2BA((LPBYTE)cParam, 20, pView->m_Parameters.szServerIP, wcslen(pView->m_Parameters.szServerIP));

					nParam	= _wtoi(pView->m_Parameters.szServerPort);
				}
				break;
			}

			CONNECTION_ERROR	connError = pView->m_pComm->Connect(cParam, nParam);

			LOGFILE(L"%S[%d] - Connect [%S, %d] = %d\r\n", __FUNCTION__, __LINE__, cParam, nParam, connError);

			switch (connError)
			{
			case CONN_NOERROR:
				{
					pView->SetThreadStatus(TSTATUS_WAITING);
				}
				break;
			case CONN_DIALUP_ERROR:
				{
					pView->SetThreadError(ERR_DIALUP_FAILED);
					Sleep(300);
					continue;
				}
				break;
			case CONN_SOCKET_ERROR:
				{
					pView->SetThreadError(ERR_CONNECT_FAILED);
					Sleep(300);
					continue;
				}
				break;
			case CONN_CONNECT_ERROR:
				{
					pView->SetThreadError(ERR_CONNECT_FAILED);
					Sleep(300);
					continue;
				}
				break;
			case CONN_PHONE_ERROR:
				{
					pView->SetThreadError(ERR_GPRS_NOT_READY);
					Sleep(300);
					continue;
				}
				break;
			case CONN_SIM_ERROR:
				{
					pView->SetThreadError(ERR_GPRS_NOT_READY);
					Sleep(300);
					continue;
				}
				break;
			case CONN_REG_ERROR:
				{
					pView->SetThreadError(ERR_GPRS_NOT_READY);
					Sleep(300);
					continue;
				}
				break;
			case CONN_GPRS_ERROR:
				{
					pView->SetThreadError(ERR_GPRS_NOT_READY);
					Sleep(300);
					continue;
				}
				break;
			}
		}

		if (pView->DisconnectRequested())
		{
			pView->RequestConnect(REQ_BEFORE, FALSE);
			pView->SetThreadCount(THREAD_RETRY_INITIAL);
			pView->SetThreadMode(TMODE_READY);
			pView->ClearThreadStatus();
			Sleep(1);
			continue;
		}

		if (pView->GetThreadStatus() == TSTATUS_WAITING)
		{
			CONNECTION_STATUS	connStatus	= pView->m_pComm->CheckConnection();

			switch (connStatus)
			{
			case CONN_CONNECTED:
				{
					pView->SetThreadStatus(TSTATUS_CONNECTED);
					pView->SetThreadCount(THREAD_RETRY_INITIAL);
				}
				break;
			case CONN_PSTN_CONNECTING:
				{
					Sleep(50);
				}
				continue;
			case CONN_PSTN_LINEBUSY:
				{
					dwLastConnectError	= ERR_CONN_LINE_BUSY;

					pView->SetThreadStatus(TSTATUS_RETRY);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			case CONN_PSTN_NOCARRIER:
				{
					dwLastConnectError	= ERR_CONN_NO_CARRIER;

					pView->SetThreadStatus(TSTATUS_RETRY);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			case CONN_PSTN_NODIALTONE:
				{
					dwLastConnectError	= ERR_CONN_NO_DIALTONE;

					pView->SetThreadStatus(TSTATUS_RETRY);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			case CONN_PSTN_TIMEOUT:
				{
					dwLastConnectError	= ERR_CONN_TIMEOUT;

					pView->SetThreadStatus(TSTATUS_RETRY);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			case CONN_GPRS_NOT_CONNECTED:
				{
					pView->SetThreadError(ERR_CONN_GPRS_NOT_CONNECTED);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			case CONN_HOST_NOT_RESOLVED:
				{
					pView->SetThreadError(ERR_CONN_HOST_NOT_RESOLVED);
					pView->RequestConnect(REQ_BEFORE);
					pView->ForcePaintFlag();
					Sleep(200);
				}
				continue;
			}
		}

		if (pView->DisconnectRequested())
		{
			pView->SetThreadMode(TMODE_READY);
			pView->ClearThreadStatus();
			Sleep(1);
			continue;
		}

		while (pView->ReversalReady())
		{
			pView->SetThreadMode(TMODE_REVERSAL);
			pView->ForcePaintFlag();

			if (pView->m_dwReversalTick > GetTickCount())
			{
				break;
			}

			memcpy(&info, &pView->m_database.m_arrReversal.GetAt(0), sizeof(TRANSACTION_INFO));

			info.transactionStatus	= STATUS_REVERSAL;

			pView->m_database.UpdateTransactionStatus(&info);
			pView->SetReversalStatus();

			pView->SetReversalData(&info);
			pView->SetTransportData(info.transactionMode, (pView->m_dwReceivedTime - pView->m_dwSentTime), info.nAttempts);
			pView->SetThreadStatus(TSTATUS_SENDING);
			pView->BuildTransactionData();

#if !IGNORE_FINAL_MAC
			pView->SetFinalMAC();
#endif

			if (pView->m_pComm->Send(pView->m_cTransactionBuffer, pView->m_nTransactionSize, pView->m_Parameters.nTimeoutConnected))
			{
				pView->m_dwSentTime	= GetTickCount() / 1000;

				if (pView->GetThreadStatus() == TSTATUS_SENDING)
				{
					pView->SetThreadStatus(TSTATUS_RECEIVING);
					memset(cRecvBuffer, 0, sizeof(UCHAR) * PSTN_BUFFER_SIZE);

					uRecvSize	= PSTN_BUFFER_SIZE;

					if (pView->m_pComm->Receive(cRecvBuffer, &uRecvSize, pView->m_Parameters.nTimeoutConnected))
					{
						pView->m_dwReceivedTime	= GetTickCount() / 1000;

						pView->SetThreadStatus(TSTATUS_RECEIVED);

						switch (pView->ParseTransactionResult(&info, cRecvBuffer, uRecvSize))
						{
						case RESULT_SUCCESS:
							{
								pView->TransactionProcess(&info);
							}
							break;
						case RESULT_FAIL:
							{
								pView->TransactionFailure(&info);
							}
							break;
						case RESULT_INVALID:
							{
								pView->TransactionTimeout(&info);
							}
							break;
						}
					}
					else
					{
						pView->TransactionTimeout(&info);
					}
				}
			}
			else
			{
				switch (pView->m_pComm->CheckConnection())
				{
				case CONN_CONNECTED:
					{
						pView->SetThreadError(ERR_SEND_FAILED);
						pView->ForcePaintFlag();
					}
					break;
				case CONN_PSTN_NOCARRIER:
					{
						pView->SetThreadError(ERR_CONN_NO_CARRIER);
						pView->ForcePaintFlag();
					}
					break;
				case CONN_PSTN_NODIALTONE:
					{
						pView->SetThreadError(ERR_CONN_NO_DIALTONE);
						pView->ForcePaintFlag();
					}
					break;
				case CONN_PSTN_TIMEOUT:
					{
						pView->SetThreadError(ERR_CONN_TIMEOUT);
						pView->ForcePaintFlag();
					}
					break;
				case CONN_GPRS_NOT_CONNECTED:
					{
						pView->SetThreadError(ERR_CONN_GPRS_NOT_CONNECTED);
						pView->ForcePaintFlag();
					}
					break;
				case CONN_HOST_NOT_RESOLVED:
					{
						pView->SetThreadError(ERR_CONN_HOST_NOT_RESOLVED);
						pView->ForcePaintFlag();
					}
					break;
				}
			}

			if (pView->GetThreadStatus() != TSTATUS_ERROR)
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
			}
		}

		if (pView->GetThreadMode() == TMODE_REVERSAL)
		{
			if ((pView->GetThreadStatus() == TSTATUS_ERROR) && (pView->GetViewMode() != VIEW_MESSAGE))
			{
				pView->ShowMessage();
			}
			else if (!pView->ReversalExist())
			{
				pView->RequestDisconnect();
				pView->Restart();
			}

			Sleep(1);
			continue;
		}
 
		if (pView->TransactionReady())
		{
			pView->RequestCancelAdvice(FALSE);

			if (!pView->SetTransactionData(&pView->m_TransInfo, pView->GetSystemTraceNo()))
			{
				pView->SetThreadError(ERR_INVALID_PIN_BLOCK);
				pView->ForcePaintFlag();
			}
			else
			{
				pView->SetThreadMode(TMODE_TRANSACTION);
				memcpy(&info, &pView->m_TransInfo, sizeof(TRANSACTION_INFO));
				pView->SetThreadStatus(TSTATUS_SENDING);
				pView->BuildTransactionData();

#if !IGNORE_FINAL_MAC
				pView->SetFinalMAC();
#endif

				if (pView->m_pComm->Send(pView->m_cTransactionBuffer, pView->m_nTransactionSize, pView->m_Parameters.nTimeoutConnected))
				{
					if (info.transactionStatus == STATUS_TRANSACTION)
					{
						GetLocalTime(&info.stTimestamp);
						pView->m_database.InsertTransactionRecord(&info);
					}

					pView->m_dwSentTime	= GetTickCount() / 1000;

					if (pView->GetThreadStatus() == TSTATUS_SENDING)
					{
						pView->SetThreadStatus(TSTATUS_RECEIVING);
						memset(cRecvBuffer, 0, sizeof(UCHAR) * PSTN_BUFFER_SIZE);

						uRecvSize	= PSTN_BUFFER_SIZE;

						if (pView->m_pComm->Receive(cRecvBuffer, &uRecvSize, pView->m_Parameters.nTimeoutConnected))
						{
							pView->m_dwReceivedTime	= GetTickCount() / 1000;

							pView->SetThreadStatus(TSTATUS_RECEIVED);

							switch (pView->ParseTransactionResult(&info, cRecvBuffer, uRecvSize))
							{
							case RESULT_SUCCESS:
								{
									pView->TransactionProcess(&info);
								}
								break;
							case RESULT_FAIL:
								{
									pView->TransactionFailure(&info);
								}
								break;
							case RESULT_INVALID:
								{
									pView->TransactionTimeout(&info);
								}
								break;
							}
						}
						else
						{
							pView->TransactionTimeout(&info);
						}
					}
				}
				else
				{
					switch (pView->m_pComm->CheckConnection())
					{
					case CONN_CONNECTED:
						{
							pView->SetThreadError(ERR_SEND_FAILED);
							pView->ForcePaintFlag();
						}
						break;
					case CONN_PSTN_NOCARRIER:
						{
							pView->SetThreadError(ERR_CONN_NO_CARRIER);
							pView->ForcePaintFlag();
						}
						break;
					case CONN_PSTN_NODIALTONE:
						{
							pView->SetThreadError(ERR_CONN_NO_DIALTONE);
							pView->ForcePaintFlag();
						}
						break;
					case CONN_PSTN_TIMEOUT:
						{
							pView->SetThreadError(ERR_CONN_TIMEOUT);
							pView->ForcePaintFlag();
						}
						break;
					case CONN_GPRS_NOT_CONNECTED:
						{
							pView->SetThreadError(ERR_CONN_GPRS_NOT_CONNECTED);
							pView->ForcePaintFlag();
						}
						break;
					case CONN_HOST_NOT_RESOLVED:
						{
							pView->SetThreadError(ERR_CONN_HOST_NOT_RESOLVED);
							pView->ForcePaintFlag();
						}
						break;
					}
				}
			}

			if (pView->GetThreadStatus() != TSTATUS_ERROR)
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
			}
		}

		if ((pView->GetThreadMode() == TMODE_TRANSACTION) && (pView->GetThreadStatus() == TSTATUS_ERROR) && (pView->GetViewMode() != VIEW_MESSAGE))
		{
			pView->ShowMessage();
			Sleep(1);
			continue;
		}

		if (pView->AdviceReady())
		{
			pView->SetThreadMode(TMODE_ADVICE);

			if (pView->m_dwAdviceTick > GetTickCount())
			{
				continue;
			}

			if (pView->CancelAdviceRequested())
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
				pView->SetNextAdviceTime();
				continue;
			}

			memcpy(&info, &pView->m_database.m_arrAdvice.GetAt(0), sizeof(TRANSACTION_INFO));
			pView->SetAdviceData(&info);
			pView->SetTransportData(info.transactionMode, (pView->m_dwReceivedTime - pView->m_dwSentTime), info.nAttempts);
			pView->SetThreadStatus(TSTATUS_SENDING);
			pView->BuildTransactionData();

#if !IGNORE_FINAL_MAC
			pView->SetFinalMAC();
#endif

			if (pView->CancelAdviceRequested())
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
				pView->SetNextAdviceTime();
				continue;
			}

			if (pView->m_pComm->Send(pView->m_cTransactionBuffer, pView->m_nTransactionSize, pView->m_Parameters.nTimeoutConnected))
			{
				if (pView->CancelAdviceRequested())
				{
					pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
					pView->SetNextAdviceTime();
					continue;
				}

				pView->m_dwSentTime	= GetTickCount() / 1000;

				pView->SetThreadStatus(TSTATUS_RECEIVING);
				memset(cRecvBuffer, 0, sizeof(UCHAR) * PSTN_BUFFER_SIZE);

				uRecvSize	= PSTN_BUFFER_SIZE;

				if (pView->m_pComm->Receive(cRecvBuffer, &uRecvSize, pView->m_Parameters.nTimeoutConnected))
				{
					if (pView->CancelAdviceRequested())
					{
						pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
						pView->SetNextAdviceTime();
						continue;
					}

					pView->m_dwReceivedTime	= GetTickCount() / 1000;

					pView->SetThreadStatus(TSTATUS_RECEIVED);

					switch (pView->ParseTransactionResult(&info, cRecvBuffer, uRecvSize))
					{
					case RESULT_SUCCESS:
						{
							pView->TransactionProcess(&info);
						}
						break;
					case RESULT_FAIL:
						{
							pView->TransactionFailure(&info);
						}
						break;
					case RESULT_INVALID:
						{
							pView->TransactionTimeout(&info);
						}
						break;
					}
				}
				else
				{
					pView->TransactionTimeout(&info);
				}
			}
			else
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
				pView->SetNextAdviceTime();
			}

			if (pView->GetThreadStatus() != TSTATUS_ERROR)
			{
				pView->SetThreadStatus(((pView->m_pComm->CheckConnection() == CONN_CONNECTED) ? TSTATUS_CONNECTED : TSTATUS_READY));
			}
		}

		if (pView->AllTransactionsFinished(&info))
		{
			pView->SetThreadMode(TMODE_READY);
			pView->ClearThreadStatus();
			Sleep(1);
			continue;
		}

		if ((pView->GetThreadMode() == TMODE_ADVICE) && (pView->GetThreadStatus() == TSTATUS_ERROR))
		{
			pView->SetThreadMode(TMODE_READY);
			pView->ClearThreadStatus();
			Sleep(1);
			continue;
		}

		Sleep(1);
	}

	return 0;
}

THREAD_MODE CChildView::GetThreadMode()
{
	return m_tThreadMode;
}

void CChildView::SetThreadMode(THREAD_MODE mode)
{
	if (mode != m_tThreadMode)
	{
		m_tPrevThreadMode	= m_tThreadMode;
		m_tThreadMode		= mode;
	}
}

void CChildView::CleanUpGarbage()
{
	if (ReversalExist())
	{
		for (int i = 0; i < m_database.m_arrReversal.GetCount(); i++)
		{
			if (m_database.m_arrReversal.GetAt(i).transactionStatus == STATUS_TRANSACTION)
			{
				m_database.m_arrReversal.GetAt(i).transactionStatus	= STATUS_REVERSAL;

				memcpy(&m_TransInfo, &m_database.m_arrReversal.GetAt(i), sizeof(TRANSACTION_INFO));
				m_database.UpdateTransactionStatus(&m_TransInfo);
				SetPrintInfo(&m_PrintInfo, RECEIPT_TIMEOUT, &m_TransInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
				SetViewMode(VIEW_PRINT);
				ForcePaintFlag();
				return;
			}
		}
	}

	m_tThreadFirstRun	= TS_CLEARED;
}

void CChildView::ClearThreadStatus(BOOL bDisconnect)
{
//	RequestTransaction(FALSE);
	SetThreadStatus(TSTATUS_READY);
	SetThreadCount(THREAD_RETRY_INITIAL);

	if (bDisconnect)
	{
		m_pComm->Disconnect();
	}

	m_bThreadDisconnect	= FALSE;
	m_bThreadDialup		= FALSE;
}

THREAD_STATUS CChildView::GetThreadStatus()
{
	return m_tThreadStatus;
}

void CChildView::SetThreadStatus(THREAD_STATUS status)
{
	if (status != m_tThreadStatus)
	{
		m_tPrevThreadStatus	= m_tThreadStatus;
		m_tThreadStatus		= status;
	}
}

BOOL CChildView::GetThreadError(UINT& nID, MESSAGE_TYPE& messageType, BOOL& bPrint)
{
	if (GetThreadStatus() == TSTATUS_ERROR)
	{
		nID			= m_nInfo[INFO_MESSAGE_ID];
		messageType	= (MESSAGE_TYPE)m_nInfo[INFO_MESSAGE_TYPE];
		bPrint		= m_nInfo[INFO_MESSAGE_PRINT];

		return TRUE;
	}

	return FALSE;
}

void CChildView::SetThreadError(UINT nID, MESSAGE_TYPE messageType, BOOL bPrint)
{
	if (nID)
	{
		m_nInfo[INFO_MESSAGE_ID]	= nID;
		m_nInfo[INFO_MESSAGE_TYPE]	= messageType;
		m_nInfo[INFO_MESSAGE_PRINT]	= bPrint;

		RequestTransaction(FALSE);
		SetThreadStatus(TSTATUS_ERROR);
		m_pComm->Disconnect();

		m_bThreadDisconnect	= FALSE;
		m_bThreadDialup		= FALSE;
	}
}

void CChildView::ClearThreadError()
{
	if (GetThreadStatus() == TSTATUS_ERROR)
	{
		RequestTransaction(FALSE);
		SetThreadStatus(TSTATUS_READY);
		SetThreadCount(THREAD_RETRY_INITIAL);
		m_pComm->Disconnect();

		m_bThreadDisconnect	= FALSE;
		m_bThreadDialup		= FALSE;
	}
}

int CChildView::GetThreadCount()
{
	return m_nThreadCount;
}

void CChildView::SetThreadCount(int nCount)
{
	m_nThreadCount	= nCount;
}

ACTION_REQUESTER CChildView::GetActionRequester()
{
	return m_ActionRequester;
}

void CChildView::SetActionRequester(ACTION_REQUESTER ActionRequester)
{
	m_ActionRequester	= ActionRequester;
}

void CChildView::RequestConnect(ACTION_REQUESTER ActionRequester, BOOL bRequest)
{
	if (GetActionRequester() != REQ_BEFORE)
	{
		SetActionRequester(ActionRequester);
	}

	if (bRequest)
	{
		m_bThreadDialup		= TRUE;
		m_bThreadConnect	= TRUE;
	}
	else
	{
		m_bThreadConnect	= FALSE;
	}
}

void CChildView::RequestTransaction(BOOL bRequest)
{
	m_bThreadTransaction	= bRequest;
}

void CChildView::RequestDisconnect(BOOL bRequest)
{
	if (bRequest)
	{
		if (GetThreadStatus() == TSTATUS_READY)
		{
			m_bThreadDisconnect	= FALSE;
			return;
		}

		m_bThreadDialup		= FALSE;
		m_bThreadDisconnect	= TRUE;
	}
	else
	{
		m_bThreadDisconnect	= FALSE;
	}
}

void CChildView::RequestCancelAdvice(BOOL bRequest)
{
	m_bThreadCancelAdvice	= bRequest;
}

BOOL CChildView::ConnectRequested()
{
	if ((GetThreadStatus() == TSTATUS_READY) || (GetThreadStatus() == TSTATUS_RETRY))
	{
		if (m_bThreadConnect)
		{
			RequestConnect(REQ_BEFORE, FALSE);
			return TRUE;
		}
		
		if (ReversalExist())
		{
			SetThreadMode(TMODE_REVERSAL);

			if (m_dwReversalTick <= GetTickCount())
			{
				RequestConnect(REQ_REVERSAL, FALSE);
				return TRUE;
			}
		}

		if (TransactionRequested())
		{
			RequestConnect(REQ_TRANSACTION, FALSE);
			return TRUE;
		}

		if (AdviceExist())
		{
			if (m_dwAdviceTick <= GetTickCount())
			{
				RequestConnect(REQ_ADVICE, FALSE);
				return TRUE;
			}
		}
	}

	RequestConnect(REQ_BEFORE, FALSE);
	return FALSE;
}

BOOL CChildView::TransactionRequested()
{
	return m_bThreadTransaction;
}

BOOL CChildView::DisconnectRequested()
{
	if (GetThreadMode() == TMODE_REVERSAL)
	{
		RequestDisconnect(FALSE);

		return FALSE;
	}

	switch (GetThreadStatus())
	{
	case TSTATUS_READY:
	case TSTATUS_SENDING:
	case TSTATUS_RECEIVING:
	case TSTATUS_RECEIVED:
		{
			RequestDisconnect(FALSE);

			return FALSE;
		}
		break;
	}

	if (ReversalExist())
	{
		if (m_dwReversalTick <= GetTickCount())
		{
			RequestDisconnect(FALSE);

			return FALSE;
		}
	}

	if (AdviceExist())
	{
		if (m_dwAdviceTick <= GetTickCount())
		{
			RequestDisconnect(FALSE);

			return FALSE;
		}
	}

	if (TransactionRequested())
	{
		BOOL bRequest	= m_bThreadDisconnect;

		RequestTransaction(!bRequest);
		RequestDisconnect(FALSE);
		return bRequest;
	}

	if (m_bThreadDisconnect == TRUE)
	{
		RequestDisconnect(FALSE);

		return TRUE;
	}

	RequestDisconnect(FALSE);

	return FALSE;
}

BOOL CChildView::CancelAdviceRequested()
{
	return m_bThreadCancelAdvice;
}

BOOL CChildView::ReversalExist()
{
	BOOL	bExist	= FALSE;

	switch (m_tReversalStatus)
	{
	case TS_INITIATED:
		{
			bExist = (BOOL)(m_database.SelectReversalRecords());
		}
		break;
	case TS_CLEARED:
		{
			bExist	= FALSE;
		}
		break;
	case TS_SET:
		{
			bExist	= TRUE;
		}
		break;
	}

	m_tReversalStatus	= (bExist ? TS_SET : TS_CLEARED);

	return bExist;
}

void CChildView::SetReversalStatus()
{
	m_tReversalStatus	= (m_database.SelectReversalRecords() ? TS_SET : TS_CLEARED);

	if (m_tReversalStatus == TS_CLEARED)
	{
		RequestDisconnect();
	}
}

BOOL CChildView::AdviceExist()
{
	BOOL	bExist	= FALSE;

	switch (m_tAdviceStatus)
	{
	case TS_INITIATED:
		{
			bExist = (BOOL)(m_database.SelectAdviceRecords());
		}
		break;
	case TS_CLEARED:
		{
			bExist	= FALSE;
		}
		break;
	case TS_SET:
		{
			bExist	= TRUE;
		}
		break;
	}

	m_tAdviceStatus	= (bExist ? TS_SET : TS_CLEARED);

	return bExist;
}

void CChildView::SetAdviceStatus()
{
	m_tAdviceStatus	= (m_database.SelectAdviceRecords() ? TS_SET : TS_CLEARED);

	if (m_tAdviceStatus == TS_CLEARED)
	{
		RequestDisconnect();
	}
}

BOOL CChildView::TransactionReady()
{
	if (!TransactionRequested())
	{
		return FALSE;
	}

	if ((GetThreadStatus() == TSTATUS_ERROR) && (GetActionRequester() != REQ_TRANSACTION))
	{
		ClearThreadStatus();
		return FALSE;
	}

	if (GetThreadStatus() != TSTATUS_CONNECTED)
	{
		return FALSE;
	}

	RequestTransaction(FALSE);
	return TRUE;
}

BOOL CChildView::LogonReady()
{
	if (m_Settings.logonStatus != LOGON_FAILED)
	{
		return FALSE;
	}

	if (GetTickCount() < m_dwLogonTick)
	{
		return FALSE;
	}
	
	if (GetViewMode() != VIEW_INTRO)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CChildView::ReversalReady()
{
	if (m_Settings.logonStatus != LOGON_SUCCEEDED)
	{
		return FALSE;
	}

	if (!ReversalExist())
	{
		return FALSE;
	}

	if ((GetThreadStatus() == TSTATUS_ERROR) && (GetActionRequester() != REQ_REVERSAL))
	{
		ClearThreadStatus();
		return FALSE;
	}

	if (GetThreadStatus() != TSTATUS_CONNECTED)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CChildView::AdviceReady()
{
	if (m_Settings.logonStatus != LOGON_SUCCEEDED)
	{
		return FALSE;
	}

	if (!AdviceExist())
	{
		return FALSE;
	}

	if ((GetThreadStatus() == TSTATUS_ERROR) && (GetActionRequester() != REQ_ADVICE))
	{
		ClearThreadStatus();
		return FALSE;
	}

	if (GetThreadStatus() != TSTATUS_CONNECTED)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CChildView::TMSReady()
{
	if (m_bTMSRequested == FALSE)
	{
		return FALSE;
	}

	if (GetThreadStatus() != TSTATUS_READY)
	{
		return FALSE;
	}

	if (GetThreadMode() != TMODE_READY)
	{
		return FALSE;
	}

	m_bTMSRequested	= FALSE;

	return TRUE;
}

BOOL CChildView::BatteryIsLow()
{
	return (BOOL)(m_BatteryStatus == BATTERY_CRITICAL);
}

void CChildView::SetNextLogonTime()
{
	SYSTEMTIME	st;

	GetLocalTime(&st);
	AddSystemTime(&st, ((INTERVAL_LOGON / 1000) * OS_100NS));
	CeRunAppAtTime(PATH_WAKEUP, &st);

	m_dwLogonTick	= GetTickCount() + INTERVAL_LOGON;
}

void CChildView::ResetLogonTime()
{
	m_dwLogonTick	= 0;
}

void CChildView::SetNextReversalTime()
{
	SYSTEMTIME	st;
	int			nBefore	= ((m_Parameters.nTimeoutReversal > 10) ? 10 : 0);

	GetLocalTime(&st);
	AddSystemTime(&st, ((m_Parameters.nTimeoutReversal - nBefore) * OS_100NS));
	CeRunAppAtTime(PATH_WAKEUP, &st);

	m_dwReversalTick	= GetTickCount() + (m_Parameters.nTimeoutReversal * 1000);
}

void CChildView::ResetReversalTime()
{
	m_dwReversalTick	= 0;
}

void CChildView::SetNextAdviceTime()
{
	SYSTEMTIME	st;
	int			nBefore	= ((m_Parameters.nTimeoutAdvice > 10) ? 10 : 0);

	GetLocalTime(&st);
	AddSystemTime(&st, ((m_Parameters.nTimeoutAdvice - nBefore) * OS_100NS));
	CeRunAppAtTime(PATH_WAKEUP, &st);

	m_dwAdviceTick	= GetTickCount() + (m_Parameters.nTimeoutAdvice * 1000);
}

void CChildView::ResetAdviceTime()
{
	m_dwAdviceTick	= 0;
}

TRANSACTION_STATUS CChildView::GetTransactionStatus(TRANSACTION_INFO* pInfo)
{
	return pInfo->transactionStatus;
}

void CChildView::SetTransactionStatus(TRANSACTION_INFO* pInfo, TRANSACTION_STATUS transactionStatus)
{
	pInfo->transactionStatus	= transactionStatus;
}

BOOL CChildView::AllTransactionsFinished(TRANSACTION_INFO* pInfo)
{
	if (ReversalExist())
	{
		return FALSE;
	}

	if (AdviceExist())
	{
		return FALSE;
	}

	switch (GetTransactionStatus(pInfo))
	{
	case STATUS_TRANSACTION:
	case STATUS_REVERSAL:
		{
			return FALSE;
		}
		break;
	}

	switch (GetThreadStatus())
	{
	case TSTATUS_READY:
	case TSTATUS_SENDING:
	case TSTATUS_RECEIVING:
	case TSTATUS_RECEIVED:
		{
			return FALSE;
		}
		break;
	}
	
	switch (GetViewMode())
	{
	case VIEW_MENU_MAIN:
	case VIEW_PIN:
	case VIEW_DIALUP:
	case VIEW_SALE_AMOUNT:
	case VIEW_SALE_CONFIRM:
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
	case VIEW_BILL_CONFIRM:
	case VIEW_CHARGE_COUNT:
	case VIEW_MENU_PROVIDER:
	case VIEW_MENU_CATEGORY:
	case VIEW_CHARGE_CONFIRM:
		{
			return FALSE;
		}
		break;
	}
	
	if (m_bThreadDialup)
	{
		return FALSE;
	}

	return TRUE;
}

int CChildView::GetNextServerPhone(int nServer)
{
	do
	{
		if (nServer < (MAX_PHONEBOOK - 1))
		{
			nServer++;
		}
		else
		{
			nServer	= THREAD_RETRY_INITIAL;

			break;
		}
	} while (!wcslen(m_Parameters.szPhonebook[nServer]));

	return nServer;
}

void CChildView::ClearTransactionData()
{
	ISO8583_Reset();
}

void CChildView::SetMandatoryData(TRANSACTION_INFO* pInfo)
{
	BYTE	cMsgType[5];
	BYTE	cProcessingCode[7];
	BYTE	cPOSEntryMode[4];
	BYTE	cNII[4];
	BYTE	cPOSConditionCode[3];
	BYTE	cCardAcceptorTerminalId[9];
	BYTE	cCardAcceptorId[16];
	BYTE	cMAC[9]					= { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x30, 0x31, 0x00 };

	switch (GetTransactionMode())
	{
	case TRANSACTION_SALE:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_SALE);
			strcpy((char*)cProcessingCode, PROCODE_SALE);
		}
		break;
	case TRANSACTION_BALANCE:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_BALANCE);
			strcpy((char*)cProcessingCode, PROCODE_BALANCE);
		}
		break;
	case TRANSACTION_BILL_PAYMENT:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_BILL_PAYMENT);
			strcpy((char*)cProcessingCode, PROCODE_BILL_PAYMENT);
		}
		break;
	case TRANSACTION_CHARGE:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_CHARGE);
			strcpy((char*)cProcessingCode, PROCODE_CHARGE);
		}
		break;
	case TRANSACTION_LOGON:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_LOGON);
			strcpy((char*)cProcessingCode, PROCODE_LOGON);
		}
		break;
	case TRANSACTION_SERVICE_REQUEST:
		{
			strcpy((char*)cMsgType, MSGTYPE_REQ_SERVICE_REQUEST);
			strcpy((char*)cProcessingCode, PROCODE_SERVICE_REQUEST);
		}
		break;
	}

	WA2BA(cPOSEntryMode, 4, m_Parameters.szPOSEntryMode, wcslen(m_Parameters.szPOSEntryMode));
	WA2BA(cNII, 4, m_Parameters.szNII, wcslen(m_Parameters.szNII));
	WA2BA(cPOSConditionCode, 3, m_Parameters.szPOSConditonCode, wcslen(m_Parameters.szPOSConditonCode));

#if TEST_MODE
	sprintf((char*)cCardAcceptorTerminalId, "%S", TEST_TERMINAL_ID);
	sprintf((char*)cCardAcceptorId, "%S", TEST_MERCHANT_ID);
#else
	WA2BA(cCardAcceptorTerminalId, 9, m_Parameters.szTerminalID, wcslen(m_Parameters.szTerminalID));
	WA2BA(cCardAcceptorId, 16, m_Parameters.szMerchantID, wcslen(m_Parameters.szMerchantID));
#endif

	ISO8583_SetField(F00_MSG_TYPE_ID, cMsgType, 4);
	ISO8583_SetField(F03_PROCESSING_CODE, cProcessingCode, 6);
	ISO8583_SetField(F22_POS_ENTRY_MODE, cPOSEntryMode, 3);
	ISO8583_SetField(F24_NII, cNII, 3);
	ISO8583_SetField(F25_POS_CONDITION_CODE, cPOSConditionCode, 2);
	ISO8583_SetField(F41_CARD_ACCEPTOR_TERMINAL_ID, cCardAcceptorTerminalId, 8);
	ISO8583_SetField(F42_CARD_ACCEPTOR_ID, cCardAcceptorId, 15);
	ISO8583_SetField(F64_MESSAGE_AUTH_CODE, cMAC, 8);

	pInfo->transactionMode	= GetTransactionMode();

	BA2WA(pInfo->szProcessingCode, 7, cProcessingCode, 6);
}

BOOL CChildView::SetTransactionData(TRANSACTION_INFO* pInfo, int nTraceNo)
{
	BOOL	bResult;
	BYTE	cBuffer[512];
	BYTE	cPIN[32];
	DWORD	dwPINSize	= 32;

#if LOG_TRANSACTION_DATA
	LOGSTART;
	LOGWRITE(L"------------------------\r\n");
	LOGWRITE(L"%S[%d] -\r\n", __FUNCTION__, __LINE__);
	LOGWRITE(L"Information Block:\r\n");

	for (int i = 0; i < INFO_MAX; i++)
	{
		LOGWRITE(L"m_sInfo[%2d] = '%s' (%d)\r\n", i, m_sInfo[i], m_sInfo[i].GetLength());
	}

	for (int i = 0; i < INFO_MAX; i++)
	{
		LOGWRITE(L"m_nInfo[%2d] = '%d'\r\n", i, m_nInfo[i]);
	}

	LOGWRITE(L"------------------------\r\n");
	LOGEND;
#endif

	GetDatabaseID(pInfo->szID, 17);

	switch (pInfo->transactionMode)
	{
	case TRANSACTION_SALE:
		{
			// Amount
			WA2BA(cBuffer, 512, pInfo->szAmount, wcslen(pInfo->szAmount));
			ISO8583_SetField(F04_TRXN_AMOUNT, cBuffer, strlen((const char*)cBuffer));

			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			// Track 2 Data
			wcstombs((char*)cBuffer, g_strMSRData[1], 512);
			wcscpy(pInfo->szTrack2Data, g_strMSRData[1]);
			ISO8583_SetField(F35_TRACK_2_DATA, cBuffer, strlen((const char*)cBuffer));

			// PIN Block
			bResult	= MakePINBlock(cPIN, &dwPINSize, m_sInfo[INFO_PIN], g_strMSRData[1]);

			ISO8583_SetField(F52_PIN_DATA, cPIN, LENGTH_PIN_BLOCK);

			pInfo->transactionStatus	= STATUS_TRANSACTION;
		}
		break;
	case TRANSACTION_BALANCE:
		{
			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			// Track 2 Data
			wcstombs((char*)cBuffer, g_strMSRData[1], 512);
			wcscpy(pInfo->szTrack2Data, g_strMSRData[1]);
			ISO8583_SetField(F35_TRACK_2_DATA, cBuffer, strlen((const char*)cBuffer));

			// PIN Block
			bResult	= MakePINBlock(cPIN, &dwPINSize, m_sInfo[INFO_PIN], g_strMSRData[1]);

			ISO8583_SetField(F52_PIN_DATA, cPIN, LENGTH_PIN_BLOCK);

			pInfo->transactionStatus	= STATUS_VOLATILE;
		}
		break;
	case TRANSACTION_BILL_PAYMENT:
		{
			// Amount
			WA2BA(cBuffer, 512, pInfo->szAmount, wcslen(pInfo->szAmount));
			ISO8583_SetField(F04_TRXN_AMOUNT, cBuffer, strlen((const char*)cBuffer));

			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			// Track 2 Data
			wcstombs((char*)cBuffer, g_strMSRData[1], 512);
			wcscpy(pInfo->szTrack2Data, g_strMSRData[1]);
			ISO8583_SetField(F35_TRACK_2_DATA, cBuffer, strlen((const char*)cBuffer));

			// Additional Data
			sprintf((char*)cBuffer, "%013d%013d", _wtoi(pInfo->szBillID), _wtoi(pInfo->szPaymentID));
			ISO8583_SetField(F48_ADD_DATA_PRIVATE, cBuffer, strlen((const char*)cBuffer));

			// PIN Block
			bResult	= MakePINBlock(cPIN, &dwPINSize, m_sInfo[INFO_PIN], g_strMSRData[1]);

			ISO8583_SetField(F52_PIN_DATA, cPIN, LENGTH_PIN_BLOCK);

			pInfo->transactionStatus	= STATUS_TRANSACTION;
		}
		break;
	case TRANSACTION_CHARGE:
		{
			// Amount
			WA2BA(cBuffer, 512, pInfo->szAmount, wcslen(pInfo->szAmount));
			ISO8583_SetField(F04_TRXN_AMOUNT, cBuffer, strlen((const char*)cBuffer));

			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			// Track 2 Data
			wcstombs((char*)cBuffer, g_strMSRData[1], 512);
			wcscpy(pInfo->szTrack2Data, g_strMSRData[1]);
			ISO8583_SetField(F35_TRACK_2_DATA, cBuffer, strlen((const char*)cBuffer));

			// Add Data - Private
			sprintf((char*)cBuffer,"%04d%02d %02d", _wtoi(pInfo->szProviderID), _wtoi(pInfo->szCategoryID), pInfo->nChargeCount);
			ISO8583_SetField(F48_ADD_DATA_PRIVATE, cBuffer, strlen((const char*)cBuffer));

			// PIN Block
			bResult	= MakePINBlock(cPIN, &dwPINSize, m_sInfo[INFO_PIN], g_strMSRData[1]);

			ISO8583_SetField(F52_PIN_DATA, cPIN, LENGTH_PIN_BLOCK);

			pInfo->transactionStatus	= STATUS_TRANSACTION;
		}
		break;
	case TRANSACTION_LOGON:
		{
			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			pInfo->transactionStatus	= STATUS_LOGON;
		}
		break;
	case TRANSACTION_SERVICE_REQUEST:
		{
			// Trace Number
			sprintf((char*)cBuffer, "%06d", nTraceNo);
			BA2WA(pInfo->szTraceNo, 7, cBuffer, strlen((const char*)cBuffer));
			ISO8583_SetField(F11_STAN, cBuffer, strlen((const char*)cBuffer));

			// Service Type
			WA2BA(cBuffer, 7, pInfo->szServiceType, wcslen(pInfo->szServiceType));
			ISO8583_SetField(F48_ADD_DATA_PRIVATE, cBuffer, strlen((const char*)cBuffer));

			pInfo->transactionStatus	= STATUS_VOLATILE;
		}
		break;
	}

	wcscpy(pInfo->szResponseCode, L"FF");

#if LOG_TRANSACTION_DATA
	LOGSTART;
	LOGWRITE(L"------------------------\r\n");
	LOGWRITE(L"%S[%d] -\r\n", __FUNCTION__, __LINE__);
	LOGWRITE(L"TRANSACTION_INFO:\r\n");
	LOGWRITE(L" szID              = '%s'\r\n", pInfo->szID);
	LOGWRITE(L" nAttempts         = '%d'\r\n", pInfo->nAttempts);
	LOGWRITE(L" transactionStatus = '%d'\r\n", pInfo->transactionStatus);
	LOGWRITE(L" transactionMode   = '%d'\r\n", pInfo->transactionMode);
	LOGWRITE(L" szProcessingCode  = '%s'\r\n", pInfo->szProcessingCode);
	LOGWRITE(L" szAmount          = '%s'\r\n", pInfo->szAmount);
	LOGWRITE(L" szTraceNo         = '%s'\r\n", pInfo->szTraceNo);
	LOGWRITE(L" stTimestamp       = '%04d/%02d/%02d %02d:%02d:%02d'\r\n", pInfo->stTimestamp.wYear, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
	LOGWRITE(L" szTrack2Data      = '%s'\r\n", pInfo->szTrack2Data);
	LOGWRITE(L" szReferenceNo     = '%s'\r\n", pInfo->szReferenceNo);
	LOGWRITE(L" szAuthID          = '%s'\r\n", pInfo->szAuthID);
	LOGWRITE(L" szResponseCode    = '%s'\r\n", pInfo->szResponseCode);
	LOGWRITE(L" szProviderID      = '%s'\r\n", pInfo->szProviderID);
	LOGWRITE(L" szCategoryID      = '%s'\r\n", pInfo->szCategoryID);
	LOGWRITE(L" szAdditionalData  = '%s'\r\n", pInfo->szAdditionalData);
	LOGWRITE(L" nChargeCount      = '%d'\r\n", pInfo->nChargeCount);
	LOGWRITE(L" szSerial[0]       = '%s'\r\n", pInfo->szSerial[0]);
	LOGWRITE(L" szSerial[1]       = '%s'\r\n", pInfo->szSerial[1]);
	LOGWRITE(L" szSerial[2]       = '%s'\r\n", pInfo->szSerial[2]);
	LOGWRITE(L" szSerial[3]       = '%s'\r\n", pInfo->szSerial[3]);
	LOGWRITE(L" szSerial[4]       = '%s'\r\n", pInfo->szSerial[4]);
	LOGWRITE(L" szChargePIN[0]    = '%s'\r\n", pInfo->szChargePIN[0]);
	LOGWRITE(L" szChargePIN[1]    = '%s'\r\n", pInfo->szChargePIN[1]);
	LOGWRITE(L" szChargePIN[2]    = '%s'\r\n", pInfo->szChargePIN[2]);
	LOGWRITE(L" szChargePIN[3]    = '%s'\r\n", pInfo->szChargePIN[3]);
	LOGWRITE(L" szChargePIN[4]    = '%s'\r\n", pInfo->szChargePIN[4]);
	LOGWRITE(L" szBillID          = '%s'\r\n", pInfo->szBillID);
	LOGWRITE(L" szPaymentID       = '%s'\r\n", pInfo->szPaymentID);
	LOGWRITE(L" nServiceCode      = '%d'\r\n", pInfo->nServiceCode);
	LOGWRITE(L" szServiceType     = '%s'\r\n", pInfo->szServiceType);
	LOGWRITE(L"------------------------\r\n");
	LOGEND;
#endif

	return bResult;
}

void CChildView::SetReversalData(TRANSACTION_INFO* pInfo)
{
	BYTE	cMsgType[5];
	BYTE	cProcessingCode[7];
	BYTE	cAmount[13];
	BYTE	cTraceNo[7];
	BYTE	cPOSEntryMode[4];
	BYTE	cNII[4];
	BYTE	cPOSConditionCode[3];
	BYTE	cTrack2Data[41];
	BYTE	cCardAcceptorTerminalId[9];
	BYTE	cCardAcceptorId[16];
	BYTE	cMAC[9]					= { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x30, 0x31, 0x00 };

	strcpy((char*)cMsgType, MSGTYPE_REQ_REVERSAL);
	WA2BA(cProcessingCode, 7, pInfo->szProcessingCode, wcslen(pInfo->szProcessingCode));
	WA2BA(cAmount, 13, pInfo->szAmount, wcslen(pInfo->szAmount));
	WA2BA(cTraceNo, 7, pInfo->szTraceNo, wcslen(pInfo->szTraceNo));
	WA2BA(cPOSEntryMode, 4, m_Parameters.szPOSEntryMode, wcslen(m_Parameters.szPOSEntryMode));
	WA2BA(cNII, 4, m_Parameters.szNII, wcslen(m_Parameters.szNII));
	WA2BA(cPOSConditionCode, 3, m_Parameters.szPOSConditonCode, wcslen(m_Parameters.szPOSConditonCode));
	WA2BA(cTrack2Data, 41, pInfo->szTrack2Data, wcslen(pInfo->szTrack2Data));

#if TEST_MODE
	sprintf((char*)cCardAcceptorTerminalId, "%S", TEST_TERMINAL_ID);
	sprintf((char*)cCardAcceptorId, "%S", TEST_MERCHANT_ID);
#else
	WA2BA(cCardAcceptorTerminalId, 9, m_Parameters.szTerminalID, wcslen(m_Parameters.szTerminalID));
	WA2BA(cCardAcceptorId, 16, m_Parameters.szMerchantID, wcslen(m_Parameters.szMerchantID));
#endif

	ISO8583_Reset();
	ISO8583_SetField(F00_MSG_TYPE_ID, cMsgType, 4);
	ISO8583_SetField(F03_PROCESSING_CODE, cProcessingCode, 6);
	ISO8583_SetField(F04_TRXN_AMOUNT, cAmount, strlen((const char*)cAmount));
	ISO8583_SetField(F11_STAN, cTraceNo, strlen((const char*)cTraceNo));
	ISO8583_SetField(F22_POS_ENTRY_MODE, cPOSEntryMode, 3);
	ISO8583_SetField(F24_NII, cNII, 3);
	ISO8583_SetField(F25_POS_CONDITION_CODE, cPOSConditionCode, 2);
	ISO8583_SetField(F35_TRACK_2_DATA, cTrack2Data, strlen((const char*)cTrack2Data));
	ISO8583_SetField(F41_CARD_ACCEPTOR_TERMINAL_ID, cCardAcceptorTerminalId, 8);
	ISO8583_SetField(F42_CARD_ACCEPTOR_ID, cCardAcceptorId, 15);
	ISO8583_SetField(F64_MESSAGE_AUTH_CODE, cMAC, 8);
}

void CChildView::SetAdviceData(TRANSACTION_INFO* pInfo)
{
	BYTE	cMsgType[5];
	BYTE	cProcessingCode[7];
	BYTE	cOriginalTraceNo[7];
	BYTE	cPOSEntryMode[4];
	BYTE	cNII[4];
	BYTE	cPOSConditionCode[3];
	BYTE	cRRN[13];
	BYTE	cCardAcceptorTerminalId[9];
	BYTE	cCardAcceptorId[16];
	BYTE	cMAC[9]					= { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x30, 0x31, 0x00 };

	strcpy((char*)cMsgType, MSGTYPE_REQ_ADVICE);
	WA2BA(cProcessingCode, 7, pInfo->szProcessingCode, wcslen(pInfo->szProcessingCode));
	WA2BA(cOriginalTraceNo, 7, pInfo->szTraceNo, wcslen(pInfo->szTraceNo));
	WA2BA(cPOSEntryMode, 4, m_Parameters.szPOSEntryMode, wcslen(m_Parameters.szPOSEntryMode));
	WA2BA(cNII, 4, m_Parameters.szNII, wcslen(m_Parameters.szNII));
	WA2BA(cPOSConditionCode, 3, m_Parameters.szPOSConditonCode, wcslen(m_Parameters.szPOSConditonCode));
	WA2BA(cRRN, 13, pInfo->szReferenceNo, wcslen(pInfo->szReferenceNo));

#if TEST_MODE
	sprintf((char*)cCardAcceptorTerminalId, "%S", TEST_TERMINAL_ID);
	sprintf((char*)cCardAcceptorId, "%S", TEST_MERCHANT_ID);
#else
	WA2BA(cCardAcceptorTerminalId, 9, m_Parameters.szTerminalID, wcslen(m_Parameters.szTerminalID));
	WA2BA(cCardAcceptorId, 16, m_Parameters.szMerchantID, wcslen(m_Parameters.szMerchantID));
#endif

	ISO8583_Reset();
	ISO8583_SetField(F00_MSG_TYPE_ID, cMsgType, 4);
	ISO8583_SetField(F03_PROCESSING_CODE, cProcessingCode, 6);
	ISO8583_SetField(F11_STAN, cOriginalTraceNo, 7);
	ISO8583_SetField(F22_POS_ENTRY_MODE, cPOSEntryMode, 3);
	ISO8583_SetField(F24_NII, cNII, 3);
	ISO8583_SetField(F25_POS_CONDITION_CODE, cPOSConditionCode, 2);
	ISO8583_SetField(F37_RRN, cRRN, strlen((const char*)cRRN));
	ISO8583_SetField(F41_CARD_ACCEPTOR_TERMINAL_ID, cCardAcceptorTerminalId, 8);
	ISO8583_SetField(F42_CARD_ACCEPTOR_ID, cCardAcceptorId, 15);
	ISO8583_SetField(F62_PRIVATE_USE, cOriginalTraceNo, 7);
	ISO8583_SetField(F64_MESSAGE_AUTH_CODE, cMAC, 8);
}

void CChildView::SetTransportData(TRANSACTION_MODE transactionMode, DWORD dwWaitTime, UINT nAttempts)
{
	BYTE	cBuffer[512];
	BYTE	cText[64];
	UINT	nPos		= 0;

	switch (transactionMode)
	{
	case TRANSACTION_SERVICE_REQUEST:
		{
		}
		break;
	default:
		{
			memset(cBuffer, 0, 512);

			// Structure Version
			sprintf((char*)cText, "%1d", TD_STRUCTURE_VERSION);
			memcpy(&cBuffer[nPos], cText, strlen((const char*)cText));

			nPos	+= strlen((const char*)cText);

			// Connection Attempts
			sprintf((char*)cText, "%02d", nAttempts);
			memcpy(&cBuffer[nPos], cText, strlen((const char*)cText));

			nPos	+= strlen((const char*)cText);

			// Transaction Wait Time

			if ((dwWaitTime < 0) || (dwWaitTime > 99))
			{
				dwWaitTime = 0;
			}

 			sprintf((char*)cText, "%02d", dwWaitTime);
			memcpy(&cBuffer[nPos], cText, strlen((const char*)cText));

			nPos	+= strlen((const char*)cText);

			// POS Information
			memcpy(&cBuffer[nPos], m_Settings.cPOSInformation, LENGTH_POS_INFORMATION);

			nPos	+= LENGTH_POS_INFORMATION;

			// Encrypt Method
			sprintf((char*)cText, "%1c", TD_ENCRYPT_METHOD);
			memcpy(&cBuffer[nPos], cText, strlen((const char*)cText));

			nPos	+= strlen((const char*)cText);

			ISO8583_SetField(F59_TRANSPORT_DATA, cBuffer, nPos);
		}
		break;
	}
}

void CChildView::BuildTransactionData()
{
	BYTE	cISO8583[ISO8583_MSGLEN_MAX];
	BYTE	cNII[3];
	WCHAR	szNII[5];
	int		nLen	= 0;
	UINT	uSize	= 3;

	memset(m_cTransactionBuffer, 0, sizeof(BYTE) * 512);
	memset(cISO8583, 0, sizeof(BYTE) * ISO8583_MSGLEN_MAX);

	// Set TPDU
	memcpy(&m_cTransactionBuffer[0], cHDLC_TPDU, sizeof(BYTE) * 5);

	// Set NII
	swprintf_s(szNII, 5, L"0%3s", m_Parameters.szNII);
	WideStringToHexPair(szNII, 4, cNII, &uSize);

	m_cTransactionBuffer[1] = cNII[0];
	m_cTransactionBuffer[2] = cNII[1];

	m_nTransactionSize	= 5;

	// Set ISO8583 Message
	nLen	= ISO8583_Build(cISO8583);

	memcpy(&m_cTransactionBuffer[m_nTransactionSize], cISO8583, nLen);

	m_nTransactionSize	+= nLen;
}

#if !IGNORE_FINAL_MAC
void CChildView::SetFinalMAC()
{
	BYTE	cMAC[32] = {
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x30, 0x31,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	BYTE	cKSN[10];
	DWORD	dwResult;
	DWORD	dwLength	= 32;
	WCHAR	szBankID[9]	= L"11111111";

	memset(cMAC, 0, 32 * sizeof(BYTE));
	memset(cKSN, 0, 10 * sizeof(BYTE));

	dwResult	= BBSecure_GetMac(m_cTransactionBuffer, m_nTransactionSize - 8, 0, 0, 0, szBankID, wcslen(szBankID), cMAC, &dwLength, cKSN);

	LOGFILE(L"%S[%d] - BBSecure_GetMac() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

	memcpy(&m_cTransactionBuffer[m_nTransactionSize - 8], cMAC, 8);
}
#endif

TRANSACTION_RESULT CChildView::ParseTransactionResult(TRANSACTION_INFO* pInfo, UCHAR* pRecvData, UINT uSize)
{
	int		nLength			= 0;
	BYTE	cBuffer[9];
	WCHAR	szBuffer[9];

	nLength	+= 5;	// 0x60 0x80 0xB5 NII[0] NII[1]

	ISO8583_Reset();

	if (ISO8583_Parse(pRecvData + nLength, uSize - nLength) == ISO8583_SUCCESS)
	{
		nLength	= 9;

		if (ISO8583_GetField(F41_CARD_ACCEPTOR_TERMINAL_ID, cBuffer, &nLength))
		{
			BA2WA(szBuffer, 9, cBuffer, strlen((const char*)cBuffer));

#if TEST_MODE
			if (wcscmp(szBuffer, TEST_TERMINAL_ID))
#else
			if (wcscmp(szBuffer, m_Parameters.szTerminalID))
#endif
			{
				return RESULT_INVALID;
			}
		}
		else
		{
			return RESULT_INVALID;
		}

		nLength	= 9;

		if (ISO8583_GetField(F03_PROCESSING_CODE, cBuffer, &nLength))
		{
			BA2WA(szBuffer, 9, cBuffer, strlen((const char*)cBuffer));

			if (wcscmp(szBuffer, pInfo->szProcessingCode))
			{
				return RESULT_INVALID;
			}
		}
		else
		{
			return RESULT_INVALID;
		}

		nLength	= 9;

		if (ISO8583_GetField(F11_STAN, cBuffer, &nLength))
		{
			BA2WA(szBuffer, 9, cBuffer, strlen((const char*)cBuffer));

			if (wcscmp(szBuffer, pInfo->szTraceNo))
			{
				return RESULT_INVALID;
			}

			wcscpy(pInfo->szTraceNo, szBuffer);
		}
		else
		{
			return RESULT_INVALID;
		}

		nLength	= 9;

		if (ISO8583_GetField(F39_RESPONSE_CODE, cBuffer, &nLength))
		{
			BA2WA(pInfo->szResponseCode, 3, cBuffer, strlen((const char*)cBuffer));

			if (!wcscmp(pInfo->szResponseCode, L"00"))
			{
				return RESULT_SUCCESS;
			}

			int	nResponseCode	= _wtoi(pInfo->szResponseCode);

			if ((nResponseCode < 1) || (nResponseCode > (MAX_RESPONSE_CODE - 1)))
			{
				return RESULT_INVALID;
			}

			m_nInfo[INFO_MESSAGE_TYPE]	= MESSAGE_ERROR;
			m_nInfo[INFO_MESSAGE_ID]	= nResponseCode;
			m_nInfo[INFO_MESSAGE_PRINT]	= TRUE;

			return RESULT_FAIL;
		}
		else
		{
			return RESULT_INVALID;
		}
	}

	return RESULT_INVALID;
}

void CChildView::TransactionProcess(TRANSACTION_INFO* pInfo)
{
	BYTE	cBuffer[512];
	CString	strBuffer;
	int		nLength;

	switch (pInfo->transactionStatus)
	{
	case STATUS_NONE:
		{
		}
		break;
	case STATUS_VOLATILE:
		{
			switch (pInfo->transactionMode)
			{
			case TRANSACTION_BALANCE:
				{
					RequestDisconnect();

					nLength	= 512;

					if (ISO8583_GetField(F04_TRXN_AMOUNT, cBuffer, &nLength))
					{
						LONGLONG	llAmount	= _atoi64((const char*)cBuffer);
						
						swprintf(pInfo->szAmount, L"%lld", llAmount);
					}

					nLength	= 512;

					if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
						pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F37_RRN, cBuffer, &nLength))
					{
						BA2WA(pInfo->szReferenceNo, 13, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F38_AUTH_ID_RESPONSE, cBuffer, &nLength))
					{
						BA2WA(pInfo->szAuthID, 7, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F44_ADDITIONAL_RESPONSE_DATA, cBuffer, &nLength))
					{
						ISESA2WA(pInfo->szAdditionalData, 26, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
					{
						pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
					}

					SetLocalTime(&pInfo->stTimestamp);
					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetPrintInfo(&m_PrintInfo, RECEIPT_BALANCE, pInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
					SetViewMode(VIEW_PRINT);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			case TRANSACTION_SERVICE_REQUEST:
				{
					RequestDisconnect();

					nLength	= 512;

					if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
						pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
					{
						pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
					}

					SetLocalTime(&pInfo->stTimestamp);
					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetPrintInfo(&m_PrintInfo, RECEIPT_SERVICE_REQUEST, pInfo, &m_Parameters, RSEQ_ACCEPTOR_ONLY);
					SetViewMode(VIEW_PRINT);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			}
		}
		break;
	case STATUS_LOGON:
		{
			switch (pInfo->transactionMode)
			{
			case TRANSACTION_LOGON:
				{
					RequestDisconnect();

					nLength	= 512;

					if (ISO8583_GetField(F48_ADD_DATA_PRIVATE, cBuffer, &nLength))
					{
						int		nTMSNeed		= cBuffer[0] - '0';
						int		nChangeKeyNeed	= cBuffer[1] - '0';
						DWORD	dwBankID		= 0x11111111;
						BYTE	cDESKey[32];
						BYTE	cAESKey[32];

						if (nChangeKeyNeed)
						{
							BYTE	KCV[3];
							DWORD	dwResult;
							UINT	uDES	= 16;
							UINT	uAES	= 16;

							StringToHexPair(&cBuffer[2], 32, cDESKey, &uDES);
							StringToHexPair(&cBuffer[34], 32, cAESKey, &uAES);

							if (!m_nSecureStatus)
							{
								dwResult	= BBSecure_EraseKey(dwBankID, TDES_PIN_ENC_KEY, 0);

								LOGFILE(L"%S[%d] - BBSecure_EraseKey() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

								dwResult	= BBSecure_SendPaymentKey_simple(KCV, cDESKey, 0, TDES_PIN_ENC_KEY, 16, dwBankID, ECB, WRITE_DECRYPTION, 0, FALSE, 0, NULL);

								LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

								dwResult	= BBSecure_EraseKey(dwBankID, TDES_MAC_KEY, 0);

								LOGFILE(L"%S[%d] - BBSecure_EraseKey() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

								dwResult	= BBSecure_SendPaymentKey_simple(KCV, cDESKey, 0, TDES_MAC_KEY, 16, dwBankID, ECB, WRITE_DECRYPTION, 0, FALSE, 0, NULL);

								LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);
							}
						}

						m_Settings.logonStatus	= LOGON_SUCCEEDED;
						
						ResetLogonTime();
						SaveSettings(&m_Settings);

#if !ALWAYS_TMS_NEED
						if (nTMSNeed)
						{
#endif
							::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_REQ_PARAMETERS, 0);
							LOGFILE(L"%S[%d] - TMSNeed\r\n", __FUNCTION__, __LINE__);
#if !ALWAYS_TMS_NEED
						}
#endif
					}

					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetViewMode(VIEW_RESULT);

					m_dwTimeoutResultTick	= GetTickCount() + TIMEOUT_RESULT;

					SetTransactionStatus(pInfo, STATUS_COMPLETED);
					SetTransactionStatus(&m_TransInfo, STATUS_COMPLETED);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			}
		}
		break;
	case STATUS_TRANSACTION:
		{
			switch (pInfo->transactionMode)
			{
			case TRANSACTION_SALE:
				{
					nLength	= 512;

					if (ISO8583_GetField(F04_TRXN_AMOUNT, cBuffer, &nLength))
					{
						LONGLONG	llAmount	= _atoi64((const char*)cBuffer);
						
						swprintf(pInfo->szAmount, L"%lld", llAmount);
					}

					nLength	= 512;

					if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
						pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F37_RRN, cBuffer, &nLength))
					{
						BA2WA(pInfo->szReferenceNo, 13, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F38_AUTH_ID_RESPONSE, cBuffer, &nLength))
					{
						BA2WA(pInfo->szAuthID, 7, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F44_ADDITIONAL_RESPONSE_DATA, cBuffer, &nLength))
					{
						ISESA2WA(pInfo->szAdditionalData, 26, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
					{
						pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
					}

					SetLocalTime(&pInfo->stTimestamp);
					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetPrintInfo(&m_PrintInfo, RECEIPT_SALE, pInfo, &m_Parameters, RSEQ_ALL);
					SetViewMode(VIEW_PRINT);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			case TRANSACTION_BILL_PAYMENT:
				{
					nLength	= 512;

					if (ISO8583_GetField(F04_TRXN_AMOUNT, cBuffer, &nLength))
					{
						LONGLONG	llAmount	= _atoi64((const char*)cBuffer);
						
						swprintf(pInfo->szAmount, L"%lld", llAmount);
					}

					nLength	= 512;

					if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
						pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F37_RRN, cBuffer, &nLength))
					{
						BA2WA(pInfo->szReferenceNo, 13, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F38_AUTH_ID_RESPONSE, cBuffer, &nLength))
					{
						BA2WA(pInfo->szAuthID, 7, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F44_ADDITIONAL_RESPONSE_DATA, cBuffer, &nLength))
					{
						ISESA2WA(pInfo->szAdditionalData, 26, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
					{
						pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
					}

					SetLocalTime(&pInfo->stTimestamp);
					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetPrintInfo(&m_PrintInfo, RECEIPT_BILL_PAYMENT, pInfo, &m_Parameters, RSEQ_ALL);
					SetViewMode(VIEW_PRINT);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			case TRANSACTION_CHARGE:
				{
					nLength	= 512;

					if (ISO8583_GetField(F04_TRXN_AMOUNT, cBuffer, &nLength))
					{
						LONGLONG	llAmount	= _atoi64((const char*)cBuffer);
						
						swprintf(pInfo->szAmount, L"%lld", llAmount);
					}

					nLength	= 512;

					if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
						pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
					{
						BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

						pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
						pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
					}

					nLength	= 512;

					if (ISO8583_GetField(F37_RRN, cBuffer, &nLength))
					{
						BA2WA(pInfo->szReferenceNo, 13, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F38_AUTH_ID_RESPONSE, cBuffer, &nLength))
					{
						BA2WA(pInfo->szAuthID, 7, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F44_ADDITIONAL_RESPONSE_DATA, cBuffer, &nLength))
					{
						ISESA2WA(pInfo->szAdditionalData, 26, cBuffer, strlen((const char*)cBuffer));
					}

					nLength	= 512;

					if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
					{
						pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
					}

					nLength	= 512;

					if (ISO8583_GetField(F62_PRIVATE_USE, cBuffer, &nLength))
					{
						BYTE		cLength[3];
						int			nSerialLength;
						int			nPINLength;

						memset(cLength, 0, 3);
						memcpy(cLength, &cBuffer[0], 2);

						pInfo->nChargeCount	= atoi((const char*)cLength);

						memcpy(cLength, &cBuffer[2], 2);

						nSerialLength	= atoi((const char*)cLength);

						memcpy(cLength, &cBuffer[4], 2);

						nPINLength		= atoi((const char*)cLength);

						for (int i = 0; i < (int)pInfo->nChargeCount; i++)
						{
							BA2WA(pInfo->szSerial[i], 21, &cBuffer[6 + i * (nSerialLength + nPINLength)], nSerialLength);
							BA2WA(pInfo->szChargePIN[i], 21, &cBuffer[6 + nSerialLength + i * (nSerialLength + nPINLength)], nPINLength);
						}
					}

					SetLocalTime(&pInfo->stTimestamp);
					memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
					SetPrintInfo(&m_PrintInfo, RECEIPT_CHARGE, pInfo, &m_Parameters, RSEQ_ALL);
					SetViewMode(VIEW_PRINT);
					ResetReversalTime();
					ResetAdviceTime();
				}
				break;
			}
		}
		break;
	case STATUS_REVERSAL:
		{
			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
			{
				pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
			}

			SetLocalTime(&pInfo->stTimestamp);
			SetTransactionStatus(pInfo, STATUS_REVERSAL_RESPONSED);
			ResetReversalTime();
			ResetAdviceTime();
			m_database.UpdateTransactionRecord(pInfo);
			SetReversalStatus();
		}
		break;
	case STATUS_ADVICE:
		{
			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
			{
				pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
			}

			SetLocalTime(&pInfo->stTimestamp);
			SetTransactionStatus(pInfo, STATUS_COMPLETED);
			ResetReversalTime();
			ResetAdviceTime();
			m_database.UpdateTransactionRecord(pInfo);
			SetAdviceStatus();
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::TransactionFailure(TRANSACTION_INFO* pInfo)
{
	BYTE	cBuffer[512];
	CString	strBuffer;
	int		nLength;

	RequestDisconnect();

	switch (GetTransactionStatus(pInfo))
	{
	case STATUS_NONE:
		{
		}
		break;
	case STATUS_VOLATILE:
		{
			m_dwTimeoutResultTick	= GetTickCount() + TIMEOUT_RESULT;

			ResetReversalTime();
			ResetAdviceTime();

			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			if (ISO8583_GetField(F60_PRIVATE_USE, cBuffer, &nLength))
			{
				pInfo->stTimestamp.wYear	= 2000 + (WORD)atoi((const char*)cBuffer);
			}

			SetLocalTime(&pInfo->stTimestamp);
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			SetPrintInfo(&m_PrintInfo, RECEIPT_UNSUCCESSFUL, pInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
			ShowMessage();
		}
		break;
	case STATUS_LOGON:
		{
			m_dwTimeoutResultTick	= GetTickCount() + TIMEOUT_RESULT;
			
			SetNextLogonTime();
			ResetReversalTime();
			ResetAdviceTime();
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			ShowMessage();
		}
		break;
	case STATUS_TRANSACTION:
		{
			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			SetTransactionStatus(pInfo, STATUS_ERROR);
			ResetReversalTime();
			ResetAdviceTime();
			SetLocalTime(&pInfo->stTimestamp);
			m_database.UpdateTransactionRecord(pInfo);
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			SetPrintInfo(&m_PrintInfo, RECEIPT_UNSUCCESSFUL, pInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
			ShowMessage();
		}
		break;
	case STATUS_REVERSAL:
		{
			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			SetLocalTime(&pInfo->stTimestamp);
			SetReversalRetry(pInfo, TRUE);
			ResetReversalTime();
			ResetAdviceTime();
		}
		break;
	case STATUS_ADVICE:
		{
			nLength	= 512;

			if (ISO8583_GetField(F12_TRXN_TIME, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wHour	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wMinute	= (WORD)_wtoi(strBuffer.Mid(2, 2));
				pInfo->stTimestamp.wSecond	= (WORD)_wtoi(strBuffer.Mid(4, 2));
			}

			nLength	= 512;

			if (ISO8583_GetField(F13_TRXN_DATE, cBuffer, &nLength))
			{
				BA2CS(strBuffer, cBuffer, strlen((const char*)cBuffer));

				pInfo->stTimestamp.wMonth	= (WORD)_wtoi(strBuffer.Mid(0, 2));
				pInfo->stTimestamp.wDay		= (WORD)_wtoi(strBuffer.Mid(2, 2));
			}

			SetLocalTime(&pInfo->stTimestamp);
			SetAdviceRetry(pInfo, TRUE);
			ResetReversalTime();
			ResetAdviceTime();
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::TransactionTimeout(TRANSACTION_INFO* pInfo)
{
	switch (GetTransactionStatus(pInfo))
	{
	case STATUS_NONE:
		{
		}
		break;
	case STATUS_VOLATILE:
		{
			RequestDisconnect();
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			SetThreadError(ERR_NO_RESPONSE, MESSAGE_ERROR);
		}
		break;
	case STATUS_LOGON:
		{
			RequestDisconnect();
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			SetNextLogonTime();
			SetThreadError(ERR_NO_RESPONSE, MESSAGE_ERROR);
		}
		break;
	case STATUS_TRANSACTION:
		{
			SetTransactionStatus(pInfo, STATUS_REVERSAL);
			
			ResetReversalTime();
			memcpy(&m_TransInfo, pInfo, sizeof(TRANSACTION_INFO));
			m_database.UpdateTransactionRecord(pInfo);
			SetReversalStatus();
			SetPrintInfo(&m_PrintInfo, RECEIPT_TIMEOUT, pInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
			SetThreadError(ERR_NO_RESPONSE, MESSAGE_ERROR, TRUE);
		}
		break;
	case STATUS_REVERSAL:
		{
			RequestDisconnect();
			SetReversalRetry(pInfo, FALSE);
			SetThreadError(ERR_NO_RESPONSE, MESSAGE_ERROR);
		}
		break;
	case STATUS_ADVICE:
		{
			RequestDisconnect();
			SetAdviceRetry(pInfo, FALSE);
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::SetReversalRetry(TRANSACTION_INFO* pInfo, BOOL bFullUpdate)
{
	pInfo->nAttempts++;

	if (bFullUpdate)
	{
		m_database.UpdateTransactionRecord(pInfo);
	}
	else
	{
		m_database.UpdateTransactionAttempts(pInfo);
	}

	SetNextReversalTime();
}

void CChildView::SetAdviceRetry(TRANSACTION_INFO* pInfo, BOOL bFullUpdate)
{
	pInfo->nAttempts++;

	if (bFullUpdate)
	{
		m_database.UpdateTransactionRecord(pInfo);
	}
	else
	{
		m_database.UpdateTransactionAttempts(pInfo);
	}

	SetNextAdviceTime();
}
