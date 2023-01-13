// ViewUtilities.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "DCX.h"
#include "Utilities.h"
#include "StringRes.h"
#include "PNGRes.h"

#if defined(MODEL_CT360)
#define FS_ANNUNCIATOR		16
#define FS_HEADER_LARGE		27
#define FS_HEADER_SMALL		21
#define FS_HEADER_DESC		21
#define FS_MESSAGE			24
#define FS_DIGIT_LARGE		39
#define FS_DIGIT_SMALL		27
#define FS_INTRO_DATE		27
#define FS_INTRO_TIME		27
#elif defined(MODEL_MT360)
#define FS_ANNUNCIATOR		16
#define FS_HEADER_LARGE		27
#define FS_HEADER_SMALL		21
#define FS_HEADER_DESC		21
#define FS_MESSAGE			24
#define FS_DIGIT_LARGE		39
#define FS_DIGIT_SMALL		27
#define FS_INTRO_DATE		27
#define FS_INTRO_TIME		27
#elif defined(MODEL_MT760)
#define FS_ANNUNCIATOR		24
#define FS_HEADER_LARGE		36
#define FS_HEADER_SMALL		28
#define FS_HEADER_DESC		28
#define FS_MESSAGE			36
#define FS_DIGIT_LARGE		48
#define FS_DIGIT_SMALL		36
#define FS_INTRO_DATE		44
#define FS_INTRO_TIME		80
#endif

void CChildView::InitDC()
{
	CDCX DC;

	DC.GetDC(m_hWnd);
    m_DC.CreateCompatibleDC(&DC);

	m_bmpDC = CBitmap::FromHandle(CreateCompatibleBitmap(DC.GetSafeHdc(), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

	m_DC.SelectObject(m_bmpDC);
	DC.ReleaseDC();
}
 
void CChildView::ExitDC()
{
	m_DC.ReleaseDC();
}

void CChildView::InitFont()
{
	ExitFont();

	m_fntAnnunciator.CreateFont(FS_ANNUNCIATOR, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntHeaderLarge.CreateFont(FS_HEADER_LARGE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntHeaderSmall.CreateFont(FS_HEADER_SMALL, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntHeaderDesc.CreateFont(FS_HEADER_DESC, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntMessage.CreateFont(FS_MESSAGE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntDigitLarge.CreateFont(FS_DIGIT_LARGE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntDigitSmall.CreateFont(FS_DIGIT_SMALL, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntIntroDate.CreateFont(FS_INTRO_DATE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
	m_fntIntroTime.CreateFont(FS_INTRO_TIME, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"tahoma");
}

void CChildView::ExitFont()
{
	m_fntIntroTime.DeleteObject();
	m_fntIntroDate.DeleteObject();
	m_fntDigitSmall.DeleteObject();
	m_fntDigitLarge.DeleteObject();
	m_fntMessage.DeleteObject();
	m_fntHeaderDesc.DeleteObject();
	m_fntHeaderSmall.DeleteObject();
	m_fntHeaderLarge.DeleteObject();
	m_fntAnnunciator.DeleteObject();
}

void CChildView::ClearInfo()
{
	memset(&m_TransInfo, 0, sizeof(TRANSACTION_INFO));

	for (int i = 0; i < INFO_MAX; i++)
	{
		m_sInfo[i].Empty();

		m_nInfo[i]	= 0;
	}
}

BOOL CChildView::IsRightToLeft()
{
	BOOL	bRightToLeft;

	switch (m_Settings.langMode)
	{
	case LANGUAGE_FARSI:
		{
			bRightToLeft	= TRUE;
		}
		break;
	default:
		{
			bRightToLeft	= FALSE;
		}
		break;
	}

	return bRightToLeft;
}

CString CChildView::LoadText(UINT nID)
{
	return ::LoadText(nID, m_Settings.langMode);
}

CString CChildView::LoadText(BILINGUAL text)
{
	CString	strText;

	switch (m_Settings.langMode)
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

	return strText;
}

void CChildView::DrawPNGResource(RES_PNG resPNG, HDC hDC, BOOL bEraseBackground, COLORREF cr)
{
	if (bEraseBackground)
	{
		LOGBRUSH	lbr;
		HBRUSH		br;
		RECT		rc;

		lbr.lbColor = cr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbHatch = NULL;
		br			= CreateBrushIndirect(&lbr);

		GetRectFromPNGResource(&rc, resPNG);
		::FillRect(hDC, &rc, br);
		DeleteObject(br);
	}

	::DrawPNGResource(resPNG, hDC, IsRightToLeft());
}

void CChildView::GetRectFromPNGResource(LPRECT lprect, RES_PNG resPNG)
{
	::GetRectFromPNGResource(lprect, resPNG, IsRightToLeft());
}

void CChildView::PaintBackground(CDCX* pDC, COLORREF cr)
{
	pDC->FillSolidRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), cr);
}

#define LARGE_TITLE_LEN	18

void CChildView::PaintHeader(LPCTSTR szText, CDCX* pDC)
{
	RECT	rc;

	GetRectFromPNGResource(&rc, RP_HEADER);
	pDC->SetTextColor(COLOR_RGB_TEXT);

	if (wcslen(szText) > LARGE_TITLE_LEN)
	{
		pDC->SelectObject(&m_fntHeaderSmall);
	}
	else
	{
		pDC->SelectObject(&m_fntHeaderLarge);
	}

	pDC->DrawText(szText, &rc, DT_CENTER | DT_VCENTER);
	DrawPNGResource(RP_HEADER_LINE, pDC->GetSafeHdc());
}

void CChildView::PaintHeaderDesc(LPCTSTR szText, CDCX* pDC)
{
	RECT	rc;

	GetRectFromPNGResource(&rc, RP_HEADER_DESC);
	pDC->SetTextColor(COLOR_RGB_TEXT);
	pDC->SelectObject(&m_fntHeaderDesc);
	pDC->DrawText(szText, &rc, DT_CENTER | DT_VCENTER);
}

void CChildView::PaintMessage(LPCTSTR szText, RES_PNG resPNG, CDCX* pDC)
{
	RECT	rc;

	GetRectFromPNGResource(&rc, resPNG);
	pDC->SetTextColor(COLOR_RGB_TEXT);
	pDC->SelectObject(&m_fntMessage);
	pDC->DrawText(szText, &rc, DT_CENTER | DT_VCENTER);
}

void CChildView::PaintButton(RES_PNG resPNG, RES_PNG resPNGPress, DWORD dwFlag, CDCX* pDC, LPCWSTR szText, COLORREF crText)
{
	if (((GetPaintFlag() & dwFlag) == dwFlag) || (GetPaintFlag() == PAINT_FULL_SCREEN))
	{
		pDC->SelectObject(&m_fntMessage);

		if ((m_nInfo[INFO_BUTTON_STATUS] & dwFlag) == dwFlag)
		{
			DrawPNGResource(resPNGPress, pDC->GetSafeHdc());
		}
		else
		{
			DrawPNGResource(resPNG, pDC->GetSafeHdc());
		}

		if (szText != NULL)
		{
			RECT		rc;
			COLORREF	cr;

			cr	= pDC->SetTextColor(crText);

			GetRectFromPNGResource(&rc, resPNG);
			pDC->DrawText(szText, &rc, DT_CENTER | DT_VCENTER);
			pDC->SetTextColor(cr);
		}
	}
}

int CChildView::GetSystemTraceNo()
{
	if (m_Settings.dwTraceNo < MAX_TRACE_NO)
	{
		++m_Settings.dwTraceNo;
	}
	else
	{
		m_Settings.dwTraceNo	= 1;
	}

	SaveSettings(&m_Settings);

	return m_Settings.dwTraceNo;
}

BOOL CChildView::ValidatePassword()
{
	SYSTEMTIME	st;
	BYTE		cPassword[LENGTH_PASSWORD + 1];
	BYTE		cCurrentPassword[LENGTH_PASSWORD + 1];

	memset(cPassword, 0, LENGTH_PASSWORD + 1);

	GetLocalTime(&st);
	sprintf((char*)cCurrentPassword, "%1d%1d%1d%1d****", st.wMinute % 10, st.wMinute / 10, st.wHour % 10, st.wHour / 10);

	for (int i = 0; i < LENGTH_PASSWORD; i++)
	{
		if (i < m_sInfo[INFO_PASSWORD].GetLength())
		{
			cPassword[i]	= (BYTE)(m_sInfo[INFO_PASSWORD].GetAt(i) & 0xFF);
		}
		else
		{
			cPassword[i]	= '*';
		}
	}

	switch (GetTransactionMode())
	{
	case TRANSACTION_F2:
		{
			return (BOOL)(!memcmp(cPassword, m_Settings.cPassword, LENGTH_PASSWORD));
		}
		break;
	}

	return (BOOL)(!memcmp(cPassword, cCurrentPassword, LENGTH_PASSWORD));
}

void CChildView::ResetConnection()
{
	EndTransactionThread();
	m_pComm->Close();
	m_pComm->SetConnectionMode((CONNECTION_MODE)m_Parameters.nCurrentConnectionType);
	m_pComm->Open();
	StartTransactionThread();
}

#if GPRS_AVAILABLE
#define CODE_LENGTH	5

BOOL CChildView::GetOperatorName(WCHAR* szName, UINT nSize, LPCWSTR szOperatorCode)
{
	for (int i = 0; i < (int)m_Parameters.nMaxGPRSCode; i++)
	{
		if (!wcsncmp(szOperatorCode, m_Parameters.GPRSCode[i].szCode, CODE_LENGTH))
		{
			if (nSize < (wcslen(m_Parameters.GPRSCode[i].szName) + 1))
			{
				return FALSE;
			}

			wcscpy(szName, m_Parameters.GPRSCode[i].szName);

			return TRUE;
		}
	}

	return FALSE;
}
#endif
