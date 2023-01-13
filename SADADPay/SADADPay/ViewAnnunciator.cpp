// ViewAnnunciator.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "Device.h"
#include "RilApis.h"

DWORD DoAnnunciator(LPVOID lp)
{
	CChildView*	pView	= (CChildView*)lp;

	while (!pView->m_bThreadForceClose)
	{
		pView->UpdateViewAnnunciator();
		Sleep(INTERVAL_STATUS);
	}

	return 0;
}

GPRS_STATUS CChildView::GetGPRSStatus()
{
	if (m_AnnunciatorFlags.dwConnectStatus != DATA_CONNECTED)
	{
		return GPRS_NONE;
	}

	return m_AnnunciatorFlags.gprsStatus;
}

void CChildView::UpdateViewAnnunciator()
{
	SYSTEMTIME	st;

	if (m_dwAnnunciatorTick < GetTickCount())
	{
#if GPRS_AVAILABLE
		DWORD	dwConnectStatus	= DATA_DISCONNECTED;

		RilApi_GetDataConnectionStatusStandAlone(dwConnectStatus);

		if (dwConnectStatus != m_AnnunciatorFlags.dwConnectStatus)
		{
			m_AnnunciatorFlags.dwConnectStatus	= dwConnectStatus;

			ForcePaintFlag(PAINT_ANNUNCIATOR);
		}

		if (m_AnnunciatorFlags.dwConnectStatus == DATA_CONNECTED)
		{
			OPERATORINFO	OperatorInfo;
			WCHAR			szOperator[32];

			if (RilApi_GetCurrentOperator(OperatorInfo))
			{
				if (GetOperatorName(szOperator, 32, OperatorInfo.szNumName))
				{
					if (wcscmp(szOperator, m_AnnunciatorFlags.szOperator))
					{
						wcscpy_s(m_AnnunciatorFlags.szOperator, 32, szOperator);
						ForcePaintFlag(PAINT_ANNUNCIATOR);
					}
				}
				else
				{
					if (wcscmp(OperatorInfo.szLongName, m_AnnunciatorFlags.szOperator))
					{
						wcscpy_s(m_AnnunciatorFlags.szOperator, 32, OperatorInfo.szLongName);
						ForcePaintFlag(PAINT_ANNUNCIATOR);
					}
				}
			}

			DWORD	dwSignalStrength	= 0xFFFF;

			if (!RilApi_GetSignalStrength(dwSignalStrength))
			{
				dwSignalStrength	= 0xFFFF;
			}

			if (GetSignalStrength(dwSignalStrength) != m_AnnunciatorFlags.gprsStatus)
			{
				m_AnnunciatorFlags.gprsStatus	= GetSignalStrength(dwSignalStrength);

				ForcePaintFlag(PAINT_ANNUNCIATOR);
			}
		}
#endif

		BATTERY_STATUS	batteryStatus	= GetBatteryStatus();

		if (batteryStatus != m_AnnunciatorFlags.batteryStatus)
		{
			m_AnnunciatorFlags.batteryStatus	= batteryStatus;

			ForcePaintFlag(PAINT_ANNUNCIATOR);
		}

		m_dwAnnunciatorTick	= GetTickCount() + INTERVAL_ANNUNCIATOR;
	}

	if (m_dwStatusTick < GetTickCount())
	{
		if (GetThreadStatus() != m_AnnunciatorFlags.threadStatus)
		{
			m_AnnunciatorFlags.threadStatus	= GetThreadStatus();

			ForcePaintFlag(PAINT_ANNUNCIATOR);
		}

		if (GetThreadCount() != m_AnnunciatorFlags.nCount)
		{
			m_AnnunciatorFlags.nCount	= GetThreadCount();

			ForcePaintFlag(PAINT_ANNUNCIATOR);
		}

		GetLocalTime(&st);

		st.wSecond			= 0;
		st.wMilliseconds	= 0;

		if (memcmp(&st, &m_AnnunciatorFlags.stTime, sizeof(SYSTEMTIME)))
		{
			memcpy(&m_AnnunciatorFlags.stTime, &st, sizeof(SYSTEMTIME));
			ForcePaintFlag(PAINT_ANNUNCIATOR);
		}

		m_dwStatusTick	= GetTickCount() + INTERVAL_STATUS;
	}
}

void CChildView::PaintViewAnnunciator(CDCX* pDC)
{
	RECT		rc;
	CString		strText;

	if ((GetScreenMode() == SCREEN_GLANCE) && (GetViewMode() == VIEW_INTRO))
	{
		return;
	}

	if (PaintRequired(PAINT_ANNUNCIATOR))
	{
		GetRectFromPNGResource(&rc, RP_ANNUNCIATOR);
		pDC->FillSolidRect(&rc, COLOR_RGB_BLACK);
		pDC->SetTextColor(COLOR_RGB_WHITE);
		pDC->SelectObject(&m_fntAnnunciator);

#if GPRS_AVAILABLE
		DrawPNGResource(RP_ANN_GPRS[m_AnnunciatorFlags.gprsStatus], pDC->GetSafeHdc());

		if (wcslen(m_AnnunciatorFlags.szOperator))
		{
			GetRectFromPNGResource(&rc, RP_ANN_OPER);
			pDC->DrawText(m_AnnunciatorFlags.szOperator, &rc, DT_LEFT | DT_VCENTER);
		}
#endif

		DrawPNGResource(RP_ANN_BATT[m_AnnunciatorFlags.batteryStatus], pDC->GetSafeHdc());

		switch (m_AnnunciatorFlags.threadStatus)
		{
		case TSTATUS_READY:
		case TSTATUS_ERROR:
			{
				DrawPNGResource(RP_ANN_CONN[0], pDC->GetSafeHdc());
			}
			break;
		case TSTATUS_DIALING:
		case TSTATUS_CONNECTING:
		case TSTATUS_WAITING:
		case TSTATUS_RETRY:
			{
				DrawPNGResource(RP_ANN_CONN[1], pDC->GetSafeHdc());
			}
			break;
		case TSTATUS_CONNECTED:
			{
				DrawPNGResource(RP_ANN_CONN[2], pDC->GetSafeHdc());
			}
			break;
		case TSTATUS_SENDING:
			{
				DrawPNGResource(RP_ANN_CONN[3], pDC->GetSafeHdc());
			}
			break;
		case TSTATUS_RECEIVING:
			{
				DrawPNGResource(RP_ANN_CONN[4], pDC->GetSafeHdc());
			}
			break;
		case TSTATUS_RECEIVED:
			{
				DrawPNGResource(RP_ANN_CONN[5], pDC->GetSafeHdc());
			}
			break;
		}

		if (m_AnnunciatorFlags.nCount != THREAD_RETRY_INITIAL)
		{
			GetRectFromPNGResource(&rc, RP_ANN_COUNT);
			strText.Format(L"%d", m_AnnunciatorFlags.nCount);
			pDC->DrawText(FormattingDigit(strText, FALSE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
		}

		GetRectFromPNGResource(&rc, RP_ANN_TIME);
		strText.Format(L"%02d:%02d", m_AnnunciatorFlags.stTime.wHour, m_AnnunciatorFlags.stTime.wMinute);
		pDC->DrawText(FormattingDigit(strText, FALSE, m_Settings.langMode), &rc, DT_CENTER | DT_VCENTER);
	}
}
