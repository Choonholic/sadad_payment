// RilTools.cpp

#include "StdAfx.h"
#include "RilApiEx.h"
#include "RilTools.h"

CRilTools*	CRilTools::m_pRilTools;

CRilTools::CRilTools() : m_hRil(NULL)
{
	::InitializeCriticalSection(&m_csMap);

	m_hStopWaitingEvent	= ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CRilTools::~CRilTools()
{
	::DeleteCriticalSection(&m_csMap);
	::SetEvent(m_hStopWaitingEvent);
	::CloseHandle(m_hStopWaitingEvent);

	if (m_hRil != NULL)
	{
		RIL_Deinitialize(m_hRil);
	}
	
	map<LONG, CCmd*>::iterator pos;
	
	for (pos = m_mapCommand.begin(); pos != m_mapCommand.end(); ++pos)
	{
		delete pos->second;
	}
}

void CRilTools::ResultProc(DWORD dwResultCode, HRESULT hrCommandID, const void* pData, DWORD dwDataSize)
{
	CCmd*	pCmd;

	if (m_mapCommand.find(hrCommandID) == m_mapCommand.end())
	{
		return;
	}
	
	::EnterCriticalSection(&m_csMap);

	map<LONG, CCmd*>::iterator	k	= m_mapCommand.find(hrCommandID);

	if (k != m_mapCommand.end())
	{
		pCmd	= m_mapCommand[hrCommandID];

		pCmd->SetResult(dwResultCode);
		pCmd->SetBlob(pData, dwDataSize);
		::SetEvent(pCmd->GetResponseEvent());
	}

	::LeaveCriticalSection(&m_csMap);
}

CRilTools* CRilTools::CreateInstance()
{
	if (m_pRilTools == NULL)
	{
		m_pRilTools	= new CRilTools;

		HRESULT	hr	= RIL_Initialize(1, RTK_RILResultProc, NULL, 0, (DWORD)m_pRilTools, &m_pRilTools->m_hRil);

		if (FAILED(hr))
		{
			m_pRilTools->m_hRil	= NULL;

			return NULL;
		}
	}
	
	return m_pRilTools;
}

void CRilTools::DestroyInstance()
{
	delete m_pRilTools;

	m_pRilTools	= NULL;
}

void CRilTools::RTK_RILResultProc(DWORD dwResultCode, HRESULT hrCommandID, const void* pData, DWORD dwDataSize, DWORD dwParam)
{
	((CRilTools*)dwParam)->ResultProc(dwResultCode, hrCommandID, pData, dwDataSize);
}

int CRilTools::GetEquipmentState(RILEQUIPMENTSTATE& RilEquipmentState)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_GetEquipmentState(m_hRil, NULL);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			RILEQUIPMENTSTATE*	pRilEquipmentState	= NULL;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pRilEquipmentState	= (RILEQUIPMENTSTATE*)pCmd->GetData(NULL);
				RilEquipmentState	= *pRilEquipmentState;
				nFunctionResult		= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::SetEquipmentState(DWORD dwEquipmentState)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_SetEquipmentState(m_hRil, dwEquipmentState);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 10000) == WAIT_OBJECT_0)
		{
			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetEquipmentInfo(RILEQUIPMENTINFO& RilEquipmentInfo)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_GetEquipmentInfo(m_hRil);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 10000) == WAIT_OBJECT_0)
		{
			RILEQUIPMENTINFO*	pRilEquipmentInfo;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pRilEquipmentInfo	= (RILEQUIPMENTINFO*)pCmd->GetData(NULL);
				RilEquipmentInfo	= *pRilEquipmentInfo;
				nFunctionResult		= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetVersionInfo(RADIO_VERSION_INFO& RadioVersionInfo)
{
	HRESULT	hresCmdID;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	BYTE	byBuffer[256];
	BYTE	byAsyncResults[256];
	DWORD	dwAsyncResults	= 0;
	DWORD	dwCode			= RIL_DEVSPECIFIC_GETVERSIONINFO;
	
	ZeroMemory(byBuffer, sizeof(byBuffer));
	ZeroMemory(byAsyncResults, sizeof(byAsyncResults));
	memcpy((void*)byBuffer, (void*)&dwCode, sizeof(DWORD));

	hresCmdID	= RIL_DevSpecific(m_hRil, (BYTE*)byBuffer, sizeof(byBuffer), (BYTE*)byAsyncResults, sizeof(byAsyncResults), &dwAsyncResults, 2500);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		memcpy((void*)&RadioVersionInfo, (void*)byAsyncResults, dwAsyncResults);

		nFunctionResult	= RILAPI_SUCCESS;
	}

	return nFunctionResult;
}

int CRilTools::GetPhoneLockedState(DWORD& dwPhoneLockedState)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_GetPhoneLockedState(m_hRil);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			DWORD*	pdwPhoneLockedState;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pdwPhoneLockedState	= (DWORD*)pCmd->GetData(NULL);
				dwPhoneLockedState	= *pdwPhoneLockedState;
				nFunctionResult		= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetPinCount(DWORD& dwPinCount)
{
	HRESULT	hresCmdID;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	BYTE	byBuffer[256];
	BYTE	byAsyncResults[256];
	DWORD	dwAsyncResults	= 0;
	DWORD	dwCode			= RIL_DEVSPECIFIC_GET_PIN_COUNT;
	
	ZeroMemory(byBuffer, sizeof(byBuffer));
	ZeroMemory(byAsyncResults, sizeof(byAsyncResults));
	memcpy((void*)byBuffer, (void*)&dwCode, sizeof(DWORD));

	hresCmdID	= RIL_DevSpecific(m_hRil, (BYTE*)byBuffer, sizeof(byBuffer), (BYTE*)byAsyncResults, sizeof(byAsyncResults), &dwAsyncResults, 2500);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		memcpy((void*)&dwPinCount, (void*)byAsyncResults, dwAsyncResults);

		nFunctionResult	= RILAPI_SUCCESS;
	}
	
	return nFunctionResult;
}

int CRilTools::UnlockPhone(LPCSTR lpszPassword, LPCSTR lpszNewPassword)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_UnlockPhone(m_hRil, lpszPassword, lpszNewPassword);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			if (pCmd->GetResult() == RIL_RESULT_OK)
			{
				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetLockingStatus(DWORD dwFacility, LPCSTR lpszPassword, DWORD& dwLockingStatus)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_GetLockingStatus(m_hRil, dwFacility, lpszPassword);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			DWORD*	pdwLockingStatus;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pdwLockingStatus	= (DWORD*)pCmd->GetData(NULL);
				dwLockingStatus		= *pdwLockingStatus;
				nFunctionResult		= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::SetLockingStatus(DWORD dwFacility, LPCSTR lpszPassword, DWORD dwLockingStatus)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_SetLockingStatus(m_hRil, dwFacility, lpszPassword, dwLockingStatus);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::ChangeLockingPassword(DWORD dwFacility, DWORD dwOldPasswordType, LPCSTR lpszOldPassword, LPCSTR lpszNewPassword)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_ChangeLockingPassword(m_hRil, dwFacility, dwOldPasswordType, lpszOldPassword, lpszNewPassword);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}
		
		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetRegistrationStatus(DWORD& dwRegStatus)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_GetRegistrationStatus(m_hRil, NULL);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID] = pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			DWORD*	pdwRegStatus;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pdwRegStatus	= (DWORD*)pCmd->GetData(NULL);
				dwRegStatus		= *pdwRegStatus;
				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetRegistrationStatus2(DWORD& dwRegStatus)
{
	HRESULT	hresCmdID;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	BYTE	byBuffer[256];
	BYTE	byAsyncResults[256];
	DWORD	dwAsyncResults	= 0;
	DWORD	dwCode			= RIL_DEVSPECIFIC_GET_REGSTATUS;

	ZeroMemory(byBuffer, sizeof(byBuffer));
	ZeroMemory(byAsyncResults, sizeof(byAsyncResults));
	memcpy((void*)byBuffer, (void*)&dwCode, sizeof(DWORD));

	hresCmdID	= RIL_DevSpecific(m_hRil, (BYTE*)byBuffer, sizeof(byBuffer), (BYTE*)byAsyncResults, sizeof(byAsyncResults), &dwAsyncResults, 2500);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		memcpy((void*)&dwRegStatus, (void*)byAsyncResults, dwAsyncResults);

		nFunctionResult	= RILAPI_SUCCESS;
	}
	
	return nFunctionResult;
}

int CRilTools::GetCurrentOperator(RILOPERATORNAMES& RilOperatorNames, DWORD dwFormat)
{
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	ZeroMemory(&RilOperatorNames, sizeof(RILOPERATORNAMES));

	hresCmdID	= RIL_GetCurrentOperator(m_hRil, dwFormat);

	if (IS_ERROR(hresCmdID))
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID]	= pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			RILOPERATORNAMES*	pRilOperatorNames;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pRilOperatorNames	= (RILOPERATORNAMES*)pCmd->GetData(NULL);
				nFunctionResult		= RILAPI_SUCCESS;

				memcpy(&RilOperatorNames, pRilOperatorNames, sizeof(RILOPERATORNAMES));
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::SendATCommand(LPCSTR szATCommand, LPCSTR szResponse, DWORD dwResponseLen)
{
	HRESULT	hresCmdID;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	BYTE	byBuffer[256];
	BYTE	byAsyncResults[256];
	DWORD	dwAsyncResults	= 0;
	DWORD	dwCode			= RIL_DEVSPECIFIC_SEND_ATC;
	DWORD	dwATCmdLen		= strlen(szATCommand);

	if (dwATCmdLen > (sizeof(byBuffer) - sizeof(DWORD)))
	{
		return nFunctionResult;
	}

	ZeroMemory(byBuffer, sizeof(byBuffer));
	ZeroMemory(byAsyncResults, sizeof(byAsyncResults));
	memcpy((void*)byBuffer, (void*)&dwCode, sizeof(DWORD));
	memcpy((void*)(byBuffer + sizeof(DWORD)), (void*)szATCommand, strlen(szATCommand));

	hresCmdID	= RIL_DevSpecific(m_hRil, (BYTE*)byBuffer, sizeof(byBuffer), (BYTE*)byAsyncResults, sizeof(byAsyncResults), &dwAsyncResults, 2500);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		if (dwResponseLen > dwAsyncResults)
		{
			memcpy((void*)szResponse, (void*)byAsyncResults, dwAsyncResults);
		}
		else
		{
			memcpy((void*)szResponse, (void*)byAsyncResults, dwResponseLen);
		}

		nFunctionResult	= RILAPI_SUCCESS;
	}
	
	return nFunctionResult;
}

int CRilTools::SendRestrictedSimCmd(DWORD dwCommand, RILSIMCMDPARAMETERS simcmd, BYTE* szResponse, int &nResponselen)
{ 
	HRESULT	hresCmdID;
	CCmd*	pCmd;
	int		nFunctionResult	= RILAPI_CRITICAL_ERROR;

	hresCmdID	= RIL_SendRestrictedSimCmd(m_hRil, dwCommand, &simcmd, NULL, 0);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		pCmd					= new CCmd;
		m_mapCommand[hresCmdID] = pCmd;

		if (::WaitForSingleObject(pCmd->GetResponseEvent(), 2500) == WAIT_OBJECT_0)
		{
			RILSIMRESPONSE*	pSimResponse	= NULL;

			if (pCmd->GetResult() == RIL_RESULT_OK) 
			{
				pSimResponse	= (RILSIMRESPONSE*)pCmd->GetData(NULL);

				int	nDataLen	= pSimResponse->cbSize - (4 * sizeof(DWORD));

				if (szResponse)
				{
					memcpy(szResponse, pSimResponse->pbResponse, (nDataLen < nResponselen) ? nDataLen : nResponselen);

					nResponselen	= nDataLen;
				}

				nFunctionResult	= RILAPI_SUCCESS;
			}
			else
			{
				nFunctionResult	= RILAPI_FAILED;
			}
		}
		else
		{
			nFunctionResult	= RILAPI_CRITICAL_ERROR;
		}

		m_mapCommand.erase(hresCmdID);

		delete pCmd;
	}

	return nFunctionResult;
}

int CRilTools::GetICCID(LPCSTR szICCID)
{
	HRESULT	hresCmdID;
	int		nFunctionResult		= RILAPI_CRITICAL_ERROR;
	BYTE	byBuffer[256];
	BYTE	byAsyncResults[256];
	DWORD	dwAsyncResults		= 0;
	DWORD	dwCode				= RIL_DEVSPECIFIC_GET_ICCID;
	
	ZeroMemory(byBuffer, sizeof(byBuffer));
	ZeroMemory(byAsyncResults, sizeof(byAsyncResults));
	memcpy((void*)byBuffer, (void*)&dwCode, sizeof(DWORD));

	hresCmdID	= RIL_DevSpecific(m_hRil, (BYTE*)byBuffer, sizeof(byBuffer), (BYTE*)byAsyncResults, sizeof(byAsyncResults), &dwAsyncResults, 2500);

	if (IS_ERROR(hresCmdID)) 
	{
		nFunctionResult	= RILAPI_CRITICAL_ERROR;
	}
	else
	{
		memcpy((void*)szICCID, (void*)byAsyncResults, dwAsyncResults);

		nFunctionResult	= RILAPI_SUCCESS;
	}

	return nFunctionResult;
}
