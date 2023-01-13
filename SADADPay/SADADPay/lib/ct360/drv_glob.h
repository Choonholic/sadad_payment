//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/* Copyright ?1999-2000 Intel Corp.  */
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995, 1996, 1997  Microsoft Corporation

Module Name:

    drv_glob.h

Abstract:

    This file provides the C definitions for the driver globals area of
    shared memory, used to coordinate between ISRs and ISTs.

--*/
#ifndef _DRV_GLOB_H
#define _DRV_GLOB_H

//#include <image_cfg.h>      // This is S3C2440 Memory Map Informaiton...

#define PAD(label,amt)  UCHAR Pad##label[amt]

// Make sure these match up with the defs in config.bib, and drv_glob.inc
#define DRIVER_GLOBALS_MEMORY_UA_START		(0xA00FF000)
#define DRIVER_GLOBALS_MEMORY_CA_START		(0x800FF000)
#define DRIVER_GLOBALS_MEMORY_PA_START		(0x300FF000)

#define DRIVER_GLOBALS_MEMORY_SIZE			(0x1000)

// In OEMInit, we zero out the region specified by the following defs
#define DRIVER_GLOBALS_ZEROINIT_SIZE	DRIVER_GLOBALS_MEMORY_SIZE

#define BOOT_DEVICE_NONE	0
#define BOOT_DEVICE_SLOT0	1
#define BOOT_DEVICE_SLOT1	2
#define BOOT_DEVICE_SMC		3


//
// Constant definitions
//  for UNINIT_MISC_GLOBALS.offButton
#define DRVG_OFFBTN_INACTIVE      0
#define DRVG_OFFBTN_PWRBUTTON     1
#define DRVG_OFFBTN_SUSPEND       2
#define DRVG_OFFBTN_SOFT_RESET    3

#define ULDR_MENUMODE_NONE						0x0000
#define ULDR_MENUMODE_USBDOWN					0x0001
#define ULDR_MENUMODE_USERSTOREDOWN				0x0002
#define ULDR_MENUMODE_MENU						0x0003
#define ULDR_MENUMODE_ALLDOWNLOADSD				0x0004
#define ULDR_MENUMODE_ALLDOWNLOADSD_FORMATTED	0x0005
#define ULDR_MENUMODE_ALLDOWNLOAD_USB_MEMORY				0x0006
#define ULDR_MENUMODE_ALLDOWNLOAD_USB_MEMORY_FORMATTED		0x0007
#define ULDR_MENUMODE_ALLDOWNLOAD_USER_STORAGE				0x0008
#define ULDR_MENUMODE_ALLDOWNLOAD_USER_STORAGE_FORMATTED	0x0009

#define ULDR_MENUMODE_LAUNCH			(0x0001 << 16)

#define GOTO_NANDFORMAT		0x52455041
#define GOTO_ULDR			0x554C4452
#define GOTO_ULDRALLSD		0x554C5344
#define APPLY_HARDRESET		0x48415244
#define GOTO_ULDRALLUSBMEMORY		0x554C554D
#define GOTO_ULDRALLUSERSTORAGE		0x554C5553


#pragma pack(4)	// 4 byte alignment
// Un-initialized Miscellaneous
// Modified by gtkim@bluebird.co.kr
//struct BOOTLOADER_GLOBALS size 48 (0x30) byte
typedef struct _UNINIT_MISC_GLOBALS {
	DWORD	DID0;                   /* 4byte */     // DID0 : Device ID read from the StrataFlash
	DWORD	DID1;                   /* 4byte */     // DID1 : Device ID read from the StrataFlash
	DWORD	DID2;                   /* 4byte */     // DID2 : Device MAC ID for otp register
	DWORD	DID3;                   /* 4byte */     // DID3 : Device MAC ID for otp register
    UCHAR   ucDIDSct;               /* 1byte */
    UCHAR   ucSerialNumberSct;      /* 1byte */
    UINT8   PAD[2];                 /*2byte*/       // NULL Pad... gtkim@bluebird.co.kr
    UCHAR   ucSerialNumber[16];     /* 16byte */
    UINT8   PAD1[12];               /*12byte*/       // NULL Pad... gtkim@bluebird.co.kr
} UNINIT_MISC_GLOBALS, *PUNINIT_MISC_GLOBALS;

// Modified by gtkim@bluebird.co.kr
//struct BOOTLOADER_GLOBALS size 80 (0x50) byte
typedef struct _BOOTLOADER_GLOBALS
{
    DWORD   MagicNum;                               /* 4byte */     //MagicNum���� �� ���� ���ǰ� �ִ����� �˾Ƴ���.
#define BOOTLOADER_MAGIC_NUM 0x424F4F54                         //"BOOT"
    DWORD   SchemeVersion;                          /* 4byte */     //�� struct ������ ������ ��Ÿ����.
#define BOOTLOADER_SCHEME_VERSION 0x00000001	                //���� 1
    DWORD   LastLaunchAddrValid;                    /* 4byte */
    DWORD   LastLaunchAddr;                         /* 4byte */
    DWORD   ImageStart;                             /* 4byte */
    DWORD   ImageLength;                            /* 4byte */
    DWORD   ImageChecksum;                          /* 4byte */
    char    *VersionInfo;		                    /* 4byte */     // Not Used.
    char    *CustomInfo;                            /* 4byte */
#define BOOTLOADER_UPDATE_MAGIC_NUM 0x5550494D //"UPIM"
	DWORD   dwUpgradeMagicKey;                      /* 4byte */
	DWORD   dwUpgradeImageStartAddr;                /* 4byte */
	DWORD   dwUpgradeImageLen;                      /* 4byte */
	DWORD   dwUpgradeImageType;                     /* 4byte */
    char    BL2Version[10];	                        /* 10byte */    // STEPLDR Version
    char    IPLVersion[10];                         /* 10byte */    // 2NDBL Version
    UINT16  dummy;                                  /* 2byte */
    UINT16  uPSSizeMB;                              /* 2byte */     // ULDR���� ������ PS ������	- Not Used in NERF
	char   FirmwareVersion[3];                      /*3byte*/	
	UINT8   PAD[1];                                 /*1byte*/
} BOOTLOADER_GLOBALS, *PBOOTLOADER_GLOBALS;


// Modified by gtkim@bluebird.co.kr
//struct BOOTLOADER_GLOBALS size 48 (0x30) byte
typedef struct _BOOT_FLAG_GLOBALS
{
	// �����Ǹ� ULDR �̹����� Launch �Ѵ�. ULDR ���� Clear �� �ش�.
	DWORD	dwImageUpdateMode;                      /* 4byte */     //set TRUE when ULDR mode
	// �ǹ� ����
	DWORD	dwSystemHiveCleanFlag;                  /* 4byte */     //set TRUE when Clean System hive reg
	// �ǹ� ����
	DWORD	dwUserHiveCleanFlag;                    /* 4byte */     //set TRUE when Clean User hive reg
	// Boot���� ������ Clear �ϰ� ���Ͻý��� ��Ƽ�� ���� �� Set �� �ش�.
	DWORD	dwCleanUserStorage;                     /* 4byte */     //set TRUE when clean user storage area
	// ULDR �� �����ؼ� ������ ���� �����Ѵ�.  ULDR ���� Clear �� �ش�.
	DWORD	dwULDRMenuMode;                         /* 4byte */
	// ULDR �� �ٿ�ε� ���� �̹��� ����
	DWORD	dwUpgradeByUsbMode;                     /* 4byte */     //set Number of Images for upgrade.
	// ������Ʈ �� ��Ʈ����  �ϵ帮�� ( �ʱ�ȭ ) �ϵ��� �Ѵ�.
	DWORD	dwIsHardResetByApp;                     /* 4byte */     //set Value 0x48415244 when Hard Reset
	// ���ý� ULDR �� �����ϱ� �ϵ��� �Ѵ�.
	DWORD	dwGotoULDR;                             /* 4byte */
	DWORD	dwGotoNandFormatByULDR;                 /* 4byte */
	DWORD	dwFastProductOption;	                /* 4byte */		// 0 - FastProduct none     1 - FastProduct normal       2- FastProduct without ROMST
	DWORD	dwEnableTemper;							/* 4Byte */		// 0 - Disable				1 - Enable
#if (PCB_MT760_NEWBOARD == 1) // new board
	DWORD dwBoardRev;								/* 4Byte */	  // 0 - Board Rev0.1~1.1        1 - Board Rev 1.2 ~    
#else 
    UINT8   PAD[4];                                /*4byte*/
#endif  // PCB_MT760_REV
} BOOT_FLAG_GLOBALS, *PBOOT_FLAG_GLOBALS;


#define MAXQ_PASSWORD_LEN	8
#define MAXQ_CODEKEY_LEN	24
#define MAXQ_SIGNATURE_LEN	32
#define MAXQ_VERSION_LEN	30


typedef struct _FastProductData_
{
	BYTE	aPassword[MAXQ_PASSWORD_LEN];
	
	BYTE	aFSK[MAXQ_CODEKEY_LEN];
	BYTE	aLSK[MAXQ_CODEKEY_LEN];
	BYTE	aBSK[MAXQ_CODEKEY_LEN];
	BYTE	aUSK[MAXQ_CODEKEY_LEN];
	BYTE	aKSK[MAXQ_CODEKEY_LEN];
	
	BYTE	aLoaderSignature[MAXQ_SIGNATURE_LEN];
	BYTE	aLoaderVersion[MAXQ_VERSION_LEN];
	BYTE	aMOBMSignature[MAXQ_SIGNATURE_LEN];
	BYTE	aMOBMVersion[MAXQ_VERSION_LEN];
	BYTE	aULDRSignature[MAXQ_SIGNATURE_LEN];
	BYTE	aULDRVersion[MAXQ_VERSION_LEN];
	BYTE	aKernelSignature[MAXQ_SIGNATURE_LEN];
	BYTE	aKernelVersion[MAXQ_VERSION_LEN];
}
FastProductData;



// Total 176(0xB0) byte..
typedef struct _DRIVER_GLOBALS
{
    UNINIT_MISC_GLOBALS     uninit_misc;    //  0x30 byte
    BOOT_FLAG_GLOBALS       bootflag;       //  0x30 byte
    BOOTLOADER_GLOBALS      boot;           //  0x50 byte
    UINT		MAXQ_UseFlag;				// 4Byte
	UINT		bcrShadow[4];
	
    UINT		StepldrVersion[3];
    UINT		EBOOTVersion[3];	// ULDR ����.   Major, Minor, BuildNumber
#if 1	// add by yjs1212. 2011.09.21. Boot Version
    UCHAR		HardReset;	// 0 : softreset, 1:hardreset
#endif
	
#if 1	
// �ΰ��� NAND Driver ���� ���ÿ� NAND �� ���ٽ� ���ʿ��� �̻��� �����͸� �޴� ��찡 �߻���, 
// ���� ������ ���� ���� �߰�.
    UCHAR		NandSMP;	// 0 : no use, bit 1 set(0x01) : system, bit 2 set(0x02):pocketstore, bit 3 set(0x04):toolBlackbox
	FastProductData			fastProductData;	//376Byte, 
	UINT8	PAD[3520];
#else
	UINT8	PAD[3897];
#endif
} DRIVER_GLOBALS, *PDRIVER_GLOBALS;
// ��ü ������ 4120, 0x1018

#pragma pack()

// Prototype functions from drvlib
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif


#endif // _DRV_GLOB_H
