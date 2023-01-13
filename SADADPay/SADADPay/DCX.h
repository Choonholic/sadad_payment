// DCX.h

#pragma once

class CDCX
{
public:
	CDCX(void);
	~CDCX(void);

	void GetDC(HWND hWnd);
	void ReleaseDC();

	HDC GetSafeHdc();

	BOOL CreateCompatibleDC(CDCX* pDC);

	int SaveDC();
	BOOL RestoreDC(int nSavedDC);

	CPen* SelectObject(CPen* pPen);
	CBrush* SelectObject(CBrush* pBrush);
	CFont* SelectObject(CFont* pFont);
	CBitmap* SelectObject(CBitmap* pBitmap);

	COLORREF GetBkColor();
	int GetBkMode();
	COLORREF GetTextColor();
	COLORREF SetBkColor(COLORREF crColor);
	int SetBkMode(int nBkMode);
	COLORREF SetTextColor(COLORREF crColor);

	CPoint GetCurrentPosition();
	CPoint MoveTo(int x, int y);
	CPoint MoveTo(POINT point);
	BOOL LineTo(int x, int y);
	BOOL LineTo(POINT point);
	BOOL Polyline(const POINT* lpPoints, int nCount);

	BOOL DrawIcon(int x, int y, HICON hIcon);
	BOOL DrawIcon(POINT point, HICON hIcon);
	BOOL Ellipse(int x1, int y1, int x2, int y2);
	BOOL Ellipse(LPCRECT lpRect);
	BOOL Polygon(const POINT* lpPoints, int nCount);	
	BOOL PolyPolygon(const POINT* lpPoints, INT* lpPolyCounts, int nCount);
	BOOL Rectangle(int x1, int y1, int x2, int y2);
	BOOL Rectangle(LPCRECT lpRect);
	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
	BOOL RoundRect(LPCRECT lpRect, POINT point);

	BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
	BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDCX* pSrcDC, int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDCX* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);

	BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths);
	BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, CString& str, LPINT lpDxWidths);

	int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat);
	int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);

	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);

private:
	HDC		m_hDC;
	HWND	m_hWnd;
};
