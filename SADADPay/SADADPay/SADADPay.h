// SADADPay.h

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Definitions.h"
#include "Global.h"

class CSADADPayApp : public CWinApp
{
public:
	CSADADPayApp();

	virtual BOOL InitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CSADADPayApp theApp;
