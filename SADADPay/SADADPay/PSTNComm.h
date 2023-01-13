// PSTNComm.h

#pragma once

#include "Definitions.h"

class CPSTNComm
{
public:
	CPSTNComm();
	~CPSTNComm();

	BOOL	Open();
	BOOL	Close();

	CONNECTION_ERROR	DialUp(char* szNumber);
	CONNECTION_STATUS	CheckCommStatus();
	BOOL				HangUp();

	BOOL	Send(UCHAR* pData, UINT nSize, UINT uTimeout);
	BOOL	Receive(UCHAR* pData, UINT* pBufferSize, UINT uTimeout);

private:
	BOOL	m_bConnected;
};
