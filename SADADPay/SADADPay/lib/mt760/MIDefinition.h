
#pragma once

#include <Windows.h>

/*
#include "s3c2450_lcd.h"
#include "s3c2450_ioport.h"
#include "s3c2450_intr.h"
#include "s3c2450_clkpwr.h"
#include "drv_glob.h" 
*/

#pragma warning(disable:4200)

#define INVILAD_VALUE		0xFFFFFFFF
#define MAGIC_NUMBER		{ 0x42, 0x30, 0x30, 0x30, 0x46, 0x46, 0x0A }
#define DOWN_MODELINFO		0x83F00000
#define END_POINT			0xFFFFFFFF

#define DISPLAY_STANDARD TRUE
#define DISPLAY_REVERSE  FALSE



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MS BIN file header
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct __binHeader
{
	BYTE	magic[7];
	DWORD	address;
	DWORD	fileSize;
}BinFileHeader;
#pragma pack(pop)

typedef struct __RecordHeader__
{
	DWORD	dwRecAddr;
	DWORD	dwRecLen;
	DWORD	dwRecChk;
}RecordHeader;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MI Structure
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __modelHeader
{
	WCHAR	modelInfoVer[12];	// ModelInfo 버젼.
	WCHAR	modelInfoID[12]; 	// 품목 정보( a, b, ac ...등등)	// 두개 합쳐 24 +24 = 48
	DWORD	ioAddressOffset;	// IO & ADDRESS 시작 주소	
	DWORD	regOffset;			// Registery 시작 주소.	// 48 + 8 = 56
	DWORD	iplLogoOffset;		// 56 + 4 = 60
	DWORD	TableItemCount;		// 60 + 4 = 64
	DWORD	PinCount;			// 64 + 4 = 68
	DWORD	CheckSum;			// 68 + 4 = 72
	BYTE	pack[28];			// 사이즈 100BYte 채운다.
}ModelHeader;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FeatureDesc Structure
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct __FeatureDesc__
{
	WORD			wSizeOfElement;
	WORD			wCount;
	unsigned char	aData[];
}FeatureDesc;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FeatureDesc Structure
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _RegDesc__
{
	DWORD dwCommonRegLength;
	DWORD dwLanguageCount;
	struct _Rihanna
	{
		DWORD dwLangCode;
		DWORD dwRegSize;
		DWORD dwOffsetFromRegDesc;
	}langOfReg[30];
}RegDesc;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MI Feature ID
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! Feature Management Rule !
//   a. Feature는 적으면 적을수록 좋다는 것을 잊지말자.. Feature가 늘어날 수록 우리의 관리비용도 늘어나게 된다
//      - 의존적인것은 생략하자.. DPI, VGA, RESH 등의 값은 서로 의존적이다
//        이런 의존적인 feature들은 하나로 통합하고 BSP가 하나의 feature만 알아도 나머지 값을 구해서 동작할 수 있도록 수정해야 한다
//
//   b. 추가할 때는 미래를 생각하되, 미래에 쓸 것을 미리 추가하지는 말아야 한다
//      - 최대한 일반화 하자.. IsVGA? IsQVGA 이렇게 만들필요가 있나? DPI하나면 족하다.. boolean 형식의 feature를 추가하려 한다면
//        미래의 용도를 한번 더 생각해보자
//        드라이버글로벌 변수중에는 특정모듈이름이 들어가는 경우도  있는데 매우 좋지 않다
//      - 어떤 기능을 쓰는지와 그 기능을 씀으로 해서 필요한 값을 둘다 feature로 만들 필요는 없다
//        그냥 필요한 값이 있으면 그 기능을 쓰는거다 이러한 원칙으로 feature를 추가하고 bsp는 그에 맞게 돌아갈 수 있어야 한다
//   c. 반드시 prefix 규칙을 지킬것
//      - 규정을 따로 정리해 놓지는 않았다 그냥 이미 추가된것들에 위화감이 없도록 추가하자
//

enum
{
	// System
	MI_FID_SYSTEM_MODEL_MAJOR				= 500,
	MI_FID_SYSTEM_MODEL_MINOR,
	MI_FID_SYSTEM_MODEL_TYPE,
	MI_FID_SYSTEM_RAMSIZE,
	MI_FID_SYSTEM_ROMSIZE,
	MI_FID_SYSTEM_PROGSTORE_SIZE,
	MI_FID_SYSTEM_PROGSTORE_MIN_SIZE,
	MI_FID_SYSTEM_GPIO_INITVALUE,
	MI_FID_SYSTEM_MFPR_INITVALUE,
	
	MI_FID_SYSTEM_LP,
	MI_FID_SYSTEM_DEBUGUART_BAUD			= 600,
	MI_FID_SYSTEM_DEBUGUART_CH,
    MI_FID_SYSTEM_SWITCH_CIRCUIT_INFO		= 700,
    MI_FID_SYSTEM_USERIDLE,
	
    // USB
	MI_FID_USBSWITCH						= 1000,
	
	// Display
	MI_FID_DISPLAY_DPI						= 1500,
	MI_FID_DISPLAY_RESH,
	MI_FID_DISPLAY_RESV,
	MI_FID_DISPLAY_MODULE,
	MI_FID_DISPLAY_LOTATE,
	
	// Keypad
	MI_FID_KEY_MODULE						= 2000,
	MI_FID_KEY_LANG,
	MI_FID_KEY_INFO,
	MI_FID_KEY_INFO_LONG_N_SHORT,
	MI_FID_KEY_INFO_DOUBLE,
	MI_FID_KEY_INFO_ADDKEY,
	MI_FID_KEY_MATRIX_SCANCODE				= 2100,
	MI_FID_KEY_MATRIX_COLS,
	MI_FID_KEY_MATRIX_ROWS,
	MI_FID_KEY_DIRECT_SCANCODE,
	MI_FID_KEY_WAKEUP_EDGE_PHONE,
	MI_FID_KEY_WAKEUP_EDGE_NONPHONE,
	MI_FID_KEY_ULDR_KEYINFO,
	MI_FID_KEY_BOOTMODE_KEYINFO,
	MI_FID_KEY_MULTIKEY_FUNC_KEYINFO,
	MI_FID_KEY_AD3ER,
    MI_FID_KEY_USE_XMLMAP					= 2200,
    MI_FID_KEY_CHR_CERT,
	MI_FID_KEY_IMHW_INFO_NONKOR				= 2300,
	MI_FID_KEY_IMHW_INFO_KOR,
	
	// Modem
	MI_FID_MODEM_MODULE						= 3000,
	
	// Barcode
	MI_FID_BARCODE_MODULE					= 3500,
	
	// Camera
	MI_FID_CAMERA_MODULE					= 4000,
	
	// RFID
	MI_FID_RFID_MODULE						= 4500,
	
	// GPS
	MI_FID_GPS_MODULE						= 5000,
	
	// WiFi
	MI_FID_WIFI_MODULE						= 5500,

	// BATT
	MI_FID_BATT_MODULE						= 6000,
	MI_FID_BATT_AC_TABLEINFO				= 6001,
	MI_FID_BATT_NOAC_TABLEINFO				= 6002,

	// Bluetooth
	MI_FID_BLUETOOTH_MOUDLE					= 6500,

	// Contact
	MI_FID_ICCARD_MODULE					= 7000,
	
	// MSR
	MI_FID_MSR_MODULE						= 7500,
	
	// Printer
	MI_FID_PRINTER_MODULE					= 8000,
	
	// PSTN
	MI_FID_PSTN_MODULE						= 8500,
	
	// Ethernet
	MI_FID_ETHERNET_MODULE					= 9000,
	
	// FingerPrinter
	MI_FID_FINGERPRINT_MODULE				= 9500,
	
	// Option
	MI_OPT_ULDR_PASSWD						= 100000,
	
	// CustomerLCD
	MI_FID_CUSTOMERLCD_MODULE				= 100500,
	
	// Backlight
	MI_FID_BKL_BRIGHTNESS_TABLEINFO			= 101000,
	
	// Fiscal
	MI_FID_FISCAL_MODULE					= 101500,
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MI Feature Option
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MI_FID_SYSTEM_MODEL_MAJOR
enum
{
	MI_MODEL_NONE,
	MI_MODEL_AUTO,
	
	MI_MODEL_CT360							= 0x00010001,
	
	MI_MODEL_MT760							= 0x00020001,
	MI_MODEL_MT760S							= 0x00020002,
	
	MI_MODEL_MT360							= 0x00030001,
	MI_MODEL_MT360B							= 0x00030002,
	MI_MODEL_MT360E							= 0x00030003,
	
	MI_MODEL_MTCRADLE						= 0x00090001,
	
	// 최초 리팩토링을 개발했을때는 지금과 모델명이 달랐었다.
	// 13년말에는 MT360E 최초생산단말기에 이런 MI가 들어있어서 예외처리를 해줘야했었지만, 이젠 필요없어서 이력만 남기고 지웠다. 
	//MI_MODEL_MT360E_OLD_IMAGE				= 0x000B0002, 
};


// MI_FID_SYSTEM_MODEL_TYPE
enum
{
	MI_TYPE_CT360_NONE,
	MI_TYPE_CT360_AUTO,
	MI_TYPE_CT360_A,
	MI_TYPE_CT360_B,
	MI_TYPE_CT360_C,
	MI_TYPE_CT360_D,
	MI_TYPE_CT360_E,
	MI_TYPE_CT360_F,
	MI_TYPE_CT360_G,
	MI_TYPE_CT360_H,
	MI_TYPE_CT360_I,
	MI_TYPE_CT360_J,
	MI_TYPE_CT360_K,
	MI_TYPE_CT360_L,
	MI_TYPE_CT360_M,
	MI_TYPE_CT360_N,
};

enum
{
	MI_TYPE_MT760_NONE,
	MI_TYPE_MT760_AUTO,
	MI_TYPE_MT760_A,
	MI_TYPE_MT760_B,
	MI_TYPE_MT760_C,
	MI_TYPE_MT760_D,
	MI_TYPE_MT760_E,
	MI_TYPE_MT760_1,
	MI_TYPE_MT760_SA,
};

enum
{
	MI_TYPE_MT760S_NONE,
	MI_TYPE_MT760S_AUTO,
	MI_TYPE_MT760S_A,
	MI_TYPE_MT760S_B,
};

enum
{
	MI_TYPE_MT360_NONE,
	MI_TYPE_MT360_AUTO,
	MI_TYPE_MT360_A,
	MI_TYPE_MT360_B,
	MI_TYPE_MT360_C,
	MI_TYPE_MT360_D,
	MI_TYPE_MT360_E,
	MI_TYPE_MT360_F,
	MI_TYPE_MT360_G,
	MI_TYPE_MT360_H,
	MI_TYPE_MT360_J,
	MI_TYPE_MT360_K,
	MI_TYPE_MT360_L,
	MI_TYPE_MT360_M,
	MI_TYPE_MT360_N,
	MI_TYPE_MT360_O,
	MI_TYPE_MT360_SA,
};

enum
{
	MI_TYPE_MT360B_NONE,
	MI_TYPE_MT360B_AUTO,
	MI_TYPE_MT360B_A,
	MI_TYPE_MT360B_B,
	MI_TYPE_MT360B_C,
	MI_TYPE_MT360B_D,
	MI_TYPE_MT360B_E,
	MI_TYPE_MT360B_F,
	MI_TYPE_MT360B_G,
};

enum
{
	MI_TYPE_MT360E_NONE,
	MI_TYPE_MT360E_AUTO,
	MI_TYPE_MT360E_A,
	MI_TYPE_MT360E_B,
};

enum
{
	MI_TYPE_MTCRADLE_NONE,
	MI_TYPE_MTCRADLE_AUTO,
	MI_TYPE_MTCRADLE_A,
	MI_TYPE_MTCRADLE_B,
	MI_TYPE_MTCRADLE_C,
};


// MI_FID_MODEM_MODULE
enum
{
	MI_MODEM_MODULE_NONE,
	MI_MODEM_MODULE_AUTO,
	MI_MODEM_MODULE_CINTERION_MC75i				= 100,	// MODEM_MC75i,
	MI_MODEM_MODULE_CINTERION_3G,						// MODEM_HC25 & PHS-8,
	MI_MODEM_MODULE_CINTERION_3G_PELEPHONE,
	MI_MODEM_MODULE_CINTERION_3G_EHS5,
	MI_MODEM_MODULE_CINTERION_2G_BGS2,
	
	MI_MODEM_MODULE_SIERRA_Q2687				= 200,	// MODEM_Q2687,
	
	MI_MODEM_MODULE_TELIT_UC864_SKT				= 300,	// MODEM_UC864_SKT,
	MI_MODEM_MODULE_TELIT_UC864_KTF,					// MODEM_UC864_KTF,
	MI_MODEM_MODULE_TELIT_UC864_WORLD,					// MODEM_UC864_WORLD,
	
	MI_MODEM_MODULE_TELIT_GC864					= 320,	// MODEM_GC864,
	
	MI_MODEM_MODULE_AMTELECOM_AME200_LGT		= 400,	// MODEM_AME200_LGT,
	
	MI_MODEM_MODULE_KDDI						= 500,
};


// MI_FID_BARCODE_MODULE
enum
{
	MI_BARCODE_MODULE_NONE,
	MI_BARCODE_MODULE_AUTO,
	MI_BARCODE_MODULE_SE950						= 100,	
	MI_BARCODE_MODULE_IT5000,
	MI_BARCODE_MODULE_SE955,
	MI_BARCODE_MODULE_SE380,
	MI_BARCODE_MODULE_SE4500,
};


// MI_FID_CAMERA_MODULE
enum
{
	MI_CAMERA_MODULE_NONE,
	MI_CAMERA_MODULE_AUTO,
	MI_CAMERA_MODULE_MT9T112					= 100,
};


// MI_FID_RFID_MODULE
enum
{
	MI_RFID_MODULE_NONE,
	MI_RFID_MODULE_AUTO,
	MI_RFID_MODULE								= 100,
	MI_RFID_MODULE_LF,
	MI_RFID_MODULE_UHF_KOREA,
	MI_RFID_MODULE_UHF_JAPAN,
	MI_RFID_MODULE_UHF_EURO,
	MI_RFID_MODULE_UHF_USA,
	MI_RFID_MODULE_UHF_CHINA,
	MI_RFID_MODULE_UHF_TAIWAN,
	MI_RFID_MODULE_UHF_BRAZIL,
	MI_RFID_MODULE_UHF_MALAYSIA,
	MI_RFID_MODULE_UHF_ASIA,
};


// MI_FID_GPS_MODULE
enum
{
	MI_GPS_MODULE_NONE,
	MI_GPS_MODULE_AUTO,
	MI_GPS_MODULE								= 100,
};


// MI_FID_WIFI_MODULE
enum
{
	MI_WIFI_MODULE_NONE,
	MI_WIFI_MODULE_AUTO,
	MI_WIFI_MODULE_MMC_CM6060G					= 100,
	
	MI_WIFI_MODULE_SUMMIT_SD30AG				= 200,
};


// MI_FID_BATT_MODULE
enum
{
	MI_BATT_MODULE_NONE,
	MI_BATT_MODULE_AUTO,
	MI_BATT_MODULE_CT360						= 100,
	MI_BATT_MODULE_MT760						= 200,
};


// MI_FID_BLUETOOTH_MOUDLE
enum
{
	MI_BLUETOOTH_MODULE_NONE,
	MI_BLUETOOTH_MODULE_AUTO,
};


// MI_FID_ICCARD_MODULE
enum
{
	MI_ICCARD_MODULE_NONE,
	MI_ICCARD_MODULE_AUTO,
};


// MI_FID_MSR_MODULE
enum
{
	MI_MSR_MODULE_NONE,
	MI_MSR_MODULE_AUTO,
	MI_MSR_MODULE_SIMTECK						= 100,
	MI_MSR_MODULE_MAGTECK,
};


// MI_FID_PRINTER_MODULE
enum
{
	MI_PRINTER_MODULE_NONE,
	MI_PRINTER_MODULE_AUTO,
	MI_PRINTER_MODULE_608MCL					= 100,
	MI_PRINTER_MODULE_LPT02						= 200,
};


// MI_FID_PSTN_MODULE
enum
{
	MI_PSTN_MODULE_NONE,
	MI_PSTN_MODULE_AUTO,
};

// MI_FID_ETHERNET_MODULE
enum
{
	MI_ETHERNET_MODULE_NONE,
	MI_ETHERNET_MODULE_AUTO,
};


// MI_FID_FINGERPRINTER_MODULE
enum
{
	MI_FINGERPRINT_MODULE_NONE,
	MI_FINGERPRINT_MODULE_AUTO,
};


// MaxQ Module
enum
{
	MI_USB_MODULE_AUTO,
	MI_USB_MODULE_NONE,
	MI_LED_MODULE_NONE,
	MI_UART_MODULE_NONE,
	MI_TOUCH_MODULE_NONE,
	MI_SOUND_MODULE_BUZZER,
	MI_SOUND_MODULE_AUDIO,
	MI_SOUND_MODULE_ALL,
	MI_SOUND_MODULE_NONE,
	MI_FID_NFC_MODULE,
	MI_CONTACTLESS_MODULE_NONE,
	MI_CONTACT_MODULE_NONE,
	MI_TOUCH_MODULE_AUTO,
	MI_MAXQ_MODULE_NONE,
	MI_MAXQ_MODULE_AUTO,
};


// MI_FID_FISCAL_MODULE
enum
{
	MI_FISCAL_MODULE_NONE,
	MI_FISCAL_MODULE_AUTO,
};































/*
// Touch Module
enum
{
	MI_TOUCH_MODULE_NONE,
	MI_TOUCH_MODULE_AUTO,
};


// BACKLIGHT Module
enum
{
	MI_BACKLIGHT_MODULE_NONE,
	MI_BACKLIGHT_MODULE_AUTO,
};
*/



// Key Module
enum
{
	MI_KEY_MODULE_NONE,
	MI_KEY_MODULE_AUTO,
	MI_KEY_MODULE_360STYLE					= 100,
	MI_KEY_MODULE_760STYLE,
	MI_KEY_MODULE_NUMERIC					= 200,
	MI_KEY_MODULE_QWERTY,
};


// IrDA
enum
{
	MI_IrDA_MODULE_NONE,
	MI_IrDA_MODULE_AUTO,
	MI_IrDA_MODULE,
};


// Key Language
enum
{
	MI_KEY_LANG_NONE,
	MI_KEY_LANG_AUTO,
	MI_KEY_LANG_ENG							= 100,
	MI_KEY_LANG_HEBREW
};



// NFC module
enum
{
	MI_NFC_MODULE_NONE,
	MI_NFC_MODULE_AUTO,
};


// 부팅시 화면을 선택할수있게함
enum
{
	MI_BOOTLOGO_NONE,
	MI_BOOTLOGO_PIDION_BLACK,  // 검은색 PIDION 화면
	MI_BOOTLOGO_BLUEBIRD_BLUE, // 파란색 블루버드 화면
};


enum
{
	MI_DISPLAY_MODULE_NONE,
	MI_DISPLAY_MODULE_AUTO,
	MI_DISPLAY_MODULE_HD480G800 = 10,
	MI_DISPLAY_MODULE_ILI9341,
};


enum
{
	MI_LCD_PORTRAIT		=	0,
	MI_LCD_LANDSCAPE	=	1
};


// Display Lotate - ULDR에서 화면회전각도로 사용함. 커널에서 최초부팅시 화면회전정보로 사용함.
enum
{
	MI_LCD_LOTATE_0							=0,
	MI_LCD_LOTATE_90						=90,
	MI_LCD_LOTATE_180						=180,
	MI_LCD_LOTATE_270						=270,
};


// Display DPI
enum
{
	MI_LCD_DPI_NONE,
	MI_LCD_DPI_AUTO,
    MI_LCD_DPI_96							= 100,
    MI_LCD_DPI_128,
    MI_LCD_DPI_131,
    MI_LCD_DPI_192,
};


// CustomerLCD
enum
{
	MI_CUSTOMERLCD_NONE						= 0,
	MI_CUSTOMERLCD_AUTO,
};


// UART
enum
{
	MI_UART_NONE,
	MI_UART_AUTO,
	MI_UART_FFUART							= 100,
	MI_UART_BTUART,
	MI_UART_STUART,
	MI_UART_EXUART
};






// Modem Module Type
#define MI_MODEM_TYPE_NONE		0x00

// IMHW
#define	WM_IM_MAKE_BLOCK	WM_USER	+ 0x100

// typedef enum
// {
	// e_NONE 									= 0,
    // e_ENG_SMALL_LETTER,
    // e_ENG_SHIFT_BIG_LETTER,
    // e_ENG_BIG_LETTER,
    // e_HANGUL,
    // e_SPECIAL_LETTER,
// } E_KEYPAD_TYPE;

// struct IMHWState
// {
	// E_KEYPAD_TYPE   m_eType;
	// HWND            m_hWnd;
// };




enum
{
	LOGIC_SERIAL_BLUETOOTH		= 0,
	LOGIC_SERIAL_OUTSIDEPORT,
	LOGIC_SERIAL_GPS,
	LOGIC_SERIAL_RFID,
	LOGIC_SERIAL_IRDA,
	LOGIC_SERIAL_MODEM_MAIN,
	LOGIC_SERIAL_MODEM_SUB,
	LOGIC_SERIAL_BARCODE,
	
	LOGIC_CIF_CAMERA			= 100,
	LOGIC_CIF_IMAGER,
};

