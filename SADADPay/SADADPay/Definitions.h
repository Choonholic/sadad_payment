// Definitions.h

#pragma once

// Models

#if defined(MODEL_CT360)
#define MODEL_NAME					L"CT360"
#define PSTN_AVAILABLE				TRUE
#define LAN_AVAILABLE				TRUE
#define GPRS_AVAILABLE				FALSE
#define	SOFTWARE_KEYPAD				FALSE
#define BATTERY_EXIST				FALSE
#define MENU_ARROW					FALSE
#elif defined(MODEL_MT360)
#define MODEL_NAME					L"MT360"
#define PSTN_AVAILABLE				FALSE
#define LAN_AVAILABLE				TRUE
#define GPRS_AVAILABLE				TRUE
#define	SOFTWARE_KEYPAD				FALSE
#define BATTERY_EXIST				TRUE
#define MENU_ARROW					FALSE
#elif defined(MODEL_MT760)
#define MODEL_NAME					L"MT760"
#define PSTN_AVAILABLE				FALSE
#define LAN_AVAILABLE				TRUE
#define GPRS_AVAILABLE				TRUE
#define	SOFTWARE_KEYPAD				TRUE
#define BATTERY_EXIST				TRUE
#define MENU_ARROW					TRUE
#endif

// Flags - Default Value is FALSE

#define TEST_MODE					FALSE

#if TEST_MODE
#define TEST_TERMINAL_ID			L"C0000202"
#define TEST_MERCHANT_ID			L"M00000000000093"
#define TEST_DEVICE_ID				L"CT360QSNHQQ002"
#define TEST_HW_VERSION				L"00002"
#define TEST_SW_VERSION				L"010002"
#define TEST_FW_VERSION				L"016000"
#endif

#define IGNORE_LOGON				FALSE
#define IGNORE_PAPER_SENSOR			FALSE
#define IGNORE_FINAL_MAC			TRUE
#define ALWAYS_TMS_NEED				FALSE
#define LOG_STATUS					FALSE
#define LOG_SEND_PACKETS			FALSE
#define	LOG_RECV_PACKETS			FALSE
#define LOG_TRANSACTION_DATA		FALSE

// Versions

#define VERSION_SETTINGS			0x04092015
#define VERSION_PARAMETERS			0x04092015
#define VERSION_DATABASE			3

// Paths

#define PATH_CONFIGXML				L"\\UPS\\Payment\\Data\\PayAppConfig.xml"
#define PATH_PARAMETERS				L"\\UPS\\Payment\\Data\\Parameters.ini"
#define PATH_SETTINGS				L"\\UPS\\Payment\\Data\\Settings.ini"
#define PATH_MASTERKEY				L"\\UPS\\Payment\\Data\\MasterKey.dat"
#define PATH_TMSCONFIGXML			L"\\PocketStore\\BluebirdTMS\\Config\\TmsConfig.xml"
#define PATH_TMSUPDATEXML			L"\\PocketStore\\Temp\\Update.xml"
#define PATH_TMSTEMPDIRECTORY		L"\\PocketStore\\Temp"
#define PATH_WAKEUP					L"\\SADADWakeup.exe"

// Registry

#define RK_BLUEBIRD				    L"System\\BlueBird"
#define RK_PAYMENTAPP				L"Software\\Bluebird\\PaymentApp"
#define RK_PRINTER					L"Drivers\\Printer"
#define RK_BATTERY					L"Drivers\\BuiltIn\\Battery"
#define RK_SHELL_ONTOP				L"SOFTWARE\\Microsoft\\Shell\\OnTop"
#define RK_SMSC95001_TCPIP			L"Comm\\SMSC95001\\Parms\\TcpIp"

#define RV_ADC_NOPAPER				L"ADCNoPaperValue"
#define RV_APP_KEEP_EXECUTE			L"AppKeepExecute"
#define RV_BACKUP					L"Backup"
#define RV_RECEIVE_STATUS			L"ReceiveStatus"
#define RV_DATE						L"Date"
#define RV_TIME						L"Time"
#define RV_POWEROFF_POPUP			L"UsePowerOffPopup"

// Debug Flags

#define DBG_VIEW					1
#define DBG_THREAD					1
#define DBG_SECURE					1
#define DBG_DB						1

// Constants

#define WM_CARDCLOSE				(WM_USER + 2001)
#define WM_CARDRESTART				(WM_USER + 2002)

#define NETWORK_NONE				0x00
#define NETWORK_SUCCESS				0x01
#define NETWORK_ADVICE				0x02
#define NETWORK_REVERSAL			0x03

#if defined(MODEL_CT360)
#define MAXQ_WAIT_ACTIVE			500
#elif defined(MODEL_MT360)
#define MAXQ_WAIT_ACTIVE			500
#elif defined(MODEL_MT760)
#define MAXQ_WAIT_ACTIVE			500
#endif

#define MAXQ_ERROR_EXCEEDED			40	// 20 seconds

#define TIMEOUT_MESSAGE				3000
#define TIMEOUT_RESULT				5000
#define TIMEOUT_PARAMETER			300000
#define TIMEOUT_GLANCE				600000

#define SCREEN_SAVE_WIDTH			168
#define SCREEN_SAVE_HEIGHT			100

#define INTERVAL_LOGON				300000
#define INTERVAL_GLANCE				2000
#define INTERVAL_ANNUNCIATOR		2000
#define INTERVAL_STATUS				100

#define PSTN_BUFFER_SIZE			1024
#define PSTN_CHUNK_SIZE				512

#if defined(MODEL_CT360)
#define MENU_ITEMS_PER_PAGE			4
#elif defined(MODEL_MT360)
#define MENU_ITEMS_PER_PAGE			4
#elif defined(MODEL_MT760)
#define MENU_ITEMS_PER_PAGE			10
#endif

#define	MAX_MENU_ITEMS				16
#define MAX_FNKEY					4

#define THREAD_RETRY_INITIAL		(-1)
#define	THREAD_RETRY_EXCEEDED		5

#define PAINT_NONE					0x00000000
#define PAINT_INTRO_TIME			0x00000001

#define PAINT_MENU_ITEM				0x00000001
#define PAINT_MENU_ARROW			0x00000002
#define PAINT_FIELDS				0x00000001
#define PAINT_BTN_1					0x00000100
#define PAINT_BTN_2					0x00000200
#define PAINT_BTN_3					0x00000400
#define PAINT_BTN_4					0x00000800
#define PAINT_KEYPAD_1				0x00010000
#define PAINT_KEYPAD_2				0x00020000
#define PAINT_KEYPAD_3				0x00040000
#define PAINT_KEYPAD_4				0x00080000
#define PAINT_KEYPAD_5				0x00100000
#define PAINT_KEYPAD_6				0x00200000
#define PAINT_KEYPAD_7				0x00400000
#define PAINT_KEYPAD_8				0x00800000
#define PAINT_KEYPAD_9				0x01000000
#define PAINT_KEYPAD_0				0x02000000
#define PAINT_KEYPAD_DOT			0x04000000
#define PAINT_KEYPAD_CLEAR			0x08000000
#define PAINT_ANNUNCIATOR			0x80000000

#define PAINT_FULL_SCREEN			0xFFFFFFFF

#define	INFO_MAX					0x20

#define INFO_AMOUNT					0x00
#define INFO_BILLID					0x01
#define INFO_PAYMENTID				0x02
#define INFO_CHARGE_COUNT			0x03
#define INFO_CITY_CODE				0x06
#define INFO_DATE					0x07
#define INFO_TIME					0x08
#define INFO_PHONEBOOK				0x09
#define INFO_SETTINGS				0x0A
#define INFO_TRACENO				0x0B
#define INFO_ENDDATE				0x0C
#define INFO_IPADDRESS				0x0D
#define INFO_PORT					0x0E
#define INFO_TMS_SETTINGS			0x0F
#define INFO_CPW_NEW				0x1C
#define INFO_CPW_CONFIRM			0x1D
#define INFO_PASSWORD				0x1E
#define	INFO_PIN					0x1F

#define INFO_DEFAULT_VALUE			0x00
#define INFO_CHANGE_PASSWORD		0x01
#define INFO_RECEIPT_TYPE			0x02
#define INFO_DIALUP_STATUS			0x03
#define INFO_PRINT_STATUS			0x04
#define INFO_RESET_F3_PASSWORD		0x05
#define INFO_TMS_WEEKDAYS			0x06
#define INFO_TMS_UPDATE				0x07
#define INFO_BUTTON_STATUS			0x1B
#define INFO_KEYPAD_STATUS			0x1C
#define INFO_MESSAGE_PRINT			0x1D
#define INFO_MESSAGE_TYPE			0x1E
#define INFO_MESSAGE_ID				0x1F

#define	ERR_INTERNAL_ERROR			(100 + IDS_ERROR_INTERNAL_ERROR)
#define ERR_CONN_NO_CARRIER			(100 + IDS_ERROR_NO_CARRIER)
#define ERR_CONN_LINE_BUSY			(100 + IDS_ERROR_LINE_BUSY)
#define ERR_CONN_NO_DIALTONE		(100 + IDS_ERROR_NO_DIALTONE)
#define	ERR_CONN_TIMEOUT			(100 + IDS_ERROR_TIMEOUT)
#define ERR_CONN_GPRS_NOT_CONNECTED	(100 + IDS_ERROR_GPRS_NOT_CONNECTED)
#define ERR_CONN_HOST_NOT_RESOLVED	(100 + IDS_ERROR_HOST_NOT_RESOLVED)
#define ERR_DIALUP_FAILED			(100 + IDS_ERROR_DIALUP_FAILED)
#define ERR_CONNECT_FAILED			(100 + IDS_ERROR_CONNECT_FAILED)
#define ERR_SEND_FAILED				(100 + IDS_ERROR_SEND_FAILED)
#define ERR_NO_RESPONSE				(100 + IDS_ERROR_NO_RESPONSE)
#define ERR_GPRS_NOT_READY			(100 + IDS_ERROR_GPRS_NOT_READY)
#define ERR_INVALID_PASSWORD		(100 + IDS_ERROR_INVALID_PASSWORD)
#define ERR_INVALID_PASSWORD_OF		(100 + IDS_ERROR_INVALID_PASSWORD_OVERFLOW)
#define ERR_KEY_CARD_ERROR			(100 + IDS_ERROR_KEY_CARD_ERROR)
#define ERR_NO_RECORD_FOUND			(100 + IDS_ERROR_NO_RECORD_FOUND)
#define ERR_LOGON_REQUIRED			(100 + IDS_ERROR_LOGON_REQUIRED)
#define ERR_INVALID_BILLID			(100 + IDS_ERROR_INVALID_BILLID)
#define ERR_INVALID_PAYMENTID		(100 + IDS_ERROR_INVALID_PAYMENTID)
#define ERR_INTRO_NO_PAPER			(100 + IDS_ERROR_INTRO_NO_PAPER)
#define ERR_PRINT_GENERIC			(100 + IDS_ERROR_PRINT_GENERIC)
#define ERR_PRINT_NO_PAPER			(100 + IDS_ERROR_PRINT_NO_PAPER)
#define ERR_PRINT_OVERHEAT			(100 + IDS_ERROR_PRINT_OVERHEAT)
#define ERR_PRINT_LOW_VOLT			(100 + IDS_ERROR_PRINT_LOWVOLTAGE)
#define ERR_PRINT_HIGH_VOLT			(100 + IDS_ERROR_PRINT_HIGHVOLTAGE)
#define ERR_INVALID_PIN_BLOCK		(100 + IDS_ERROR_INVALID_PIN_BLOCK)
#define ERR_NOT_IMPLEMENTED			(100 + IDS_ERROR_NOT_IMPLEMENTED)

#define CPW_NONE					0x00
#define CPW_NEW						0x01
#define CPW_CONFIRM					0x02

#define SR_PAPER					0x01
#define SR_RECONCILIATION			0x02
#define SR_FAILURE					0x03

#define MAX_FNK						4
#define FNK_F1						0
#define FNK_F2						1
#define FNK_F3						2
#define FNK_F4						3

#define MAX_HWK						21
#define HWK_LEFT					0
#define HWK_DOWN					1
#define HWK_UP						2
#define HWK_RIGHT					3
#define HWK_ESCAPE					4
#define HWK_BACK					5
#define HWK_RETURN					6
#define HWK_CENTER					7
#define HWK_F						8
#define HWK_DOT						9
#define HWK_0						10
#define HWK_1						11
#define HWK_2						12
#define HWK_3						13
#define HWK_4						14
#define HWK_5						15
#define HWK_6						16
#define HWK_7						17
#define HWK_8						18
#define HWK_9						19
#define HWK_HOME					20

#define	LENGTH_PASSWORD				8
#define	LENGTH_PASSWORD_MIN			4
#define	LENGTH_PIN					4
#define LENGTH_PIN_BLOCK			8
#define LENGTH_PAN					16
#define LENGTH_AMOUNT				12
#define LENGTH_BILLID				13
#define LENGTH_PAYMENTID			13
#define LENGTH_TRACENO				6
#define LENGTH_CITY_CODE			4
#define LENGTH_DATE					8
#define LENGTH_TIME					4
#define LENGTH_PHONE				16
#define LENGTH_PHONE_BOX			12
#define LENGTH_SETTINGS_BOX			3
#define LENGTH_IPADDRESS			12
#define LENGTH_PORT					6
#define LENGTH_EXIT_SEQUENCE		4
#define	LENGTH_EXIT_STEPS			4
#define LENGTH_RESET_F3_SEQUENCE	4
#define	LENGTH_RESET_F3_STEPS		4
#define LENGTH_SERIAL_NUMBER		16
#define LENGTH_POS_INFORMATION		33

#define MAX_KEYPAD					13
#define MAX_BRIGHTNESS				8
#define MAX_CONNECTION_TYPE			3
#define MAX_GPRS_CODE				32
#define MAX_PHONEBOOK				8
#define MAX_TMS						4
#define MAX_LANGUAGE				2
#define MAX_RESPONSE_CODE			100
#define MAX_BILL_SERVICE			8
#define MAX_CARD_PRODUCT			4
#define MAX_BANK_INFO				60
#define MAX_PROVIDER				10
#define MAX_VOUCHER					10
#define	MAX_CHARGE					5
#define MAX_TRACE_NO				999999
#define MAX_CONN_STATUS				6
#define MAX_BATT_STATUS				9
#define MAX_GPRS_STATUS				6
#define MAX_WEEKDAYS				7

#define INVALID_PASSWORD_COUNT		3

#define PARAMETER_NOT_READY			0
#define PARAMETER_READY				1
#define PARAMETER_CONFIG			2
#define PARAMETER_MASTER_KEY		3

#if defined(MODEL_CT360)
#define BX_EN_KEY					10
#define BX_EN_VALUE					130
#define BX_FA_KEY					200
#define BX_FA_VALUE					10
#define BW_KEY						110
#define BW_VALUE					180
#define CX_EN_KEY					10
#define CX_EN_VALUE					160
#define CX_FA_KEY					170
#define CX_FA_VALUE					10
#define CW_KEY						150
#define CW_VALUE					140
#define IY							48
#define IH							33
#define IG							44
#elif defined(MODEL_MT360)
#define BX_EN_KEY					10
#define BX_EN_VALUE					130
#define BX_FA_KEY					200
#define BX_FA_VALUE					10
#define BW_KEY						110
#define BW_VALUE					180
#define CX_EN_KEY					10
#define CX_EN_VALUE					160
#define CX_FA_KEY					170
#define CX_FA_VALUE					10
#define CW_KEY						150
#define CW_VALUE					140
#define IY							48
#define IH							33
#define IG							44
#elif defined(MODEL_MT760)
#define BX_EN_KEY					16
#define BX_EN_VALUE					194
#define BX_FA_KEY					294
#define BX_FA_VALUE					16
#define BW_KEY						170
#define BW_VALUE					270
#define CX_EN_KEY					16
#define CX_EN_VALUE					254
#define CX_FA_KEY					234
#define CX_FA_VALUE					16
#define CW_KEY						230
#define CW_VALUE					210
#define IY							56
#define IH							50
#define IG							66
#endif

// ISO8583 Message Types

#define MSGTYPE_REQ_BALANCE			"0100"
#define MSGTYPE_REQ_SALE			"0200"
#define MSGTYPE_REQ_CHARGE			"0200"
#define MSGTYPE_REQ_QUICKCHARGE		"0200"
#define MSGTYPE_REQ_BILL_PAYMENT	"0200"
#define MSGTYPE_REQ_ADVICE			"0220"
#define MSGTYPE_REQ_REVERSAL		"0400"
#define MSGTYPE_REQ_SERVICE_REQUEST	"0600"
#define MSGTYPE_REQ_LOGON			"0800"

#define MSGTYPE_RES_BALANCE			"0110"
#define MSGTYPE_RES_SALE			"0210"
#define MSGTYPE_RES_CHARGE			"0210"
#define MSGTYPE_RES_QUICKCHARGE		"0210"
#define MSGTYPE_RES_BILL_PAYMENT	"0210"
#define MSGTYPE_RES_ADVICE			"0230"
#define MSGTYPE_RES_REVERSAL		"0410"
#define MSGTYPE_RES_SERVICE_REQUEST	"0610"
#define MSGTYPE_RES_LOGON			"0810"

// ISO8583 Processing Codes

#define PROCODE_BALANCE				"310000"
#define PROCODE_SALE				"000000"
#define PROCODE_CHARGE				"150000"
#define PROCODE_QUICKCHARGE			"160000"
#define PROCODE_BILL_PAYMENT		"170000"
#define PROCODE_SERVICE_REQUEST		"920000"
#define PROCODE_LOGON				"920000"

// Secure Area

#define WRITE_DIRECTLY				0
#define WRITE_ENCRYPTION			1
#define WRITE_DECRYPTION			2

#define ECB							0
#define CBC							1

#define TDES_MASTER_KEY				1
#define TDES_PIN_ENC_KEY			2
#define TDES_MAC_KEY				3
#define TDES_GEN_PUR_INCMAC_KEY		4
#define AES_GEN_PUR_INCMAC_KEY		5
#define TDES_GEN_PUR_EXCMAC_KEY		7
#define AES_GEN_PUR_EXCMAC_KEY		8

#define CONFIG_XML_FILENAME			L"\\PayAppConfig.xml"

// Structures

typedef enum language_mode_t
{
	LANGUAGE_ENGLISH,
	LANGUAGE_FARSI
} LANGUAGE_MODE;

typedef enum message_type_t
{
	MESSAGE_NONE,
	MESSAGE_ERROR,
	MESSAGE_INFO
} MESSAGE_TYPE;

typedef enum action_requester_t
{
	REQ_BEFORE,
	REQ_TRANSACTION,
	REQ_REVERSAL,
	REQ_ADVICE
} ACTION_REQUESTER;

typedef enum screen_save_t
{
	SCREEN_NORMAL,
	SCREEN_OUT,
	SCREEN_GLANCE
} SCREEN_MODE;

typedef enum transaction_mode_t
{
	TRANSACTION_NONE,
	TRANSACTION_SALE,
	TRANSACTION_BALANCE,
	TRANSACTION_BILL_PAYMENT,
	TRANSACTION_CHARGE,
	TRANSACTION_F1,
	TRANSACTION_SELL_CHARGE,
	TRANSACTION_REPRINT,
	TRANSACTION_REPORTS,
	TRANSACTION_GOODS,
	TRANSACTION_FOOD,
	TRANSACTION_F2,
	TRANSACTION_CHANGE_PASSWORD,
	TRANSACTION_LOGON,
	TRANSACTION_SERVICE_REQUEST,
	TRANSACTION_F3,
	TRANSACTION_CHANGE_CITY_CODE,
	TRANSACTION_PHONEBOOK,
	TRANSACTION_SET_DATE,
	TRANSACTION_SET_TIME,
	TRANSACTION_BRIGHTNESS,
	TRANSACTION_LANGUAGE,
	TRANSACTION_SEND_PARAMETER,
	TRANSACTION_KEY_INJECTION,
	TRANSACTION_POS_SETTINGS,
	TRANSACTION_TMS_SETTINGS,
	TRANSACTION_MAX_MODE
} TRANSACTION_MODE;

typedef enum logon_status_t
{
	LOGON_NONE,
	LOGON_SUCCEEDED,
	LOGON_FAILED
} LOGON_STATUS;

typedef enum transaction_result_t
{
	RESULT_SUCCESS,
	RESULT_FAIL,
	RESULT_INVALID
} TRANSACTION_RESULT;

typedef enum tri_status_t
{
	TS_INITIATED,
	TS_CLEARED,
	TS_SET
} TRI_STATUS;

typedef enum connection_mode_t
{
	MODE_DIALUP,
	MODE_LAN,
	MODE_GPRS
} CONNECTION_MODE;

typedef enum connection_error_t
{
	CONN_NOERROR,
	CONN_DIALUP_ERROR,
	CONN_SOCKET_ERROR,
	CONN_CONNECT_ERROR,
	CONN_PHONE_ERROR,
	CONN_SIM_ERROR,
	CONN_REG_ERROR,
	CONN_GPRS_ERROR,
	CONN_NOT_COMPATIBLE,
	CONN_UNKNOWN_ERROR
} CONNECTION_ERROR;

typedef enum connection_status_t
{
	CONN_CONNECTED,
	CONN_PSTN_NOCARRIER,
	CONN_PSTN_NODIALTONE,
	CONN_PSTN_LINEBUSY,
	CONN_PSTN_TIMEOUT,
	CONN_PSTN_CONNECTING,
	CONN_GPRS_NOT_CONNECTED,
	CONN_HOST_NOT_RESOLVED,
	CONN_UNKNOWN_STATUS
} CONNECTION_STATUS;

typedef enum transaction_status_t
{
	STATUS_NONE,
	STATUS_VOLATILE,
	STATUS_LOGON,
	STATUS_TRANSACTION,
	STATUS_REVERSAL,
	STATUS_ADVICE,
	STATUS_COMPLETED,
	STATUS_REVERSAL_RESPONSED,
	STATUS_BALANCE_RESPONSED,
	STATUS_ERROR
} TRANSACTION_STATUS;

typedef enum view_mode_t
{
	VIEW_NONE,
	VIEW_INTRO,
	VIEW_MESSAGE,
	VIEW_MENU_MAIN,
	VIEW_PASSWORD,
	VIEW_PIN,
	VIEW_DIALUP,
	VIEW_RESULT,
	VIEW_PRINT,
	VIEW_PRINT_ACCEPTOR,
	VIEW_PRINT_RETRY,
	VIEW_SALE_AMOUNT,
	VIEW_SALE_CONFIRM,
	VIEW_BILL_BILLID,
	VIEW_BILL_PAYMENTID,
	VIEW_BILL_CONFIRM,
	VIEW_CHARGE_COUNT,
	VIEW_MENU_PROVIDER,
	VIEW_MENU_CATEGORY,
	VIEW_CHARGE_CONFIRM,
	VIEW_MENU_F1,
	VIEW_MENU_REPORTS,
	VIEW_REPORTS_TRACENO,
	VIEW_REPORTS_STARTDATE,
	VIEW_REPORTS_ENDDATE,
	VIEW_MENU_F2,
	VIEW_MENU_SERVICE_REQUEST,
	VIEW_MENU_F3,
	VIEW_CHANGE_CITY_CODE,
	VIEW_MENU_PHONEBOOK,
	VIEW_PHONEBOOK,
	VIEW_SET_DATE,
	VIEW_SET_TIME,
	VIEW_BRIGHTNESS,
	VIEW_MENU_LANGUAGE,
	VIEW_SEND_PARAMETER,
	VIEW_KEY_INJECTION,
	VIEW_MENU_POS_SETTINGS,
	VIEW_MENU_TMS_SETTINGS,
	VIEW_VERSION,
	VIEW_SETTINGS_NII,
	VIEW_SETTINGS_CONNECTION_TIMEOUT,
	VIEW_SETTINGS_CONNECTED_TIMEOUT,
	VIEW_SETTINGS_PIN_TIMEOUT,
	VIEW_SETTINGS_ADVICE_TIMEOUT,
	VIEW_SETTINGS_REVERSAL_TIMEOUT,
	VIEW_SETTINGS_SCREEN_TIMEOUT,
	VIEW_MENU_CONNECTION_TYPE,
	VIEW_SETTINGS_SERVER_IPADDRESS,
	VIEW_SETTINGS_SERVER_PORT,
	VIEW_TMS_SETTINGS_STARTTIME,
	VIEW_TMS_SETTINGS_ENDTIME,
	VIEW_TMS_SETTINGS_INTERVAL,
	VIEW_TMS_SETTINGS_COUNT,
	VIEW_TMS_SETTINGS_WEEKDAYS,
	VIEW_MAX_MODE
} VIEW_MODE;

typedef enum thread_status_t
{
	TSTATUS_READY,
	TSTATUS_DIALING,
	TSTATUS_CONNECTING,
	TSTATUS_WAITING,
	TSTATUS_RETRY,
	TSTATUS_CONNECTED,
	TSTATUS_SENDING,
	TSTATUS_RECEIVING,
	TSTATUS_RECEIVED,
	TSTATUS_ERROR
} THREAD_STATUS;

typedef enum thread_mode_t
{
	TMODE_READY,
	TMODE_WAITING_CONNECTION,
	TMODE_TRANSACTION,
	TMODE_ADVICE,
	TMODE_REVERSAL,
	TMODE_TMS,
	TMODE_LOWBATTERY
} THREAD_MODE;

typedef enum battery_status_t
{
	BATTERY_EMPTY,
	BATTERY_CRITICAL,
	BATTERY_LOW,
	BATTERY_HIGH,
	BATTERY_FULL,
	BATTERY_AC_EMPTY,
	BATTERY_AC_LOW,
	BATTERY_AC_HIGH,
	BATTERY_AC_FULL
} BATTERY_STATUS;

typedef enum gprs_status_t
{
	GPRS_NONE,
	GPRS_20,
	GPRS_40,
	GPRS_60,
	GPRS_80,
	GPRS_FULL
} GPRS_STATUS;

typedef enum menu_status_t
{
	MENU_UNCHECKED,
	MENU_CHECKED,
	MENU_GRAYED
} MENU_STATUS;

typedef enum menu_type_t
{
	MENU_MAIN,
	MENU_PROVIDER,
	MENU_CATEGORY,
	MENU_F1,
	MENU_REPORTS,
	MENU_F2,
	MENU_SERVICE_REQUEST,
	MENU_F3,
	MENU_PHONEBOOK,
	MENU_LANGUAGE,
	MENU_POS_SETTINGS,
	MENU_TMS_SETTINGS,
	MENU_CONNECTION_TYPE
} MENU_TYPE;

typedef enum menu_arrow_status_t
{
	MENU_ARROW_NONE,
	MENU_ARROW_UP,
	MENU_ARROW_DOWN
} MENU_ARROW_STATUS;

typedef enum receipt_type_t
{
	RECEIPT_NONE,
	RECEIPT_SALE,
	RECEIPT_BALANCE,
	RECEIPT_BILL_PAYMENT,
	RECEIPT_CHARGE,
	RECEIPT_SELL_CHARGE,
	RECEIPT_SERVICE_REQUEST,
	RECEIPT_TRANSACTION_DETAILS,
	RECEIPT_UNSUCCESSFUL,
	RECEIPT_TIMEOUT
} RECEIPT_TYPE;

typedef enum receipt_sequence_t
{
	RSEQ_NONE,
	RSEQ_CUSTOMER_ONLY,
	RSEQ_ACCEPTOR_ONLY,
	RSEQ_ALL
} RECEIPT_SEQUENCE;

typedef enum key_injection_t
{
	KEY_INJECTION_READY,
	KEY_INJECTION_START,
	KEY_INJECTION_PROCESS,
	KEY_INJECTION_END,
	KEY_INJECTION_ERROR
} KEY_INJECTION;

typedef struct bilingual_t
{
	WCHAR	szE[64];
	WCHAR	szF[64];
} BILINGUAL;

typedef struct paint_flags_t
{
	LANGUAGE_MODE		langMode;
	TRANSACTION_MODE	transactionMode;
	VIEW_MODE			viewMode;
	THREAD_MODE			threadMode;
	THREAD_STATUS		threadStatus;
	SYSTEMTIME			stIntroTime;
} PAINT_FLAGS;

typedef struct annunciator_flags_t
{
	DWORD			dwConnectStatus;
	GPRS_STATUS		gprsStatus;
	BATTERY_STATUS	batteryStatus;
	WCHAR			szOperator[32];
	SYSTEMTIME		stTime;
	UINT			nCount;
	THREAD_STATUS	threadStatus;
} ANNUNCIATOR_FLAGS;

typedef struct res_png_t
{
	UINT	nResID;
	LONG	width;
	LONG	height;
	POINT	ptLeftToRight;
	POINT	ptRightToLeft;
} RES_PNG;

typedef struct menu_item_t
{
	int			nMenuSeq;
	MENU_STATUS	nStatus;
	BILINGUAL	MenuTitle;
} MENU_ITEM;

typedef struct menu_info_t
{
	MENU_TYPE			menuType;
	BOOL				bCheckType;
	BOOL				bArrow;
	MENU_ARROW_STATUS	menuArrowStatus;
	int					nItemsPerPage;
	int					nPrevItem;
	int					nCurItem;
	int					nPrevPage;
	int					nCurPage;
	int					nMaxItems;
	int					nMaxPages;
	UINT				nTitleID;
	MENU_ITEM			menuItems[MAX_MENU_ITEMS];
	BOOL				bHierarchy;
	VIEW_MODE			previousViewMode;
	TRANSACTION_MODE	previousTransactionMode;
} MENU_INFO;

typedef struct voucher_info_t
{
	int			nIndex;
	int			nCID;
	LONGLONG	llAmount;
} VOUCHER_INFO;

typedef struct provider_info_t
{
	WCHAR			szProviderID[5];
	BILINGUAL		providerName;
	WCHAR			szCustomerSupport[17];
	WCHAR			szLoadProcedure[33];
	UINT			nVoucherCount;
	VOUCHER_INFO	vouchers[MAX_VOUCHER];
} PROVIDER_INFO;

typedef struct conn_type_t
{
	UINT		nIndex;
	BILINGUAL	connectionName;
} CONNECTION_TYPE;

typedef struct gprs_code_t
{
	WCHAR	szCode[6];
	WCHAR	szName[32];
} GPRS_CODE;

typedef struct bill_service_t
{
	UINT		nServiceCodeID;
	BILINGUAL	serviceCodeName;
} BILL_SERVICE;

typedef struct bank_info_t
{
	WCHAR		szBankBIN[7];
	BILINGUAL	bankName;
} BANK_INFO;

typedef struct card_product_t
{
	WCHAR		szCardProduct[9];
	BILINGUAL	cardProductName;
} CARD_PRODUCT;

typedef struct response_code_t
{
	UINT		nCode;
	BILINGUAL	message;
} RESPONSE_CODE;

typedef struct transaction_info_t
{
	WCHAR				szID[17];
	UINT				nAttempts;
	TRANSACTION_STATUS	transactionStatus;
	TRANSACTION_MODE	transactionMode;
	WCHAR				szProcessingCode[7];
	WCHAR				szAmount[13];
	WCHAR				szTraceNo[7];
	SYSTEMTIME			stTimestamp;
	WCHAR				szTrack2Data[41];
	WCHAR				szReferenceNo[13];
	WCHAR				szAuthID[7];
	WCHAR				szResponseCode[3];
	WCHAR				szProviderID[5];
	WCHAR				szCategoryID[3];
	WCHAR				szAdditionalData[26];
	UINT				nChargeCount;
	WCHAR				szSerial[5][21];
	WCHAR				szChargePIN[5][33];
	WCHAR				szBillID[14];
	WCHAR				szPaymentID[14];
	UINT				nServiceCode;
	WCHAR				szServiceType[3];
} TRANSACTION_INFO;

typedef struct charge_info_t
{
	WCHAR				szID[17];
	SYSTEMTIME			stTimestamp;
	WCHAR				szTraceNo[7];
	WCHAR				szProviderID[5];
	WCHAR				szCategoryID[3];
	WCHAR				szAmount[13];
	WCHAR				szSerial[21];
	WCHAR				szChargePIN[33];
	WCHAR				szReferenceNo[13];
	WCHAR				szAuthID[7];
} CHARGE_INFO;

typedef struct settings_info_t
{
	DWORD			dwVersion;
	DWORD			dwSize;
	LANGUAGE_MODE	langMode;
	int				nBrightness;
	BOOL			bMasterKey;
	LOGON_STATUS	logonStatus;
	BYTE			cPassword[LENGTH_PASSWORD];
	int				nF2Invalid;
	SYSTEMTIME		stF2Open;
	int				nF3Invalid;
	SYSTEMTIME		stF3Open;
	WCHAR			szCityCode[LENGTH_CITY_CODE + 1];
	DWORD			dwTraceNo;
	BYTE			cPOSInformation[LENGTH_POS_INFORMATION];
} SETTINGS_INFO;

typedef struct parameters_info_t
{
	DWORD			dwVersion;
	DWORD			dwSize;
	UINT			nCurrentConnectionType;
	UINT			nMaxConnectionType;
	CONNECTION_TYPE	connectionType[MAX_CONNECTION_TYPE];
	UINT			nMaxGPRSCode;
	GPRS_CODE		GPRSCode[MAX_GPRS_CODE];
	UINT			nMaxPhonebook;
	WCHAR			szPhonebook[MAX_PHONEBOOK][LENGTH_PHONE + 1];
	UINT			nTMSIndex;
	UINT			nMaxTMS;
	WCHAR			szTMS[MAX_TMS][LENGTH_PHONE + 1];
	WCHAR			szServerIP[16];
	WCHAR			szServerPort[6];
	UINT			nTimeoutScreen;
	UINT			nTimeoutConnection;
	UINT			nTimeoutConnected;
	UINT			nTimeoutPIN;
	UINT			nTimeoutAdvice;
	UINT			nTimeoutReversal;
	WCHAR			szPOSIP[16];
	WCHAR			szPOSGateway[16];
	WCHAR			szPOSSubnetMask[16];
	BOOL			bBeep;
	UINT			nLanguage;
	UINT			nMaxLanguage;
	BILINGUAL		languageType[MAX_LANGUAGE];
	SYSTEMTIME		stSyncTime;
	WCHAR			szSupportPhone[LENGTH_PHONE + 1];
	BILINGUAL		merchantName;
	WCHAR			szMerchantPhone[LENGTH_PHONE + 1];
	BILINGUAL		merchantAddress;
	WCHAR			szMerchantPostal[11];
	WCHAR			szPOSEntryMode[4];
	WCHAR			szNII[4];
	WCHAR			szPOSConditonCode[3];
	WCHAR			szTerminalID[9];
	WCHAR			szMerchantID[16];
	UINT			nTips;
	UINT			nAmountType;
	UINT			nMaxResponseCode;
	RESPONSE_CODE	responseCode[MAX_RESPONSE_CODE];
	UINT			nMaxBillService;
	BILL_SERVICE	billService[MAX_BILL_SERVICE];
	UINT			nMaxBankInfo;
	BANK_INFO		bankInfo[MAX_BANK_INFO];
	UINT			nMaxCardProduct;
	CARD_PRODUCT	cardProduct[MAX_CARD_PRODUCT];
	UINT			nMaxProviders;
	PROVIDER_INFO	providers[MAX_PROVIDER];
} PARAMETERS_INFO;

typedef struct tms_update_info_t
{
	WORD	wStartHour;
	WORD	wStartMinute;
	WORD	wEndHour;
	WORD	wEndMinute;
	WORD	wIntervalHour;
	WORD	wIntervalMinute;
	WORD	wCheckCount;
	WORD	wWeekdays;
} TMS_UPDATE_INFO;

typedef struct print_transaction_info_t
{
	RECEIPT_TYPE		ReceiptType;
	TRANSACTION_INFO	TransactionInfo;
	CHARGE_INFO			ChargeInfo;
	PARAMETERS_INFO*	pParameters;
	RECEIPT_SEQUENCE	ReceiptSequence;
} PRINT_INFO;

// Colors

#define COLOR_RGB_WHITE			RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RGB_BLACK			RGB(0x00, 0x00, 0x00)
#define COLOR_RGB_GRAY			RGB(0x80, 0x80, 0x80)
#define COLOR_RGB_RED			RGB(0xFF, 0x00, 0x00)
#define COLOR_RGB_BLUE			RGB(0x00, 0x00, 0xFF)
#define COLOR_RGB_TEXT			RGB(0x5F, 0x61, 0x64)
#define COLOR_RGB_CONFIRM		RGB(0x60, 0x80, 0xFF)

#if defined(MODEL_CT360)
#define COLOR_RGB_INTRO_TOP		RGB(0x33, 0x33, 0x33)
#define COLOR_RGB_INTRO_DATE	RGB(0xFF, 0xFF, 0xFF)
#elif defined(MODEL_MT360)
#define COLOR_RGB_INTRO_TOP		RGB(0x33, 0x33, 0x33)
#define COLOR_RGB_INTRO_DATE	RGB(0xFF, 0xFF, 0xFF)
#elif defined(MODEL_MT760)
#define COLOR_RGB_INTRO_DATE	RGB(0xC9, 0xC9, 0xC9)
#define COLOR_RGB_KEYPAD		RGB(0x33, 0x9B, 0xEC)
#define COLOR_RGB_KEYPAD_PRESS	RGB(0x34, 0x82, 0xC1)
#endif

// Utilities

#if LOG_STATUS
#define LOGFILE					g_Log.Write
#define LOGSTART				g_Log.StartMultiWrite()
#define LOGWRITE				g_Log.MultiWrite
#define LOGEND					g_Log.EndMultiWrite()
#else
#define LOGFILE
#define LOGSTART
#define LOGWRITE
#define LOGEND
#endif
