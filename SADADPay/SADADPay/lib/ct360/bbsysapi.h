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
	DWORD	dwComponent;	/* reserved, BIP2000������ AUDIO �˻縦 ���� ����. */
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
#define BB_KEYPADLIGHT_ONLY_KEPAD		(1<<2)	//�̰��� 1�̸� keypad�϶��� ����ȴ�.

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
#define BB_UPDATE_ERR_NORMAL						1		// �Լ� ���п� ���� ���� ���ϰ�

#define BB_UPDATE_ERR_PARAM							11		// �Լ� ������ �߸��� ��뿡 ���� ���� ���ϰ�
#define BB_UPDATE_ERR_PATHLENGTH					12		// �������̴� 0~200���̳�������
#define BB_UPDATE_ERR_ROOTPATH						13		// ��Ʈ path�� �߸� �����Ǿ���
#define BB_UPDATE_ERR_FOLDERLENGTH					14
#define BB_UPDATE_ERR_FILEEXIST						15		// �ؼ� �Ұ��� ���� flag�� �ְų� ������ flag�� �ش��ϴ� ������ �������� ����
#define BB_UPDATE_ERR_BUFFERSIZE					16		// �Ķ���ͷ� ���� ���� ����� ������
#define BB_UPDATE_ERR_AUTH							17		// ���� ����

#define BB_UPDATE_ERR_MEMORYALLOC					51		// drvglob �޸𸮿� �����ϴµ� ����
#define BB_UPDATE_ERR_STORECONFIG					52		// drvglob�� ����ҿ� ����ϴµ� ����
#define BB_UPDATE_ERR_LOADCONFIG					53		// drvglob�� ����ҿ��� �ε��ϴµ� ����

#define BB_UPDATE_ERR_HASHMATCHING					101		// BBSysCheckFileValid ���� �ؽ� ��Ī�� ����
#define BB_UPDATE_ERR_SIGNATURECERT					102		// BBSysCheckFileValid ���� ���� ������ ����



#define BINARY_FILE	0x1
#define EXCUTE_FILE	0x2
#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

// BBSysUpdateSetCondition ���� �����ϴ� ����ü. BBSysUpdateGetStatus ���� ��ȯ�ϱ⵵�Ѵ�. 
typedef struct BBUpdateRequestInfo
{
	DWORD	 dwSize;							/* 4byte */		// ���� ����ü�� ũ�⸦ �־ API�� ȣ���ؾ��� 
	DWORD	 dwUpdateRoot;						/* 4byte */		// ������Ʈ�� ������ �ִ� ����̺��� ��ġ
    TCHAR    szUpdateFolder[200];				/* 200byte */		// ������Ʈ�� ������ �ִ� ������ġ. EX> PS\MT760\ULDR.bin  -> szUpdateFolder = L"MT760'"
    DWORD   dwUpdateFileList;					/* 4byte */		// ������Ʈ�� ���� ����Ʈ�� ��Ÿ��. OR�������� .BL , LOGO, ULDR , KERNEL(3����) , PS,  
} BBUpdateRequestInfo, *PBBUpdateRequestInfo;

// BBSysUpdateGetStatus ���� ����� ��ȯ�ϴ� ����ü. 
typedef struct BBUpdateResultInfo
{
	DWORD	 dwSize;							/* 4byte */		// ���� ����ü�� ũ�⸦ �־ API�� ȣ���ؾ��� 
	DWORD	 dwProgressState;					/* 4byte */		// BB_UPDATE_RESULT_NOTIFY ���� ��������� OS�� starter.exe�� PS\PidionUpdater.exe�� ������. 
    DWORD   dwUpdateResult;						/* 4byte */		// dwUpdateFileList �߿��� �ٿ�ε� ������ ������ 1�� ���õȴ�. 
	DWORD	 dwErrorReason;						/* 4byte */		// �����ڵ� - �ڸ������� �ٸ� ���̾��� �����ڵ带 ����ִ�. �ڼ��Ѱ��� bbsysapi.h ����.
	DWORD	 dwErrorReason2nd;					/* 4byte */		// �����ڵ�2 - ����¾Ⱦ��� 
} BBUpdateResultInfo, *PBBUpdateResultInfo;

// BBUpdateResultVersion ���� ����ϴ� ����ü. 
typedef struct BBBinVersion
{
	TCHAR	szUpdatemBinType[32];				/* 32byte */		// bin ���� type 
	DWORD	dwVersion;							/* 4byte */		// �ش� type�� image version 
} BBBinVersion, *PBBBinVersion;


// BBUPGRADE API���� ����ϴ� ���ϳ������ ��Ʈ�δ����� ����ϱ⶧���� DRV_GLOB.H �� ���ǵǾ���Ѵ�. �׷��� �ּ�ó����.

// dwNotifyMode - BB_UPDATE_RESULT_NOTIFY�� �����Ǿ������� KERNEL�����Ŀ� �ý��۾�����(starter.exe) PS���ִ� BBUPDATE.EXE�� �����Ѵ�.
#define BB_UPDATE_RESULT_NOTIFY					0x1234  
 
/* 
// dwUpdateRoot - ������Ʈ�� ������ ��ġ�� ��Ʈ  
#define UPGRADE_FOLDER_USERSTORAGE				0x01 // ������������. ����� �����߻�.
#define UPGRADE_FOLDER_PS								0x02
#define UPGRADE_FOLDER_SD								0x03
#define UPGRADE_FOLDER_USB								0x04


// dwUpdateFileList, dwUpdateResult - ������Ʈ�� ���� ����. OR�������� �������Է°���.

// dwUpdateFileList, dwUpdateResult - ������Ʈ�� ���� ����. OR�������� �������Է°���.  
#define UPGRADE_ALL									0x00000000 // ��� ������ ������Ʈ�Ҷ�  (API���ο��� ��������� ������ OR������ ������ ġȯ)
#define UPGRADE_BL1									0x00000001 // BL, MOBM,  Stepldr2ndBL
#define UPGRADE_BL2									0x00000010 // EBOOT, IPL
#define UPGRADE_LOGO								0x00000100 // ��� ���׷��̵� ������������
#define UPGRADE_ULDR								0x00001000
#define UPGRADE_PS									0x00010000
#define UPGRADE_KERNEL							0x00100000 // CT,MT���� ���� Ŀ���� �̰� �ϳ�����. ���� Ŀ���� PUBLIC, BSP, language pack 3���� ���� ������.  
//#define UPGRADE_KERNEL_PUBLIC				0x00100000 // ���� ��Ƽ��Ŀ�� ������ ��뿹��
//#define UPGRADE_KERNEL_BSP					0x00200000 // ���� ��Ƽ��Ŀ�� ������ ��뿹��
#define UPGRADE_KERNEL_LANG_PACK			0x00400000 // language pack -  CT/MT�� ���� ��Ƽ��Ŀ�� ������ ��뿹��

// special 
#define UPGRADE_MI									0x01000000 // Model info (refactoring BSP)
#define UPGRADE_IPL_HYBRID						0x02000000 // IPL Hybrid (refactoring BSP) (�Ϲ�->�������� �Ѿ���� ����) 

 */
 
 
// PBBUpdateRequestInfo ����ü�� size, ������Ʈ�� ������ �����ϴ�  root, �������, ���������� ���ڷ� �޾Ƽ�  NAND�� �����̳����� ���. 
BBAPI DWORD WINAPI BBSysCreateUpdateTransaction( const PBBUpdateRequestInfo updateInfo  );

// PBBUpdateRequestInfo����ü�� ������Ʈ ��ɿ����� ������, PBBUpdateResultInfo����ü�� ������Ʈ�� ����� ���� ������ ��ȯ.
BBAPI DWORD WINAPI BBSysCheckUpdateTransaction( PBBUpdateRequestInfo pRequestInfo , PBBUpdateResultInfo pResultInfo );

// ������Ʈ������ ����� ���.  BBSysUpdateGetStatus �Ŀ��� PBBUpdateRequestInfo�� �������� �����Ƿ� ������Ʈ�� ���� �Ŀ��� �ݵ�� �����ؾ��Ѵ�. 
BBAPI DWORD WINAPI BBSysClearUpdateTransaction( void );

// ���� DrvGlob ���¸� NAND�� ������. �ϵ帮�¾��� �� �ý��� ���α׷������� ����ϱ����� �н����带 �ɾ���. 
BBAPI DWORD WINAPI BBSysSaveDrvGlob( DWORD dwPassword );
BBAPI DWORD WINAPI BBSysLoadDrvGlob( DWORD dwPassword );

// �� Bin ������ ������ �о�´�.
BBAPI DWORD WINAPI BBSysGetVersions( PBBBinVersion pBinVersion, DWORD* pdwArrayCount );

// Bin������ Signature�� ���Ѵ�.

BBAPI DWORD WINAPI BBSysCheckFileValid( DWORD dwType, PTCHAR tszFilePath, const BYTE* CheckHashValue, DWORD dwHashLength );

// ���� Mode�� �ҷ��´�. 
BBAPI DWORD WINAPI BBSysGetOperationMode( DWORD* pdwModeCheck );

// �̹��� Update�ϱ��� Mode�� �����Ѵ�.
BBAPI DWORD WINAPI BBSysSetOperationMode( DWORD dwSetMode );

/*
BBUPDATE API���� ����ϴ�  dwErrorReason  ������  0x XXXX XXXX  -> �� �ڸ��� ���� ��Ÿ���� �����ڵ��� �Լ����̾ �ٸ��ϴ�.

[dwErrorReason �� �����ڵ� ����]

- ULDR ALLby�ٿ�ε� API(ULDR_AllDownloadAPI)
0x1 - Upgrade Folder file not exist
0x2 - Upgrade file not exist
0x3 - stepldr2ndBL.bin download fail
0x4 - Bootlogo.bin download fail
0x5 - ULDR.bin download fail
0x6 - PS folder download fail
0x7 - KERNEL.bin download fail
0x8 - KERNEL.bin download fail  (KERNEL_BSP ���� ������)
0x9 - KERNEL.bin download fail  (KERNEL_LANG_PACK ���� ������)
0xA - file does not download anything

- ULDR ���ϴٿ�ε��Լ� (ULDRImageDownLoad)
0x10 - Initialize Downloader Failed (constructor)
0x20 - Init() fail
0x30 - LOADER_COMMAND_FAILED (downloaderThread)
0x40 - verifying fail

- �ٿ�ε彺���� (ULDRBinDownLoader::DownLoaderThread())
0x100 - DownStream Init Failed!
0x200 - read signature Failed!   (���ε����� or �̹��������̾ƴ�)
0x300 - read image startpos Failed!
0x400 - read image length Failed!
0x500 - BIN signature Invalid!  (���ε����� or �̹��������̾ƴ�)
0x600 - MAXQ error
0x700 - RealInit error
0x800 - UpgradeBinFile() fail 1
0x900 - UpgradeBinFile() fail 2
0xA00 - wrong routine

- bin���� �������� (ULDRBinDownLoader::UpgradeBinFile())
0x1000 - READ dwRecAddr failed!
0x2000 - READ dwRecLength failed!
0x3000 - READ dwRecCheckSum failed!
0x4000 - read image record Failed
0x5000 - image record checksum error!
0x6000 - ENDCHECK Error! (DownStream error)

- ��Ʈ�δ� (2ndBL)
0x10000 - OEMLaunch() - Error Load Partition..
0x20000 - CheckEBOOTKey() - Enter Boot Key Menu

- ��Ʈ�δ� (Maxq_NotifyImage)
0x100000 - Write fail
0x200000 - Read fail
0x300000 - RecvPacket[4] != RESULT_OK
0x400000 - RecvPacket CRC error

- ��Ʈ�δ� (Maxq_SendMode)
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
