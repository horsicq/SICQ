#include "stdafx.h"
#include "Utils.h"

//! Fills a block of memory with zeros
//! \param pDest [in] a pointer to the starting address of the block of memory to fill with zeros
//! \param nSize [in] the size of the block of memory to fill with zeros, in bytes
void _ZeroMemory(void *pDest,int nSize)
{

	if(nSize)
	{
		__asm
		{
			xor		eax,eax
			mov		ecx,nSize
			mov		edi,pDest
			cld	
			rep		stosb
		}
	}
}
//! Copies a block of memory from one location to another
//! \param pDest [in] a pointer to the starting address of the copied block's destination
//! \param pSource [in] a pointer to the starting address of the block of memory to copy
//! \param nSize [in] the size of the block of memory to copy, in bytes
void _CopyMemory(void *pDest,void *pSource,int nSize)
{
	if(nSize)
	{
		__asm
		{
			mov		ecx,nSize
			mov		esi,pSource
			mov		edi,pDest
			cld	
			rep		movsb
		}
	}
}
//! Allocates the specified number of bytes from the heap
//! \param nSize [in] the number of bytes to allocate
//! \return a pointer to the memory
void *_Alloc(int nSize)
{
	return (void *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nSize);
}
//! Frees the specified global memory object and invalidates its handle
//! \param pMemory [in] a pointer to the memory
void _Free(void *pMemory)
{
	GlobalFree(pMemory);
}