// Definitions.h

#pragma once

#define RILAPI_SUCCESS					0
#define RILAPI_FAILED					1
#define	RILAPI_CRITICAL_ERROR			(-1)

#define RIL_DEVSPECIFIC_GETVERSIONINFO	0x03
#define RIL_DEVSPECIFIC_SEND_ATC		0x06
#define RIL_DEVSPECIFIC_GET_PIN_COUNT	0x90
#define RIL_DEVSPECIFIC_GET_REGSTATUS	0x91
#define RIL_DEVSPECIFIC_GET_ICCID		0x92

#define MAX_RETRY_COUNT					20

#define RK_PHONE						TEXT("System\\State\\Phone")
#define RV_STATUS						TEXT("Status")
#define RV_SIGNAL						TEXT("Signal Strength Raw")

typedef struct tagRadioVersionInfo
{
	DWORD	dwStructVersion;
	DWORD	dwRILVersion;
	DWORD	dwMuxVersion;
	DWORD	dwAudVersion;
	DWORD	dwRHAVersion;
	DWORD	dwUSBDrvVersion;
	DWORD	dwNDISAPIVersion;
} RADIO_VERSION_INFO, *PRADIO_VERSION_INFO;
