// LogEx.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "LogEx.h"

CLogEx::CLogEx(BOOL bSave)
{
	m_dwLines	= 0;
	m_bSave		= bSave;
	m_hFile		= INVALID_HANDLE_VALUE;

	MakeLogFileName();
}

CLogEx::~CLogEx()
{
	Close();
}

LPCTSTR CLogEx::GetFileName()
{
	return m_szFileName;
}

BOOL CLogEx::Write(const wchar_t* szFormat, ...)
{
	va_list		args;
	WCHAR		szBuffer[BUFFER_SIZE];
	SYSTEMTIME	st;

	if (!m_bSave)
	{
		return TRUE;
	}

	CheckLogList();

	if (!Open())
	{
		return FALSE; 
	}

	va_start(args, szFormat);
	vswprintf(szBuffer, szFormat, args);
	va_end(args);

	GetLocalTime(&st);
	swprintf(m_szBuffer, L"%02d/%02d %02d:%02d:%02d - %s", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, szBuffer);

	RETAILMSG(LOG_RETAILMSG, (m_szBuffer));

	WriteBuffer();
	
	return Close();
}

BOOL CLogEx::StartMultiWrite()
{
	SYSTEMTIME	st;

	if (!m_bSave)
	{
		return TRUE;
	}

	CheckLogList();

	if (!Open())
	{
		return FALSE;
	}

	GetLocalTime(&st);
	swprintf(m_szBuffer, L"%02d/%02d/%04d %02d:%02d:%02d -\n", st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

	return WriteBuffer();
}

BOOL CLogEx::MultiWrite(const wchar_t* szFormat, ...)
{
	if (!m_bSave)
	{
		return TRUE;
	}

	va_list	args;

	va_start(args, szFormat);
	vswprintf(m_szBuffer, szFormat, args);
	va_end(args);

	RETAILMSG(LOG_RETAILMSG, (m_szBuffer));

	return WriteBuffer();
}

BOOL CLogEx::EndMultiWrite()
{
	if (!m_bSave)
	{
		return TRUE;
	}

	return Close();
}

BOOL CLogEx::Open()
{
	BYTE	cBOM[2]		= { 0xFF, 0xFE };
	DWORD	dwWritten;

	if (!m_bSave)
	{
		return TRUE;
	}

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);

		m_hFile	= INVALID_HANDLE_VALUE;
	}

	if (m_dwLines >= MAX_LOG_LINES)
	{
		MakeLogFileName();

		m_dwLines	= 0;
	}

	m_hFile	= CreateFile(m_szFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (m_dwLines == 0)
	{
		WriteFile(m_hFile, cBOM, 2 * sizeof(BYTE), &dwWritten, NULL);
	}

	return TRUE;
}

BOOL CLogEx::Close()
{
	if (!m_bSave)
	{
		return TRUE;
	}

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);

		m_hFile	= INVALID_HANDLE_VALUE;
	}

	return TRUE;
}

BOOL CLogEx::WriteBuffer()
{
	DWORD	dwWritten;

	if (!m_bSave)
	{
		return TRUE;
	}

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	SetFilePointer(m_hFile, 0, NULL, FILE_END);
	WriteFile(m_hFile, m_szBuffer, wcslen(m_szBuffer) * sizeof(WCHAR), &dwWritten, NULL);
	++m_dwLines;

	return TRUE;
}

void CLogEx::MakeLogFileName()
{
	SYSTEMTIME	st;

	GetLocalTime(&st);
	swprintf(m_szFileName, FORMAT_LOG, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void CLogEx::CheckLogList()
{
	CArray<LOGEX_ITEM, LOGEX_ITEM>	arrLog;
	HANDLE							hFind;
	WIN32_FIND_DATA					fd;
	LOGEX_ITEM						logItem;
	ULARGE_INTEGER					uiTotal;

	if (!m_bSave)
	{
		return;
	}

	uiTotal.QuadPart	= 0;
	hFind				= FindFirstFile(PATH_LOG_FIND, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			wcscpy(logItem.szFileName, fd.cFileName);

			logItem.uiFileSize.LowPart	= fd.nFileSizeLow;
			logItem.uiFileSize.HighPart	= fd.nFileSizeHigh;
			uiTotal.QuadPart			+= logItem.uiFileSize.QuadPart;

			arrLog.Add(logItem);
		} while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}

	while (uiTotal.QuadPart > MAX_LOG_SPACE)
	{
		uiTotal.QuadPart	-= arrLog.GetAt(0).uiFileSize.QuadPart;

		DeleteFile(arrLog.GetAt(0).szFileName);
		arrLog.RemoveAt(0);
	} 
}
