// ViewSale.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

void CChildView::PaintViewSaleAmount(CDCX* pDC)
{
	RECT	rc;

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_ENTER_AMOUNT), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		GetRectFromPNGResource(&rc, RP_INPUT_WO_DESC);
		pDC->FillSolidRect(&rc, COLOR_RGB_WHITE);
		pDC->SelectObject(&m_fntDigitLarge);
		pDC->SetTextColor(COLOR_RGB_TEXT);
		pDC->DrawText(FormattingDigit(m_sInfo[INFO_AMOUNT], TRUE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSaleConfirm(CDCX* pDC)
{
	RECT	rc;

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_CONFIRM_CANCEL), pDC);
		GetRectFromPNGResource(&rc, RP_INPUT_WO_DESC);
		pDC->FillSolidRect(&rc, COLOR_RGB_CONFIRM);
		pDC->SelectObject(&m_fntDigitLarge);
		pDC->SetTextColor(COLOR_RGB_WHITE);
		pDC->DrawText(FormattingDigit(m_sInfo[INFO_AMOUNT], TRUE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewSale(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_SALE_AMOUNT:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					if ((m_sInfo[INFO_AMOUNT].Compare(L"0")) && (m_sInfo[INFO_AMOUNT].GetLength() > 1))
					{
						wcscpy(m_TransInfo.szAmount, m_sInfo[INFO_AMOUNT]);

						SetViewMode(VIEW_SALE_CONFIRM);
						SetTransactionMode(TRANSACTION_SALE);
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
					if (m_sInfo[INFO_AMOUNT].GetLength() >= 1)
					{
						m_sInfo[INFO_AMOUNT].Delete(m_sInfo[INFO_AMOUNT].GetLength() - 1);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
					{
						if (m_sInfo[INFO_AMOUNT].IsEmpty() && (GetHardwareKey(nChar) == HWK_0))
						{
							break;
						}

						if (m_sInfo[INFO_AMOUNT].GetLength() < LENGTH_AMOUNT)
						{
							m_sInfo[INFO_AMOUNT].AppendChar(nChar);
							ForcePaintFlag(PAINT_FIELDS);
						}
					}
				}
				break;
			}
		}
		break;
	case VIEW_SALE_CONFIRM:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					SetViewMode(VIEW_PIN);
					SetTransactionMode(TRANSACTION_SALE);

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

void CChildView::OnLButtonDownViewSale(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_SALE_AMOUNT:
		{
			OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
		}
		break;
	case VIEW_SALE_CONFIRM:
		{
			OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
		}
		break;
	}
}

void CChildView::OnLButtonUpViewSale(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_SALE_AMOUNT:
		{
			OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
		}
		break;
	case VIEW_SALE_CONFIRM:
		{
			OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);		}
		break;
	}
}
