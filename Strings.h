//! \file Strings.h Header for Strings.cpp
#pragma once
#include <windows.h>
#include "Utils.h"

//! \defgroup strings_functions Strings functions
//! \{
int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString);
int _CharsToString(TCHAR *pszString,int nStringLength,char *pszChars);
int _BinToHEX(char *pData,int nDataSize,TCHAR *pszHEX);
int _BinToHEXTable(char *pData,int nDataSize,TCHAR *pszHEX);
int _StringToUnicode(WCHAR *pszUnicode,int nUnicodeLength,TCHAR *pszString);
int _UnicodeToString(TCHAR *pszString,int nStringLength,WCHAR *pszUnicode);
int _CharsToUnicode(WCHAR *pszUnicode,int nUnicodeLength,char *pszChars);
int _UnicodeToChars(char *pszChars,int nCharsSize,WCHAR *pszUnicode);
int _UnicodeToNet(WCHAR *pszNet,int nNetLength,WCHAR *pszUnicode);
int _NetToUnicode(WCHAR *pszUnicode,int nUnicodeLength,WCHAR *pszNet);
//! \}