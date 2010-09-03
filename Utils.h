#pragma once
#include <windows.h>

#ifdef  _DEBUG
//##################################################
#include "Console.h"
//##################################################
#endif

void _ZeroMemory(void *pDest,int nSize);
void _CopyMemory(void *pDest,void *pSource,int nSize);
void *_Alloc(int nSize);
void _Free(void *pMemory);
int _MaxInt(int n1,int n2);
int _MinInt(int n1,int n2);