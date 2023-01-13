// stdafx.h

#pragma once

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

#pragma warning(disable:4819)

#define WINVER _WIN32_WCE

#define _CRT_SECURE_NO_WARNINGS

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

#define _AFX_ALL_WARNINGS

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#include <afxwin.h>
#include <afxext.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

#include <afxsock.h>

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if (_WIN32_WCE < 0x500) && (defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>
