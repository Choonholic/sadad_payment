
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





// return code
#define BB_NEOPRINTER_CODE_SUCCESS                   0
#define BB_NEOPRINTER_CODE_ERR_UNKNOWN               1
#define BB_NEOPRINTER_CODE_ERR_NORMAL                2
#define BB_NEOPRINTER_CODE_ERR_INVALIDHANDLE         3
#define BB_NEOPRINTER_CODE_ERR_NOFONT                4
#define BB_NEOPRINTER_CODE_ERR_READFILE              5
#define BB_NEOPRINTER_CODE_ERR_NOBITMAPFILE          6
#define BB_NEOPRINTER_CODE_ERR_INVALID_BITMAPFILE    7
#define BB_NEOPRINTER_CODE_ERR_NOPAPER               8
#define BB_NEOPRINTER_CODE_ERR_HIGHTEMPERATURE       9
#define BB_NEOPRINTER_CODE_ERR_SETDENSITY            10
#define BB_NEOPRINTER_CODE_ERR_SETDENSITY_OUTOFRANGE 11
#define BB_NEOPRINTER_CODE_ERR_GETDENSITY            12
#define BB_NEOPRINTER_CODE_ERR_FEEDMARK              13

// printer option
#define BB_NEOPRINTER_OPTION_DEFAULT                 0   // LEFT ALIGN, NO BOLD/ITALIC
#define BB_NEOPRINTER_OPTION_LEFTALIGN               0
#define BB_NEOPRINTER_OPTION_CENTERALIGN             1
#define BB_NEOPRINTER_OPTION_RIGHTALIGN              2
#define BB_NEOPRINTER_OPTION_UNDERLINE               4
#define BB_NEOPRINTER_OPTION_EMPHESIZE               8
#define BB_NEOPRINTER_OPTION_DOUBLEHEIGHT            16
#define BB_NEOPRINTER_OPTION_DOUBLEWIDTH             32
#define BB_NEOPRINTER_OPTION_DOUBLESIZE              48
#define BB_NEOPRINTER_OPTION_ITALIC                  64
#define BB_NEOPRINTER_OPTION_FONTBYFILE             128

// barcode option
#define BB_NEOPRINTER_BARCODE_UPCA                   65
#define BB_NEOPRINTER_BARCODE_UPCE                   66
#define BB_NEOPRINTER_BARCODE_JAN13                  67
#define BB_NEOPRINTER_BARCODE_JAN8                   68
#define BB_NEOPRINTER_BARCODE_CODE39                 69
#define BB_NEOPRINTER_BARCODE_ITF                    70
#define BB_NEOPRINTER_BARCODE_CODABAR                71
#define BB_NEOPRINTER_BARCODE_CODE93                 72
#define BB_NEOPRINTER_BARCODE_CODE128                73

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


/*
	Backlight
*/
#define IOCTL_BB_BACKLIGHT_ON_OFF			        1005
typedef	LPVOID HBBBACKLIGHT;
BBAPI HBBBACKLIGHT WINAPI BBBaclightOpen();
BBAPI DWORD WINAPI BBBaclightClose(HBBBACKLIGHT hBacklight);
BBAPI DWORD WINAPI BBBaclightOnOff(HBBBACKLIGHT hBacklight, BOOL bIsOn);

/*
	NMNT
*/

typedef	LPVOID HBBNMNT;
BBAPI HBBNMNT WINAPI BBNMNTOpen();
BBAPI DWORD WINAPI BBNMNTClose( HBBNMNT hNMNT );


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
#define BB_RADIO_NONE		0
#define BB_RADIO_1X 		1
#define BB_RADIO_2G 		2
#define BB_RADIO_3G 		3

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

//---------- Key - begin -----------------------------------------
#define		VKEY_SHIFT_ENAVLE		    0x100
#define		KEY_TYPE_NONE				    0x00
#define		KEY_TYPE_ONETIME			    0x01	//	keyboard(one time)
#define		KEY_TYPE_REPEAT				    0x02	//	keyboard + repeat
#define		KEY_TYPE_LONG_N_SHORT		    0x03	//	long & short
#define		KEY_TYPE_DOWN_N_UP			    0x04	//	down & up
#define		KEY_TYPE_USER_DEFINE		    0x05	//	user define
#define		KEY_TYPE_CHANGE_IM			    0x06	//	change im mode
#define		KEY_TYPE_T_KEY_WITH_IM		    0x07	//	vk_t with im key
#define		KEY_TYPE_T_KEY_WITHOUT_IM	    0x08	//	vk_t without im key
#define		KEY_TYPE_KB_WITH_IM			    0x09	//	keyboard with im key
#define		KEY_TYPE_TPOUND_N_KEYLOCK	    0x0A	//	vk_tpound + vk_keylock을 위한 key
#define		KEY_TYPE_TPOUND_N_DZ		    0x0B	//	vk_tpound + '00'을 위한 key
#define		KEY_TYPE_TSTAR_N_PERIOD		    0x0C	//	vk_tstar + '.'을 위한 key
#define		KEY_TYPE_TSTAR_N_VIBRATE	    0x0D	//	vk_tstar + sound(vibrate)
#define		KEY_TYPE_TSTAR_N_VOLUMEOFF	    0x0E	//	vk_tstar + sound(volume off)
#define		KEY_TYPE_TEND_N_PHONEOFF	    0x0F	//	vk_tend + phone off
#define		KEY_TYPE_IM_N_TP_N_KL		    0x10	//	change im mode + vk_tpound + vk_keylock
#define		KEY_TYPE_REPEAT_WITH_IM		    0x11	//	im 상태 확인 후 keyboard + repeat
#define		KEY_TYPE_IM_N_TZERO_N_SVC	    0x12	//	skt irm규격을 위한 vk_t0 + app 실행 key
#define		KEY_TYPE_SOMEKEY_N_VIBRATE	    0x13	//	some key + sound(vibrate)
#define		KEY_TYPE_DOUBLE				    0x14	//	key가 연속으로 2번 눌렸을 때
#define		KEY_TYPE_CHANGE_MODE		    0x16	//	key  mode를 설정하는 있는 key
#define		KEY_TYPE_TEL_VOLUME			    0x17	//  통화중일때는 볼륨 기능으로 작동하기 위한 key
#define		KEY_TYPE_T_KEY_WITH_IM_LONG		0x18	//	vk_t with im key + LongKey 기능
#define		KEY_TYPE_APPLICATION_ADDKEY		0x19	//	어플리케이션 실행키로 6개 이상 지원시 사용.
#define 	KEY_TYPE_LONG_N_SHORT_HOLD		0x1A	// 	HOLD키의 동작방식 설정을 위해 추가 2010/08/16
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

/*
	ModeSwitch (Only BPL-10)
*/

#define BB_SELECT_UNKOWN					0x00000000
#define BB_SELECT_EXTUSB_TO_USB_CLIENT		0x00000001
#define BB_SELECT_EXTUSB_TO_USB_HOST		0x00000002
#define BB_SELECT_EXTUSB_TO_DM				0x00000004
#define BB_SELECT_EXTUSB_TO_UART1			0x00000008
#define BB_SELECT_BT_TO_UART1				0x00000010
#define BB_SELECT_MODEM_TO_USB_HOST			0x00000020
#define BB_SELECT_DEFAULT					(BB_SELECT_EXTUSB_TO_USB_CLIENT | BB_SELECT_MODEM_TO_USB_HOST | BB_SELECT_BT_TO_UART1)

typedef	LPVOID HMODESWITCH;
BBAPI HMODESWITCH WINAPI BBModeSwitchOpen();
BBAPI void WINAPI BBModeSwitchClose(HMODESWITCH hModeSwitch);
BBAPI DWORD WINAPI BBModeSwitchSetMode(unsigned int nMode);
BBAPI DWORD WINAPI BBModeSwitchGetMode(unsigned int *npMode);

BBAPI BOOL WINAPI BBPhoneSelectData(HBBPHONE hPhone, UINT pnDataStatus);
BBAPI void WINAPI BBModeUpdate(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED ( but This funtions are DEPRECATED. You can use Microsoft NLED API )

#define	BB_LED_1	(1)	
#define	BB_LED_2	(2)	
#define	BB_LED_3	(4)	
#define	BB_LED_4	(8)	
#define	BB_VIBRATOR	(16)
#define	BB_KEY_LED	(32)

typedef LPVOID HBBLED;
BBAPI HBBLED WINAPI BBLedOpen();
BBAPI void WINAPI BBLedClose(HBBLED hLED);
BBAPI DWORD WINAPI BBLedOn(unsigned int nLed);
BBAPI DWORD WINAPI BBLedOff(unsigned int nLed);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PSTN 


#define MAX_SDLC_FRAME_LEN  1024+128 
#define BB_MODEM_ERROR      BB_ERROR 
#define BB_MODEM_SUCCESS    BB_SUCCESS 
 
#define BB_MODEM_MODE_ASYS  0 
#define BB_MODEM_MODE_SDLC  1 
#define BB_MODEM_MODE_PPP   2 

#define BB_ONLINE_AT        1 
#define BB_ONLINE_DIALING   2 
#define BB_ONLINE_CONNECTED 3 
#define BB_ONLINE_DATA      4 
#define BB_ONLINE_OFFLINE   5 

#define BB_HDLC_TRANSMIT_NODATA			    0
#define BB_HDLC_TRANSMIT_DATA			    1
#define BB_HDLC_TRANSMIT_DATA_NOACK			2

#define UNSIGNED unsigned int

#define BB_DIALERR_NONE         0 
#define BB_DIALERR_NOCARRIER    1 
#define BB_DIALERR_NOANSWER     2 
#define BB_DIALERR_NODIALTONE   3 
#define BB_DIALERR_BUSY         4 
#define BB_DIALERR_TIMEOUT      5 
#define BB_DIALERR_ELSE         6

#define BB_MODEM_CX         1 
#define BB_MODEM_SI         2 
#define BB_MODEM_UNKNOWN    0 

#define BB_ERROR 0
#define BB_SUCCESS 1
// HDLC 
#define		BB_im_ERR_PACKET_DATA	(uchar)(0x0C) 
#define		BB_im_ERR_PACKET_FORMAT	(uchar)(0x0D) 
#define		BB_im_ERR_PACKET_NONE	(uchar)(0x0E) 
#define		BB_im_ERR_PACKET_REJ	(uchar)(0x0F) 
#define		BB_im_ERR_TXIF_FAIL		(uchar)(0x10) 
#define		BB_im_ERR_POWERUP		(uchar)(0x11) 
#define		BB_im_ERR_POWERDOWN		(uchar)(0x12) 
 
#define		BB_modem_ERRHANDLE		BB_ERRHANDLE		//0x8C 
#define		BB_modem_ERR_PARA		BB_ERRPARAM			//0x8B 
#define		BB_im_ERR_NO_DATAMODE	(uchar)(0x52) 
#define		BB_im_ERR_UN_OBTAINABLE_NUMBER	(uchar)(0x54) 
#define		BB_im_ERR_NO_CMDMODE	(uchar)(0x53) 
#define		BB_modem_DATAMODE		(uchar)(0x51) 

#define		BB_modem_SUCCESS		EM_SUCCESS 
#define		BB_modem_ERROR			EM_ERROR 
#define		BB_modem_FAIL			(uchar)'F' 
#define		BB_modem_NODIALTONE		(uchar)'6' 
#define		BB_modem_PHONEINUSE		(uchar)'P' 
#define		BB_modem_NOCARRIER		(uchar)'3' 
#define		BB_modem_LINEBUSY		(uchar)'7' 
#define		BB_modem_HANGUP			(uchar)'O' 
#define		BB_modem_INDIAL			(uchar)'D' 
#define		BB_modem_RECVNOACK		(uchar)'G' 
#define		BB_modem_ATCOMRIGHT		(uchar)'0' 
#define		BB_modem_ATCOMERROR		(uchar)'E' 
#define		BB_modem_INPROCESS		(uchar)'C' 
#define		BB_modem_TIMEOUT		(uchar)'V' 
#define     BB_modem_NOPATCH        (uchar)'Z'

#define     BB_modem_CNT            (uchar)(0xC0)
#define 	BB_modem_CNT300V21		(uchar)(0xA0) 
#define 	BB_modem_HDLC300V21		(uchar)(0xA1) 
 
#define		BB_modem_CNT1200V22		(uchar)(0xA2) 
#define 	BB_modem_HDLC1200V22	(uchar)(0xA3) 
#define 	BB_modem_CNT1200V22V42BIS	(uchar)(0xA4) 
 
#define 	BB_modem_CNT2400V22B		(uchar)(0xA5) 
#define		BB_modem_HDLC2400V22B	    (uchar)(0xA6) 
#define 	BB_modem_CNT2400V22BV42BIS  (uchar)(0xA7) 
#define		BB_modem_CNT2400V34		(uchar)(0xA8) 
 
#define		BB_modem_CNT4800V34		(uchar)(0xA9) 
 
#define		BB_modem_CNT9600V32		(uchar)(0xAA) 
#define 	BB_modem_CNT9600V32V42BIS	(uchar)(0xAB) 
#define		BB_modem_CNT9600V34		(uchar)(0xAC) 
 
 
#define 	BB_modem_CNT14400V32B		(uchar)(0xAD) 
#define 	BB_modem_CNT14400V32BV42BIS (uchar)(0xAE) 
#define		BB_modem_CNT14400V34		(uchar)(0xAF) 
 
#define		BB_modem_CNT19200V34		(uchar)(0xB1) 
#define 	BB_modem_CNT19200V34V42BIS	(uchar)(0xB2) 
 
#define 	BB_modem_CNT24000V34		(uchar)(0xB3) 
#define 	BB_modem_CNT24000V34V42BIS	(uchar)(0xB4) 
 
#define 	BB_modem_CNT26400V34		(uchar)(0xB5) 
#define 	BB_modem_CNT26400V34V42BIS	(uchar)(0xB6) 
 
#define		BB_modem_CNT28800V34		(uchar)(0xB7) 
#define 	BB_modem_CNT28800V34V42BIS	(uchar)(0xB8) 
 
#define		BB_modem_CNT33600V34		(uchar)(0xB9) 
#define		BB_modem_CNT28800V8		    (uchar)(0xBA) 
#define		BB_modem_CNT33600V34V42BIS	(uchar)(0xBB)

typedef struct _BB_HDLC_CONF_INFO
{
	unsigned char   addr;
	unsigned int    re_cnt;
	unsigned int    max_time_toconnect; 
    unsigned int    max_time_todata; 
}
BB_HDLC_CONF_INFO, *LP_BB_HDLC_CONF_INFO;

BBAPI uchar WINAPI BBPstnModemSdlcOpen(WCHAR* wcsComPort);
BBAPI uchar WINAPI BBPstnModemSdlcClose(void);
BBAPI uchar WINAPI BBPstnModemDial(const char *pcsBuf);	
BBAPI uchar WINAPI BBPstnModemSendSdlc( const char *pcBuf, uint uiLen, uint msTime);
BBAPI uchar WINAPI BBPstnModemReadSdlc( void *pvBuf, uint *puiRealLen, uint msTime);
BBAPI uchar WINAPI BBPstnModemIfExistDialTone(void);
BBAPI uchar WINAPI BBPstnModemDialOff(void);
BBAPI uchar WINAPI BBPstnModemGetLineStatus(uchar *pucSTS);
BBAPI uchar WINAPI BBPstnModemAtCmd(uchar *pcAtcmd);
BBAPI void WINAPI BBPStnModemSetConnectTimeoutValue(uint connectTime);
BBAPI void WINAPI BBPstnModemSetSnrmTimeoutValue(uint snrmTime);
BBAPI uchar WINAPI BBPstnModemSendRawPacket(uchar *pData, uint uiLen);
BBAPI uchar WINAPI BBPstnModemSdlcConfigure(BB_HDLC_CONF_INFO* pstBbHdlcInfo);
	
#ifdef __cplusplus
}
#endif


#endif//__BBAPPAPI_H__BLUEBIRDSOFT
