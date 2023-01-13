// RilApis.h

#pragma once

#ifdef RILAPIEX_EXPORTS
#define RILAPI	__declspec(dllexport) 
#else
#define RILAPI	__declspec(dllimport) 
#endif

#define RILAPIEX_VERSION	0x00030000	// 0.3.0

RILAPI	BOOL	RilApi_Initialize();
RILAPI	void	RilApi_Cleanup();

RILAPI	HRESULT	RilApi_GetLastResult();

#define EQSTATE_UNKNOWN				0	// Unknown
#define EQSTATE_MINIMUM				1	// Phone off
#define EQSTATE_FULL				2	// Phone on

RILAPI	BOOL	RilApi_GetEquipmentState(DWORD& dwEquipmentState);
RILAPI	BOOL	RilApi_SetEquipmentState(DWORD dwEquipmentState);

typedef struct _EQUIPMENTINFO
{
	TCHAR	szManufacturer[128];
	TCHAR	szModel[128];
	TCHAR	szRevision[128];
	TCHAR	szSerialNumber[128];
} EQUIPMENTINFO, *LPEQUIPMENTINFO;

RILAPI	BOOL	RilApi_GetEquipmentInfo(EQUIPMENTINFO& EquipmentInfo);

typedef struct _VERSIONINFO
{
	TCHAR	szRILVersion[128];
	TCHAR	szRHAVersion[128];
	TCHAR	szMuxVersion[128];
} VERSIONINFO, *LPVERSIONINFO;

RILAPI	BOOL	RilApi_GetVersionInfo(VERSIONINFO& VersionInfo);

#define SIM_NOT_INSERTED			0	// SIM is not inserted
#define SIM_INSERTED				1	// SIM is inserted

RILAPI	BOOL	RilApi_GetSimInserted(DWORD& dwSimInserted);

#define LOCKEDSTATE_UNKNOWN			0	// Unknown
#define LOCKEDSTATE_READY			1	// Maintenance Entity Mobile Equipment(ME) not locked
#define LOCKEDSTATE_SIM_PIN			2	// ME awaiting PIN
#define LOCKEDSTATE_SIM_PUK			3	// ME awaiting PUK

RILAPI	BOOL	RilApi_GetPhoneLockedState(DWORD& dwPhoneLockedState);
RILAPI	BOOL	RilApi_GetPinCount(DWORD& dwPinCount);
RILAPI	BOOL	RilApi_UnlockPhone(LPCSTR lpszPassword, LPCSTR lpszNewPassword);

#define LOCKINGSTATUS_UNKNOWN		0	// Unknown
#define LOCKINGSTATUS_DISABLED		1	// Disabled
#define LOCKINGSTATUS_ENABLED		2	// Enabled
 
RILAPI	BOOL	RilApi_GetLockingStatus(DWORD& dwLockingStatus);
RILAPI	BOOL	RilApi_SetLockingStatus(LPCSTR lpszPassword, DWORD dwLockingStatus);
RILAPI	BOOL	RilApi_ChangeLockingPassword(LPCSTR lpszOldPassword, LPCSTR lpszNewPassword);

#define REGSTAT_UNKNOWN				0	// Unknown
#define REGSTAT_UNREGISTERED		1	// Unregistered
#define REGSTAT_HOME				2	// Registered on home network
#define REGSTAT_ATTEMPTING			3	// Attempting to register
#define REGSTAT_DENIED				4	// Registration denied
#define REGSTAT_ROAMING				5	// Registered on roaming network

RILAPI	BOOL	RilApi_GetRegistrationStatus(DWORD& dwRegStatus);
RILAPI	BOOL	RilApi_GetPacketDomainRegistrationStatus(DWORD& dwPacketDomainRegStatus);

typedef struct _OPERATORINFO
{
    TCHAR	szLongName[32];
	TCHAR	szShortName[16];
	TCHAR	szNumName[16];
	TCHAR	szCountryCode[8];
} OPERATORINFO, *LPOPERATORINFO;

RILAPI	BOOL	RilApi_GetCurrentOperator(OPERATORINFO& OperatorInfo);

RILAPI	BOOL	RilApi_GetSignalStrength(DWORD& dwSignalStrength);

#define PHONE_NONE						0x00000000	// None
#define PHONE_SIM_FULL					0x00000001	// the Subscriber Identity Module (SIM) is full
#define PHONE_SIM_INSTALLED				0x00000002	// a SIM is installed in the mobile device
#define PHONE_SIM_INVALID				0x00000004	// the SIM is invalid
#define PHONE_SIM_BLOCKED				0x00000008	// the SIM is blocked
#define PHONE_RADIO_OFF					0x00000010	// the phone's radio is turned off
#define PHONE_PHONE_EXIST				0x00000020	// the mobile device has a phone
#define PHONE_RINGER_OFF				0x00000040	// the phone's ringer is off
#define PHONE_LINE1_SELECTED			0x00000080	// Line 1 is selected
#define PHONE_LINE2_SELECTED			0x00000100	// Line 2 is selected
#define PHONE_IN_ROAMING				0x00000200	// the phone is currently in roaming mode
#define PHONE_LINE1_CALL_FORWARDING		0x00000400	// call forwarding is enabled on Line 1
#define PHONE_CALL_MISSED				0x00000800	// a call has been missed
#define PHONE_ACTIVE_DATA_CONNECTION	0x00001000	// the phone has an active cellular data connection
#define PHONE_CALL_BARRING_ENABLED		0x00002000	// the call barring feature is enabled
#define PHONE_CALL_ON_HOLD				0x00004000	// a phone call is currently on hold
#define PHONE_CONFERENCE_CALL_PROGRESS	0x00008000	// a conference call is currently in progress
#define PHONE_INCOMING_CALL				0x00010000	// there is an incoming call
#define PHONE_ATTEMPTING_CONNECT_CALL	0x00020000	// the phone is attempting to connect an outgoing call
#define PHONE_GPRS_COVERAGE				0x00100000	// the phone currently has GPRS coverage
#define PHONE_CONNECTED_NETWORK			0x00200000	// the phone is currently connected to a network
#define	PHONE_SEARCHING_NETWORK			0x00400000	// the phone is currently searching for service on a network
#define PHONE_REGISTERED_ON_HOME		0x00800000	// the phone is registered on its home network
#define PHONE_1xRTT_COVERAGE			0x01000000	// the phone currently has 1xRTT coverage
#define PHONE_IN_TALKING				0x02000000	// there is currently a phone call in the talking state
#define PHONE_LINE2_CALL_FORWARDING		0x04000000	// call forwarding is active on Line 2

RILAPI	BOOL	RilApi_GetPhoneStatus(DWORD& dwStatus);

RILAPI	BOOL	RilApi_DataConnect(DWORD& dwStatus);
RILAPI	void	RilApi_DataDisconnect();

#define DATA_DISCONNECTED			0	// Disconnected
#define DATA_CONNECTED				1	// Connected

RILAPI	BOOL	RilApi_GetDataConnectionStatus(DWORD& dwDataConnectionStatus);
RILAPI	BOOL	RilApi_GetDataConnectionStatusStandAlone(DWORD& dwDataConnectionStatus);

typedef struct _GPRSENTRY
{
	TCHAR	szName[256];
	TCHAR	szAPN[256];
	TCHAR	szUserName[256];
	TCHAR	szPassword[256];
	BOOL	bEnabled;
} GPRSENTRY, *LPGPRSENTRY;

RILAPI	BOOL	RilApi_AddGprsEntry(GPRSENTRY GprsEntry);
RILAPI	BOOL	RilApi_DeleteGprsEntry(TCHAR szName[]);
RILAPI	BOOL	RilApi_GetGprsEntryInfo(GPRSENTRY GprsEntry[], DWORD dwBufferSize, DWORD& dwEntryCount);
RILAPI	BOOL	RilApi_EnableGprsEntry(TCHAR szName[]);
RILAPI	BOOL	RilApi_DisableGprsEntry(TCHAR szName[]);

RILAPI	BOOL	RilApi_SendATCommand(LPCSTR szATCommand, LPCSTR szResponse, DWORD dwResonseLen);
RILAPI	BOOL	RilApi_GetICCID(TCHAR* pszICCID, DWORD dwSize);
