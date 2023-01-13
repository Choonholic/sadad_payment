// ViewKeypad.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "StringRes.h"

void CChildView::PaintViewKeypad(CDCX* pDC)
{
#if SOFTWARE_KEYPAD
	pDC->SelectObject(&m_fntDigitLarge);

	for (int i = 0; i < (MAX_KEYPAD - 1); i++)
	{
		if (PaintRequired((0x01 << i) << 16))
		{
			RECT	rc;

			GetRectFromPNGResource(&rc, RP_KEYPAD[i]);

			if (i < (MAX_KEYPAD - 2))
			{
				if (m_nInfo[INFO_KEYPAD_STATUS]	== ((0x01 << i) << 16))
				{
					pDC->FillSolidRect(&rc, COLOR_RGB_KEYPAD_PRESS);
				}
				else
				{
					pDC->FillSolidRect(&rc, COLOR_RGB_KEYPAD);
				}

				pDC->SetTextColor(COLOR_RGB_WHITE);

				switch (i)
				{
				case 9:
					{
						pDC->DrawText(LoadText(IDS_NUM0), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				case 10:
					{
						pDC->DrawText(L".", &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				default:
					{
						pDC->DrawText(LoadText(IDS_NUM1 + i), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				}
			}
			else
			{
				if (m_nInfo[INFO_KEYPAD_STATUS]	== ((0x01 << i) << 16))
				{
					DrawPNGResource(RP_KEYPAD[12], pDC->GetSafeHdc());
				}
				else
				{
					DrawPNGResource(RP_KEYPAD[11], pDC->GetSafeHdc());
				}
			}
		}
	}
#endif
}

void CChildView::OnLButtonDownViewKeypad(UINT nFlags, CPoint point)
{
#if SOFTWARE_KEYPAD
	RECT	rc;

	for (int i = 0; i < (MAX_KEYPAD - 1); i++)
	{
		GetRectFromPNGResource(&rc, RP_KEYPAD[i]);

		if (PtInRect(&rc, point))
		{
			m_nInfo[INFO_KEYPAD_STATUS]	= ((0x01 << i) << 16);

			ForcePaintFlag((0x01 << i) << 16);
		}
	}
#endif
}

#if SOFTWARE_KEYPAD
UINT	nKey[MAX_KEYPAD]	=
{
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', VK_PERIOD, VK_BACK, VK_BACK
};
#endif

void CChildView::OnLButtonUpViewKeypad(UINT nFlags, CPoint point)
{
#if SOFTWARE_KEYPAD
	if (m_nInfo[INFO_KEYPAD_STATUS] != 0x00)
	{
		ForcePaintFlag(m_nInfo[INFO_KEYPAD_STATUS]);

		for (int i = 0; i < (MAX_KEYPAD - 1); i++)
		{
			if (m_nInfo[INFO_KEYPAD_STATUS]	== ((0x01 << i) << 16))
			{
				PostMessage(WM_KEYDOWN, nKey[i]);
			}
		}

		m_nInfo[INFO_KEYPAD_STATUS]	= 0x00;
	}
#endif
}
