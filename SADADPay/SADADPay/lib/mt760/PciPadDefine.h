///CommPCI의 Define 파일 
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>
#include <WinSock2.h>

#include "commondefine.h"


/////////////////////////////////////////////////////// common function END


//#define PAYMENT_MASTER_KEY_FILE_NAME			L"BBPaymentMasterKey.key"
#define CERT_FILE_PATH							L"C:\\BBPCI\\"
#define CERT_FILE_PATH_PDA						L"\\PocketStore\\Data\\"
#define CERT_FILE_NAME_PC						L"bb_cert_pc.pem"
#define CERT_FILE_NAME_TERMINAL					L"bb_cert_t.pem"
#define RSA_PUBLIC_KEY_FILE_NAME_PC				L"bb_public_key_t.key"

// 2014-03-17 erulil ksn 파일 관련 define 추가
#define KSN_FILE_PATH							L"\\Windows\\"
#define KSN_FILE_NAME							L"ksn_list.ksn"


#define TIMEOUT_WAIT_SINGLE_OBJ					(1000 * 4)//10)//30)//60) // 2014-05-12 timeout 1분에서 30초로 변경, 2014-07-30 30초에서 10초로 변경
#define TIMEOUT_WAIT_SINGLE_OBJ_ADDITIONAL		800 // 500 // 2014-08-04 erulil 500 -> 800으로 변경
#define TIMEOUT_WAIT_SINGLE_OBJ_LONG			(1000 * 15)

struct PACKET
{
	const SOCKET socket;
	BYTE *pPacket;
	int size;

	PACKET::PACKET()
		: size(0)
		, socket(0)
	{
		pPacket = NULL;
	}

	PACKET::PACKET(LPBYTE pBuffer, int nBufferSize, SOCKET sock)
		: size(nBufferSize)
		, socket(sock)
	{
		pPacket = NULL;
		pPacket = new BYTE[size];
		memset(pPacket, 0, sizeof(size));
		memcpy(pPacket, pBuffer, size);
	}

	PACKET::PACKET(PACKET *packet)
		: size(packet->size)
		, socket(packet->socket)
	{
		pPacket = NULL;
		pPacket = new BYTE[size];
		memset(pPacket, 0, sizeof(size));
		memcpy(pPacket, packet->pPacket, size);
	}

	void Release()
	{
		SAFE_ARRAY_DELETE(pPacket);
		size = 0;
	}
};




// 2013-10-04 erulil 추가, MaxQ에서 받은 msg data를 저장한다
struct RECV_MSG_DATA
{
	HANDLE hEvent;
	BYTE byResult;
	BYTE byCode;
	BYTE *pData;
	DWORD dwDataLen;

	RECV_MSG_DATA()
		: byResult(0)
		, byCode(0)
		, dwDataLen(0)
		, pData(NULL)
		, hEvent(NULL)
	{
	}

	~RECV_MSG_DATA()
	{
		this->Release();
	}

	BOOL CreateEvent(BOOL bManualReset)
	{
		hEvent = ::CreateEvent(NULL, bManualReset, FALSE, NULL);
		if(hEvent == NULL)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL ResetEvent()
	{
		if(hEvent != NULL)
		{
			return ::ResetEvent(hEvent);
		}
		return FALSE;
	}

	BOOL SetEvent()
	{
		if(hEvent != NULL)
		{
			return ::SetEvent(hEvent);
		}
		return FALSE;
	}

	DWORD GetResultCode()
	{
		DWORD dwResult = byCode + (byResult << 8);
		return dwResult;
	}

	void DeleteData()
	{
		if(pData != NULL)
		{
			delete []pData;
			pData = NULL;
		}

		dwDataLen = 0;
		byResult = 0;
		byCode = 0;
	}

	void Release()
	{
		SAFE_CLOSE_HANDLE(hEvent);
		DeleteData();
	}
};


struct PCI_TIME
{
	BYTE byYear; // 00 ~99 (20XX)
	BYTE byMonth; // 1 ~ 12
	BYTE byDay; // 1 ~ 31
	BYTE byHour; // 1 ~ 12
	BYTE byMinute; // 0 ~ 59
	BYTE bySecond; // 0 ~ 59

	PCI_TIME()
		: byYear(0)
		, byMonth(0)
		, byDay(0)
		, byHour(0)
		, byMinute(0)
		, bySecond(0)
	{
	}

	// 2014-04-04 erulil 사용하지 않게 됨
	//BOOL SetYear(UINT number)
	//{
	//	if(number < 99)
	//	{
	//		byYear = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL SetMonth(UINT number)
	//{
	//	if(number >= 1 && number <= 12)
	//	{
	//		byMonth = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL SetDay(UINT number)
	//{
	//	if(number >= 1 && number <= 31)
	//	{
	//		byDay = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL SetHour(UINT number)
	//{
	//	if(number <= 23)
	//	{
	//		byHour = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL SetMinute(UINT number)
	//{
	//	if(number <= 59)
	//	{
	//		byMinute = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL SetSecond(UINT number)
	//{
	//	if(number <= 59)
	//	{
	//		bySecond = ((number / 10) * 16) + number % 10;
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
};


// 2013-10-04 erulil Key type 추가 ---------------------------- START
#define KEY_TYPE_TDES						0
#define KEY_TYPE_DUKPT						1
#define KEY_TYPE_TDES_GP					2
#define KEY_TYPE_AES_GP						3
#define KEY_TYPE_RSA						4
#define KEY_TYPE_LSK						0x00
#define KEY_TYPE_BSK						0x01
#define KEY_TYPE_USK						0x02
#define KEY_TYPE_KSK						0x03
#define KEY_TYPE_FSK						0x04
// 2014-02-18 erulil key usage + type을 나타낼 수 있는 define 추가
#define KEY_USAGETYPE_DUKPT_INITIAL_KEY					0
#define KEY_USAGETYPE_TDES_MASTER_KEY					1
#define KEY_USAGETYPE_TDES_ENC_PIN						2
#define KEY_USAGETYPE_TDES_MAKING_MAC					3
#define KEY_USAGETYPE_TDES_GPK_INCLUDE_MAKING_MAC		4
#define KEY_USAGETYPE_AES_GPK_INCLUDE_MAKING_MAC		5
#define KEY_USAGETYPE_TDES_FIX_KEY						6 // 2014-03-17 erulil 추가
#define KEY_USAGETYPE_TDES_GPK_EXCLUDE_MAKING_MAC		7
#define KEY_USAGETYPE_AES_GPK_EXCLUDE_MAKING_MAC		8
// 2013-10-04 erulil Key type 추가 ---------------------------- END



// 2013-10-04 erulil Key Usage 추가 ---------------------------- START
#define MSG_KEY_USAGE_DUKPUT_INITIAL_KEY						0x4231
#define MSG_KEY_USAGE_TDES_MASTER_KEY							0x4B30
#define MSG_KEY_USAGE_TDES_KEY_FOR_MAKING_MAC					0x4D00
#define MSG_KEY_USAGE_TDES_KEY_FOR_PIN_ENC						0x5030
#define MSG_KEY_USAGE_GP_KEY_INCLUDE_MAKING_MAC					0x6000
#define MSG_KEY_USAGE_GP_KEY_EXCLUDE_MAKING_MAC					0x6001
#define MSG_KEY_USAGE_RSA_KEY_PAIR								0x7000
#define MSG_KEY_USAGE_RSA_PRV_KEY_FOR_MOF_SSL					0x7030
#define MSG_KEY_USAGE_RSA_PRV_KEY_FOR_BANK_SSL					0x7031
#define MSG_KEY_USAGE_RSA_PRV_KEY_FOR_BANK_KEY_ENC				0x7032
#define MSG_KEY_USAGE_RSA_PRV_KEY_FOR_BANK_SIGN					0x7033
// 2014-04-07 erulil key export 전용 usage 추가
#define MSG_KEY_USAGE_TDES_GP_KEY_INCLUDE_MAKING_MAC			0x6154
#define MSG_KEY_USAGE_TDES_GP_KEY_EXCLUDE_MAKING_MAC			0x6054
#define MSG_KEY_USAGE_AES_GP_KEY_INCLUDE_MAKING_MAC				0x6141
#define MSG_KEY_USAGE_AES_GP_KEY_EXCLUDE_MAKING_MAC				0x6041
// 2013-10-04 erulil Key Usage 추가 ---------------------------- END



// 2013-10-04 erulil mode 추가 ---------------------------- START
#define MSG_KEY_MODE_RSA_2_KEY						0 // N + D (각 256B)
#define MSG_KEY_MODE_RSA_5_KEY						1 // p + q + dP + dQ + qInv (각 128B)
#define HEADER_MODE_OVERWRITE						0x00
#define HEADER_MODE_XOR								0x80
#define HEADER_MODE_KCV_0							0x00
#define HEADER_MODE_KCV_1							0x01
// 2013-10-04 erulil mode 추가 ---------------------------- END


// 2013-10-04 erulil key algorithm 추가 ---------------------------- START
#define MSG_KEY_ALGORITHM_AES						0x41
#define MSG_KEY_ALGORITHM_RSA						0x52
#define MSG_KEY_ALGORITHM_TDES						0x54
// 2013-10-04 erulil key algorithm 추가 ---------------------------- END


// 2013-10-09 erulil key index for save(header) 추가 ---------------------------- START
#define HEADER_KEY_INDEX_FOR_SAVE_DUKPT_MAX				4 // DUKPT key index로 쓸 수 있는 최대값(0 ~ 3) 
#define HEADER_KEY_INDEX_FOR_SAVE_TDES_MK_MAX			7 // TDES Master key index로 쓸 수 있는 최대값(0 ~ 7)
#define HEADER_KEY_INDEX_FOR_SAVE_TDES_FOR_MAC_MAX		7 // TDES key (for making MAC) index로 쓸 수 있는 최대값(0 ~ 7)
#define HEADER_KEY_INDEX_FOR_SAVE_TDES_FOR_PIN_MAX		7 // TDES key (for PIN Encryption) index로 쓸 수 있는 최대값(0 ~ 7)
#define HEADER_KEY_INDEX_FOR_SAVE_TDES_GP_MAX			15 // TDES key (for General Purpose) index로 쓸 수 있는 최대값(0 ~ 15)
#define HEADER_KEY_INDEX_FOR_SAVE_AES_GP_MAX			15 // AES key (for General Purpose) index로 쓸 수 있는 최대값(0 ~ 15) // 2014-06-06 erulil 18에서 15로 수정
#define HEADER_KEY_INDEX_RSA_MAX						1 // RSA key index로 쓸 수 있는 최대값(0 ~ 1)
#define MAX_RSA_KEY_INDEX								7 // send public key(0x18d0)을 사용할 때 쓸 수 있는 index 최대 값(0 ~ 7)
// 2013-10-09 erulil key index for save(header) 추가 ---------------------------- END


// 2013-10-09 erulil key length 추가 ---------------------------- START
enum KEY_LENGTH
{
	KEY_LENGTH_8 = 8, // des
	KEY_LENGTH_16 = 16, // tdes, aes
	KEY_LENGTH_24 = 24, // tdes, aes
	KEY_LENGTH_32 = 32, // aes
};

enum TDES_KEY_LENGTH
{
	TDES_KEY_LENGTH_16 = 16,
	TDES_KEY_LENGTH_24 = 24,
};

enum AES_KEY_LENGTH
{
	AES_KEY_LENGTH_16 = 16,
	AES_KEY_LENGTH_24 = 24,
	AES_KEY_LENGTH_32 = 32,
};
// 2013-10-09 erulil key length ---------------------------- END


// 2013-10-03 erulil header 추가 ---------------------------- START
#define MSG_HEADER_LSK								0x00
#define MSG_HEADER_BSK								0x01
#define MSG_HEADER_USK								0x02
#define MSG_HEADER_KSK								0x03
#define MSG_HEADER_FSK								0x04
#define MSG_HEADER_METHOD_SECURE_ROOM_PW_LOGIN		0x42
// 2013-10-03 erulil header 추가 ---------------------------- END


// 2013-10-03 erulil MSG Position 추가 ---------------------------- START
// ack packet
#define MSG_POSITION_INDICATOR_ACK					0
#define MSG_POSITION_TAG_ACK						1
#define MSG_POSITION_LENGTH_1_ACK					2 // length : 2B
#define MSG_POSITION_LENGTH_2_ACK					3 // length : 2B
#define MSG_POSITION_RESULT_ACK						4
#define MSG_POSITION_CODE_ACK						5
#define MSG_POSITION_DATA_LENGTH_1_ACK				6
#define MSG_POSITION_DATA_LENGTH_2_ACK				7
#define MSG_POSITION_TIME							6
#define MSG_POSITION_DATA_ACK						8
#define MSG_POSITION_DATA_PIN_MAC					6 // data start position : enc pin, mac ack packet
// notify packet
#define MSG_POSITION_HEADER_START_NOTIFY			4 // key injection notify msg에서 header의 시작 위치
#define HEADER_POSITION_KEY_USAGE_1					1 // header 내에서의 key usage 위치
#define HEADER_POSITION_KEY_USAGE_2					2 // header 내에서의 key usage 위치
#define HEADER_POSITION_ALGORITHM					3 // header 내에서의 algorithm 위치
#define HEADER_POSITION_APP_ID_START				4 // header 내에서의 app id 시작 위치
#define HEADER_POSITION_APP_ID_2					5 // header 내에서의 app id 2
#define HEADER_POSITION_APP_ID_3					6 // header 내에서의 app id 3
#define HEADER_POSITION_APP_ID_END					7 // header 내에서의 app id 끝 위치
#define HEADER_POSITION_KEY_INDEX					8
#define HEADER_POSITION_KEY_LENGTH					9
#define HEADER_POSITION_VERSION_1					10
#define HEADER_POSITION_VERSION_2					11
#define HEADER_POSITION_MASTER_KEY_INDEX			12
#define HEADER_POSITION_MODE						13
// 2013-10-03 erulil MSG Position 추가 ---------------------------- END


// 2013-10-01 erulil command 추가 ---------------------------- START
#define MSG_INDICATOR_FIRMWARE						0x18
#define MSG_INDICATOR_CUSTOM						0x19

 
///////////////////////////////////////////////////////////// 0X19 COMMAND : erulil 자체 추가 커맨드 START
#define MSG_CMD_READY_RECV_PUBLIC_REQUEST			0x1900 // PC에서 public request 패킷을 받을 준비가 되었음을 알리는 커맨드
#define MSG_TAG_READY_RECV_PUBLIC_REQUEST			0x00
#define MSG_CMD_FILE_RECV_START						0x1901 // file recv start
#define MSG_TAG_FILE_RECV_START						0x01
#define MSG_CMD_FILE_RECV_ING						0x1902 // file recv ing
#define MSG_TAG_FILE_RECV_ING						0x02
#define MSG_CMD_FILE_RECV_END						0x1903 // file recv end
#define MSG_TAG_FILE_RECV_END						0x03
#define MSG_CMD_FILE_SEND_START						0x1904 // file send start
#define MSG_TAG_FILE_SEND_START						0x04
#define MSG_CMD_FILE_SEND_ING						0x1905 // file send ing
#define MSG_TAG_FILE_SEND_ING						0x05
#define MSG_CMD_FILE_SEND_END						0x1906 // file send end
#define MSG_TAG_FILE_SEND_END						0x06
#define MSG_CMD_SEND_RSA_PUBLIC_KEY_NOTI			0x1907
#define MSG_TAG_SEND_RSA_PUBLIC_KEY_NOTI			0x07 // 2014-01-07 erulil PDA에서PC로 public key를 보낼 때 사용되는 command
#define MSG_CMD_SEND_RSA_PUBLIC_KEY_ACK				0x1908
#define MSG_TAG_SEND_RSA_PUBLIC_KEY_ACK				0x08 // 2014-01-07 erulil PC에서 PDA로 public key 수신을 확인할 때 보내는 command
#define MSG_CMD_GET_RSA_PUBLIC_KEY_NOTI				0x1909
#define MSG_TAG_GET_RSA_PUBLIC_KEY_NOTI				0x09 // 2014-01-17 erulil PC에서 PDA로 public키를 요청할 때 사용 하는 패킷. 이 패킷을 받으면 단말기는 RSA key pair를 생성해서 maxq로 보내고 public key는 PC로 전송하게 된다(CommPCI)
#define MSG_CMD_MAXQ_CONNECTION_CHECK_NOTI			0x1910
#define MSG_TAG_MAXQ_CONNECTION_CHECK_NOTI			0x10 // 2014-01-18 erulil PC에서 PDA로 maxq 연결 상태를 확인할 때 사용되는 커맨드.
#define MSG_CMD_MAXQ_CONNECTION_CHECK_ACK			0x1911
#define MSG_TAG_MAXQ_CONNECTION_CHECK_ACK			0x11 // 2014-01-18 erulil PDA에서 PC로 maxq 연결 상태를 답변해 줄 때 사용되는 커맨드. 통신키를 포함한다
///////////////////////////////////////////////////////////// 0X19 COMMAND : erulil 자체 추가 커맨드 END


///////////////////////////////////////////////////////////// 0X18 COMMAND
// 2013-11-20 erulil 추가
#define MSG_CMD_PUBLIC_KEY_REQUEST 				0x1838
#define MSG_TAG_PUBLIC_KEY_REQUEST 				0x38
#define MSG_CMD_PUBLIC_KEY_ACK					0x1839
#define MSG_TAG_PUBLIC_KEY_ACK					0x39
// 2014-02-18 erulil random number에 관련된 패킷의 커맨드 변경 : garanti 관련된 패킷과 겹쳐서 indicator를 firmware에서 custom으로 수정함
//#define MSG_CMD_RAND_NUM_REQUEST				0x183A
//#define MSG_TAG_RAND_NUM_REQUEST				0x3A
//#define MSG_CMD_RAND_NUM_ACK					0x183B
//#define MSG_TAG_RAND_NUM_ACK					0x3B
#define MSG_CMD_RAND_NUM_REQUEST				0x193A
#define MSG_TAG_RAND_NUM_REQUEST				0x3A
#define MSG_CMD_RAND_NUM_ACK					0x193B
#define MSG_TAG_RAND_NUM_ACK					0x3B
#define MSG_CMD_ASYM_PAYMENT_KEY_REQUEST		0x183C // to MAXQ. remote key injection - asymmetric
#define MSG_TAG_ASYM_PAYMENT_KEY_REQUEST		0x3C // to MAXQ
#define MSG_CMD_ASYM_PAYMENT_KEY_ACK			0x183D // from MAXQ. remote key injection - asymmetric
#define MSG_TAG_ASYM_PAYMENT_KEY_ACK			0x3D // from MAXQ
#define MSG_CMD_DEBUG							0x18ED // from MAXQ. remote key injection - asymmetric
#define MSG_TAG_DEBUG							0xED // from MAXQ


// old command
#define MSG_CMD_CONNECTION_ACK					0x1860     /// MAXQ로부터의 연결 요청에대한 응답.
#define MSG_TAG_CONNECTION_ACK					0x60
#define MSG_CMD_CONNECTION_REQUEST				0x1861     /// MAXQ로부터의 연결 요청.
#define MSG_TAG_CONNECTION_REQUEST				0x61
#define MSG_CMD_KEY_END							0x1856
#define MSG_TAG_KEY_END							0x56
#define MSG_CMD_KEY_END_ACK						0x1857
#define MSG_TAG_KEY_END_ACK						0x57
#define MSG_CMD_Fn_KEY_INPUT_ACK				0x1882
#define MSG_TAG_Fn_KEY_INPUT_ACK				0x82
#define MSG_CMD_Fn_KEY_INPUT_REQUEST			0x1883
#define MSG_TAG_Fn_KEY_INPUT_REQUEST			0x83
#define MSG_CMD_PW_INPUT_ACK					0x1884
#define MSG_TAG_PW_INPUT_ACK					0x84
#define MSG_CMD_PW_INPUT_REQUEST				0x1885
#define MSG_TAG_PW_INPUT_REQUEST				0x85
#define MSG_CMD_SN_REQUEST						0x18E4
#define MSG_TAG_SN_REQUEST						0xE4
#define MSG_CMD_SN_ACK							0x18E5
#define MSG_TAG_SN_ACK							0xE5
#define MSG_CMD_PASSWORD_CORRECT				0x18C0
#define MSG_TAG_PASSWORD_CORRECT				0xC0
#define MSG_CMD_CONNECTION_ALIVE_NOTI			0x1863
#define MSG_TAG_CONNECTION_ALIVE_NOTI			0x63
#define MSG_CMD_CONNECTION_ALIVE_ACK			0x1862
#define MSG_TAG_CONNECTION_ALIVE_ACK			0x62
#define MSG_CMD_PIN_DATA_NOTI					0x1889
#define MSG_TAG_PIN_DATA_NOTI					0x89
#define MSG_CMD_PIN_DATA_ACK					0x1888
#define MSG_TAG_PIN_DATA_ACK					0x88
#define	MSG_CMD_UI_DISCONNECTION_NOTIFY			0x1864
#define	MSG_TAG_UI_DISCONNECTION_NOTIFY			0x64	//	Length = 0, PCI disconnect command notify
#define	MSG_CMD_UI_DISCONNECTION_ACK			0x1865
#define	MSG_TAG_UI_DISCONNECTION_ACK			0x65	//	Length = 2, PCI disconnect command ack

// key injection
#define MSG_CMD_CODE_KEY_INJECTION_NOTI							0x1800
#define MSG_TAG_CODE_KEY_INJECTION_NOTI							0x00
#define MSG_CMD_CODE_KEY_INJECTION_ACK							0x1801
#define MSG_TAG_CODE_KEY_INJECTION_ACK							0x01
#define MSG_CMD_RSA_KEY_PAIR_INJECTION_NOTI						0x1802
#define MSG_TAG_RSA_KEY_PAIR_INJECTION_NOTI						0x02
#define MSG_CMD_RSA_KEY_PAIR_INJECTION_ACK						0x1803
#define MSG_TAG_RSA_KEY_PAIR_INJECTION_ACK						0x03
#define MSG_CMD_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_NOTI			0x1804
#define MSG_TAG_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_NOTI			0x04
#define MSG_CMD_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_ACK			0x1805
#define MSG_TAG_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_ACK			0x05
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_NOTI			0x1806
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_NOTI			0x06
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_ACK			0x1807
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_ACK			0x07
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_NOTI		0x1808
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_NOTI		0x08
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_ACK		0x1809
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_ACK		0x09
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_NOTI		0x180A
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_NOTI		0x0A
#define MSG_CMD_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_ACK			0x180B
#define MSG_TAG_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_ACK			0x0B
#define MSG_CMD_SECURE_ROOM_PW_LOGIN_PK_INJECTION_NOTI			0x1810 
#define MSG_TAG_SECURE_ROOM_PW_LOGIN_PK_INJECTION_NOTI			0x10 
#define MSG_CMD_SECURE_ROOM_PW_LOGIN_PK_INJECTION_ACK			0x1811
#define MSG_TAG_SECURE_ROOM_PW_LOGIN_PK_INJECTION_ACK			0x11
#define MSG_CMD_SYM_PAYMENT_KEY_NOTI							0x183E
#define MSG_TAG_SYM_PAYMENT_KEY_NOTI							0x3E
#define MSG_CMD_SYM_PAYMENT_KEY_ACK								0x183F
#define MSG_TAG_SYM_PAYMENT_KEY_ACK								0x3F
#define MSG_CMD_GARANTI_PAYMENT_KEY_NOTI						0x183A
#define MSG_TAG_GARANTI_PAYMENT_KEY_NOTI						0x3A
#define MSG_CMD_GARANTI_PAYMENT_KEY_ACK							0x183B
#define MSG_TAG_GARANTI_PAYMENT_KEY_ACK							0x3B

// 2014-02-19 erulil 추가. key 삭제 패킷
#define	MSG_CMD_SEND_CLEAR_KEYS_NOTI							0x185E	//	Length = 4
#define	MSG_TAG_SEND_CLEAR_KEYS_NOTI							0x5E
#define	MSG_CMD_SEND_CLEAR_KEYS_ACK								0x185F	//	Length = 2
#define	MSG_TAG_SEND_CLEAR_KEYS_ACK								0x5F
// 2014-03-17 erulil 추가
#define MSG_CMD_ERASE_KEY_NOTI									0x185c
#define MSG_TAG_ERASE_KEY_NOTI									0x5c
#define MSG_CMD_ERASE_KEY_ACK									0x185d
#define MSG_TAG_ERASE_KEY_ACK									0x5d
#define MSG_CMD_GET_KCV_NOTI									0x181e
#define MSG_TAG_GET_KCV_NOTI									0x1e
#define MSG_CMD_GET_KCV_ACK										0x181f
#define MSG_TAG_GET_KCV_ACK										0x1f

// 2014-01-17 erulil 터미널에서 생성한 RSA key pair 관련 packet 추가
#define	MSG_CMD_SEND_TERMINAL_KEY_PAIR_NOTIFY					0x1812
#define	MSG_TAG_SEND_TERMINAL_KEY_PAIR_NOTIFY					0x12	//	Length = 1432
#define	MSG_CMD_SEND_TERMINAL_KEY_PAIR_ACK						0x1813
#define	MSG_TAG_SEND_TERMINAL_KEY_PAIR_ACK						0x13	//	Length = 2
#define	MSG_CMD_SEND_TERMINAL_KEY_CONFIRM_NOTIFY				0x1814
#define	MSG_TAG_SEND_TERMINAL_KEY_CONFIRM_NOTIFY				0x14	//	Length = 0
#define	MSG_CMD_SEND_TERMINAL_KEY_CONFIRM_ACK					0x1815
#define	MSG_TAG_SEND_TERMINAL_KEY_CONFIRM_ACK					0x15	//	Length = 2

// operation
#define MSG_CMD_TDES_PIN_OPERATION_NOTI				0x1820
#define MSG_TAG_TDES_PIN_OPERATION_NOTI				0x20
#define MSG_CMD_TDES_PIN_OPERATION_ACK				0x1821
#define MSG_TAG_TDES_PIN_OPERATION_ACK				0x21
#define MSG_CMD_DUKPT_PIN_OPERATION_NOTI			0x1822
#define MSG_TAG_DUKPT_PIN_OPERATION_NOTI			0x22
#define MSG_CMD_DUKPT_PIN_OPERATION_ACK				0x1823
#define MSG_TAG_DUKPT_PIN_OPERATION_ACK				0x23
#define MSG_CMD_TDES_MAC_OPERATION_NOTI				0x1830
#define MSG_TAG_TDES_MAC_OPERATION_NOTI				0x30
#define MSG_CMD_TDES_MAC_OPERATION_ACK				0x1831
#define MSG_TAG_TDES_MAC_OPERATION_ACK				0x31
#define MSG_CMD_DUKPT_MAC_OPERATION_NOTI			0x1832
#define MSG_TAG_DUKPT_MAC_OPERATION_NOTI			0x32
#define MSG_CMD_DUKPT_MAC_OPERATION_ACK				0x1833
#define MSG_TAG_DUKPT_MAC_OPERATION_ACK				0x33
#define MSG_CMD_TDES_GP_MAC_OPERATION_NOTI			0x1836
#define MSG_TAG_TDES_GP_MAC_OPERATION_NOTI			0x36
#define MSG_CMD_TDES_GP_MAC_OPERATION_ACK			0x1837
#define MSG_TAG_TDES_GP_MAC_OPERATION_ACK			0x37
#define MSG_CMD_AES_GP_MAC_OPERATION_NOTI			0x1834
#define MSG_TAG_AES_GP_MAC_OPERATION_NOTI			0x34
#define MSG_CMD_AES_GP_MAC_OPERATION_ACK			0x1835
#define MSG_TAG_AES_GP_MAC_OPERATION_ACK			0x35
#define MSG_CMD_TDES_GP_KEY_ENC_NOTI				0x1840	// encryption data
#define MSG_TAG_TDES_GP_KEY_ENC_NOTI				0x40	// encryption data
#define MSG_CMD_TDES_GP_KEY_ENC_ACK					0x1841
#define MSG_TAG_TDES_GP_KEY_ENC_ACK					0x41
#define MSG_CMD_TDES_GP_KEY_DEC_NOTI				0x1842	// decryption data
#define MSG_TAG_TDES_GP_KEY_DEC_NOTI				0x42	// decryption data
#define MSG_CMD_TDES_GP_KEY_DEC_ACK					0x1843
#define MSG_TAG_TDES_GP_KEY_DEC_ACK					0x43
#define MSG_CMD_AES_GP_KEY_ENC_NOTI					0x1844	// encryption data
#define MSG_TAG_AES_GP_KEY_ENC_NOTI					0x44	// encryption data
#define MSG_CMD_AES_GP_KEY_ENC_ACK					0x1845
#define MSG_TAG_AES_GP_KEY_ENC_ACK					0x45
#define MSG_CMD_AES_GP_KEY_DEC_NOTI					0x1846	// decryption data
#define MSG_TAG_AES_GP_KEY_DEC_NOTI					0x46	// decryption data
#define MSG_CMD_AES_GP_KEY_DEC_ACK					0x1847
#define MSG_TAG_AES_GP_KEY_DEC_ACK					0x47
#define MSG_CMD_DUKPT_KEY_ENC_NOTI					0x184E	// encryption data
#define MSG_TAG_DUKPT_KEY_ENC_NOTI					0x4E	// encryption data
#define MSG_CMD_DUKPT_KEY_ENC_ACK					0x184F
#define MSG_TAG_DUKPT_KEY_ENC_ACK					0x4F
#define MSG_CMD_RSA_ENCRYPT_OR_VERIFY_NOTI			0x1848
#define MSG_TAG_RSA_ENCRYPT_OR_VERIFY_NOTI			0x48
#define MSG_CMD_RSA_ENCRYPT_OR_VERIFY_ACK			0x1849
#define MSG_TAG_RSA_ENCRYPT_OR_VERIFY_ACK			0x49
#define MSG_CMD_RSA_DECRYPT_OR_SIGN_NOTI			0x184A
#define MSG_TAG_RSA_DECRYPT_OR_SIGN_NOTI			0x4A
#define MSG_CMD_RSA_DECRYPT_OR_SIGN_ACK				0x184B
#define MSG_TAG_RSA_DECRYPT_OR_SIGN_ACK				0x4B

// 2014-05-12 erulil 추가 : Enc PIN 패킷을 취소하기 위한 패킷
#define	MSG_CMD_ENCRYPT_PIN_CANCEL_NOTI				0x1826	//	Length = 0, 2014-05-12 erulil Pin enc cancel noti packet(BBSecureLib -> MAXQ)
#define	MSG_TAG_ENCRYPT_PIN_CANCEL_NOTI				0x26	//	Length = 0, 2014-05-12 erulil Pin enc cancel noti packet(BBSecureLib -> MAXQ)
#define	MSG_CMD_ENCRYPT_PIN_CANCEL_ACK				0x1827	//	Length = 0, 2014-05-12 erulil Pin enc cancel noti packet(MAXQ -> BBSecureLib)
#define	MSG_TAG_ENCRYPT_PIN_CANCEL_ACK				0x27	//	Length = 0, 2014-05-12 erulil Pin enc cancel noti packet(MAXQ -> BBSecureLib)

// 2014-04-07 erulil 추가 : export(= generate) key 기능을 위해 사용되는 패킷. MSG_TAG_SEND_GENERATE_KEY_NOTI를 받으면 MAXQ가 key를 생성해서 MSG_TAG_SEND_GENERATE_KEY_ACK로 보낸다
#define	MSG_CMD_SEND_GENERATE_KEY_NOTI				0x1816	//	Length = 522 : KeyIndex(1B) + KeyLength(1B) + KeyType(2B) + BankID(4B) + ByteLengthOfRSA(2B) + Modulus(256B) + Exponent(256B)
#define	MSG_TAG_SEND_GENERATE_KEY_NOTI				0x16
#define	MSG_CMD_SEND_GENERATE_KEY_ACK				0x1817	//	Length = 284 : Result(1B) + Code(1B) + EncryptedKey(256B) + KCV(16B) + KSN(10B)
#define	MSG_TAG_SEND_GENERATE_KEY_ACK				0x17
// 2014-04-14 erulil 추가 : generate key2 : 저장된 RSA public key로 암호화를 진행
#define MSG_CMD_SEND_GENERATE2_KEY_NOTI				0x18D2  //  Length = 10 : KeyIndex(1B) + KeyLength(1B) + KeyType(2B) + BankID(4B) + Public Key Index(1B) + Zero(1B)
#define MSG_TAG_SEND_GENERATE2_KEY_NOTI				0xD2
#define MSG_CMD_SEND_GENERATE2_KEY_ACK				0x18D3  //  Length = 286 : Result(1B) + Code(1B) + EncryptedKey(256B) + KCV(16B) + KSN(10B) + KeyLength(2B)
#define MSG_TAG_SEND_GENERATE2_KEY_ACK				0xD3
// 2014-04-14 erulil 추가 : RSA Public key injection command(plain key data), Index는 0~7
#define MSG_CMD_SEND_PUBLIC_KEY_NOTI				0x18D0  //  Length = 516	: RSA Key Index(1B) + Length(2B) + Zero(1B) + N(256B) + E(256B)
#define MSG_TAG_SEND_PUBLIC_KEY_NOTI				0xD0
#define MSG_CMD_SEND_PUBLIC_KEY_ACK					0x18D1  //  Length = 514 : Result(1B) + Code(1B) + N(256B) + E(256B)
#define MSG_TAG_SEND_PUBLIC_KEY_ACK					0xD1
// 2014-06-09 erulil 추가 : temp solution for BKM key (generate key or send public key 시 펌웨어 죽는 이슈 때문에 임시로 제작)
#define	MSG_CMD_SEND_GENERATE_KEY_BKM_NOTI			0x18D4	//	Length = 8	//	KeyIndex(1B) + KeyLength(1B) + KeyType(2B) + BankID(4B)
#define	MSG_TAG_SEND_GENERATE_KEY_BKM_NOTI			0xD4
#define	MSG_CMD_SEND_GENERATE_KEY_BKM_ACK			0x18D5	//	Length = 47	//	Result(1B) + Code(1B) + Key Data(32B) + KCV(3B) + KSN (10B)
#define	MSG_TAG_SEND_GENERATE_KEY_BKM_ACK			0xD5




// 2014-04-09 erulil 추가 : BBSecure_SendPaymentKey_simple 관련 cmd
#define	MSG_CMD_SEND_RKL_SIMPLE_NOTI				0x1824	//	Length = 52 : BankID(4B) + KeyType(1B) + SaveIndex(1B) + MasterIndex(1B) + KeyLength(1B) + Operation(1B) + Mode(1B) + WriteMode(1B) + KeyIndex(1B) + KeyData(32B) + IV(8B)
														//		KeyType:	1 = TDES Master, 2 = TDES PIN Encryption, 3 = TDES MAC Making, 4 = TDES General Purpose with MAC making, 7 = TDES General Purpose with MAC making, 5 = AES General Purpose with MAC making, 8 = AES General Purpose without MAC making, 6 = TDES Fixed
														//		Operation:	0 = Encrypt, 1 = Decrypt
														//		Mode:		0 = ECB, 1 = CBC
														//		Write Mode:	0 = Write Direct, 1 = write XOR result with Same Key Type Indexed Key(KeyIndex)
														//		KeyIndex:	using only when Write Mode == 1
														//		Note : If KeyType is 0 and MasterIndex = 0xFF, then KeyData is plane text
														//		Note : Key Value is left just in KeyData and remain right is filled zero
#define	MSG_TAG_SEND_RKL_SIMPLE_NOTI				0x24
#define	MSG_CMD_SEND_RKL_SIMPLE_ACK					0x1825	//	Length = 18 : Result(1B) + Code(1B) + KCV(16B)
#define	MSG_TAG_SEND_RKL_SIMPLE_ACK					0x25

// 2014-06-27 erulil
#define	MSG_CMD_FLASH_TEST_NOTIFY					0x18E6	//	Length = 2	// 2014-06-27 erulil test cmd
#define	MSG_TAG_FLASH_TEST_NOTIFY					0xE6	//	Length = 2	// 2014-06-27 erulil test cmd
#define	MSG_CMD_FLASH_TEST_ACK						0x18E7	//	Length = 2 // 2014-06-27 erulil test cmd
#define	MSG_TAG_FLASH_TEST_ACK						0xE7	//	Length = 2 // 2014-06-27 erulil test cmd


// 2014-09-29 erulil
#define	MSG_CMD_FAST_SEND_PW_NOTI					0x18BA	//	Length = 8
#define	MSG_CMD_FAST_SEND_PW_ACK					0x18BB	//	Length = 2
#define	MSG_TAG_FAST_SEND_PW_NOTI					0xBA	//	Length = 8
#define	MSG_TAG_FAST_SEND_PW_ACK					0xBB	//	Length = 2


// current time
#define MSG_CMD_GET_CURRENT_TIME_NOTI				0x1890
#define MSG_TAG_GET_CURRENT_TIME_NOTI				0x90
#define MSG_CMD_GET_CURRENT_TIME_ACK				0x1891
#define MSG_TAG_GET_CURRENT_TIME_ACK				0x91
#define MSG_CMD_SET_CURRENT_TIME_NOTI				0x1892
#define MSG_TAG_SET_CURRENT_TIME_NOTI				0x92
#define MSG_CMD_SET_CURRENT_TIME_ACK				0x1893
#define MSG_TAG_SET_CURRENT_TIME_ACK				0x93

// 2013-10-14 erulil KCV packet 관련은 보류
#define MSG_CMD_TDES_KCV_NOTI						0x1850
#define MSG_TAG_TDES_KCV_NOTI						0x50
#define MSG_CMD_TDES_KCV_ACK						0x1851
#define MSG_TAG_TDES_KCV_ACK						0x51
#define MSG_CMD_AES_KCV_NOTI						0x1852
#define MSG_TAG_AES_KCV_NOTI						0x52
#define MSG_CMD_AES_KCV_ACK							0x1853
#define MSG_TAG_AES_KCV_ACK							0x53
// 2013-10-01 erulil command 추가 ---------------------------- END




// 2013-10-01 erulil msg length 추가(command ~ crc) ---------------------------- START
#define MSG_LENGTH_COM_LENGTH_CRC						6 // command(2B) + length(2B) + crc(2B) = 6B
#define MSG_LENGTH_COMMAND								2
#define MSG_LENGTH_PACKET_LENGTH						2
#define MSG_LENGTH_CRC									2
#define MSG_LENGTH_APP_ID								4
#define MSG_LENGTH_CARDNUMBER							8
#define MSG_LENGTH_SN									8
#define MSG_LENGTH_KSN									10
#define MSG_LENGTH_KCV									3 // emv 규격에 따른 kcv length
#define MSG_LENGTH_KCV_AES								16 // kcv max length = using aes algorithm 
#define MSG_LENGTH_PIN_MIN								4
#define MSG_LENGTH_PIN_MAX								12
#define MSG_LENGTH_ENC_PIN								8
#define MSG_LENGTH_ENC_MAC_TDES_DUKPT					8
#define MSG_LENGTH_ENC_MAC_AES							16
#define MSG_LENGTH_CODE_KEY_HEADER						8
#define MSG_LENGTH_CODE_KEY_DATA						24
#define MSG_LENGTH_DUKPT_KEY_DATA						16
#define MSG_LENGTH_HEADER								16
#define MSG_LENGTH_GARANTI_FLAG							20
#define MSG_LENGTH_HEADER_KEY_USAGE						2
#define MSG_LENGTH_PAYLOAD								32
#define MSG_LENGTH_SHA_256_HASH							32
#define MSG_LENGTH_RAND_NUM								32
#define MSG_LENGTH_MASTER_KEY							24
#define MSG_LENGTH_DERIVATION_KEY						24
#define MSG_LENGTH_RSA_DATA_BUFFER						256
#define MSG_LENGTH_RSA_2048								256
#define MSG_LENGTH_FILE_LINE							1024 // 파일 송수신 시 한 패킷에 전달되는 파일 크기
#define MSG_LENGTH_RSA_KEY_PAIR							1408 // RSA private, public key 길이 합계
#define MSG_LENGTH_RESULT_PAYLOAD						18
#define MSG_LENGTH_OID									11
#define MSG_LENGTH_ENC_PIN_BLOCK						8
#define MSG_LENGTH_TIME									6
#define MSG_LENGTH_PASSWORD_HALF						4
#define MSG_LENGTH_TDES_IV								8
#define MSG_LENGTH_AES_IV								16

#define HEADER_LENGTH_KEY_INJECTION_PACKET				16 // key injection packet에서 사용하는 header의 길이
#define HEADER_LENGTH_KEY_USAGE							2 // key injection packet header 내에서 key usage 


#define MSG_LENGTH_CONNECTION_ACK						MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_CONNECTION_REQUEST					MSG_LENGTH_COM_LENGTH_CRC + 260
#define MSG_LENGTH_KEY_END								MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_KEY_END_ACK							MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_Fn_KEY_INPUT_ACK						MSG_LENGTH_COM_LENGTH_CRC + 18
#define MSG_LENGTH_Fn_KEY_INPUT_REQUEST					MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_PW_INPUT_REQUEST						MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_PW_INPUT_ACK							MSG_LENGTH_COM_LENGTH_CRC + 18
#define MSG_LENGTH_SN_REQUEST							MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_SN_ACK								MSG_LENGTH_COM_LENGTH_CRC + 8
#define MSG_LENGTH_PASSWORD_CORRECT						MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_CONNECTION_ALIVE_NOTI				MSG_LENGTH_COM_LENGTH_CRC // no data
#define MSG_LENGTH_CONNECTION_ALIVE_ACK					MSG_LENGTH_COM_LENGTH_CRC
#define	MSG_LENGTH_UI_DISCONNECTION_NOTIFY				MSG_LENGTH_COM_LENGTH_CRC	//	Length = 0, PCI disconnect command notify
#define	MSG_LENGTH_UI_DISCONNECTION_ACK					MSG_LENGTH_COM_LENGTH_CRC + 2	//	Length = 2, PCI disconnect command ack


#define MSG_LENGTH_CODE_KEY_INJECTION_NOTI							40 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_CODE_KEY_INJECTION_ACK							20 + MSG_LENGTH_COM_LENGTH_CRC // 2013-10-09 erulil 수정
#define MSG_LENGTH_RSA_KEY_PAIR_INJECTION_NOTI						1432 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RSA_KEY_PAIR_INJECTION_ACK						2 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_NOTI			24 + MSG_LENGTH_COM_LENGTH_CRC // data length is variable (512 or 640)
#define MSG_LENGTH_RSA_PRV_KEY_FOR_MOF_SSL_INJECTION_ACK			2 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_NOTI			24 + MSG_LENGTH_COM_LENGTH_CRC // data length is variable (512 or 640)
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_SSL_INJECTION_ACK			2 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_NOTI		24 + MSG_LENGTH_COM_LENGTH_CRC // data length is variable (512 or 640)
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_KEY_ENC_INJECTION_ACK		2 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_NOTI			24 + MSG_LENGTH_COM_LENGTH_CRC // data length is variable (512 or 640)
#define MSG_LENGTH_RSA_PRV_KEY_FOR_BANK_SIGN_INJECTION_ACK			2 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_SECURE_ROOM_PW_LOGIN_PK_INJECTION_NOTI			56 + MSG_LENGTH_COM_LENGTH_CRC // 2013-10-09 erulil 수정
#define MSG_LENGTH_SECURE_ROOM_PW_LOGIN_PK_INJECTION_ACK			20 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_TDES_PIN_OPERATION_NOTI							16 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_TDES_PIN_OPERATION_ACK							10 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_DUKPT_PIN_OPERATION_NOTI							16 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_DUKPT_PIN_OPERATION_ACK							20 + MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_PIN_DATA_ACK										MSG_LENGTH_COM_LENGTH_CRC + 18
#define MSG_LENGTH_PIN_DATA_NOTI									MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_ENCRYPT_PIN_CANCEL_NOTI							MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_ENCRYPT_PIN_CANCEL_ACK							MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_TDES_MAC_OPERATION_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 8 // data length is variable (1 ~ 1024)
#define MSG_LENGTH_TDES_MAC_OPERATION_ACK							MSG_LENGTH_COM_LENGTH_CRC + 10
#define MSG_LENGTH_DUKPT_MAC_OPERATION_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 8 // data length is variable (1 ~ 1024)
#define MSG_LENGTH_DUKPT_MAC_OPERATION_ACK							MSG_LENGTH_COM_LENGTH_CRC + 20
#define MSG_LENGTH_TDES_GP_MAC_OPERATION_NOTI						MSG_LENGTH_COM_LENGTH_CRC + 8 // data length is variable (1 ~ 1024)
#define MSG_LENGTH_TDES_GP_MAC_OPERATION_ACK						MSG_LENGTH_COM_LENGTH_CRC + 10
#define MSG_LENGTH_AES_GP_MAC_OPERATION_NOTI						MSG_LENGTH_COM_LENGTH_CRC + 8 // data length is variable (1 ~ 1024)
#define MSG_LENGTH_AES_GP_MAC_OPERATION_ACK							MSG_LENGTH_COM_LENGTH_CRC + 18
#define MSG_LENGTH_TDES_GP_KEY_ENC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 16 // data length is variable (8 ~ 512)
#define MSG_LENGTH_TDES_GP_KEY_ENC_ACK								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (8 ~ 512)
#define MSG_LENGTH_TDES_GP_KEY_DEC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 16 // data length is variable (8 ~ 512)
#define MSG_LENGTH_TDES_GP_KEY_DEC_ACK								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (8 ~ 512)
#define MSG_LENGTH_AES_GP_KEY_ENC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 24 // data length is variable (16 ~ 512)
#define MSG_LENGTH_AES_GP_KEY_ENC_ACK								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (16 ~ 512)
#define MSG_LENGTH_AES_GP_KEY_DEC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 24 // data length is variable (16 ~ 512)
#define MSG_LENGTH_AES_GP_KEY_DEC_ACK								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (16 ~ 512)
#define MSG_LENGTH_DUKPT_KEY_ENC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 16 // data length is variable (8 ~ 512)
#define MSG_LENGTH_DUKPT_KEY_ENC_ACK								MSG_LENGTH_COM_LENGTH_CRC + 14 // data length is variable (8 ~ 512)
#define MSG_LENGTH_RSA_ENC_OR_VERIFY_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (256 ~ 1024)
#define MSG_LENGTH_RSA_ENC_OR_VERIFY_ACK							MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (256 ~ 1024)
#define MSG_LENGTH_RSA_DEC_OR_SIGN_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (256 ~ 1024)
#define MSG_LENGTH_RSA_DEC_OR_SIGN_ACK								MSG_LENGTH_COM_LENGTH_CRC + 4 // data length is variable (256 ~ 1024)
#define MSG_LENGTH_GET_CURRENT_TIME_NOTI							MSG_LENGTH_COM_LENGTH_CRC // no data
#define MSG_LENGTH_GET_CURRENT_TIME_ACK								MSG_LENGTH_COM_LENGTH_CRC + 8
#define MSG_LENGTH_SET_CURRENT_TIME_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 6
#define MSG_LENGTH_SET_CURRENT_TIME_ACK								MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_PAYMENT_KEY_INJECTION_NOTI						MSG_LENGTH_COM_LENGTH_CRC + 56
#define MSG_LENGTH_PAYMENT_KEY_INJECTION_ACK						MSG_LENGTH_COM_LENGTH_CRC + 20
#define MSG_LENGTH_GARANTI_PAYMENT_KEY_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 788
#define MSG_LENGTH_GARANTI_PAYMENT_KEY_ACK							MSG_LENGTH_COM_LENGTH_CRC + 20

// 2014-02-19 erulil 추가
#define	MSG_LENGTH_SEND_CLEAR_KEYS_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 4
#define	MSG_LENGTH_SEND_CLEAR_KEYS_ACK								MSG_LENGTH_COM_LENGTH_CRC + 2

// 2014-03-17 erulil 추가
#define MSG_LENGTH_ERASE_KEY_NOTI									MSG_LENGTH_COM_LENGTH_CRC + 6
#define MSG_LENGTH_ERASE_KEY_ACK									MSG_LENGTH_COM_LENGTH_CRC + 2
#define MSG_LENGTH_GET_KCV_NOTI										MSG_LENGTH_COM_LENGTH_CRC + 8
#define MSG_LENGTH_GET_KCV_ACK										MSG_LENGTH_COM_LENGTH_CRC + 18

// 2013-11-21 erulil 추가
#define MSG_LENGTH_PUBLIC_KEY_REQUEST 								MSG_LENGTH_COM_LENGTH_CRC + 298 //852 : public key length 554 빠짐
#define MSG_LENGTH_PUBLIC_KEY_ACK									MSG_LENGTH_COM_LENGTH_CRC + 2 //556 : public key length 554 빠짐
#define MSG_LENGTH_RAND_NUM_REQUEST									MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_RAND_NUM_ACK										MSG_LENGTH_COM_LENGTH_CRC + 34
#define MSG_LENGTH_PAYMENT_KEY_REQUEST_PC							MSG_LENGTH_COM_LENGTH_CRC + 512 // from pc
#define MSG_LENGTH_PAYMENT_KEY_REQUEST_PDA							MSG_LENGTH_COM_LENGTH_CRC + 288 // to MAXQ
#define MSG_LENGTH_PAYMENT_KEY_ACK									MSG_LENGTH_COM_LENGTH_CRC + 20 // from MAXQ


// 2014-01-17 erulil 터미널에서 생성한 RSA key pair 관련 packet 추가
#define	MSG_LENGTH_SEND_TERMINAL_KEY_PAIR_NOTIFY					MSG_LENGTH_COM_LENGTH_CRC + 1432
#define	MSG_LENGTH_SEND_TERMINAL_KEY_PAIR_ACK						MSG_LENGTH_COM_LENGTH_CRC + 2
#define	MSG_LENGTH_SEND_TERMINAL_KEY_CONFIRM_NOTIFY					MSG_LENGTH_COM_LENGTH_CRC
#define	MSG_LENGTH_SEND_TERMINAL_KEY_CONFIRM_ACK					MSG_LENGTH_COM_LENGTH_CRC + 2


// 2014-04-07 erulil 추가
#define MSG_LENGTH_SEND_GENERATE_KEY_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 522
#define MSG_LENGTH_SEND_GENERATE_KEY_ACK							MSG_LENGTH_COM_LENGTH_CRC + 284 // Result(1B) + Code(1B) + EncryptedKey(256B) + KCV(16B) + KSN(10B)
// 2014-04-14 erulil 추가
#define MSG_LENGTH_SEND_GENERATE2_KEY_NOTI							MSG_LENGTH_COM_LENGTH_CRC + 10 // KeyIndex(1B) + KeyLength(1B) + KeyType(2B) + BankID(4B) + Public Key Index(1B) + Zero(1B)
#define MSG_LENGTH_SEND_GENERATE2_KEY_ACK							MSG_LENGTH_COM_LENGTH_CRC + 286 // Result(1B) + Code(1B) + EncryptedKey(256B) + KCV(16B) + KSN(10B) + Key Length(2B)
// 2014-04-14 erulil 추가 : RSA Public key injection command(plain key data), Index는 0~7
#define MSG_LENGTH_SEND_PUBLIC_KEY_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 516 // RSA Key Index(1B) + Length(2B) + Zero(1B) + N(256B) + E(256B)
#define MSG_LENGTH_SEND_PUBLIC_KEY_ACK								MSG_LENGTH_COM_LENGTH_CRC + 514 // Result(1B) + Code(1B) + N(256B) + E(256B)

// 2014-04-09 erulil 추가
#define MSG_LENGTH_SEND_RKL_SIMPLE_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 52
#define MSG_LENGTH_SEND_RKL_SIMPLE_ACK								MSG_LENGTH_COM_LENGTH_CRC + 18

// 2014-06-09 erulil 추가
#define	MSG_LENGTH_SEND_GENERATE_KEY_BKM_NOTI						MSG_LENGTH_COM_LENGTH_CRC + 8	//	KeyIndex(1B) + KeyLength(1B) + KeyType(2B) + BankID(4B)
#define	MSG_LENGTH_SEND_GENERATE_KEY_BKM_ACK						MSG_LENGTH_COM_LENGTH_CRC + 47	//	Result(1B) + Code(1B) + Key Data(32B) + KCV(3B) + KSN (10B)



// 2014-09-29 erulil
#define	MSG_LENGTH_FAST_SEND_PW_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 8	//	Length = 8
#define	MSG_LENGTH_FAST_SEND_PW_ACK									MSG_LENGTH_COM_LENGTH_CRC + 2	//	Length = 2



#define MSG_LENGTH_FILE_MAX_TRASFER									MSG_LENGTH_COM_LENGTH_CRC + MSG_LENGTH_FILE_LINE
// 0x19 msg start
#define MSG_LENGTH_READY_RECV_PUBLIC_REQUEST						MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_FILE_RECV_START									MSG_LENGTH_COM_LENGTH_CRC
//#define MSG_LENGTH_FILE_RECV_ING									MSG_LENGTH_COM_LENGTH_CRC + LENGTH_FILE_MAX_TRASFER
#define MSG_LENGTH_FILE_RECV_ING									MSG_LENGTH_COM_LENGTH_CRC + MSG_LENGTH_FILE_LINE
#define MSG_LENGTH_FILE_RECV_END									MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_FILE_SEND_START									MSG_LENGTH_COM_LENGTH_CRC
//#define MSG_LENGTH_FILE_SEND_ING									MSG_LENGTH_COM_LENGTH_CRC + LENGTH_FILE_MAX_TRASFER
#define MSG_LENGTH_FILE_SEND_ING									MSG_LENGTH_COM_LENGTH_CRC + MSG_LENGTH_FILE_LINE
#define MSG_LENGTH_FILE_SEND_END									MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_SEND_RSA_PUBLIC_NOTI								MSG_LENGTH_COM_LENGTH_CRC + 2 + (MSG_LENGTH_RSA_DATA_BUFFER * 2) // command + length + make state 2B(result code, code - 0:fail, 1:success), N(256) + E(256) + crc
#define MSG_LENGTH_SEND_RSA_PUBLIC_ACK								MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_MAXQ_CONNECTION_CHECK_NOTI						MSG_LENGTH_COM_LENGTH_CRC
#define MSG_LENGTH_MAXQ_CONNECTION_CHECK_ACK						MSG_LENGTH_COM_LENGTH_CRC + (24 * 2) + 1 + 1 // command + length + connection state(1 : 0-FALSE or 1-TRUE) + connection mode(1) + tmack(24) + ptk(24) + crc
// 0x19 msg end


// 2013-10-14 erulil KCV packet 관련은 보류 START
#define MSG_LENGTH_TDES_KCV_NOTI									MSG_LENGTH_COM_LENGTH_CRC + 8
#define MSG_LENGTH_TDES_KCV_ACK										MSG_LENGTH_COM_LENGTH_CRC + 10
#define MSG_LENGTH_AES_KCV_NOTI										MSG_LENGTH_COM_LENGTH_CRC + 8
#define MSG_LENGTH_AES_KCV_ACK										MSG_LENGTH_COM_LENGTH_CRC + 18
// 2013-10-14 erulil KCV packet 관련은 보류 END


#define DATA_MAX_LENGTH_GP_DUKPT						512 // data enc & dec 할 때 max data length : AES GP, TDES GP, DUKPT
#define DATA_MAX_LENGTH_RSA								1024 // data enc & dec 할 때 max data length : RSA

#define MULTIPLE_NUMBER_ENC_DATA_AES_GP					16 // data enc & dec 할 때 data length가 이 수의 배수여야만 한다 : AES GP key 사용 시
#define MULTIPLE_NUMBER_ENC_DATA_TDES_GP				8 // data enc & dec 할 때 data length가 이 수의 배수여야만 한다 : TDES GP key 사용 시
#define MULTIPLE_NUMBER_ENC_DATA_DUKPT					8 // data enc & dec 할 때 data length가 이 수의 배수여야만 한다 : DUKPT key 사용 시
#define MULTIPLE_NUMBER_ENC_DATA_RSA					256 // data enc & dec 할 때 data length가 이 수의 배수여야만 한다 : RSA key 사용 시
// 2013-10-01 erulil msg length(command ~ crc) 추가 ---------------------------- END



//////////////////////////////////////////////////////////////////////////
//

//buffer size
#define MAX_RECV_BUFF_SIZE				2048


enum TERMINAL_UI_TYPE
{
	TERMINAL_UI_TYPE_NONE = 0,
	TERMINAL_UI_TYPE_1,
	TERMINAL_UI_TYPE_2,
	TERMINAL_UI_TYPE_3,
};

enum MODEL_TYPE
{            
	MODEL_MT_760 = 0,
	MODEL_MT_360,
	MODEL_CT,
};

enum PORT_TYPE
{            
	PORT_TYPE_PC = 1,		// pc에서 포트 오픈
	PORT_TYPE_PDA_MAXQ,		// pda에서 포트 오픈 : maxq와 연결
	PORT_TYPE_PDA_PC,		// pda에서 포트 오픈 : pc와 연결
};

enum OP_MODE_ENC_DEC
{
	OPMODE_ECB = 0,
	OPMODE_CBC,
};

enum SIMPLE_KEY_WRITE_MODE
{
	WRITE_MODE_DIRECTLY = 0, // plain key 바로 write
	WRITE_MODE_ENCRYPT, // key data enc해서 write
	WRITE_MODE_DECRYPT, // key data dec해서 write
};

enum OPER_TYPE
{
	ENCRYPTION = 0,
	DECRYPTION = 1,
};

enum RSA_KEY_ID
{
	RSA_KEY_ID_0 = 0, // terminal key
	RSA_KEY_ID_1,
	RSA_KEY_ID_2,
	RSA_KEY_ID_3,
	RSA_KEY_ID_4,
};

enum MAC_MODE
{
	ISO9797_1 = 0,
	ISO9797_2,
	ISO9797_3,
	ISO9797_4,
	ISO9797_5,
	ISO9797_6,
};

enum BLOCK_FORMAT
{
	BLOCK_FORMAT_0 = 0,
	BLOCK_FORMAT_1 = 1,
	BLOCK_FORMAT_3 = 3,
};

enum PCI_KEY_TYPE
{
	KEY_LSK = 0,
	KEY_BSK,
	KEY_USK,
	KEY_KSK,
	KEY_FSK,
	KEY_RSA,
	KEY_PAYMENT,
	KEY_PAYMENT_NOR,
};

enum KEYPAD_INPUT_DISPLAY
{            
	SHOW_STAR = 0,		// '*'
	SHOW_NUMBER,		// show number
	SHOW_NO,			// no show
};

enum MAXQ_CONNECTION_MODE
{
	NORMAL = 0,
	INPUT_IMAGE_KEY,
	INPUT_MASTER_KEY,
	FACTORY_PASSWORD,
	USER_PASSWORD,
	INPUT_CODE_SIGNATURE, // img update
	REPAIR,
};


#define WM_COMM_READ_FROM_PC                    (WM_USER + 1) // comm pci에서 사용. pc msg 수신
#define WM_SEND_PACKET_ACK						(WM_USER + 2) // pc lib에서 사용
#define WM_COMM_READ_MAXQ                       (WM_USER + 4)
#define WM_RECV_PCIPAD_DLL                      (WM_USER + 6) // 2013-10-04 erulil pcipad.dll에서 전달받는 메시지. comm pci 에서 사용
#define WM_PIN_TIMEOUT							(WM_USER + 7) // 2014-02-07 erulil PIN operation 시 time out이 발생하면 pin dlg로 메시지를 전달하기 위해 사용함

#define BUTTON_DEFAULT							WM_USER+339
#define BUTTON_KEYPAD							WM_USER+340


static unsigned char g_byOID[11] = { 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01 };


/**********************************************************************************/
/**********************************************************************************/
#define	RESULT_OK							0x00
#define	RESULT_NOT_READY					0xF0
#define	RESULT_ERROR						0x0F
/**********************************************************************************/
/**********************************************************************************/
#define	COMMAND_RESULT_CODE_NONE						0x00
#define	COMMAND_RESULT_CODE_ERROR_LENGTH				0x01
#define	COMMAND_RESULT_CODE_ERROR_CRC					0x02
#define	COMMAND_RESULT_CODE_ERROR_IN_PAYLOAD			0x03
#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_DECRYPT		0x05
#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_MAC			0x06
#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_SAVE			0x07
#define	COMMAND_RESULT_CODE_ERROR_IN_MASTER_KEY			0x08
#define	COMMAND_RESULT_CODE_ERROR_IN_MAC_KEY			0x09	//	SK_MACK error
#define	COMMAND_RESULT_CODE_ERROR_NO_MEMORY				0x0A
#define	COMMAND_RESULT_CODE_ERROR_TIMEOUT				0x0B
#define	COMMAND_RESULT_CODE_ERROR_IN_PIN				0x0C
#define	COMMAND_RESULT_CODE_ERROR_IN_HASH_RECEIVE_TWICE	0x0D
#define	COMMAND_RESULT_CODE_ERROR_IN_HASH_WRITE			0x0E
#define	COMMAND_RESULT_CODE_ERROR_READ_TIME				0x0F

#define	COMMAND_RESULT_CODE_ERROR_KEY_IS_SAME_WITH_COMMUNICATION_KEY	0x10
#define	COMMAND_RESULT_CODE_ERROR_KEY_IS_SAME_WITH_MASTER_KEY			0x11
#define	COMMAND_RESULT_CODE_ERROR_SET_TIME_IS_CURRENT_TIME				0x12
#define	COMMAND_RESULT_CODE_ERROR_SET_TIME_IS_INVALID					0x13
#define	COMMAND_RESULT_CODE_ERROR_IN_READ_SIGNATURE						0x14
#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_READ							0x15
#define	COMMAND_RESULT_CODE_ERROR_NOT_SAME_KEY							0x16 // COMMAND_RESULT_CODE_ERROR_SAME_KEY_IS_EXIST
#define	COMMAND_RESULT_CODE_ERROR_IN_DELETE_KEY							0x17
#define	COMMAND_RESULT_CODE_ERROR_IN_PIN_REQUEST_SIZE					0x18

#define	COMMAND_RESULT_CODE_ERROR_PIN_KEY_AND_MAC_KEY_ARE_SAME			0x19

#define	COMMAND_RESULT_CODE_ERROR_IN_PIN_ENCRYPT_INTERVAL				0x20

#ifdef	IMPLEMENT_FACTORY_FAST_PRODUCT
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_FAST_PRODUCT				0x30
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_SAME_PASSWORD				0x31
#endif	//	IMPLEMENT_FACTORY_FAST_PRODUCT

#define	COMMAND_RESULT_CODE_ERROR_HERADER_ERROR							0x32
#define	COMMAND_RESULT_CODE_ERROR_MASTER_KEY_LENGTH						0x33
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_USAGE						0x34
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_EXPORTABILITY				0x35
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_ALGORITHM					0x36
#define	COMMAND_RESULT_CODE_ERROR_NOT_SUPPORT_METHOD					0x37

#define	COMMAND_RESULT_CODE_ERROR_DUKPT_SAVE							0x40
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_INITIAL							0x41
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_PACKET							0x42
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_INDEX					0x43
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_USE						0x44
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_VERSION					0x45
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_MODE						0x46
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_ALGORITHM				0x47
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_USAGE					0x48
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_HEADER_METHOD					0x49
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_ECRYPT_PIN						0x4A
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_ECRYPT_MAC						0x4B
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_MODE							0x4D

#define	COMMAND_RESULT_CODE_ERROR_RSA_SAVE								0x50
#define	COMMAND_RESULT_CODE_ERROR_RSA_ALREADY_SAVED						0x51
#define	COMMAND_RESULT_CODE_ERROR_RSA_PACKET							0x52
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_INDEX						0x53
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_VERSION					0x54
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_MODE						0x55
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_ALGORITHM					0x56
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_USAGE						0x57
#define	COMMAND_RESULT_CODE_ERROR_RSA_HEADER_METHOD						0x58
#define	COMMAND_RESULT_CODE_ERROR_RSA_MODE								0x59
#define	COMMAND_RESULT_CODE_ERROR_RSA_INDEX								0x5A
#define	COMMAND_RESULT_CODE_ERROR_RSA_KEY_NONE							0x5B
#define	COMMAND_RESULT_CODE_ERROR_RSA_KEY_TYPE							0x5C
#define	COMMAND_RESULT_CODE_ERROR_RSA_OPERATION							0x5D

#define	COMMAND_RESULT_CODE_ERROR_RSA_KEY_USAGE							0x5E

// 2014-04-14 erulil 추가
#define	COMMAND_RESULT_CODE_ERROR_PUBLIC_KEY_INDEX						0x60
#define	COMMAND_RESULT_CODE_ERROR_PUBLIC_KEY_WRITE						0x61

#define	COMMAND_RESULT_CODE_ERROR_CODE_KEY_INDEX						0x80

#define	COMMAND_RESULT_CODE_ERROR_HEADER_METHOD							0x81
#define	COMMAND_RESULT_CODE_ERROR_HEADER_KEY_USAGE						0x82
#define	COMMAND_RESULT_CODE_ERROR_HEADER_ALGORITHM						0x83
#define	COMMAND_RESULT_CODE_ERROR_HEADER_BANK_ID						0x84
#define	COMMAND_RESULT_CODE_ERROR_HEADER_INDEX							0x85
#define	COMMAND_RESULT_CODE_ERROR_HEADER_LENGTH							0x86
#define	COMMAND_RESULT_CODE_ERROR_HEADER_VERSION						0x87
#define	COMMAND_RESULT_CODE_ERROR_HEADER_MASTER_INDEX					0x88
#define	COMMAND_RESULT_CODE_ERROR_HEADER_MODE							0x89
#define	COMMAND_RESULT_CODE_ERROR_HEADER_INDICATOR						0x8A

#define	COMMAND_RESULT_CODE_ERROR_IN_KCV								0x90
#define	COMMAND_RESULT_CODE_ERROR_IN_DUKPT_INITIAL_KEY_SAVE				0x91
#define	COMMAND_RESULT_CODE_ERROR_IN_PIN_ENCRYPT_BLOCK_FORMAT			0x92
#define	COMMAND_RESULT_CODE_ERROR_IN_PIN_ENCRYPT_KEY_FIND				0x93
#define	COMMAND_RESULT_CODE_ERROR_IN_PIN_KEY_PERMISSION					0x94
#define	COMMAND_RESULT_CODE_ERROR_NOT_PERMITTED_KEY_INJECTION			0x95

#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_INDEX							0x96
#define	COMMAND_RESULT_CODE_ERROR_CIPHER_OPERATION_MODE					0x97
#define	COMMAND_RESULT_CODE_ERROR_IN_ENCRYPT_KEY_PERMISSION				0x98
#define	COMMAND_RESULT_CODE_ERROR_IN_DECRYPT_KEY_PERMISSION				0x99
#define	COMMAND_RESULT_CODE_ERROR_IN_MAKE_MAC_KEY_PERMISSION			0x9A
#define	COMMAND_RESULT_CODE_ERROR_DUKPT_KEY_PERMISSION					0x4E

#define	COMMAND_RESULT_CODE_ERROR_DUKPT_ECRYPT_DATA						0x4C
#define	COMMAND_RESULT_CODE_ERROR_TDES_ECRYPT_DATA						0x9B
#define	COMMAND_RESULT_CODE_ERROR_TDES_DERYPT_DATA						0x9C
#define	COMMAND_RESULT_CODE_ERROR_TDES_MAKE_MAC							0x9D
#define	COMMAND_RESULT_CODE_ERROR_AES_ECRYPT_DATA						0x9E
#define	COMMAND_RESULT_CODE_ERROR_AES_DERYPT_DATA						0x9F
#define	COMMAND_RESULT_CODE_ERROR_AES_MAKE_MAC							0xA0
#define	COMMAND_RESULT_CODE_ERROR_IN_KEY_LENGTH							0xA1
#define	COMMAND_RESULT_CODE_ERROR_MAC_ALGORITHM							0xA2

#define	COMMAND_RESULT_CODE_ERROR_RKL_DECRYPT							0xB0 // RKL : Remote Key Loading
#define	COMMAND_RESULT_CODE_ERROR_RKL_PACKET							0xB1 // RKL : Remote Key Loading
#define	COMMAND_RESULT_CODE_ERROR_RKL_SN								0xB2 // RKL : Remote Key Loading
#define	COMMAND_RESULT_CODE_ERROR_RKL_GARANTI_DECRYPT					0xB3
#define	COMMAND_RESULT_CODE_ERROR_RKL_GARANTI_PACKET					0xB4
#define	COMMAND_RESULT_CODE_ERROR_CLEAR_KEYS							0xB5

#define	COMMAND_RESULT_CODE_ERROR_CLEAR_ONE_KEY_TYPE					0xB6 // key type 범위를 벗어났을 때 발생
#define	COMMAND_RESULT_CODE_ERROR_CLEAR_ONE_KEY_INDEX					0xB7 // index 범위를 벗어났을 때 발생
#define	COMMAND_RESULT_CODE_ERROR_CLEAR_ONE_KEY							0xB8 // bank id가 없거나 기타 오류 상황일 때 발생

#define	COMMAND_RESULT_CODE_ERROR_KEY_CKECK_KEY_TYPE					0xB6
#define	COMMAND_RESULT_CODE_ERROR_KEY_CKECK_KEY_INDEX					0xB7
#define	COMMAND_RESULT_CODE_ERROR_KEY_CKECK_KEY_READ					0xB8
#define	COMMAND_RESULT_CODE_ERROR_KEY_CKECK_VALUE						0xB9

#define	COMMAND_RESULT_CODE_ERROR_THERE_IS_NO_TERMINAL_KEY				0xC0
#define	COMMAND_RESULT_CODE_ERROR_ALREADY_CONFIRMED_TERMINAL_KEY		0xC1
#define	COMMAND_RESULT_CODE_ERROR_NOT_RECEIVED_TERMINAL_KEY				0xC2

// 2014-04-09 erulil 추가 : BBSecure_SendPaymentKey_simple 관련 error code
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_KEY_TYPE					0xC3
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_SAVE_INDEX					0xC4
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_MASTER_INDEX				0xC5
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_KEY_SIZE					0xC6
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_OPERATION					0xC7
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_MODE						0xC8
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_READ_MASTER_KEY			0xC9
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_SAME_KEY					0xCA
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_WRITE						0xCB
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_KCV						0xCC

// 2014-04-14 erulil 추가
#define	COMMAND_RESULT_CODE_ERROR_RKL_WRITE_MODE						0xCD
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_WITER_MODE_KEY_INDEX		0xCE
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_READ_WRITE_MODE_KEY		0xCF
#define	COMMAND_RESULT_CODE_ERROR_RKL_SIMPLE_READ_WRITE_MODE_KEY_SIZE	0xDF

// 2014-04-07 erulil 추가
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_PARAMETER_TYPE			0xD0
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_PARAMETER_RSA_LENGTH		0xD1
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_PARAMETER_LENGTH			0xD2
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_PARAMETER_INDEX			0xD3
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_SAVE						0xD4
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_KCV						0xD5
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_ENCRYPT					0xD6

// 2014-04-14 erulil 추가
#define	COMMAND_RESULT_CODE_ERROR_GENERATE_KEY_READ_PUBLIC				0xD7


// 2013-11-24 erulil 펌웨어와 상관없는 app 자체 에러 코드
#define EMVPIN_CODE_ERROR_INVALID_INPUT_PARAM				0x00000001 // invalid input parameter
#define EMVPIN_CODE_ERROR_INPUT_PIN_TIMEOUT					0x00000002 // input pin tiume out
#define EMVPIN_CODE_ERROR_INPUT_CANCEL						0x00000003 // input pin cancel
#define EMVPIN_CODE_ERROR_OUTPUT_BUFFER_LENGTH				0x00000004 // output buffer length error (less 256)
#define EMVPIN_CODE_ERROR_INVALID_INPUT_PARAM_LENGTH_ZERO	0x00000005 // invalid input parameter
#define EMVPIN_CODE_ERROR_INVALID_INPUT_PARAM_LENGTH		0x00000006 // invalid input parameter

#define	PCIAPP_ERROR_MAKE_PACKET_FAIL					0x10F0 // make packet fail
#define	PCIAPP_ERROR_FILE_CREATE_FAIL					0x10F1 // file crate fail
#define	PCIAPP_ERROR_FILE_CLOSE_FAIL					0x10F2 // file close fail
#define	PCIAPP_ERROR_FILE_WRITE_FAIL					0x10F3 // file write fail
#define	PCIAPP_ERROR_INPUT_BUFFER_NULL					0x10F4 // input buffer 오류
//#define	PCIAPP_ERROR_CREATE_FILE_FAIL					0x10F5 // file create fail
#define	PCIAPP_ERROR_FILE_OPEN_FAIL						0x10F6 // file open fail
#define	PCIAPP_ERROR_CREATE_HASH						0x10F7 // 해시 생성 실패
#define	PCIAPP_ERROR_INVALID_BUFFER						0x10F8 // output 버퍼 오류
#define PCIAPP_ERROR_INVALID_INPUT_PARAM				0x1001 // pin 입력 시 timeout
#define PCIAPP_ERROR_INPUT_PIN_TIMEOUT					0x1002 // pin dlg cancel
#define PCIAPP_ERROR_INPUT_PIN_CANCEL					0x1003 // 2013-10-09 erulil 추가 pin 입력을 하지 않고 dlg cancel 하면 반환
#define PCIAPP_ERROR_ALREADY_INIT						0x1004 // 2013-09-24 erulil 추가 이미 초기화 된 상태인데 다시 시도했을 경우(pMainThread가 생성된 상태)
#define PCIAPP_ERROR_INVALID_OP_MODE					0x1006 // 2013-10-04 erulil 추가 op mode를 지정된 값 이외의 값을 넣었을 때 반환
#define PCIAPP_ERROR_NO_APP_HANDLE						0x1007 // 2013-10-06 erulil 추가 g_hApp이 없을 경우 반환
#define PCIAPP_ERROR_OPERATION							0x1009 // 2013-10-09 erulil 추가 nOpType 오류. OPER_TYPE 이외의 값이 들어오면 반환됨
#define PCIAPP_ERROR_KEY_ID								0x1010 // key id를 잘못 넣은 경우
#define PCIAPP_ERROR_KEY_TYPE							0x1011 // key type을 잘못 넣은 경우
//#define PCIAPP_ERROR_BUFFER   							0x1013 // output buffer가 null이거나 length가 null 혹은 0인 경우 :  PCIAPP_ERROR_INVALID_BUFFER랑 겹쳐서 뺌
#define PCIAPP_ERROR_NOT_READY							0x1014 // maxQ가 아직 준비되지 않은 경우(미연결)
#define	PCIAPP_ERROR_KEY_INDEX							0x1015 // invalid key index
#define	PCIAPP_ERROR_NOT_FOUND_RECV_MSG_MAP				0x1016 // m_mapRecvMsgData에서 인덱스를 찾을 수 없음
#define	PCIAPP_ERROR_WAIT_OBJECT_FAIL					0x1017 // wait for object가 -1을 리턴할 경우
#define	PCIAPP_ERROR_NOT_CONNECTION_MAXQ				0x1018 // maxq에 연결되지 않음
#define	PCIAPP_ERROR_NOT_OPEN_PCI						0x1019 // pci api를 사용할 수 있는 환경이 아님(pci open 필요)
#define	PCIAPP_ERROR_PORT_OPEN							0x1020 // PORT open fail
#define PCIAPP_ERROR_RSA_KEY_PAIR_MATCH					0x1021 // 2014-01-15 erulil RSA key pair 유효성 검사 실패
#define PCIAPP_ERROR_RSA_KEY_PAIR_CREATE				0x1022 // 2014-01-15 erulil RSA key pair 생성 실패
#define PCIAPP_ERROR_OPEN_RSA_PUBLIC_KEY_FILE			0x1023 // 2014-01-15 erulil RSA key file이 존재하지 않음
#define PCIAPP_ERROR_WAIT_TIME_OUT						0x1024 // 2014-02-02 erulil 어플리케이션 타임아웃 코드 추가(waitforsingleobject 에서 타임아웃 났을 때 발생함)
#define PCIAPP_ERROR_CERT_LENGTH_OVER					0x1025 // 2014-02-20 erulil 인증서 데이터의 길이가 내부 버퍼 길이를 초과하면 반환된다
#define PCIAPP_ERROR_OPERATION_MODE						0x1026 // 2014-03-27 erulil operation mode 오류. OPMODE_ECB, OPMODE_CBC 이외의 값이 들어오면 반환됨
#define PCIAPP_ERROR_GET_HASH_FAIL						0x1027 // 2014-03-27 erulil create hash value - fail
#define PCIAPP_ERROR_INVALID_HASH_ALGORITHM				0x1028 // 2014-03-27 erulil invalid hash algorithm
#define PCIAPP_ERROR_RSA_KEY_NOT_COMPARE				0x1029 // 2014-04-17 erulil BBSecure_SendRSAPublicKey에서 input rsa key와 저장 후 반환된 rsa key가 같지 않을 떄
#define PCIAPP_ERROR_CREATE_QUEUE						0x102A // 2014-05-25 erulil create queue fail
#define PCIAPP_ERROR_INVALID_WRITE_MODE					0x102B // 2014-08-12 erulil 추가
#define KEY_USAGETYPE_RSA					9
/**********************************************************************************/




//Connection Request
#define	CONNECTION_MODE_REQUEST_NORMAL				0x00 // normal 모드
#define	CONNECTION_MODE_REQUEST_FACTORY_PASSWORD	0x01 // Factory PW 설정
#define	CONNECTION_MODE_REQUEST_SIGNATURE_CODE		0x02 // 이미지 업데이트
#define	CONNECTION_MODE_REQUEST_SIGNATURE_KEY		0x04 // 코드키 입력
#define	CONNECTION_MODE_REQUEST_USER_PASSWORD		0x08 // user PW 설정
#define	CONNECTION_MODE_REQUEST_MASTER_KEY			0x10 // master key 입력
#define	CONNECTION_MODE_REPAIR						0xFF // reapaire 모드






typedef struct _stKeyInfo
{
	int nType;	      // Key Type 	PCI_KEY_TYPE
	int nIndex;	      // Key 의 index
	int nMIdex;	      // Master Key Index 
	int nLength;      // Key 길이 
    int nUsing;       // Using 
	BYTE byHeader[16]; // 2013-10-21 erulil 추가
	BYTE byKSN[10]; // 2013-10-21 erulil 추가
	BYTE byKeyData[2048];
	_stKeyInfo::_stKeyInfo()
	{
		nType   = 0;
		nIndex  = 0;
		nMIdex  = 0;
        nLength = 0;
		nUsing  = 0x38;
		memset(byKeyData,0,sizeof(byKeyData));
		memset(byHeader,0,sizeof(byHeader));
		memset(byKSN,0,sizeof(byKSN));
	}
} KEYINFO, *PKEYINFO;


// 2013-11-14 erulil 추가
struct KEYINFO_RSA
{
	int nType;
	int nLength;
	BYTE byKeyData[2048];

	KEYINFO_RSA()
	{
		nType   = 0;
        nLength = 0;
		memset(byKeyData,0,sizeof(byKeyData));
	}

	KEYINFO_RSA(KEYINFO *pCopy)
	{
		nType = pCopy->nType;
        nLength = pCopy->nLength;
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}
};

// 2013-11-12 erulil 추가, payment key용 구조체
struct KEY_INFO_PAYMENT
{
	int nIndex;	
	int nMIdex;	
	int nLength;
	BYTE byUsage[2];
	BYTE byHeader[16];
	BYTE byKSN[10];
    BYTE byKeyData[32];

	KEY_INFO_PAYMENT()
	{
		nIndex  = 0;
		nMIdex  = 0;
        nLength = 0;
		memset(byKeyData,0,sizeof(byKeyData));
		memset(byHeader,0,sizeof(byHeader));
		memset(byKSN,0,sizeof(byKSN));
		memset(byUsage,0,sizeof(byUsage));
	}

	KEY_INFO_PAYMENT(KEYINFO *pCopy)
	{
		nIndex = pCopy->nIndex;
		nMIdex = pCopy->nMIdex;
		nLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
		memcpy(byHeader, pCopy->byHeader, sizeof(byHeader));
		byUsage[0] = byHeader[1];
		byUsage[1] = byHeader[2];
	}

	KEY_INFO_PAYMENT(KEY_INFO_PAYMENT *pCopy)
	{
		nIndex = pCopy->nIndex;
		nMIdex = pCopy->nMIdex;
		nLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
		memcpy(byHeader, pCopy->byHeader, sizeof(byHeader));
		memcpy(byUsage, pCopy->byUsage, sizeof(byUsage));
	}

	void Copy(KEY_INFO_PAYMENT *pCopy)
	{
		nIndex = pCopy->nIndex;
		nMIdex = pCopy->nMIdex;
		nLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
		memcpy(byHeader, pCopy->byHeader, sizeof(byHeader));
		memcpy(byUsage, pCopy->byUsage, sizeof(byUsage));
	}
};

// 2013-11-08 erulil 추가
struct PAYMENT_MASTER_KEY
{
	int nIndex;
	BYTE byKSN[10]; // 2013-10-21 erulil 추가
	int nKeyLength;
	BYTE byKeyData[24];

	PAYMENT_MASTER_KEY()
		: nIndex(0)
		, nKeyLength(0)
	{
		memset(byKSN,0,sizeof(byKSN));
		memset(byKeyData,0,sizeof(byKeyData));
	}

	PAYMENT_MASTER_KEY(PAYMENT_MASTER_KEY *pCopy)
	{
		nIndex = pCopy->nIndex;
		nKeyLength = pCopy->nKeyLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}

	PAYMENT_MASTER_KEY(KEYINFO *pCopy)
	{
		nIndex = pCopy->nIndex;
		nKeyLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}

	PAYMENT_MASTER_KEY(KEY_INFO_PAYMENT *pCopy)
	{
		nIndex = pCopy->nIndex;
		nKeyLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}

	void Copy(KEYINFO *pCopy)
	{
		nIndex = pCopy->nIndex;
		nKeyLength = pCopy->nLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}

	void Copy(PAYMENT_MASTER_KEY *pCopy)
	{
		nIndex = pCopy->nIndex;
		nKeyLength = pCopy->nKeyLength;
		memcpy(byKSN, pCopy->byKSN, sizeof(byKSN));
		memcpy(byKeyData, pCopy->byKeyData, sizeof(byKeyData));
	}
};



static unsigned char pN_CA[256] =
{
	0xDD, 0x6F, 0xBC, 0xFE, 0x12, 0x1D, 0x99, 0x7E, 0xFC, 0x9B, 0x18, 0xAA, 0x09, 0x5D, 0xB2, 0x1C, 
	0xF4, 0xF8, 0x39, 0x7A, 0x8B, 0x9E, 0x85, 0x1B, 0x4A, 0x0A, 0x58, 0x7F, 0xDF, 0x61, 0x17, 0x09, 
	0xBA, 0x10, 0x47, 0x55, 0x41, 0xDF, 0xAF, 0xAC, 0x69, 0xB7, 0xFF, 0xD2, 0xBC, 0x01, 0x85, 0xA6, 
	0x7E, 0x7C, 0xB9, 0xA1, 0xD8, 0x52, 0x6E, 0x23, 0x4D, 0x2B, 0x60, 0xAA, 0xE6, 0x0C, 0x07, 0x9B, 
	0x92, 0x67, 0x33, 0x1B, 0x64, 0x3B, 0xA3, 0x98, 0x6B, 0xFF, 0xD3, 0x42, 0xA3, 0x1B, 0x69, 0x08, 
	0x87, 0x3B, 0xBD, 0x5B, 0xE1, 0x0C, 0xE5, 0x6D, 0xAF, 0x2E, 0x77, 0x29, 0x56, 0xD5, 0x1C, 0x29, 
	0x3B, 0xBD, 0xC3, 0x00, 0xE9, 0x42, 0xB5, 0xEB, 0xCA, 0x16, 0xBD, 0x31, 0x1E, 0xC9, 0xB4, 0x71, 
	0xF1, 0xDF, 0x06, 0x3D, 0x6A, 0xBF, 0xA1, 0x9D, 0x87, 0xF5, 0xC5, 0xBC, 0x5B, 0x04, 0xED, 0x1D, 
	0x74, 0x8C, 0x47, 0x33, 0xF6, 0x31, 0x52, 0xE8, 0x57, 0x7F, 0x1B, 0xD8, 0xF6, 0x86, 0x8A, 0x8A, 
	0xB4, 0x02, 0x65, 0x61, 0x4A, 0xFD, 0xB8, 0xE2, 0x91, 0x03, 0xA3, 0x2B, 0x98, 0xED, 0x41, 0xFE, 
	0xA0, 0xAE, 0xB2, 0x87, 0xAA, 0xB4, 0x1F, 0x69, 0x6B, 0xC6, 0x34, 0x55, 0xED, 0x06, 0x71, 0xA2, 
	0xCF, 0x42, 0x84, 0x6E, 0x9F, 0x4D, 0x86, 0xA2, 0xEC, 0x80, 0xA3, 0xEC, 0xC6, 0x8A, 0x47, 0x56, 
	0xF1, 0xCB, 0xCB, 0x5F, 0xEE, 0xC5, 0x2F, 0xBE, 0x14, 0x8B, 0xFE, 0x31, 0xEF, 0x55, 0xEF, 0x51, 
	0x4B, 0x01, 0xE5, 0xB9, 0xAB, 0x39, 0x24, 0x80, 0x4A, 0x3E, 0xC5, 0x54, 0xD1, 0xAE, 0x7E, 0xD7, 
	0x7A, 0x99, 0x59, 0x64, 0x8D, 0xB1, 0xFD, 0xDB, 0x08, 0x38, 0x48, 0x1E, 0xFB, 0x74, 0x8A, 0xE7, 
	0xAB, 0x90, 0x1E, 0x9C, 0x92, 0x1B, 0x41, 0xC0, 0x67, 0x54, 0x14, 0x0B, 0xB1, 0x03, 0xBC, 0xB6
};

static unsigned char pE_CA[256] =
{
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x84, 0x9A, 0xB1, 0xC9, 0xE2, 0xFC, 0x17, 0x33, 0x50, 0x6E, 0x8D, 0xAD, 
	0xBE, 0xD0, 0xE3, 0xF7, 0x0C, 0x22, 0x39, 0x51, 0x6A, 0x84, 0x9F, 0xBB, 0xD8, 0xF6, 0x15, 0x35, 
	0x00, 0x00, 0x00, 0x00, 0x94, 0xAA, 0xC1, 0xD9, 0xF2, 0x0C, 0x27, 0x43, 0x60, 0x7E, 0x9D, 0xBD, 
	0xCE, 0xE0, 0xF3, 0x07, 0x1C, 0x32, 0x49, 0x61, 0x7A, 0x94, 0xAF, 0xCB, 0xE8, 0x06, 0x25, 0x45, 
	0x00, 0x00, 0x00, 0x00, 0xA4, 0xBA, 0xD1, 0xE9, 0x02, 0x1C, 0x37, 0x53, 0x70, 0x8E, 0xAD, 0xCD, 
	0xDE, 0xF0, 0x03, 0x17, 0x2C, 0x42, 0x59, 0x71, 0x8A, 0xA4, 0xBF, 0xDB, 0xF8, 0x16, 0x35, 0x55, 
	0x42, 0x54, 0x67, 0x7B, 0x90, 0xA6, 0xBD, 0xD5, 0xEE, 0x08, 0x23, 0x3F, 0x5C, 0x7A, 0x99, 0xB9, 
	0xCA, 0xDC, 0xEF, 0x03, 0x18, 0x2E, 0x45, 0x5D, 0x76, 0x90, 0xAB, 0xC7, 0xE4, 0x02, 0x21, 0x41, 
	0x52, 0x64, 0x77, 0x8B, 0xA0, 0xB6, 0xCD, 0xE5, 0xFE, 0x18, 0x33, 0x4F, 0x6C, 0x8A, 0xA9, 0xC9, 
	0xDA, 0xEC, 0xFF, 0x13, 0x28, 0x3E, 0x55, 0x6D, 0x86, 0xA0, 0xBB, 0xD7, 0xF4, 0x12, 0x31, 0x51, 
	0x62, 0x74, 0x87, 0x9B, 0xB0, 0xC6, 0xDD, 0xF5, 0x0E, 0x28, 0x43, 0x5F, 0x7C, 0x9A, 0xB9, 0xD9, 
	0xEA, 0xFC, 0x0F, 0x23, 0x38, 0x4E, 0x65, 0x7D, 0x96, 0xB0, 0xCB, 0xE7, 0x04, 0x22, 0x41, 0x61, 
	0x72, 0x84, 0x97, 0xAB, 0xC0, 0xD6, 0xED, 0x05, 0x1E, 0x38, 0x53, 0x6F, 0x8C, 0xAA, 0xC9, 0xE9, 
	0xFA, 0x0C, 0x1F, 0x33, 0x48, 0x5E, 0x75, 0x8D, 0xA6, 0xC0, 0xDB, 0xF7, 0x14, 0x32, 0x51, 0x71
};


static unsigned char pD_CA[256] =
{
	0x13, 0x5B, 0x7F, 0x92, 0xCC, 0xAA, 0x0D, 0x5E, 0x32, 0xF7, 0xEE, 0xBF, 0x3B, 0x3F, 0x35, 0x4E, 
	0x3B, 0xC5, 0xEC, 0x6F, 0xDD, 0x8A, 0x9A, 0xC7, 0xC2, 0xBC, 0x2D, 0xB8, 0x5B, 0x7D, 0x98, 0xA7, 
	0xE1, 0xE7, 0x61, 0x9F, 0x0B, 0xDC, 0x16, 0x89, 0x8F, 0xDB, 0x80, 0xE4, 0xA4, 0xFA, 0x0A, 0x38, 
	0x71, 0x7E, 0x86, 0x4E, 0x54, 0x88, 0x42, 0x33, 0x5A, 0xE1, 0x61, 0x2B, 0x92, 0x9A, 0x3C, 0x18, 
	0x01, 0x16, 0x8D, 0x72, 0x5A, 0xA5, 0x04, 0x11, 0x0E, 0x1C, 0x5E, 0x6D, 0xD0, 0x2F, 0xE3, 0x7F, 
	0x94, 0x90, 0x72, 0x8B, 0x10, 0xFB, 0x0A, 0xCA, 0x9E, 0x2C, 0x9B, 0x18, 0x2D, 0x91, 0xBF, 0x73, 
	0xF4, 0x46, 0xDD, 0x3A, 0xC0, 0x03, 0x97, 0x79, 0x88, 0xB5, 0x39, 0xFA, 0xB6, 0x27, 0x0B, 0xDB, 
	0x30, 0xF2, 0x00, 0x42, 0xA9, 0xB0, 0x3F, 0x00, 0xF1, 0x6C, 0x93, 0xB8, 0x19, 0x29, 0xCC, 0xF1, 
	0xF6, 0xB2, 0x2F, 0x22, 0xA4, 0x76, 0xE1, 0x9A, 0x3A, 0xAA, 0x67, 0xE5, 0xF9, 0x59, 0x5C, 0x5C, 
	0x78, 0xAC, 0x98, 0xEB, 0x86, 0x53, 0x7B, 0xEC, 0x0B, 0xAD, 0x6C, 0x72, 0x65, 0x9E, 0xD6, 0xFE, 
	0x15, 0x1F, 0x77, 0x5A, 0x1C, 0x23, 0x15, 0x46, 0xF2, 0x2E, 0x23, 0x8E, 0xF3, 0x59, 0x4B, 0x6C, 
	0x8A, 0x2C, 0x58, 0xF4, 0x14, 0x89, 0x59, 0x6C, 0x48, 0xAB, 0x17, 0xF3, 0xD9, 0xB1, 0x2F, 0xE4, 
	0xA0, 0x32, 0xDD, 0x3F, 0xF4, 0x83, 0xCA, 0x7E, 0xB8, 0x5C, 0x54, 0x21, 0x4A, 0x39, 0x4A, 0xE1, 
	0xDC, 0x00, 0xEE, 0x7B, 0x72, 0x26, 0x18, 0x00, 0x87, 0x29, 0x2E, 0xE3, 0xE0, 0xC9, 0xA9, 0x8F, 
	0xFC, 0x10, 0x91, 0xED, 0x08, 0x21, 0xA9, 0x92, 0xB0, 0x7A, 0x85, 0x69, 0xA7, 0xF8, 0x06, 0x45, 
	0x1D, 0x0B, 0xBF, 0x12, 0xB7, 0x67, 0x2B, 0x80, 0x9A, 0x8D, 0x0D, 0xB2, 0x20, 0xAD, 0xD2, 0x79
};





static unsigned char pN_Terminal[256] =
{
	0x63, 0x6E, 0x93, 0xBF, 0xF6, 0xC3, 0xDB, 0xEF, 0xC5, 0x10, 0x01, 0x56, 0xD9, 0x92, 0x07, 0xC5, 
	0x33, 0x51, 0xE9, 0xA3, 0xC6, 0x9E, 0xBD, 0x49, 0x6B, 0xAB, 0x88, 0x16, 0x30, 0x71, 0x60, 0xBE, 
	0xBD, 0x17, 0x17, 0xE0, 0x6C, 0x9E, 0x94, 0x37, 0xF0, 0x98, 0x16, 0xFC, 0x9D, 0xAF, 0xBE, 0x04, 
	0x10, 0xB2, 0x0A, 0xDF, 0x0A, 0x0B, 0x83, 0x84, 0xD0, 0x07, 0x5E, 0x99, 0x2B, 0x00, 0xD2, 0x43, 
	0x5B, 0xFE, 0x6E, 0x18, 0xDD, 0xAE, 0xF5, 0x0E, 0x04, 0x31, 0xD4, 0x8E, 0x37, 0x58, 0x1E, 0x05, 
	0x21, 0xD0, 0x90, 0x2A, 0x8E, 0xEC, 0xF2, 0xFC, 0x33, 0x0C, 0x67, 0x56, 0x0F, 0x48, 0x67, 0x91, 
	0xA8, 0x12, 0xF0, 0x97, 0x36, 0x53, 0x79, 0x64, 0x70, 0xE7, 0xD5, 0x08, 0x42, 0xAF, 0x43, 0xC4, 
	0x08, 0x8F, 0x6A, 0x0A, 0xC6, 0x83, 0x64, 0xE8, 0x71, 0xD3, 0xC8, 0x2C, 0x72, 0x6E, 0x13, 0x1E, 
	0xC7, 0x1F, 0x3D, 0x2A, 0x3D, 0xD8, 0xB4, 0x00, 0xB3, 0x12, 0x88, 0x45, 0xA6, 0x0C, 0x6C, 0x8E, 
	0xAB, 0xD0, 0x49, 0xE8, 0x6D, 0xC5, 0x7A, 0xB3, 0x4A, 0x5A, 0x46, 0x84, 0x96, 0xEB, 0x31, 0xD3, 
	0xE7, 0x70, 0x8E, 0x15, 0x2B, 0x6D, 0x85, 0x64, 0x63, 0x17, 0xBA, 0xB6, 0xD3, 0x85, 0xEF, 0xF6, 
	0xED, 0x58, 0x4F, 0xC5, 0xF4, 0xBB, 0x93, 0x28, 0x74, 0x85, 0x36, 0x01, 0x77, 0xDF, 0x40, 0xCF, 
	0xF3, 0x6F, 0x49, 0xE6, 0x30, 0x45, 0xFF, 0xB4, 0x78, 0x2F, 0xD3, 0xB1, 0xFA, 0x68, 0x13, 0x8D, 
	0x40, 0xF5, 0x7A, 0x1D, 0xFB, 0x32, 0x43, 0xB0, 0x7E, 0xFC, 0xD6, 0x6F, 0x12, 0x72, 0xD2, 0x0C, 
	0x32, 0x98, 0x5E, 0x23, 0x18, 0x52, 0xEB, 0xB1, 0x1C, 0x13, 0x7C, 0x3B, 0x28, 0x5B, 0xC0, 0x19, 
	0xDA, 0xFF, 0x03, 0x4D, 0xCD, 0x02, 0x26, 0xA0, 0x69, 0x0F, 0x8E, 0xBB, 0x68, 0x43, 0x7D, 0xA9
};

static unsigned char pE_Terminal[256] =
{
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x88, 0x9E, 0xB5, 0xCD, 0xE6, 0x00, 0x1B, 0x37, 0x54, 0x72, 0x91, 0xB1, 
	0xC2, 0xD4, 0xE7, 0xFB, 0x10, 0x26, 0x3D, 0x55, 0x6E, 0x88, 0xA3, 0xBF, 0xDC, 0xFA, 0x19, 0x39, 
	0x00, 0x00, 0x00, 0x00, 0x98, 0xAE, 0xC5, 0xDD, 0xF6, 0x10, 0x2B, 0x47, 0x64, 0x82, 0xA1, 0xC1, 
	0xD2, 0xE4, 0xF7, 0x0B, 0x20, 0x36, 0x4D, 0x65, 0x7E, 0x98, 0xB3, 0xCF, 0xEC, 0x0A, 0x29, 0x49, 
	0x00, 0x00, 0x00, 0x00, 0xA8, 0xBE, 0xD5, 0xED, 0x06, 0x20, 0x3B, 0x57, 0x74, 0x92, 0xB1, 0xD1, 
	0xE2, 0xF4, 0x07, 0x1B, 0x30, 0x46, 0x5D, 0x75, 0x8E, 0xA8, 0xC3, 0xDF, 0xFC, 0x1A, 0x39, 0x59, 
	0x82, 0x94, 0xA7, 0xBB, 0xD0, 0xE6, 0xFD, 0x15, 0x2E, 0x48, 0x63, 0x7F, 0x9C, 0xBA, 0xD9, 0xF9, 
	0x0A, 0x1C, 0x2F, 0x43, 0x58, 0x6E, 0x85, 0x9D, 0xB6, 0xD0, 0xEB, 0x07, 0x24, 0x42, 0x61, 0x81, 
	0x92, 0xA4, 0xB7, 0xCB, 0xE0, 0xF6, 0x0D, 0x25, 0x3E, 0x58, 0x73, 0x8F, 0xAC, 0xCA, 0xE9, 0x09, 
	0x1A, 0x2C, 0x3F, 0x53, 0x68, 0x7E, 0x95, 0xAD, 0xC6, 0xE0, 0xFB, 0x17, 0x34, 0x52, 0x71, 0x91, 
	0xA2, 0xB4, 0xC7, 0xDB, 0xF0, 0x06, 0x1D, 0x35, 0x4E, 0x68, 0x83, 0x9F, 0xBC, 0xDA, 0xF9, 0x19, 
	0x2A, 0x3C, 0x4F, 0x63, 0x78, 0x8E, 0xA5, 0xBD, 0xD6, 0xF0, 0x0B, 0x27, 0x44, 0x62, 0x81, 0xA1, 
	0xB2, 0xC4, 0xD7, 0xEB, 0x00, 0x16, 0x2D, 0x45, 0x5E, 0x78, 0x93, 0xAF, 0xCC, 0xEA, 0x09, 0x29, 
	0x3A, 0x4C, 0x5F, 0x73, 0x88, 0x9E, 0xB5, 0xCD, 0xE6, 0x00, 0x1B, 0x37, 0x54, 0x72, 0x91, 0xB1
};


static unsigned char pD_Terminal[256] =
{
	0x4B, 0x16, 0x52, 0x61, 0x42, 0x72, 0xDE, 0x59, 0xFC, 0xF1, 0xAA, 0x84, 0xED, 0x5B, 0x01, 0x35, 
	0x20, 0xDC, 0xA5, 0xCE, 0x57, 0xEC, 0xBB, 0xDD, 0xB7, 0x51, 0x20, 0xD8, 0xB4, 0x00, 0x0F, 0xAA, 
	0xAC, 0x86, 0xD7, 0x92, 0xF1, 0xB6, 0x1F, 0xAC, 0xA2, 0x52, 0x4E, 0x7C, 0x03, 0xD4, 0x48, 0xFE, 
	0xDD, 0xF1, 0x70, 0x93, 0x7E, 0x4D, 0x46, 0xBF, 0xAD, 0xC2, 0x77, 0xD9, 0x4D, 0x6B, 0x27, 0xA1, 
	0x4C, 0x5A, 0x8F, 0xCB, 0x56, 0x27, 0x9E, 0x12, 0xF0, 0xCD, 0xAF, 0xC5, 0xE4, 0x48, 0xDE, 0xFE, 
	0x87, 0x8F, 0x6D, 0x31, 0x65, 0xE5, 0xE1, 0x93, 0xE6, 0xBE, 0x9C, 0xBC, 0x6B, 0x0F, 0xF8, 0x2B, 
	0x5B, 0xE2, 0x91, 0x00, 0x00, 0x56, 0xE5, 0x84, 0x1B, 0x6F, 0x6F, 0x0B, 0xF4, 0xD7, 0x8D, 0xF1, 
	0x4A, 0xD0, 0xB4, 0xF9, 0x52, 0x48, 0xD9, 0x8C, 0x56, 0x90, 0xA3, 0x47, 0xD6, 0xAA, 0xDF, 0xFD, 
	0xD8, 0xBF, 0x28, 0x1C, 0x7E, 0xE5, 0xCD, 0x55, 0x77, 0x0C, 0xB0, 0x83, 0x19, 0xB3, 0xF2, 0x5E, 
	0x72, 0xE0, 0xDB, 0x9A, 0x9E, 0x83, 0xFC, 0xCC, 0x31, 0x3C, 0x84, 0xAD, 0xB9, 0x47, 0x21, 0xE2, 
	0xEF, 0xF5, 0x5E, 0x0E, 0x72, 0xF3, 0x58, 0x98, 0x97, 0x0F, 0x7C, 0x24, 0x8D, 0xAE, 0x9F, 0xA4, 
	0x9E, 0x90, 0xDF, 0xD8, 0x4D, 0x7D, 0x62, 0x70, 0x4D, 0xAE, 0x79, 0xAB, 0xA4, 0x3F, 0x2B, 0x8A, 
	0xA2, 0x4A, 0x86, 0x99, 0x20, 0x2E, 0xAA, 0x78, 0x50, 0xCA, 0x8C, 0x76, 0xFC, 0x45, 0x62, 0xB3, 
	0xD5, 0xF8, 0x51, 0xBE, 0xFC, 0x21, 0x82, 0x75, 0x54, 0xA8, 0xE4, 0x9F, 0x61, 0xA1, 0xE1, 0x5D, 
	0x21, 0x10, 0x3F, 0xC2, 0xBA, 0x36, 0xF2, 0xCB, 0xBD, 0x0C, 0xA8, 0x27, 0x70, 0xE7, 0x2A, 0x11, 
	0x3C, 0x55, 0xAD, 0x88, 0x33, 0x57, 0x19, 0xC0, 0x9B, 0x5F, 0x09, 0x7D, 0xF0, 0x2C, 0xFE, 0x70
};




static unsigned char pN_PC[256] =
{
	0xD5, 0x43, 0x2B, 0x2A, 0xA4, 0xD7, 0xF1, 0x96, 0x1D, 0xD9, 0x93, 0x8E, 0x2E, 0x29, 0x54, 0xF0, 
	0xF0, 0x66, 0x7A, 0xA7, 0x2E, 0x7E, 0x17, 0x01, 0x1A, 0x4F, 0xF2, 0x34, 0x68, 0x0E, 0x78, 0xE2, 
	0xD4, 0xC5, 0xFE, 0x49, 0xFE, 0xC8, 0x43, 0xF5, 0xAD, 0xF4, 0xBC, 0x9B, 0x0A, 0x81, 0x20, 0xA0, 
	0x62, 0x1D, 0x75, 0x47, 0xC4, 0x18, 0x2E, 0x45, 0x3A, 0xEF, 0xBC, 0x05, 0xF3, 0xF4, 0xF5, 0x79, 
	0xFC, 0xFE, 0x68, 0x31, 0x51, 0x83, 0x0B, 0x8E, 0x8F, 0xD4, 0xED, 0xAA, 0x59, 0xB6, 0xB6, 0xAB, 
	0x7C, 0x33, 0xC6, 0x55, 0xE3, 0xFF, 0x27, 0x31, 0x31, 0xB0, 0x06, 0x42, 0xC7, 0x33, 0xE9, 0x2D, 
	0xE9, 0xC5, 0x6D, 0xC9, 0x17, 0xB1, 0xFD, 0xAA, 0x5D, 0xE5, 0xC8, 0xCE, 0x90, 0xC4, 0x47, 0x58, 
	0xAC, 0x58, 0x88, 0x21, 0xC4, 0x22, 0x4A, 0x40, 0x76, 0x89, 0x73, 0x73, 0xCA, 0x70, 0x2E, 0x9D, 
	0x85, 0xF2, 0x46, 0xA0, 0x61, 0x33, 0x13, 0x3A, 0x42, 0x39, 0x7C, 0x79, 0x05, 0x97, 0x16, 0xD4, 
	0x81, 0x25, 0x54, 0x96, 0xAE, 0xA4, 0x44, 0xEC, 0x32, 0xEF, 0x5E, 0xA4, 0xC2, 0x99, 0x29, 0xA8, 
	0xCD, 0x47, 0x18, 0x75, 0x7C, 0x46, 0xEE, 0xBA, 0xE2, 0x37, 0xB9, 0x9E, 0x44, 0x6D, 0xFB, 0x24, 
	0x25, 0x2E, 0x1C, 0xC6, 0x01, 0x45, 0x15, 0x07, 0x82, 0x59, 0x84, 0xA9, 0x9D, 0x1D, 0xEA, 0xB3, 
	0x14, 0x46, 0x36, 0x21, 0xEF, 0xF5, 0x84, 0xE1, 0xE6, 0x2E, 0xA3, 0x64, 0x46, 0x59, 0xFB, 0x71, 
	0x01, 0xB4, 0xA0, 0x64, 0x1F, 0x1C, 0xA7, 0x2D, 0x6D, 0xE0, 0xAA, 0x92, 0xB9, 0x91, 0xA2, 0xDC, 
	0x03, 0xB2, 0x40, 0xD6, 0xD8, 0x61, 0xE8, 0x27, 0x1E, 0xEE, 0xEE, 0x60, 0x1F, 0x00, 0x21, 0xDE, 
	0x33, 0x9C, 0x08, 0xF9, 0x55, 0x82, 0xB4, 0x94, 0xF3, 0xA1, 0x3C, 0xFF, 0x05, 0x3D, 0xB2, 0xAC
};

static unsigned char pE_PC[256] =
{
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xD0, 0xE6, 0xFD, 0x15, 0x2E, 0x48, 0x63, 0x7F, 0x9C, 0xBA, 0xD9, 0xF9, 
	0x0A, 0x1C, 0x2F, 0x43, 0x58, 0x6E, 0x85, 0x9D, 0xB6, 0xD0, 0xEB, 0x07, 0x24, 0x42, 0x61, 0x81, 
	0x00, 0x00, 0x00, 0x00, 0xE0, 0xF6, 0x0D, 0x25, 0x3E, 0x58, 0x73, 0x8F, 0xAC, 0xCA, 0xE9, 0x09, 
	0x1A, 0x2C, 0x3F, 0x53, 0x68, 0x7E, 0x95, 0xAD, 0xC6, 0xE0, 0xFB, 0x17, 0x34, 0x52, 0x71, 0x91, 
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x06, 0x1D, 0x35, 0x4E, 0x68, 0x83, 0x9F, 0xBC, 0xDA, 0xF9, 0x19, 
	0x2A, 0x3C, 0x4F, 0x63, 0x78, 0x8E, 0xA5, 0xBD, 0xD6, 0xF0, 0x0B, 0x27, 0x44, 0x62, 0x81, 0xA1, 
	0x8E, 0xA0, 0xB3, 0xC7, 0xDC, 0xF2, 0x09, 0x21, 0x3A, 0x54, 0x6F, 0x8B, 0xA8, 0xC6, 0xE5, 0x05, 
	0x16, 0x28, 0x3B, 0x4F, 0x64, 0x7A, 0x91, 0xA9, 0xC2, 0xDC, 0xF7, 0x13, 0x30, 0x4E, 0x6D, 0x8D, 
	0x9E, 0xB0, 0xC3, 0xD7, 0xEC, 0x02, 0x19, 0x31, 0x4A, 0x64, 0x7F, 0x9B, 0xB8, 0xD6, 0xF5, 0x15, 
	0x26, 0x38, 0x4B, 0x5F, 0x74, 0x8A, 0xA1, 0xB9, 0xD2, 0xEC, 0x07, 0x23, 0x40, 0x5E, 0x7D, 0x9D, 
	0xAE, 0xC0, 0xD3, 0xE7, 0xFC, 0x12, 0x29, 0x41, 0x5A, 0x74, 0x8F, 0xAB, 0xC8, 0xE6, 0x05, 0x25, 
	0x36, 0x48, 0x5B, 0x6F, 0x84, 0x9A, 0xB1, 0xC9, 0xE2, 0xFC, 0x17, 0x33, 0x50, 0x6E, 0x8D, 0xAD, 
	0xBE, 0xD0, 0xE3, 0xF7, 0x0C, 0x22, 0x39, 0x51, 0x6A, 0x84, 0x9F, 0xBB, 0xD8, 0xF6, 0x15, 0x35, 
	0x46, 0x58, 0x6B, 0x7F, 0x94, 0xAA, 0xC1, 0xD9, 0xF2, 0x0C, 0x27, 0x43, 0x60, 0x7E, 0x9D, 0xBD
};


static unsigned char pD_PC[256] =
{
	0xB3, 0x9E, 0xA9, 0xEE, 0xBE, 0xD8, 0x0F, 0x7F, 0x78, 0xD6, 0x5E, 0x71, 0x82, 0x06, 0xA9, 0xE7, 
	0x75, 0xC1, 0x45, 0x39, 0xA4, 0x0B, 0x00, 0x9E, 0xF9, 0x5E, 0x9B, 0x40, 0x6E, 0x55, 0xC6, 0xAB, 
	0xCC, 0xFC, 0xFE, 0x47, 0x77, 0x96, 0x50, 0xF7, 0x2F, 0xAE, 0xBF, 0xBE, 0x06, 0x7F, 0x23, 0xAA, 
	0xDC, 0xA0, 0x08, 0xA5, 0xE9, 0xD1, 0x03, 0x80, 0x6D, 0x9E, 0xE2, 0x28, 0x70, 0xFE, 0x93, 0xCC, 
	0x6F, 0x6D, 0x16, 0x2F, 0x96, 0xB4, 0xCB, 0x97, 0x75, 0x3B, 0x40, 0x33, 0x01, 0x72, 0xFD, 0x24, 
	0x98, 0xFD, 0x01, 0x20, 0x12, 0x55, 0xD4, 0x53, 0xAD, 0x57, 0xBF, 0xAB, 0x22, 0xA9, 0x6D, 0x5E, 
	0x5A, 0x54, 0x10, 0x52, 0x6E, 0x7A, 0x33, 0xF8, 0xD1, 0xD0, 0x56, 0xF9, 0x10, 0xAB, 0xFF, 0x02, 
	0x61, 0xEE, 0x86, 0xE0, 0xD6, 0x53, 0x59, 0x63, 0xF1, 0x41, 0x45, 0xA8, 0xE8, 0x28, 0x10, 0xA5, 
	0x02, 0xF7, 0xD9, 0x6A, 0x96, 0x77, 0xB7, 0x26, 0x2C, 0x26, 0xA8, 0xFB, 0x58, 0x0F, 0x0F, 0x38, 
	0x01, 0x19, 0x38, 0x64, 0x74, 0x18, 0x83, 0x9D, 0xCC, 0xF4, 0xE9, 0xC2, 0x81, 0x66, 0xC6, 0x1A, 
	0x89, 0xDA, 0xBA, 0xF8, 0x52, 0x84, 0x49, 0x27, 0x97, 0x7A, 0x7B, 0x14, 0x83, 0xF3, 0xFC, 0x6D, 
	0xC3, 0x1E, 0x68, 0xD9, 0xAB, 0xD8, 0xB8, 0x04, 0xAC, 0x3B, 0x58, 0xC6, 0x13, 0x69, 0xF1, 0x77, 
	0xB8, 0x2E, 0x24, 0x16, 0x4A, 0xF9, 0xAD, 0xEB, 0x99, 0x74, 0x17, 0x43, 0x84, 0x3B, 0x52, 0xA1, 
	0xAB, 0x22, 0x6B, 0x98, 0xBF, 0x12, 0x1A, 0xC9, 0x48, 0x40, 0xC7, 0x61, 0x26, 0x61, 0x6C, 0xE8, 
	0x57, 0x21, 0x2B, 0xE4, 0xE5, 0xEB, 0x9A, 0x1A, 0x14, 0xF4, 0x49, 0xEB, 0x14, 0x00, 0x16, 0x94, 
	0x22, 0x68, 0xB0, 0x50, 0x39, 0xAC, 0xCD, 0x0D, 0x4D, 0xC1, 0x7D, 0xFF, 0x03, 0x7E, 0x21, 0x73
};


