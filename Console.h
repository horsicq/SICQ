//! \file Console.h Header for Console.cpp
#pragma once

#include "Utils.h"
#include "Strings.h"

//! \defgroup console_functions Console functions
//! \{
int _PrintText(TCHAR *pszString);
int _PrintHEX(char *pData,int nDataSize);
int _PrintNS();
int _PrintTextNS(TCHAR *pszString);
int _PrintHEXNS(char *pData,int nDataSize);
int _PrintHEXTable(char *pData,int nDataSize);
//! \}