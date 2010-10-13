#include "StdAfx.h"
#include "random.h"

//! Constructor
random::random(void)
{
	unsigned int nTemp;

	__asm
	{
		rdtsc
			bswap	eax

			mov		nTemp,eax
	}

	nInit=nTemp;

	randomDWORD();
}
//! Destructor
random::~random(void)
{
}
//! \return a random number between 0 and 4294967295
unsigned int random::randomDWORD()
{
	nInit=((400014+40692)*nInit)%2147483563;
	return nInit;
}
//! \return a random number between 0 and 65534
unsigned short random::randomWORD()
{
	return randomDWORD()>>16;
}