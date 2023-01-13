// HexPair.h

#pragma once

BOOL	HexPairToString(LPBYTE pData, UINT uDataSize, LPBYTE pString, UINT* puStringSize);
BOOL	StringToHexPair(LPBYTE pString, UINT uLength, LPBYTE pData, UINT* puSize);
BOOL	StringToHexPair(LPBYTE pString, UINT uLength);

BOOL	HexPairToWideString(LPBYTE pData, UINT uDataSize, WCHAR* pString, UINT* puStringSize);
BOOL	WideStringToHexPair(LPCWSTR pString, UINT uLength, LPBYTE pData, UINT* puSize);
