// ViewPrint.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "StringRes.h"
#include "PNGRes.h"
#include "Utilities.h"

void CChildView::SetPrintInfo(PRINT_INFO* pPrintInfo, RECEIPT_TYPE ReceiptType, TRANSACTION_INFO* pTransactionInfo, PARAMETERS_INFO* pParameters, RECEIPT_SEQUENCE ReceiptSequence)
{
	memset(pPrintInfo, 0, sizeof(PRINT_INFO));

	pPrintInfo->ReceiptType		= ReceiptType;
	pPrintInfo->pParameters		= pParameters;
	pPrintInfo->ReceiptSequence	= ReceiptSequence;

	memcpy(&pPrintInfo->TransactionInfo, pTransactionInfo, sizeof(TRANSACTION_INFO));
}

void CChildView::SetPrintInfo(PRINT_INFO* pPrintInfo, RECEIPT_TYPE ReceiptType, CHARGE_INFO* pChargeInfo, PARAMETERS_INFO* pParameters, RECEIPT_SEQUENCE ReceiptSequence)
{
	memset(pPrintInfo, 0, sizeof(PRINT_INFO));

	pPrintInfo->ReceiptType		= ReceiptType;
	pPrintInfo->pParameters		= pParameters;
	pPrintInfo->ReceiptSequence	= ReceiptSequence;

	memcpy(&pPrintInfo->ChargeInfo, pChargeInfo, sizeof(CHARGE_INFO));
}

void CChildView::PrintReceipt(PRINT_INFO* pPrintInfo)
{
	DWORD	dwError	= 0;

	switch (pPrintInfo->ReceiptType)
	{
	case RECEIPT_TRANSACTION_DETAILS:
		{
			dwError	= m_pPrinter->PrintTransactionDetails(m_arrDetailsType, m_arrDetailsFeed, pPrintInfo->pParameters); 

			switch (dwError)
			{
			case BB_PRINT_ERROR_NONE:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					Restart();
				}
				break;
			case BB_PRINT_ERROR_NOPAPER:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					ShowMessage(ERR_PRINT_NO_PAPER);
				}
				return;
			case BB_PRINT_ERROR_OVERHEAT:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					ShowMessage(ERR_PRINT_OVERHEAT);
				}
				return;
			case BB_PRINT_ERROR_LOWVOLTAGE:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					ShowMessage(ERR_PRINT_LOW_VOLT);
				}
				return;
			case BB_PRINT_ERROR_HIGHVOLTAGE:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					ShowMessage(ERR_PRINT_HIGH_VOLT);
				}
				return;
			default:
				{
					m_arrDetailsType.RemoveAll();
					m_arrDetailsFeed.RemoveAll();
					ShowMessage(ERR_PRINT_GENERIC);
				}
				return;
			}
		}
		break;
	default:
		{
			m_nInfo[INFO_RECEIPT_TYPE]	= ((pPrintInfo->ReceiptSequence == RSEQ_ACCEPTOR_ONLY) ? 1 : 0);

			if (m_nInfo[INFO_PRINT_STATUS])
			{
				m_nInfo[INFO_PRINT_STATUS]	= FALSE;

				if (m_nInfo[INFO_RECEIPT_TYPE])
				{
					dwError	= m_pPrinter->PrintReceiptHeaderAndBody(pPrintInfo, (BOOL)m_nInfo[INFO_RECEIPT_TYPE]);
				}
				else
				{
					dwError	= m_pPrinter->PrintReceiptBody(pPrintInfo, (BOOL)m_nInfo[INFO_RECEIPT_TYPE]);
				}
			}
			else
			{
				dwError	= m_pPrinter->PrintReceiptHeaderAndBody(pPrintInfo, (BOOL)m_nInfo[INFO_RECEIPT_TYPE]);
			}

			if (m_nInfo[INFO_MESSAGE_PRINT])
			{
				ResetMessage();
			}

			switch (dwError)
			{
			case BB_PRINT_ERROR_NONE:
				{
					DoActionAfterPrint(pPrintInfo, TRUE);
				}
				break;
			case BB_PRINT_ERROR_NOPAPER:
				{
					ShowMessage(ERR_PRINT_NO_PAPER);
				}
				return;
			case BB_PRINT_ERROR_OVERHEAT:
				{
					ShowMessage(ERR_PRINT_OVERHEAT);
				}
				return;
			case BB_PRINT_ERROR_LOWVOLTAGE:
				{
					ShowMessage(ERR_PRINT_LOW_VOLT);
				}
				return;
			case BB_PRINT_ERROR_HIGHVOLTAGE:
				{
					ShowMessage(ERR_PRINT_HIGH_VOLT);
				}
				return;
			default:
				{
					ShowMessage(ERR_PRINT_GENERIC);
				}
				return;
			}

			if (pPrintInfo->ReceiptSequence == RSEQ_ALL)
			{
				pPrintInfo->ReceiptSequence	= RSEQ_ACCEPTOR_ONLY;

				SetViewMode(VIEW_PRINT_ACCEPTOR);
				ForcePaintFlag();
			}
		}
		break;
	}
}

void CChildView::DoActionAfterPrint(PRINT_INFO* pPrintInfo, BOOL bSuccess)
{
	if (m_nInfo[INFO_RECEIPT_TYPE] == 0)
	{
		switch (pPrintInfo->ReceiptType)
		{
		case RECEIPT_SALE:
			{
				if (bSuccess)
				{
					SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_ADVICE);
					ResetReversalTime();
					ResetAdviceTime();
					m_database.UpdateTransactionRecord(&pPrintInfo->TransactionInfo);
					SetAdviceStatus();
				}
				else
				{
					SetReversalStatus();
				}
			}
			break;
		case RECEIPT_BALANCE:
			{
				SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_BALANCE_RESPONSED);
				ResetReversalTime();
				ResetAdviceTime();
			}
			break;
		case RECEIPT_BILL_PAYMENT:
			{
				if (bSuccess)
				{
					SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_ADVICE);
					ResetReversalTime();
					ResetAdviceTime();
					m_database.UpdateTransactionRecord(&pPrintInfo->TransactionInfo);
					SetAdviceStatus();
				}
				else
				{
					SetReversalStatus();
				}
			}
			break;
		case RECEIPT_CHARGE:
			{
				if (bSuccess)
				{
					CHARGE_INFO	chargeInfo;

					memset(&chargeInfo, 0, sizeof(CHARGE_INFO));
					SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_ADVICE);
					ResetReversalTime();
					ResetAdviceTime();
					m_database.UpdateTransactionRecord(&pPrintInfo->TransactionInfo);
					SetAdviceStatus();

					memcpy(&chargeInfo.stTimestamp, &pPrintInfo->TransactionInfo.stTimestamp, sizeof(SYSTEMTIME));
					wcscpy(chargeInfo.szTraceNo, pPrintInfo->TransactionInfo.szTraceNo);
					wcscpy(chargeInfo.szProviderID, pPrintInfo->TransactionInfo.szProviderID);
					wcscpy(chargeInfo.szCategoryID, pPrintInfo->TransactionInfo.szCategoryID);
					swprintf(chargeInfo.szAmount, L"%lld", (_wtoll(pPrintInfo->TransactionInfo.szAmount) / pPrintInfo->TransactionInfo.nChargeCount));
					wcscpy(chargeInfo.szReferenceNo, pPrintInfo->TransactionInfo.szReferenceNo);
					wcscpy(chargeInfo.szAuthID, pPrintInfo->TransactionInfo.szAuthID);

					for (int i = 0; i < (int)pPrintInfo->TransactionInfo.nChargeCount; i++)
					{
						GetDatabaseID(chargeInfo.szID, 17, i);

						wcscpy(chargeInfo.szSerial, pPrintInfo->TransactionInfo.szSerial[i]);
						wcscpy(chargeInfo.szChargePIN, pPrintInfo->TransactionInfo.szChargePIN[i]);

						m_database.InsertChargeRecord(&chargeInfo);
					}
				}
				else
				{
					SetReversalStatus();
				}
			}
			break;
		case RECEIPT_SELL_CHARGE:
			{
				m_database.DeleteChargeRecord(&pPrintInfo->ChargeInfo);
			}
			break;
		case RECEIPT_SERVICE_REQUEST:
			{
				SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_COMPLETED);
				ResetReversalTime();
				ResetAdviceTime();
			}
			break;
		case RECEIPT_UNSUCCESSFUL:
			{
				SetTransactionStatus(&pPrintInfo->TransactionInfo, STATUS_ERROR);
				ResetReversalTime();
				ResetAdviceTime();
				m_database.UpdateTransactionRecord(&pPrintInfo->TransactionInfo);
			}
			break;
		case RECEIPT_TIMEOUT:
			{
			}
			break;
		}
	}

	Restart();
}

void CChildView::DoAfterPaintViewPrint()
{
	PrintReceipt(&m_PrintInfo);
}

void CChildView::PaintViewPrint(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		if (m_nInfo[INFO_MESSAGE_PRINT])
		{
			CString	strResponseText;

			PaintBackground(pDC);
			PaintHeader(LoadText(m_nInfo[INFO_MESSAGE_TYPE] == MESSAGE_ERROR ? IDS_ERROR : IDS_INFORMATION), pDC);

			if (m_nInfo[INFO_MESSAGE_ID] < ERR_INTERNAL_ERROR)
			{
				strResponseText	= LoadText(GetResponseText(m_nInfo[INFO_MESSAGE_ID], &m_Parameters));
			}
			else
			{
				strResponseText	= LoadText(m_nInfo[INFO_MESSAGE_ID] - 100);
			}

			PaintMessage(strResponseText, RP_MESSAGE, pDC);
		}
		else
		{
			PaintBackground(pDC);
			PaintHeader(LoadText(IDS_PRINTING), pDC);
			DrawPNGResource(RP_PRINT, pDC->GetSafeHdc());
			PaintMessage(LoadText(IDS_PLEASE_WAIT), RP_PRINT_MESSAGE, pDC);
		}
	}
}

void CChildView::PaintViewPrintAcceptor(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_PRINT_RECEIPT), pDC);
		DrawPNGResource(RP_PRINT, pDC->GetSafeHdc());
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewPrintRetry(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_PRINT_RETRY), pDC);
		DrawPNGResource(RP_PRINT, pDC->GetSafeHdc());
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewPrint(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_PRINT:
		{
			if (m_nInfo[INFO_MESSAGE_PRINT])
			{
				ResetMessage();
				ForcePaintFlag();
			}
		}
		break;
	case VIEW_PRINT_ACCEPTOR:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					m_nInfo[INFO_RECEIPT_TYPE]	= 1;

					SetViewMode(VIEW_PRINT);
					ForcePaintFlag();
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					DoActionAfterPrint(&m_PrintInfo, FALSE);
					Restart();
				}
				break;
			}
		}
		break;
	case VIEW_PRINT_RETRY:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					SetViewMode(VIEW_PRINT);
					ForcePaintFlag();
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					DoActionAfterPrint(&m_PrintInfo, FALSE);
					Restart();
				}
				break;
			}
		}
		break;
	}
}

void CChildView::OnLButtonDownViewPrint(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_PRINT:
		{
		}
		break;
	case VIEW_PRINT_ACCEPTOR:
	case VIEW_PRINT_RETRY:
		{
			OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
		}
		break;
	}
}

void CChildView::OnLButtonUpViewPrint(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_PRINT:
		{
			if (m_nInfo[INFO_MESSAGE_PRINT])
			{
				ResetMessage();
				ForcePaintFlag();
			}
		}
		break;
	case VIEW_PRINT_ACCEPTOR:
	case VIEW_PRINT_RETRY:
		{
			OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);
		}
		break;
	}
}
