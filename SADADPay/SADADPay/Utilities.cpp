// Utilities.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "Utilities.h"
#include "DrawPNG.h"
#include "PciPad_t.h"
#include "Device.h"

void HideTaskBar(BOOL bHide)
{
	HWND	hTaskBar		= ::FindWindow(_T("HHTaskBar"), NULL);
	HKEY	hKey;
	DWORD	dwDisposition;
	DWORD	dwType			= REG_DWORD;
	DWORD	dwSize			= sizeof(DWORD);
	DWORD	dwValue;
	DWORD	nTop			= 0;

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_SHELL_ONTOP, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	if (bHide)
	{
		if (RegQueryValueEx(hKey, L"", 0, &dwType, reinterpret_cast<LPBYTE>(&nTop), &dwSize) != ERROR_SUCCESS) 
		{
			dwValue	= 1;
		}
		else
		{
			dwValue	= nTop;
		}

		RegSetValueEx(hKey, RV_BACKUP, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));

		dwValue	= 0;

		RegSetValueEx(hKey, L"", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD));
	}
	else
	{
		RegQueryValueEx(hKey, RV_BACKUP, 0, &dwType, reinterpret_cast<LPBYTE>(&nTop), &dwSize);
		RegSetValueEx(hKey, L"", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&nTop), sizeof(DWORD));
	}

	RegCloseKey(hKey);

	if (hTaskBar)
	{
		::SendMessage(hTaskBar, WM_WININICHANGE, 0, (LPARAM)5000);
	}
}

void DrawPNGResource(RES_PNG resPNG, HDC hDC, BOOL bRightToLeft)
{
	if (bRightToLeft)
	{
		DrawPNGResource(resPNG.nResID, resPNG.ptRightToLeft.x, resPNG.ptRightToLeft.y, resPNG.width, resPNG.height, hDC);
	}
	else
	{
		DrawPNGResource(resPNG.nResID, resPNG.ptLeftToRight.x, resPNG.ptLeftToRight.y, resPNG.width, resPNG.height, hDC);
	}
}

void GetRectFromPNGResource(LPRECT lprect, RES_PNG resPNG, BOOL bRightToLeft)
{
	if (bRightToLeft)
	{
		lprect->left	= resPNG.ptRightToLeft.x;
		lprect->top		= resPNG.ptRightToLeft.y;
	}
	else
	{
		lprect->left	= resPNG.ptLeftToRight.x;
		lprect->top		= resPNG.ptLeftToRight.y;
	}

	lprect->right	= lprect->left + resPNG.width;
	lprect->bottom	= lprect->top + resPNG.height;
}

CString LoadText(UINT nID, LANGUAGE_MODE langMode)
{
	switch (langMode)
	{
	case LANGUAGE_FARSI:
		{
			return LoadFarsiText(nID);
		}
		break;
	}

	return LoadEnglishText(nID);
}

CString LoadFarsiText(UINT nID)
{
	return g_strFarsi[nID];
}

CString LoadEnglishText(UINT nID)
{
	return g_strEnglish[nID];
}

CString	AddComma(CString strDigit)
{
	return FormattingArabicDigit(strDigit, TRUE);
}

CString FormattingDigit(CString strDigit, BOOL bComma, LANGUAGE_MODE langMode)
{
	switch (langMode)
	{
	case LANGUAGE_FARSI:
		{
			return FormattingFarsiDigit(strDigit, bComma);
		}
		break;
	}

	return FormattingArabicDigit(strDigit, bComma);
}

CString FormattingFarsiDigit(CString strDigit, BOOL bComma)
{
	CString strFarsi	= L"";
	int		nLength		= strDigit.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		WCHAR	c	= strDigit.GetAt(i);

		if (c < _T('0') || c > _T('9'))
		{
			strFarsi.AppendChar(c);
		}
		else
		{
			strFarsi.Append(LoadFarsiText(IDS_NUM0 + (strDigit.GetAt(i) - _T('0'))));
		}
	}

	if (bComma)
	{
		CString strResult	= L"";

		nLength	= strFarsi.GetLength();
		
		for (int i = 0; i < nLength; i++)
		{
			if (i && !(i % 3)) 
			{
				strResult	= L"," + strResult;
			}

			strResult	= CString(strFarsi[nLength - 1 - i]) + strResult;
		}

		for (int i = 0; i < strResult.GetLength(); i++)
		{
			if ((strResult.GetAt(i) >= L'۰') && (strResult.GetAt(i) <= L'۹'))
			{
				break;
			}

			if (strResult.GetAt(i) == L',')
			{
				strResult.SetAt(i, L' ');
			}
		}

		return strResult;
	}


	return strFarsi;
}

CString FormattingArabicDigit(CString strDigit, BOOL bComma)
{
	CString strArabic	= L"";
	int		nLength		= strDigit.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		WCHAR	c	= strDigit.GetAt(i);

		if (c < _T('0') || c > _T('9'))
		{
			strArabic.AppendChar(c);
		}
		else
		{
			strArabic.Append(LoadEnglishText(IDS_NUM0 + (strDigit.GetAt(i) - _T('0'))));
		}
	}

	if (bComma)
	{
		CString strResult	= L"";

		nLength	= strArabic.GetLength();
		
		for (int i = 0; i < nLength; i++)
		{
			if (i && !(i % 3))
			{
				strResult	= L"," + strResult;
			}

			strResult	= CString(strArabic[nLength - 1 - i]) + strResult;
		}

		for (int i = 0; i < strResult.GetLength(); i++)
		{
			if ((strResult.GetAt(i) >= L'0') && (strResult.GetAt(i) <= L'9'))
			{
				break;
			}

			if (strResult.GetAt(i) == L',')
			{
				strResult.SetAt(i, L' ');
			}
		}

		return strResult;
	}

	return strArabic;
}

LPCWSTR ExtractPANFromTrack2DataLeft(LPCWSTR szTrack2Data)
{
	CString	strTrack2Data	= szTrack2Data;
	int		nStartSentinel	= strTrack2Data.Find(';') + 1;
	int		nSeparator		= strTrack2Data.Find('=');
	CString	strPAN			= strTrack2Data.Mid(nStartSentinel, nSeparator - nStartSentinel);

	if (strPAN.GetLength() > LENGTH_PAN)
	{
		strPAN	= strPAN.Left(LENGTH_PAN);
	}

	return (LPCWSTR)(strPAN);
}

LPCWSTR ExtractPANFromTrack2DataRight(LPCWSTR szTrack2Data)
{
	CString	strTrack2Data	= szTrack2Data;
	int		nStartSentinel	= strTrack2Data.Find(';') + 1;
	int		nSeparator		= strTrack2Data.Find('=');
	CString	strPAN			= strTrack2Data.Mid(nStartSentinel, nSeparator - nStartSentinel);

	if (strPAN.GetLength() > LENGTH_PAN)
	{
		strPAN	= strPAN.Right(LENGTH_PAN);
	}

	return (LPCWSTR)(strPAN);
}

LPCWSTR BlindMarkPAN(LPCWSTR szPAN, LANGUAGE_MODE langMode)
{
	CString	strPAN		= szPAN;
	CString	strMarked	= L"";

	if (strPAN.GetLength() == LENGTH_PAN)
	{
		switch (langMode)
		{
		case LANGUAGE_FARSI:
			{
				strMarked.Append(strPAN.Mid(12, 4));
				strMarked.Append(L"******");
				strMarked.Append(strPAN.Mid(0, 6));
			}
			break;
		default:
			{
				strMarked.Append(strPAN.Mid(0, 6));
				strMarked.Append(L"******");
				strMarked.Append(strPAN.Mid(12, 4));
			}
			break;
		}
	}

	return (LPCWSTR)strMarked;
}

LPCWSTR GetBankBIN(LPCWSTR szPAN)
{
	CString	strPAN	= szPAN;

	return (LPCWSTR)(strPAN.Mid(0, 6));
}

BILINGUAL GetBankName(LPCWSTR szBIN, PARAMETERS_INFO* pInfo)
{
	BILINGUAL	biText;

	for (int i = 0; i < MAX_BANK_INFO; i++)
	{
		if (!wcscmp(szBIN, pInfo->bankInfo[i].szBankBIN))
		{
			return pInfo->bankInfo[i].bankName;
		}
	}

	wcscpy_s(biText.szE, 64, LoadEnglishText(IDS_ERROR_UNKNOWN_BANKID));
	wcscpy_s(biText.szF, 64, LoadFarsiText(IDS_ERROR_UNKNOWN_BANKID));
	return biText;
}

BOOL ValidateBillID(WCHAR* szBillID)
{
	WCHAR	cMultiply[]	= L"02345672345672345672345672345";
	WCHAR	cTemp[32];
	int		nSum		= 0;

	wcscpy(cTemp, szBillID);
	wcsrev(cTemp);

	for (int i = 0; i < (int)wcslen(szBillID); i++)
	{
		nSum	+= (cTemp[i] - 0x30) * (cMultiply[i] - 0x30);
	}

	nSum	%= 11;
	nSum	= ((nSum <= 1) ? 0 : 11 - nSum);

	return (BOOL)((nSum + 0x30) == cTemp[0]);
}

BOOL ValidatePaymentID(WCHAR* szBillID, WCHAR* szPaymentID)
{
	WCHAR	cTemp[32];
	int		nSum		= 0;
	WCHAR*	pBillID		= szBillID;
	WCHAR*	pPaymentID	= szPaymentID;

	wcscpy(cTemp, szPaymentID);

	cTemp[wcslen(szPaymentID) - 1]	= 0x00;

	if (ValidateBillID(cTemp) == FALSE)
	{
		return FALSE;
	}

	while (*pBillID == L'0')
	{
		pBillID++;
	}

	while (*pPaymentID == L'0')
	{
		pPaymentID++;
	}

	swprintf(cTemp, L"%s%s", pBillID, pPaymentID);

	if (ValidateBillID(cTemp) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

UINT GetServiceCodeFromBillID(WCHAR* szBillID)
{
	CString strBillID	= szBillID;

	if (!ValidateBillID(szBillID))
	{
		return 1;
	}

	return (UINT)(_wtoi(strBillID.Mid(11, 1)));
}

BILINGUAL GetServiceCodeName(UINT nServiceCode, PARAMETERS_INFO* pInfo)
{
	BILINGUAL	text;

	memset(&text, 0, sizeof(BILINGUAL));

	for (int i = 0; i < MAX_BILL_SERVICE; i++)
	{
		if (pInfo->billService[i].nServiceCodeID == nServiceCode)
		{
			return pInfo->billService[i].serviceCodeName;
		}
	}

	return text;
}

BILINGUAL	GetProviderName(WCHAR* szProviderID, PARAMETERS_INFO* pInfo)
{
	BILINGUAL	text;

	memset(&text, 0, sizeof(BILINGUAL));

	for (int i = 0; i < MAX_PROVIDER; i++)
	{
		if (!wcscmp(pInfo->providers[i].szProviderID, szProviderID))
		{
			return pInfo->providers[i].providerName;
		}
	}

	return text;
}

BILINGUAL GetResponseText(UINT nResponseCode, PARAMETERS_INFO* pInfo)
{
	BILINGUAL	text;

	memset(&text, 0, sizeof(BILINGUAL));

	for (int i = 0; i < MAX_RESPONSE_CODE; i++)
	{
		if (pInfo->responseCode[i].nCode == nResponseCode)
		{
			return pInfo->responseCode[i].message;
		}
	}

	return text;
}

CString GetResponseText(UINT nResponseCode, LANGUAGE_MODE langMode, PARAMETERS_INFO* pInfo)
{
	CString	strResponseCode;

	for (int i = 0; i < MAX_RESPONSE_CODE; i++)
	{
		if (pInfo->responseCode[i].nCode == nResponseCode)
		{
			switch (langMode)
			{
			case LANGUAGE_FARSI:
				{
					strResponseCode.Format(L"%s[%02d]", pInfo->responseCode[i].message.szF, pInfo->responseCode[i].nCode);
				}
				break;
			default:
				{
					strResponseCode.Format(L"%s[%02d]", pInfo->responseCode[i].message.szE, pInfo->responseCode[i].nCode);
				}
				break;
			}
			
			break;
		}
	}

	return strResponseCode;
}

BOOL MakePINBlock(LPBYTE lpPINBlock, LPDWORD lpdwBlockSize, LPCWSTR szPIN, LPCWSTR szTrack2Data)
{
	DWORD	dwResult	= 0;
	BYTE	KSN[10]		= { 0, };
	CString	strPAN		= ExtractPANFromTrack2DataRight(szTrack2Data);
	WCHAR	szBankID[9]	= L"11111111";

	for (int i = 0; i < 3; i++)
	{
		memset(lpPINBlock, 0, (*lpdwBlockSize));
		LOGFILE(L"%S[%d] - BBSecure_GetPINBlock_withData('%s', 4, '%s', 16, 0, 0, 0, '%s', 8, 60, lpPINBlock, lpdwBlockSize, KSN)\r\n", __FUNCTION__, __LINE__, szPIN, strPAN, szBankID);

		dwResult	= BBSecure_GetPINBlock_withData(szPIN, 4, strPAN, 16, 0, 0, 0, szBankID, 8, 60, lpPINBlock, lpdwBlockSize, KSN);
		
		LOGFILE(L"%S[%d] - BBSecure_GetPINBlock_withData() = %x [%d][%02X %02X %02X %02X %02X %02X %02X %02X]\r\n", __FUNCTION__, __LINE__, dwResult, (*lpdwBlockSize), lpPINBlock[0], lpPINBlock[1], lpPINBlock[2], lpPINBlock[3], lpPINBlock[4], lpPINBlock[5], lpPINBlock[6], lpPINBlock[7]);

		if (!dwResult)
		{
			break;
		}
	}

	return (BOOL)(dwResult == 0);
}

LONG CompareSystemTime(const LPSYSTEMTIME lpSystemTime1, const LPSYSTEMTIME lpSystemTime2)
{
	FILETIME	ftTime1;
	FILETIME	ftTime2;

	SystemTimeToFileTime(lpSystemTime1, &ftTime1);
	SystemTimeToFileTime(lpSystemTime2, &ftTime2);

	return (CompareFileTime(&ftTime1, &ftTime2));
}

void AddSystemTime(LPSYSTEMTIME lpSystemTime, ULONGLONG ullSpan)
{
	FILETIME		ftTimeSrc;
	FILETIME		ftTimeDst;
	ULARGE_INTEGER	uiTime;

	SystemTimeToFileTime(lpSystemTime, &ftTimeSrc);

	uiTime.LowPart	= ftTimeSrc.dwLowDateTime;
	uiTime.HighPart	= ftTimeSrc.dwHighDateTime;
	uiTime.QuadPart	+= ullSpan;

	ftTimeDst.dwLowDateTime		= uiTime.LowPart;
	ftTimeDst.dwHighDateTime	= uiTime.HighPart;

	FileTimeToSystemTime(&ftTimeDst, lpSystemTime);
}

BOOL WA2BA(LPBYTE lpBuffer, UINT uSize, const WCHAR* lpWide, UINT uLength)
{
	if (uSize <= uLength)
	{
		return FALSE;
	}

	memset(lpBuffer, 0, uSize * sizeof(BYTE));

	for (UINT i = 0; i < uLength; i++)
	{
		lpBuffer[i]	= (BYTE)(lpWide[i] & 0xFF);
	}

	return TRUE;
}

BOOL BA2WA(WCHAR* lpWide, UINT uSize, const BYTE* lpBuffer, UINT uLength)
{
	if (uSize <= uLength)
	{
		return FALSE;
	}

	memset(lpWide, 0, uSize * sizeof(WCHAR));

	for (UINT i = 0; i < uLength; i++)
	{
		lpWide[i]	= (WCHAR)(lpBuffer[i]);
	}

	return TRUE;
}

BOOL BA2CS(CString &strString, const BYTE* lpBuffer, UINT uLength)
{
	strString.Empty();

	for (UINT i = 0; i < uLength; i++)
	{
		strString.AppendChar(lpBuffer[i]);
	}

	return TRUE;
}

BOOL WA2BN(LPBYTE lpBuffer, UINT uSize, const WCHAR* lpWide, UINT uLength)
{
	if (uSize < uLength)
	{
		return FALSE;
	}

	memset(lpBuffer, 0, uSize * sizeof(BYTE));

	for (UINT i = 0; i < uLength; i++)
	{
		lpBuffer[i]	= (BYTE)(lpWide[i] & 0xFF) - 0x30;
	}

	return TRUE;
}

BOOL BA2WN(WCHAR* lpWide, UINT uSize, const BYTE* lpBuffer, UINT uLength)
{
	if (uSize < uLength)
	{
		return FALSE;
	}

	memset(lpWide, 0, uSize * sizeof(WCHAR));

	for (UINT i = 0; i < uLength; i++)
	{
		lpWide[i]	= (WCHAR)(lpBuffer[i]) - 0x30;
	}

	return TRUE;
}

WORD BA2WD(const BYTE* lpBuffer, UINT uLength)
{
	char	cBuffer[6];
	WORD	wValue	= 0;

	memset(cBuffer, 0, sizeof(char) * 6);
	memcpy(cBuffer, lpBuffer, uLength);

	for (UINT i = 0; i < uLength; i++)
	{
		wValue	*= 10;
		wValue	+= (int)(cBuffer[i] - 0x30);
	}
	
	return wValue;
}

WORD HC2DN(WCHAR cHex)
{
	WORD	wDec	= 0;

	switch (cHex)
	{
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
		{
			wDec	= cHex - '0' + 0x00;
		}
		break;
	case L'A':
	case L'B':
	case L'C':
	case L'D':
	case L'E':
	case L'F':
		{
			wDec	= cHex - 'A' + 0x0A;
		}
		break;
	case L'a':
	case L'b':
	case L'c':
	case L'd':
	case L'e':
	case L'f':
		{
			wDec	= cHex - 'a' + 0x0A;
		}
		break;
	}

	return wDec;
}

BOOL IP2CS(CString& strString, WCHAR* lpWide, UINT uLength)
{
	int nCount	= 0;

	strString.Empty();

	for (int i = 0; i < (int)uLength; i++)
	{
		if (lpWide[i] == '.')
		{
			while ((nCount % 3) != 0)
			{
				strString.AppendChar(L' ');
				nCount++;
			}

			nCount	= 0;
		}
		else
		{
			if (nCount >= 3)
			{
				return FALSE;
			}

			strString.AppendChar(lpWide[i]);
	
			nCount++;
		}
	}

	while (strString.GetLength() < 12)
	{
		strString.AppendChar(L' ');
	}

	return (BOOL)(strString.GetLength() == 12);
}

BOOL CS2IP(WCHAR* lpWide, UINT uSize, CString& strString)
{
	WCHAR	szBlock[4];
	int		nBlock;
	int		nPos	= 0;

	if (uSize < 16)
	{
		return FALSE;
	}

	if ((strString.GetLength() < 10) || (strString.GetLength() > 12))
	{
		return FALSE;
	}

	while (strString.GetLength() < 12)
	{
		strString.AppendChar(L' ');
	}

	memset(lpWide, 0, sizeof(WCHAR) * uSize);

	for (int i = 0; i < 12; i++)
	{
		if ((i % 3) == 0)
		{
			if (i > 0)
			{
				if (wcslen(szBlock) < 1)
				{
					return FALSE;
				}
				
				nBlock	= _wtoi(szBlock);

				if ((nBlock < 0) || (nBlock > 255))
				{
					return FALSE;
				}

				wcscat(lpWide, szBlock);
				wcscat(lpWide, L".");
			}

			memset(szBlock, 0, sizeof(WCHAR) * 4);

			nPos	= 0;
		}

		if (strString.GetAt(i) != L' ')
		{
			szBlock[nPos++]	= strString.GetAt(i);
		}
	}

	wcscat(lpWide, szBlock);

	return TRUE;
}

WCHAR ISES2WC(BYTE cISE)
{
	if ((cISE >= 0) && (cISE < 0x80))
	{
		return (WCHAR)cISE;
	}

	return g_ISES_CNVT_TBL[cISE - 0x80];
}

BOOL ISESA2WA(WCHAR* lpWide, UINT uSize, LPBYTE lpBuffer, UINT uLength)
{
	if (uSize <= uLength)
	{
		return FALSE;
	}

	memset(lpWide, 0, uSize * sizeof(WCHAR));

	for (UINT i = 0; i < uLength; i++)
	{
		lpWide[i]	= ISES2WC(lpBuffer[i]);
	}

	return TRUE;

}

BOOL GetDatabaseID(WCHAR* szID, UINT uSize, UINT nAdditional)
{
	SYSTEMTIME	st;

	if (uSize < 17)
	{
		return FALSE;
	}

	memset(szID, 0, uSize * sizeof(WCHAR));
	GetLocalTime(&st);
	swprintf(szID, L"%02d%02d%02d%02d%02d%02d%03d%1d", st.wYear % 100, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, GetTickCount() % 1000, nAdditional % 10);

	return TRUE;
}

BOOL GetApplicationVersion(WORD* wMajor, WORD* wMinor, WORD* wRevision, WORD* wBuild)
{
	HRSRC				hResInfo;
	DWORD				dwSize;
	HGLOBAL				hResData;
	LPVOID				pRes;
	LPVOID				pResCopy;
	UINT				uLength;
	VS_FIXEDFILEINFO*	lpFFI;
	CString				strVersion;

	(*wMajor)		= 0;
	(*wMinor)		= 0;
	(*wRevision)	= 0;
	(*wBuild)		= 0;
	hResInfo		= FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);

	if (hResInfo == NULL)
	{
		return FALSE;
	}
	
	dwSize		= SizeofResource(AfxGetResourceHandle(), hResInfo);

	if (dwSize == 0)
	{
		return FALSE;
	}

	hResData	= LoadResource(AfxGetResourceHandle(), hResInfo);

	if (hResData == NULL)
	{
		return FALSE;
	}

	pRes		= LockResource(hResData);
	pResCopy	= LocalAlloc(LMEM_FIXED, dwSize);

	CopyMemory(pResCopy, pRes, dwSize);
	FreeResource(hResData);

	VerQueryValue(pResCopy, L"\\", (LPVOID*)&lpFFI, &uLength);

	(*wMajor)		= HIWORD(lpFFI->dwFileVersionMS);
	(*wMinor)		= LOWORD(lpFFI->dwFileVersionMS);
	(*wRevision)	= HIWORD(lpFFI->dwFileVersionLS);
	(*wBuild)		= LOWORD(lpFFI->dwFileVersionLS);

	LocalFree(pResCopy);

	return TRUE;
}

BOOL GetAppVersion(WCHAR* szVersion, UINT nSize)
{
	WORD	wMajor;
	WORD	wMinor;
	WORD	wRevision;
	WORD	wBuild;
	WCHAR	szBuffer[32];

	GetApplicationVersion(&wMajor, &wMinor, &wRevision, &wBuild);
	swprintf(szBuffer, L"%02d.%02d.%02d", wMajor, wMinor, wRevision);

	if (nSize <= wcslen(szBuffer))
	{
		return FALSE;
	}

	wcscpy_s(szVersion, nSize, szBuffer);

	return TRUE;
}

BOOL GetPOSSoftwareVersion(WCHAR* szVersion, UINT nSize)
{
#if TEST_MODE
	wcscpy_s(szVersion, nSize, TEST_SW_VERSION);
#else
	WORD	wMajor;
	WORD	wMinor;
	WORD	wRevision;
	WORD	wBuild;
	WCHAR	szBuffer[32];

	GetApplicationVersion(&wMajor, &wMinor, &wRevision, &wBuild);
	swprintf(szBuffer, L"%02d%02d%02d", wMajor, wMinor, wRevision);

	if (nSize <= wcslen(szBuffer))
	{
		return FALSE;
	}

	wcscpy_s(szVersion, nSize, szBuffer);
#endif

	return TRUE;
}

int	Pseudorandom(int nMin, int nMax)
{
	return (int)((double)rand() / (RAND_MAX + 1) * (nMax - nMin) + nMin);
}

void XOR(LPBYTE pData1, LPBYTE pData2, LPBYTE pResult, UINT uLength)
{
	while (uLength)
	{
		*pResult++	= *pData1++ ^ *pData2++;
		uLength--;
    }
}
