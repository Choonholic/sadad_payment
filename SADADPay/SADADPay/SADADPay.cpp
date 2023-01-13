// SADADPay.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "MainFrm.h"
#include "Splash.h"
#include "Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSADADPayApp, CWinApp)
END_MESSAGE_MAP()

CSADADPayApp::CSADADPayApp() : CWinApp()
{
}

CSADADPayApp theApp;

BOOL CSADADPayApp::InitInstance()
{
	CSplash	splash;
	WCHAR	szModel[64];

	GetModel(szModel, 64);

	if (wcscmp(szModel, MODEL_NAME))
	{
		AfxMessageBox(IDS_INVALID_MODEL, MB_ICONERROR | MB_OK);
		return FALSE;
	}

	splash.Initialize(AfxGetInstanceHandle(), IDB_SPLASH);
	splash.Show();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	
	SetRegistryKey(_T("Payment Solution for SADAD Informatic Corporation"));

	CMainFrame* pFrame = new CMainFrame;

	if (!pFrame)
	{
		return FALSE;
	}

	m_pMainWnd = pFrame;

	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}
