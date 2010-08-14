#pragma once
#include <windows.h>

void _ZeroMemory(void *pDest,int nSize);
void _CopyMemory(void *pDest,void *pSource,int nSize);
void *_Alloc(int nSize);
void _Free(void *pMemory);