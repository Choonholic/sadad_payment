// PciPad.h : PciPad DLL�� �⺻ ��� �����Դϴ�.
//
#pragma once
#include "PciPadDefine.h"

#define USE_INTERNAL_API // 2014-02-11 erulil �ܺη� ����� dll�� ������ ������ �� define�� ���� �ؾ� �Ѵ�
#define USE_PC_API // 2014-02-11 erulil PC�� ��� �� ���Ǵ� API�� ����� �� �� define�� ���� �Ѵ�

#ifdef BBSECURE_EXPORT
#define BBSECURE			__declspec(dllexport)
#ifdef USE_INTERNAL_API
#define BBSECURE_INTERNAL	__declspec(dllexport) // 2014-02-11 erulil �ܺο� ������� �ʰ� ���ο��� ����� API�� ����
#endif
#ifdef USE_PC_API
#define BBSECURE_USE_PC		__declspec(dllexport) // 2014-02-11 erulil �ܺο� ������� �ʰ� ���ο��� ����� API�� ����
#endif
#else
#define BBSECURE			__declspec(dllimport)
#ifdef USE_INTERNAL_API
#define BBSECURE_INTERNAL	__declspec(dllimport)
#endif
#ifdef USE_PC_API
#define BBSECURE_USE_PC		__declspec(dllimport) // 2014-02-11 erulil �ܺο� ������� �ʰ� ���ο��� ����� API�� ����
#endif
#endif


extern "C++"
{
	//--------------------------------------------------------------------------------------------------------------------------------
	// maxq �� api ----------------------------------------------------------------------------------------------- start
	//--------------------------------------------------------------------------------------------------------------------------------
	BBSECURE DWORD BBSecure_Deinit();
	BBSECURE DWORD BBSecure_Init(HWND hApp = NULL, DWORD dwMsgValue = 0, int nTerminalUIType = TERMINAL_UI_TYPE_2, TCHAR* strPort = L"COM6:", DWORD dwBaudrate = 115200, 
							BYTE byByteSize = 8, BYTE byStopBits = 0, BYTE byParity = 0, DWORD dwSoftFlow = 0, DWORD dwHardFlow = 0);	
	BBSECURE DWORD BBSecure_ConnectionCheck(int *pConnectionMode, DWORD *pRepairCode = NULL);
	BBSECURE DWORD BBSecure_CheckErrorStatus(DWORD *pErrorCode);

	// 2014-03-28 erulil pin entry dlg GUI�� ������ �� �ִ� �Լ� �߰�
	BBSECURE DWORD BBSecure_SetEntryPINSceneGUI(LPCTSTR pFreeText, UINT nFreeTextSize,
												UINT nTextFontSize, LPCTSTR pTextFontType, int nTextFontTypeSize, COLORREF clrTextFont,
												UINT nPinFontSize, LPCTSTR pPinFontType, int nPinFontTypeSize, COLORREF clrPinFont,
												COLORREF clrBackground = RGB(0, 0, 0), int nDlgStartY = -1, int nDlgHeight = -1,
												int nTextStartPositionX = -1, int nTextStartPositionY = -1, int nLineSpacing = 10);

	BBSECURE DWORD BBSecure_GetSerialNumber(LPBYTE lpBuffer, int nBufferSize);


	// 2014-02-19 erulil lBankID�� �ش��ϴ� key���� ���� ����
	BBSECURE DWORD BBSecure_ClearKeys(DWORD dwBankID);
	// 2014-03-17 erulil Ư�� Ű �ϳ��� ����
	BBSECURE DWORD BBSecure_EraseKey(DWORD dwBankID, DWORD dwKeyType, DWORD dwKeyIndex);

	// 2014-03-17 erulil KSN ����
	BBSECURE DWORD BBSecure_GenerateKSN(BYTE *pKSN); // pKSN = 10B	
	// 2014-03-17 erulil Ư�� Ű KCV ��ȯ
	BBSECURE DWORD BBSecure_GetKeyCheckValue(BYTE *pKCV, DWORD dwBankID, DWORD dwKeyType, DWORD dwKeyIndex, DWORD dwKCVMode = 0x00); // pKCV = 3B


	// 2014-02-19 erulil send certificate file data. pFileData : buffer pointer, nFileDataLength : file data length,
	// pFileData�� ������ �����͸� ������ �ܸ��� ���� Ư�� ��� �ȿ� �����Ѵ�. �ܸ����� ��� �� ������ ����ڰ� �˱� ������ �Ѵ�.
	// ���� ��� : \\Windows\\PAC.DLL
	BBSECURE DWORD BBSecure_WriteCertificate(LPBYTE pCertificateData, UINT nCertificateDataLength);

	// 2014-02-19 erulil send certificate file data. pFileData : buffer pointer, pFileDataLength : [IN] pFileData buffer length, [OUT] write file data length
	// ������ ��ο��� ������ �����͸� �о�ͼ� ��ȯ�Ѵ�.
	// ���� ��� : \\Windows\\PAC.DLL
	BBSECURE DWORD BBSecure_ReadCertificate(LPBYTE pCertificateData, UINT *pCertificateDataLength);



	// key injection ----------------------------------------------------------------------------------------------- start
	// ----------------------------------------------- for verisoft start
	// 2014-04-09 erulil �߰� plain key data�� �޾Ƽ� key injection
	//						  nWriteMode : write_directly, write_encrypt with master key, write_decrypt with master key / pKCV : 3Bytes / nMode : ecb or cbc
	//						  nMasterKeyIndex : write_encrypt or write_decrypt �� ������ ��ȿ. master key�� injection �Ǿ� �־�� ��
	//						  nOverwriteMode : 0 - overwrite, 1 - XOR
	BBSECURE DWORD BBSecure_SendPaymentKey_simple(LPBYTE pKCV, LPBYTE pKeyData, UINT nKeyIndex, UINT nKeyType, UINT nKeyLengthOfByte, DWORD dwBankID, 
												UINT nMode, UINT nWriteMode, UINT nMasterKeyIndex, BOOL bXORWrite = FALSE, UINT nXORBaseKeyIndex = 0, LPBYTE pIV = NULL);
	// 2014-02-18 erulil Garanti key injection API �߰�. pEncryptedKey, pPattern, pMask�� �迭 ũ��� 256�̾�� �Ѵ�(�� 256B�� ����) 
	//													 = pEncryptedKey, pPattern, pMask�� ������ ���̴� nEncryptedKeyLength�� �����ؾ� �Ѵ�(=256)
	BBSECURE DWORD BBSecure_SendPaymentKey_Garanti(int nKeyType, DWORD dwBankID, UINT nKeyIndex, UINT nKeyLength, UINT nMode, 
												UINT nKeyLocation, UINT nKSNLocation, BOOL bUsingMaskAndPattern,
												LPBYTE pEncryptedKey, UINT nEncryptedKeyLength, LPBYTE pPattern, LPBYTE pMask,
												BYTE *pOutput, int nOutputSize);
	// 2014-04-14 erulil �߰�. �ܸ��⿡ �̹� injection �� �� RSA ���� Ű�� ������ Ű�� ��ȣȭ�Ѵ�
	BBSECURE DWORD BBSecure_GenerateKey_useSavedKey(LPBYTE pEncryptedKey, UINT *pKeyLength, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, 
													UINT nKeyLength, UINT nKeyType, DWORD dwBankID, UINT nMasterKeyIndex);
	BBSECURE DWORD BBSecure_GenerateKey_useSavedKeyforV(LPBYTE pEncryptedKey, UINT *pKeyLength, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, 
													UINT nKeyLength, UINT nKeyType, DWORD dwBankID, UINT nMasterKeyIndex, LPBYTE pClearKeyData = NULL);
	// 2014-04-14 erulil �߰�. RSA ����Ű�� �ܸ��⿡ �����Ѵ�. nKeyIndex : 0 ~ 7 
	BBSECURE DWORD BBSecure_SendRSAPublicKey(LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent, UINT nKeyIndex);
#if 1
	// 2014-06-09 erulil �߰�
	BBSECURE DWORD BBSecure_GenerateKey(LPBYTE pEncryptedKey, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, UINT nKeyLength, UINT nKeyType, DWORD dwBankID, LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent);
#else
	// 2014-04-07 erulil �߰�. export key func. [out] -> pEncryptedKey : buffer size must be 256B, pKSN : 10B, pKCV : 3B. �Է¹��� ����Ű�� ������ Ű�� ��ȣȭ�Ѵ�
	BBSECURE DWORD BBSecure_GenerateKey(LPBYTE pEncryptedKey, LPBYTE pKCV, LPBYTE pKSN, UINT nKeyIndex, UINT nKeyLength, UINT nKeyType, DWORD dwBankID, LPBYTE pModulus, UINT nByteLengthOfModulus, LPBYTE pExponent, UINT nByteLengthOfExponent);
#endif
	// ----------------------------------------------- for verisoft end


	// 2014-02-11 erulil �߰� Remote key injection�� API
	BBSECURE DWORD BBSecure_SendKeyEndPacket();
	BBSECURE DWORD BBSecure_SendPaymentKey_symmetric(BYTE* pKeyPacket, int nKeyPacketLength, BYTE *pOutput, int nOutputSize);
	BBSECURE DWORD BBSecure_SendPaymentKey_asymmetric(BYTE *pEncryptedKeyData, int nEncryptedKeyDataLength, 
													BYTE *pRandomNumber, int nRandomNumberLength, BYTE *pOutput, int nOutputSize);


	// 2013-4-15 erulil RSA key pair�� �����ؼ� key pair�� maxq�� ������, ����Ű�� ��ȯ�Ѵ�
	BBSECURE DWORD BBSecure_GenerateTerminalKey(LPBYTE pN, UINT *pByteLengthOfN, LPBYTE pE, UINT *pByteLengthOfE);
	BBSECURE DWORD BBSecure_SendTerminalKeyConfirm(); // ���۵� RSAŰ�� injection�϶�� ��Ŷ�� ������
	// key injection ----------------------------------------------------------------------------------------------- end



	// 2013-09-25 erulil �Լ� �߰�. lpCardNumber = 16 digit(=8B), lpBankID = 8 digit(=4B)
	BBSECURE DWORD BBSecure_GetPINBlock(LPCTSTR lpCardNumber, DWORD dwCardNumberLength, UINT nKeyType, UINT nKeyIndex, UINT nBlockFormat, 
										  //LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, DWORD dwAmount = 0);
										  LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, LPBYTE pKSN);//, LPCTSTR pAmountText = NULL, int nAmountTextSize = 0);
	// 2014-10-07 erulil �߰� : for SADAD
	BBSECURE DWORD BBSecure_GetPINBlock_withData(LPCTSTR lpPINData, DWORD dwPINLength, LPCTSTR lpCardNumber, DWORD dwCardNumberLength, UINT nKeyType, UINT nKeyIndex, UINT nBlockFormat, 
										  LPCTSTR lpBankID, UINT nBankIDLength, DWORD dwTimeOutSecond, BYTE *lpOutBuffer, DWORD *pOutLength, LPBYTE pKSN);
										  
	// mac encrypt
	BBSECURE DWORD BBSecure_GetMac(LPBYTE lpMacData, DWORD dwMaclen, UINT nKeyType, UINT nKeyIndex, 
									UINT nMode, LPCTSTR lpBankID, UINT nBankIDLength, BYTE *lpOutBuffer, DWORD *pOutputLen, LPBYTE pKSN);

	// 2013-10-08 erulil �Լ� �߰�. EMV�԰����� ��ȣȭ�� PIN ����� ��ȯ�Ѵ�
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
	// -> 2014-04-03 erulil �������� �ʴ� �Լ�
	//BBSECURE DWORD BBSecure_RSAOperation(UINT nOpType, UINT nKeyIndex, UINT nKeyID, 
	//								LPBYTE lpInputData, DWORD dwInputlen, LPBYTE lpOutBuffer, DWORD *pOutputlen);


	BBSECURE DWORD BBSecure_GetCurrentTime(PCI_TIME *pOutTime);
	BBSECURE DWORD BBSecure_SetCurrentTime(PCI_TIME inputTime);



	//////////////////////////////////////////////// ���� ��� �� API
	BBSECURE_INTERNAL DWORD BBSecure_WriteDataToMaxQ(BYTE *pSendBuff, DWORD nToWrite, int interval = 10, int cut_byte = 300);
	// 2014-03-28 erulil byAckCommand �߰�
	BBSECURE_INTERNAL DWORD BBSecure_SendEmptyPacket(BYTE byCommand, BYTE byAckCommand = 0);
	// erulil, SendStopPCI�� SendResumePCI�� ���߽� ���Ǵ� API�̴�. ������ ����Ǹ� �ȵ�. CommPCIApp���� Ż���ڵ� ���� MaxQ������ ����&�簳 ��Ű�� ���� ����
	BBSECURE_INTERNAL DWORD BBSecure_SendStopProcessor();
	BBSECURE_INTERNAL DWORD BBSecure_SendResumeProcessor();
	// 2014-02-26 erulil normal ��忡�� secure room mode ���� �����ϱ� ���� ���Ǵ� API. maxq���� connection�� �Ϸ�� ���Ŀ� ��� �Ѵ�
	BBSECURE_INTERNAL DWORD BBSecure_StartSecureRoomMode();
	BBSECURE_INTERNAL DWORD BBSecure_SendPassword(LPBYTE lpPassword1, LPBYTE lpPassword2, int nPasswordSize);
	// 2014-09-29 erulil �߰�
	BBSECURE_INTERNAL DWORD BBSecure_SendFastPassword(LPBYTE lpPassword1, LPBYTE lpPassword2, int nPasswordSize);
	//--------------------------------------------------------------------------------------------------------------------------------
	// maxq �� api ----------------------------------------------------------------------------------------------- end
	//--------------------------------------------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------------------------------------
	// no maxq ----------------------------------------------------------------------------------------------- start
	//----------------------------------------------------------------------------------------------------------------------------
	////// ���� ��� �� API
	BBSECURE_INTERNAL DWORD BBGet_Shuffle_Input(LPTSTR tszGetBuffer, int& nSize, int nMinSize, int nMaxSize, int nShowType);
	BOOL connection_check(); // 2014-07-14 erulil �߰�


	//2013-10-17 erulil errorCode�� �ؽ�Ʈ�� ��ȯ�ϴ� �Լ� �߰�
	BBSECURE BOOL BBGet_ErrorText(DWORD dwErrorCode, TCHAR* pTextBuffer, DWORD dwBufferSize);
	BBSECURE unsigned short BBGet_crc16(unsigned char *pData, unsigned long lByteLength);


	// 2014-04-03 erulil �߰�
	BBSECURE DWORD BBSecure_Rsa(BYTE *pOutputData, BYTE *pInputData, BYTE *pModulus, DWORD dwByteLengthOfModulus, BYTE *pExponent, DWORD dwaByteLengthOfExponent);
	BBSECURE DWORD BBSecure_GetRandomNumber(BYTE *pRandom, int nLength);
	BBSECURE DWORD BBSecure_Des(BYTE *pOutputData, BYTE *pInputData, DWORD dwByteLengthOfInputData, BYTE *pKeyData, DWORD dwByteLengthOfKeyData, 
								int nOperation, int nOperationMode, BYTE *pIV = NULL);
	//----------------------------------------------------------------------------------------------------------------------------
	// no maxq ----------------------------------------------------------------------------------------------- end
	//----------------------------------------------------------------------------------------------------------------------------




	//----------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------- 2013-12-19 erulil serial ����(PC) ���� API �߰� ----- start
	//----------------------------------------------------------------------------------------------------------------------------
	BBSECURE_USE_PC DWORD BBSecure_ConnectPC(HWND hApp, DWORD dwMsgValue, TCHAR* strPort = L"", DWORD dwBaudrate = 115200, 
									BYTE byByteSize = 8, BYTE byStopBits = 0, BYTE byParity = 0, DWORD dwSoftFlow = 0, DWORD dwHardFlow = 0);
	BBSECURE_USE_PC void BBSecure_ReleasePC();
	BBSECURE_USE_PC int BBGet_ModelType();
	BBSECURE_USE_PC DWORD BBSecure_WriteDataToPC(BYTE *pSendBuff, DWORD nToWrite);
	BBSECURE_USE_PC DWORD BBSecure_SendRSAPublicKeyToPC(DWORD dwErrorCode, LPBYTE pN, UINT nNSize, LPBYTE pE, UINT nESize);
	BBSECURE_USE_PC DWORD BBSecure_SendMaxQConnectionCheckAckToPC();

	// 2013-12-23 erulil remoteKeyInjection_PDA API ---------------------------------- start
	// ���� ���� ��Ŷ�� �����Ѵ�(start, ing, end)
	BBSECURE_USE_PC DWORD BBSecure_SendFileEndToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileStartToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileDataToPC(LPBYTE pFileData, int nDataSize);
	BBSECURE_USE_PC DWORD BBSecure_SendPublicKeyRequestToPC(LPBYTE pSN, int nSNSize);
	// �ܸ����� ������ �����ѹ��� �����Ѵ�
	BBSECURE_USE_PC DWORD BBSecure_SendRandomNumberAckToPC(LPBYTE lpByRandomNumber, UINT nRandomNumberSize);
	// ���������� ���������� �˸��� ��Ŷ�� �����Ѵ�(start, ing, end)
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvEndToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvStartToPC();
	BBSECURE_USE_PC DWORD BBSecure_SendFileRecvIngToPC();
	// 2013-12-23 erulil remoteKeyInjection_PDA API ---------------------------------- end
	//----------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------- 2013-12-19 erulil serial ����(PC) ���� API �߰� ----- end
	//----------------------------------------------------------------------------------------------------------------------------




	// test
	BBSECURE DWORD BBSecure_DoFlashTest(int nTestIndex); // nTestIndex : 0 ~ 6

}