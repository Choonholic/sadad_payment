#ifndef __BBSYSAPI_H__BLUEBIRDSOFT
#define __BBSYSAPI_H__BLUEBIRDSOFT

#ifndef BBAPI
#ifdef BBPDA_EXPORTS
#define BBAPI __declspec(dllexport)
#else
#define BBAPI __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------------------------------------------- */

typedef LPVOID HBBSAMCARD;
#define BBSAMCARDMUTEX	(_T("BBSAMCARDMUTEX"))

BBAPI HBBSAMCARD WINAPI BBSAMCardOpen();
BBAPI DWORD WINAPI BBSAMCardClose( HBBSAMCARD hSAMCard );
//BBAPI DWORD	WINAPI BBSAMCardPowerUp( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, PDWORD	pdwActualOut);
BBAPI DWORD	WINAPI BBSAMCardPowerUp( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, DWORD* pdwActualOut);
BBAPI DWORD	WINAPI BBSAMCardSetCurrent( HBBSAMCARD hSAMCard, PBYTE pBufIn );
BBAPI DWORD	WINAPI BBSAMCardSAMStatus( HBBSAMCARD hSAMCard, PBYTE pBufOut, PDWORD	pdwActualOut );
BBAPI DWORD	WINAPI BBSAMCardPowerDown( HBBSAMCARD hSAMCard );
BBAPI DWORD	WINAPI BBSAMCardChangeCommParam( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwBufInLen );
BBAPI DWORD	WINAPI BBSAMCardExchangePPS( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwBufInLen, PBYTE pBufOut, PDWORD	pdwActualOut );
BBAPI DWORD	WINAPI BBSAMCardExchangeIFSD( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, PDWORD	pdwActualOut );
BBAPI DWORD	WINAPI BBSAMCardOutPut( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD dwBufIn, PBYTE pBufOut, PDWORD	pdwActualOut );
BBAPI DWORD	WINAPI BBSAMCardInPut( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD	dwLenIn, PBYTE pBufOut, PDWORD	pdwActualOut );
BBAPI DWORD	WINAPI BBSAMCardExchangeAPDU( HBBSAMCARD hSAMCard, PBYTE pBufIn, DWORD	dwLenIn, PBYTE pBufOut, PDWORD	pdwActualOut );

#define BB_SAMCARD_SUCCESS                  	0
#define BB_SAMCARD_ERR_DRIVER					1
#define BB_SAMCARD_ERR_NOT_OPERATION			2
#define BB_SAMCARD_ERR_ARGUMENT_OR_VALUE_ERROR 	3
#define BB_SAMCARD_ERR_TIMEOUT					4
#define BB_SAMCARD_ERR_SMALL_BUFFER				5
#define BB_SAMCARD_ERR_TS_VALUE					6
#define BB_SAMCARD_ERR_POWERED_DOWN				7
#define BB_SAMCARD_ERR_TCK_VALUE_RECEIVE		8
#define BB_SAMCARD_ERR_ATR						9
#define BB_SAMCARD_ERR_CARD_PROTOCOL_ERROR		10
#define BB_SAMCARD_ERR_COMMUNICATION_TIMEOUT	11
#define BB_SAMCARD_ERR_MANY_PARITY_ERROR		12
#define BB_SAMCARD_ERR_ABORT_CHAINING_T1		13
#define BB_SAMCARD_ERR_TRANSMISSION_ABORT_T1	14
#define BB_SAMCARD_ERR_PPS_EXCHANGE_ERROR		15	
#define BB_SAMCARD_ERR_ALREADY_POWERED_UP		16
#define BB_SAMCARD_ERR_CARD_REMOVED				17
#define BB_SAMCARD_ERR_CARD_MISSING				18
#define BB_SAMCARD_ERR_RETURNED_BY_CARD			19
#define BB_SAMCARD_ERR_UNKNOWN_ERROR			20
#define BB_SAMCARD_ERR_EXCHANGE_INTERRUPTED		21
#define BB_SAMCARD_ERR_INCORRECT_NUMBER			22
#define BB_SAMCARD_ERR_DEVICE_ERROR				23


/*
	Model
*/
#define BB_MODELNUMBER_BIP1000			(0x00010001)
#define BB_MODELNUMBER_BIP1000WL		(0x00010002)
#define BB_MODELNUMBER_BIP1100			(0x00010000)
#define BB_MODELNUMBER_BIP1100WL		(0x00011002)
#define BB_MODELNUMBER_BIP1200WL		(0x00012002)
#define BB_MODELNUMBER_BIP1250			(0x00012501)
#define BB_MODELNUMBER_BIP1250_REV2		(0x00012502)
#define BB_MODELNUMBER_BIP1250_BT		(0x00012503)
#define BB_MODELNUMBER_BIP2000			(0x00020001)
#define BB_MODELNUMBER_BIP3000			(0x00030001)
#define BB_MODELNUMBER_BIP3010			(0x00030010)
#define BB_MODELNUMBER_BIP3010_R11		(0x00030011)
#define BB_MODELNUMBER_BIP3010_GSM		(0x00030012)
#define BB_MODELNUMBER_BIP2010			(0x00020010)
#define BB_MODELNUMBER_ROADBEE			(0x00050010)
#define BB_MODELNUMBER_ROADBEE_GSM		(0x00050010)
#define BB_MODELNUMBER_TEST_MODEL		(0x00050020)
#define BB_MODELNUMBER_BM150R			(0x00050025)
#define BB_MODELNUMBER_KOSNEY			(0x00050030)	
#define BB_MODELNUMBER_BIP5000			(0x00050040)	
#define BB_MODELNUMBER_BIP5000R			(0x00050045)
#define BB_MODELNUMBER_BIP5100			(0x00051000)
#define BB_MODELNUMBER_BIP5300			(0x00053000)
#define BB_MODELNUMBER_BIP5300_BMT      (0x00053100)
#define BB_MODELNUMBER_SCUTE			(0x00051100)
#define BB_PRODUCTNAME_BM100			(0x00001000)
#define BB_PRODUCTNAME_BM110			(0x00001100)
#define BB_PRODUCTNAME_BM150			(0x00001500)
#define BB_PRODUCTNAME_BM150_WL			(0x00001510)
#define BB_PRODUCTNAME_BM200			(0x00002000)
#define BB_PRODUCTNAME_BM200_WL			(0x00002010)
#define BB_PRODUCTNAME_BM200_GSM		(0x00002020)
#define BB_MODELNUMBER_DORNIER			(0x00060010)
#define BB_MODELNUMBER_BIP1300_EVDO		(0x00060020)
#define BB_MODELNUMBER_BIP1300_GSM		(0x00060030)
#define BB_PRODUCTNAME_BIP1300			(0x00060040)
#define BB_PRODUCTNAME_BIP1300_IRIS		(0x00060050)
#define BB_PRODUCTNAME_BIP1300R			(0x00060060)
#define BB_MODELNUMBER_BM300			(0x00070001)
#define BB_MODELNUMBER_BPL10			(0x00080001)
#define BB_MODELNUMBER_CT360			(0x00090001)
#define BB_MODELNUMBER_MT760			(0x00100001)

#define BB_BIP2000_AUDIOUSE             	(0x00000001)
#define IOCTL_BBKEYPAD_TOGGLE_SCREEN		(0x0000000E)
typedef struct _tagBBModelInfo
{
	DWORD	cbSize;
	DWORD	dwModelNumber;
	DWORD	dwComponent;	/* reserved, BIP2000에서는 AUDIO 검사를 위해 쓴다. */
} BBModelInfo;

BBAPI DWORD WINAPI BBModelGetInfo( BBModelInfo* pMI );

/*
	Button Mapping
*/
#define BB_BUTTONMAP_PROGRAM			0
#define BB_BUTTONMAP_VIRTUALKEY			1
#define BB_BUTTONMAP_FUNCTION           2
#define BB_BUTTONMAP_NONE               3

#if (_WIN32_WCE < 300)   
typedef struct _tagBBButtonMappingInfo
{
	DWORD cbSize;
	DWORD dwKind;
	union
	{
		unsigned char ucVKey;
		TCHAR szFile[ MAX_PATH ];
	};
} BBButtonMappingInfo;
#elif  (_WIN32_WCE > 300 )
#define BB_BUTTONMAP_FUNC_NONE              0x00000000
#define BB_BUTTONMAP_FUNC_LCD_LIGHTOFF	    0x00001000
#define BB_BUTTONMAP_FUNC_LCD_LIGHTON	    0x00001001
#define BB_BUTTONMAP_FUNC_LCD_DIMMDOWN	    0x00001010
#define BB_BUTTONMAP_FUNC_LCD_DIMMUP	    0x00001011
#define BB_BUTTONMAP_FUNC_BARCODE_TRIGGER	0x00002000
#define BB_BUTTONMAP_FUNC_VKEYSEQ_CTRLC	    0x00003000
#define BB_BUTTONMAP_FUNC_VKEYSEQ_CTRLX	    0x00003001
#define BB_BUTTONMAP_FUNC_VKEYSEQ_CTRLV	    0x00003002
#define BB_BUTTONMAP_FUNC_SIP_TOGGLE		0x00004000

typedef struct _tagBBButtonMappingInfo
{
    DWORD cbSize;
	DWORD dwKind;
	union
	{
        unsigned char ucVKey;
        TCHAR szFile[ MAX_PATH ];
        DWORD dwFunctionCode;
	}; 
	TCHAR szParam[ MAX_PATH ];
} BBButtonMappingInfo;

typedef struct _tagBBButtonSoundMappingInfo
{  	
    TCHAR szFile[ MAX_PATH ];
	unsigned nIndex;
	BOOL bSoft;
} BBButtonSoundMappingInfo;

#endif

BBAPI DWORD WINAPI BBButtonGetMappingCount();
BBAPI DWORD WINAPI BBButtonSetMapping( DWORD dwButton, const BBButtonMappingInfo* pBMI );
BBAPI DWORD WINAPI BBButtonGetMapping( DWORD dwButton, BBButtonMappingInfo* pBMI );

BBAPI DWORD WINAPI BBButtonGetSoundMappingCount();
BBAPI DWORD WINAPI BBButtonSetSoundMapping( DWORD dwButton, const BBButtonSoundMappingInfo* pBSMI );
BBAPI DWORD WINAPI BBButtonGetSoundMapping( DWORD dwButton, BBButtonSoundMappingInfo* pBMI );
BBAPI DWORD WINAPI BBButtonGetSoundDefultMapping( DWORD dwButton, BBButtonSoundMappingInfo* pBMI );

/*
 * Keypad
 */
#if (_WIN32_WCE > 300)
typedef LPVOID HBBKEYPAD;
BBAPI HBBKEYPAD WINAPI BBKeypadOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBKeypadClose(HBBKEYPAD hKeypad);
BBAPI DWORD WINAPI BBKeypadResetSettings(HBBKEYPAD hKeypad);
typedef struct BBKeypadHook
{
	DWORD cbSize;
	DWORD dwReserved;
	HWND hWndCallback;
	UINT uMsgCallback;
} BBKeypadHook;
BBAPI DWORD WINAPI BBKeypadRegisterHook(HBBKEYPAD hKeypad, const BBKeypadHook * pNewHook, BBKeypadHook * pOldHook);
BBAPI DWORD WINAPI BBKeypadGetAutoRepeat(HBBKEYPAD hKeypad, BOOL * pEnable, DWORD * pInitialDelayMs, DWORD * pDelayMs);
BBAPI DWORD WINAPI BBKeypadSetAutoRepeat(HBBKEYPAD hKeypad, BOOL bEnable, DWORD uInitialDelay, UINT uDelay);
BBAPI DWORD WINAPI BBKeypadGetSwitchingMode(HBBKEYPAD hKeypad, BOOL * pEnable);
BBAPI DWORD WINAPI BBKeypadSetSwitchingMode(HBBKEYPAD hKeypad, BOOL bEnable);
typedef struct BBKeypadKeyInfo
{
	DWORD cbSize;
	DWORD dwScanCode;
	TCHAR szName[32];
	HBITMAP hbmIcon;
} BBKeypadKeyInfo;
BBAPI DWORD WINAPI BBKeypadGetKeyCount(HBBKEYPAD hKeypad, DWORD * pdwKeyCount);
BBAPI DWORD WINAPI BBKeypadGetKeyInfo(HBBKEYPAD hKeypad, DWORD dwIndex, BBKeypadKeyInfo * pKeyInfo);
typedef struct BBKeypadUserDefinableKeyInfo
{
	DWORD cbSize;
	DWORD dwScanCode;
	DWORD dwModifierBits;
} BBKeypadUserDefinableKeyInfo;
BBAPI DWORD WINAPI BBKeypadGetUserDefinableKeyCount(HBBKEYPAD hKeypad, DWORD * pdwUDKCount);
BBAPI DWORD WINAPI BBKeypadGetUserDefinableKeyInfo(HBBKEYPAD hKeypad, DWORD dwUDKIndex, BBKeypadUserDefinableKeyInfo * pUDKInfo);
BBAPI DWORD WINAPI BBKeypadGetModifierKeyScanCode(HBBKEYPAD hKeypad, DWORD dwModifierIndex, DWORD * pdwScanCode);

typedef struct BBKeypadUserDefinableSoundInfo
{	
	DWORD dwScanCode;
	DWORD dwModifierBits;
} BBKeypadUserDefinableSoundInfo;

BBAPI DWORD WINAPI BBKeypadGetUserDefinableSoundCount(HBBKEYPAD hKeypad, DWORD * pdwUDKCount);
BBAPI DWORD WINAPI BBKeypadGetUserDefinableSoundInfo(HBBKEYPAD hKeypad, DWORD dwUDKIndex, BBKeypadUserDefinableSoundInfo * pUDSInfo);


typedef struct BBKeypadFunctionInfo
{
	DWORD cbSize;
	DWORD dwFunctionCode;
	TCHAR szName[32];
	TCHAR szDescription[128];
} BBKeypadFunctionInfo;
BBAPI DWORD WINAPI BBKeypadGetFunctionCount(HBBKEYPAD hKeypad, DWORD * pdwFuncCount);
BBAPI DWORD WINAPI BBKeypadGetFunctionInfo(HBBKEYPAD hKeypad, DWORD dwFuncIndex, BBKeypadFunctionInfo * pFuncInfo);
BBAPI DWORD WINAPI BBKeypadGetSoundVolume( DWORD *Volume);
BBAPI DWORD WINAPI BBKeypadSetSoundVolume( DWORD Volume);
#endif

/*
	Red+Green LED
*/
#define BB_LEDCOLOR_OFF				0
#define BB_LEDCOLOR_GREEN			1
#define BB_LEDCOLOR_RED				2
#define BB_LEDCOLOR_ORANGE			3
#define BB_LEDCOLORCOUNT			4

typedef LPVOID HBBLED;
BBAPI HBBLED WINAPI BBLEDOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBLEDClose(HBBLED hLED);
BBAPI DWORD WINAPI BBLEDGetColor(HBBLED hLED, DWORD *pdwLEDColor);
BBAPI DWORD WINAPI BBLEDSetColor(HBBLED hLED, DWORD dwLEDColor);

/*
	RTC
*/
//typedef LPVOID HBBRTC;
//BBAPI HBBRTC WINAPI BBRTCOpen(DWORD dwReserved);
//BBAPI DWORD WINAPI BBRTCClose(HBBRTC hRTC);
//BBAPI DWORD WINAPI BBRTCReadTime(LPSYSTEMTIME lpst);
//BBAPI DWORD WINAPI BBRTCWriteTime(LPSYSTEMTIME lpst);
BBAPI BOOL WINAPI BBOEMGetRealTime(SYSTEMTIME *pTime);
BBAPI BOOL WINAPI BBOEMSetRealTime(LPSYSTEMTIME pTime);


/*
	Touch Screen
*/
#define BB_TOUCHSOUNDVOL_OFF	0
#define BB_TOUCHSOUNDVOL_SOFT	1
#define BB_TOUCHSOUNDVOL_LOUD	2
#define BB_TOUCHSOUNDVOLCOUNT	3

typedef LPVOID HBBTOUCHSOUND;
BBAPI HBBTOUCHSOUND WINAPI BBTouchSoundOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBTouchSoundClose(HBBTOUCHSOUND hTouchSound);
BBAPI DWORD WINAPI BBTouchSoundSetVolume(HBBTOUCHSOUND hTouchSound, DWORD dwVol);
BBAPI DWORD WINAPI BBTouchSoundGetVolume(HBBTOUCHSOUND hTouchSound, DWORD *pdwVol);


/*
	LCD
*/
#define BB_FRONTLIGHT_CLICK_NOT_ON	0
#define BB_FRONTLIGHT_CLICK_ON		1
#define BB_FRONTLIGHT_ALWAYS_ON		2
#define	BB_FRONTLIGHT_NOT_SETTING	999

typedef LPVOID HBBLCD;
BBAPI HBBLCD WINAPI BBLCDOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBLCDClose(HBBLCD hLCD);
//deprecated
BBAPI DWORD WINAPI BBLCDGetBacklight(HBBLCD hLCD, BOOL *pbEnable);
BBAPI DWORD WINAPI BBLCDSetBacklight(HBBLCD hLCD, BOOL bEnable);		
BBAPI DWORD WINAPI BBLCDGetBacklightTouchingLit(HBBLCD hLCD, DWORD *pdwValue);
BBAPI DWORD WINAPI BBLCDSetBacklightTouchingLit(HBBLCD hLCD, DWORD dwValue);
//new
BBAPI DWORD WINAPI BBLCDGetFrontlight(HBBLCD hLCD, BOOL *pbEnable);
BBAPI DWORD WINAPI BBLCDSetFrontlight(HBBLCD hLCD, BOOL bEnable);
BBAPI DWORD WINAPI BBLCDGetFrontlightTouchingLit(HBBLCD hLCD, DWORD *pdwValue);
BBAPI DWORD WINAPI BBLCDSetFrontlightTouchingLit(HBBLCD hLCD, DWORD dwValue);

BBAPI DWORD WINAPI BBLCDGetDimm(HBBLCD hLCD, DWORD * pdwLevel);
BBAPI DWORD WINAPI BBLCDSetDimm(HBBLCD hLCD, DWORD dwLevel);
BBAPI DWORD WINAPI BBLCDGetDimmMaxLevel(HBBLCD hLCD, DWORD *pdwLevel);

BBAPI DWORD WINAPI BBLCDSetRotate(DWORD dwRotate);
BBAPI DWORD WINAPI BBLCDGetState(HBBLCD hLCD, BOOL * pbEnable);
BBAPI DWORD WINAPI BBLCDSetToggle(HBBLCD hLCD);
BBAPI DWORD WINAPI BBLCDSetLight(HBBLCD hLCD, BOOL bEnable);

#define BB_WAKEUPREASON_USER	0
#define BB_WAKEUPREASON_RING	1
typedef LPVOID HBBWAKEUPCALLBACKREGISTRY;
typedef void (WINAPI * BBWakeupCallback)(LPVOID pParam, DWORD dwReason);
BBAPI HBBWAKEUPCALLBACKREGISTRY BBWakeupCallbackRegistryOpen(DWORD dwReserved);
BBAPI HBBWAKEUPCALLBACKREGISTRY BBWakeupCallbackRegistryClose(HBBWAKEUPCALLBACKREGISTRY hRegistry);
BBAPI DWORD WINAPI BBWakeupCallbackRegister(HBBWAKEUPCALLBACKREGISTRY hRegistry, BBWakeupCallback pCallback, LPVOID pParam, DWORD * pdwRegistrationToken);
BBAPI DWORD WINAPI BBWakeupCallbackUnregister(HBBWAKEUPCALLBACKREGISTRY hRegistry, DWORD dwRegistrationToken);



/*
	KeyPadLight
*/
// KeyPad Light Mode
#define BB_KEYPADLIGHT_CLICK_NOT_ON		0
#define BB_KEYPADLIGHT_CLICK_ON			1
#define BB_KEYPADLIGHT_ALWAYS_ON		2
#define BB_KEYPADLIGHT_ONLY_KEPAD		(1<<2)	//이것이 1이면 keypad일때만 적용된다.

// KeyEvent Mode
#define BB_KEYPAD_SEND_KEYEVENT				0	// send KeyEvent Mode
#define BB_KEYPAD_SEND_NO_KEYEVENT			1	//Not send KeyEvent Mode


typedef LPVOID HBBKPL;
BBAPI HBBKPL WINAPI BBKeyLightOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBKeyLightClose(HBBLCD hKPL);
BBAPI DWORD WINAPI BBKeyLightGETLight(HBBLCD hKPL,BOOL *pdwValue );
BBAPI DWORD WINAPI BBKeyLightSetLight( HBBLCD hKPL,BOOL bEnable );
BBAPI DWORD WINAPI BBGetKeyLightTouchingLit(HBBLCD hKPL, DWORD *pdwValue);
BBAPI DWORD WINAPI BBSetKeyLightTouchingLit(HBBLCD hKPL, DWORD dwValue);
BBAPI DWORD WINAPI BBSetKeyEventMode(HBBLCD hKPL, UCHAR ucValue);

/*
	Touch Button
*/
typedef LPVOID HBBTOUCHBUTTON;
#define MIN_TOUCH_RBUTTON_DELAY		100
#define MAX_TOUCH_RBUTTON_DELAY		5000	
BBAPI HBBTOUCHBUTTON WINAPI BBTouchButtonOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBTouchButtonClose(HBBTOUCHBUTTON hTouch);
BBAPI DWORD WINAPI BBTouchButtonGetRButton(HBBTOUCHBUTTON hTouch, BOOL * pbEnable);
BBAPI DWORD WINAPI BBTouchButtonSetRButton(HBBTOUCHBUTTON hTouch, BOOL bEnable);
BBAPI DWORD WINAPI BBTouchButtonGetRButtonDelay(HBBTOUCHBUTTON hTouch, DWORD * pdwDelay);
BBAPI DWORD WINAPI BBTouchButtonSetRButtonDelay(HBBTOUCHBUTTON hTouch, DWORD dwDelay);



/*
    SysBarcode
*/
typedef LPVOID HBBSYSBARCODE;
BBAPI HBBSYSBARCODE WINAPI BBSysBarcodeOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBSysBarcodeClose(HBBSYSBARCODE);
BBAPI DWORD WINAPI BBSysBarcodeSetPower(HBBSYSBARCODE, BOOL on);
BBAPI DWORD WINAPI BBSysBarcodeSetTrigger(HBBSYSBARCODE, BOOL on);
BBAPI DWORD WINAPI BBSysBarcodeEnableTriggerByKeypad(HBBSYSBARCODE, BOOL on);
BBAPI void WINAPI BBSysBarcodeWakeup(HBBSYSBARCODE hBarcode);
BBAPI DWORD WINAPI BBSysBarcodeGetModuleType(HBBSYSBARCODE hBarcode);
BBAPI DWORD WINAPI BBSysBarcodeCanRun(HBBSYSBARCODE hBarcode);

// user idle timer reset 
BBAPI void BBSysUserIdleTimerReset();

/*
BBAPI DWORD WINAPI BBBarcodePowerOn();
BBAPI DWORD WINAPI BBBarcodePowerOff();
//test code by coffriend - 2003.7.1
BBAPI DWORD WINAPI BBBarcodeTrigger( BOOL bOn );
*/

/*
    SysPrinter
*/
typedef LPVOID HBBSYSPRINTER;

// Error Code
#define BB_PRINTER_SUCCESS                 0
#define BB_PRINTER_ERR_NOPAPER             1 
#define BB_PRINTER_ERR_FEEDMARK            2
#define BB_PRINTER_ERR_TMPHIGH             3
#define BB_PRINTER_ERR_UNKNOWN             4
#define BB_PRINTER_ERR_SETDENSITY          10
#define BB_PRINTER_ERR_DENSITY_OUTOFRANGE  11 
#define BB_PRINTER_ERR_GETDENSITY          12
#define BB_PRINTER_ERR_TMPLOW             13
#define BB_PRINTER_ERR_VOLTAGELOW         14
#define BB_PRINTER_ERR_VOLTAGEHIGH        15
#define BB_PRINTER_ERR_PRINTSTART         17
#define BB_PRINTER_ERR_PRINTSTOP          18
#define BB_PRINTER_ERR_SETLINESPACING     19
#define BB_PRINTER_ERR_GETLINESPACING     20
#define BB_PRINTER_ERR_GETSTATE            21
#define BB_PRINTER_ERR_BUFFERFULL         22


BBAPI HBBSYSPRINTER WINAPI BBSysPrinterOpen(DWORD dwReserved);
BBAPI DWORD WINAPI BBSysPrinterClose(HBBSYSPRINTER);
BBAPI DWORD WINAPI BBSysPrinterSendData(HBBSYSPRINTER hPrinter, LPBYTE pData, DWORD dwSize, BOOL bLineEnd);
BBAPI DWORD WINAPI BBSysPrinterSetDensity(HBBSYSPRINTER hPrinter, UINT nDensity);
BBAPI DWORD WINAPI BBSysPrinterGetDensity(HBBSYSPRINTER hPrinter, UINT* pDensity);
BBAPI DWORD WINAPI BBSysPrinterStart(HBBSYSPRINTER hPrinter);
BBAPI DWORD WINAPI BBSysPrinterStop(HBBSYSPRINTER hPrinter);
BBAPI DWORD WINAPI BBSysPrinterFeedUntilMark(HBBSYSPRINTER hPrinter);
BBAPI DWORD WINAPI BBSysPrinterSetLineSpacing(HBBSYSPRINTER hPrinter, UINT nSpacing);
BBAPI DWORD WINAPI BBSysPrinterGetLineSpacing(HBBSYSPRINTER hPrinter, UINT* pSpacing);
BBAPI DWORD WINAPI BBSysPrinterWaitUntilPrintEnd(HBBSYSPRINTER hPrinter);


/*
	SDIO
*/
BBAPI DWORD WINAPI BBSDIOStatus();


/*
	OS Image Update
*/

// Error Code
#define BB_UPDATE_SUCCESS							0
#define BB_UPDATE_ERR_NORMAL						1		// 함수 실패에 대한 범용 리턴값

#define BB_UPDATE_ERR_PARAM							11		// 함수 인자의 잘못된 사용에 대한 범용 리턴값
#define BB_UPDATE_ERR_PATHLENGTH					12		// 폴더길이는 0~200자이내여야함
#define BB_UPDATE_ERR_ROOTPATH						13		// 루트 path가 잘못 지정되었음
#define BB_UPDATE_ERR_FOLDERLENGTH					14
#define BB_UPDATE_ERR_FILEEXIST						15		// 해석 불가한 파일 flag가 있거나 지정된 flag에 해당하는 파일이 존재하지 않음
#define BB_UPDATE_ERR_BUFFERSIZE					16		// 파라미터로 들어온 버퍼 사이즈가 부족함
#define BB_UPDATE_ERR_AUTH							17		// 권한 실패

#define BB_UPDATE_ERR_MEMORYALLOC					51		// drvglob 메모리에 접근하는데 실패
#define BB_UPDATE_ERR_STORECONFIG					52		// drvglob를 저장소에 기록하는데 실패
#define BB_UPDATE_ERR_LOADCONFIG					53		// drvglob를 저장소에서 로드하는데 실패

#define BB_UPDATE_ERR_HASHMATCHING					101		// BBSysCheckFileValid 에서 해쉬 매칭에 실패
#define BB_UPDATE_ERR_SIGNATURECERT					102		// BBSysCheckFileValid 에서 사인 인증에 실패



#define BINARY_FILE	0x1
#define EXCUTE_FILE	0x2
#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

// BBSysUpdateSetCondition 에서 셋팅하는 구조체. BBSysUpdateGetStatus 에서 반환하기도한다. 
typedef struct BBUpdateRequestInfo
{
	DWORD	 dwSize;							/* 4byte */		// 현재 구조체의 크기를 넣어서 API를 호출해야함 
	DWORD	 dwUpdateRoot;						/* 4byte */		// 업데이트할 파일이 있는 드라이브의 위치
    TCHAR    szUpdateFolder[200];				/* 200byte */		// 업데이트할 파일이 있는 폴더위치. EX> PS\MT760\ULDR.bin  -> szUpdateFolder = L"MT760'"
    DWORD   dwUpdateFileList;					/* 4byte */		// 업데이트할 파일 리스트를 나타냄. OR연산으로 .BL , LOGO, ULDR , KERNEL(3가지) , PS,  
} BBUpdateRequestInfo, *PBBUpdateRequestInfo;

// BBSysUpdateGetStatus 에서 결과로 반환하는 구조체. 
typedef struct BBUpdateResultInfo
{
	DWORD	 dwSize;							/* 4byte */		// 현재 구조체의 크기를 넣어서 API를 호출해야함 
	DWORD	 dwProgressState;					/* 4byte */		// BB_UPDATE_RESULT_NOTIFY 값이 들어있으면 OS의 starter.exe가 PS\PidionUpdater.exe를 실행함. 
    DWORD   dwUpdateResult;						/* 4byte */		// dwUpdateFileList 중에서 다운로드 실패한 파일이 1로 셋팅된다. 
	DWORD	 dwErrorReason;						/* 4byte */		// 에러코드 - 자릿수별로 다른 레이어의 에러코드를 담고있다. 자세한것은 bbsysapi.h 참고.
	DWORD	 dwErrorReason2nd;					/* 4byte */		// 에러코드2 - 현재는안쓰임 
} BBUpdateResultInfo, *PBBUpdateResultInfo;

// BBUpdateResultVersion 에서 사용하는 구조체. 
typedef struct BBBinVersion
{
	TCHAR	szUpdatemBinType[32];				/* 32byte */		// bin 파일 type 
	DWORD	dwVersion;							/* 4byte */		// 해당 type의 image version 
} BBBinVersion, *PBBBinVersion;


// BBUPGRADE API에서 사용하는 이하내용들은 부트로더에서 사용하기때문에 DRV_GLOB.H 에 정의되어야한다. 그래서 주석처리함.

// dwNotifyMode - BB_UPDATE_RESULT_NOTIFY로 설정되어있으면 KERNEL부팅후에 시스템어플이(starter.exe) PS에있는 BBUPDATE.EXE를 실행한다.
#define BB_UPDATE_RESULT_NOTIFY					0x1234  
 
/* 
// dwUpdateRoot - 업데이트할 파일이 위치한 루트  
#define UPGRADE_FOLDER_USERSTORAGE				0x01 // 추후지원예정. 현재는 에러발생.
#define UPGRADE_FOLDER_PS								0x02
#define UPGRADE_FOLDER_SD								0x03
#define UPGRADE_FOLDER_USB								0x04


// dwUpdateFileList, dwUpdateResult - 업데이트할 파일 종류. OR연산으로 여러개입력가능.

// dwUpdateFileList, dwUpdateResult - 업데이트할 파일 종류. OR연산으로 여러개입력가능.  
#define UPGRADE_ALL									0x00000000 // 모든 파일을 업데이트할때  (API내부에서 모든종류의 파일을 OR연산한 값으로 치환)
#define UPGRADE_BL1									0x00000001 // BL, MOBM,  Stepldr2ndBL
#define UPGRADE_BL2									0x00000010 // EBOOT, IPL
#define UPGRADE_LOGO								0x00000100 // 없어도 업그레이드 실패하지않음
#define UPGRADE_ULDR								0x00001000
#define UPGRADE_PS									0x00010000
#define UPGRADE_KERNEL							0x00100000 // CT,MT에서 현재 커널은 이거 하나만씀. 추후 커널이 PUBLIC, BSP, language pack 3개로 나뉠 예정임.  
//#define UPGRADE_KERNEL_PUBLIC				0x00100000 // 추후 멀티플커널 지원시 사용예정
//#define UPGRADE_KERNEL_BSP					0x00200000 // 추후 멀티플커널 지원시 사용예정
#define UPGRADE_KERNEL_LANG_PACK			0x00400000 // language pack -  CT/MT는 추후 멀티플커널 지원시 사용예정

// special 
#define UPGRADE_MI									0x01000000 // Model info (refactoring BSP)
#define UPGRADE_IPL_HYBRID						0x02000000 // IPL Hybrid (refactoring BSP) (일반->리팩으로 넘어갈때만 쓰임) 

 */
 
 
// PBBUpdateRequestInfo 구조체에 size, 업데이트할 파일이 존재하는  root, 폴더경로, 파일종류를 인자로 받아서  NAND에 업데이내용을 기록. 
BBAPI DWORD WINAPI BBSysCreateUpdateTransaction( const PBBUpdateRequestInfo updateInfo  );

// PBBUpdateRequestInfo구조체에 업데이트 명령에대한 내용을, PBBUpdateResultInfo구조체에 업데이트에 결과에 관한 내용을 반환.
BBAPI DWORD WINAPI BBSysCheckUpdateTransaction( PBBUpdateRequestInfo pRequestInfo , PBBUpdateResultInfo pResultInfo );

// 업데이트내용을 지우는 기능.  BBSysUpdateGetStatus 후에도 PBBUpdateRequestInfo는 지워지지 않으므로 업데이트가 끝난 후에는 반드시 삭제해야한다. 
BBAPI DWORD WINAPI BBSysClearUpdateTransaction( void );

// 현재 DrvGlob 상태를 NAND에 저장함. 하드리셋어플 등 시스템 프로그램에서만 사용하기위해 패스워드를 걸었다. 
BBAPI DWORD WINAPI BBSysSaveDrvGlob( DWORD dwPassword );
BBAPI DWORD WINAPI BBSysLoadDrvGlob( DWORD dwPassword );

// 각 Bin 파일의 버젼을 읽어온다.
BBAPI DWORD WINAPI BBSysGetVersions( PBBBinVersion pBinVersion, DWORD* pdwArrayCount );

// Bin파일의 Signature를 비교한다.

BBAPI DWORD WINAPI BBSysCheckFileValid( DWORD dwType, PTCHAR tszFilePath, const BYTE* CheckHashValue, DWORD dwHashLength );

// 현재 Mode를 불러온다. 
BBAPI DWORD WINAPI BBSysGetOperationMode( DWORD* pdwModeCheck );

// 이미지 Update하기전 Mode를 설정한다.
BBAPI DWORD WINAPI BBSysSetOperationMode( DWORD dwSetMode );

/*
BBUPDATE API에서 사용하는  dwErrorReason  변수는  0x XXXX XXXX  -> 각 자리수 별로 나타내는 에러코드의 함수레이어가 다릅니다.

[dwErrorReason 의 에러코드 정의]

- ULDR ALLby다운로드 API(ULDR_AllDownloadAPI)
0x1 - Upgrade Folder file not exist
0x2 - Upgrade file not exist
0x3 - stepldr2ndBL.bin download fail
0x4 - Bootlogo.bin download fail
0x5 - ULDR.bin download fail
0x6 - PS folder download fail
0x7 - KERNEL.bin download fail
0x8 - KERNEL.bin download fail  (KERNEL_BSP 현재 미지원)
0x9 - KERNEL.bin download fail  (KERNEL_LANG_PACK 현재 미지원)
0xA - file does not download anything

- ULDR 파일다운로드함수 (ULDRImageDownLoad)
0x10 - Initialize Downloader Failed (constructor)
0x20 - Init() fail
0x30 - LOADER_COMMAND_FAILED (downloaderThread)
0x40 - verifying fail

- 다운로드스레드 (ULDRBinDownLoader::DownLoaderThread())
0x100 - DownStream Init Failed!
0x200 - read signature Failed!   (사인된파일 or 이미지파일이아님)
0x300 - read image startpos Failed!
0x400 - read image length Failed!
0x500 - BIN signature Invalid!  (사인된파일 or 이미지파일이아님)
0x600 - MAXQ error
0x700 - RealInit error
0x800 - UpgradeBinFile() fail 1
0x900 - UpgradeBinFile() fail 2
0xA00 - wrong routine

- bin파일 업데이터 (ULDRBinDownLoader::UpgradeBinFile())
0x1000 - READ dwRecAddr failed!
0x2000 - READ dwRecLength failed!
0x3000 - READ dwRecCheckSum failed!
0x4000 - read image record Failed
0x5000 - image record checksum error!
0x6000 - ENDCHECK Error! (DownStream error)

- 부트로더 (2ndBL)
0x10000 - OEMLaunch() - Error Load Partition..
0x20000 - CheckEBOOTKey() - Enter Boot Key Menu

- 부트로더 (Maxq_NotifyImage)
0x100000 - Write fail
0x200000 - Read fail
0x300000 - RecvPacket[4] != RESULT_OK
0x400000 - RecvPacket CRC error

- 부트로더 (Maxq_SendMode)
0x1000000 - Write fail
0x2000000 - Read fail
0x5000000 - Read fail 2
0x3000000 - RecvPacket[4] != RESULT_OK
0x4000000 - RecvPacket CRC error

*/

#define DEV_INFO_SIZE 30

#define BB_INFO_MANUFACTURE 		1
#define BB_INFO_PRODUCT_TYPE		2
#define BB_INFO_IMAGE_CODE			3
#define BB_INFO_BUILD_VERSION		4
#define BB_INFO_PS_VERSION			5
#define BB_INFO_SECURE_PROCESSOR	6
#define BB_INFO_ETHERNET_MAC    	7
#define BB_INFO_WIFI_MAC			8
#define BB_INFO_MAX					9

typedef struct BB_DeviceInfo
{
    WCHAR     Manufacture[DEV_INFO_SIZE];
	WCHAR     PartNumber[DEV_INFO_SIZE];
	WCHAR     FirmwareVersion[DEV_INFO_SIZE];
	WCHAR     Reserved[DEV_INFO_SIZE];
}BB_DEVICEINFO, *LPBB_DEVICEINFO;

BBAPI DWORD WINAPI BBGetProductName( WCHAR *wcsProductName, DWORD dwBufLength );
BBAPI BOOL WINAPI BBGetSerialNumber( WCHAR *wcsSerialNo, DWORD dwBufLength );
BBAPI BOOL WINAPI BBGetDeviceInfo( WCHAR dwDevIndexInfo, WCHAR *wcsDevInfo, DWORD dwBufLength );

/* ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif


#endif//__BBSYSAPI_H__BLUEBIRDSOFT
