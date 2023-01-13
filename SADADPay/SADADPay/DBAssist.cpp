// DBAssist.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "DBAssist.h"
#include "Utilities.h"

CDBAssist::CDBAssist()
{
}

CDBAssist::~CDBAssist()
{
}

static int callback(void* pNotUsed, int argc, char** argv, char** azColName)
{
	return 0;
}

int CDBAssist::Open(LPCWSTR szFileName)
{
	int	rc;

	WideCharToMultiByte(CP_UTF8, 0, szFileName, -1, m_cSQLBuffer, LENGTH_SQL, NULL, NULL);

	if ((rc = sqlite3_open(m_cSQLBuffer, &m_DB)) != SQLITE_OK)
	{
		DebugErrorMessage();
 		sqlite3_close(m_DB);
	}
	else
	{
		wcscpy(m_szFileName, szFileName);
	}

	return rc;
}

int CDBAssist::Close()
{
	return sqlite3_close(m_DB);
}

int CDBAssist::Reopen()
{
	Close();
	Sleep(200);

	return Open(m_szFileName);
}

void CDBAssist::DebugErrorMessage()
{
	RETAILMSG(DBG_DB, (L"[SQLite] %s\r\n", sqlite3_errmsg16(m_DB)));
}

int CDBAssist::ExecuteNonQuery(LPCWSTR szSQL)
{
    char*	pErrMsg;
	int		rc;

	WideCharToMultiByte(CP_UTF8, 0, szSQL, -1, m_cSQLBuffer, LENGTH_SQL, NULL, NULL);

	if ((rc = sqlite3_exec(m_DB, m_cSQLBuffer, callback, 0, &pErrMsg)) != SQLITE_OK)
	{
		DebugErrorMessage();
	}
	    
	sqlite3_free(pErrMsg);
	return rc;
}

int CDBAssist::ExecuteIntQuery(LPCWSTR szSQL, int* pResult)
{
	int	rc;

	if ((rc = Prepare(szSQL)) != SQLITE_OK)
	{
		DebugErrorMessage();
		return rc;
	}

	while (Step() == SQLITE_ROW)
	{
		*pResult	= sqlite3_column_int(GetSTMT(), 0);
	}

	return Finalize();
}

int CDBAssist::Prepare(LPCWSTR szSQL)
{
	WideCharToMultiByte(CP_UTF8, 0, szSQL, -1, m_cSQLBuffer, LENGTH_SQL, NULL, NULL);

	return sqlite3_prepare_v2(m_DB, m_cSQLBuffer, strlen(m_cSQLBuffer), &m_pSTMT, NULL);
}

int CDBAssist::Step()
{
	return sqlite3_step(m_pSTMT);
}

int CDBAssist::Finalize()
{
	return sqlite3_finalize(m_pSTMT);
}

PSTMT CDBAssist::GetSTMT()
{
	return m_pSTMT;
}
