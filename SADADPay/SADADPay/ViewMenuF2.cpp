// ViewMenuF2.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "StringRes.h"

void CChildView::Logon()
{
	m_Settings.logonStatus	= LOGON_FAILED;

	SetNextLogonTime();
	SaveSettings(&m_Settings);
	SetTransactionMode(TRANSACTION_LOGON);
	RequestCancelAdvice();
	ClearTransactionData();
	SetMandatoryData(&m_TransInfo);
	SetTransactionData(&m_TransInfo, GetSystemTraceNo());
	SetTransportData(m_TransInfo.transactionMode, (m_dwReceivedTime - m_dwSentTime));
	RequestConnect(REQ_TRANSACTION);
	RequestTransaction();
	SetViewMode(VIEW_DIALUP);
	ForcePaintFlag();
}

void CChildView::ExecuteMenuF2()
{
	switch (m_menuInfoF2.nCurItem)
	{
	case 0:
		{
			m_nInfo[INFO_CHANGE_PASSWORD]	= CPW_NEW;
			
			m_sInfo[INFO_CPW_NEW].Empty();
			SetViewMode(VIEW_PASSWORD);
			SetTransactionMode(TRANSACTION_CHANGE_PASSWORD);
		}
		break;
	case 1:
		{
			Logon();
		}
		break;
	case 2:
		{
			InitializeMenu(&m_menuInfoServiceRequest, MENU_SERVICE_REQUEST, IDS_SERVICE_REQUEST, IDS_PAPER_REQUEST, IDS_DEVICE_FAILURE, TRUE, VIEW_MENU_F2, TRANSACTION_F2);
			SetViewMode(VIEW_MENU_SERVICE_REQUEST);
			SetTransactionMode(TRANSACTION_SERVICE_REQUEST);
		}
		break;
	}
}

void CChildView::ExecuteMenuServiceRequest()
{
	swprintf(m_TransInfo.szServiceType, L"%02d", (m_menuInfoServiceRequest.nCurItem + 1));
	SetTransactionMode(TRANSACTION_SERVICE_REQUEST);
	RequestCancelAdvice();
	ClearTransactionData();
	SetMandatoryData(&m_TransInfo);
	SetTransactionData(&m_TransInfo, GetSystemTraceNo());
	RequestConnect(REQ_TRANSACTION);
	RequestTransaction();
	SetViewMode(VIEW_DIALUP);
	ForcePaintFlag();

	m_nInfo[INFO_PRINT_STATUS]	= FALSE;

	if ((GetThreadStatus() == TSTATUS_DIALING) || (GetThreadStatus() == TSTATUS_CONNECTING))
	{
		m_pPrinter->PrintReceiptHeader(&m_Parameters);

		m_nInfo[INFO_PRINT_STATUS]	= TRUE;
	}
}
