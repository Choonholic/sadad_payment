// Global.h

#pragma once

#include "LogEx.h"

// External Variables

extern	BOOL	g_bAppActivated;

extern	CString	g_strMSRData[3];

extern	int		g_nSecureMode;
extern	BYTE	g_cMasterKey[];
extern	BYTE	g_cPrivateKey[];
extern	BYTE	g_cPublicKey[];

extern	WCHAR	g_ISES_CNVT_TBL[];

extern	UINT	WM_TMS_MESSAGE;

extern	CLogEx	g_Log;

// Global Thread Functions

DWORD	DoTransaction(LPVOID lp);
DWORD	DoAnnunciator(LPVOID lp);
