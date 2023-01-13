// DrawPNG.h

#pragma once

void	DrawPNGResource(UINT nResourceID, int left, int top, int width, int height, HDC hDC);
void	DrawPNGImage(LPCTSTR szImageFile, int left, int top, int width, int height, HDC hDC);

HBITMAP	GetBitmapFromPNG(LPCTSTR szImageFile, HDC hDC);
