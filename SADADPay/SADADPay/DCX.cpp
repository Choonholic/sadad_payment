// DCX.cpp

#include "StdAfx.h"
#include "DCX.h"

CDCX::CDCX(void)
{
	m_hDC = NULL;
	m_hWnd = NULL;
}

CDCX::~CDCX(void)
{
	if (m_hDC != NULL || m_hWnd != NULL)
	{
		ReleaseDC();
	}
}

void CDCX::GetDC(HWND hWnd)
{
	ReleaseDC();

	m_hWnd = hWnd; 
	m_hDC = ::GetDC(m_hWnd);
}

void CDCX::ReleaseDC()
{
	::ReleaseDC(m_hWnd, m_hDC);
	::DeleteDC(m_hDC);

	m_hDC = NULL;
	m_hWnd = NULL;
}

HDC CDCX::GetSafeHdc()
{
	return m_hDC;
}

BOOL CDCX::CreateCompatibleDC(CDCX* pDC)
{
	this->m_hDC = ::CreateCompatibleDC(pDC->GetSafeHdc());

	return (BOOL)(this->m_hDC != NULL);
}

int CDCX::SaveDC()
{
	return ::SaveDC(m_hDC);
}

BOOL CDCX::RestoreDC(int nSavedDC)
{
	return ::RestoreDC(m_hDC, nSavedDC);
}

CPen* CDCX::SelectObject(CPen* pPen)
{
	::SelectObject(m_hDC, pPen->GetSafeHandle());

	return pPen;
}

CBrush* CDCX::SelectObject(CBrush* pBrush)
{
	::SelectObject(m_hDC, pBrush->GetSafeHandle());

	return pBrush;
}

CFont* CDCX::SelectObject(CFont* pFont)
{
	::SelectObject(m_hDC, pFont->GetSafeHandle());

	return pFont;
}

CBitmap* CDCX::SelectObject(CBitmap* pBitmap)
{
	::SelectObject(m_hDC, pBitmap->GetSafeHandle());

	return pBitmap;
}


COLORREF CDCX::GetBkColor()
{
	return ::GetBkColor(m_hDC);
}

int CDCX::GetBkMode()
{
	return ::GetBkMode(m_hDC);
}

COLORREF CDCX::GetTextColor()
{
	return ::GetTextColor(m_hDC);
}

COLORREF CDCX::SetBkColor(COLORREF crColor)
{
	return ::SetBkColor(m_hDC, crColor);
}

int CDCX::SetBkMode(int nBkMode)
{
	return ::SetBkMode(m_hDC, nBkMode);
}

COLORREF CDCX::SetTextColor(COLORREF crColor)
{
	return ::SetTextColor(m_hDC, crColor);
}

CPoint CDCX::GetCurrentPosition()
{
	CPoint point;

	::GetCurrentPositionEx(m_hDC, &point);

	return point;
}

CPoint CDCX::MoveTo(int x, int y)
{
	CPoint point;

	::MoveToEx(m_hDC, x, y, &point);

	return point;
}

CPoint CDCX::MoveTo(POINT point)
{
	return MoveTo(point.x, point.y);
}

BOOL CDCX::LineTo(int x, int y)
{
	return ::LineTo(m_hDC, x, y);
}

BOOL CDCX::LineTo(POINT point)
{
	return LineTo(point.x, point.y);
}

BOOL CDCX::Polyline(const POINT* lpPoints, int nCount)
{
	return ::Polyline(m_hDC, lpPoints, nCount);
}

BOOL CDCX::DrawIcon(int x, int y, HICON hIcon)
{
	return ::DrawIcon(m_hDC, x, y, hIcon);
}

BOOL CDCX::DrawIcon(POINT point, HICON hIcon)
{
	return ::DrawIcon(m_hDC, point.x, point.y, hIcon);
}

BOOL CDCX::Ellipse(int x1, int y1, int x2, int y2)
{
	return ::Ellipse(m_hDC, x1, y1, x2, y2);
}

BOOL CDCX::Ellipse(LPCRECT lpRect)
{
	return ::Ellipse(m_hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
}

BOOL CDCX::Polygon(const POINT* lpPoints, int nCount)
{
	return ::Polygon(m_hDC, lpPoints, nCount);
}

BOOL CDCX::PolyPolygon(const POINT* lpPoints, INT* lpPolyCounts, int nCount)
{
	return ::PolyPolygon(m_hDC, lpPoints, lpPolyCounts, nCount);
}

BOOL CDCX::Rectangle(int x1, int y1, int x2, int y2)
{
	return ::Rectangle(m_hDC, x1, y1, x2, y2);
}

BOOL CDCX::Rectangle(LPCRECT lpRect)
{
	return ::Rectangle(m_hDC, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top);
}

BOOL CDCX::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return ::RoundRect(m_hDC, x1, y1, x2, y2, x3, y3);
}

BOOL CDCX::RoundRect(LPCRECT lpRect, POINT point)
{
	return ::RoundRect(m_hDC, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, point.x, point.y);
}

BOOL CDCX::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop)
{
	return ::PatBlt(m_hDC, x, y, nWidth, nHeight, dwRop);
}

BOOL CDCX::BitBlt(int x, int y, int nWidth, int nHeight, CDCX* pSrcDC, int xSrc, int ySrc, DWORD dwRop)
{
	return ::BitBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC->GetSafeHdc(), xSrc, ySrc, dwRop);
}

BOOL CDCX::StretchBlt(int x, int y, int nWidth, int nHeight, CDCX* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
{
	return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC->GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}

BOOL CDCX::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths)
{
	return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths);
}

BOOL CDCX::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, CString& str, LPINT lpDxWidths)
{
	return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, str.GetString(), -1, lpDxWidths);
}

int CDCX::DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat)
{
	return ::DrawText(m_hDC, lpszString, nCount, lpRect, nFormat);
}

int CDCX::DrawText(const CString& str, LPRECT lpRect, UINT nFormat)
{
	return ::DrawText(m_hDC, str.GetString(), -1, lpRect, nFormat);
}

void CDCX::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	LOGBRUSH	lbr;
	HBRUSH		br;

	lbr.lbColor	= clr;
	lbr.lbStyle	= BS_SOLID;
	lbr.lbHatch	= NULL;
	br			= CreateBrushIndirect(&lbr);

	::FillRect(m_hDC, lpRect, br);
	DeleteObject(br);
}

void CDCX::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	FillSolidRect(&CRect(x, y, x + cx, y + cy), clr);
}
