// ViewCharge.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

LONGLONG CChildView::CaculateChargeAmount(WCHAR* szProviderID, WCHAR* szCategoryID)
{
	int	nCategoryID	= _wtoi(szCategoryID);

	for (int i = 0; i < MAX_PROVIDER; i++)
	{
		if (!wcscmp(m_Parameters.providers[i].szProviderID, szProviderID))
		{
			for (int j = 0; j < MAX_VOUCHER; j++)
			{
				if (m_Parameters.providers[i].vouchers[j].nCID = nCategoryID)
				{
					return m_Parameters.providers[i].vouchers[j].llAmount;
				}
			}
		}
	}

	return 0;
}

void CChildView::PaintViewChargeCount(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_CHARGE_COUNT), pDC);
		PaintHeaderDesc(LoadText(IDS_CHARGE_DESCRIPTION), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		GetRectFromPNGResource(&rc, RP_INPUT_W_DESC);
		pDC->FillSolidRect(&rc, COLOR_RGB_WHITE);
		pDC->SelectObject(&m_fntDigitLarge);
		pDC->DrawText(FormattingDigit(m_sInfo[INFO_CHARGE_COUNT], TRUE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewChargeConfirm(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		RECT	rcKey;
		RECT	rcValue;
		CString	strText;

		PaintBackground(pDC);
		pDC->SelectObject(&m_fntHeaderSmall);
		pDC->SetTextColor(COLOR_RGB_TEXT);

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				rcKey.left		= CX_FA_KEY;
				rcKey.top		= IY;
				rcKey.right		= rcKey.left + CW_KEY;
				rcKey.bottom	= rcKey.top + IH;

				rcValue.left	= CX_FA_VALUE;
				rcValue.top		= IY;
				rcValue.right	= rcValue.left + CW_VALUE;
				rcValue.bottom	= rcValue.top + IH;

				pDC->DrawText(LoadText(IDS_PROVIDER), &rcKey, DT_RIGHT | DT_VCENTER);
				pDC->DrawText(m_Parameters.providers[m_menuInfoProvider.nCurItem].providerName.szF, &rcValue, DT_RIGHT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_CATEGORY), &rcKey, DT_RIGHT | DT_VCENTER);

				strText.Format(L"%lld %s", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[m_menuInfoCategory.nCurItem].llAmount, LoadFarsiText(IDS_RIALS));
				pDC->DrawText(FormattingFarsiDigit(strText, FALSE), &rcValue, DT_RIGHT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_CHARGECOUNT), &rcKey, DT_RIGHT | DT_VCENTER);
				pDC->DrawText(FormattingFarsiDigit(m_sInfo[INFO_CHARGE_COUNT], TRUE), &rcValue, DT_RIGHT | DT_VCENTER);
			}
			break;
		default:
			{
				rcKey.left		= CX_EN_KEY;
				rcKey.top		= IY;
				rcKey.right		= rcKey.left + CW_KEY;
				rcKey.bottom	= rcKey.top + IH;

				rcValue.left	= CX_EN_VALUE;
				rcValue.top		= IY;
				rcValue.right	= rcValue.left + CW_VALUE;
				rcValue.bottom	= rcValue.top + IH;

				pDC->DrawText(LoadText(IDS_PROVIDER), &rcKey, DT_LEFT | DT_VCENTER);
				pDC->DrawText(m_Parameters.providers[m_menuInfoProvider.nCurItem].providerName.szE, &rcValue, DT_LEFT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_CATEGORY), &rcKey, DT_LEFT | DT_VCENTER);

				strText.Format(L"%lld %s", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[m_menuInfoCategory.nCurItem].llAmount, LoadEnglishText(IDS_RIALS));
				pDC->DrawText(FormattingArabicDigit(strText, FALSE), &rcValue, DT_LEFT | DT_VCENTER);

				rcKey.top		+= IG;
				rcKey.bottom	+= IG;
				rcValue.top		+= IG;
				rcValue.bottom	+= IG;

				pDC->DrawText(LoadText(IDS_CHARGECOUNT), &rcKey, DT_LEFT | DT_VCENTER);
				pDC->DrawText(FormattingArabicDigit(m_sInfo[INFO_CHARGE_COUNT], TRUE), &rcValue, DT_LEFT | DT_VCENTER);
			}
			break;
		}
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewCharge(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_CHARGE_COUNT:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					if (!m_sInfo[INFO_CHARGE_COUNT].IsEmpty())
					{
						m_TransInfo.nChargeCount	= _wtoi(m_sInfo[INFO_CHARGE_COUNT]);

						InitializeMenuProvider();
						SetViewMode(VIEW_MENU_PROVIDER);
						SetTransactionMode(TRANSACTION_CHARGE);
					}
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					Restart();
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_1) && (GetHardwareKey(nChar) <= HWK_5))
					{
						m_sInfo[INFO_CHARGE_COUNT].Format(L"%c", nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
				break;
			}
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					SetViewMode(VIEW_PIN);
					SetTransactionMode(TRANSACTION_CHARGE);

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

void CChildView::OnLButtonDownViewCharge(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_CHARGE_COUNT:
		{
			OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
			OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
		}
		break;
	}
}

void CChildView::OnLButtonUpViewCharge(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_CHARGE_COUNT:
		{
			OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
			OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);
		}
		break;
	}
}
