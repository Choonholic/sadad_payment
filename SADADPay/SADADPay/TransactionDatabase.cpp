// TransactionDatabase.cpp
// Based on the code by piwpiw.

#include "stdafx.h"
#include "SADADPay.h"
#include "TransactionDatabase.h"
#include "Utilities.h"

CTransactionDatabase::CTransactionDatabase()
{
}

CTransactionDatabase::~CTransactionDatabase()
{
}

void CTransactionDatabase::Initialize()
{
	CreateDB_TRANS();
	CreateDB_CHARGE();
}

void CTransactionDatabase::Close()
{
	m_DB_TRANS.Close();
	m_DB_CHARGE.Close();
}

void CTransactionDatabase::CreateDB_TRANS()
{
	int		rc			= m_DB_TRANS.Open(PATH_DB_TRANS);
	int		nVersion	= 0;
	CString	strSchema;

	if (rc == SQLITE_OK)
	{
		m_DB_TRANS.ExecuteIntQuery(L"PRAGMA user_version;", &nVersion);

		if (nVersion != VERSION_DATABASE)
		{
			strSchema.Format(L"DROP TABLE IF EXISTS %s;", DB_TABLE_NAME_TRANS);
			m_DB_TRANS.ExecuteNonQuery(strSchema);
		}

		strSchema.Format(L"PRAGMA user_version=%d;", VERSION_DATABASE);
		m_DB_TRANS.ExecuteNonQuery(strSchema);

		m_DB_TRANS.ExecuteNonQuery(L"PRAGMA encoding=\"UTF-8\";");

		strSchema.Format(L"CREATE TABLE IF NOT EXISTS %s (", DB_TABLE_NAME_TRANS);
		strSchema.Append(L"_id VARCHAR(17),");
		strSchema.Append(L"attempts INTEGER,");
		strSchema.Append(L"transactionStatus INTEGER,");
		strSchema.Append(L"transactionMode INTEGER,");
		strSchema.Append(L"processingCode VARCHAR(7),");
		strSchema.Append(L"amount VARCHAR(13),");
		strSchema.Append(L"traceNo VARCHAR(7),");
		strSchema.Append(L"timestamp VARCHAR(15),");
		strSchema.Append(L"track2Data VARCHAR(41),");
		strSchema.Append(L"referenceNo VARCHAR(13),");
		strSchema.Append(L"approvalCode VARCHAR(7),");
		strSchema.Append(L"responseCode VARCHAR(3),");
		strSchema.Append(L"providerId VARCHAR(5),");
		strSchema.Append(L"categoryId VARCHAR(3),");
		strSchema.Append(L"additionalData VARCHAR(26),");
		strSchema.Append(L"chargeCount INTEGER,");
		strSchema.Append(L"serial0 VARCHAR(21),");
		strSchema.Append(L"serial1 VARCHAR(21),");
		strSchema.Append(L"serial2 VARCHAR(21),");
		strSchema.Append(L"serial3 VARCHAR(21),");
		strSchema.Append(L"serial4 VARCHAR(21),");
		strSchema.Append(L"chargePin0 VARCHAR(33),");
		strSchema.Append(L"chargePin1 VARCHAR(33),");
		strSchema.Append(L"chargePin2 VARCHAR(33),");
		strSchema.Append(L"chargePin3 VARCHAR(33),");
		strSchema.Append(L"chargePin4 VARCHAR(33),");
		strSchema.Append(L"billId VARCHAR(14),");
		strSchema.Append(L"paymentId VARCHAR(14),");
		strSchema.Append(L"serviceCode INTEGER,");
		strSchema.Append(L"serviceType VARCHAR(3)");
		strSchema.Append(L");");

		rc	= m_DB_TRANS.ExecuteNonQuery(strSchema);

		CreateIndexFromColumn(&m_DB_TRANS, DB_TABLE_NAME_TRANS, L"_id ASC", L"idx");
		CreateIndexFromColumn(&m_DB_TRANS, DB_TABLE_NAME_TRANS, L"timestamp", L"idxTimestamp");
		CreateIndexFromColumn(&m_DB_TRANS, DB_TABLE_NAME_TRANS, L"traceNo", L"idxTraceNo");
	}
}

void CTransactionDatabase::CreateDB_CHARGE(void)
{
	int		rc			= m_DB_CHARGE.Open(PATH_DB_CHARGE);
	int		nVersion	= 0;
	CString	strSchema;

	if (rc == SQLITE_OK)
	{
		m_DB_CHARGE.ExecuteIntQuery(L"PRAGMA user_version;", &nVersion);

		if (nVersion != VERSION_DATABASE)
		{
			UpgradeDB_CHARGE();
		}

		strSchema.Format(L"PRAGMA user_version=%d;", VERSION_DATABASE);
		m_DB_CHARGE.ExecuteNonQuery(strSchema);

		m_DB_CHARGE.ExecuteNonQuery(L"PRAGMA encoding=\"UTF-8\";");

		strSchema.Format(L"CREATE TABLE IF NOT EXISTS %s (", DB_TABLE_NAME_CHARGE);
		strSchema.Append(L"_id VARCHAR(17),");
		strSchema.Append(L"timestamp VARCHAR(15),");
		strSchema.Append(L"traceNo VARCHAR(7),");
		strSchema.Append(L"providerId VARCHAR(5),");
		strSchema.Append(L"categoryId VARCHAR(3),");
		strSchema.Append(L"amount VARCHAR(13),");
		strSchema.Append(L"serial VARCHAR(21),");
		strSchema.Append(L"chargePin VARCHAR(33),");
		strSchema.Append(L"referenceNo VARCHAR(13),");
		strSchema.Append(L"approvalCode VARCHAR(7));");

		rc	= m_DB_CHARGE.ExecuteNonQuery(strSchema);

		CreateIndexFromColumn(&m_DB_CHARGE, DB_TABLE_NAME_CHARGE, L"_id ASC", L"idx");
		CreateIndexFromColumn(&m_DB_CHARGE, DB_TABLE_NAME_CHARGE, L"timestamp", L"idxTimestamp");
		CreateIndexFromColumn(&m_DB_CHARGE, DB_TABLE_NAME_CHARGE, L"traceNo", L"idxTraceNo");
		CreateIndexFromColumn(&m_DB_CHARGE, DB_TABLE_NAME_CHARGE, L"providerId", L"idxProviderId");
		CreateIndexFromColumn(&m_DB_CHARGE, DB_TABLE_NAME_CHARGE, L"categoryId", L"idxCategoryId");
	}
}

void CTransactionDatabase::CreateIndexFromColumn(CDBAssist* pAssist, LPCWSTR szTable, LPCWSTR szColumn, LPCWSTR szIndex)
{
	CString	strSQL;

	strSQL.Format(L"CREATE INDEX IF NOT EXISTS %s ON %s (%s);", szIndex, szTable, szColumn);
	pAssist->ExecuteNonQuery(strSQL);
}

void CTransactionDatabase::UpgradeDB_TRANS(void)
{
	CString	strSQL;

	strSQL.Format(L"DROP TABLE IF EXISTS %s;", DB_TABLE_NAME_TRANS);
	m_DB_TRANS.ExecuteNonQuery(strSQL);
}

void CTransactionDatabase::UpgradeDB_CHARGE(void)
{
	CString	strSQL;

	strSQL.Format(L"DROP TABLE IF EXISTS %s;", DB_TABLE_NAME_CHARGE);
	m_DB_CHARGE.ExecuteNonQuery(strSQL);
}

int CTransactionDatabase::GetTransactionCount()
{
	CString	strSQL;
	int		nCount	= 0;

	strSQL.Format(L"SELECT COUNT(*) FROM %s;", DB_TABLE_NAME_TRANS);
	m_DB_TRANS.ExecuteIntQuery(strSQL, &nCount);
	return nCount;
}

int CTransactionDatabase::GetChargeCount()
{
	CString	strSQL;
	int		nCount	= 0;

	strSQL.Format(L"SELECT COUNT(*) FROM %s;", DB_TABLE_NAME_CHARGE);
	m_DB_CHARGE.ExecuteIntQuery(strSQL, &nCount);
	return nCount;
}

BOOL CTransactionDatabase::TrimTransactionDatabase()
{
	CString	strSQL;
	int		rc;
	int		nCount	= GetTransactionCount();

	if (nCount > (DB_MAX_RECORD_TRANS - 1))
	{
		strSQL.Format(L"DELETE FROM %s WHERE timestamp IN (SELECT timestamp FROM %s ORDER BY timestamp ASC LIMIT %d);", DB_TABLE_NAME_TRANS, DB_TABLE_NAME_TRANS, nCount - (DB_MAX_RECORD_TRANS - 1));

		rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);
	}

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::InsertTransactionRecord(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	CString	strColumns;
	CString	strValues;
	int		rc		= 0;

	TrimTransactionDatabase();

	strSQL.Format(L"INSERT INTO %s ", DB_TABLE_NAME_TRANS);

	strColumns = L"(_id, attempts, transactionStatus, transactionMode, processingCode, amount, traceNo, timestamp, track2Data, referenceNo, approvalCode, responseCode, providerId, categoryId, additionalData, chargeCount, serial0, serial1, serial2, serial3, serial4, chargePin0, chargePin1, chargePin2, chargePin3, chargePin4, billId, paymentId, serviceCode, serviceType) ";

	strValues.Format(L"VALUES ('%s','%d','%d','%d','%s','%s','%s','%04d%02d%02d%02d%02d%02d','%s','%s','%s','%s','%s','%s','%s','%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s');",
		pInfo->szID,
		pInfo->nAttempts,
		pInfo->transactionStatus,
		pInfo->transactionMode,
		pInfo->szProcessingCode,
		pInfo->szAmount,
		pInfo->szTraceNo,
		pInfo->stTimestamp.wYear,
		pInfo->stTimestamp.wMonth,
		pInfo->stTimestamp.wDay,
		pInfo->stTimestamp.wHour,
		pInfo->stTimestamp.wMinute,
		pInfo->stTimestamp.wSecond,
		pInfo->szTrack2Data,
		pInfo->szReferenceNo,
		pInfo->szAuthID,
		pInfo->szResponseCode,
		pInfo->szProviderID,
		pInfo->szCategoryID,
		pInfo->szAdditionalData,
		pInfo->nChargeCount,
		pInfo->szSerial[0],
		pInfo->szSerial[1],
		pInfo->szSerial[2],
		pInfo->szSerial[3],
		pInfo->szSerial[4],
		pInfo->szChargePIN[0],
		pInfo->szChargePIN[1],
		pInfo->szChargePIN[2],
		pInfo->szChargePIN[3],
		pInfo->szChargePIN[4],
		pInfo->szBillID,
		pInfo->szPaymentID,
		pInfo->nServiceCode,
		pInfo->szServiceType);

	strSQL.Append(strColumns);
	strSQL.Append(strValues);

	rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::InsertChargeRecord(CHARGE_INFO* pInfo)
{
	CString	strSQL;
	CString	strColumns;
	CString	strValue;
	int		rc		= 0;

	if (GetChargeCount() >= DB_MAX_RECORD_CHARGE)
	{
		return FALSE;
	}

	strSQL.Format(L"INSERT INTO %s ", DB_TABLE_NAME_CHARGE);

	strColumns	= L"(_id,timestamp,traceNo,providerId,categoryId,amount,serial,chargePin,referenceNo,approvalCode) ";

	strValue.Format(L"VALUES ('%s','%04d%02d%02d%02d%02d%02d','%s','%s','%s','%s','%s','%s','%s','%s');",
		pInfo->szID,
		pInfo->stTimestamp.wYear,
		pInfo->stTimestamp.wMonth,
		pInfo->stTimestamp.wDay,
		pInfo->stTimestamp.wHour,
		pInfo->stTimestamp.wMinute,
		pInfo->stTimestamp.wSecond,
		pInfo->szTraceNo,
		pInfo->szProviderID,
		pInfo->szCategoryID,
		pInfo->szAmount,
		pInfo->szSerial,
		pInfo->szChargePIN,
		pInfo->szReferenceNo,
		pInfo->szAuthID);

	strSQL.Append(strColumns);
	strSQL.Append(strValue);

	rc	= m_DB_CHARGE.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::SelectLastTransactionRecord(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	BOOL	bFound		= FALSE;

	if (GetTransactionCount() < 1)
	{
		return FALSE;
	}

	strSQL.Format(L"SELECT * FROM %s WHERE (transactionStatus='%d' OR transactionStatus='%d' OR transactionStatus='%d') ORDER BY timestamp DESC LIMIT 1;", DB_TABLE_NAME_TRANS, STATUS_COMPLETED, STATUS_ADVICE, STATUS_BALANCE_RESPONSED);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		if (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			bFound	= TRUE;

			memset(pInfo, 0, sizeof(TRANSACTION_INFO));

			BA2WA(pInfo->szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			pInfo->nAttempts			= sqlite3_column_int(pSTMT, 1);
			pInfo->transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			pInfo->transactionMode		= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(pInfo->szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(pInfo->szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(pInfo->szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);
			
			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			pInfo->stTimestamp.wYear	= BA2WD((const BYTE*)&cDate[0], 4);
			pInfo->stTimestamp.wMonth	= BA2WD((const BYTE*)&cDate[4], 2);
			pInfo->stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			pInfo->stTimestamp.wHour	= BA2WD((const BYTE*)&cDate[8], 2);
			pInfo->stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			pInfo->stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(pInfo->szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(pInfo->szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(pInfo->szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(pInfo->szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(pInfo->szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(pInfo->szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);	
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, pInfo->szAdditionalData, 26);

			pInfo->nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(pInfo->szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(pInfo->szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(pInfo->szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(pInfo->szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(pInfo->szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(pInfo->szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(pInfo->szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(pInfo->szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(pInfo->szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(pInfo->szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(pInfo->szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(pInfo->szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			pInfo->nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(pInfo->szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);
		}

		m_DB_TRANS.Finalize();
    }

	return bFound;
}

UINT CTransactionDatabase::SelectTransactionRecords(WCHAR* szTraceNo)
{
	CString				strSQL;
	TRANSACTION_INFO	info;

	strSQL.Format(L"SELECT * FROM %s WHERE (traceNo='%s') AND (transactionStatus='%d' OR transactionStatus='%d' OR transactionStatus='%d');", DB_TABLE_NAME_TRANS, szTraceNo, STATUS_COMPLETED, STATUS_ADVICE, STATUS_BALANCE_RESPONSED);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		m_arrTransaction.RemoveAll();

		while (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			memset(&info, 0, sizeof(TRANSACTION_INFO));

			BA2WA(info.szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			info.nAttempts			= sqlite3_column_int(pSTMT, 1);
			info.transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			info.transactionMode	= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(info.szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(info.szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(info.szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);

			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			info.stTimestamp.wYear		= BA2WD((const BYTE*)&cDate[0], 4);
			info.stTimestamp.wMonth		= BA2WD((const BYTE*)&cDate[4], 2);
			info.stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			info.stTimestamp.wHour		= BA2WD((const BYTE*)&cDate[8], 2);
			info.stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			info.stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(info.szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(info.szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(info.szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(info.szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(info.szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(info.szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, info.szAdditionalData, 26);

			info.nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(info.szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(info.szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(info.szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(info.szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(info.szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(info.szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(info.szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(info.szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(info.szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(info.szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(info.szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(info.szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			info.nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(info.szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);

			m_arrTransaction.Add(info);
		}

		m_DB_TRANS.Finalize();
    }

	return m_arrTransaction.GetCount();
}

UINT CTransactionDatabase::SelectTransactionRecords(WCHAR* szStartDate, WCHAR* szEndDate)
{
	CString				strSQL;
	TRANSACTION_INFO	info;

	strSQL.Format(L"SELECT * FROM %s WHERE (timestamp BETWEEN '%s' AND '%s') AND (transactionStatus='%d' OR transactionStatus='%d');", DB_TABLE_NAME_TRANS, szStartDate, szEndDate, STATUS_ADVICE, STATUS_COMPLETED);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		m_arrTransaction.RemoveAll();

		while (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			memset(&info, 0, sizeof(TRANSACTION_INFO));

			BA2WA(info.szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			info.nAttempts			= sqlite3_column_int(pSTMT, 1);
			info.transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			info.transactionMode	= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(info.szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(info.szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(info.szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);
			
			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			info.stTimestamp.wYear		= BA2WD((const BYTE*)&cDate[0], 4);
			info.stTimestamp.wMonth		= BA2WD((const BYTE*)&cDate[4], 2);
			info.stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			info.stTimestamp.wHour		= BA2WD((const BYTE*)&cDate[8], 2);
			info.stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			info.stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(info.szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(info.szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(info.szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(info.szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(info.szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(info.szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, info.szAdditionalData, 26);

			info.nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(info.szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(info.szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(info.szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(info.szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(info.szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(info.szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(info.szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(info.szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(info.szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(info.szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(info.szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(info.szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			info.nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(info.szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);

			m_arrTransaction.Add(info);
		}

		m_DB_TRANS.Finalize();
    }

	return m_arrTransaction.GetCount();
}

UINT CTransactionDatabase::SelectAllTransactionRecords()
{
	CString				strSQL;
	TRANSACTION_INFO	info;

	strSQL.Format(L"SELECT * FROM %s;", DB_TABLE_NAME_TRANS);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		m_arrTransaction.RemoveAll();

		while (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			memset(&info, 0, sizeof(TRANSACTION_INFO));

			BA2WA(info.szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			info.nAttempts			= sqlite3_column_int(pSTMT, 1);
			info.transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			info.transactionMode	= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(info.szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(info.szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(info.szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);
			
			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			info.stTimestamp.wYear		= BA2WD((const BYTE*)&cDate[0], 4);
			info.stTimestamp.wMonth		= BA2WD((const BYTE*)&cDate[4], 2);
			info.stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			info.stTimestamp.wHour		= BA2WD((const BYTE*)&cDate[8], 2);
			info.stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			info.stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(info.szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(info.szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(info.szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(info.szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(info.szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(info.szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, info.szAdditionalData, 26);

			info.nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(info.szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(info.szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(info.szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(info.szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(info.szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(info.szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(info.szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(info.szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(info.szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(info.szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(info.szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(info.szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			info.nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(info.szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);

			m_arrTransaction.Add(info);
		}

		m_DB_TRANS.Finalize();
    }

	return m_arrTransaction.GetCount();
}

UINT CTransactionDatabase::SelectReversalRecords()
{
	CString				strSQL;
	TRANSACTION_INFO	info;

	strSQL.Format(L"SELECT * FROM %s WHERE transactionStatus='%d' OR transactionStatus='%d';", DB_TABLE_NAME_TRANS, STATUS_TRANSACTION, STATUS_REVERSAL);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		m_arrReversal.RemoveAll();

		while (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			memset(&info, 0, sizeof(TRANSACTION_INFO));

			BA2WA(info.szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			info.nAttempts			= sqlite3_column_int(pSTMT, 1);
			info.transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			info.transactionMode	= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(info.szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(info.szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(info.szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);
			
			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			info.stTimestamp.wYear		= BA2WD((const BYTE*)&cDate[0], 4);
			info.stTimestamp.wMonth		= BA2WD((const BYTE*)&cDate[4], 2);
			info.stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			info.stTimestamp.wHour		= BA2WD((const BYTE*)&cDate[8], 2);
			info.stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			info.stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(info.szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(info.szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(info.szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(info.szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(info.szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(info.szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, info.szAdditionalData, 26);

			info.nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(info.szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(info.szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(info.szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(info.szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(info.szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(info.szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(info.szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(info.szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(info.szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(info.szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(info.szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(info.szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			info.nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(info.szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);

			m_arrReversal.Add(info);
		}

		m_DB_TRANS.Finalize();
    }

	return m_arrReversal.GetCount();
}

UINT CTransactionDatabase::SelectAdviceRecords()
{
	CString				strSQL;
	TRANSACTION_INFO	info;

	strSQL.Format(L"SELECT * FROM %s WHERE transactionStatus='%d';", DB_TABLE_NAME_TRANS, STATUS_ADVICE);

	if (m_DB_TRANS.Prepare(strSQL) == SQLITE_OK)
	{
		m_arrAdvice.RemoveAll();

		while (m_DB_TRANS.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_TRANS.GetSTMT();
			char	cDate[15];

			memset(&info, 0, sizeof(TRANSACTION_INFO));

			BA2WA(info.szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			info.nAttempts			= sqlite3_column_int(pSTMT, 1);
			info.transactionStatus	= (TRANSACTION_STATUS)sqlite3_column_int(pSTMT, 2);
			info.transactionMode	= (TRANSACTION_MODE)sqlite3_column_int(pSTMT, 3);

			BA2WA(info.szProcessingCode, 7, sqlite3_column_text(pSTMT, 4), 6);
			BA2WA(info.szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(info.szTraceNo, 7, sqlite3_column_text(pSTMT, 6), 6);
			
			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 7), 14);

			info.stTimestamp.wYear		= BA2WD((const BYTE*)&cDate[0], 4);
			info.stTimestamp.wMonth		= BA2WD((const BYTE*)&cDate[4], 2);
			info.stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			info.stTimestamp.wHour		= BA2WD((const BYTE*)&cDate[8], 2);
			info.stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			info.stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(info.szTrack2Data, 41, sqlite3_column_text(pSTMT, 8), 40);
			BA2WA(info.szReferenceNo, 13, sqlite3_column_text(pSTMT, 9), 12);
			BA2WA(info.szAuthID, 7, sqlite3_column_text(pSTMT, 10), 6);
			BA2WA(info.szResponseCode, 3, sqlite3_column_text(pSTMT, 11), 2);
			BA2WA(info.szProviderID, 5, sqlite3_column_text(pSTMT, 12), 4);
			BA2WA(info.szCategoryID, 3, sqlite3_column_text(pSTMT, 13), 4);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)sqlite3_column_text(pSTMT, 14), 26, info.szAdditionalData, 26);

			info.nChargeCount	= sqlite3_column_int(pSTMT, 15);

			BA2WA(info.szSerial[0], 21, sqlite3_column_text(pSTMT, 16), 20);
			BA2WA(info.szSerial[1], 21, sqlite3_column_text(pSTMT, 17), 20);
			BA2WA(info.szSerial[2], 21, sqlite3_column_text(pSTMT, 18), 20);
			BA2WA(info.szSerial[3], 21, sqlite3_column_text(pSTMT, 19), 20);
			BA2WA(info.szSerial[4], 21, sqlite3_column_text(pSTMT, 20), 20);
			BA2WA(info.szChargePIN[0], 33, sqlite3_column_text(pSTMT, 21), 32);
			BA2WA(info.szChargePIN[1], 33, sqlite3_column_text(pSTMT, 22), 32);
			BA2WA(info.szChargePIN[2], 33, sqlite3_column_text(pSTMT, 23), 32);
			BA2WA(info.szChargePIN[3], 33, sqlite3_column_text(pSTMT, 24), 32);
			BA2WA(info.szChargePIN[4], 33, sqlite3_column_text(pSTMT, 25), 32);
			BA2WA(info.szBillID, 14, sqlite3_column_text(pSTMT, 26), 13);
			BA2WA(info.szPaymentID, 14, sqlite3_column_text(pSTMT, 27), 13);

			info.nServiceCode 	= sqlite3_column_int(pSTMT, 28);

			BA2WA(info.szServiceType, 3, sqlite3_column_text(pSTMT, 29), 2);

			m_arrAdvice.Add(info);
		}

		m_DB_TRANS.Finalize();
    }

	return m_arrAdvice.GetCount();
}

BOOL CTransactionDatabase::SelectChargeRecord(WCHAR* szProviderID, WCHAR* szCategoryID, CHARGE_INFO* pInfo)
{
	CString	strSQL;
	BOOL	bFound	= FALSE;

	strSQL.Format(L"SELECT * FROM %s WHERE providerId='%s' AND categoryId='%s';", DB_TABLE_NAME_CHARGE, szProviderID, szCategoryID);

	if (m_DB_CHARGE.Prepare(strSQL) == SQLITE_OK)
	{
		if (m_DB_CHARGE.Step() != SQLITE_DONE)
        {
			PSTMT	pSTMT		= m_DB_CHARGE.GetSTMT();
			char	cDate[15];

			bFound	= TRUE;

			memset(pInfo, 0, sizeof(CHARGE_INFO));

			BA2WA(pInfo->szID, 17, sqlite3_column_text(pSTMT, 0), 16);

			memset(cDate, 0, sizeof(char) * 15);
			memcpy(cDate, sqlite3_column_text(pSTMT, 1), 14);

			pInfo->stTimestamp.wYear	= BA2WD((const BYTE*)&cDate[0], 4);
			pInfo->stTimestamp.wMonth	= BA2WD((const BYTE*)&cDate[4], 2);
			pInfo->stTimestamp.wDay		= BA2WD((const BYTE*)&cDate[6], 2);
			pInfo->stTimestamp.wHour	= BA2WD((const BYTE*)&cDate[8], 2);
			pInfo->stTimestamp.wMinute	= BA2WD((const BYTE*)&cDate[10], 2);
			pInfo->stTimestamp.wSecond	= BA2WD((const BYTE*)&cDate[12], 2);

			BA2WA(pInfo->szTraceNo, 7, sqlite3_column_text(pSTMT, 2), 6);
			BA2WA(pInfo->szProviderID, 5, sqlite3_column_text(pSTMT, 3), 4);
			BA2WA(pInfo->szCategoryID, 3, sqlite3_column_text(pSTMT, 4), 2);
			BA2WA(pInfo->szAmount, 13, sqlite3_column_text(pSTMT, 5), 12);
			BA2WA(pInfo->szSerial, 21, sqlite3_column_text(pSTMT, 6), 20);
			BA2WA(pInfo->szChargePIN, 33, sqlite3_column_text(pSTMT, 7), 32);

			BA2WA(pInfo->szReferenceNo, 13, sqlite3_column_text(pSTMT, 8), 12);
			BA2WA(pInfo->szAuthID, 7, sqlite3_column_text(pSTMT, 9), 6);
		}

		m_DB_CHARGE.Finalize();
    }

	return bFound;
}

BOOL CTransactionDatabase::UpdateTransactionRecord(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	CString	strField;
	int		rc;

	strSQL.Format(L"UPDATE %s SET ", DB_TABLE_NAME_TRANS);
	strField.Format(L"attempts='%d',", pInfo->nAttempts);
	strSQL.Append(strField);
	strField.Format(L"transactionStatus='%d',", pInfo->transactionStatus);
	strSQL.Append(strField);
	strField.Format(L"transactionMode='%d',", pInfo->transactionMode);
	strSQL.Append(strField);
	strField.Format(L"processingCode='%s',", pInfo->szProcessingCode);
	strSQL.Append(strField);
	strField.Format(L"amount='%s',", pInfo->szAmount);
	strSQL.Append(strField);
	strField.Format(L"traceNo='%s',", pInfo->szTraceNo);
	strSQL.Append(strField);
	strField.Format(L"timestamp='%04d%02d%02d%02d%02d%02d',", pInfo->stTimestamp.wYear, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
	strSQL.Append(strField);
	strField.Format(L"track2Data='%s',", pInfo->szTrack2Data);
	strSQL.Append(strField);
	strField.Format(L"referenceNo='%s',", pInfo->szReferenceNo);
	strSQL.Append(strField);
	strField.Format(L"approvalCode='%s',", pInfo->szAuthID);
	strSQL.Append(strField);
	strField.Format(L"responseCode='%s',", pInfo->szResponseCode);
	strSQL.Append(strField);
	strField.Format(L"providerId='%s',", pInfo->szProviderID);
	strSQL.Append(strField);
	strField.Format(L"categoryId='%s',", pInfo->szCategoryID);
	strSQL.Append(strField);
	strField.Format(L"additionalData='%s',", pInfo->szAdditionalData);
	strSQL.Append(strField);
	strField.Format(L"chargeCount='%d',", pInfo->nChargeCount);
	strSQL.Append(strField);
	strField.Format(L"serial0='%s',", pInfo->szSerial[0]);
	strSQL.Append(strField);
	strField.Format(L"serial1='%s',", pInfo->szSerial[1]);
	strSQL.Append(strField);
	strField.Format(L"serial2='%s',", pInfo->szSerial[2]);
	strSQL.Append(strField);
	strField.Format(L"serial3='%s',", pInfo->szSerial[3]);
	strSQL.Append(strField);
	strField.Format(L"serial4='%s',", pInfo->szSerial[4]);
	strSQL.Append(strField);
	strField.Format(L"chargePin0='%s',", pInfo->szChargePIN[0]);
	strSQL.Append(strField);
	strField.Format(L"chargePin1='%s',", pInfo->szChargePIN[1]);
	strSQL.Append(strField);
	strField.Format(L"chargePin2='%s',", pInfo->szChargePIN[2]);
	strSQL.Append(strField);
	strField.Format(L"chargePin3='%s',", pInfo->szChargePIN[3]);
	strSQL.Append(strField);
	strField.Format(L"chargePin4='%s',", pInfo->szChargePIN[4]);
	strSQL.Append(strField);
	strField.Format(L"billId='%s',", pInfo->szBillID);
	strSQL.Append(strField);
	strField.Format(L"paymentId='%s',", pInfo->szPaymentID);
	strSQL.Append(strField);
	strField.Format(L"serviceCode='%d',", pInfo->nServiceCode);
	strSQL.Append(strField);
	strField.Format(L"serviceType='%s'", pInfo->szServiceType);
	strSQL.Append(strField);
	strField.Format(L" WHERE _id='%s';", pInfo->szID);
	strSQL.Append(strField);

	rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::UpdateTransactionStatus(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	CString	strField;
	int		rc;

	strSQL.Format(L"UPDATE %s SET ", DB_TABLE_NAME_TRANS);
	strField.Format(L"transactionStatus='%d'", pInfo->transactionStatus);
	strSQL.Append(strField);
	strField.Format(L" WHERE _id='%s';", pInfo->szID);
	strSQL.Append(strField);

	rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::UpdateTransactionAttempts(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	CString	strField;
	int		rc;

	strSQL.Format(L"UPDATE %s SET ", DB_TABLE_NAME_TRANS);
	strField.Format(L"attempts='%d'", pInfo->nAttempts);
	strSQL.Append(strField);
	strField.Format(L" WHERE _id='%s';", pInfo->szID);
	strSQL.Append(strField);

	rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::UpdateChargeRecord(CHARGE_INFO* pInfo)
{
	CString	strSQL;
	CString	strField;
	int		rc;

	strSQL.Format(L"UPDATE %s SET ", DB_TABLE_NAME_CHARGE);
	strField.Format(L"timestamp='%04d%02d%02d%02d%02d%02d',", pInfo->stTimestamp.wYear, pInfo->stTimestamp.wMonth, pInfo->stTimestamp.wDay, pInfo->stTimestamp.wHour, pInfo->stTimestamp.wMinute, pInfo->stTimestamp.wSecond);
	strSQL.Append(strField);
	strField.Format(L"traceNo='%s',", pInfo->szTraceNo);
	strSQL.Append(strField);
	strField.Format(L"providerId='%s',", pInfo->szProviderID);
	strSQL.Append(strField);
	strField.Format(L"categoryId='%s',", pInfo->szCategoryID);
	strSQL.Append(strField);
	strField.Format(L"amount='%d',", pInfo->szAmount);
	strSQL.Append(strField);
	strField.Format(L"serial='%s',", pInfo->szSerial);
	strSQL.Append(strField);
	strField.Format(L"chargePin='%s',", pInfo->szChargePIN);
	strSQL.Append(strField);
	strField.Format(L"referenceNo='%d',", pInfo->szReferenceNo);
	strSQL.Append(strField);
	strField.Format(L"approvalCode='%d'", pInfo->szAuthID);
	strSQL.Append(strField);
	strField.Format(L" WHERE _id='%s';", pInfo->szID);
	strSQL.Append(strField);

	rc	= m_DB_CHARGE.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::DeleteTransactionRecord(TRANSACTION_INFO* pInfo)
{
	CString	strSQL;
	int		rc;

	strSQL.Format(L"DELETE FROM %s WHERE _id='%s';", DB_TABLE_NAME_TRANS, pInfo->szID);

	rc	= m_DB_TRANS.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}

BOOL CTransactionDatabase::DeleteChargeRecord(CHARGE_INFO* pInfo)
{
	CString	strSQL;
	int		rc;

	strSQL.Format(L"DELETE FROM %s WHERE _id='%s';", DB_TABLE_NAME_CHARGE, pInfo->szID);

	rc	= m_DB_CHARGE.ExecuteNonQuery(strSQL);

	return (BOOL)(rc == SQLITE_OK);
}
