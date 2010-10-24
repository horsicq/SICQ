#include "Debug.h"

//! Writes a character string to a debug screen buffer beginning at the current cursor location.
//! \param pszString [in] a pointer to a string.
//! \return the number of characters written to the debug
int _PrintDebugText(TCHAR *pszString)
{
	OutputDebugString(pszString);

	return strlen(pszString);
}
//! Writes a binary data to a debug screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the debug
int _PrintDebugHEX(char *pData,int nDataSize)
{
	TCHAR *pszString=(TCHAR *)_Alloc(2*(sizeof(TCHAR))*(nDataSize+1));

	_BinToHEX(pData,nDataSize,pszString);

	int nResult=_PrintDebugText(pszString);

	_Free(pszString);

	return nResult;
}
//! Writes a new string symbol("\n") to a debug screen buffer beginning at the current cursor location.
//! \return the number of characters written to the debug
int _PrintDebugNS()
{
	return _PrintDebugText(TEXT("\n"));
}
//! Writes a character string and a new string symbol("\n") to a debug screen buffer beginning at the current cursor location.
//! \param pszString [in] a pointer to a string.
//! \return the number of characters written to the debug
int _PrintDebugTextNS(TCHAR *pszString)
{
	int nResult=_PrintDebugText(pszString);
	nResult+=_PrintDebugNS();

	return nResult;
}
//! Writes a binary data and a new string symbol("\n") to a debug screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the debug
int _PrintDebugHEXNS(char *pData,int nDataSize)
{
	int nResult=_PrintDebugHEX(pData,nDataSize);
	nResult+=_PrintDebugNS();

	return nResult;
}
//! Writes a binary data as a table to a debug screen buffer beginning at the current cursor location.
//! \param pData [in] a pointer to a binary data.
//! \param nDataSize [in] a size of the binary data.
//! \return the number of characters written to the debug
int _PrintDebugHEXTable(char *pData,int nDataSize)
{
	TCHAR *pszString=(TCHAR *)_Alloc(3*(sizeof(TCHAR))*(nDataSize+1));

	_BinToHEXTable(pData,nDataSize,pszString);

	int nResult=_PrintDebugText(pszString);

	_Free(pszString);

	return nResult;
}