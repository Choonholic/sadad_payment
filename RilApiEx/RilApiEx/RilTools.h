// RilTools.h

#pragma once

#include <map>
#include "ril.h"
#include "Definitions.h"
#include "Cmd.h"

using	namespace	std;

class CRilTools
{
private:
	CRilTools();
	~CRilTools();

	void	ResultProc(DWORD dwResultCode, HRESULT hrCommandID, const void* pData, DWORD dwDataSize);
	
	CRITICAL_SECTION	m_csMap;
	map<LONG, CCmd*>	m_mapCommand;
	HRIL				m_hRil;
	static CRilTools*	m_pRilTools;
	HANDLE				m_hStopWaitingEvent;

public:
	void	StopWaiting(void);

	static	CRilTools*	CreateInstance();
	static	void		DestroyInstance();
	static	void		RTK_RILResultProc(DWORD dwResultCode, HRESULT hrCommandID, const void* pData, DWORD dwDataSize, DWORD dwParam);

	int	GetEquipmentState(RILEQUIPMENTSTATE& RilEquipmentState);
	int	SetEquipmentState(DWORD dwEquipmentState);

	int	GetEquipmentInfo(RILEQUIPMENTINFO& RilEquipmentInfo);

	int	GetVersionInfo(RADIO_VERSION_INFO& RadioVersionInfo);

	int	GetPhoneLockedState(DWORD& dwPhoneLockedState);

	int	GetPinCount(DWORD& dwPinCount);

	int	UnlockPhone(LPCSTR lpszPassword, LPCSTR lpszNewPassword);

	int	GetLockingStatus(DWORD dwFacility, LPCSTR lpszPasword, DWORD& dwLockingStatus);
	int	SetLockingStatus(DWORD dwFacility, LPCSTR lpszPassword, DWORD dwLockingStatus);

	int	ChangeLockingPassword(DWORD dwFacility, DWORD dwOldPasswordType, LPCSTR lpszOldPassword, LPCSTR lpszNewPassword);

	int	GetRegistrationStatus(DWORD& dwRegStatus);
	int	GetRegistrationStatus2(DWORD& dwRegStatus);

	int	GetCurrentOperator(RILOPERATORNAMES& RilOperatorNames, DWORD dwFormat);

	int	SendATCommand(LPCSTR szATCommand, LPCSTR szResponse, DWORD dwResponseLen);

	int	SendRestrictedSimCmd(DWORD dwCommand, RILSIMCMDPARAMETERS simcmd, BYTE* szResponse, int &nResponselen);

	int	GetICCID(LPCSTR szICCID);
};
