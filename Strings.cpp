#include "Strings.h"

//! \note Convert string(TCHAR[]) to chars.
//! \param pszChars [out] a pointer to a buffer that receives the converted string.
//! \param nCharsSize [in] a size, in bytes, of the buffer indicated by pszChars.
//! \param pszString [in] a pointer to the string to convert.
//! \return the number of bytes written to the buffer pointed to by pszChars if successful.
//! \return The function returns 0 if it does not succeed. 
int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString)
{
#ifdef  UNICODE
	return _UnicodeToChars(pszChars,nCharsSize,pszString);
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
	return _CharsToUnicode(pszString,nStringLength,pszChars);
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
		lstrcat(pszHEX,TEXT("\n"));
	}

	return lstrlen(pszHEX);
}
//! \note Convert string(TCHAR[]) to Unicode(WCHAR[]).
//! \param pszUnicode [out] a pointer to a buffer that receives the converted string.
//! \param nUnicodeLength [in] a length, in characters, of the buffer indicated by pszUnicode.
//! \param pszString [in] a pointer to string to convert.
//! \return the number of characters written to the buffer pointed to by pszUnicode if successful.
//! \return The function returns 0 if it does not succeed. 
int _StringToUnicode(WCHAR *pszUnicode,int nUnicodeLength,TCHAR *pszString)
{
#ifdef  UNICODE
	lstrcpyn(pszUnicode,pszString,nUnicodeLength);
	return lstrlen(pszString);
#else
	return _CharsToUnicode(pszUnicode,nUnicodeLength,pszString);
#endif
}
//! \note Convert Unicode(WCHAR[]) to string(TCHAR[])
//! \param pszString [out] a pointer to a buffer that receives the converted string.
//! \param nStringLength [in] a size, in characters, of the buffer indicated by pszString.
//! \param pszUnicode [in] a pointer to the Unicode string to convert.
//! \return the number of bytes written to the buffer pointed to by pszString if successful.
//! \return The function returns 0 if it does not succeed. 
int _UnicodeToString(TCHAR *pszString,int nStringLength,WCHAR *pszUnicode)
{
#ifdef  UNICODE
	lstrcpyn(pszString,pszUnicode,nStringLength);
	return lstrlen(pszUnicode);
#else
	return _UnicodeToChars(pszString,nStringLength,pszUnicode);
#endif
}
//! \note Convert chars to Unicode(WCHAR[]).
//! \param pszUnicode [out] a pointer to a buffer that receives the converted string.
//! \param nUnicodeLength [in] a length, in characters, of the buffer indicated by pszUnicode.
//! \param pszChars [in] a pointer to chars to convert.
//! \return the number of characters written to the buffer pointed to by pszUnicode if successful.
//! \return The function returns 0 if it does not succeed. 
int _CharsToUnicode(WCHAR *pszUnicode,int nUnicodeLength,char *pszChars)
{
	return MultiByteToWideChar(CP_ACP,0,pszChars,-1,pszUnicode,nUnicodeLength);
}
//! \note Convert Unicode(WCHAR[]) to chars
//! \param pszChars [out] a pointer to a buffer that receives the converted string.
//! \param nCharsSize [in] a size, in bytes, of the buffer indicated by pszChars.
//! \param pszUnicode [in] a pointer to the Unicode string to convert.
//! \return the number of bytes written to the buffer pointed to by pszChars if successful.
//! \return The function returns 0 if it does not succeed. 
int _UnicodeToChars(char *pszChars,int nCharsSize,WCHAR *pszUnicode)
{
	return WideCharToMultiByte(CP_ACP,0,pszUnicode,-1,pszChars,nCharsSize,NULL,NULL);
}
//! \note Convert Unicode(WCHAR[]) to Net(WCHAR[] big-endian)
//! \param pszNet [out] a pointer to a buffer that receives the converted string.
//! \param nNetLength [in] a size, in characters, of the buffer indicated by pszNet.
//! \param pszUnicode [in] a pointer to the Unicode string to convert.
//! \return the number of bytes written to the buffer pointed to by pszNet.
int _UnicodeToNet(WCHAR *pszNet,int nNetLength,WCHAR *pszUnicode)
{
	for(int i=0;i<nNetLength;i++)
	{
		pszNet[i]=htons(pszUnicode[i]);
	}

	return nNetLength;
}
//! \note Convert Net(WCHAR[] big-endian) to Unicode(WCHAR[]) 
//! \param pszUnicode [out] a pointer to a buffer that receives the converted string.
//! \param nUnicodeLength [in] a size, in characters, of the buffer indicated by pszUnicode.
//! \param pszNet [in] a pointer to the Net string to convert.
//! \return the number of bytes written to the buffer pointed to by pszUnicode.
int _NetToUnicode(WCHAR *pszUnicode,int nUnicodeLength,WCHAR *pszNet)
{
	for(int i=0;i<nUnicodeLength;i++)
	{
		pszUnicode[i]=ntohs(pszNet[i]);
	}

	return nUnicodeLength;
}