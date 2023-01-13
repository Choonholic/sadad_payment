// MTCTPrinter.h

#pragma once

#include "bbappapi.h"
#include "Definitions.h"
#include "StringRes.h"
#include "Settings.h"

#ifndef BB_PRINT_EX_OPTIONS
#define BB_PRINT_EX_OPTIONS		0x00FF0000
#define BB_PRINT_EX_ALIGNED		0x00010000
#define BB_PRINT_EX_REVERSED	0x00020000
#endif

class MTCTPrinter
{
public:
	MTCTPrinter();
	MTCTPrinter(LANGUAGE_MODE langMode);
	~MTCTPrinter();

	BOOL	SetLanguageMode(LANGUAGE_MODE langMode);
	BOOL	SetDensity(int nDensity);
	BOOL	SetLineSpacing(UCHAR nLineSpacing);
	BOOL	SetFontCodeSet(int nSubCode);
	void    SetNewLine(BOOL bEnable);

	BOOL	Open();
	BOOL	Close();
		
	BOOL	IsPaperLoaded();

	DWORD	WaitUntilDone();

	DWORD	PrintText(LPCWSTR szText, DWORD dwOptions = BB_PRINT_CENTERALIGN);
	DWORD	PrintBitmap(LPCWSTR szPath, DWORD dwOptions = BB_PRINT_BITMAP_ALIGN_CENTER);
	DWORD	PrintBitmap(UINT nID);
	DWORD	PrintBarcode(LPCWSTR szText, UINT uHeight = 100, UINT uWidth = BB_PRINT_BARCODE_WIDTH_NORMAL, UINT uBarcodeSystem = BB_PRINT_BARCODE_CODE128, UINT uAlign = BB_PRINT_BARCODE_ALIGN_CENTER);

	DWORD	DoLineFeed(DWORD dwOptions = BB_PRINT_CENTERALIGN);
	DWORD	DoLineFeedForAligned();
	DWORD	DoLineFeedForReversed();

	DWORD	DoPrintOut();

	DWORD	PrintTitle(CString strText, UINT uID = IDS_NOSTRING, DWORD dwSizeOptions = BB_PRINT_DOUBLESIZE);
	DWORD	PrintKeyValuePair(LPCWSTR szKey, LPCWSTR szValue, DWORD dwOptions = 0);

	DWORD	PrintStarLine();
	DWORD	PrintBarLine();
	DWORD	PrintHalfLine();

	DWORD	PrintTextEx(CString strText, BOOL bDigit, BOOL bComma, UINT uID = IDS_NOSTRING, DWORD dwOptions = BB_PRINT_CENTERALIGN);
	DWORD	PrintTextEx(BILINGUAL text, BOOL bDigit, BOOL bComma, DWORD dwOptions = BB_PRINT_CENTERALIGN);

	DWORD	PrintKeyValuePairEx(CString strKey, CString strValue, BOOL bDigit, BOOL bComma, UINT uKeyID = IDS_NOSTRING, UINT uValueID = IDS_NOSTRING, DWORD dwOptions = 0);
	DWORD	PrintKeyValuePairEx(BILINGUAL key, CString strValue, BOOL bDigit, BOOL bComma, UINT uValueID = IDS_NOSTRING, DWORD dwOptions = 0);
	DWORD	PrintKeyValuePairEx(CString strKey, BILINGUAL value, BOOL bDigit, BOOL bComma, UINT uKeyID = IDS_NOSTRING, DWORD dwOptions = 0);
	DWORD	PrintKeyValuePairEx(BILINGUAL key, BILINGUAL value, BOOL bDigit, BOOL bComma, DWORD dwOptions = 0);

	DWORD	PrintReceiptHeader(PARAMETERS_INFO* pParameters);
	DWORD	PrintReceiptHeader(PRINT_INFO* pPrintInfo);
	DWORD	PrintReceiptBody(PRINT_INFO* pPrintInfo, BOOL bAcceptor = FALSE);
	DWORD	PrintReceiptHeaderAndBody(PRINT_INFO* pPrintInfo, BOOL bAcceptor = FALSE);

	DWORD	PrintReceiptSale(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptBalance(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptBillPayment(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptCharge(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptSellCharge(CHARGE_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptServiceRequest(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptUnsuccessful(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);
	DWORD	PrintReceiptTimeout(TRANSACTION_INFO* pInfo, PARAMETERS_INFO* pParameters, BOOL bAcceptor);

	DWORD	PrintTransactionDetails(CByteArray& arrTypes, CStringArray& arrFeeds, PARAMETERS_INFO* pParameters);

	DWORD	PrintPOSInformation(PARAMETERS_INFO* pParameters);

protected:
	HBBPRINTER		m_hPrinter;
	UINT			m_uPrintHeight;
	LANGUAGE_MODE	m_LangMode;
	BOOL			m_bNewLine;
};
