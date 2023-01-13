// DBAssist.h

#pragma once

#include "sqlite3.h"

typedef sqlite3_stmt*	PSTMT;

#define LENGTH_SQL		4096

class CDBAssist
{
public:
    CDBAssist();
    ~CDBAssist();

    int		Open(LPCWSTR szFileName);
    int		Close();
	int		Reopen();

	void	DebugErrorMessage();

    int		ExecuteNonQuery(LPCWSTR szSQL);
    int		ExecuteIntQuery(LPCWSTR szSQL, int* pResult);

	int		Prepare(LPCWSTR szSQL);
    int		Step();
    int		Finalize();

	PSTMT	GetSTMT();

protected:
	WCHAR		m_szFileName[MAX_PATH + 1];
    sqlite3*	m_DB;
	PSTMT		m_pSTMT;
	char		m_cSQLBuffer[LENGTH_SQL];
};
