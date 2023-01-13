// Device.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "bbsysapi.h"
#include "PidionSystem.h"
#include "drv_glob.h"
#include "BBMemoryAlloc.hpp"
#include "winioctl.h"
#include "ntddndis.h"
#include "Definitions.h"
#include "Utilities.h"
#include "Device.h"
#include "Ras.h"
#include "Pm.h"

// POS Information

#define DEF_HW_VERSION				L"00002"

BOOL GetSerialNumber(WCHAR* szSerial, UINT nSize)
{
	if (nSize < 17)
	{
		return FALSE;
	}

#if TEST_MODE
	wcscpy_s(szSerial, nSize, TEST_DEVICE_ID);

	return TRUE;
#else
	memset(szSerial, 0, nSize * sizeof(WCHAR));

	return BBGetSerialNumber(szSerial, nSize);
#endif
}

BOOL GetPOSHardwareVersion(WCHAR* szVersion, UINT nSize)
{
	if (nSize < 6)
	{
		return FALSE;
	}

#if TEST_MODE
	wcscpy_s(szVersion, nSize, TEST_HW_VERSION);
#else
	wcscpy_s(szVersion, nSize, DEF_HW_VERSION);
#endif

	return TRUE;
}

BOOL GetPOSFirmwareVersion(WCHAR* szVersion, UINT nSize)
{
#if TEST_MODE
	wcscpy_s(szVersion, nSize, TEST_FW_VERSION);
#else
	DWORD	dwROMVersion;
	WCHAR	szROMVersion[32];

	memset(szROMVersion, 0, 32 * sizeof(WCHAR));
	GetROMVersion(szROMVersion, 32);

	dwROMVersion	= _wtoi(szROMVersion);

	swprintf(szROMVersion, L"%06d", dwROMVersion);

	if (nSize <= wcslen(szROMVersion))
	{
		return FALSE;
	}

	wcscpy_s(szVersion, nSize, szROMVersion);
#endif

	return TRUE;
}

BOOL SetPOSInformation(BYTE* cBuffer, UINT nSize)
{
	WCHAR	szBuffer[24];
	BYTE	cByte[24];
	int		nPos	= 0;

	if (nSize < LENGTH_POS_INFORMATION)
	{
		return FALSE;
	}

	GetPOSHardwareVersion(szBuffer, 24);
	WA2BA(cByte, 24, szBuffer, wcslen(szBuffer));

	LOGFILE(L"%S[%d] - HW: '%s'\r\n", __FUNCTION__, __LINE__, szBuffer);

	if (strlen((const char*)cByte) < 5)
	{
		memset(&cBuffer[nPos], 0x30, 5 - strlen((const char*)cByte));

		nPos	+= 5 - strlen((const char*)cByte);
	}

	memcpy(&cBuffer[nPos], cByte, strlen((const char*)cByte));

	nPos	+= strlen((const char*)cByte);

	GetPOSSoftwareVersion(szBuffer, 24);
	WA2BA(cByte, 24, szBuffer, wcslen(szBuffer));

	LOGFILE(L"%S[%d] - SW: '%s'\r\n", __FUNCTION__, __LINE__, szBuffer);

	if (strlen((const char*)cByte) < 6)
	{
		memset(&cBuffer[nPos], 0x30, 6 - strlen((const char*)cByte));

		nPos	+= 6 - strlen((const char*)cByte);
	}

	memcpy(&cBuffer[nPos], cByte, strlen((const char*)cByte));

	nPos	+= strlen((const char*)cByte);

	GetPOSFirmwareVersion(szBuffer, 24);
	WA2BA(cByte, 24, szBuffer, wcslen(szBuffer));

	LOGFILE(L"%S[%d] - FW: '%s'\r\n", __FUNCTION__, __LINE__, szBuffer);

	if (strlen((const char*)cByte) < 6)
	{
		memset(&cBuffer[nPos], 0x30, 6 - strlen((const char*)cByte));

		nPos	+= 6 - strlen((const char*)cByte);
	}

	memcpy(&cBuffer[nPos], cByte, strlen((const char*)cByte));

	nPos	+= strlen((const char*)cByte);

	GetSerialNumber(szBuffer, 24);
	WA2BA(cByte, 24, szBuffer, wcslen(szBuffer));

	LOGFILE(L"%S[%d] - SN: '%s'\r\n", __FUNCTION__, __LINE__, szBuffer);

	if (strlen((const char*)cByte) < 16)
	{
		memset(&cByte[strlen((const char*)cByte)], 0x20, 16 - strlen((const char*)cByte));
	}

	cByte[16]	= NULL;

	memcpy(&cBuffer[nPos], cByte, strlen((const char*)cByte));

	nPos	+= strlen((const char*)cByte);

	return (BOOL)(nPos == LENGTH_POS_INFORMATION);
}

int GetBrightness()
{
	DWORD	dwLevel;
	HBBLCD	hLCD	= BBLCDOpen(0);

	BBLCDGetDimm(hLCD, &dwLevel);
	BBLCDClose(hLCD);

	return ((dwLevel == 0) ? 0 : (dwLevel / 2) + 1);
}

void SetBrightness(int nLevel)
{
	HBBLCD	hLCD	= BBLCDOpen(0);

	BBLCDSetDimm(hLCD, (nLevel * 2) - 1);
	BBLCDClose(hLCD);
}

BOOL GetModel(WCHAR* szModel, UINT nSize)
{
	memset(szModel, 0, nSize);

	return BBGetProductName(szModel, nSize);
}

BOOL GetDeviceType(WCHAR* szType, UINT nSize)
{
	memset(szType, 0, nSize);

	return BBGetDeviceInfo(BB_INFO_PRODUCT_TYPE, szType, nSize);
}

BOOL GetROMVersion(WCHAR* szVersion, UINT nSize)
{
	memset(szVersion, 0, nSize);

	return BBGetDeviceInfo(BB_INFO_BUILD_VERSION, szVersion, nSize);
}

BOOL GetImageCode(WCHAR* szCode, UINT nSize)
{
	memset(szCode, 0, nSize);

	return BBGetDeviceInfo(BB_INFO_IMAGE_CODE, szCode, nSize);
}

BOOL GetMAXQVersion(WCHAR* szVersion, UINT nSize)
{
	memset(szVersion, 0, nSize);

	return BBGetDeviceInfo(BB_INFO_SECURE_PROCESSOR, szVersion, nSize);
}

BOOL GetPsVersion(WCHAR* szVersion, UINT nSize)
{
	memset(szVersion, 0, nSize);

	return BBGetDeviceInfo(BB_INFO_PS_VERSION, szVersion, nSize);
}

BOOL RebindNetworkAdapter(WCHAR* szAdapterName)
{
	HANDLE	hNDIS;
	BOOL	bResult			= FALSE;
	DWORD	dwCount;
	int		nLength			= wcslen(szAdapterName);
	int		nAdapterSize	= sizeof(WCHAR) * (nLength + 2);
	WCHAR	szAdapter[64];

	wcscpy(szAdapter, szAdapterName);

	szAdapter[nLength + 0]	= L'\0';
	szAdapter[nLength + 1]	= L'\0';

	hNDIS	= CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);

	if (hNDIS != INVALID_HANDLE_VALUE)
	{
		bResult	= DeviceIoControl(hNDIS, IOCTL_NDIS_REBIND_ADAPTER, szAdapter, nAdapterSize, NULL, 0, &dwCount, NULL);

		CloseHandle(hNDIS);
	}

	return bResult;
}

void AdjustPrinterPaperSensor(DWORD dwValue)
{
	HKEY	hKey;
	DWORD	dwDisposition;

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PRINTER, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	switch (dwValue)
	{
	case 0:
		{
		}
		break;
	case MAXDWORD:
		{
			RegDeleteValue(hKey, RV_ADC_NOPAPER);
		}
		break;
	default:
		{
			DWORD	dwType	= REG_DWORD;
			DWORD	dwSize	= sizeof(DWORD);

			RegSetValueEx(hKey, RV_ADC_NOPAPER, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
		}
		break;
	}

	RegCloseKey(hKey);
}

void SetRestartFlag(BOOL bRestart)
{
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwType	= REG_DWORD;
	DWORD	dwSize	= sizeof(DWORD);
	DWORD	dwValue	= (bRestart ? 1 : 0);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_BLUEBIRD, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	RegSetValueEx(hKey, RV_APP_KEEP_EXECUTE, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
	RegCloseKey(hKey);
}

void SetNetworkAdapter(PARAMETERS_INFO* pInfo)
{
#if defined(MODEL_CT360)
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwValue;
	WCHAR	szValue[16];

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_SMSC95001_TCPIP, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	if (wcslen(pInfo->szPOSIP))
	{
		dwValue	= 0;

		RegSetValueEx(hKey, L"EnableDHCP", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
		RegSetValueEx(hKey, L"IpAddress", 0, REG_SZ, reinterpret_cast<LPBYTE>(pInfo->szPOSIP), sizeof(WCHAR) * (wcslen(pInfo->szPOSIP) + 1));
		RegSetValueEx(hKey, L"DefaultGateway", 0, REG_SZ, reinterpret_cast<LPBYTE>(pInfo->szPOSGateway), sizeof(WCHAR) * (wcslen(pInfo->szPOSGateway) + 1));
		RegSetValueEx(hKey, L"Subnetmask", 0, REG_SZ, reinterpret_cast<LPBYTE>(pInfo->szPOSSubnetMask), sizeof(WCHAR) * (wcslen(pInfo->szPOSSubnetMask) + 1));
	}
	else
	{
		dwValue	= 1;
		
		wcscpy(szValue, L"0.0.0.0");

		RegSetValueEx(hKey, L"EnableDHCP", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
		RegSetValueEx(hKey, L"IpAddress", 0, REG_SZ, reinterpret_cast<LPBYTE>(szValue), sizeof(WCHAR) * (wcslen(szValue) + 1));
		RegSetValueEx(hKey, L"DefaultGateway", 0, REG_SZ, reinterpret_cast<LPBYTE>(szValue), sizeof(WCHAR) * (wcslen(szValue) + 1));
		RegSetValueEx(hKey, L"Subnetmask", 0, REG_SZ, reinterpret_cast<LPBYTE>(szValue), sizeof(WCHAR) * (wcslen(szValue) + 1));
	}

	RegCloseKey(hKey);
	RebindNetworkAdapter(NETWORK_DRIVER_NAME);
#endif
}

GPRS_STATUS GetSignalStrength(DWORD dwSignalStrength)
{
	GPRS_STATUS	gprsStatus	= GPRS_NONE;

#if GPRS_AVAILABLE
	if (dwSignalStrength == 0)
	{
		gprsStatus	= GPRS_NONE;
	}
	else if (dwSignalStrength <= 20)
	{
		gprsStatus	= GPRS_20;
	}
	else if (dwSignalStrength <= 40)
	{
		gprsStatus	= GPRS_40;
	}
	else if (dwSignalStrength <= 60)
	{
		gprsStatus	= GPRS_60;
	}
	else if (dwSignalStrength <= 80)
	{
		gprsStatus	= GPRS_80;
	}
	else if (dwSignalStrength <= 100)
	{
		gprsStatus	= GPRS_FULL;
	}
#endif

	return gprsStatus;
}

BATTERY_STATUS GetBatteryStatus()
{
#if BATTERY_EXIST
	BATTERY_STATUS			batteryStatus		= BATTERY_EMPTY;
	SYSTEM_POWER_STATUS_EX	SystemPowerStatus;

	if (!GetSystemPowerStatusEx(&SystemPowerStatus, TRUE))
	{
		batteryStatus	= BATTERY_EMPTY;
	}
	else
	{
		if (SystemPowerStatus.ACLineStatus == AC_LINE_ONLINE)
		{
			switch (SystemPowerStatus.BatteryFlag)
			{
			case BATTERY_FLAG_UNKNOWN:
			case BATTERY_FLAG_NO_BATTERY:
				{
					batteryStatus	= BATTERY_AC_EMPTY;
				}
				break;
			default:
				{
					if (SystemPowerStatus.BatteryLifePercent < 33)
					{
						batteryStatus	= BATTERY_AC_LOW;
					}
					else if (SystemPowerStatus.BatteryLifePercent < 66)
					{
						batteryStatus	= BATTERY_AC_HIGH;
					}
					else
					{
						batteryStatus	= BATTERY_AC_FULL;
					}
				}
				break;
			}
		}
		else
		{
			switch (SystemPowerStatus.BatteryFlag)
			{
			case BATTERY_FLAG_UNKNOWN:
			case BATTERY_FLAG_NO_BATTERY:
				{
					batteryStatus	= BATTERY_EMPTY;
				}
				break;
			default:
				{
					if (SystemPowerStatus.BatteryLifePercent < 15)
					{
						batteryStatus	= BATTERY_CRITICAL;
					}
					else if (SystemPowerStatus.BatteryLifePercent < 33)
					{
						batteryStatus	= BATTERY_LOW;
					}
					else if (SystemPowerStatus.BatteryLifePercent < 66)
					{
						batteryStatus	= BATTERY_HIGH;
					}
					else
					{
						batteryStatus	= BATTERY_FULL;
					}
				}
				break;
			}
		}
	}

	return batteryStatus;
#else
	return BATTERY_AC_EMPTY;
#endif
}

BOOL EnableBatteryPopup(BOOL bEnable)
{
#if BATTERY_EXIST
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwValue	= (bEnable ? 1 : 0);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_BATTERY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	RegSetValueEx(hKey, RV_POWEROFF_POPUP, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
	RegCloseKey(hKey);

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL ResetDevice()
{
	return (BOOL)(SetSystemPowerState(NULL, POWER_STATE_RESET, 0) == ERROR_SUCCESS);
}
