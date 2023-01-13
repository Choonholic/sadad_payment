
#ifndef __BBAPPAPI_H__BLUEBIRDSOFT
#define __BBAPPAPI_H__BLUEBIRDSOFT

#ifndef BBAPI
#ifdef BBPDA_EXPORTS
#define BBAPI __declspec(dllexport)
#else
#define BBAPI __declspec(dllimport)
#endif
#endif

#include "bberror.h"

#ifdef __cplusplus
extern "C" {
#endif




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CardReader ( MSR )

#define BITBUFFERSIZE					1024

#define BB_CARDREADER_READ_TRACK1		1
#define BB_CARDREADER_READ_TRACK2		2
#define BB_CARDREADER_READ_TRACK3		4

#define BB_CARDREADER_MODE_ISO			0x0
#define BB_CARDREADER_MODE_RAW			0x2
#define BB_CARDREADER_MODE_ENC_ISO		0x3
#define BB_CARDREADER_MODE_ENC_RAW		0x4
#define BB_CARDREADER_MODE_ENC_IV_ISO	0x5
#define BB_CARDREADER_MODE_ENC_IV_RAW	0x6

#define REG_MSR_VOLUME					TEXT("Drivers\\BuiltIn\\msr")
#define RV_VOLUME						TEXT("Volume")
#define CARDREADER_VOLUME_EVENT			TEXT("CardReaderVolume")

#define REG_MSR_MODE					TEXT("Drivers\\BuiltIn\\msr")
#define RV_MODE							TEXT("DataMode")


typedef struct
{
	char	m_track1RawData[BITBUFFERSIZE];
	char	m_track2RawData[BITBUFFERSIZE];
	char	m_track3RawData[BITBUFFERSIZE];
	
	DWORD	m_dwTrack1_RawLen;
	DWORD	m_dwTrack2_RawLen;
	DWORD	m_dwTrack3_RawLen;
}
RAWTRACKSDATA;


typedef LPVOID HBBCARDREADER;
typedef void ( WINAPI *CardReaderCallbackFunc )( DWORD dwArg, LPCTSTR data );
typedef void ( WINAPI *CardReaderRawDataCallbackFunc )( DWORD dwArg, LPCTSTR data, RAWTRACKSDATA* pRawTrackData );


BBAPI HBBCARDREADER WINAPI BBCardReaderOpen( DWORD dwReserved );
BBAPI DWORD WINAPI BBCardReaderClose( HBBCARDREADER hCardReader );

BBAPI DWORD WINAPI BBCardReaderGetVersion( HBBCARDREADER hCardReader, LPTSTR lpszVersion, DWORD* pdwVersionSize );
BBAPI DWORD WINAPI BBCardReaderReadData( HBBCARDREADER hCardReader, int nTrack, LPBYTE pBuffer, int nBufferSize );
BBAPI DWORD WINAPI BBCardReaderStartAsyncReadData( HBBCARDREADER hCardReader, CardReaderCallbackFunc pfCallback, DWORD dwCallbackArg );
BBAPI DWORD WINAPI BBCardReaderStopAsyncReadData( HBBCARDREADER hCardReader );
BBAPI DWORD WINAPI BBCardReaderReadCardData( HBBCARDREADER hCardReader, int nTrack, LPTSTR pBuffer, DWORD* pdwBufferSize );
BBAPI DWORD WINAPI BBCardReaderGetReadTrack( HBBCARDREADER hCardReader, DWORD* pdwReadTrack );
BBAPI DWORD WINAPI BBCardReaderSetVolume( HBBCARDREADER hCardReader, DWORD dwVolume );
BBAPI DWORD WINAPI BBCardReaderGetVolume( HBBCARDREADER hCardReader, DWORD* pdwVolume );
BBAPI DWORD WINAPI BBCardReaderEnableTrackSound( HBBCARDREADER hCardReader, int nTrack, BOOL bOn );

// Removed Function
// BBAPI DWORD WINAPI BBCardReaderReadString( HBBCARDREADER hCardReader, int track, LPTSTR buf );

// Not fixed
// BBAPI DWORD WINAPI BBCardReaderStartAsyncReadRawData( HBBCARDREADER hCardReader, CardReaderRawDataCallbackFunc pfRawCallback, DWORD dwCallbackArg );
// BBAPI DWORD WINAPI BBCardReaderStopAsyncReadRawData( HBBCARDREADER hCardReader );
// BBAPI DWORD WINAPI BBCardReaderReadCardRawData( HBBCARDREADER hCardReader, RAWTRACKSDATA* TrackData );
// BBAPI DWORD WINAPI BBCardReaderDecodingISOFormat( HBBCARDREADER hCardReader, RAWTRACKSDATA * TrackData );
// BBAPI DWORD WINAPI BBCardReaderDecodingJIS2Format( HBBCARDREADER hCardReader, RAWTRACKSDATA * TrackData );
// BBAPI DWORD WINAPI BBCardReaderDUKPT_Key( HBBCARDREADER hCardReader, UCHAR Key[4][4], BYTE KSN[10], DWORD Enc_cnt );
// BBAPI DWORD WINAPI BBCardReaderDecryption( HBBCARDREADER hCardReader, char Data[], DWORD Data_Len, UCHAR Key[4][4], UCHAR Vector[4][4], RAWTRACKSDATA * TrackData );
// BBAPI DWORD WINAPI BBCardReaderGetMode( DWORD* Mode );




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Printer

#define BB_PRINTER_SEORO            		0
#define BB_PRINTER_SEIKO            		1
#define BB_PRINTER_NONE             		2

#define BB_PRINT_DEFAULT					0
#define BB_PRINT_EMPHASIZE					1
#define BB_PRINT_CENTERALIGN				2
#define BB_PRINT_RIGHTALIGN					4
#define BB_PRINT_DOUBLEWIDTH				8
#define BB_PRINT_DOUBLEHEIGHT				16
#define BB_PRINT_DOUBLESIZE					24
#define BB_PRINT_UNDERLINE					32
#define BB_PRINT_BOTH_ALIGN					256
#define BB_PRINT_REVERSEIMAGE				512

#define BB_PRINT_DENSITY_60PERCENT			0
#define BB_PRINT_DENSITY_70PERCENT			1
#define BB_PRINT_DENSITY_80PERCENT			2
#define BB_PRINT_DENSITY_90PERCENT			3
#define BB_PRINT_DENSITY_100PERCENT			4
#define BB_PRINT_DENSITY_110PERCENT			5
#define BB_PRINT_DENSITY_120PERCENT			6
#define BB_PRINT_DENSITY_130PERCENT			7
#define BB_PRINT_DENSITY_140PERCENT			8
#define BB_PRINT_DENSITY_DEFAULT			BB_PRINT_DENSITY_90PERCENT
#define BB_PRINT_DENSITY_MIN				BB_PRINT_DENSITY_60PERCENT
#define BB_PRINT_DENSITY_MAX				BB_PRINT_DENSITY_140PERCENT

#define BB_PRINT_LINESPACING_ONESIXTH		24
#define BB_PRINT_LINESPACING_DEFAULT		24
#define BB_PRINT_LINESPACING_MAX			255

#define BB_PRINT_BARCODE_UPCA				65
#define BB_PRINT_BARCODE_JAN13				67
#define BB_PRINT_BARCODE_JAN8				68
#define BB_PRINT_BARCODE_CODE39				69
#define BB_PRINT_BARCODE_ITF				70
#define BB_PRINT_BARCODE_CODABAR			71
#define BB_PRINT_BARCODE_CODE93				72
#define BB_PRINT_BARCODE_CODE128			73
#define BB_PRINT_BARCODE_CODE128A			73
#define BB_PRINT_BARCODE_CODE128B			74
#define BB_PRINT_BARCODE_CODE128C			75

#define BB_PRINT_BARCODE_WIDTH_THIN			2
#define BB_PRINT_BARCODE_WIDTH_NORMAL	    3
#define BB_PRINT_BARCODE_WIDTH_THICK	    4
#define BB_PRINT_BARCODE_WIDTH_MORETHICK    5
#define BB_PRINT_BARCODE_WIDTH_MOSTTHICK    6

#define BB_PRINT_BARCODE_ALIGN_LEFT			0
#define BB_PRINT_BARCODE_ALIGN_CENTER	    2
#define BB_PRINT_BARCODE_ALIGN_RIGHT	    4

#define BB_PRINT_BITMAP_NORMAL			    0
#define BB_PRINT_BITMAP_ALIGN_CENTER		2
#define BB_PRINT_BITMAP_ALIGN_RIGHT 		4
#define BB_PRINT_BITMAP_DOUBLE_WIDTH	    8
#define BB_PRINT_BITMAP_DOUBLE_HEIGHT	    16
#define BB_PRINT_BITMAP_DOUBLE			    24

#define BB_PRINT_FONTTYPE_42                0
#define BB_PRINT_FONTTYPE_40                1
#define BB_PRINT_FONTTYPE_32                2

#define BB_PRINT_FONTSET_ASCII              0
#define BB_PRINT_FONTSET_HANGUL             1
#define BB_PRINT_FONTSET_SYMBOL             2
#define BB_PRINT_FONTSET_THAI               3


enum
{
	BB_PRINT_ERROR_NONE						= 0,
	BB_PRINT_ERROR_NOPAPER,
	BB_PRINT_ERROR_NOBLACKMARK,
	BB_PRINT_ERROR_OVERHEAT,
	BB_PRINT_ERROR_UNKNOWN,
	BB_PRINT_ERROR_INVALIDHANDLE,
	BB_PRINT_ERROR_NOFONT,
	BB_PRINT_ERROR_READFILE,
	BB_PRINT_ERROR_NOBITMAPFILE,
	BB_PRINT_ERROR_INVALIDBITMAPFILE,
	BB_PRINT_ERROR_SETDENSITY,
	BB_PRINT_ERROR_SETDENSITY_OUTOFRANGE,
	BB_PRINT_ERROR_GETDENSITY,
	BB_PRINT_ERROR_LOWTEMPERATURE,
	BB_PRINT_ERROR_LOWVOLTAGE,
	BB_PRINT_ERROR_HIGHVOLTAGE,
	BB_PRINT_ERROR_SETLINESPACING,
	BB_PRINT_ERROR_GETLINESPACING,
};


typedef struct _FONTINFO
{
	LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    WCHAR     lfFaceName[LF_FACESIZE];
}
FONTINFO, *LPFONTINFO;

typedef LPVOID HBBPRINTER;


BBAPI HBBPRINTER WINAPI BBPrinterOpen( DWORD dwReserved );
BBAPI DWORD WINAPI BBPrinterClose( HBBPRINTER hPrinter );

BBAPI DWORD WINAPI BBPrinterInit( HBBPRINTER hPrinter );
BBAPI DWORD WINAPI BBPrinterSetDensity( HBBPRINTER hPrinter, UCHAR ucDensity );
BBAPI DWORD WINAPI BBPrinterGetDensity( HBBPRINTER hPrinter, UCHAR* pucDensity );
BBAPI DWORD WINAPI BBPrinterSetLineSpacing( HBBPRINTER hPrinter, UCHAR ucLineSpacing );
BBAPI DWORD WINAPI BBPrinterGetLineSpacing( HBBPRINTER hPrinter, UCHAR* pucLineSpacing );
BBAPI DWORD WINAPI BBPrinterPrint( HBBPRINTER hPrinter, LPCTSTR lpcszData, DWORD dwOptions );
BBAPI DWORD WINAPI BBPrinterPrintBarcode( HBBPRINTER hPrinter, LPCTSTR lpcszData, UCHAR ucHeight, UCHAR ucWidth, UCHAR ucBarcodeSystem, UCHAR ucAlign );
BBAPI DWORD WINAPI BBPrinterPrintBitmap( HBBPRINTER hPrinter, LPCTSTR lpcszFilename, DWORD dwMode );
BBAPI DWORD WINAPI BBPrinterFeedUntilMark( HBBPRINTER hPrinter );
BBAPI DWORD WINAPI BBPrinterWaitUntilPrintEnd( HBBPRINTER hPrinter );
BBAPI DWORD WINAPI BBPrinterGetPaperState( HBBPRINTER hPrinter );

BBAPI DWORD WINAPI BBPrinterRegisterFontType( HBBPRINTER hPrinter, UCHAR ucFontSubCode );
BBAPI DWORD WINAPI BBPrinterRegisterFontFromSet( HBBPRINTER hPrinter, UCHAR ucCodeOfSet, UCHAR ucFontSubCode );

BBAPI DWORD WINAPI BBPrinterCreateDC( HBBPRINTER hPrinter, UINT nWidth, UINT nHeight );
BBAPI DWORD WINAPI BBPrinterDeleteDC( HBBPRINTER hPrinter );
BBAPI DWORD WINAPI BBPrinterPrintDC( HBBPRINTER hPrinter );
BBAPI DWORD WINAPI BBPrinterSetFont( HBBPRINTER hPrinter, LPFONTINFO lpFontInfo );
BBAPI DWORD WINAPI BBPrinterDrawText( HBBPRINTER hPrinter, LPCTSTR lpText, int nCount, int nLeft, int nTop, int nRight, int nBottom, UINT uFormat );
BBAPI DWORD WINAPI BBPrinterBitBlt( HBBPRINTER hPrinter, LPCTSTR lpFilePath, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, DWORD dwRop );
BBAPI DWORD WINAPI BBPrinterDrawBitmap( HBBPRINTER hPrinter, HBITMAP hUserBit,int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Customer LCD

typedef LPVOID HBBCUSTOMERLCD;

BBAPI HBBCUSTOMERLCD WINAPI BBCustomerLCDOpen( DWORD dwReserved );
BBAPI DWORD WINAPI BBCustomerLCDClose( HBBCUSTOMERLCD hCustomerLCD );

BBAPI DWORD WINAPI BBCustomerLCDOn( HBBCUSTOMERLCD hCustomerLCD );
BBAPI DWORD WINAPI BBCustomerLCDOff( HBBCUSTOMERLCD hCustomerLCD );

BBAPI DWORD WINAPI BBCustomerLCDPrint( HBBCUSTOMERLCD hCustomerLCD, LPCTSTR lpcszData, DWORD dwOptions );
BBAPI DWORD WINAPI BBCustomerLCDClear( HBBCUSTOMERLCD hCustomerLCD );
BBAPI DWORD WINAPI BBCustomerLCDReverseMode( HBBCUSTOMERLCD hCustomerLCD, BOOL bReverse );



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contact ( IC & SAM Card )

#define BB_IC_SAM_READER_SLOT_0	0
#define BB_IC_SAM_READER_SLOT_1	1	//FOR SIM.  Not for sam card.
#define BB_IC_SAM_READER_SLOT_2	2
#define BB_IC_SAM_READER_SLOT_3	3
#define BB_IC_SAM_READER_SLOT_4	4

enum SlotNum
{
	SLOT_IC1	= 0,
	SLOT_SAM1	= 1,
	SLOT_SAM2	= 2,
	SLOT_SAM3	= 3,
	SLOT_SAM4	= 4,
	SLOT_IC2	= 5
};


enum DESFireCmd
{
    cSAMOnOff						= 0x00,
    cAuthenticate					= 0x01,
    cChangeKeySettings				= 0x02,
    cGetKeySettings					= 0x03,
    cChangeKey						= 0x04,
    cGetKeyVersion					= 0x05,
    cCreateApplication				= 0x06,
    cDeleteApplication				= 0x07,
    cGetApplicationIDs				= 0x08,
    cSelectApplication				= 0x09,
    cFormatPICC						= 0x0A,
    cGetVersion						= 0x0B,
    cGetFileIDs						= 0x0C,
    cGetFileSettings				= 0x0D,
    cChangeFileSettings				= 0x0E,
    cCreateStandardDataFile			= 0x0F,
    cCreateBackupDataFile			= 0x10,
    cCreateValueFile				= 0x11,
    cCreateLinearRecordFile			= 0x12,
    cCreateCyclicRecordFile			= 0x13,
    cDeleteFile						= 0x14,
    cReadData						= 0x15,
    cReadRecords					= 0x16,
    cWriteData						= 0x17,
    cWriteRecords					= 0x18,
    cGetValue						= 0x19,
    cCredit							= 0x1A,
    cDebit							= 0x1B,
    cLimitedCredit					= 0x1C,
    cClearRecordFile				= 0x1D,
    cCommitTransaction				= 0x1E,
    cAbortTransaction				= 0x1F,
    cSetFileSettings				= 0x20,

    cDisableCrypto					= 0x30,
    cChangeKeyEntry					= 0x31,
    cGetKeyEntry					= 0x32,
    cChangeKUCEntry					= 0x33,
    cGetKUCEntry					= 0x34,
    cChangeKeyPICC					= 0x35,
    cGetVersionSAM					= 0x36,
    cAuthenticateHost				= 0x37,
    cSelectApplicationSAM			= 0x38,
    cDumpSessionKey					= 0x39,
    cLoadInitVector					= 0x3A,
    cAuthenticatePICC1				= 0x3B,
    cAuthenticatePICC2				= 0x3C,
    cVerifyMAC						= 0x3D,
    cGenerateMAC					= 0x3E,
    cDecipherData					= 0x3F,
    cEncipherData					= 0x40,
    cSetLogicalChannel				= 0x42
};

enum CalypsoCmd
{
	// SAM
	cChangeSpeed					= 0x01,
	cCheckSignedData				= 0x02,
	cCipherCardData					= 0x03,
	cCipherData						= 0x04,
	cCipherSAMData					= 0x05,
	cCipherSecret					= 0x06,
	cComputeDigitalCertificate		= 0x07,
	cComputeDigitalSignature		= 0x08,
	cCTMGetChallenge				= 0x09,
	cCTMGetKey						= 0x0A,
	cCTMVerifyAntiCloseSignature	= 0x0B,
	cDataDecipher					= 0x0C,
	cDataEncipher					= 0x0D,
	cDigestClose					= 0x0E,
	cDigestInit						= 0x0F,
	cDigestUpdate					= 0x10,
	cEPCancelPurchase				= 0x11,
	cEPCheck						= 0x12,
	cEPLoad							= 0x13,
	cEPPurchase						= 0x14,
	cExternalAuthenticate			= 0x15,
	cSAMGetChallenge				= 0x16, // Get Challenge
	cSAMGetResponse					= 0x17, // Get Response
	cGiveRandom						= 0x18,
	cInternalAuthenticate			= 0x19,
	cSAMReadData					= 0x1A, // Read Data
	cReadKeyParameters				= 0x1B,
	cReadWorkKeyParameters			= 0x1C,
	cSelectDiversifier				= 0x1D,
	cVerifyDigitalCertificate		= 0x1E,
	cVerifyDigitalSignature			= 0x1F,
	cVerifySecret					= 0x20,
	cSAMWriteData					= 0x21, // Write Data
	cWriteKey						= 0x22,
	
	// RFID
	cGetATR							= 0x40,
	cGetResponse					= 0x41,
	cSelectApp						= 0x42, // cSelectApplication
	cSelectFile						= 0x43,
	cInvalidate						= 0x44,
	cRehabilitate					= 0x45,
	cAppendRecord					= 0x46,
	cDecrease						= 0x47,
	cDecreaseMultiple				= 0x48,
	cIncrease						= 0x49,
	cIncreaseMultiple				= 0x4A,
	cReadBinary						= 0x4B,
	cReadRecord						= 0x4C,
	cReadRecordMultiple				= 0x4D,
	cSearchRecordMultiple			= 0x4E,
	cUpdateBinary					= 0x4F,
	cUpdateRecord					= 0x50,
	cWriteBinary					= 0x51,
	cWriteRecord					= 0x52,
	cOpenSecureSession				= 0x53,
	cCloseSecureSession				= 0x54,
	cGetChallenge					= 0x55,
	cVerifyPin						= 0x56,
	cChangeKeys						= 0x57, // Change Key
	cGetEP							= 0x58,
	cDebitEP						= 0x59,
	cUnDebitEP						= 0x5A,
	cReloadEP						= 0x5B,
	cStatus							= 0x5c,
	cReadRecordStamped				= 0x5D,
	cAbortSecureSession				= 0x5E
};

enum FeliCaCmd
{
	cOpen							= 0x10,
	cPolling						= 0x11,
	cReqeustService					= 0x12,
	cRequestResponse				= 0x13,
	cReadWithoutEncryption			= 0x14,
	cWriteWithoutEncryption			= 0x15,
	cRequestSystemCode				= 0x16,
	cRemoval						= 0x17,
	cClose							= 0x18
};

BBAPI BOOL WINAPI BBICReader_GetVersion( char *pGetVersion, BYTE *pLengthOfData);
BBAPI BOOL WINAPI BBICReader_Open( );
BBAPI BOOL WINAPI BBICReader_Close( );
BBAPI BOOL WINAPI BBICReader_PowerOn( int nSlotNum, BYTE *pbyResultBuf );
BBAPI BOOL WINAPI BBICReader_PowerDown( int nSlotNum, BYTE *pbyResultBuf );
BBAPI BOOL WINAPI BBICReader_GetStatus( int nSlotNum,  BYTE *pbyResultBuf );
BBAPI BOOL WINAPI BBICReader_SendCommand( int nSlotNum,  BYTE *pbyInputData  );
BBAPI BOOL WINAPI BBICReader_GetSAMData( BYTE *pbyResultBuf );

BBAPI BOOL WINAPI BBICReader_ConnectSyncCard( int nSlotNum, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBICReader_DisconnectSyncCard( int nSlotNum );
BBAPI BOOL WINAPI BBICReader_ReadMemory( int nSlotNum, BYTE* pbyReadInfoBuf, BYTE* pbyResultBuf);
BBAPI BOOL WINAPI BBICReader_UpdateMemory( int nSlotNum, BYTE* pbyWriteInfoBuf, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBICReader_CompareVerificationData( int nSlotNum, BYTE* pbyVerificateInfoBuf, BYTE* pbyResultBuf );


BBAPI BOOL WINAPI BBRFReader_OpenICComm( void );
BBAPI BOOL WINAPI BBRFReader_CloseICComm( void );

BBAPI BOOL WINAPI BBRFReader_ICPowerDown( BOOL bIsSAM, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_ICPowerOn( BOOL bIsSAM, BYTE* pbyInputParam, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_SendSAMCommand( BOOL bIsSAM, BYTE* pbyInputData );
BBAPI BOOL WINAPI BBRFReader_GetSAMData( BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_GetICStatus( BOOL bIsSAM, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_GetICVersion( char* pGetVersion, BYTE* pbyLengthOfData );

//SLE4442
BBAPI BOOL WINAPI BBRFReader_ConnectSyncCard( BOOL bIsSAM, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_DisconnectSyncCard( BOOL bIsSAM );
BBAPI BOOL WINAPI BBRFReader_ReadMemory( BOOL bIsSAM, BYTE* pbyReadInfoBuf, BYTE* pbyResultBuf);
BBAPI BOOL WINAPI BBRFReader_UpdateMemory( BOOL bIsSAM, BYTE* pbyWriteInfoBuf, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_CompareVerificationData( BOOL bIsSAM, BYTE* pbyVerificateInfoBuf, BYTE* pbyResultBuf );

// Deprecated Functions
BBAPI BOOL WINAPI BBRFReader_GetSAMDataTimeout( unsigned char* pbyResultBuf, DWORD timeout );

// Dummy Functions
BBAPI BOOL WINAPI BBRFReader_ICChangePPS( BOOL bIsSAM, BYTE* pbyInputParam, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_ICChangebaudrate( DWORD dwBaudRate, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_SAMSlotIOMode( BOOL bIsSAM, BYTE byMode, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_SAMDefType( BYTE bySAMSelectType, BYTE bySlotSAM, BYTE* pbyResultBuf );
BBAPI BOOL WINAPI BBRFReader_ICDefType( BYTE bySAMSelectType, BYTE* pbyResultBuf );




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contactless ( RFID Card )

typedef struct _CommSettings
{
	DWORD			baudrate;
	char 			protocol;
}
CommSettings;


typedef struct _ReaderSettings
{
//	CommSettings	commSettings;
	DWORD			baudrate;
	char 			protocol;
	unsigned char 	stationID;
}
ReaderSettings;


BBAPI char WINAPI BBRFReader_OpenComm( const char* commDevice, char autoDetect, const CommSettings* commSettings );
BBAPI void  WINAPI BBRFReader_CloseComm( void );

BBAPI DWORD WINAPI BBRFReader_OpenReader( unsigned char id, short knownReader );
BBAPI DWORD WINAPI BBRFReader_CloseReader( void );

BBAPI DWORD WINAPI BBRFReader_SendCommand( char* readerCmd, unsigned char *data );
BBAPI DWORD WINAPI BBRFReader_SendCommandGetData( char *readerCmd, unsigned char* data, unsigned char* resultBuf );

BBAPI DWORD WINAPI BBRFReader_GetData( unsigned char* resultBuf );

BBAPI DWORD WINAPI BBRFReader_Calypso( byte command, const unsigned char* data, unsigned char* resultBuf );
BBAPI DWORD	WINAPI BBRFReader_FeliCa( byte cCommand, const unsigned char* pbyInputData, unsigned char* pbyResultBuf );

// Deprecated Functions
BBAPI DWORD WINAPI BBRFReader_GetDataTimeout( unsigned char* buffer, DWORD timeout );
BBAPI DWORD WINAPI BBRFReader_SendCommandGetDataTimeout( char* readerCmd, unsigned char* data, unsigned char* resultBuf, long timeout);

// Removed Functions
// This function will return -1 or NULL, if function should return value.
// BBAPI void WINAPI BBRFReader_EmptyCommRcvBuffer();
// BBAPI char WINAPI BBRFReader_GetResumeState();
// BBAPI void WINAPI BBRFReader_SetCommBaudrate( DWORD dwBaudrate );
// BBAPI DWORD WINAPI BBRFReader_GetCommBaudrate();
// BBAPI char WINAPI BBRFReader_GetCommProtocol();
// BBAPI void WINAPI BBRFReader_SetCommProtocol( char protocol );
// BBAPI void WINAPI BBRFReader_SetCommTimeout( DWORD timeout );
// BBAPI DWORD WINAPI BBRFReader_GetCommTimeout();
// BBAPI ReaderSettings WINAPI BBRFReader_GetReaderConfig();
// BBAPI unsigned char* WINAPI BBRFReader_GetReaderType( unsigned char *deviceVersion );
// BBAPI char WINAPI BBRFReader_GetDebugOutputState();
// BBAPI void WINAPI BBRFReader_SetDebugOutputState( char active );
// BBAPI char* WINAPI BBRFReader_GetDebugOutput( char *buffer );




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buzzer

#define BB_BUZZER_RE			0
#define BB_BUZZER_MI			1
#define BB_BUZZER_FA			2
#define BB_BUZZER_OFF			3
#define BB_BUZZER_MIDDLE_RE		4
#define BB_BUZZER_MIDDLE_MI		5
#define BB_BUZZER_MIDDLE_FA		6
#define BB_BUZZER_LOW_RE		7
#define BB_BUZZER_LOW_MI		8
#define BB_BUZZER_LOW_FA		9

// Buzzer Error Code
#define BB_BUZZER_OPEN_FAIL			1
#define BB_BUZZER_INVALIDHANDLE			2

typedef	LPVOID HBBBUZZER;

BBAPI HBBBUZZER WINAPI BBBuzzerOpen( void );
BBAPI void WINAPI BBBuzzerClose( HBBBUZZER hBuzzer );
BBAPI BOOL WINAPI BBBuzzerOn( unsigned char nBuzzer, unsigned int nInterval );
/**********************************camera////////////////////////////

*/

/*
 *	camera
 */
#define WM_BBCAMERA_UPDATE_FPS					WM_USER+1000

#define BB_CAMERA_CODE_SUCCESS                  0
#define BB_CAMERA_CODE_ERR_UNKNOWN				1
#define BB_CAMERA_CODE_ERR_INVALID_HANDLE      	2
#define BB_CAMERA_CODE_ERR_INVALID_PARAMETER   	3

typedef enum
{
	wb_auto = 0,
	wb_cloudy,
	wb_daylight,
	wb_fluorescent_1,
	wb_fluorescent_2,
	wb_light_bulb

} white_balance_mode;

typedef enum
{
	effect_none = 0,
	effect_negative,
	effect_embossing,
	effect_black_n_white,
	effect_sketch,
	effect_solarization,
	effect_sephia,
	effect_aqua,
	effect_posterize,
	effect_warm,
	effect_cool,
	effect_antique,
	effect_moonlight,
	effect_fog,
	effect_gray,
	effect_violet,
} effect_mode;

typedef enum
{
	flip_x = 1,
	flip_y,
	flip_x_y,
	flip_origin


} flip_mode;

typedef enum
{
	rate_auto = 1,
	rate_day_manual,
	rate_night_manual
} frame_rate_mode;

typedef enum
{
	auto_save = 0,
	manual_save
} save_mode;

typedef enum
{
	file_type_jpg = 0,
	file_type_bmp
} save_image_type;

typedef enum
{
	rotate_default=0,
	rotate_0,
	rotate_90,
	rotate_180,
	rotate_270

} rotation_degree;

typedef enum
{
	pixel_format_ycbcr = 0,
	pixel_format_rgb_565,		// output data 가 bmp 이다.
	pixel_format_rgb_565_raw	// output data 가 16bit rgb 데이터 이다.

} pixel_format_select;


typedef struct _tagBBCameraParameter
{
	unsigned int preview_width;
	unsigned int preview_height;
	unsigned int preview_x;
	unsigned int preview_y;
	unsigned int preview_format;
	unsigned int preview_zoom;

	const TCHAR* capture_file_name;
	unsigned int capture_width;
	unsigned int capture_height;
	pixel_format_select capture_format;
	unsigned int capture_strobe_on;

	unsigned int contrast;
	unsigned int saturation;
	unsigned int brightness;
	effect_mode ef_mode;
	white_balance_mode wb_mode;
	flip_mode	fp_mode;
	frame_rate_mode fr_mode;

	unsigned int reserved[20];
	BYTE* 		p_app_buffer;
	BYTE* 		p_app_capture_buffer;	//still image capture에서 데이터를 받기위한 포인터

	save_mode			stillimage_savemode;
	save_image_type		save_fileType;
	unsigned int		ImageQuality;

	BOOL				isSaveFile;
	rotation_degree		rotation;

} BBCameraParameter;

typedef struct _tagBBCameraInfo
{
	unsigned int preview_max_width;
	unsigned int preview_max_height;
	unsigned int preview_min_width;
	unsigned int preview_min_height;
	unsigned int image_max_width;
	unsigned int image_max_height;

	unsigned int reserved[20];

} BBCameraInfo;


typedef LPVOID HBBCAMERA;

BBAPI HBBCAMERA WINAPI BBCameraOpen(HWND);
BBAPI DWORD WINAPI BBCameraClose(HBBCAMERA);

BBAPI DWORD WINAPI BBCameraPreviewStart(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraPreviewStop(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraPreviewPause(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraPreviewResume(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraPreviewZoom(HBBCAMERA, BBCameraParameter*);

BBAPI DWORD WINAPI BBCameraGetRawDataStart(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraGetRawDataStop(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraGetRawDataPause(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraGetRawDataResume(HBBCAMERA);
BBAPI DWORD WINAPI BBCameraGetRawDataZoom(HBBCAMERA, BBCameraParameter*);

BBAPI DWORD WINAPI BBCameraCapture(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraStoreCaptureImage(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraRestartPreviewFromCapture(HBBCAMERA, BBCameraParameter*);

BBAPI DWORD WINAPI BBCameraSetWhiteBalance(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraSetContrast(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraSetSaturation(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraSetBrightness(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraSetEffect(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraSetFlip(HBBCAMERA, BBCameraParameter*);
BBAPI DWORD WINAPI BBCameraNightMode(HBBCAMERA, BBCameraParameter*);

BBAPI DWORD WINAPI BBCameraGetInfo(HBBCAMERA, BBCameraInfo*);
BBAPI DWORD WINAPI BBCameraDumpRegister(HBBCAMERA);

BBAPI DWORD WINAPI BBCameraSetStrobe(HBBCAMERA, BOOL);
BBAPI DWORD WINAPI BBCameraSetAEMode(HBBCAMERA, BOOL);
BBAPI DWORD WINAPI BBCameraSetExposureTime( HBBCAMERA, DWORD );
BBAPI DWORD WINAPI BBCameraSetFlash(HBBCAMERA, DWORD);
BBAPI BOOL WINAPI BBCameraSetFocus();



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// barcode

//duration
#define WM_SCANDECODEDATA   ( WM_USER + 701 )
#define WM_SCANTRIGGER      ( WM_USER + 702 )
#define WM_REPORT_TO_APP	( WM_USER + 703 )

enum
{
	BB_BARCODE_RPT_USER_MSG,
};

enum
{
	BB_BARCODE_UM_BATTERY_LOW,
};

#define BB_BARCODE_DUR_SHORT 0
#define BB_BARCODE_DUR_LONG  1
#define BB_BARCODE_DUR_FASTWARBLE 2
#define BB_BARCODE_DUR_SLOWWARBLE 3
#define BB_BARCODE_DUR_MIX1 4
#define BB_BARCODE_DUR_MIX2 5
#define BB_BARCODE_DUR_MIX3 6
#define BB_BARCODE_DUR_MIX4 7

//return code
#define BB_BARCODE_SC_SUCCESS  0   // 일반적인 성공시의 code
#define BB_BARCODE_SC_ERR_NORMAL 1         // 일반적인 Error시의 code
#define BB_BARCODE_SC_ERR_SERIALPORT 2     // Serial Port와 관련된 Error
#define BB_BARCODE_SC_ERR_POWERMODE  3     // Power mode Setting Error
#define BB_BARCODE_SC_ERR_SCANENABLE 4     // SCAN_ENABLE/SCAN_DISABLE Setting Error
#define BB_BARCODE_SC_ERR_SCANDISABLE 5    // SCAN이 가능하지 않은 경우 내는 Error
#define BB_BARCODE_SC_ERR_STARTDECODE 6    // START_DECODE Error
#define	BB_BARCODE_SC_ERR_STOPDECODE 7     // STOP_DECODE Error
#define BB_BARCODE_SC_ERR_PARAMREQUEST 8   // PARAM_REQUEST Error
#define	BB_BARCODE_SC_ERR_PARAMSEND 9      // PARAM_SEND Error
#define BB_BARCODE_SC_ERR_PARAMDEFAULTS 10 // PARAM_DEFAULTS Error
#define BB_BARCODE_SC_ERR_LED  11          // LED_ON/LED_OFF Error
#define BB_BARCODE_SC_ERR_BEEP 12          // BEEP Error
#define BB_BARCODE_SC_ERR_SENDPACKET 13    // Error occurred during Sending Packet.
#define BB_BARCODE_SC_ERR_AIM 14           // AIM_ON/OFF Error
#define BB_BARCODE_SC_ERR_NOBUFFER 15      // Buffer 크기가 작은 경우 error
#define BB_BARCODE_SC_ERR_SCANCLOSE 16     // Scan Close 도중에 생긴 error
#define BB_BARCODE_SC_ERR_TOOBIGSIZE 17    // 보낼 data의 size가 너무 클 경우의 error
#define BB_BARCODE_SC_ERR_NOPARAMVALUE 18  //  찾고자하는 PARAMETER의 VALUE가 없을때 생기는 error
#define BB_BARCODE_SC_ERR_NOHANDLEGRABBER 19 // 현재 Barcode를 점유하는 handle이 없을 경우 error

// barcode symbology
#define BB_BARCODE_NOTAPPLICABLE        0x00
#define BB_BARCODE_UPC_A                0x11
#define BB_BARCODE_UPC_E                0x12
#define BB_BARCODE_UPC_E1               0x13
#define BB_BARCODE_EAN8                 0x14
#define BB_BARCODE_EAN13                0x15
#define BB_BARCODE_SUPPLEMENTCODE       0x16
#define BB_BARCODE_UCC_COUPON_EXTENDED  0x17
#define BB_BARCODE_CODE39               0x18
#define BB_BARCODE_CODE93               0x19
#define BB_BARCODE_CODE128              0x1A
#define BB_BARCODE_INTERLEAVED2OF5      0x1B
#define BB_BARCODE_INDUSTRIAL2OF5       0x1C
#define BB_BARCODE_CODABAR              0x1D
#define BB_BARCODE_KOREAN_POST          0x1E
#define BB_BARCODE_CODE11               0x1F
#define BB_BARCODE_MSI                  0x20
#define BB_BARCODE_CHINESE_POST         0x21
#define BB_BARCODE_RSS                  0x22
#define BB_BARCODE_PDF417               0x23
#define BB_BARCODE_ISBT128              0x24
#define BB_BARCODE_IATA25               0x25
#define BB_BARCODE_TELEPEN              0x26
#define BB_BARCODE_MATRIX2OF5           0x27
#define BB_BARCODE_COMPOSITE            0x28
#define BB_BARCODE_DATAMATRIX           0x29
#define BB_BARCODE_MAXICODE             0x2A
#define BB_BARCODE_AZTECCODE            0x2B
#define BB_BARCODE_MICROPDF             0x2C
#define BB_BARCODE_QRCODE               0x2D
#define BB_BARCODE_TRIOPTICCODE         0x2E
#define BB_BARCODE_PLESSEY              0x2F
#define BB_BARCODE_CODE32               0x30
#define BB_BARCODE_POSICODE             0x31
#define BB_BARCODE_JAPANESE_POST        0x32
#define BB_BARCODE_AUSTRALIAN_POST      0x33
#define BB_BARCODE_BRITISH_POST         0x34
#define BB_BARCODE_CANADIAN_POST        0x35
#define BB_BARCODE_NETHERLANDS_POST     0x36
#define BB_BARCODE_POSTNET              0x37
#define BB_BARCODE_OCR                  0x38
#define BB_BARCODE_AZTEC_MESA           0x39
#define BB_BARCODE_CODE49               0x3A
#define BB_BARCODE_CODABLOCK            0x3B
#define BB_BARCODE_PLANET               0x3C
#define BB_BARCODE_TLC39                0x3D
#define BB_BARCODE_STRAIGHT2OF5         0x3E
#define BB_BARCODE_CODE16K              0x3F
#define BB_BARCODE_DISCRETE2OF5         0x40
#define BB_BARCODE_UK_PLESSEY           0x41
#define BB_BARCODE_AZTEC_RUNES          0x42
#define BB_BARCODE_USPS4CB              0x43
#define BB_BARCODE_IDTAG                0x44
#define BB_BARCODE_RSS14                0x45
#define BB_BARCODE_RSS_LIMITED          0x46
#define BB_BARCODE_RSS_EXPANDED         0x47
#define BB_BARCODE_GERMAN_POST          0x48
#define BB_BARCODE_HANXIN               0x49
#define BB_BARCODE_GS1_128              0x4A
#define BB_BARCODE_EAN128               0x4B
#define BB_BARCODE_BOOKLAND_EAN         0x4C	//SE4500용 추가
#define BB_BARCODE_USER_SUPPLEMENTAL1	0x4D	//SE4500용 추가
#define BB_BARCODE_USER_SUPPLEMENTAL2	0x4E	//SE4500용 추가
#define BB_BARCODE_DUMMY				0x69	//SE4500용 추가



//OPTION
#define BB_BARCODE_OPT_SYMBOLOGY                            0x00
#define BB_BARCODE_OPT_ABC_CODE_ONLY                        0x01
#define BB_BARCODE_OPT_BOOKLAND                             0x02
#define BB_BARCODE_OPT_CHECK_DIGIT                          0x03
#define BB_BARCODE_OPT_CHECK_DIGIT_ALGORITHM                0x04
#define BB_BARCODE_OPT_CLSI_EDITING                         0x05
#define BB_BARCODE_OPT_COMPOSITE_ON_UCC_EAN                 0x06
#define BB_BARCODE_OPT_CONVERT_A_TO_X                       0x07
#define BB_BARCODE_OPT_CONVERT_CODE39_TO_CODE32             0x08
#define BB_BARCODE_OPT_CONVERT_EAN_8_TO_EAN_13              0x09
#define BB_BARCODE_OPT_CONVERT_INTERLEAVED2OF5_TO_EAN_13    0x0A
#define BB_BARCODE_OPT_CONVERT_RSS_TO_UPC_EAN               0x0B
#define BB_BARCODE_OPT_CONVERT_UPC_E_TO_UPC_A               0x0C
#define BB_BARCODE_OPT_CONVERT_UPC_E1_TO_UPC_A              0x0D
#define BB_BARCODE_OPT_CX_CODE_ONLY                         0x0E
#define BB_BARCODE_OPT_EXPAND_VERSION_E                     0x0F
#define BB_BARCODE_OPT_FONT                                 0x10
#define BB_BARCODE_OPT_FULL_ASCII                           0x11
#define BB_BARCODE_OPT_GROUP_G                              0x12
#define BB_BARCODE_OPT_GROUP_H                              0x13
#define BB_BARCODE_OPT_IGNORE_LINK_FLAG                     0x14
#define BB_BARCODE_OPT_INSERT_SPACE                         0x15
#define BB_BARCODE_OPT_INTER_CHARACTER_GAP_CHECK            0x16
#define BB_BARCODE_OPT_LEADING_ZERO                         0x17
#define BB_BARCODE_OPT_LENGTH_MIN                           0x18
#define BB_BARCODE_OPT_LENGTH_MAX                           0x19
#define BB_BARCODE_OPT_LIMITED                              0x1A
#define BB_BARCODE_OPT_MESA_CODE39                          0x1B
#define BB_BARCODE_OPT_MESA_CODE93                          0x1C
#define BB_BARCODE_OPT_MESA_CODE128                         0x1D
#define BB_BARCODE_OPT_MESA_EAN13                           0x1E
#define BB_BARCODE_OPT_MESA_INTERLEAVED2OF5                 0x1F
#define BB_BARCODE_OPT_MESA_UPC_A                           0x20
#define BB_BARCODE_OPT_NOTIS_EDITING                        0x21
#define BB_BARCODE_OPT_OLDSTYLE                             0x22
#define BB_BARCODE_OPT_OUTPUT_MODE                          0x23
#define BB_BARCODE_OPT_SUPPLEMENTCODE2                      0x24
#define BB_BARCODE_OPT_SUPPLEMENTCODE5                      0x25
#define BB_BARCODE_OPT_SUPPLEMENTCODE_SEPARATOR             0x26
#define BB_BARCODE_OPT_TEMPLATE                             0x27
#define BB_BARCODE_OPT_TRANSMIT_APPLICATION_IDENTIFIER      0x28
#define BB_BARCODE_OPT_TRANSMIT_CHECK_DIGIT                 0x29
#define BB_BARCODE_OPT_TRANSMIT_DASH                        0x2A
#define BB_BARCODE_OPT_TRANSMIT_ISBN                        0x2B
#define BB_BARCODE_OPT_TRANSMIT_ISSN                        0x2C
#define BB_BARCODE_OPT_TRANSMIT_ISMN                        0x2D
#define BB_BARCODE_OPT_TRANSMIT_LEADING_A                   0x2E
#define BB_BARCODE_OPT_TRANSMIT_NUMERIC_SYSTEM_DIGIT        0x2F
#define BB_BARCODE_OPT_TRANSMIT_S_CODE_AS_INTERLEAVED2OF5   0x30
#define BB_BARCODE_OPT_TRANSMIT_START_STOP                  0x31
#define BB_BARCODE_OPT_TRIOPTIC_CODE39                      0x32
#define BB_BARCODE_OPT_UCCEAN_128                           0x33
#define BB_BARCODE_OPT_UPCEAN_SECURITY_LEVEL                0x34
#define BB_BARCODE_OPT_APPEND                               0X35
#define BB_BARCODE_OPT_FNC                                  0x36
#define BB_BARCODE_OPT_TRANSMIT_FNC                         0x37

// N560x OCR에서만 사용
#define BB_BARCODE_OPT_OCR_TEMPLATE                    		0x38
#define BB_BARCODE_OPT_OCR_TEMPLATE_USER_PARAMETER			0x39

//N560x OCR에서만 사용
#define BB_BARCODE_OPT_OCR_TEMPLATE_USER  0x00
#define BB_BARCODE_OPT_OCR_TEMPLATE_PASSPORT  0x01
#define BB_BARCODE_OPT_OCR_TEMPLATE_ISBN  0x02
#define BB_BARCODE_OPT_OCR_TEMPLATE_PRICE_FIELD  0x03
#define BB_BARCODE_OPT_OCR_TEMPLATE_MICRE13B  0x04

#define BB_BARCODE_ENABLE   0x01
#define BB_BARCODE_DISABLE  0x00

#define BB_BARCODE_MODE_TRIGGER         0x00
#define BB_BARCODE_MODE_CONTINUOUS      0x01
#define BB_BARCODE_MODE_MULTIPLE        0x02

enum RevisonType
{
    BB_BARCODE_REVISION_API = 0,
    BB_BARCODE_REVISION_DECODER,
    BB_BARCODE_REVISION_DRIVER
};

typedef LPVOID HBBBARCODE;

BBAPI HBBBARCODE WINAPI BBBarcodeOpen(BOOL bEnableTrigger);
BBAPI DWORD WINAPI BBBarcodeClose(HBBBARCODE hBarcode);

BBAPI DWORD WINAPI BBBarcodeGetDecodeData(HBBBARCODE hBarcode, LPTSTR lpcszBuf, int Bufsize, int* ReadSize );
BBAPI DWORD WINAPI BBBarcodeGetDecodeDataNType(HBBBARCODE hBarcode, LPTSTR lpcszBuf, BYTE* pType, int Bufsize, int* ReadSize);
BBAPI DWORD WINAPI BBBarcodeGetDecodeDataRaw(HBBBARCODE hBarcode, BYTE *pBuf, int Bufsize, int* ReadSize );
BBAPI DWORD WINAPI BBBarcodeGetDecodeDataNTypeRaw(HBBBARCODE hBarcode, BYTE *pBuf, BYTE* pType, int Bufsize, int* ReadSize);
BBAPI DWORD WINAPI BBBarcodeSetClientHandle(HBBBARCODE hBarcode, HWND hwnd );
BBAPI DWORD WINAPI BBBarcodeReleaseClientHandle(HBBBARCODE hBarcode);
BBAPI DWORD WINAPI BBBarcodeGetParameterEx(HBBBARCODE hBarcode, BYTE paramCode, BYTE extraParamCode, BYTE& paramValue);
BBAPI DWORD WINAPI BBBarcodeSetParameterEx(HBBBARCODE hBarcode, BYTE paramCode, BYTE extraParamCode, BYTE paramValue, BOOL bPermanent);
BBAPI DWORD WINAPI BBBarcodeScanEnable(HBBBARCODE hBarcode, BOOL bEnable);
BBAPI BOOL WINAPI BBBarcodeGetTriggerState(HBBBARCODE hBarcode);
BBAPI DWORD WINAPI BBBarcodeSetTrigger(HBBBARCODE hBarcode, BOOL bOn);
BBAPI DWORD WINAPI BBBarcodeGetRevision(HBBBARCODE hBarcode, BYTE bRevisionType, TCHAR *wcRevision);

BBAPI BOOL WINAPI BBBarcodeGetVirtualWedge(HBBBARCODE hBarcode);
BBAPI DWORD WINAPI BBBarcodeSetVirtualWedge(HBBBARCODE hBarcode, BOOL bOn);
BBAPI DWORD WINAPI BBBarcodeGetNotifyMessage(HBBBARCODE hBarcode, unsigned int *unMessage);
BBAPI DWORD WINAPI BBBarcodeSetNotifyMessage(HBBBARCODE hBarcode, unsigned int unMessage);
BBAPI DWORD WINAPI BBBarcodeGetDecodeMode(HBBBARCODE hBarcode, BYTE &bMode);
BBAPI DWORD WINAPI BBBarcodeSetDecodeMode(HBBBARCODE hBarcode, BYTE bMode);
BBAPI DWORD WINAPI BBBarcodeSetTimeoutBetweenSameSymbol(HBBBARCODE hBarcode, DWORD dwMillisecond);
BBAPI DWORD WINAPI BBBarcodeSetDecodeOnTime(HBBBARCODE hBarcode,DWORD dwDeciSecond);
BBAPI DWORD WINAPI BBBarcodeSetPushingTriggerMode(HBBBARCODE hBarcode, BYTE bPushingTriggerMode);

BBAPI DWORD WINAPI BBBarcodeSymbologyAllEnable(HBBBARCODE hBarcode);
BBAPI DWORD WINAPI BBBarcodeSymbologyAllDisable(HBBBARCODE hBarcode);

//ImagerBarcode only
typedef enum {
    SCAN_ILLUM_AIMER_OFF = 0,   // Neither aimers or illumination
    SCAN_ILLUM_ONLY_ON,         // Illumination only
    SCAN_AIMER_ONLY_ON,         // Aimers only
    SCAN_ILLUM_AIMER_ON         // Both aimers and illumination.
} ScanIlluminat_t;

typedef enum {
    DECODE_MODE_STANDARD=1,
    DECODE_MODE_ADVANCED_LINEAR,
    DECODE_MODE_QUICK_OMNI=4
} ImagerDecodeMode_t;

typedef struct {
    int Exposure;
    int MaxExposure;
    int Gain;
    int MaxGain;
    int TargetWhite;            // Acceptable target white value falls within the
    int TargetWhiteWindow;      // range TargetWhite +/- TargetWhiteWindow
    int ImageMustConform;       // Image must conform to defined exposure parameters
    int NumUpdates;             // Max number of attempts to achieve target white value
    int FrameRate;
    int SpecExclusion;
    int SpecSaturation;
    int SpecLimit;
    int FixedExposure;          // Exposure setting for fixed exposure mode
    int FixedGain;              // Gain setting for fixed exposure mode
    int FixedFrameRate;         // Frame rate for fixed exposure mode
} ExposureSettings_t;

typedef struct {

    DWORD dwCols;
    DWORD dwRows;
    DWORD dwBits;
} ImagerInfo_t;


BBAPI DWORD WINAPI BBBarcodeImagerGetImagerInfo(HBBBARCODE hBarcode, PWORD pnCols, PWORD pnRows, PWORD pnBits);
BBAPI DWORD WINAPI BBBarcodeImagerStreamInit(HBBBARCODE hBarcode, WORD nSkip, RECT * imgRect, BOOL bFlip);
BBAPI DWORD WINAPI BBBarcodeImagerStreamStart(HBBBARCODE hBarcode, RECT *previewRect);
BBAPI DWORD WINAPI BBBarcodeImagerStreamStop(HBBBARCODE hBarcode);
BBAPI DWORD WINAPI BBBarcodeImagerStreamCapture(HBBBARCODE hBarcode, TCHAR *filename, unsigned int nFileWidth, unsigned int nFileHeight, BOOL bAutoSave=FALSE);
BBAPI DWORD WINAPI BBBarcodeImagerSetExposureSettings(HBBBARCODE hBarcode, ExposureSettings_t *pExpsoureSettings);
BBAPI DWORD WINAPI BBBarcodeImagerGetExposureSettings(HBBBARCODE hBarcode, ExposureSettings_t *pExpsoureSettings);
BBAPI DWORD WINAPI BBBarcodeImagerSetScanningLightMode(HBBBARCODE hBarcode, ScanIlluminat_t LightMode);
BBAPI DWORD WINAPI BBBarcodeImagerLeaveLightsOn(HBBBARCODE hBarcode, BOOL bLeaveLightOn);
BBAPI DWORD WINAPI BBBarcodeImagerSetDecodeMode(HBBBARCODE hBarcode, ImagerDecodeMode_t DecodeMode);
BBAPI DWORD WINAPI BBBarcodeImagerSetInverseMode(HBBBARCODE hBarcode, BOOL bInverseMode);
BBAPI DWORD WINAPI BBBarcodeImagerGetDecodeCenteringWindow(HBBBARCODE hBarcode, BOOL &bCenteringWindow, RECT &rect);
BBAPI DWORD WINAPI BBBarcodeImagerSetDecodeCenteringWindow(HBBBARCODE hBarcode, BOOL bCenteringWindow, RECT rect);


/*
BBAPI BOOL WINAPI BBImagerStreamN560xIsOpened();
BBAPI DWORD WINAPI BBImagerStreamN560xGetImagerInfo(ImagerInfo_t *Imagerinfo);
BBAPI DWORD WINAPI BBImagerStreamN560xOpen();
BBAPI DWORD WINAPI BBImagerStreamN560xClose();
BBAPI DWORD WINAPI BBImagerStreamN560xInit(WORD nSkip, RECT* imgRect, BOOL bFlip);
BBAPI DWORD WINAPI BBImagerStreamN560xStart(RECT *previewRect);
BBAPI DWORD WINAPI BBImagerStreamN560xStop();
BBAPI DWORD WINAPI BBImagerStreamN560xPause();
BBAPI DWORD WINAPI BBImagerStreamN560xResume();
BBAPI DWORD WINAPI BBImagerStreamN560xCapture(TCHAR* filename, unsigned int nFileWidth, unsigned int nFileHeight, BOOL bAutoSave);
*/
// C#의 Wrapper Dll에서 Reading이 다 되었다는 것을 받기 위해 기다리는 함수
DWORD WINAPI BBBarcodeWaitReading(HBBBARCODE hBarcode);

// code by coffriend - 2003.11.17
// : Amble/Fix 등 일일이 설정해 줄 경우 귀찮은 값들을 API를 써서
//   쉽게 설정할 수 있도록 한다.
BBAPI DWORD WINAPI BBBarcodeSetPreamble(HBBBARCODE hBarcode, LPTSTR szPreamble);
BBAPI DWORD WINAPI BBBarcodeSetPostamble(HBBBARCODE hBarcode, LPTSTR szPostamble);
BBAPI DWORD WINAPI BBBarcodeSetPrefix(HBBBARCODE hBarcode, LPTSTR szPrefix);
BBAPI DWORD WINAPI BBBarcodeSetSuffix(HBBBARCODE hBarcode, LPTSTR szSuffix);
BBAPI DWORD WINAPI BBBarcodeSetOutputMode(HBBBARCODE hBarcode, BOOL bClipboard);
BBAPI DWORD WINAPI BBBarcodeSetVolume(int nVolume);
// 이 API를 호출하면 Setting을 Update한다.
BBAPI DWORD WINAPI BBBarcodeUpdateSetting(HBBBARCODE hBarcode);
// coffriend end - 2003.11.17

// xml 파일 설정 정보 start

typedef const char* PCCHAR;
#define MAX_TEXT_BUFFER 256


// xml 파일 설정을 위한 구조체
typedef struct
{
	DWORD ItemIndex;
	const char* item_name;
	const char* group_name;
	const char* parameter_name;
	const char* value_name;
	DWORD type;
	BOOL IsValueDword;
	BOOL IsValueWChar;
	union
	{
		DWORD value_dword; //- PARAM_NUMERIC, PARAM_SLIDER, PARAM_CHAR
		TCHAR value_wcharbuffer[MAX_TEXT_BUFFER]; //PARAM_TEXT, PARAM_PATH, PARAM_READONLY
	};

}SettingInfo;

enum ItemIndex
{
	VWEDGETABINDEX = 0,
	SYMBOLOGYTABINDEX,
	SCANNERTABINDEX,
	DATAOPTIONTABINDEX,
	IMGCAPTABINDEX,
    REVISIONTABINDEX
};

enum ParamType {
    PARAM_NUMERIC = 0,   //numeric value(숫자값) 선택
	PARAM_OPTION,        //option(여러개중 하나를 선택)
	PARAM_ENABLE,        //enable/disable 중 하나 선택
	PARAM_CHAR,          //character값
	PARAM_TEXT,          //text값(scanner에 setting할 값은 아니다.)
	PARAM_PATH,			 // file path - add csyou
	PARAM_SLIDER,		 // Slider - add csyou
	PARAM_NUMERIC_TIME,	 // continues time(between same symbol)
	PARAM_NULL,           //not a parameter type
    PARAM_READONLY
};


enum SoundMode{
    BB_BARCODE_SOUND_WAVE = 0,
    BB_BARCODE_SOUND_VIBRATE,
    BB_BARCODE_SOUND_WAVE_VIBRATE,
    BB_BARCODE_SOUND_EXTERNAL,
    BB_BARCODE_SOUND_ERROR
};

enum VolumeMode{
    BB_BARCODE_VOLUME_SYSTEM = 0,
    BB_BARCODE_VOLUME_BARCODE
};

// xml 파일 설정 정보 end

// xml 파일에서 정보를 얻어오는 API
BBAPI PCCHAR WINAPI BBBarcodeGetNextItemName(HBBBARCODE hBarcode, const char* item_name,BOOL* is_ignore_item);
BBAPI BOOL WINAPI BBBarcodeGetItemString(HBBBARCODE hBarcode, const char* item_name, TCHAR* pbuffer, unsigned int buffer_length);
BBAPI BOOL WINAPI BBBarcodeGetItemNumber(HBBBARCODE hBarcode, const char* item_name, unsigned int* item_number);
BBAPI PCCHAR WINAPI BBBarcodeGetNextGroupName(HBBBARCODE hBarcode, const char* item_name, const char* group_name, BOOL* is_ignore_group);
BBAPI BOOL WINAPI BBBarcodeGetGroupString(HBBBARCODE hBarcode, const char* item_name, const char* group_name, TCHAR* pbuffer, unsigned int buffer_length);
BBAPI PCCHAR WINAPI BBBarcodeGetNextParameterName(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, BOOL* is_ignore_parameter);
BBAPI BOOL WINAPI BBBarcodeGetParameterString(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, TCHAR* pbuffer, unsigned int buffer_length);
BBAPI BOOL WINAPI BBBarcodeGetParameterState(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, TCHAR* pbuffer, unsigned int buffer_length);
BBAPI BOOL WINAPI BBBarcodeGetParameterType(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, unsigned int* type );
BBAPI PCCHAR WINAPI BBBarcodeGetParameterNextOptionName(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, const char* option_name);
BBAPI BOOL WINAPI BBBarcodeGetParameterOptionString(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, const char* option_name, TCHAR* pbuffer, unsigned int buffer_length);
BBAPI BOOL WINAPI BBBarcodeSetParameterValue(HBBBARCODE hBarcode, SettingInfo* settinginfo);
BBAPI BOOL WINAPI BBBarcodeXmlFileSave(HBBBARCODE hBarcode);
BBAPI BOOL WINAPI BBBarcodeInitSetting(HBBBARCODE hBarcode);

BBAPI BOOL WINAPI BBBarcodeGetCanWaveUse(HBBBARCODE hBarcode);
BBAPI BOOL WINAPI BBBarcodeGetVolume(HBBBARCODE hBarcode, DWORD* volume);
BBAPI BOOL WINAPI BBBarcodeGetWaveFilePath(HBBBARCODE hBarcode, TCHAR* file_path, unsigned int file_path_length);

BBAPI PCCHAR WINAPI BBBarcodeGetParameterStateChar(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name);
BBAPI DWORD WINAPI BBBarcodeGetParameterStateDword(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name, DWORD* dwvalue);
BBAPI PCCHAR WINAPI BBBarcodeGetParameterStateName(HBBBARCODE hBarcode, const char* item_name, const char* group_name, const char* parameter_name);
BBAPI BOOL WINAPI BBBarcodeReadDefaultFile(HBBBARCODE hBarcode);
BBAPI BOOL WINAPI BBBarcodeCloseDefaultFile(HBBBARCODE hBarcode);

BBAPI BOOL WINAPI BBBarcodeSetSoundMode(HBBBARCODE hBarcode, SoundMode nSoundMode);
BBAPI BOOL WINAPI BBBarcodeGetSoundMode(HBBBARCODE hBarcode, SoundMode &nSoundMode);
BBAPI BOOL WINAPI BBBarcodeSetVibrateTimeout(HBBBARCODE hBarcode, DWORD dwVibrateTimeout);
BBAPI BOOL WINAPI BBBarcodeGetVibrateTimeout(HBBBARCODE hBarcode, DWORD &dwVibrateTimeout);
BBAPI BOOL WINAPI BBBarcodeGetVolumeMode(HBBBARCODE hBarcode, VolumeMode &nVolumeMode);
BBAPI BOOL WINAPI BBBarcodeSetVolumeMode(HBBBARCODE hBarcode, VolumeMode nVolumeMode);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modem & Phone

#define BB_MODEM_STATUS_UNKNOWN	0
#define BB_MODEM_STATUS_ON		1
#define BB_MODEM_STATUS_OFF		2

// Modem type
#define BB_MODEM_NONE			(0x00000000)
#define BB_MODEM_BGS2			(0x00010001)
#define BB_MODEM_PHS8			(0x00010002)
#define BB_MODEM_M320			(0x00010003)
#define BB_MODEM_EHS5			(0x00010004)

// Network attach status
#define BB_REG_NOT_ATTACH		0
#define BB_REG_CS_ATTACH		1
#define BB_REG_PS_ATTACH		2
#define BB_REG_BOTH_ATTACH		3

/// Network Selection Mode
#define BB_OP_AUTO_MODE			0
#define BB_OP_MANUAL_MODE		1

/// SIM status
#define BB_SIM_UNKNOWN			0
#define BB_SIM_NOT_INSERT		1
#define BB_SIM_BLOCKED			2
#define BB_SIM_PIN				3
#define BB_SIM_PUK				4
#define BB_SIM_READY			5

/// SIM Lock Status
#define BB_STAT_UNLOCK			0
#define BB_STAT_LOCK			1

/// Radio System
#define BB_RADIO_NONE			0
#define BB_RADIO_1X 			1
#define BB_RADIO_2G 			2
#define BB_RADIO_3G 			3

typedef struct _BBPhoneOperator_tag
{
	UINT nOpStatus;
	WCHAR wszAlphaOpName[32];
	WCHAR wszNumericOpName[32];
}
BBPhoneOperator;

typedef	LPVOID HBBMODEM;
typedef LPVOID HBBPHONE;


// Modem
BBAPI HBBMODEM WINAPI BBModemOpen( void );
BBAPI void WINAPI BBModemClose( HBBMODEM hModem );

BBAPI BOOL WINAPI BBModemOn( HBBMODEM hModem );
BBAPI BOOL WINAPI BBModemOff( HBBMODEM hModem );
BBAPI BOOL WINAPI BBModemGetStatus( HBBMODEM hModem, UINT* pnModemStatus );


// Phone
BBAPI HBBPHONE WINAPI BBPhoneOpen( HWND hWndCallback, UINT nNotifyEventCode, DWORD dwAnswerWaitTime, DWORD* pdwResult, DWORD dwReserved );
BBAPI BOOL WINAPI BBPhoneClose( HBBPHONE hPhone );

//		AT cmd test API
BBAPI BOOL WINAPI BBPhoneSendATCommand( HBBPHONE hPhone, LPTSTR lpATCmd, LPTSTR lpResponse, UINT nRspBuffSize);

//		Modem config API
BBAPI BOOL WINAPI BBPhoneModemInitial( HBBPHONE hPhone );
BBAPI BOOL WINAPI BBPhoneGetModemVersion( HBBPHONE hPhone, LPTSTR lpVersion, UINT nDestSize );
BBAPI BOOL WINAPI BBPhoneGetPhoneNumber( HBBPHONE hPhone, LPTSTR lpPhoneNumber, UINT nDestSize );
BBAPI BOOL WINAPI BBPhoneSendATH( HBBPHONE hPhone );

//		Network API
BBAPI BOOL WINAPI BBPhoneIsRegistered( HBBPHONE hPhone, UINT* pnStsReg );
BBAPI BOOL WINAPI BBPhoneGetRSSI( HBBPHONE hPhone, int* piRSSIValue );
BBAPI BOOL WINAPI BBPhoneGetOperatorMode( HBBPHONE hPhone, UINT* pnMode );
BBAPI BOOL WINAPI BBPhoneGetOperatorList( HBBPHONE hPhone, BBPhoneOperator* pstOperator, UINT* pnOpCount );
BBAPI BOOL WINAPI BBPhoneSelectOperator( HBBPHONE hPhone, UINT nMode, LPTSTR lpOpName );
BBAPI BOOL WINAPI BBPhoneGetRadioSys( HBBPHONE hPhone, UINT* pnBand );

// 		SIM API
BBAPI BOOL WINAPI BBPhoneGetSimStatus( HBBPHONE hPhone, UINT* pnSimstatus );
BBAPI BOOL WINAPI BBPhoneSetLockingStatus( HBBPHONE hPhone, LPTSTR lpPassword, BOOL bLocking );
BBAPI BOOL WINAPI BBPhoneGetLockingStatus( HBBPHONE hPhone, UINT* pnStatus );
BBAPI BOOL WINAPI BBPhonePinUnlock( HBBPHONE hPhone, LPTSTR lpPassword );
BBAPI BOOL WINAPI BBPhonePukUnlock( HBBPHONE hPhone, LPTSTR lpPukPassword, LPTSTR lpPinPassword );
BBAPI BOOL WINAPI BBPhoneGetPinCount( HBBPHONE hPhone, UINT* pnCount );
BBAPI BOOL WINAPI BBPhoneChangePassword( HBBPHONE hPhone, LPTSTR lpOldPassword, LPTSTR lpNewPassword );

//		Data Call
BBAPI BOOL WINAPI BBPhoneSetAPN( HBBPHONE hPhone, LPTSTR lpAPN );
BBAPI BOOL WINAPI BBPhoneGetAPN( HBBPHONE hPhone, LPTSTR lpAPN, UINT nDestSize );

//		Get IMEI
BBAPI BOOL WINAPI BBPhoneGetIMEI( HBBPHONE hPhone, LPTSTR lpIMEI, UINT nDestSize );

//		2G& 3G
BBAPI BOOL WINAPI BBPhoneSelectData( HBBPHONE hPhone, UINT pnDataStatus );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KeyMapping
#define		VKEY_SHIFT_ENAVLE			    	0x100

#define		KEY_TYPE_NONE				    	0x00
#define		KEY_TYPE_ONETIME			    	0x01	//	keyboard(one time)
#define		KEY_TYPE_REPEAT				    	0x02	//	keyboard + repeat
#define		KEY_TYPE_LONG_N_SHORT		    	0x03	//	long & short
#define		KEY_TYPE_DOWN_N_UP			    	0x04	//	down & up
#define		KEY_TYPE_USER_DEFINE		    	0x05	//	user define
#define		KEY_TYPE_CHANGE_IM			    	0x06	//	change im mode
#define		KEY_TYPE_T_KEY_WITH_IM		    	0x07	//	vk_t with im key
#define		KEY_TYPE_T_KEY_WITHOUT_IM	    	0x08	//	vk_t without im key
#define		KEY_TYPE_KB_WITH_IM			    	0x09	//	keyboard with im key
#define		KEY_TYPE_TPOUND_N_KEYLOCK	    	0x0A	//	vk_tpound + vk_keylock을 위한 key
#define		KEY_TYPE_TPOUND_N_DZ		    	0x0B	//	vk_tpound + '00'을 위한 key
#define		KEY_TYPE_TSTAR_N_PERIOD		    	0x0C	//	vk_tstar + '.'을 위한 key
#define		KEY_TYPE_TSTAR_N_VIBRATE	    	0x0D	//	vk_tstar + sound(vibrate)
#define		KEY_TYPE_TSTAR_N_VOLUMEOFF	    	0x0E	//	vk_tstar + sound(volume off)
#define		KEY_TYPE_TEND_N_PHONEOFF	    	0x0F	//	vk_tend + phone off
#define		KEY_TYPE_IM_N_TP_N_KL		    	0x10	//	change im mode + vk_tpound + vk_keylock
#define		KEY_TYPE_REPEAT_WITH_IM		    	0x11	//	im 상태 확인 후 keyboard + repeat
#define		KEY_TYPE_IM_N_TZERO_N_SVC	    	0x12	//	skt irm규격을 위한 vk_t0 + app 실행 key
#define		KEY_TYPE_SOMEKEY_N_VIBRATE	    	0x13	//	some key + sound(vibrate)
#define		KEY_TYPE_DOUBLE				    	0x14	//	key가 연속으로 2번 눌렸을 때
#define		KEY_TYPE_CHANGE_MODE		    	0x16	//	key  mode를 설정하는 있는 key
#define		KEY_TYPE_TEL_VOLUME			    	0x17	//  통화중일때는 볼륨 기능으로 작동하기 위한 key
#define		KEY_TYPE_T_KEY_WITH_IM_LONG			0x18	//	vk_t with im key + LongKey 기능
#define		KEY_TYPE_APPLICATION_ADDKEY			0x19	//	어플리케이션 실행키로 6개 이상 지원시 사용.
#define 	KEY_TYPE_LONG_N_SHORT_HOLD			0x1A	// 	HOLD키의 동작방식 설정을 위해 추가 2010/08/16

#define		USER_TYPE_KEY_PROGRAM_RUN			0x01	// KEY_TYPE_USER_DEFINE 일때 program run 기능을 위한 define
#define		USER_TYPE_KEY_SET_EVENT				0x02	// KEY_TYPE_USER_DEFINE 일때 Setevent 기능을 위한 define
#define		USER_TYPE_KEY_PRGM_RUN_PROTRUSION	0x03	// KEY_TYPE_USER_DEFINE 일때 Camera 버튼 기능을 위한 define(어플실행기능 + 찰영기능)
#define		USER_TYPE_KEY_SET_MESSAGE			0x04	// KEY_TYPE_USER_DEFINE 일때 레지에 셋팅한 VK키를 던지기 위한 기능을 위한 define


BBAPI BOOL WINAPI BBKeyMapping_Init(void);
BBAPI BOOL WINAPI BBKeyMapping_SetKeyData(const int nIndex,const WORD wKey,const WORD wType);
BBAPI BOOL WINAPI BBKeyMapping_XMLFileSave(void);
BBAPI BOOL WINAPI BBKeyMapping_Close(void);
BBAPI BOOL WINAPI BBKeyMapping_SetDefaultKey(void);
BBAPI BOOL WINAPI BBKeyMapping_GetKeyData(const int nIndex,WORD *wKey,WORD *wType,TCHAR *strName,unsigned int buffer_length);
BBAPI BOOL WINAPI BBKeyMapping_GetAllKeyData(WORD *wKey,WORD *wType,TCHAR **strName,unsigned int buffer_length);
BBAPI BOOL WINAPI BBKeyMapping_SetAllKeyData(const WORD *wKey,const WORD *wType);
BBAPI BOOL WINAPI BBKeyMapping_XMLCreateHead(const int nCount);
BBAPI BOOL WINAPI BBKeyMapping_XMLCreateData(const WORD wCode,const UINT nType,const int nKeyname);
BBAPI BOOL WINAPI BBKeyMapping_XMLCreateTail(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED ( but This funtions are DEPRECATED. You can use Microsoft NLED API )

#define	BB_LED_1	(1)	
#define	BB_LED_2	(2)	
#define	BB_LED_3	(4)	
#define	BB_LED_4	(8)	
#define	BB_KEY_LED	(16)
#define	BB_VIBRATOR	(32)

typedef LPVOID HBBLED;
BBAPI HBBLED WINAPI BBLedOpen();
BBAPI void WINAPI BBLedClose( HBBLED hLED );
BBAPI DWORD WINAPI BBLedOn( unsigned int nLed );
BBAPI DWORD WINAPI BBLedOff( unsigned int nLed );

#ifdef __cplusplus
}
#endif


#endif//__BBAPPAPI_H__BLUEBIRDSOFT
