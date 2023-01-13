#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>
#include <afxmt.h>

//#define _DEBUG // 2014-05-12 erulil test

#ifdef _DEBUG
#define RETAIL_SHOW		1
#else
#define RETAIL_SHOW		0
#endif

//#define RELEASE_PRINT

#ifdef _DEBUG
//#define SAVE_DEBUG_FILE
#endif

#define SAFE_DELETE(a)				{ if(a != NULL) { delete a; a = NULL; } }
#define SAFE_RELEASE(a)				{ if(a != NULL) { a.Release(); } }
#define SAFE_P_RELEASE(a)			{ if(a != NULL) { a->Release(); a = NULL; } }
#define SAFE_ARRAY_DELETE(a)		{ if(a != NULL) { delete []a; a = NULL; } }
#define SAFE_NEW(a, b)				{ if(a == NULL) { a = new b; } }
#define SAFE_ARRAY_NEW(a, b, c)		{ if(a == NULL) { a = new b[c]; memset(a, 0, sizeof(b) * c); } }
#define SAFE_CLOSE_HANDLE(a)		{ if(a != NULL) { CloseHandle(a); a = NULL; } }
#define SAFE_CLOSE_MSG_Q(a)			{ if(a != NULL) { CloseMsgQueue(a); a = NULL; } }
#define SAFE_SET_EVENT(a)			{ if(a != NULL) { SetEvent(a); } }



#define UPPER8BIT(x)       ((int)((x&0XFF00)>>8))
#define LOWER8BIT(x)       ((int)(x&0x00FF))
#define BITCOMPARE(x,y)    ((y&0xFF00)==0) ? (((int)x[0]==y) ? 1:0) : (((int)x[0]==UPPER8BIT(y))&&((int)x[1]==LOWER8BIT(y)) ? 1:0)


// 2014-03-27 erulil 추가
#define HEXA_TO_DECIMAL(h, d)		{ d = ((h / 16) * 10) + h % 10; } // 십진수 15 -> 0x15 변환
#define DECIMAL_TO_HEXA(d, h)		{ h = ((d / 10) * 16) + d % 10; } // 0x27 -> 십진수 27로 변환





static CStdioFile *g_pDebugFile = NULL;

bool BITCOMPARE2(unsigned char* byBYTE1, unsigned char* byBYTE2);

/////////////////////////////////////////////////////// common function START

// 2013-10-21 erulil 함수 추가. strInputData를 BYTE로 변환해서 pOutputBuffer에 저장한다.
// "1234"라면 0x12, 0x34로 변환. nStartIndex에서 nInputLength만큼 증가된 index를 반환한다.
int StringToBYTE(CString strInputData, int nByteLengthOfInputData, BYTE *pOutputBuffer, int nBufferSize, int nBufferStartIndex);
CString ByteToString(BYTE *pByte, int nSize);
CString ByteToString_noSpace(BYTE *pByte, int nSize);
// 2014-04-04 erulil 추가
BYTE IntToByte(int number);
int ByteToInt(BYTE byNumber);
DWORD StringToDWORD(CString strData);


// 2013-11-21 erulil, add debug print function
void print_debug_msg(CString msg);
void print_debug_msg(CString msg, BYTE *pByte, int nSize);
void print_debug_msg(BYTE *pByte, int nSize);
void print_release_msg(CString msg);
void print_release_msg(CString msg, BYTE *pByte, int nSize);
void print_release_msg(BYTE *pByte, int nSize);

void print_retail_msg(BOOL bShow, LPCTSTR lpszFmt, ...);
void CommonRelease();
void DeleteWinThread(CWinThread* pThread);

BOOL WriteTimeLog(CString strClassName);


//void MakeRandomNumber(BYTE *pBuffer, int nSize);

// 2014-09-02 erulil 추가
void LockCS(CCriticalSection *pCS);
void UnLockCS(CCriticalSection *pCS);