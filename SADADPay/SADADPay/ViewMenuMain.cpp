// ViewMenuMain.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

void CChildView::ExecuteMenuMain()
{
	if (!m_pPrinter->IsPaperLoaded())
	{
		SetTransactionMode(TRANSACTION_NONE);
		ShowMessage(ERR_INTRO_NO_PAPER);
		return;
	}

	switch (m_menuInfoMain.nCurItem)
	{
	case 0:
		{
			RequestCancelAdvice();
			SetViewMode(VIEW_SALE_AMOUNT);
			SetTransactionMode(TRANSACTION_SALE);
			ClearTransactionData();
			SetMandatoryData(&m_TransInfo);
			SetTransportData(GetTransactionMode(), (m_dwReceivedTime - m_dwSentTime));
		}
		break;
	case 1:
		{
			RequestCancelAdvice();
			SetViewMode(VIEW_PIN);
			SetTransactionMode(TRANSACTION_BALANCE);
			ClearTransactionData();
			SetMandatoryData(&m_TransInfo);
			SetTransportData(GetTransactionMode(), (m_dwReceivedTime - m_dwSentTime));

			m_dwTimeoutPINTick	= GetTickCount() + (m_Parameters.nTimeoutPIN * 1000);
		}
		break;
	case 2:
		{
			RequestCancelAdvice();
			SetViewMode(VIEW_BILL_BILLID);
			SetTransactionMode(TRANSACTION_BILL_PAYMENT);
			ClearTransactionData();
			SetMandatoryData(&m_TransInfo);
			SetTransportData(GetTransactionMode(), (m_dwReceivedTime - m_dwSentTime));
		}
		break;
	case 3:
		{
			RequestCancelAdvice();
			SetViewMode(VIEW_CHARGE_COUNT);
			SetTransactionMode(TRANSACTION_CHARGE);
			ClearTransactionData();
			SetMandatoryData(&m_TransInfo);
			SetTransportData(GetTransactionMode(), (m_dwReceivedTime - m_dwSentTime));
		}
		break;
	}
}

void CChildView::DoBeforePaintViewMenuMain()
{
	if (GetPaintFlag() == PAINT_FULL_SCREEN)
	{
		RequestConnect(REQ_TRANSACTION);
	}
}
