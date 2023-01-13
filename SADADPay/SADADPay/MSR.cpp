// MSR.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "StringRes.h"
#include "Device.h"

void CChildView::OpenMSR()
{
	if (m_hMSR != NULL)
	{
		return;
	}

	m_hMSR	= BBCardReaderOpen(0);

	if (m_hMSR != NULL)
	{		
		BBCardReaderStartAsyncReadData(m_hMSR, &cbMSRCallback, (DWORD)this);
		BBCardReaderEnableTrackSound(m_hMSR, 0, TRUE);
	}
}

void CChildView::RestartMSR()
{
	g_strMSRData[0].Empty();
	g_strMSRData[1].Empty();
	g_strMSRData[2].Empty();
	OpenMSR();
}

void CChildView::CloseMSR()
{	
	if (m_hMSR == NULL)
	{
		return;
	}

	BBCardReaderClose(m_hMSR);

	m_hMSR	= NULL;
}

void CChildView::cbMSRCallback(DWORD dwArg, LPCTSTR data)
{
	CChildView* pView	= reinterpret_cast<CChildView*>(dwArg);

	if (pView->GetViewMode() == VIEW_INTRO)
	{
		pView->SetScreenMode(SCREEN_NORMAL);

		DWORD	dwTrack	= 0;

		WCHAR szBuffer1[80];
		WCHAR szBuffer2[50];
		WCHAR szBuffer3[150];

		DWORD dwBufferSize1	= sizeof(szBuffer1);
		DWORD dwBufferSize2	= sizeof(szBuffer2);
		DWORD dwBufferSize3	= sizeof(szBuffer3);

		memset(szBuffer1, 0, sizeof(szBuffer1));
		memset(szBuffer2, 0, sizeof(szBuffer2));
		memset(szBuffer3, 0, sizeof(szBuffer3));

		if (BBCardReaderGetReadTrack(pView->m_hMSR, &dwTrack) != 0)
		{
			pView->StartFlow(FALSE);
			return;
		}

		if ((dwTrack & BB_CARDREADER_READ_TRACK1) != 0)
		{		
			if (BBCardReaderReadCardData(pView->m_hMSR, BB_CARDREADER_READ_TRACK1, szBuffer1, &dwBufferSize1))
			{
				pView->StartFlow(FALSE);
				return;
			}

			g_strMSRData[0].Format(L"%s", szBuffer1);
		}

		if ((dwTrack & BB_CARDREADER_READ_TRACK2) != 0)
		{
			if (BBCardReaderReadCardData(pView->m_hMSR, BB_CARDREADER_READ_TRACK2, szBuffer2, &dwBufferSize2))
			{
				pView->StartFlow(FALSE);
				return;
			}

			g_strMSRData[1].Format(L"%s", szBuffer2);
		}
		else
		{
			pView->StartFlow(FALSE);
			return;
		}

		if ((dwTrack & BB_CARDREADER_READ_TRACK3) != 0)
		{
			if (BBCardReaderReadCardData(pView->m_hMSR, BB_CARDREADER_READ_TRACK3, szBuffer3, &dwBufferSize3))
			{
				pView->StartFlow(FALSE);
				return;
			}

			g_strMSRData[2].Format(L"%s", szBuffer3);
		}

		pView->StartFlow();
	
		pView	= NULL;	
	}
}

void CChildView::StartFlow(BOOL bClose)
{
	LOGFILE(L"%S[%d] - Card Swiped\r\n", __FUNCTION__, __LINE__);
	SetScreenMode(SCREEN_NORMAL);

#if !IGNORE_LOGON
	if (m_Settings.logonStatus != LOGON_SUCCEEDED)
	{
		SetTransactionMode(TRANSACTION_NONE);
		ShowMessage(ERR_LOGON_REQUIRED, MESSAGE_INFO);
		::PostMessage(this->GetSafeHwnd(), WM_CARDRESTART, 0, 0);
	}
	else
#endif
	if (ReversalExist())
	{
		ResetReversalTime();
		SetTransactionMode(TRANSACTION_NONE);
		::PostMessage(this->GetSafeHwnd(), WM_CARDRESTART, 0, 0);
	}
	else
	{
		if (bClose)
		{
			InitializeMenu(&m_menuInfoMain, MENU_MAIN, IDS_NOSTRING, IDS_SALE, IDS_CHARGE);
			SetViewMode(VIEW_MENU_MAIN);
			SetTransactionMode(TRANSACTION_NONE);

			ForcePaintFlag();
			::PostMessage(this->GetSafeHwnd(), WM_CARDCLOSE, 0, 0);
		}
		else
		{
			::PostMessage(this->GetSafeHwnd(), WM_CARDRESTART, 0, 0);
		}
	}
}

LRESULT CChildView::OnCardClose(WPARAM wParam, LPARAM lParam)
{
	CloseMSR();
	return 0;
}

LRESULT CChildView::OnCardRestart(WPARAM wParam, LPARAM lParam)
{
	RestartMSR();
	return 0;
}
