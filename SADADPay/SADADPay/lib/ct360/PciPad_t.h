// PciPad.h : PciPad DLL의 기본 헤더 파일입니다.
//
#pragma once
#include "PciPadDefine.h"

#define USE_INTERNAL_API // 2014-02-11 erulil 외부로 노출될 dll을 빌드할 때에는 이 define을 삭제 해야 한다
#define USE_PC_API // 2014-02-11 erulil PC와 통신 시 사용되는 API를 사용할 때 이 define을 포함 한다

#ifdef BBSECURE_EXPORT
#define BBSECURE			__declspec(dllexport)
#ifdef USE_INTERNAL_API
#define BBSECURE_INTERNAL	__declspec(dllexport) // 2014-02-11 erulil 외부에 노출되지 않고 내부에서 사용할 API에 선언
#endif
#ifdef USE_PC_API
#define BBSECURE_USE_PC		__declspec(dllexport) // 2014-02-11 erulil 외부에 노출되지 않고 내부에서 사용할 API에 선언
#endif
#else
#define BBSECURE			__declspec(dllimport)
#ifdef USE_INTERNAL_API
#define BBSECURE_INTERNAL	__declspec(dllimport)
#endif
#ifdef USE_PC_API
#define BBSECURE_USE_PC		__declspec(dllimport) // 2014-02-11 erulil 외부에 노출되지 않고 내부에서 사용할 API에 선언
#endif
#endif


extern "C++"
{
	//--------------------------------------------------------------------------------------------------------------------------------
	// maxq 용 api ----------------------------------------------------------------------------------------------- start
	//--------------------------------------------------------------------------------------------------------------------------------
	BBSECURE DWORD BBSecure_Deinit();
	BBSECURE DWORD BBSecure_Init(HWND hApp = NULL, DWORD dwMsgValue = 0, int nTerminalUIType = TERMINAL_UI_TYPE_2, TCHAR* strPort = L"COM6:", DWORD dwBaudrate = 115200, 
							BYTE byByteSize = 8, BYTE byStopBits = 0, BYTE byParity = 0, DWORD dwSoftFlow = 0, DWORD dwHardFlow = 0);	
	BBSECURE DWORD BBSecure_ConnectionCheck(int *pConnectionMode, DWORD *pRepairCode = NULL);
	BBSECURE DWORD BBSecure_CheckErrorStatus(DWORD *pErrorCode);

	// 2014-03-28 erulil pin entry dlg GUI를 변경할 수 있는 함수 추가
	BBSECURE DWORD BBSecure_SetEntryPINSceneGUI(LPCTSTR pFreeText, UINT nFreeTextSize,
												UINT nTextFontSize, LPCTSTR pTextFontType, int nTextFontTypeSize, COLORREF clrTextFont,
												UINT nPinFontSize, LPCTSTR pPinFontType, int nPinFontTypeSize, COLORREF clrPinFont,
												COLORREF clrBackground = RGB(0, 0, 0), int nDlgStartY = -1, int nDlgHeight = -1,
												int nTextStartPositionX = -1, int nTextStartPositionY = -1, int nLineSpacing = 10);

	BBSECURE DWORD BBSecure_GetSerialNumber(LPBYTE lpBuffer, int nBufferSize);


	// 2014-02-19 erulil lBankID에 해당하는 key들을 전부 삭제
	BBSECURE DWORD BBSecure_ClearKeys(DWORD dwBankID);
	// 2014-03-17 erulil 특정 키 하나만 삭제
	BBSECURE DWORD BBSecure_EraseKey(DWORD dwBankID, DWORD dwKeyType, DWORD dwKeyIndex);

	// 2014-03-17 erulil KSN 생성
	BBSECURE DWORD BBSecure_GenerateKSN(BYTE *pKSN); // pKSN = 10B	
	// 2014-03-17 erulil 특정 키 KCV 반환
	BBSECURE DWORD BBSecure_GetKeyCheckValue(BYTE *pKCV, DWORD dwBankID, DWORD dwKeyType, DWORD dwKeyIndex, DWORD dwKCVMode = 0x00); // pKCV = 3B


	// 2014-02-19 erulil send certificate file data. pFileData : buffer pointer, nFileDataLength : file data length,
	// pFileData에 인증서 데이터를 보내면 단말기 안의 특정 경로 안에 저장한다. 단말기의 경로 및 파일은 사용자가 알기 힘들어야 한다.
	// 지정 경로 : \\Windows\\PAC.DLL
	BBSECURE DWORD BBSecure_WriteCertificate(LPBYTE pCertificateData, UINT nCertificateDataLength);

	// 2014-02-19 erulil send certificate file data. pFileData : buffer pointer, pFileDataLength : [IN] pFileData buffer length, [OUT] write file data length
	// 지정된 경로에서 인증서 데이터를 읽어와서 반환한다.
	// 지정 경로 : \\Windows\\PAC.DLL
	BBSECURE DWORD BBSecure_ReadCertificate(LPBYTE pCertificateData, UINT *pCertificateDataLength);



	// key injection ----------------------------------------------------------------------------------------------- start
	// ----------------------------------------------- for verisoft start
	// 2014-04-09 erulil 추가 plain key data를 받아서 key injection
	//						  nWriteMode : write_directly, write_encrypt with master key, write_decrypt with master key / pKCV : 3Bytes / nMode : ecb or cbc
	//						  nMasterKeyIndex : write_encrypt or write_decrypt 일 때에만 유효. master key는 injection 되어 있어야 함
	//						  nOverwriteMode : 0 - overwrite, 1 - XOR
	BBSECURE DWORD BBSecure_SendPaymentKey_simple(LPBYTE pKCV, LPBYTE pKeyData, UINT nKeyIndex, UINT nKeyType, UINT nKeyLengthOfByte, DWORD dwBankID, 
												UINT nMode, UINT nWriteMode, UINT nMasterKeyIndex, BOOL bXORWrite = FALSE, UINT nXORBaseKeyIndex = 0, LPBYTE pIV = NULL);
	// 2014-02-18 erulil Garanti key injection API 추가. pEncryptedKey, pPattern, pMask의 배열 크기는 256이어야 한다(각 256B로 구성) 
	//													 = pEncryptedKey, pPattern, pMask의 버퍼의 길이는 nEncryptedKeyLength와 동일해야 한다(=256)
	BBSECURE DWORD BBSecure_SendPaymentKey_Garanti(int nKeyType, DWORD dwBankID, UINT nKeyIndex, UINT nKeyLength, UINT nMode, 
												UINT nKeyLocation, UINT nKSNLocation, BOOL bUsingMaskAndPattern,
												LPBYTE pEncryptedKey, UINT nEncryptedKeyLength, LPBYTE pPattern, LPBYTE pMask,
												BYTE *pOutput, int nOutputSize);
	// 2014-04-14 erulil 추가. 단말기에 이미 injection 해 둔 RSA 공개 키로 생성한 키를 암호화한다
	BBSECURE DWORD BBSecure_GenerateKey_useSavedKey(LPBYTE pEncryptedKey, UINT *pKeyLength, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, 
													UINT nKeyLength, UINT nKeyType, DWORD dwBankID, UINT nMasterKeyIndex);
	BBSECURE DWORD BBSecure_GenerateKey_useSavedKeyforV(LPBYTE pEncryptedKey, UINT *pKeyLength, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, 
													UINT nKeyLength, UINT nKeyType, DWORD dwBankID, UINT nMasterKeyIndex, LPBYTE pClearKeyData = NULL);
	// 2014-04-14 erulil 추가. RSA 공개키를 단말기에 저장한다. nKeyIndex : 0 ~ 7 
	BBSECURE DWORD BBSecure_SendRSAPublicKey(LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent, UINT nKeyIndex);
#if 1
	// 2014-06-09 erulil 추가
	BBSECURE DWORD BBSecure_GenerateKey(LPBYTE pEncryptedKey, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, UINT nKeyLength, UINT nKeyType, DWORD dwBankID, LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent);
#else
	// 2014-04-07 erulil 추가. export key func. [out] -> pEncryptedKey : buffer size must be 256B, pKSN : 10B, pKCV : 3B. 입력받은 공개키로 생성한 키를 암호화한다
	BBSECURE DWORD BBSecure_GenerateKey(LPBYTE pEncryptedKey, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, UINT nKeyLength, UINT nKeyType, DWORD dwBankID, LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent);
#endif
	// ----------------------------------------------- for verisoft end


	// 2014-02-11 erulil 추가 Remote key injection용 API
	BBSECURE DWORD BBSecure_SendKeyEndPacket();
	BBSECURE DWORD BBSecure_SendPaymentKey_symmetric(BYTE* pKeyPacket, int nKeyPacketLength, BYTE *pOutput, int nOutputSize);
	BBSECURE DWORD BBSecure_SendPaymentKey_asymmetric(BYTE *pEncryptedKeyData, int nEncryptedKeyDataLength, 
													BYTE *pRandomNumber, int nRandomNumberLength, BYTE *pOutput, int nOutputSize);


	// 2013-4-15 erulil RSA key pair를 생성해서 key pair는 maxq로 보내고, 공개키를 반환한다
	BBSECURE DWORD BBSecure_GenerateTerminalKey(LPBYTE pN, UINT *pByteLengthOfN, LPBYTE pE, UINT *pByteLengthOfE);
	BBSECURE DWORD BBSecure_SendTerminalKeyConfirm(); // 전송된 RSA키를 injection하라는 패킷을 보낸다
	// key injection ----------------------------------------------------------------------------------------------- end



	// 2013-09-25 erulil 함수 추가. lpCardNumber = 16 digit(=8B), lpBankID = 8 digit(=4B)
	BBSECURE DWORD BBSecure_GetPINBlock(LPCTSTR lpCardNumber, DWORD dwCardNumberLength, UINT nKeyType, UINT nKeyIndex, UINT nBlockFormat, 
										  //LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, DWORD dwAmount = 0);
										  LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, LPBYTE pKSN);//, LPCTSTR pAmountText = NULL, int nAmountTextSize = 0);
	// 2014-10-07 erulil 추가 : for SADAD
	BBSECURE DWORD BBSecure_GetPINBlock_withData(LPCTSTR lpPINData, DWORD dwPINLength, LPCTSTR lpCardNumber, DWORD dwCardNumberLength, UINT nKeyType, UINT nKeyIndex, UINT nBlockFormat, 
										  LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, LPBYTE pKSN);
										  
	// mac encrypt
	BBSECURE DWORD BBSecure_GetMac(LPBYTE lpMacData, DWORD dwMaclen, UINT nKeyType, UINT nKeyIndex, 
									UINT nMode, LPCTSTR lpBankID, UINT nBankIDLength, BYTE *lpOutBuffer, DWORD *pOutputLen, LPBYTE pKSN);

	// 2013-10-08 erulil 함수 추가. EMV규격으로 암호화된 PIN 블록을 반환한다
	BBSECURE DWORD BBSecure_GetPINBlockforEMV(unsigned char *pRetrunPinBlock, DWORD *pPinBlockLength, // out
											 unsigned char *pRSA_N, DWORD dwByteLengthOfRSA_N, // input
											 unsigned char *pRSA_E, DWORD dwByteLengthOfRSA_E, // input
											 unsigned char *pRandomNumber, DWORD dwByteLengthOfRandomNumber, // input
											 DWORD dwTimeOutSecond);//, LPCTSTR pAmountText = NULL, int nAmountTextSize = 0); // input
											 //DWORD dwTimeOutSecond, DWORD dwAmount = 0); // input

	BBSECURE DWORD BBSecure_EncryptData(UINT nKeyType, UINT nKeyIndex, UINT nOpMode, LPCTSTR lpBankID, UINT nBankIDLength,
										LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen, LPBYTE pKSN, LPBYTE pIV = NULL);
	BBSECURE DWORD BBSecure_EncryptData_big(UINT nKeyType, UINT nKeyIndex, UINT nOpMode, LPCTSTR lpBankID, UINT nBankIDLength,
										LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen, LPBYTE pKSN, LPBYTE pIV = NULL);

	BBSECURE DWORD BBSecure_DecryptData(UINT nKeyType, UINT nKeyIndex, UINT nOpMode, LPCTSTR lpBankID, UINT nBankIDLength,
										LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen, LPBYTE pIV = NULL);
	BBSECURE DWORD BBSecure_DecryptData_big(UINT nKeyType, UINT nKeyIndex, UINT nOpMode, LPCTSTR lpBankID, UINT nBankIDLength,
										LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen, LPBYTE pIV = NULL);

	// RSA encrypt or verify / RSA decrypt or sign. lpOutBuffer must be null.
	// -> 2014-04-03 erulil 제공되지 않는 함수
	//BBSECURE DWORD BBSecure_RSAOperation(UINT nOpType, UINT nKeyIndex, UINT nKeyID, 
	//								LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen);


	BBSECURE DWORD BBSecure_GetCurrentTime(PCI_TIME *pOutTime);
	BBSECURE DWORD BBSecure_SetCurrentTime(PCI_TIME inputTime);



	//////////////////////////////////////////////// 내부 사용 용 API
	BBSECURE_INTERNAL DWORD BBSecure_WriteDataToMaxQ(BYTE *pSendBuff, DWORD nToWrite, int interval = 10, int cut_byte = 300);
	// 2014-03-28 erulil byAckCommand 추가
	BBSECURE_INTERNAL DWORD BBSecure_SendEmptyPacket(BYTE byCommand, BYTE byAckCommand = 0);
	// erulil, SendStopPCI와 SendResumePCI는 개발시 사용되는 API이다. 고객측에 노출되면 안됨. CommPCIApp에서 탈출코드 사용시 MaxQ동작을 정지&재개 시키기 위해 사용됨
	BBSECURE_INTERNAL DWORD BBSecure_SendStopProcessor();
	BBSECURE_INTERNAL DWORD BBSecure_SendResumeProcessor();
	// 2014-02-26 erulil normal 모드에서 secure room mode 모드로 진입하기 위해 사용되는 API. maxq와의 connection이 완료된 이후에 사용 한다
	BBSECURE_INTERNAL DWORD BBSecure_StartSecureRoomMode();
	BBSECURE_INTERNAL DWORD BBSecure_SendPassword(LPBYTE lpPassword1, LPBYTE lpPassword2, int nPasswordSize);
	// 2014-09-29 erulil 추가
	BBSECURE_INTERNAL DWORD BBSecure_SendFastPassword(LPBYTE lpPassword1, LPBYTE lpPassword2, int nPasswordSize);
	//--------------------------------------------------------------------------------------------------------------------------------
	// maxq 용 api ----------------------------------------------------------------------------------------------- end
	//--------------------------------------------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------------------------------------
	// no maxq ----------------------------------------------------------------------------------------------- start
	//----------------------------------------------------------------------------------------------------------------------------
	////// 내부 사용 용 API
	BBSECURE_INTERNAL DWORD BBGet_Shuffle_Input(LPTSTR tszGetBuffer, int& nSize, int nMinSize, int nMaxSize, int nShowType);
	BOOL connection_check(); // 2014-07-14 erulil 추가


	//2013-10-17 erulil errorCode를 텍스트로 반환하는 함수 추가
	BBSECURE BOOL BBGet_ErrorText(DWORD dwErrorCode, TCHAR* pTextBuffer, DWORD dwBufferSize);
	BBSECURE unsigned short BBGet_crc16(unsigned char *pData, unsigned long lByteLength);


	// 2014-04-03 erulil 추가
	BBSECURE DWORD BBSecure_Rsa(BYTE *pOutputData, BYTE *pInputData, BYTE *pModulus, DWORD dwByteLengthOfModulus, BYTE *pExponent, DWORD dwaByteLengthOfExponent);
	BBSECURE DWORD BBSecure_GetRandomNumber(BYTE *pRandom, int nLength);
	BBSECURE DWORD BBSecure_Des(BYTE *pOutputData, BYTE *pInputData, DWORD dwByteLengthOfInputData, BYTE *pKeyData, DWORD dwByteLengthOfKeyData, 
								int nOperation, int nOperationMode, BYTE *pIV = NULL);
	//----------------------------------------------------------------------------------------------------------------------------
	// no maxq ----------------------------------------------------------------------------------------------- end
	//----------------------------------------------------------------------------------------------------------------------------




	//----------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------- 2013-12-19 erulil serial 연결(PC) 관련 API 추가 ----- start
	//----------------------------------------------------------------------------------------------------------------------------
	BBSECURE_USE_PC DWORD BBSecure_ConnectPC(HWND hApp, DWORD dwMsgValue, TCHAR* strPort = L"", DWORD dwBaudrate = 115200, 
									BYTE byByteSize = 8, BYTE byStopBits = 0, BYTE byParity = 0, DWORD dwSoftFlow = 0, DWORD dwHardFlow = 0);
	BBSECURE_USE_PC void BBSecure_ReleasePC();
	BBSECURE_USE_PC int BBGet_ModelType();
	BBSECURE_USE_PC DWORD BBSecure_WriteDataToPC(BYTE *pSendBuff, DWORD nToWrite);
	BBSECURE_USE_PC DWORD BBSecure_SendRSAPublicKeyToPC(DWORD dwErrorCode, LPBYTE pN, UINT nNSize, LPBYTE pE, UINT nESize);
	BBSECURE_USE_PC DWORD BBSecure_SendMaxQConnectionCheckAckToPC();

	// 2013-12-23 erulil remoteKeyInjection_PDA API ---------------------------------- start
	// 파일 정보 패킷을 전송한다(start, ing, end)
	BBSECURE_USE_PC DWORD BBSecure_SendFileEndToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileStartToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileDataToPC(LPBYTE pFileData, int nDataSize);
	BBSECURE_USE_PC DWORD BBSecure_SendPublicKeyRequestToPC(LPBYTE pSN, int nSNSize);
	// 단말에서 생성한 랜덤넘버를 전송한다
	BBSECURE_USE_PC DWORD BBSecure_SendRandomNumberAckToPC(LPBYTE lpByRandomNumber, UINT nRandomNumberSize);
	// 파일정보를 수신했음을 알리는 패킷을 전송한다(start, ing, end)
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvEndToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvStartToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvIngToPC();
	// 2013-12-23 erulil remoteKeyInjection_PDA API ---------------------------------- end
	//----------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------- 2013-12-19 erulil serial 연결(PC) 관련 API 추가 ----- end
	//----------------------------------------------------------------------------------------------------------------------------




	// test
	BBSECURE DWORD BBSecure_DoFlashTest(int nTestIndex); // nTestIndex : 0 ~ 6

}