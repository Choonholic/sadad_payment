// Utilities.cpp

#include "StdAfx.h"
#include "RilApiEx.h"
#include <connmgr.h>
#include <cfgmgrapi.h>
#include <ras.h>
#include "Utilities.h"
#include "RilApis.h"

CRilTools*	g_pRilTools;
HRESULT		g_hResult					= NULL;
BOOL		g_bDataState				= FALSE;
HANDLE		g_hConnection				= NULL;
TCHAR		g_szGetGprsEntryName[2048]	= L"<wap-provisioningdoc><characteristic-query type=\"CM_GPRSEntries\" /></wap-provisioningdoc>";
TCHAR		g_szAddGprsEntry[4096]		= L"<wap-provisioningdoc><characteristic type=\"CM_GPRSEntries\"><characteristic type=\"INPUT_NAME\"><parm name=\"DestId\" value=\"{436EF144-B4FB-4863-A041-8F905A62C572}\" /><parm name=\"UserName\" value=\"INPUT_USER_NAME\" /><parm name=\"Password\" value=\"INPUT_PASSWORD\" /><parm name=\"Domain\" value=\"\" /><parm name=\"AlwaysOn\" value=\"1\" /><parm name=\"Enabled\" value=\"0\" /><characteristic type=\"DevSpecificCellular\"><parm name=\"GPRSInfoValid\" value=\"1\" /><parm name=\"GPRSInfoAccessPointName\" value=\"INPUT_APN\" /></characteristic></characteristic></characteristic></wap-provisioningdoc>";
TCHAR		g_szDeleteGprsEntry[2048]	= L"<wap-provisioningdoc><characteristic type=\"CM_GPRSEntries\"><nocharacteristic type=\"INPUT_NAME\" /></characteristic></wap-provisioningdoc>";
TCHAR		g_szEnableGprsEntry[2048]	= L"<wap-provisioningdoc><characteristic type=\"CM_GPRSEntries\"><characteristic type=\"INPUT_NAME\"><parm name=\"Enabled\" value=\"1\" /><parm name=\"AlwaysOn\" value=\"1\" /></characteristic></characteristic></wap-provisioningdoc>";
TCHAR		g_szDisableGprsEntry[2048]	= L"<wap-provisioningdoc><characteristic type=\"CM_GPRSEntries\"><characteristic type=\"INPUT_NAME\"><parm name=\"Enabled\" value=\"0\" /><parm name=\"AlwaysOn\" value=\"0\" /></characteristic></characteristic></wap-provisioningdoc>";

LONG GetRegistryDWORD(HKEY hKeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD* pValue)
{
    HKEY	hKey			= NULL;
    DWORD	dwDisposition	= REG_CREATED_NEW_KEY;
    DWORD	cbValue;
    LONG	lRet			= RegCreateKeyEx(hKeyRoot, lpSubKey, 0, (LPTSTR)lpValueName, 0, 0, NULL, &hKey, &dwDisposition);

	if (lRet != ERROR_SUCCESS)
    {
        return lRet;
    }

	cbValue	= sizeof(DWORD);
    lRet	= RegQueryValueEx(hKey, lpValueName, NULL, NULL, (LPBYTE)pValue, &cbValue);

	if (lRet != ERROR_SUCCESS)
    {
		RegCloseKey(hKey);

		return lRet;
    }

	RegCloseKey(hKey);

	return ERROR_SUCCESS;
}

GUID GetNetworkForURL(LPCTSTR szUrl)
{
	DWORD	dwIndex	= 0;
	GUID	rv;

	if (!SUCCEEDED(ConnMgrMapURL(szUrl, &rv, &dwIndex)))
	{
		rv	= GUID_NULL;
	}

	return rv;
}

void DataDisconnect(TCHAR* tszName)
{
	RASCONN	RasConn[50];
	DWORD	dwRasConnSize;
	DWORD	dwNumConnections;
	UINT	index;
	DWORD	dwRasRtn	= 0;

	RasConn[0].dwSize	= sizeof(RASCONN);
	dwRasConnSize		= 50 * sizeof(RASCONN);
	dwRasRtn			= RasEnumConnections(RasConn, &dwRasConnSize, &dwNumConnections);

	if (0 != dwRasRtn)
	{
		return;
	}

	if (!dwNumConnections)
	{
		return;
	}

	for (index = 0; index < (int)dwNumConnections; ++index)
	{
		if (!wcscmp(tszName, RasConn[index].szEntryName))
		{
			if (0 != RasHangUp(RasConn[index].hrasconn))
			{
				return;
			}
		}
	}
}

void GetEnabledGprsEntryName(TCHAR* tszName)
{
	LPWSTR		pOutput		= NULL;
	HRESULT		hr			= NULL;
	BOOL		bRes		= FALSE;
	DWORD		dwCount		= 0;
	int			nEntry		= 0;
	GPRSENTRY	GprsEntry[5];

	ZeroMemory(GprsEntry, sizeof(GprsEntry));

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

		for (int i = 0; i < 32; i++)
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

			GprsEntry[dwCount].bEnabled = (BOOL)(_tstoi(strEnabled.GetBuffer()) > 0);

			dwCount++;
		}
	}

	delete [] pOutput;

	for (int i = 0; i < (int)dwCount; i++)
	{
		if (GprsEntry[i].bEnabled == TRUE)
		{
			wcscpy(tszName, GprsEntry[i].szName);
			break;
		}
	}
}

BOOL DataState(TCHAR* tszName)
{
	RASCONNSTATUS	RasConnStatus;
	RASCONN			RasConn[50];
	DWORD			dwRasConnSize;
	DWORD			dwNumConnections;
	DWORD			dwRasRtn	= 0;
	
	g_bDataState		= FALSE;
	RasConn[0].dwSize	= sizeof(RASCONN);
	dwRasConnSize		= 50 * sizeof(RASCONN);
	dwRasRtn			= RasEnumConnections(RasConn, &dwRasConnSize, &dwNumConnections);

	if ((0 != dwRasRtn) || (!dwNumConnections))
	{
		return g_bDataState;
	}

	for (DWORD i = 0; i < dwNumConnections; ++i)
	{
		if (!wcscmp(tszName, RasConn[i].szEntryName))
		{
			if (RasGetConnectStatus(RasConn[i].hrasconn, &RasConnStatus) != 0)
			{
				return g_bDataState;
			}

			if (RasConnStatus.rasconnstate == RASCS_Connected)
			{
				g_bDataState	= TRUE;
			}

			return g_bDataState;
		}
	}

	return g_bDataState;
}
