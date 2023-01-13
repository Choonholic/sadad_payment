// TransactionDatabase.h
// Based on the code by piwpiw.

#pragma once

#include "DBAssist.h"
#include "Definitions.h"

#define SQL

#define PATH_DB_TRANS			L"\\UPS\\Payment\\Data\\trans.db"
#define PATH_DB_CHARGE			L"\\UPS\\Payment\\Data\\charge.db"

#define DB_TABLE_NAME_TRANS		L"TRANS"
#define DB_TABLE_NAME_CHARGE	L"CHARGE"

#define DB_MAX_RECORD_TRANS		10000
#define	DB_MAX_RECORD_CHARGE	1000

#define DB_MODE_TRANS			0
#define DB_MODE_CHARGE			1

class CTransactionDatabase
{
public:
	CTransactionDatabase();
	~CTransactionDatabase();

	void	Initialize();
	void	Close();

	void	CreateDB_TRANS();
	void	CreateDB_CHARGE();

	void	CreateIndexFromColumn(CDBAssist* pAssist, LPCWSTR szTable, LPCWSTR szColumn, LPCWSTR szIndex);

	void	UpgradeDB_TRANS();
	void	UpgradeDB_CHARGE();

	int		GetTransactionCount();
	int		GetChargeCount();

	BOOL	TrimTransactionDatabase();

	BOOL	InsertTransactionRecord(TRANSACTION_INFO* pInfo);
	BOOL	InsertChargeRecord(CHARGE_INFO* pInfo);

	BOOL	SelectLastTransactionRecord(TRANSACTION_INFO* pInfo);

	UINT	SelectTransactionRecords(WCHAR* szTraceNo);
	UINT	SelectTransactionRecords(WCHAR* szStartDate, WCHAR* szEndDate);

	UINT	SelectAllTransactionRecords();

	UINT	SelectReversalRecords();
	UINT	SelectAdviceRecords();

	BOOL	SelectChargeRecord(WCHAR* szProviderID, WCHAR* szCategoryID, CHARGE_INFO* pInfo);

	BOOL	UpdateTransactionRecord(TRANSACTION_INFO* pInfo);
	BOOL	UpdateTransactionStatus(TRANSACTION_INFO* pInfo);
	BOOL	UpdateTransactionAttempts(TRANSACTION_INFO* pInfo);

	BOOL	UpdateChargeRecord(CHARGE_INFO* pInfo);

	BOOL	DeleteTransactionRecord(TRANSACTION_INFO* pInfo);
	BOOL	DeleteChargeRecord(CHARGE_INFO* pInfo);

	CArray<TRANSACTION_INFO>	m_arrTransaction;
	CArray<TRANSACTION_INFO>	m_arrReversal;
	CArray<TRANSACTION_INFO>	m_arrAdvice;

	CDBAssist	m_DB_TRANS;
	CDBAssist	m_DB_CHARGE;
};
