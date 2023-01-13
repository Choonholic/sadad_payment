// Cmd.cpp

#include "StdAfx.h"
#include "RilApiEx.h"
#include "Cmd.h"

CCmd::CCmd() : m_dwResultCode(0), m_pData(NULL), m_dwDataSize(0), m_hRespondEvent(NULL)
{
	m_hRespondEvent	= ::CreateEvent(NULL, FALSE, FALSE, NULL);
}
	
CCmd::~CCmd()
{
	delete [] m_pData;

	CloseHandle(m_hRespondEvent);
}
	
void CCmd::SetResult(DWORD dwResultCode)
{
	m_dwResultCode	= dwResultCode;
}
	
DWORD CCmd::GetResult(void) const
{
	return m_dwResultCode;
}
	
void CCmd::SetBlob(const void* pData, DWORD dwDataSize)
{
	m_dwDataSize	= dwDataSize;
	m_pData			= new char[m_dwDataSize];

	memcpy(m_pData, pData, m_dwDataSize);
}
	
HANDLE CCmd::GetResponseEvent() const
{
	return m_hRespondEvent;
}
	
void* CCmd::GetData(DWORD* pDataSize) const
{
	if (pDataSize != NULL)
	{
		*pDataSize	= m_dwDataSize;
	}

	return m_pData;
}	
