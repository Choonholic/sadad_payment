// ViewCommon.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

BOOL CChildView::SetMessage(UINT uID, MESSAGE_TYPE messageType, BOOL bPrint)
{
	if ((uID != 0) && (messageType != MESSAGE_NONE) && (!m_nInfo[INFO_MESSAGE_ID]))
	{
		m_nInfo[INFO_MESSAGE_ID]	= uID;
		m_nInfo[INFO_MESSAGE_TYPE]	= messageType;
		m_nInfo[INFO_MESSAGE_PRINT]	= bPrint;

		return TRUE;
	}

	return FALSE;
}

void CChildView::ResetMessage()
{
	m_nInfo[INFO_MESSAGE_ID]	= 0;
	m_nInfo[INFO_MESSAGE_TYPE]	= MESSAGE_NONE;
	m_nInfo[INFO_MESSAGE_PRINT]	= FALSE;
}

void CChildView::ShowMessage(UINT uID, MESSAGE_TYPE messageType, BOOL bPrint)
{
	if (SetMessage(uID, messageType, bPrint))
	{
		if (bPrint)
		{
			SetViewMode(VIEW_PRINT);
		}
		else
		{
			SetViewMode(VIEW_MESSAGE);

			m_dwTimeoutMessageTick	= GetTickCount() + TIMEOUT_MESSAGE;
		}

		HandleMessage();
		ForcePaintFlag();
	}
}

void CChildView::ShowMessage()
{
	if (m_nInfo[INFO_MESSAGE_ID])
	{
		if (m_nInfo[INFO_MESSAGE_PRINT])
		{
			SetViewMode(VIEW_PRINT);
		}
		else
		{
			SetViewMode(VIEW_MESSAGE);

			m_dwTimeoutMessageTick	= GetTickCount() + TIMEOUT_MESSAGE;
		}

		HandleMessage();
		ForcePaintFlag();
	}
}

void CChildView::UpdateViewMessage()
{
	if (GetViewMode() == VIEW_MESSAGE)
	{
		if ((m_nInfo[INFO_MESSAGE_ID] == 0) || (GetTickCount() > m_dwTimeoutMessageTick))
		{
			m_dwTimeoutMessageTick	= 0;

			SetViewByMessage();
		}
	}
}

void CChildView::UpdateViewDialUp()
{
	if (GetViewMode() == VIEW_DIALUP)
	{
		switch (GetThreadStatus())
		{
		case TSTATUS_READY:
			{
				if ((!m_bThreadConnect) && (!m_bThreadDialup))
				{
					Restart();
				}
			}
			break;
		case TSTATUS_ERROR:
			{
				SetTransactionMode(TRANSACTION_NONE);
				ShowMessage();
			}
			break;
		default:
			{
				if (m_nInfo[INFO_DIALUP_STATUS] != GetThreadStatus())
				{
					m_nInfo[INFO_DIALUP_STATUS]	= GetThreadStatus();

					ForcePaintFlag(PAINT_FIELDS);
				}
			}
			break;
		}
	}
	else
	{
		m_nInfo[INFO_DIALUP_STATUS]	= TSTATUS_READY;
	}
}

void CChildView::DoEnterViewDialUp()
{
}

void CChildView::DoExitViewDialUp()
{
}

void CChildView::UpdateViewResult()
{
	if (GetViewMode() == VIEW_RESULT)
	{
		if (GetTickCount() > m_dwTimeoutResultTick) 
		{
			m_dwTimeoutResultTick	= 0;

			Restart();
		}
	}
}

void CChildView::UpdateViewPIN()
{
	if (GetViewMode() == VIEW_PIN)
	{
		if (GetTickCount() > m_dwTimeoutPINTick)
		{
			m_dwTimeoutPINTick	= 0;

			Restart();
		}
	}
}

void CChildView::HandleMessage()
{
	switch (m_nInfo[INFO_MESSAGE_ID])
	{
	case ERR_INVALID_PASSWORD:
	case ERR_INVALID_BILLID:
	case ERR_INVALID_PAYMENTID:
	case ERR_INTRO_NO_PAPER:
	case ERR_PRINT_NO_PAPER:
	case ERR_PRINT_OVERHEAT:
	case ERR_PRINT_LOW_VOLT:
	case ERR_PRINT_HIGH_VOLT:
	case ERR_PRINT_GENERIC:
		{
		}
		break;
	default:
		{
			switch (GetThreadStatus())
			{
			case TSTATUS_ERROR:
				{
					ClearThreadError();
					m_pComm->Disconnect();
					SetNextReversalTime();
					SetNextAdviceTime();

					m_bThreadDisconnect	= FALSE;
					m_bThreadDialup		= FALSE;
				}
				break;
			}
		}
		break;
	}
}

void CChildView::SetViewByMessage()
{
	switch (m_nInfo[INFO_MESSAGE_ID])
	{
	case ERR_INVALID_PASSWORD:
		{
			switch (GetTransactionMode())
			{
			case TRANSACTION_CHANGE_PASSWORD:
				{
					ResetMessage();
					SetViewMode(VIEW_PASSWORD);
					ForcePaintFlag();
				}
				break;
			default:
				{
					Restart();
				}
				break;
			}
		}
		break;
	case ERR_INVALID_BILLID:
		{
			ResetMessage();
			SetViewMode(VIEW_BILL_BILLID);
			ForcePaintFlag();
		}
		break;
	case ERR_INVALID_PAYMENTID:
		{
			ResetMessage();
			SetViewMode(VIEW_BILL_PAYMENTID);
			ForcePaintFlag();
		}
		break;
	case ERR_INTRO_NO_PAPER:
		{
			Restart();
		}
		break;
	case ERR_PRINT_NO_PAPER:
	case ERR_PRINT_OVERHEAT:
	case ERR_PRINT_LOW_VOLT:
	case ERR_PRINT_HIGH_VOLT:
	case ERR_PRINT_GENERIC:
		{
			ResetMessage();
			SetViewMode(VIEW_PRINT_RETRY);
			ForcePaintFlag();
		}
		break;
	case ERR_NOT_IMPLEMENTED:
		{
			ResetMessage();
			SetViewMode(VIEW_MENU_F1);
			SetTransactionMode(TRANSACTION_F1);
			ForcePaintFlag();
		}
		break;
	default:
		{
			Restart();
		}
		break;
	}
}

void CChildView::PaintViewMessage(CDCX* pDC)
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

void CChildView::PaintViewPassword(CDCX* pDC)
{
	int		nInfo	= INFO_PASSWORD;

	if (GetTransactionMode() == TRANSACTION_CHANGE_PASSWORD)
	{
		nInfo	= INFO_CPW_NEW + m_nInfo[INFO_CHANGE_PASSWORD] - 1;
	}

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		UINT	nID;

		PaintBackground(pDC);
		
		switch (GetTransactionMode())
		{
		case TRANSACTION_CHANGE_PASSWORD:
			{
				nID	= ((nInfo == INFO_CPW_NEW) ? IDS_ENTER_NEW_PASSWORD : IDS_VERIFY_PASSWORD);
			}
			break;
		default:
			{
				nID	= IDS_ENTER_PASSWORD;
			}
			break;
		}

		PaintHeader(LoadText(nID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		DrawPNGResource(RP_PASSWORD_BOX, pDC->GetSafeHdc());

		for (int i = 0; i < m_sInfo[nInfo].GetLength(); i++)
		{
			DrawPNGResource(RP_PASSWORD_ASTERISK[i], pDC->GetSafeHdc());
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewPIN(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_ENTER_PIN), pDC);
		PaintHeaderDesc(LoadText(IDS_CARD_HOLDER_ONLY), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		DrawPNGResource(RP_PIN_BOX, pDC->GetSafeHdc());

		for (int i = 0; i < m_sInfo[INFO_PIN].GetLength(); i++)
		{
			DrawPNGResource(RP_PIN_ASTERISK[i], pDC->GetSafeHdc());
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewDialUp(CDCX* pDC)
{
	UINT	nID;

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_DIALUP), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		switch (GetThreadStatus())
		{
		case TSTATUS_DIALING:
		case TSTATUS_WAITING:
		case TSTATUS_RETRY:
			{
				nID	= IDS_DIALING;
			}
			break;
		case TSTATUS_CONNECTING:
			{
				nID	= IDS_CONNECTING;
			}
			break;
		case TSTATUS_CONNECTED:
			{
				nID	= IDS_CONNECTED;
			}
			break;
		case TSTATUS_SENDING:
			{
				nID	= IDS_SENDING;
			}
			break;
		case TSTATUS_RECEIVING:
			{
				nID	= IDS_RECEIVING;
			}
			break;
		default:
			{
				nID	= IDS_PROCESSING;
			}
			break;
		}

		PaintMessage(LoadText(nID), RP_MESSAGE, pDC);
	}
}

void CChildView::PaintViewResult(CDCX* pDC)
{
	int		nID;
	RECT	rc;

	PaintBackground(pDC);
	pDC->SelectObject(&m_fntHeaderLarge);
	pDC->SetTextColor(COLOR_RGB_WHITE);

	if (!wcscmp(m_TransInfo.szResponseCode, L"00"))
	{
		DrawPNGResource(RP_SUCCESSFUL, pDC->GetSafeHdc());
		GetRectFromPNGResource(&rc, RP_RESULT);
		pDC->FillSolidRect(&rc, COLOR_RGB_BLUE);

		nID	= IDS_SUCCESSFUL;
	}
	else
	{
		DrawPNGResource(RP_UNSUCCESSFUL, pDC->GetSafeHdc());
		GetRectFromPNGResource(&rc, RP_RESULT);
		pDC->FillSolidRect(&rc, COLOR_RGB_RED);

		nID	= IDS_UNSUCCESSFUL;
	}

	GetRectFromPNGResource(&rc, RP_RESULT_TEXT);

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			pDC->DrawText(LoadText(nID), &rc, DT_RIGHT | DT_TOP | DT_WORDBREAK);
		}
		break;
	default:
		{
			pDC->DrawText(LoadText(nID), &rc, DT_LEFT | DT_TOP | DT_WORDBREAK);
		}
		break;
	}
}

void CChildView::OnKeyDownViewSecure(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_PASSWORD:
		{
			int	nInfo	= INFO_PASSWORD;

			if (GetTransactionMode() == TRANSACTION_CHANGE_PASSWORD)
			{
				nInfo	= INFO_CPW_NEW + m_nInfo[INFO_CHANGE_PASSWORD] - 1;
			}

			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					if (m_sInfo[nInfo].GetLength() >= LENGTH_PASSWORD_MIN)
					{
						switch (nInfo)
						{
						case INFO_PASSWORD:
							{
								if (ValidatePassword())
								{
									switch (GetTransactionMode())
									{
									case TRANSACTION_F2:
										{
											if (ReversalExist())
											{
												InitializeMenu(&m_menuInfoF2, MENU_F2, IDS_NOSTRING, IDS_CHANGE_PASSWORD, IDS_CHANGE_PASSWORD);
											}
											else
											{
												InitializeMenu(&m_menuInfoF2, MENU_F2, IDS_NOSTRING, IDS_CHANGE_PASSWORD, IDS_SERVICE_REQUEST);
											}

											m_Settings.nF2Invalid	= 0;
											
											SetViewMode(VIEW_MENU_F2);
											SetTransactionMode(TRANSACTION_F2);
											ForcePaintFlag();

											memset(&m_Settings.stF2Open, 0, sizeof(SYSTEMTIME));
											SaveSettings(&m_Settings);
										}
										break;
									case TRANSACTION_F3:
										{
											InitializeMenu(&m_menuInfoF3, MENU_F3, IDS_NOSTRING, IDS_CHANGE_CITY_CODE, IDS_VERSION);

											m_Settings.nF3Invalid	= 0;

											SetViewMode(VIEW_MENU_F3);
											SetTransactionMode(TRANSACTION_F3);
											ForcePaintFlag();

											memset(&m_Settings.stF3Open, 0, sizeof(SYSTEMTIME));
											SaveSettings(&m_Settings);
										}
										break;
									}
								}
								else
								{
									switch (GetTransactionMode())
									{
									case TRANSACTION_F2:
										{
											m_Settings.nF2Invalid++;

											if (m_Settings.nF2Invalid >= INVALID_PASSWORD_COUNT)
											{
												GetLocalTime(&m_Settings.stF2Open);
												AddSystemTime(&m_Settings.stF2Open, OD_100NS);
											}
										}
										break;
									case TRANSACTION_F3:
										{
											m_Settings.nF3Invalid++;

											if (m_Settings.nF3Invalid >= INVALID_PASSWORD_COUNT)
											{
												GetLocalTime(&m_Settings.stF3Open);
												AddSystemTime(&m_Settings.stF3Open, OD_100NS);
											}
										}
										break;
									}

									SaveSettings(&m_Settings);
									ShowMessage(ERR_INVALID_PASSWORD);
								}
							}
							break;
						case INFO_CPW_NEW:
							{
								m_nInfo[INFO_CHANGE_PASSWORD]	= CPW_CONFIRM;

								m_sInfo[INFO_CPW_CONFIRM].Empty();

								ForcePaintFlag();
							}
							break;
						case INFO_CPW_CONFIRM:
							{
								if (!m_sInfo[INFO_CPW_NEW].Compare(m_sInfo[INFO_CPW_CONFIRM]))
								{
									for (int i = 0; i < LENGTH_PASSWORD; i++)
									{
										if (i < m_sInfo[INFO_CPW_NEW].GetLength())
										{
											m_Settings.cPassword[i]	= (BYTE)(m_sInfo[INFO_CPW_NEW].GetAt(i) & 0xFF);
										}
										else
										{
											m_Settings.cPassword[i]	= '*';
										}
									}

									SaveSettings(&m_Settings);
									Restart();
								}
								else
								{
									ShowMessage(ERR_INVALID_PASSWORD);
								}
							}
							break;
						}
					}
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					switch (nInfo)
					{
					case INFO_PASSWORD:
						{
							Restart();
						}
						break;
					case INFO_CPW_NEW:
					case INFO_CPW_CONFIRM:
						{
							SetViewMode(VIEW_MENU_F2);
							SetTransactionMode(TRANSACTION_F2);
							ForcePaintFlag();
						}
						break;
					}
				}
				break;
			case HWK_BACK:
				{
					if (m_sInfo[nInfo].GetLength() > 0)
					{
						m_sInfo[nInfo].Delete(m_sInfo[nInfo].GetLength() - 1);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
					{
						if (m_sInfo[nInfo].GetLength() < LENGTH_PASSWORD)
						{
							m_sInfo[nInfo].AppendChar(nChar);
							ForcePaintFlag(PAINT_FIELDS);
						}
					}
				}
				break;
			}
		}
		break;
	case VIEW_PIN:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_RETURN:
				{
					if (m_sInfo[INFO_PIN].GetLength() == LENGTH_PIN)
					{
						switch (GetTransactionMode())
						{
						case TRANSACTION_KEY_INJECTION:
							{
								m_tKeyInjection	= KEY_INJECTION_READY;	

								SetViewMode(VIEW_KEY_INJECTION);
								ForcePaintFlag();
							}
							break;
						default:
							{
								if (GetThreadStatus() != TSTATUS_ERROR)
								{
									m_nInfo[INFO_PRINT_STATUS]	= FALSE;

									switch (GetThreadStatus())
									{
									case TSTATUS_DIALING:
									case TSTATUS_WAITING:
									case TSTATUS_CONNECTING:
										{
											m_pPrinter->PrintReceiptHeader(&m_Parameters);

											m_nInfo[INFO_PRINT_STATUS]	= TRUE;
										}
										break;
									}

									RequestTransaction();
									SetViewMode(VIEW_DIALUP);
									ForcePaintFlag();
								}
								else
								{
									ShowMessage();
								}
							}
							break;
						}
					}
				}
				break;
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					switch (GetTransactionMode())
					{
					case TRANSACTION_KEY_INJECTION:
						{
							SetViewMode(VIEW_MENU_F3);
							SetTransactionMode(TRANSACTION_F3);
							ForcePaintFlag();
						}
						break;
					default:
						{
							Restart();
						}
						break;
					}
				}
				break;
			case HWK_BACK:
				{
					if (m_sInfo[INFO_PIN].GetLength() > 0)
					{
						m_sInfo[INFO_PIN].Delete(m_sInfo[INFO_PIN].GetLength() - 1);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
				break;
			default:
				{
					if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
					{
						if (m_sInfo[INFO_PIN].GetLength() < LENGTH_PIN)
						{
							m_sInfo[INFO_PIN].AppendChar(nChar);
							ForcePaintFlag(PAINT_FIELDS);
						}
					}
				}
				break;
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewCommon(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetViewMode())
	{
	case VIEW_MESSAGE:
		{
			SetViewByMessage();
		}
		break;
	case VIEW_DIALUP:
		{
			switch (GetHardwareKey(nChar))
			{
			case HWK_ESCAPE:
			case HWK_HOME:
				{
					RequestDisconnect();
				}
				break;
			}
		}
		break;
	case VIEW_RESULT:
		{
			Restart();
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::OnLButtonDownViewSecure(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSecure(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewCommon(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_MESSAGE:
		{
		}
		break;
	case VIEW_DIALUP:
		{
		}
		break;
	case VIEW_RESULT:
		{
		}
		break;
	}
}

void CChildView::OnLButtonUpViewCommon(UINT nFlags, CPoint point)
{
	switch (GetViewMode())
	{
	case VIEW_MESSAGE:
		{
			SetViewByMessage();
		}
		break;
	case VIEW_DIALUP:
		{
		}
		break;
	case VIEW_RESULT:
		{
			Restart();
		}
		break;
	}

	ForcePaintFlag();
}
