// Settings.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "Settings.h"
#include "Device.h"
#include "XMLManager.h"

void SetDefaultSettings(SETTINGS_INFO* pSettings)
{
	pSettings->dwVersion	= VERSION_SETTINGS;
	pSettings->dwSize		= sizeof(SETTINGS_INFO);
	pSettings->langMode		= LANGUAGE_ENGLISH;
	pSettings->nBrightness	= 8;
	pSettings->bMasterKey	= FALSE;
	pSettings->logonStatus	= LOGON_NONE;
	pSettings->nF2Invalid	= 0;
	pSettings->nF3Invalid	= 0;
	pSettings->dwTraceNo	= 0;

	memcpy(pSettings->cPassword, "0000****", LENGTH_PASSWORD);

	memset(&pSettings->stF2Open, 0, sizeof(SYSTEMTIME));
	memset(&pSettings->stF3Open, 0, sizeof(SYSTEMTIME));
	memset(pSettings->szCityCode, 0, sizeof(WCHAR) * (LENGTH_CITY_CODE + 1));

	SetPOSInformation(pSettings->cPOSInformation, LENGTH_POS_INFORMATION);
}

void LoadSettings(SETTINGS_INFO* pSettings)
{
	HANDLE	hFile;
	DWORD	dwBytes;

	hFile	= CreateFile(PATH_SETTINGS, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadFile(hFile, pSettings, sizeof(SETTINGS_INFO), &dwBytes, NULL);
		CloseHandle(hFile);
		SetPOSInformation(pSettings->cPOSInformation, LENGTH_POS_INFORMATION);

		if ((pSettings->dwVersion != VERSION_SETTINGS) || (pSettings->dwSize != sizeof(SETTINGS_INFO)))
		{
			SetDefaultSettings(pSettings);
			SaveSettings(pSettings);
		}
	}
	else
	{
		SetDefaultSettings(pSettings);
		SaveSettings(pSettings);
	}
}

void SaveSettings(SETTINGS_INFO* pSettings)
{
	HANDLE	hFile;
	DWORD	dwBytes;

	hFile	= CreateFile(PATH_SETTINGS, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, pSettings, sizeof(SETTINGS_INFO), &dwBytes, NULL);
	CloseHandle(hFile);
}

BOOL LoadParameters(PARAMETERS_INFO* pParameters)
{
	HANDLE	hFile;
	DWORD	dwBytes;

	hFile	= CreateFile(PATH_PARAMETERS, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadFile(hFile, pParameters, sizeof(PARAMETERS_INFO), &dwBytes, NULL);
		CloseHandle(hFile);

		if ((pParameters->dwVersion == VERSION_PARAMETERS) && (pParameters->dwSize == sizeof(PARAMETERS_INFO)))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void SaveParameters(PARAMETERS_INFO* pParameters)
{
	HANDLE	hFile;
	DWORD	dwBytes;

	hFile	= CreateFile(PATH_PARAMETERS, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, pParameters, sizeof(PARAMETERS_INFO), &dwBytes, NULL);
	CloseHandle(hFile);
}

BOOL LoadConfigXML(PARAMETERS_INFO* pParameters)
{
	CXMLManager		xmlManager;
	CString			strNodePath;
	CString			strText;
	PXmlNodeList	nodeList;
	PXmlNodeList	childNodeList;
	PXmlNode		root;
	PXmlNode		item;
	PXmlNode		childItem;
	long			lLength;
	long			lChildLength;
	int				i;
	int				j;

	memset(pParameters, 0, sizeof(PARAMETERS_INFO));
	xmlManager.Init();

	if (!xmlManager.Load(PATH_CONFIGXML))
	{
		xmlManager.Release();

		return FALSE;
	}

	// Connection Types
	strNodePath	= L"//PaymentConfig//ConSet//Con//CT";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxConnectionType	= (lLength - 1);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			if (xmlManager.GetNodeName(item) == L"use")
			{
				pParameters->nCurrentConnectionType	= _wtoi(xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"Opt")
			{
				pParameters->connectionType[i - 1].nIndex	= _wtoi(xmlManager.GetNodeText(item, L"idx"));

				wcscpy(pParameters->connectionType[i - 1].connectionName.szE, xmlManager.GetNodeText(item, L"en"));
				wcscpy(pParameters->connectionType[i - 1].connectionName.szF, xmlManager.GetNodeText(item, L"fn"));
			}
		}
	}

	// GPRS Codes
	strNodePath	= L"//PaymentConfig//ConSet//Con//GPRS";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxGPRSCode	= (lLength - 1);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			if (xmlManager.GetNodeName(item) == L"Opt")
			{
				wcscpy(pParameters->GPRSCode[i].szCode, xmlManager.GetNodeText(item, L"code"));
				wcscpy(pParameters->GPRSCode[i].szName, xmlManager.GetNodeText(item, L"name"));
			}
		}
	}

	// Phonebook
	strNodePath	= L"//PaymentConfig//ConSet//Con//TPL";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxPhonebook	= lLength;

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			wcscpy(pParameters->szPhonebook[i], xmlManager.GetNodeText(item, L"num"));
		}
	}

	// TMS Phonebook
	strNodePath	= L"//PaymentConfig//ConSet//Con//TMSPL";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxTMS	= (lLength - 1);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			if (xmlManager.GetNodeName(item) == L"use")
			{
				pParameters->nTMSIndex	= _wtoi(xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"Opt")
			{
				wcscpy(pParameters->szTMS[i - 1], xmlManager.GetNodeText(item, L"num"));
			}
		}
	}

	// Server Settings
	strNodePath	= L"//PaymentConfig//ConSet//Con//Server";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		wcscpy(pParameters->szServerIP, xmlManager.GetNodeText(root, L"ip"));
		wcscpy(pParameters->szServerPort, xmlManager.GetNodeText(root, L"port"));
	}

	// Timeout
	strNodePath	= L"//PaymentConfig//ConSet//TO";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		pParameters->nTimeoutConnection	= _wtoi(xmlManager.GetNodeText(root, L"cto"));
		pParameters->nTimeoutConnected	= _wtoi(xmlManager.GetNodeText(root, L"cdto"));
		pParameters->nTimeoutPIN		= _wtoi(xmlManager.GetNodeText(root, L"pinto"));
		pParameters->nTimeoutAdvice		= _wtoi(xmlManager.GetNodeText(root, L"ato"));
		pParameters->nTimeoutReversal	= _wtoi(xmlManager.GetNodeText(root, L"rto"));
		pParameters->nTimeoutScreen		= _wtoi(xmlManager.GetNodeText(root, L"sto"));
	}

	// Network Settings
	strNodePath	= L"//PaymentConfig//ConSet//Net";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		wcscpy(pParameters->szPOSIP, xmlManager.GetNodeText(root, L"ip"));
		wcscpy(pParameters->szPOSGateway, xmlManager.GetNodeText(root, L"gw"));
		wcscpy(pParameters->szPOSSubnetMask, xmlManager.GetNodeText(root, L"sm"));
	}

	// Terminal Settings
	strNodePath	= L"//PaymentConfig//Sys//Tm";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			if (xmlManager.GetNodeName(item) == L"beep")
			{
				pParameters->bBeep	= (BOOL)_wtoi(xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"lang")
			{
				item->get_childNodes(&childNodeList);
				childNodeList->get_length(&lChildLength);

				pParameters->nMaxLanguage	= (lChildLength - 1);

				for (j = 0; j < lChildLength; j++)
				{
					childNodeList->get_item(j, &childItem);

					if (xmlManager.GetNodeName(childItem) == L"use")
					{
						pParameters->nLanguage	= _wtoi(xmlManager.GetNodeText(childItem));
					}
					else if (xmlManager.GetNodeName(childItem) == L"Opt")
					{
						wcscpy(pParameters->languageType[j - 1].szE, xmlManager.GetNodeText(childItem, L"en"));
						wcscpy(pParameters->languageType[j - 1].szF, xmlManager.GetNodeText(childItem, L"fn"));
					}
				}
			}
			else if (xmlManager.GetNodeName(item) == L"date")
			{
				strText							= xmlManager.GetNodeText(item);
				pParameters->stSyncTime.wYear	= (WORD)_wtoi(strText.Mid(0, 4));
				pParameters->stSyncTime.wMonth	= (WORD)_wtoi(strText.Mid(4, 2));
				pParameters->stSyncTime.wDay	= (WORD)_wtoi(strText.Mid(6, 2));
			}
			else if (xmlManager.GetNodeName(item) == L"time")
			{
				strText							= xmlManager.GetNodeText(item);
				pParameters->stSyncTime.wHour	= (WORD)_wtoi(strText.Mid(0, 2));
				pParameters->stSyncTime.wMinute	= (WORD)_wtoi(strText.Mid(2, 2));
				pParameters->stSyncTime.wSecond	= (WORD)_wtoi(strText.Mid(4, 2));
			}
		}
	}

	// Merchant Settings
	strNodePath	= L"//PaymentConfig//Sys//Mc";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			if (xmlManager.GetNodeName(item) == L"stel")
			{
				wcscpy(pParameters->szSupportPhone, xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"mtel")
			{
				wcscpy(pParameters->szMerchantPhone, xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"mname")
			{
				wcscpy(pParameters->merchantName.szE, xmlManager.GetNodeText(item, L"en"));
				wcscpy(pParameters->merchantName.szF, xmlManager.GetNodeText(item, L"fn"));
			}
			else if (xmlManager.GetNodeName(item) == L"mpc")
			{
				wcscpy(pParameters->szMerchantPostal, xmlManager.GetNodeText(item));
			}
			else if (xmlManager.GetNodeName(item) == L"mad")
			{
				wcscpy(pParameters->merchantAddress.szE, xmlManager.GetNodeText(item, L"en"));
				wcscpy(pParameters->merchantAddress.szF, xmlManager.GetNodeText(item, L"fn"));
			}
		}
	}

	// Transaction Settings
	strNodePath	= L"//PaymentConfig//Trs";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		wcscpy(pParameters->szPOSEntryMode, xmlManager.GetNodeText(root, L"pem"));
		wcscpy(pParameters->szNII, xmlManager.GetNodeText(root, L"nii"));
		wcscpy(pParameters->szPOSConditonCode, xmlManager.GetNodeText(root, L"pconcd"));
		wcscpy(pParameters->szTerminalID, xmlManager.GetNodeText(root, L"catid"));
		wcscpy(pParameters->szMerchantID, xmlManager.GetNodeText(root, L"caaic"));

		pParameters->nTips			= _wtoi(xmlManager.GetNodeText(root, L"tips"));
		pParameters->nAmountType	= _wtoi(xmlManager.GetNodeText(root, L"atype"));
	}

	// Response Codes
	strNodePath	= L"//PaymentConfig//Trs//RP";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxResponseCode	= lLength;

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			pParameters->responseCode[i].nCode	= _wtoi(xmlManager.GetNodeText(item, L"code"));

			wcscpy(pParameters->responseCode[i].message.szE, xmlManager.GetNodeText(item, L"en"));
			wcscpy(pParameters->responseCode[i].message.szF, xmlManager.GetNodeText(item, L"fn"));
		}
	}

	// Bill Service Codes
	strNodePath	= L"//PaymentConfig//Trs//Billsc";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxBillService	= lLength;

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			pParameters->billService[i].nServiceCodeID	= _wtoi(xmlManager.GetNodeText(item, L"sid"));

			wcscpy(pParameters->billService[i].serviceCodeName.szE, xmlManager.GetNodeText(item, L"en"));
			wcscpy(pParameters->billService[i].serviceCodeName.szF, xmlManager.GetNodeText(item, L"fn"));
		}
	}

	// Banks
	strNodePath	= L"//PaymentConfig//Trs//Bank";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxBankInfo	= lLength;

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			wcscpy(pParameters->bankInfo[i].szBankBIN, xmlManager.GetNodeText(item, L"bin"));
			wcscpy(pParameters->bankInfo[i].bankName.szE, xmlManager.GetNodeText(item, L"en"));
			wcscpy(pParameters->bankInfo[i].bankName.szF, xmlManager.GetNodeText(item, L"fn"));
		}
	}

	// Card Product Codes
	strNodePath	= L"//PaymentConfig//Trs//Cpcode";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		pParameters->nMaxCardProduct	= lLength;

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);

			wcscpy(pParameters->cardProduct[i].szCardProduct, xmlManager.GetNodeText(item, L"pcode"));
			wcscpy(pParameters->cardProduct[i].cardProductName.szE, xmlManager.GetNodeText(item, L"en"));
			wcscpy(pParameters->cardProduct[i].cardProductName.szF, xmlManager.GetNodeText(item, L"fn"));
		}
	}

	// Providers
	strNodePath	= L"//PaymentConfig//Cg//max";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		pParameters->nMaxProviders	= _wtoi(xmlManager.GetNodeText(root));
	}

	strNodePath	= L"//PaymentConfig//Cg//Pvs";

	if (xmlManager.SelectSingleNode(strNodePath, &root) == S_OK)
	{
		root->get_childNodes(&nodeList);
		nodeList->get_length(&lLength);

		for (i = 0; i < lLength; i++)
		{
			nodeList->get_item(i, &item);
			item->get_childNodes(&childNodeList);
			childNodeList->get_length(&lChildLength);

			pParameters->providers[i].nVoucherCount	= lChildLength - 5;

			for (j = 0; j < lChildLength; j++)
			{
				childNodeList->get_item(j, &childItem);

				if (xmlManager.GetNodeName(childItem) == L"id")
				{
					wcscpy(pParameters->providers[i].szProviderID, xmlManager.GetNodeText(childItem));
				}
				else if (xmlManager.GetNodeName(childItem) == L"en")
				{
					wcscpy(pParameters->providers[i].providerName.szE, xmlManager.GetNodeText(childItem));
				}
				else if (xmlManager.GetNodeName(childItem) == L"fn")
				{
					wcscpy(pParameters->providers[i].providerName.szF, xmlManager.GetNodeText(childItem));
				}
				else if (xmlManager.GetNodeName(childItem) == L"supp")
				{
					wcscpy(pParameters->providers[i].szCustomerSupport, xmlManager.GetNodeText(childItem));
				}
				else if (xmlManager.GetNodeName(childItem) == L"load")
				{
					wcscpy(pParameters->providers[i].szLoadProcedure, xmlManager.GetNodeText(childItem));
				}
				else if (xmlManager.GetNodeName(childItem) == L"voucher")
				{
					pParameters->providers[i].vouchers[j - 5].nIndex	= _wtoi(xmlManager.GetNodeText(childItem, L"idx"));
					pParameters->providers[i].vouchers[j - 5].nCID		= _wtoi(xmlManager.GetNodeText(childItem, L"cid"));
					pParameters->providers[i].vouchers[j - 5].llAmount	= _wtoll(xmlManager.GetNodeText(childItem, L"amount"));
				}
			}
		}
	}

	xmlManager.Release();

	pParameters->dwVersion	= VERSION_PARAMETERS;
	pParameters->dwSize		= sizeof(PARAMETERS_INFO);

	return TRUE;
}
