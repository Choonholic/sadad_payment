// LogEx.h

#pragma once

#define MAX_LOG_LINES	2000
#define BUFFER_SIZE		512

#define LOG_RETAILMSG	1
#define	FORMAT_LOG		L"\\SADADPayLog_%04d%02d%02d_%02d%02d%02d.log"
#define PATH_LOG_FIND	L"\\SADADPayLog_*.log"
#define MAX_LOG_SPACE	(102400L)

typedef struct log_list_t
{
	WCHAR			szFileName[MAX_PATH + 1];
	ULARGE_INTEGER	uiFileSize;
} LOGEX_ITEM;

class CLogEx
{
public:
	CLogEx(BOOL bSave = TRUE);
	~CLogEx();

	LPCTSTR	GetFileName();

	BOOL	Write(const wchar_t* szFormat, ...);

	BOOL	StartMultiWrite();
	BOOL	MultiWrite(const wchar_t* szFormat, ...);
	BOOL	EndMultiWrite();

protected:
	BOOL	Open();
	BOOL	Close();
	BOOL	WriteBuffer();
	
	void	MakeLogFileName();
	void	CheckLogList();

	DWORD	m_dwLines;
	BOOL	m_bSave;
	WCHAR	m_szFileName[MAX_PATH + 1];
	WCHAR	m_szBuffer[BUFFER_SIZE];
	HANDLE	m_hFile;
};
