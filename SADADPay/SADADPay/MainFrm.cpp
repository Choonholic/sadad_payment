// MainFrm.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD dwAdornmentFlags = 0;

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

#if USE_SADADPAY_CLASS
	// Check if our class is already defined
	LPCTSTR		lpszClassName	= SADADPAY_CLASS;
	WNDCLASS	wndClass;

	if (!::GetClassInfo(AfxGetInstanceHandle(), lpszClassName, &wndClass))
	{
		// Get the current requested window class
		VERIFY(GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndClass));

		// We want to register this info with our name
		wndClass.lpszClassName	= lpszClassName;

		// Need to preset the icon otherwise the function GetIconWndClass calling us will overwrite our class
		LPCTSTR		lpszIcon	= MAKEINTRESOURCE(IDR_MAINFRAME);
		HINSTANCE	hInstance	= AfxFindResourceHandle(lpszIcon, ATL_RT_GROUP_ICON);

		_ASSERTE(hInstance != NULL);

		wndClass.hIcon	= ::LoadIcon(hInstance, lpszIcon);

		// Register our class now and check the outcome
		if (!::RegisterClass(&wndClass))
        {
			_ASSERTE(!__FUNCTION__ "RegisterClass Failed");
			return FALSE;
		}
	}

    // Now use our class 
    cs.lpszClass	= lpszClassName;
#else
	cs.lpszClass	= AfxRegisterWndClass(0);
#endif

	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}
#endif

void CMainFrame::OnSetFocus(CWnd*)
{
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if ((nState == WA_INACTIVE) && (g_bAppActivated))
	{
		SetActiveWindow();
	}
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message >= 0xC000) && (pMsg->message <= 0xFFFF))
	{
		m_wndView.PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
