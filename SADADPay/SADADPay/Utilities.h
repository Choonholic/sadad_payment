// Utilities.h

#pragma once

#include "Definitions.h"
#include "StringRes.h"
#include "Settings.h"
#include "DCX.h"

#define OS_100NS	(ULONGLONG)(10000000UL)
#define OM_100NS	(ULONGLONG)((ULONGLONG)(10000000UL) * (ULONGLONG)(60UL))
#define OH_100NS	(ULONGLONG)((ULONGLONG)(10000000UL) * (ULONGLONG)(3600UL))
#define OD_100NS	(ULONGLONG)((ULONGLONG)(10000000UL) * (ULONGLONG)(86400UL))

void	HideTaskBar(BOOL bHide = TRUE);

void	DrawPNGResource(RES_PNG resPNG, HDC hDC, BOOL bRightToLeft);
void	GetRectFromPNGResource(LPRECT lprect, RES_PNG resPNG, BOOL bRightToLeft);

CString	LoadText(UINT nID, LANGUAGE_MODE langMode);
CString	LoadFarsiText(UINT nID);
CString LoadEnglishText(UINT nID);

CString	AddComma(CString strDigit);

CString	FormattingDigit(CString strDigit, BOOL bComma, LANGUAGE_MODE langMode);
CString	FormattingFarsiDigit(CString strDigit, BOOL bComma);
CString	FormattingArabicDigit(CString strDigit, BOOL bComma);

TRANSACTION_MODE	GetTransactionMode(LPCWSTR szMsgType);

LPCWSTR	ExtractPANFromTrack2DataLeft(LPCWSTR szTrack2Data);
LPCWSTR	ExtractPANFromTrack2DataRight(LPCWSTR szTrack2Data);
LPCWSTR	BlindMarkPAN(LPCWSTR szPAN, LANGUAGE_MODE langMode);

LPCWSTR		GetBankBIN(LPCWSTR szPAN);
BILINGUAL	GetBankName(LPCWSTR szBIN, PARAMETERS_INFO* pInfo);

BOOL		ValidateBillID(WCHAR* szBillID);
BOOL		ValidatePaymentID(WCHAR* szBillID, WCHAR* szPaymentID);
UINT		GetServiceCodeFromBillID(WCHAR* szBillID);
BILINGUAL	GetServiceCodeName(UINT nServiceCode, PARAMETERS_INFO* pInfo);

BILINGUAL	GetProviderName(WCHAR* szProviderID, PARAMETERS_INFO* pInfo);

BILINGUAL	GetResponseText(UINT nResponseCode, PARAMETERS_INFO* pInfo);
CString		GetResponseText(UINT nResponseCode, LANGUAGE_MODE langMode, PARAMETERS_INFO* pInfo);

BOOL	MakePINBlock(LPBYTE lpPINBlock, LPDWORD lpdwBlockSize, LPCWSTR szPIN, LPCWSTR szTrack2Data);

LONG	CompareSystemTime(const LPSYSTEMTIME lpSystemTime1, const LPSYSTEMTIME lpSystemTime2);
void	AddSystemTime(LPSYSTEMTIME lpSystemTime, ULONGLONG ullSpan);

BOOL	WA2BA(LPBYTE lpBuffer, UINT uSize, const WCHAR* lpWide, UINT uLength);
BOOL	BA2WA(WCHAR* lpWide, UINT uSize, const BYTE* lpBuffer, UINT uLength);
BOOL	BA2CS(CString &strString, const BYTE* lpBuffer, UINT uLength);
BOOL	WA2BN(LPBYTE lpBuffer, UINT uSize, const WCHAR* lpWide, UINT uLength);
BOOL	BA2WN(WCHAR* lpWide, UINT uSize, const BYTE* lpBuffer, UINT uLength);
WORD	BA2WD(const BYTE* lpBuffer, UINT uLength);
WORD	HC2DN(WCHAR cHex);

BOOL	IP2CS(CString& strString, WCHAR* lpWide, UINT uLength);
BOOL	CS2IP(WCHAR* lpWide, UINT uSize, CString& strString);

WCHAR	ISES2WC(BYTE cISE);
BOOL	ISESA2WA(WCHAR* lpWide, UINT uSize, LPBYTE lpBuffer, UINT uLength);

BOOL	GetDatabaseID(WCHAR* szID, UINT uSize, UINT nAdditional = 0);

BOOL	GetApplicationVersion(WORD* wMajor, WORD* wMinor, WORD* wRevision, WORD* wBuild);
BOOL	GetAppVersion(WCHAR* szVersion, UINT nSize);
BOOL	GetPOSSoftwareVersion(WCHAR* szVersion, UINT nSize);

int		Pseudorandom(int nMin, int nMax);

void	XOR(LPBYTE pData1, LPBYTE pData2, LPBYTE pResult, UINT uLength);
