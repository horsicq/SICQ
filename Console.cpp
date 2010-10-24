#include "Console.h"

//! Writes a character string to a console screen buffer beginning at the current cursor location.
//! \param pszString [in] a pointer to a string.
//! \return the number of characters written to the console
int _PrintText(TCHAR *pszString)
{
	int nTemp;

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),pszString,lstrlen(pszString),(LPDWORD)&nTemp,0);

	return nTemp;
}
//! Writes a binary data to a console screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the console
int _PrintHEX(char *pData,int nDataSize)
{
	TCHAR *pszString=(TCHAR *)_Alloc(2*(sizeof(TCHAR))*(nDataSize+1));

	_BinToHEX(pData,nDataSize,pszString);

	int nResult=_PrintText(pszString);

	_Free(pszString);

	return nResult;
}
//! Writes a new string symbol("\n") to a console screen buffer beginning at the current cursor location.
//! \return the number of characters written to the console
int _PrintNS()
{
	return _PrintText(TEXT("\n"));
}
//! Writes a character string and a new string symbol("\n") to a console screen buffer beginning at the current cursor location.
//! \param pszString [in] a pointer to a string.
//! \return the number of characters written to the console
int _PrintTextNS(TCHAR *pszString)
{
	int nResult=_PrintText(pszString);
	nResult+=_PrintNS();

	return nResult;
}
//! Writes a binary data and a new string symbol("\n") to a console screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the console
int _PrintHEXNS(char *pData,int nDataSize)
{
	int nResult=_PrintHEX(pData,nDataSize);
	nResult+=_PrintNS();

	return nResult;
}
//! Writes a binary data as a table to a console screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the console
int _PrintHEXTable(char *pData,int nDataSize)
{
	TCHAR *pszString=(TCHAR *)_Alloc(3*(sizeof(TCHAR))*(nDataSize+1));

	_BinToHEXTable(pData,nDataSize,pszString);

	int nResult=_PrintText(pszString);

	_Free(pszString);

	return nResult;
}