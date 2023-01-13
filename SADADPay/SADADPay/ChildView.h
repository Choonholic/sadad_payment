// ChildView.h

#pragma once

#include "DCX.h"
#include "bbappapi.h"
#include "MTCTPrinter.h"
#include "Comm.h"
#include "Settings.h"
#include "TransactionDatabase.h"

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

	void	StartUp();
	void	CleanUp();
	void	ExitApplication();

	void	StartTransactionThread();
	void	EndTransactionThread();

	void	FirstSetupParameters(PARAMETERS_INFO* pInfo, BOOL bFirst, BOOL bSetTime	= TRUE);

	void	ForcePaintFlag(DWORD dwFlag = PAINT_FULL_SCREEN);
	void	ResetPaintFlag();
	DWORD	GetPaintFlag();
	BOOL	PaintRequired(DWORD dwFlag = PAINT_FULL_SCREEN, BOOL bIncludeFullScreen = TRUE);

	void	UpdatePaint();

	VIEW_MODE	GetViewMode();
	void		SetViewMode(VIEW_MODE viewMode);
	void		DoChangeViewMode();

	TRANSACTION_MODE	GetTransactionMode();
	void				SetTransactionMode(TRANSACTION_MODE transactionMode);
	void				DoChangeTransactionMode();

	void	PaintView();
	void	DoBeforePaintByType();
	void	DoAfterPaintByType();
	void	PaintViewByType(CDCX* pDC);

	void	PaintPreprocess();

	void	UpdateViewTimeout();

	BOOL	SetMessage(UINT uID, MESSAGE_TYPE messageType = MESSAGE_ERROR, BOOL bPrint = FALSE);
	void	ResetMessage();
	void	ShowMessage(UINT uID, MESSAGE_TYPE messageType = MESSAGE_ERROR, BOOL bPrint = FALSE);
	void	ShowMessage();
	void	HandleMessage();
	void	SetViewByMessage();

	GPRS_STATUS	GetGPRSStatus();

	void	UpdateViewAnnunciator();
	void	PaintViewAnnunciator(CDCX* pDC);

	void	UpdateViewMessage();
	void	PaintViewMessage(CDCX* pDC);

	void	InitializeMenuBase(MENU_INFO* pMenuInfo, MENU_TYPE type, UINT nTitleID, UINT uItemCount, BOOL bHierarchy = FALSE, VIEW_MODE previousViewMode = VIEW_NONE, TRANSACTION_MODE previousTransactionMode = TRANSACTION_NONE, BOOL bCheckList = FALSE);
	BOOL	SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, BILINGUAL text, int nStatus = MENU_UNCHECKED);
	BOOL	SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, UINT uID, int nStatus = MENU_UNCHECKED);
	BOOL	SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, LPCWSTR szE, LPCWSTR szF, int nStatus = MENU_UNCHECKED);

	void	InitializeMenu(MENU_INFO* pMenuInfo, MENU_TYPE type, UINT nTitleID, UINT uFirstID, UINT uLastID, BOOL bHierarchy = FALSE, VIEW_MODE previousViewMode = VIEW_NONE, TRANSACTION_MODE previousTransactionMode = TRANSACTION_NONE, BOOL bCheckList = FALSE);
	void	ExecuteMenu(MENU_INFO* pMenuInfo);
	void	DoCancelMenu(MENU_INFO* pMenuInfo);
	void	PaintViewMenu(MENU_INFO* pMenuInfo, CDCX* pDC);
	void	PaintViewMenuArrow(MENU_INFO* pMenuInfo, CDCX* pDC);
	void	PaintViewMenuItem(MENU_INFO* pMenuInfo, CDCX* pDC, int nItem);
	void	OnKeyDownViewMenu(MENU_INFO* pMenuInfo, UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewMenu(MENU_INFO* pMenuInfo, UINT nFlags, CPoint point);
	void	OnLButtonUpViewMenu(MENU_INFO* pMenuInfo, UINT nFlags, CPoint point);

	void	Restart();

	void	UpdateViewPIN();
	void	PaintViewPassword(CDCX* pDC);
	void	PaintViewPIN(CDCX* pDC);
	void	OnKeyDownViewSecure(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewSecure(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSecure(UINT nFlags, CPoint point);

	void	DoEnterViewDialUp();
	void	DoExitViewDialUp();
	void	UpdateViewDialUp();
	void	UpdateViewResult();
	void	PaintViewDialUp(CDCX* pDC);
	void	PaintViewResult(CDCX* pDC);
	void	OnKeyDownViewCommon(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewCommon(UINT nFlags, CPoint point);
	void	OnLButtonUpViewCommon(UINT nFlags, CPoint point);

	void	SetPrintInfo(PRINT_INFO* pPrintInfo, RECEIPT_TYPE ReceiptType, TRANSACTION_INFO* pTransactionInfo, PARAMETERS_INFO* pParameters, RECEIPT_SEQUENCE ReceiptSequence);
	void	SetPrintInfo(PRINT_INFO* pPrintInfo, RECEIPT_TYPE ReceiptType, CHARGE_INFO* pChargeInfo, PARAMETERS_INFO* pParameters, RECEIPT_SEQUENCE ReceiptSequence);
	void	PrintReceipt(PRINT_INFO* pPrintInfo);
	void	DoActionAfterPrint(PRINT_INFO* pPrintInfo, BOOL bSuccess);
	void	DoAfterPaintViewPrint();
	void	PaintViewPrint(CDCX* pDC);
	void	PaintViewPrintAcceptor(CDCX* pDC);
	void	PaintViewPrintRetry(CDCX* pDC);
	void	OnKeyDownViewPrint(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewPrint(UINT nFlags, CPoint point);
	void	OnLButtonUpViewPrint(UINT nFlags, CPoint point);

	void	UpdateViewIntro();
	void	DoBeforePaintViewIntro();
	void	PaintViewIntro(CDCX* pDC);
	void	OnKeyDownViewIntro(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewIntro(UINT nFlags, CPoint point);
	void	OnLButtonUpViewIntro(UINT nFlags, CPoint point);

	void	ExecuteMenuMain();
	void	DoBeforePaintViewMenuMain();

	void	ExecuteMenuProvider();
	void	ExecuteMenuCategory();

	void	ExecuteMenuF1();
	void	ExecuteMenuReports();

	void	ExecuteMenuF2();
	void	ExecuteMenuServiceRequest();

	void	ExecuteMenuF3();
	void	DoCancelMenuTMSSettings();
	void	ExecuteMenuPhonebook();
	void	ExecuteMenuLanguage();
	void	ExecuteMenuSettings();
	void	ExecuteMenuTMSSettings();
	void	ExecuteMenuConnectionType();

	void	PaintViewSaleAmount(CDCX* pDC);
	void	PaintViewSaleConfirm(CDCX* pDC);
	void	OnKeyDownViewSale(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewSale(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSale(UINT nFlags, CPoint point);

	void	PaintViewBillBillId(CDCX* pDC);
	void	PaintViewBillPaymentId(CDCX* pDC);
	void	PaintViewBillConfirm(CDCX* pDC);
	void	OnKeyDownViewBill(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewBill(UINT nFlags, CPoint point);
	void	OnLButtonUpViewBill(UINT nFlags, CPoint point);

	void	InitializeMenuProvider();
	void	InitializeMenuCategory();

	LONGLONG	CaculateChargeAmount(WCHAR* szProviderID, WCHAR* szCategoryID);
	void		PaintViewChargeCount(CDCX* pDC);
	void		PaintViewChargeConfirm(CDCX* pDC);
	void		OnKeyDownViewCharge(UINT nChar, UINT nRepCnt, UINT nFlags);
	void		OnLButtonDownViewCharge(UINT nFlags, CPoint point);
	void		OnLButtonUpViewCharge(UINT nFlags, CPoint point);

	void	BuildTransactionDetails(CTransactionDatabase& database, WCHAR* szStartDate, WCHAR* szEndDate, CByteArray& arrType, CStringArray& arrFeed);
	void	PaintViewReportsTraceNo(CDCX* pDC);
	void	PaintViewReportsDate(CDCX* pDC);
	void	OnKeyDownViewReportsTraceNo(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewReportsDate(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewReportsTraceNo(UINT nFlags, CPoint point);
	void	OnLButtonUpViewReportsTraceNo(UINT nFlags, CPoint point);
	void	OnLButtonDownViewReportsDate(UINT nFlags, CPoint point);
	void	OnLButtonUpViewReportsDate(UINT nFlags, CPoint point);

	void	Logon();

	void	InitializeMenuPhonebook();
	void	InitializeMenuLanguage();
	void	InitializeMenuConnectionType();
	void	SetSendParameter(DWORD dwStatus);
	DWORD	GetSendParameter();
	BOOL	SetPOSDateTime();
	BOOL	LoadMasterKeyParameter();
	void	UpdateViewSendParameter();
	void	UpdateViewKeyInjection();
	void	PaintViewChangeCityCode(CDCX* pDC);
	void	PaintViewPhonebook(CDCX* pDC);
	void	PaintViewSetDate(CDCX* pDC);
	void	PaintViewSetTime(CDCX* pDC);
	void	PaintViewBrightness(CDCX* pDC);
	void	PaintViewSendParameter(CDCX* pDC);
	void	PaintViewKeyInjection(CDCX* pDC);
	void	PaintViewSettings(CDCX* pDC);
	void	PaintViewVersion(CDCX* pDC);
	void	PaintViewSettingsServerIPAddress(CDCX* pDC);
	void	PaintViewSettingsServerPort(CDCX* pDC);
	void	PaintViewTMSSettings(CDCX* pDC);
	void	PaintViewTMSSettingsCount(CDCX* pDC);
	void	PaintViewTMSSettingsWeekdays(CDCX* pDC);
	void	OnKeyDownViewChangeCityCode(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewPhonebook(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSetDate(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSetTime(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewBrightness(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSendParameter(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewKeyInjection(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSettings(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewVersion(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSettingsServerIPAddress(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewSettingsServerPort(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewTMSSettings(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewTMSSettingsCount(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnKeyDownViewTMSSettingsWeekdays(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDownViewChangeCityCode(UINT nFlags, CPoint point);
	void	OnLButtonUpViewChangeCityCode(UINT nFlags, CPoint point);
	void	OnLButtonDownViewPhonebook(UINT nFlags, CPoint point);
	void	OnLButtonUpViewPhonebook(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSetDate(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSetDate(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSetTime(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSetTime(UINT nFlags, CPoint point);
	void	OnLButtonDownViewBrightness(UINT nFlags, CPoint point);
	void	OnLButtonUpViewBrightness(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSendParameter(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSendParameter(UINT nFlags, CPoint point);
	void	OnLButtonDownViewKeyInjection(UINT nFlags, CPoint point);
	void	OnLButtonUpViewKeyInjection(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSettings(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSettings(UINT nFlags, CPoint point);
	void	OnLButtonDownViewVersion(UINT nFlags, CPoint point);
	void	OnLButtonUpViewVersion(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSettingsServerIPAddress(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSettingsServerIPAddress(UINT nFlags, CPoint point);
	void	OnLButtonDownViewSettingsServerPort(UINT nFlags, CPoint point);
	void	OnLButtonUpViewSettingsServerPort(UINT nFlags, CPoint point);
	void	OnLButtonDownViewTMSSettings(UINT nFlags, CPoint point);
	void	OnLButtonUpViewTMSSettings(UINT nFlags, CPoint point);
	void	OnLButtonDownViewTMSSettingsCount(UINT nFlags, CPoint point);
	void	OnLButtonUpViewTMSSettingsCount(UINT nFlags, CPoint point);
	void	OnLButtonDownViewTMSSettingsWeekdays(UINT nFlags, CPoint point);
	void	OnLButtonUpViewTMSSettingsWeekdays(UINT nFlags, CPoint point);

	void	PaintViewKeypad(CDCX* pDC);
	void	OnLButtonDownViewKeypad(UINT nFlags, CPoint point);
	void	OnLButtonUpViewKeypad(UINT nFlags, CPoint point);

	void	OnLButtonDownButton(RES_PNG resPNG, POINT pt, DWORD dwFlag);
	void	OnLButtonUpButton(RES_PNG resPNG, POINT pt, UINT nChar);

	void	InitDC();
	void	ExitDC();

	void	InitFont();
	void	ExitFont();

	void	ResetConnection();

	void	ClearInfo();

	void	PaintBackground(CDCX* pDC, COLORREF cr = COLOR_RGB_WHITE);
	void	PaintHeader(LPCTSTR szText, CDCX* pDC);
	void	PaintHeaderDesc(LPCTSTR szText, CDCX* pDC);
	void	PaintMessage(LPCTSTR szText, RES_PNG resPNG, CDCX* pDC);
	void	PaintButton(RES_PNG resPNG, RES_PNG resPNGPress, DWORD dwFlag, CDCX* pDC, LPCWSTR szText = NULL, COLORREF crText = COLOR_RGB_WHITE);

	void	OpenMSR();
	void	RestartMSR();
	void	CloseMSR();
	void	StartFlow(BOOL bClose = TRUE);

	THREAD_MODE	GetThreadMode();
	void		SetThreadMode(THREAD_MODE mode);

	void			CleanUpGarbage();
	void			ClearThreadStatus(BOOL bDisconnect = TRUE);
	THREAD_STATUS	GetThreadStatus();
	void			SetThreadStatus(THREAD_STATUS status);

	BOOL	GetThreadError(UINT& nID, MESSAGE_TYPE& messageType, BOOL& bPrint);
	void	SetThreadError(UINT nID, MESSAGE_TYPE messageType = MESSAGE_ERROR, BOOL bPrint = FALSE);
	void	ClearThreadError();

	int		GetThreadCount();
	void	SetThreadCount(int nCount = THREAD_RETRY_INITIAL);

	ACTION_REQUESTER	GetActionRequester();
	void				SetActionRequester(ACTION_REQUESTER ActionRequester);

	void	RequestConnect(ACTION_REQUESTER ActionRequester, BOOL bRequest = TRUE);
	void	RequestTransaction(BOOL bRequest = TRUE);
	void	RequestDisconnect(BOOL bRequest = TRUE);
	void	RequestCancelAdvice(BOOL bRequest = TRUE);
	void	RequestPrint(BOOL bRequest = TRUE);

	BOOL	ConnectRequested();
	BOOL	DisconnectRequested();
	BOOL	TransactionRequested();
	BOOL	CancelAdviceRequested();

	BOOL	ReversalExist();
	void	SetReversalStatus();
	BOOL	AdviceExist();
	void	SetAdviceStatus();

	BOOL	TransactionReady();
	BOOL	LogonReady();
	BOOL	ReversalReady();
	BOOL	AdviceReady();
	BOOL	TMSReady();

	BOOL	BatteryIsLow();

	void	SetNextLogonTime();
	void	ResetLogonTime();
	void	SetNextReversalTime();
	void	ResetReversalTime();
	void	SetNextAdviceTime();
	void	ResetAdviceTime();

	TRANSACTION_STATUS	GetTransactionStatus(TRANSACTION_INFO* pInfo);
	void				SetTransactionStatus(TRANSACTION_INFO* pInfo, TRANSACTION_STATUS transactionStatus);

	BOOL	AllTransactionsFinished(TRANSACTION_INFO* pInfo);

	int		GetNextServerPhone(int nServer);

	void	ClearTransactionData();
	void	SetMandatoryData(TRANSACTION_INFO* pInfo);
	BOOL	SetTransactionData(TRANSACTION_INFO* pInfo, int nTraceNo);
	void	SetReversalData(TRANSACTION_INFO* pInfo);
	void	SetAdviceData(TRANSACTION_INFO* pInfo);

#if !IGNORE_FINAL_MAC
	void	SetFinalMAC();
#endif

	void	SetTransportData(TRANSACTION_MODE transactionMode, DWORD dwWaitTime, UINT nAttempts = 0);
	void	BuildTransactionData();

	TRANSACTION_RESULT	ParseTransactionResult(TRANSACTION_INFO* pInfo, UCHAR* pRecvData, UINT uSize);

	void	TransactionProcess(TRANSACTION_INFO* pInfo);
	void	TransactionFailure(TRANSACTION_INFO* pInfo);
	void	TransactionTimeout(TRANSACTION_INFO* pInfo);

	void	SetReversalRetry(TRANSACTION_INFO* pInfo, BOOL bFullUpdate);
	void	SetAdviceRetry(TRANSACTION_INFO* pInfo, BOOL bFullUpdate);

	int		GetSystemTraceNo();

	BOOL	ValidatePassword();

	SCREEN_MODE	GetScreenMode();
	void		SetScreenMode(SCREEN_MODE mode);

	void	ClearFunctionKeys();
	void	SetFunctionKey(UINT nFunctionKey, UINT nChar);
	UINT	GetFunctionKey(UINT nChar);
	UINT	GetFunctionChar(UINT nFunctionKey);

	void	ClearHardwareKeys();
	void	SetHardwareKey(UINT nHardwareKey, UINT nChar);
	UINT	GetHardwareKey(UINT nChar);
	UINT	GetHardwareChar(UINT nHardwareKey);

	static void		cbMSRCallback(DWORD dwArg, LPCTSTR data);

	virtual	BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);

	LRESULT			OnTMS(WPARAM wParam, LPARAM lParam);

	SETTINGS_INFO		m_Settings;
	PARAMETERS_INFO		m_Parameters;
	TMS_UPDATE_INFO		m_UpdateInfo;
	TRANSACTION_INFO	m_TransInfo;

	PRINT_INFO			m_PrintInfo;

	VIEW_MODE			m_ViewMode;
	VIEW_MODE			m_prevViewMode;

	TRANSACTION_MODE	m_TransactionMode;
	TRANSACTION_MODE	m_prevTransactionMode;

	CDCX				m_DC;
	BYTE				m_cTransactionBuffer[512];
	int					m_nTransactionSize;

	CString				m_sInfo[INFO_MAX];
	LONG				m_nInfo[INFO_MAX];

	CComm*				m_pComm;
	MTCTPrinter*		m_pPrinter;

	TRI_STATUS			m_tThreadFirstRun;
	BOOL				m_bThreadForceClose;
	BOOL				m_bThreadDialup;
	BOOL				m_bThreadConnect;
	BOOL				m_bThreadTransaction;
	BOOL				m_bThreadDisconnect;
	BOOL				m_bThreadCancelAdvice;
	int					m_nThreadCount;

	THREAD_STATUS		m_tThreadStatus;
	THREAD_STATUS		m_tPrevThreadStatus;

	THREAD_MODE			m_tThreadMode;
	THREAD_MODE			m_tPrevThreadMode;

	KEY_INJECTION		m_tKeyInjection;

	DWORD				m_dwTimeoutGlanceTick;
	DWORD				m_dwTimeoutTransactionTick;
	DWORD				m_dwTimeoutScreenTick;
	DWORD				m_dwTimeoutMessageTick;
	DWORD				m_dwTimeoutResultTick;
	DWORD				m_dwTimeoutPINTick;
	DWORD				m_dwTimeoutSendParameterTick;

	DWORD				m_dwReversalTick;
	DWORD				m_dwAdviceTick;
	DWORD				m_dwLogonTick;
	DWORD				m_dwAnnunciatorTick;
	DWORD				m_dwStatusTick;

	DWORD				m_dwSentTime;
	DWORD				m_dwReceivedTime;

	TRI_STATUS			m_tReversalStatus;
	TRI_STATUS			m_tAdviceStatus;

	ACTION_REQUESTER	m_ActionRequester;

	CTransactionDatabase	m_database;
	CByteArray				m_arrDetailsType;
	CStringArray			m_arrDetailsFeed;

protected:
	BOOL				m_bTMSRequested;
	BOOL				m_bTMSNeed;

	BATTERY_STATUS		m_BatteryStatus;

	UINT				m_nFNK[MAX_FNK];
	UINT				m_nHWK[MAX_HWK];

	DWORD				m_dwPaintFlag;
	PAINT_FLAGS			m_PaintFlags;
	ANNUNCIATOR_FLAGS	m_AnnunciatorFlags;

	SCREEN_MODE			m_screenMode;

	MENU_INFO			m_menuInfoMain;
	MENU_INFO			m_menuInfoProvider;
	MENU_INFO			m_menuInfoCategory;
	MENU_INFO			m_menuInfoF1;
	MENU_INFO			m_menuInfoReports;
	MENU_INFO			m_menuInfoF2;
	MENU_INFO			m_menuInfoServiceRequest;
	MENU_INFO			m_menuInfoF3;
	MENU_INFO			m_menuInfoPhonebook;
	MENU_INFO			m_menuInfoLanguage;
	MENU_INFO			m_menuInfoSettings;
	MENU_INFO			m_menuInfoTMSSettings;
	MENU_INFO			m_menuInfoConnectionType;

	HANDLE				m_hTransactionThread;
	int					m_nTransactionPriority;
	HANDLE				m_hAnnunciatorThread;
	int					m_nAnnunciatorPriority;

	UINT				m_uExitSeqPos;
	UINT				m_uExitSeq[LENGTH_EXIT_SEQUENCE];
	UINT				m_uExitSteps;

	UINT				m_uResetF3SeqPos;
	UINT				m_uResetF3Seq[LENGTH_RESET_F3_SEQUENCE];
	UINT				m_uResetF3Steps;

	BOOL				m_bLButtonDown;

	CBitmap*			m_bmpDC;

	HBBCARDREADER		m_hMSR;

	CFont				m_fntAnnunciator;
	CFont				m_fntHeaderLarge;
	CFont				m_fntHeaderSmall;
	CFont				m_fntHeaderDesc;
	CFont				m_fntMessage;
	CFont				m_fntDigitLarge;
	CFont				m_fntDigitSmall;
	CFont				m_fntIntroDate;
	CFont				m_fntIntroTime;

	int					m_nSecureStatus;
	BOOL				m_bMasterKey;

	BOOL	IsRightToLeft();

	CString	LoadText(UINT nID);
	CString	LoadText(BILINGUAL text);

#if GPRS_AVAILABLE
	BOOL	GetOperatorName(WCHAR* szName, UINT nSize, LPCWSTR szOperatorCode);
#endif

	void	DrawPNGResource(RES_PNG resPNG, HDC hDC, BOOL bEraseBackground = FALSE, COLORREF cr = COLOR_RGB_WHITE);
	void	GetRectFromPNGResource(LPRECT lprect, RES_PNG resPNG);

	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void	OnPaint();

	afx_msg	LRESULT	OnCardClose(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT	OnCardRestart(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
