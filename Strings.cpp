#include "stdafx.h"
#include "Strings.h"

//! Convert string(TCHAR[]) to chars.
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
//! Convert chars to string(TCHAR[]).
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