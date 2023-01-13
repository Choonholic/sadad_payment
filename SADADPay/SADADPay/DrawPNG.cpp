// DrawPNG.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "DrawPNG.h"
#include <imaging.h>
#include <initguid.h>
#include <imgguids.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DrawPNGResource(UINT nResourceID, int left, int top, int width, int height, HDC hDC)
{
	HRSRC	hResInfo	= ::FindResource(NULL, MAKEINTRESOURCE(nResourceID), L"PNG");
    HGLOBAL	hGlobal		= ::LoadResource(NULL, hResInfo);

    if (hGlobal)
    {
        void* dataBlock = LockResource(hGlobal);           

        if (dataBlock)
        {
			int			nLen		= ::SizeofResource(NULL, hResInfo);
            IStream*	pStream		= NULL;
            ULONG		dwReadWrite	= 0;

            CreateStreamOnHGlobal(NULL, TRUE, &pStream);

			pStream->Write(dataBlock, nLen, &dwReadWrite);

            IImagingFactory*	pImgFactory = NULL;
            IImage*				pImage = NULL;

			CoInitializeEx(NULL, COINIT_MULTITHREADED);

            if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory)))
            {
                if (SUCCEEDED(pImgFactory->CreateImageFromBuffer(dataBlock, nLen, BufferDisposalFlagNone, &pImage)))
                {
                    pImage->Draw(hDC, &CRect(left, top, left + width, top + height), NULL);
                    pImage->Release();
                }

                pImgFactory->Release();
            }

			CoUninitialize();

            pStream->Release();

			UnlockResource(hGlobal);
            FreeResource(hGlobal);
        }
    }
}

void DrawPNGImage(LPCTSTR szImageFile, int left, int top, int width, int height, HDC hDC)
{
    IImagingFactory*	pImgFactory = NULL;
    IImage*				pImage = NULL;
   
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory)))
    {
        if (SUCCEEDED(pImgFactory->CreateImageFromFile(szImageFile, &pImage)))
        {
            pImage->Draw(hDC, &CRect(left, top, left + width, top + height), NULL);
            pImage->Release();
        }

        pImgFactory->Release();
    }

	CoUninitialize();
}


HBITMAP GetBitmapFromPNG(LPCTSTR szImageFile, HDC hDC)
{
    IImagingFactory*	pImageFactory = 0;
    IImage*				pImage = 0;
    ImageInfo			imageInfo;

    CoInitializeEx(0, COINIT_MULTITHREADED);

    HBITMAP	hBitmap = 0;
    LPBYTE	lpByte;

    if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImageFactory)))
    {
        if (SUCCEEDED(pImageFactory->CreateImageFromFile(szImageFile, &pImage)) && SUCCEEDED(pImage->GetImageInfo(&imageInfo)))
        {
            HDC			bmpDC	= CreateCompatibleDC(hDC);
			BITMAPINFO*	pbinfo	= (BITMAPINFO*)calloc(1, sizeof(BITMAPINFO) + 4 * sizeof(INT));

			if (!pbinfo)
			{
				return FALSE;
			}

            pbinfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
            pbinfo->bmiHeader.biWidth			= imageInfo.Width;
            pbinfo->bmiHeader.biHeight			= imageInfo.Height * (-1);
            pbinfo->bmiHeader.biPlanes			= 1;
            pbinfo->bmiHeader.biBitCount		= 32;
            pbinfo->bmiHeader.biCompression		= BI_ALPHABITFIELDS;
            pbinfo->bmiHeader.biSizeImage		= 0;
            pbinfo->bmiHeader.biXPelsPerMeter	= 11811;
            pbinfo->bmiHeader.biYPelsPerMeter	= 11811;
            pbinfo->bmiHeader.biClrUsed			= 0;
            pbinfo->bmiHeader.biClrImportant	= 0;

			int*	pMask	= (int*)&(pbinfo->bmiColors[0]);

			*pMask++ = 0x00FF0000;
            *pMask++ = 0x0000FF00;
            *pMask++ = 0x000000FF;
            *pMask++ = 0xFF000000;

            hBitmap = CreateDIBSection(NULL, pbinfo, DIB_RGB_COLORS, (void**)&lpByte, NULL, 0);

			free(pbinfo);

			if ((!hBitmap) || (!lpByte))
			{
				return FALSE;
			}

            RECT			rect			= {0, 0, imageInfo.Width, imageInfo.Height};
            IBitmapImage*	pBitmapImage	= NULL;
            BitmapData		bitmapData;

			bitmapData.Width		= imageInfo.Width;
            bitmapData.Height		= imageInfo.Height;
            bitmapData.PixelFormat	= imageInfo.PixelFormat;

			pImageFactory->CreateBitmapFromImage(pImage, imageInfo.Width, imageInfo.Height, PIXFMT_32BPP_ARGB, InterpolationHintDefault, &pBitmapImage);
            pBitmapImage->LockBits(&rect, ImageLockModeRead, PIXFMT_32BPP_ARGB, &bitmapData);
            memcpy(lpByte, bitmapData.Scan0, imageInfo.Width * imageInfo.Height * 4);
            pBitmapImage->UnlockBits(&bitmapData);
            pBitmapImage->Release();
            pImage->Release();
            DeleteDC(bmpDC);
        }

		pImageFactory->Release();
    }

	CoUninitialize();

	for (UINT y = 0; y < imageInfo.Height; y++)
    {
        BYTE* pPixel	= (BYTE*)lpByte + imageInfo.Width * 4 * y;

        for (UINT x = 0; x < imageInfo.Width; x++)
        {
            pPixel[0] = pPixel[0] * pPixel[3] / 255;
            pPixel[1] = pPixel[1] * pPixel[3] / 255;
            pPixel[2] = pPixel[2] * pPixel[3] / 255;
            pPixel += 4;
        }
    }

    return hBitmap;
}
