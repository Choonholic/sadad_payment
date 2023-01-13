// Device.h

#pragma once

#include "Settings.h"

#define	MAX_BACKLIGHT_LEVEL	15

#if defined(MODEL_CT360)
#define NETWORK_DRIVER_NAME	L"SMSC95001"
#endif

BOOL	GetSerialNumber(WCHAR* szSerial, UINT nSize);
BOOL	GetPOSHardwareVersion(WCHAR* szVersion, UINT nSize);
BOOL	GetPOSFirmwareVersion(WCHAR* szVersion, UINT nSize);

BOOL	SetPOSInformation(BYTE* cBuffer, UINT nSize);

int		GetBrightness();
void	SetBrightness(int nLevel);

BOOL	GetModel(WCHAR* szModel, UINT nSize);
BOOL	GetDeviceType(WCHAR* szType, UINT nSize);
BOOL	GetROMVersion(WCHAR* szVersion, UINT nSize);
BOOL	GetPsVersion(WCHAR* szVersion, UINT nSize);
BOOL	GetImageCode(WCHAR* szCode, UINT nSize);
BOOL	GetMAXQVersion(WCHAR* szVersion, UINT nSize);

BOOL	RebindNetworkAdapter(WCHAR* szAdapterName);

void	AdjustPrinterPaperSensor(DWORD dwValue = 0);
void	SetRestartFlag(BOOL bRestart = TRUE);

void	SetNetworkAdapter(PARAMETERS_INFO* pInfo);

GPRS_STATUS		GetSignalStrength(DWORD dwSignalStrength);
BATTERY_STATUS	GetBatteryStatus();
BOOL			EnableBatteryPopup(BOOL bEnable);

BOOL	ResetDevice();
