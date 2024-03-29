// MTCTPrinter.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "MTCTPrinter.h"
#include "Utilities.h"
#include "StringRes.h"
#include "Jalali.h"
#include "Device.h"
#include "FontCode.h"

#define MAX_RETRY	4

MTCTPrinter::MTCTPrinter()
{
	m_hPrinter		= NULL;
	m_uPrintHeight	= 0;
	m_LangMode		= LANGUAGE_FARSI;
	m_bNewLine		= TRUE;
}

MTCTPrinter::MTCTPrinter(LANGUAGE_MODE langMode)
{
	m_hPrinter		= NULL;
	m_uPrintHeight	= 0;
	m_LangMode		= langMode;
	m_bNewLine		= TRUE;
}

MTCTPrinter::~MTCTPrinter()
{
}

BOOL MTCTPrinter::SetLanguageMode(LANGUAGE_MODE langMode)
{
	m_LangMode	= langMode;

	Close();
	
	return Open();
}

BOOL MTCTPrinter::SetDensity(int nDensity)
{
	return (BOOL)(BBPrinterSetDensity(m_hPrinter, nDensity) == BB_PRINT_ERROR_NONE);
}

BOOL MTCTPrinter::SetLineSpacing(UCHAR nLineSpacing)
{
	return (BOOL)(BBPrinterSetLineSpacing(m_hPrinter, nLineSpacing) == BB_PRINT_ERROR_NONE);
}

BOOL MTCTPrinter::SetFontCodeSet(int nSubCode)
{
	UCHAR	uCodeSet;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			uCodeSet	= 0x0E;
		}
		break;
	default:
		{
			uCodeSet	= 0x00;
		}
		break;
	}

	return (BOOL)(BBPrinterRegisterFontFromSet(m_hPrinter, uCodeSet, nSubCode) == BB_PRINT_ERROR_NONE);
}

void MTCTPrinter::SetNewLine(BOOL bEnable)
{
	m_bNewLine	= bEnable;
}

BOOL MTCTPrinter::Open()
{
	if (m_hPrinter != NULL)
	{
		return TRUE;
	}

	for (int i = 0; i < MAX_RETRY; i++)
	{
		if ((m_hPrinter = BBPrinterOpen(0)) != NULL)
		{
			if (BBPrinterInit(m_hPrinter) != BB_PRINT_ERROR_NONE)
			{
				// Maybe need to reboot the device.
				return FALSE;
			}

			SetDensity(BB_PRINT_DENSITY_80PERCENT);
			SetLineSpacing(2);
			SetFontCodeSet(1);
			return TRUE;
		}

		Sleep(100);
	}

	return FALSE;
}

BOOL MTCTPrinter::Close()
{
	if (BBPrinterClose(m_hPrinter) != 0)
	{
		return FALSE;
	}

	m_hPrinter	= NULL;

	return TRUE;
}

BOOL MTCTPrinter::IsPaperLoaded()
{
#if IGNORE_PAPER_SENSOR
	return TRUE;
#else
	return (BOOL)(BBPrinterGetPaperState(m_hPrinter) == 0);
#endif
}

DWORD MTCTPrinter::WaitUntilDone()
{
	return BBPrinterWaitUntilPrintEnd(m_hPrinter);
}

DWORD MTCTPrinter::PrintText(LPCWSTR szText, DWORD dwOptions)
{
	DWORD	dwResult	= BBPrinterPrint(m_hPrinter, szText, dwOptions);

	if ((!(dwOptions & BB_PRINT_BOTH_ALIGN)) && (m_bNewLine))
	{
		DoLineFeed(0);
	}

	return dwResult;
}

DWORD MTCTPrinter::PrintBitmap(LPCWSTR szPath, DWORD dwOptions)
{
	return BBPrinterPrintBitmap(m_hPrinter, szPath, dwOptions);
}

DWORD MTCTPrinter::PrintBitmap(UINT nID)
{
	BITMAP	bitmap;
	HBITMAP	hBitmap	= LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(nID));

	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	BBPrinterCreateDC(m_hPrinter, bitmap.bmWidth, bitmap.bmHeight);
	BBPrinterDrawBitmap(m_hPrinter, hBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0);

	return BBPrinterPrintDC(m_hPrinter);
}

DWORD MTCTPrinter::PrintBarcode(LPCWSTR szText, UINT uHeight, UINT uWidth, UINT uBarcodeSystem, UINT uAlign)
{
	return BBPrinterPrintBarcode(m_hPrinter, szText, uHeight, uWidth, uBarcodeSystem, uAlign);
}

DWORD MTCTPrinter::DoLineFeed(DWORD dwOptions)
{
	return BBPrinterPrint(m_hPrinter, L"\n", dwOptions);
}

DWORD MTCTPrinter::DoLineFeedForAligned()
{
	DWORD	dwOptions;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			dwOptions	= BB_PRINT_RIGHTALIGN;
		}
		break;
	default:
		{
			dwOptions	= BB_PRINT_DEFAULT;
		}
		break;
	}

	return DoLineFeed(dwOptions);
}

DWORD MTCTPrinter::DoLineFeedForReversed()
{
	DWORD	dwOptions;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			dwOptions	= BB_PRINT_DEFAULT;
		}
		break;
	default:
		{
			dwOptions	= BB_PRINT_RIGHTALIGN;
		}
		break;
	}

	return DoLineFeed(dwOptions);
}

DWORD MTCTPrinter::DoPrintOut()
{
	m_uPrintHeight	= 0;

	return WaitUntilDone();
}

DWORD MTCTPrinter::PrintTitle(CString strText, UINT uID, DWORD dwSizeOptions)
{
	if ((strText.IsEmpty()) && (uID != IDS_NOSTRING))
	{
		strText	= LoadText(uID, m_LangMode);
	}

	return PrintText(strText, BB_PRINT_CENTERALIGN | dwSizeOptions);
}

DWORD MTCTPrinter::PrintKeyValuePair(LPCWSTR szKey, LPCWSTR szValue, DWORD dwOptions)
{
	CString	strText;

	strText.Append(szKey);
	strText.Append(L"\t");
	strText.Append(szValue);

	return PrintText(strText, BB_PRINT_BOTH_ALIGN | dwOptions);
}

DWORD MTCTPrinter::PrintStarLine()
{
	CString	strText;
	
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText	= L"********************************";
		}
		break;
	default:
		{
			strText	= L"****************************************";
		}
		break;
	}

	return PrintText(strText, BB_PRINT_CENTERALIGN);
}

DWORD MTCTPrinter::PrintBarLine()
{
	CString	strText;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText	= L"--------------------------------";
		}
		break;
	default:
		{
			strText	= L"----------------------------------------";
		}
		break;
	}

	return PrintText(strText, BB_PRINT_CENTERALIGN);
}

DWORD MTCTPrinter::PrintHalfLine()
{
	CString	strText;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText	= L"----------------";
		}
		break;
	default:
		{
			strText	= L"--------------------";
		}
		break;
	}

	return PrintText(strText, BB_PRINT_CENTERALIGN);
}

DWORD MTCTPrinter::PrintTextEx(CString strText, BOOL bDigit, BOOL bComma, UINT uID, DWORD dwOptions)
{
	if (strText.IsEmpty() && (uID != IDS_NOSTRING))
	{
		strText	= LoadText(uID, m_LangMode);
	}

	if (bDigit)
	{
		CString	strDigit	= FormattingDigit(strText, bComma, m_LangMode);

		strText	= strDigit;
	}

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			switch (dwOptions)
			{
			case BB_PRINT_EX_ALIGNED:
				{
					dwOptions	= BB_PRINT_RIGHTALIGN;
				}
				break;
			case BB_PRINT_EX_REVERSED:
				{
					dwOptions	= BB_PRINT_DEFAULT;
				}
				break;
			default:
				{
				}
				break;
			}
		}
		break;
	default:
		{
			switch (dwOptions)
			{
			case BB_PRINT_EX_ALIGNED:
				{
					dwOptions	= BB_PRINT_DEFAULT;
				}
				break;
			case BB_PRINT_EX_REVERSED:
				{
					dwOptions	= BB_PRINT_RIGHTALIGN;
				}
				break;
			default:
				{
				}
				break;
			}
		}
		break;
	}

	return PrintText(strText, dwOptions);
}

DWORD MTCTPrinter::PrintTextEx(BILINGUAL text, BOOL bDigit, BOOL bComma, DWORD dwOptions)
{
	CString strText;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText	= text.szF;
		}
		break;
	default:
		{
			strText	= text.szE;
		}
		break;
	}

	return PrintTextEx(strText, bDigit, bComma, IDS_NOSTRING, dwOptions);
}

DWORD MTCTPrinter::PrintKeyValuePairEx(CString strKey, CString strValue, BOOL bDigit, BOOL bComma, UINT uKeyID, UINT uValueID, DWORD dwOptions)
{
	if (strKey.IsEmpty() && (uKeyID != IDS_NOSTRING))
	{
		switch (m_LangMode)
		{
		case LANGUAGE_FARSI:
			{
				strKey	= LoadFarsiText(uKeyID);
			}
			break;
		default:
			{
				strKey	= LoadEnglishText(uKeyID);
			}
			break;
		}
	}

	if (strValue.IsEmpty() && (uValueID != IDS_NOSTRING))
	{
		switch (m_LangMode)
		{
		case LANGUAGE_FARSI:
			{
				strValue	= LoadFarsiText(uValueID);
			}
			break;
		default:
			{
				strValue	= LoadEnglishText(uValueID);
			}
			break;
		}
	}

	if (bDigit)
	{
		CString	strDigit	= FormattingDigit(strValue, bComma, m_LangMode);

		strValue	= strDigit;
	}

	return PrintKeyValuePair(strKey, strValue, dwOptions);
}

DWORD MTCTPrinter::PrintKeyValuePairEx(BILINGUAL key, CString strValue, BOOL bDigit, BOOL bComma, UINT uValueID, DWORD dwOptions)
{
	CString	strKey;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strKey	= key.szF;
		}
		break;
	default:
		{
			strKey	= key.szE;
		}
		break;
	}

	return PrintKeyValuePairEx(strKey, strValue, bDigit, bComma, IDS_NOSTRING, uValueID, dwOptions);
}

DWORD MTCTPrinter::PrintKeyValuePairEx(CString strKey, BILINGUAL value, BOOL bDigit, BOOL bComma, UINT uKeyID, DWORD dwOptions)
{
	CString	strValue;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strValue	= value.szF;
		}
		break;
	default:
		{
			strValue	= value.szE;
		}
		break;
	}

	return PrintKeyValuePairEx(strKey, strValue, bDigit, bComma, uKeyID, IDS_NOSTRING, dwOptions);
}

DWORD MTCTPrinter::PrintKeyValuePairEx(BILINGUAL key, BILINGUAL value, BOOL bDigit, BOOL bComma, DWORD dwOptions)
{
	CString	strKey;
	CString	strValue;

	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strKey		= key.szF;
			strValue	= value.szF;
		}
		break;
	default:
		{
			strKey		= key.szE;
			strValue	= value.szE;
		}
		break;
	}

	return PrintKeyValuePairEx(strKey, strValue, bDigit, bComma, IDS_NOSTRING, IDS_NOSTRING, dwOptions);
}

DWORD MTCTPrinter::PrintReceiptHeader(PARAMETERS_INFO* pParameters)
{
	// Shaparak Logo
	PrintBitmap(IDB_SHAPARAK);

	// Shop Information
	PrintTextEx(pParameters->merchantName, TRUE, FALSE);
	PrintTextEx(pParameters->szMerchantPhone, TRUE, FALSE);
	PrintTextEx(pParameters->szMerchantPostal, TRUE, FALSE);

	// Terminal Information
#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_TERMINAL_ID, TRUE, FALSE, IDS_TERMINAL_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szTerminalID, TRUE, FALSE, IDS_TERMINAL_ID);
#endif

#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_MERCHANT_ID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szMerchantID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#endif

	return BB_PRINT_ERROR_NONE;
}

DWORD MTCTPrinter::PrintReceiptHeader(PRINT_INFO* pPrintInfo)
{
	return PrintReceiptHeader(pPrintInfo->pParameters);
}

DWORD MTCTPrinter::PrintReceiptBody(PRINT_INFO* pPrintInfo, BOOL bAcceptor)
{
	DWORD	dwResult;
	CString	strPath;

	// Receipt Body
	switch (pPrintInfo->ReceiptType)
	{
	case RECEIPT_SALE:
		{
			dwResult	= PrintReceiptSale(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_BALANCE:
		{
			dwResult	= PrintReceiptBalance(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_BILL_PAYMENT:
		{
			dwResult	= PrintReceiptBillPayment(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_CHARGE:
		{
			dwResult	= PrintReceiptCharge(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_SELL_CHARGE:
		{
			dwResult	= PrintReceiptSellCharge(&pPrintInfo->ChargeInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_SERVICE_REQUEST:
		{
			dwResult	= PrintReceiptServiceRequest(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_UNSUCCESSFUL:
		{
			dwResult	= PrintReceiptUnsuccessful(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	case RECEIPT_TIMEOUT:
		{
			dwResult	= PrintReceiptTimeout(&pPrintInfo->TransactionInfo, pPrintInfo->pParameters, bAcceptor);
		}
		break;
	}

	return dwResult;
}

DWORD MTCTPrinter::PrintReceiptHeaderAndBody(PRINT_INFO* pPrintInfo, BOOL bAcceptor)
{
	PrintReceiptHeader(pPrintInfo);

	return PrintReceiptBody(pPrintInfo, bAcceptor);
}

DWORD MTCTPrinter::PrintReceiptSale(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			SetNewLine(FALSE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
			SetNewLine(TRUE);
			PrintTitle(L"", IDS_PRINT_PURCHASE);
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_PURCHASE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Card ID
	PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

	// Bank Issuer
	PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	// Reference No.
	PrintKeyValuePairEx(L"", pInfo->szReferenceNo, TRUE, FALSE, IDS_REFERENCE_NO);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Amount
	PrintKeyValuePairEx(L"", pInfo->szAmount, TRUE, TRUE, IDS_AMOUNT_RIALS, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT);

	// Additional Text
	if (wcslen(pInfo->szAdditionalData))
	{
		PrintText(pInfo->szAdditionalData, BB_PRINT_DEFAULT);
	}

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptBalance(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			SetNewLine(FALSE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
			SetNewLine(TRUE);
			PrintTitle(L"", IDS_PRINT_BALANCE);
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_BALANCE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Card ID
	PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

	// Bank Issuer
	PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	// Reference No.
	PrintKeyValuePairEx(L"", pInfo->szReferenceNo, TRUE, FALSE, IDS_REFERENCE_NO);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Amount
	PrintKeyValuePairEx(L"", pInfo->szAmount, TRUE, TRUE, IDS_BALANCE_RIALS, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT);

	// Additional Text
	if (wcslen(pInfo->szAdditionalData))
	{
		PrintText(pInfo->szAdditionalData, BB_PRINT_DEFAULT);
	}

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptBillPayment(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			SetNewLine(FALSE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
			SetNewLine(TRUE);
			PrintTitle(L"", IDS_PRINT_BILL_PAYMENT, BB_PRINT_DOUBLEHEIGHT);
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_BILL_PAYMENT);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Bill ID
	PrintKeyValuePairEx(L"", pInfo->szBillID, TRUE, FALSE, IDS_BILL_ID);

	// Payment ID
	PrintKeyValuePairEx(L"", pInfo->szPaymentID, TRUE, FALSE, IDS_PAYMENT_ID);

	// Service Code Name
	PrintTextEx(GetServiceCodeName(pInfo->nServiceCode, pParameters), FALSE, FALSE);

	// Card ID
	PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

	// Bank Issuer
	PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	// Reference No.
	PrintKeyValuePairEx(L"", pInfo->szReferenceNo, TRUE, FALSE, IDS_REFERENCE_NO);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Amount
	PrintKeyValuePairEx(L"", pInfo->szAmount, TRUE, TRUE, IDS_AMOUNT_RIALS, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT);

	// Additional Text
	if (wcslen(pInfo->szAdditionalData))
	{
		PrintText(pInfo->szAdditionalData, BB_PRINT_DEFAULT);
	}

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptCharge(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			SetNewLine(FALSE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
			SetNewLine(TRUE);
			PrintTitle(L"", IDS_PRINT_CHARGE_BUY);
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_CHARGE_BUY);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Card ID
	PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

	// Bank Issuer
	PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);

	// Charge Provider
	strText	= L"++++++++";
	
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText.Append(GetProviderName(pInfo->szProviderID, pParameters).szF);
		}
		break;
	default:
		{
			strText.Append(GetProviderName(pInfo->szProviderID, pParameters).szE);
		}
		break;
	}

	strText.Append(L"\t++++++++");

	PrintText(strText, BB_PRINT_BOTH_ALIGN);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	// Reference No.
	PrintKeyValuePairEx(L"", pInfo->szReferenceNo, TRUE, FALSE, IDS_REFERENCE_NO);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Amount
	PrintKeyValuePairEx(L"", pInfo->szAmount, TRUE, TRUE, IDS_AMOUNT_RIALS, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT);

	// Serial
	PrintTextEx(L"", FALSE, FALSE, IDS_SERIAL, BB_PRINT_EX_ALIGNED);
	
	for (int i = 0; i < (int)pInfo->nChargeCount; i++)
	{
		strText	= pInfo->szSerial[i];

		PrintTextEx(strText, TRUE, FALSE, IDS_NOSTRING, BB_PRINT_EX_REVERSED);
	}

	// Additional Text
	if (wcslen(pInfo->szAdditionalData))
	{
		PrintText(pInfo->szAdditionalData, BB_PRINT_DEFAULT);
	}

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptSellCharge(CHARGE_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			SetNewLine(FALSE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
			SetNewLine(TRUE);
			PrintTitle(L"", IDS_PRINT_SELL_CHARGE, BB_PRINT_DOUBLEHEIGHT);
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_SELL_CHARGE);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Charge Provider
	strText	= L"++++++++";
	
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			strText.Append(GetProviderName(pInfo->szProviderID, pParameters).szF);
		}
		break;
	default:
		{
			strText.Append(GetProviderName(pInfo->szProviderID, pParameters).szE);
		}
		break;
	}

	strText.Append(L"\t++++++++");

	PrintText(strText, BB_PRINT_BOTH_ALIGN);

	// Charge Serial
	PrintKeyValuePairEx(L"", pInfo->szSerial, TRUE, FALSE, IDS_SERIAL);

	// Charge Code
	PrintKeyValuePairEx(L"", pInfo->szChargePIN, TRUE, FALSE, IDS_CODE);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	// Reference No.
	PrintKeyValuePairEx(L"", pInfo->szReferenceNo, TRUE, FALSE, IDS_REFERENCE_NO);

	PrintBarLine();

	// Amount
	PrintKeyValuePairEx(L"", pInfo->szAmount, TRUE, TRUE, IDS_AMOUNT_RIALS, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT);

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptServiceRequest(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			PrintTitle(L"", IDS_PRINT_SERVICE_REQUEST);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	default:
		{
			PrintTitle(L"", IDS_PRINT_SERVICE_REQUEST);
			PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));
		}
		break;
	}

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptUnsuccessful(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (pInfo->transactionMode)
	{
	case TRANSACTION_SALE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_PURCHASE, m_LangMode));
		}
		break;
	case TRANSACTION_BALANCE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_BALANCE, m_LangMode));
		}
		break;
	case TRANSACTION_BILL_PAYMENT:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_BILL_PAYMENT, m_LangMode));
		}
		break;
	case TRANSACTION_CHARGE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_CHARGE_BUY, m_LangMode));
		}
		break;
	case TRANSACTION_SERVICE_REQUEST:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_SERVICE_REQUEST, m_LangMode));
		}
		break;
	default:
		{
			strText	= LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode);
		}
		break;
	}

	PrintTextEx(strText, FALSE, FALSE, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT | BB_PRINT_CENTERALIGN);
	PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));

	if (pInfo->transactionMode != TRANSACTION_SERVICE_REQUEST)
	{
		// Card ID
		PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

		// Bank Issuer
		PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);
	}

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	PrintBarLine();

	// Response Code
	PrintKeyValuePairEx(L"", GetResponseText(_wtoi(pInfo->szResponseCode), m_LangMode, pParameters), TRUE, FALSE, IDS_RESPONSE_CODE);

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintReceiptTimeout(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor)
{
	CString	strText;
	WCHAR	szVersion[16];

	// Title
	switch (pInfo->transactionMode)
	{
	case TRANSACTION_SALE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_PURCHASE, m_LangMode));
		}
		break;
	case TRANSACTION_BALANCE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_BALANCE, m_LangMode));
		}
		break;
	case TRANSACTION_BILL_PAYMENT:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_BILL_PAYMENT, m_LangMode));
		}
		break;
	case TRANSACTION_CHARGE:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_CHARGE_BUY, m_LangMode));
		}
		break;
	case TRANSACTION_SERVICE_REQUEST:
		{
			strText.Format(L"%s-%s", LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode), LoadText(IDS_PRINT_SERVICE_REQUEST, m_LangMode));
		}
		break;
	default:
		{
			strText	= LoadText(IDS_PRINT_UNSUCCESSFUL, m_LangMode);
		}
		break;
	}

	PrintTextEx(strText, FALSE, FALSE, IDS_NOSTRING, BB_PRINT_DOUBLEHEIGHT | BB_PRINT_CENTERALIGN);
	PrintTextEx(L"", FALSE, FALSE, (bAcceptor ? IDS_ACCEPTOR_RECEIPT : IDS_CUSTOMER_RECEIPT));

	// Card ID
	PrintKeyValuePairEx(L"", BlindMarkPAN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data), m_LangMode), TRUE, FALSE, IDS_CARD_ID);

	// Bank Issuer
	PrintKeyValuePairEx(L"", GetBankName(GetBankBIN(ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data)), pParameters), FALSE, FALSE, IDS_BANK_NAME);

	// Trace No
	PrintKeyValuePairEx(L"", pInfo->szTraceNo, TRUE, FALSE, IDS_TRACE_NO);

	// Date/Time
	switch (m_LangMode)
	{
	case LANGUAGE_FARSI:
		{
			CJalali	jalali;
			int		nYear;
			int		nMonth;
			int		nDay;

			jalali.GetJalaliDate(pInfo->stTimestamp, &nYear, &nMonth, &nDay);
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", nYear % 100, nMonth, nDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	default:
		{
			strText.Format(L"%02d/%02d/%02d %02d:%02d:%02d", pInfo->stTimestamp.wYear % 100, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
		}
		break;
	}

	PrintKeyValuePairEx(L"", strText, TRUE, FALSE, IDS_DATE_TIME);

	PrintBarLine();

	// Customer Message
	SetFontCodeSet(m_LangMode == LANGUAGE_FARSI ? 0 : 1);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_1, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_2, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_3, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_4, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_5, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_6, BB_PRINT_EX_ALIGNED);
	PrintTextEx(L"", TRUE, FALSE, IDS_TIMEOUT_MESSAGE_7, BB_PRINT_EX_ALIGNED);
	SetFontCodeSet(1);

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintTransactionDetails(CByteArray& arrTypes, CStringArray& arrFeeds, PARAMETERS_INFO* pParameters)
{
	CString	strPath;
	WCHAR	szVersion[16];

	// Shaparak Logo
	PrintBitmap(IDB_SHAPARAK);

	// Shop Information
	PrintTextEx(pParameters->merchantName, FALSE, FALSE);
	PrintTextEx(pParameters->szMerchantPhone, TRUE, FALSE);
	PrintTextEx(pParameters->szMerchantPostal, TRUE, FALSE);
	DoLineFeed(BB_PRINT_BOTH_ALIGN);

	// Terminal Information
#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_TERMINAL_ID, TRUE, FALSE, IDS_TERMINAL_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szTerminalID, TRUE, FALSE, IDS_TERMINAL_ID);
#endif

#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_MERCHANT_ID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szMerchantID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#endif

	DoLineFeed();

	// Title
	PrintTitle(L"", IDS_PRINT_TRANSACTION_DETAILS);
	DoLineFeed();

	for (int i = 0; i < arrTypes.GetCount(); i++)
	{
		switch (arrTypes.GetAt(i))
		{
		case 'A':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE, IDS_NOSTRING, BB_PRINT_EX_ALIGNED);
			}
			break;
		case 'a':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE, IDS_NOSTRING, BB_PRINT_EX_ALIGNED);
			}
			break;
		case 'V':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE, IDS_NOSTRING, BB_PRINT_EX_REVERSED);
			}
			break;
		case 'v':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE, IDS_NOSTRING, BB_PRINT_EX_REVERSED);
			}
			break;
		case 'B':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE, IDS_NOSTRING, BB_PRINT_BOTH_ALIGN);
			}
			break;
		case 'b':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE, IDS_NOSTRING, BB_PRINT_BOTH_ALIGN);
			}
			break;
		case 'L':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE, IDS_NOSTRING, BB_PRINT_DEFAULT);
			}
			break;
		case 'l':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE, IDS_NOSTRING, BB_PRINT_DEFAULT);
			}
			break;
		case 'C':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE);
			}
			break;
		case 'c':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE);
			}
			break;
		case 'R':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, FALSE, IDS_NOSTRING, BB_PRINT_RIGHTALIGN);
			}
			break;
		case 'r':
			{
				PrintTextEx(arrFeeds.GetAt(i), TRUE, TRUE, IDS_NOSTRING, BB_PRINT_RIGHTALIGN);
			}
			break;
		case 'N':
			{
				SetNewLine(TRUE);
			}
			break;
		case 'n':
			{
				SetNewLine(FALSE);
			}
			break;
		case ' ':
			{
				DoLineFeed(BB_PRINT_BOTH_ALIGN);
			}
			break;
		case '*':
			{
				PrintStarLine();
			}
			break;
		case '=':
			{
				PrintBarLine();
			}
			break;
		case '-':
			{
				PrintHalfLine();
			}
			break;
		case '0':
			{
				SetFontCodeSet(0);
			}
			break;
		case '1':
			{
				SetFontCodeSet(1);
			}
			break;
		case '2':
			{
				SetFontCodeSet(2);
			}
			break;
		}
	}

	// Sadad Logo
	PrintBitmap(IDB_SADAD);

	// Customer SOS
	PrintKeyValuePairEx(L"", pParameters->szSupportPhone, TRUE, FALSE, IDS_CUSTOMER_SOS);

	// Version
	GetAppVersion(szVersion, 16);
	SetFontCodeSet(2);
	PrintKeyValuePairEx(L"", szVersion, TRUE, FALSE, IDS_RECEIPT_VERSION, IDS_NOSTRING);
	SetFontCodeSet(1);

	// Feed Out
	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}

DWORD MTCTPrinter::PrintPOSInformation(PARAMETERS_INFO* pParameters)
{
	CString	strPath;
	CString	strText;
	WCHAR	szBuffer[64];
	WCHAR	wcsAsciiFontCode[FONT_ASCII_END - FONT_ASCII_START + 1];
	WCHAR	wcsFarsiFontCode[FONT_FARSI_END - FONT_FARSI_START + 1];

	memset(wcsAsciiFontCode, 0, sizeof(wcsAsciiFontCode));
	memset(wcsFarsiFontCode, 0, sizeof(wcsFarsiFontCode));

	// Shaparak Logo
//	PrintBitmap(IDB_SHAPARAK);
	PrintBitmap(IDB_SHAPARAK);

	// Shop Information
	PrintTextEx(pParameters->merchantName, FALSE, FALSE);
	PrintTextEx(pParameters->szMerchantPhone, TRUE, FALSE);
	PrintTextEx(pParameters->szMerchantPostal, TRUE, FALSE);

	// Terminal Information
#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_TERMINAL_ID, TRUE, FALSE, IDS_TERMINAL_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szTerminalID, TRUE, FALSE, IDS_TERMINAL_ID);
#endif

#if TEST_MODE
	PrintKeyValuePairEx(L"", TEST_MERCHANT_ID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#else
	PrintKeyValuePairEx(L"", pParameters->szMerchantID, TRUE, FALSE, IDS_ACCEPTOR_ID);
#endif

	// Title
	PrintStarLine();
	PrintTitle(L"", IDS_PRINT_POS_INFORMATION);
	PrintStarLine();

	// Information
	GetModel(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"Model", szBuffer, FALSE, FALSE);
	GetDeviceType(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"Type", szBuffer, FALSE, FALSE);
	GetMAXQVersion(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"Secure AP Ver", szBuffer, FALSE, FALSE);
	GetROMVersion(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"Image Ver", szBuffer, FALSE, FALSE);
	GetPsVersion(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"PS Ver", szBuffer, FALSE, FALSE);
	GetAppVersion(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"App Ver", szBuffer, FALSE, FALSE);
	GetSerialNumber(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"POS S/N", szBuffer, FALSE, FALSE, IDS_NOSTRING);
	GetImageCode(szBuffer, sizeof(szBuffer));
	PrintKeyValuePairEx(L"Image Code", szBuffer, FALSE, FALSE, IDS_NOSTRING);

	// System Font
	PrintText(L"[System Font]", BB_PRINT_DEFAULT);

	for (int i = 0; i < (FONT_ASCII_END - FONT_ASCII_START); i++)
	{
		wcsAsciiFontCode[i]	= FONT_ASCII_START+i;
	}

	PrintText(wcsAsciiFontCode);

	for (int i = 0, j = 0; i < (FONT_FARSI_END - FONT_FARSI_START); i++)
	{
		if (FONT_FARSI_START + i == EmptyFarsiFontCode[j])
		{
			j++;
			continue;
		}

		wcsFarsiFontCode[i - j]	= FONT_FARSI_START + i;
	}

	SetFontCodeSet(0);
	PrintText(wcsFarsiFontCode, BB_PRINT_CENTERALIGN);
	SetFontCodeSet(1);

	// Printer Information
	PrintText(L"[Printer Information]", BB_PRINT_DEFAULT);
	PrintText(L" 32 characters/line", BB_PRINT_DEFAULT);
	PrintText(L" 40 characters/line", BB_PRINT_DEFAULT);
	PrintText(L" 42 characters/line", BB_PRINT_DEFAULT);

	DoLineFeedForAligned();

	// Barcode Data
	PrintText(L"[Barcode Data]", BB_PRINT_DEFAULT);
	PrintBarcode(L"abcdefg~!@#$%^&*()_+01234567890:ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	// Sadad Logo
	PrintText(L"[LOGO Image]", BB_PRINT_DEFAULT);
	PrintBitmap(IDB_SADAD);

	DoLineFeed();
	DoLineFeed();

	return DoPrintOut();
}
