//! \file Utils.h Header for Utils.cpp

#pragma once
#include <windows.h>

#ifdef  _DEBUG
//##################################################
#include "Debug.h"
//##################################################
#endif

//! \defgroup advanced_functions Advanced functions
//! \{
void _ZeroMemory(void *pDest,int nSize);
void _CopyMemory(void *pDest,void *pSource,int nSize);
void *_Alloc(int nSize);
void _Free(void *pMemory);
int _MaxInt(int n1,int n2);
int _MinInt(int n1,int n2);
//! \}