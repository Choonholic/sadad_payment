// ChildView.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "bbappapi.h"
#include "ChildView.h"
#include "PciPad_t.h"
#include "Device.h"
#include "Utilities.h"
#include "PNGRes.h"
#include "TMS.h"
#include "RilApis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
	m_Settings.langMode					= LANGUAGE_FARSI;
	m_TransactionMode					= TRANSACTION_NONE;
	m_ViewMode							= VIEW_NONE;

	m_PaintFlags.langMode				= LANGUAGE_FARSI;
	m_PaintFlags.transactionMode		= TRANSACTION_NONE;
	m_PaintFlags.viewMode				= VIEW_NONE;
	m_PaintFlags.threadMode				= TMODE_READY;
	m_PaintFlags.threadStatus			= TSTATUS_READY;

	GetLocalTime(&m_PaintFlags.stIntroTime);

	m_AnnunciatorFlags.dwConnectStatus	= DATA_DISCONNECTED;
	m_AnnunciatorFlags.gprsStatus		= GPRS_NONE;
	m_AnnunciatorFlags.batteryStatus	= BATTERY_EMPTY;

	memset(m_AnnunciatorFlags.szOperator, 0, 32 * sizeof(WCHAR));
	GetLocalTime(&m_AnnunciatorFlags.stTime);

	m_AnnunciatorFlags.nCount			= THREAD_RETRY_INITIAL;
	m_AnnunciatorFlags.threadStatus		= TSTATUS_READY;

	m_dwPaintFlag						= PAINT_FULL_SCREEN;

	m_hMSR								= NULL;
	m_hTransactionThread				= NULL;
	m_nTransactionPriority				= THREAD_PRIORITY_NORMAL;
	m_hAnnunciatorThread				= NULL;
	m_nAnnunciatorPriority				= THREAD_PRIORITY_NORMAL;

	m_pPrinter							= NULL;
	m_pComm								= NULL;

	m_dwTimeoutGlanceTick				= 0;
	m_dwTimeoutTransactionTick			= 0;
	m_dwTimeoutScreenTick				= 0;
	m_dwTimeoutMessageTick				= 0;
	m_dwTimeoutResultTick				= 0;
	m_dwTimeoutPINTick					= 0;
	m_dwTimeoutSendParameterTick		= 0;

	ResetReversalTime();
	ResetAdviceTime();
	ResetLogonTime();

	m_dwAnnunciatorTick					= 0;
	m_dwStatusTick						= 0;

	m_dwSentTime						= 0;
	m_dwReceivedTime					= 0;

	m_tReversalStatus					= TS_INITIATED;
	m_tAdviceStatus						= TS_INITIATED;

	m_nSecureStatus						= 0;
	m_bMasterKey						= FALSE;

	m_tThreadFirstRun					= TS_INITIATED;
	m_bThreadForceClose					= FALSE;
	m_bThreadDialup						= FALSE;
	m_nThreadCount						= THREAD_RETRY_INITIAL;

	m_bLButtonDown						= FALSE;

	m_bTMSRequested						= FALSE;
	m_bTMSNeed							= FALSE;

	m_BatteryStatus						= BATTERY_EMPTY;

	m_screenMode						= SCREEN_GLANCE;

	SetThreadMode(TMODE_READY);
	SetThreadStatus(TSTATUS_READY);

	SetActionRequester(REQ_BEFORE);
	RequestConnect(REQ_BEFORE, FALSE);
	RequestTransaction(FALSE);
	RequestDisconnect(FALSE);
	RequestCancelAdvice(FALSE);
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CARDCLOSE, OnCardClose)
	ON_MESSAGE(WM_CARDRESTART, OnCardRestart)
END_MESSAGE_MAP()

void CChildView::StartUp()
{
	DWORD	dwResult	= 0;
	DWORD	dwCount		= 0;

	HideTaskBar();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	LoadSettings(&m_Settings);

	if (!LoadParameters(&m_Parameters))
	{
		if (!LoadConfigXML(&m_Parameters))
		{
			AfxMessageBox(IDS_CONFIG_NOT_FOUND, MB_ICONERROR | MB_OK);
			ResetDevice();
			return;
		}

		SaveParameters(&m_Parameters);
		FirstSetupParameters(&m_Parameters, TRUE);
	}

	AdjustPrinterPaperSensor();
	SetRestartFlag();

	SetScreenMode(SCREEN_NORMAL);

	do
	{
		dwResult	= BBSecure_Init();

		LOGFILE(L"%S[%d] - BBSecure_Init() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

		if (dwResult == 0)
		{
			break;
		}

		Sleep(MAXQ_WAIT_ACTIVE);
	} while (TRUE);

	do
	{
		dwResult	= BBSecure_ConnectionCheck(&m_nSecureStatus);
		
		LOGFILE(L"%S[%d] - BBSecure_ConnectionCheck() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

		if (dwResult == 0)
		{
			break;
		}

		dwCount++;

		if (dwCount >= MAXQ_ERROR_EXCEEDED)
		{
			AfxMessageBox(IDS_SECURE_ERROR, MB_ICONERROR | MB_OK);
			ResetDevice();
			return;
		}

		Sleep(MAXQ_WAIT_ACTIVE);
	} while (TRUE);

	m_database.Initialize();

	SetWindowPos(this, 0, 0, 0, 0, WS_EX_TOPMOST);

	if ((!m_nSecureStatus) && (!m_Settings.bMasterKey))
	{
		DWORD	dwBankID	= 0x11111111;
		BYTE	KCV[3];

		dwResult	= BBSecure_ClearKeys(0);

		LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

		dwResult	= BBSecure_ClearKeys(dwBankID);

		LOGFILE(L"%S[%d] - BBSecure_ClearKeys() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

		dwResult	= BBSecure_SendPaymentKey_simple(KCV, g_cMasterKey, 0, TDES_MASTER_KEY, 16, dwBankID, ECB, WRITE_DIRECTLY, 0, FALSE, 0, NULL);

		LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

		m_Settings.bMasterKey	= TRUE;
		m_Settings.logonStatus	= LOGON_NONE;

		SaveSettings(&m_Settings);
	}

	BYTE	cPIN[32];
	DWORD	dwPINSize	= 32;

	BYTE	KSN[10]		= { 0, };

	LOGFILE(L"%S[%d] - BBSecure_GetPINBlock_withData() [%ld]\r\n", __FUNCTION__, __LINE__, GetTickCount());

	dwResult	= BBSecure_GetPINBlock_withData(L"1111", 4, L"1234567812345678", 16, 0, 0, 0, L"11111111", 8, 60, cPIN, &dwPINSize, KSN);

	LOGFILE(L"%S[%d] - BBSecure_GetPINBlock_withData() [%ld] = %x\r\n", __FUNCTION__, __LINE__, GetTickCount(), dwResult);

	InitDC();
	InitFont();

	OpenMSR();

	m_pPrinter	= new MTCTPrinter(m_Settings.langMode);

	m_pPrinter->Open();

	m_pComm		= new CComm((CONNECTION_MODE)m_Parameters.nCurrentConnectionType);

	m_pComm->Open();

	ClearFunctionKeys();
	ClearHardwareKeys();

	BBKeyMapping_SetDefaultKey();
	BBKeyMapping_Init();

#if defined(MODEL_CT360)
	BBKeyMapping_SetKeyData(0, VK_LEFT, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(3, VK_RIGHT, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(16, VK_MULTIPLY, KEY_TYPE_ONETIME);

	SetFunctionKey(FNK_F1, VK_LEFT);
	SetFunctionKey(FNK_F2, VK_DOWN);
	SetFunctionKey(FNK_F3, VK_UP);
	SetFunctionKey(FNK_F4, VK_RIGHT);

	SetHardwareKey(HWK_LEFT, VK_LEFT);
	SetHardwareKey(HWK_DOWN, VK_DOWN);
	SetHardwareKey(HWK_UP, VK_UP);
	SetHardwareKey(HWK_RIGHT, VK_RIGHT);
	SetHardwareKey(HWK_ESCAPE, VK_ESCAPE);
	SetHardwareKey(HWK_BACK, VK_BACK);
	SetHardwareKey(HWK_RETURN, VK_RETURN);
	SetHardwareKey(HWK_CENTER, VK_RETURN);
	SetHardwareKey(HWK_F, VK_MULTIPLY);
	SetHardwareKey(HWK_DOT, VK_PERIOD);
	SetHardwareKey(HWK_0, '0');
	SetHardwareKey(HWK_1, '1');
	SetHardwareKey(HWK_2, '2');
	SetHardwareKey(HWK_3, '3');
	SetHardwareKey(HWK_4, '4');
	SetHardwareKey(HWK_5, '5');
	SetHardwareKey(HWK_6, '6');
	SetHardwareKey(HWK_7, '7');
	SetHardwareKey(HWK_8, '8');
	SetHardwareKey(HWK_9, '9');
	SetHardwareKey(HWK_HOME, VK_ESCAPE);
#elif defined(MODEL_MT360)
	BBKeyMapping_SetKeyData(0, VK_F1, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(1, VK_F2, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(2, VK_F3, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(3, VK_F4, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(20, VK_MULTIPLY, KEY_TYPE_ONETIME);

	SetFunctionKey(FNK_F1, VK_F1);
	SetFunctionKey(FNK_F2, VK_F2);
	SetFunctionKey(FNK_F3, VK_F3);
	SetFunctionKey(FNK_F4, VK_F4);

	SetHardwareKey(HWK_LEFT, VK_LEFT);
	SetHardwareKey(HWK_DOWN, VK_DOWN);
	SetHardwareKey(HWK_UP, VK_UP);
	SetHardwareKey(HWK_RIGHT, VK_RIGHT);
	SetHardwareKey(HWK_ESCAPE, VK_ESCAPE);
	SetHardwareKey(HWK_BACK, VK_BACK);
	SetHardwareKey(HWK_RETURN, VK_RETURN);
	SetHardwareKey(HWK_CENTER, VK_RETURN);
	SetHardwareKey(HWK_F, VK_MULTIPLY);
	SetHardwareKey(HWK_DOT, VK_PERIOD);
	SetHardwareKey(HWK_0, '0');
	SetHardwareKey(HWK_1, '1');
	SetHardwareKey(HWK_2, '2');
	SetHardwareKey(HWK_3, '3');
	SetHardwareKey(HWK_4, '4');
	SetHardwareKey(HWK_5, '5');
	SetHardwareKey(HWK_6, '6');
	SetHardwareKey(HWK_7, '7');
	SetHardwareKey(HWK_8, '8');
	SetHardwareKey(HWK_9, '9');
	SetHardwareKey(HWK_HOME, VK_ESCAPE);
#elif defined(MODEL_MT760)
	BBKeyMapping_SetKeyData(0, VK_HOME, KEY_TYPE_ONETIME);
	BBKeyMapping_SetKeyData(2, VK_BACK, KEY_TYPE_ONETIME);

	SetFunctionKey(FNK_F1, VK_F1);
	SetFunctionKey(FNK_F2, VK_F2);
	SetFunctionKey(FNK_F3, VK_F3);
	SetFunctionKey(FNK_F4, VK_F4);

	SetHardwareKey(HWK_LEFT, VK_LEFT);
	SetHardwareKey(HWK_DOWN, VK_DOWN);
	SetHardwareKey(HWK_UP, VK_UP);
	SetHardwareKey(HWK_RIGHT, VK_RIGHT);
	SetHardwareKey(HWK_ESCAPE, VK_ESCAPE);
	SetHardwareKey(HWK_BACK, VK_BACK);
	SetHardwareKey(HWK_RETURN, VK_RETURN);
	SetHardwareKey(HWK_CENTER, VK_RETURN);
	SetHardwareKey(HWK_F, VK_MULTIPLY);
	SetHardwareKey(HWK_DOT, VK_PERIOD);
	SetHardwareKey(HWK_0, '0');
	SetHardwareKey(HWK_1, '1');
	SetHardwareKey(HWK_2, '2');
	SetHardwareKey(HWK_3, '3');
	SetHardwareKey(HWK_4, '4');
	SetHardwareKey(HWK_5, '5');
	SetHardwareKey(HWK_6, '6');
	SetHardwareKey(HWK_7, '7');
	SetHardwareKey(HWK_8, '8');
	SetHardwareKey(HWK_9, '9');
	SetHardwareKey(HWK_HOME, VK_ESCAPE);
#endif

	BBKeyMapping_Close();

#if BATTERY_EXIST
	EnableBatteryPopup(FALSE);
#endif

	memset(m_uExitSeq, 0, sizeof(UINT) * LENGTH_EXIT_SEQUENCE);

	m_uExitSeqPos	= 0;
	m_uExitSteps	= 0;

	memset(m_uResetF3Seq, 0, sizeof(UINT) * LENGTH_RESET_F3_SEQUENCE);

	m_uResetF3SeqPos	= 0;
	m_uResetF3Steps		= 0;

	m_dwTimeoutTransactionTick	= GetTickCount();
	m_dwTimeoutGlanceTick		= GetTickCount();
	m_dwTimeoutScreenTick		= GetTickCount();

	SetViewMode(VIEW_INTRO);
	SetTransactionMode(TRANSACTION_NONE);
	StartTransactionThread();
}

void CChildView::CleanUp()
{
	EndTransactionThread();

	DWORD	dwResult	= BBSecure_Deinit();

	LOGFILE(L"%S[%d] - BBSecure_SendPaymentKey_simple() = %x\r\n", __FUNCTION__, __LINE__, dwResult);

	SetScreenMode(SCREEN_NORMAL);

	BBKeyMapping_SetDefaultKey();
	BBKeyMapping_Close();

	CloseMSR();

	if (m_pPrinter != NULL)
	{
		m_pPrinter->Close();

		delete m_pPrinter;

		m_pPrinter = NULL;
	}

	if (m_pComm != NULL)
	{
		m_pComm->Close();

		delete m_pComm;

		m_pComm = NULL;
	}

	m_database.Close();

#if BATTERY_EXIST
	EnableBatteryPopup(TRUE);
#endif

	ExitFont();
	ExitDC();

	SetRestartFlag(FALSE);

	CoUninitialize();
	HideTaskBar(FALSE);
}

void CChildView::ExitApplication()
{
	CleanUp();
	exit(0);
}

void CChildView::StartTransactionThread()
{
	m_bThreadForceClose		= FALSE;
	m_hTransactionThread	= ::CreateThread(0, 0, DoTransaction, (LPVOID)this, 0, 0);
	m_nTransactionPriority	= GetThreadPriority(m_hTransactionThread);
	m_hAnnunciatorThread	= ::CreateThread(0, 0, DoAnnunciator, (LPVOID)this, 0, 0);
	m_nAnnunciatorPriority	= GetThreadPriority(m_hAnnunciatorThread);
}

void CChildView::EndTransactionThread()
{
	m_bThreadForceClose	= TRUE;

	Sleep(500);

	if (m_hTransactionThread != NULL)
	{
		::TerminateThread(m_hTransactionThread, 0);
		::CloseHandle(m_hTransactionThread);

		m_hTransactionThread	= NULL;
		m_nTransactionPriority	= THREAD_PRIORITY_NORMAL;
	}

	if (m_hAnnunciatorThread != NULL)
	{
		::TerminateThread(m_hAnnunciatorThread, 0);
		::CloseHandle(m_hAnnunciatorThread);

		m_hAnnunciatorThread	= NULL;
		m_nAnnunciatorPriority	= THREAD_PRIORITY_BELOW_NORMAL;
	}

	m_bThreadForceClose	= FALSE;
}

void CChildView::FirstSetupParameters(PARAMETERS_INFO* pInfo, BOOL bFirst, BOOL bSetTime)
{
	if (bSetTime)
	{
		SetLocalTime(&pInfo->stSyncTime);
	}

	SetNetworkAdapter(pInfo);

	m_Settings.langMode	= (LANGUAGE_MODE)pInfo->nLanguage;

	if (!bFirst)
	{
		ResetConnection();
	}
}

VIEW_MODE CChildView::GetViewMode()
{
	return m_ViewMode;
}

void CChildView::SetViewMode(VIEW_MODE viewMode)
{
	m_prevViewMode	= m_ViewMode;
	m_ViewMode		= viewMode;

#if FALSE
	DoChangeViewMode();
#endif
}

void CChildView::DoChangeViewMode()
{
#if FALSE
	switch (m_prevViewMode)
	{
	}

	switch (m_ViewMode)
	{
	}
#endif
}

TRANSACTION_MODE CChildView::GetTransactionMode()
{
	return m_TransactionMode;
}

void CChildView::SetTransactionMode(TRANSACTION_MODE transactionMode)
{
	m_prevTransactionMode	= m_TransactionMode;
	m_TransactionMode		= transactionMode;
}

void CChildView::PaintPreprocess()
{
	UpdatePaint();
	UpdateViewIntro();
	UpdateViewDialUp();
	UpdateViewTimeout();
	UpdateViewSendParameter();
	UpdateViewKeyInjection();
	UpdateViewMessage();
	UpdateViewResult();
	UpdateViewPIN();
}

void CChildView::ForcePaintFlag(DWORD dwFlag)
{
	switch (dwFlag)
	{
	case PAINT_NONE:
		{
			m_dwPaintFlag	= PAINT_NONE;
		}
		break;
	case PAINT_FULL_SCREEN:
		{
			m_dwPaintFlag	= PAINT_FULL_SCREEN;
		}
		break;
	default:
		{
			if (m_dwPaintFlag != PAINT_FULL_SCREEN)
			{
				m_dwPaintFlag	|= dwFlag;
			}
		}
		break;
	}
}

void CChildView::ResetPaintFlag()
{
	m_dwPaintFlag	= PAINT_NONE;
}

DWORD CChildView::GetPaintFlag()
{
	return m_dwPaintFlag;
}

BOOL CChildView::PaintRequired(DWORD dwFlag, BOOL bIncludeFullScreen)
{
	switch (dwFlag)
	{
	case PAINT_NONE:
		{
		}
		break;
	case PAINT_FULL_SCREEN:
		{
			return (BOOL)(m_dwPaintFlag == PAINT_FULL_SCREEN);
		}
		break;
	default:
		{
			if ((m_dwPaintFlag & dwFlag) == dwFlag)
			{
				return TRUE;
			}

			if (bIncludeFullScreen)
			{
				return (BOOL)(m_dwPaintFlag == PAINT_FULL_SCREEN);
			}
		}
		break;
	}

	return FALSE;
}

void CChildView::UpdatePaint()
{
	if (m_PaintFlags.langMode != m_Settings.langMode)
	{
		m_PaintFlags.langMode	= m_Settings.langMode;

		ForcePaintFlag();
	}
	
	if (m_PaintFlags.transactionMode != GetTransactionMode())
	{
		m_PaintFlags.transactionMode	= GetTransactionMode();

		ForcePaintFlag();
	}

	if (m_PaintFlags.viewMode != GetViewMode())
	{
		m_PaintFlags.viewMode	= GetViewMode();

		ForcePaintFlag();
	}

	if (m_PaintFlags.threadMode != GetThreadMode())
	{
		m_PaintFlags.threadMode	= GetThreadMode();

		ForcePaintFlag();
	}

	if (m_PaintFlags.threadStatus != GetThreadStatus())
	{
		m_PaintFlags.threadStatus	= GetThreadStatus();

		ForcePaintFlag();
	}
}

void CChildView::PaintView()
{
	CDCX	DC;
	DWORD	dwPaintTick;

	if (GetPaintFlag() != PAINT_NONE)
	{
		if (GetPaintFlag() != PAINT_ANNUNCIATOR)
		{
			m_dwTimeoutTransactionTick	= GetTickCount();
		}

		if (GetViewMode() != VIEW_INTRO)
		{
			m_dwTimeoutGlanceTick	= GetTickCount();
			m_dwTimeoutScreenTick	= GetTickCount();
		}

		dwPaintTick	= GetTickCount();

		DoBeforePaintByType();
		DC.GetDC(m_hWnd);
		PaintViewByType(&m_DC);
		DC.BitBlt(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), &m_DC, 0, 0, SRCCOPY);
		DC.ReleaseDC();
		ResetPaintFlag();
		DoAfterPaintByType();
	}
}

void CChildView::DoBeforePaintByType()
{
	switch (GetViewMode())
	{
	case VIEW_INTRO:
		{
			DoBeforePaintViewIntro();
		}
		break;
	case VIEW_MENU_MAIN:
		{
			DoBeforePaintViewMenuMain();
		}
		break;
	}
}

void CChildView::DoAfterPaintByType()
{
	switch (GetViewMode())
	{
	case VIEW_PRINT:
		{
			DoAfterPaintViewPrint();
		}
		break;
	}
}

void CChildView::PaintViewByType(CDCX* pDC) 
{
	int			nBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	crText	= pDC->GetTextColor();
	COLORREF	crBack	= pDC->GetBkColor();

    switch (GetViewMode())
	{
	case VIEW_INTRO:
		{
			PaintViewIntro(pDC);
		}
		break;
	case VIEW_MESSAGE:
		{
			PaintViewMessage(pDC);
		}
		break;
	case VIEW_MENU_MAIN:
		{
			PaintViewMenu(&m_menuInfoMain, pDC);
		}
		break;
	case VIEW_PASSWORD:
		{
			PaintViewPassword(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_PIN:
		{
			PaintViewPIN(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_DIALUP:
		{
			PaintViewDialUp(pDC);
		}
		break;
	case VIEW_RESULT:
		{
			PaintViewResult(pDC);
		}
		break;
	case VIEW_PRINT:
		{
			PaintViewPrint(pDC);
		}
		break;
	case VIEW_PRINT_ACCEPTOR:
		{
			PaintViewPrintAcceptor(pDC);
		}
		break;
	case VIEW_PRINT_RETRY:
		{
			PaintViewPrintRetry(pDC);
		}
		break;
	case VIEW_SALE_AMOUNT:
		{
			PaintViewSaleAmount(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_SALE_CONFIRM:
		{
			PaintViewSaleConfirm(pDC);
		}
		break;
	case VIEW_BILL_BILLID:
		{
			PaintViewBillBillId(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_BILL_PAYMENTID:
		{
			PaintViewBillPaymentId(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			PaintViewBillConfirm(pDC);
		}
		break;
	case VIEW_CHARGE_COUNT:
		{
			PaintViewChargeCount(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_MENU_PROVIDER:
		{
			PaintViewMenu(&m_menuInfoProvider, pDC);
		}
		break;
	case VIEW_MENU_CATEGORY:
		{
			PaintViewMenu(&m_menuInfoCategory, pDC);
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			PaintViewChargeConfirm(pDC);
		}
		break;
	case VIEW_MENU_F1:
		{
			PaintViewMenu(&m_menuInfoF1, pDC);
		}
		break;
	case VIEW_MENU_REPORTS:
		{
			PaintViewMenu(&m_menuInfoReports, pDC);
		}
		break;
	case VIEW_REPORTS_TRACENO:
		{
			PaintViewReportsTraceNo(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_REPORTS_STARTDATE:
	case VIEW_REPORTS_ENDDATE:
		{
			PaintViewReportsDate(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_MENU_F2:
		{
			PaintViewMenu(&m_menuInfoF2, pDC);
		}
		break;
	case VIEW_MENU_SERVICE_REQUEST:
		{
			PaintViewMenu(&m_menuInfoServiceRequest, pDC);
		}
		break;
	case VIEW_MENU_F3:
		{
			PaintViewMenu(&m_menuInfoF3, pDC);
		}
		break;
	case VIEW_CHANGE_CITY_CODE:
		{
			PaintViewChangeCityCode(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_MENU_PHONEBOOK:
		{
			PaintViewMenu(&m_menuInfoPhonebook, pDC);
		}
		break;
	case VIEW_PHONEBOOK:
		{
			PaintViewPhonebook(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_SET_DATE:
		{
			PaintViewSetDate(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_SET_TIME:
		{
			PaintViewSetTime(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_BRIGHTNESS:
		{
			PaintViewBrightness(pDC);
		}
		break;
	case VIEW_MENU_LANGUAGE:
		{
			PaintViewMenu(&m_menuInfoLanguage, pDC);
		}
		break;
	case VIEW_SEND_PARAMETER:
		{
			PaintViewSendParameter(pDC);
		}
		break;
	case VIEW_KEY_INJECTION:
		{
			PaintViewKeyInjection(pDC);
		}
		break;
	case VIEW_MENU_POS_SETTINGS:
		{
			PaintViewMenu(&m_menuInfoSettings, pDC);
		}
		break;
	case VIEW_MENU_TMS_SETTINGS:
		{
			PaintViewMenu(&m_menuInfoTMSSettings, pDC);
		}
		break;
	case VIEW_VERSION:
		{
			PaintViewVersion(pDC);
		}
		break;
	case VIEW_SETTINGS_NII:
	case VIEW_SETTINGS_CONNECTION_TIMEOUT:
	case VIEW_SETTINGS_CONNECTED_TIMEOUT:
	case VIEW_SETTINGS_PIN_TIMEOUT:
	case VIEW_SETTINGS_ADVICE_TIMEOUT:
	case VIEW_SETTINGS_REVERSAL_TIMEOUT:
	case VIEW_SETTINGS_SCREEN_TIMEOUT:
		{
			PaintViewSettings(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_TMS_SETTINGS_STARTTIME:
	case VIEW_TMS_SETTINGS_ENDTIME:
	case VIEW_TMS_SETTINGS_INTERVAL:
		{
			PaintViewTMSSettings(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_TMS_SETTINGS_COUNT:
		{
			PaintViewTMSSettingsCount(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_TMS_SETTINGS_WEEKDAYS:
		{
			PaintViewTMSSettingsWeekdays(pDC);
		}
		break;
	case VIEW_MENU_CONNECTION_TYPE:
		{
			PaintViewMenu(&m_menuInfoConnectionType, pDC);
		}
		break;
	case VIEW_SETTINGS_SERVER_IPADDRESS:
		{
			PaintViewSettingsServerIPAddress(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	case VIEW_SETTINGS_SERVER_PORT:
		{
			PaintViewSettingsServerPort(pDC);
			PaintViewKeypad(pDC);
		}
		break;
	}

	PaintViewAnnunciator(pDC);

	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);
	pDC->SetBkMode(nBkMode);
}

void CChildView::UpdateViewTimeout()
{
	if (GetViewMode() == VIEW_INTRO)
	{
		m_dwTimeoutTransactionTick	= GetTickCount();
	}
	else
	{
		if ((GetTickCount() - m_dwTimeoutTransactionTick) > (m_Parameters.nTimeoutConnected * 1000))
		{
			m_dwTimeoutTransactionTick	= GetTickCount();

			Restart();
		}
	}
}

void CChildView::ClearFunctionKeys()
{
	memset(m_nFNK, 0, sizeof(UINT) * MAX_FNK);
}

void CChildView::SetFunctionKey(UINT nFunctionKey, UINT nChar)
{
	m_nFNK[nFunctionKey]	= nChar;
}

UINT CChildView::GetFunctionKey(UINT nChar)
{
	for (int i = 0; i < MAX_FNK; i++)
	{
		if (nChar == m_nFNK[i])
		{
			return i;
		}
	}

	return 0xFFFFFFFF;
}

UINT CChildView::GetFunctionChar(UINT nFunctionKey)
{
	return m_nFNK[nFunctionKey];
}

void CChildView::ClearHardwareKeys()
{
	memset(m_nHWK, 0, sizeof(UINT) * MAX_HWK);
}

void CChildView::SetHardwareKey(UINT nHardwareKey, UINT nChar)
{
	m_nHWK[nHardwareKey]	= nChar;
}

UINT CChildView::GetHardwareKey(UINT nChar)
{
	for (int i = 0; i < MAX_HWK; i++)
	{
		if (nChar == m_nHWK[i])
		{
			return i;
		}
	}

	return 0xFFFFFFFF;
}

UINT CChildView::GetHardwareChar(UINT nHardwareKey)
{
	return m_nHWK[nHardwareKey];
}

SCREEN_MODE CChildView::GetScreenMode()
{
	return m_screenMode;
}

void CChildView::SetScreenMode(SCREEN_MODE mode)
{
	if (mode == GetScreenMode())
	{
		return;
	}

	switch (mode)
	{
	case SCREEN_NORMAL:
		{
			SetBrightness(m_Settings.nBrightness);
		}
		break;
	case SCREEN_OUT:
		{
			SetBrightness(1);
		}
		break;
	case SCREEN_GLANCE:
		{
			SetBrightness(1);
		}
		break;
	}

	m_screenMode	= mode;

	ForcePaintFlag();
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

#pragma warning(disable:4065)

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	default:
		{
			if ((WM_TMS_MESSAGE != 0) && (pMsg->message == WM_TMS_MESSAGE))
			{
				OnTMS(pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
		break;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	StartUp();

	return 0;
}

void CChildView::OnPaint()
{
	PaintPreprocess();
	PaintView();
	Sleep(10);
}

void CChildView::OnDestroy()
{
	CleanUp();
	CWnd::OnDestroy();
}

void CChildView::OnLButtonDownButton(RES_PNG resPNG, POINT pt, DWORD dwFlag)
{
	RECT	rc;

	GetRectFromPNGResource(&rc, resPNG);

	if (PtInRect(&rc, pt))
	{
		m_nInfo[INFO_BUTTON_STATUS]	= dwFlag;

		ForcePaintFlag(dwFlag);
	}
}

void CChildView::OnLButtonUpButton(RES_PNG resPNG, POINT pt, UINT nChar)
{
	RECT	rc;

	if (m_nInfo[INFO_BUTTON_STATUS])
	{
		ForcePaintFlag(m_nInfo[INFO_BUTTON_STATUS]);
	}

	GetRectFromPNGResource(&rc, resPNG);

	if (PtInRect(&rc, pt))
	{
		m_nInfo[INFO_BUTTON_STATUS]	= PAINT_NONE;

		PostMessage(WM_KEYDOWN, nChar);
	}
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SetScreenMode(SCREEN_NORMAL);

	m_dwTimeoutTransactionTick	= GetTickCount();
	m_dwTimeoutGlanceTick		= GetTickCount();
	m_dwTimeoutScreenTick		= GetTickCount();

	switch (GetViewMode())
	{
	case VIEW_INTRO:
		{
			OnKeyDownViewIntro(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_MAIN:
		{
			OnKeyDownViewMenu(&m_menuInfoMain, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_PASSWORD:
	case VIEW_PIN:
		{
			OnKeyDownViewSecure(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MESSAGE:
	case VIEW_DIALUP:
	case VIEW_RESULT:
		{
			OnKeyDownViewCommon(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_PRINT:
	case VIEW_PRINT_ACCEPTOR:
	case VIEW_PRINT_RETRY:
		{
			OnKeyDownViewPrint(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SALE_AMOUNT:
	case VIEW_SALE_CONFIRM:
		{
			OnKeyDownViewSale(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
	case VIEW_BILL_CONFIRM:
		{
			OnKeyDownViewBill(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_CHARGE_COUNT:
	case VIEW_CHARGE_CONFIRM:
		{
			OnKeyDownViewCharge(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_PROVIDER:
		{
			OnKeyDownViewMenu(&m_menuInfoProvider, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_CATEGORY:
		{
			OnKeyDownViewMenu(&m_menuInfoCategory, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_F1:
		{
			OnKeyDownViewMenu(&m_menuInfoF1, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_REPORTS:
		{
			OnKeyDownViewMenu(&m_menuInfoReports, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_REPORTS_TRACENO:
		{
			OnKeyDownViewReportsTraceNo(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_REPORTS_STARTDATE:
	case VIEW_REPORTS_ENDDATE:
		{
			OnKeyDownViewReportsDate(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_F2:
		{
			OnKeyDownViewMenu(&m_menuInfoF2, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_SERVICE_REQUEST:
		{
			OnKeyDownViewMenu(&m_menuInfoServiceRequest, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_F3:
		{
			OnKeyDownViewMenu(&m_menuInfoF3, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_CHANGE_CITY_CODE:
		{
			OnKeyDownViewChangeCityCode(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_PHONEBOOK:
		{
			OnKeyDownViewMenu(&m_menuInfoPhonebook, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_PHONEBOOK:
		{
			OnKeyDownViewPhonebook(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SET_DATE:
		{
			OnKeyDownViewSetDate(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SET_TIME:
		{
			OnKeyDownViewSetTime(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_BRIGHTNESS:
		{
			OnKeyDownViewBrightness(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_LANGUAGE:
		{
			OnKeyDownViewMenu(&m_menuInfoLanguage, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SEND_PARAMETER:
		{
			OnKeyDownViewSendParameter(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_KEY_INJECTION:
		{
			OnKeyDownViewKeyInjection(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_POS_SETTINGS:
		{
			OnKeyDownViewMenu(&m_menuInfoSettings, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_TMS_SETTINGS:
		{
			OnKeyDownViewMenu(&m_menuInfoTMSSettings, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_VERSION:
		{
			OnKeyDownViewVersion(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SETTINGS_NII:
	case VIEW_SETTINGS_CONNECTION_TIMEOUT:
	case VIEW_SETTINGS_CONNECTED_TIMEOUT:
	case VIEW_SETTINGS_PIN_TIMEOUT:
	case VIEW_SETTINGS_ADVICE_TIMEOUT:
	case VIEW_SETTINGS_REVERSAL_TIMEOUT:
	case VIEW_SETTINGS_SCREEN_TIMEOUT:
		{
			OnKeyDownViewSettings(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_TMS_SETTINGS_STARTTIME:
	case VIEW_TMS_SETTINGS_ENDTIME:
	case VIEW_TMS_SETTINGS_INTERVAL:
		{
			OnKeyDownViewTMSSettings(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_TMS_SETTINGS_COUNT:
		{
			OnKeyDownViewTMSSettingsCount(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_TMS_SETTINGS_WEEKDAYS:
		{
			OnKeyDownViewTMSSettingsWeekdays(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_MENU_CONNECTION_TYPE:
		{
			OnKeyDownViewMenu(&m_menuInfoConnectionType, nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SETTINGS_SERVER_IPADDRESS:
		{
			OnKeyDownViewSettingsServerIPAddress(nChar, nRepCnt, nFlags);
		}
		break;
	case VIEW_SETTINGS_SERVER_PORT:
		{
			OnKeyDownViewSettingsServerPort(nChar, nRepCnt, nFlags);
		}
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetScreenMode(SCREEN_NORMAL);

	m_dwTimeoutTransactionTick	= GetTickCount();
	m_dwTimeoutGlanceTick		= GetTickCount();
	m_dwTimeoutScreenTick		= GetTickCount();
	m_bLButtonDown				= TRUE;

	switch (GetViewMode())
	{
	case VIEW_INTRO:
		{
			OnLButtonDownViewIntro(nFlags, point);
		}
		break;
	case VIEW_MENU_MAIN:
		{
			OnLButtonDownViewMenu(&m_menuInfoMain, nFlags, point);
		}
		break;
	case VIEW_PASSWORD:
	case VIEW_PIN:
		{
			OnLButtonDownViewSecure(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MESSAGE:
	case VIEW_DIALUP:
	case VIEW_RESULT:
		{
			OnLButtonDownViewCommon(nFlags, point);
		}
		break;
	case VIEW_PRINT:
	case VIEW_PRINT_ACCEPTOR:
	case VIEW_PRINT_RETRY:
		{
			OnLButtonDownViewPrint(nFlags, point);
		}
		break;
	case VIEW_SALE_AMOUNT:
	case VIEW_SALE_CONFIRM:
		{
			OnLButtonDownViewSale(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
		{
			OnLButtonDownViewBill(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			OnLButtonDownViewBill(nFlags, point);
		}
		break;
	case VIEW_CHARGE_COUNT:
		{
			OnLButtonDownViewCharge(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			OnLButtonDownViewCharge(nFlags, point);
		}
		break;
	case VIEW_MENU_PROVIDER:
		{
			OnLButtonDownViewMenu(&m_menuInfoProvider, nFlags, point);
		}
		break;
	case VIEW_MENU_CATEGORY:
		{
			OnLButtonDownViewMenu(&m_menuInfoCategory, nFlags, point);
		}
		break;
	case VIEW_MENU_F1:
		{
			OnLButtonDownViewMenu(&m_menuInfoF1, nFlags, point);
		}
		break;
	case VIEW_MENU_REPORTS:
		{
			OnLButtonDownViewMenu(&m_menuInfoReports, nFlags, point);
		}
		break;
	case VIEW_REPORTS_TRACENO:
		{
			OnLButtonDownViewReportsTraceNo(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_REPORTS_STARTDATE:
	case VIEW_REPORTS_ENDDATE:
		{
			OnLButtonDownViewReportsDate(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MENU_F2:
		{
			OnLButtonDownViewMenu(&m_menuInfoF2, nFlags, point);
		}
		break;
	case VIEW_MENU_SERVICE_REQUEST:
		{
			OnLButtonDownViewMenu(&m_menuInfoServiceRequest, nFlags, point);
		}
		break;
	case VIEW_MENU_F3:
		{
			OnLButtonDownViewMenu(&m_menuInfoF3, nFlags, point);
		}
		break;
	case VIEW_CHANGE_CITY_CODE:
		{
			OnLButtonDownViewChangeCityCode(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MENU_PHONEBOOK:
		{
			OnLButtonDownViewMenu(&m_menuInfoPhonebook, nFlags, point);
		}
		break;
	case VIEW_PHONEBOOK:
		{
			OnLButtonDownViewPhonebook(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SET_DATE:
		{
			OnLButtonDownViewSetDate(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SET_TIME:
		{
			OnLButtonDownViewSetTime(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BRIGHTNESS:
		{
			OnLButtonDownViewBrightness(nFlags, point);
		}
		break;
	case VIEW_MENU_LANGUAGE:
		{
			OnLButtonDownViewMenu(&m_menuInfoLanguage, nFlags, point);
		}
		break;
	case VIEW_SEND_PARAMETER:
		{
			OnLButtonDownViewSendParameter(nFlags, point);
		}
		break;
	case VIEW_KEY_INJECTION:
		{
			OnLButtonDownViewKeyInjection(nFlags, point);
		}
		break;
	case VIEW_MENU_POS_SETTINGS:
		{
			OnLButtonDownViewMenu(&m_menuInfoSettings, nFlags, point);
		}
		break;
	case VIEW_MENU_TMS_SETTINGS:
		{
			OnLButtonDownViewMenu(&m_menuInfoTMSSettings, nFlags, point);
		}
		break;
	case VIEW_VERSION:
		{
			OnLButtonDownViewVersion(nFlags, point);
		}
		break;
	case VIEW_SETTINGS_NII:
	case VIEW_SETTINGS_CONNECTION_TIMEOUT:
	case VIEW_SETTINGS_CONNECTED_TIMEOUT:
	case VIEW_SETTINGS_PIN_TIMEOUT:
	case VIEW_SETTINGS_ADVICE_TIMEOUT:
	case VIEW_SETTINGS_REVERSAL_TIMEOUT:
	case VIEW_SETTINGS_SCREEN_TIMEOUT:
		{
			OnLButtonDownViewSettings(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_STARTTIME:
	case VIEW_TMS_SETTINGS_ENDTIME:
	case VIEW_TMS_SETTINGS_INTERVAL:
		{
			OnLButtonDownViewTMSSettings(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_COUNT:
		{
			OnLButtonDownViewTMSSettingsCount(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_WEEKDAYS:
		{
			OnLButtonDownViewTMSSettingsWeekdays(nFlags, point);
		}
		break;
	case VIEW_MENU_CONNECTION_TYPE:
		{
			OnLButtonDownViewMenu(&m_menuInfoConnectionType, nFlags, point);
		}
		break;
	case VIEW_SETTINGS_SERVER_IPADDRESS:
		{
			OnLButtonDownViewSettingsServerIPAddress(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SETTINGS_SERVER_PORT:
		{
			OnLButtonDownViewSettingsServerPort(nFlags, point);
			OnLButtonDownViewKeypad(nFlags, point);
		}
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetScreenMode(SCREEN_NORMAL);

	m_dwTimeoutTransactionTick	= GetTickCount();
	m_dwTimeoutGlanceTick		= GetTickCount();
	m_dwTimeoutScreenTick		= GetTickCount();

	if (m_bLButtonDown == FALSE)
	{
		OnLButtonDown(nFlags, point);
	}

	m_bLButtonDown	= FALSE;

	if (m_nInfo[INFO_BUTTON_STATUS] != PAINT_NONE)
	{
		ForcePaintFlag(m_nInfo[INFO_BUTTON_STATUS]);
	}

	switch (GetViewMode())
	{
	case VIEW_INTRO:
		{
			OnLButtonUpViewIntro(nFlags, point);
		}
		break;
	case VIEW_MENU_MAIN:
		{
			OnLButtonUpViewMenu(&m_menuInfoMain, nFlags, point);
		}
		break;
	case VIEW_PASSWORD:
	case VIEW_PIN:
		{
			OnLButtonUpViewSecure(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MESSAGE:
	case VIEW_DIALUP:
	case VIEW_RESULT:
		{
			OnLButtonUpViewCommon(nFlags, point);
		}
		break;
	case VIEW_PRINT:
	case VIEW_PRINT_ACCEPTOR:
	case VIEW_PRINT_RETRY:
		{
			OnLButtonUpViewPrint(nFlags, point);
		}
		break;
	case VIEW_SALE_AMOUNT:
	case VIEW_SALE_CONFIRM:
		{
			OnLButtonUpViewSale(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BILL_BILLID:
	case VIEW_BILL_PAYMENTID:
		{
			OnLButtonUpViewBill(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BILL_CONFIRM:
		{
			OnLButtonUpViewBill(nFlags, point);
		}
		break;
	case VIEW_CHARGE_COUNT:
		{
			OnLButtonUpViewCharge(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_CHARGE_CONFIRM:
		{
			OnLButtonUpViewCharge(nFlags, point);
		}
		break;
	case VIEW_MENU_PROVIDER:
		{
			OnLButtonUpViewMenu(&m_menuInfoProvider, nFlags, point);
		}
		break;
	case VIEW_MENU_CATEGORY:
		{
			OnLButtonUpViewMenu(&m_menuInfoCategory, nFlags, point);
		}
		break;
	case VIEW_MENU_F1:
		{
			OnLButtonUpViewMenu(&m_menuInfoF1, nFlags, point);
		}
		break;
	case VIEW_MENU_REPORTS:
		{
			OnLButtonUpViewMenu(&m_menuInfoReports, nFlags, point);
		}
		break;
	case VIEW_REPORTS_TRACENO:
		{
			OnLButtonUpViewReportsTraceNo(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_REPORTS_STARTDATE:
	case VIEW_REPORTS_ENDDATE:
		{
			OnLButtonUpViewReportsDate(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MENU_F2:
		{
			OnLButtonUpViewMenu(&m_menuInfoF2, nFlags, point);
		}
		break;
	case VIEW_MENU_SERVICE_REQUEST:
		{
			OnLButtonUpViewMenu(&m_menuInfoServiceRequest, nFlags, point);
		}
		break;
	case VIEW_MENU_F3:
		{
			OnLButtonUpViewMenu(&m_menuInfoF3, nFlags, point);
		}
		break;
	case VIEW_CHANGE_CITY_CODE:
		{
			OnLButtonUpViewChangeCityCode(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_MENU_PHONEBOOK:
		{
			OnLButtonUpViewMenu(&m_menuInfoPhonebook, nFlags, point);
		}
		break;
	case VIEW_PHONEBOOK:
		{
			OnLButtonUpViewPhonebook(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SET_DATE:
		{
			OnLButtonUpViewSetDate(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SET_TIME:
		{
			OnLButtonUpViewSetTime(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_BRIGHTNESS:
		{
			OnLButtonUpViewBrightness(nFlags, point);
		}
		break;
	case VIEW_MENU_LANGUAGE:
		{
			OnLButtonUpViewMenu(&m_menuInfoLanguage, nFlags, point);
		}
		break;
	case VIEW_SEND_PARAMETER:
		{
			OnLButtonUpViewSendParameter(nFlags, point);
		}
		break;
	case VIEW_KEY_INJECTION:
		{
			OnLButtonUpViewKeyInjection(nFlags, point);
		}
		break;
	case VIEW_MENU_POS_SETTINGS:
		{
			OnLButtonUpViewMenu(&m_menuInfoSettings, nFlags, point);
		}
		break;
	case VIEW_MENU_TMS_SETTINGS:
		{
			OnLButtonUpViewMenu(&m_menuInfoTMSSettings, nFlags, point);
		}
		break;
	case VIEW_VERSION:
		{
			OnLButtonUpViewVersion(nFlags, point);
		}
		break;
	case VIEW_SETTINGS_NII:
	case VIEW_SETTINGS_CONNECTION_TIMEOUT:
	case VIEW_SETTINGS_CONNECTED_TIMEOUT:
	case VIEW_SETTINGS_PIN_TIMEOUT:
	case VIEW_SETTINGS_ADVICE_TIMEOUT:
	case VIEW_SETTINGS_REVERSAL_TIMEOUT:
	case VIEW_SETTINGS_SCREEN_TIMEOUT:
		{
			OnLButtonUpViewSettings(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_STARTTIME:
	case VIEW_TMS_SETTINGS_ENDTIME:
	case VIEW_TMS_SETTINGS_INTERVAL:
		{
			OnLButtonUpViewTMSSettings(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_COUNT:
		{
			OnLButtonUpViewTMSSettingsCount(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_TMS_SETTINGS_WEEKDAYS:
		{
			OnLButtonUpViewTMSSettingsWeekdays(nFlags, point);
		}
		break;
	case VIEW_MENU_CONNECTION_TYPE:
		{
			OnLButtonUpViewMenu(&m_menuInfoConnectionType, nFlags, point);
		}
		break;
	case VIEW_SETTINGS_SERVER_IPADDRESS:
		{
			OnLButtonUpViewSettingsServerIPAddress(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	case VIEW_SETTINGS_SERVER_PORT:
		{
			OnLButtonUpViewSettingsServerPort(nFlags, point);
			OnLButtonUpViewKeypad(nFlags, point);
		}
		break;
	}

	CWnd::OnLButtonUp(nFlags, point);
}
