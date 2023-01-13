// ViewBill.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

void CChildView::PaintViewBillBillId(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_ENTER_BILL_ID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		GetRectFromPNGResource(&rc, RP_INPUT_WO_DESC);
		pDC->SelectObject(&m_fntDigitLarge);
		pDC->SetTextColor(COLOR_RGB_TEXT);
		pDC->FillSolidRect(&rc, COLOR_RGB_WHITE);
		pDC->DrawText(FormattingDigit(m_sInfo[INFO_BILLID], FALSE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewBillPaymentId(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_ENTER_PAYMENT_ID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		GetRectFromPNGResource(&rc, RP_INPUT_WO_DESC);
		pDC->SelectObject(&m_fntDigitLarge);
		pDC->SetTextColor(COLOR_RGB_TEXT);
		pDC->FillSolidRect(&rc, COLOR_RGB_WHITE);
		pDC->DrawText(FormattingDigit(m_sInfo[INFO_PAYMENTID], FALSE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewBillConfirm(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		RECT	rcKey;
		RECT	rcValue;

		PaintBackground(pDC);
		pDC->SetTextColor(COLOR_RGB_TEXT);
		pDC->SelectObject(&m_fntHeaderSmall);

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				rcKey.left		= BX_FA_KEY;
				rcKey.top		= IY;
				rcKey.right		= rcKey.left + BW_KEY;
				rcKey.bottom	= rcKey.top + IH;

				rcValue.left	= BX_FA_VALUE;
				rcValue.top		= IY;
				rcValue.right	= rcValue.left + BW_VALUE;
				rcValue.bottom	= rcValue.top + IH;

				pDC->DrawText(LoadText(IDS_BILL_ID), &rcKey, DT_RIGHT | DT_VCENTER);
				pDC->DrawText(FormattingFarsiDigit(m_TransInfo.szBillID, FALSE), &rcValue, DT_RIGHT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_PAYMENT_ID), &rcKey, DT_RIGHT | DT_VCENTER);
				pDC->DrawText(FormattingFarsiDigit(m_TransInfo.szPaymentID, FALSE), &rcValue, DT_RIGHT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_AMOUNT), &rcKey, DT_RIGHT | DT_VCENTER);
				pDC->DrawText(FormattingFarsiDigit(m_TransInfo.szAmount, TRUE), &rcValue, DT_RIGHT | DT_VCENTER);
			}
			break;
		default:
			{
				rcKey.left		= BX_EN_KEY;
				rcKey.top		= IY;
				rcKey.right		= rcKey.left + BW_KEY;
				rcKey.bottom	= rcKey.top + IH;

				rcValue.left	= BX_EN_VALUE;
				rcValue.top		= IY;
				rcValue.right	= rcValue.left + BW_VALUE;
				rcValue.bottom	= rcValue.top + IH;

				pDC->DrawText(LoadText(IDS_BILL_ID), &rcKey, DT_LEFT | DT_VCENTER);
				pDC->DrawText(FormattingArabicDigit(m_TransInfo.szBillID, FALSE), &rcValue, DT_LEFT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_PAYMENT_ID), &rcKey, DT_LEFT | DT_VCENTER);
				pDC->DrawText(FormattingArabicDigit(m_TransInfo.szPaymentID, FALSE), &rcValue, DT_LEFT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_AMOUNT), &rcKey, DT_LEFT | DT_VCENTER);
				pDC->DrawText(FormattingArabicDigit(m_TransInfo.szAmount, TRUE), &rcValue, DT_LEFT | DT_VCENTER);
			}
			break;
		}
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewBill(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_BILL_BILLID:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					WCHAR	szBillID[32];

					wcscpy(szBillID, m_sInfo[INFO_BILLID]);

					if (ValidateBillID(szBillID))
					{
						wcscpy(m_TransInfo.szBillID, m_sInfo[INFO_BILLID]);

						m_TransInfo.nServiceCode	= GetServiceCodeFromBillID(m_TransInfo.szBillID);

						SetViewMode(VIEW_BILL_PAYMENTID);
						SetTransactionMode(TRANSACTION_BILL_PAYMENT);
					}
					else
					{
						m_sInfo[INFO_BILLID].Empty();
						ShowMessage(ERR_INVALID_BILLID, MESSAGE_ERROR);
					}
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					Restart();
				}
				break;
			case HWK_BACK:
				{
					if (m_sInfo[INFO_BILLID].GetLength() >= 1)
					{
						m_sInfo[INFO_BILLID].Delete(m_sInfo[INFO_BILLID].GetLength() - 1);
					}
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
					{
						if (m_sInfo[INFO_BILLID].GetLength() < LENGTH_BILLID)
						{
							m_sInfo[INFO_BILLID].AppendChar(nChar);
						}
					}
				}
				break;
			}
		}
		break;
	case VIEW_BILL_PAYMENTID:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					WCHAR	cBillID[32];
					WCHAR	cPaymentID[32];

					wcscpy(cBillID, m_TransInfo.szBillID);
					wcscpy(cPaymentID, m_sInfo[INFO_PAYMENTID]);

					if (ValidatePaymentID(cBillID, cPaymentID))
					{
						wcscpy(m_TransInfo.szPaymentID, m_sInfo[INFO_PAYMENTID]);
						wcscpy(m_TransInfo.szAmount, m_sInfo[INFO_PAYMENTID].Left(m_sInfo[INFO_PAYMENTID].GetLength() - 5) + L"000");
						SetViewMode(VIEW_BILL_CONFIRM);
						SetTransactionMode(TRANSACTION_BILL_PAYMENT);
					}
					else
					{
						m_sInfo[INFO_PAYMENTID].Empty();
						ShowMessage(ERR_INVALID_BILLID, MESSAGE_ERROR);
					}
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					Restart();
				}
				break;
			case HWK_BACK:
				{
					if (m_sInfo[INFO_PAYMENTID].GetLength() >= 1)
					{
						m_sInfo[INFO_PAYMENTID].Delete(m_sInfo[INFO_PAYMENTID].GetLength() - 1);
					}
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
					{
						if (m_sInfo[INFO_PAYMENTID].GetLength() < LENGTH_PAYMENTID)
						{
							m_sInfo[INFO_PAYMENTID].AppendChar(nChar);
						}
					}
				}
				break;
			}
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					SetViewMode(VIEW_PIN);
					SetTransactionMode(TRANSACTION_BILL_PAYMENT);

					m_dwTimeoutPINTick	= GetTickCount() + (m_Parameters.nTimeoutPIN * 1000);
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					Restart();
				}
				break;
			}
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::OnLButtonDownViewBill(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
		{
			OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
		}
		break;
	}
}

void CChildView::OnLButtonUpViewBill(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
		{
			OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);
		}
		break;
	}
}
