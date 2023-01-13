// RilApiEx.cpp

#include "stdafx.h"
#include "RilApiEx.h"
#include <initguid.h>
#include <connmgr.h>
#include <cfgmgrapi.h>
#include "RilApis.h"
#include "Utilities.h"
#include "RilTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRilApiExApp, CWinApp)
END_MESSAGE_MAP()

CRilApiExApp::CRilApiExApp()
{
}

CRilApiExApp	theApp;

BOOL CRilApiExApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

BOOL RilApi_Initialize()
{
	g_pRilTools	= CRilTools::CreateInstance();

	return (BOOL)(g_pRilTools != NULL);
}

void RilApi_Cleanup()
{
	g_pRilTools->DestroyInstance();
}

HRESULT RilApi_GetLastResult()
{
	return g_hResult;
}

BOOL RilApi_GetEquipmentState(DWORD& dwEquipmentState)
{
	RILEQUIPMENTSTATE	RilEquipmentState;

	if (g_pRilTools->GetEquipmentState(RilEquipmentState))
	{
		return FALSE;
	}

	switch (RilEquipmentState.dwEqState)
	{
	case RIL_EQSTATE_MINIMUM:
		{
			dwEquipmentState	= EQSTATE_MINIMUM;
		}
		break;
	case RIL_EQSTATE_FULL:
		{
			dwEquipmentState	= EQSTATE_FULL;
		}
		break;
	default:
		{
			dwEquipmentState	= EQSTATE_UNKNOWN;
		}
		break;
	}

	return TRUE;
}

BOOL RilApi_SetEquipmentState(DWORD dwEquipmentState)
{
	DWORD	dwEQState	= RIL_EQSTATE_UNKNOWN;
	int		nRetry		= 0;
	TCHAR	tszGprsEntryName[256];

	ZeroMemory(tszGprsEntryName, sizeof(tszGprsEntryName));

	switch (dwEquipmentState)
	{
	case EQSTATE_MINIMUM:
		{
			GetEnabledGprsEntryName(tszGprsEntryName);
			DataDisconnect(tszGprsEntryName);

			dwEQState	= RIL_EQSTATE_MINIMUM;
		}
		break;
	case EQSTATE_FULL:
		{
			dwEQState	= RIL_EQSTATE_FULL;
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	for (nRetry = 0; nRetry < MAX_RETRY_COUNT; nRetry++)
	{
		if (!g_pRilTools->SetEquipmentState(dwEQState))
		{
			break;
		}
	}

	return (BOOL)(nRetry < MAX_RETRY_COUNT);
}

BOOL RilApi_GetEquipmentInfo(EQUIPMENTINFO& EquipmentInfo)
{
	RILEQUIPMENTINFO	RilEquipmentInfo;

	ZeroMemory(&RilEquipmentInfo, sizeof(RILEQUIPMENTINFO));

	if(g_pRilTools->GetEquipmentInfo(RilEquipmentInfo))
	{
		return FALSE;
	}

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilEquipmentInfo.szManufacturer, sizeof(RilEquipmentInfo.szManufacturer), EquipmentInfo.szManufacturer, sizeof(EquipmentInfo.szManufacturer));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilEquipmentInfo.szModel, sizeof(RilEquipmentInfo.szModel), EquipmentInfo.szModel, sizeof(EquipmentInfo.szModel));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilEquipmentInfo.szRevision, sizeof(RilEquipmentInfo.szRevision), EquipmentInfo.szRevision, sizeof(EquipmentInfo.szRevision));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilEquipmentInfo.szSerialNumber, sizeof(RilEquipmentInfo.szSerialNumber), EquipmentInfo.szSerialNumber, sizeof(EquipmentInfo.szSerialNumber));

	return TRUE;
}

BOOL RilApi_GetVersionInfo(VERSIONINFO& VersionInfo)
{
	RADIO_VERSION_INFO	RadioVersionInfo;

	ZeroMemory(&RadioVersionInfo, sizeof(RADIO_VERSION_INFO));

	if(g_pRilTools->GetVersionInfo(RadioVersionInfo))
	{
		return FALSE;
	}

	_snwprintf(VersionInfo.szRILVersion, sizeof(VersionInfo.szRILVersion), _T("%d,%d,%d,%d"), (RadioVersionInfo.dwRILVersion>>24) & 0xFF, (RadioVersionInfo.dwRILVersion>>16) & 0xFF, (RadioVersionInfo.dwRILVersion>>8) & 0xFF, RadioVersionInfo.dwRILVersion & 0xFF);
	_snwprintf(VersionInfo.szRHAVersion, sizeof(VersionInfo.szRHAVersion), _T("%d,%d,%d,%d"), (RadioVersionInfo.dwRHAVersion>>24) & 0xFF, (RadioVersionInfo.dwRHAVersion>>16) & 0xFF, (RadioVersionInfo.dwRHAVersion>>8) & 0xFF, RadioVersionInfo.dwRHAVersion & 0xFF);
	_snwprintf(VersionInfo.szMuxVersion, sizeof(VersionInfo.szMuxVersion), _T("%d,%d,%d,%d"), (RadioVersionInfo.dwMuxVersion>>24) & 0xFF, (RadioVersionInfo.dwMuxVersion>>16) & 0xFF, (RadioVersionInfo.dwMuxVersion>>8) & 0xFF, RadioVersionInfo.dwMuxVersion & 0xFF);

	return TRUE;
}

BOOL RilApi_GetSimInserted(DWORD& dwSimInserted)
{
	DWORD	dwStatus	= 0;
	
	if (ERROR_SUCCESS != GetRegistryDWORD(HKEY_LOCAL_MACHINE, RK_PHONE, RV_STATUS, &dwStatus))
	{
		return FALSE;
	}

	dwSimInserted	= ((dwStatus & 0x02) ? SIM_NOT_INSERTED : SIM_INSERTED);

	return TRUE;
}

BOOL RilApi_GetPhoneLockedState(DWORD& dwPhoneLockedState)
{
	DWORD	dwTempState	= 0;

	if (g_pRilTools->GetPhoneLockedState(dwTempState))
	{
		return FALSE;
	}

	switch (dwTempState)
	{
	case RIL_LOCKEDSTATE_READY:
		{
			dwPhoneLockedState	= LOCKEDSTATE_READY;
		}
		break;
	case RIL_LOCKEDSTATE_SIM_PIN:
		{
			dwPhoneLockedState	= LOCKEDSTATE_SIM_PIN;
		}
		break;
	case RIL_LOCKEDSTATE_SIM_PUK:
		{
			dwPhoneLockedState	= LOCKEDSTATE_SIM_PUK;
		}
		break;
	default:
		{
			dwPhoneLockedState	= LOCKEDSTATE_UNKNOWN;
		}
		break;
	}

	return TRUE;
}

BOOL RilApi_GetPinCount(DWORD& dwPinCount)
{
	DWORD				dwPhoneLockedState	= RIL_LOCKEDSTATE_UNKNOWN;
	int					nSize				= 0;
	RILSIMCMDPARAMETERS	simcmd;
	BYTE				byResponse[2048];

	if (g_pRilTools->GetPhoneLockedState(dwPhoneLockedState))
	{
		return FALSE;
	}

	memset(&simcmd, 0, sizeof(RILSIMCMDPARAMETERS));

	simcmd.cbSize	= sizeof(RILSIMCMDPARAMETERS); 
	simcmd.dwParams	= RIL_PARAM_SCP_FILEID; 
	simcmd.dwFileID	= 0x6F7B;
	nSize			= sizeof(byResponse);

	if (g_pRilTools->SendRestrictedSimCmd(RIL_SIMCMD_STATUS, simcmd, byResponse, nSize))
	{
		return FALSE;
	}

	switch (dwPhoneLockedState)
	{
	case LOCKEDSTATE_SIM_PIN:
		{
			dwPinCount	= (DWORD)(0xF & byResponse[18]);
		}
		break;
	case LOCKEDSTATE_SIM_PUK:
		{
			dwPinCount	= (DWORD)(0xF & byResponse[19]);
		}
		break;
	default:
		{
			dwPinCount	= (DWORD)(0xF & byResponse[18]);
		}
		break;
	}

	return TRUE;
}

BOOL RilApi_UnlockPhone(LPCSTR lpszPassword, LPCSTR lpszNewPassword)
{
	return (BOOL)(!g_pRilTools->UnlockPhone(lpszPassword, lpszNewPassword));
}

BOOL RilApi_GetLockingStatus(DWORD& dwLockingStatus)
{
	DWORD	dwTempStatus	= 0;

	if (g_pRilTools->GetLockingStatus(RIL_LOCKFACILITY_SIM, NULL, dwTempStatus))
	{
		return FALSE;
	}

	switch (dwTempStatus)
	{
	case RIL_LOCKINGSTATUS_DISABLED:
		{
			dwLockingStatus	= LOCKINGSTATUS_DISABLED;
		}
		break;
	case RIL_LOCKINGSTATUS_ENABLED:
		{
			dwLockingStatus	= LOCKINGSTATUS_ENABLED;
		}
		break;
	default:
		{
			dwLockingStatus	= LOCKINGSTATUS_UNKNOWN;
		}
		break;	
	}

	return TRUE;
}

BOOL RilApi_SetLockingStatus(LPCSTR lpszPassword, DWORD dwLockingStatus)
{
	DWORD	dwTempStatus	= 0;
	
	switch (dwLockingStatus)
	{
	case LOCKINGSTATUS_DISABLED:
		{
			dwTempStatus = RIL_LOCKINGSTATUS_DISABLED;
		}
		break;
	case LOCKINGSTATUS_ENABLED:
		{
			dwTempStatus = RIL_LOCKINGSTATUS_ENABLED;
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	
	return (BOOL)(!g_pRilTools->SetLockingStatus(RIL_LOCKFACILITY_SIM, lpszPassword, dwTempStatus));
}

BOOL RilApi_ChangeLockingPassword(LPCSTR lpszOldPassword, LPCSTR lpszNewPassword)
{
	return (BOOL)(!g_pRilTools->ChangeLockingPassword(RIL_LOCKFACILITY_SIM, RIL_PASSWORDTYPE_PIN, lpszOldPassword, lpszNewPassword));
}

BOOL RilApi_GetRegistrationStatus(DWORD& dwRegStatus)
{
	dwRegStatus	= REGSTAT_UNKNOWN;

	return (BOOL)(ERROR_SUCCESS == GetRegistryDWORD(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\RIL"), _T("RegStatus"), &dwRegStatus));
}

BOOL RilApi_GetPacketDomainRegistrationStatus(DWORD& dwPacketDomainRegStatus)
{
	char	szCommand[32]	= "AT+CGREG?\r";
	char	szResponse[32];
	DWORD	dwTempRegStatus	= REGSTAT_UNKNOWN;

	ZeroMemory(szResponse, sizeof(szResponse));

	if (g_pRilTools->SendATCommand(szCommand, szResponse, sizeof(szResponse)))
	{
		return FALSE;
	}

	CString	strResponse(szResponse);
	int		nIndex	= strResponse.Find(_T(","));

	strResponse		= strResponse.Mid(nIndex + 1);
	dwTempRegStatus = _ttoi(strResponse.GetBuffer());

	switch (dwTempRegStatus)
	{
	case 0:
		{
			dwPacketDomainRegStatus	= REGSTAT_UNREGISTERED;
		}
		break;
	case 1:
		{
			dwPacketDomainRegStatus	= REGSTAT_HOME;
		}
		break;
	case 2:
		{
			dwPacketDomainRegStatus	= REGSTAT_ATTEMPTING;
		}
		break;
	case 3:
		{
			dwPacketDomainRegStatus	= REGSTAT_DENIED;
		}
		break;
	case 5:
		{
			dwPacketDomainRegStatus	= REGSTAT_ROAMING;
		}
		break;
	default:
		{
			dwPacketDomainRegStatus = REGSTAT_UNKNOWN;
		}
		break;
	}

	return TRUE;
}

BOOL RilApi_GetCurrentOperator(OPERATORINFO& OperatorInfo)
{
	RILOPERATORNAMES	RilOperatorNames;

	if (g_pRilTools->GetCurrentOperator(RilOperatorNames, RIL_OPFORMAT_NUM))
	{
		return FALSE;
	}

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilOperatorNames.szLongName, sizeof(RilOperatorNames.szLongName), OperatorInfo.szLongName, sizeof(OperatorInfo.szLongName));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilOperatorNames.szShortName, sizeof(RilOperatorNames.szShortName), OperatorInfo.szShortName, sizeof(OperatorInfo.szShortName));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilOperatorNames.szNumName, sizeof(RilOperatorNames.szNumName), OperatorInfo.szNumName, sizeof(OperatorInfo.szNumName));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, RilOperatorNames.szCountryCode, sizeof(RilOperatorNames.szCountryCode), OperatorInfo.szCountryCode, sizeof(OperatorInfo.szCountryCode));

	return TRUE;
}

BOOL RilApi_GetSignalStrength(DWORD& dwSignalStrength)
{
	return (BOOL)(ERROR_SUCCESS == GetRegistryDWORD(HKEY_LOCAL_MACHINE, RK_PHONE, RV_SIGNAL, &dwSignalStrength));
}

BOOL RilApi_GetPhoneStatus(DWORD& dwStatus)
{
	dwStatus	= PHONE_NONE;

	return (BOOL)(ERROR_SUCCESS == GetRegistryDWORD(HKEY_LOCAL_MACHINE, _T("System\\State\\Phone"), _T("Status"), &dwStatus));
}

BOOL RilApi_DataConnect(DWORD& dwStatus)
{
    CONNMGR_CONNECTIONINFO	connInfo;

	memset(&connInfo, 0, sizeof(CONNMGR_CONNECTIONINFO));

	connInfo.cbSize			= sizeof(CONNMGR_CONNECTIONINFO);
    connInfo.dwParams		= CONNMGR_PARAM_GUIDDESTNET;
	connInfo.dwFlags		= CONNMGR_FLAG_PROXY_HTTP | CONNMGR_FLAG_PROXY_WAP | CONNMGR_FLAG_PROXY_SOCKS4 | CONNMGR_FLAG_PROXY_SOCKS5;
    connInfo.dwPriority		= CONNMGR_PRIORITY_USERINTERACTIVE;
	connInfo.guidDestNet	= IID_DestNetInternet;
	connInfo.bExclusive		= FALSE;
	g_hConnection			= NULL;
	dwStatus				= 0;

	g_hResult	= ConnMgrEstablishConnectionSync(&connInfo, &g_hConnection, 30000, &dwStatus);

	if (g_hResult != S_OK)
	{
		ConnMgrReleaseConnection(g_hConnection, 1);

		g_hConnection	= NULL;
	}

	return (BOOL)(g_hResult == S_OK);
}

void RilApi_DataDisconnect()
{
	TCHAR	tszGprsEntryName[256];

	if (g_hConnection != NULL)
	{
		ConnMgrReleaseConnection(g_hConnection, 1);
	}

	ZeroMemory(tszGprsEntryName, sizeof(tszGprsEntryName));
	GetEnabledGprsEntryName(tszGprsEntryName);
	DataDisconnect(tszGprsEntryName);
}

BOOL RilApi_GetDataConnectionStatus(DWORD& dwDataConnectionStatus)
{
	TCHAR	tszGprsEntryName[256];
	DWORD	dwStatus;
	DWORD	dwConnMgrStatus		= DATA_DISCONNECTED;
	DWORD	dwGprsEntryStatus	= DATA_DISCONNECTED;

	if (g_hConnection != NULL)
	{
		if (ConnMgrConnectionStatus(g_hConnection, &dwStatus) == S_OK)
		{
			dwConnMgrStatus	= ((dwStatus == CONNMGR_STATUS_CONNECTED) ? DATA_CONNECTED : DATA_DISCONNECTED);
		}
	}

	ZeroMemory(tszGprsEntryName, sizeof(tszGprsEntryName));
	GetEnabledGprsEntryName(tszGprsEntryName);

	dwGprsEntryStatus		= (DataState(tszGprsEntryName) ? DATA_CONNECTED : DATA_DISCONNECTED);
	dwDataConnectionStatus	= (((dwConnMgrStatus == DATA_CONNECTED) && (dwGprsEntryStatus == DATA_CONNECTED)) ? DATA_CONNECTED : DATA_DISCONNECTED);

	return TRUE;
}

BOOL RilApi_GetDataConnectionStatusStandAlone(DWORD& dwDataConnectionStatus)
{
	TCHAR	tszGprsEntryName[256];
	
	ZeroMemory(tszGprsEntryName, sizeof(tszGprsEntryName));
	GetEnabledGprsEntryName(tszGprsEntryName);

	dwDataConnectionStatus	= (DataState(tszGprsEntryName) ? DATA_CONNECTED : DATA_DISCONNECTED);

	return TRUE;
}

BOOL RilApi_AddGprsEntry(GPRSENTRY GprsEntry)
{
	LPWSTR	pOutput	= NULL;
	HRESULT	hr		= NULL;
	BOOL	bRes	= FALSE;
	CString	strProvision(g_szAddGprsEntry);

	strProvision.Replace(_T("INPUT_NAME"), GprsEntry.szName);
	strProvision.Replace(_T("INPUT_USER_NAME"), GprsEntry.szUserName);
	strProvision.Replace(_T("INPUT_PASSWORD"), GprsEntry.szPassword);
	strProvision.Replace(_T("INPUT_APN"), GprsEntry.szAPN);
	
	hr	= CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr	= DMProcessConfigXML(strProvision.GetBuffer(strProvision.GetLength() + 1), CFGFLAG_PROCESS, &pOutput);

		if (SUCCEEDED(hr))
		{
			bRes	= TRUE;
		}

		delete [] pOutput;

		CoUninitialize(); 
	}

	return bRes;
}

BOOL RilApi_DeleteGprsEntry(TCHAR szName[])
{
	LPWSTR	pOutput	= NULL;
	HRESULT	hr		= NULL;
	BOOL	bRes	= FALSE;
	CString strProvision(g_szDeleteGprsEntry);

	strProvision.Replace(_T("INPUT_NAME"), szName);

	hr	= CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr	= DMProcessConfigXML(strProvision.GetBuffer(strProvision.GetLength() + 1), CFGFLAG_PROCESS, &pOutput);

		if (SUCCEEDED(hr))
		{
			bRes	= TRUE;
		}

		delete [] pOutput;

		CoUninitialize(); 
	}

	return bRes;
}

BOOL RilApi_GetGprsEntryInfo(GPRSENTRY GprsEntry[], DWORD dwBufferSize, DWORD& dwEntryCount)
{
	LPWSTR	pOutput	= NULL;
	HRESULT	hr		= NULL;
	BOOL	bRes	= FALSE;
	DWORD	dwCount	= 0;
	int		nEntry	= dwBufferSize / sizeof(GPRSENTRY);
	CString	strProvision(g_szGetGprsEntryName);

	hr	= CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr	= DMProcessConfigXML(strProvision.GetBuffer(strProvision.GetLength() + 1), CFGFLAG_PROCESS, &pOutput);

		if (SUCCEEDED(hr))
		{
			bRes	= TRUE;
		}

		CoUninitialize(); 
	}

	if (bRes)
	{
		CString	strTemp(pOutput);
		CString	strName;
		CString	strUserName;
		CString	strPassword;
		CString	strEnabled;
		CString	strAPN;
		int		nIndex	= strTemp.Find(_T("type=\""));

		strTemp	= strTemp.Mid(nIndex + 6);

		for (int i = 0; i < nEntry; i++)
		{
			nIndex	= strTemp.Find(_T("type=\""));

			if (nIndex == -1)
			{
				break;
			}

			strTemp	= strTemp.Mid(nIndex + 6);
			nIndex	= strTemp.Find(_T("\""));
			strName	= strTemp.Mid(0, nIndex);

			nIndex		= strTemp.Find(_T("UserName\" value=\""));
			strTemp		= strTemp.Mid(nIndex + 17);
			nIndex		= strTemp.Find(_T("\""));
			strUserName	= strTemp.Mid(0, nIndex);

			nIndex		= strTemp.Find(_T("Password\" value=\""));
			strTemp		= strTemp.Mid(nIndex + 17);
			nIndex		= strTemp.Find(_T("\""));
			strPassword	= strTemp.Mid(0, nIndex);

			nIndex		= strTemp.Find(_T("Enabled\" value=\""));
			strTemp		= strTemp.Mid(nIndex + 16);
			nIndex		= strTemp.Find(_T("\""));
			strEnabled	= strTemp.Mid(0, nIndex);

			nIndex		= strTemp.Find(_T("GPRSInfoAccessPointName\" value=\""));
			strTemp		= strTemp.Mid(nIndex + 32);
			nIndex		= strTemp.Find(_T("\""));
			strAPN		= strTemp.Mid(0, nIndex);

			wcsncpy_s(GprsEntry[dwCount].szName, 256, strName.GetBuffer(), strName.GetLength());
			wcsncpy_s(GprsEntry[dwCount].szUserName, 256, strUserName.GetBuffer(), strUserName.GetLength());
			wcsncpy_s(GprsEntry[dwCount].szPassword, 256, strPassword.GetBuffer(), strPassword.GetLength());
			wcsncpy_s(GprsEntry[dwCount].szAPN, 256, strAPN.GetBuffer(), strAPN.GetLength());

			GprsEntry[dwCount].bEnabled	= (BOOL)(_tstoi(strEnabled.GetBuffer()) > 0);

			dwCount++;
		}
	}

	dwEntryCount	= dwCount;

	delete [] pOutput;
	
	return bRes;	
}

BOOL RilApi_EnableGprsEntry(TCHAR szName[])
{
	LPWSTR	pOutput	= NULL;
	HRESULT	hr		= NULL;
	BOOL	bRes	= FALSE;
	TCHAR	tszGprsEntryName[256];
	
	if (g_bDataState == FALSE)
	{
		return bRes;
	}

	RilApi_DataDisconnect();
	ZeroMemory(tszGprsEntryName, sizeof(tszGprsEntryName));

	CString	strProvision(g_szEnableGprsEntry);

	strProvision.Replace(_T("INPUT_NAME"), szName);

	hr	= CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr	= DMProcessConfigXML(strProvision.GetBuffer(strProvision.GetLength() + 1), CFGFLAG_PROCESS, &pOutput);

		if (SUCCEEDED(hr))
		{
			bRes	= TRUE;
		}

		delete [] pOutput;

		CoUninitialize(); 
	}

	return bRes;
}

BOOL RilApi_DisableGprsEntry(TCHAR szName[])
{
	LPWSTR	pOutput	= NULL;
	HRESULT	hr		= NULL;
	BOOL	bRes	= FALSE;
	CString	strProvision(g_szDisableGprsEntry);

	strProvision.Replace(_T("INPUT_NAME"), szName);

	hr	= CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr	= DMProcessConfigXML(strProvision.GetBuffer(strProvision.GetLength() + 1), CFGFLAG_PROCESS, &pOutput);

		if (SUCCEEDED(hr))
		{
			bRes	= TRUE;
		}

		delete [] pOutput;

		CoUninitialize(); 
	}

	return bRes;
}

BOOL RilApi_SendATCommand(LPCSTR szATCommand, LPCSTR szResponse, DWORD dwResonseLen)
{
	return (BOOL)(!g_pRilTools->SendATCommand(szATCommand, szResponse, dwResonseLen));
}

BOOL RilApi_GetICCID(TCHAR* pszICCID, DWORD dwSize)
{
	char	szICCID[128];

	ZeroMemory(szICCID, sizeof(szICCID));

	if (g_pRilTools->GetICCID(szICCID))
	{
		return FALSE;
	}

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szICCID, strlen(szICCID), pszICCID, dwSize);

	return TRUE;
}
