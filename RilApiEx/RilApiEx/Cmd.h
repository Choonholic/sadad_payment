// Cmd.h

#pragma once

class CCmd
{
public:
	CCmd();
	~CCmd();

	void	SetResult(DWORD dwResultCode);
	DWORD	GetResult() const;

	void	SetBlob(const void* pData, DWORD dwDataSize);

	HANDLE	GetResponseEvent() const;

	void*	GetData(DWORD* pDataSize) const;

protected:
	DWORD	m_dwResultCode;
	void*	m_pData;
	DWORD	m_dwDataSize;
	HANDLE	m_hRespondEvent;
};
