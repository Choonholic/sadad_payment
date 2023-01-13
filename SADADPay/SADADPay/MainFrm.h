// MainFrm.h

#pragma once

#include "ChildView.h"

#define USE_SADADPAY_CLASS	TRUE
#define SADADPAY_CLASS		L"SADADPay"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	virtual	BOOL	PreCreateWindow(CREATESTRUCT& cs);
	virtual	BOOL	OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual	BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg void	OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void	OnDestroy();

#ifdef _DEBUG
	virtual	void	AssertValid()	const;
#endif

protected: 
	DECLARE_DYNAMIC(CMainFrame)

	CChildView	m_wndView;

	afx_msg	int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg	void	OnSetFocus(CWnd* pOldWnd);
	
	DECLARE_MESSAGE_MAP()
};

