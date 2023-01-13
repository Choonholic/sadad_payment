// ViewIntro.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"
#include "Jalali.h"
#include "Device.h"
#include "DrawPNG.h"

void CChildView::Restart()
{
	RequestDisconnect();
	ClearThreadError();
	ClearInfo();
	RestartMSR();
	SetViewMode(VIEW_INTRO);
	SetTransactionMode(TRANSACTION_NONE);
	ForcePaintFlag();
}

void CChildView::UpdateViewIntro()
{
	if (GetViewMode() == VIEW_INTRO)
	{
		SYSTEMTIME	st;

		if (m_tThreadFirstRun == TS_INITIATED)
		{
			m_tThreadFirstRun	= TS_SET;
		}

		switch (GetScreenMode())
		{
		case SCREEN_NORMAL:
			{
				if ((GetTickCount() - m_dwTimeoutScreenTick) > (m_Parameters.nTimeoutScreen * 1000))
				{
					m_dwTimeoutScreenTick	= GetTickCount();
					m_dwTimeoutGlanceTick	= GetTickCount();

					SetScreenMode(SCREEN_OUT);
				}

				GetLocalTime(&st);

				st.wSecond			= 0;
				st.wMilliseconds	= 0;

				if (memcmp(&st, &m_PaintFlags.stIntroTime, sizeof(SYSTEMTIME)))
				{
					memcpy(&m_PaintFlags.stIntroTime, &st, sizeof(SYSTEMTIME));
					ForcePaintFlag(PAINT_INTRO_TIME);
				}
			}
			break;
		case SCREEN_OUT:
			{
				if ((GetTickCount() - m_dwTimeoutGlanceTick) > TIMEOUT_GLANCE)
				{
					m_dwTimeoutScreenTick	= GetTickCount();
					m_dwTimeoutGlanceTick	= GetTickCount();

					SetScreenMode(SCREEN_GLANCE);
				}

				GetLocalTime(&st);

				st.wSecond			= 0;
				st.wMilliseconds	= 0;

				if (memcmp(&st, &m_PaintFlags.stIntroTime, sizeof(SYSTEMTIME)))
				{
					memcpy(&m_PaintFlags.stIntroTime, &st, sizeof(SYSTEMTIME));
					ForcePaintFlag(PAINT_INTRO_TIME);
				}
			}
			break;
		case SCREEN_GLANCE:
			{
				if ((GetTickCount() - m_dwTimeoutGlanceTick) > INTERVAL_GLANCE)
				{
					m_dwTimeoutScreenTick	= GetTickCount();
					m_dwTimeoutGlanceTick	= GetTickCount();

					ForcePaintFlag();
				}
			}
			break;
		}
	}
}

void CChildView::DoBeforePaintViewIntro()
{
	switch (GetScreenMode())
	{
	case SCREEN_NORMAL:
	case SCREEN_OUT:
		{
			switch (GetThreadMode())
			{
			case TMODE_LOWBATTERY:
				{
				}
				break;
			case TMODE_TMS:
				{
				}
				break;
			default:
				{
					SetThreadMode(ReversalExist() ? TMODE_REVERSAL : TMODE_READY);
				}
				break;
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

void CChildView::PaintViewIntro(CDCX* pDC)
{
	RECT	rc;

	switch (GetScreenMode())
	{
	case SCREEN_NORMAL:
	case SCREEN_OUT:
		{
			if (PaintRequired(PAINT_FULL_SCREEN))
			{
				PaintBackground(pDC);
				pDC->SelectObject(&m_fntHeaderLarge);
				pDC->SetTextColor(COLOR_RGB_WHITE);
				DrawPNGResource(RP_BOTTOM, pDC->GetSafeHdc());
				GetRectFromPNGResource(&rc, RP_BOTTOM);

				switch (GetThreadMode())
				{
				case TMODE_REVERSAL:
					{
						DrawPNGResource(RP_WAIT, pDC->GetSafeHdc());
						pDC->DrawText(LoadText(IDS_REVERSAL_MODE), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				case TMODE_LOWBATTERY:
					{
						DrawPNGResource(RP_WAIT, pDC->GetSafeHdc());
						pDC->DrawText(LoadText(IDS_LOW_BATTERY), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				case TMODE_TMS:
					{
						DrawPNGResource(RP_WAIT, pDC->GetSafeHdc());
						pDC->DrawText(LoadText(IDS_CHECKING_UPDATES), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				default:
					{
						DrawPNGResource(RP_SWIPE, pDC->GetSafeHdc());
						pDC->DrawText(LoadText(IDS_SWIPE_CARD), &rc, DT_CENTER | DT_VCENTER);
					}
					break;
				}
			}

			if (PaintRequired(PAINT_INTRO_TIME))
			{
				CString	strDate;
				CString	strTime;

				switch (m_Settings.langMode)
				{
				case LANGUAGE_FARSI:
					{
						CJalali	jalali;
						CString	strJalali;
						int		nYear;
						int		nMonth;
						int		nDay;

						jalali.GetJalaliDate(m_PaintFlags.stIntroTime, &nYear, &nMonth, &nDay);

#if defined(MODEL_CT360)
						strJalali.Format(L"%04d/%02d/%02d %02d:%02d", nYear, nMonth, nDay, m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);

						strTime	= FormattingFarsiDigit(strJalali, FALSE);
#elif defined(MODEL_MT360)
						strJalali.Format(L"%04d/%02d/%02d %02d:%02d", nYear, nMonth, nDay, m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);

						strTime	= FormattingFarsiDigit(strJalali, FALSE);
#elif defined(MODEL_MT760)
						strJalali.Format(L"%04d/%02d/%02d", nYear, nMonth, nDay);

						strDate	= FormattingFarsiDigit(strJalali, FALSE);

						strJalali.Format(L"%02d:%02d", m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);

						strTime	= FormattingFarsiDigit(strJalali, FALSE);
#endif
					}
					break;
				default:
					{
#if defined(MODEL_CT360)
						strTime.Format(L"%04d/%02d/%02d %02d:%02d", m_PaintFlags.stIntroTime.wYear, m_PaintFlags.stIntroTime.wMonth, m_PaintFlags.stIntroTime.wDay, m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);
#elif defined(MODEL_MT360)
						strTime.Format(L"%04d/%02d/%02d %02d:%02d", m_PaintFlags.stIntroTime.wYear, m_PaintFlags.stIntroTime.wMonth, m_PaintFlags.stIntroTime.wDay, m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);
#elif defined(MODEL_MT760)
						strDate.Format(L"%04d/%02d/%02d", m_PaintFlags.stIntroTime.wYear, m_PaintFlags.stIntroTime.wMonth, m_PaintFlags.stIntroTime.wDay);
						strTime.Format(L"%02d:%02d", m_PaintFlags.stIntroTime.wHour, m_PaintFlags.stIntroTime.wMinute);
#endif
					}
					break;
				}

				pDC->SetTextColor(COLOR_RGB_INTRO_DATE);

#if defined(MODEL_CT360)
				pDC->SelectObject(&m_fntIntroTime);
				GetRectFromPNGResource(&rc, RP_INTRO_TOP);
				pDC->FillSolidRect(&rc, COLOR_RGB_INTRO_TOP);
				pDC->DrawText(strTime, &rc, DT_CENTER | DT_VCENTER);
#elif defined(MODEL_MT360)
				pDC->SelectObject(&m_fntIntroTime);
				GetRectFromPNGResource(&rc, RP_INTRO_TOP);
				pDC->FillSolidRect(&rc, COLOR_RGB_INTRO_TOP);
				pDC->DrawText(strTime, &rc, DT_CENTER | DT_VCENTER);
#elif defined(MODEL_MT760)
				switch (m_Settings.langMode)
				{
				case LANGUAGE_FARSI:
					{
						pDC->SelectObject(&m_fntIntroDate);
						DrawPNGResource(RP_INTRO_DATE, pDC->GetSafeHdc());
						GetRectFromPNGResource(&rc, RP_INTRO_DATE);
						pDC->DrawText(strDate, &rc, DT_LEFT | DT_VCENTER);
						pDC->SelectObject(&m_fntIntroTime);
						DrawPNGResource(RP_INTRO_TIME, pDC->GetSafeHdc());
						GetRectFromPNGResource(&rc, RP_INTRO_TIME);
						pDC->DrawText(strTime, &rc, DT_LEFT | DT_VCENTER);
					}
					break;
				default:
					{
						pDC->SelectObject(&m_fntIntroDate);
						DrawPNGResource(RP_INTRO_DATE, pDC->GetSafeHdc());
						GetRectFromPNGResource(&rc, RP_INTRO_DATE);
						pDC->DrawText(strDate, &rc, DT_RIGHT | DT_VCENTER);
						pDC->SelectObject(&m_fntIntroTime);
						DrawPNGResource(RP_INTRO_TIME, pDC->GetSafeHdc());
						GetRectFromPNGResource(&rc, RP_INTRO_TIME);
						pDC->DrawText(strTime, &rc, DT_RIGHT | DT_VCENTER);
					}
					break;
				}
#endif
			}

#if SOFTWARE_KEYPAD
			PaintButton(RP_FNKEY[0], RP_FNKEY_PRESS[0], PAINT_BTN_1, pDC, LoadText(IDS_F1KEY));
			PaintButton(RP_FNKEY[1], RP_FNKEY_PRESS[1], PAINT_BTN_2, pDC, LoadText(IDS_F2KEY));
			PaintButton(RP_FNKEY[2], RP_FNKEY_PRESS[2], PAINT_BTN_3, pDC, LoadText(IDS_F3KEY));
			PaintButton(RP_FNKEY[3], RP_FNKEY_PRESS[3], PAINT_BTN_4, pDC, LoadText(IDS_F4KEY));
#endif
		}
		break;
	case SCREEN_GLANCE:
		{
			if (PaintRequired(PAINT_FULL_SCREEN))
			{
				PaintBackground(pDC, COLOR_RGB_BLACK);

				int	x	= Pseudorandom(0, GetSystemMetrics(SM_CXSCREEN) - SCREEN_SAVE_WIDTH);
				int	y	= Pseudorandom(0, GetSystemMetrics(SM_CYSCREEN) - SCREEN_SAVE_HEIGHT);

				::DrawPNGResource(IDB_GLANCE, x, y, SCREEN_SAVE_WIDTH, SCREEN_SAVE_HEIGHT, pDC->GetSafeHdc());
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewIntro(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SYSTEMTIME	st;

	SetBrightness(m_Settings.nBrightness);

	if (m_nInfo[INFO_RESET_F3_PASSWORD])
	{
		UINT	uResetF3Seq[LENGTH_RESET_F3_SEQUENCE]	= { 0x38, 0x35, 0x36, 0x39 };
		UINT	i;

		if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
		{
			m_uResetF3Seq[m_uResetF3SeqPos] = nChar;
			m_uResetF3SeqPos++;

			for (i = 0; i < m_uResetF3SeqPos; i++)
			{
				if (m_uResetF3Seq[i] != uResetF3Seq[i])
				{
					m_uResetF3SeqPos				= 0;
					m_uResetF3Steps					= 0;
					m_nInfo[INFO_RESET_F3_PASSWORD]	= FALSE;
					break;
				}
			}

			if (i >= LENGTH_RESET_F3_SEQUENCE)
			{
				m_Settings.nF3Invalid	= 0;
				
				memset(&m_Settings.stF3Open, 0, sizeof(SYSTEMTIME));
				SaveSettings(&m_Settings);
			}
		}
		else
		{
			m_uResetF3Steps					= 0;
			m_nInfo[INFO_RESET_F3_PASSWORD]	= FALSE;
		}
	}

	if (!m_nInfo[INFO_RESET_F3_PASSWORD])
	{
		switch (GetFunctionKey(nChar))
		{
		case FNK_F1:
			{
				CloseMSR();
				InitializeMenu(&m_menuInfoF1, MENU_F1, IDS_NOSTRING, IDS_SELL_CHARGE, IDS_FOOD_SECURITY);
				SetViewMode(VIEW_MENU_F1);
				SetTransactionMode(TRANSACTION_F1);
				ForcePaintFlag();
			}
			break;
		case FNK_F2:
			{
				if (m_Settings.nF2Invalid >= INVALID_PASSWORD_COUNT)
				{
					GetLocalTime(&st);

					if (CompareSystemTime(&st, &m_Settings.stF2Open) > 0)
					{
						m_Settings.nF2Invalid	= 0;
						
						memset(&m_Settings.stF2Open, 0, sizeof(SYSTEMTIME));
						SaveSettings(&m_Settings);
					}
					else
					{
						SetTransactionMode(TRANSACTION_NONE);
						ShowMessage(ERR_INVALID_PASSWORD_OF);
					}
				}

				if (m_Settings.nF2Invalid < INVALID_PASSWORD_COUNT)
				{
					CloseMSR();
					SetViewMode(VIEW_PASSWORD);
					SetTransactionMode(TRANSACTION_F2);
					ForcePaintFlag();
				}
			}
			break;
		case FNK_F3:
			{
				if (m_Settings.nF3Invalid >= INVALID_PASSWORD_COUNT)
				{
					GetLocalTime(&st);

					if (CompareSystemTime(&st, &m_Settings.stF3Open) > 0)
					{
						m_Settings.nF3Invalid	= 0;
						
						memset(&m_Settings.stF3Open, 0, sizeof(SYSTEMTIME));
						SaveSettings(&m_Settings);
					}
					else
					{
						SetTransactionMode(TRANSACTION_NONE);
						ShowMessage(ERR_INVALID_PASSWORD_OF);
					}
				}

				if (m_Settings.nF3Invalid < INVALID_PASSWORD_COUNT)
				{
					CloseMSR();
					SetViewMode(VIEW_PASSWORD);
					SetTransactionMode(TRANSACTION_F3);
					ForcePaintFlag();
				}
			}
			break;
		case FNK_F4:
			{
				m_uResetF3SeqPos				= 0;
				m_uResetF3Steps					= 0;
				m_nInfo[INFO_RESET_F3_PASSWORD]	= TRUE;
			}
			break;
		}
	}

	switch (GetHardwareKey(nChar))
	{
	case HWK_ESCAPE:
	case HWK_HOME:
	case HWK_BACK:
		{
			Restart();
		}
		break;
	}
}

void CChildView::OnLButtonDownViewIntro(UINT nFlags, CPoint point)
{
#if SOFTWARE_KEYPAD
	if (m_nInfo[INFO_RESET_F3_PASSWORD])
	{
	}
	else
	{
		OnLButtonDownButton(RP_FNKEY[0], point, PAINT_BTN_1);
		OnLButtonDownButton(RP_FNKEY[1], point, PAINT_BTN_2);
		OnLButtonDownButton(RP_FNKEY[2], point, PAINT_BTN_3);
		OnLButtonDownButton(RP_FNKEY[3], point, PAINT_BTN_4);
	}
#endif
}

void CChildView::OnLButtonUpViewIntro(UINT nFlags, CPoint point)
{
#if SOFTWARE_KEYPAD
	if (m_nInfo[INFO_RESET_F3_PASSWORD])
	{
		RECT	rc[4];
		int		nWidth	= GetSystemMetrics(SM_CXSCREEN) / 3;
		int		nHeight	= GetSystemMetrics(SM_CYSCREEN) / 3;

		rc[0].left		= nWidth;
		rc[0].right		= nWidth * 2;
		rc[0].top		= GetSystemMetrics(SM_CYSCREEN) - nHeight;
		rc[0].bottom	= GetSystemMetrics(SM_CYSCREEN);

		rc[1].left		= nWidth;
		rc[1].right		= nWidth * 2;
		rc[1].top		= nHeight;
		rc[1].bottom	= nHeight * 2;

		rc[2].left		= GetSystemMetrics(SM_CXSCREEN) - nWidth;
		rc[2].right		= GetSystemMetrics(SM_CXSCREEN);
		rc[2].top		= nHeight;
		rc[2].bottom	= nHeight * 2;

		rc[3].left		= GetSystemMetrics(SM_CXSCREEN) - nWidth;
		rc[3].right		= GetSystemMetrics(SM_CXSCREEN);
		rc[3].top		= GetSystemMetrics(SM_CYSCREEN) - nHeight;
		rc[3].bottom	= GetSystemMetrics(SM_CYSCREEN);

		if (PtInRect(&rc[m_uResetF3Steps], point))
		{
			++m_uResetF3Steps;

			if (m_uResetF3Steps >= LENGTH_RESET_F3_STEPS)
			{
				m_Settings.nF3Invalid	= 0;
				
				memset(&m_Settings.stF3Open, 0, sizeof(SYSTEMTIME));
				SaveSettings(&m_Settings);
			}
		}
		else
		{
			m_uResetF3Steps					= 0;
			m_nInfo[INFO_RESET_F3_PASSWORD]	= FALSE;
		}
	}
	else
	{
		OnLButtonUpButton(RP_FNKEY[0], point, VK_F1);
		OnLButtonUpButton(RP_FNKEY[1], point, VK_F2);
		OnLButtonUpButton(RP_FNKEY[2], point, VK_F3);
		OnLButtonUpButton(RP_FNKEY[3], point, VK_F4);
	}
#endif
}
