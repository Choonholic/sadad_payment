// Splash.h

#pragma once

class CSplash
{
public:
	CSplash();
	virtual	~CSplash();

	void	Initialize(HINSTANCE hResource, UINT nID);

	void	Show();
	void	Hide();

private:
    BOOL	m_bShowStatus;
	HWND	m_hWnd;    
};
