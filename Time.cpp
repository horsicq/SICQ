#include "Time.h"

//! _FileTimeToUnixTime
//! \param pFileTeme [in] a pointer to a buffer that contains FILETIME struct
//! \return Unix timestamp
//! \note Convert file time to Unix timestamp 
int _FileTimeToUnixTime(FILETIME *pFileTeme)
{
	return (int)(((*(__int64*) pFileTeme)-EPOCH_BIAS)/10000000i64);
}
//! _UnixTimeToFileTime
//! \param nUnixTime [in] Unix timestamp
//! \param pFileTeme [out] a pointer to a buffer that receives the converted Unix timestamp
//! \note Convert Unix timestamp to file time
void _UnixTimeToFileTime(int nUnixTime,FILETIME *pFileTeme)
{
	__int64 nTemp;

	nTemp=(__int64)nUnixTime*10000000i64+EPOCH_BIAS;

	*(__int64*) pFileTeme=nTemp;
}
//! _SystemTimeAsUnixTime
//! \return current system Unix timestamp
int _SystemTimeAsUnixTime()
{
	FILETIME ft;

	GetSystemTimeAsFileTime(&ft);

	return _FileTimeToUnixTime(&ft);
}
//! _LocalTimeAsUnixTime
//! \return current local Unix timestamp
int _LocalTimeAsUnixTime()
{
	FILETIME ft;
	SYSTEMTIME st;

	GetLocalTime(&st);

	SystemTimeToFileTime(&st,&ft);

	return _FileTimeToUnixTime(&ft);
}
//! _UnixTimeAsString
//! \param nUnixTime [in] Unix timestamp.
//! \param pszString [out] a pointer to a buffer that receives the converted string.
//! \param nStringLength [in] a length, in characters, of the buffer indicated by pszString.
//! \return a size of a string.
//! \note Convert Unix timestamp to string
int _UnixTimeAsString(int nUnixTime,PTSTR pszString,int nStringLength)
{
	TCHAR szBuffer[256];
	FILETIME ft;
	SYSTEMTIME st;
	int nResultLenght=0;

	_UnixTimeToFileTime(nUnixTime,&ft);

	FileTimeToSystemTime(&ft,&st);

	wsprintf(szBuffer,TEXT("%d:%d:%d %d.%d.%d"),st.wHour,st.wMinute,st.wSecond,st.wDay,st.wMonth,st.wYear);

	nResultLenght=lstrlen(szBuffer);
	
	if(nResultLenght<nStringLength)
	{
		lstrcpy(pszString,szBuffer);
		return nResultLenght;
	}
	else
	{
		return 0;
	}
}
