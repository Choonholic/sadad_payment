#include <Windows.h>
#include "s3c2450_ioport.h"
#define IOCTL_DO_VIRTUALCOPYEX 0x0926



typedef struct __VIRTUAL_COPY_DESC__
{
	PVOID	pDest;
	PVOID	pSrc;
	DWORD	dwSize;
	DWORD	dwProtect;
}
VIRTUAL_COPY_DESC;



static BOOL BBVirtualCopy( LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect )
{
	BOOL bRes = FALSE;
	
	if( lpvDest == NULL ||
		lpvSrc == NULL	||
		cbSize == 0 )
	{
		RETAILMSG(1,(TEXT("BBVirtualCopy : Invalid Parameter\r\n")));
	}
	else
	{
		HANDLE				hVirtualCopyHelper;
		VIRTUAL_COPY_DESC	desc;
					
		desc.pDest		= lpvDest;
		desc.pSrc		= lpvSrc;
		desc.dwSize		= cbSize;
		desc.dwProtect	= fdwProtect;
		
		hVirtualCopyHelper = CreateFile( _T("MEM0:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
		if( hVirtualCopyHelper != INVALID_HANDLE_VALUE )
		{
			if( TRUE == DeviceIoControl( hVirtualCopyHelper, IOCTL_DO_VIRTUALCOPYEX, &desc, sizeof(desc), NULL, 0, NULL, NULL ) )
			{
				bRes = TRUE;
			}
			else
			{
				RETAILMSG(1,(TEXT("BBVirtualCopy : DeviceIoControl failed\r\n")));
			}
		}
		else
		{
			RETAILMSG(1,(TEXT("BBVirtualCopy : CreateFile failed\r\n")));
		}
		
		CloseHandle( hVirtualCopyHelper );
	}
	
	return bRes;
}
