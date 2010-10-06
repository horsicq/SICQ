#pragma once
#include <windows.h>
#include "Utils.h"

//! \defgroup strings_functions Strings functions
//! \{
int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString);
int _CharsToString(TCHAR *pszString,int nStringLength,char *pszChars);
int _BinToHEX(char *pData,int nDataSize,TCHAR *pszHEX);
int _BinToHEXTable(char *pData,int nDataSize,TCHAR *pszHEX);
//! \}