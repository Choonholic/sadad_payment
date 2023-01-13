// PidionSystem.h : PidionSystem DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------
BOOL    GetPidionSerialNo(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt)

Synopsis:
    Pidion�� �Էµ� Serial number�� ���´�.

input arguments:
    [out] pBuffer
        Serial number�� ������ ����, NULL�� ��� FALSE�� ��ȯ
    [in] dwBufSize
        pBuffer�� �迭 ũ��, �Է°��� ���� Serial number�� ũ�⺸�� ���� ��� �Էµ� ���� ũ�⸸ŭ�� serial number�� ��ȯ
    [out] pdwDataSize
        pBuffer�� ���ؼ� ��ȯ�Ǵ� Serial number�� ũ��, NULL�� ��� FALSE�� ��ȯ
    [out] pdwRewriteCnt
        �ܸ��⿡ Serial number�� ��� Ƚ�� (Pidion �ܸ��⿡�� �� 3ȸ������ Serial number �Է��� �����ϴ�.)

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionSerialNo(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt = NULL);

/*---------------------------------------------------------
BOOL    GetPidionBluetoothID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt)

Synopsis:
    Pidion�� �Էµ� Bluetooth ID�� ���´�.

input arguments:
    [out] pBuffer
        Bluetooth ID�� ������ ����, NULL�� ��� FALSE�� ��ȯ
    [in] dwBufSize
        pBuffer�� �迭 ũ��, �Է°��� ���� Bluetooth ID���� ���� ��� FALSE�� ��ȯ
    [out] pdwDataSize
        pBuffer�� ���ؼ� ��ȯ�Ǵ� Bluetooth ID�� ũ��, NULL�� ��� FALSE�� ��ȯ
    [out] pdwRewriteCnt
        �ܸ��⿡ Bluetooth ID�� ��� Ƚ�� (Pidion �ܸ��⿡�� �� 3ȸ������ Bluetooth ID �Է��� �����ϴ�.)

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionBluetoothID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize, DWORD* pdwRewriteCnt = NULL);

/*---------------------------------------------------------
BOOL    GetPidionPlatformID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion�� �Էµ� Platform ID�� ���´�.

input arguments:
    [out] pBuffer
        Platform ID�� ������ ����, NULL�� ��� FALSE�� ��ȯ
    [in] dwBufSize
        pBuffer�� �迭 ũ��, �Է°��� ���� Platform ID���� ���� ��� FALSE�� ��ȯ
    [out] pdwDataSize
        pBuffer�� ���ؼ� ��ȯ�Ǵ� Platform ID�� ũ��, NULL�� ��� FALSE�� ��ȯ

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionPlatformID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

/*---------------------------------------------------------
BOOL    GetPidionManufactureID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion�� �Էµ� Manufacture ID�� ���´�.

input arguments:
    [out] pBuffer
        Manufacture ID�� ������ ����, NULL�� ��� FALSE�� ��ȯ
    [in] dwBufSize
        pBuffer�� �迭 ũ��, �Է°��� ���� Manufacture ID���� ���� ��� FALSE�� ��ȯ
    [out] pdwDataSize
        pBuffer�� ���ؼ� ��ȯ�Ǵ� Manufacture ID�� ũ��, NULL�� ��� FALSE�� ��ȯ

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionManufactureID(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

/*---------------------------------------------------------
BOOL    GetPidionVersion(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize)

Synopsis:
    Pidion�� �Էµ� Version�� ���´�.

input arguments:
    [out] pBuffer
        Version�� ������ ����, NULL�� ��� FALSE�� ��ȯ
    [in] dwBufSize
        pBuffer�� �迭 ũ��, �Է°��� ���� Version���� ���� ��� FALSE�� ��ȯ
    [out] pdwDataSize
        pBuffer�� ���ؼ� ��ȯ�Ǵ� Version�� ũ��, NULL�� ��� FALSE�� ��ȯ

return values:
    TRUE    if successful
	FALSE   if internal err 
---------------------------------------------------------*/
BOOL GetPidionVersion(WCHAR* pBuffer, const DWORD dwBufSize, DWORD* pdwDataSize);

#ifdef __cplusplus
}
#endif