// PidionSystem.h : PidionSystem DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------
BOOL    GetPidionSerialNo(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt)

Synopsis:
    Pidion에 입력된 Serial number을 얻어온다.

input arguments:
    [out] pBuffer
        Serial number을 저장할 버퍼, NULL일 경우 FALSE을 반환
    [in] dwBufSize
        pBuffer의 배열 크기, 입력값이 실제 Serial number의 크기보다 작을 경우 입력된 값의 크기만큼만 serial number을 반환
    [out] pdwDataSize
        pBuffer을 통해서 반환되는 Serial number의 크기, NULL일 경우 FALSE을 반환
    [out] pdwRewriteCnt
        단말기에 Serial number을 덮어쓴 횟수 (Pidion 단말기에는 총 3회까지만 Serial number 입력이 가능하다.)

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionSerialNo(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt = NULL);

/*---------------------------------------------------------
BOOL    GetPidionBluetoothID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt)

Synopsis:
    Pidion에 입력된 Bluetooth ID을 얻어온다.

input arguments:
    [out] pBuffer
        Bluetooth ID을 저장할 버퍼, NULL일 경우 FALSE을 반환
    [in] dwBufSize
        pBuffer의 배열 크기, 입력값이 실제 Bluetooth ID보다 작을 경우 FALSE을 반환
    [out] pdwDataSize
        pBuffer을 통해서 반환되는 Bluetooth ID의 크기, NULL일 경우 FALSE을 반환
    [out] pdwRewriteCnt
        단말기에 Bluetooth ID을 덮어쓴 횟수 (Pidion 단말기에는 총 3회까지만 Bluetooth ID 입력이 가능하다.)

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionBluetoothID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt = NULL);

/*---------------------------------------------------------
BOOL    GetPidionPlatformID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion에 입력된 Platform ID을 얻어온다.

input arguments:
    [out] pBuffer
        Platform ID을 저장할 버퍼, NULL일 경우 FALSE을 반환
    [in] dwBufSize
        pBuffer의 배열 크기, 입력값이 실제 Platform ID보다 작을 경우 FALSE을 반환
    [out] pdwDataSize
        pBuffer을 통해서 반환되는 Platform ID의 크기, NULL일 경우 FALSE을 반환

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionPlatformID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

/*---------------------------------------------------------
BOOL    GetPidionManufactureID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion에 입력된 Manufacture ID을 얻어온다.

input arguments:
    [out] pBuffer
        Manufacture ID을 저장할 버퍼, NULL일 경우 FALSE을 반환
    [in] dwBufSize
        pBuffer의 배열 크기, 입력값이 실제 Manufacture ID보다 작을 경우 FALSE을 반환
    [out] pdwDataSize
        pBuffer을 통해서 반환되는 Manufacture ID의 크기, NULL일 경우 FALSE을 반환

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionManufactureID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

/*---------------------------------------------------------
BOOL    GetPidionVersion(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion에 입력된 Version을 얻어온다.

input arguments:
    [out] pBuffer
        Version을 저장할 버퍼, NULL일 경우 FALSE을 반환
    [in] dwBufSize
        pBuffer의 배열 크기, 입력값이 실제 Version보다 작을 경우 FALSE을 반환
    [out] pdwDataSize
        pBuffer을 통해서 반환되는 Version의 크기, NULL일 경우 FALSE을 반환

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionVersion(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

#ifdef __cplusplus
}
#endif