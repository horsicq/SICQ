#pragma once
//! A class represents a pseudo-random number generator
class random
{
private:
	int nInit;
public:
	random(void);
	~random(void);

	unsigned int randomDWORD();
	unsigned short randomWORD();
};

