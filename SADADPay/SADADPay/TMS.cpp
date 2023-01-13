// TMS.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "TMS.h"
#include "XMLManager.h"
#include "Utilities.h"

BOOL LoadTMSConfigXML(TMS_UPDATE_INFO* pUpdateInfo)
{
	CXMLManager		xmlManager;
	CString			strNodePath;
	CString			strText;
	PXmlNode		root;

	memset(pUpdateInfo, 0, sizeof(TMS_UPDATE_INFO));
	xmlManager.Init();

	if (!xmlManager.Load(PATH_TMSCONFIGXML))
	{
		xmlManager.Release();

		return FALSE;
	}

	strNodePath	= L"//TMSConfig//UPDATE";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		strText	= xmlManager.GetNodeText(root, L"STARTTIME");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wStartHour			= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wStartMinute		= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"ENDTIME");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wEndHour			= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wEndMinute			= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"INTERVAL");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wIntervalHour		= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wIntervalMinute	= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"CHECKCNT");

		if (strText.GetLength())
		{
			pUpdateInfo->wCheckCount		= (WORD)_wtoi(strText);
		}

		strText	= xmlManager.GetNodeText(root, L"WEEK");

		if (strText.GetLength() == 4)
		{
			pUpdateInfo->wWeekdays			= (((WORD)HC2DN(strText.GetAt(2)) << 4) | (WORD)HC2DN(strText.GetAt(3)));
		}
	}

	xmlManager.Release();

	return TRUE;
}

BOOL LoadTMSUpdateXML(TMS_UPDATE_INFO* pUpdateInfo)
{
	CXMLManager		xmlManager;
	CString			strNodePath;
	CString			strText;
	PXmlNode		root;

	memset(pUpdateInfo, 0, sizeof(TMS_UPDATE_INFO));
	xmlManager.Init();

	if (!xmlManager.Load(PATH_TMSUPDATEXML))
	{
		xmlManager.Release();

		return FALSE;
	}

	strNodePath	= L"//UPDATE";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		strText	= xmlManager.GetNodeText(root, L"STARTTIME");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wStartHour			= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wStartMinute		= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"ENDTIME");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wEndHour			= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wEndMinute			= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"INTERVAL");

		if (strText.GetLength() == 5)
		{
			pUpdateInfo->wIntervalHour		= (WORD)_wtoi(strText.Mid(0, 2));
			pUpdateInfo->wIntervalMinute	= (WORD)_wtoi(strText.Mid(3, 2));
		}

		strText	= xmlManager.GetNodeText(root, L"CHECKCNT");

		if (strText.GetLength())
		{
			pUpdateInfo->wCheckCount		= (WORD)_wtoi(strText);
		}

		strText	= xmlManager.GetNodeText(root, L"WEEK");

		if (strText.GetLength() == 4)
		{
			pUpdateInfo->wWeekdays			= (((WORD)HC2DN(strText.GetAt(2)) << 4) | (WORD)HC2DN(strText.GetAt(3)));
		}
	}

	xmlManager.Release();

	return TRUE;
}

#define TMSXML_HEADER		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
#define TMSXML_UPDATE_OPEN	"<UPDATE>\r\n"
#define TMSXML_STARTTIME	"\t<STARTTIME>%02d:%02d</STARTTIME>\r\n"
#define TMSXML_ENDTIME		"\t<ENDTIME>%02d:%02d</ENDTIME>\r\n"
#define TMSXML_INTERVAL		"\t<INTERVAL>%02d:%02d</INTERVAL>\r\n"
#define TMSXML_CHECKCOUNT	"\t<CHECKCNT>%d</CHECKCNT>\r\n"
#define TMSXML_WEEKDAYS		"\t<WEEK>0x%02X</WEEK>\r\n"
#define TMSXML_UPDATE_CLOSE	"</UPDATE>\r\n"

BOOL SaveTMSUpdateXML(TMS_UPDATE_INFO* pUpdateInfo)
{
	HANDLE	hFile;
	char	cBuffer[128];
	DWORD	dwBytes;

	CreateDirectory(PATH_TMSTEMPDIRECTORY, NULL);

	hFile	= CreateFile(PATH_TMSUPDATEXML, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	sprintf_s(cBuffer, 128, TMSXML_HEADER);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_UPDATE_OPEN);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_STARTTIME, pUpdateInfo->wStartHour, pUpdateInfo->wStartMinute);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_ENDTIME, pUpdateInfo->wEndHour, pUpdateInfo->wEndMinute);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_INTERVAL, pUpdateInfo->wIntervalHour, pUpdateInfo->wIntervalMinute);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_CHECKCOUNT, pUpdateInfo->wCheckCount);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_WEEKDAYS, pUpdateInfo->wWeekdays);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);
	sprintf_s(cBuffer, 128, TMSXML_UPDATE_CLOSE);
	WriteFile(hFile, cBuffer, strlen(cBuffer), &dwBytes, NULL);

	CloseHandle(hFile);
	return TRUE;
}

LRESULT CChildView::OnTMS(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TMS_MSG_RES_PARAMETERS:
		{
			LOGFILE(L"%S[%d] - MSG_RES_PARAMETERS\r\n", __FUNCTION__, __LINE__);

			if ((GetThreadMode() != TMODE_TMS) && (!m_bTMSNeed))
			{
				m_bTMSNeed	= TRUE;
			}
		}
		break;
	case TMS_MSG_REQ_RESOURCE:
		{
			LOGFILE(L"%S[%d] - MSG_REQ_RESOURCE\r\n", __FUNCTION__, __LINE__);

			if ((GetThreadMode() != TMODE_TMS) && (!m_bTMSRequested))
			{
				m_bTMSRequested	= TRUE;
			}
		}
		break;
	case TMS_MSG_REQ_RELEASE:
		{
			LOGFILE(L"%S[%d] - MSG_REQ_RELEASE\r\n", __FUNCTION__, __LINE__);

			if (GetThreadMode() == TMODE_TMS)
			{
				LOGFILE(L"%S[%d] - MSG_RES_RELEASE\r\n", __FUNCTION__, __LINE__);
				::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_RES_RELEASE, 0);

				if (!m_bTMSNeed)
				{
					g_bAppActivated	= TRUE;

					m_pComm->Open();
					SetThreadMode(TMODE_READY);
					Sleep(500);
					ForcePaintFlag();
				}
			}
				
		}
		break;
	case TMS_MSG_REQ_EXIT:
		{
			LOGFILE(L"%S[%d] - MSG_REQ_EXIT\r\n", __FUNCTION__, __LINE__);

			if (GetThreadMode() == TMODE_TMS)
			{
				LOGFILE(L"%S[%d] - MSG_RES_EXIT\r\n", __FUNCTION__, __LINE__);
				::PostMessage(HWND_BROADCAST, WM_TMS_MESSAGE, TMS_MSG_RES_EXIT, 0);
				ExitApplication();
			}
		}
		break;
	case TMS_RES_SUCCESS:
		{
			LOGFILE(L"%S[%d] - RES_SUCCESS = %d\r\n", __FUNCTION__, __LINE__, GetThreadMode());

			if ((GetThreadMode() == TMODE_TMS) && (m_bTMSNeed))
			{
				m_bTMSNeed		= FALSE;
				g_bAppActivated	= TRUE;

				LoadConfigXML(&m_Parameters);
				SaveParameters(&m_Parameters);
				FirstSetupParameters(&m_Parameters, FALSE);
				m_pComm->Open();
				SetThreadMode(TMODE_READY);
				Sleep(500);
				ForcePaintFlag();
			}
		}
		break;
	}

	return 0;
}
