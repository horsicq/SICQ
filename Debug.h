//! \file Debug.h Header for Debug.cpp
#pragma once

#include "Utils.h"
#include "Strings.h"

//! \defgroup debug_functions Debug functions
//! \{
int _PrintDebugText(TCHAR *pszString);
int _PrintDebugHEX(char *pData,int nDataSize);
int _PrintDebugNS();
int _PrintDebugTextNS(TCHAR *pszString);
int _PrintDebugHEXNS(char *pData,int nDataSize);
int _PrintDebugHEXTable(char *pData,int nDataSize);
//! \}