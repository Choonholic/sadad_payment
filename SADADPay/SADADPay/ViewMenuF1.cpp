// ViewMenuF1.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "StringRes.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "Jalali.h"

void CChildView::BuildTransactionDetails(CTransactionDatabase& database, WCHAR* szStartDate, WCHAR* szEndDate, CByteArray& arrType, CStringArray& arrFeed)
{
	CString				strText;
	CString				strValue;
	CString				strFormatTotal;
	CJalali				jalali;
	SYSTEMTIME			st;
	int					nY;
	int					nM;
	int					nD;
	CString				strStartDate	= szStartDate;
	CString				strEndDate		= szEndDate;
	int					nCountSale		= 0;
	int					nCountBill		= 0;
	int					nCountCharge	= 0;
	LONGLONG			llTotalSale		= 0;
	LONGLONG			llTotalBill		= 0;
	LONGLONG			llTotalCharge	= 0;
	LONGLONG			llTotalDaily	= 0;
	int					nCount			= 0;
	TRANSACTION_INFO*	pInfo;

	arrType.RemoveAll();
	arrFeed.RemoveAll();

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			strFormatTotal	= L"%d";
		}
		break;
	default:
		{
			strFormatTotal	= L"%13d";
		}
		break;
	}

	arrType.Add('1');
	arrFeed.Add(L"");

	// Start Date and End Date
	arrType.Add('C');

	memset(&st, 0, sizeof(SYSTEMTIME));

	st.wYear	= (WORD)_wtoi(strStartDate.Mid(0, 4));
	st.wMonth	= (WORD)_wtoi(strStartDate.Mid(4, 2));
	st.wDay		= (WORD)_wtoi(strStartDate.Mid(6, 2));

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			jalali.GetJalaliDate(st, &nY, &nM, &nD);
		}
		break;
	default:
		{
			nY	= st.wYear;
			nM	= st.wMonth;
			nD	= st.wDay;
		}
		break;
	}

	strValue.Format(L"%02d/%02d/%02d", nY, nM, nD);

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			strText	= L" - " + strValue;
		}
		break;
	default:
		{
			strText	= strValue + L" - ";
		}
		break;
	}

	st.wYear	= (WORD)_wtoi(strEndDate.Mid(0, 4));
	st.wMonth	= (WORD)_wtoi(strEndDate.Mid(4, 2));
	st.wDay		= (WORD)_wtoi(strEndDate.Mid(6, 2));

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			jalali.GetJalaliDate(st, &nY, &nM, &nD);
			strValue.Format(L"%02d/%02d/%02d", nY, nM, nD);

			strText	= strValue + strText;
		}
		break;
	default:
		{
			nY	= st.wYear;
			nM	= st.wMonth;
			nD	= st.wDay;

			strValue.Format(L"%02d/%02d/%02d", nY, nM, nD);

			strText	+= strValue;
		}
		break;
	}

	arrFeed.Add(strText);

	strStartDate	= L"";

	arrType.Add('0');
	arrFeed.Add(L"");
	arrType.Add('n');
	arrFeed.Add(L"");

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_NO));
			arrType.Add('C');
			arrFeed.Add(L"   ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TRACENO));
			arrType.Add('C');
			arrFeed.Add(L" ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TIME));
			arrType.Add('C');
			arrFeed.Add(L"      ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_AMOUNT));
			arrType.Add('C');
			arrFeed.Add(L"  ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_CARDNO));
			arrType.Add('C');
			arrFeed.Add(L" ");
			arrType.Add('N');
			arrFeed.Add(L"");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TYPE));
		}
		break;
	default:
		{
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TYPE));
			arrType.Add('C');
			arrFeed.Add(L" ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_CARDNO));
			arrType.Add('C');
			arrFeed.Add(L"   ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_AMOUNT));
			arrType.Add('C');
			arrFeed.Add(L"    ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TIME));
			arrType.Add('C');
			arrFeed.Add(L" ");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_TRACENO));
			arrType.Add('C');
			arrFeed.Add(L"   ");
			arrType.Add('N');
			arrFeed.Add(L"");
			arrType.Add('C');
			arrFeed.Add(LoadText(IDS_DETAILS_NO));
		}
		break;
	}

	arrType.Add('1');
	arrFeed.Add(L"");

	for (int i = 0; i < database.m_arrTransaction.GetCount(); i++)
	{
		pInfo	= &database.m_arrTransaction.GetAt(i);

		switch (pInfo->transactionMode)
		{
		case TRANSACTION_SALE:
		case TRANSACTION_BILL_PAYMENT:
		case TRANSACTION_CHARGE:
			break;
		default:
			continue;
		}

		switch (GetTransactionStatus(pInfo))
		{
		case STATUS_ADVICE:
		case STATUS_COMPLETED:
			break;
		default:
			continue;
		}

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				jalali.GetJalaliDate(pInfo->stTimestamp, &nY, &nM, &nD);
			}
			break;
		default:
			{
				nY	= pInfo->stTimestamp.wYear;
				nM	= pInfo->stTimestamp.wMonth;
				nD	= pInfo->stTimestamp.wDay;
			}
			break;
		}

		strEndDate.Format(L"%04d/%02d/%02d", nY, nM, nD);

		if (strStartDate.Compare(strEndDate))
		{
			if (nCount != 0)
			{
				arrType.Add('1');
				arrFeed.Add(L"");

				strValue.Format(strFormatTotal, llTotalDaily);

				strText	= AddComma(strValue);

				arrType.Add('=');
				arrFeed.Add(L"");
				arrType.Add('R');
				arrFeed.Add(strText);
				arrType.Add(' ');
				arrFeed.Add(L"");
			}

			nCount			= 0;
			llTotalDaily	= 0;
			strStartDate	= strEndDate;

			arrType.Add('L');
			arrFeed.Add(strStartDate);
			arrType.Add('=');
			arrFeed.Add(L"");
		}

		switch (pInfo->transactionMode)
		{
		case TRANSACTION_SALE:
			{
				strText			= L"S";
				llTotalSale		+= _wtoll(pInfo->szAmount);
				nCountSale++;
			}
			break;
		case TRANSACTION_BILL_PAYMENT:
			{
				strText			= L"B";
				llTotalBill		+= _wtoll(pInfo->szAmount);
				nCountBill++;
			}
			break;
		case TRANSACTION_CHARGE:
			{
				strText			= L"C";
				llTotalCharge	+= _wtoll(pInfo->szAmount);
				nCountCharge++;
			}
			break;
		}

		strValue	= ExtractPANFromTrack2DataLeft(pInfo->szTrack2Data);

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				strText	= strValue.Right(4) + strText;
			}
			break;
		default:
			{
				strText	+= strValue.Right(4);
			}
			break;
		}

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				strText	= L" " + strText;

				strValue.Format(L"%12d", _wtoi(pInfo->szAmount));

				strEndDate	= AddComma(strValue);

				strValue.Format(L"%s", strEndDate);

				strText	= strValue.Right(11) + strText;
				strText	= L" " + strText;

				strEndDate.Format(L"%02d%02d", pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute);

				strText	= L" " + strEndDate + strText;
				strText	= pInfo->szTraceNo + strText;
				strText	= L" " + strText;

				strValue.Format(L"%-3d", ++nCount);

				strText	= strValue + strText;
			}
			break;
		default:
			{
				strValue.Format(L"%12d", _wtoi(pInfo->szAmount));

				strEndDate	= AddComma(strValue);

				strValue.Format(L"%17s", strEndDate);

				strText	+= strValue.Right(16);
				strText	+= L"  ";

				strEndDate.Format(L"%02d%02d", pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute);

				strText	+= strEndDate;
				strText	+= L"  ";
				strText	+= pInfo->szTraceNo;
				strText	+= L"  ";

				strValue.Format(L"%3d", ++nCount);

				strText	+= strValue;
			}
			break;
		}

		arrType.Add('0');
		arrFeed.Add(L"");
		arrType.Add('C');
		arrFeed.Add(strText);

		llTotalDaily	+= _wtoll(pInfo->szAmount);

		if ((i == database.m_arrTransaction.GetUpperBound()) && (nCount != 0))
		{
			strValue.Format(strFormatTotal, llTotalDaily);

			strText	= AddComma(strValue);

			arrType.Add('1');
			arrFeed.Add(L"");
			arrType.Add('=');
			arrFeed.Add(L"");
			arrType.Add('R');
			arrFeed.Add(strText);
		}
	}

	arrType.Add('1');
	arrFeed.Add(L"");
	arrType.Add(' ');
	arrFeed.Add(L"");

	strText.Format(L"%s\t%d", LoadText(IDS_COUNT_SALE), nCountSale);
	arrType.Add('B');
	arrFeed.Add(strText);

	strEndDate.Format(L"%d", llTotalSale);

	strValue	= AddComma(strEndDate);

	strText.Format(L"%s\t%s", LoadText(IDS_TOTAL), strValue);
	arrType.Add('B');
	arrFeed.Add(strText);

	strText.Format(L"%s\t%d", LoadText(IDS_COUNT_CHARGE), nCountCharge);
	arrType.Add('B');
	arrFeed.Add(strText);

	strEndDate.Format(L"%d", llTotalCharge);

	strValue	= AddComma(strEndDate);

	strText.Format(L"%s\t%s", LoadText(IDS_TOTAL), strValue);
	arrType.Add('B');
	arrFeed.Add(strText);

	strText.Format(L"%s\t%d", LoadText(IDS_COUNT_BILL), nCountBill);
	arrType.Add('B');
	arrFeed.Add(strText);

	strEndDate.Format(L"%d", llTotalBill);

	strValue	= AddComma(strEndDate);

	strText.Format(L"%s\t%s", LoadText(IDS_TOTAL), strValue);
	arrType.Add('B');
	arrFeed.Add(strText);

	arrType.Add('=');
	arrFeed.Add(L"");

	arrType.Add('A');
	arrFeed.Add(LoadText(IDS_DESC_SALE));
	arrType.Add('A');
	arrFeed.Add(LoadText(IDS_DESC_CHARGE));
	arrType.Add('A');
	arrFeed.Add(LoadText(IDS_DESC_BILL_PAYMENT));
}

void CChildView::ExecuteMenuF1()
{
	BOOL	bPaper	= TRUE;

	switch (m_menuInfoF1.nCurItem)
	{
	case 0:
		{
			bPaper	= m_pPrinter->IsPaperLoaded();

			if (bPaper == TRUE)
			{
				InitializeMenuProvider();
				SetViewMode(VIEW_MENU_PROVIDER);
				SetTransactionMode(TRANSACTION_SELL_CHARGE);
			}
			else
			{
				SetTransactionMode(TRANSACTION_NONE);
				ShowMessage(ERR_INTRO_NO_PAPER);
			}
		}
		break;
	case 1:
		{
			bPaper	= m_pPrinter->IsPaperLoaded();

			if (bPaper == TRUE)
			{
				if (m_database.SelectLastTransactionRecord(&m_TransInfo))
				{
					SetPrintInfo(&m_PrintInfo, RECEIPT_NONE, &m_TransInfo, &m_Parameters, RSEQ_ACCEPTOR_ONLY);
					
					if (!wcscmp(m_TransInfo.szResponseCode, L"FF"))
					{
						m_PrintInfo.ReceiptType	= RECEIPT_TIMEOUT;
					}
					else if (wcscmp(m_TransInfo.szResponseCode, L"00"))
					{
						m_PrintInfo.ReceiptType	= RECEIPT_UNSUCCESSFUL;
					}
					else
					{
						switch (m_TransInfo.transactionMode)
						{
						case TRANSACTION_SALE:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_SALE;
							}
							break;
						case TRANSACTION_BALANCE:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_BALANCE;
							}
							break;
						case TRANSACTION_BILL_PAYMENT:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_BILL_PAYMENT;
							}
							break;
						case TRANSACTION_CHARGE:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_CHARGE;
							}
							break;
						case TRANSACTION_SELL_CHARGE:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_SELL_CHARGE;
							}
							break;
						case TRANSACTION_SERVICE_REQUEST:
							{
								m_PrintInfo.ReceiptType	= RECEIPT_SERVICE_REQUEST;
							}
							break;
						}
					}

					SetViewMode(VIEW_PRINT);
					ForcePaintFlag();
				}
				else
				{
					SetTransactionMode(TRANSACTION_REPRINT);
					ShowMessage(ERR_NO_RECORD_FOUND, MESSAGE_INFO);
				}
			}
			else
			{
				SetTransactionMode(TRANSACTION_NONE);
				ShowMessage(ERR_INTRO_NO_PAPER);
			}
		}
		break;
	case 2:
		{
			bPaper	= m_pPrinter->IsPaperLoaded();

			if (bPaper == TRUE)
			{
				InitializeMenu(&m_menuInfoReports, MENU_REPORTS, IDS_REPORTS, IDS_SELECTED_TRANSACTION, IDS_TRANSACTION_DETAILS, TRUE, VIEW_MENU_F1, TRANSACTION_F1);
				SetViewMode(VIEW_MENU_REPORTS);
				SetTransactionMode(TRANSACTION_REPORTS);
			}
			else
			{
				SetTransactionMode(TRANSACTION_NONE);
				ShowMessage(ERR_INTRO_NO_PAPER);
			}
		}
		break;
	case 3:
		{
			SetTransactionMode(TRANSACTION_NONE);
			ShowMessage(ERR_NOT_IMPLEMENTED, MESSAGE_INFO);
		}
		break;
	case 4:
		{
			SetTransactionMode(TRANSACTION_NONE);
			ShowMessage(ERR_NOT_IMPLEMENTED, MESSAGE_INFO);
		}
		break;
	}
}

void CChildView::ExecuteMenuReports()
{
	switch (m_menuInfoReports.nCurItem)
	{
	case 0:
		{
			m_sInfo[INFO_TRACENO].Empty();

			SetViewMode(VIEW_REPORTS_TRACENO);
			SetTransactionMode(TRANSACTION_REPORTS);
		}
		break;
	case 1:
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
					m_sInfo[INFO_ENDDATE].Format(L"%04d%02d%02d", nYear, nMonth, nDay);
				}
				break;
			default:
				{
					m_sInfo[INFO_DATE].Format(L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
					m_sInfo[INFO_ENDDATE].Format(L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
				}
				break;
			}

			m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

			SetViewMode(VIEW_REPORTS_STARTDATE);
			SetTransactionMode(TRANSACTION_REPORTS);
		}
		break;
	}
}

void CChildView::PaintViewReportsTraceNo(CDCX* pDC)
{
	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(IDS_TRACE_NUMBER), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_TRACE_NO_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < m_sInfo[INFO_TRACENO].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_TRACE_NO_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[INFO_TRACENO].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::PaintViewReportsDate(CDCX* pDC)
{
	UINT	nID;
	UINT	nInfo;

	switch (GetViewMode())
	{
	case VIEW_REPORTS_STARTDATE:
		{
			nID		= IDS_START_DATE;
			nInfo	= INFO_DATE;
		}
		break;
	case VIEW_REPORTS_ENDDATE:
		{
			nID		= IDS_END_DATE;
			nInfo	= INFO_ENDDATE;
		}
		break;
	}

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);
		PaintHeader(LoadText(nID), pDC);
	}

	if (PaintRequired(PAINT_FIELDS))
	{
		RECT	rc;

		DrawPNGResource(RP_DATE_BOX, pDC->GetSafeHdc());
		pDC->SelectObject(&m_fntDigitSmall);

		for (int i = 0; i < m_sInfo[nInfo].GetLength(); i++)
		{
			GetRectFromPNGResource(&rc, RP_DATE_NUMBER[i]);
			pDC->DrawText(LoadText(IDS_NUM0 + (m_sInfo[nInfo].GetAt(i) - L'0')), &rc, DT_CENTER | DT_VCENTER);
		}
	}

	PaintButton(RP_YES_BTN_PAD, RP_YES_BTN_PAD_PRESS, PAINT_BTN_1, pDC);
	PaintButton(RP_NO_BTN_PAD, RP_NO_BTN_PAD_PRESS, PAINT_BTN_2, pDC);
}

void CChildView::OnKeyDownViewReportsTraceNo(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			CString	strTraceNo;

			strTraceNo.Format(L"%06d", _wtol(m_sInfo[INFO_TRACENO].GetString()));

			if (m_database.SelectTransactionRecords((WCHAR*)strTraceNo.GetString()))
			{
				SetPrintInfo(&m_PrintInfo, RECEIPT_NONE, &m_database.m_arrTransaction.GetAt(0), &m_Parameters, RSEQ_ACCEPTOR_ONLY);

				if (!wcscmp(m_PrintInfo.TransactionInfo.szResponseCode, L"FF"))
				{
					m_PrintInfo.ReceiptType	= RECEIPT_TIMEOUT;
				}
				else if (wcscmp(m_PrintInfo.TransactionInfo.szResponseCode, L"00"))
				{
					m_PrintInfo.ReceiptType	= RECEIPT_UNSUCCESSFUL;
				}
				else
				{
					switch (m_PrintInfo.TransactionInfo.transactionMode)
					{
					case TRANSACTION_SALE:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_SALE;
						}
						break;
					case TRANSACTION_BALANCE:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_BALANCE;
						}
						break;
					case TRANSACTION_BILL_PAYMENT:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_BILL_PAYMENT;
						}
						break;
					case TRANSACTION_CHARGE:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_CHARGE;
						}
						break;
					case TRANSACTION_SELL_CHARGE:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_SELL_CHARGE;
						}
						break;
					case TRANSACTION_SERVICE_REQUEST:
						{
							m_PrintInfo.ReceiptType	= RECEIPT_SERVICE_REQUEST;
						}
						break;
					}
				}

				SetViewMode(VIEW_PRINT);
				ForcePaintFlag();
			}
			else
			{
				SetTransactionMode(TRANSACTION_REPRINT);
				ShowMessage(ERR_NO_RECORD_FOUND, MESSAGE_INFO);
			}
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			SetViewMode(VIEW_MENU_REPORTS);
			SetTransactionMode(TRANSACTION_REPORTS);
			ForcePaintFlag();
		}
		break;
	case HWK_BACK:
		{
			if (m_sInfo[INFO_TRACENO].GetLength() > 0)
			{
				m_sInfo[INFO_TRACENO].Delete(m_sInfo[INFO_TRACENO].GetLength() - 1);
				ForcePaintFlag(PAINT_FIELDS);
			}
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if (m_sInfo[INFO_TRACENO].GetLength() < LENGTH_TRACENO)
				{
					m_sInfo[INFO_TRACENO].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
			}
		}
		break;
	}
}

void CChildView::OnKeyDownViewReportsDate(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UINT	nInfo;

	switch (GetViewMode())
	{
	case VIEW_REPORTS_STARTDATE:
		{
			nInfo	= INFO_DATE;
		}
		break;
	case VIEW_REPORTS_ENDDATE:
		{
			nInfo	= INFO_ENDDATE;
		}
		break;
	}

	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			if ((m_sInfo[nInfo].GetLength() > 0) && (m_sInfo[nInfo].GetLength() < LENGTH_DATE))
			{
				break;
			}

			if (m_sInfo[nInfo].GetLength() == LENGTH_DATE)
			{
				switch (GetViewMode())
				{
				case VIEW_REPORTS_STARTDATE:
					{
						m_nInfo[INFO_DEFAULT_VALUE]	= TRUE;

						SetViewMode(VIEW_REPORTS_ENDDATE);
						SetTransactionMode(TRANSACTION_REPORTS);
					}
					break;
				case VIEW_REPORTS_ENDDATE:
					{
						CJalali	Jalali;
						WCHAR	szStartDate[15];
						WCHAR	szEndDate[15];
						int		nSY;
						int		nSM;
						int		nSD;
						int		nEY;
						int		nEM;
						int		nED;
						int		nJY;
						int		nJM;
						int		nJD;

						switch (m_Settings.langMode)
						{
						case LANGUAGE_FARSI:
							{
								nJY	= _wtoi(m_sInfo[INFO_DATE].Mid(0, 4));
								nJM	= _wtoi(m_sInfo[INFO_DATE].Mid(4, 2));
								nJD	= _wtoi(m_sInfo[INFO_DATE].Mid(6, 2));
								nSY	= 0;
								nSM	= 0;
								nSD	= 0;

								Jalali.JalaliToGregorian(nJY, nJM, nJD, &nSY, &nSM, &nSD);

								nJY	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(0, 4));
								nJM	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(4, 2));
								nJD	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(6, 2));
								nEY	= 0;
								nEM	= 0;
								nED	= 0;

								Jalali.JalaliToGregorian(nJY, nJM, nJD, &nEY, &nEM, &nED);
							}
							break;
						default:
							{
								nSY	= _wtoi(m_sInfo[INFO_DATE].Mid(0, 4));
								nSM	= _wtoi(m_sInfo[INFO_DATE].Mid(4, 2));
								nSD	= _wtoi(m_sInfo[INFO_DATE].Mid(6, 2));
								nEY	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(0, 4));
								nEM	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(4, 2));
								nED	= _wtoi(m_sInfo[INFO_ENDDATE].Mid(6, 2));
							}
							break;
						}

						swprintf(szStartDate, L"%04d%02d%02d000000", nSY, nSM, nSD);
						swprintf(szEndDate, L"%04d%02d%02d235959", nEY, nEM, nED);

						if (m_database.SelectTransactionRecords(szStartDate, szEndDate))
						{
							BuildTransactionDetails(m_database, szStartDate, szEndDate, m_arrDetailsType, m_arrDetailsFeed);
							memset(&m_PrintInfo, 0, sizeof(PRINT_INFO));

							m_PrintInfo.ReceiptType		= RECEIPT_TRANSACTION_DETAILS;
							m_PrintInfo.pParameters		= &m_Parameters;
							m_PrintInfo.ReceiptSequence	= RSEQ_ACCEPTOR_ONLY;

							SetViewMode(VIEW_PRINT);
							ForcePaintFlag();
						}
						else
						{
							SetTransactionMode(TRANSACTION_REPRINT);
							ShowMessage(ERR_NO_RECORD_FOUND, MESSAGE_INFO);
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
			switch (GetViewMode())
			{
			case VIEW_REPORTS_STARTDATE:
				{
					SetViewMode(VIEW_MENU_REPORTS);
					SetTransactionMode(TRANSACTION_REPORTS);
				}
				break;
			case VIEW_REPORTS_ENDDATE:
				{
					SetViewMode(VIEW_REPORTS_STARTDATE);
					SetTransactionMode(TRANSACTION_REPORTS);
				}
				break;
			}

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
				if (m_nInfo[INFO_DEFAULT_VALUE])
				{
					m_nInfo[INFO_DEFAULT_VALUE]	= FALSE;

					m_sInfo[nInfo].Empty();
					m_sInfo[nInfo].AppendChar(nChar);
					ForcePaintFlag(PAINT_FIELDS);
				}
				else
				{
					if (m_sInfo[nInfo].GetLength() < LENGTH_DATE)
					{
						m_sInfo[nInfo].AppendChar(nChar);
						ForcePaintFlag(PAINT_FIELDS);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnLButtonDownViewReportsTraceNo(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewReportsTraceNo(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}

void CChildView::OnLButtonDownViewReportsDate(UINT nFlags, CPoint point)
{
	OnLButtonDownButton(RP_YES_BTN_PAD, point, PAINT_BTN_1);
	OnLButtonDownButton(RP_NO_BTN_PAD, point, PAINT_BTN_2);
}

void CChildView::OnLButtonUpViewReportsDate(UINT nFlags, CPoint point)
{
	OnLButtonUpButton(RP_YES_BTN_PAD, point, VK_RETURN);
	OnLButtonUpButton(RP_NO_BTN_PAD, point, VK_ESCAPE);
}
