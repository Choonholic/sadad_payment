// Utilities.h

#pragma once

#include "RilTools.h"

extern	CRilTools*	g_pRilTools;
extern	HRESULT		g_hResult;
extern	BOOL		g_bDataState;
extern	HANDLE		g_hConnection;

extern	TCHAR		g_szGetGprsEntryName[2048];
extern	TCHAR		g_szAddGprsEntry[4096];
extern	TCHAR		g_szDeleteGprsEntry[2048];
extern	TCHAR		g_szEnableGprsEntry[2048];
extern	TCHAR		g_szDisableGprsEntry[2048];

LONG	GetRegistryDWORD(HKEY hKeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD* pValue);

GUID	GetNetworkForURL(LPCTSTR szUrl);
void	DataDisconnect(TCHAR* tszName);
void	GetEnabledGprsEntryName(TCHAR* tszName);
BOOL	DataState(TCHAR* tszName);
