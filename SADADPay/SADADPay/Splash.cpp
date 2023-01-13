// Splash.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "Splash.h"

CSplash::CSplash()
{
}

CSplash::~CSplash()
{
	DestroyWindow(m_hWnd);
}

void CSplash::Initialize(HINSTANCE hInstance, UINT nID)
{
	m_hWnd	= CreateWindowEx(WS_EX_CLIENTEDGE, L"STATIC", L"", WS_POPUP | WS_DLGFRAME | SS_BITMAP, -2, -2, GetSystemMetrics(SM_CXSCREEN) + 4, GetSystemMetrics(SM_CYSCREEN) + 4, NULL, NULL, hInstance, NULL);

	SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInstance, MAKEINTRESOURCE(nID)));

	m_bShowStatus	= FALSE;
}

void CSplash::Show()
{
	MoveWindow(m_hWnd, -2, -2, GetSystemMetrics(SM_CXSCREEN) + 4, GetSystemMetrics(SM_CYSCREEN) + 4, TRUE);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	m_bShowStatus	= TRUE;
}

void CSplash::Hide()
{
	ShowWindow(m_hWnd, SW_HIDE);

	m_bShowStatus	= FALSE;
}
