#include "stdafx.h"
#include "Strings.h"

//! \note Convert string(TCHAR[]) to chars.
//! \param pszChars [out] a pointer to a buffer that receives the converted string.
//! \param nCharsSize [in] a size, in bytes, of the buffer indicated by pszChars.
//! \param pszString [in] a pointer to the Unicode string to convert.
//! \return the number of bytes written to the buffer pointed to by pszChars if successful.
//! \return The function returns 0 if it does not succeed. 
int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString)
{
#ifdef  UNICODE
	return WideCharToMultiByte(CP_ACP,0,pszString,-1,pszChars,nCharsSize,NULL,NULL);
#else
	lstrcpyn(pszChars,pszString,nCharsSize);
	return lstrlen(pszChars);
#endif
}
//! \note Convert chars to string(TCHAR[]).
//! \param pszString [out] a pointer to a buffer that receives the converted string.
//! \param nStringLength [in] a length, in characters, of the buffer indicated by pszString.
//! \param pszChars [in] a pointer to chars to convert.
//! \return the number of characters written to the buffer pointed to by pszString if successful.
//! \return The function returns 0 if it does not succeed. 
int _CharsToString(TCHAR *pszString,int nStringLength,char *pszChars)
{
#ifdef  UNICODE
	return MultiByteToWideChar(CP_ACP,0,pszChars,-1,pszString,nStringLength);
#else
	lstrcpyn(pszString,pszChars,nStringLength);
	return lstrlen(pszString);
#endif
}
//! \note Convert binary data to HEX.
//! \param pData [in] a pointer to binary data.
//! \param nDataSize [in] a size, in bytes, of binary data.
//! \param pszHEX [out] a pointer to a buffer that receives the HEX string.
//! \return the number of characters written to the buffer pointed to by pszHEX if successful.
//! \return the function returns 0 if it does not succeed. 
int _BinToHEX(char *pData,int nDataSize,TCHAR *pszHEX)
{
	TCHAR szBuffer[16];
	*pszHEX=0;

	for(int i=0;i<nDataSize;i++)
	{
		wsprintf(szBuffer,TEXT("%02X"),*((unsigned char *)(pData+i)));
		lstrcat(pszHEX,szBuffer);
	}
	
	return lstrlen(pszHEX);
}
//! \note Convert binary data to HEX Table[8].
//! \param pData [in] a pointer to binary data.
//! \param nDataSize [in] a size, in bytes, of binary data.
//! \param pszHEX [out] a pointer to a buffer that receives the HEX string.
//! \return the number of characters written to the buffer pointed to by pszHEX if successful.
//! \return The function returns 0 if it does not succeed. 
int _BinToHEXTable(char *pData,int nDataSize,TCHAR *pszHEX)
{
	TCHAR szBuffer[256];
	*pszHEX=0;
	for(int i=0;i<nDataSize;i+=8)
	{
		wsprintf(szBuffer,TEXT("%04X|"),i);
		lstrcat(pszHEX,szBuffer);
		_BinToHEX(pData+i,_MinInt(8,nDataSize-i),szBuffer);
		lstrcat(pszHEX,szBuffer);
		lstrcat(pszHEX,_TEXT("\n"));
	}

	return lstrlen(pszHEX);
}