// ViewMenuF3.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "StringRes.h"
#include "PNGRes.h"
#include "Device.h"
#include "Jalali.h"
#include "Utilities.h"
#include "ICCardLoader.h"
#include "PciPad_t.h"
#include "TMS.h"
#include "tdes_dec.h"

void CChildView::UpdateViewSendParameter()
{
	if ((GetViewMode() == VIEW_SEND_PARAMETER) && (m_dwTimeoutSendParameterTick != 0))
	{
		m_dwTimeoutTransactionTick	= GetTickCount();

		switch (GetSendParameter())
		{
		case PARAMETER_CONFIG:
			{
				BOOL	bSetTime	= SetPOSDateTime();

				m_dwTimeoutSendParameterTick	= 0;

				LoadConfigXML(&m_Parameters);
				SaveParameters(&m_Parameters);
				FirstSetupParameters(&m_Parameters, FALSE, !bSetTime);
				SetSendParameter(PARAMETER_NOT_READY);
				ResetDevice();
			}
			break;
		case PARAMETER_MASTER_KEY:
			{
				m_dwTimeoutSendParameterTick	= 0;

				if (LoadMasterKeyParameter())
				{
					DWORD	dwResult;
					DWORD	dwBankID	= 0x11111111;
					BYTE	KCV[3];

					dwResult				= BBSecure_ClearKeys(0);

					LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

					dwResult				= BBSecure_ClearKeys(dwBankID);

					LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

					dwResult				= BBSecure_SendPaymentKey_simple(KCV, g_cMasterKey, 0, TDES_MASTER_KEY, 16, dwBankID, ECB, WRITE_DIRECTLY, 0, FALSE, 0, NULL);

					LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

					m_Settings.bMasterKey	= TRUE;
					m_Settings.logonStatus	= LOGON_NONE;

					SaveSettings(&m_Settings);
				};

				SetSendParameter(PARAMETER_NOT_READY);
				ResetDevice();
			}
			break;
		default:
			{
				if ((GetTickCount() - m_dwTimeoutSendParameterTick) > TIMEOUT_PARAMETER)
				{
					m_dwTimeoutSendParameterTick	= 0;

					SetSendParameter(PARAMETER_NOT_READY);
					Restart();
				}
			}
			break;
		}
	}
}

void CChildView::UpdateViewKeyInjection()
{
	if (GetViewMode() == VIEW_KEY_INJECTION)
	{
		m_dwTimeoutTransactionTick	= GetTickCount();
	}
}

void CChildView::InitializeMenuPhonebook()
{
	InitializeMenuBase(&m_menuInfoPhonebook, MENU_PHONEBOOK, IDS_PHONEBOOK, MAX_PHONEBOOK, TRUE, VIEW_MENU_F3, TRANSACTION_F3);

	for (int i = 0; i < m_menuInfoPhonebook.nMaxItems; i++)
	{
		SetMenuItem(&m_menuInfoPhonebook, i, FormattingArabicDigit(m_Parameters.szPhonebook[i], FALSE), FormattingFarsiDigit(m_Parameters.szPhonebook[i], FALSE));
	}
}

void CChildView::InitializeMenuLanguage()
{
	InitializeMenuBase(&m_menuInfoLanguage, MENU_LANGUAGE, IDS_LANGUAGE, m_Parameters.nMaxLanguage, TRUE, VIEW_MENU_F3, TRANSACTION_F3);

	for (int i = 0; i < m_menuInfoLanguage.nMaxItems; i++)
	{
		SetMenuItem(&m_menuInfoLanguage, i, m_Parameters.languageType[i]);
	}
}

void CChildView::InitializeMenuConnectionType()
{
	InitializeMenuBase(&m_menuInfoConnectionType, MENU_CONNECTION_TYPE, IDS_SETTINGS_CONNECTION_TYPE, m_Parameters.nMaxConnectionType, TRUE, VIEW_MENU_POS_SETTINGS, TRANSACTION_POS_SETTINGS);

	for (int i = 0; i < m_menuInfoConnectionType.nMaxItems; i++)
	{
		SetMenuItem(&m_menuInfoConnectionType, i, m_Parameters.connectionType[i].connectionName);
	}
}

void CChildView::SetSendParameter(DWORD dwStatus)
{
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwType			= REG_DWORD;
	DWORD	dwSize			= sizeof(DWORD);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PAYMENTAPP, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	RegSetValueEx(hKey, RV_RECEIVE_STATUS, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwStatus), sizeof(DWORD));
	RegCloseKey(hKey);
}

DWORD CChildView::GetSendParameter()
{
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwType			= REG_DWORD;
	DWORD	dwSize			= sizeof(DWORD);
	DWORD	dwValue			= PARAMETER_NOT_READY;

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PAYMENTAPP, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	if (RegQueryValueEx(hKey, RV_RECEIVE_STATUS, 0, &dwType, reinterpret_cast<LPBYTE>(&dwValue), &dwSize) != ERROR_SUCCESS)
	{
		dwValue	= PARAMETER_NOT_READY;

		RegSetValueEx(hKey, RV_RECEIVE_STATUS, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
	}

	RegCloseKey(hKey);

	return dwValue;
}

BOOL CChildView::SetPOSDateTime()
{
	HKEY		hKey;
	DWORD		dwDisposition;
	DWORD		dwType			= REG_DWORD;
	DWORD		dwSize			= sizeof(DWORD);
	DWORD		dwDate			= 0;
	DWORD		dwTime			= 0;
	DWORD		dwFlags			= 0;
	SYSTEMTIME	st;

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PAYMENTAPP, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	if (RegQueryValueEx(hKey, RV_DATE, 0, &dwType, reinterpret_cast<LPBYTE>(&dwDate), &dwSize) == ERROR_SUCCESS)
	{
		dwFlags		|= 0x01;
		st.wYear	= (WORD)(dwDate / 10000);
		dwDate		-= st.wYear * 10000;
		st.wMonth	= (WORD)(dwDate / 100);
		dwDate		-= st.wMonth * 100;
		st.wDay		= (WORD)(dwDate);

		RegDeleteValue(hKey, RV_DATE);
	}

	if (RegQueryValueEx(hKey, RV_TIME, 0, &dwType, reinterpret_cast<LPBYTE>(&dwTime), &dwSize) == ERROR_SUCCESS)
	{
		dwFlags		|= 0x02;
		st.wHour	= (WORD)(dwTime / 10000);
		dwTime		-= st.wHour * 10000;
		st.wMinute	= (WORD)(dwTime / 100);
		dwTime		-= st.wMinute * 100;
		st.wSecond	= (WORD)(dwTime);

		RegDeleteValue(hKey, RV_TIME);
	}

	RegCloseKey(hKey);

	if (dwFlags == 0x03)
	{
		SetLocalTime(&st);
		return TRUE;
	}

	return FALSE;
}

BOOL CChildView::LoadMasterKeyParameter()
{
	HANDLE	hFile;
	DWORD	dwBytes;
	BYTE	cReadKey[24];
	BYTE	cMasterKey[16];
	BYTE	cBaseKey[16]	= { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

	ZeroMemory(cReadKey, sizeof(cReadKey));

	hFile	= CreateFile(PATH_MASTERKEY, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadFile(hFile, cReadKey, sizeof(cReadKey), &dwBytes, NULL);
		CloseHandle(hFile);

		if (dwBytes != 16)
		{
			return FALSE;			
		}

		tdes_dec_ecb(cReadKey, cMasterKey, dwBytes, cBaseKey);
		memcpy(g_cMasterKey, cMasterKey, dwBytes);
		DeleteFile(PATH_MASTERKEY);
		return TRUE;
	}

	return FALSE;
}

void CChildView::ExecuteMenuF3()
{
	switch (m_menuInfoF3.nCurItem)
	{
	case 0:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;
			m_sInfo[INFO_CITY_CODE]		= m_Settings.szCityCode;

			SetViewMode(VIEW_CHANGE_CITY_CODE);
			SetTransactionMode(TRANSACTION_CHANGE_CITY_CODE);
		}
		break;
	case 1:
		{
			InitializeMenuPhonebook();
			SetViewMode(VIEW_MENU_PHONEBOOK);
			SetTransactionMode(TRANSACTION_PHONEBOOK);
		}
		break;
	case 2:
		{
			SYSTEMTIME	st;
			CJalali		Jalali;
			CString		strDate;

			GetLocalTime(&st);

			switch (m_Settings.langMode)
			{
			case LANGUAGE_FARSI:
				{
					int		nYear;
					int		nMonth;
					int		nDay;
					CString	strFarsi;

					Jalali.GetJalaliDate(st, &nYear, &nMonth, &nDay);
					m_sInfo[INFO_DATE].Format(L"%04d%02d%02d", nYear, nMonth, nDay);
				}
				break;
			default:
				{
					m_sInfo[INFO_DATE].Format(L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
				}
				break;
			}

			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			SetViewMode(VIEW_SET_DATE);
			SetTransactionMode(TRANSACTION_SET_DATE);
		}
		break;
	case 3:
		{
			SYSTEMTIME	st;
			CString		strTime;

			GetLocalTime(&st);
			
			m_sInfo[INFO_TIME].Format(L"%02d%02d", st.wHour, st.wMinute);

			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			SetViewMode(VIEW_SET_TIME);
			SetTransactionMode(TRANSACTION_SET_TIME);
		}
		break;
	case 4:
		{
			SetViewMode(VIEW_BRIGHTNESS);
			SetTransactionMode(TRANSACTION_BRIGHTNESS);
		}
		break;
	case 5:
		{
			InitializeMenuLanguage();

			m_menuInfoLanguage.nPrevItem	= m_Settings.langMode;
			m_menuInfoLanguage.nCurItem		= m_Settings.langMode;

			SetViewMode(VIEW_MENU_LANGUAGE);
			SetTransactionMode(TRANSACTION_LANGUAGE);
		}
		break;
	case 6:
		{
			SetViewMode(VIEW_SEND_PARAMETER);
			SetTransactionMode(TRANSACTION_SEND_PARAMETER);
			SetSendParameter(PARAMETER_READY);

			m_dwTimeoutSendParameterTick	= GetTickCount();
		}
		break;
	case 7:
		{
			SetViewMode(VIEW_PIN);
			SetTransactionMode(TRANSACTION_KEY_INJECTION);
			m_sInfo[INFO_PIN].Empty();

			m_dwTimeoutPINTick	= GetTickCount() + (m_Parameters.nTimeoutPIN * 1000);
		}
		break;
	case 8:
		{
			InitializeMenu(&m_menuInfoSettings, MENU_POS_SETTINGS, IDS_POS_SETTINGS, IDS_SETTINGS_NII, IDS_SETTINGS_SERVER_SETTINGS, TRUE, VIEW_MENU_F3, TRANSACTION_F3);
			SetViewMode(VIEW_MENU_POS_SETTINGS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 9:
		{
			if (!LoadTMSUpdateXML(&m_UpdateInfo))
			{
				LoadTMSConfigXML(&m_UpdateInfo);
			}

			InitializeMenu(&m_menuInfoTMSSettings, MENU_TMS_SETTINGS, IDS_TMS_SETTINGS, IDS_TMS_SETTINGS_STARTTIME, IDS_TMS_SETTINGS_WEEKDAYS, TRUE, VIEW_MENU_F3, TRANSACTION_F3);
			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	case 10:
		{
			if (WM_TMS_MESSAGE)
			{
				LOGFILE(L"%S[%d] - Download Application\r\n", __FUNCTION__, __LINE__);
				::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_COM_OFFLINE, 0);
				ExitApplication();
			}
		}
		break;
	case 11:
		{
			if (WM_TMS_MESSAGE)
			{
				LOGFILE(L"%S[%d] - Manual Online Update\r\n", __FUNCTION__, __LINE__);
				::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_COM_ONLINE, 0);
				ExitApplication();
			}
		}
		break;
	case 12:
		{
			BOOL	bPaper	= m_pPrinter->IsPaperLoaded();

			if (bPaper == TRUE)
			{
				m_pPrinter->PrintPOSInformation(&m_Parameters);
				Restart();
			}
			else
			{
				SetTransactionMode(TRANSACTION_NONE);
				ShowMessage(ERR_INTRO_NO_PAPER);
			}
		}
		break;
	case 13:
		{
			m_Settings.nF2Invalid	= 0;
			
			memset(&m_Settings.stF2Open, 0, sizeof(SYSTEMTIME));
			SaveSettings(&m_Settings);
		}
		break;
	case 14:
		{
			m_uExitSeqPos	= 0;
			m_uExitSteps	= 0;

			SetViewMode(VIEW_VERSION);
			SetTransactionMode(TRANSACTION_NONE);
		}
		break;
	}
}

void CChildView::DoCancelMenuTMSSettings()
{
	if (m_nInfo[INFO_TMS_UPDATE])
	{
		m_nInfo[INFO_TMS_UPDATE]	= 0;

		SaveTMSUpdateXML(&m_UpdateInfo);
		::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_REQ_CONFIG_REFRESH, 0);
	}
}

void CChildView::ExecuteMenuPhonebook()
{
	m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;
	m_sInfo[INFO_PHONEBOOK]		= m_Parameters.szPhonebook[m_menuInfoPhonebook.nCurItem];
	
	SetViewMode(VIEW_PHONEBOOK);
	SetTransactionMode(TRANSACTION_PHONEBOOK);
}

void CChildView::ExecuteMenuLanguage()
{
	switch (m_menuInfoLanguage.nCurItem)
	{
	case 0:
		{
			m_Settings.langMode	= LANGUAGE_ENGLISH;
		}
		break;
	case 1:
		{
			m_Settings.langMode	= LANGUAGE_FARSI;
		}
		break;
	}

	m_pPrinter->SetLanguageMode(m_Settings.langMode);
	SaveSettings(&m_Settings);
	SetViewMode(VIEW_MENU_F3);
	SetTransactionMode(TRANSACTION_F3);
	ForcePaintFlag();
}

void CChildView::ExecuteMenuSettings()
{
	switch (m_menuInfoSettings.nCurItem)
	{
	case 0:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;
			m_sInfo[INFO_SETTINGS]		= m_Parameters.szNII;

			SetViewMode(VIEW_SETTINGS_NII);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 1:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutConnection);

			SetViewMode(VIEW_SETTINGS_CONNECTION_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 2:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutConnected);

			SetViewMode(VIEW_SETTINGS_CONNECTED_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 3:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutPIN);

			SetViewMode(VIEW_SETTINGS_PIN_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 4:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutAdvice);

			SetViewMode(VIEW_SETTINGS_ADVICE_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 5:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutReversal);

			SetViewMode(VIEW_SETTINGS_REVERSAL_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 6:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_SETTINGS].Format(L"%d", m_Parameters.nTimeoutScreen);

			SetViewMode(VIEW_SETTINGS_SCREEN_TIMEOUT);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 7:
		{
			InitializeMenuConnectionType();

			for (int i = 0; i < (int)m_Parameters.nMaxConnectionType; i++)
			{
				if (m_Parameters.connectionType[i].nIndex == m_Parameters.nCurrentConnectionType)
				{
					m_menuInfoConnectionType.nPrevItem	= i;
					m_menuInfoConnectionType.nCurItem	= i;
					break;
				}
			}
			
			SetViewMode(VIEW_MENU_CONNECTION_TYPE);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	case 8:
		{
			IP2CS(m_sInfo[INFO_IPADDRESS], m_Parameters.szServerIP, wcslen(m_Parameters.szServerIP));

			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;
			m_sInfo[INFO_PORT]			= m_Parameters.szServerPort;
			
			SetViewMode(VIEW_SETTINGS_SERVER_IPADDRESS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
		}
		break;
	}
}

void CChildView::ExecuteMenuTMSSettings()
{
	switch (m_menuInfoTMSSettings.nCurItem)
	{
	case 0:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;
			
			m_sInfo[INFO_TMS_SETTINGS].Format(L"%02d%02d", m_UpdateInfo.wStartHour, m_UpdateInfo.wStartMinute);

			SetViewMode(VIEW_TMS_SETTINGS_STARTTIME);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	case 1:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_TMS_SETTINGS].Format(L"%02d%02d", m_UpdateInfo.wEndHour, m_UpdateInfo.wEndMinute);

			SetViewMode(VIEW_TMS_SETTINGS_ENDTIME);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	case 2:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_TMS_SETTINGS].Format(L"%02d%02d", m_UpdateInfo.wIntervalHour, m_UpdateInfo.wIntervalMinute);

			SetViewMode(VIEW_TMS_SETTINGS_INTERVAL);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	case 3:
		{
			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			m_sInfo[INFO_TMS_SETTINGS].Format(L"%d", m_UpdateInfo.wCheckCount);

			SetViewMode(VIEW_TMS_SETTINGS_COUNT);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	case 4:
		{
			m_nInfo[INFO_TMS_WEEKDAYS]	= ((m_UpdateInfo.wWeekdays == 0xFF) ? 0x7F : m_UpdateInfo.wWeekdays);

			SetViewMode(VIEW_TMS_SETTINGS_WEEKDAYS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
		}
		break;
	}
}

void CChildView::ExecuteMenuConnectionType()
{
	m_Parameters.nCurrentConnectionType	= m_Parameters.connectionType[m_menuInfoConnectionType.nCurItem].nIndex;

	SaveParameters(&m_Parameters);

	ResetConnection();
	SetViewMode(VIEW_MENU_POS_SETTINGS);
	SetTransactionMode(TRANSACTION_POS_SETTINGS);
	ForcePaintFlag();
}

void CChildView::PaintViewChangeCityCode(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_CHANGE_CITY_CODE), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_CITY_CODE_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_CITY_CODE].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_CITY_CODE_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_CITY_CODE].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewPhonebook(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_PHONEBOOK), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_PHONE_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < m_sInfo[INFO_PHONEBOOK].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_PHONE_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_PHONEBOOK].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSetDate(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_SET_DATE), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_DATE_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < m_sInfo[INFO_DATE].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_DATE_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_DATE].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSetTime(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_SET_TIME), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_TIME_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_TIME].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_TIME_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_TIME].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewBrightness(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_BRIGHTNESS), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		DrawPNGResource(RP_BRIGHTNESS_BOX, pDC->GetSafeHdc());

		for (int i = 0; i < GetBrightness(); i++)
		{
			DrawPNGResource(RP_BRIGHTNESS_FILL[i], pDC->GetSafeHdc());
		}
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSendParameter(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_SEND_PARAMETER), pDC);
		PaintMessage(LoadText(IDS_RECEIVING), RP_MESSAGE, pDC);
	}
}

void CChildView::PaintViewKeyInjection(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_KEY_INJECTION), pDC);
		PaintMessage(LoadText(IDS_PRESS_ENTER), RP_MESSAGE, pDC);
	}
}

void CChildView::PaintViewVersion(CDCX* pDC)
{
	if (PaintRequired())
	{
		CString	strVersion;

		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_VERSION), pDC);
		GetAppVersion(strVersion.GetBufferSetLength(64), 64);
		strVersion.ReleaseBuffer();
		PaintMessage(FormattingDigit(strVersion, FALSE, m_Settings.langMode), RP_MESSAGE, pDC);
	}
}

void CChildView::PaintViewSettings(CDCX* pDC)
{
	if (PaintRequired())
	{
		UINT	nID;

		PaintBackground(pDC);

		switch (GetViewMode())
		{
		case VIEW_SETTINGS_NII:
			{
				nID	= IDS_SETTINGS_NII;
			}
			break;
		case VIEW_SETTINGS_CONNECTION_TIMEOUT:
			{
				nID	= IDS_SETTINGS_CONNECTION_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_CONNECTED_TIMEOUT:
			{
				nID	= IDS_SETTINGS_CONNECTED_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_PIN_TIMEOUT:
			{
				nID	= IDS_SETTINGS_PIN_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_ADVICE_TIMEOUT:
			{
				nID	= IDS_SETTINGS_ADVICE_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_REVERSAL_TIMEOUT:
			{
				nID	= IDS_SETTINGS_REVERSAL_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_SCREEN_TIMEOUT:
			{
				nID	= IDS_SETTINGS_SCREEN_TIMEOUT;
			}
			break;
		case VIEW_SETTINGS_SERVER_IPADDRESS:
		case VIEW_SETTINGS_SERVER_PORT:
			{
				nID	= IDS_SETTINGS_SERVER_SETTINGS;
			}
			break;
		}

		PaintHeader(LoadText(nID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_SETTINGS_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_SETTINGS].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_SETTINGS_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_SETTINGS].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSettingsServerIPAddress(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_SETTINGS_SERVER_SETTINGS), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;
		CString	strText;

		DrawPNGResource(RP_IPADDRESS_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < m_sInfo[INFO_IPADDRESS].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_IPADDRESS_NUMBER[i]);

			if (m_sInfo[INFO_IPADDRESS].GetAt(i) == L' ')
			{
				strText	= L" ";
			}
			else
			{
				strText	= LoadText(IDS_NUM0 + (m_sInfo[INFO_IPADDRESS].GetAt(i) - L'0'));
			}

			pDC->DrawText(strText, &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewSettingsServerPort(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_SETTINGS_SERVER_SETTINGS), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_PORT_BOX, pDC->GetSafeHdc());

		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_PORT].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_PORT_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_PORT].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewTMSSettings(CDCX* pDC)
{
	if (PaintRequired())
	{
		UINT	nID;

		PaintBackground(pDC);

		switch (GetViewMode())
		{
		case VIEW_TMS_SETTINGS_STARTTIME:
			{
				nID	= IDS_TMS_SETTINGS_STARTTIME;
			}
			break;
		case VIEW_TMS_SETTINGS_ENDTIME:
			{
				nID	= IDS_TMS_SETTINGS_ENDTIME;
			}
			break;
		case VIEW_TMS_SETTINGS_INTERVAL:
			{
				nID	= IDS_TMS_SETTINGS_INTERVAL;
			}
			break;
		}

		PaintHeader(LoadText(nID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_TIME_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_TMS_SETTINGS].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_TIME_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_TMS_SETTINGS].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewTMSSettingsCount(CDCX* pDC)
{
	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_TMS_SETTINGS_COUNT), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_SETTINGS_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitLarge);

		for (int i = 0; i < m_sInfo[INFO_TMS_SETTINGS].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_SETTINGS_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_TMS_SETTINGS].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewTMSSettingsWeekdays(CDCX* pDC)
{
	RECT	rc;

	if (PaintRequired())
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_TMS_SETTINGS_WEEKDAYS), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		DrawPNGResource(RP_WEEKDAYS_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < MAX_WEEKDAYS; i++)
		{
			if ((m_nInfo[INFO_TMS_WEEKDAYS] & (0x01 << i)) == (0x01 << i))
			{
				DrawPNGResource(RP_WEEKDAYS_FILL[i], pDC->GetSafeHdc());
			}

			GetRectFromPNGResource(&rc, RP_WEEKDAYS_FILL[i]);
			pDC->DrawText(LoadText(IDS_SUNDAY + i), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN, RP_YES_BTN_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN, RP_NO_BTN_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewChangeCityCode(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			memset(m_Settings.szCityCode, 0, sizeof(WCHAR) * (LENGTH_CITY_CODE + 1));
			wcscpy(m_Settings.szCityCode, m_sInfo[INFO_CITY_CODE]);
			SaveSettings(&m_Settings);
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_CITY_CODE].GetLength() > 0)
			{
				m_sInfo[INFO_CITY_CODE].Delete(m_sInfo[INFO_CITY_CODE].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_CITY_CODE].Empty();
					m_sInfo[INFO_CITY_CODE].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_CITY_CODE].GetLength() < LENGTH_CITY_CODE)
					{
						m_sInfo[INFO_CITY_CODE].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewPhonebook(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			memset(m_Parameters.szPhonebook[m_menuInfoPhonebook.nCurItem], 0, sizeof(WCHAR) * (LENGTH_PHONE));
			wcscpy(m_Parameters.szPhonebook[m_menuInfoPhonebook.nCurItem], m_sInfo[INFO_PHONEBOOK]);
			SaveParameters(&m_Parameters);
			InitializeMenuPhonebook();
			SetViewMode(VIEW_MENU_PHONEBOOK);
			SetTransactionMode(TRANSACTION_PHONEBOOK);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_PHONEBOOK);
			SetTransactionMode(TRANSACTION_PHONEBOOK);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_PHONEBOOK].GetLength() > 0)
			{
				m_sInfo[INFO_PHONEBOOK].Delete(m_sInfo[INFO_PHONEBOOK].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_PHONEBOOK].Empty();
					m_sInfo[INFO_PHONEBOOK].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_PHONEBOOK].GetLength() < LENGTH_PHONE_BOX)
					{
						m_sInfo[INFO_PHONEBOOK].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewSetDate(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			if ((m_sInfo[INFO_DATE].GetLength() > 0) && (m_sInfo[INFO_DATE].GetLength() < LENGTH_DATE))
			{
				break;
			}

			if (m_sInfo[INFO_DATE].GetLength() == LENGTH_DATE)
			{
				SYSTEMTIME	st;

				GetLocalTime(&st);

				switch (m_Settings.langMode)
				{
				case LANGUAGE_FARSI:
					{
						CJalali	Jalali;
						int		nYear	= _wtoi(m_sInfo[INFO_DATE].Mid(0, 4));
						int		nMonth	= _wtoi(m_sInfo[INFO_DATE].Mid(4, 2));
						int		nDay	= _wtoi(m_sInfo[INFO_DATE].Mid(6, 2));
						int		nGYear	= 0;
						int		nGMonth	= 0;
						int		nGDay	= 0;

						Jalali.JalaliToGregorian(nYear, nMonth, nDay, &nGYear, &nGMonth, &nGDay);

						st.wYear	= (WORD)nGYear;
						st.wMonth	= (WORD)nGMonth;
						st.wDay		= (WORD)nGDay;
					}
					break;
				default:
					{
						st.wYear	= (WORD)_wtoi(m_sInfo[INFO_DATE].Mid(0, 4));
						st.wMonth	= (WORD)_wtoi(m_sInfo[INFO_DATE].Mid(4, 2));
						st.wDay		= (WORD)_wtoi(m_sInfo[INFO_DATE].Mid(6, 2));
					}
					break;
				}

				SetLocalTime(&st);
			}

			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_DATE].GetLength() > 0)
			{
				m_sInfo[INFO_DATE].Delete(m_sInfo[INFO_DATE].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_DATE].Empty();
					m_sInfo[INFO_DATE].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_DATE].GetLength() < LENGTH_DATE)
					{
						m_sInfo[INFO_DATE].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewSetTime(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			if ((m_sInfo[INFO_TIME].GetLength() > 0) && (m_sInfo[INFO_TIME].GetLength() < LENGTH_TIME))
			{
				break;
			}

			if (m_sInfo[INFO_TIME].GetLength() == LENGTH_TIME)
			{
				SYSTEMTIME	st;

				GetLocalTime(&st);

				st.wHour	= (WORD)_wtoi(m_sInfo[INFO_TIME].Mid(0, 2));
				st.wMinute	= (WORD)_wtoi(m_sInfo[INFO_TIME].Mid(2, 2));

				SetLocalTime(&st);
			}

			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				m_sInfo[INFO_TIME].Empty();
				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_TIME].GetLength() > 0)
			{
				m_sInfo[INFO_TIME].Delete(m_sInfo[INFO_TIME].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_TIME].Empty();
					m_sInfo[INFO_TIME].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_TIME].GetLength() < LENGTH_TIME)
					{
						m_sInfo[INFO_TIME].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewBrightness(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int	nBrightness	= GetBrightness();

	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			m_Settings.nBrightness	= nBrightness;

			SaveSettings(&m_Settings);
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetBrightness(m_Settings.nBrightness);
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	case HWK_UP:
		{
			if (nBrightness < MAX_BRIGHTNESS)
			{
				SetBrightness(nBrightness + 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	case HWK_DOWN:
		{
			if (nBrightness > 0)
			{
				SetBrightness(nBrightness - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewSendParameter(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			switch (GetSendParameter())
			{
			case PARAMETER_READY:
				{
					SetViewMode(VIEW_MENU_F3);
					SetTransactionMode(TRANSACTION_F3);
					ForcePaintFlag();
				}
				break;
			case PARAMETER_NOT_READY:
				{
					ResetDevice();
				}
				break;
			}
		}
		break;
	default:
		{
			switch (GetSendParameter())
			{
			case PARAMETER_NOT_READY:
				{
					ResetDevice();
				}
				break;
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewKeyInjection(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			DWORD	dwResult;
			DWORD	dwBankID		= 0x11111111;
			BYTE	cBuffer[5];
			int		nKeyLength;
			BYTE	cKeyBuffer[24];
			BYTE	cStatus[2];
			BYTE	KCV[3];

			WA2BA(cBuffer, 5, m_sInfo[INFO_PIN], LENGTH_PIN);

			if (BBICKEYINJECTOR_LoadKeyFromSecureICCard(cBuffer, 4, g_cPrivateKey, 128, g_cPublicKey, 128, 0, cKeyBuffer, &nKeyLength, 24, cStatus) == BBICKEYINJECTOR_NOERROR)
			{
				dwResult	= BBSecure_ClearKeys(0);

				LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

				dwResult	= BBSecure_ClearKeys(dwBankID);

				LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

				dwResult	= BBSecure_SendPaymentKey_simple(KCV, cKeyBuffer, 0, TDES_MASTER_KEY, 16, dwBankID, ECB, WRITE_DIRECTLY, 0, FALSE, 0, NULL);

				LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

				m_Settings.bMasterKey	= TRUE;
				m_Settings.logonStatus	= LOGON_NONE;
				m_tKeyInjection			= KEY_INJECTION_END;

				SaveSettings(&m_Settings);
				SetViewMode(VIEW_MENU_F3);
				SetTransactionMode(TRANSACTION_F3);
				ForcePaintFlag();

				m_tKeyInjection	= KEY_INJECTION_READY;
			}
			else
			{
				m_tKeyInjection	= KEY_INJECTION_ERROR;

				ShowMessage(ERR_KEY_CARD_ERROR);
			}
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_F3);
			SetTransactionMode(TRANSACTION_F3);
			ForcePaintFlag();
		}
		break;
	}

	ForcePaintFlag();
}

void CChildView::OnKeyDownViewVersion(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UINT	uExitSeq[LENGTH_EXIT_SEQUENCE]	= { 0x37, 0x37, 0x33, 0x30 };
	UINT	i;

	if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
	{
		m_uExitSeq[m_uExitSeqPos] = nChar;
		m_uExitSeqPos++;

		for (i = 0; i < m_uExitSeqPos; i++)
		{
			if (m_uExitSeq[i] != uExitSeq[i])
			{
				m_uExitSeqPos	= 0;
				m_uExitSteps	= 0;

				SetViewMode(VIEW_MENU_F3);
				SetTransactionMode(TRANSACTION_F3);
				ForcePaintFlag();
				break;
			}
		}

		if (i >= LENGTH_EXIT_SEQUENCE)
		{
			ExitApplication();
		}
	}
	else
	{
		m_uExitSeqPos	= 0;
		m_uExitSteps	= 0;

		SetViewMode(VIEW_MENU_F3);
		SetTransactionMode(TRANSACTION_F3);
		ForcePaintFlag();
	}
}

void CChildView::OnKeyDownViewSettings(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			switch (GetViewMode())
			{
			case VIEW_SETTINGS_NII:
				{
					swprintf(m_Parameters.szNII, L"%03d", _wtoi(m_sInfo[INFO_SETTINGS]));
					RequestDisconnect();
				}
				break;
			case VIEW_SETTINGS_CONNECTION_TIMEOUT:
				{
					m_Parameters.nTimeoutConnection	= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			case VIEW_SETTINGS_CONNECTED_TIMEOUT:
				{
					m_Parameters.nTimeoutConnected	= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			case VIEW_SETTINGS_PIN_TIMEOUT:
				{
					m_Parameters.nTimeoutPIN		= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			case VIEW_SETTINGS_ADVICE_TIMEOUT:
				{
					m_Parameters.nTimeoutAdvice		= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			case VIEW_SETTINGS_REVERSAL_TIMEOUT:
				{
					m_Parameters.nTimeoutReversal	= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			case VIEW_SETTINGS_SCREEN_TIMEOUT:
				{
					m_Parameters.nTimeoutScreen		= _wtoi(m_sInfo[INFO_SETTINGS]);
				}
				break;
			}

			SaveParameters(&m_Parameters);
			SetViewMode(VIEW_MENU_POS_SETTINGS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_POS_SETTINGS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_SETTINGS].GetLength() > 0)
			{
				m_sInfo[INFO_SETTINGS].Delete(m_sInfo[INFO_SETTINGS].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_SETTINGS].Empty();
					m_sInfo[INFO_SETTINGS].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_SETTINGS].GetLength() < LENGTH_SETTINGS_BOX)
					{
						m_sInfo[INFO_SETTINGS].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewSettingsServerIPAddress(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			WCHAR	szIP[16];

			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			if (CS2IP(szIP, 16, m_sInfo[INFO_IPADDRESS]))
			{
				wcscpy(m_Parameters.szServerIP, szIP);
				SaveParameters(&m_Parameters);
				SetViewMode(VIEW_SETTINGS_SERVER_PORT);
				SetTransactionMode(TRANSACTION_POS_SETTINGS);
			}
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_POS_SETTINGS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_IPADDRESS].GetLength() > 0)
			{
				m_sInfo[INFO_IPADDRESS].Delete(m_sInfo[INFO_IPADDRESS].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	case HWK_DOT:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				m_sInfo[INFO_IPADDRESS].Empty();
				ForcePaintFlag(PAINT_FIELDS);
			}

			switch (m_sInfo[INFO_IPADDRESS].GetLength())
			{
			case 0:
			case 3:
			case 6:
			case 9:
				{
				}
				break;
			case 1:
			case 4:
			case 7:
			case 10:
				{
					m_sInfo[INFO_IPADDRESS].AppendChar(L' ');
				}
			case 2:
			case 5:
			case 8:
			case 11:
				{
					m_sInfo[INFO_IPADDRESS].AppendChar(L' ');
					ForcePaintFlag(PAINT_FIELDS);
				}
				break;
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_IPADDRESS].Empty();
					m_sInfo[INFO_IPADDRESS].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_IPADDRESS].GetLength() < LENGTH_IPADDRESS)
					{
						m_sInfo[INFO_IPADDRESS].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewSettingsServerPort(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			wcscpy(m_Parameters.szServerPort, m_sInfo[INFO_PORT]);
			SaveParameters(&m_Parameters);
			EndTransactionThread();
			StartTransactionThread();
			SetViewMode(VIEW_MENU_POS_SETTINGS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_SETTINGS_SERVER_IPADDRESS);
			SetTransactionMode(TRANSACTION_POS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_PORT].GetLength() > 0)
			{
				m_sInfo[INFO_PORT].Delete(m_sInfo[INFO_PORT].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}

			ForcePaintFlag();
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_PORT].Empty();
					m_sInfo[INFO_PORT].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_PORT].GetLength() < LENGTH_PORT)
					{
						m_sInfo[INFO_PORT].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}

			ForcePaintFlag();
		}
		break;
	}
}

void CChildView::OnKeyDownViewTMSSettings(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			if ((m_sInfo[INFO_TMS_SETTINGS].GetLength() > 0) && (m_sInfo[INFO_TMS_SETTINGS].GetLength() < LENGTH_TIME))
			{
				break;
			}

			if (m_sInfo[INFO_TMS_SETTINGS].GetLength() == LENGTH_TIME)
			{
				switch (GetViewMode())
				{
				case VIEW_TMS_SETTINGS_STARTTIME:
					{
						m_UpdateInfo.wStartHour			= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(0, 2));
						m_UpdateInfo.wStartMinute		= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(2, 2));
					}
					break;
				case VIEW_TMS_SETTINGS_ENDTIME:
					{
						m_UpdateInfo.wEndHour			= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(0, 2));
						m_UpdateInfo.wEndMinute			= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(2, 2));
					}
					break;
				case VIEW_TMS_SETTINGS_INTERVAL:
					{
						m_UpdateInfo.wIntervalHour		= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(0, 2));
						m_UpdateInfo.wIntervalMinute	= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS].Mid(2, 2));
					}
					break;
				}
			}

			m_nInfo[INFO_TMS_UPDATE]	= 1;

			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				m_sInfo[INFO_TMS_SETTINGS].Empty();
				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_TMS_SETTINGS].GetLength() > 0)
			{
				m_sInfo[INFO_TMS_SETTINGS].Delete(m_sInfo[INFO_TMS_SETTINGS].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_TMS_SETTINGS].Empty();
					m_sInfo[INFO_TMS_SETTINGS].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_TMS_SETTINGS].GetLength() < LENGTH_TIME)
					{
						m_sInfo[INFO_TMS_SETTINGS].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewTMSSettingsCount(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			m_UpdateInfo.wCheckCount	= (WORD)_wtoi(m_sInfo[INFO_TMS_SETTINGS]);
			m_nInfo[INFO_TMS_UPDATE]	= 1;

			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_nInfo[INFO_DEFAULT_VALUE])
			{
				m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

				ForcePaintFlag(PAINT_FIELDS);
			}

			if (m_sInfo[INFO_TMS_SETTINGS].GetLength() > 0)
			{
				m_sInfo[INFO_TMS_SETTINGS].Delete(m_sInfo[INFO_TMS_SETTINGS].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[INFO_TMS_SETTINGS].Empty();
					m_sInfo[INFO_TMS_SETTINGS].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[INFO_TMS_SETTINGS].GetLength() < LENGTH_SETTINGS_BOX)
					{
						m_sInfo[INFO_TMS_SETTINGS].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewTMSSettingsWeekdays(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			m_UpdateInfo.wWeekdays		= (WORD)((m_nInfo[INFO_TMS_WEEKDAYS] == 0x7F) ? 0xFF : m_nInfo[INFO_TMS_WEEKDAYS]);
			m_nInfo[INFO_TMS_UPDATE]	= 1;

			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
	case HWK_BACK:
		{
			SetViewMode(VIEW_MENU_TMS_SETTINGS);
			SetTransactionMode(TRANSACTION_TMS_SETTINGS);
			ForcePaintFlag();
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_1) && (GetHardwareKey(nChar) <= HWK_7))
			{
				if (!(m_nInfo[INFO_TMS_WEEKDAYS] & (0x01 << (nChar - '1'))))
				{
					m_nInfo[INFO_TMS_WEEKDAYS] |= (0x01 << (nChar - '1'));
				}
				else
				{
					m_nInfo[INFO_TMS_WEEKDAYS] &= ~(0x01 << (nChar - '1'));
				}

				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	}
}

void CChildView::OnLButtonDownViewChangeCityCode(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewChangeCityCode(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewPhonebook(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewPhonebook(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewSetDate(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSetDate(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewSetTime(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSetTime(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewBrightness(UINT nFlags, CPoint point)
{
	RECT	rc;

	for (int i = 0; i < MAX_BRIGHTNESS; i++)
	{
		GetRectFromPNGResource(&rc, RP_BRIGHTNESS_FILL[i]);

		if (PtInRect(&rc, point))
		{
			SetBrightness(i + 1);
			ForcePaintFlag(PAINT_FIELDS);
			return;
		}
	}

	OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewBrightness(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewSendParameter(UINT nFlags, CPoint point)
{
}

void CChildView::OnLButtonUpViewSendParameter(UINT nFlags, CPoint point)
{
}

void CChildView::OnLButtonDownViewKeyInjection(UINT nFlags, CPoint point)
{
}

void CChildView::OnLButtonUpViewKeyInjection(UINT nFlags, CPoint point)
{
	PostMessage(WM_KEYDOWN, VK_RETURN);
}

void CChildView::OnLButtonDownViewSettings(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSettings(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewTMSSettings(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewTMSSettings(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewTMSSettingsCount(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewTMSSettingsCount(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewTMSSettingsWeekdays(UINT nFlags, CPoint point)
{
	RECT	rc;

	for (int i = 0; i < MAX_WEEKDAYS; i++)
	{
		GetRectFromPNGResource(&rc, RP_WEEKDAYS_FILL[i]);

		if (PtInRect(&rc, point))
		{
			if ((m_nInfo[INFO_TMS_WEEKDAYS] & (0x01 << i)) == (0x01 << i))
			{
				m_nInfo[INFO_TMS_WEEKDAYS] |= (0x01 << i);
			}
			else
			{
				m_nInfo[INFO_TMS_WEEKDAYS] &= ~(0x01 << i);
			}

			ForcePaintFlag(PAINT_FIELDS);
			return;
		}
	}

	OnLButtonDownButton(RP_YES_BTN, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewTMSSettingsWeekdays(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewVersion(UINT nFlags, CPoint point)
{
}

void CChildView::OnLButtonUpViewVersion(UINT nFlags, CPoint point)
{
#if SOFTWARE_KEYPAD
	RECT	rc[4];
	int		nWidth	= GetSystemMetrics(SM_CXSCREEN) / 3;
	int		nHeight	= GetSystemMetrics(SM_CYSCREEN) / 3;

	rc[0].left		= 0;
	rc[0].right		= nWidth;
	rc[0].top		= GetSystemMetrics(SM_CYSCREEN) - nHeight;
	rc[0].bottom	= GetSystemMetrics(SM_CYSCREEN);

	rc[1].left		= 0;
	rc[1].right		= nWidth;
	rc[1].top		= GetSystemMetrics(SM_CYSCREEN) - nHeight;
	rc[1].bottom	= GetSystemMetrics(SM_CYSCREEN);

	rc[2].left		= GetSystemMetrics(SM_CXSCREEN) - nWidth;
	rc[2].right		= GetSystemMetrics(SM_CXSCREEN);
	rc[2].top		= 0;
	rc[2].bottom	= nHeight;

	rc[3].left		= nWidth;
	rc[3].right		= nWidth * 2;
	rc[3].top		= GetSystemMetrics(SM_CYSCREEN) - nHeight;
	rc[3].bottom	= GetSystemMetrics(SM_CYSCREEN);

	if (PtInRect(&rc[m_uExitSteps], point))
	{
		++m_uExitSteps;

		if (m_uExitSteps >= LENGTH_EXIT_STEPS)
		{
			ExitApplication();
		}
	}
	else
	{
		m_uExitSeqPos	= 0;
		m_uExitSteps	= 0;

		SetViewMode(VIEW_MENU_F3);
		SetTransactionMode(TRANSACTION_F3);
		ForcePaintFlag();
	}
#endif
}

void CChildView::OnLButtonDownViewSettingsServerIPAddress(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSettingsServerIPAddress(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewSettingsServerPort(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewSettingsServerPort(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}
